/* GLoBES -- General LOng Baseline Experiment Simulator
 * (C) 2002 - 2011,  The GLoBES Team
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
 *
 * 
 *
 * The functions collected here form a kind of mini-library of
 * utilities needed in locating degeneracies and computing chi^2
 * projections.
 *
 * Finding degenerate solutions is achieved by first computing the
 * systematics-only chi^2 on a grid in the log(sin^22theta_13))-delta
 * plane and finding all true minima on the grid. Also pseudo-minima
 * at the boundaries of the grid are taken into account. Once those
 * initial guesses are found, they are refined by ChiAll. Next all
 * duplicates are removed from the list of solutions.
 *
 * For each of the remaining solutions it is now possible to compute
 * the projection on the theta_13-axis starting at the solution and
 * wandering outwards, if necessary in both directions. In this
 * process the position of the minimum found in the previous step is
 * used as starting guess for the minimizer.
 *
 * This algorithm usually produces stable results, but sometimes a
 * jump into a deeper minimum occurs. These cases can be handled with
 * a back-propagation algorithm, which detects the jump and goes
 * backwards from there using the position of the new, lower minimum
 * as starting guesses for the above tracking algorithm.
 *
 * In principle it should be straight forward to extend the code for
 * projections on any axis.
 */

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>


#include "globes/globes.h"   /* GLoBES library */

#include "glb_tools_eightfold.h"


/* This is the data type for the result list. It is implemented as a
 * doubly linked chain. And only pointers to this structure are
 * visible beyond this file, thus it is an opaque structure. The
 * get/set function for each member. 
 */

typedef struct {
  result forward;
  result backward;
  glb_params minimum;
  double chi;
  double chi_sys;
  double chi_no_sys;
  double x_val;
  int diagnosis;
} result_type;

result alloc_result()
{
  result_type *tmp;
  tmp =(result_type *) malloc(sizeof(result_type));
  if(!tmp) {fprintf(stderr,"FATAL: no mem\n");exit(1);}
  tmp->minimum=glbAllocParams();
  tmp->forward=NULL;
  tmp->backward=NULL;
  tmp->diagnosis=0;
  return (result) tmp;  
}

void free_result(result stale)
{
  glbFreeParams(((result_type *) stale)->minimum);
  free(stale);
}

result set_forward(result in,result p)
{
  result_type *s;
  if(!in) return in;
  s=(result_type *) in;
  s->forward=p;
  return (result) s;
}

result set_backward(result in,result p)
{
  result_type *s;
  if(!in) return in;
  s=(result_type *) in;
  s->backward=p;
  return (result) s;
}

result get_forward(result in)
{
  result_type *s;
  result out;
  if(!in) return in;
  s=(result_type *) in;
  out=s->forward;
  return out;
}

result get_backward(result in)
{
  result_type *s;
  result out;
  if(!in) return in;
  s=(result_type *) in;
  out=s->backward;
  return out;
}

result set_minimum(result in, glb_params p)
{
  result_type *s;
  s=(result_type *) in;
  glbCopyParams(p,s->minimum);
  return (result) s;

}

glb_params get_minimum(result in, glb_params p)
{
  result_type *s;
  s=(result_type *) in;
  glbCopyParams(s->minimum,p);
  return p;
}


result set_diagnosis(result in,int p)
{
  result_type *s;
  s=(result_type *) in;
  s->diagnosis=p;
  return (result) s;
}

int get_diagnosis(result in)
{
  result_type *s;
  int out;
  s=(result_type *) in;
  out=s->diagnosis;
  return out;
}

result set_chi(result in,double p)
{
  result_type *s;
  s=(result_type *) in;
  s->chi=p;
  return (result) s;
}

double get_chi(result in)
{
  result_type *s;
  double out;
  s=(result_type *) in;
  out=s->chi;
  return out;
}

result set_chi_sys(result in,double p)
{
  result_type *s;
  s=(result_type *) in;
  s->chi_sys=p;
  return (result) s;
}

double get_chi_sys(result in)
{
  result_type *s;
  double out;
  s=(result_type *) in;
  out=s->chi_sys;
  return out;
}

