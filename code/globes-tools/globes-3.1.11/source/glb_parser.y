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
%{
#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif



#define YYDEBUG 1
//int yydebug = 1; /* Uncomment this line to get debug output from the parser */
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <globes/globes.h>
#include <gsl/gsl_spline.h>
#include "glb_smear.h"
#include "glb_multiex.h"
#include "glb_types.h"
#include "glb_error.h"
#include "glb_lexer.h"
#include "glb_parser_type.h"
#include "glb_fluxes.h"
#include "glb_sys.h"
#include "glb_minimize.h"

#define TWICE 100

#define UNTYPE -1
#define INT 0
#define DOUBLE 1
#define INT_LIST 2
#define DOUBLE_LIST 3
#define DOUBLE_LIST_INDEXED 4
#define INT_LIST_INDEXED 5
#define ENERGY_MATRIX 6
#define INT_INDEXED 7
#define INT_INDEXED_PAIR 8
#define DOUBLE_INDEXED 9
#define DOUBLE_INDEXED_PAIR 10
#define DOUBLE_INDEXED_PAIR_INV 11
#define CHAR 12
#define FUN 13
#define DOUBLE_LIST_INDEXED_SL 14
#define DOUBLE_LIST_INDEXED_BL 15
#define COUNTER 16
#define DOUBLE_PAIR 17
#define GMAX 1E100

  static int exp_count=1;
  static int energy_len;
  static int loc_count;
  static int energy_count=-1;
  static int cross_count=-1;
  static int flux_count=-1;
  static struct glb_experiment buff;
  static struct glb_experiment buff_list[GLB_MAX_EXP];
  static glb_smear ibf;
  static glb_option_type opt;
  static glb_flux flt;
  static glb_xsec xsc;
  static int errordim_sys_on=-1;         /* Temp. storage for the old numerical errordims */
  static int errordim_sys_off=-1;
  static char *context;

  int yyerror (const char *s, ...);           /* Forward declaration to suppress compiler warning */


  typedef struct
  {
    char *token; // the string which is used as lhs in the ini-file
    int scalar; // data type flag
    double rl;
    double ru; // allowed range
    void *ptr; // this is a pointer the corresponding part of the exp structure
    void *len; /* this is a pointer which points to the length of the vector
		*  in a struct glb_experiment. Example: if we parse densitytab,
		*  this things points
		*  to psteps
		*/

    char *ctx; /* here the type of the environment is given, e.g. rule
		* thus the corresponding token is matched onyl within
		* a rule type environment
		*/
  } glb_parser_decl;


  static glb_parser_decl token_list[]={
    {"$version",CHAR,0,1E8,&buff.version,NULL,"global"},
    {"$citation",CHAR,0,1E8,&buff.citation,NULL,"global"},
    {"$parent_energy",DOUBLE,0,GMAX,&buff.emax,NULL,"global"},
    {"$target_mass",DOUBLE,0,GMAX,&buff.targetmass,NULL,"global"},
#ifdef GLB_OLD_AEDL
    {"$simbins" ,COUNTER,0,500,&buff.simbins,NULL,"global"},
    {"$simtresh" ,DOUBLE,0,GMAX,&buff.simtresh,NULL,"global"},
    {"$simbeam" ,DOUBLE,0,GMAX,&buff.simbeam,NULL,"global"},
    {"$numofbins",COUNTER,0,500,&buff.numofbins,NULL,"global"},
    {"$simbinsize",DOUBLE_LIST,0,GMAX,&buff.simbinsize,
     &buff.simbins,"global"},
/* JK - Has been removed
    {"$errorfunction",INT,0,1,&buff.errorfunction,NULL,"global"},
    {"@treshold_setttings" ,DOUBLE_INDEXED_PAIR,
     0,100,&buff.bgtcenter[0],&loc_count,"rule"},
    {"@treshold_error" ,DOUBLE_INDEXED_PAIR,
     0,100,&buff.bgterror[0],&loc_count,"rule"}, */
#endif /* GLB_OLD_AEDL */
    {"$sampling_points" ,COUNTER,0,500,&buff.simbins,NULL,"global"},
    {"$sampling_min" ,DOUBLE,0,GMAX,&buff.simtresh,NULL,"global"},
    {"$sampling_max" ,DOUBLE,0,GMAX,&buff.simbeam,NULL,"global"},
    /* FIXME -- this was not properly recognized due to the fact the first match is performed
     * by matching only the letters up to the length of the word ins this list, ie.
     * if 'bins' is before 'binsize' in this list, 'binsize' will be matched as
     * 'bins'
     */
    {"$binsize",DOUBLE_LIST,0,GMAX,&buff.binsize,
     &buff.numofbins,"global"},

    {"$bins",COUNTER,0,500,&buff.numofbins,NULL,"global"},
    {"$emin",DOUBLE,0,GMAX,&buff.emin,NULL,"global"},
    {"$emax",DOUBLE,0,GMAX,&buff.emax,NULL,"global"},

    {"$baseline",DOUBLE,0,2*GLB_EARTH_RADIUS,&buff.baseline,NULL,"global"},
    {"$profiletype",INT,1,3,&buff.density_profile_type,NULL,"global"},

    {"$sampling_stepsize",DOUBLE_LIST,0,GMAX,&buff.simbinsize,
     &buff.simbins,"global"},

   {"$densitytab",DOUBLE_LIST,0,GMAX,&buff.densitytab,&buff.psteps,"global"},
   {"$lengthtab",DOUBLE_LIST,0,2*GLB_EARTH_RADIUS,
    &buff.lengthtab,&buff.psteps,"global"},
   {"rulechannellist",INT_LIST_INDEXED,0,GMAX,&buff.rulechannellist[0],
    &buff.lengthofrules[0],"rule"},
   {"rulescoeff",DOUBLE_LIST_INDEXED,0,GMAX,&buff.rulescoeff[0],
    &buff.lengthofrules[0],"rule"},

   {"bgrulechannellist",INT_LIST_INDEXED,0,GMAX,&buff.bgrulechannellist[0],
    &buff.lengthofbgrules[0],"rule"},
   {"bgrulescoeff",DOUBLE_LIST_INDEXED,0,GMAX,&buff.bgrulescoeff[0],
    &buff.lengthofbgrules[0],"rule"},
   {"rule",UNTYPE,-1,1,NULL,&buff.numofrules,"global"},
   {"@signal",UNTYPE,-1,1,NULL,&loc_count,"rule"},
   {"@background",UNTYPE,-1,1,NULL,&loc_count,"rule"},

    {"@pre_smearing_efficiencies",DOUBLE_LIST_INDEXED,0,GMAX,
     &buff.user_pre_smearing_channel[0],&loc_count,"channel"},

    {"@pre_smearing_background",DOUBLE_LIST_INDEXED,0,GMAX,
     &buff.user_pre_smearing_background[0],&loc_count,"channel"},

    {"@post_smearing_efficiencies",DOUBLE_LIST_INDEXED,0,GMAX,
     &buff.user_post_smearing_channel[0],&loc_count,"channel"},

    {"@post_smearing_background",DOUBLE_LIST_INDEXED,0,GMAX,
     &buff.user_post_smearing_background[0],&loc_count,"channel"},


   {"@errordim_sys_on",INT,0,20,&errordim_sys_on,NULL,"rule"},
   {"@errordim_sys_off",INT,0,20,&errordim_sys_off,NULL,"rule"},
   {"@sys_on_function",CHAR,0,20,&buff.sys_on_strings[0],&loc_count,"rule"},
   {"@sys_off_function",CHAR,0,20,&buff.sys_off_strings[0],&loc_count,"rule"},

   {"channel",UNTYPE,-1,1,NULL,&buff.numofchannels,"global"},
   {"@channel",INT_LIST_INDEXED,-1,1,&buff.listofchannels[0],
    &buff.numofchannels,"channel"},
   {"energy",UNTYPE,-1,1,NULL,&buff.num_of_sm,"global"},
   {"@energy",ENERGY_MATRIX,-1,GMAX,&buff.smear[0],&loc_count,"energy"},

   {"@signalerror",DOUBLE_INDEXED_PAIR,
    0,100,&buff.signal_errors[0],&loc_count,"rule"},
   {"@backgrounderror",DOUBLE_INDEXED_PAIR,
    0,100,&buff.bg_errors[0],&loc_count,"rule"},
   {"@backgroundcenter",DOUBLE_INDEXED_PAIR,
    0,100,&buff.bg_centers[0],&loc_count,"rule"},

   {"@sys_on_errors",DOUBLE_LIST_INDEXED,0,GMAX,
    &buff.sys_on_errors[0],&loc_count,"rule"},
   {"@sys_off_errors",DOUBLE_LIST_INDEXED,0,GMAX,
    &buff.sys_off_errors[0],&loc_count,"rule"},


   {"@energy_window" ,DOUBLE_INDEXED_PAIR_INV,0,GMAX,&buff.energy_window[0],
    &loc_count,"rule"},
   {"$densitysteps",COUNTER,1,GMAX,&buff.psteps,NULL,"global"},



   {"$filter_state",INT,GLB_OFF,GLB_ON,&buff.filter_state,NULL,"global"},
   {"$filter_value",DOUBLE,0,GMAX,&buff.filter_value,NULL,"global"},

   {"cross",UNTYPE,0,20,NULL,&buff.num_of_xsecs,"global"},
   {"@cross_file",CHAR,0,20,&xsc.file_name,NULL,"cross"},


   {"flux",UNTYPE,0,20,NULL,&buff.num_of_fluxes,"global"},
   {"@flux_file",CHAR,0,20,&flt.file_name,NULL,"flux"},

    {"@builtin",INT,1,4,&flt.builtin,NULL,"flux"},
    {"@time",DOUBLE,0,GMAX,&flt.time,NULL,"flux"},
    {"@power",DOUBLE,0,GMAX,&flt.target_power,NULL,"flux"},
    {"@stored_muons",DOUBLE,0,GMAX,&flt.stored_muons,NULL,"flux"},
    {"@parent_energy",DOUBLE,0,GMAX,&flt.parent_energy,NULL,"flux"},
    {"@norm",DOUBLE,0,GMAX,&flt.norm,NULL,"flux"},
    {"@gamma",DOUBLE,0,GMAX,&flt.gamma,NULL,"flux"},
    {"@end_point",DOUBLE,0,GMAX,&flt.end_point,NULL,"flux"},
    {"@stored_ions",DOUBLE,0,GMAX,&flt.stored_muons,NULL,"flux"},



    {"nuflux",UNTYPE,0,20,NULL,&buff.num_of_fluxes,"global"},
    {"@flux_file",CHAR,0,20,&flt.file_name,NULL,"nuflux"},

    {"@builtin",INT,1,4,&flt.builtin,NULL,"nuflux"},
    {"@time",DOUBLE,0,GMAX,&flt.time,NULL,"nuflux"},
    {"@power",DOUBLE,0,GMAX,&flt.target_power,NULL,"nuflux"},
    {"@stored_muons",DOUBLE,0,GMAX,&flt.stored_muons,NULL,"nuflux"},
    {"@parent_energy",DOUBLE,0,GMAX,&flt.parent_energy,NULL,"nuflux"},
    {"@norm",DOUBLE,0,GMAX,&flt.norm,NULL,"nuflux"},
    {"@gamma",DOUBLE,0,GMAX,&flt.gamma,NULL,"nuflux"},
    {"@end_point",DOUBLE,0,GMAX,&flt.end_point,NULL,"nuflux"},
    {"@stored_ions",DOUBLE,0,GMAX,&flt.stored_muons,NULL,"nuflux"},


   {"@type" ,INT,1,2,&ibf.type,&loc_count,"energy"},
   {"@sigma_e" ,DOUBLE_LIST,0,GMAX,&ibf.sigma,&ibf.num_of_params,"energy"},
   {"@sigma_function" ,FUN,0,1000,&ibf.sig_f,NULL,"energy"},



   {NULL,UNTYPE,0,0,NULL,NULL,"global"}
};


static void grp_start(char* name)
   {
     if(strncmp(name,"rule",4)==0 )
       {
         /* Reset variables to default values */
         errordim_sys_on  = -1.0;
         errordim_sys_off = -1.0;
       }
   }


static void grp_end(char* name)
   {
     char tmp_errordim[2];

     if(strncmp(name,"energy",6)==0 )
       {
	 if(buff.num_of_sm-1 >= 0)
	   {
	     ibf.options=glb_option_type_alloc();
	     ibf.options=(glb_option_type *) memmove(ibf.options,&opt,
					     sizeof(glb_option_type));

	     if(buff.smear_data[buff.num_of_sm-1]==NULL)
	       buff.smear_data[buff.num_of_sm-1]=glb_smear_alloc();
	     buff.smear_data[buff.num_of_sm-1]=
	       glb_copy_smear(buff.smear_data[buff.num_of_sm-1],&ibf);
	     glb_option_type_free(ibf.options);
	     glb_option_type_reset(&opt);
	     if(ibf.sigma!=NULL) glb_free(ibf.sigma);
	     glb_smear_reset(&ibf);

	   }

       }

     if(strncmp(name,"flux",4)==0 )
       {

	 if(buff.num_of_fluxes > 0)
	   {
	     glb_error("The 'flux' directive is deprecated (consider using 'nuflux').\n"
		       "The flux normalization may not be what you expect.\n"
		       "Please, consult the manual!");
	     if(flt.builtin<=0) flt.builtin=GLB_OLD_NORM;

             if (buff.fluxes[buff.num_of_fluxes-1] == NULL)
             {
               buff.fluxes[buff.num_of_fluxes-1] = glb_malloc(sizeof(glb_flux));
               memset(buff.fluxes[buff.num_of_fluxes-1], 0, sizeof(*buff.fluxes[0]));
               glb_reset_flux(buff.fluxes[buff.num_of_fluxes-1]);
             }
             if (glb_copy_flux(buff.fluxes[buff.num_of_fluxes-1], &flt) != GLB_SUCCESS)
               glb_error("grp_end: Error copying flux data");
             glb_reset_flux(&flt);
	   }
       }


     if(strncmp(name,"nuflux",6)==0 )
       {
	 if(buff.num_of_fluxes > 0)
	   {
	     if (buff.fluxes[buff.num_of_fluxes-1] == NULL)
             {
               buff.fluxes[buff.num_of_fluxes-1] = glb_malloc(sizeof(glb_flux));
               memset(buff.fluxes[buff.num_of_fluxes-1], 0, sizeof(*buff.fluxes[0]));
               glb_reset_flux(buff.fluxes[buff.num_of_fluxes-1]);
             }
             if (glb_copy_flux(buff.fluxes[buff.num_of_fluxes-1], &flt) != GLB_SUCCESS)
               glb_error("grp_end: Error copying flux data");
             glb_reset_flux(&flt);
	   }
       }

     if(strncmp(name,"cross",5)==0 )
       {
	 if(buff.num_of_xsecs > 0)
	   {
	     if (buff.xsecs[buff.num_of_xsecs-1] == NULL)
             {
               buff.xsecs[buff.num_of_xsecs-1] = glb_malloc(sizeof(glb_xsec));
               memset(buff.xsecs[buff.num_of_xsecs-1], 0, sizeof(*buff.xsecs[0]));
               glb_reset_xsec(buff.xsecs[buff.num_of_xsecs-1]);
             }
             if (glb_copy_xsec(buff.xsecs[buff.num_of_xsecs-1], &xsc) != GLB_SUCCESS)
               glb_error("grp_end: Error copying cross section data");
	     glb_reset_xsec(&xsc);
	   }
       }



     if(strncmp(name,"rule",4)==0 )
       {
         int nr = buff.numofrules - 1;

         /* Parse old (numerical) errordims */
         if (buff.sys_on_strings[nr] == NULL  &&  errordim_sys_on >= 0)
           buff.sys_on_strings[nr] = glbConvertErrorDim(errordim_sys_on);
         if (buff.sys_off_strings[nr] == NULL  &&  errordim_sys_off >= 0)
           buff.sys_off_strings[nr] = glbConvertErrorDim(errordim_sys_off);
       }

     glb_free(context);
     context = (char *) strdup("global");

   }

static int set_channel_data(int x[6],int loc_count)
   {
     // I am sorry for this -- it's a kludge
     int i;

     for(i=0;i<6;i++) {
       if (loc_count >= buff.numofchannels) /* Don't realloc when parsing a re-definition */
         buff.listofchannels[i]=
			(int*) glb_realloc(buff.listofchannels[i]
					,sizeof(int)*loc_count);

       buff.listofchannels[i][loc_count-1]=x[i];
     }


     return 0;
   }


static int step_counter(char *name)
{
  int i=0;
  int *ibf;
  ibf=NULL;
  for(i=0;token_list[i].token!=NULL;i++)
    {
      if(strncmp(name,token_list[i].token,
		 strlen(token_list[i].token))==0 )
	{

		     ibf=(int*) token_list[i].len;
		     if(*ibf==-1) *ibf=1;// first time encounter
		     else (*ibf)++;

	}
    }
  if(ibf!=NULL) return *ibf; // otherwise a SEGFAULT occurs when there
  // is no matching name
  return 0;
}

static int set_fnct(char *name,void *in)
{
  int i;
  sigfun *dbf;


  for(i=0;token_list[i].token!=NULL;i++)
    {
      if(strncmp(name,token_list[i].token,
		 strlen(token_list[i].token))==0&&
	 strncmp(context,token_list[i].ctx,
		 strlen(token_list[i].ctx))==0 )
	{
	     if(token_list[i].scalar==FUN) //double
	       {
		 dbf=(sigfun *) token_list[i].ptr;
		 *dbf=(sigfun) in;
	      	 return 0;
	       }
	     else
	       {
		 fprintf(stderr,"Error: Value for %s out of range\n",
			 token_list[i].token);
		 return 2;
	       }
	}


    }


  return 1;
}


static int set_exp(char *name,double value,int scalar)
{
  int i;
  double *dbf;
  int *ibf;
  int *xibf;
  for(i=0;token_list[i].token!=NULL;i++)
    {
      if(strncmp(name,token_list[i].token,
		 strlen(token_list[i].token))==0 &&
	 strncmp(context,token_list[i].ctx,
		 strlen(token_list[i].ctx))==0
	 )
	{
	     if(token_list[i].scalar==DOUBLE) //double
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {
		     dbf=(double*) token_list[i].ptr;
		     *dbf=value;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }
	     if(token_list[i].scalar==INT) //int
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {
		     ibf=(int*) token_list[i].ptr;
		     *ibf=value;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }

	     if(token_list[i].scalar==COUNTER) //counter
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {

		     ibf=(int*) token_list[i].ptr;
		     if(!((*ibf == -1) || (*ibf == (int) value))) {
		       glb_warning("Given length does not"
				   " match actual length");
		       return 2;

		     }
		     *ibf=value;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }

	     if(token_list[i].scalar==INT_INDEXED) //int
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {
		     xibf=(int*) token_list[i].ptr;
		     xibf[loc_count-1]=(int) value;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }

	     if(token_list[i].scalar==DOUBLE_INDEXED) //int
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {
		     dbf=(double*) token_list[i].ptr;
		     dbf[loc_count-1]=(double) value;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }


	   }
       }

   return 1;
}

static int set_pair(char *name,double value,double value2,int scalar)
{
  int i;
  double *dbf;
  int *ibf;
  for(i=0;token_list[i].token!=NULL;i++)
    {
      if(strncmp(name,token_list[i].token,
		 strlen(token_list[i].token))==0&&
	 strncmp(context,token_list[i].ctx,
		 strlen(token_list[i].ctx))==0 )
	{
	     if(token_list[i].scalar==DOUBLE_PAIR)
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {

		     dbf=(double*) token_list[i].ptr;
		     dbf[0]=(double) value;
		     dbf[1]=(double) value2;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }

	     if(token_list[i].scalar==INT_INDEXED_PAIR) //int
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {

		     ibf=(int*) token_list[i].ptr;
		     ibf[(loc_count-1)+0*32]=(int) value;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }

	     if(token_list[i].scalar==DOUBLE_INDEXED_PAIR) //int
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {

		     dbf=(double*) token_list[i].ptr;
		     dbf[(loc_count-1)+0*32]=(double) value;
		     dbf[(loc_count-1)+1*32]=(double) value2;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }

	     if(token_list[i].scalar==DOUBLE_INDEXED_PAIR_INV) //int
	       {
		 if(value >= token_list[i].rl && value <= token_list[i].ru)
		   {

		     dbf=(double*) token_list[i].ptr;
		     dbf[(loc_count-1)*2+0]=(double) value;
		     dbf[(loc_count-1)*2+1]=(double) value2;
		     return 0;
		   }
		 else
		   {
		     fprintf(stderr,"Error: Value for %s out of range\n",
			     token_list[i].token);
		     return 2;
		   }
	       }
	   }
       }

   return 1;
}


static int set_string(char *name, char *str)
{
  int i;
  for(i=0; token_list[i].token !=NULL; i++)
  {
    if (strncmp(name, token_list[i].token, strlen(token_list[i].token)) == 0)
    {
      char **p = (char **)(token_list[i].ptr);
      *p = strdup(str);
      return 0;
    }
  }
  return 1;
}


static size_t list_length (glb_List *head)
{
  size_t n;
  for (n = 0; head; ++n)
    head = head->next;
  return n;
}


static void list_free(glb_List *stale)
{
 glb_List *ptr;
 glb_List *dummy;
 ptr=stale;
 while(ptr != (glb_List *) NULL)
   {
     dummy=ptr->next;
     glb_free(ptr);
     ptr=dummy;
   }

}


static double glb_reverse(double x)
{
  return x;
}


static glb_List *list_cons (glb_List *tail, double newdata)
{
  glb_List *res = (glb_List*) glb_malloc(sizeof(glb_List));
  res->next=tail;
  res->entry=newdata;
  return res;
}

/* this functions threads a funct_t function (double f (double x))
   over a list */

static double glb_list_copy(double x)
{
  return x;
}

static glb_List *thread_list(func_t f, int reverse, int destroy ,glb_List *tail)
{
  glb_List *res, *head;
  size_t n,l;
  double nv;
  res=NULL;
  /* the problem is that threading reverses the list, hence we
     re-reverse it by default, unless someone wants to reverse himself
  */
  if(reverse==1)
    {
      head=tail;
      for (n = 0; head; ++n)
	{
	  nv=f(head->entry);
	  res=list_cons(res,nv);
	  head = head->next;
	}
    }
  else
    {
      double *rlist,x;
      l=list_length(tail);
      rlist=(double *) malloc(sizeof(double)*l);
      head=tail;
      for (n = 0; head; ++n)
	{
	  rlist[n]=head->entry;
	  head = head->next;
	}

      for(n=l;n>0; n--)
	{
	  x=f(rlist[n-1]);
	  res=list_cons(res,x);
	}
      free(rlist);
    }
  if(destroy==1) list_free(tail);
  if(destroy==-1) {list_free(res);res=tail;}
  return res;
}


static void showlist(glb_List *lp)
{

  if (lp){
    showlist(lp->next);             // show the tail
    printf("%.10g " , lp->entry);     // show the head
  }

}

static double list_take(glb_List *li,int k)
{
  int i;
  double erg;
  glb_List *bf;
  bf=li;
  erg=bf->entry;
  for(i=0;i<k;i++)
    {
      bf=bf->next;
      erg=bf->entry;
    }
  return erg;
}


static glb_List *glb_interpolation(glb_List *xval,glb_List *yval,int flag,glb_List *where)
{
  glb_List *head,*res=NULL;
  size_t xl,yl,rl,i;
  double *xlist,*ylist,*rlist,x;
  gsl_interp_type type;

  if(flag==1) type=*gsl_interp_linear;
  else if (flag==2) type=*gsl_interp_cspline;
  else {glb_error("Invalid interpolation scheme flag");return NULL;}

  xl=list_length(xval);
  yl=list_length(yval);
  rl=list_length(where);

  if(yl!=xl) {glb_error("Xval and Yval in glb_interpolation are not of the same length"); return NULL;}

  xlist=(double*) malloc(sizeof(double)*xl);
  ylist=(double*) malloc(sizeof(double)*yl);
  rlist=(double*) malloc(sizeof(double)*rl);

  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc (&type,yl);

  head=xval;
  for (i = yl-1; head; i--){
    xlist[i]=head->entry;
    head = head->next;
  }

  head=yval;
  for (i = yl-1; head; i--){
    ylist[i]=head->entry;
    head = head->next;
  }

  gsl_spline_init(spline,xlist,ylist,yl);

  head=where;
  for(i=0; head; i++){
     rlist[i]=gsl_spline_eval(spline,head->entry,acc);
    head=head->next;
 }

  for(i=rl;i>0;i--) res=list_cons(res,rlist[i-1]);

  free(xlist);
  free(ylist);
  free(rlist);
  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);

  return res;
}


