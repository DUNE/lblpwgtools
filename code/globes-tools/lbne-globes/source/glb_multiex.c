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
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <globes/globes.h>

#include "glb_probability.h"
#include "glb_fluxes.h"
#include "glb_rate_engine.h"

#include "glb_error.h"

#include "glb_minimize.h"
#include "glb_smear.h"
#include "glb_types.h"
#include "glb_multiex.h"
#include "glb_version.h"
#include "glb_sys.h"

#define FLOAT double

#define PI 3.1415

/* global variables */
int glb_current_exp = -1;  // Indicates which experiment is currently
                           // being processed by the rate engine
int glb_ignore_invalid_chi2 = 0; /* If != 0, use chiZero instead of throwing an error
                                    if unknown chi^2 function is used. This is necessary
                                    when an AEDL file using user-defined systematics is
                                    run through the standalone globes binary */

static struct glb_experiment MInitMemory0(struct glb_experiment in);

/* Functions for dealing with glb_naming */

static void
glb_free_names(glb_naming *stale)
{
    glb_naming *ptr;
    glb_naming *dummy;
    ptr=stale;
    while(ptr != (glb_naming *) NULL)
      {
        glb_free(ptr->name);
        glb_free(ptr->context);
        dummy=ptr->next;
        glb_free(ptr);
        ptr=dummy;
      }
    stale=NULL;
}



void glbInitExp(glb_exp ins)
{
  int i;
  struct glb_experiment *in;
  in=(struct glb_experiment *) ins;
  in->version=NULL;
  in->citation=NULL;
  in->filename=NULL;
  in->names=NULL;
  in->num_of_fluxes=-1;
  in->density_profile_type=-1;
  /* FIXME - a potential memory leak */
  for(i=0;i<32;i++) in->fluxes[i]=NULL;
  in->num_of_xsecs=-1;
  /* FIXME - a potential memory leak */
  for(i=0;i<32;i++) in->xsecs[i]=NULL;

  in->binsize=NULL;
  in->simbinsize=NULL;
  for(i=0;i<32;i++) in->sys_on[i]=NULL;
  for(i=0;i<32;i++) in->sys_off[i]=NULL;
  for(i=0;i<32;i++) in->sys_on_strings[i]=NULL;
  for(i=0;i<32;i++) in->sys_off_strings[i]=NULL;
  for(i=0;i<32;i++) in->sys_on_off[i]=GLB_ON;
  in->baseline=-1;
  in->emin=-1;
  in->emax=-1;
  in->numofbins=-1;
  in->targetmass=-1;

  in->numofchannels=-1;
  for(i=0;i<6;i++) in->listofchannels[i]=NULL;
  in->numofrules=-1;
  in->num_of_sm=-1;
  for(i=0;i<32;i++) in->lengthofrules[i]=-1;
  for(i=0;i<32;i++) in->rulescoeff[i]=NULL;
  for(i=0;i<32;i++) in->rulechannellist[i]=NULL;
  for(i=0;i<32;i++) in->lengthofbgrules[i]=-1;
  for(i=0;i<32;i++) in->bgrulescoeff[i]=NULL;
  for(i=0;i<32;i++) in->bgrulechannellist[i]=NULL;
  for(i=0;i<32;i++) in->bg_centers[0][i]=1.0;
  for(i=0;i<32;i++) in->bg_centers[1][i]=0.0;
  for(i=0;i<32;i++) in->signal_errors[0][i]=-1;
  for(i=0;i<32;i++) in->signal_errors[1][i]=-1;
  for(i=0;i<32;i++) in->signal_startvals[0][i]=0.0;
  for(i=0;i<32;i++) in->signal_startvals[1][i]=0.0;
  for(i=0;i<32;i++) in->bg_errors[0][i]=-1;
  for(i=0;i<32;i++) in->bg_errors[1][i]=-1;
  for(i=0;i<32;i++) in->bg_startvals[0][i]=0.0;
  for(i=0;i<32;i++) in->bg_startvals[1][i]=0.0;
  for(i=0;i<32;i++) in->sys_on_errors[i]=NULL;
  for(i=0;i<32;i++) in->sys_on_startvals[i]=NULL;
  for(i=0;i<32;i++) in->sys_off_errors[i]=NULL;
  for(i=0;i<32;i++) in->sys_off_startvals[i]=NULL;
  for(i=0;i<32;i++) {in->smear_data[i]=NULL;}
  for(i=0;i<32;i++) in->smear[i]=NULL;
  for(i=0;i<32;i++) in->lowrange[i]=NULL;
  for(i=0;i<32;i++) in->uprange[i]=NULL;
  for(i=0;i<32;i++) in->chrb_0[i]=NULL;
  for(i=0;i<32;i++) in->chrb_1[i]=NULL;
  for(i=0;i<32;i++) in->chra_0[i]=NULL;
  for(i=0;i<32;i++) in->chra_1[i]=NULL;
  for(i=0;i<32;i++) in->chr_template[i]=NULL;
  in->simtresh=-1;
  in->simbeam=-1;
  in->simbins=-1;

  in->filter_state=-1;
  in->filter_value=-1;
  in->psteps=-1;
  in->lengthtab=NULL;
  in->densitytab=NULL;
  in->densitybuffer=NULL;
  for(i=0;i<32;i++) in->user_pre_smearing_channel[i]=NULL;
  for(i=0;i<32;i++) in->user_post_smearing_channel[i]=NULL;
  for(i=0;i<32;i++) in->user_pre_smearing_background[i]=NULL;
  for(i=0;i<32;i++) in->user_post_smearing_background[i]=NULL;
  for(i=0;i<32;i++) in->energy_window[i][0]=-1;
  for(i=0;i<32;i++) in->energy_window[i][1]=-1;
  for(i=0;i<32;i++) in->energy_window_bins[i][0]=-1;
  for(i=0;i<32;i++) in->energy_window_bins[i][1]=-1;
  for(i=0;i<32;i++)
    {
      in->SignalRates[i]=NULL;
      in->BackgroundRates[i]=NULL;
      in->rates0[i]=NULL;
      in->rates1[i]=NULL;
      in->rates1BG[i]=NULL;
    }
  in->energy_tab=NULL;
  for(i=0;i<32;i++) in->no_osc_background[i]=NULL;
}


