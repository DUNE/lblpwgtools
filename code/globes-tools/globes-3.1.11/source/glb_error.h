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
#ifndef GLB_ERROR_H
#define GLB_ERROR_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "glb_types.h"

/* Error codes */
#define GLB_SUCCESS                 0  /* No error, everything's fine                           */
#define GLBERR_INVALID_ARGS        -1  /* A function has been given invalid arguments           */
#define GLBERR_UNINITIALIZED       -2  /* An object has not been initialized properly           */
#define GLBERR_MALLOC_FAILED       -3  /* Memory allocation error                               */
#define GLBERR_FILE_NOT_FOUND      -4  /* File not found                                        */
#define GLBERR_INVALID_FILE_FORMAT -5  /* Error in an input file                                */
#define GLBERR_GSL_ERROR           -6  /* A call to a GSL routine failed                        */
#define GLBERR_NO_CONVERGENCE      -7  /* Numerical algorithm failed to converge                */
#define GLBERR_PREM_NOT_LOADED     -8  /* Error with PREM profile data                          */
#define GLBERR_NOT_IMPLEMENTED     -9  /* Requested a feature that has not been implemented yet */
#define GLBERR_DEPRECATED         -10  /* Requested a deprecated feature                        */
#define GLBERR_NAME_NOT_FOUND     -11  /* A name could not be translated into an index          */
#define GLBERR_GENERIC            -99  /* Unspecified error                                     */


/* Define NaN (not a number) */
#ifdef NAN
  #define GLB_NAN      NAN
  #define GLB_ISNAN(x) isnan(x)
#else
  #define GLB_NAN      DBL_MAX
  #define GLB_ISNAN(x) ((x) == DBL_MAX)
#endif


extern const char *glb_prog_name;
extern void glb_prog_name_init (const char *argv0);

extern int glbSetVerbosityLevel(int level);


extern void glb_warning      (const char *message, ...);
extern void glb_error        (const char *message, ...);
extern void glb_fatal        (const char *message, ...);
extern void glb_exp_error    (const struct glb_experiment *exp, const char *message, ...);
extern void glb_rule_error   (const struct glb_experiment *exp, int rule, const char *message, ...);

extern void *glb_malloc (size_t size);
extern void *glb_realloc (void *ptr, size_t size);
extern void glb_free(void *ptr);
extern FILE *glb_fopen(const char *filename, const char *mode);
extern int glb_fclose(FILE *stream);

#endif /* !GLB_ERROR_H */