static int set_exp_list(char *name,glb_List *value,int scalar)
{
  int i,k;
  double **dbf;
  int **ibf;
  int *lbf;
  int len;
  double val;
  double *list;
  int *ilist;
  for(i=0;token_list[i].token!=NULL;i++)
    {
      if(strncmp(name,token_list[i].token,strlen(token_list[i].token))==0 &&
	 strncmp(context,token_list[i].ctx,
		 strlen(token_list[i].ctx))==0)
	{
	  switch((int) token_list[i].scalar) {
	  case DOUBLE_LIST:

	    //here we will have to do a lot asking asf.
	    len=list_length(value); // how long is the list
	    lbf=(int*) token_list[i].len;
	    if(*lbf==-1) *lbf=len;  // setting the length correctly in exp
	    else if(*lbf!=len) glb_warning("Length mismatch or list"
					   " length changed");


	    dbf = (double**) token_list[i].ptr;
	    if(*dbf!=NULL){glb_free(*dbf);*dbf=NULL;}
	    list=(double*) glb_malloc(sizeof(double)*len);
	    *dbf=list;


	    for(k=0;k<len;k++)
	       {
		  val=list_take(value,len-k-1);

		  if(val >= token_list[i].rl && val <= token_list[i].ru)
		    {
		      list[k]=val;
		    }
		  else
		    {
		      fprintf(stderr,"Error: Value for %s out of range\n",
			      token_list[i].token);
		      glb_free(list);
		      *dbf=NULL;
		      return 2;
		    }

	       }
	    if(scalar!=TWICE)  list_free(value);
	    return 0;
	    break;

	  case DOUBLE_LIST_INDEXED:
	    len=list_length(value); // how long is the list
	    lbf=(int*) token_list[i].len;


	    //  lbf[loc_count-1]=len;  // setting the length correctly in exp

	    dbf= (double**) token_list[i].ptr;
	    if(dbf[loc_count-1]!=NULL){glb_free(dbf[loc_count-1]);dbf[loc_count-1]=NULL;}
	    list=(double*) glb_malloc(sizeof(double)*(len+1));

	    dbf[loc_count-1]=list;
	    list[len]=-1;

	    for(k=0;k<len;k++)
	      {
		val=list_take(value,len-k-1);

		if(val >= token_list[i].rl && val <= token_list[i].ru)
		  {
		    list[k]=val;

		  }
		else
		  {
		    fprintf(stderr,"Error: In line %d: "
			    "Value for %s out of range\n",
			    glb_line_num,token_list[i].token);
		    glb_free(list);
		    dbf[loc_count-1]=NULL;

		   return 2;
		  }
	      }
	    if(scalar!=TWICE) list_free(value);
	    return 0;
	    break;


	  case INT_LIST_INDEXED: //integer list indexed
	    //with loc_counter

	    //here we will have to do a lot asking asf.
	    len=list_length(value); // how long is the list
	    lbf=(int*) token_list[i].len; //FIXME danger !!!!
	    lbf[loc_count-1]=len;  // setting the length correctly in exp
	    ibf= (int**) token_list[i].ptr;
	    if(ibf[loc_count-1]!=NULL)glb_free(ibf[loc_count-1]);
	    ilist=(int*) glb_malloc(sizeof(int)*len);

	    ibf[loc_count-1]=ilist;


	    for(k=0;k<len;k++)
	      {
		val=list_take(value,len-k-1);

		if(val >= token_list[i].rl && val <= token_list[i].ru)
		  {
		    ilist[k]=(int) val;

		  }
		else
		  {
		    fprintf(stderr,"Error: Value for %s out of range\n",
			    token_list[i].token);
		   glb_free(ilist);
		    return 2;
		  }
	      }
	    if(scalar!=TWICE) list_free(value);
	    return 0;
	    break;
	  default:
	    return 1;
	    break;
	  }
	}
    }
  return 1;
}