static void my_free(void *ptr)
{
  if(ptr==NULL) return;
  glb_free(ptr);
  ptr=NULL;
}

void glbFreeExp(glb_exp ins)
{
  int i,j;
  struct glb_experiment *in;
  in=(struct glb_experiment *) ins;
  if(ins==NULL) return;
  glb_free_names(in->names);
  glb_free(in->version);
  glb_free(in->citation);
  glb_free(in->filename);
  for(i=0;i<32;i++) { glb_free_flux(in->fluxes[i]); in->fluxes[i]=NULL; }
  for(i=0;i<32;i++) { glb_free_xsec(in->xsecs[i]);  in->xsecs[i]=NULL;  }

  for(i=0;i<6;i++) my_free(in->listofchannels[i]);

  my_free(in->binsize);
  in->binsize=NULL;
  my_free(in->simbinsize);
  in->simbinsize=NULL;

  for(i=0;i<in->numofrules;i++)
    {
      my_free(in->rulescoeff[i]);
      my_free(in->rulechannellist[i]);
      my_free(in->bgrulescoeff[i]);
      my_free(in->bgrulechannellist[i]);
    }

  for(i=0;i<in->numofrules;i++)
  {
    my_free(in->sys_on_strings[i]);
    my_free(in->sys_off_strings[i]);
  }


  for(i=0;i<32;i++) glb_smear_free(in->smear_data[i]);
  for(i=0;i<in->num_of_sm;i++)
    {
      my_free(in->lowrange[i]);
      my_free(in->uprange[i]);
      if (in->smear != NULL  &&  in->smear[i] != NULL)
      {
        for(j=0;j<in->numofbins;j++)
          my_free(in->smear[i][j]);
        my_free(in->smear[i]);
      }
    }
  for(i=0;i<in->numofchannels;i++)
    {
      my_free(in->chr_template[i]);
      my_free(in->chrb_0[i]);
      my_free(in->chrb_1[i]);
      my_free(in->chra_0[i]);
      my_free(in->chra_1[i]);
      my_free(in->user_pre_smearing_channel[i]);
      my_free(in->user_post_smearing_channel[i]);
      my_free(in->user_pre_smearing_background[i]);
      my_free(in->user_post_smearing_background[i]);
    }
  my_free(in->lengthtab);
  my_free(in->densitytab);
  my_free(in->densitybuffer);

  for(i=0;i<in->numofrules;i++)
    {
      my_free(in->SignalRates[i]);
      my_free(in->BackgroundRates[i]);
      my_free(in->rates0[i]);
      my_free(in->rates1[i]);
      my_free(in->rates1BG[i]);
      my_free(in->sys_on_errors[i]);
      my_free(in->sys_on_startvals[i]);
      my_free(in->sys_off_errors[i]);
      my_free(in->sys_off_startvals[i]);
    }
  my_free(in->energy_tab);
  my_free(in);

}







