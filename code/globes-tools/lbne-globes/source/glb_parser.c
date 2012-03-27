
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 22 "glb_parser.y"

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


 

/* Line 189 of yacc.c  */
#line 1108 "glb_parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     SFNCT = 259,
     BOGUS = 260,
     LVAR = 261,
     VAR = 262,
     FNCT = 263,
     IDN = 264,
     CROSS = 265,
     FLUXP = 266,
     FLUXM = 267,
     NUFLUX = 268,
     SYS_ON_FUNCTION = 269,
     SYS_OFF_FUNCTION = 270,
     GRP = 271,
     GID = 272,
     FNAME = 273,
     VERS = 274,
     SIGNAL = 275,
     BG = 276,
     ENERGY = 277,
     CHANNEL = 278,
     NDEF = 279,
     GRPOPEN = 280,
     GRPCLOSE = 281,
     PM = 282,
     FLAVOR = 283,
     NOGLOBES = 284,
     RULESEP = 285,
     RULEMULT = 286,
     NAME = 287,
     RDF = 288,
     NEG = 289
   };
#endif
/* Tokens.  */
#define NUM 258
#define SFNCT 259
#define BOGUS 260
#define LVAR 261
#define VAR 262
#define FNCT 263
#define IDN 264
#define CROSS 265
#define FLUXP 266
#define FLUXM 267
#define NUFLUX 268
#define SYS_ON_FUNCTION 269
#define SYS_OFF_FUNCTION 270
#define GRP 271
#define GID 272
#define FNAME 273
#define VERS 274
#define SIGNAL 275
#define BG 276
#define ENERGY 277
#define CHANNEL 278
#define NDEF 279
#define GRPOPEN 280
#define GRPCLOSE 281
#define PM 282
#define FLAVOR 283
#define NOGLOBES 284
#define RULESEP 285
#define RULEMULT 286
#define NAME 287
#define RDF 288
#define NEG 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 1055 "glb_parser.y"

  double  val;  /* For returning numbers.                   */
  double *dpt;  /* for rules */
  glb_List *ptr;
  glb_List **ptrq;
  glb_symrec  *tptr;  /* For returning symbol-table pointers      */
  char *name;
  char *iname;
  int in;
  glb_namerec *nameptr;



