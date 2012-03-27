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

#include <math.h>
#include <string.h>
#include <globes/globes.h>
#include "glb_error.h"
#include "glb_multiex.h"
#include "glb_rate_engine.h"
#include "glb_minimize.h"


/* Global variables */
glb_systematic *glb_sys_list;   /* Connected list of systematics definitions */

static const char *glb_2011_compatible_chi_functions[] =
  {
    "chiSpectrumTilt",
    "chiNoSysSpectrum",
    "chiSpectrumOnly",
    "chiTotalRatesTilt",
    "chiNoSysTotalRates",
    "chiSpectrumCalib",
    ""    /* The last entry must be the empty string for loops to terminate */
  };



/***************************************************************************
 *          I N T E R N A L   H E L P E R   F U N C T I O N S              *
 ***************************************************************************/

/***************************************************************************
 * Function glbIsChiFunction2011Comptabile                                 *
 ***************************************************************************
 * Determines whether a given, named chi^2 function is compatible to       *
 * those from GLoBES 2.0.11 in the sense that it understands the           *
 * @signal_error and @background_error directives, as well as the          *
 * corresponding API functions.                                            *
 ***************************************************************************/
int glbIsChiFunction2011Compatible(const char *name)
{
  int i;

  for (i=0; glb_2011_compatible_chi_functions[i][0] != '\0'; i++)
    if (strcmp(name, glb_2011_compatible_chi_functions[i]) == 0)
      return 1;

  return 0;
}


/***************************************************************************
 * Function glbFindChiFunctionByName                                       *
 ***************************************************************************
 * Returns a pointer to the glb_systematic structure for the chi^2         *
 * function with the given name                                            *
 ***************************************************************************/
glb_systematic *glbFindChiFunctionByName(const char *name)
{
  glb_systematic *sys;

  for (sys = glb_sys_list; sys != NULL; sys = sys->next)
    if (strcmp(name, sys->name) == 0)
      break;

  if (sys == NULL)
  {
    glb_error("glbFindChiFunctionByName: Unknown chi^2 function specified");
    return NULL;
  }

  return sys;
}


/***************************************************************************
 * Function glbConvertErrorDim                                             *
 ***************************************************************************
 * Returns the name of the chi^2 function belonging to the given GLoBES    *
 * 2.0.11 errordim.                                                        *
 * The function allocates memory for the name string, which has to be      *
 * freed by the caller.                                                    *
 ***************************************************************************/
char *glbConvertErrorDim(int errordim)
{
  char name[50];

  switch (errordim)
  {
    case 0:
      strcpy(name, "chiSpectrumTilt");
      break;
    case 2:
      strcpy(name, "chiNoSysSpectrum");
      break;
    case 4:
      strcpy(name, "chiTotalRatesTilt");
      break;
    case 7:
      strcpy(name, "chiSpectrumOnly");
      break;
    case 8:
      strcpy(name, "chiNoSysTotalRates");
      break;
    case 9:
      strcpy(name, "chiSpectrumCalib");
      break;
    default:
      glb_error("glbConvertErrorDim: Invalid errordim");
      return NULL;
  }

  return strdup(name);
}


/***************************************************************************
 * Function glbCleanSysList                                                *
 ***************************************************************************
 * Removes all chi^2 functions from the list glb_sys_list.                 *
 ***************************************************************************/
int glbCleanSysList()
{
  glb_systematic *p = glb_sys_list;

  while (glb_sys_list != NULL)
  {
    glb_free(glb_sys_list->name);
    p = glb_sys_list;
    glb_sys_list = glb_sys_list->next;
    glb_free(p);
  }
  return 0;
}


/***************************************************************************
 * Function glbSetChiFunctionInRule                                        *
 ***************************************************************************
 * This internal helper function is called by glbSetChiFunctionInExperiment*
 * Its parameters are similar to those of glbSetChiFunction, but it does   *
 * allow experiment=GLB_ALL or rule=GLB_ALL                                *
 ***************************************************************************/
int glbSetChiFunctionInRule(struct glb_experiment *exp, int rule, int on_off,
                            const char *sys_id, double *errors)
{
  glb_systematic *sys;
  double tmp_errorlist[5];
  int k;

  /* Something tells me that people will come up with the idea to pass
   * exp->sys_on_errors; this has to be blocked */
  if (errors != NULL  &&
        (errors == exp->sys_on_errors[rule]  ||  errors == exp->sys_off_errors[rule]))
  {
    glb_error("glbSetChiFunctionInRule: Invalid error array");
    return -1;
  }

  /* Find requested glb_sys_list entry */
  if ((sys=glbFindChiFunctionByName(sys_id)) == NULL)
  {
    glb_error("glbSetChiFunctionInRule: Unknown chi^2 function specified");
    return -1;
  }

  /* Allow @signal_error/@background_error directives for old chi^2 functions */
  if (errors == NULL  &&  sys->dim > 0)
  {
    if (glbIsChiFunction2011Compatible(sys_id))
    {
      if (exp->signal_errors[0][rule] > 0.0  &&  exp->signal_errors[1][rule] > 0.0 &&
          exp->bg_errors[0][rule] > 0.0  &&  exp->bg_errors[1][rule] > 0.0)
      {
        tmp_errorlist[0] = exp->signal_errors[0][rule];
        tmp_errorlist[1] = exp->signal_errors[1][rule];
        tmp_errorlist[2] = exp->bg_errors[0][rule];
        tmp_errorlist[3] = exp->bg_errors[1][rule];
        tmp_errorlist[4] = -1.0;
        errors = tmp_errorlist;
      }
      else
      {
        glb_error("glbSetChiFunctionInRule: Incomplete systematics definition");
        return -1;
      }
    }
    else
    {
      glb_error("glbSetChiFunctionInRule: Invalid error array");
      return -1;
    }
  }


  if (on_off == GLB_ON)
  {
    if (rule >= 0 && rule < exp->numofrules)
    {
      /* Set new chi^2 function */
      exp->sys_on[rule] = sys;
      if (exp->sys_on_strings[rule] != NULL)  glb_free(exp->sys_on_strings[rule]);
      exp->sys_on_strings[rule] = strdup(sys->name);

      /* Copy error array and set central/starting values */
      if (sys->dim > 0)
      {
        glb_free(exp->sys_on_errors[rule]);
        glb_free(exp->sys_on_startvals[rule]);
        exp->sys_on_errors[rule]    = (double *) glb_malloc(sizeof(double) * (sys->dim+1));
        exp->sys_on_startvals[rule] = (double *) glb_malloc(sizeof(double) * (sys->dim+1));
        if (exp->sys_on_errors[rule] != NULL  &&  exp->sys_on_startvals[rule] != NULL)
        {
          for (k = 0; k < sys->dim; k++)
          {
            exp->sys_on_errors[rule][k]    = errors[k];
            exp->sys_on_startvals[rule][k] = 0.0;
          }
          exp->sys_on_errors[rule][sys->dim]    = -1.0;
          exp->sys_on_startvals[rule][sys->dim] = -1.0;
        }
      }
      else
      {
        glb_free(exp->sys_on_errors[rule]);      exp->sys_on_errors[rule]    = NULL;
        glb_free(exp->sys_on_startvals[rule]);   exp->sys_on_startvals[rule] = NULL;
      }
    }
    else
    {
      glb_error("glbSetChiFunctionInRule: Invalid rule number");
      return -1;
    }
  }
  else if (on_off == GLB_OFF)
  {
    if (rule >= 0 && rule < exp->numofrules)
    {
      /* Set new chi^2 function */
      exp->sys_off[rule] = sys;
      if (exp->sys_off_strings[rule] != NULL)  free(exp->sys_off_strings[rule]);
      exp->sys_off_strings[rule] = strdup(sys->name);

      /* Copy error array and set central/starting values */
      if (sys->dim > 0)
      {
        glb_free(exp->sys_off_errors[rule]);
        glb_free(exp->sys_off_startvals[rule]);
        exp->sys_off_errors[rule]    = (double *) glb_malloc(sizeof(double) * (sys->dim+1));
        exp->sys_off_startvals[rule] = (double *) glb_malloc(sizeof(double) * (sys->dim+1));
        if (exp->sys_off_errors[rule] != NULL  &&  exp->sys_off_startvals[rule] != NULL)
        {
          for (k = 0; k < sys->dim; k++)
          {
            exp->sys_off_errors[rule][k]    = errors[k];
            exp->sys_off_startvals[rule][k] = 0.0;
          }
          exp->sys_off_errors[rule][sys->dim]    = -1.0;
          exp->sys_off_startvals[rule][sys->dim] = -1.0;
        }
      }
      else
      {
        glb_free(exp->sys_off_errors[rule]);      exp->sys_off_errors[rule]    = NULL;
        glb_free(exp->sys_off_startvals[rule]);   exp->sys_off_startvals[rule] = NULL;
      }
    }
    else
    {
      glb_error("glbSetChiFunctionInRule: Invalid rule number");
      return -1;
    }
  }
  else
  {
    glb_error("glbSetChiFunctionInRule: on_off must be GLB_ON or GLB_OFF");
    return -1;
  }

  /* Make sure that the error list and the old signal_error / bg_error arrays
   * are consistent */
  if (glbIsChiFunction2011Compatible(sys->name) && sys->dim > 0)
  {
    exp->signal_errors[0][rule]    = errors[0];
    exp->signal_startvals[0][rule] = 0.0;
  }
  if (glbIsChiFunction2011Compatible(sys->name) && sys->dim > 1)
  {
    exp->signal_errors[1][rule]    = errors[1];
    exp->signal_startvals[1][rule] = 0.0;
  }
  if (glbIsChiFunction2011Compatible(sys->name) && sys->dim > 2)
  {
    exp->bg_errors[0][rule]        = errors[2];
    exp->bg_startvals[0][rule]     = 0.0;
  }
  if (glbIsChiFunction2011Compatible(sys->name) && sys->dim > 3)
  {
    exp->bg_errors[1][rule]        = errors[3];
    exp->bg_startvals[1][rule]     = 0.0;
  }

  return 0;
}