glb_exp glbAllocExp()
{
  struct glb_experiment *in;
  in=(struct glb_experiment *) glb_malloc(sizeof(struct glb_experiment));
  glbInitExp((glb_exp) in);
  return (glb_exp) in;
}


static int setup_density_profile(glb_exp ins)
{
  int s=0;
  double *lb,*db;
  size_t l;
  double ttl=0;
  int i;
  struct glb_experiment *in;
  in=(struct glb_experiment *) ins;
  if(ins->density_profile_type==-1)
    {glb_exp_error(in, "No profile type specified");s=-1;}
  if(ins->density_profile_type==1)
    {
      glb_free(ins->lengthtab);
      glb_free(ins->densitytab);
      ins->psteps=1;
      if(ins->baseline > 0)
        {
          s+=glbAverageDensityProfile(ins->baseline,&lb,&db);
          ins->densitytab=db;
          ins->lengthtab=lb;
        }
      else
        {
          glb_exp_error(in, "Baseline must be a positive number");
          s=-1;
        }
    }

  if(ins->density_profile_type==2)
    {
      glb_free(ins->lengthtab);
      glb_free(ins->densitytab);

      if(ins->baseline > 0)
        {
          if(ins->psteps > 0)
            {
              l=(size_t) ins->psteps;
              s+=glbStaceyProfile(ins->baseline,l,&lb,&db);
              ins->densitytab=db;
              ins->lengthtab=lb;
            }
          else
            {
              glb_exp_error(in, "Densitysteps must be a positive number");
              s=-1;
            }

        }
      else
        {
          glb_exp_error(in, "Baseline must be a positive number");
          s=-1;
        }
    }

  if(ins->density_profile_type==3)
    {
      if(ins->lengthtab!=NULL && ins->psteps > 0)
      {
        for(i=0;i<ins->psteps;i++) ttl+=ins->lengthtab[i];
      }
      if(ttl!=ins->baseline&&ins->baseline!=-1)
        glb_warning("Override baseline with sum of lengthtab");
      ins->baseline=ttl;
    }

  return s;
}

