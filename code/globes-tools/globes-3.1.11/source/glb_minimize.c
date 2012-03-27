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


/* --------------------------------------------------------
 * --- Comment on numerical accuracy ----------------------
 * --------------------------------------------------------
 * The whole code uses double precision, whatever this is
 * on Your machine. The following information was derived
 * on 32-bit Pentium III processor.
 * Even though double precision should give relative errors of
 * order <1E-12, functions involving minimization like ChiTheta
 * may be off in the order of 1E-8 (absolute). This may cause
 * ChiTheta and SingleChiTheta to yield results differing by 1E-8.
 * The same applies for all other ChiglbXSection and SingleChiglbXSection functions.
 * The crucial lines are:
 * erg2 = erg2
 *    + glb_prior(x[1],start[2],inp_errs[2])
 *    + glb_prior(x[3],start[4],inp_errs[4])
 *    + glb_prior(x[4],start[5],inp_errs[5])
 *    + glb_prior(x[5],(glb_experiment_list[glb_single_experiment_number]).density_center,
 *      (glb_experiment_list[glb_single_experiment_number]).density_error);
 * in the chi_xxx functions. This looks of course different
 * in the MDchi_xxx functions. Basically its a matter of how
 * and in which order the truncations are performed. This may
 * also change under compiler optimization. In any case the errors
 * should be very small (i.e. <<1E-6) and should not have any impact
 * on the physics results.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include <globes/globes.h>

#include "glb_probability.h"
#include "glb_fluxes.h"
#include "glb_rate_engine.h"
#include "glb_min_sup.h"
#include "glb_minimize.h"
#include "glb_types.h"
#include "glb_multiex.h"
#include "glb_error.h"
#include "glb_wrapper.h"

/* global variabels */
int glb_single_experiment_number=GLB_ALL;
int glb_current_minimizer=GLB_MIN_DEFAULT;
int glb_single_rule_number=GLB_ALL; /* Used to pass rule number to glb_hybrid_chi_callback in 1-exp mode */
double glb_last_priors=0.0; /* Used to store prior terms between calls to glb_hybrid_chi_callback */


/* Warning-- fiddling around with these in general may
 * influence the stability of the minimization process !
 *
 * The latest Recator (D-CHOOZ as in hep-ph/0403068) setups
 * have TOLSYS 1.0E-12, whereas all the other setups have TOLSYS 1.0E-5
 * and TOLSYS 1.0E-8 may be faster by 20% for Nufact (improves convergence
 * of the second layer of minimization)
 */

#define TOLSYS 1.0E-8
#define TOLOSC 1.0E-5


/* defines wether the program will run
 * with (1) or w/o mathematica (0)
 */

#define GLB_MATHLINK 1

/* Defines wether the thing runs on UNIX (0) or not (1) */

#define UNIX 0

#define FLOAT double

//----------------------

/* this is for making the Chi... functions abortable */
static int okay_flag = 0;
/* this needed by the wrappers in order not to return garbage */
static jmp_buf env;
/* this is need by setjmp() longjmp() */
//--------------------------------------------------------


static glb_params input_errors = NULL;         /* Errors for prior terms */
static glb_params central_values = NULL;       /* Central values for prior terms */
static int count=0;

//This serves for ChiNP
static double *fix_params = NULL;
static int *para_tab = NULL;
static int *index_tab = NULL;
static int n_free;
static int n_fix;

//This serves for SingleChiNP
static double *s_fix_params = NULL;
static int *s_para_tab = NULL;
static int *s_index_tab = NULL;
static int s_n_free;
static int s_n_fix;

// the pointer to the userdefined prior function
double (*glb_user_defined_prior)(const glb_params, void *user_data);
int (*glb_user_defined_starting_values)(const glb_params, void *user_data);
int (*glb_user_defined_input_errors)(const glb_params, void *user_data);
void *glb_prior_user_data=NULL;

//-----------------------


//-------------------------------------------------------------
//------------------    MLAbort handling     ------------------
//-------------------------------------------------------------
// the whole stuff is being switched of if GLB_MATHLINK is set to 0.

// this function handels a caught abort message
// and jumps to the point where setjmp(env) has
// been called. usually from one of the ChiTheta(),
// MDChiTheta() or Chi() or SingleChi(). this
// function then will return a garbage value. this
// fact is comunicated to the wrappers with the
// okay_flag. if they encounter an okay_flag=1 they
// will send the Abort[] mathematica comand and then
// return.

static void ml_abort_handler()
{
  fprintf(stderr,"Mathlink programe catched abort!\n");
  longjmp(env,1);
}

// this function checks wether a abort message is sent by the
// math-kernel, if so it calls the ml_abort_handler().
// otherwise it calls MLCallYieldFunction() in order to give
// the kernel a possibility to send its message (this should
// be needed only in Windows and on MacOS)

#ifdef MLabort
static void ml_abort_check(int flag)
{

  if(flag==0) return;
  if(!MLAbort)
    {
      if(UNIX==1) MLCallYieldFunction(MLYieldFunction(stdlink),
				      stdlink,(MLYieldParameters)0);
    }
  if(MLAbort)
    {
      ml_abort_handler();
    }
}
#else
static void  ml_abort_check(int flag)
{
  /* To silence gcc -Wall */
  int i;
  i=flag;
  return;
}

#endif


/***************************************************************************
 * Function glb_init_minimizer                                             *
 ***************************************************************************
 * Allocate internal data structures for the minimizer. This function must *
 * be called whenever the number of oscillation parameters or the number   *
 * of experiments has been changed.                                        *
 ***************************************************************************/
int glb_init_minimizer()
{
  int i;

  glb_free_minimizer();

  /* Parameters of prior terms */
  input_errors   = glbAllocParams();
  central_values = glbAllocParams();

  /* Data structures for internal_glbChiNP */
  fix_params = (double *) glb_malloc((glbGetNumOfOscParams()+GLB_MAX_EXP) * sizeof(fix_params[0]));
  para_tab = (int *) glb_malloc((glbGetNumOfOscParams()+GLB_MAX_EXP) * sizeof(para_tab[0]));
  index_tab = (int *) glb_malloc((glbGetNumOfOscParams()+GLB_MAX_EXP) * sizeof(index_tab[0]));

  /* Data structures for internal_glbSingleChiNP */
  s_fix_params = (double *) glb_malloc((glbGetNumOfOscParams()+1) * sizeof(s_fix_params[0]));
  s_para_tab = (int *) glb_malloc((glbGetNumOfOscParams()+1) * sizeof(s_para_tab[0]));
  s_index_tab = (int *) glb_malloc((glbGetNumOfOscParams()+1) * sizeof(s_index_tab[0]));

  /* Select default projection */
  for (i=0; i < glbGetNumOfOscParams()+GLB_MAX_EXP; i++)
    para_tab[i] = GLB_UNDEFINED;
  for (i=0; i < glbGetNumOfOscParams()+1; i++)
    s_para_tab[i] = GLB_UNDEFINED;
  return 0;
}