static int set_exp_energy(char *name, glb_List **value)
{
  int i,k,l;
  double ***dbf;
  int len;
  double val;
  int v1,v2;
  double **list;

  for(i=0;token_list[i].token!=NULL;i++)
    {
      if(strncmp(name,token_list[i].token,strlen(token_list[i].token))==0&&
	 strncmp(context,token_list[i].ctx,
		 strlen(token_list[i].ctx))==0 )
	{
	  switch((int) token_list[i].scalar) {

	  case ENERGY_MATRIX:
	    list=(double**) glb_malloc(sizeof(double* ) * energy_len);
	    buff.lowrange[loc_count-1]=(int*) glb_malloc(energy_len*sizeof(int));
	    buff.uprange[loc_count-1]=(int*) glb_malloc(energy_len*sizeof(int));

	    for(l=0;l<energy_len;l++)
	      {
		len=(int) list_length(value[l]); // how long is the list
		if(len<2) {fprintf(stderr,"Error: in line %d: in @smear "
				   "number %d: sublist %d is too short!\n"
				   ,glb_line_num,loc_count,l);return 2;}
		//lbf=(int*) token_list[i].len;


		//lbf[loc_count-1]=len;  // setting the length correctly in exp



		dbf= (double***) token_list[i].ptr;
		list[l]=(double*) glb_malloc(sizeof(double)*(len-2));
		dbf[loc_count-1]=list;


		v1=(int) list_take(value[l],len-0-1);
		v2=(int) list_take(value[l],len-1-1);

		if(v1 >= 0 &&  v2 <= buff.simbins
		   && v2 >= v1&&v2-v1==len-3 )
		  {

		    buff.lowrange[loc_count-1][l]= v1;
		    buff.uprange[loc_count-1][l]= v2;

		  }
		else
		  {
		    fprintf(stderr,"Error: In line %d: "
			    "Value for ranges in smear out of range\n",
			    glb_line_num);
		    glb_free(list[l]);
		    glb_free(buff.lowrange[loc_count-1]);
		    glb_free(buff.uprange[loc_count-1]);
		    return 2;
		  }

	      	for(k=0;k<len-2;k++)
		  {
		    val=list_take(value[l],len-(k+2)-1);

		    if(val >= token_list[i].rl && val <= token_list[i].ru)
		      {
			list[l][k]=val;

		      }
		    else
		      {
			fprintf(stderr,"Error: In line %d: "
				"Value for %s out of range\n",
				glb_line_num,token_list[i].token);
			free(list[l]);
			return 2;
		      }
		  }
		list_free(value[l]);
	      }
	    glb_free(value);
	    return 0;
	    break;

	  }
	}
    }
  return 1;
}


 %}
