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
#include <stdio.h>
#include "glb_error.h"
#include "glb_min_sup.h"


/***************************************************************************
 *         C O M B I N E D   O S C / S Y S   M I N I M I Z E R             *
 ***************************************************************************/

#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);

#define ITMAX_BRENT   100.0  /* Max. number of iterations in 1d Brent minimizer */
#define ITMAX_POWELL  100.0  /* Max. number of iterations per dimensions in     */
                             /*   hybrid Powell minimizer                       */

#define GOLD      1.618034
#define CGOLD     0.3819660
#define TINY      1.0e-20
#define GLIMIT    100.0
#define ZEPS      1.0e-10
#define TOL_BRENT 2.0e-4


/***************************************************************************
 * Function glb_hybrid_eval_1d                                             *
 ***************************************************************************
 * This helper routine evaluates the function we are minimizing at the     *
 * point p + u * xi                                                        *
 ***************************************************************************
 * Parameters:                                                             *
 *   f:     The function to minimize                                       *
 *   P:     The starting point                                             *
 *   u:     The offset along direction xi                                  *
 *   xi:    The direction over which we are currently minimizing           *
 *   n:     Total number of dimensions                                     *
 *   new_rates_flag: Determines whether rates should be recomputed in f    *
 *   user_data: Arbitrary data to be passed to func                        *
 ***************************************************************************/
double glb_hybrid_eval_1d(glb_minimize_func f, double *P, double u, double *xi,
                          int n, int new_rates_flag, void *user_data)
{
  int j;
  double ux[n];

  for (j=0; j < n; j++)
    ux[j] = P[j] + u * xi[j];

  return (*f)(ux, new_rates_flag, user_data);
}


/***************************************************************************
 * Function glb_hybrid_bracket                                             *
 ***************************************************************************
 * Finds an interval bracketing the minimum of f along direction xi        *
 ***************************************************************************
 * Parameters:                                                             *
 *   P:     The starting point                                             *
 *   xi:    The direction over which minimization is requested             *
 *   a, b, c: Output: Bracketing interval for the minimum in units of xi   *
 *          The points fulfill a < b < c
 *   n:     Total number of dimensions                                     *
 *   f:     The function to minimize                                       *
 *   new_rates_flag: Determines whether rates should be recomputed in f    *
 *   user_data: Arbitrary data to be passed to func                        *
 ***************************************************************************/