/***************************************************************************
 * Function glb_free_minimizer                                             *
 ***************************************************************************
 * Frees the internal data structures of the minimizer                     *
 ***************************************************************************/
int glb_free_minimizer()
{
  if (input_errors != NULL)   { glbFreeParams(input_errors);                 }
  if (central_values != NULL) { glbFreeParams(central_values);               }
  if (fix_params != NULL)     { glb_free(fix_params);   fix_params = NULL;   }
  if (para_tab != NULL)       { glb_free(para_tab);     para_tab = NULL;     }
  if (index_tab != NULL)      { glb_free(index_tab);    index_tab = NULL;    }
  if (s_fix_params != NULL)   { glb_free(s_fix_params); s_fix_params = NULL; }
  if (s_para_tab != NULL)     { glb_free(s_para_tab);   s_para_tab = NULL;   }
  if (s_index_tab != NULL)    { glb_free(s_index_tab);  s_index_tab = NULL;  }

  return 0;
}


static void SelectProjection(int *vec, int check_proj)
{
  int status=0;
  int i,c,c1,c2;

  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++)
    {
      if (check_proj  &&  vec[i] != GLB_FREE  &&  vec[i] != GLB_FIXED)
        {
          status = -1;
          para_tab[i] = GLB_FREE;
        }
      else
        para_tab[i] = vec[i];
    }

  if (status != 0)
    glb_error("SelectProjection: Projection partly undefined. Using default GLB_FREE");

  c=0;
  c1=0;
  c2=0;
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++)
    {
      if(para_tab[i]==GLB_FREE) c++;
    }
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++)
    {
      if(para_tab[i]==GLB_FREE)
	{
	  index_tab[c1]=i;
	  c1++;
	}
      else if(para_tab[i]==GLB_FIXED)
	{
	  index_tab[c+c2]=i;
	  c2++;
	}
    }
  n_free=c;
  n_fix=c2;
  return;
}


static int CheckFree()
{
  int k;
  k=n_free;
  return k;
}