%union {
  double  val;  /* For returning numbers.                   */
  double *dpt;  /* for rules */
  glb_List *ptr;
  glb_List **ptrq;
  glb_symrec  *tptr;  /* For returning symbol-table pointers      */
  char *name;
  char *iname;
  int in;
  glb_namerec *nameptr;
}

%token <val> NUM
%token <nameptr> SFNCT
%token <tptr> BOGUS LVAR VAR FNCT   /* Variable and Function */
%token <name> IDN CROSS FLUXP FLUXM NUFLUX
%token <name> SYS_ON_FUNCTION SYS_OFF_FUNCTION
%token <name> GRP GID FNAME VERS
%token <name> SIGNAL BG
%token <name> ENERGY CHANNEL
%token <name> NDEF
%token <in> GRPOPEN GRPCLOSE
%token <in> PM FLAVOR
%token <in> NOGLOBES
%token <in> RULESEP RULEMULT
%token <nameptr> NAME RDF
%type <ptr> seq list listcopy
%type <ptrq> rule brule srule energy ene
%type <dpt> rulepart
%type <val> exp
%type <val> group
%type <in> pm
%type <nameptr> name
%type <name> cross
%type <name> flux
%type <name> nuflux

%type <name> version

%expect 2
%nonassoc ','
%right '='
%left RULESEP
%left RULEMULT
%right '>'
%right '}'
%left  '{'
%left '-' '+'
%left '*' '/'
%left NEG     /* Negation--unary minus */
%right '^'    /* Exponentiation        */