int glbDefaultExp(glb_exp ins)
{
  int i,k,status,def,ct;
  int sys_dim;


  struct glb_experiment *in;
  in=(struct glb_experiment *) ins;
  double *tmp_errorlist;
  double tmp;

  status=0;
  def=0;

  status+=setup_density_profile(ins);
  if(in->version==NULL)
    {
      glb_fatal("Missing version in AEDL file");
      def=-1;
      in->version=(char *) strdup(glb_release_version);
    }

  if(glbTestReleaseVersion(in->version)<0) {
    glb_error("AEDL file has a more recent version number than the"
                " installed globes package");}

  if(in->num_of_xsecs<1)  {glb_exp_error(in, "No X-section selected!");status=-1;}
  if(in->num_of_xsecs>31)  {glb_exp_error(in, "To many X-sections!");status=-1;}
  if(in->num_of_fluxes<1)  {glb_exp_error(in, "No flux selected!");status=-1;}
  if(in->num_of_fluxes>31)  {glb_exp_error(in, "To many fluxes!");status=-1;}

  /* Initialize flux tables */
  if(in->num_of_fluxes>0&&in->num_of_fluxes<32)
  {
    for(i=0;i<in->num_of_fluxes;i++)
    {
      if(in->fluxes[i] == NULL)
      {
        glb_exp_error(in, "Missing flux specification");
        status = -1;
      }
      else
      {
        if (glb_init_flux(in->fluxes[i]) != GLB_SUCCESS)
          status=-1;
      }
    }
  }

  /* Load cross sections */
  if(in->num_of_xsecs > 0 && in->num_of_xsecs < 32)
  {
    for(i=0;i < in->num_of_xsecs; i++)
    {
      if(in->xsecs[i] == NULL)
      {
        glb_exp_error(in, "Missing cross section specification");
        status = -1;
      }
      else
      {
        if (in->xsecs[i]->builtin >= 0)
        {
          glb_exp_error(in, "No builtin cross sections available. "
                            "Please specify cross section file");
          status = -1;
        }
        else
          glb_init_xsec(in->xsecs[i]);
      }
    }
  }

  /* Initialization of systematics data */
  /* ---------------------------------- */
  int old_verbosity = glbGetVerbosityLevel();
  int old_status    = status;
  if (glb_ignore_invalid_chi2)
    glbSetVerbosityLevel(0);

  /* Definitions for systematics ON */
  for (i=0; i < in->numofrules; i++)
  {
    if (in->sys_on_strings[i] == NULL)
      { glb_rule_error(in, i, "No chi^2 function specified"); status=-1; }
    else
    {
      tmp_errorlist = in->sys_on_errors[i];
      in->sys_on_errors[i] = NULL;

      /* Check length of error list and make sure that all sigma are > 0.0 */
      sys_dim = glbGetSysDim(in->sys_on_strings[i]);
      if (sys_dim >= 0)
      {
        if (tmp_errorlist != NULL)
        {
          for (k=0; tmp_errorlist[k] > 0.0; k++)
            ;
          if (k != sys_dim)
          {
            glb_rule_error(in, i, "Invalid systematical error list @sys_on_errors");
            status=-1;
          }
        }
      }
      else
        { glb_rule_error(in, i, "Invalid @sys_on_function/@sys_on_errors"); status=-1; }

      if (glbSetChiFunctionInExperiment(in, i, GLB_ON, in->sys_on_strings[i],
                                        tmp_errorlist) != 0)
        { glb_rule_error(in, i, "Invalid @sys_on_function/@sys_on_errors"); status=-1; }

      glb_free(tmp_errorlist);
    }

    if (glb_ignore_invalid_chi2 && status != 0)
    {
      if (glbSetChiFunctionInExperiment(in, i, GLB_ON, "chiZero", NULL) == 0)
        status = old_status;
    }


    /* Treatment of parameters for systematics OFF is equivalent to systematics ON */
    if (in->sys_off_strings[i] == NULL)
      { glb_rule_error(in, i, "No chi^2 function specified"); status=-1; }
    else
    {
      tmp_errorlist = in->sys_off_errors[i];
      in->sys_off_errors[i] = NULL;

      /* Check length of error list and make sure that all sigma are > 0.0 */
      sys_dim = glbGetSysDim(in->sys_off_strings[i]);
      if (sys_dim >= 0)
      {
        if (tmp_errorlist != NULL)
        {
          for (k=0; tmp_errorlist[k] > 0.0; k++)
            ;
          if (k != sys_dim)
          {
            glb_rule_error(in, i, "Invalid systematical error list @sys_off_errors");
            status=-1;
          }
        }
      }
      else
        { glb_rule_error(in, i, "Invalid @sys_off_function/@sys_off_errors"); status=-1; }

      if (glbSetChiFunctionInExperiment(in, i, GLB_OFF, in->sys_off_strings[i],
                                        tmp_errorlist) != 0)
        { glb_rule_error(in, i, "Invalid @sys_off_function/@sys_off_errors"); status=-1; }

      glb_free(tmp_errorlist);
    }

    if (glb_ignore_invalid_chi2 && status != 0)
    {
      if (glbSetChiFunctionInExperiment(in, i, GLB_OFF, "chiZero", NULL) == 0)
        status = old_status;
    }
  }

  if (glb_ignore_invalid_chi2)
    glbSetVerbosityLevel(old_verbosity);

  if(in->baseline==-1){glb_exp_error(in, "No baseline specified!");status=-1;}
  if(in->emin==-1){glb_exp_error(in, "No emin specified!");status=-1;}
  if(in->emax==-1){glb_exp_error(in, "No emax specified!");status=-1;}
  if(in->emin>in->emax){glb_exp_error(in, "emin must be less than emax!");status=-1;}
  if(in->numofbins<=0){glb_exp_error(in, "numofbins is not set!");status=-1;}
  if(in->numofbins<=0) { glb_exp_error(in, "Too few bins defined!");status=-1;}


  if(in->targetmass==-1){glb_exp_error(in, "No targetmass specified!");status=-1;}

  if(in->numofchannels==-1){
    glb_exp_error(in, "numofchannels not specified!");status=-1;}

  for(i=0;i<6;i++){
    if(in->listofchannels[i]==NULL)
      {  glb_exp_error(in, "listofchannels not specified!"); status=-1;}
  }
  if(in->numofrules==-1){glb_exp_error(in, "numofrules not specified!");status=-1;}
  for(i=0;i<in->numofrules;i++)
    {
      if(in->lengthofrules[i]==-1)
        { glb_rule_error(in, i, "No targetmass specified!"); status=-1; }
      if(in->rulescoeff[i]==NULL)
        { glb_rule_error(in, i, "No rulescoeff specified!"); status=-1; }
      if(in->rulechannellist[i]==NULL)
        { glb_rule_error(in, i, "No rulechannellist specified!"); status=-1; }
      if(in->lengthofbgrules[i]==-1)
        { glb_rule_error(in, i, "No lengthofbgrules specified!"); status=-1; }
      if(in->bgrulescoeff[i]==NULL)
        {  glb_rule_error(in, i, "No bgrulescoeff specified!"); status=-1; }
      if(in->bgrulechannellist[i]==NULL)
        { glb_rule_error(in, i, "No bgruloechannellist specified!"); status=-1; }
    }


  if(in->filter_state==-1){in->filter_state=1;def=-1;}
  if(in->filter_value==-1){in->filter_value=0;def=-1;}


  if(in->num_of_sm==-1){
    glb_exp_error(in, "No smearing data specified!");status=-1;}

  /* Energy resolution/detector response function (``smearing'') */
  for(i=0;i<in->num_of_sm;i++)
  {
    /* Smearing matrix explicitly given in AEDL file? */
    if(in->smear[i] != NULL)
    {
      glb_set_up_smear_data(in->smear_data[i],in);
      glb_default_smear(in->smear_data[i],in);
    }

    /* If not, generate it according to the parameters from the AEDL file */
    else
    {
      /* Right now we do not support different binnings
       * in the reconstructed energy */
      in->smear_data[i]->numofbins=in->numofbins;
      in->smear_data[i]->e_min=in->emin;
      in->smear_data[i]->e_max=in->emax;

      if(glb_default_smear(in->smear_data[i],in)==1) def=-1;

      glb_compute_smearing_matrix(&in->smear[i],
                            &in->lowrange[i],&in->uprange[i],
                            in->smear_data[i],in);

      in->simtresh=in->smear_data[i]->e_sim_min;
      in->simbeam=in->smear_data[i]->e_sim_max;
      in->simbins=in->smear_data[i]->simbins;
    }

    if(in->smear[i]==NULL){glb_exp_error(in, "No smear matrix defined!"); status=-1;}
    if(in->lowrange[i]==NULL){glb_exp_error(in, "No lowrange defined!");status=-1;}
    if(in->uprange[i]==NULL){glb_exp_error(in, "No uprange defined!");status=-1;}
  }

  if(in->simtresh==-1){glb_exp_error(in, "No simtresh defined!");status=-1;}
  if(in->simbeam==-1){glb_exp_error(in, "No simbeam defined!");status=-1;}
  if(in->simbins==-1){glb_exp_error(in, "No simbins defined!");status=-1;}
  if(in->simbins<in->numofbins){glb_exp_error(in, "Less sampling points than bins");
                                status=-1;}

//---------------------------------------------------------

  *in=MInitMemory0(*in);

//-------------------------------------------------------


  if(in->psteps==-1){glb_exp_error(in, "psteps not defined!");status=-1;}
  if(in->lengthtab==NULL){glb_exp_error(in, "lengthtab not allocated!");status=-1;}
  if(in->densitytab==NULL){glb_exp_error(in, "densitytab not allocated!");status=-1;}
  if(in->densitybuffer==NULL){glb_exp_error(in, "densitybuffer not allocated!");
  status=-1;}
  // here we use lot of defaults -- and its not that clear how many
  // there are numofrules ? numofchannels ?
  for(i=0;i<in->numofchannels;i++)
    {
      if(in->simbins<=0) glb_exp_error(in, "Too few simbins defined!");
      else
        {

          if(in->user_pre_smearing_channel[i]==NULL)
            {
              in->user_pre_smearing_channel[i]=(double*)
                glb_malloc(sizeof(double)* in->simbins);
              for(k=0;k<in->simbins;k++)
                in->user_pre_smearing_channel[i][k]=1;
              def=-1;
            }
          else
            {
              for(ct=0;in->user_pre_smearing_channel[i][ct]!=-1;ct++) ct=ct;
              if(ct!=in->simbins)
              {
                glb_exp_error(in, "user_pre_smearing_channel has not simbins elements");
                status=-1;
              }
            }

          if(in->user_pre_smearing_background[i]==NULL)
            {
              in->user_pre_smearing_background[i]=(double*)
                glb_malloc(sizeof(double)*in->simbins);
              for(k=0;k<in->simbins;k++)
                {in->user_pre_smearing_background[i][k]=0;}
              def=-1;
            }
          else
            {
              for(ct=0;in->user_pre_smearing_background[i][ct]!=-1;ct++) ct=ct;
              if(ct!=in->simbins)
              {
                glb_exp_error(in, "user_pre_smearing_background has not simbins elements");
                status=-1;
              }
            }

          if(in->user_post_smearing_channel[i]==NULL)
            {
              in->user_post_smearing_channel[i]=(double*)
                glb_malloc(sizeof(double)*in->numofbins);
              for(k=0;k<in->numofbins;k++)
                in->user_post_smearing_channel[i][k]=1;
              def=-1;
            }
          else
            {
              for(ct=0;in->user_post_smearing_channel[i][ct]!=-1;ct++) ct=ct;
              if(ct!=in->numofbins)
              {
                glb_exp_error(in, "user_post_smearing_channel has not numofbins elements");
                status=-1;
              }
            }
          if(in->user_post_smearing_background[i]==NULL)
            {
              in->user_post_smearing_background[i]=(double*)
                glb_malloc(sizeof(double)*in->numofbins);
              for(k=0;k<in->numofbins;k++)
                {in->user_post_smearing_background[i][k]=0;}
              def=-1;
            }
          else
            {
              for(ct=0;in->user_post_smearing_background[i][ct]!=-1;ct++)
                ct=ct;
              if(ct!=in->numofbins)
              {
                glb_exp_error(in, "user_post_smearing_background has not numofbins elements");
                status=-1;
              }
            }
        }
    }

    for(i=0;i<in->numofrules;i++)
    {
      if (in->energy_window[i][0] == -1  ||  in->energy_window[i][0] < in->emin)
      {
        in->energy_window[i][0] = in->emin;
        def=-1;
      }
      if (in->energy_window[i][1] == -1  ||  in->energy_window[i][1] > in->emax)
      {
        in->energy_window[i][1]=in->emax;
        def=-1;
      }

      if (in->energy_window[i][0] > in->energy_window[i][1])
      {
        tmp                     = in->energy_window[i][1];
        in->energy_window[i][1] = in->energy_window[i][0];
        in->energy_window[i][0] = tmp;
      }

      /* Calculate bin ranges corresponding to the energy window,
       * trying to reproduce the behaviour of the old glb_window_function */
      k = 0;
      while (in->smear_data[0]->bincenter[k] <= in->energy_window[i][0])
        k++;
      in->energy_window_bins[i][0] = k;
      while (k < in->numofbins && in->smear_data[0]->bincenter[k] < in->energy_window[i][1])
        k++;
      in->energy_window_bins[i][1] = k-1;
    }
  for(i=0;i<in->numofrules;i++)
    {
      if(in->SignalRates[i]==NULL||
         in->BackgroundRates[i]==NULL||
         in->rates0[i]==NULL||
         in->rates1[i]==NULL||
         in->rates1BG[i]==NULL)
      {
        glb_rule_error(in, i, "No memory for ratevectors allocated!");
        status=-1;
      }
    }

  for(i=0; i < in->numofchannels; i++)
  {
    if(in->chrb_0[i]==NULL || in->chrb_1[i]==NULL ||
       in->chra_0[i]==NULL || in->chra_1[i]==NULL || in->chr_template[i]==NULL)
    {
      glb_exp_error(in, "No memory for convolution allocated!");
      status=-1;
    }
  }

  if(in->energy_tab==NULL){glb_exp_error(in, "energy_tab not allocated!");
  status=-1;}


  // okay thats missing
  /*
    for(i=0;i<32;i++) in->no_osc_background[i]=NULL;
    for(i=0;i<32;i++) init_struct_systematic(&(in->sys[i]));
  */
  // issue a final report
  if(status!=0) glb_fatal("Incompletely defined experiment!");
  if(def!=0) glb_warning("Incompletely defined experiment!"
                         " Defaults are going to be used!");
  return def+status;
};




