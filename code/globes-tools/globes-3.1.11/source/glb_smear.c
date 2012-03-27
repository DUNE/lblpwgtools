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

/* These routines compute the smear matrix in the same way as CalcSmearA
 * in GlobesMESbeta.m. CalcSmearB is also implemented, but it seems
 * that the algorithm can be improved.
 *
 * Patrick Huber (c) 2004
 */


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* That's new -- using the GNU Scientific Library */
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_erf.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_sf_gamma.h>



#include "glb_error.h"
#include "glb_types.h"
#include "glb_multiex.h"
#include "glb_smear.h"


#define TOL 1.0E-7

/* An example for a pretty general sigma(E,params)
   sigma(E)/E = in[0] + in[1]/sqrt(E) + in[2]/E */

static double my_sigma(double x, double *in)
{
  return in[0]*x + in[1]*sqrt(x) + in[2];
}


/* Prototypes for private things */
void glb_init_bin_data(glb_smear *in);



/* First some functions for checking user input.
 *
 *
 * The following call sequence is our safety net:
 *   glb_smear *s;  *   s=glb_smear_alloc();
 *    user input (via parser)
 *   glb_default_smear(s);
 *   SetupSmearMatixX(s);
 *    now it is safe to use function like BinBoundary ...
 *  SmearMatrixX(s,...);
 * Once everthing is finished
 *  glb_smear_free(s);
 * Other call sequences may lead to undefined behaviour!
 */


/* These are called by init_glb_smear and glb_default_smear */
glb_option_type *glb_option_type_alloc()
{
  glb_option_type *in;
  in=(glb_option_type *) glb_malloc(sizeof(glb_option_type));
  in->corr_fac=-1;
  in->confidence_level=-1;
  in->offset=-1;
  in->low_bound=-1;
  in->up_bound=-1;
  return in;
}

glb_option_type *glb_option_type_reset(glb_option_type *in)
{
  in->corr_fac=-1;
  in->confidence_level=-1;
  in->offset=-1;
  in->low_bound=-1;
  in->up_bound=-1;
  return in;
}

void glb_option_type_free(glb_option_type *in)
{
    glb_free(in);
}

int glb_default_option_type(glb_option_type *in, const glb_smear *head,
const struct glb_experiment *headex)
{
  int s,d;
  s=0;
  d=0;
  in->corr_fac=headex->filter_value;

  if(in->confidence_level==-1) {in->confidence_level=0.999;d=2;}
  if(in->confidence_level >= 1 || in->confidence_level <= 0)
    {glb_error("The confidence level has to be inbetween 0 and 1!");s=-1;}
  if(in->offset==-1)  {in->offset=0;d=2;}
  if(in->offset<0) {glb_error("Offset must not be smaller than 0!");s=-1;}

  if(head==NULL)
    {
      if(in->low_bound==-1) {glb_error("No low_bound given!");s=-1;}
      if(in->up_bound==-1) {glb_error("No up_bound given!");s=-1;}
    }
  else
    {
      if(in->low_bound==-1) {in->low_bound=head->e_min;;d=2;}
      if(in->up_bound==-1) {in->up_bound=head->e_max;d=2;}
    }
  if(in->low_bound<0) {glb_error("low_bound must be positive!");s=-1;}
  if(in->up_bound<0) {glb_error("up_bound must be positive!");s=-1;}
  if(in->low_bound >= in->up_bound)
    {glb_error("low_bound must be smaller than up_bound!");s=-1;}
  if(s!=0) glb_fatal("glb_option_type not properly set up!");
  return d;
}


glb_smear  *glb_smear_alloc()
{
  glb_smear *in;
  in=(glb_smear *) glb_malloc(sizeof(glb_smear));
  in->type=-1;
  in->numofbins=-1;
  in->simbins=-1;
  in->e_min=-1;
  in->e_max=-1;
  in->e_sim_min=-1;
  in->e_sim_max=-1;
  in->sigma=NULL;
  in->num_of_params=-1;
  in->sig_f=NULL;
  in->binsize=NULL;
  in->simbinsize=NULL;
  in->bincenter=NULL;
  in->simbincenter=NULL;
  in->options=NULL;
  return in;
}

glb_smear  *glb_smear_reset(glb_smear *in)
{
  in->type=-1;
  in->numofbins=-1;
  in->simbins=-1;
  in->e_min=-1;
  in->e_max=-1;
  in->e_sim_min=-1;
  in->e_sim_max=-1;
   /* FIXME a possible memory leak -> dereferencing memory to NULL
    * Problem: at start up sigma is not NULL, but undefined cannot be free'd
    * here !
    */
  in->sigma=NULL;
  in->num_of_params=-1;
  in->sig_f=NULL;

  /* FIXME a possible memory leak -> dereferencing memory to NULL */
  in->binsize=NULL;
  in->simbinsize=NULL;
  in->bincenter=NULL;
  in->simbincenter=NULL;
  in->options=NULL;
  return in;
}



void glb_smear_free(glb_smear *in)
{
  if(in==NULL) return;
  if(in->binsize!=NULL) glb_free(in->binsize);
  if(in->bincenter!=NULL) glb_free(in->bincenter);

  if(in->simbinsize!=NULL) glb_free(in->simbinsize);
  if(in->simbincenter!=NULL) glb_free(in->simbincenter);
  if(in->sigma!=NULL) glb_free(in->sigma);

  if(in->options!=NULL) glb_option_type_free(in->options);
  glb_free(in);
}