static int* CheckProjection()
{
  return &para_tab[0];
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//------------------------ Systematics -------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


/* Callback function for the systematics minimizer;        */
/* Calls the actual (possibly user-defined) chi^2 function */
/* with the appropriate parameters                         */
// JK - FIXME - This should be solved more elegantly
static glb_chi_function glb_current_chi_function;
static void *glb_current_chi_user_data;
static int glb_current_n_sys;
static double *glb_current_errorlist;
double glb_chi_callback(double *params)
{
  return glb_current_chi_function(glb_current_exp, glb_rule_number,
             glb_current_n_sys, &(params[1]), glb_current_errorlist,
             glb_current_chi_user_data);
}




// this an init function -
// setting up the input matrix for minimization
// ie. the set of starting directions
static void init_mat(double **m, int dim)
{
  int i;
  int j;
  for (i=1 ;i<= dim;i++)
    {
      for (j=1;j<= dim;j++)
	{
	  if (i==j)
	    {
	      m[i][j]=1.0;
	    }
	  else
	    {
	      m[i][j]=0.0;
	    }
	}
    }
}


// this is the same as ChiSO()  but it allows access to a single rule !

static double ChiS0_Rule(int rule)
{
  double **mat; // contains the direction set
  double *sp;   // stores the coordinates of the minimum
  double res;   // stores the minimum value
  int it=0;     // counts the number of iterations
  glb_systematic *sys;
  struct glb_experiment *exp = glb_experiment_list[glb_current_exp];
  int i;

  res = 0.0;
  glb_rule_number = rule;
  if (exp->sys_on_off[rule] == GLB_ON)
    sys = exp->sys_on[rule];
  else
    sys = exp->sys_off[rule];

  mat  = glb_alloc_mat(1, sys->dim, 1, sys->dim);
  sp   = glb_alloc_vec(1, sys->dim);
  init_mat(mat, sys->dim);

  /* Prepare data for callback function */
  glb_current_chi_function  = sys->chi_func;
  glb_current_n_sys         = sys->dim;
  glb_current_chi_user_data = sys->user_data;
  if (exp->sys_on_off[rule] == GLB_ON)
  {
    glb_current_errorlist = exp->sys_on_errors[rule];
    for (i=0; i < sys->dim; i++)
      sp[i+1] = exp->sys_on_startvals[rule][i];
  }
  else
  {
    glb_current_errorlist = exp->sys_off_errors[rule];
    for (i=0; i < sys->dim; i++)
      sp[i+1] = exp->sys_off_startvals[rule][i];
  }
  if (glb_powell(sp, mat, sys->dim, TOLSYS, &it, &res, &glb_chi_callback) != 0)
  {
    glb_warning("Systematics minimization failed.");
    return -res;
  }
  glb_free_vec(sp, 1, sys->dim);
  glb_free_mat(mat, 1, sys->dim, 1, sys->dim);

  glb_rule_number = 0;
  return res;
}

static double ChiS0()
{
  int i;
  double res = 0.0;

//  ml_abort_check(GLB_MATHLINK);    /* RELICT??? WW */

  for (i=0; i < glb_num_of_rules; i++)
    res += ChiS0_Rule(i);

  return res;
}

//---------------------------------------------------------------
//---------- MES begins here ------------------------------------
//---------------------------------------------------------------

// redfinition of ChiS

static double ChiS()
{
  double erg;
  int i;
  erg=0;
  for (i=0;i<glb_num_of_exps;i++)
    {
      glbSetExperiment(glb_experiment_list[i]);
      erg += ChiS0();
    }
  return erg;
}

//redefinition of Chi

static double Chi(double x[])
{
  int i;
  double erg;

  glb_params p = glbAllocParams();
  for (i=0; i < glbGetNumOfOscParams(); i++)
    p->osc->osc_params[i] = x[i];
  glb_hook_set_oscillation_parameters(p, glb_probability_user_data);
  glbFreeParams(p);

  for (i=0;i<glb_num_of_exps;i++)
    {
      glbSetExperiment(glb_experiment_list[i]);
      glb_set_profile_scaling(x[glbGetNumOfOscParams()+i],i);
      glb_set_new_rates(GLB_SLOW_RATES);
    }
  if (setjmp(env)==1)
    {
      okay_flag=1;
      return erg;
    }
  erg=ChiS();
  return erg;
}




// chi^2 with systematics for each Experiment

static double SingleChi(double x[glbGetNumOfOscParams()+1],int exp)
{
  int i;
  double erg;

  glb_params p = glbAllocParams();
  for (i=0; i < glbGetNumOfOscParams(); i++)
    p->osc->osc_params[i] = x[i];
  glb_hook_set_oscillation_parameters(p, glb_probability_user_data);
  glbFreeParams(p);

  glbSetExperiment(glb_experiment_list[exp]);
  glb_set_profile_scaling(x[glbGetNumOfOscParams()],exp);
  glb_set_new_rates(GLB_SLOW_RATES);

  glbSetExperiment(glb_experiment_list[exp]);
  if (setjmp(env)==1)
    {
      okay_flag=1;
      return erg;
    }
  erg=ChiS0();
  return erg;
}

// chi^2 with systematics for each Experiment

static double SingleRuleChi(double x[glbGetNumOfOscParams()+1],int exp, int rule)
{
  int i;
  double erg;

  glb_params p = glbAllocParams();
  for (i=0; i < glbGetNumOfOscParams(); i++)
    p->osc->osc_params[i] = x[i];
  glb_hook_set_oscillation_parameters(p, glb_probability_user_data);
  glbFreeParams(p);

  glbSetExperiment(glb_experiment_list[exp]);
  glb_set_profile_scaling(x[glbGetNumOfOscParams()],exp);
  glb_set_new_rates(GLB_SLOW_RATES);

  glbSetExperiment(glb_experiment_list[exp]);
  erg=ChiS0_Rule(rule);
  return erg;
}

/* Wrappers for the API */

double glbChiSys(const glb_params in, int experiment, int rule)
{
  int i;
  double res,x[GLB_MAX_EXP+glbGetNumOfOscParams()];
  int old_proj[glbGetNumOfOscParams()+glb_num_of_exps];

  if(in==NULL)
    {
      glb_error("Failure in glbChiSys: Input pointer must be non-NULL");
      return -1;
    }

  for(i=0; i < glbGetNumOfOscParams(); i++)
    {
      x[i] = glbGetOscParams(in,i);
      if (GLB_ISNAN(x[i]))
        {
          glb_error("glbChiSys: Oscillation parameters incompletely defined");
          return GLB_NAN;
        }
    }

  switch (glb_current_minimizer)
  {
    case GLB_MIN_POWELL:
      for(i=0; i < glb_num_of_exps; i++)
        {
          x[i+glbGetNumOfOscParams()] = glbGetDensityParams(in,i);
          if ((experiment==GLB_ALL || i==experiment)  &&  GLB_ISNAN(x[i+glbGetNumOfOscParams()]))
            {
              glb_warning("glbChiSys: Density parameters incompletely defined. Using default 1.0");
              x[i+glbGetNumOfOscParams()] = 1.0;
            }
        }

      /* Define projection which keeps all oscillation and density parameters fixed */
      memcpy(old_proj, CheckProjection(), sizeof(*old_proj)*(glbGetNumOfOscParams()+glb_num_of_exps));
      glb_projection new_proj=glbAllocProjection();
      for (i=0; i < glbGetNumOfOscParams(); i++)
        glbSetProjectionFlag(new_proj,GLB_FIXED,i);
      glbSetDensityProjectionFlag(new_proj,GLB_FIXED,GLB_ALL);
      glbSetProjection(new_proj);

      /* Invoke minimizer */
      glb_invoke_hybrid_minimizer(experiment, rule, x, &res);

      SelectProjection(old_proj, 0);   /* Don't check old_proj - it might still be GLB_UNDEFINED */
      glbFreeProjection(new_proj);
      break;

    case GLB_MIN_NESTED_POWELL:
      if(experiment==GLB_ALL)
        {
          if(rule==GLB_ALL)
            {
              for(i=0; i < glb_num_of_exps; i++)
                {
                  x[i+glbGetNumOfOscParams()] = glbGetDensityParams(in,i);
                  if (GLB_ISNAN(x[i+glbGetNumOfOscParams()]))
                    {
                      glb_warning("glbChiSys: Density parameters incompletely defined. Using default 1.0");
                      x[i+glbGetNumOfOscParams()] = 1.0;
                    }
                }
              res=Chi(x);
            }
          else
            {
              res=0;
              for(i=0;i<glb_num_of_exps;i++)
                {
                  if(rule < glb_experiment_list[i]->numofrules)
                    {
                      x[glbGetNumOfOscParams()] = glbGetDensityParams(in,i);
                      if (GLB_ISNAN(x[glbGetNumOfOscParams()]))
                        {
                          glb_warning("glbChiSys: Density parameters incompletely defined. Using default 1.0");
                          x[glbGetNumOfOscParams()] = 1.0;
                         }
                      res += SingleRuleChi(x,i,rule);
                    }
                  else
                    {glb_error("Invalid rule number");return -1;}
                }
            }
        }
      else
        {
          if(experiment >= glb_num_of_exps)
            {
              glb_error("Failure in glbChiSys: 2nd argument must be smaller than"
                        "glb_num_of_exps");
              return -1;
            }
          x[glbGetNumOfOscParams()] = glbGetDensityParams(in,experiment);
          if (GLB_ISNAN(x[glbGetNumOfOscParams()]))
            {
              glb_warning("glbChiSys: Density parameters incompletely defined. Using default 1.0");
              x[glbGetNumOfOscParams()] = 1.0;
            }
          if(rule==GLB_ALL) res=SingleChi(x,experiment);
            else
              {
                if(rule >= glb_experiment_list[experiment]->numofrules)
                  {
                    glb_error("Failure in glbChiSys: 3rd argument must be"
                              " smaller than numofrules");
                    return -1;
                  }
                res=SingleRuleChi(x,experiment,rule);
              }
        }
      break;

    default:
      glb_error("glbChiSys: Invalid minimization algorithm");
      return -1;
  }

  return res;
}


//-----------------------------------------------------------------
//------------------- END -----------------------------------------
//--------------- Systematics -------------------------------------
//-----------------------------------------------------------------


/***************************************************************************
 * Function glbSetInputErrors                                              *
 ***************************************************************************
 * Sets the input errors (the denominators of the prior terms).            *
 ***************************************************************************/
int glbSetInputErrors(const glb_params in)
{
  int i;

  if (in == NULL  ||  input_errors == NULL)
    { glb_error("glbSetInputErrors: NULL pointer detected"); return -1; }
  if (glbCopyParams(in, input_errors) == NULL)
    { glb_error("glbSetInputErrors: Failed to copy input errors"); return -2; }

  return glb_user_defined_input_errors(input_errors, glb_prior_user_data);
}


/***************************************************************************
 * Function glbGetInputErrors                                              *
 ***************************************************************************
 * Returns the input errors (the denominators of the prior terms).         *
 ***************************************************************************/
int glbGetInputErrors(glb_params in)
{
  if (in == NULL  ||  input_errors == NULL)
    { glb_error("glbGetInputErrors: NULL pointer detected"); return -1; }
  if (glbCopyParams(input_errors, in) == NULL)
    { glb_error("glbGetInputErrors: Failed to copy input errors"); return -2; }
  return 0;
}


/***************************************************************************
 * Function glbSetCentralValues                                            *
 ***************************************************************************
 * Sets the central values for the prior terms.                            *
 ***************************************************************************/
int glbSetCentralValues(const glb_params in)
{
  int i;

  if (in == NULL  ||  central_values == NULL)
    { glb_error("glbSetCentralValues: NULL pointer detected"); return -1; }
  if (glbCopyParams(in, central_values) == NULL)
    { glb_error("glbSetCentralValues: Failed to copy central values"); return -2; }

  return glb_user_defined_starting_values(central_values, glb_prior_user_data);
}


/***************************************************************************
 * Function glbGetCentralValues                                            *
 ***************************************************************************
 * Returns the central values for the prior terms.                         *
 ***************************************************************************/
int glbGetCentralValues(glb_params in)
{
  if (in == NULL  ||  central_values == NULL)
    { glb_error("glbGetCentralValues: NULL pointer detected"); return -1; }
  if (glbCopyParams(central_values, in) == NULL)
    { glb_error("glbGetCentralValues: Failed to copy central values"); return -2; }

  return 0;
}




//------------------------------------------------------------------
//---- Chi^2 with arbitrary number of free parameters --------------
//----------------------- 23.01.2004 -------------------------------
//------------------------------------------------------------------

static double sglb_prior(double x, double center, double sigma)
{
  if(fabs(sigma-0)<1E-12) return 0;
  return (x-center)*(x-center)/sigma/sigma;
}

static int my_default_sv(const glb_params in, void *user_data)
{
  return 0;
}

static int my_default_er(const glb_params in, void *user_data)
{
  return 0;
}

static double my_default_prior(const glb_params in, void *user_data)
{
  return 0;
}

// the user interface to register such a function ...
int glbRegisterPriorFunction(double (*prior)(const glb_params, void *user_data),
			     int (*starting)(const glb_params, void *user_data),
			     int (*error)(const glb_params, void *user_data),
                             void *user_data)
{
  if(prior==NULL)
    glb_user_defined_prior=my_default_prior;
  else
    glb_user_defined_prior=prior;

  if(starting==NULL)
    glb_user_defined_starting_values=my_default_sv;
  else
     glb_user_defined_starting_values=starting;

  if(error==NULL)
    glb_user_defined_input_errors=my_default_er;
  else
    glb_user_defined_input_errors=error;

  glb_prior_user_data = user_data;

  return 0;
}

// multi-experiment functions MDglbXSection
static double MD_chi_NP(double x[])
{
  glb_params prior_input;
  double erg2;
  double y[glbGetNumOfOscParams()+GLB_MAX_EXP];
  int i;
//  double nsp[glbGetNumOfOscParams()-6+1];
  prior_input=glbAllocParams();
  count = count +1;
  for(i=0;i<n_free;i++) y[index_tab[i]]=x[i+1];
  // This basically is superflous, however it appears to be safer not
  // change a global (i.e to this file) parameter (fix_params) at this place
  for(i=n_free;i<n_free+n_fix;i++) y[index_tab[i]]=fix_params[index_tab[i]];

  glb_params p = glbAllocParams();
  for (i=0; i < glbGetNumOfOscParams(); i++)
    p->osc->osc_params[i] = y[i];
  glb_hook_set_oscillation_parameters(p, glb_probability_user_data);
  glbFreeParams(p);

  for (i=0;i<glb_num_of_exps;i++)
    {
      glbSetExperiment(glb_experiment_list[i]);
      glb_set_profile_scaling(y[glbGetNumOfOscParams()+i],i);
      glb_set_new_rates(GLB_SLOW_RATES);
    }

  erg2=ChiS();
  // adding  the user defined prior
  // shoufling the parameter vector y into an glb_params structure
  for (i=0;i<glbGetNumOfOscParams();i++) glbSetOscParams(prior_input,y[i],i);
  for (i=0;i<glb_num_of_exps;i++) glbSetDensityParams(prior_input,
						      y[i+glbGetNumOfOscParams()],i);
  glbSetIteration(prior_input,count);

  erg2 = erg2 + glb_user_defined_prior(prior_input, glb_prior_user_data);

  glbFreeParams(prior_input);
  return erg2;
}


// single-experiment functions ChiXXXSection

static void single_SelectProjection(int set, int check_proj)
{
  int status=0;
  int i,c,c1,c2;

  for(i=0;i<glbGetNumOfOscParams();i++)
    {
      if (check_proj  &&  para_tab[i] != GLB_FREE  && para_tab[i] != GLB_FIXED)
        {
          status = -1;
          para_tab[i] = GLB_FREE;
        }
      s_para_tab[i]=para_tab[i];
    }
  if (check_proj  &&
      para_tab[glbGetNumOfOscParams()+set] != GLB_FREE  &&
      para_tab[glbGetNumOfOscParams()+set] != GLB_FIXED)
    {
      status = -1;
      para_tab[glbGetNumOfOscParams()+set] = GLB_FREE;
    }
  s_para_tab[glbGetNumOfOscParams()]=para_tab[glbGetNumOfOscParams()+set];

  if (status != 0)
    glb_error("single_SelectProjection: Projection partly undefined. Using default GLB_FREE");

  c=0;
  c1=0;
  c2=0;
  for(i=0;i<glbGetNumOfOscParams()+1;i++)
    {
      if(s_para_tab[i]==1) c++;
    }
  for(i=0;i<glbGetNumOfOscParams()+1;i++)
    {
      if(s_para_tab[i]==1)
	{
	  s_index_tab[c1]=i;
	  c1++;
	}
      else if(s_para_tab[i]==0)
	{
	  s_index_tab[c+c2]=i;
	  c2++;
	}
    }

  s_n_free=c;
  s_n_fix=c2;
  return;
}


static double chi_NP(double x[])
{
  glb_params prior_input;
  double erg2;
  double nsp[glbGetNumOfOscParams()-6+1];
  double y[glbGetNumOfOscParams()+1];
  int i;
  prior_input=glbAllocParams();
  count = count +1;
  for(i=0;i<s_n_free;i++) y[s_index_tab[i]]=x[i+1];
  // This basically is superflous, however it appears to be safer not
  // change a global (i.e to this file) parameter (fix_params) at this place
  for(i=s_n_free;i<s_n_free+s_n_fix;i++) y[s_index_tab[i]]
					 =s_fix_params[s_index_tab[i]];

  glb_params p = glbAllocParams();
  for (i=0; i < glbGetNumOfOscParams(); i++)
    p->osc->osc_params[i] = y[i];
  glb_hook_set_oscillation_parameters(p, glb_probability_user_data);
  glbFreeParams(p);

  glbSetExperiment(glb_experiment_list[glb_single_experiment_number]);
  glb_set_profile_scaling(y[glbGetNumOfOscParams()],glb_single_experiment_number);
  glb_set_new_rates(GLB_SLOW_RATES);


  erg2=ChiS0();

  // adding  the user defined prior
  // shoufling the parameter vector y into an glb_params structure
  for (i=0;i<glbGetNumOfOscParams();i++) glbSetOscParams(prior_input,y[i],i);
  glbSetDensityParams(prior_input,y[glbGetNumOfOscParams()],glb_single_experiment_number);
  glbSetIteration(prior_input,count);

  erg2 = erg2 + glb_user_defined_prior(prior_input, glb_prior_user_data);

  glbFreeParams(prior_input);
  return erg2;
}





/* This implemenst the API for the ChiXXX functions */



static double internal_glbSingleChiNP(const glb_params in, glb_params out,
                                      int exp)
{
  double *sp2;
  double **mat2;
  double er1;
  glb_projection fnew=NULL,fbuf=NULL;

  double x[GLB_MAX_EXP+glbGetNumOfOscParams()];
  int it;
  int i;
  int dim;
  if(exp >=  glb_num_of_exps)
    {
      glb_error("Failure in internal_glbSingleChiNP: exp must be smaller than"
		" glb_num_of_exps");
      return -1;
    }

  glb_single_experiment_number=exp;

  //creating memory
  single_SelectProjection(exp, 1);

  /* declaring temporariliy all densities of all other experiments as fixed */
  fbuf=glbAllocProjection();
  fnew=glbAllocProjection();
  glbGetProjection(fbuf);
  fnew=glbCopyProjection(fbuf,fnew);
  fnew=glbSetDensityProjectionFlag(fnew,GLB_FIXED,GLB_ALL);
  fnew=glbSetDensityProjectionFlag(fnew,glbGetDensityProjectionFlag(fbuf,exp)
                              ,exp);
  glbSetProjection(fnew);
  /* - finis - */

  if(out!=NULL) {
    out=glbCopyParams(in,out);
    if(out==NULL)
      {
	glb_error("Failure while copying input of glbChiNP");
	return -1;
      }
  }

  /* Check fit values */
  for(i=0; i < glbGetNumOfOscParams(); i++)
    {
      x[i] = glbGetOscParams(in,i);
      if (GLB_ISNAN(x[i]))
        {
          glb_error("internal_glbSingleChiNP: Oscillation parameters incompletely defined");
          return GLB_NAN;
        }
    }

  switch (glb_current_minimizer)
  {
    case GLB_MIN_POWELL:
      for(i=0; i < glb_num_of_exps; i++)
        {
          x[i+glbGetNumOfOscParams()] = glbGetDensityParams(in,i);
          if (i==exp  &&  GLB_ISNAN(x[i+glbGetNumOfOscParams()]))
            {
              glb_warning("internal_glbSingleChiNP: Density parameters incompletely defined. "
                          "Using default 1.0");
              x[i+glbGetNumOfOscParams()] = 1.0;
            }
        }
      break;

    case GLB_MIN_NESTED_POWELL:
      x[glbGetNumOfOscParams()] = glbGetDensityParams(in,exp);
      if (GLB_ISNAN(x[glbGetNumOfOscParams()]))
        {
          glb_warning("internal_glbSingleChiNP: Density parameters incompletely defined. Using default 1.0");
          x[glbGetNumOfOscParams()] = 1.0;
        }
      break;

    default:
      glb_error("internal_glbSingleChiNP: Invalid minimization algorithm");
      return -1;
  }


  /* Check input errors and central values */
  glb_params op = glbAllocParams();
  glbGetOscillationParameters(op);
  for (i=0; i < glbGetNumOfOscParams(); i++)
  {
    if (glbGetProjectionFlag(fnew, i) == GLB_FREE)
    {
      if (GLB_ISNAN(glbGetOscParams(input_errors, i)))
      {
        glb_error("internal_glbSingleChiNP: Innput errors incompletely defined. Using default 0.0");
        glbSetOscParams(input_errors, 0.0, i);
      }
      if (GLB_ISNAN(glbGetOscParams(central_values, i)))
      {
        glb_error("internal_glbSingleChiNP: Central values incompletely defined. "
                  "Using default (curr. osc. params)");
        glbSetOscParams(central_values, glbGetOscParams(op, i), i);
      }
    }
  }
  for (i=0; i < glb_num_of_exps; i++)
  {
    if (glbGetDensityProjectionFlag(fnew, i) == GLB_FREE)
    {
      if (GLB_ISNAN(glbGetDensityParams(input_errors, i)))
      {
        glb_error("internal_glbSingleChiNP: Density input errors incompletely defined. "
                  "Using default 0.05");
        glbSetDensityParams(input_errors, 0.05, i);
      }
      if (GLB_ISNAN(glbGetDensityParams(central_values, i)))
      {
        glb_warning("internal_glbSingleChiNP: Density central values incompletely defined. "
                    "Using default 1.0");
        glbSetDensityParams(central_values, 1.0, i);
      }
    }
  }
  glbFreeParams(op);
  if (glb_user_defined_input_errors(input_errors, glb_prior_user_data) != 0)
    { glb_error("internal_glbSingleChiNP: Failed to set input errors"); return GLB_NAN; }
  if (glb_user_defined_starting_values(central_values, glb_prior_user_data) != 0)
    { glb_error("internal_glbSingleChiNP: Failed to set central values"); return GLB_NAN; }


  /* Minimize over the free oscillation parameters */
  switch (glb_current_minimizer)
  {
    case GLB_MIN_POWELL:
      count = 0;
      glb_invoke_hybrid_minimizer(exp, GLB_ALL, x, &er1);
      if (out != NULL)
      {
        for (i=0; i < glbGetNumOfOscParams(); i++)
          glbSetOscParams(out, x[i], i);
        for (i=0; i < glb_num_of_exps; i++)
          glbSetDensityParams(out, x[i+glbGetNumOfOscParams()], i);
        glbSetIteration(out,count);
      }
      break;

    case GLB_MIN_NESTED_POWELL:
      dim=s_n_free;
      mat2=glb_alloc_mat(1,dim,1,dim);
      sp2=glb_alloc_vec(1,dim);
      init_mat(mat2,dim);
      count=0;
      for(i=0;i<glbGetNumOfOscParams()+1;i++) s_fix_params[i]=x[i];
      for(i=0;i<s_n_free;i++) sp2[i+1]=x[s_index_tab[i]];
      if (setjmp(env)==1)
        {
          okay_flag=1;
          return er1;
        }
      if(glb_powell2(sp2,mat2,dim,TOLOSC,&it,&er1,chi_NP)!=0) count=-count;
      if(out!=NULL)
        {
          for(i=0;i<s_n_free;i++)
            {
              if(s_index_tab[i]<glbGetNumOfOscParams())
                glbSetOscParams(out,sp2[i+1],s_index_tab[i]);
              else
                glbSetDensityParams(out,sp2[i+1],exp);
            }
          out=glbSetIteration(out,count);
        }

      glb_free_vec(sp2,1,dim);
      glb_free_mat(mat2,1,dim,1,dim);
      break;

    default:
      glb_error("internal_glbSingleChiNP: Invalid minimization algorithm");
      return -1;
  }

  glb_single_experiment_number=GLB_ALL;
  glbSetProjection(fbuf);
  glbFreeProjection(fnew);
  glbFreeProjection(fbuf);
  return er1;
}



static double internal_glbChiNP(const glb_params in, glb_params out)
{
  double *sp2;
  double **mat2;
  double er1;


  double x[GLB_MAX_EXP+glbGetNumOfOscParams()];
  int it;
  int i;
  int dim;

  /* Make sure that callback function knows we are in GLB_ALL mode */
  glb_single_experiment_number = GLB_ALL;

  /* Dummy call to force checking of the projection */
  SelectProjection(para_tab, 1);

  if(out!=NULL) {
    out=glbCopyParams(in,out);
    if(out==NULL)
      {
	glb_error("Failure while copying input of glbChiNP");
	return -1;
      }
  }

  /* Check fit values */
  for(i=0; i < glbGetNumOfOscParams(); i++)
    {
      x[i] = glbGetOscParams(in,i);
      if (GLB_ISNAN(x[i]))
        {
          glb_error("internal_glbChiNP: Oscillation parameters incompletely defined");
          return GLB_NAN;
        }
    }
  for(i=0; i < glb_num_of_exps; i++)
    {
      x[i+glbGetNumOfOscParams()] = glbGetDensityParams(in,i);
      if (GLB_ISNAN(x[i+glbGetNumOfOscParams()]))
        {
          glb_warning("internal_glbChiNP: Density parameters incompletely defined. Using default 1.0");
          x[i+glbGetNumOfOscParams()] = 1.0;
        }
    }


  /* Check input errors and central values */
  glb_params osc_params = glbAllocParams();
  glb_projection proj = glbAllocProjection();
  glbGetOscillationParameters(osc_params);
  glbGetProjection(proj);
  for (i=0; i < glbGetNumOfOscParams(); i++)
  {
    if (glbGetProjectionFlag(proj, i) == GLB_FREE)
    {
      if (GLB_ISNAN(glbGetOscParams(input_errors, i)))
      {
        glb_error("internal_glbChiNP: Innput errors incompletely defined. Using default 0.0");
        glbSetOscParams(input_errors, 0.0, i);
      }
      if (GLB_ISNAN(glbGetOscParams(central_values, i)))
      {
        glb_error("internal_glbChiNP: Central values incompletely defined. "
                  "Using default (curr. osc. params)");
        glbSetOscParams(central_values, glbGetOscParams(osc_params, i), i);
      }
    }
  }
  for (i=0; i < glb_num_of_exps; i++)
  {
    if (glbGetDensityProjectionFlag(proj, i) == GLB_FREE)
    {
      if (GLB_ISNAN(glbGetDensityParams(input_errors, i)))
      {
        glb_error("internal_glbChiNP: Density input errors incompletely defined. "
                  "Using default 0.05");
        glbSetDensityParams(input_errors, 0.05, i);
      }
      if (GLB_ISNAN(glbGetDensityParams(central_values, i)))
      {
        glb_warning("internal_glbChiNP: Density central values incompletely defined. "
                    "Using default 1.0");
        glbSetDensityParams(central_values, 1.0, i);
      }
    }
  }
  glbFreeProjection(proj);
  glbFreeParams(osc_params);
  if (glb_user_defined_input_errors(input_errors, glb_prior_user_data) != 0)
    { glb_error("internal_glbChiNP: Failed to set input errors"); return GLB_NAN; }
  if (glb_user_defined_starting_values(central_values, glb_prior_user_data) != 0)
    { glb_error("internal_glbChiNP: Failed to set central values"); return GLB_NAN; }

  /* Minimize over the free oscillation parameters */
  switch (glb_current_minimizer)
  {
    case GLB_MIN_POWELL:
      count = 0;
      glb_invoke_hybrid_minimizer(GLB_ALL, GLB_ALL, x, &er1);
      if (out != NULL)
      {
        for (i=0; i < glbGetNumOfOscParams(); i++)
          glbSetOscParams(out, x[i], i);
        for (i=0; i < glb_num_of_exps; i++)
          glbSetDensityParams(out, x[i+glbGetNumOfOscParams()], i);
        glbSetIteration(out, count);
      }
      break;

    case GLB_MIN_NESTED_POWELL:
      dim=n_free;
      mat2=glb_alloc_mat(1,dim,1,dim);
      sp2=glb_alloc_vec(1,dim);
      init_mat(mat2,dim);
      count=0;
      for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++) fix_params[i]=x[i];
      for(i=0;i<n_free;i++) sp2[i+1]=x[index_tab[i]];
      if (setjmp(env)==1)
        {
          okay_flag=1;
          return er1;
        }
      if(glb_powell2(sp2,mat2,dim,TOLOSC,&it,&er1,MD_chi_NP)!=0) count=-count;
      if(out!=NULL)
        {
          for(i=0;i<n_free;i++)
            {
              if(index_tab[i]<glbGetNumOfOscParams())
                glbSetOscParams(out,sp2[i+1],index_tab[i]);
              else
                glbSetDensityParams(out,sp2[i+1],index_tab[i]-glbGetNumOfOscParams());
            }
          out=glbSetIteration(out,count);
        }
      glb_free_vec(sp2,1,dim);
      glb_free_mat(mat2,1,dim,1,dim);
      break;

    default:
      glb_error("internal_glbChiNP: Invalid minimization algorithm");
      return -1;
  }

  return er1;
}

