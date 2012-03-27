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
#ifndef GLB_MMINIMIZE_H
#define GLB_MMINIMIZE_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include <glb_min_sup.h>


BEGIN_C_DECLS
int glb_init_minimizer();
int glb_free_minimizer();

void glb_set_errordim(int typ,int rule);
int glb_check_errordim(int rule);



// setting all the glb_prior need for projections
void glbSetDensityStartingValue(double start, int typ);
void glbSetDensityInputError(double error, int typ);
void glbSetDensityPrior(double start, double error, int typ);

int glb_set_solar_input_errors(double a);
int glb_set_solar_starting_values(double a);
int glb_set_input_errors(double a,double b, double c,double d, 
		   double e, double f);
int glb_set_starting_values(double a,double b, double c,double d, 
		      double e, double f);
int glb_set_ns_input_errors(double v[]);
int glb_set_ns_starting_values(double v[]);


extern int (*glb_user_defined_starting_values)(const glb_params, void *user_data);
extern int (*glb_user_defined_input_errors)(const glb_params, void *user_data);

double* glb_return_input_errors();
double* glb_return_input_values();


int glb_invoke_hybrid_minimizer(int exp, int rule, double *x, double *chi2);


// various projections with th12 fixed for a single experiment
// as determined by glb_single_experiment_number
extern int glb_single_experiment_number;

END_C_DECLS

#endif /*  GLB_MMINIMIZE_H */