int glb_default_smear(glb_smear *in,const struct glb_experiment *head)
{
  double med;
  int s,d,i;
  s=0;
  d=0;
  if(in->type==-1) {glb_exp_error(head, "Smearing type has not been defined!");s=-1;}
  if(in->numofbins==-1) {glb_exp_error(head, "Number of bins has not been defined!");
  s=-1;}
  if(in->numofbins<=0) {glb_exp_error(head, "Number of bins must be positive!");
  s=-1;}

  /* simbins, simtresh and simbeam are no longer defaultized here
   * but by the SmearMatrix computation functions
   *
   *  if(in->simbins==-1) {in->simbins=in->numofbins;d=2;}
   *  if(in->simbins <=0)
   * {glb_exp_error(head, "Number of simbins must be positive!");s=-1;}
   */
  if(in->e_min==-1) {glb_exp_error(head, "No minimal energy defined!");s=-1;}
  if(in->e_min<0) {glb_exp_error(head, "Minimal energy must be positive!");s=-1;}
  if(in->e_max==-1) {glb_exp_error(head, "No maximal energy defined!");s=-1;}
  if(in->e_max<=in->e_min) {glb_error("Maximal energy must be larger than"
                                      " the minimal energy!");s=-1;}
  if(in->e_sim_min!=-1||in->e_sim_max!=-1)
    {
      if(in->e_sim_min<0)
        {glb_exp_error(head, "Minimal sim energy must be positive!");s=-1;}
      if(in->e_sim_max<=in->e_sim_min)
        {glb_exp_error(head, "Maximal sim energy must belarger than the minimal"
                   " sim energy!");s=-1;}
    }
  /*
   * if(in->e_sim_min==-1) {in->e_sim_min=in->e_min;d=2;}
   * if(in->e_sim_max==-1) {in->e_sim_max=in->e_max;d=2;}
   */

  if(in->num_of_params==-1)
    {
      /*    glb_error("Number of parameters for the energy"
              " resolution function unspecified!");
              s=-1;*/
      in->num_of_params=3;
    }

  if(in->num_of_params<0) {
    glb_error("Number of parameters for the energy"
              " resolution function must larger"
              " than zero!");
    s=-1;
  }

  if(in->num_of_params==0) {in->sigma=NULL;d=2;}

  if(in->num_of_params>0&&in->sigma==NULL)
    {
       glb_error("No parameters for the energy"
              " resolution function unspecified!");
       s=-1;
    }
  /*if(in->sig_f==NULL) {glb_error("No energy resolution"
    " function defined!");s=-1;}*/


   if(in->sig_f==NULL) {in->sig_f=&my_sigma;d=2;}
   if(in->binsize==NULL&&s==0&&head->binsize==NULL)
    {
      /* In this case we assume that the bins are equidistant */
      in->binsize=(double *) glb_malloc(sizeof(double)*in->numofbins);
      med=(in->e_max-in->e_min)/in->numofbins;
      for(i=0;i<in->numofbins;i++) in->binsize[i]=med;
    }
   if(in->binsize==NULL&&s==0&&head->binsize!=NULL)
    {
      in->binsize=(double *) glb_malloc(sizeof(double)*in->numofbins);
      for(i=0;i<in->numofbins;i++) in->binsize[i]=head->binsize[i];
    }

   if(s==0) {
     glb_init_bin_data(in);
   }


  /* simbincenter etc. will be intialized by InitSmearMatrix */

  d=d+glb_default_option_type(in->options,in,head);


  if(s!=0) glb_fatal("glb_smear not properly set up!");
  return d;
}


glb_smear  *glb_copy_smear(glb_smear *dest, const glb_smear *src)
{
  int i;
  glb_option_type *temp;
  if(dest->options==NULL) temp=glb_option_type_alloc();
  else temp=dest->options;
  dest=(glb_smear *) memmove(dest,src,sizeof(glb_smear));
  dest->options=temp;
  dest->options=(glb_option_type *) memmove(dest->options,src->options,
                                    sizeof(glb_option_type));


  if(src->num_of_params <= 0) return dest;
  dest->sigma=(double *) glb_malloc(src->num_of_params*sizeof(double));
  for(i=0;i<src->num_of_params;i++) dest->sigma[i]=src->sigma[i];
  /* What about binsize etc ? */


  return dest;
}


/* end */

/* These functions provide data like bin boundaries and bin centers
 * in lookup tables in order to improve performance. Furthemore they
 * also give access to those data. They all are able to deal with user
 * defined bins, ie. variable width.
 */



void glb_init_bin_data(glb_smear *in)
{
  int i;
  double med;
  in->bincenter=(double *) glb_malloc(sizeof(double)*in->numofbins);
  med=in->e_min;
  for(i=0;i<in->numofbins;i++)
    {
      med += in->binsize[i];
      in->bincenter[i]=med-in->binsize[i]*0.5;
    }
}

void glb_init_sim_bin_data(glb_smear *in)
{
  int i;
  double med;

  in->simbincenter=(double *) glb_malloc(sizeof(double)*in->simbins);

  med=in->e_sim_min;
  for(i=0;i<in->simbins;i++)
    {
      med += in->simbinsize[i];
      in->simbincenter[i]=med-in->simbinsize[i]*0.5;
    }
}

double glb_lower_bin_boundary(int bin, const glb_smear *data)
{
  return
    data->bincenter[bin] - data->binsize[bin] * 0.5;
}

double glb_upper_bin_boundary(int bin, const glb_smear *data)
{
  return
   data->bincenter[bin] + data->binsize[bin] * 0.5;
}

double glb_bin_center(int bin, const glb_smear *data)
{
  return
    data->bincenter[bin];
}


double glb_lower_sbin_boundary(int bin, const glb_smear *data)
{
  return
    data->simbincenter[bin] - data->simbinsize[bin] * 0.5;
}