double glbChiNP(const glb_params in, glb_params out, int exp)
{
  double res;

  if(in==NULL)
    {
      glb_error("Failure in glbChiNP: Input pointer must be non-NULL");
      return -1;
    }

  if(exp==GLB_ALL) res=internal_glbChiNP(in,out);
  else res=internal_glbSingleChiNP(in,out,exp);
  return res;
}


/* Convenience wrappers for glbChiNP
 *
 * First the 1-d wrappers
 */

static double glbChi1P(const glb_params in,
	       	      glb_params out, int exp, int n)
{
  double res;
  int i,*b;
  int swit[GLB_MAX_EXP+glbGetNumOfOscParams()];
  int buff[GLB_MAX_EXP+glbGetNumOfOscParams()];
  b=CheckProjection();
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++) buff[i]=b[i];
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++) swit[i]=GLB_FREE;
  swit[n]=GLB_FIXED;
  SelectProjection(swit, 1);
  if(in==NULL)
    {
      glb_error("Failure in glbChi1P: Input pointer must be non-NULL");
      return -1;
    }

  if(exp==GLB_ALL) res=internal_glbChiNP(in,out);
  else res=internal_glbSingleChiNP(in,out,exp);
  SelectProjection(buff, 0); /* Don't check buff - it might still be at its default GLB_UNDEFINED */
  return res;
}