int glb_hybrid_bracket(double *P, double *xi, double *a, double *b, double *c, int n,
                       glb_minimize_func f, int new_rates_flag, void *user_data)
{
  double ax, bx, cx;
  long double fa, fb, fc, fu; /* long double declaration avoids problems with  */
  double u, ulim, q, r;       /* extra floating point digits on x86 processors */
  int i, j;

  ax = 0.0;
  bx = 1.0;
  fa = glb_hybrid_eval_1d(f, P, ax, xi, n, new_rates_flag, user_data);
  fb = glb_hybrid_eval_1d(f, P, bx, xi, n, new_rates_flag, user_data);

  if (fb > fa)      /* Make sure we are moving downhill */
  {
    SHFT(u, ax, bx, u);
    SHFT(u, fa, fb, u);
  }

  cx = bx + GOLD * (bx - ax);
  fc = glb_hybrid_eval_1d(f, P, cx, xi, n, new_rates_flag, user_data);

  while (fb > fc)   /* Continue as long as minimum is not between ax, bx, and cx */
  {
    /* Quadratic extrapolation of ax, bx, cx */
    r = (bx - ax) * (fb - fc);
    q = (bx - cx) * (fb - fa);
    u = bx - ((bx - cx)*q - (bx - ax)*r) / (2.0*SIGN(MAX(fabs(q-r),TINY), q-r));

    ulim = bx + GLIMIT * (cx - bx);         /* Maximum magnification */
    if ((bx - u) * (u - cx) > 0.0)           /* u is between b and c */
    {
      fu = glb_hybrid_eval_1d(f, P, u, xi, n, new_rates_flag, user_data);
      if (fu < fc)                        /* Minimum between b and c */
      {
        ax = bx;    fa = fb;
        bx = u;     fb = fu;
        break;
      }
      else if (fu > fb)                   /* Minimum between a and b */
      {
        cx = u;     fc = fu;
        break;
      }
      else    /* Parabolic fit was no use, use default magnification */
      {
        u  = cx + GOLD*(cx - bx);
        fu = glb_hybrid_eval_1d(f, P, u, xi, n, new_rates_flag, user_data);
      }
    }
    else if ((cx - u) * (u - ulim) > 0.0)   /* u between cx and ulim */
    {
      fu = glb_hybrid_eval_1d(f, P, u, xi, n, new_rates_flag, user_data);
      if (fu < fc)
      {
        SHFT(bx, cx, u, cx+GOLD*(cx-bx));
        SHFT(fb, fc, fu, glb_hybrid_eval_1d(f, P, u, xi, n, new_rates_flag, user_data));
      }
    }
    else if ((u-ulim)*(ulim-cx) >= 0.0) /* u > ulim ==> limit to ulim */
    {
      u  = ulim;
      fu = glb_hybrid_eval_1d(f, P, u, xi, n, new_rates_flag, user_data);
    }
    else                /* Reject parabola, use default magnification */
    {
      u  = cx + GOLD*(cx-bx);
      fu = glb_hybrid_eval_1d(f, P, u, xi, n, new_rates_flag, user_data);
    }
    SHFT(ax, bx, cx, u);               /* Eliminate the oldest point */
    SHFT(fa, fb, fc, fu);
  }

  *a = ((ax < cx) ? ax : cx);                    /* Ensure a < b < c */
  *c = ((ax > cx) ? ax : cx);
  *b = bx;

  return 0;
}


/***************************************************************************
 * Function glb_hybrid_linmin                                              *
 ***************************************************************************
 * Performs the minimization in one direction, using the Brent-Dekker      *
 * algorithm.                                                              *
 ***************************************************************************
 * Parameters:                                                             *
 *   P:     Input: The starting point                                      *
 *          Output: The position of the minimum                            *
 *   xi:    The direction over which minimization is requested             *
 *   n:     Total number of dimensions                                     *
 *   ftol:  Convergence criterion for Brent-Dekker minimizer               *
 *   fret:  Input: Function value at the starting point                    *
 *          Output: Function value at the estimated minimum                *
 *   f:     The function to minimize                                       *
 *   new_rates_flag: Determines whether rates should be recomputed in f    *
 *   user_data: Arbitrary data to be passed to func                        *
 ***************************************************************************/
