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
#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include <globes/globes.h>

/* This file contains some deprecated functions, which are provided for
 * compatibility only
 */

/* glbChiTheta: Replaced by glbChiTheta13 in GLoBES 3.0 */
double glbChiTheta(const glb_params in, glb_params out, int exp)
{
  return glbChiTheta13(in, out, exp);
}

/* glbChiDms: Replaced by glbChiDm21 in GLoBES 3.0 */
double glbChiDms(const glb_params in, glb_params out, int exp)
{
  return glbChiDm21(in, out, exp);
}

/* glbChiDm: Replaced by glbChiDm31 in GLoBES 3.0 */
double glbChiDm(const glb_params in, glb_params out, int exp)
{
  return glbChiDm31(in, out, exp);
}

/* glbChiThetaDelta: Replaced by glbChiTheta13Delta in GLoBES 3.0 */
double glbChiThetaDelta(const glb_params in, glb_params out, int exp)
{
  return glbChiTheta13Delta(in, out, exp);
}


/* glbSetStartingValues: Replaced by glbSetCentralValues in GLoBES 3.0 */
int glbSetStartingValues(const glb_params in)
{
  return glbSetCentralValues(in);
}

/* glbGetStartingValues: Replaced by glbGetCentralValues in GLoBES 3.0 */
int glbGetStartingValues(glb_params in)
{
  return glbGetCentralValues(in);
}

/* glbGetNumberOfSimBins: Replaced by glbGetNumberOfSamplingPoints */
int glbGetNumberOfSimBins(int exp)
{
  return glbGetNumberOfSamplingPoints(exp);
}