/* Projection of chi^2 onto theta-12 */
double glbChiTheta12(const glb_params in,glb_params out, int exp)
{
  double res;
  res=glbChi1P(in,out,exp,GLB_THETA_12);
  return res;
}

/* Projection of chi^2 onto theta-13 */
double glbChiTheta13(const glb_params in,glb_params out, int exp)
{
  double res;
  res=glbChi1P(in,out,exp,GLB_THETA_13);
  return res;
}

/* Projection of chi^2 onto theta-23 */
double glbChiTheta23(const glb_params in,glb_params out, int exp)
{
  double res;
  res=glbChi1P(in,out,exp,GLB_THETA_23);
  return res;
}

/* Projection of chi^2 onto delta_CP */
double glbChiDelta(const glb_params in,glb_params out, int exp)
{
  double res;
  res=glbChi1P(in,out,exp,GLB_DELTA_CP);
  return res;
}

/* Projection of chi^2 onto Delta m_{21}^2 */
double glbChiDm21(const glb_params in,glb_params out, int exp)
{
  double res;
  res=glbChi1P(in,out,exp,GLB_DM_21);
  return res;
}

/* Projection of chi^2 onto Delta m_{31}^2 */
double glbChiDm31(const glb_params in,glb_params out, int exp)
{
  double res;
  res=glbChi1P(in,out,exp,GLB_DM_31);
  return res;
}

