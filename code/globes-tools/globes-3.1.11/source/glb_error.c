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




//The principles used in this file are taken from the autotools book by
//Gary V. Vaughan.


#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "glb_error.h"
#include <globes/globes.h>

/* this insures that the exit status of our program has the
 * expected meanining irrespective of the OS
 */
#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS  0
#  define EXIT_FAILURE  1
#endif

const char *glb_prog_name=NULL;

void glb_prog_name_init (const char *path)
{
  if (!glb_prog_name)
    glb_prog_name =(char*) strdup(path);
}


static int verbosity_level=1;


/***************************************************************************
 * Function glbSetVerbosityLevel                                           *
 ***************************************************************************
 * Set the verbosity level.                                                *
 ***************************************************************************/
/* Set and return the level of verbosity */
int glbSetVerbosityLevel(int level)
{
  if(level<0) return -1;
  verbosity_level=level;
  return 0;
}


/***************************************************************************
 * Function glbGetVerbosityLevel                                           *
 ***************************************************************************
 * Return the current verbosity level.                                     *
 ***************************************************************************/
int glbGetVerbosityLevel()
{
  return verbosity_level;
}

/***************************************************************************
 * Function error                                                          *
 ***************************************************************************
 * Print error message with suitable prefix ("ERROR", "FATAL", etc.) and,  *
 * if exit_status >= 0, terminate the program. The error message can be    *
 * composed in the printf style                                            *
 ***************************************************************************/
static void error(int exit_status, int verb_level, const char *type, const char *format,
                  va_list args)
{
  if((verb_level==1)||(exit_status)>=0)
  {
    char s[1024];
    sprintf(s, "%s: %s: %s.\n", glb_prog_name, type, format);
    vfprintf(stderr, s, args);
  }
  va_end(args);

  if (exit_status >= 0)
    exit (exit_status);
}

void
glb_warning (const char *message, ...)
{
  int v=0;
  va_list args;
  va_start(args, message);
  if(verbosity_level >= 2) v=1;
  error (-1, v, "Warning", message, args);
}

void
glb_error (const char *message, ...)
{
  int v=0;
  va_list args;
  va_start(args, message);
  if(verbosity_level >= 1) v=1;
  error(-1, v, "ERROR", message, args);
}

void
glb_fatal (const char *message, ...)
{
  va_list args;
  va_start(args, message);
  /* A FATAL message always is displayed */
  error(EXIT_FAILURE, 1, "FATAL", message, args);
}

void
glb_exp_error (const struct glb_experiment *exp, const char *message, ...)
{
  char s[100];
  int v=0;
  va_list args;
  va_start(args, message);
  if(verbosity_level >= 1) v=1;
  if (exp == NULL  ||  exp->filename == NULL)
    error(-1, v, "ERROR", message, args);
  else
  {
    sprintf(s, "ERROR in experiment %.60s", exp->filename);
    error(-1, v, s, message, args);
  }
}

void
glb_rule_error (const struct glb_experiment *exp, int rule, const char *message, ...)
{
  char s[100];
  int v=0;
  va_list args;
  va_start(args, message);
  if(verbosity_level >= 1) v=1;
  if (exp == NULL  ||  exp->filename == NULL)
    error(-1, v, "ERROR", message, args);
  else if (rule < 0  ||  rule >= exp->numofrules)
  {
    sprintf(s, "ERROR in experiment %.60s", exp->filename);
    error(-1, v, s, message, args);
  }
  else
  {
    sprintf(s, "ERROR in experiment %.60s, rule %d", exp->filename, rule);
    error(-1, v, s, message, args);
  }
}

/* ----- making the system calls safe ----- */
void *glb_malloc (size_t size)
{
  register void *value = malloc (size);
  if (value == NULL)
    glb_fatal("Virtual memory exhausted");
  return value;
}

void *glb_realloc (void *ptr, size_t size)
{
  register void *value = realloc (ptr, size);
  if (value == NULL)
    glb_fatal("Virtual memory exhausted");
  return value;
}

void glb_free(void *ptr)
{
  if (ptr != NULL)
  {
    free(ptr);
    ptr=NULL;   // FIXME This has no effect
  }
}

FILE *glb_fopen(const char *filename, const char *mode)
{
  size_t i,a,b;
  char *test_name=NULL;
  char *new_name=NULL;

  FILE *t;
  register FILE *value;

 /* handling an absolute path */
  if(strlen(filename)>0 && filename[0] == '/')
    {
      if(verbosity_level >= 4)
	fprintf(stderr,"Searched path: %s\n",filename);
      t=fopen(filename,"r");
      if(t!=NULL)
	{
	  new_name=strdup(filename);
	  fclose(t);
	}
    }

  if(new_name==NULL)
    {
      for(i=0;i<glb_path_vector_length;i++)
	{
	  a=strlen(filename);
	  b=strlen(glb_path_vector[i]);
	  test_name=glb_malloc((a+b+2)*sizeof(char));
	  test_name=strcpy(test_name,glb_path_vector[i]);
	  test_name=strcat(test_name,filename);
	  if(verbosity_level >= 4)
	    fprintf(stderr,"Searched path: %s\n",test_name);
	  t=fopen(test_name,"r");
	  if(t!=NULL)
	    {
	      new_name=strdup(test_name);
	      fclose(t);
	      glb_free(test_name);
	      break;
	    }
	  glb_free(test_name);
	  /* Repeating the exercise with an additional '/' between
	   * path and filename.
	   */
	  a=strlen(filename);
	  b=strlen(glb_path_vector[i]);
	  test_name=glb_malloc((a+b+2)*sizeof(char));
	  test_name=strcpy(test_name,glb_path_vector[i]);
	  test_name=strcat(test_name,"/");
	  test_name=strcat(test_name,filename);
	  t=fopen(test_name,"r");
	  if(t!=NULL)
	    {
	      new_name=strdup(test_name);
	      glb_free(test_name);
	      fclose(t);
	      break;
	    }
	  glb_free(test_name);
	}
  }

  if(new_name==NULL)
  {
    char msg[100+strlen(filename)];
    sprintf(msg, "File %s not found", filename);
    glb_error(msg);
    return NULL;
  }

  value = fopen(new_name,mode);
  if(verbosity_level >= 3) fprintf(stderr,"File read: %s\n",new_name);
  glb_free(new_name);
  if(value== NULL)
  {
    char msg[100+strlen(filename)];
    sprintf(msg, "Could not open file %s", filename);
    glb_error(msg);
  }
  return value;
}

int glb_fclose(FILE *stream)
{
  int i=fclose(stream);
  if(i!=0)
    glb_error("Could not close stream");
  return i;
}


