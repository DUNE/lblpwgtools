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
#ifndef GLB_FLUXES_H
#define GLB_FLUXES_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include "glb_types.h"


void glb_set_max_energy(double en);
double glb_get_max_energy();

/* Fluxes */
int glb_load_n_columns(const char *file_name, const int n_columns, int *n_lines, double *data[]);
int glb_init_flux(glb_flux *flux);
double glb_get_flux(double E, double L, int f, int cp_sign, const glb_flux *flux);
int glb_free_flux(glb_flux *flux);
int glb_reset_flux(glb_flux *flux);
int glb_copy_flux(glb_flux *dest, const glb_flux *src);

/* Cross sections */
int glb_init_xsec(glb_xsec *xs);
double glb_get_xsec(double E, int f, int cp_sign, const glb_xsec *xs);
int glb_free_xsec(glb_xsec *xs);
int glb_reset_xsec(glb_xsec *xs);
int glb_copy_xsec(glb_xsec *dest, const glb_xsec *src);

#endif /* GLB_FLUXES_H */
