/* GLoBES -- General LOng Baseline Experiment Simulator
 * (C) 2002 - 2007,  The GLoBES Team
 *
 * GLoBES is mainly intended for academic purposes. Proper
 * credit must be given if you use GLoBES or parts of it. Please
 * read the section 'Credit' in the README file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <obstack.h>
#include <globes/globes.h>


#include "glb_probability.h"
#include "glb_fluxes.h"
#include "glb_rate_engine.h"
#include "glb_minimize.h"
#include "glb_types.h"
#include "glb_multiex.h"
#include "glb_sys.h"
#include "glb_version.h"
#include "glb_error.h"
#include "glb_smear.h"
#include "glb_lexer.h"
#include "glb_parser_addons.h"
#include "glb_path.h"


#include "glb_wrapper.h"

/* The global variables */
int glb_num_of_exps;
glb_exp glb_experiment_list[GLB_MAX_EXP];
int glb_rule_number;

char **glb_param_names = NULL;


#define obstack_chunk_alloc glb_malloc
#define obstack_chunk_free glb_free

/* This  is a bunch of function in order to deal with the
 * glb_params_type type
 */

glb_osc_type *glb_alloc_osc_type()
{
  glb_osc_type *temp;
  int i;

  temp = (glb_osc_type *) glb_malloc(sizeof(glb_osc_type));
  if (temp != NULL)
  {
    temp->osc_params = (double *) glb_malloc(sizeof(double) * glbGetNumOfOscParams());
    if (temp->osc_params != NULL)
    {
      for (i=0; i < glbGetNumOfOscParams(); i++)
        temp->osc_params[i] = GLB_NAN;
    }
    temp->length = (size_t) glbGetNumOfOscParams();
  }
  return temp;
}

void glb_free_osc_type(glb_osc_type *stale)
{
  if(stale!=NULL) glb_free(stale->osc_params);
  glb_free(stale);
}

glb_density_type *glb_alloc_density_type()
{
  glb_density_type *temp;
  int i;

  temp = (glb_density_type *) glb_malloc(sizeof(glb_density_type));
  if (temp != NULL)
  {
    temp->density_params = (double *) glb_malloc(sizeof(double)*glb_num_of_exps);


    if (temp->density_params != NULL)
    {
      for(i=0; i < glb_num_of_exps; i++)
        temp->density_params[i] = GLB_NAN;
    }
    temp->length = (size_t) glb_num_of_exps;
  }
  return temp;
}

void glb_free_density_type(glb_density_type *stale)
{
  if(stale!=NULL) glb_free(stale->density_params);
  glb_free(stale);
}

glb_params glbAllocParams()
{
  glb_params temp;
  temp=(glb_params) glb_malloc(sizeof(struct glb_params_type));
  if (temp != NULL)
  {
    temp->osc=glb_alloc_osc_type();
    temp->density=glb_alloc_density_type();
    temp->iterations=0;
  }
  return temp;
}

void glbFreeParams(glb_params stale)
{
  if (stale != NULL)
  {
    glb_free_osc_type(((struct glb_params_type *) stale)->osc);
    glb_free_density_type(((struct glb_params_type *) stale)->density);
    glb_free(stale);
  }
  stale = NULL;
}

glb_params glbSetIteration(glb_params in, int iter)
{
  in->iterations=iter;
  return in;
}

int glbGetIteration(const glb_params in)
{
  return in->iterations;
}

glb_params glbSetDensityParams(glb_params in,
				 double dens, int which)
{
  int i;
  if(which==GLB_ALL)
    {
      for(i=0;i<(in->density)->length;i++) (in->density)->density_params[i]=dens;
    }
  else if(0 <= which&&which < (in->density)->length )
    {
      (in->density)->density_params[which]=dens;
    }
  else
    {
      glb_error("glbSetDensityParams: Density list length mismatch");
      return NULL;
    }
  return in;
}

double glbGetDensityParams(const glb_params in, int which)
{
  double out;
  if(0 <= which&&which < (in->density)->length )
    {
      out=(in->density)->density_params[which];
    }
  else
    {
      glb_error("glbGetDensityParams: Density list length mismatch");
      return 0;
    }
  return out;
}

glb_params glbSetOscParams(glb_params in,
				 double osc, int which)
{
  if (0 <= which&&which < glbGetNumOfOscParams())
    in->osc->osc_params[which] = osc;
  else
  {
    char msg[255];
    sprintf(msg, "glbSetOscParams: Invalid parameter index: %d", which);
    glb_error(msg);
    return NULL;
  }
  return in;
}

double glbGetOscParams(const glb_params in, int which)
{
  double out;
 if(0 <= which&&which < glbGetNumOfOscParams() )
    {
      out=(in->osc)->osc_params[which];
    }
  else
    {
      char msg[255];
      sprintf(msg, "glbGetOscParams: Invalid parameter index: %d", which);
      glb_error(msg);
      return 0;
    }
  return out;
}

glb_params
glbDefineParams(glb_params in,double theta12, double theta13, double theta23,
	      double delta, double dms, double dma)
{
  in=glbSetOscParams(in,theta12,0);
  in=glbSetOscParams(in,theta13,1);
  in=glbSetOscParams(in,theta23,2);
  in=glbSetOscParams(in,delta,3);
  in=glbSetOscParams(in,dms,4);
  in=glbSetOscParams(in,dma,5);
  return in;
}


glb_params
glbCopyParams(const glb_params source, glb_params dest)
{
  int i;

  if(dest==NULL||source==NULL) return NULL;
  if(source==dest) return dest;
  dest->iterations=source->iterations;

  /* BUG #13 -- The use of realloc should solve a memory leak and makes this
     function pretty featureful, since it supports copying parameters
     even if the number of parameters and/or experiments has changed.
  */

  if((source->osc)->length>0&&
     (source->osc)->length!=(dest->osc)->length)
    {
      (dest->osc)->osc_params=glb_realloc((dest->osc)->osc_params,(source->osc)->length * sizeof(double));
      (dest->osc)->length=(source->osc)->length;
    }

  if((source->density)->length>0&&
     (source->density)->length!=(dest->density)->length)
    {
      (dest->density)->density_params=
	glb_realloc((dest->density)->density_params,(source->density)->length * sizeof(double));
      (dest->density)->length=(source->density)->length;
    }

  for(i=0;i<(dest->osc)->length;i++)
    (dest->osc)->osc_params[i]=(source->osc)->osc_params[i];

  for(i=0;i<(dest->density)->length;i++)
    (dest->density)->density_params[i]=(source->density)->density_params[i];

  return dest;

}

void glbPrintParams(FILE *stream, const glb_params in)
{
  size_t k;
  for(k=0;k<(in->osc)->length;k++)
	fprintf(stream,"%g ",(in->osc)->osc_params[k]);
  fprintf(stream,"\n");
  for(k=0;k<(in->density)->length;k++)
	fprintf(stream,"%g ",(in->density)->density_params[k]);
  fprintf(stream,"\n");
  fprintf(stream,"Iterations: %d\n",in->iterations);
}


/***************************************************************************
 * Function glbClearParamNames                                             *
 ***************************************************************************
 * Clear all names assigned to oscillation parameters.                     *
 ***************************************************************************/
int glbClearParamNames()
{
  int i;
  if (glb_param_names)
  {
    for (i=0; i < glbGetNumOfOscParams(); i++)
      if (glb_param_names[i])
      {
        glb_free(glb_param_names[i]);
        glb_param_names[i] = NULL;
      }
    glb_free(glb_param_names);
    glb_param_names = NULL;
  }

  return GLB_SUCCESS;
}


/***************************************************************************
 * Function glbSetParamNames                                               *
 ***************************************************************************
 * Assign human-readable names to the oscillation parameters that can be   *
 * used in subsequent calls to glbSetOscParamByName etc. The array names   *
 * must have length glbGetNuOfOscParams(); some entries may be NULL, in    *
 * which case no name will be assigned to the corresponding parameter      *
 ***************************************************************************/
int glbSetParamNames(char **names)
{
  int i;

  if (!names)
  {
    glb_error("glbSetParamNames: Invalid parameter names (NULL)");
    return GLBERR_INVALID_ARGS;
  }

  if (!glb_param_names)
  {
    glb_param_names = glb_malloc(glbGetNumOfOscParams() * sizeof(glb_param_names[0]));
    memset(glb_param_names, 0, glbGetNumOfOscParams() * sizeof(glb_param_names[0]));
  }

  for (i=0; i < glbGetNumOfOscParams(); i++)
  {
    if (glb_param_names[i])
    {
      glb_free(glb_param_names[i]);
      glb_param_names[i] = NULL;
    }
    if (names[i])
      glb_param_names[i] = strdup(names[i]);
  }

  return GLB_SUCCESS;
}


/***************************************************************************
 * Function glbSetParamName                                                *
 ***************************************************************************
 * Similar to glbSetParamNames, but assigns a name to only one oscillation *
 * parameter, identified by the index i                                    *
 ***************************************************************************/
int glbSetParamName(const char *name, int i)
{
  if (!name)
  {
    glb_error("glbSetParamName: Invalid parameter name (NULL)");
    return GLBERR_INVALID_ARGS;
  }

  if (i < 0 || i >= glbGetNumOfOscParams())
  {
    char msg[255];
    sprintf(msg, "glbSetParamName: Invalid parameter index: %d", i);
    glb_error(msg);
    return GLBERR_INVALID_ARGS;
  }

  if (!glb_param_names)
  {
    glb_param_names = glb_malloc(glbGetNumOfOscParams() * sizeof(glb_param_names[0]));
    memset(glb_param_names, 0, glbGetNumOfOscParams() * sizeof(glb_param_names[0]));
  }

  if (glb_param_names[i])
  {
    glb_free(glb_param_names[i]);
    glb_param_names[i] = NULL;
  }
  glb_param_names[i] = strdup(name);

  return GLB_SUCCESS;
}


/***************************************************************************
 * Function glbGetParamName                                                *
 ***************************************************************************
 * Returns a string containing the name of the oscillation parameter       *
 * identified by its index i. The return value is NULL if that parameter   *
 * has not been assigned a name. It is the user's responsibility to free   *
 * the memory associated with the returned string.                         *
 ***************************************************************************/