int glb_hybrid_linmin(double *P, double *xi, int n, double ftol,
          double *fret, glb_minimize_func f, int new_rates_flag, void *user_data)
{
  double a, b, d, p, q, r, u, v, w, x, xm;
  double fu, fv, fw, fx;
  double tol1, tol2;
  double e, etemp;
  int iter, j;

  /* Find bracketing interval */
  if (glb_hybrid_bracket(P, xi, &a, &x, &b, n, f, new_rates_flag, user_data) != 0)
    return -2;

  /* Execute Brent-Dekker algorithm */
  e = 0.0;
  w = v = x;
  fw = fv = fx = glb_hybrid_eval_1d(f, P, x, xi, n, new_rates_flag, user_data);
  for (iter=1; iter <= 100; iter++)
  {
    xm = 0.5 * (a + b);

    tol1 = ftol * fabs(x) + ZEPS;
    tol2 = 2.0*tol1;
    if (fabs(x - xm) <= tol2 - 0.5*(b - a))        /* Test for convergence */
    {
      for (j=0;j<n;j++) P[j]=P[j]+x*xi[j];
      *fret = fx;
      return 0;
    }

    if (fabs(e) > tol1)                   /* Construct trial parabolic fit */
    {
      r = (x-w) * (fx-fv);
      q = (x-v) * (fx-fw);
      p = (x-v)*q - (x-w)*r;
      q = 2.0 * (q - r);
      if (q > 0.0)
      {
        p = -p;
      }
      q = fabs(q);
      etemp = e;
      e = d;
      if (fabs(p) >= fabs(0.5*q*etemp)  ||  p <= q*(a-x)  ||  p >= q*(b-x))
      {    /* If parabolic fit is not acceptable, take golden section step */
        e = (x>=xm) ? a-x : b-x;
        d = CGOLD * e;
      }
      else
      {
        d = p / q;
        u = x + d;
        if (u-a < tol2  ||  b-u < tol2)    /* Never move by less than tol1 */
          d = SIGN(tol1, xm -x);
      }
    }
    else
    {
      e = (x>=xm) ? a-x : b-x;
      d = CGOLD * e;
    }

    u  = (fabs(d)>=tol1) ? x+d : x+SIGN(tol1,d);
    fu = glb_hybrid_eval_1d(f, P, u, xi, n, new_rates_flag, user_data);
    if (fu <= fx)
    {
      if (u >= x)
        a = x;
      else
        b = x;
      SHFT(v, w, x, u);
      SHFT(fv, fw, fx, fu);
    }
    else
    {
      if (u < x)
        a = u;
      else
        b = u;

      if (fu <= fw  ||  w == x)
      {
        v = w;   fv = fw;
        w = u;   fw = fu;
      }
      else if (fu <= fv  ||  v == x  ||  v == w)
      {
        v = u;   fv = fu;
      }
    }
  }

  glb_warning("glb_hybrid_linmin: No convergence in 1-dim Brent minimizer");

  for (j=0;j<n;j++) P[j]=P[j]+x*xi[j];
  *fret = fx;
  return -1;
}


/***************************************************************************
 * Function glb_iterate_hybrid_minimizer                                   *
 ***************************************************************************
 * The actual contents of the iteration loop of the Powell minimizer       *
 ***************************************************************************
 * Parameters:                                                             *
 *   P:     Input: The starting point                                      *
 *          Output: The new estimate for the minimum                       *
 *   xi:    The current set of minimization directions                     *
 *   n:     Total number of dimensions                                     *
 *   n_min, n_max: The directions over which minimization is requested     *
 *   ftol:  Relative decrease of function value signaling convergence      *
 *   fret:  Input: Function value at the starting point                    *
 *          Output: Function value at the estimated minimum                *
 *   f:     The function to minimize                                       *
 *   new_rates_flag: Determines whether rates should be recomputed in f    *
 *   user_data: Arbitrary data to be passed to func                        *
 ***************************************************************************/
int glb_iterate_hybrid_minimizer(double *P, double *_xi, int n, int n_min, int n_max,
                      double ftol, double *fret, glb_minimize_func f, int new_rates_flag,
                      void *user_data)
{
  double del;
  int ibig;
  double (*xi)[n] = (double (*)[n]) _xi;
  double pt[n], ptt[n], xit[n];
  double fp, fptt, t;
  int i, j;

  fp = *fret;
  for (j=0;j<n;j++) pt[j] = P[j];

  ibig = 0;
  del  = 0.0;
  for (i=n_min; i <= n_max; i++)
  {
    for (j=0;j<n;j++) xit[j] = xi[j][i];
    fptt = *fret;
    glb_hybrid_linmin(P, xit, n, TOL_BRENT, fret, f, new_rates_flag, user_data);
    if (fptt - *fret > del)
    {
      del  = fptt - *fret;
      ibig = i;
    }
  }

  /* Check for convergence */
  if (2.0 * (fp - *fret) <= ftol * (fabs(fp) + fabs(*fret)))
    return 0;

  for (j=0; j < n; j++)
  {
    ptt[j] = 2.0*P[j] - pt[j];
    xit[j] = P[j] - pt[j];
  }
  fptt = (*f)(ptt, new_rates_flag, user_data);
  if (fptt < fp)
  {
    t = 2.0 * (fp - 2.0*(*fret) + fptt) * SQR(fp - (*fret) - del) - del*SQR(fp-fptt);
    if (t < 0.0)
    {
      glb_hybrid_linmin(P, xit, n, TOL_BRENT, fret, f, new_rates_flag, user_data);
      for (j=0; j < n; j++)
      {
        xi[j][ibig]  = xi[j][n_max];
        xi[j][n_max] = xit[j];
      }
    }
  }

  return 1;
}