/* Line 214 of yacc.c  */
#line 1226 "glb_parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 1238 "glb_parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  36
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   353

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNRULES -- Number of states.  */
#define YYNSTATES  162

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,    47,    41,    40,    34,    39,     2,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    48,
       2,    35,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    44,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,    37,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    45,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    43
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    28,    32,    36,    42,    47,    51,    55,
      59,    63,    66,    70,    74,    76,    78,    83,    87,    91,
      94,    98,   102,   106,   111,   116,   120,   122,   126,   137,
     139,   143,   144,   153,   161,   162,   165,   167,   169,   171,
     173,   175,   177,   179,   181,   185,   189,   193,   197,   211,
     213,   215,   217,   219,   221,   225,   229,   231,   234,   238,
     242,   246,   250,   252,   254,   258
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      50,     0,    -1,    51,    -1,    50,    51,    -1,    29,    -1,
      45,    -1,    57,    -1,    52,    -1,    55,    -1,     3,    -1,
      32,    -1,     7,    -1,     7,    35,    52,    -1,     9,    35,
      52,    -1,     9,    35,     4,    -1,     9,    35,    52,    30,
      52,    -1,     8,    46,    52,    47,    -1,    52,    40,    52,
      -1,    52,    39,    52,    -1,    52,    41,    52,    -1,    52,
      42,    52,    -1,    39,    52,    -1,    52,    44,    52,    -1,
      46,    52,    47,    -1,    61,    -1,    24,    -1,     9,    30,
      35,     6,    -1,    52,    34,    52,    -1,    54,    34,    52,
      -1,    38,    37,    -1,    38,    54,    37,    -1,    38,    52,
      37,    -1,     9,    35,    55,    -1,     8,    46,    54,    47,
      -1,     8,    46,    55,    47,    -1,     8,    46,    47,    -1,
       6,    -1,     6,    35,    55,    -1,     8,    46,    55,    34,
      55,    34,    52,    34,    55,    47,    -1,    53,    -1,    52,
      31,    52,    -1,    -1,    17,    46,    32,    47,    58,    25,
      59,    26,    -1,    17,    46,    33,    47,    25,    59,    26,
      -1,    -1,    59,    60,    -1,    52,    -1,    55,    -1,    72,
      -1,    69,    -1,    65,    -1,    62,    -1,    63,    -1,    64,
      -1,    19,    35,    18,    -1,    10,    35,    18,    -1,    11,
      35,    18,    -1,    13,    35,    18,    -1,    23,    35,    66,
      30,    67,    30,    28,    30,    28,    30,    66,    30,    66,
      -1,    32,    -1,    24,    -1,    27,    -1,    40,    -1,    39,
      -1,    22,    35,    55,    -1,    68,    30,    55,    -1,    68,
      -1,    68,    48,    -1,    21,    35,    56,    -1,    70,    30,
      56,    -1,    20,    35,    56,    -1,    71,    30,    56,    -1,
      70,    -1,    71,    -1,    14,    35,    18,    -1,    15,    35,
      18,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1112,  1112,  1113,  1114,  1115,  1119,  1120,  1121,  1125,
    1126,  1127,  1128,  1129,  1134,  1138,  1143,  1149,  1150,  1151,
    1152,  1153,  1154,  1155,  1156,  1157,  1161,  1170,  1175,  1179,
    1180,  1181,  1182,  1187,  1188,  1189,  1190,  1191,  1192,  1193,
    1197,  1208,  1207,  1218,  1225,  1226,  1230,  1231,  1232,  1233,
    1234,  1235,  1236,  1237,  1241,  1251,  1261,  1273,  1285,  1303,
    1304,  1308,  1309,  1310,  1314,  1323,  1337,  1338,  1342,  1351,
    1362,  1372,  1383,  1392,  1401,  1406
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "SFNCT", "BOGUS", "LVAR", "VAR",
  "FNCT", "IDN", "CROSS", "FLUXP", "FLUXM", "NUFLUX", "SYS_ON_FUNCTION",
  "SYS_OFF_FUNCTION", "GRP", "GID", "FNAME", "VERS", "SIGNAL", "BG",
  "ENERGY", "CHANNEL", "NDEF", "GRPOPEN", "GRPCLOSE", "PM", "FLAVOR",
  "NOGLOBES", "RULESEP", "RULEMULT", "NAME", "RDF", "','", "'='", "'>'",
  "'}'", "'{'", "'-'", "'+'", "'*'", "'/'", "NEG", "'^'", "'\\247'", "'('",
  "')'", "';'", "$accept", "input", "topleveldirective", "exp", "listcopy",
  "seq", "list", "rulepart", "group", "$@1", "ingroup",
  "ingroup_statement", "version", "cross", "flux", "nuflux", "channel",
  "name", "pm", "ene", "energy", "brule", "srule", "rule", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,    44,    61,    62,   125,   123,    45,
      43,    42,    47,   289,    94,   167,    40,    41,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    50,    50,    50,    51,    51,    51,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    54,    54,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      56,    58,    57,    57,    59,    59,    60,    60,    60,    60,
      60,    60,    60,    60,    61,    62,    63,    64,    65,    66,
      66,    67,    67,    67,    68,    68,    69,    69,    70,    70,
      71,    71,    72,    72,    72,    72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     5,     4,     3,     3,     3,
       3,     2,     3,     3,     1,     1,     4,     3,     3,     2,
       3,     3,     3,     4,     4,     3,     1,     3,    10,     1,
       3,     0,     8,     7,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,    13,     1,
       1,     1,     1,     1,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     1,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     9,    36,    11,     0,     0,     0,     0,    25,     4,
      10,     0,     0,     5,     0,     0,     2,     7,    39,     8,
       6,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,     0,    21,     0,     1,     3,     0,     0,
       0,     0,     0,     0,     0,    37,    12,    35,     0,     0,
       0,     0,    14,    13,    32,     0,     0,    54,     0,     0,
       0,    31,     0,    30,    23,    18,    17,    19,    20,    22,
       0,     0,    16,    33,     0,    34,    26,     0,    41,     0,
       0,    27,    28,     0,     0,    15,     0,    44,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,    46,    47,    45,    51,    52,    53,    50,
      66,    49,    72,    73,    48,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,     0,
       0,    55,    56,    57,    74,    75,     0,    70,    68,    64,
      60,    59,     0,    65,    69,    71,    38,     0,     0,    40,
      61,    63,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    58
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    15,    16,   136,    18,    49,    19,   137,    20,    86,
      90,   105,    21,   106,   107,   108,   109,   142,   153,   110,
     111,   112,   113,   114
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -142
static const yytype_int16 yypact[] =
{
     210,  -142,   -10,    -4,    -9,    33,    10,    12,  -142,  -142,
    -142,   272,   307,  -142,   307,    72,  -142,    43,  -142,  -142,
    -142,  -142,    -2,   307,   106,    38,   254,     1,    56,    30,
      51,  -142,   196,   -24,    53,   137,  -142,  -142,   307,   307,
     307,   307,   307,    49,    63,  -142,    43,  -142,    82,    17,
      18,    94,  -142,   -12,  -142,    52,    54,  -142,   307,   298,
     307,  -142,   307,  -142,  -142,    13,    13,    53,    53,    53,
     106,    -2,  -142,  -142,    -2,  -142,  -142,   307,  -142,    77,
     243,    43,    43,    93,    73,    43,    81,  -142,   307,  -142,
     148,   225,   182,    84,    85,    96,   101,   104,   105,   107,
     108,   111,  -142,    43,  -142,  -142,  -142,  -142,  -142,  -142,
     -28,  -142,    78,    87,  -142,    -2,  -142,   110,   123,   130,
     131,   132,   307,   307,    -2,   -16,    -2,  -142,   307,   307,
     113,  -142,  -142,  -142,  -142,  -142,     4,  -142,  -142,  -142,
    -142,  -142,   134,  -142,  -142,  -142,  -142,   307,   -18,    43,
    -142,  -142,  -142,   135,   138,   143,   147,   152,   -16,   153,
     -16,  -142
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -142,  -142,   183,     0,  -142,   188,   -21,   -62,  -142,  -142,
     118,  -142,  -142,  -142,  -142,  -142,  -142,  -141,  -142,  -142,
    -142,  -142,  -142,  -142
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      17,    45,   126,    50,     2,    54,    43,    44,   140,   150,
      62,    32,    34,    63,    35,    17,   141,   159,    77,   161,
     127,   151,   152,    46,    48,    22,    53,    38,    39,    40,
      41,    23,    42,    55,    56,   147,    11,    24,    65,    66,
      67,    68,    69,    38,    39,    40,    41,    28,    42,    50,
      54,    62,    74,    84,    40,    41,    27,    42,    80,    53,
      81,   138,    82,    25,    73,    75,   144,   145,    26,   104,
      83,   104,    36,    51,    57,     1,    58,    85,     2,     3,
       4,     5,    38,    39,    40,    41,    59,    42,    91,     6,
     103,     7,   103,    25,   130,    70,     8,    42,    71,    78,
      76,    79,    87,   139,    10,   143,    89,    88,   128,     1,
      11,    12,     2,     3,     4,     5,    60,   129,    14,   117,
     118,    38,    39,    40,    41,     7,    42,    60,   131,    72,
       8,   119,    38,    39,    40,    41,   120,    42,    10,   121,
     122,   132,   123,   124,    11,    12,   125,   149,   133,   134,
     135,     1,    14,    47,     2,     3,     4,     5,    93,    94,
     146,    95,    96,    97,   148,   154,   155,     7,    98,    99,
     100,   101,     8,   156,   102,   157,    38,    39,    40,    41,
      10,    42,   158,   160,    64,     1,    11,    12,     2,     3,
       4,     5,    93,    94,    14,    95,    96,    97,    37,    33,
       0,     7,    98,    99,   100,   101,     8,    92,   116,     0,
       0,     0,     0,     1,    10,     0,     2,     3,     4,     5,
      11,    12,     0,     0,     0,     0,     0,     6,    14,     7,
      60,     0,     0,    61,     8,    38,    39,    40,    41,     9,
      42,     0,    10,     0,     0,     0,     0,     0,    11,    12,
       0,     0,     0,     0,     0,    13,    14,     1,    52,   115,
       2,     3,     4,     5,    38,    39,    40,    41,     0,    42,
       0,     0,     0,     7,     0,     1,     0,     0,     8,     3,
      29,    30,    38,    39,    40,    41,    10,    42,     0,     0,
      72,     7,    11,    12,     0,     0,     8,     0,     0,     0,
      14,     1,    52,     0,    10,     3,    29,    30,     0,    31,
       1,    12,     0,     0,     3,    29,    30,     7,    14,     0,
       0,     0,     8,     0,     0,     0,     7,     0,     0,     0,
      10,     8,     0,     0,     0,     0,     0,    12,     0,    10,
       0,     0,     0,     0,    14,     0,    12,     0,     0,     0,
       0,     0,     0,    14
};