char *glbGetParamName(int i)
{
  if (i < 0 || i >= glbGetNumOfOscParams())
  {
    char msg[255];
    sprintf(msg, "glbSetParamName: Invalid parameter index: %d", i);
    glb_error(msg);
    return NULL;
  }

  if (!glb_param_names || !glb_param_names[i])
    return NULL;

  return strdup(glb_param_names[i]);
}


/***************************************************************************
 * Function glbFindParamByName                                             *
 ***************************************************************************
 * Returns the index of the oscillation parameter that has been assigned   *
 * the given name; the return value is negative in case of an error        *
 ***************************************************************************/
int glbFindParamByName(const char *name)
{
  int i;

  if (!name)
  {
    glb_error("glbFindParamByName: Invalid parameter name (NULL)");
    return GLBERR_INVALID_ARGS;
  }

  if (glb_param_names)
  {
    for (i=0; i < glbGetNumOfOscParams(); i++)
      if (glb_param_names[i] && strcmp(glb_param_names[i], name) == 0)
        return i;
  }

  char msg[255];
  sprintf(msg, "glbFindParamByName: Name not found: %s", name);
  glb_error(msg);
  return GLBERR_NAME_NOT_FOUND;
}


/***************************************************************************
 * Function glbSetOscParamByName                                           *
 ***************************************************************************
 * Same as glbSetOscParams, but identifies the parameter by name. Return   *
 * value is GLB_SUCCESS or one of the (negative) GLBERR_XXX error codes    *
 ***************************************************************************/
int glbSetOscParamByName(glb_params in, double value, const char *name)
{
  int index;

  if (!name)
  {
    glb_error("glbSetOscParamByName: Invalid parameter name (NULL)");
    return GLBERR_INVALID_ARGS;
  }

  if ((index=glbFindParamByName(name)) < 0)
    return index;
  else
  {
    if (!glbSetOscParams(in, value, index))
      return GLBERR_GENERIC;
    else
      return GLB_SUCCESS;
  }
}


/***************************************************************************
 * Function glbGetOscParamByName                                           *
 ***************************************************************************
 * Same as glbGetOscParams, but identifies the parameter by name.          *
 * In case of an error, the return value is 0.0                            *
 ***************************************************************************/
double glbGetOscParamByName(const glb_params in, const char *name)
{
  int index;
  double value;

  if (!name)
  {
    glb_error("glbSetOscParamByName: Invalid parameter name (NULL)");
    return 0.0;
  }

  if ((index=glbFindParamByName(name)) < 0)
    return index;
  else
    return glbGetOscParams(in, index);
}



/* This  is a bunch of function in order to deal with the
 * glb_projection_type type
 */

glb_osc_proj_type *glb_alloc_osc_proj_type()
{
  int i;
  glb_osc_proj_type *temp;
  temp=(glb_osc_proj_type *) glb_malloc(sizeof(glb_osc_proj_type));
  temp->osc_params=(int *) glb_malloc(sizeof(int) *  glbGetNumOfOscParams());
  temp->length = (size_t) glbGetNumOfOscParams();
  for(i=0;i<glbGetNumOfOscParams();i++) temp->osc_params[i]=GLB_UNDEFINED;
  return temp;
}

void glb_free_osc_proj_type(glb_osc_proj_type *stale)
{
  if(stale!=NULL) glb_free(stale->osc_params);
  glb_free(stale);
}

glb_density_proj_type *glb_alloc_density_proj_type()
{
  int i;
  glb_density_proj_type *temp;
  temp=(glb_density_proj_type *) glb_malloc(sizeof(glb_density_proj_type));
  temp->density_params=(int *) glb_malloc(sizeof(int)*glb_num_of_exps);
  temp->length=(size_t) glb_num_of_exps;
  for(i=0;i<glb_num_of_exps;i++) temp->density_params[i]=GLB_UNDEFINED;
  return temp;
}

void glb_free_density_proj_type(glb_density_proj_type *stale)
{
  if(stale!=NULL) glb_free(stale->density_params);
  glb_free(stale);
}

glb_projection glbAllocProjection()
{
  glb_projection temp;
  temp=(glb_projection) glb_malloc(sizeof(struct glb_projection_type));
  temp->osc=glb_alloc_osc_proj_type();
  temp->density=glb_alloc_density_proj_type();
  return temp;
}

void glbFreeProjection(glb_projection stale)
{
  glb_free_osc_proj_type(stale->osc);
  glb_free_density_proj_type(stale->density);
  /* BUG #14 -- This line solves a serious leak in connection with the
   * prior-module.
   */
  glb_free(stale);
  stale = NULL;
}


glb_projection glbSetDensityProjectionFlag(glb_projection in,
				       int flag, int which)
{
  int i;
  if((flag!=GLB_FREE)&&(flag!=GLB_FIXED)) {
    glb_error("Projection flag must be either GLB_FREE or GLB_FIXED");
    return NULL;
  }

  if(which==GLB_ALL)
    {
      for(i=0;i<glb_num_of_exps;i++) (in->density)->density_params[i]=flag;
    }
  else if(0 <= which&&which < (in->density)->length )
    {
      (in->density)->density_params[which]=flag;
    }
  else
    {
      glb_error("glbSetDensityProjectionFlag: Invalid experiment index");
      return NULL;
    }
  return in;
}

int glbGetDensityProjectionFlag(const glb_projection in, int which)
{
  int out;
  if(0 <= which&&which < (in->density)->length )
    {
      out=(in->density)->density_params[which];
    }
  else
    {
      glb_error("glbGetDensityProjectionFlag: Invalid experiment index");
      return -1;
    }
  return out;
}

glb_projection glbSetProjectionFlag(glb_projection in,
				 int flag, int which)
{
  if((flag!=GLB_FREE)&&(flag!=GLB_FIXED)) {
    glb_error("Projection flag must be either GLB_FREE or GLB_FIXED");
    return NULL;
  }

   if(0 <= which&&which < glbGetNumOfOscParams() )
    {
      (in->osc)->osc_params[which]=flag;
    }
  else
    {
      glb_error("glbSetProjectionFlag: Invalid oscillation parameter");
      return NULL;
    }
  return in;
}

int glbGetProjectionFlag(const glb_projection in, int which)
{
  int out;
  if(0 <= which&&which < glbGetNumOfOscParams() )
    {
      out=(in->osc)->osc_params[which];
    }
  else
    {
      glb_error("glbGetProjectionFlag: Invalid oscillation parameter");
      return -1;
    }
  return out;
}


glb_projection
glbDefineProjection(glb_projection in,int theta12, int theta13, int theta23,
	      int delta, int dms, int dma)
{
  in=glbSetProjectionFlag(in,theta12,0);
  in=glbSetProjectionFlag(in,theta13,1);
  in=glbSetProjectionFlag(in,theta23,2);
  in=glbSetProjectionFlag(in,delta,3);
  in=glbSetProjectionFlag(in,dms,4);
  in=glbSetProjectionFlag(in,dma,5);
  return in;
}


glb_projection
glbCopyProjection(const glb_projection source, glb_projection dest)
{
  int i;

  if(dest==NULL||source==NULL) return NULL;
  if(dest==source) return (glb_projection) dest;

  /* BUG #13 -- The use of realloc should solve a memory leak and makes this
     function pretty featureful, since it supports copying parameters
     even if the number of parameters and/or experiments has changed.
  */

  if((source->osc)->length>0&&
     (source->osc)->length!=(dest->osc)->length)
    {
      (dest->osc)->osc_params=glb_realloc((dest->osc)->osc_params,(source->osc)->length * sizeof(int));
      (dest->osc)->length=(source->osc)->length;
    }

  if((source->density)->length>0&&
     (source->density)->length!=(dest->density)->length)
    {
      (dest->density)->density_params=
	glb_realloc((dest->density)->density_params,(source->density)->length * sizeof(int));
      (dest->density)->length=(source->density)->length;
    }

  for(i=0;i<(source->osc)->length;i++)
    (dest->osc)->osc_params[i]=(source->osc)->osc_params[i];

  for(i=0;i<(source->density)->length;i++)
    (dest->density)->density_params[i]=(source->density)->density_params[i];

  return dest;
}


void glbPrintProjection(FILE *stream, const glb_projection in)
{
  size_t k;
  for(k=0;k<(in->osc)->length;k++)
    {
      if((in->osc)->osc_params[k]==GLB_FREE)
	fprintf(stream,"free  ");
      if((in->osc)->osc_params[k]==GLB_FIXED)
	fprintf(stream,"fixed ");
      if(((in->osc)->osc_params[k]!=GLB_FIXED) &&
	 ((in->osc)->osc_params[k]!=GLB_FREE) )
	fprintf(stream,"undef ");
    }
  fprintf(stream,"\n");
  for(k=0;k<(in->density)->length;k++)
    {
      if((in->density)->density_params[k]==GLB_FREE)
	fprintf(stream,"free  ");
      if((in->density)->density_params[k]==GLB_FIXED)
	fprintf(stream,"fixed ");
      if(((in->density)->density_params[k]!=GLB_FIXED) &&
	 ((in->density)->density_params[k]!=GLB_FREE) )
	fprintf(stream,"undef ");
    }
  fprintf(stream,"\n");
}


/***************************************************************************
 * Function glbSetProjectionFlagByName                                     *
 ***************************************************************************
 * Same as glbSetProjectionFlag, but identifies the parameter by name.     *
 * Return value is GLB_SUCCESS or one of the (negative) GLBERR_XXX error   *
 * codes                                                                   *
 ***************************************************************************/
int glbSetProjectionFlagByName(glb_projection in, int flag, const char *name)
{
  int index;

  if((flag!=GLB_FREE)&&(flag!=GLB_FIXED))
  {
    glb_error("glbSetProjectionFlagByName: Projection flag must be GLB_FREE or GLB_FIXED");
    return GLBERR_INVALID_ARGS;
  }

  if (!name)
  {
    glb_error("glbSetProjectionFlagByName: Invalid parameter name (NULL)");
    return GLBERR_INVALID_ARGS;
  }

  if ((index=glbFindParamByName(name)) < 0)
    return index;
  else
  {
    if (!glbSetProjectionFlag(in, flag, index))
      return GLBERR_GENERIC;
    else
      return GLB_SUCCESS;
  }
}


