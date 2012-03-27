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
#ifndef GLB_OSZPROB_H
#define GLB_OSZPROB_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include <complex.h>
#include <globes/globes.h>

/* Constants */
//#define GLB_V_FACTOR        7.56e-14   /* Conversion factor for matter potentials */
#define GLB_V_FACTOR        7.5e-14   /* Conversion factor for matter potentials */
//#define GLB_Ne_MANTLE       0.497      /* Effective electron numbers for calculation */
#define GLB_Ne_MANTLE       0.5        /* Effective electron numbers for calculation */
#define GLB_Ne_CORE         0.468      /*   of MSW potentials                        */
#define V_THRESHOLD 0.001*GLB_V_FACTOR*GLB_Ne_MANTLE  /* The minimum matter potential below */
                                                      /* which vacuum algorithms are used   */
#define M_SQRT3  1.73205080756887729352744634151      /* sqrt(3) */

/* External variables */
extern int glb_oscp;
extern glb_probability_matrix_function glb_hook_probability_matrix;
extern glb_set_oscillation_parameters_function glb_hook_set_oscillation_parameters;
extern glb_get_oscillation_parameters_function glb_hook_get_oscillation_parameters;
extern void *glb_probability_user_data;



/* Macros */
#define SQR(x)      ((x)*(x))                        /* x^2   */
#define SQR_ABS(x)  (SQR(creal(x)) + SQR(cimag(x)))  /* |x|^2 */


/* Functions */
int glb_init_probability_engine();
int glb_free_probability_engine();
int glb_set_oscillation_parameters(glb_params p, void *user_data);
int glb_get_oscillation_parameters(glb_params p, void *user_data);
int glb_hamiltonian_cd(double E, double V, int cp_sign);
int glb_S_matrix_cd(double E, double L, double V, int cp_sign);
int glb_filtered_probability_matrix_cd(double P[3][3], double E, double L,
                                       double V, double sigma, int cp_sign);
int glb_probability_matrix(double P[3][3], int cp_sign, double E,
    int psteps, const double *length, const double *density,
    double filter_sigma, void *user_data);

#endif /* GLB_OSZPROB_H */


