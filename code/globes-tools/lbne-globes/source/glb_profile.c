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
#include <math.h>
#include <globes/globes.h>

#include "glb_types.h"
#include "glb_error.h"
#include "glb_multiex.h"
#include "glb_probability.h"




/* -----------------------------------------------------------
 * Stacey matter profile data
 * -----------------------------------------------------------
 */
static const double stacey[501] = {
 2.8, 2.92, 3.05, 3.18, 3.30, 3.30, 3.31, 3.32, 3.32, 3.33, 3.34, 3.35, 3.35,
3.36, 3.37, 3.38, 3.38, 3.39, 3.40, 3.40, 3.41, 3.42, 3.43, 3.43, 3.44, 3.45,
3.46, 3.46, 3.47, 3.48, 3.48, 3.49, 3.73, 3.81, 3.82, 3.84, 3.86, 3.87, 3.89,
3.90, 3.92, 3.94, 3.95, 3.97, 3.98, 4.00, 4.02, 4.03, 4.05, 4.06, 4.08, 4.09,
4.40, 4.40, 4.41, 4.42, 4.42, 4.43, 4.44, 4.45, 4.45, 4.46, 4.47, 4.47, 4.48,
4.49, 4.49, 4.50, 4.51, 4.52, 4.52, 4.53, 4.54, 4.54, 4.55, 4.56, 4.56, 4.57,
4.58, 4.58, 4.59, 4.60, 4.61, 4.61, 4.62, 4.63, 4.63, 4.64, 4.65, 4.65, 4.66,
4.67, 4.68, 4.68, 4.69, 4.70, 4.70, 4.71, 4.72, 4.72, 4.73, 4.74, 4.75, 4.75,
4.76, 4.77, 4.77, 4.78, 4.79, 4.79, 4.80, 4.81, 4.82, 4.82, 4.83, 4.84, 4.84,
4.85, 4.86, 4.86, 4.87, 4.88, 4.89, 4.89, 4.90, 4.91, 4.91, 4.92, 4.93, 4.93,
4.94, 4.95, 4.95, 4.96, 4.97, 4.98, 4.98, 4.99, 5.00, 5.00, 5.01, 5.02, 5.02,
5.03, 5.04, 5.05, 5.05, 5.06, 5.07, 5.07, 5.08, 5.09, 5.09, 5.10, 5.11, 5.12,
5.12, 5.13, 5.14, 5.14, 5.15, 5.16, 5.16, 5.17, 5.18, 5.19, 5.19, 5.20, 5.21,
5.21, 5.22, 5.23, 5.23, 5.24, 5.25, 5.26, 5.26, 5.27, 5.28, 5.28, 5.29, 5.30,
5.30, 5.31, 5.32, 5.33, 5.33, 5.34, 5.35, 5.35, 5.36, 5.37, 5.37, 5.38, 5.39,
5.39, 5.40, 5.41, 5.42, 5.42, 5.43, 5.44, 5.44, 5.45, 5.46, 5.46, 5.47, 5.48,
5.49, 5.49, 5.50, 5.51, 5.51, 5.52, 5.53, 5.53, 5.54, 5.55, 5.56, 5.56, 5.57,
5.58, 5.58, 5.59, 7.40, 9.91, 9.92, 9.94, 9.96, 9.98, 10.0, 10.0, 10.0, 10.0,
10.0, 10.0, 10.1, 10.1, 10.1, 10.1, 10.1, 10.2, 10.2, 10.2, 10.2, 10.2, 10.2,
10.3, 10.3, 10.3, 10.3, 10.3, 10.4, 10.4, 10.4, 10.4, 10.4, 10.5, 10.5, 10.5,
10.5, 10.5, 10.5, 10.6, 10.6, 10.6, 10.6, 10.6, 10.7, 10.7, 10.7, 10.7, 10.7,
10.7, 10.8, 10.8, 10.8, 10.8, 10.8, 10.9, 10.9, 10.9, 10.9, 10.9, 10.9, 10.9,
10.9, 10.9, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.1, 11.1,
11.1, 11.1, 11.1, 11.1, 11.1, 11.1, 11.1, 11.2, 11.2, 11.2, 11.2, 11.2, 11.2,
11.2, 11.2, 11.2, 11.3, 11.3, 11.3, 11.3, 11.3, 11.3, 11.3, 11.3, 11.3, 11.4,
11.4, 11.4, 11.4, 11.4, 11.4, 11.4, 11.4, 11.4, 11.5, 11.5, 11.5, 11.5, 11.5,
11.5, 11.5, 11.5, 11.5, 11.6, 11.6, 11.6, 11.6, 11.6, 11.6, 11.6, 11.6, 11.6,
11.6, 11.6, 11.6, 11.7, 11.7, 11.7, 11.7, 11.7, 11.7, 11.7, 11.7, 11.7, 11.7,
11.7, 11.7, 11.7, 11.8, 11.8, 11.8, 11.8, 11.8, 11.8, 11.8, 11.8, 11.8, 11.8,
11.8, 11.8, 11.8, 11.9, 11.9, 11.9, 11.9, 11.9, 11.9, 11.9, 11.9, 11.9, 11.9,
11.9, 11.9, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0,
12.0, 12.0, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7, 12.7,
12.7, 12.7, 12.7, 12.7, 12.7, 12.8, 12.8, 12.8, 12.8, 12.8, 12.8, 12.8, 12.8,
12.8, 12.8, 12.8, 12.8, 12.8, 12.8, 12.8, 12.9, 12.9, 12.9, 12.9, 12.9, 12.9,
12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 12.9, 13.0, 13.0, 13.0,
13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0,
13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0,
13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.0,
13.0, 13.0, 13.0, 13.0, 13.0, 13.0, 13.1};


