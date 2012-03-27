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


/* Resolution of a path given by an environment variable.
 * The variable is called glb_PATH and can be set by
 * export glb_PATH=${GLB_PATH}:path1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#include "glb_error.h"

#include <globes/globes.h>

/* global variables */
char **glb_path_vector;
size_t glb_path_vector_length;


/* This function is taken from the GNU C library reference manual */
static char *my_getcwd ()
{
  size_t size = 100;
  while (1)
    {
      char *buffer = (char *) glb_malloc (size);
      if (getcwd (buffer, size) == buffer)
	return buffer;
      free (buffer);
      if (errno != ERANGE)
	return 0;
      size *= 2;
    }
}


int glb_break_up_path(char *in_path, char ***pathv, size_t *len)
{
  const char *delim=":";
  char *token=NULL;
  char **vec=NULL;
  size_t length=1;
  vec=glb_malloc(sizeof(char*));
  /* The first place to search shall always be the local directory, i.e.
   * the directory where the binary is started from.
   */
  vec[0]=my_getcwd();
  if(in_path==NULL)
    {
      *len=length;
      *pathv=vec;
      return 0;
    }
  token=strtok(in_path,delim);
  if(token!=NULL)
    {
      vec=(char **) glb_realloc(vec,sizeof(char*)*(length+1));
      vec[length]=strdup(token);
      length++;
    }
  while(token!=NULL)
    {
      token=strtok(NULL,delim);
      if(token!=NULL)
	{
	  vec=glb_realloc(vec,sizeof(char*)*(length+1));
	  vec[length]=strdup(token);
	  length++;
	}
    }

  *len=length;
  *pathv=vec;
  return 0;
}

int glb_setup_path()
{
  int s=1;
  char *in_path;
  char *path;
  in_path=getenv("GLB_PATH");
  if(in_path==NULL) path=NULL;
  else path=strdup(in_path);
  s=glb_break_up_path(path,&glb_path_vector,&glb_path_vector_length);
  glb_free(path);
  return s;
}