double glb_upper_sbin_boundary(int bin, const glb_smear *data)
{
  return
   data->simbincenter[bin] + data->simbinsize[bin] * 0.5;
}

double glb_sbin_center(int bin, const glb_smear *data)
{
  return
    data->simbincenter[bin];
}





/* end */

static double Sigma(int bin, const glb_smear *data)
{
  return data->sig_f(glb_sbin_center(bin,data),data->sigma);
}

/* It really would be nice to have an algorithm for type A matrices
 * which allows different number of simbins and bins. It is possible
 * and already implemented.
 */

/*
  double SmearMatrixElementA(int i, int j, const glb_smear *data)
  {
  double p1,p2;
  p1=0.5*(1+gsl_sf_erf(
  (glb_upper_bin_boundary(j,data)-glb_bin_center(i,data))/
  (sqrt(2)*Sigma(i,data))
  ));
  p2=0.5*(1+gsl_sf_erf(
  (glb_lower_bin_boundary(j,data)-glb_bin_center(i,data))/
  (sqrt(2)*Sigma(i,data))
  ));
  return p1-p2;
}
*/

/* In computing the matrix element it is important to keep
 * the indices in correct ordering. We used to compute the transpose
 * of the smearing matrix -- now it is changed according to the formulas
 * in the documentation. For a equal number of bins and simbins the difference
 * is given by the variation of sigma, which changes from sigma(E') to sigma(E)
 * in the new version.
 *
 * Furthermore the correct scaling with the binsize and simbinsize
 * has been implemented. Simbinize is multiplied as a number onto
 * the matrix element.
 *
 * The actual binsize is taken into account by the integration
 * over the bin width with respect to E', which is already analytically taken
 * care of and contained in the from of the matrix elements.
 */

static double SmearMatrixElementA(int i, int j, const glb_smear *data)
  {
  double p1,p2;
  p1=0.5*(1+gsl_sf_erf(
  (glb_upper_bin_boundary(i,data)-glb_sbin_center(j,data))/
  (sqrt(2)*Sigma(j,data))
  ));
  p2=0.5*(1+gsl_sf_erf(
  (glb_lower_bin_boundary(i,data)-glb_sbin_center(j,data))/
  (sqrt(2)*Sigma(j,data))
  ));

  return (p1-p2);
}

static double Round(double in)
{
  double erg,x;
  if(in<0) x= -in;
  else x=in;
  if( fabs(x-floor(x))<0.5) erg=floor(x);
     else erg=ceil(x);
  if(in<0) erg= -erg;
  return erg;

}

static void SetupSmearMatrixA(glb_smear *test,const struct glb_experiment *head)
{
  int i;
  double med;
  if(test->e_sim_min==-1)
    {
      if(head->simtresh==-1)
        test->e_sim_min=head->emin;
      else
        test->e_sim_min=head->simtresh;
    }

  if(test->e_sim_max==-1)
    {
      if(head->simbeam==-1)
        test->e_sim_max=head->emax;
      else
        test->e_sim_max=head->simbeam;
    }

  if(test->simbins==-1)
    {
      if(head->simbins==-1)
        test->simbins=head->numofbins;
      else
        test->simbins=head->simbins;
    }




  if(test->simbinsize==NULL&&head->simbinsize!=NULL)
    {
      /* In this case we assume that the bins are equidistant */
      test->simbinsize=(double *) glb_malloc(sizeof(double)*test->simbins);
      for(i=0;i<test->simbins;i++) test->simbinsize[i]=head->simbinsize[i];
    }


  if(test->simbinsize==NULL)
    {
      test->simbinsize=(double *) glb_malloc(sizeof(double)*test->simbins);
      med=(test->e_sim_max-test->e_sim_min)/test->simbins;
      for(i=0;i<test->simbins;i++) test->simbinsize[i]=med;
    }


  glb_init_sim_bin_data(test);
  return;
}



void glb_set_up_smear_data(glb_smear *test,const struct glb_experiment *head)
{
  int i;
  double med;
  if(test->e_min==-1) test->e_min=head->emin;

  if(test->e_max==-1) test->e_max=head->emax;

  if(test->numofbins==-1) test->numofbins=head->numofbins;


  if(test->binsize==NULL&&head->binsize==NULL)
    {
      test->binsize=(double *) glb_malloc(sizeof(double)*test->numofbins);
      med=(test->e_max-test->e_min)/test->numofbins;
      for(i=0;i<test->numofbins;i++) test->binsize[i]=med;
    }

  if(test->binsize==NULL&&head->binsize!=NULL)
    {
      test->binsize=(double *) glb_malloc(sizeof(double)*test->numofbins);
      for(i=0;i<test->numofbins;i++) test->binsize[i]=head->binsize[i];
    }

  glb_init_bin_data(test);



  if(test->e_sim_min==-1)
    {
      if(head->simtresh==-1)
        test->e_sim_min=head->emin;
      else
        test->e_sim_min=head->simtresh;
    }

  if(test->e_sim_max==-1)
    {
      if(head->simbeam==-1)
        test->e_sim_max=head->emax;
      else
        test->e_sim_max=head->simbeam;
    }

  if(test->simbins==-1)
    {
      if(head->simbins==-1)
        test->simbins=head->numofbins;
      else
        test->simbins=head->simbins;
    }

  if(test->simbinsize==NULL&&head->simbinsize==NULL)
    {
      test->simbinsize=(double *) glb_malloc(sizeof(double)*test->simbins);
      med=(test->e_sim_max-test->e_sim_min)/test->simbins;
      for(i=0;i<test->simbins;i++) test->simbinsize[i]=med;
    }

  if(test->simbinsize==NULL&&head->simbinsize!=NULL)
    {
      test->simbinsize=(double *) glb_malloc(sizeof(double)*test->simbins);
      for(i=0;i<test->simbins;i++) test->simbinsize[i]=head->simbinsize[i];
    }

  glb_init_sim_bin_data(test);

  test->sig_f=NULL;
  test->num_of_params=0;
  test->type=0;

  return;
}