static const yytype_int16 yycheck[] =
{
       0,    22,    30,    24,     6,    26,     8,     9,    24,    27,
      34,    11,    12,    37,    14,    15,    32,   158,    30,   160,
      48,    39,    40,    23,    24,    35,    26,    39,    40,    41,
      42,    35,    44,    32,    33,    31,    38,    46,    38,    39,
      40,    41,    42,    39,    40,    41,    42,    35,    44,    70,
      71,    34,    34,    74,    41,    42,    46,    44,    58,    59,
      60,   123,    62,    30,    47,    47,   128,   129,    35,    90,
      70,    92,     0,    35,    18,     3,    46,    77,     6,     7,
       8,     9,    39,    40,    41,    42,    35,    44,    88,    17,
      90,    19,    92,    30,   115,    46,    24,    44,    35,    47,
       6,    47,    25,   124,    32,   126,    25,    34,    30,     3,
      38,    39,     6,     7,     8,     9,    34,    30,    46,    35,
      35,    39,    40,    41,    42,    19,    44,    34,    18,    47,
      24,    35,    39,    40,    41,    42,    35,    44,    32,    35,
      35,    18,    35,    35,    38,    39,    35,   147,    18,    18,
      18,     3,    46,    47,     6,     7,     8,     9,    10,    11,
      47,    13,    14,    15,    30,    30,    28,    19,    20,    21,
      22,    23,    24,    30,    26,    28,    39,    40,    41,    42,
      32,    44,    30,    30,    47,     3,    38,    39,     6,     7,
       8,     9,    10,    11,    46,    13,    14,    15,    15,    11,
      -1,    19,    20,    21,    22,    23,    24,    89,    26,    -1,
      -1,    -1,    -1,     3,    32,    -1,     6,     7,     8,     9,
      38,    39,    -1,    -1,    -1,    -1,    -1,    17,    46,    19,
      34,    -1,    -1,    37,    24,    39,    40,    41,    42,    29,
      44,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    45,    46,     3,     4,    34,
       6,     7,     8,     9,    39,    40,    41,    42,    -1,    44,
      -1,    -1,    -1,    19,    -1,     3,    -1,    -1,    24,     7,
       8,     9,    39,    40,    41,    42,    32,    44,    -1,    -1,
      47,    19,    38,    39,    -1,    -1,    24,    -1,    -1,    -1,
      46,     3,     4,    -1,    32,     7,     8,     9,    -1,    37,
       3,    39,    -1,    -1,     7,     8,     9,    19,    46,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      32,    24,    -1,    -1,    -1,    -1,    -1,    39,    -1,    32,
      -1,    -1,    -1,    -1,    46,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     6,     7,     8,     9,    17,    19,    24,    29,
      32,    38,    39,    45,    46,    50,    51,    52,    53,    55,
      57,    61,    35,    35,    46,    30,    35,    46,    35,     8,
       9,    37,    52,    54,    52,    52,     0,    51,    39,    40,
      41,    42,    44,     8,     9,    55,    52,    47,    52,    54,
      55,    35,     4,    52,    55,    32,    33,    18,    46,    35,
      34,    37,    34,    37,    47,    52,    52,    52,    52,    52,
      46,    35,    47,    47,    34,    47,     6,    30,    47,    47,
      52,    52,    52,    52,    55,    52,    58,    25,    34,    25,
      59,    52,    59,    10,    11,    13,    14,    15,    20,    21,
      22,    23,    26,    52,    55,    60,    62,    63,    64,    65,
      68,    69,    70,    71,    72,    34,    26,    35,    35,    35,
      35,    35,    35,    35,    35,    35,    30,    48,    30,    30,
      55,    18,    18,    18,    18,    18,    52,    56,    56,    55,
      24,    32,    66,    55,    56,    56,    47,    31,    30,    52,
      27,    39,    40,    67,    30,    28,    30,    28,    30,    66,
      30,    66
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 1112 "glb_parser.y"
    {}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 1113 "glb_parser.y"
    {}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 1114 "glb_parser.y"
    {YYABORT;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 1115 "glb_parser.y"
    {YYABORT;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 1119 "glb_parser.y"
    {}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 1120 "glb_parser.y"
    {}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 1121 "glb_parser.y"
    {}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 1125 "glb_parser.y"
    { (yyval.val) = (yyvsp[(1) - (1)].val);                     }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 1126 "glb_parser.y"
    { (yyval.val) = (yyvsp[(1) - (1)].nameptr)->value;              }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 1127 "glb_parser.y"
    { (yyval.val) = (yyvsp[(1) - (1)].tptr)->value.var;          }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 1128 "glb_parser.y"
    { (yyval.val) = (yyvsp[(3) - (3)].val); (yyvsp[(1) - (3)].tptr)->value.var = (yyvsp[(3) - (3)].val); }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 1129 "glb_parser.y"
    {
  if(set_exp((yyvsp[(1) - (3)].name),(yyvsp[(3) - (3)].val),0)==1) yyerror("Unknown identifier: %s", (yyvsp[(1) - (3)].name));
  (yyval.val) = (yyvsp[(3) - (3)].val);
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 1134 "glb_parser.y"
    {
  if(set_fnct((yyvsp[(1) - (3)].name),(yyvsp[(3) - (3)].nameptr)->sf)==1) yyerror("Unknown identifier: %s", (yyvsp[(1) - (3)].name));
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 1138 "glb_parser.y"
    {
  if(set_pair((yyvsp[(1) - (5)].name),(yyvsp[(3) - (5)].val),(yyvsp[(5) - (5)].val),0)==1) yyerror("Unknown identifier: %s", (yyvsp[(1) - (5)].name));
  (yyval.val) = (yyvsp[(3) - (5)].val);
  if ((yyvsp[(1) - (5)].name))  { glb_free((yyvsp[(1) - (5)].name));  (yyvsp[(1) - (5)].name)=NULL; }
}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 1143 "glb_parser.y"
    {
  /* added safety in case the function pointer is NULL, which is
     sometimes useful for special functions */
  if((yyvsp[(1) - (4)].tptr)->value.fnctptr==NULL) yyerror("Improper use of special function %s", (yyvsp[(1) - (4)].tptr)->name);
  else (yyval.val) = (*((yyvsp[(1) - (4)].tptr)->value.fnctptr))((yyvsp[(3) - (4)].val)); }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 1149 "glb_parser.y"
    { (yyval.val) = (yyvsp[(1) - (3)].val) + (yyvsp[(3) - (3)].val);      }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 1150 "glb_parser.y"
    { (yyval.val) = (yyvsp[(1) - (3)].val) - (yyvsp[(3) - (3)].val);      }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 1151 "glb_parser.y"
    { (yyval.val) = (yyvsp[(1) - (3)].val) * (yyvsp[(3) - (3)].val);      }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 1152 "glb_parser.y"
    { (yyval.val) = (yyvsp[(1) - (3)].val) / (yyvsp[(3) - (3)].val);      }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 1153 "glb_parser.y"
    { (yyval.val) = -(yyvsp[(2) - (2)].val);          }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 1154 "glb_parser.y"
    { (yyval.val) = pow ((yyvsp[(1) - (3)].val), (yyvsp[(3) - (3)].val)); }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 1155 "glb_parser.y"
    { (yyval.val) = (yyvsp[(2) - (3)].val);           }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 1156 "glb_parser.y"
    { (yyval.val) = 0;            }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 1157 "glb_parser.y"
    { yyerror("Unknown name: %s", (yyvsp[(1) - (1)].name)); YYERROR; }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 1161 "glb_parser.y"
    {
  glb_List *ltemp;
  ltemp=thread_list(&glb_list_copy,0,0,(yyvsp[(4) - (4)].tptr)->list);
  if(set_exp_list((yyvsp[(1) - (4)].name),ltemp,3)==1) yyerror("Unknown identifier");
  (yyval.ptr) = ltemp;
  if ((yyvsp[(1) - (4)].name))  { glb_free((yyvsp[(1) - (4)].name));  (yyvsp[(1) - (4)].name)=NULL; }
}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 1170 "glb_parser.y"
    {
   glb_List *buf = list_cons(NULL, (yyvsp[(1) - (3)].val));
   buf = list_cons(buf, (yyvsp[(3) - (3)].val));
   (yyval.ptr)  = buf;
}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 1175 "glb_parser.y"
    { (yyval.ptr) = list_cons((yyvsp[(1) - (3)].ptr), (yyvsp[(3) - (3)].val)); }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 1179 "glb_parser.y"
    {(yyval.ptr)=NULL;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 1180 "glb_parser.y"
    {(yyval.ptr)=(yyvsp[(2) - (3)].ptr); }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 1181 "glb_parser.y"
    {(yyval.ptr)=list_cons(NULL,(yyvsp[(2) - (3)].val)); }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 1182 "glb_parser.y"
    {
  if(set_exp_list((yyvsp[(1) - (3)].name),(yyvsp[(3) - (3)].ptr),3)==1)  yyerror("Unknown identifier");
  (yyval.ptr) = (yyvsp[(3) - (3)].ptr);
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 1187 "glb_parser.y"
    {(yyval.ptr) = thread_list((yyvsp[(1) - (4)].tptr)->value.fnctptr,(yyvsp[(1) - (4)].tptr)->reverse,(yyvsp[(1) - (4)].tptr)->destroy,(yyvsp[(3) - (4)].ptr));}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 1188 "glb_parser.y"
    {(yyval.ptr) = thread_list((yyvsp[(1) - (4)].tptr)->value.fnctptr,(yyvsp[(1) - (4)].tptr)->reverse,(yyvsp[(1) - (4)].tptr)->destroy,(yyvsp[(3) - (4)].ptr));}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 1189 "glb_parser.y"
    {(yyval.ptr) = (*((yyvsp[(1) - (3)].tptr)->value.lfnctptr))();}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 1190 "glb_parser.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].tptr)->list;              }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 1191 "glb_parser.y"
    { (yyval.ptr) = (yyvsp[(3) - (3)].ptr); (yyvsp[(1) - (3)].tptr)->list = (yyvsp[(3) - (3)].ptr); }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 1192 "glb_parser.y"
    {(yyval.ptr)=glb_interpolation((yyvsp[(3) - (10)].ptr),(yyvsp[(5) - (10)].ptr),floor((yyvsp[(7) - (10)].val)),(yyvsp[(9) - (10)].ptr));}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 1193 "glb_parser.y"
    {}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 1197 "glb_parser.y"
    {
  double *buf;
  buf=(double*) glb_malloc(sizeof(double)*2);
  buf[0]=(yyvsp[(1) - (3)].val);
  buf[1]=(yyvsp[(3) - (3)].val)-1;
  (yyval.dpt)=buf;
}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 1208 "glb_parser.y"
    { if((yyvsp[(3) - (4)].nameptr)->value==-1) {(yyvsp[(3) - (4)].nameptr)->value=step_counter((yyvsp[(1) - (4)].name)); }
  loc_count=(yyvsp[(3) - (4)].nameptr)->value;
  glb_free(context);
  context =(char *) strdup((yyvsp[(1) - (4)].name));
  grp_start(context);
  if ((yyvsp[(1) - (4)].name))  { glb_free((yyvsp[(1) - (4)].name));  (yyvsp[(1) - (4)].name)=NULL; }
}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 1215 "glb_parser.y"
    {
  grp_end(context);
}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 1218 "glb_parser.y"
    {
    yyerror("Redefinition of an automatic variable %s", (yyvsp[(3) - (7)].nameptr)->name); YYERROR;
    if ((yyvsp[(1) - (7)].name))  { glb_free((yyvsp[(1) - (7)].name));  (yyvsp[(1) - (7)].name)=NULL; }
}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 1230 "glb_parser.y"
    {}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 1231 "glb_parser.y"
    {}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 1232 "glb_parser.y"
    {}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 1233 "glb_parser.y"
    {}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 1234 "glb_parser.y"
    {}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 1235 "glb_parser.y"
    {}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 1236 "glb_parser.y"
    {}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 1237 "glb_parser.y"
    {}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 1241 "glb_parser.y"
    {
//  buff.version=strdup($3);
  if (set_string((yyvsp[(1) - (3)].name), (yyvsp[(3) - (3)].name)) != 0)
    yyerror("Unknown identifier: %s", (yyvsp[(1) - (3)].name));
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  if ((yyvsp[(3) - (3)].name))  { glb_free((yyvsp[(3) - (3)].name));  (yyvsp[(3) - (3)].name)=NULL; }
}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 1251 "glb_parser.y"
    {
  //load_cross($3,loc_count-1);
  xsc.file_name=strdup((yyvsp[(3) - (3)].name));
  (yyval.name)=(yyvsp[(3) - (3)].name);
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  if ((yyvsp[(3) - (3)].name))  { glb_free((yyvsp[(3) - (3)].name));  (yyvsp[(3) - (3)].name)=NULL; }
}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 1261 "glb_parser.y"
    {
  //load_flux($3,loc_count-1,1);
  flt.file_name=strdup((yyvsp[(3) - (3)].name));

  //if(set_exp($1,$3,0)==1) yyerror("Unknown identifier");
  (yyval.name)=(yyvsp[(3) - (3)].name);
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  if ((yyvsp[(3) - (3)].name))  { glb_free((yyvsp[(3) - (3)].name));  (yyvsp[(3) - (3)].name)=NULL; }
}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 1273 "glb_parser.y"
    {
  //load_flux($3,loc_count-1,1);
  flt.file_name=strdup((yyvsp[(3) - (3)].name));

  //if(set_exp($1,$3,0)==1) yyerror("Unknown identifier");
  (yyval.name)=(yyvsp[(3) - (3)].name);
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  if ((yyvsp[(3) - (3)].name))  { glb_free((yyvsp[(3) - (3)].name));  (yyvsp[(3) - (3)].name)=NULL; }
}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 1286 "glb_parser.y"
    {

  int x[6];
  x[0]=(yyvsp[(3) - (13)].nameptr)->value - 1 ;
  x[1]=(yyvsp[(5) - (13)].in);
  x[2]=(yyvsp[(7) - (13)].in);
  x[3]=(yyvsp[(9) - (13)].in);
  x[4]=(int) (yyvsp[(11) - (13)].nameptr)->value -1;
  x[5]=(int) (yyvsp[(13) - (13)].nameptr)->value -1;

  set_channel_data(x,loc_count);
  if ((yyvsp[(1) - (13)].name))  { glb_free((yyvsp[(1) - (13)].name));  (yyvsp[(1) - (13)].name)=NULL; }
}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 1303 "glb_parser.y"
    {(yyval.nameptr)=(yyvsp[(1) - (1)].nameptr);}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 1304 "glb_parser.y"
    { yyerror("Unknown name: %s", (yyvsp[(1) - (1)].name)); YYERROR; }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 1308 "glb_parser.y"
    {(yyval.in)=(yyvsp[(1) - (1)].in);}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 1309 "glb_parser.y"
    {(yyval.in)=1;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1310 "glb_parser.y"
    {(yyval.in)=-1;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1314 "glb_parser.y"
    {
  glb_List **buf;
  energy_len=1;

  buf=(glb_List**) glb_malloc(sizeof( glb_List* ) );
  buf[0]=(yyvsp[(3) - (3)].ptr);
  (yyval.ptrq)=buf;
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 1324 "glb_parser.y"
    {
  glb_List **buf;
  buf=(yyvsp[(1) - (3)].ptrq);
  energy_len++;

  buf=(glb_List**) glb_realloc((void**) buf , sizeof( glb_List* ) * energy_len);

  buf[energy_len-1]=(yyvsp[(3) - (3)].ptr);
  (yyval.ptrq)=buf;
}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 1337 "glb_parser.y"
    { set_exp_energy("@energy",(yyvsp[(1) - (1)].ptrq)); }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 1338 "glb_parser.y"
    { set_exp_energy("@energy",(yyvsp[(1) - (2)].ptrq)); }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 1342 "glb_parser.y"
    {
  glb_List **buf;
  buf=(glb_List**) glb_malloc(sizeof(glb_List*)*2);
  buf[0]=list_cons(NULL,(yyvsp[(3) - (3)].dpt)[0]);
  buf[1]=list_cons(NULL,(yyvsp[(3) - (3)].dpt)[1]);
  glb_free((yyvsp[(3) - (3)].dpt));
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  (yyval.ptrq)=buf;
}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 1351 "glb_parser.y"
    {
  glb_List **buf;
  buf=(yyvsp[(1) - (3)].ptrq);
  buf[0]=list_cons(buf[0],(yyvsp[(3) - (3)].dpt)[0]);
  buf[1]=list_cons(buf[1],(yyvsp[(3) - (3)].dpt)[1]);
  glb_free((yyvsp[(3) - (3)].dpt));
  (yyval.ptrq)=buf;
}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 1362 "glb_parser.y"
    {
  glb_List **buf;

  buf=(glb_List**) glb_malloc(sizeof(glb_List*)*2);
  buf[0]=list_cons(NULL,(yyvsp[(3) - (3)].dpt)[0]);
  buf[1]=list_cons(NULL,(yyvsp[(3) - (3)].dpt)[1]);
  glb_free((yyvsp[(3) - (3)].dpt));
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  (yyval.ptrq)=buf;
}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 1372 "glb_parser.y"
    {
  glb_List **buf;
  buf=(yyvsp[(1) - (3)].ptrq);
  buf[0]=list_cons(buf[0],(yyvsp[(3) - (3)].dpt)[0]);
  buf[1]=list_cons(buf[1],(yyvsp[(3) - (3)].dpt)[1]);
  glb_free((yyvsp[(3) - (3)].dpt));
  (yyval.ptrq)=buf;
}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 1383 "glb_parser.y"
    {
  int flag;
  (yyval.ptrq)=(yyvsp[(1) - (1)].ptrq);
  flag=set_exp_list("bgrulescoeff",(yyvsp[(1) - (1)].ptrq)[0],0);
  if(flag==1) yyerror("Invalid coefficient in @background");
  flag=set_exp_list("bgrulechannellist",(yyvsp[(1) - (1)].ptrq)[1],0);
  if(flag==1) yyerror("Invalid channel in @background");
  glb_free((yyvsp[(1) - (1)].ptrq));
}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 1392 "glb_parser.y"
    {
  int flag;
  (yyval.ptrq)=(yyvsp[(1) - (1)].ptrq);
  flag=set_exp_list("rulescoeff",(yyvsp[(1) - (1)].ptrq)[0],0);
  if(flag==1) yyerror("Invalid coefficient in @signal");
  flag=set_exp_list("rulechannellist",(yyvsp[(1) - (1)].ptrq)[1],0);
  if(flag==1) yyerror("Invalid channel in @signal");
  glb_free((yyvsp[(1) - (1)].ptrq));
}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 1401 "glb_parser.y"
    {
  buff.sys_on_strings[buff.numofrules-1] = strdup((yyvsp[(3) - (3)].name));
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  if ((yyvsp[(3) - (3)].name))  { glb_free((yyvsp[(3) - (3)].name));  (yyvsp[(3) - (3)].name)=NULL; }
}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 1406 "glb_parser.y"
    {
  buff.sys_off_strings[buff.numofrules-1] = strdup((yyvsp[(3) - (3)].name));
  if ((yyvsp[(1) - (3)].name))  { glb_free((yyvsp[(1) - (3)].name));  (yyvsp[(1) - (3)].name)=NULL; }
  if ((yyvsp[(3) - (3)].name))  { glb_free((yyvsp[(3) - (3)].name));  (yyvsp[(3) - (3)].name)=NULL; }
}
    break;



/* Line 1455 of yacc.c  */
#line 3268 "glb_parser.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1413 "glb_parser.y"


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