/***************************************************************************
 * Function glbGetProjectionFlagByName                                     *
 ***************************************************************************
 * Same as glbGetProjectionFlag, but identifies the parameter by name.     *
 * In case of an error, the return value is negative                       *
 ***************************************************************************/
int glbGetProjectionFlagByName(const glb_projection in, const char *name)
{
  int index;
  
  if (!name)
  {
    glb_error("glbSetProjectionFlagByName: Invalid parameter name (NULL)");
    return GLBERR_INVALID_ARGS;
  }

  if ((index=glbFindParamByName(name)) < 0)
    return index;
  else
    return glbGetProjectionFlag(in, index);
}



/* Wrappers for the old functions dealing with setting the oscillation
 * parameters
 */


int
glbSetOscillationParameters(const glb_params in)
{
  if (in == NULL)
    return -1;
  else
    return glb_hook_set_oscillation_parameters(in, glb_probability_user_data);
}


int
glbGetOscillationParameters(glb_params in)
{
  if (in == NULL)
    return -1;
  else
    return glb_hook_get_oscillation_parameters(in, glb_probability_user_data);
}


/* Here we have some "memory managament" functions, ie. things
 * to be called at start-up or exit...
 */

static struct obstack glb_rate_stack;

void
glbClearExperimentList()
{
  int i;
  for(i=0;i<GLB_MAX_EXP;i++) glbFreeExp(glb_experiment_list[i]);
  for(i=0;i<GLB_MAX_EXP;i++) glb_experiment_list[i]=glbAllocExp();
  glb_num_of_exps=0;
  glbResetCounters();
  glb_init_minimizer();   /* Re-initialize minimizer */
}


/**********************************************
 * Low-level access to the event rate vectors *
 **********************************************/

/***************************************************************************
 * Function glbGetChannelRatePtr                                           *
 ***************************************************************************
 * Get at pointer to the "true" event rate vector for a specific channel.  *
 * pre_post = GLB_PRE/GLB_POST determines whether pre-smearing or post-    *
 * smearing rates are requested.                                           *
 ***************************************************************************/
double *glbGetChannelRatePtr(int exp, int channel, int pre_post)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetChannelRatePtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (channel < 0 || channel >= glb_experiment_list[exp]->numofchannels)
  {
    glb_error("glbGetChannelRatePtr: Parameter \"channel\" is invalid");
    return NULL;
  }

  switch (pre_post)
  {
    case GLB_PRE:
      return glb_experiment_list[exp]->chrb_0[channel];
    case GLB_POST:
      return glb_experiment_list[exp]->chra_0[channel];
    default:
      glb_error("glbGetChannelRatePtr: Parameter \"pre_post\" is invalid");
  }

  return NULL;
}


/***************************************************************************
 * Function glbGetRuleRatePtr                                              *
 ***************************************************************************
 * Get at pointer to the "true" event rate vector for a specific rule.     *
 ***************************************************************************/