static double** SmearMatrixA(glb_smear *data, int **lowrange, int **uprange,
                      const struct glb_experiment *head)
{
  int i,j,nonzero,zero,prev;
  double erg;
  double **out=NULL;

  int *low,*up;

  SetupSmearMatrixA(data,head);
  if(data->type != GLB_TYPE_A)
    {
      glb_error("Type mismatch in SmearMatrixA");
      return NULL;
    }

  out=(double**) glb_malloc(sizeof(double*) * data->numofbins);
  up=(int*) glb_malloc(sizeof(int) * data->numofbins);
  low=(int*) glb_malloc(sizeof(int) * data->numofbins);

  for(i=0;i<data->numofbins;i++)
    {
      nonzero=0;
      zero=0;
      prev=0;
      out[i]=NULL;
      for(j=0;j<data->simbins;j++)
        {

          erg=Round(1/TOL*SmearMatrixElementA(i,j,data))*TOL;
          if(erg!=0)
            {
              prev=1;
              nonzero++;
              out[i]=(double*) glb_realloc(out[i],sizeof(double)*nonzero);
              out[i][nonzero-1]=erg;
            }
          else
            {
              if(prev==0) zero++;
            }

        }

      low[i]=zero;
      up[i]=zero+nonzero-1;


     }
  *lowrange=&low[0];
  *uprange=&up[0];
  return out;
}





/* The Filter feature requires to divided the half width of the Gaussian,
 * into two parts. One is dealt with by the filter and the other is
 * given by truncated_sigma
 */
static double truncated_sigma(double x, const glb_smear *in)
{
  /*sigma=in->sig_f(in->e_min,in->sigma)/(in->options->corr_fac);*/
  double sigma,res;
  sigma=in->options->corr_fac;
  res=pow(in->sig_f(x,in->sigma),2)-sigma*sigma;
  if(res<=0) {glb_fatal("Filter value too large");exit(1);}
  else return sqrt(res);
}



static double SmearMatrixElementC(int i, int j, const glb_smear *data)
  {
  double p1,p2;
  p1=0.5*(1+gsl_sf_erf(
  (glb_upper_bin_boundary(i,data)-glb_sbin_center(j,data))/
  (sqrt(2)*truncated_sigma(glb_sbin_center(j,data),data))
  ));
  p2=0.5*(1+gsl_sf_erf(
  (glb_lower_bin_boundary(i,data)-glb_sbin_center(j,data))/
  (sqrt(2)*truncated_sigma(glb_sbin_center(j,data),data))
  ));

  return (p1-p2);
}


static double** SmearMatrixC(glb_smear *data, int **lowrange, int **uprange,
                      const struct glb_experiment *head)
{
  int i,j,nonzero,zero,prev;
  double erg;
  double **out=NULL;

  int *low,*up;

  SetupSmearMatrixA(data,head);
  if(data->type != GLB_TYPE_C)
    {
      glb_error("Type mismatch in SmearMatrixC");
      return NULL;
    }


  out=(double**) malloc(sizeof(double*) * data->numofbins);
  up=(int*) malloc(sizeof(int) * data->numofbins);
  low=(int*) malloc(sizeof(int) * data->numofbins);

  for(i=0;i<data->numofbins;i++)
    {
      nonzero=0;
      zero=0;
      prev=0;
      out[i]=NULL;
      for(j=0;j<data->simbins;j++)
        {

          erg=Round(1/TOL*SmearMatrixElementC(i,j,data))*TOL;


          if(erg!=0)
            {
              prev=1;
              nonzero++;
              out[i]=(double*) realloc(out[i],sizeof(double)*nonzero);
              out[i][nonzero-1]=erg;
            }
          else
            {
              if(prev==0) zero++;
            }

        }

      low[i]=zero;
      up[i]=zero+nonzero-1;


     }
  *lowrange=&low[0];
  *uprange=&up[0];
  return out;
}


/***************************************************************************
 * Function glb_compute_smearing_matrix                                    *
 ***************************************************************************
 * Compute the smearing matrix and store it efficiently in the internal    *
 * sparse matrix data structures                                           *
 ***************************************************************************
 * Parameters:                                                             *
 *   matrix: The smearing matrix entries                                   *
 *   lower:  Array of indices of lowest nonzero entry in each row          *
 *   upper:  Array of indices of highest nonzero entry in each row         *
 *   s:      glb_smear structure containing metadata about the smearing    *
 *   exp:    The experiment                                                *
 ***************************************************************************/
void glb_compute_smearing_matrix(double ***matrix,
                          int **lower, int **upper, glb_smear *s,
                          const struct glb_experiment *exp)
{
  if(s->type==GLB_TYPE_A)
    *matrix=SmearMatrixA(s, lower, upper, exp);
  else if(s->type==GLB_TYPE_C)
    *matrix=SmearMatrixC(s, lower, upper, exp);
  else
  {
    *matrix = NULL;
    *lower  = NULL;
    *upper  = NULL;
    glb_fatal("Smear type out of range");
  }

  return;
}