result set_chi_no_sys(result in,double p)
{
  result_type *s;
  s=(result_type *) in;
  s->chi_no_sys=p;
  return (result) s;
}

double get_chi_no_sys(result in)
{
  result_type *s;
  double out;
  s=(result_type *) in;
  out=s->chi_no_sys;
  return out;
}


result set_xval(result in,double p)
{
  result_type *s;
  s=(result_type *) in;
  s->x_val=p;
  return (result) s;
}

double get_xval(result in)
{
  result_type *s;
  double out;
  s=(result_type *) in;
  out=s->x_val;
  return out;
}

result rewind_chain(result in)
{
  while(get_backward(in)) {in=get_backward(in);}
  return in;
}


void free_chain(result stale)
{
  result tmp,next;
  next=rewind_chain(stale);
  while(next) {
    tmp=next;
    next=get_forward(tmp);
    free_result(tmp);
  }
}

result setup_chain(double Xrange[2],double Resolution)
{
  double X,Res;
  result tmp=NULL,start=NULL;
  Res=Resolution;  
  if(Res<1) Res=1;
  int save=0;
  for(X=Xrange[0];X<=Xrange[1]+(Xrange[1]-Xrange[0])/Res/2;
      X=X+(Xrange[1]-Xrange[0])/Res)
    { save++;
      if(save>1E6) {fprintf(stderr,"recompile if you need chains longer than 1E6\n");exit(1);}
      tmp=alloc_result();
      set_xval(tmp,X);
      set_forward(start,tmp);
      set_backward(tmp,start);
      start=tmp;
    }
  tmp=rewind_chain(tmp);
  return tmp;
}

/* Some printing functions */

static void my_print_params(FILE *fp,const glb_params iv)
{
  int i;
  for(i=0;i<GLB_OSCP;i++) {
    if(i==GLB_DELTA_CP) fprintf(fp,"%6.6e\t",fmod(glbGetOscParams(iv,i),2*M_PI));
    else fprintf(fp,"%6.6e\t",glbGetOscParams(iv,i));
      }
  for(i=0;i<glb_num_of_exps;i++) fprintf(fp,"%6.6f\t",glbGetDensityParams(iv,i));
}


void print_result(FILE *fp,result in)
{
  glb_params tmp=glbAllocParams();
  tmp=get_minimum(in,tmp);
  fprintf(fp,"%d %6.6f\t",get_diagnosis(in),get_xval(in));
  fprintf(fp,"%6.6e\t%6.6e\t%6.6e\t",get_chi_no_sys(in),get_chi_sys(in),get_chi(in));
  my_print_params(fp,tmp);
  fprintf(fp,"\n");
  glbFreeParams(tmp);
}

/* These function operate on chains and provide some basic utilities */

result find_nearest_neighbor(result in, double xval)
{
  result tmp=NULL,out=NULL;
  double s,res=DBL_MAX;
  tmp=rewind_chain(in);
  while(tmp) {
    s=get_xval(tmp);
    if(fabs(s-xval)<=res) {res=fabs(s-xval);out=tmp;}
    tmp=get_forward(tmp);
  }
  return out;
}

/* A jump is defined as sequence where the derivative changes its sign
 * like this -+- or +-+ within just four grid points. Seems to be
 * pretty robust.
 */

result find_jump(result in, int fwd)
{
  result tmp,bw1,bw2,fw;
  double a,b,c,d;
  tmp=in;

  while(tmp) {
    a=-1;
    b=-1;
    d=-1;
    if((bw1=get_backward(tmp))) b=get_chi(bw1);
    if((bw2=get_backward(bw1))) a=get_chi(bw2);
    if((fw=get_forward(tmp))) d=get_chi(fw);
    c=get_chi(tmp);

    if(fwd==FORWARD) {if(a<b&&b>c&&c<d) break;}
    else if(fwd==BACKWARD) {if(a>b&&b<c&&c>d) break;}
    else break;

    if(fwd==FORWARD) tmp=get_forward(tmp);
    else if (fwd==BACKWARD) tmp=get_backward(tmp);
  }
  return tmp;
}