/***************************************************************************
 * Function glbSetChiFunctionInExperiment                                  *
 ***************************************************************************
 * This internal helper function is called by glbSetChiFunction. Its       *
 * parameters are similar to those of glbSetChiFunction, but it does not   *
 * allow experiment=GLB_ALL, and the sys_id string is replaced by a        *
 * pointer.                                                                *
 ***************************************************************************/
int glbSetChiFunctionInExperiment(struct glb_experiment *exp, int rule, int on_off,
                                  const char *sys_id, double *errors)
{
  int j;

  if (rule == GLB_ALL)
  {
    for (j=0; j < exp->numofrules; j++)
      if (glbSetChiFunctionInRule(exp, j, on_off, sys_id, errors) != 0)
        return -1;
  }
  else
    return glbSetChiFunctionInRule(exp, rule, on_off, sys_id, errors);

  return 0;
}


/***************************************************************************
 * Function glbSwitchSystematicsInExperiment                               *
 ***************************************************************************
 * This internal helper function is called by glbSwitchSystematics. Its    *
 * parameters are similar to those of glbSwitchSystematics, but it does    *
 * not allow experiment=GLB_ALL                                            *
 ***************************************************************************/
int glbSwitchSystematicsInExperiment(int experiment, int rule, int on_off)
{
  int i;
  struct glb_experiment *in;

  if(on_off != GLB_ON  &&  on_off!=GLB_OFF)
  {
    glb_error("glbSwitchSystematicsInExperiment: Invalid value for on_off");
    return -1;
  }

  in = (struct glb_experiment *) glb_experiment_list[experiment];
  if(rule == GLB_ALL)
  {
    for(i=0; i < in->numofrules; i++)
      in->sys_on_off[i] = on_off;
  }
  else if (rule >= 0  &&  rule < in->numofrules)
    in->sys_on_off[rule] = on_off;
  else
  {
    glb_error("glbSwitchSystematicsInExperiment: Invalid value for rule number");
    return -1;
  }
  return 0;
}


/***************************************************************************
 * Function glbSetSignalErrorsInRule                                       *
 ***************************************************************************
 * This internal helper function is similar to glbSetSignalErrors, but     *
 * it does not allow exp or rule to be GLB_ALL.                            *
 ***************************************************************************/