/***************************************************************************
 * Function glb_optimize_smearing_matrix                                   *
 ***************************************************************************
 * Remove entries that are close to zero from the smearing matrix to       *
 * speed up smearing and reduce memory usage                               *
 ***************************************************************************
 * Parameters:                                                             *
 *   smear:  glb_smear structure containing metadata about the smearing    *
 *   matrix: The smearing matrix entries                                   *
 *   lower:  Array of indices of lowest nonzero entry in each row          *
 *   upper:  Array of indices of highest nonzero entry in each row         *
 ***************************************************************************/
void glb_optimize_smearing_matrix(glb_smear *s, double **matrix, int *lower, int *upper)
{
  if (!s || !lower || !upper || !matrix)
    glb_fatal("glb_optimize_smearing_matrix: NULL pointer argument encountered.");

  for (int i=0; i < s->numofbins; i++)
  {
    if (!matrix[i])
      glb_fatal("glb_optimize_smearing_matrix: Incomplete smearing matrix encountered.");

    int new_lower = lower[i];
    int new_upper = upper[i];
    int j=0, k=upper[i]-lower[i];
    while (new_lower < upper[i]  &&  fabs(matrix[i][j]) < TOL/2.0)
    {
      j++;
      new_lower++;
    }
    while (new_upper > new_lower  &&  fabs(matrix[i][k]) < TOL/2.0)
    {
      k--;
      new_upper--;
    }

    double *new_entry = (double*) malloc(sizeof(double)*(new_upper-new_lower+1));
    if (!new_entry)
      glb_fatal("glb_optimize_smearing_matrix: Unable to allocate smearing matrix.");

    for (int m=0; m < new_upper-new_lower+1; m++)
      new_entry[m] = matrix[i][j++];

    free(matrix[i]);
    matrix[i] = new_entry;
    lower[i]  = new_lower;
    upper[i]  = new_upper;
  }
}


/***************************************************************************
 * Function glb_compensate_filter                                          *
 ***************************************************************************
 * Modify the smearing matrix in such a way that the effect of the         *
 * low-pass filter ($filter) is compensated.                               *
 ***************************************************************************
 * Parameters:                                                             *
 *   smear:  glb_smear structure containing metadata about the smearing    *
 *   matrix: The smearing matrix entries                                   *
 *   lower:  Array of indices of lowest nonzero entry in each row          *
 *   upper:  Array of indices of highest nonzero entry in each row         *
 *                                                                         *
 ***************************************************************************/
void glb_filter_compensate(glb_smear *s, double **matrix, int *lower, int *upper)
{
  if (!s || !lower || !upper || !matrix)
    glb_fatal("glb_filter_compensate: NULL pointer argument encountered.");

  double sigma = s->options->corr_fac;
  int factor = 16;
  int hires_bins = factor * s->simbins;

  gsl_matrix *Sin  = gsl_matrix_alloc(s->numofbins, s->simbins);
  gsl_matrix *Sout = gsl_matrix_alloc(s->numofbins, s->simbins);
  gsl_matrix *R    = gsl_matrix_alloc(s->simbins, hires_bins);
  gsl_matrix *SR   = gsl_matrix_alloc(s->numofbins, hires_bins);
  gsl_matrix *F    = gsl_matrix_alloc(s->simbins, hires_bins);
  gsl_matrix *FT   = gsl_matrix_alloc(hires_bins, s->simbins);
  gsl_matrix *V    = gsl_matrix_alloc(s->simbins, s->simbins);
  gsl_vector *b    = gsl_vector_alloc(hires_bins);
  gsl_vector *sv   = gsl_vector_alloc(s->simbins);
  gsl_vector *x    = gsl_vector_alloc(s->simbins);
  int signum;

  if (!Sin || !Sout | !R || !SR || !F || !FT || !V || !b || !sv || !x)
    glb_fatal("glb_filter_compensate: Unable to allocate temporary memory for smearing matrix.");

  /* Copy existing smearing matrix into a non-sparse data structure */
  for (int i=0; i < s->numofbins; i++)
  {
    if (!matrix[i])
      glb_fatal("glb_filter_compensate: Incomplete smearing matrix encountered.");
    for (int j=0; j < s->simbins; j++)
    {
      if (j >= lower[i] && j <= upper[i])
        gsl_matrix_set(Sin, i, j, matrix[i][j-lower[i]]);
      else
        gsl_matrix_set(Sin, i, j, 0.0);
    }
  }

  /* Generate matrix that projects a vector with highres_bins entries onto
   * a vector with simbins entries */
  gsl_matrix_set_zero(R);
  for (int i=0; i < s->simbins; i++)
    for (int j=0; j < factor; j++)
      gsl_matrix_set(R, i, factor*i + j, 1.0);
  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Sin, R, 0.0, SR);

  /* Generate matrix describing effect of filter. For numerical stability, we
   * use erfc instead of erf if erf would be too close to unity. Maybe this
   * does not really help, but at least it doesn't hurt. */
  for (int i=0; i < s->simbins; i++)
  {
    for (int j=0; j < hires_bins; j++)
    {
      double c = glb_lower_sbin_boundary(j/factor,s) +
         (glb_upper_sbin_boundary(j/factor,s) - glb_lower_sbin_boundary(j/factor,s))
                     * (j%factor + 0.5) / factor;
      double x1 = (glb_upper_sbin_boundary(i,s) - c) / (sqrt(2.0)*sigma);
      double x2 = (glb_lower_sbin_boundary(i,s) - c) / (sqrt(2.0)*sigma);
      double erf1 = gsl_sf_erf(x1);
      double erf2 = gsl_sf_erf(x2);
      if (erf1 < 0.5 && erf2 < 0.5)
        gsl_matrix_set(F, i, j, 0.5 * (erf1 - erf2));
      else if (erf1 < 0.5 && erf2 >= 0.5)
        gsl_matrix_set(F, i, j, 0.5 * ((erf1 + gsl_sf_erfc(x2)) - 1.0));
      else if (erf1 >= 0.5 && erf2 < 0.5)
        gsl_matrix_set(F, i, j, 0.5 * ((-erf2 - gsl_sf_erfc(x1)) + 1.0));
      else
        gsl_matrix_set(F, i, j, 0.5 * (gsl_sf_erfc(x2) - gsl_sf_erfc(x1)));
    }
  }

  /* Solve linear system F^T (S R F^-1)^T = (S R)^T to find (S R F^-1) */
  gsl_matrix_transpose_memcpy(FT, F);
  gsl_linalg_SV_decomp(FT, V, sv, x);