//#include "StructExp.c"




// function setting glb_num_of_exps

static void glbSetNumberOfExperiments(int in)
{
  glb_num_of_exps = in;
  return;
}


// this function moves the pointers to the ratevectors to the correct address
// allocated with MInitFile().
// this is crucial for multiple experiment support
// without reprogramming everything !
// here we pass a pointer for efficiency since this function is used in every step

static void MMovePointers(struct glb_experiment *in)
{
  int k;
  for (k=0;k<in->numofrules;k++)
    {
      glb_calc_rates_0[k] = in->rates0[k];
      glb_calc_rates_1[k] = in->rates1[k];
      glb_calc_rates_1BG[k] = in->rates1BG[k];
      glb_calc_signal_rates[k] = in->SignalRates[k];
      glb_calc_bg_rates[k] = in->BackgroundRates[k];
      /* FIXME -- wrong number */
    }
  for(k=0;k<in->num_of_sm;k++) glb_calc_smear_data[k]=in->smear_data[k];
  for(k=0;k<in->numofchannels;k++)
    {
      glb_calc_chrb_0[k] = in->chrb_0[k];
      glb_calc_chrb_1[k] = in->chrb_1[k];
      glb_calc_chra_0[k] = in->chra_0[k];
      glb_calc_chra_1[k] = in->chra_1[k];
      glb_calc_chr_template[k] = in->chr_template[k];
      glb_calc_user_pre_sm_channel[k]=in->user_pre_smearing_channel[k];
      glb_calc_user_post_sm_channel[k]= in->user_post_smearing_channel[k];
      glb_calc_user_pre_sm_background[k]=in->user_pre_smearing_background[k];
      glb_calc_user_post_sm_background[k]=in-> user_post_smearing_background[k];
    }

  for(k=0;k<in->num_of_fluxes;k++) glb_calc_fluxes[k]=in->fluxes[k];
  for(k=0;k<in->num_of_xsecs;k++) glb_calc_xsecs[k]=in->xsecs[k];
  glb_calc_energy_tab = in->energy_tab;
  return;
}

