/* GLoBES -- General LOng Baseline Experiment Simulator
 * (C) 2002 - 2011,  The GLoBES Team
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


#include <time.h>
#include <stdio.h>

#include "glb_tools_header.h"

/* That is not yet part of the globes API, but should become soon */
extern const char *glb_release_version; 

void print_header(FILE *fp, const char *program_short_name, 
		  const char *version, int argc, char **argv)
{
  int i;

  time_t now;
  char *times;
  
  now=time(NULL);
  times=asctime(localtime(&now));
  fprintf(fp,"# This file was created by '%s %s' using GLoBES %s\n",
	  program_short_name,version,glb_release_version); 
  fprintf(fp,"# The command line was\n# ");
  for(i=0;i<argc;i++) fprintf(fp,"%s ",argv[i]);
  fprintf(fp,"\n#\n# %s\n",times);

}



void print_footer(FILE *fp)
{
  int i;

  time_t now;
  char *times;
  
  now=time(NULL);
  times=asctime(localtime(&now));
  fprintf(fp,"\n# %s",times);
  fprintf(fp,"##### FINIS #####\n");

}