/***************************************************************************
 * Function glb_hybrid_minimizer                                           *
 ***************************************************************************
 * The central minimization function (based on Powell's method, as         *
 * described in Press: Numerical Recipes). The function is designed        *
 * to simultaneously minimize the oscillation and systematics parameters,  *
 * and to keep track of whether it is necessary to recompute the event     *
 * rates.                                                                  *
 ***************************************************************************
 * Parameters:                                                             *
 *   P:     Input: The starting point (the first n_osc components must be  *
 *          the oscillation parameters, the rest is for systematics)       *
 *          Output: The position of the minimum                            *
 *   n:     The total number of dimensions (oscillation + systematics)     *
 *   n_osc: Number of oscillation parameters                               *
 *   ftol:  Relative decrease of function value signaling convergence      *
 *   iter:  Output: The number of required Powell iterations               *
 *   fret:  Output: Function value at the minimum                          *
 *   f:     The function to minimize                                       *
 *   user_data: Arbitrary data to be passed to func                        *
 ***************************************************************************/
int glb_hybrid_minimizer(double *P, int n, int n_osc, double ftol, int *iter,
                         double *fret, glb_minimize_func f, void *user_data)
{
  double xi[n][n];
  int i, j;
  int converged;

  /* Choose orthogonal initial directions. Take the length of the direction
   * vectors of the order of the respective starting value; if this is zero,
   * take unit vector. */
  for (i=0; i < n; i++)
  {
    for (j=i+1; j < n; j++)
      xi[i][j] = xi[j][i] = 0.0;
    if (fabs(P[i]) > ftol)
      xi[i][i] = 0.1 * fabs(P[i]);
    else
      xi[i][i] = 1.0;
  }

  *fret = (*f)(P, 1, user_data);
  for (*iter=1; *iter < n*ITMAX_POWELL; (*iter)++)
  {
    converged = 1;

    /* Iterate minimizer in subspace of oscillation parameters */
    if (glb_iterate_hybrid_minimizer(P, &xi[0][0], n, 0, n_osc-1, ftol, fret, f, 1, user_data) > 0)
      converged = 0;

    /* Force recomputation of event rates at the current minimum (it is not
     * guaranteed, that the last function evaluation in glb_iterate_hybrid_minimizer
     * was at the minimum) */
    *fret = (*f)(P, 1, user_data);

    /* Iterate minimizer in subspace of systematics parameters */
    if (glb_iterate_hybrid_minimizer(P, &xi[0][0], n, n_osc, n-1, ftol, fret, f, 0, user_data) > 0)
      converged = 0;

    if (converged)
      return 0;
  }

  glb_warning("glb_hybrid_minimizer: No convergence in n-dim Powell minimizer");
  return -1;
}



/***************************************************************************
 *O R I G I N A L   M I N I M I Z E R   ( O S C / S Y S   S E P A R A T E )*
 ***************************************************************************/

static void glb_minimizer_error(char error_text[])
{
  glb_warning(error_text);
}