%%
/* Grammar rules and actions */
/* ------------------------- */

/* input: This is where the parser starts */
input: topleveldirective {}
| input topleveldirective {}
| NOGLOBES {YYABORT;}
| '§' {YYABORT;}
;

/* topleveldirective: anything that can appear outside any groups */
topleveldirective: group {}
| exp {}
| list {}
;

/* exp: An expression, including assignments, algebraic expressions, etc. */
exp: NUM             { $$ = $1;                     }
| NAME               { $$ = $1->value;              }
| VAR                { $$ = $1->value.var;          }
| VAR '=' exp        { $$ = $3; $1->value.var = $3; }
| IDN '=' exp {
  if(set_exp($1,$3,0)==1) yyerror("Unknown identifier: %s", $1);
  $$ = $3;
  if ($1)  { glb_free($1);  $1=NULL; }
}
| IDN '=' SFNCT {
  if(set_fnct($1,$3->sf)==1) yyerror("Unknown identifier: %s", $1);
  if ($1)  { glb_free($1);  $1=NULL; }
}
| IDN '=' exp RULESEP exp {
  if(set_pair($1,$3,$5,0)==1) yyerror("Unknown identifier: %s", $1);
  $$ = $3;
  if ($1)  { glb_free($1);  $1=NULL; }
}
| FNCT '(' exp ')'   {
  /* added safety in case the function pointer is NULL, which is
     sometimes useful for special functions */
  if($1->value.fnctptr==NULL) yyerror("Improper use of special function %s", $1->name);
  else $$ = (*($1->value.fnctptr))($3); }

| exp '+' exp        { $$ = $1 + $3;      }
| exp '-' exp        { $$ = $1 - $3;      }
| exp '*' exp        { $$ = $1 * $3;      }
| exp '/' exp        { $$ = $1 / $3;      }
| '-' exp  %prec NEG { $$ = -$2;          }
| exp '^' exp        { $$ = pow ($1, $3); }
| '(' exp ')'        { $$ = $2;           }
| version            { $$ = 0;            }
| NDEF               { yyerror("Unknown name: %s", $1); YYERROR; }
;

/* listcopy: A statement that duplicates a list */
listcopy: IDN RULESEP '=' LVAR {
  glb_List *ltemp;
  ltemp=thread_list(&glb_list_copy,0,0,$4->list);
  if(set_exp_list($1,ltemp,3)==1) yyerror("Unknown identifier");
  $$ = ltemp;
  if ($1)  { glb_free($1);  $1=NULL; }
}

/* seq: A comma-separated sequence of expressions */
seq: exp ',' exp {
   glb_List *buf = list_cons(NULL, $1);
   buf = list_cons(buf, $3);
   $$  = buf;
}
| seq ',' exp { $$ = list_cons($1, $3); }
;

/* list: A list, i.e. a sequence enclosed in { } */
list: '{' '}'  {$$=NULL;}
| '{' seq '}'  {$$=$2; }
| '{' exp '}'  {$$=list_cons(NULL,$2); }
| IDN '=' list {
  if(set_exp_list($1,$3,3)==1)  yyerror("Unknown identifier");
  $$ = $3;
  if ($1)  { glb_free($1);  $1=NULL; }
}
| FNCT '(' seq ')' {$$ = thread_list($1->value.fnctptr,$1->reverse,$1->destroy,$3);}
| FNCT '(' list ')' {$$ = thread_list($1->value.fnctptr,$1->reverse,$1->destroy,$3);}
| FNCT '('')' {$$ = (*($1->value.lfnctptr))();}
| LVAR                { $$ = $1->list;              }
| LVAR '=' list       { $$ = $3; $1->list = $3; }
| FNCT '(' list ',' list ',' exp  ',' list ')'          {$$=glb_interpolation($3,$5,floor($7),$9);}
| listcopy {}
;

