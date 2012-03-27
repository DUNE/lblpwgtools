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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#if HAVE_CONFIG_H
#  include "config.h"
#endif


#include "glb_error.h"

const char *glb_release_version = VERSION;
static const char *library_version = GLB_LIBVERSION;

static int
break_up_version(char const *version,int *first,int *second, int *third)
{

  char c;
  size_t length,i,k,ct=0,ctb=0;
  char *vec=NULL;
  int res[4]={-1,-1,-1,-1};
  if(version==NULL)
    {
      *third=-1;
      *second=-1;
      *first=-1;
      return 0;
    }
  length=strlen(version);
  for(i=0;i<length+1;i++)
    {
      c=version[i];
      if(isdigit(c))
	{
	  ct++;
	  vec=(char *) glb_realloc(vec,sizeof(char)*(ct+1));
	  vec[ct-1]=c;
	  vec[ct]='\0';
	}
      else
	{
	  ctb++;
	  if((ctb>4)||(vec==NULL))
	    {glb_fatal("Invalid version string");exit(1);}
	  sscanf(vec,"%d",&res[ctb-1]);
	  ct=0;
	}
    }
  glb_free(vec);
  *first=res[0];
  *second=res[1];
  *third=res[2];
  return 0;
}

int
glbTestReleaseVersion(const char *version)
{
  char buf[10];
  int s;
  int major,minor,bug,tmajor,tminor,tbug;
  break_up_version(version,&major,&minor,&bug);
  break_up_version(glb_release_version,&tmajor,&tminor,&tbug);

  if(major==tmajor)
    {
      if(minor==tminor)
	{
	  if(bug==tbug)
	    {
	      s=0;
	    }
	  else if(bug<tbug) s=-1;
	  else if (bug>tbug) s=+1;
	}
      else if(minor<tminor) s=-1;
      else if (minor>tminor) s=+1;
    }
  else if(major<tmajor) s=-1;
  else if (major>tmajor) s=+1;
  return -s;
}

int
glbTestLibraryVersion(const char *version)
{
  int s;
  int major,minor,bug,tmajor,tminor,tbug;
  break_up_version(version,&major,&bug,&minor);
  break_up_version(library_version,&tmajor,&tbug,&tminor);
  if(major==tmajor)
    {
      if(minor==tminor)
	{
	  if(bug==tbug)
	    {
	      s=0;
	    }
	  else if(bug<tbug) s=-1;
	  else if (bug>tbug) s=+1;
	}
      else if(minor<tminor) s=-1;
      else if (minor>tminor) s=+1;
    }
  else if(major<tmajor) s=-1;
  else if (major>tmajor) s=+1;
  return -s;
}