/* Number of steps calculated for Stacey probability */
#define NSTACEY 500



/* -----------------------------------------------------------
 * Load matter profile from file
 * each line of the file should contain a length value (in km)
 * and a density value (in g/cm^3) separated by space
 * -----------------------------------------------------------
 * filename   filename of the data file
 * -----------------------------------------------------------
 */
int glbLoadProfileData(const char* filename,
		       size_t *layers, double **length,
		       double **density)
{

  double *l,*d;
  size_t i,steps=0;
  char line[100];
  FILE *fp;

  if(filename==NULL)
    {
      glb_error("Input filename may not be NULL");
      return -1;
    }

  fp = glb_fopen(filename,"r");
  if(fp!=NULL)
    {
      steps=0;
      while (fgets(line,100,fp)!=NULL) steps++;
      l=(double*) glb_malloc(steps*sizeof(double));
      d=(double*) glb_malloc(steps*sizeof(double));
      rewind(fp);
      for (i=0; i<steps; i++) fscanf(fp,"%lf %lf",&l[i],&d[i]);
      glb_fclose(fp);
    }
  else
    {
      glb_error("Failed to open matter profile file");
      *layers=0;
      return -1;
    }
  *layers=steps;
  *density=d;
  *length=l;
  return 0;
}

/* -----------------------------------------------------------
 * Generate matter profile from Stacey data
 * -----------------------------------------------------------
 * nadir      nadir angle in rad
 * steps      numer of aequidistant layers
 * -----------------------------------------------------------
 */


static double CoreDistance(double l, double bl)
{
	return sqrt(pow(l,2) + pow(GLB_EARTH_RADIUS,2) - bl*l);
}

static double MatterProfile(double r)
{
	int nr = (int) floor(NSTACEY*r/GLB_EARTH_RADIUS);
	return stacey[nr];
}


static int earth_range(double baseline)
{
  if(baseline<0)
    {
      glb_error("The baseline may not be negative");
      return -1;
    }

  if(baseline>2*GLB_EARTH_RADIUS)
    {
      glb_error("The baseline may not exceed 2*GLB_EARTH_RADIUS");
      return -1;
    }
  return 0;
}


int glbStaceyProfile(double baseline, size_t layers, double **length,
		  double **density)
{

  double delta_l = baseline/layers;
  double *d,*l;
  int i;

  if(earth_range(baseline)!=0) return -1;
  l=(double*) glb_malloc(layers*sizeof(double));
  d=(double*) glb_malloc(layers*sizeof(double));

  for (i=0; i<layers; i++)
    {
      /* the +0.5 makes a symmetric profile */
      d[i] = MatterProfile(GLB_EARTH_RADIUS
				 - CoreDistance((i+0.5)*delta_l,baseline));
      l[i] = delta_l;
    }
  *density=d;
  *length=l;
  return 0;
}

int glbAverageDensityProfile(double baseline, double **length,
			     double **density)
{
  double sum,*l,*d,res;
  int s,i,st=1000;
  sum=0.0;

  if(earth_range(baseline)!=0) return -1;
  s=glbStaceyProfile(baseline,st,&l,&d);
  if(s!=0) return -1;
  for(i=0;i<st;i++) sum += d[i];
  glb_free(l);
  glb_free(d);
  res=(double) sum/st;
  l=(double *) glb_malloc(sizeof(double));
  d=(double *) glb_malloc(sizeof(double));
  l[0]=baseline;
  d[0]=res;
  *density=d;
  *length=l;
  return 0;
}

/* Accessing the actual profile data as used in oszprob */

int glbGetProfileData(size_t *layers, double** length, double** density)
{
  return glbGetProfileDataInExperiment(glb_current_exp, layers, length, density);
}