double *glbGetRuleRatePtr(int exp, int rule)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetRuleRatePtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (rule < 0 || rule >= glb_experiment_list[exp]->numofrules)
  {
    glb_error("glbGetRuleRatePtr: Parameter \"rule\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->rates0[rule];
}


/***************************************************************************
 * Function glbGetSignalRatePtr                                            *
 ***************************************************************************
 * Get at pointer to the "true" signal event rate vector for a specific    *
 * rule.                                                                   *
 ***************************************************************************/
double *glbGetSignalRatePtr(int exp, int rule)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetSignalRatePtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (rule < 0 || rule >= glb_experiment_list[exp]->numofrules)
  {
    glb_error("glbGetSignalRatePtr: Parameter \"rule\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->SignalRates[rule];
}


/***************************************************************************
 * Function glbGetBGRatePtr                                                *
 ***************************************************************************
 * Get at pointer to the "true" background event rate vector for a         *
 * specific rule.                                                          *
 ***************************************************************************/
double *glbGetBGRatePtr(int exp, int rule)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetBGRatePtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (rule < 0 || rule >= glb_experiment_list[exp]->numofrules)
  {
    glb_error("glbGetBGRatePtr: Parameter \"rule\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->BackgroundRates[rule];
}


/***************************************************************************
 * Function glbGetChannelFitRatePtr                                        *
 ***************************************************************************
 * Get at pointer to the fitted event rate vector for a specific channel.  *
 * pre_post = GLB_PRE/GLB_POST determines whether pre-smearing or post-    *
 * smearing rates are requested.                                           *
 ***************************************************************************/
double *glbGetChannelFitRatePtr(int exp, int channel, int pre_post)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetChannelFitRatePtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (channel < 0 || channel >= glb_experiment_list[exp]->numofchannels)
  {
    glb_error("glbGetChannelFitRatePtr: Parameter \"channel\" is invalid");
    return NULL;
  }

  switch (pre_post)
  {
    case GLB_PRE:
      return glb_experiment_list[exp]->chrb_1[channel];
    case GLB_POST:
      return glb_experiment_list[exp]->chra_1[channel];
    default:
      glb_error("glbGetChannelFitRatePtr: Parameter \"pre_post\" is invalid");
  }

  return NULL;
}


/***************************************************************************
 * Function glbGetSignalFitRatePtr                                         *
 ***************************************************************************
 * Get at pointer to the fitted signal event rate vector for a specific    *
 * rule.                                                                   *
 ***************************************************************************/
double *glbGetSignalFitRatePtr(int exp, int rule)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetSignalFitRatePtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (rule < 0 || rule >= glb_experiment_list[exp]->numofrules)
  {
    glb_error("glbGetSignalFitRatePtr: Parameter \"rule\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->rates1[rule];
}


/***************************************************************************
 * Function glbGetBGFitRatePtr                                             *
 ***************************************************************************
 * Get at pointer to the fitted background event rate vector for a         *
 * specific rule.                                                          *
 ***************************************************************************/
double *glbGetBGFitRatePtr(int exp, int rule)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetBGFitRatePtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (rule < 0 || rule >= glb_experiment_list[exp]->numofrules)
  {
    glb_error("glbGetBGFitRatePtr: Parameter \"rule\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->rates1BG[rule];
}


/***************************************************************************
 * Function glbGetEfficiencyPtr                                            *
 ***************************************************************************
 * Get at pointer to the pre- or post-smearing efficiency vector, depending*
 * on the parameter pre_post (GLB_PRE or GLB_POST)                         *
 ***************************************************************************/
double *glbGetEfficiencyPtr(int exp, int channel, int pre_post)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetEfficiencyPtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (channel < 0 || channel >= glb_experiment_list[exp]->numofchannels)
  {
    glb_error("glbGetEfficiencyPtr: Parameter \"channel\" is invalid");
    return NULL;
  }

  switch (pre_post)
  {
    case GLB_PRE:
      return glb_experiment_list[exp]->user_pre_smearing_channel[channel];
    case GLB_POST:
      return glb_experiment_list[exp]->user_post_smearing_channel[channel];
    default:
      glb_error("glbGetEfficiencyPtr: Parameter \"pre_post\" is invalid");
  }

  return NULL;
}


/***************************************************************************
 * Function glbGetBackgroundPtr                                            *
 ***************************************************************************
 * Get at pointer to the pre- or post-smearing background vector, depending*
 * on the parameter pre_post (GLB_PRE or GLB_POST)                         *
 ***************************************************************************/
double *glbGetBackgroundPtr(int exp, int channel, int pre_post)
{
  if (exp < 0 || exp >= glb_num_of_exps)
  {
    glb_error("glbGetBackgroundPtr: Parameter \"exp\" is invalid");
    return NULL;
  }
  if (channel < 0 || channel >= glb_experiment_list[exp]->numofchannels)
  {
    glb_error("glbGetBackgroundPtr: Parameter \"channel\" is invalid");
    return NULL;
  }

  switch (pre_post)
  {
    case GLB_PRE:
      return glb_experiment_list[exp]->user_pre_smearing_background[channel];
    case GLB_POST:
      return glb_experiment_list[exp]->user_post_smearing_background[channel];
    default:
      glb_error("glbGetBackgroundPtr: Parameter \"pre_post\" is invalid");
  }

  return NULL;
}


/* Deprecated */
int
glbGetChannelRates(double **data, size_t *length,
		   int exp, int channel, int smearing)
{
  size_t l;
  int i;
  double *temp;
  if(exp>=glb_num_of_exps)
    {
      glb_error("Error in glbGetChannelRates: 4th argument is larger than"
		" glb_num_of_exps");
      *data=NULL;
      *length=0;
      return -1;
    }
  if(channel>= ((struct glb_experiment *) glb_experiment_list[exp])->numofchannels)
    {
      glb_error("Error in glbGetChannelRates: 5th argument is larger than"
		" numofchannels");
      *data=NULL;
      *length=0;
      return -1;
    }

  if(exp<0)
    {
      glb_error("Error in glbGetChannelRates: 4th argument must be larger than"
		" 0");
      *data=NULL;
      *length=0;
      return -1;
    }
 if(channel<0)
    {
      glb_error("Error in glbGetChannelRates: 5th argument must be larger than"
		" 0");
      *data=NULL;
      *length=0;
      return -1;
    }
 if((smearing!=GLB_PRE) && (smearing!=GLB_POST))
   {
     glb_error("Error in glbGetChannelRates: 3rd argument must be either"
	       " GLB_PRE or GLB_POST");
     *data=NULL;
     *length=0;
     return -1;
   }


  if(smearing==GLB_PRE) l=(size_t)
			  ((struct glb_experiment *) glb_experiment_list[exp])->simbins;
  else l=(size_t)
	 ((struct glb_experiment *) glb_experiment_list[exp])->numofbins;
  if(l<=0)
    {
      glb_error("Error in glbGetChannelRates: Could not determine rate vector"
		" length");
      *data=NULL;
      *length=0;
      return -1;
    }

  temp=(double *) obstack_alloc(&glb_rate_stack,(l+1) * sizeof(double));
  if(smearing==GLB_PRE)
    for(i=0;i<l;i++)
      temp[i]= ((struct glb_experiment *) glb_experiment_list[exp])->chrb_0[channel][i];
  else
    for(i=0;i<l;i++)
      temp[i]= ((struct glb_experiment *) glb_experiment_list[exp])->chra_0[channel][i];

  temp[l]=-1.0;

  *data=temp;
  *length=l;
  return 0;
}



int
glbGetUserData(double **data, size_t *length,
		   int exp, int channel, int smearing, int bgeff)
{
  size_t l;
  int i;
  double *temp;
  if(exp>=glb_num_of_exps)
    {
      glb_error("Error in glbGetUserData: 5th argument is larger than"
		" glb_num_of_exps");
      *data=NULL;
      *length=0;
      return -1;
    }
  if(channel>= ((struct glb_experiment *) glb_experiment_list[exp])->numofchannels)
    {
      glb_error("Error in glbGetUserData: 6th argument is larger than"
		" numofchannels");
      *data=NULL;
      *length=0;
      return -1;
    }

  if(exp<0)
    {
      glb_error("Error in glbGetUserData: 5th argument must be larger than"
		" 0");
      *data=NULL;
      *length=0;
      return -1;
    }
 if(channel<0)
    {
      glb_error("Error in glbGetUserData: 6th argument must be larger than"
		" 0");
      *data=NULL;
      *length=0;
      return -1;
    }
 if((smearing!=GLB_PRE) && (smearing!=GLB_POST))
   {
     glb_error("Error in glbGetUserData: 3rd argument must be either"
	       " GLB_PRE or GLB_POST");
     *data=NULL;
     *length=0;
     return -1;
   }
 if((bgeff!=GLB_EFF) && (bgeff!=GLB_BG))
   {
     glb_error("Error in glbGetUserData: 4th argument must be either"
	       " GLB_EFF or GLB_BG");
     *data=NULL;
     *length=0;
     return -1;
   }


  if(smearing==GLB_PRE) l=(size_t)
			  ((struct glb_experiment *) glb_experiment_list[exp])->simbins;
  else l=(size_t)
	 ((struct glb_experiment *) glb_experiment_list[exp])->numofbins;
  if(l<=0)
    {
      glb_error("Error in glbGetUserData: Could not determine rate vector"
		" length");
      *data=NULL;
      *length=0;
      return -1;
    }

  temp=(double *) obstack_alloc(&glb_rate_stack,(l+1) * sizeof(double));
  if(smearing==GLB_PRE)
    {
      if(bgeff==GLB_EFF)
	for(i=0;i<l;i++)
	  temp[i]=
	    ((struct glb_experiment *)
	     glb_experiment_list[exp])->user_pre_smearing_channel[channel][i];
      else
	for(i=0;i<l;i++)
	  temp[i]=
	    ((struct glb_experiment *)
	     glb_experiment_list[exp])->user_pre_smearing_background[channel][i];
    }
  else
    {
      if(bgeff==GLB_EFF)
	for(i=0;i<l;i++)
	  temp[i]= ((struct glb_experiment *)
		    glb_experiment_list[exp])->user_post_smearing_channel[channel][i];
      else
	for(i=0;i<l;i++)
	  temp[i]= ((struct glb_experiment *)
		    glb_experiment_list[exp])->user_post_smearing_background[channel][i];

    }
  temp[l]=-1.0;

  *data=temp;
  *length=l;
  return 0;
}


void glbResetRateStack()
{
  obstack_free(&glb_rate_stack,NULL);
  obstack_init(&glb_rate_stack);
}

/* Some bound checking */

static int exp_range(int exp)
{
  if((exp>=0)&&(exp<glb_num_of_exps)) return 0;
  char msg[255];
  sprintf(msg, "Experiment index %d out of range", exp);
  glb_error(msg);
  return -1;
}


static int channel_range(int exp, int channel)
{
  if(exp_range(exp)!=0) return -1;
  if((channel>=0)&&
     (channel<((struct glb_experiment *) glb_experiment_list[exp])->numofchannels)) return 0;
  char msg[255];
  sprintf(msg, "Channel index %d in experiment %d out of range", channel, exp);
  glb_error(msg);
  return -1;
}

static int signal_range(int signal)
{
  if((signal==GLB_SIG)||(signal==GLB_BG)) return 0;
  glb_error("Signal flag is neither GLB_SIG nor GLB_BG");
  return -1;
}



static int rule_range(int exp, int rule)
{
  if(exp_range(exp)!=0) return -1;
  if((rule>=0)&&
     (rule<((struct glb_experiment *) glb_experiment_list[exp])->numofrules)) return 0;
  char msg[255];
  sprintf(msg, "Rule index %d in experiment %d out of range", rule, exp);
  glb_error(msg);
  return -1;
}


static int pos_range(int exp, int rule, int pos, int signal)
{
  if(rule_range(exp,rule)!=0) return -1;
  if(signal_range(signal)!=0) return -1;
  if(signal==GLB_SIG)
    if((pos>=0)&&
       (pos<((struct glb_experiment *)
	    glb_experiment_list[exp])->lengthofrules[rule])) return 0;

  if(signal==GLB_BG)
    if((pos>=0)&&
       (pos<((struct glb_experiment *)
	     glb_experiment_list[exp])->lengthofbgrules[rule])) return 0;

  char msg[255];
  sprintf(msg, "Position index %d in experiment %d, rule %d out of range", pos, exp, rule);
  glb_error(msg);
  return -1;
}



/* Accessing channel and rule internals */

int glbGetNumberOfSamplingPoints(int exp)
{
  if (exp_range(exp) !=0)
    return -1;
  else
    return glb_experiment_list[exp]->simbins;
}

int glbGetNumberOfBins(int exp)
{
  if (exp_range(exp) !=0)
    return -1;
  else
    return glb_experiment_list[exp]->numofbins;
}

int glbGetNumberOfChannels(int exp)
{
  int i;
  if(exp_range(exp)!=0) return -1;
  i=((struct glb_experiment *) glb_experiment_list[exp])->numofchannels;
  return i;
}

int glbGetNumberOfRules(int exp)
{
  int i;
  if(exp_range(exp)!=0) return -1;
  i=((struct glb_experiment *) glb_experiment_list[exp])->numofrules;
  return i;
}

/***************************************************************************
 * Function glbGetBinSizeListPtr                                           *
 ***************************************************************************
 * Returns a pointer to the array of bin widths in a given experiment.     *
 ***************************************************************************/
double *glbGetBinSizeListPtr(int exp)
{
  if (exp_range(exp) != 0)
  {
    glb_error("glbGetBinSizeListPtr: Parameter \"exp\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->smear_data[0]->binsize;
}

/***************************************************************************
 * Function glbGetBinCentersListPtr                                        *
 ***************************************************************************
 * Returns a pointer to the array of bin central energies in a given       *
 * experiment.                                                             *
 ***************************************************************************/
double *glbGetBinCentersListPtr(int exp)
{
  if (exp_range(exp) != 0)
  {
    glb_error("glbGetBinCentersListPtr: Parameter \"exp\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->smear_data[0]->bincenter;
}

/***************************************************************************
 * Function glbGetSamplingStepsizeListPtr                                  *
 ***************************************************************************
 * Returns a pointer to the array of sampling bin widths in a given        *
 * experiment.                                                             *
 ***************************************************************************/
double *glbGetSamplingStepsizeListPtr(int exp)
{
  if (exp_range(exp) != 0)
  {
    glb_error("glbGetSamplingStepsizeListPtr: Parameter \"exp\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->smear_data[0]->simbinsize;
}

/***************************************************************************
 * Function glbGetSamplingPointsListPtr                                    *
 ***************************************************************************
 * Returns a pointer to the array of sampling points.                      *
 ***************************************************************************/
double *glbGetSamplingPointsListPtr(int exp)
{
  if (exp_range(exp) != 0)
  {
    glb_error("glbGetSamplingPointsListPtr: Parameter \"exp\" is invalid");
    return NULL;
  }

  return glb_experiment_list[exp]->smear_data[0]->simbincenter;
}

int glbGetLengthOfRule(int exp, int rule, int signal)
{
  int i=-1;
  if(signal_range(signal)!=0) return -1;
  if(rule_range(exp,rule)!=0) return -1;

  if(signal==GLB_SIG)
    i=((struct glb_experiment *) glb_experiment_list[exp])->lengthofrules[rule];

  if(signal==GLB_BG)
    i=((struct glb_experiment *) glb_experiment_list[exp])->lengthofbgrules[rule];

  return i;
}

int glbGetChannelInRule(int exp, int rule, int pos, int signal)
{
  int i=-1;
  if(pos_range( exp,  rule,  pos,  signal)!=0) return -1;

  if(signal==GLB_SIG)
    i=((struct glb_experiment *) glb_experiment_list[exp])->rulechannellist[rule][pos];

  if(signal==GLB_BG)
    i=((struct glb_experiment *) glb_experiment_list[exp])->bgrulechannellist[rule][pos];

  return i;
}


/***************************************************************************
 * Function glbGetCoefficientInRule                                        *
 ***************************************************************************
 * Returns the coefficient of term #pos in the given experiment and rule.  *
 * signal=GLB_SIG is for signal contribution to the rule, signal=GLB_BG is *
 * for the background contribution                                         *
 ***************************************************************************/
double glbGetCoefficientInRule(int exp, int rule, int pos, int signal)
{
  double res=-1.0;
  if(pos_range(exp,rule,pos,signal)!=0) return -1.0;

  if(signal==GLB_SIG)
    res=((struct glb_experiment *) glb_experiment_list[exp])->rulescoeff[rule][pos];

  if(signal==GLB_BG)
    res=((struct glb_experiment *) glb_experiment_list[exp])->bgrulescoeff[rule][pos];

  return res;
}


/***************************************************************************
 * Function glbSetCoefficientInRule                                        *
 ***************************************************************************
 * Sets the coefficient of term #pos in the given experiment and rule      *
 * to the value coeff. signal=GLB_SIG is for signal contribution to the    *
 * rule, signal=GLB_BG is for the background contribution                  *
 ***************************************************************************/
int glbSetCoefficientInRule(int exp, int rule, int pos, int signal, double coeff)
{
  if(pos_range(exp,rule,pos,signal)!=0) return -1.0;

  if(signal==GLB_SIG)
    ((struct glb_experiment *) glb_experiment_list[exp])->rulescoeff[rule][pos] = coeff;

  if(signal==GLB_BG)
    ((struct glb_experiment *) glb_experiment_list[exp])->bgrulescoeff[rule][pos] = coeff;

  return 0;
}

/***************************************************************************
 * Function glbGetChannelFlux                                              *
 ***************************************************************************
 * Returns the index of the neutrino flux definition used in the given     *
 * channel, or -1 in case of an error.                                     *
 ***************************************************************************/
int glbGetChannelFlux(int exp, int channel)
{
  if (channel_range(exp, channel) != 0)
    return -1;
  else
    return glb_experiment_list[exp]->listofchannels[0][channel];
}

//FIXME Write more functions to allow read and write access to all elements of
//channel definition

double glbGetNormalizationInRule(int exp, int rule, int signal)
{
  double res=-1.0;
  if(signal_range(signal)!=0) return -1.0;
  if(rule_range(exp,rule)!=0) return -1.0;

  if(signal==GLB_SIG)
    res=1.0;

  if(signal==GLB_BG)
    res=((struct glb_experiment *) glb_experiment_list[exp])->bg_centers[0][rule];

  return res;
}



static
int channel_bg(const double *ch, const double *bg, double **res, int flag)
{
  size_t lch,lbg;
  int i;
  double *temp;
  if((ch==NULL)||(bg==NULL))
    {
      glb_error("Rate or background vector may not be NULL");
      return -1;
    }
  /* FIXME That`s not very safe */
  lch=0;
  while(ch[lch]!=-1.0) lch++;
  lbg=0;
  while(bg[lbg]!=-1.0) lbg++;
  if(lbg!=lch)
    {
      glb_error("Length of rate or background vector may not be different");
      return -1;
    }
  if((flag!=GLB_W_BG)&&(flag!=GLB_WO_BG))
    {
      glb_error("Background flag is neither glb_W_BG nor GLB_WO_BG");
      return -1;
    }
  temp=(double *) obstack_alloc(&glb_rate_stack,sizeof(double) * (lch+1));

  if(flag==GLB_W_BG)
    for(i=0;i<lch;i++) temp[i]=ch[i];

  if(flag==GLB_WO_BG)
    for(i=0;i<lch;i++) temp[i]=ch[i]-bg[i];

  temp[lch]=-1.0;
  *res=temp;
  return 0;
}

static int
channel_eff(const double *ch, const double *bg, double **res, int flag)
{
  size_t lch,lbg;
  int i;
  double *temp;
  if((ch==NULL)||(bg==NULL))
    {
      glb_error("Rate or background vector may not be NULL");
      return -1;
    }
  /* FIXME That`s not very safe */
  lch=0;
  while(ch[lch]!=-1.0) lch++;
  lbg=0;
  while(bg[lbg]!=-1.0) lbg++;
  if(lbg!=lch)
    {
      glb_error("Length of rate or background vector may not be different");
      return -1;
    }
  if((flag!=GLB_W_EFF)&&(flag!=GLB_WO_EFF))
    {
      glb_error("Background flag is neither GLB_W_EFF nor GLB_WO_EFF");
      return -1;
    }
  temp=(double *) obstack_alloc(&glb_rate_stack,sizeof(double) * (lch+1));

  if(flag==GLB_W_EFF)
    for(i=0;i<lch;i++) temp[i]=ch[i];

  if(flag==GLB_WO_EFF)
    for(i=0;i<lch;i++) temp[i]=ch[i]/bg[i];
  temp[lch]=-1.0;
  *res=temp;
  return 0;
}


static int
rule_coeff(const double *ch, double coeff, double **res, int flag)
{
  size_t lch;
  int i;
  double *temp;
  if((ch==NULL))
    {
      glb_error("Rate or background vector may not be NULL");
      return -1;
    }
  /* FIXME That`s not very safe */
  lch=0;
  while(ch[lch]!=-1.0) lch++;

  if((flag!=GLB_W_COEFF)&&(flag!=GLB_WO_COEFF))
    {
      glb_error("Coefficient flag is neither glb_W_COEFF nor glb_WO_COEFF");
      return -1;
    }
  temp=(double *) obstack_alloc(&glb_rate_stack,sizeof(double) * (lch+1));

  if(flag==GLB_W_COEFF)
    for(i=0;i<lch;i++) temp[i]=ch[i]*coeff;

  if(flag==GLB_WO_COEFF)
    for(i=0;i<lch;i++) temp[i]=ch[i];
  temp[lch]=-1.0;
  *res=temp;
  return 0;
}



static glb_smear
*get_channel_smear_data(int exp, int channel)
{
  glb_smear *p;
  size_t w;
  if(channel_range(exp,channel)!=0) return NULL;
  w=(size_t) ((struct glb_experiment *) glb_experiment_list[exp])->listofchannels[5][channel];
  p=((struct glb_experiment *) glb_experiment_list[exp])->smear_data[w];
  return p;
}

void (*glb_channel_print_function)(FILE *stream,const double *energy,
				   double **res, size_t l, size_t c);

int
glbShowChannelProbs(FILE *stream,
			int exp, int channel, int smearing, int effi, int bgi)
{
  int i,s,cc,currentchannel[6],filter;
  size_t k,l,m,c;
  double *ch,*bg,*eff,*temp,**res,*energy,sum;
  glb_smear *smt;
  s=0;
  sum=0.0;
  cc=(size_t) glbGetNumberOfChannels(exp);

  filter = glbGetFilterStateInExperiment(exp);
  if(s!=0) return -1;
  if(channel!=GLB_ALL) smt=get_channel_smear_data(exp,channel);
  if(channel==GLB_ALL) smt=get_channel_smear_data(exp,0);
  s+=glbGetChannelRates(&ch,&l,exp,0,smearing);
  energy=(double *) obstack_alloc(&glb_rate_stack,sizeof(double)*(l+1));
  if(smearing==GLB_PRE)
      for(i=0;i<l;i++) energy[i]=glb_sbin_center(i,smt);
  if(smearing==GLB_POST)
    for(i=0;i<l;i++) energy[i]=glb_bin_center(i,smt);
  energy[l]=-1.0;
  if(channel!=GLB_ALL)
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*2);
      res[0]=(double *) obstack_alloc(&glb_rate_stack,sizeof(double)*(l+1));
      res[1]=NULL;
      c=1;

      for(i=0;i<l;i++)
	{
	  for(m=0;m<6;m++) currentchannel[m]=((struct glb_experiment *)
					      glb_experiment_list[exp])->listofchannels[m][channel];
	  if(currentchannel[2]>3||currentchannel[3]>3) res[k][i]=1.0;
	  else
	    res[0][i] = glbProfileProbability(exp,currentchannel[3],currentchannel[2],
                                              currentchannel[1],energy[i]);
	}

   }
  else
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*(cc+1));
      for(i=0;i<cc;i++) res[i]=(double *)    obstack_alloc(&glb_rate_stack,sizeof(double)*(l+1));
      res[cc]=NULL;
      c=cc;
      for(k=0;k<cc;k++)
	{
	  for(m=0;m<6;m++) currentchannel[m]=((struct glb_experiment *)
					      glb_experiment_list[exp])->listofchannels[m][k];
	  for(i=0;i<l;i++)
	    {

	      if(currentchannel[2]>3||currentchannel[3]>3) res[k][i]=1.0;
	      else
		res[k][i] = glbProfileProbability(exp,currentchannel[3],currentchannel[2],
                                                  currentchannel[1],energy[i]);
	    }
	}
    }



  glb_channel_print_function(stream,energy,res,l,c);

  obstack_free(&glb_rate_stack,ch);
  return 0;
}



int
glbShowChannelRates(FILE *stream,
			int exp, int channel, int smearing, int effi, int bgi)
{
  int i,s,cc;
  size_t k,l,m,c;
  double *ch,*bg,*eff,*temp,**res,*energy,sum;
  glb_smear *smt;
  s=0;
  sum=0.0;
  cc=(size_t) glbGetNumberOfChannels(exp);
  if(channel!=GLB_ALL)
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*2);
      s+=glbGetChannelRates(&ch,&l,exp,channel,smearing);
      s+=glbGetUserData(&bg,&k,exp,channel,smearing,GLB_BG);
      s+=glbGetUserData(&eff,&m,exp,channel,smearing,GLB_EFF);
      if(s!=0) return -1;
      if(!((l==k)&&(k==m))) return -1;

      s+=channel_bg(ch,bg,&temp,bgi);
      s+=channel_eff(temp,eff,&res[0],effi);
      res[1]=NULL;
      c=1;
    }
  else
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*(cc+1));
      for(i=0;i<cc;i++)
	{
	  s+=glbGetChannelRates(&ch,&l,exp,i,smearing);
	  s+=glbGetUserData(&bg,&k,exp,i,smearing,GLB_BG);
	  s+=glbGetUserData(&eff,&m,exp,i,smearing,GLB_EFF);
	  if(s!=0) return -1;
	  if(!((l==k)&&(k==m))) return -1;

	  s+=channel_bg(ch,bg,&temp,bgi);
	  s+=channel_eff(temp,eff,&res[i],effi);

	}
      res[cc]=NULL;
      c=cc;
    }

  if(s!=0) return -1;
  if(channel!=GLB_ALL) smt=get_channel_smear_data(exp,channel);
  if(channel==GLB_ALL) smt=get_channel_smear_data(exp,0);
  energy=(double *) obstack_alloc(&glb_rate_stack,sizeof(double)*(l+1));
  if(smearing==GLB_PRE)
      for(i=0;i<l;i++) energy[i]=glb_sbin_center(i,smt);
  if(smearing==GLB_POST)
    for(i=0;i<l;i++) energy[i]=glb_bin_center(i,smt);
  energy[l]=-1.0;

  glb_channel_print_function(stream,energy,res,l,c);

  obstack_free(&glb_rate_stack,ch);
  return 0;
}




int
glbShowRuleRates(FILE *stream,
		 int exp, int rule, int pos,
		 int effi, int bgi, int coeffi,
		 int signal)
{
  int i,s,cc,channel;
  size_t k,l,m,c;
  double *ch,*bg,*eff,*temp,*ceff,**res,*energy,sum,coeff;
  glb_smear *smt;
  s=0;
  sum=0.0;

  cc=(size_t) glbGetLengthOfRule(exp,rule,signal);
  if(pos!=GLB_ALL)
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*2);
      channel=glbGetChannelInRule(exp,rule,pos,signal);
      coeff=glbGetCoefficientInRule(exp,rule,pos,signal)*
	glbGetNormalizationInRule(exp,rule,signal);
      s+=glbGetChannelRates(&ch,&l,exp,channel,GLB_POST);
      s+=glbGetUserData(&bg,&k,exp,channel,GLB_POST,GLB_BG);
      s+=glbGetUserData(&eff,&m,exp,channel,GLB_POST,GLB_EFF);
      if(s!=0) return -1;
      if(!((l==k)&&(k==m))) return -1;
      s+=channel_bg(ch,bg,&temp,bgi);
      s+=channel_eff(temp,eff,&ceff,effi);
      s+=rule_coeff(ceff,coeff,&res[0],coeffi);
      res[1]=NULL;
      c=1;
    }
  else
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*(cc+1));
      for(i=0;i<cc;i++)
	{
	  channel=glbGetChannelInRule(exp,rule,i,signal);
	  coeff=glbGetCoefficientInRule(exp,rule,i,signal)*
	    glbGetNormalizationInRule(exp,rule,signal);

	  s+=glbGetChannelRates(&ch,&l,exp,channel,GLB_POST);
	  s+=glbGetUserData(&bg,&k,exp,channel,GLB_POST,GLB_BG);
	  s+=glbGetUserData(&eff,&m,exp,channel,GLB_POST,GLB_EFF);
	  if(s!=0) return -1;
	  if(!((l==k)&&(k==m))) return -1;
	  s+=channel_bg(ch,bg,&temp,bgi);
	  s+=channel_eff(temp,eff,&ceff,effi);
	  s+=rule_coeff(ceff,coeff,&res[i],coeffi);
	}
      res[cc]=NULL;
      c=cc;
    }


  if(s!=0) return -1;
  smt=get_channel_smear_data(exp,0);

  energy=(double *) obstack_alloc(&glb_rate_stack,sizeof(double)*(l+1));

  for(i=0;i<l;i++) energy[i]=glb_bin_center(i,smt);
  energy[l]=-1.0;

  glb_channel_print_function(stream,energy,res,l,c);

  obstack_free(&glb_rate_stack,ch);
  return 0;
}