/* This function looks for minima on a grid, either a true minimum,
 * where all six surrounding points have a larger value or for
 * pseudo-minima on the boundaries of the grid.
 */ 

int find_min(double **array,
	     const double xrange[2],
	     const double yrange[2],
	     const int resolution[2], 
	     glb_params tv, 
	     glb_params **lis)
{
  size_t k,l,c=0,xc=0,yc=0,xs,ys;
  double res,x,y,*xvals,*yvals;
  glb_params *list=NULL;
  int s=-1;


  xs=resolution[0];
  ys=resolution[1];
  xvals=(double *) malloc(sizeof(double)*(xs));
  yvals=(double *) malloc(sizeof(double)*(ys));
  if(!xvals||!yvals) 
    {
      fprintf(stderr,"no mem\n");
      exit(1);
    }

  for(x=xrange[0];x<=xrange[1]+(xrange[1]-xrange[0])/(resolution[0]-1)/2;
      x=x+(xrange[1]-xrange[0])/(resolution[0]-1))
    {
      xvals[xc]=x;
      xc++;
    }
  for(y=yrange[0];y<=yrange[1]+(yrange[1]-yrange[0])/(resolution[1]-1)/2;
      y=y+(yrange[1]-yrange[0])/(resolution[1]-1))
    {
      yvals[yc]=y;
      yc++;
    }
	     
  
  for(k=1;k<xs-1;k++)
    {
      res=1000000;
      for(l=1;l<ys-1;l++)
	{

	  /* A true minumum surrounded by only larger values */
	  if(array[k][l]<=array[k][l+1]&&
	     array[k][l]<=array[k][l-1]&&
	     array[k][l]<=array[k+1][l]&&
	     array[k][l]<=array[k-1][l]&&
	     array[k][l]<=array[k+1][l+1]&&
	     array[k][l]<=array[k-1][l-1]&&
	     array[k][l]<=array[k-1][l+1]&&
	     array[k][l]<=array[k+1][l-1]
	     ) s=0;
	  /* A minimum at the lower boundary in x-direction */
	  else if(k==1&&
		  array[k][l]<=array[k][l+1]&&
		  array[k][l]<=array[k][l-1]&&
		  array[k][l]<=array[k+1][l]&&    
		  array[k][l]<=array[k+1][l+1]&&
		  array[k][l]<=array[k+1][l-1]
		  ) s=0;
	  /* A minimum at the upper boundary in x-direction */
	  else if(k==xs-2&&
		  array[k][l]<=array[k][l+1]&&
		  array[k][l]<=array[k][l-1]&&
		  array[k][l]<=array[k-1][l]&&
		  array[k][l]<=array[k-1][l-1]&&
		  array[k][l]<=array[k-1][l+1]
		  ) s=0;
	  /* A minimum at the upper boundary in y-direction */
	  else if(l==ys-2&&
		  array[k][l]<=array[k][l-1]&&
		  array[k][l]<=array[k+1][l]&&
		  array[k][l]<=array[k-1][l]&&
		  array[k][l]<=array[k-1][l-1]&&
		  array[k][l]<=array[k+1][l-1]
		  ) s=0;
	  /* A minimum at the lower boundary in y-direction */
	  else if(l==1&&
		  array[k][l]<=array[k][l+1]&&
		  array[k][l]<=array[k+1][l]&&
		  array[k][l]<=array[k-1][l]&&
		  array[k][l]<=array[k+1][l+1]&&
		  array[k][l]<=array[k-1][l+1]
		  ) s=0;

	  if(s==0){
	    //printf("min %d %d %f\n",k,l,array[k][l]);
	    c++;
	    list=(glb_params*) realloc(list,sizeof(glb_params*)*c);
	    list[c-1]=glbAllocParams();
	    glbCopyParams(tv,list[c-1]);
	    glbSetOscParams(list[c-1],asin(sqrt(pow(10,xvals[k])))/2,GLB_THETA_13);  
	    glbSetOscParams(list[c-1],yvals[l]/180*M_PI,GLB_DELTA_CP);
	    s=-1;
	  }

	}
    }
  free(xvals);
  free(yvals);
  *lis=list;
  return c;
}