/* This here is code which works fine, but somehow valgrind complains
 * about leaking memory. Therefore we use the version just below this
 *  comment, which works fine and makes valgrind happy

 double *glb_alloc_vec(int nl,int nh)
 {
 double *v;
 v=(double *)glb_malloc((unsigned) (nh-nl+1)*sizeof(double));
 return v-nl;
 }

 double **glb_alloc_mat(int nrl,int nrh, int ncl,int nch)
 {
 int i;
 double **m;

 m=(double **) glb_malloc((unsigned) (nrh-nrl+1)*sizeof(double*));
 m -= nrl;
 for(i=nrl;i<=nrh;i++)
 {
 m[i]=(double *) glb_malloc((unsigned)
 (nch-ncl+1)*sizeof(double));
 m[i] -= ncl;
 }
 return m;
 }

 void glb_free_vec(double *v,int nl,int nh)
 {
 glb_free((char*) (v+nl));
 }

 void glb_free_mat(double **m,int nrl,int nrh,int ncl,int nch)
 {
 int i;
 for(i=nrh;i>=nrl;i--) glb_free((char*) (m[i]+ncl));
 glb_free((char*) (m+nrl));
 }
 */

double *glb_alloc_vec(int nl,int nh)
{
	double *v;
	v=(double *)glb_malloc((unsigned) (nh+1)*sizeof(double));
	return v;
}

double **glb_alloc_mat(int nrl,int nrh, int ncl,int nch)
{
	int i;
	double **m;

	m=(double **) glb_malloc((unsigned) (nrh+1)*sizeof(double*));
	//m -= nrl;
	for(i=0;i<=nrh;i++)
	  {
		m[i]=(double *) glb_malloc((unsigned)
					   (nch+1)*sizeof(double));
		//m[i] -= ncl;
	  }
	return m;
}

void glb_free_vec(double *v,int nl,int nh)
{
	glb_free((char*) (v));
}

void glb_free_mat(double **m,int nrl,int nrh,int ncl,int nch)
{
	int i;
	for(i=nrh;i>=0;i--) glb_free((char*) (m[i]));

	glb_free((char*) (m));
}


#define TOL 2.0e-4


typedef struct {
  int ncom;
  double *pcom;
  double *xicom;
  double (*nrfunc)(double*);
} glb_min_data;


static double one_dim_projection(double x,glb_min_data *in)
{
	int j;
	double f,*xt;

	xt=glb_alloc_vec(1,in->ncom);
	for (j=1;j<=in->ncom;j++) xt[j]=in->pcom[j]+x*in->xicom[j];
	f=in->nrfunc(xt);
	glb_free_vec(xt,1,in->ncom);
	return f;
}



#define ITMAX 100
#define CGOLD 0.3819660
#define ZEPS 1.0e-10
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);

static double glb_brent_min(double ax,double bx,double cx,
			    double (*f)(double,glb_min_data*),
			    double tol,double *xmin,glb_min_data *in)
{
	int iter;
	double a,b,d,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
	double e=0.0;

	a=((ax < cx) ? ax : cx);
	b=((ax > cx) ? ax : cx);
	x=w=v=bx;
	fw=fv=fx=(*f)(x,in);
	for (iter=1;iter<=ITMAX;iter++) {
		xm=0.5*(a+b);
		tol2=2.0*(tol1=tol*fabs(x)+ZEPS);
		if (fabs(x-xm) <= (tol2-0.5*(b-a))) {
			*xmin=x;
			return fx;
		}
		if (fabs(e) > tol1) {
			r=(x-w)*(fx-fv);
			q=(x-v)*(fx-fw);
			p=(x-v)*q-(x-w)*r;
			q=2.0*(q-r);
			if (q > 0.0) p = -p;
			q=fabs(q);
			etemp=e;
			e=d;
			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
				d=CGOLD*(e=(x >= xm ? a-x : b-x));
			else {
				d=p/q;
				u=x+d;
				if (u-a < tol2 || b-u < tol2)
					d=SIGN(tol1,xm-x);
			}
		} else {
			d=CGOLD*(e=(x >= xm ? a-x : b-x));
		}
		u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
		fu=(*f)(u,in);
		if (fu <= fx) {
			if (u >= x) a=x; else b=x;
			SHFT(v,w,x,u)
			SHFT(fv,fw,fx,fu)
		} else {
			if (u < x) a=u; else b=u;
			if (fu <= fw || w == x) {
				v=w;
				w=u;
				fv=fw;
				fw=fu;
			} else if (fu <= fv || v == x || v == w) {
				v=u;
				fv=fu;
			}
		}
	}
	glb_minimizer_error("glb_brent_min");
	*xmin=x;
	return fx;
}