//  gsl_linalg_SV_decomp_jacobi(FT, V, sv);
  if (gsl_vector_min(sv) < TOL  ||  gsl_vector_max(sv) / gsl_vector_min(sv) > 1/TOL)
    glb_error("glb_filter_compensate: Unfolding of filter is numerically unstable. "
              "Reduce filter width.");
  for (int i=0; i < s->numofbins; i++)
  {
    gsl_matrix_get_row(b, SR, i);
    gsl_linalg_SV_solve(FT, V, sv, b, x);
    gsl_matrix_set_row(Sout, i, x);
  }

  /* Copy smearing matrix back into GLoBES data structures */
  for (int i=0; i < s->numofbins; i++)
  {
    /* Comment about TOL: The value value TOL/2.0 makes this criterion
     * equivalent to the one in SmearMatrixC and SmearMatrixA. It is
     * responsible for tiny differences observed between the results for
     * no compensation and compensation with F = id. */
    lower[i] = 0;
    while (lower[i] < s->simbins-1  &&  fabs(gsl_matrix_get(Sout, i, lower[i])) < TOL/2.0)
      lower[i]++;

    upper[i] = s->simbins - 1;
    while (upper[i] > lower[i]  &&  fabs(gsl_matrix_get(Sout, i, upper[i])) < TOL/2.0)
      upper[i]--;

    if (matrix[i])
      free(matrix[i]);
    matrix[i] =  (double*) malloc(sizeof(double)*(upper[i]-lower[i]+1));
    if (!matrix[i])
      glb_fatal("glb_filter_compensate: Unable to allocate smearing matrix.");

    for (int j=0; j < upper[i]-lower[i]+1; j++)
      matrix[i][j] = gsl_matrix_get(Sout, i, lower[i]+j);
  }

  gsl_vector_free(x);
  gsl_vector_free(sv);
  gsl_vector_free(b);
  gsl_matrix_free(V);
  gsl_matrix_free(FT);
  gsl_matrix_free(F);
  gsl_matrix_free(SR);
  gsl_matrix_free(R);
  gsl_matrix_free(Sout);
  gsl_matrix_free(Sin);
}