/* Comparsion of oscillations paramters with a certain tolerance and
 * mod(2pi) for the cp phase.
 */

int glb_cmp_osc_params(glb_params a,glb_params b, double tol)
{
  int i;
  double x,y,d,s;
  for(i=0;i<GLB_OSCP;i++) 
    {
      x=glbGetOscParams(a,i);
      y=glbGetOscParams(b,i);
      d=fabs(x-y);
      s=fabs(x)+fabs(y);
      if(i==GLB_DELTA_CP)
	{
	  d=fabs(fmod(x-y,2*M_PI));
	  if(!(d<=tol*2*M_PI)) return -1;
	}
      else
	{
	  if(s<=tol)
	    {
	      if(!(d<=tol)) return -1;
	    }
	  else
	    {
	      if(!(d<=tol*s/2)) return -1;
	    }
	}
      
    }
  return 0;
}

static int test_membership(const glb_params *l,size_t n,glb_params b, double tol)
{
  size_t k;
  for(k=0;k<n;k++)
    {
      if(glb_cmp_osc_params(l[k],b,tol)==0) return 0; 
    }  
  return -1;

}

static int rm_duplicates(const glb_params *l,const double *chi,glb_params **r,double **chi_r, size_t n,double tol) 
{
  size_t m,k;
  glb_params *tmp=NULL;
  double *ctmp=NULL;
  m=0;
  for(k=0;k<n;k++)
    {
      if(test_membership(tmp,m,l[k],tol)==-1)
	{
	  m++;
	  ctmp=(double *) realloc(ctmp,sizeof(double)*m);
	  if(!ctmp) {fprintf(stderr,"no mem\n");exit(1);}
	  tmp=(glb_params *) realloc(tmp,sizeof(glb_params)*m);
	  if(!tmp) {fprintf(stderr,"no mem\n");exit(1);}
	  tmp[m-1]=glbAllocParams();
	  if(!tmp[m-1])  {fprintf(stderr,"no mem\n");exit(1);}
	  glbCopyParams(l[k],tmp[m-1]);
	  ctmp[m-1]=chi[k];
	}
    }
  *r=tmp;
  *chi_r=ctmp;

  return m;
}

void free_list(glb_params *stale, size_t n)
{
  size_t i;
  for(i=0;i<n;i++) glbFreeParams(stale[i]);
  free(stale);
}

/* Computes the systematics-only chi^2 on a grid in the
 * log(sin^22theta_13)-delta plane, ranging from xrange[0] to
 * xrange[1] in the x-direction and anlogously in the y-direction as
 * given in yrange. The number of points is given by resol, two values
 * for two directions. the result is returned in the array r_chi,
 * which is allocated by this function. true_values denotes as usual
 * the parameters for which the data is computed and test_values
 * allows to choose any cut you need, like inverted hierarchy.
 */

int compute_grid(const glb_params true_values,
		 const glb_params test_values,
		 double xrange[2],
		 double yrange[2],
		 int resol[2],
		 double ***r_chi)
{
  double resolution[2];
  int i;
  int xc=0;
  int yc=0;
  glb_params ctest_values=glbAllocParams();
  double **chi_values,res,x,y;
  double osc[GLB_OSCP];
  double thetheta13;

  glbCopyParams(test_values,ctest_values);

  for(i=0;i<GLB_OSCP;i++) osc[i]=glbGetOscParams(true_values,i);
  resolution[0]=resol[0]-1;
  resolution[1]=resol[1]-1;
 
  /* The simulated data are computed */
  glbSetOscillationParameters(true_values);
  glbSetRates();

  /* Iteration over all values to be computed */
	  
  res=0; 
  chi_values=(double**) malloc(sizeof(double*)*(resolution[0]+1));
  for(i=0;i<resolution[0]+1;i++) chi_values[i]=(double*) malloc(sizeof(double)*(resolution[1]+1));
  
  for(x=xrange[0];x<=xrange[1]+(xrange[1]-xrange[0])/resolution[0]/2;
      x=x+(xrange[1]-xrange[0])/resolution[0])
    {
      xc++;
      yc=0;
     
      for(y=yrange[0];y<=yrange[1]+(yrange[1]-yrange[0])/resolution[1]/2;
	  y=y+(yrange[1]-yrange[0])/resolution[1])
	{
	  yc++;
	  /* Set vector of test values */
	  thetheta13=asin(sqrt(pow(10,x)))/2;
	  glbSetOscParams(ctest_values,thetheta13,GLB_THETA_13);
	  glbSetOscParams(ctest_values,y*M_PI/180.0,GLB_DELTA_CP);
	  /* Compute Chi^2 for all loaded experiments and all rules */
	  res=0;
#ifndef MEMCHECK
	  //  glbSelectMinimizer(GLB_MIN_NESTED_POWELL);
	  res=glbChiSys(ctest_values,GLB_ALL,GLB_ALL);
	  //glbSelectMinimizer(GLB_MIN_POWELL);
	  
#endif
	  chi_values[xc-1][yc-1]=res;
	}
      
    }
  glbFreeParams(ctest_values);
  *r_chi=chi_values;
  return 0;
}