/* 2-dim Projection of chi^2 onto the theta-13 - delta_CP plane */
double glbChiTheta13Delta(const glb_params in,glb_params out, int exp)
{
  double res;
  int i,*b;
  int swit[GLB_MAX_EXP+glbGetNumOfOscParams()];
  int buff[GLB_MAX_EXP+glbGetNumOfOscParams()];
  b=CheckProjection();
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++) buff[i]=b[i];
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++) swit[i]=GLB_FREE;
  swit[GLB_THETA_13]=GLB_FIXED;
  swit[GLB_DELTA_CP]=GLB_FIXED;
  SelectProjection(swit, 1);
  if(in==NULL)
    {
      glb_error("Failure in glbChiNP: Input pointer must be non-NULL");
      return -1;
    }

  if(exp==GLB_ALL) res=internal_glbChiNP(in,out);
  else res=internal_glbSingleChiNP(in,out,exp);
  SelectProjection(buff, 0); /* Don't check buff - it might still be at its default GLB_UNDEFINED */
  return res;
}

/* Wrapper for ChiAll */
double glbChiAll(const glb_params in,glb_params out, int exp)
{
  double res;
  int i,*b;
  int swit[GLB_MAX_EXP+glbGetNumOfOscParams()];
  int buff[GLB_MAX_EXP+glbGetNumOfOscParams()];
  b=CheckProjection();
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++) buff[i]=b[i];
  for(i=0;i<glbGetNumOfOscParams()+glb_num_of_exps;i++) swit[i]=GLB_FREE;
  SelectProjection(swit, 1);
  if(in==NULL)
    {
      glb_error("Failure in glbChiAll: Input pointer must be non-NULL");
      return -1;
    }

  if(exp==GLB_ALL) res=internal_glbChiNP(in,out);
  else res=internal_glbSingleChiNP(in,out,exp);
  SelectProjection(buff, 0); /* Don't check buff - it might still be at its default GLB_UNDEFINED */
  return res;
}