///***************************************************************************
// * Function glb_compensate_filter                                          *
// ***************************************************************************
// * Modify the smearing matrix in such a way that the effect of the         *
// * low-pass filter ($filter) is compensated.                               *
// ***************************************************************************
// * Parameters:                                                             *
// *   smear:  glb_smear structure containing metadata about the smearing    *
// *   matrix: The smearing matrix entries                                   *
// *   lower:  Array of indices of lowest nonzero entry in each row          *
// *   upper:  Array of indices of highest nonzero entry in each row         *
// *                                                                         *
// ***************************************************************************/
//void glb_filter_compensate(glb_smear *s, double **matrix, int *lower, int *upper)
//{
//  if (!s || !lower || !upper || !matrix)
//    glb_fatal("glb_filter_compensate: NULL pointer argument encountered.");
//
//  double sigma = s->options->corr_fac;
//  int factor = 8;
//  int hires_bins = factor * s->simbins;
//
//  gsl_matrix *Sin  = gsl_matrix_alloc(s->numofbins, s->simbins);
//  gsl_matrix *Sout = gsl_matrix_alloc(s->numofbins, s->simbins);
//  gsl_matrix *R    = gsl_matrix_alloc(s->simbins, hires_bins);
//  gsl_matrix *SR   = gsl_matrix_alloc(s->numofbins, hires_bins);
//  gsl_matrix *V    = gsl_matrix_alloc(s->simbins, s->simbins);
////  gsl_matrix *F    = gsl_matrix_alloc(s->simbins, s->simbins);
//  gsl_matrix *F    = gsl_matrix_alloc(s->simbins, hires_bins);
////  gsl_matrix *FT   = gsl_matrix_alloc(s->simbins, s->simbins);
//  gsl_matrix *FT   = gsl_matrix_alloc(hires_bins, s->simbins);
//  gsl_matrix *Finv = gsl_matrix_alloc(s->simbins, s->simbins);
//  gsl_permutation *p = gsl_permutation_alloc(s->simbins);
////  gsl_vector *v    = gsl_vector_alloc(s->simbins);
//  gsl_vector *v    = gsl_vector_alloc(hires_bins);
//  gsl_vector *tau  = gsl_vector_alloc(s->simbins);
//  gsl_vector *r    = gsl_vector_alloc(hires_bins);
//  gsl_vector *x    = gsl_vector_alloc(s->simbins);
//  gsl_vector *norm = gsl_vector_alloc(s->simbins);
//  int signum;
//
//  if (!Sin || !Sout | !F) //FIXME
//    glb_fatal("glb_filter_compensate: Unable to allocate temporary memory for smearing matrix.");
//
//  /* Copy existing smearing matrix into a non-sparse data structure */
//  for (int i=0; i < s->numofbins; i++)
//  {
//    if (!matrix[i])
//      glb_fatal("glb_filter_compensate: Incomplete smearing matrix encountered.");
//    for (int j=0; j < s->simbins; j++)
//    {
//      if (j >= lower[i] && j <= upper[i])
//        gsl_matrix_set(Sin, i, j, matrix[i][j-lower[i]]);
//      else
//        gsl_matrix_set(Sin, i, j, 0.0);
//    }
//  }
//
////////  /* Assuming the energy resolution function to be Gaussian, compensate
////////   * for the filter by narrowing the Gaussian */
////////  for (int j=0; j < s->simbins; j++)
////////  {
////////    double norm_in = 0.0, norm_out = 0.0;
////////    for (int i=0; i < s->numofbins; i++)
////////    {
////////      norm_in += gsl_matrix_get(Sin, i, j); /* Remember norm (relevant for boundary bins) */
////////
////////      double sigma_prime = sigma*sigma/(sigm0*sigma0*(sigma0*sigma0-sigma*sigma));
////////      double t = (glb_sbin_center(i,s) - glb_sbin_center(j,s)) / (sqrt(2.0)*sigma_prime);
//////////      t = 0.0;
////////      if (gsl_matrix_get(Sin, i, j) > TOL/2.0)
////////        gsl_matrix_set(Sout, i, j, gsl_matrix_get(Sin, i, j) * exp(-t*t));
////////      else
////////        gsl_matrix_set(Sout, i, j, 0.0);
////////      norm_out += gsl_matrix_get(Sout, i, j);
////////    }
////////
////////    for (int i=0; i < s->numofbins; i++)  /* Correct normalization */
////////      gsl_matrix_set(Sout, i, j, gsl_matrix_get(Sout, i, j) * norm_in/norm_out);
////////  }
//
//  /* Generate matrix that projects a vector with highres_bins entries onto
//   * a vector with simbins entries */
//  gsl_matrix_set_zero(R);
//  for (int i=0; i < s->simbins; i++)
//    for (int j=0; j < factor; j++)
//      gsl_matrix_set(R, i, factor*i + j, 1.0);
//
//  /* Generate matrix describing effect of filter. For numerical stability, we
//   * use erfc instead of erf if erf would be too close to unity */
//  for (int i=0; i < s->simbins; i++)
//  {
////    for (int j=0; j < s->simbins; j++)
//    for (int j=0; j < hires_bins; j++)
//    {
//      double c = glb_lower_sbin_boundary(j/factor,s) +
//         (glb_upper_sbin_boundary(j/factor,s) - glb_lower_sbin_boundary(j/factor,s))
//                     * (j%factor + 0.5) / factor;
//      double x1 = (glb_upper_sbin_boundary(i,s) - c) / (sqrt(2.0)*sigma);
//      double x2 = (glb_lower_sbin_boundary(i,s) - c) / (sqrt(2.0)*sigma);
////      double x1 = (glb_upper_sbin_boundary(i,s)-glb_sbin_center(j,s))/(sqrt(2.0)*sigma);
////      double x2 = (glb_lower_sbin_boundary(i,s)-glb_sbin_center(j,s))/(sqrt(2.0)*sigma);
//      double erf1 = gsl_sf_erf(x1);
//      double erf2 = gsl_sf_erf(x2);
//      if (erf1 < 0.5 && erf2 < 0.5)
//        gsl_matrix_set(F, i, j, 0.5 * (erf1 - erf2));
//      else if (erf1 < 0.5 && erf2 >= 0.5)
//        gsl_matrix_set(F, i, j, 0.5 * ((erf1 + gsl_sf_erfc(x2)) - 1.0));
//      else if (erf1 >= 0.5 && erf2 < 0.5)
//        gsl_matrix_set(F, i, j, 0.5 * ((-erf2 - gsl_sf_erfc(x1)) + 1.0));
//      else
//        gsl_matrix_set(F, i, j, 0.5 * (gsl_sf_erfc(x2) - gsl_sf_erfc(x1)));
//
////      double t = (glb_sbin_center(i,s) - glb_sbin_center(j,s)) / (sqrt(2.0)*sigma);
////      gsl_matrix_set(F, i, j, 1.0/(sqrt(2.0*M_PI)*sigma)
////                   * (glb_upper_sbin_boundary(i,s)-glb_lower_sbin_boundary(i,s)) * exp(-t*t));
//
////      t = (glb_sbin_center(i,s) - glb_sbin_center(j,s));
////      t = (i - j) * 0.125;
////      gsl_matrix_set(F, i, j, exp(-t*t));
//////      printf("%g  %g\n", 0.5 * (erf1 - erf2),
////            gsl_matrix_get(F, i, j));
////      getchar();
//
////      if (gsl_matrix_get(F, i, j) < TOL/2.0)
////        gsl_matrix_set(F, i, j, 0.0);
//    }
//  }
//
////  for (int i=0; i < s->simbins; i++)
////  {
////    for (int j=0; j < s->simbins; j++)
////      printf("%g  ", gsl_matrix_get(F, i,j));
////      printf("\n");
////  }
////  getchar();
//
//  /* Solve linear system F^T (S R F^-1)^T = (S R)^T to find (S R F^-1) */
//  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Sin, R, 0.0, SR);
////  gsl_matrix_set_identity(F); //FIXME
//  gsl_matrix_transpose_memcpy(FT, F);
////  gsl_linalg_SV_decomp_jacobi(FT, V, r);
//  gsl_linalg_QR_decomp(FT, tau);
////  gsl_linalg_LU_decomp(FT, p, &signum);
//  for (int i=0; i < s->numofbins; i++)
//  {
////    gsl_matrix_get_row(v, Sin, i);
//    gsl_matrix_get_row(v, SR, i);
//
////    gsl_linalg_SV_solve(FT, V, r, v, x);
////    gsl_matrix_transpose_memcpy(FT, F);
////    gsl_linalg_HH_solve(FT, v, x);
//    gsl_linalg_QR_lssolve(FT, tau, v, x, r);
////    gsl_linalg_LU_solve(FT, p, v, x);
//
//    gsl_matrix_set_row(Sout, i, x);
//  }
//
////  /* Invert filter matrix */
////  if (gsl_linalg_LU_decomp(F, p, &signum) != GSL_SUCCESS)
////    glb_fatal("glb_filter_compensate: LU decomposition of filter matrix failed.");
////  printf("%d   %g\n", s->simbins, gsl_linalg_LU_det(F, signum));
////  getchar();
////  if (gsl_linalg_LU_invert(F, p, Finv) != GSL_SUCCESS)
////    glb_fatal("glb_filter_compensate: Inversion of filter matrix failed.");
////
////  /* Multiply normal smearing matrix with inverse of filter matrix */
////  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, Sin, Finv, 0.0, Sout);
//
//
//  /* Copy smearing matrix back into GLoBES data structures */
//  for (int i=0; i < s->numofbins; i++)
//  {
////    for (int j=0; j < s->simbins; j++)
////      printf("IN %d %g\n", j, gsl_matrix_get(Sin, i, j));
////    printf("IN LOWER: %d, UPPER: %d, \n", lower[i], upper[i]);
////    getchar();
//
//    /* Comment about TOL: The value value TOL/2.0 makes this criterion
//     * equivalent to the one in SmearMatrixC and SmearMatrixA. It is
//     * responsible for tiny differences observed between the results for
//     * no compensation and compensation with F = id. */
//    lower[i] = 0;
//    while (lower[i] < s->simbins-1  &&  fabs(gsl_matrix_get(Sout, i, lower[i])) < TOL/2.0)
//      lower[i]++;
//
//    upper[i] = s->simbins - 1;
//    while (upper[i] > lower[i]  &&  fabs(gsl_matrix_get(Sout, i, upper[i])) < TOL/2.0)
//      upper[i]--;
//
//    if (matrix[i])
//      free(matrix[i]);
//    matrix[i] =  (double*) malloc(sizeof(double)*(upper[i]-lower[i]+1));
//    if (!matrix[i])
//      glb_fatal("glb_filter_compensate: Unable to allocate smearing matrix.");
//
////    for (int j=0; j < s->simbins; j++)
////      printf("OUT %d %g\n", j, gsl_matrix_get(Sout, i, j));
////    printf("OUT LOWER: %d, UPPER: %d, \n", lower[i], upper[i]);
////    getchar();
//
//    for (int j=0; j < upper[i]-lower[i]+1; j++)
//      matrix[i][j] = gsl_matrix_get(Sout, i, lower[i]+j);
//  }
//
//  gsl_vector_free(norm);
//  gsl_vector_free(x);
//  gsl_vector_free(r);
//  gsl_vector_free(tau);
//  gsl_vector_free(v);
//  gsl_permutation_free(p);
//  gsl_matrix_free(Finv);
//  gsl_matrix_free(FT);
//  gsl_matrix_free(F);
//  gsl_matrix_free(V);
//  gsl_matrix_free(SR);
//  gsl_matrix_free(R);
//  gsl_matrix_free(Sout);
//  gsl_matrix_free(Sin);
//}