int glbSetSignalErrorsInRule(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0  &&  rule < in->numofrules)
    {
      if (norm > 0  &&  tilt > 0)
      {
        /* Set errors */
        in->signal_errors[0][rule] = norm;
        in->signal_errors[1][rule] = tilt;

        /* If we are using GLoBES 2.0.11 compatible systematics, they
         * should be affected by SetSignal/BGErrors */
        if (glbIsChiFunction2011Compatible(in->sys_on_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 0)
            in->sys_on_errors[rule][0] = in->signal_errors[0][rule];
          if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 1)
            in->sys_on_errors[rule][1] = in->signal_errors[1][rule];
        }
        if (glbIsChiFunction2011Compatible(in->sys_off_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 0)
            in->sys_off_errors[rule][0] = in->signal_errors[0][rule];
          if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 1)
            in->sys_off_errors[rule][1] = in->signal_errors[1][rule];
        }
      }
      else
        { glb_error("glbSetSignalErrorsInRule: Errors must be > 0"); return -1; }
    }
    else
      { glb_error("glbSetSignalErrorsInRule: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSignalErrorsInRule: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetBGErrorsInRule                                           *
 ***************************************************************************
 * This internal helper function is similar to glbSetBGErrors, but         *
 * it does not allow exp or rule to be GLB_ALL.                            *
 ***************************************************************************/
int glbSetBGErrorsInRule(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0  &&  rule < in->numofrules)
    {
      if (norm > 0  &&  tilt > 0)
      {
        /* Set errors */
        in->bg_errors[0][rule] = norm;
        in->bg_errors[1][rule] = tilt;

        /* If we are using GLoBES 2.0.11 compatible systematics, they
         * should be affected by SetSignal/BGErrors */
        if (glbIsChiFunction2011Compatible(in->sys_on_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 2)
            in->sys_on_errors[rule][2] = in->bg_errors[0][rule];
          if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 3)
            in->sys_on_errors[rule][3] = in->bg_errors[1][rule];
        }
        if (glbIsChiFunction2011Compatible(in->sys_off_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 2)
            in->sys_off_errors[rule][2] = in->bg_errors[0][rule];
          if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 3)
            in->sys_off_errors[rule][3] = in->bg_errors[1][rule];
        }
      }
      else
        { glb_error("glbSetBGErrorsInRule: Errors must be > 0"); return -1; }
    }
    else
      { glb_error("glbSetBGErrorsInRule: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetBGErrorsInRule: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetSysErrorsListInRule                                      *
 ***************************************************************************
 * This internal helper function is similar to glbSetSysErrorsList, but    *
 * it does not allow exp or rule to be GLB_ALL.                            *
 ***************************************************************************/
int glbSetSysErrorsListInRule(int exp, int rule, int on_off, const double *sys_list)
{
  struct glb_experiment *in;
  int i;

  if (sys_list == NULL)
    { glb_error("glbSetSysErrorsListInRule: Input list is NULL"); return -1; }

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0  &&  rule < in->numofrules)
    {
      if (on_off == GLB_ON)
      {
        for (i=0; i < glbGetSysDimInExperiment(exp, rule, on_off); i++)
          in->sys_on_errors[rule][i] = sys_list[i];

        /* For GLoBES 2.0.11-compatible chi^2 functions, make sure that the
         * errorlist and the signal/bg errors are consistent */
        if (glbIsChiFunction2011Compatible(in->sys_on_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 0)
            in->signal_errors[0][rule] = sys_list[0];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 1)
            in->signal_errors[1][rule] = sys_list[1];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 2)
            in->bg_errors[0][rule]     = sys_list[2];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 3)
            in->bg_errors[1][rule]     = sys_list[3];
        }
      }
      else if (on_off == GLB_OFF)
      {
        for (i=0; i < glbGetSysDimInExperiment(exp, rule, on_off); i++)
          in->sys_off_errors[rule][i] = sys_list[i];

        if (glbIsChiFunction2011Compatible(in->sys_off_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 0)
            in->signal_errors[0][rule] = sys_list[0];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 1)
            in->signal_errors[1][rule] = sys_list[1];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 2)
            in->bg_errors[0][rule]     = sys_list[2];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 3)
            in->bg_errors[1][rule]     = sys_list[3];
        }
      }
      else
        { glb_error("glbSetSysErrorsListInRule: on_off must be GLB_ON or GLB_OFF"); return -1; }
    }
    else
      { glb_error("glbSetSysErrorsListInRule: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSysErrorsListInRule: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetSignalStartingValuesInRule                               *
 ***************************************************************************
 * This internal helper function is similar to glbSetSignalStartingValues, *
 * but it does not allow exp or rule to be GLB_ALL.                        *
 ***************************************************************************/
int glbSetSignalStartingValuesInRule(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0  &&  rule < in->numofrules)
    {
      /* Set starting values */
      in->signal_startvals[0][rule] = norm;
      in->signal_startvals[1][rule] = tilt;

      /* If we are using GLoBES 2.0.11 compatible systematics, they
       * should be affected by SetSignal/BGStartingValues */
      if (glbIsChiFunction2011Compatible(in->sys_on_strings[rule]))
      {
        if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 0)
          in->sys_on_startvals[rule][0] = in->signal_startvals[0][rule];
        if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 1)
          in->sys_on_startvals[rule][1] = in->signal_startvals[1][rule];
      }
      if (glbIsChiFunction2011Compatible(in->sys_off_strings[rule]))
      {
        if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 0)
          in->sys_off_startvals[rule][0] = in->signal_startvals[0][rule];
        if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 1)
          in->sys_off_startvals[rule][1] = in->signal_startvals[1][rule];
      }
    }
    else
      { glb_error("glbSetSignalStartingValuesInRule: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSignalStartingValuesInRule: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetBGStartingValuesInRule                                   *
 ***************************************************************************
 * This internal helper function is similar to glbSetBGStartingValues, but *
 * it does not allow exp or rule to be GLB_ALL.                            *
 ***************************************************************************/
int glbSetBGStartingValuesInRule(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0  &&  rule < in->numofrules)
    {
      /* Set starting values */
      in->bg_startvals[0][rule] = norm;
      in->bg_startvals[1][rule] = tilt;

      /* If we are using GLoBES 2.0.11 compatible systematics, they
       * should be affected by SetSignal/BGStartingValues */
      if (glbIsChiFunction2011Compatible(in->sys_on_strings[rule]))
      {
        if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 2)
          in->sys_on_startvals[rule][2] = in->bg_startvals[0][rule];
        if (glbGetSysDimInExperiment(exp, rule, GLB_ON) > 3)
          in->sys_on_startvals[rule][3] = in->bg_startvals[1][rule];
      }
      if (glbIsChiFunction2011Compatible(in->sys_off_strings[rule]))
      {
        if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 2)
          in->sys_off_startvals[rule][2] = in->bg_startvals[0][rule];
        if (glbGetSysDimInExperiment(exp, rule, GLB_OFF) > 3)
          in->sys_off_startvals[rule][3] = in->bg_startvals[1][rule];
      }
    }
    else
      { glb_error("glbSetBGStartingValuesInRule: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetBGStartingValuesInRule: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetSysStartingValuesListInRule                              *
 ***************************************************************************
 * This internal helper function is similar to glbSetSysStartingValuesList,*
 * but it does not allow exp or rule to be GLB_ALL.                        *
 ***************************************************************************/
int glbSetSysStartingValuesListInRule(int exp, int rule, int on_off, const double *sys_list)
{
  struct glb_experiment *in;
  int i;

  if (sys_list == NULL)
    { glb_error("glbSetSysStartingValuesListInRule: Input list is NULL"); return -1; }

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0  &&  rule < in->numofrules)
    {
      if (on_off == GLB_ON)
      {
        for (i=0; i < glbGetSysDimInExperiment(exp, rule, on_off); i++)
          in->sys_on_startvals[rule][i] = sys_list[i];

        /* For GLoBES 2.0.11-compatible chi^2 functions, make sure that the
         * errorlist and the signal/bg errors are consistent */
        if (glbIsChiFunction2011Compatible(in->sys_on_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 0)
            in->signal_startvals[0][rule] = sys_list[0];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 1)
            in->signal_startvals[1][rule] = sys_list[1];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 2)
            in->bg_startvals[0][rule]     = sys_list[2];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 3)
            in->bg_startvals[1][rule]     = sys_list[3];
        }
      }
      else if (on_off == GLB_OFF)
      {
        for (i=0; i < glbGetSysDimInExperiment(exp, rule, on_off); i++)
          in->sys_off_startvals[rule][i] = sys_list[i];

        if (glbIsChiFunction2011Compatible(in->sys_off_strings[rule]))
        {
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 0)
            in->signal_startvals[0][rule] = sys_list[0];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 1)
            in->signal_startvals[1][rule] = sys_list[1];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 2)
            in->bg_startvals[0][rule]     = sys_list[2];
          if (glbGetSysDimInExperiment(exp, rule, on_off) > 3)
            in->bg_startvals[1][rule]     = sys_list[3];
        }
      }
      else
        { glb_error("glbSetSysStartingValuesListInRule: on_off must be GLB_ON or GLB_OFF");
          return -1; }
    }
    else
      { glb_error("glbSetSysStartingValuesListInRule: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSysStartingValuesListInRule: Invalid experiment number"); return -1; }

  return 0;
}



/***************************************************************************
 * Function glb_likelihood                                                 *
 ***************************************************************************
 * Calculate the Poisson likelihood for a specific true and fitted event   *
 * rate.                                                                   *
 ***************************************************************************/