double
glbTotalRuleRate(
		 int exp, int rule, int pos,
		 int effi, int bgi, int coeffi,
		 int signal)
{
  double out;
  int i,s,cc,channel;
  size_t k,l,m,c,bins;
  double *ch,*bg,*eff,*temp,*ceff,**res,*energy,sum,coeff;
  glb_smear *smt;
  s=0;
  sum=0.0;
  bins=glb_experiment_list[exp]->numofbins;
  cc=(size_t) glbGetLengthOfRule(exp,rule,signal);
  if(pos!=GLB_ALL)
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*2);
      channel=glbGetChannelInRule(exp,rule,pos,signal);
      coeff=glbGetCoefficientInRule(exp,rule,pos,signal)*
	glbGetNormalizationInRule(exp,rule,signal);
      s+=glbGetChannelRates(&ch,&l,exp,channel,GLB_POST);
      s+=glbGetUserData(&bg,&k,exp,channel,GLB_POST,GLB_BG);
      s+=glbGetUserData(&eff,&m,exp,channel,GLB_POST,GLB_EFF);
      if(s!=0) return -1;
      if(!((l==k)&&(k==m))) return -1;
      s+=channel_bg(ch,bg,&temp,bgi);
      s+=channel_eff(temp,eff,&ceff,effi);
      s+=rule_coeff(ceff,coeff,&res[0],coeffi);
      res[1]=NULL;
      c=1;
    }
  else
    {
      res=(double **) obstack_alloc(&glb_rate_stack,sizeof(double *)*(cc+1));
      for(i=0;i<cc;i++)
	{
	  channel=glbGetChannelInRule(exp,rule,i,signal);
	  coeff=glbGetCoefficientInRule(exp,rule,i,signal)*
	    glbGetNormalizationInRule(exp,rule,signal);

	  s+=glbGetChannelRates(&ch,&l,exp,channel,GLB_POST);
	  s+=glbGetUserData(&bg,&k,exp,channel,GLB_POST,GLB_BG);
	  s+=glbGetUserData(&eff,&m,exp,channel,GLB_POST,GLB_EFF);
	  if(s!=0) return -1;
	  if(!((l==k)&&(k==m))) return -1;
	  s+=channel_bg(ch,bg,&temp,bgi);
	  s+=channel_eff(temp,eff,&ceff,effi);
	  s+=rule_coeff(ceff,coeff,&res[i],coeffi);
	}
      res[cc]=NULL;
      c=cc;
    }


  if(s!=0) return -1;
  out=0;
  for(i=0;i<c;i++)
    for(k=0;res[i][k]!=-1;k++) out += res[i][k];

  obstack_free(&glb_rate_stack,ch);
  return out;
}


