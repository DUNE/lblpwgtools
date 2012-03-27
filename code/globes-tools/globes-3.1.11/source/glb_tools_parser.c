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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "globes/globes.h"


void parse_definition(const char *in)
{
  const char *delim="=";
  double val;
  char *token=NULL;
  char *lhs=NULL;
  char *rhs=NULL;
  char *inc=NULL;
  size_t length=0;

  inc=strdup(in);
  if(inc==NULL) return;
  token=strtok(inc,delim);
  if(token!=NULL) 
    {
     lhs=strdup(token);
     length++;
    }
  token=strtok(NULL,delim); 
  if(token!=NULL) 
    {
     rhs=strdup(token);
     length++;
    }
  
  if(length!=2) {
    free(lhs);
    free(rhs);
    fprintf(stderr,"ERROR: Definition is not of form 'DEFINITION=VALUE'\n");
    return;}
  val=atof(rhs);
  glbDefineAEDLVariable(lhs,val);
  free(lhs);
  free(rhs);
  return;
}


/* This should be a bullet proof parser for parameter vectors as
 * returned by an argp-parser.  It expects the format to be '1,2,3',
 * where '1, 3 ,4 , 9.3' also would be okay. It uses 'strtod' to get
 * the double and tests for - any errors during the call to 'strtod',
 * like an overflow - wether there are enough numbers - wether there
 * too many numbers
 */
 

int vector_read_double(const char *str, /* a string returned by argp,
					   basically it must end after
					   the n-th number */
		       double *result, /* this pointer must point to
					  something which can hold n
					  doubles */ 
		       size_t n, /* number of doubles to be read */
		       void (*parse_error)(void) /* Function to be
						    called in case of
						    a parse error */
		       )
{
  size_t c=0;
  double res;
  char *endp;
  const char *wrk;
  if(!str) return 0;
  wrk=str;
  while(wrk&&c<n)
    {
      errno=0;
      res=strtod(wrk,&endp);
      if(errno) {fprintf(stderr,"While parsing input the following error occured\nFATAL: '%s'\n",
			 strerror(errno));exit(1);}
      if(endp) if(strcmp(wrk,endp)==0) {parse_error();return c;}
      wrk=endp;
      /* Eat up trailing white space */
      while(isspace(wrk[0])) wrk++;
      /* Take care of the separator */
      if(wrk[0]==','||wrk[0]=='\0') wrk++;
      result[c]=res;
      c++;
    }
  /* Issue an error whenever there are too few conversions or the
   * string is longer than it should */
  if(c!=n||wrk[-1]!='\0') parse_error();
  return c;
}

int vector_read_int(const char *str, int *result, size_t n,void (*parse_error)(void))
{
  size_t c=0;
  int res;
  char *endp;
  const char *wrk;

  if(!str) return 0;
  wrk=str;
  while(wrk&&c<n)
    {
      errno=0;
      res=strtol(wrk,&endp,10);
      if(errno) {fprintf(stderr,"While parsing input the following error occured\nFATAL: '%s'\n",
			 strerror(errno));exit(1);}
      if(endp) if(strcmp(wrk,endp)==0) {parse_error();return c;}
      wrk=endp;
      /* Eat up trailing white space */
      while(isspace(wrk[0])) wrk++;
      /* Take care of the separator */
      if(wrk[0]==','||wrk[0]=='\0') wrk++;
      result[c]=res;
      c++;
    }
  /* Issue an error whenever there are too few conversions or the
   * string is longer than it should */
  if(c!=n||wrk[-1]!='\0') parse_error();
  return c;
}

void parse_error()
{
  fprintf(stderr,"FATAL: Wrong format in parameter vector\n");
  exit(1);
}


void my_print_params(FILE *fp,const glb_params iv)
{
  int i;
  for(i=0;i<GLB_OSCP;i++) {
    if(i==GLB_DELTA_CP) fprintf(fp,"%6.6e\t",fmod(glbGetOscParams(iv,i),2*M_PI));
    else fprintf(fp,"%6.6e\t",glbGetOscParams(iv,i));
      }
  for(i=0;i<glb_num_of_exps;i++) fprintf(fp,"%6.6f\t",glbGetDensityParams(iv,i));
}


char *get_output_filename(const char *basename)
{
  char *base;
  size_t len;
  len=strlen(basename);
 
  base=(char *) malloc((len+5)*sizeof(char));
  base=strncpy(base,basename,len+1);
  if(!base) {fprintf(stderr,"No mem\n");exit(1);}
  base=strncat(base,".dat",len+5);
  return base;
}

