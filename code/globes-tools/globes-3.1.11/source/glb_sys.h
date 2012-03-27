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
#ifndef GLB_SYS_H
#define GLB_SYS_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include "glb_types.h"

extern glb_systematic *glb_sys_list;

int glbIsChiFunction2011Compatible(const char *name);
glb_systematic *glbFindChiFunctionByName(const char *name);
char *glbConvertErrorDim(int errordim);
int glbCleanSysList();

int glbSetChiFunctionInRule(struct glb_experiment *exp, int rule, int on_off,
                            const char *sys_id, double *errors);
int glbSetChiFunctionInExperiment(struct glb_experiment *exp, int rule, int on_off,
                                  const char *sys_id, double *errors);
int glbSwitchSystematicsInExperiment(int experiment, int rule, int on_off);

int glbSetSignalErrorsInRule(int exp, int rule, double norm, double tilt);
int glbSetBGErrorsInRule(int exp, int rule, double norm, double tilt);
int glbSetSysErrorsListInRule(int exp, int rule, int on_off, const double *sys_list);
int glbSetSignalStartingValuesInRule(int exp, int rule, double norm, double tilt);
int glbSetBGStartingValuesInRule(int exp, int rule, double norm, double tilt);
int glbSetSysStartingValuesListInRule(int exp, int rule, int on_off, const double *sys_list);


#endif /* GLB_SYS_H */