inline double glb_likelihood(double true_rate, double fit_rate)
{
  double res;
  res = fit_rate - true_rate;
  if (true_rate > 0)
  {
    if (fit_rate <= 0.0)
      res = 1e100;
    else
      res += true_rate * log(true_rate/fit_rate);
  }
  else
    res = fabs(res);

  return 2.0 * res;

/* JK - 2008-03-11
  double res;
  res = fit_rate - true_rate;
  if (fit_rate <= 0.0)
    res = 1e100;
  else if (true_rate > 0)
    res += true_rate * log(true_rate/fit_rate);

  return 2.0 * res;
*/

/* JK - 2007-06-29
  double res;
  res = fit_rate - true_rate;
  if (true_rate > 0)
  {
    if (fit_rate <= 0.0)
      res = 1e100;
    else
      res += true_rate * log(true_rate/fit_rate);
  }
  return 2.0 * res;
*/

/* JK - 29.03.2007
  if(fit_rate > 0)
    return 2*(true_rate - fit_rate) + 2*fit_rate * log(fit_rate/true_rate);
  else
    return 2*(true_rate - fit_rate);
*/
}

/* Alternative version of likelihood, using Gauss distribution */
/*inline double likelihood(double ntheo, double nobs)
{
  if(nobs>0)
    return (ntheo-nobs)*(ntheo-nobs)/ntheo;
  else
    return 0;
}*/


/***************************************************************************
 * Function glb_prior                                                      *
 ***************************************************************************
 * Calculate prior term of the form ((x - x_center)/error)^2.              *
 ***************************************************************************/
inline double glb_prior(double x, double center, double sigma)
{
  double tmp = (x - center)/sigma;
  return tmp*tmp;
}


/***************************************************************************
 *                     C H I ^ 2   F U N C T I O N S                       *
 ***************************************************************************/

/***************************************************************************
 * Function glbChiSpectrumTilt                                             *
 ***************************************************************************
 * chi^2 including the standard signal and background errors, as well as   *
 * spectral information                                                    *
 ***************************************************************************/
double glbChiSpectrumTilt(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  int n_bins = glbGetNumberOfBins(exp);
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double *bin_centers      = glbGetBinCentersListPtr(exp);
  double signal_norm, signal_tilt;
  double bg_norm_center, bg_tilt_center;
  double bg_norm, bg_tilt;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate, true_rate;
  double chi2 = 0.0;
  int i;

  glbGetEminEmax(exp, &emin, &emax);
  ecenter = 0.5 * (emax + emin);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  signal_norm = 1.0 + x[0];
  signal_tilt = x[1] / (emax - emin);
  bg_norm = bg_norm_center * (1.0 + x[2]);
  bg_tilt = x[3] / (emax - emin);
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_norm*signal_fit_rates[i]
                + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                + bg_norm*bg_fit_rates[i]
                + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    chi2 += glb_likelihood(true_rates[i], fit_rate);
  }

  chi2 += glb_prior(x[0], 0.0, errors[0])
            + glb_prior(x[1], 0.0, errors[1])
            + glb_prior(bg_norm, bg_norm_center, errors[2])
            + glb_prior(bg_tilt, bg_tilt_center, errors[3]);

  return chi2;
}


/***************************************************************************
 * Function glbChiNoSysSpectrum                                            *
 ***************************************************************************
 * chi^2 without systematical errors                                       *
 ***************************************************************************/
double glbChiNoSysSpectrum(int exp, int rule, int n_params, double *x, double *errors,
                           void *user_data)
{
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double bg_norm_center, bg_tilt_center;
  int ew_low, ew_high;
  double fit_rate, true_rate;
  double chi2 = 0.0;
  int i;

  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_fit_rates[i] + bg_norm_center * bg_fit_rates[i];
    chi2 += glb_likelihood(true_rates[i], fit_rate);
  }

  return chi2;
}


/***************************************************************************
 * Function glbChiSpectrumOnly                                             *
 ***************************************************************************
 * chi^2 with spectral information, an unconstrained signal normalization  *
 * error, and no background errors                                         *
 ***************************************************************************/
double glbChiSpectrumOnly(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data)
{
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double signal_norm;
  double bg_norm_center, bg_tilt_center;
  int ew_low, ew_high;
  double fit_rate, true_rate;
  double chi2 = 0.0;
  int i;

  signal_norm = 1.0 + x[0];
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_norm*signal_fit_rates[i] + bg_norm_center*bg_fit_rates[i];
    chi2 += glb_likelihood(true_rates[i], fit_rate);
  }

  return chi2;
}


/***************************************************************************
 * Function glbChiTotalRatesTilt                                           *
 ***************************************************************************
 * chi^2 including the standard signal and background errors, but          *
 * considering only total rates                                            *
 ***************************************************************************/
double glbChiTotalRatesTilt(int exp, int rule, int n_params, double *x, double *errors,
                            void *user_data)
{
  int n_bins = glbGetNumberOfBins(exp);
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double *bin_centers      = glbGetBinCentersListPtr(exp);
  double signal_norm, signal_tilt;
  double bg_norm_center, bg_tilt_center;
  double bg_norm, bg_tilt;
  int ew_low, ew_high;
  double emin, emax, ecenter;
  double fit_rate, true_rate;
  double total_fit_rate, total_true_rate;
  double chi2 = 0.0;
  int i;

  glbGetEminEmax(exp, &emin, &emax);
  ecenter = 0.5 * (emax + emin);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  signal_norm = 1.0 + x[0];
  signal_tilt = x[1] / (emax - emin);
  bg_norm = bg_norm_center * (1.0 + x[2]);
  bg_tilt = x[3] / (emax - emin);

  total_fit_rate  = 0.0;
  total_true_rate = 0.0;
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_norm*signal_fit_rates[i]
                + signal_tilt*(bin_centers[i]-ecenter)*signal_fit_rates[i]
                + bg_norm*bg_fit_rates[i]
                + bg_tilt*(bin_centers[i]-ecenter)*bg_fit_rates[i];
    total_fit_rate  += fit_rate;
    total_true_rate += true_rates[i];
  }

  chi2 = glb_likelihood(total_true_rate, total_fit_rate);

  chi2 += glb_prior(x[0], 0.0, errors[0])
            + glb_prior(x[1], 0.0, errors[1])
            + glb_prior(bg_norm, bg_norm_center, errors[2])
            + glb_prior(bg_tilt, bg_tilt_center, errors[3]);

  return chi2;
}


/***************************************************************************
 * Function glbChiNoSysTotalRates                                          *
 ***************************************************************************
 * chi^2 without systematical errors, and considering only total rates     *
 ***************************************************************************/
double glbChiNoSysTotalRates(int exp, int rule, int n_params, double *x, double *errors,
                             void *user_data)
{
  double *true_rates       = glbGetRuleRatePtr(exp, rule);
  double *signal_fit_rates = glbGetSignalFitRatePtr(exp, rule);
  double *bg_fit_rates     = glbGetBGFitRatePtr(exp, rule);
  double bg_norm_center, bg_tilt_center;
  int ew_low, ew_high;
  double fit_rate, true_rate;
  double total_fit_rate, total_true_rate;
  double chi2 = 0.0;
  int i;

  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  total_fit_rate  = 0.0;
  total_true_rate = 0.0;
  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = (signal_fit_rates[i] + bg_norm_center*bg_fit_rates[i]);
    total_fit_rate  += fit_rate;
    total_true_rate += true_rates[i];
  }

  chi2 = glb_likelihood(total_true_rate, total_fit_rate);

  return chi2;
}


/***************************************************************************
 * Function glbChiSpectrumCalib                                            *
 ***************************************************************************
 * chi^2 including the standard signal and background errors, as well as   *
 * spectral information. Energy tilt is replaced by energy calibration.    *
 ***************************************************************************/