/* rulepart: Building block of signal and background rules */
rulepart: exp RULEMULT exp {
  double *buf;
  buf=(double*) glb_malloc(sizeof(double)*2);
  buf[0]=$1;
  buf[1]=$3-1;
  $$=buf;
}
;

/* group: An environment containing statements that belong together */
group: GID '(' NAME ')'
{ if($3->value==-1) {$3->value=step_counter($1); }
  loc_count=$3->value;
  glb_free(context);
  context =(char *) strdup($1);
  grp_start(context);
  if ($1)  { glb_free($1);  $1=NULL; }
}
GRPOPEN ingroup GRPCLOSE {
  grp_end(context);
}
| GID '(' RDF ')' GRPOPEN ingroup  GRPCLOSE {
    yyerror("Redefinition of an automatic variable %s", $3->name); YYERROR;
    if ($1)  { glb_free($1);  $1=NULL; }
}
;

/* ingroup: The contents of a group environment */
ingroup: /* empty */
| ingroup ingroup_statement
;

/* ingroup_statement: A single statement inside a group */
ingroup_statement: exp {}
| list {}
| rule {}
| energy {}
| channel {}
| cross {}
| flux {}
| nuflux {}
;

/* version: A version declaration */
version: VERS '=' FNAME {
//  buff.version=strdup($3);
  if (set_string($1, $3) != 0)
    yyerror("Unknown identifier: %s", $1);
  if ($1)  { glb_free($1);  $1=NULL; }
  if ($3)  { glb_free($3);  $3=NULL; }
}
;

/* cross: Specification of a cross sections file (to be used inside cross group) */
cross: CROSS '=' FNAME {
  //load_cross($3,loc_count-1);
  xsc.file_name=strdup($3);
  $$=$3;
  if ($1)  { glb_free($1);  $1=NULL; }
  if ($3)  { glb_free($3);  $3=NULL; }
}
;

/* flux: Specification of flux file (to be used inside flux group) */
flux: FLUXP '=' FNAME {
  //load_flux($3,loc_count-1,1);
  flt.file_name=strdup($3);

  //if(set_exp($1,$3,0)==1) yyerror("Unknown identifier");
  $$=$3;
  if ($1)  { glb_free($1);  $1=NULL; }
  if ($3)  { glb_free($3);  $3=NULL; }
}
;

/* nuflux: same as flux, but for nuflux group */
nuflux: NUFLUX '=' FNAME {
  //load_flux($3,loc_count-1,1);
  flt.file_name=strdup($3);

  //if(set_exp($1,$3,0)==1) yyerror("Unknown identifier");
  $$=$3;
  if ($1)  { glb_free($1);  $1=NULL; }
  if ($3)  { glb_free($3);  $3=NULL; }
}
;

/* channel: A channel definition */
channel: CHANNEL '=' name RULESEP pm RULESEP FLAVOR RULESEP FLAVOR RULESEP
 name RULESEP name {

  int x[6];
  x[0]=$3->value - 1 ;
  x[1]=$5;
  x[2]=$7;
  x[3]=$9;
  x[4]=(int) $11->value -1;
  x[5]=(int) $13->value -1;

  set_channel_data(x,loc_count);
  if ($1)  { glb_free($1);  $1=NULL; }
}
;

/* name */
/* FIXME, maybe we had a bug here */
name: NAME {$$=$1;}
| NDEF { yyerror("Unknown name: %s", $1); YYERROR; }
;

/* unary plus/minus */
pm: PM {$$=$1;}
| '+' {$$=1;}
| '-' {$$=-1;}
;

/* ene: Building block of user-defined smearing matrix */
ene: ENERGY '='  list   {
  glb_List **buf;
  energy_len=1;

  buf=(glb_List**) glb_malloc(sizeof( glb_List* ) );
  buf[0]=$3;
  $$=buf;
  if ($1)  { glb_free($1);  $1=NULL; }
}
| ene RULESEP list
{
  glb_List **buf;
  buf=$1;
  energy_len++;

  buf=(glb_List**) glb_realloc((void**) buf , sizeof( glb_List* ) * energy_len);

  buf[energy_len-1]=$3;
  $$=buf;
}
;

/* energy: User-defined smearing matrix */
energy: ene { set_exp_energy("@energy",$1); }
| ene ';'   { set_exp_energy("@energy",$1); }
;

/* brule: Background rule */
brule: BG '=' rulepart {
  glb_List **buf;
  buf=(glb_List**) glb_malloc(sizeof(glb_List*)*2);
  buf[0]=list_cons(NULL,$3[0]);
  buf[1]=list_cons(NULL,$3[1]);
  glb_free($3);
  if ($1)  { glb_free($1);  $1=NULL; }
  $$=buf;
}
| brule RULESEP rulepart {
  glb_List **buf;
  buf=$1;
  buf[0]=list_cons(buf[0],$3[0]);
  buf[1]=list_cons(buf[1],$3[1]);
  glb_free($3);
  $$=buf;
}
;

/* srule: Signal rule */
srule: SIGNAL '=' rulepart {
  glb_List **buf;

  buf=(glb_List**) glb_malloc(sizeof(glb_List*)*2);
  buf[0]=list_cons(NULL,$3[0]);
  buf[1]=list_cons(NULL,$3[1]);
  glb_free($3);
  if ($1)  { glb_free($1);  $1=NULL; }
  $$=buf;
}
| srule RULESEP rulepart {
  glb_List **buf;
  buf=$1;
  buf[0]=list_cons(buf[0],$3[0]);
  buf[1]=list_cons(buf[1],$3[1]);
  glb_free($3);
  $$=buf;
}
;

/* rule: Anything that can appear inside a rule environment */
rule: brule {
  int flag;
  $$=$1;
  flag=set_exp_list("bgrulescoeff",$1[0],0);
  if(flag==1) yyerror("Invalid coefficient in @background");
  flag=set_exp_list("bgrulechannellist",$1[1],0);
  if(flag==1) yyerror("Invalid channel in @background");
  glb_free($1);
}
| srule {
  int flag;
  $$=$1;
  flag=set_exp_list("rulescoeff",$1[0],0);
  if(flag==1) yyerror("Invalid coefficient in @signal");
  flag=set_exp_list("rulechannellist",$1[1],0);
  if(flag==1) yyerror("Invalid channel in @signal");
  glb_free($1);
}
| SYS_ON_FUNCTION '=' FNAME {
  buff.sys_on_strings[buff.numofrules-1] = strdup($3);
  if ($1)  { glb_free($1);  $1=NULL; }
  if ($3)  { glb_free($3);  $3=NULL; }
}
| SYS_OFF_FUNCTION '=' FNAME {
  buff.sys_off_strings[buff.numofrules-1] = strdup($3);
  if ($1)  { glb_free($1);  $1=NULL; }
  if ($3)  { glb_free($3);  $3=NULL; }
}
;

%%

extern glb_symrec *sym_table;



/***************************************************************************
 * Function yyerror                                                        *
 ***************************************************************************
 * Print parser errors including the line number where the error occured   *
 ***************************************************************************/
int yyerror (const char *s, ...)  /* Called by yyparse on error */
{
  va_list args;
  va_start(args, s);

  if(yydebug > 0) fprintf(stderr,"*****************************************\n");
  fprintf (stderr,"%s:%d: error: ",
	   glb_file_id, glb_line_num+1);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  if(yydebug > 0) fprintf(stderr,"*****************************************\n");
  va_end(args);
  return 0;
}