static char *printf_left=NULL;
static char *printf_middle=NULL;
static char *printf_right=NULL;

void glbSetPrintDelimiters(const char *left,const char *middle,
			   const char *right)
{


  glb_free((char *) printf_left);
  glb_free((char *) printf_right);
  glb_free((char *) printf_middle);

  printf_left=(char *) strdup(left);
  printf_middle=(char *) strdup(middle);
  printf_right=(char *) strdup(right);

}

void  glbPrintDelimiter(FILE *stream, int character)
{

  if(character=='l') fprintf(stream,"%s",printf_left);
  if(character=='m') fprintf(stream,"%s",printf_middle);
  if(character=='r') fprintf(stream,"%s",printf_right);
  return;

}


static void glb_builtin_channel_printf(FILE *stream,
				       const double *energy,
				       const double **res, size_t l,size_t c)
{
  int i,k;
  double *sum;
  sum=(double *) glb_malloc(sizeof(double)*c);
  for(k=0;k<c;k++) sum[k]=0.0;
  fprintf(stream,"\n");
  fprintf(stream,"%s",printf_left);
  for(i=0;i<l;i++)
    {
      fprintf(stream,"%s",printf_left);
      fprintf(stream,"%6.4g%s",energy[i],printf_middle);
	for(k=0;k<c;k++)
	  {
	    fprintf(stream,"%12.6g%s",res[k][i],printf_middle);
	    sum[k]+=res[k][i];
	  }
	fprintf(stream,"%s",printf_right);

    }

  fprintf(stream,"----------------------");
  for(k=1;k<c;k++)
    {
      fprintf(stream,"----------------");
    }
  fprintf(stream,"\nTotal:\t");

  for(k=0;k<c;k++)
    {
      fprintf(stream,"%12.6g%s",sum[k],printf_middle);
    }

  fprintf(stream,"%s",printf_right);
  glb_free(sum);
}

void
*glbSetChannelPrintFunction(void *fp)
{
  if(fp==NULL) return glb_channel_print_function;
  glb_channel_print_function=fp;
  return glb_channel_print_function;
}

void glb_clean_up()
{
  int i;

  for(i=0;i<GLB_MAX_EXP;i++) {glbFreeExp(glb_experiment_list[i]);}
 glbCleanSysList();
 glb_clean_parser();
 glb_lexer_cleanup();
 obstack_free(&glb_rate_stack,NULL);
 glb_free((char *) printf_left);
 glb_free((char *) printf_right);
 glb_free((char *) printf_middle);
 glb_free((char *) glb_prog_name);
 for(i=0;i<glb_path_vector_length;i++)
   glb_free((char *) glb_path_vector[i]);
 glb_free((char **) glb_path_vector);
 glb_free_minimizer();
 glb_free_probability_engine();
}



/* The all important init function */

void
glb_init(char *name)
{
  int i;


  glb_prog_name_init(name);
  glb_setup_path();

  for(i=0;i<GLB_MAX_EXP;i++) glb_experiment_list[i]=glbAllocExp();
  glb_num_of_exps=0;
  glb_rule_number=0;
  obstack_init(&glb_rate_stack);
  glbSetPrintDelimiters("","\t","\n");
  glbSetChannelPrintFunction(glb_builtin_channel_printf);
  /* this has gone to glbSuperInit
   * glbLoadPrior("prior-template");
   */
}




/* Here comes a bunch of set/get functions.
 * They all have the following ordering of their arguments:
 *    glbSetXXX(experiment, rule, flags, XXX)
 */


int
glbSetTargetMass(int experiment,double mass)
{
  struct glb_experiment *in;
  int i;
  if(experiment==GLB_ALL)
    {
      if(mass > 0)
	{
	  for(i=0;i<glb_num_of_exps;i++)
	    {
	      in=(struct glb_experiment *) glb_experiment_list[i];
	      in->targetmass = mass;
	    }
	}
      else
	{
	  glb_error("Target mass has to be positive");
	  return -1;
	}
      return 0;
    }

  if((experiment >= 0)&&(experiment < glb_num_of_exps))
    {
      in=(struct glb_experiment *) glb_experiment_list[experiment];
      if(mass > 0)
	{
	  in->targetmass = mass;
	}
      else
	{
	  glb_error("Target mass has to be positive");
	  return -1;
	}
    }
  else
    {
      glb_error("Invalid value for experiment number");
      return -1;
    }
  return 0;


}

double
glbGetTargetMass(int experiment)
{
  struct glb_experiment *in;
  double out;
  if((experiment >= 0)&&(experiment < glb_num_of_exps))
    {
      in=(struct glb_experiment *) glb_experiment_list[experiment];
      out=in->targetmass;
      return out;
    }
  else
    {
      glb_error("Invalid value for experiment number");
      return -1.0;
    }
  return -1.0;
}


/***************************************************************************
 * Function glbVersionOfExperiment                                         *
 ***************************************************************************
 * Return the version of the given experiment, as specified in the AEDL    *
 * file. Note: The return value is a pointer to the internal GLoBES        *
 * storage space for the version string; it's content should not be        *
 * modified by any user code.                                              *
 ***************************************************************************/