// memory allocation for all the ratevecvtors

static struct glb_experiment MInitMemory0(struct glb_experiment in)
{
 struct glb_experiment out;
  int k;
  int len,l2;
  out=in;
  len=out.numofbins;
  if(out.numofbins>=out.simbins) l2=out.numofbins;
  else l2=out.simbins;
  if(len<=0)
    {glb_exp_error(&in, "Too few bins defined!");}
  else
    {
      for (k=0;k<out.numofrules;k++)
        {
          out.rates0[k] =  (double*) glb_malloc( len*sizeof(double));
          out.rates1[k] = (double*) glb_malloc( len*sizeof(double));
          out.rates1BG[k] = (double*) glb_malloc( len*sizeof(double));
          if(out.simbins<=0) glb_exp_error(&in, "Too few simbins defined!");
          else
            {
              out.SignalRates[k] = (double*) glb_malloc(len*sizeof(double));
              out.BackgroundRates[k] = (double*) glb_malloc(len*sizeof(double));
            }
        }
      out.energy_tab= (double*) glb_malloc(len*sizeof(double));
    }
  for(k=0;k<out.numofchannels;k++)
    {
      if (out.simbins <= 0)
        glb_exp_error(&in, "Too few simbins defined!");
      else
      {
        out.chrb_0[k] = (double*) glb_malloc(out.simbins*sizeof(double));
        out.chrb_1[k] = (double*) glb_malloc(out.simbins*sizeof(double));
        out.chr_template[k] = (double*) glb_malloc(out.simbins*sizeof(double));
      }
      if (out.numofbins <= 0)
        glb_exp_error(&in, "Too few bins defined!");
      else
      {
        out.chra_0[k] = (double*) glb_malloc(out.numofbins*sizeof(double));
        out.chra_1[k] = (double*) glb_malloc(out.numofbins*sizeof(double));
      }
    }
  if(out.psteps<=0) glb_exp_error(&in, "Too few density steps defined!");
  else if (out.densitybuffer==NULL) out.densitybuffer=(double*)
                                      glb_malloc(out.psteps*sizeof(double));
  return out;
}