/* Comnputes the grid, finds all local minima (incl. boundaries), runs
 * ChiAll on those and removes all duplicates. If hier is YES, it
 * looks for the sign-degenerate solution, if oct is YES it looks for
 * the wrong oktant solution asf. The solutions are returned as list
 * of glb_params. This could be changed to a result chain.
 */

int eightfold(const glb_params true_values,
	      const glb_params input_errors,
	      int hier,
	      int oct,
	      double xrange[2],
	      double yrange[2],
	      int resol[2],
	      glb_params **r_list,
	      double **r_chi)
{

  glb_params *list=NULL;
  FILE *output=stdout;
  glb_params test_values=glbAllocParams();
  glb_params starting_values=glbAllocParams();
  glb_params minimum=glbAllocParams();
  double **chi_values;
  double res;
  double osc[GLB_OSCP];
  int i,count=0;

  for(i=0;i<GLB_OSCP;i++) osc[i]=glbGetOscParams(true_values,i);

  glbCopyParams(true_values,test_values);  
  
  if(hier==YES) { 
    double nv;
    /* following hep-ph/0509359 eq. 3 */
    nv=-osc[5]+2*osc[4]*cos(osc[0])*cos(osc[0]);
    glbSetOscParams(test_values,nv,GLB_DM_ATM);
  }
  if(oct==YES) glbSetOscParams(test_values,M_PI/2-osc[2],GLB_THETA_23);

  compute_grid(true_values,test_values,xrange,yrange,resol,&chi_values);
  count=find_min(chi_values,xrange,yrange,resol,test_values,&list);
  for(i=0;i<resol[0];i++) free(chi_values[i]);
  free(chi_values);
 
 
  glbCopyParams(test_values,starting_values);
  glbSetInputErrors(input_errors);
  glbSetStartingValues(starting_values);
  
  /* run ChiAll */
 
  double *chi_table=(double *) malloc(sizeof(double)*count);
  
  
  if(!chi_table) {fprintf(stderr,"no mem\n"); exit(1);}
  for(i=0;i<count;i++) { 
    double nv,res2=0;
    res=0;
#ifndef MEMCHECK
   res=glbChiAll(list[i],minimum,GLB_ALL); 
#endif

#ifdef MEMCHECK
   glbCopyParams(list[i],minimum);
#endif
    /* For minima which are close to th13=0 it sometimes happens that
     * ChiAll finds a minimum at small, negative values of th13, which
     * shifts delta_cp by pi. This is taken care of with this if() 
     * statement.
     */
    if(glbGetOscParams(minimum,GLB_THETA_13)<0) {
      double del;
      del=glbGetOscParams(minimum,GLB_DELTA_CP)+M_PI;
      glbSetOscParams(minimum,del,GLB_DELTA_CP);
    }

    glbCopyParams(minimum, list[i]);
    chi_table[i]=res;
  }
  fflush(output);
  

  int ct;
  glb_params *listb=NULL; 
  double *fchi=NULL;
  ct=rm_duplicates(list,chi_table,&listb,&fchi,count,0.01);
  
  /* Destroy parameter vector(s) */
  glbFreeParams(test_values); 
  glbFreeParams(starting_values);
  glbFreeParams(minimum);
  free_list(list,count);
  free(chi_table);
  
  
  if(r_list) *r_list=listb;
  if(r_chi) *r_chi=fchi;
  return ct;
}


