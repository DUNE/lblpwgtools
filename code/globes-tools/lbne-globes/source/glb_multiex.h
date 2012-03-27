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
#ifndef GLB_MULTIEX_H
#define GLB_MULTIEX_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include "glb_types.h"

extern int glb_current_exp;
extern int glb_ignore_invalid_chi2;

glb_exp glbAllocExp();

void glbSetExperiment(glb_exp in);   
int glbDefaultExp(glb_exp ins);      
void glbInitExp(glb_exp ins);        
void glbFreeExp(glb_exp ins);        


void glb_set_profile_scaling(double scale,int i);

#endif /* GLB_MULTIEX_H */