/* New functions dealing with setting the projection flags */


int
glbSetProjection(const glb_projection in)
{
  int i;
  int *buf;
  if(in==NULL) return -1;
  buf=glb_malloc(sizeof(int)*(glbGetNumOfOscParams()+glb_num_of_exps));

  for(i=0;i<glbGetNumOfOscParams();i++) buf[i]=glbGetProjectionFlag(in,i);
  for(i=0;i<glb_num_of_exps;i++) buf[i+glbGetNumOfOscParams()]=
				   glbGetDensityProjectionFlag(in,i);
  SelectProjection(buf, 1);
  glb_free(buf);
  return 0;

}

int
glbGetProjection(glb_projection in)
{
  int i;
  if(in==NULL) return -1;
  for(i=0;i<glbGetNumOfOscParams();i++) in=glbSetProjectionFlag(in,para_tab[i],i);
  for(i=0;i<glb_num_of_exps;i++)
    in=glbSetDensityProjectionFlag(in,para_tab[i+glbGetNumOfOscParams()],i);
  return 0;
}




/***************************************************************************
 * Function glbSelectMinimizer                                             *
 ***************************************************************************
 * Selects a minimization algorithm for use in subsequent calls to the     *
 * glbChiXXX functions.                                                    *
 ***************************************************************************
 * Parameters:                                                             *
 *   minimizer_ID: One of the GLB_MIN_XXX constants identifying a          *
 *                 minimization algorithm.                                 *
 ***************************************************************************/
int glbSelectMinimizer(int minimizer_ID)
{
  if (minimizer_ID != GLB_MIN_NESTED_POWELL  &&
      minimizer_ID != GLB_MIN_POWELL)
  {
    glb_error("glbSelectMinimizer: Invalid minimization algorithm requested");
    return -1;
  }

  glb_current_minimizer = minimizer_ID;
  return 0;
}