/* Takes a chain as input and walks along into the direction as given
 * by fwd. The minimum of the previous step is used as starting guess
 * for the minimizer in the current step. inital_values is used for
 * the first step. At the same time the statistics-only and the
 * systematics-only chi^2 are computed for the values given by the
 * xval of the current chain element and the other parameters are
 * taken from chi_sys. For degenerate solution the results on chi_sys
 * and chi_no_sys may not be meaningful. If this function is used for
 * back-propagation it will also check wether the new chi^2 is really
 * smaller than the original one otherwise it terminates. It prints
 * lots of diagnostics to stdout.
 */


result track_in_chain(result in, const glb_params initial_values, const glb_params chi_sys, int fwd, 
		      projection *p, FILE *fp)
{
  result tmp;
  double thetheta13,res,sys,no_sys;
  glb_params test_values=glbAllocParams();
  glb_params minimum=glbAllocParams();
  glb_params starting_values=glbAllocParams();

  glbCopyParams(initial_values,test_values);
  glbCopyParams(chi_sys,starting_values);
  tmp=in;

  while(tmp) {
    set_diagnosis(tmp,fwd);

    thetheta13=(p->f)(get_xval(tmp));
    glbSetOscParams(test_values,thetheta13,p->which);
    glbSetOscParams(starting_values,thetheta13,p->which);
    glbSetProjection(p->direction);
    res=glbChiNP(test_values,minimum,GLB_ALL);
    
    /* Backpropagation is interrupted if the result is not improved */
    if(fwd==BACKTRACK||fwd==FWDTRACK) {
      if(res<get_chi(tmp)) set_chi(tmp,res);
      else break;}
    else set_chi(tmp,res);
    
    set_minimum(tmp,minimum);
    get_minimum(tmp,test_values);
    sys=glbChiSys(starting_values,GLB_ALL,GLB_ALL);
    set_chi_sys(tmp,sys);
    glbSwitchSystematics(GLB_ALL,GLB_ALL,GLB_OFF);
    no_sys=glbChiSys(starting_values,GLB_ALL,GLB_ALL);
    set_chi_no_sys(tmp,no_sys); 
    glbSwitchSystematics(GLB_ALL,GLB_ALL,GLB_ON);
    print_result(fp,tmp);
    
    if(fwd==FORWARD||fwd==FWDTRACK) tmp=get_forward(tmp);
    else if(fwd==BACKWARD||fwd==BACKTRACK) tmp=get_backward(tmp);
    else tmp=NULL;
  }
  /* Cleanup */
  glbFreeParams(test_values);
  glbFreeParams(minimum);
  glbFreeParams(starting_values);
  return tmp;
}

/* Finds the point in a chain where it changes from BACKWARD to
 * FORWARD and return a pointer to the first FORWARD element.
 */

result find_split(result in)
{
  int fwd;
  result tmp;
  tmp=in;
  tmp=rewind_chain(tmp);
  fwd=get_diagnosis(tmp);
  /* If there are only forwards, we're done */
  if(fwd==FORWARD) return tmp;
  while(tmp)
    {
      fwd=get_diagnosis(tmp);
      if(fwd==BACKWARD) {tmp=get_backward(tmp);break;}
      tmp=get_forward(tmp);
    }
  return tmp;
}

/* This does the back-propagation to weed out jumps, always walks
 * opposite to the initial direction.
 */