int
yywarn (const char *s)  /* Called by yyparse on warning */
{
  fprintf (stderr,"%s:%d: warning: %s\n",
	   glb_file_id, glb_line_num+1, s);
  return 0;
}




struct glb_init
{
  char *fname;
  double (*fnct)(double);
  /* these two flags determine the behaviour under threading
     reverse 0 leaves the list as is (i.e reverse)
     reverse 1 reverse the list (right order)

     destroy -1 leaves the list (even the memory location) unscathed
     destroy 0  procuces a copy
     destroy 1 destroys the input list and returns a new list
  */
  int reverse;
  int destroy;
};

struct glb_init_list
{
  char *fname;
  glb_List *(*fnct)(void);
};


struct glb_init_sig
{
  char *fname;
  sigfun sf;
};

static double echo(double x)
{
  fprintf(stdout,"%f ",x);
  return x;
}

static double echon(double x)
{
  fprintf(stdout,"%f\n",x);
  return x;
}


static double line(double x)
{
  size_t n,i;
  n=floor(x);
  for(i=0;i<n;i++) fprintf(stdout,"\n");
  return x;
}

static struct glb_init arith_fncts[] =
  {
    {"sin",  sin, 0,1},
    {"cos",  cos,0,1},
    {"tan",  tan,0,1},
    {"asin", asin,0,1},
    {"acos", acos,0,1},
    {"atan", atan,0,1},
    {"log",   log,0,1},
    {"exp",  exp,0,1},
    {"log10", log10,0,1},
    {"sqrt", sqrt,0,1},
    {"reverse",glb_reverse,1,1},
    {"copy",glb_list_copy,0,0},
    {"echo",echo,0,-1},
    {"echon",echon,0,-1},
    {"line",line,0,-1},
    {"interpolation",NULL,0,0},
    {NULL, NULL,0,0}
     };


//void glb_set_up_smear_data(glb_smear *test,const struct glb_experiment *head)
static glb_List *glb_bincenter(void)
{
  int i;
  glb_List *res=NULL;
  glb_smear *test;
  test=glb_smear_alloc();

  if(buff.numofbins<0) {glb_error("Cannot compute bincenter. Binning not set up properly."); return NULL;}

  glb_set_up_smear_data(test,&buff);


  for(i=0;i<test->numofbins;i++)
    {
      res=list_cons(res,test->bincenter[i]);
    }

  glb_smear_free(test);

  return res;
}


static glb_List *glb_samplingbincenter(void)
{
  int i;
  glb_List *res=NULL;
  glb_smear *test;
  test=glb_smear_alloc();

  if(buff.simbins<0) {glb_error("Cannot compute samplingbincenter. Sampling-binning not set up properly."); return NULL;}


  glb_set_up_smear_data(test,&buff);

  for(i=0;i<test->simbins;i++)
    {

      res=list_cons(res,test->simbincenter[i]);
    }

  glb_smear_free(test);


  return res;
}




static struct glb_init_list list_fncts[] =
  {
    {"bincenter",glb_bincenter},
    {"samplingbincenter",glb_samplingbincenter},
    {NULL, NULL}
  };

static double standard_sig(double x ,double *in)
{

  return in[0]*x + in[1]*sqrt(x) + in[2];
}


static double chooz_sig(double x, double *in)
{
  if(x<=0.0018) return in[0]/1000.0;
  else return in[0]*sqrt((x-0.0018+0.001)*1000.0)/1000.0;
}

/* Number of parameters ? */
struct glb_init_sig sig_fncts[] =
  {
    {"#standard",  standard_sig},
    {"#inverse_beta", chooz_sig},
    {NULL, NULL}
  };

/* The symbol table: a chain of `struct glb_symrec'.  */
static glb_namerec *name_table = (glb_namerec *) NULL;
/* cannot use static here, since its declared earlier as extern */
glb_symrec *sym_table = (glb_symrec *) NULL;
glb_symrec *pre_sym_table = (glb_symrec *) NULL;

#define BIN_LIST 1
#define SAMPLING_LIST 2
#define DENSITY_LIST 3


/* Put arithmetic functions in table.
 * And all user-defined stuff.
 */
static void
init_table (void)
{
  int i;
  glb_symrec *ptr,*p;

  glb_namerec *sptr;
  for (i = 0; arith_fncts[i].fname != 0; i++)
    {
      ptr = glb_putsym (arith_fncts[i].fname, FNCT);
      ptr->value.fnctptr = arith_fncts[i].fnct;
      ptr->destroy=arith_fncts[i].destroy;
      ptr->reverse=arith_fncts[i].reverse;

    }
  for (i = 0; list_fncts[i].fname != 0; i++)
    {
      ptr = glb_putsym (list_fncts[i].fname, FNCT);
      ptr->value.lfnctptr = list_fncts[i].fnct;
    }

  ptr=pre_sym_table;
  /* Copying the contents of pre_sym_table to sym_table */
  for (i=0; ptr !=0; i++)
    {
      p=glb_putsym(ptr->name,ptr->type);
      p->value.var=ptr->value.var;
      if(ptr->list!=NULL) p->list=thread_list(&glb_list_copy,0,0,ptr->list);
    ptr=ptr->next;
    }

  for (i = 0; sig_fncts[i].fname != 0; i++)
    {
      sptr = glb_putname (sig_fncts[i].fname,"energy",SFNCT);
      sptr->sf = sig_fncts[i].sf;
    }
}

static void
free_symtable()
{
    glb_symrec *ptr;
    glb_symrec *dummy;
    ptr=sym_table;
    while(ptr != (glb_symrec *) NULL)
      {
	glb_free(ptr->name);
	if(ptr->list!=NULL){ list_free(ptr->list);}
	dummy=ptr->next;
	glb_free(ptr);
	ptr=dummy;
      }
    sym_table=NULL;
}

static void
free_presymtable()
{
    glb_symrec *ptr;
    glb_symrec *dummy;
    ptr=pre_sym_table;
    while(ptr != (glb_symrec *) NULL)
      {
	glb_free(ptr->name);
	if(ptr->list!=NULL){ list_free(ptr->list);}
	dummy=ptr->next;
	glb_free(ptr);
	ptr=dummy;
      }
    pre_sym_table=NULL;
}




static void
free_nametable()
{
    glb_namerec *ptr;
    glb_namerec *dummy;
    ptr=name_table;
    while(ptr != (glb_namerec *) NULL)
      {
	glb_free(ptr->name);
	glb_free(ptr->context);
	dummy=ptr->next;
	glb_free(ptr);
	ptr=dummy;
      }
    name_table=NULL;
}

glb_symrec *
glb_putsym (char *sym_name, int sym_type)
{
  glb_symrec *ptr;
  ptr = (glb_symrec *) glb_malloc (sizeof (glb_symrec));
  ptr->name = (char *) glb_malloc (strlen (sym_name) + 1);
  strcpy (ptr->name,sym_name);
  ptr->type = sym_type;
  ptr->value.var = GLB_NAN; /* set value to GLB_NAN, which ensures an
			       error if an undefined variable is used  */
  ptr->list= NULL;
  ptr->next = (struct glb_symrec *) sym_table;
  sym_table = ptr;
  return ptr;
}





/* Name handling */