int glbGetProfileDataInExperiment(int exp,
				  size_t *layers,
				  double** length,
				  double** density)
{
  double *l,*d,*ll,*dd;
  int i,steps;
  if((exp<0)||(exp>=glb_num_of_exps))
    {
      glb_error("Experiment number out of range");
      return -1;
    }

  steps=glb_experiment_list[exp]->psteps;
  ll=glb_experiment_list[exp]->lengthtab;
  dd=glb_experiment_list[exp]->densitytab;
  if(steps>0)
    {
      l=(double*) glb_malloc(steps*sizeof(double));
      d=(double*) glb_malloc(steps*sizeof(double));
      for(i=0;i<steps;i++)
	{
	  d[i]=dd[i];
	  l[i]=ll[i];
	}
      *density=d;
      *length=l;
      *layers=steps;
      return 0;
    }
  else
    {
      glb_error("No profile data to get");
      *length=NULL;
      *density=NULL;
      *layers=0;
      return -1;
    }


}

int glbGetProfileTypeInExperiment(int exp)
{
  int i=1;
  if((exp<0)||(exp>=glb_num_of_exps))
    {
      glb_error("Experiment number out of range");
      return -1;
    }
  i= glb_experiment_list[exp]->density_profile_type;
  return i;
}

int glbSetProfileDataInExperiment(int exp,
				  size_t layers,
				  const double* length,
				  const double* density)
{
  double *ll,*dd,*bb,lk=0;
  int i=0;
  if((exp<0)||(exp>=glb_num_of_exps))
    {
      glb_error("Experiment number out of range");
      return -1;
    }

  glb_experiment_list[exp]->psteps=layers;
  glb_experiment_list[exp]->density_profile_type = 3;
  if(layers>0)
    {
      ll=(double*) glb_malloc(layers*sizeof(double));

      dd=(double*) glb_malloc(layers*sizeof(double));

      bb=(double*) glb_malloc(layers*sizeof(double));

      for(i=0;i<layers;i++)
	{
	  dd[i]=density[i];
	  bb[i]=density[i];
	  ll[i]=length[i];
	  lk+=length[i];
	}
      glb_free(glb_experiment_list[exp]->lengthtab);
      glb_free(glb_experiment_list[exp]->densitytab);
      glb_free(glb_experiment_list[exp]->densitybuffer);

      glb_experiment_list[exp]->lengthtab=ll;
      glb_experiment_list[exp]->densitytab=dd;
      glb_experiment_list[exp]->densitybuffer=bb;
      glb_experiment_list[exp]->baseline=lk;

      return 0;
    }
  else
    {
      glb_error("No profile data to set");
      return -1;
    }
}

/* Changing a baseline according to density_profile_type */

int
glbSetBaselineInExperiment(int exp, double baseline)
{
  struct glb_experiment *ins;
  double *lb,*db,scale;
  size_t l;
  int s=-1,i;
  double ttl=0;
  if((exp<0)||(exp>=glb_num_of_exps))
    {
      glb_error("Experiment number out of range");
      return -1;
    }
  ins=(struct glb_experiment *) glb_experiment_list[exp];

  if(ins->density_profile_type==-1)
    {glb_error("No profile type specified");s=-1;}

  if(ins->density_profile_type==1)
    {
      glb_free(ins->lengthtab);
      glb_free(ins->densitytab);
      ins->psteps=1;
      if(baseline > 0)
	{
	  s+=glbAverageDensityProfile(baseline,&lb,&db);
	  ins->densitytab=db;
	  ins->lengthtab=lb;
	  ins->baseline=baseline;

	}
      else
	{
	  glb_error("Baseline must be a positive number");
	  s=-1;
	}
      return s;
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
	      s+=glbStaceyProfile(baseline,l,&lb,&db);
	      ins->densitytab=db;
	      ins->lengthtab=lb;
	      ins->baseline=baseline;
	    }
	  else
	    {
	      glb_error("Densitysteps must be a positive number");
	      s=-1;
	    }
	  return s;
	}
      else
	{
	  glb_error("Baseline must be a positive number");
	  s=-1;
	}
    }

  if(ins->density_profile_type==3)
    {
      if(ins->lengthtab!=NULL && ins->psteps > 0)
	{
	  for(i=0;i<ins->psteps;i++) ttl+=ins->lengthtab[i];
	}
      else
	{
	   glb_error("glbSetBaselineInExperiment failed");
	   return -1;
	}
      scale=baseline/ttl;
      if(ins->lengthtab!=NULL && ins->psteps > 0)
	{
	  for(i=0;i<ins->psteps;i++){
	    ins->lengthtab[i] = scale * ins->lengthtab[i];
	  }
	}
      ins->baseline=baseline;
      return s;
    }

  glb_error("glbSetBaselineInExperiment failed");
  return -1;

}

double
glbGetBaselineInExperiment(int exp)
{
  struct glb_experiment *in;

  double out;
  if((exp<0)||(exp>=glb_num_of_exps))
    {
      glb_error("Experiment number out of range");
      return -1;
    }
  in=(struct glb_experiment *) glb_experiment_list[exp];

  out=in->baseline;

  return out;
}