double glbChiSpectrumCalib(int exp, int rule, int n_params, double *x, double *errors,
                           void *user_data)
{
  int n_bins = glbGetNumberOfBins(exp);
  double *true_rates = glbGetRuleRatePtr(exp, rule);
  double signal_fit_rates[n_bins];
  double bg_fit_rates[n_bins];
  double signal_norm;
  double bg_norm_center, bg_tilt_center;
  double bg_norm, bg_tilt;
  int ew_low, ew_high;
  double emin, emax;
  double fit_rate, true_rate;
  double chi2 = 0.0;
  int i;

  glbGetEminEmax(exp, &emin, &emax);
  glbGetEnergyWindowBins(exp, rule, &ew_low, &ew_high);
  glbGetBGCenters(exp, rule, &bg_norm_center, &bg_tilt_center);
  signal_norm = 1.0 + x[0];
  bg_norm     = bg_norm_center * (1.0 + x[2]);
  bg_tilt     = bg_tilt_center + x[3];

  glbShiftEnergyScale(x[1], glbGetSignalFitRatePtr(exp, rule),
                      signal_fit_rates, n_bins, emin, emax);
  glbShiftEnergyScale(bg_tilt, glbGetBGFitRatePtr(exp, rule),
                      bg_fit_rates, n_bins, emin, emax);

  for (i=ew_low; i <= ew_high; i++)
  {
    fit_rate = signal_norm*signal_fit_rates[i] + bg_norm*bg_fit_rates[i];
    chi2 += glb_likelihood(true_rates[i], fit_rate);
  }

  chi2 += glb_prior(x[0], 0.0, errors[0])
            + glb_prior(x[1], 0.0, errors[1])
            + glb_prior(bg_norm, bg_norm_center, errors[2])
            + glb_prior(bg_tilt, bg_tilt_center, errors[3]);

  return chi2;
}


/***************************************************************************
 * Function glbChiZero                                                     *
 ***************************************************************************
 * Dummy chi^2 function which simply returns zero.                         *
 ***************************************************************************/
double glbChiZero(int exp, int rule, int n_params, double *x, double *errors,
                  void *user_data)
{
  return 0.0;
}



/***************************************************************************
 *                       A P I   F U N C T I O N S                         *
 ***************************************************************************/

/***************************************************************************
 * Function glbDefineChiFunction                                           *
 ***************************************************************************
 * Registers a chi^2 function for use in AEDL files read by subsequent     *
 * calls to glbInitExperiment.                                             *
 ***************************************************************************
 * Parameters:                                                             *
 *   chi_func:  A pointer to the chi^2 function                            *
 *   dim:       Number of systematics parameters handled by chi_func       *
 *   name:      The unique name for chi_func that appears in the AEDL file *
 *   user_data: Arbitrary pointer which will be passed to the user-defined *
 *              function                                                   *
 ***************************************************************************/
int glbDefineChiFunction(glb_chi_function chi_func, int dim, const char *name,
                         void *user_data)
{
  glb_systematic *old_root = glb_sys_list;   /* Save old list root */

  if (dim < 0)
  {
    glb_error("glbDefineChiFunction: Dimension of parameter space must be >= 0");
    return -1;
  }

  /* Create new list entry */
  glb_sys_list = (glb_systematic *) glb_malloc(sizeof(glb_systematic));
  if (glb_sys_list != NULL)
  {
    glb_sys_list->chi_func  = chi_func;
    glb_sys_list->dim       = dim;
    glb_sys_list->user_data = user_data;
    glb_sys_list->name      = (char *) glb_malloc(strlen(name)+1);
    if (glb_sys_list->name != NULL)
    {
      strcpy(glb_sys_list->name, name);
      glb_sys_list->next = old_root;         /* Reconnect list */
      return 0;
    }
  }

  return -1;
}


/***************************************************************************
 * Function glbSetChiFunction                                              *
 ***************************************************************************
 * Selects a chi^2 function for a specific rule and experiment             *
 ***************************************************************************
 * Parameters:                                                             *
 *   exp:    The experiment (can be GLB_ALL)                               *
 *   rule:   The rule (can be GLB_ALL)                                     *
 *   on_off: The systematics state (GLB_ON or GLB_OFF) for which this      *
 *           chi^2 function is intended                                    *
 *   sys_id: The name of the chi^2 function set by glbDefineChiFunction    *
 *   errors: A list of numerical values for the systematical errors        *
 ***************************************************************************/
int glbSetChiFunction(int exp, int rule, int on_off, const char *sys_id, double *errors)
{
  int i;

  if (exp == GLB_ALL)
  {
    for (i=0; i < glb_num_of_exps; i++)
      if (glbSetChiFunctionInExperiment(glb_experiment_list[i], rule, on_off,
                                        sys_id, errors) != 0)
        return -1;
  }
  else
  {
    if (exp >= 0  &&  exp < glb_num_of_exps)
      return glbSetChiFunctionInExperiment(glb_experiment_list[exp], rule, on_off,
                                           sys_id, errors);
    else
    {
      glb_error("glbSetChiFunction: Invalid experiment number");
      return -1;
    }
  }

  return 0;
}


/***************************************************************************
 * Function glbGetChiFunction                                              *
 ***************************************************************************
 * Copies the name of the chi^2 function of a given experiment and rule    *
 * into the string sys_id. The length of sys_id is given in max_len.       *
 ***************************************************************************/