#undef ITMAX
#undef CGOLD
#undef ZEPS




#define GOLD 1.618034
#define GLIMIT 100.0
#define TINY 1.0e-20
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);

static void bracket(double *ax,double *bx,double *cx,double *fa,double *fb,
	    double *fc, double (*func)(double,glb_min_data*),glb_min_data *in)
{
	double ulim,u,r,q,fu,dum;

	*fa=(*func)(*ax,in);
	*fb=(*func)(*bx,in);
	if (*fb > *fa) {
		SHFT(dum,*ax,*bx,dum)
		SHFT(dum,*fb,*fa,dum)
	}
	*cx=(*bx)+GOLD*(*bx-*ax);
	*fc=(*func)(*cx,in);
	while (*fb > *fc) {
		r=(*bx-*ax)*(*fb-*fc);
		q=(*bx-*cx)*(*fb-*fa);
		u=(*bx)-((*bx-*cx)*q-(*bx-*ax)*r)/
			(2.0*SIGN(MAX(fabs(q-r),TINY),q-r));
		ulim=(*bx)+GLIMIT*(*cx-*bx);
		if ((*bx-u)*(u-*cx) > 0.0) {
			fu=(*func)(u,in);
			if (fu < *fc) {
				*ax=(*bx);
				*bx=u;
				*fa=(*fb);
				*fb=fu;
				return;
			} else if (fu > *fb) {
				*cx=u;
				*fc=fu;
				return;
			}
			u=(*cx)+GOLD*(*cx-*bx);
			fu=(*func)(u,in);
		} else if ((*cx-u)*(u-ulim) > 0.0) {
			fu=(*func)(u,in);
			if (fu < *fc) {
				SHFT(*bx,*cx,u,*cx+GOLD*(*cx-*bx))
				SHFT(*fb,*fc,fu,(*func)(u,in))
			}
		} else if ((u-ulim)*(ulim-*cx) >= 0.0) {
			u=ulim;
			fu=(*func)(u,in);
		} else {
			u=(*cx)+GOLD*(*cx-*bx);
			fu=(*func)(u,in);
		}
		SHFT(*ax,*bx,*cx,u)
		SHFT(*fa,*fb,*fc,fu)
	}
}



#undef GOLD
#undef GLIMIT
#undef TINY
#undef SHFT


static void line_minimization(double p[],double xi[],int n,double *fret,
			      double (*func)(double*),
			      glb_min_data *in)
{
	int j;
	double xx,xmin,fx,fb,fa,bx,ax;
	int ncom=0;
	double *pcom=0,*xicom=0,(*nrfunc)(double*);

	ncom=n;
	pcom=glb_alloc_vec(1,n);
	xicom=glb_alloc_vec(1,n);
	nrfunc=func;
	for (j=1;j<=n;j++) {
		pcom[j]=p[j];
		xicom[j]=xi[j];
	}

	in->ncom=n;
	in->pcom=pcom;
	in->xicom=xicom;
	in->nrfunc=func;
	ax=0.0;
	xx=1.0;
	bx=2.0;
	bracket(&ax,&xx,&bx,&fa,&fx,&fb,one_dim_projection,in);
	*fret=glb_brent_min(ax,xx,bx,one_dim_projection,TOL,&xmin,in);
	for (j=1;j<=n;j++) {
		xi[j] *= xmin;
		p[j] += xi[j];
	}
	glb_free_vec(xicom,1,n);
	glb_free_vec(pcom,1,n);
}