result backpropagate_in_chain(result in, const glb_params chi_sys, int fwd, projection *p,FILE *fp)
{
  result tmp2,tmp,nn;
  glb_params test_values=glbAllocParams();  
  glb_params starting_values=glbAllocParams();
  int c=0;

  glbCopyParams(chi_sys,starting_values);

  tmp=in;
  nn=find_split(tmp);
  if(fwd==FORWARD) {
    tmp2= find_jump(nn,FORWARD);
    while(tmp2) {
      fprintf(fp,"jump found at %f\n",get_xval(tmp2));
      tmp=get_backward(tmp2);
      test_values=get_minimum(tmp2,test_values);
      track_in_chain(tmp,test_values,starting_values,BACKTRACK,p,fp);
      c++;
      if(c>5) {fprintf(stderr,"To many jumps\n");fprintf(fp,"To many jumps\n");break;}
      tmp2=find_jump(tmp2,FORWARD);
    }
  }
  else if(fwd==BACKWARD) {
    tmp2= find_jump(get_backward(nn),BACKWARD);
    while(tmp2) {
      fprintf(fp,"jump found at %f\n",get_xval(tmp2));
      tmp=get_backward(tmp2);
      test_values=get_minimum(tmp2,test_values);
      track_in_chain(tmp,test_values,starting_values,FWDTRACK,p,fp);
      c++;
      if(c>5) {fprintf(stderr,"To many jumps\n");break;}
      tmp2=find_jump(tmp2,BACKWARD);
    }
  }
  
  tmp=rewind_chain(tmp);
  
  glbFreeParams(test_values);
  glbFreeParams(starting_values);
  
  return tmp;
}

/* Here comes stuff to allow restarts from exsiting files. Condor
   sucks! */

FILE* goto_end(const char* filename,int flag)
{
  FILE *fp=NULL;
  fp=fopen(filename,"r");
  if(!fp||flag==FORCE) 
    {
      /* file doesn't exist or we're forcing it: create for writing */
      fp=fopen(filename,"w"); 
    }
  else
    { 
      /* file does exist, read position and open it for appending */
      fclose(fp);
      fp=fopen(filename,"a");
      
    }
  return fp;
}

int read_position(const char *filename, double *posx, double *posy, int flag)
{
  FILE *fp;
  int i,s=-1,c; 
  char tok;
  double buf[2],nirvana[3];
  fp=fopen(filename,"r");
  if(fp&&flag!=FORCE)
    {
      /* peeling of leading comments */
       while(1)
	 {
	   tok=fgetc(fp);
	   if(tok=='#') 
	     while(fgetc(fp)!='\n');
	   else
	     {
	       ungetc(tok,fp);
	       break;
	     }
	 }
       
       while (1)
	 {
	   /* get rid of comments inside */
	   tok=fgetc(fp);
	   if(tok==EOF) break;
	   if(tok=='#') 
	     {
	       while(fgetc(fp)!='\n');
	       i--;
	     }
	   else
	     {
	       ungetc(tok,fp);
	       /* read the data */
	       c=fscanf(fp,"%lf %lf %lf %lf %lf \n",
			&buf[0],
			&buf[1],
			&nirvana[0],
			&nirvana[1],
			&nirvana[2]);
	       
	       if(c!=5) 
		 {
		   fprintf(stderr,"Error: Wrong format in file %s\n",
			   filename);
		 }
	       else
		 {
		   /* last fully parsed line sets positions */
		   s=1;
		   *posx=buf[0];
		   *posy=buf[1];
		 }
	     }
	 }
       fclose(fp);  
    }
  return s;
}

/* allowing to chose any plane */


double identity(double x)
{ 
 return x;
}

double log10IIrad(double x)
{
  return asin(sqrt(pow(10,x)))/2;
}

double radIIsin22theta(double x)
{
  return pow(sin(2*x),2);
}

double radIIlog10(double x)
{
  return log10(pow(sin(2*x),2));
}

double degIIrad(double x)
{
  return x/180.0*M_PI;
}

double radIIdeg(double x)
{
  return x*180.0/M_PI;
}