static glb_naming *glb_putnames (char *sym_name, char *context, int value,
				 glb_naming *in)
{
  glb_naming *ptr;
  ptr = (glb_naming *) glb_malloc (sizeof (glb_naming));
  ptr->name = (char *) glb_malloc (strlen (sym_name) + 1);
  strcpy (ptr->name,sym_name);
  ptr->context = (char *) glb_malloc (strlen (context) + 1);
  strcpy (ptr->context,context);
  ptr->value = value; /* set value to -1 for new ones  */

  ptr->next = (struct glb_naming *) in;
  //in = ptr;
  return ptr;
}

static glb_naming *copy_names (glb_naming *in)
{

  glb_namerec *ptr;
  for (ptr = name_table; ptr != (glb_namerec *) NULL;
       ptr = (glb_namerec *)ptr->next)
    {
      in=glb_putnames(ptr->name,ptr->context,ptr->value,in);
    }
  return in;
}



glb_namerec *glb_getname (const char *sym_name, char* context)
{
  glb_namerec *ptr;
  for (ptr = name_table; ptr != (glb_namerec *) NULL;
       ptr = (glb_namerec *)ptr->next)
    if (strcmp (ptr->name,sym_name) == 0 )
	return ptr;
  return 0;
}

glb_namerec *glb_putname (char *sym_name, char *context, int sym_type)
{
  glb_namerec *ptr;
  ptr = (glb_namerec *) glb_malloc (sizeof (glb_namerec));
  ptr->name = (char *) glb_malloc (strlen (sym_name) + 1);
  strcpy (ptr->name,sym_name);
  ptr->context = (char *) glb_malloc (strlen (context) + 1);
  strcpy (ptr->context,context);
  ptr->type = sym_type;
  ptr->value = -1; /* set value to -1 for new ones  */

  ptr->next = (struct glb_namerec *)name_table;
  name_table = ptr;
  return ptr;
}

glb_symrec *
glb_getsym (const char *sym_name)
{
  glb_symrec *ptr;
  for (ptr = sym_table; ptr != (glb_symrec *) NULL;
       ptr = (glb_symrec *)ptr->next)
    if (strcmp (ptr->name,sym_name) == 0)
      return ptr;
  return 0;
}
/* User pre-def variables */
glb_symrec *
glb_putpresym (const char *sym_name, int sym_type)
{
  glb_symrec *ptr;
  ptr = (glb_symrec *) glb_malloc (sizeof (glb_symrec));
  ptr->name = (char *) glb_malloc (strlen (sym_name) + 1);
  strcpy (ptr->name,sym_name);
  ptr->type = sym_type;
  ptr->value.var = 0; /* set value to 0 even if fctn.  */
  ptr->list=NULL;
  ptr->next = (struct glb_symrec *) pre_sym_table;
  pre_sym_table = ptr;
  return ptr;
}

glb_symrec *
glb_getpresym (const char *sym_name)
{
  glb_symrec *ptr;
  for (ptr = pre_sym_table; ptr != (glb_symrec *) NULL;
       ptr = (glb_symrec *)ptr->next)
    if (strcmp (ptr->name,sym_name) == 0)
      return ptr;
  return 0;
}


/* A new an powerful function which allows the user to define variables
 * for substitution in the AEDL files
 */


void glbDefineAEDLVariable(const char *name, double value)
{
  glb_symrec *ptr;
  ptr=glb_getpresym(name);
  if(ptr==0) ptr = glb_putpresym (name, VAR);
  ptr->value.var = value;
  return;
}

double glbGetAEDLVariable(const char *name)
{
  glb_symrec *ptr;
  ptr=glb_getpresym(name);
  if (!ptr)
    return GLB_NAN;
  return ptr->value.var;
}

void glbClearAEDLVariables()
{
   if(pre_sym_table!=NULL) free_presymtable();
}


/* same for lists */

void glbDefineAEDLList(const char *name, double *list, size_t length)
{
  size_t i;
  glb_symrec *ptr;
  if(name==NULL) return;
  if(name[0]!='%'){ fprintf(stderr,"ERROR: AEDL lists have to start with '\%'\n");return;}
  ptr=glb_getpresym(name);
  if(ptr==0) ptr = glb_putpresym (name, LVAR);
  for(i=0;i<length;i++) {ptr->list=list_cons(ptr->list,list[i]);
  }

  return;
}




void glb_copy_buff()
{
  /* I am not sure how well this assigment really works */
  buff.names=copy_names(buff.names);
  buff.filename=strdup(glb_file_id);
  buff_list[exp_count]=buff;
  exp_count++;
}

void glbReset()
{
  glb_line_num=0;
  energy_len=1;
  energy_count=-1;
  loc_count=-1;
  flux_count=-1;
  glbInitExp(&buff);

  if(name_table!=NULL) free_nametable();
  if(sym_table!=NULL) free_symtable();
  name_table =(glb_namerec *) NULL;
  sym_table =(glb_symrec *) NULL;
  init_table ();
}

void glbResetCounters()
{
  flux_count=-1;
  cross_count=-1;
}

void glbResetEOF()
{
  int i;
  exp_count=0;
  glb_line_num=0;
  energy_len=1;
  energy_count=-1;
  loc_count=-1;
  flux_count=-1;
  glbInitExp(&buff);
  for(i=0;i<GLB_MAX_EXP;i++)   glbInitExp(&buff_list[i]);
  /* this here would be the place to check for unuses variables, but
     for that we need an access counter */
  if(name_table!=NULL) free_nametable();
  if(sym_table!=NULL) free_symtable();

  name_table =(glb_namerec *) NULL;
  sym_table =(glb_symrec *) NULL;
  init_table ();
  glb_reset_flux(&flt);
  glb_reset_xsec(&xsc);

}

void glb_clean_parser()
{
  if(name_table!=NULL) free_nametable();
  if(sym_table!=NULL) free_symtable();
  if(pre_sym_table!=NULL) free_presymtable();

}

int glbInitExperiment(char *inf,glb_exp *in, int *counter)
{
  FILE *input;
  int k,i;
  const char tch[]="%!GLoBES";
  char tct[11];
  struct glb_experiment **ins;
  ins=(struct glb_experiment **) in;

  // yydebug=1;
  context=(char *) strdup("global");
  glb_smear_reset(&ibf);
  glb_option_type_reset(&opt);
  memset(&flt, 0, sizeof(flt));
  glb_reset_flux(&flt);
  memset(&xsc, 0, sizeof(xsc));
  glb_reset_xsec(&xsc);
  input=glb_fopen(inf,"r");
  if(input==NULL) return -2;
  /* This line produces a warning with -Wall:
   * 'warning: char format, different type arg (arg 3)'
   * I think however that understand the problem.
   */
  fscanf(input,"%8c",&tct);
  if(strncmp(tch,tct,8)!=0) {glb_error("Not a GLoBES file!"); return -2;}
  glb_fclose(input);
  yyin=glb_fopen(inf,"r");
  if(yyin==NULL) return -2;
  glb_file_id=(char*) strdup(inf);
  glbResetEOF();
  k=yyparse ();

  glb_fclose(yyin);
  glb_free(context);
  glb_free(glb_file_id);

  if(k!=0) return -2;

  k=0;

  if(*counter+exp_count>GLB_MAX_EXP) glb_fatal("Too many experiments!");
  for(i=0;i<exp_count;i++)
    {
      *ins[*counter+i]=buff_list[i];
      k=+glbDefaultExp(ins[*counter+i]);
    }
  (*counter)= (*counter) + exp_count;

  /* Reallocate data structures for the minimizer */
  glb_init_minimizer();

  if(k!=0) return -1;

  return 0;

}