#define ITMAX 5000
static double sqrarg;
#undef SQR
#define SQR(a) (sqrarg=(a),sqrarg*sqrarg)

int glb_powell(double p[],double **xi,int n,
		double ftol,int *iter,double *fret,
		double (*func)(double*))
{
  int i,ibig,j;
  double t,fptt,fp,del;
  double *pt,*ptt,*xit;
  glb_min_data in;
  pt=glb_alloc_vec(1,n);
  ptt=glb_alloc_vec(1,n);
  xit=glb_alloc_vec(1,n);
	*fret=(*func)(p);
	for (j=1;j<=n;j++) pt[j]=p[j];
	for (*iter=1;;(*iter)++) {
		fp=(*fret);
		ibig=0;
		del=0.0;
		for (i=1;i<=n;i++) {
			for (j=1;j<=n;j++) xit[j]=xi[j][i];
			fptt=(*fret);
			line_minimization(p,xit,n,fret,func,&in);
			if (fabs(fptt-(*fret)) > del) {
				del=fabs(fptt-(*fret));
				ibig=i;
			}
		}
		if (2.0*fabs(fp-(*fret)) <= ftol*(fabs(fp)+fabs(*fret))) {
			glb_free_vec(xit,1,n);
			glb_free_vec(ptt,1,n);
			glb_free_vec(pt,1,n);
			return 0;
		}
		if (*iter == ITMAX) {glb_minimizer_error("Minimizer could not converge!");return -1;}
		for (j=1;j<=n;j++) {
			ptt[j]=2.0*p[j]-pt[j];
			xit[j]=p[j]-pt[j];
			pt[j]=p[j];
		}
		fptt=(*func)(ptt);
		if (fptt < fp) {
			t=2.0*(fp-2.0*(*fret)+fptt)*SQR(fp-(*fret)-del)-del*SQR(fp-fptt);
			if (t < 0.0) {
				line_minimization(p,xit,n,fret,func,&in);
				for (j=1;j<=n;j++) xi[j][ibig]=xit[j];
			}
		}
	}
	return 0;
}


int glb_powell2(double p[],double **xi,int n,double ftol,
		 int *iter,double *fret,
		 double (*func)(double*))
{
  int i,ibig,j;
  double t,fptt,fp,del;
  double *pt,*ptt,*xit;
  glb_min_data in;
  pt=glb_alloc_vec(1,n);
  ptt=glb_alloc_vec(1,n);
  xit=glb_alloc_vec(1,n);
	*fret=(*func)(p);
	for (j=1;j<=n;j++) pt[j]=p[j];
	for (*iter=1;;(*iter)++) {
		fp=(*fret);
		ibig=0;
		del=0.0;
		for (i=1;i<=n;i++) {
			for (j=1;j<=n;j++) xit[j]=xi[j][i];
			fptt=(*fret);
			line_minimization(p,xit,n,fret,func,&in);
			if (fabs(fptt-(*fret)) > del) {
				del=fabs(fptt-(*fret));
				ibig=i;
			}
		}
		if (2.0*fabs(fp-(*fret)) <= ftol*(fabs(fp)+fabs(*fret))) {
			glb_free_vec(xit,1,n);
			glb_free_vec(ptt,1,n);
			glb_free_vec(pt,1,n);
			return 0;
		}
		if (*iter == ITMAX) {glb_minimizer_error("Minimizer could not converge!");return -1;}
		for (j=1;j<=n;j++) {
			ptt[j]=2.0*p[j]-pt[j];
			xit[j]=p[j]-pt[j];
			pt[j]=p[j];
		}
		fptt=(*func)(ptt);
		if (fptt < fp) {
			t=2.0*(fp-2.0*(*fret)+fptt)*SQR(fp-(*fret)-del)-del*SQR(fp-fptt);
			if (t < 0.0) {
				line_minimization(p,xit,n,fret,func,&in);
				for (j=1;j<=n;j++) xi[j][ibig]=xit[j];
			}
		}
	}
	return 0;
}