int glbGetChiFunction(int exp, int rule, int on_off, char *sys_id, size_t max_len)
{
  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    if (rule >= 0 && rule < glb_experiment_list[exp]->numofrules)
    {
      if (on_off == GLB_ON)
      {
        if (glb_experiment_list[exp]->sys_on_strings[rule] != NULL)
        {
          strncpy(sys_id, glb_experiment_list[exp]->sys_on_strings[rule], max_len-1);
          sys_id[max_len-1] = '\0';
          if (strlen(sys_id) < strlen(glb_experiment_list[exp]->sys_on_strings[rule]))
            { glb_error("glbGetChiFunction: Target buffer is too small."); return -1; }
        }
        else
          { glb_error("glbGetChiFunction: No chi^2 function defined for this rule");
            return -1; }
      }
      else if (on_off == GLB_OFF)
      {
        if (glb_experiment_list[exp]->sys_off_strings[rule] != NULL)
        {
          strncpy(sys_id, glb_experiment_list[exp]->sys_off_strings[rule], max_len-1);
          sys_id[max_len-1] = '\0';
          if (strlen(sys_id) < strlen(glb_experiment_list[exp]->sys_off_strings[rule]))
            { glb_error("glbGetChiFunction: Target buffer is too small."); return -1; }
        }
        else
          { glb_error("glbGetChiFunction: No chi^2 function defined for this rule");
            return -1; }
      }
      else
      {
        glb_error("glbGetChiFunction: on_off must be GLB_ON or GLB_OFF");
        return -1;
      }
    }
    else
      { glb_error("glbGetChiFunction: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetChiFunction: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetChiFunctionPtrInExperiment                               *
 ***************************************************************************
 * Returns a pointer to a given named chi^2 function.                      *
 ***************************************************************************/
glb_chi_function glbGetChiFunctionPtr(const char *name)
{
  glb_systematic *sys;

  /* Find requested glb_sys_list entry */
  if ((sys=glbFindChiFunctionByName(name)) == NULL)
  {
    glb_error("glbGetChiFunctionPtr: Unknown chi^2 function specified");
    return NULL;
  }

  return sys->chi_func;
}


/***************************************************************************
 * Function glbGetChiFunctionPtrInExperiment                               *
 ***************************************************************************
 * Returns a pointer to the chi^2 function of a given experiment and rule. *
 ***************************************************************************/
glb_chi_function glbGetChiFunctionPtrInExperiment(int exp, int rule, int on_off)
{
  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    if (rule >= 0 && rule < glb_experiment_list[exp]->numofrules)
    {
      if (on_off == GLB_ON)
        return glb_experiment_list[exp]->sys_on[rule]->chi_func;
      else if (on_off == GLB_OFF)
        return glb_experiment_list[exp]->sys_off[rule]->chi_func;
      else
      {
        glb_error("glbGetChiFunctionPtr: on_off must be GLB_ON or GLB_OFF");
        return NULL;
      }
    }
    else
      { glb_error("glbGetChiFunctionPtr: Invalid rule number"); return NULL; }
  }
  else
    { glb_error("glbGetChiFunctionPtr: Invalid experiment number"); return NULL; }

  return NULL;
}


/***************************************************************************
 * Function glbSetErrorDim                                                 *
 ***************************************************************************
 * Selects a new chi^2 function via the old, numerical error-dimensions    *
 ***************************************************************************/
int glbSetErrorDim(int exp, int rule, int on_off, int errordim)
{
  char *name;
  int status;

  /* Convert the error dimension to the name of a chi^2 function */
  if ((name=glbConvertErrorDim(errordim)) == NULL)
  {
    glb_error("glbSetErrorDim: Invalid error dimensions");
    return -1;
  }

  status = glbSetChiFunction(exp, rule, on_off, name, NULL);

  glb_free(name);
  return status;
}


/***************************************************************************
 * Function glbGetErrorDim                                                 *
 ***************************************************************************
 * Returns the chi^2 function of a given experiment and rule as one of the *
 * old error dimension.                                                    *
 ***************************************************************************/
int glbGetErrorDim(int exp, int rule, int on_off)
{
  char sys_id[100];
  int i;

  if (glbGetChiFunction(exp, rule, on_off, sys_id, 100) != 0)
  {
    glb_error("glbGetErrorDim: Invalid parameters");
    return -1;
  }

  /* OK, this is not very elegant, but it's a deprecated function anyway ... */
  if (strcmp(sys_id, "chiSpectrumTilt") == 0)
    return 0;
  else if (strcmp(sys_id, "chiNoSysSpectrum") == 0)
    return 2;
  else if (strcmp(sys_id, "chiTotalRatesTilt") == 0)
    return 4;
  else if (strcmp(sys_id, "chiSpectrumOnly") == 0)
    return 7;
  else if (strcmp(sys_id, "chiNoSysTotalRates") == 0)
    return 8;
  else if (strcmp(sys_id, "chiSpectrumCalib") == 0)
    return 9;
  else
  {
    glb_error("glbGetErrorDim: No error dimension associated with this chi^2 function");
    return -1;
  }

  return -1;
}


/***************************************************************************
 * Function glbGetSysDim                                                   *
 ***************************************************************************
 * Returns the dimension of the systematics parameter space expected by    *
 * a specific chi^2 function                                               *
 ***************************************************************************/
int glbGetSysDim(const char *name)
{
  glb_systematic *sys;

  /* Find requested glb_sys_list entry */
  if ((sys=glbFindChiFunctionByName(name)) == NULL)
  {
    glb_error("glbGetSysDim: Unknown chi^2 function specified");
    return -1;
  }

  return sys->dim;
}


/***************************************************************************
 * Function glbGetSysDimInExperiment                                       *
 ***************************************************************************
 * Returns the dimension of the systematics parameter space expected by    *
 * the chi^2 function of a specific rule in a specific experiment          *
 ***************************************************************************/
int glbGetSysDimInExperiment(int exp, int rule, int on_off)
{
  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    if (rule >= 0 && rule < glb_experiment_list[exp]->numofrules)
    {
      if (on_off == GLB_ON)
      {
        if (glb_experiment_list[exp]->sys_on[rule] != NULL)
          return glb_experiment_list[exp]->sys_on[rule]->dim;
        else
          { glb_error("glbGetSysDimInExperiment: No chi^2 function defined for this rule");
            return -1; }
      }
      else if (on_off == GLB_OFF)
      {
        if (glb_experiment_list[exp]->sys_off[rule] != NULL)
          return glb_experiment_list[exp]->sys_off[rule]->dim;
        else
          { glb_error("glbGetSysDimInExperiment: No chi^2 function defined for this rule");
            return -1; }
      }
      else
      {
        glb_error("glbGetSysDimInExperiment: on_off must be GLB_ON or GLB_OFF");
        return -1;
      }
    }
    else
      { glb_error("glbGetSysDimInExperiment: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetSysDimInExperiment: Invalid experiment number"); return -1; }

  return -1;
}


/***************************************************************************
 * Function glbSwitchSystematics                                           *
 ***************************************************************************
 * Switches between the GLB_ON and GLB_OFF systematics definitions for a   *
 * given experiment and rule (both can be GLB_ALL)                         *
 ***************************************************************************/
int glbSwitchSystematics(int experiment, int rule, int on_off)
{
  int i, s=0;

  if (experiment == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
      s += glbSwitchSystematicsInExperiment(i, rule, on_off);
  }
  else if (experiment >= 0  &&  experiment < glb_num_of_exps)
  {
    s += glbSwitchSystematicsInExperiment(experiment, rule, on_off);
  }
  else
  {
    glb_error("glbSwitchSystematics: Invalid value for experiment number");
    return -1;
  }

  return s;
}


/***************************************************************************
 * Function glbGetSysOnOffState                                            *
 ***************************************************************************
 * Returns whether systematics are switched on (GLB_ON) or off (GLB_OFF)   *
 * in a given experiment and rule.                                         *
 ***************************************************************************/
int glbGetSysOnOffState(int exp, int rule)
{
  struct glb_experiment *in;

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
      return in->sys_on_off[rule];
    else
      { glb_error("glbGetSysOnOffState: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetSysOnOffState: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetSignalErrors                                             *
 ***************************************************************************
 * Sets the signal norm and energy errors.                                 *
 * Note that these values are only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbSetSignalErrors(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;
  int i, k;

  if (exp == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
    {
      in = glb_experiment_list[i];
      if (rule == GLB_ALL)
      {
        for (k=0; k < in->numofrules; k++)
          if (glbSetSignalErrorsInRule(i, k, norm, tilt) != 0)
            return -1;;
      }
      else if (rule >= 0 && rule < in->numofrules)
      {
        if (glbSetSignalErrorsInRule(i, rule, norm, tilt) != 0)
          return -1;
      }
      else
        { glb_error("glbSetSignalErrors: Invalid rule number"); return -1; }
    }
  }
  else if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule == GLB_ALL)
    {
      for (k=0; k < in->numofrules; k++)
        if (glbSetSignalErrorsInRule(exp, k, norm, tilt) != 0)
          return -1;
    }
    else if (rule >= 0 && rule < in->numofrules)
    {
      if (glbSetSignalErrorsInRule(exp, rule, norm, tilt) != 0)
        return -1;
    }
    else
      { glb_error("glbSetSignalErrors: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSignalErrors: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetSignalErrors                                             *
 ***************************************************************************
 * Writes the signal norm and energy errors for a given experiment and     *
 * rule into the variables norm and tilt.                                  *
 * Note that this function is only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbGetSignalErrors(int exp, int rule, double *norm, double *tilt)
{
  struct glb_experiment *in;

  if (norm == NULL  ||  tilt == NULL)
    { glb_error("glbGetSignalErrors: Input pointers may not be NULL"); return -1; }

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
    {
      *norm = in->signal_errors[0][rule];
      *tilt = in->signal_errors[1][rule];
    }
    else
      { glb_error("glbGetSignalErrors: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetSignalErrors: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetBGErrors                                                 *
 ***************************************************************************
 * Sets the background norm and energy errors.                             *
 * Note that these values are only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbSetBGErrors(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;
  int i, k;

  if (exp == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
    {
      in = glb_experiment_list[i];
      if (rule == GLB_ALL)
      {
        for (k=0; k < in->numofrules; k++)
          if (glbSetBGErrorsInRule(i, k, norm, tilt) != 0)
            return -1;;
      }
      else if (rule >= 0 && rule < in->numofrules)
      {
        if (glbSetBGErrorsInRule(i, rule, norm, tilt) != 0)
          return -1;
      }
      else
        { glb_error("glbSetBGErrors: Invalid rule number"); return -1; }
    }
  }
  else if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule == GLB_ALL)
    {
      for (k=0; k < in->numofrules; k++)
        if (glbSetBGErrorsInRule(exp, k, norm, tilt) != 0)
          return -1;
    }
    else if (rule >= 0 && rule < in->numofrules)
    {
      if (glbSetBGErrorsInRule(exp, rule, norm, tilt) != 0)
        return -1;
    }
    else
      { glb_error("glbSetBGErrors: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetBGErrors: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetBGErrors                                                 *
 ***************************************************************************
 * Writes the background norm and energy errors for a given experiment and *
 * rule into the variables norm and tilt.                                  *
 * Note that this function is only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbGetBGErrors(int exp, int rule, double *norm, double *tilt)
{
  struct glb_experiment *in;

  if (norm == NULL  ||  tilt == NULL)
    { glb_error("glbGetBGErrors: Input pointers may not be NULL"); return -1; }

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
    {
      *norm = in->bg_errors[0][rule];
      *tilt = in->bg_errors[1][rule];
    }
    else
      { glb_error("glbGetBGErrors: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetBGErrors: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetBGCenters                                                *
 ***************************************************************************
 * Sets the background norm and energy central values.                     *
 ***************************************************************************/
int glbSetBGCenters(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;
  int i, k;

  if (exp == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
    {
      in = glb_experiment_list[i];
      if (rule == GLB_ALL)
      {
        for (k=0; k < in->numofrules; k++)
        {
          in->bg_centers[0][k] = norm;
          in->bg_centers[1][k] = tilt;
        }
      }
      else if (rule >= 0 && rule < in->numofrules)
      {
        in->bg_centers[0][rule] = norm;
        in->bg_centers[1][rule] = tilt;
      }
      else
        { glb_error("glbSetBGCenters: Invalid rule number"); return -1; }
    }
  }
  else if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule == GLB_ALL)
    {
      for (k=0; k < in->numofrules; k++)
      {
        in->bg_centers[0][k] = norm;
        in->bg_centers[1][k] = tilt;
      }
    }
    else if (rule >= 0 && rule < in->numofrules)
    {
      in->bg_centers[0][rule] = norm;
      in->bg_centers[1][rule] = tilt;
    }
    else
      { glb_error("glbSetBGCenters: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetBGCenters: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetBGCenters                                                *
 ***************************************************************************
 * Writes the background norm and energy central values for a given        *
 * experiment and rule into the variables norm and tilt.                   *
 ***************************************************************************/
int glbGetBGCenters(int exp, int rule, double *norm, double *tilt)
{
  struct glb_experiment *in;

  if (norm == NULL  ||  tilt == NULL)
    { glb_error("glbGetBGCenters: Input pointers may not be NULL"); return -1; }

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
    {
      *norm = in->bg_centers[0][rule];
      *tilt = in->bg_centers[1][rule];
    }
    else
      { glb_error("glbGetBGCenters: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetBGCenters: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetSysErrorsList                                            *
 ***************************************************************************
 * Sets the systematical error list for a given experiment and rule.       *
 ***************************************************************************/
int glbSetSysErrorsList(int exp, int rule, int on_off, const double *sys_list)
{
  struct glb_experiment *in;
  int i, k;

  if (exp == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
    {
      in = glb_experiment_list[i];
      if (rule == GLB_ALL)
      {
        for (k=0; k < in->numofrules; k++)
          if (glbSetSysErrorsListInRule(i, k, on_off, sys_list) != 0)
            return -1;;
      }
      else if (rule >= 0 && rule < in->numofrules)
      {
        if (glbSetSysErrorsListInRule(i, rule, on_off, sys_list) != 0)
          return -1;
      }
      else
        { glb_error("glbSetSysErrorsList: Invalid rule number"); return -1; }
    }
  }
  else if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule == GLB_ALL)
    {
      for (k=0; k < in->numofrules; k++)
        if (glbSetSysErrorsListInRule(exp, k, on_off, sys_list) != 0)
          return -1;
    }
    else if (rule >= 0 && rule < in->numofrules)
    {
      if (glbSetSysErrorsListInRule(exp, rule, on_off, sys_list) != 0)
        return -1;
    }
    else
      { glb_error("glbSetSysErrorsList: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSysErrorsList: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetSysErrorsListPtr                                         *
 ***************************************************************************
 * Returns a pointer to the systematical error list of a specific          *
 * experiment, rule, and on/off state.                                     *
 ***************************************************************************/
double *glbGetSysErrorsListPtr(int exp, int rule, int on_off)
{
  struct glb_experiment *in;

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
    {
      if (on_off == GLB_ON)
        return in->sys_on_errors[rule];
      else if (on_off == GLB_OFF)
        return in->sys_off_errors[rule];
      else
        { glb_error("glbGetSysErrorsListPtr: on_off must be GLB_ON or GLB_OFF"); return NULL; }
    }
    else
      { glb_error("glbGetSysErrorsListPtr: Invalid rule number"); return NULL; }
  }
  else
    { glb_error("glbGetSysErrorsListPtr: Invalid experiment number"); return NULL; }

  return NULL;
}


/***************************************************************************
 * Function glbSetSignalStartingValues                                     *
 ***************************************************************************
 * Sets the signal norm and energy starting values.                        *
 * Note that these values are only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbSetSignalStartingValues(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;
  int i, k;

  if (exp == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
    {
      in = glb_experiment_list[i];
      if (rule == GLB_ALL)
      {
        for (k=0; k < in->numofrules; k++)
          if (glbSetSignalStartingValuesInRule(i, k, norm, tilt) != 0)
            return -1;;
      }
      else if (rule >= 0 && rule < in->numofrules)
      {
        if (glbSetSignalStartingValuesInRule(i, rule, norm, tilt) != 0)
          return -1;
      }
      else
        { glb_error("glbSetSignalStartingValues: Invalid rule number"); return -1; }
    }
  }
  else if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule == GLB_ALL)
    {
      for (k=0; k < in->numofrules; k++)
        if (glbSetSignalStartingValuesInRule(exp, k, norm, tilt) != 0)
          return -1;
    }
    else if (rule >= 0 && rule < in->numofrules)
    {
      if (glbSetSignalStartingValuesInRule(exp, rule, norm, tilt) != 0)
        return -1;
    }
    else
      { glb_error("glbSetSignalStartingValues: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSignalStartingValues: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetSignalStartingValues                                     *
 ***************************************************************************
 * Writes the signal norm and energy starting values for a given           *
 * experiment and rule into the variables norm and tilt.                   *
 * Note that this function is only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbGetSignalStartingValues(int exp, int rule, double *norm, double *tilt)
{
  struct glb_experiment *in;

  if (norm == NULL  ||  tilt == NULL)
    { glb_error("glbGetSignalStartingValues: Input pointers may not be NULL"); return -1; }

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
    {
      *norm = in->signal_startvals[0][rule];
      *tilt = in->signal_startvals[1][rule];
    }
    else
      { glb_error("glbGetSignalStartingValues: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetSignalStartingValues: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetBGStartingValues                                         *
 ***************************************************************************
 * Sets the background norm and energy starting values.                    *
 * Note that these values are only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbSetBGStartingValues(int exp, int rule, double norm, double tilt)
{
  struct glb_experiment *in;
  int i, k;

  if (exp == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
    {
      in = glb_experiment_list[i];
      if (rule == GLB_ALL)
      {
        for (k=0; k < in->numofrules; k++)
          if (glbSetBGStartingValuesInRule(i, k, norm, tilt) != 0)
            return -1;;
      }
      else if (rule >= 0 && rule < in->numofrules)
      {
        if (glbSetBGStartingValuesInRule(i, rule, norm, tilt) != 0)
          return -1;
      }
      else
        { glb_error("glbSetBGStartingValues: Invalid rule number"); return -1; }
    }
  }
  else if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule == GLB_ALL)
    {
      for (k=0; k < in->numofrules; k++)
        if (glbSetBGStartingValuesInRule(exp, k, norm, tilt) != 0)
          return -1;
    }
    else if (rule >= 0 && rule < in->numofrules)
    {
      if (glbSetBGStartingValuesInRule(exp, rule, norm, tilt) != 0)
        return -1;
    }
    else
      { glb_error("glbSetBGStartingValues: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetBGStartingValues: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetBGStartingValues                                         *
 ***************************************************************************
 * Writes the background norm and energy starting values for a given       *
 * experiment and rule into the variables norm and tilt.                   *
 * Note that this function is only meaningful for the old chi^2 functions  *
 * that are compatible with GLoBES 2.0.11 (i.e. their errorlist looks like *
 * { signal_norm, signal_energy, bg_norm, bg_energy } )                    *
 ***************************************************************************/
int glbGetBGStartingValues(int exp, int rule, double *norm, double *tilt)
{
  struct glb_experiment *in;

  if (norm == NULL  ||  tilt == NULL)
    { glb_error("glbGetBGStartingValues: Input pointers may not be NULL"); return -1; }

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
    {
      *norm = in->bg_startvals[0][rule];
      *tilt = in->bg_startvals[1][rule];
    }
    else
      { glb_error("glbGetBGStartingValues: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbGetBGStartingValues: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbSetSysStartingValuesList                                    *
 ***************************************************************************
 * Sets the systematics starting value list for a given experiment and     *
 * rule.                                                                   *
 ***************************************************************************/
int glbSetSysStartingValuesList(int exp, int rule, int on_off, const double *sys_list)
{
  struct glb_experiment *in;
  int i, k;

  if (exp == GLB_ALL)
  {
    for(i=0; i < glb_num_of_exps; i++)
    {
      in = glb_experiment_list[i];
      if (rule == GLB_ALL)
      {
        for (k=0; k < in->numofrules; k++)
          if (glbSetSysStartingValuesListInRule(i, k, on_off, sys_list) != 0)
            return -1;;
      }
      else if (rule >= 0 && rule < in->numofrules)
      {
        if (glbSetSysStartingValuesListInRule(i, rule, on_off, sys_list) != 0)
          return -1;
      }
      else
        { glb_error("glbSetSysStartingValuesList: Invalid rule number"); return -1; }
    }
  }
  else if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule == GLB_ALL)
    {
      for (k=0; k < in->numofrules; k++)
        if (glbSetSysStartingValuesListInRule(exp, k, on_off, sys_list) != 0)
          return -1;
    }
    else if (rule >= 0 && rule < in->numofrules)
    {
      if (glbSetSysStartingValuesListInRule(exp, rule, on_off, sys_list) != 0)
        return -1;
    }
    else
      { glb_error("glbSetSysStartingValuesList: Invalid rule number"); return -1; }
  }
  else
    { glb_error("glbSetSysStartingValuesList: Invalid experiment number"); return -1; }

  return 0;
}


/***************************************************************************
 * Function glbGetSysStartingValuesListPtr                                 *
 ***************************************************************************
 * Returns a pointer to the systematics starting value list of a specific  *
 * experiment, rule, and on/off state.                                     *
 ***************************************************************************/
double *glbGetSysStartingValuesListPtr(int exp, int rule, int on_off)
{
  struct glb_experiment *in;

  if (exp >= 0  &&  exp < glb_num_of_exps)
  {
    in = glb_experiment_list[exp];
    if (rule >= 0 && rule < in->numofrules)
    {
      if (on_off == GLB_ON)
        return in->sys_on_startvals[rule];
      else if (on_off == GLB_OFF)
        return in->sys_off_startvals[rule];
      else
        { glb_error("glbGetSysStartingValuesListPtr: on_off must be GLB_ON or GLB_OFF");
          return NULL; }
    }
    else
      { glb_error("glbGetSysStartingValuesListPtr: Invalid rule number"); return NULL; }
  }
  else
    { glb_error("glbGetSysStartingValuesListPtr: Invalid experiment number"); return NULL; }

  return NULL;
}


/***************************************************************************
 * Function glbShiftEnergyScale                                            *
 ***************************************************************************
 * Applies an energy calibration error of magnitude b to rates_in and      *
 * stores the result in rates_out.                                         *
 ***************************************************************************/
void glbShiftEnergyScale(double b, double *rates_in, double *rates_out,
                         int n_bins, double emin, double emax)
{
  int i, k;
  double t0 = n_bins * emin / (emax - emin);
  double delta;

  for (i=0; i < n_bins; i++)
  {
    delta = b * (i + t0 + 0.5) + i;
    k     = (int) floor(delta);

    if (k < -1 || k > n_bins - 1)
      rates_out[i] = 0.0;
    else if (k == -1)         /* Assume out-of-bounds bins to contain 0 events */
      rates_out[i] = (1 + b) * rates_in[k+1] * (delta - k);
    else if (k == n_bins - 1)
      rates_out[i] = (1 + b) * (rates_in[k] * (delta - k) + rates_in[k]);
    else
      rates_out[i] = (1 + b) * ((rates_in[k+1] - rates_in[k]) * (delta - k) + rates_in[k]);

/* JK - 2007-06-25
    if (k < 0 || k > n_bins - 1)
      rates_out[i] = 0.0;
    else if (k == n_bins - 1)
      rates_out[i] = (1 + b) * (rates_in[k] * (delta - k) + rates_in[k]);
    else
      rates_out[i] = (1 + b) * ((rates_in[k+1] - rates_in[k]) * (delta - k) + rates_in[k]);
*/
  }
}