const char *glbVersionOfExperiment(int exp)
{
  struct glb_experiment *in;
  if (exp < 0  ||  exp >= glb_num_of_exps)
  {
    char msg[255];
    sprintf(msg, "glbVersionOfExperiment: Invalid experiment number: %d", exp);
    glb_error(msg);
    return NULL;
  }
  in = (struct glb_experiment *) glb_experiment_list[exp];
  return in->version;
}


/***************************************************************************
 * Function glbGetFilenameOfExperiment                                     *
 ***************************************************************************
 * Returns the name of the AEDL file from which the given experiment was   *
 * loaded.                                                                 *
 * Note: The return value is a pointer to the internal GLoBES              *
 * storage space for the filename string; it's content should not be       *
 * modified by any user code.                                              *
 ***************************************************************************/
const char *glbGetFilenameOfExperiment(int exp)
{
  struct glb_experiment *in;
  if (exp < 0  ||  exp >= glb_num_of_exps)
  {
    char msg[255];
    sprintf(msg, "glbGetFilenameOfExperiment: Invalid experiment number: %d", exp);
    glb_error(msg);
    return NULL;
  }
  in = (struct glb_experiment *) glb_experiment_list[exp];
  return in->filename;
}


/***************************************************************************
 * Function glbGetCitationForExperiment                                    *
 ***************************************************************************
 * Returns the citation information for the given experiment as specified  *
 * in the glb file                                                         *
 * Note: The return value is a pointer to the internal GLoBES              *
 * storage space for the filename string; it's content should not be       *
 * modified by any user code.                                              *
 ***************************************************************************/
const char *glbGetCitationForExperiment(int exp)
{
  struct glb_experiment *in;
  if (exp < 0  ||  exp >= glb_num_of_exps)
  {
    char msg[255];
    sprintf(msg, "glbGetCitationForExperiment: Invalid experiment number: %d", exp);
    glb_error(msg);
    return NULL;
  }
  in = (struct glb_experiment *) glb_experiment_list[exp];
  return in->citation;
}


/* Flux access routines */

double
glbFlux(int experiment, int flux_ident,
	double energy, double distance, int flavour, int anti)
{
  double out;
  struct glb_experiment *in;
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  in=(struct glb_experiment *) glb_experiment_list[experiment];
  if(!(((flux_ident >= 0)&&(flux_ident < in->num_of_fluxes)))) {
    glb_error("Invalid value for flux number");
    return -1;}




  out=glb_get_flux(energy, distance, flavour, anti, in->fluxes[flux_ident]);

  return out;
}

double glbXSection(int experiment, int xsec_ident,
	double energy, int flavour, int anti)
{
  double out;
  struct glb_experiment *in;
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  in=(struct glb_experiment *) glb_experiment_list[experiment];
  if(!(((xsec_ident >= 0)&&(xsec_ident < in->num_of_xsecs)))) {
    glb_error("Invalid value for X-section number");
    return -1;}

  out=glb_get_xsec(energy,flavour, anti,in->xsecs[xsec_ident]);

  return out;
}

int
glbSetSourcePower(int experiment, int flux_ident, double power)
{
  int i,k;
  struct glb_experiment *in;

  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps))
       ||(experiment==GLB_ALL))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  if(power<=0)  {glb_error("Power must be positive");return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];
      /* Testing theflux number */
      if(!(((flux_ident >= 0)&&(flux_ident < in->num_of_fluxes))
	   ||(flux_ident==GLB_ALL))) {
	glb_error("Invalid value for flux number");
	return -1;}
      for(k=0;k<in->num_of_fluxes;k++)
	{
	   if(flux_ident!=GLB_ALL) k=flux_ident;
	   /* Here should come the assignment */
	   (in->fluxes[k])->target_power=power;

	   if(flux_ident!=GLB_ALL) break;
	}

      if(experiment!=GLB_ALL) break;
    }


  return 0;
}

double
glbGetSourcePower(int experiment, int flux_ident)
{
  double out;
  struct glb_experiment *in;
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  in=(struct glb_experiment *) glb_experiment_list[experiment];
  if(!(((flux_ident >= 0)&&(flux_ident < in->num_of_fluxes)))) {
    glb_error("Invalid value for flux number");
    return -1;}

  out=(in->fluxes[flux_ident])->target_power;

  return out;
}

int
glbSetRunningTime(int experiment, int flux_ident, double time)
{

 int i,k;
  struct glb_experiment *in;

  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps))
       ||(experiment==GLB_ALL))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  if(time<=0)  {glb_error("Time must be positive");return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];
      /* Testing theflux number */
      if(!(((flux_ident >= 0)&&(flux_ident < in->num_of_fluxes))
	   ||(flux_ident==GLB_ALL))) {
	glb_error("Invalid value for flux number");
	return -1;}
      for(k=0;k<in->num_of_fluxes;k++)
	{
	   if(flux_ident!=GLB_ALL) k=flux_ident;
	   /* Here should come the assignment */
	   (in->fluxes[k])->time=time;

	   if(flux_ident!=GLB_ALL) break;
	}

      if(experiment!=GLB_ALL) break;
    }


  return 0;
}

double
glbGetRunningTime(int experiment, int flux_ident)
{
  double out;
  struct glb_experiment *in;
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  in=(struct glb_experiment *) glb_experiment_list[experiment];
  if(!(((flux_ident >= 0)&&(flux_ident < in->num_of_fluxes)))) {
    glb_error("Invalid value for flux number");
    return -1;}

  out=(in->fluxes[flux_ident])->time;

  return out;
}

/* Access to the filter feature */

int
glbSetFilterState(int on_off)
{
  glb_error("Function glbSetFilterState is deprecated.");
  return -1.0;
}

int
glbGetFilterState()
{
  glb_error("Function glbGetFilterState is deprecated.");
  return -1.0;
}


int
glbSetFilterStateInExperiment(int experiment,int on_off)
{
  int i;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps))
       ||(experiment==GLB_ALL))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  if((on_off!=GLB_ON)&&(on_off!=GLB_OFF))
    {glb_error("on_off must be either GLB_ON or GLB_OFF");return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];

      in->filter_state=on_off;

      if(experiment!=GLB_ALL) break;
    }
  return 0;
}

int
glbGetFilterStateInExperiment(int experiment)
{
  int i,out=-1;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];

      out=in->filter_state;

      if(experiment!=GLB_ALL) break;
    }
  return out;
}


int
glbSetFilter(double filter)
{
  glb_error("Function glbSetFilter is deprecated.");
  return GLBERR_DEPRECATED;
}

double
glbGetFilter()
{
  glb_error("Function glbGetFilter is deprecated.");
  return GLBERR_DEPRECATED;
}

int
glbSetFilterInExperiment(int experiment,double filter)
{
  int i;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps))
       ||(experiment==GLB_ALL))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  if(filter<0) {glb_error("Filter must be positive");return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];

      in->filter_value=filter;

      if(experiment!=GLB_ALL) break;
    }
  return 0;
}


double
glbGetFilterInExperiment(int experiment)
{
  int i;
  double out = -1;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];

      out=in->filter_value;

      if(experiment!=GLB_ALL) break;
    }
  return out;
}

int
glbOptimizeSmearingMatrixInExperiment(int experiment)
{
  int i;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps))
       ||(experiment==GLB_ALL))) {
    glb_error("Invalid value for experiment number");
    return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];

      for (int j=0; j < in->num_of_sm; j++)
        glb_optimize_smearing_matrix(in->smear_data[j], in->smear[j],
                                     in->lowrange[j], in->uprange[j]);

      if(experiment!=GLB_ALL) break;
    }
  return 0;
}


int
glbCompensateFilterInExperiment(int experiment)
{
  int i;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((experiment >= 0)&&(experiment < glb_num_of_exps))
       ||(experiment==GLB_ALL))) {
    glb_error("Invalid value for experiment number");
    return -1;}

  for(i=0;i<glb_num_of_exps;i++)
    {
      if(experiment!=GLB_ALL) i=experiment;
      in=(struct glb_experiment *) glb_experiment_list[i];

      for (int j=0; j < in->num_of_sm; j++)
        glb_filter_compensate(in->smear_data[j], in->smear[j],
                              in->lowrange[j], in->uprange[j]);

      if(experiment!=GLB_ALL) break;
    }
  return 0;
}


int
glbGetEminEmax(int experiment, double *emin, double *emax)
{
  struct glb_experiment *in;

  if (emin == NULL  ||  emax == NULL)
    { glb_error("glbGetEminEmax: Input pointers may not be NULL"); return -1; }

  if (experiment >= 0  &&  experiment < glb_num_of_exps)
  {
    in = glb_experiment_list[experiment];
    *emin = in->emin;
    *emax = in->emax;
  }
  else
    { glb_error("glbGetEminEmax: Invalid experiment number"); return -1; }
  return 0;
}


/***************************************************************************
 * Function glbGetEnergyWindow                                             *
 ***************************************************************************
 * Return lower and upper bounds of the analysis window in the given       *
 * experiment and rule.                                                    *
 ***************************************************************************/
