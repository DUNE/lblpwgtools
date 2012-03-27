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

#ifndef GLB_MODULES_HEADER
#define GLB_MODULE_HEADER 1

#define GLB_PRIOR_MODULE_ID 42

/* Fixing the module namespace */

#ifndef USE_NO_LIBTOOL
#define GLB_CONCAT3x(a,b,c) a ## _ ## b ## _ ## c
#define GLB_CONCAT3(a,b,c) GLB_CONCAT3x(a,b,c)
#ifdef     GLB_WO_MODULES
#define       GLB_MOD_NSP(x) GLB_CONCAT3(GLB_MODULE_NAME,WOM,x)
#else
#define       GLB_MOD_NSP(x) GLB_CONCAT3(GLB_MODULE_NAME,LTX,x)
#endif
#else

#ifdef GLB_WO_MODULES
#define  GLB_MOD_NSP(x) GLB_CONCAT3(GLB_MODULE_NAME,WOM,x)
#else
#define GLB_MOD_NSP(x) x
#endif
#endif

#ifdef GLB_PRIOR_MODULE
 /* This fixes the namespace for the module */
#define    glb_module_id GLB_MOD_NSP(glb_module_id)
#define    glb_module_init GLB_MOD_NSP(glb_module_init)
#define    glb_module_clean GLB_MOD_NSP(glb_module_clean)
#define    glb_module_prior GLB_MOD_NSP(glb_module_prior)
#define    glb_module_starting_values GLB_MOD_NSP(glb_module_starting_values)
#define    glb_module_input_errors GLB_MOD_NSP(glb_module_input_errors)
extern int glb_module_id; 
#endif /* GLB_PRIOR_MODULE */

#define GLB_MODULE_NOT_FOUND -2
#define GLB_MODULE_NO_INIT -1

#endif /* GLB_MODULE_HEADER */