#ifdef GLB_SMEAR_OWN

/*-----------------------------------------------------------------*/
int
main (void)
{
  int i,j;
  glb_strange_type t;
  int *low,*up;
  double **matrix;

  double sm[]={0.15,0,0};

  /* That is Reactor */

  glb_option_type *opt;
  glb_smear *testR;

  /* That is needed for the new defaultizing process */
  struct glb_experiment inl;
  inl.simtresh=0.0017366983295549717;
  inl.simbeam=0.008407625841296241;
  inl.simbins=134;

  testR=glb_smear_alloc();
  opt=glb_option_type_alloc();


  opt->corr_fac=1.0;
  opt->confidence_level=1-1E-3;
  opt->offset=5;
  opt->low_bound=.0015;
  opt->up_bound=0.01;

  testR->type=GLB_TYPE_C;
  testR->numofbins=62;
  testR->e_min=0.0018;
  testR->e_max=0.008;


  testR->num_of_params=0;
  testR->sig_f=&chooz;
  testR->options=opt;

  glb_default_smear(testR);

  /* Here an example is defined -- Corresponding to NuFact */


  //glb_smear testNF = {GLB_TYPE_A,20,20,4,50,4,50,&sm[0],&my_sigma,NULL,NULL,opt};
  /*
  opt->corr_fac=1.0;
  opt->confidence_level=1-1E-3;
  opt->offset=1;
  opt->low_bound=4;
  opt->up_bound=50;

  testR->type=GLB_TYPE_B;
  testR->numofbins=20;
  testR->e_min=4.0;
  testR->e_max=50.0;
  testR->num_of_params=3;
  testR->sig_f=&my_sigma;
  testR->sigma=&sm[0];
  testR->options=opt;

  glb_default_smear(testR);*/
  /* That´s the call to our new routine */

  matrix=SmearMatrixC(testR,&low,&up,&inl);


  /* Just pretty printing to stdout */

  for(j=0;j<testR->numofbins;j++)
    {
      printf("%d %d ",low[j],up[j]);
      for(i=0;i<=up[j]-low[j];i++) printf("%10.10g ", matrix[j][i]);
      printf("\n");
    }


  glb_smear_free(testR);



  return 0;
}

#endif /* GLB_SMEAR_OWN */
