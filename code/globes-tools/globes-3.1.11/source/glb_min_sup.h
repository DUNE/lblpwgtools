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
#ifndef GLB_MIN_SUP
#define GLB_MIN_SUP 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#define SQR(x)    ((x)*(x))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))
double *glb_alloc_vec(int nl,int nh);
double **glb_alloc_mat(int nrl,int nrh, int ncl,int nch);
void glb_free_vec(double *v,int nl,int nh);
void glb_free_mat(double **m,int nrl,int nrh,int ncl,int nch);
int glb_powell(double p[],double **xi,int n,
		double ftol,int *iter,double *fret,
		double (*func)(double*));
int glb_powell2(double p[],double **xi,int n,double ftol,
		 int *iter,double *fret,
		 double (*func)(double*));

typedef double (*glb_minimize_func)(double *, int, void *);
int glb_hybrid_minimizer(double *P, int n, int n_osc, double ftol, int *iter,
                         double *fret, glb_minimize_func f, void *user_data);


#endif /* GLB_MIN_SUP */