static void glb_set_profile_scaling_sec(struct glb_experiment *in)
{
  int k;
//  glb_set_length_ptr(in->lengthtab);
//  glb_set_psteps(in->psteps);

  for(k=0;k<in->psteps;k++)
    in->densitybuffer[k]=(double) 1.0*in->densitytab[k];
//  glb_set_density_ptr(in->densitybuffer);

}


// This function actually sets all switches and parameters to the values specified
// in an struct glb_experiment object and calls MMovePointers().
// here we pass a pointer for efficiency since this function is used in every step

void glbSetExperiment(glb_exp in)
{
  int i;
  struct glb_experiment *s;
  s=(struct glb_experiment *) in;

  glb_rule_number=0;
  glb_current_exp = -1;              // Find experiment number
  for (i=0; i < glb_num_of_exps; i++)
    if (glb_experiment_list[i] == s)
    {
      glb_current_exp = i;
      break;
    }


  // pointer moving for density profile
  glb_set_baseline(s->baseline);
  //density=in->densitytab;
  glb_set_profile_scaling_sec(s);
  //analog of SetEnergy

  glb_set_energy_treshold(s->emin);
  glb_set_max_energy(s->emax);
  glb_set_number_of_bins(s->numofbins);


  //analog of SetLuminosity

  glb_set_target_mass(s->targetmass);


  glb_set_num_of_channels(s->numofchannels);
  for (i=0;i<s->numofchannels;i++) glb_set_channel(i,s->listofchannels[0][i],
                                               s->listofchannels[1][i],
                                               s->listofchannels[2][i],
                                               s->listofchannels[3][i],
                                               s->listofchannels[4][i],
                                               s->listofchannels[5][i]
                                               );

  // analog of SetRules


  glb_set_number_of_rules(s->numofrules);
  for (i=0;i<s->numofrules;i++)
    {
      glb_set_rule(i,s->lengthofrules[i],
                   (int*) s->rulechannellist[i],s->rulescoeff[i]);
      glb_set_bg_rule(i,s->lengthofbgrules[i],
                              s->bgrulechannellist[i],s->bgrulescoeff[i]);
      glb_set_bg_center(i,(double) s->bg_centers[0][i],(double) s->bg_centers[1][i]);

      // this is preliminary (0 should be replaced by i)
      glb_calc_simbins=s->simbins;
      glb_calc_simtresh=s->simtresh;
      glb_calc_simbeam=s->simbeam;
    }

  for(i=0;i<s->num_of_sm;i++)
    {
      glb_calc_smearing[i]=s->smear[i];
      glb_calc_uprange[i]=s->uprange[i];
      glb_calc_lowrange[i]=s->lowrange[i];
    }




//  glb_switch_filter(s->filter_state);
//  glb_set_filter(s->filter_value);



  MMovePointers(s);

  return;
}

// redefinition of SetRates()

void glbSetRates()
{
  int i;
  for (i=0;i<glb_num_of_exps;i++)
    {
      glbSetExperiment(glb_experiment_list[i]);
      glb_set_rates();
    }
}

// redefinition of SetNewRates()

void glbSetNewRates()
{
  int i;
  for (i=0;i<glb_num_of_exps;i++)
    {
      glbSetExperiment(glb_experiment_list[i]);
      glb_set_new_rates();
    }
}

// a bunch of comands which allow to modify
// certain elements of a struct glb_experiment object
// in glb_experiment_list


void glb_set_profile_scaling(double scale,int i)
{
  int k;
//  glb_set_length_ptr(glb_experiment_list[i]->lengthtab);

//  glb_set_psteps(glb_experiment_list[i]->psteps);
  for(k=0;k<glb_experiment_list[i]->psteps;k++)
    {
      glb_experiment_list[i]->densitybuffer[k]=(double)
        scale*glb_experiment_list[i]->densitytab[k];
    }
//  glb_set_density_ptr(glb_experiment_list[i]->densitybuffer);

}


