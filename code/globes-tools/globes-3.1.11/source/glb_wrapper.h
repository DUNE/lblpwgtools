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
#ifndef glb_WRAPPER_H
#define  glb_WRAPPER_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include <globes/globes.h>

/* NIL value for the projection flag */
#define GLB_UNDEFINED  -2

/* External variables */
extern char **glb_param_names;

typedef struct {
  double *osc_params;
  size_t length;
} glb_osc_type;

typedef struct {
  double *density_params;
  size_t length;
} glb_density_type;


struct glb_params_type {
  glb_osc_type *osc;
  glb_density_type *density;
  int iterations;
};

typedef struct {
  int *osc_params;
  size_t length;
} glb_osc_proj_type;

typedef struct {
  int *density_params;
  size_t length;
} glb_density_proj_type;


struct glb_projection_type {
  glb_osc_proj_type *osc;
  glb_density_proj_type *density;
};





glb_osc_type *glb_alloc_osc_type();

void glb_free_osc_type(glb_osc_type *stale);

glb_density_type *glb_alloc_density_type();

void glb_free_density_type(glb_density_type *stale);

glb_osc_proj_type *glb_alloc_osc_proj_type();

void glb_free_osc_proj_type(glb_osc_proj_type *stale);

glb_density_proj_type *glb_alloc_density_proj_type();

void glb_free_density_proj_type(glb_density_proj_type *stale);

void glb_init(char *name);

void glb_clean_up();

#endif /* GLB_WRAPPER_H 1 */