int setup_axis(projection *axis, int which)
{   
  glb_projection pr=glbAllocProjection();
  glbDefineProjection(pr,GLB_FREE,GLB_FREE,GLB_FREE,GLB_FREE,GLB_FREE,GLB_FREE);

  /* to silence an error message about partially defined projections */
  size_t i;
  for(i=0;i<glb_num_of_exps;i++) glbSetDensityProjectionFlag(pr,GLB_FREE,i);

  axis->which=which;
  glbSetProjectionFlag(pr,GLB_FIXED,axis->which); 
  axis->direction=pr;
  axis->f=*identity; 
  axis->inverse_f=*identity;
  
  if(axis->which==GLB_THETA_13){
    axis->f= *log10IIrad;
    axis->inverse_f= *radIIlog10;
  }
  
  if(axis->which==GLB_DELTA_CP){
    axis->f= *degIIrad;
    axis->inverse_f= *radIIdeg;
  }
  return 0;
}

void free_axis(projection *stale)
{
  glbFreeProjection(stale->direction);
}


/* Adding our own prior which prohibits negative th13 */

/* File-scope static variables to store starting values and input errors */
static glb_params sv;
static glb_params er;


int myprior_init()
{
  sv=glbAllocParams();
  er=glbAllocParams();
  glbGetStartingValues(sv);
  glbGetInputErrors(er);


  return 0;
}

/* myprior_clean is optional. It should free all memory reserved by
 * myprior_init and also reverse all side effects of myprior_init.
 *
 * Here we just free the memory for sv and er.
 */
void myprior_clean()
{
  glbFreeParams(er);
  glbFreeParams(sv);
}

/* This is a local function which implements the prior calculation
 * and ensure that the builtin prior behaves as described in the
 * documentation, i.e. it returns 0 when the error is below 1E-12
 */
static double sprior(double x, double center, double sigma)
{
  if(fabs(sigma-0)<1E-12) return 0;
  return (x-center)*(x-center)/sigma/sigma;
}

static double tprior(double x, double center, double sigma)
{
  if(x>0) return 0;
  return x*x/1E-12;
}

/* myprior_prior is mandatory (for prior modules).
 *
 * This is the default prior, i.e. priors are only added for free
 * parameters, and the prior accepts starting values as stored in sv
 * and input errors as stored in er.
 */
double myprior_prior(const glb_params in, void *user_data)
{
  int i;
  double res=0;
  glb_projection pro;
  pro=glbAllocProjection();
  glbGetProjection(pro);
  for(i=0;i<GLB_OSCP;i++)
    {
      if(glbGetProjectionFlag(pro,i)==GLB_FREE) 
	{
	  if(i==GLB_THETA_13)
	    { 
	      res +=  
		tprior(glbGetOscParams(in,i),
		       glbGetOscParams(sv,i),
		       glbGetOscParams(er,i)
		       );

	      /* this lines adds a prior on sin^22 theta_13 */
	      res += sprior(radIIsin22theta(glbGetOscParams(in,i)),
			    radIIsin22theta(glbGetOscParams(sv,i)),
		       glbGetOscParams(er,i)
		       );  
	   
	    }
	  else
	    {
	      res +=
		sprior(glbGetOscParams(in,i),
		       glbGetOscParams(sv,i),
		       glbGetOscParams(er,i)
		       );                                              
	    }
	}
    }

  for(i=0;i<glb_num_of_exps;i++)
    {

      if(glbGetDensityProjectionFlag(pro,i)==GLB_FREE)
        res +=
          sprior(glbGetDensityParams(in,i),
                 glbGetDensityParams(sv,i),
                 glbGetDensityParams(er,i)
                 );
    }

  glbFreeProjection(pro);

  return res;
}

/* myprior_starting_values is optional. It is used to synchronize
 * the values set by glbSetStartingValues with the values used in here.
 * This function (if it exists) is called by glbSetStartingValues
 * with the same argument.
 *
 * Here we just copy the argument to our local buffer.
 */
int myprior_starting_values(const glb_params in, void *user_data)
{
  if(glbCopyParams(in,sv)!=NULL) return 0;
  return -1;
}

/* myprior_input_errors is optional. It is used to synchronize
 * the values set by glbSetInputErrors with the values used in here.
 * This function (if it exists) is called by glbSetInputErrors
 * with the same argument.
 *
 * Here we just copy the argument to our local buffer.
 */
int myprior_input_errors(const glb_params in, void *user_data)
{
  if(glbCopyParams(in,er)!=NULL) return 0;
  return -1;
}