/***************************************************************************
 * Function glb_hybrid_chi_callback                                        *
 ***************************************************************************
 * Callback function for the hybrid minimizer. Evaluates chi^2 for the     *
 * desired experiment(s) and rule(s).                                      *
 ***************************************************************************
 * Parameters:                                                             *
 *   x: Current point in minimization space                                *
 *   new_rates_flag: Is recomputation of event rates necessary?            *
 *   user_data: Buffer for user-defined additional parameters              *
 ***************************************************************************/
double glb_hybrid_chi_callback(double *x, int new_rates_flag, void *user_data)
{
  double *y = NULL;
  double chi2 = 0.0;
  int i, j, k, m;
  int max_dim, this_dim;
  struct glb_experiment *exp;
  struct glb_systematic *sys;
  glb_params p = NULL;

  /* Recalculate rates and priors only if the minimizer says it's necessary */
  if (new_rates_flag)
  {
    /* Prepare parameter structure */
    p = glbAllocParams();
    j = 0;
    for (i=0; i < glbGetNumOfOscParams(); i++)
      p->osc->osc_params[i] = (para_tab[i]==GLB_FREE) ? x[j++] : fix_params[i];
    for (k=0; k < glb_num_of_exps; k++,i++)
      p->density->density_params[k] = (para_tab[i]==GLB_FREE) ? x[j++] : fix_params[i];
    p->iterations = ++count;

    /* Re-compute rates */
    glb_hook_set_oscillation_parameters(p, glb_probability_user_data);
    for (i=0; i < glb_num_of_exps; i++)
    {
      if (glb_single_experiment_number!=GLB_ALL && glb_single_experiment_number!=i)
        continue;
      glbSetExperiment(glb_experiment_list[i]);
      glb_set_profile_scaling(p->density->density_params[i],i);
      glb_set_new_rates(GLB_FAST_RATES);
    }

    /* Compute priors */
    glb_last_priors = glb_user_defined_prior(p, glb_prior_user_data);
    glbFreeParams(p);
  }

  /* Add up chi^2 for all experiments and rules */
  m = n_free;
  max_dim = 4;   /* This is sufficient for all internal chi^2 function */
  y = (double *) glb_malloc(sizeof(*y) * max_dim);
  for (i=0; i < glb_num_of_exps; i++)
  {
    if (glb_single_experiment_number!=GLB_ALL && glb_single_experiment_number!=i)
      continue;
    exp = (struct glb_experiment *) glb_experiment_list[i];
    for (j=0; j < exp->numofrules; j++)
    {
      if (glb_single_rule_number!=GLB_ALL && glb_single_rule_number!=j)
        continue;
      sys = (exp->sys_on_off[j] == GLB_ON) ? exp->sys_on[j] : exp->sys_off[j];
      this_dim = sys->dim;
      if (this_dim > max_dim)
      {
        glb_free(y);
        y = (double *) glb_malloc(sizeof(*y) * this_dim);
        max_dim = this_dim;
      }
      for (k=0; k < this_dim; k++)    /* Construct parameter vector */
        y[k] = x[m++];

      if (exp->sys_on_off[j] == GLB_ON)
        chi2 += (*(sys->chi_func))(i, j, this_dim, y, exp->sys_on_errors[j], sys->user_data);
      else
        chi2 += (*(sys->chi_func))(i, j, this_dim, y, exp->sys_off_errors[j], sys->user_data);
    }
  }
  glb_free(y);
  y = NULL;

  /* Add priors */
  chi2 += glb_last_priors;

  return chi2;
}


/***************************************************************************
 * Function glb_invoke_hybrid_minimizer                                    *
 ***************************************************************************
 * Wrapper function for the hybrid minimizer. Builds the data structures   *
 * for the minimizer, and returns the position of the minimum.             *
 ***************************************************************************
 * Parameters:                                                             *
 *   exp:  Experiment number for which chi^2 is requested, or GLB_ALL      *
 *   rule: Rule number for which chi^2 is requested, or GLB_ALL            *
 *   x:    Input: The starting point. Components should be the oscillation *
 *         parameters, followed by the density parameters for _all_        *
 *         experiments, even if exp!=GLB_ALL.                              *
 *         Output: The position of the minimum                             *
 *   chi:  Output: The chi^2 value at the minimum                          *
 ***************************************************************************/
int glb_invoke_hybrid_minimizer(int exp, int rule, double *x, double *chi2)
{
  double *P = NULL;
  int n_sys, i, j, k;
  int iter;

  /* Determine number of systematics parameters, compute constant factors
     in event rates to speed up the rate computation later on */
  n_sys = 0;
  for (i=0; i < glb_num_of_exps; i++)
  {
    if (exp!=GLB_ALL && exp!=i)
      continue;
    for (j=0; j < glbGetNumberOfRules(i); j++)
    {
      if (rule!=GLB_ALL && rule!=j)
        continue;
      n_sys += glbGetSysDimInExperiment(i, j, glbGetSysOnOffState(i, j));
    }

    glbSetExperiment(glb_experiment_list[i]);
    glb_rate_template();
  }

  /* Initialize oscillation parameters */
  P = (double *) glb_malloc(sizeof(*P) * (n_sys + n_free));
  k = 0;                     /* Keeps track of position in starting point vector */
  for(i=0; i < glbGetNumOfOscParams()+glb_num_of_exps; i++)
  {
    fix_params[i] = x[i];
    if (para_tab[i] == GLB_FREE)
      P[k++] = x[i];
  }

  /* Initialize systematics parameters */
  for (i=0; i < glb_num_of_exps; i++)
  {
    if (exp!=GLB_ALL && exp!=i)
      continue;
    for (j=0; j < glbGetNumberOfRules(i); j++)
    {
      if (rule!=GLB_ALL && rule!=j)
        continue;
      memcpy(&(P[k]), glbGetSysStartingValuesListPtr(i, j, glbGetSysOnOffState(i, j)),
             sizeof(double)*glbGetSysDimInExperiment(i, j, glbGetSysOnOffState(i, j)));
      k += glbGetSysDimInExperiment(i, j, glbGetSysOnOffState(i, j));
    }
  }

  /* Prepare information for callback function, and invoke minimizer */
  glb_single_experiment_number = exp;
  glb_single_rule_number       = rule;
  glb_hybrid_minimizer(P, k, n_free, TOLOSC, &iter, chi2, &glb_hybrid_chi_callback, NULL);

  /* Return position of minimum */
  k = 0;
  for (i=0; i < glbGetNumOfOscParams()+glb_num_of_exps; i++)
  {
    if (para_tab[i] == GLB_FREE)
      x[i] = P[k++];
  }

  glb_free(P);
  P = NULL;

  return 0;
}