int glbGetEnergyWindow(int experiment, int rule, double *low, double *high)
{
  struct glb_experiment *in;

  if (low == NULL  ||  high == NULL)
    { glb_error("glbGetEnergyWindow: Input pointers may not be NULL"); return -1; }

  if (experiment >= 0  &&  experiment < glb_num_of_exps)
  {
    in = glb_experiment_list[experiment];
    if (rule >= 0 && rule < in->numofrules)
    {
      *low  = in->energy_window[rule][0];
      *high = in->energy_window[rule][1];
    }
    else
      { glb_error("glbGetEnergyWindow: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetEnergyWindow: Invalid experiment number"); return -1; }
  return 0;
}


/***************************************************************************
 * Function glbGetEnergyWindowBins                                         *
 ***************************************************************************
 * Return lowest and highest bin overlapping with the analysis window in   *
 * the given experiment and rule.                                          *
 ***************************************************************************/
int glbGetEnergyWindowBins(int experiment, int rule, int *low, int *high)
{
  struct glb_experiment *in;

  if (low == NULL  ||  high == NULL)
    { glb_error("glbGetEnergyWindow: Input pointers may not be NULL"); return -1; }

  if (experiment >= 0  &&  experiment < glb_num_of_exps)
  {
    in = glb_experiment_list[experiment];
    if (rule >= 0 && rule < in->numofrules)
    {
      *low  = in->energy_window_bins[rule][0];
      *high = in->energy_window_bins[rule][1];
    }
    else
      { glb_error("glbGetEnergyWindow: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetEnergyWindow: Invalid experiment number"); return -1; }
  return 0;
}


/***************************************************************************
 * Function glbSetEnergyWindowInRule                                       *
 ***************************************************************************
 * This helper function is called by glbSetEnergyWindowInExperiment. Its   *
 * parameters are similar to those of glbSetEnergyWindow, but it does not  *
 * experiment=GLB_ALL or rule=GLB_ALL                                      *
 ***************************************************************************/
int glbSetEnergyWindowInRule(int experiment, int rule, double low, double high)
{
  int k;
  struct glb_experiment *in;

  if (experiment < 0  ||  experiment >= glb_num_of_exps)
  {
    glb_error("glbSetEnergyWindowInRule: Invalid experiment number");
    return -1;
  }
  in = (struct glb_experiment *) glb_experiment_list[experiment];
  if (rule < 0  ||  rule > in->numofrules)
  {
    glb_error("glbSetEnergyWindowInRule: Invalid rule number");
    return -1;
  }

  if (low > high)
  {
    glb_error("glbSetEnergyWindowInRule: Lower boundary above upper boundary");
    return -1;
  }

  /* Set new energy window */
  in->energy_window[rule][0] = low;
  in->energy_window[rule][1] = high;

  /* Compute bin ranges corresponding to energy window */
  k = 0;
  while (in->smear_data[0]->bincenter[k] <= in->energy_window[rule][0])
    k++;
  in->energy_window_bins[rule][0] = k;
  while (k < in->numofbins && in->smear_data[0]->bincenter[k] < in->energy_window[rule][1])
    k++;
  in->energy_window_bins[rule][1] = k-1;

  return 0;
}


/***************************************************************************
 * Function glbSetEnergyWindowInExperiment                                 *
 ***************************************************************************
 * This helper function is called by glbSetEnergyWindow. Its parameters    *
 * similar to those of glbSetEnergyWindow, but it does not accept          *
 * experiment=GLB_ALL                                                      *
 ***************************************************************************/
int glbSetEnergyWindowInExperiment(int experiment, int rule, double low, double high)
{
  int i;
  struct glb_experiment *in;

  if (experiment < 0  ||  experiment >= glb_num_of_exps)
  {
    glb_error("glbSetEnergyWindowInExperiment: Invalid experiment number");
    return -1;
  }

  in = (struct glb_experiment *) glb_experiment_list[experiment];
  if(rule == GLB_ALL)
  {
    for(i=0; i < in->numofrules; i++)
      glbSetEnergyWindowInRule(experiment, i, low, high);
  }
  else if (rule >= 0  &&  rule < in->numofrules)
    glbSetEnergyWindowInRule(experiment, rule, low, high);
  else
  {
    glb_error("glbSetEnergyWindowInExperiment: Invalid value for rule number");
    return -1;
  }
  return 0;
}

/***************************************************************************
 * Function glbSetEnergyWindow                                             *
 ***************************************************************************
 * Modify the energy window in the given experiment and rule (both can be  *
 * GLB_ALL).                                                               *
 ***************************************************************************/
int glbSetEnergyWindow(int experiment, int rule, double low, double high)
{
  int i, s=0;

  if (experiment == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
      s += glbSetEnergyWindowInExperiment(i, rule, low, high);
  }
  else if (experiment >= 0  &&  experiment < glb_num_of_exps)
  {
    s += glbSetEnergyWindowInExperiment(experiment, rule, low, high);
  }
  else
  {
    glb_error("glbSetEnergyWindow: Invalid value for experiment number");
    return -1;
  }

  return s;
}


/* Accessing parser meta-information */

int
glbNameToValue(int exp,const char* context, const char *name)
{
  glb_naming *ptr;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((exp >= 0)&&(exp < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}
  in=(struct glb_experiment *) glb_experiment_list[exp];
  for (ptr = in->names; ptr != (glb_naming *) NULL;
       ptr = (glb_naming *)ptr->next)
    if (strcmp (ptr->name,name) == 0 && strcmp(ptr->context,context)==0)
	return (ptr->value)-1;
  return -1;
}


const char
*glbValueToName(int exp,const char* context, int value)
{
  glb_naming *ptr;
  struct glb_experiment *in;
  /* Testing the experiment number */
  if(!(((exp >= 0)&&(exp < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return NULL;}
  in=(struct glb_experiment *) glb_experiment_list[exp];
  for (ptr = in->names; ptr != (glb_naming *) NULL;
       ptr = (glb_naming *)ptr->next)
    {
        if (ptr->value == value + 1  && strcmp(ptr->context,context)==0)
	  return ptr->name;
    }
  return NULL;
}


/*********************************************
 * Calculation of oscillation probabilities  *
 *********************************************/

/***************************************************************************
 * Function glbVacuumProbability                                           *
 ***************************************************************************
 * Returns the vacuum oscillation probability for one specific oscillation *
 * channel                                                                 *
 ***************************************************************************
 * Parameters:                                                             *
 *   initial_flavour/final_flavour: The desired oscillation channel        *
 *   cp_sign: +1 for neutrinos, -1 for antineutrinos                       *
 *   E: Neutrino energy in GeV                                             *
 *   L: Baseline in km                                                     *
 ***************************************************************************/
double glbVacuumProbability(int initial_flavour, int final_flavour,
			    int cp_sign, double E, double L)
{
  const double rho = 0.0;
  double P[3][3];
  int status;

  if ((status=glb_hook_probability_matrix(P, cp_sign, E, 1, &L, &rho, -1.0,
                                          glb_probability_user_data)) != GLB_SUCCESS)
  {
    glb_error("Calculation of oscillation probabilities failed.");
    return -1.0;
  }
  return P[initial_flavour-1][final_flavour-1];
}


/***************************************************************************
 * Function glbConstantDensityProbability                                  *
 ***************************************************************************
 * Returns the constant density oscillation probability for one specific   *
 * oscillation channel                                                     *
 ***************************************************************************
 * Parameters:                                                             *
 *   initial_flavour/final_flavour: The desired oscillation channel        *
 *   cp_sign: +1 for neutrinos, -1 for antineutrinos                       *
 *   E: Neutrino energy in GeV                                             *
 *   L: Baseline in km                                                     *
 *   rho: Matter density in g/cm^3                                         *
 ***************************************************************************/
double glbConstantDensityProbability(int initial_flavour, int final_flavour,
			    int cp_sign, double E, double L, double rho)
{
  double P[3][3];
  int status;

  if ((status=glb_hook_probability_matrix(P, cp_sign, E, 1, &L, &rho, -1.0,
                                          glb_probability_user_data)) != GLB_SUCCESS)
  {
    glb_error("Calculation of oscillation probabilities failed.");
    return -1.0;
  }
  return P[initial_flavour-1][final_flavour-1];
}


/***************************************************************************
 * Function glbProfileProbability                                          *
 ***************************************************************************
 * Returns the oscillation probability in matter for a specific channel    *
 * in a specific experiment. All information about the matter profile is   *
 * taken from the respective glb_experiment structure, but a possible      *
 * specification of the filter feature in the experiment is neglected      *
 ***************************************************************************
 * Parameters:                                                             *
 *   exp: Number of experiment                                             *
 *   initial_flavour/final_flavour: The desired oscillation channel        *
 *   cp_sign: +1 for neutrinos, -1 for antineutrinos                       *
 *   E: Neutrino energy in GeV                                             *
 ***************************************************************************/
double glbProfileProbability(int exp,int initial_flavour, int final_flavour,
			     int cp_sign, double E)
{
  struct glb_experiment *e = glb_experiment_list[exp];
  double P[3][3];
  int status;

  if (exp < 0  ||  exp >= glb_num_of_exps)
  {
    glb_error("Experiment index out of range");
    return -1.0;
  }

  glb_set_profile_scaling(1.0, exp);
  if ((status=glb_hook_probability_matrix(P, cp_sign, E, e->psteps, e->lengthtab,
                      e->densitybuffer, -1.0, glb_probability_user_data)) != GLB_SUCCESS)
  {
    glb_error("Calculation of oscillation probabilities failed.");
    return -1.0;
  }

  return P[initial_flavour-1][final_flavour-1];
}


/***************************************************************************
 * Function glbFilteredConstantDensityProbability                          *
 ***************************************************************************
 * Returns the oscillation probability in matter for a specific channel    *
 * in a specific experiment. All information about the matter profile      *
 * _and_ about the filter feature is taken from the respective             *
 * glb_experiment structure.                                               *
 * Therefore, if no filter is specified in the experiment, this function   *
 * is identical to glbProfileProbability.                                  *
 ***************************************************************************
 * Parameters:                                                             *
 *   exp: Number of experiment                                             *
 *   initial_flavour/final_flavour: The desired oscillation channel        *
 *   cp_sign: +1 for neutrinos, -1 for antineutrinos                       *
 *   E: Neutrino energy in GeV                                             *
 ***************************************************************************/
double glbFilteredConstantDensityProbability(int exp,int initial_flavour, int final_flavour,
			    int cp_sign, double E)
{
  struct glb_experiment *e = glb_experiment_list[exp];
  double P[3][3];
  int status;

  if (exp < 0  ||  exp >= glb_num_of_exps)
  {
    glb_error("Experiment index out of range");
    return -1.0;
  }

  glb_set_profile_scaling(1.0, exp);
  if ((status=glb_hook_probability_matrix(P, cp_sign, E, e->psteps, e->lengthtab, e->densitybuffer,
      (e->filter_state == GLB_ON) ? e->filter_value : -1.0, glb_probability_user_data)) != GLB_SUCCESS)
  {
    glb_error("Calculation of oscillation probabilities failed.");
    return -1.0;
  }

  return P[initial_flavour-1][final_flavour-1];
}


/* Number of fluxes in an experiment */

int glbGetNumberOfFluxes(int exp)
{
  int s;
  /* Testing the experiment number */
  if(!(((exp >= 0)&&(exp < glb_num_of_exps)))) {
    glb_error("Invalid value for experiment number");
    return -1;}

  s=glb_experiment_list[exp]->num_of_fluxes;
  return s;

}

