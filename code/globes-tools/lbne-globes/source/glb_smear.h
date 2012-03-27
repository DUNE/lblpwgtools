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

/* This routines computes the smear matrix in the same way as CalcSmearA
   in GlobesMESbeta.m.

   Patrick Huber (c) 2004
*/


#ifndef GLB_SMEAR_H
#define GLB_SMEAR_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include "glb_types.h"

double glb_lower_bin_boundary(int bin, const glb_smear *data);
double glb_upper_bin_boundary(int bin, const glb_smear *data);
double glb_bin_center(int bin, const glb_smear *data);

double glb_lower_sbin_boundary(int bin, const glb_smear *data);
double glb_upper_sbin_boundary(int bin, const glb_smear *data);
double glb_sbin_center(int bin, const glb_smear *data);

glb_option_type *glb_option_type_alloc();
void glb_option_type_free(glb_option_type *in);
int glb_default_option_type(glb_option_type *in, const glb_smear *head,
			    const struct glb_experiment *headex);
glb_option_type *glb_option_type_reset(glb_option_type *in);

glb_smear  *glb_smear_alloc();
void glb_smear_free(glb_smear *in);
int glb_default_smear(glb_smear *in,const struct glb_experiment *head);
glb_smear  *glb_copy_smear(glb_smear *dest, const glb_smear *src);
glb_smear  *glb_smear_reset(glb_smear *in);

void glb_compute_smearing_matrix(double ***matrix, 
			   int **low, int **up, glb_smear *in
			   ,const struct glb_experiment *head);
void glb_optimize_smearing_matrix(glb_smear *s, double **matrix, int *lower, int *upper);
void glb_filter_compensate(glb_smear *s, double **matrix, int *lower, int *upper);


void glb_set_up_smear_data(glb_smear *test,const struct glb_experiment *head);
void glb_init_bin_data(glb_smear *in);
void glb_init_sim_bin_data(glb_smear *in);

double glb_constant_sigma(const glb_smear *in);

#endif /* GLB_SMEAR_H */
