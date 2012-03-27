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


%pointer
%option noyywrap
%option nounistd


/* This here caused an unpleasant problem:
 * things like
 * %s rule
 * would cause a
 * #define rule xxx
 * statement in glb_lexer.c and that in turn causes
 * trouble with inlcude files which contain
 * glbXXX(int rule)
 * because this will be expanded by the preprocessor to
 * glbXXX(int xxx)
 * which then causes a stupid error of the form
 * parse error before numeric constant
 * The flex manual says that these things have global scope
 * but as usual I read the manual after the fact
 * PH
 */


%x glb_lexer_incl
%s glb_lexer_grp
%s glb_lexer_rule
%s glb_lexer_chl
%s glb_lexer_energy
%s glb_lexer_cross
%s glb_lexer_flux
%s glb_lexer_nuflux
%s glb_lexer_defr
%s glb_lexer_defc
%s glb_lexer_defe
%s glb_lexer_defx
%s glb_lexer_deff
%s glb_lexer_defnf
%x glb_lexer_comment
%x glb_lexer_linecomment
%x glb_lexer_str


%top{
#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif
}

%{
/* need this for the call to atof() below */
#include <math.h>
#include <string.h>

#include "glb_parser_type.h"
#include "glb_parser.h"
#include "glb_error.h"


 #define MAX_INCLUDE_DEPTH 10
static YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];
static int include_stack_ptr = 0;
int glb_line_num=1;
char *glb_file_id;
static int line_buf[MAX_INCLUDE_DEPTH];
static char *file_name_buf[MAX_INCLUDE_DEPTH];
static int cond_buf;
/* I am not sure wether this works with nested contexts */
static int cond_buf_i;
static int cond_buf_str;
static char *fnbuf,*fnbuf2;
static size_t len,i;

void glb_lexer_cleanup();

// JK, 2010-09-26
// We now allow any character except '"' inside strings to allow for
// multi-line citation information
//FN       [a-zA-Z0-9./][a-zA-Z0-9_./-]*


%}

EOE       #NEXT#
FN       [^"]*

GLOB     GLoBES
RL       @signal
DIGIT    [0-9]
IVAL     $[a-zA-Z][a-zA-Z0-9_]*
ID       [a-zA-Z][a-zA-Z0-9_]*
LID      %[a-zA-Z][a-zA-Z0-9_]*

%%

"//"         {cond_buf=YY_START; BEGIN(glb_lexer_linecomment);}
<glb_lexer_linecomment>[^\n]*     /* eat anything that's not a line break */
<glb_lexer_linecomment>\n     BEGIN(cond_buf);

"/*"         {cond_buf=YY_START;BEGIN(glb_lexer_comment);}
<glb_lexer_comment>[^*\n]*        /* eat anything that's not a '*' */
<glb_lexer_comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<glb_lexer_comment>\n             ++glb_line_num;
<glb_lexer_comment>"*"+"/"        BEGIN(cond_buf);

{EOE}   {
glb_copy_buff();
glbReset();
}


<INITIAL>{
rule    {BEGIN(glb_lexer_defr);yylval.name=strdup(yytext);return GID;}
channel {BEGIN(glb_lexer_defc);yylval.name=strdup(yytext);return GID;}
energy  {BEGIN(glb_lexer_defe);yylval.name=strdup(yytext);return GID;}
cross   {BEGIN(glb_lexer_defx);yylval.name=strdup(yytext);return GID;}
flux    {BEGIN(glb_lexer_deff);yylval.name=strdup(yytext);return GID;}
nuflux    {BEGIN(glb_lexer_defnf);yylval.name=strdup(yytext);return GID;}
}

include             {cond_buf_i=YY_START;BEGIN(glb_lexer_incl);}

<glb_lexer_incl>[ \t]*      /* eat the whitespace */
<glb_lexer_incl>[^ \t\n]+   { /* got the include file name */
  if (include_stack_ptr >= MAX_INCLUDE_DEPTH)
  {
    fprintf( stderr, "FATAL: Includes nested too deeply\n" );
    exit( 1 );
  }

// This code rewritten by JK on Mar 30, 2010
//if(include_stack_ptr==0)
//{
//file_name_buf[include_stack_ptr]=glb_file_id;
//line_buf[include_stack_ptr]=glb_line_num;
//}
//
//include_stack[include_stack_ptr++] =
//YY_CURRENT_BUFFER;
//
//fnbuf=strdup(yytext);
//len=strlen(fnbuf);
//if(fnbuf[len-1]!='\"') {fprintf(stderr,"FATAL:"
//" Unterminated filename\n");exit(1) ;}
//if(fnbuf[0]!='\"') {fprintf(stderr,"FATAL:"
//" Unquoted filename\n");exit(1) ;}
//fnbuf2=(char *) glb_malloc((len-1)*sizeof(char));
//for(i=0;i<len-1;i++) fnbuf2[i]=fnbuf[i+1];
//fnbuf2[len-2]='\0';
//yyin = glb_fopen( fnbuf2, "r" );
//
//if ( ! yyin ){
//fprintf(stderr,"FATAL: Could not open file %s\n",fnbuf2);exit(1);}
//
//file_name_buf[include_stack_ptr]=(char*) strdup(fnbuf2);
//glb_file_id=file_name_buf[include_stack_ptr];
//line_buf[include_stack_ptr]=glb_line_num;
//glb_line_num=0;
//glb_free(fnbuf);
//glb_free(fnbuf2);

  /* Remember status of the including file */
  include_stack[include_stack_ptr] = YY_CURRENT_BUFFER;
  file_name_buf[include_stack_ptr] = glb_file_id;
  line_buf[include_stack_ptr]      = glb_line_num;

  /* Remove quotes from filename */
  fnbuf = strdup(yytext);
  len   = strlen(fnbuf);
  if (fnbuf[len-1] != '\"') { fprintf(stderr,"FATAL: Unterminated filename: %s\n", fnbuf); exit(1); }
  if (fnbuf[0] != '\"')     { fprintf(stderr,"FATAL: Unquoted filename: %s\n", fnbuf);     exit(1); }
  glb_file_id = (char *) glb_malloc((len-1)*sizeof(char));
  strncpy(glb_file_id, fnbuf+1, len-2);
  glb_file_id[len-2] = '\0';
  glb_free(fnbuf);

  /* Open included file and re-initialize status variables */
  yyin = glb_fopen(glb_file_id, "r");
  if (!yyin )  { fprintf(stderr,"FATAL: Could not open file %s\n",glb_file_id); exit(1); }
  glb_line_num = 0;
  include_stack_ptr++;
  yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
  BEGIN(cond_buf_i);
}



<<EOF>> {
  if (--include_stack_ptr < 0)  /* If top level buffer ends, we're done */
  {
    glb_copy_buff();
    glbReset();
    /* BUGFIX #9 -- This lines fixes a tricky bug in the parser */
    include_stack_ptr = 0;
    yyterminate();
  }
  else                          /* Otherwise, switch back to previous buffer */
  {
    glb_fclose(yyin);
    yy_delete_buffer(YY_CURRENT_BUFFER);
    yy_switch_to_buffer(include_stack[include_stack_ptr]);
    glb_line_num = line_buf[include_stack_ptr];
    glb_free(glb_file_id);
    glb_file_id  = file_name_buf[include_stack_ptr];
  }
}


<glb_lexer_rule,glb_lexer_chl,glb_lexer_energy,glb_lexer_cross,glb_lexer_flux,glb_lexer_nuflux>[<]  {
yylval.in=-1;
return GRPOPEN;}

<glb_lexer_chl>@channel          {
yylval.name=strdup(yytext);

return CHANNEL;}

<glb_lexer_chl>"+" {yylval.in=+1;return PM;}
<glb_lexer_chl>"-" {yylval.in=-1;return PM;}

[eE][lectronLECTRON]* {yylval.in=1;return FLAVOR;}
[mM][uonUON]*  {yylval.in=2;return FLAVOR;}
[tT][auAU]* {yylval.in=3;return FLAVOR;}

NOSC_[eE][lectronLECTRON]* {yylval.in=11;return FLAVOR;}
NOSC_[mM][uonUON]*  {yylval.in=12;return FLAVOR;}
NOSC_[tT][auAU]* {yylval.in=13;return FLAVOR;}


<glb_lexer_energy>@energy         {
yylval.name=strdup(yytext);

return ENERGY;}

<glb_lexer_rule>@signal           {
yylval.name=strdup(yytext);

return SIGNAL;}

<glb_lexer_rule>@background          {
yylval.name=strdup(yytext);

return BG;}

<glb_lexer_rule>@sys_on_function   {
yylval.name=strdup(yytext);
return SYS_ON_FUNCTION;
}

<glb_lexer_rule>@sys_off_function   {
yylval.name=strdup(yytext);
return SYS_OFF_FUNCTION;
}



\" {cond_buf_str=YY_START;BEGIN(glb_lexer_str);}

<glb_lexer_str>\" {BEGIN(cond_buf_str);}

 /* JK 2010-09-26
    <glb_lexer_str>\n {glb_fatal("Lexer error: Unterminated string");exit(1);}
 */;

<glb_lexer_str>{FN} {
yylval.name=strdup(yytext);
return FNAME;}

$version {
yylval.name=strdup(yytext);
return VERS;
}

$citation {
yylval.name=strdup(yytext);
return VERS;
}

<glb_lexer_cross>@cross_file           {
yylval.name=strdup(yytext);
return CROSS;}

<glb_lexer_flux>@flux_file          {
yylval.name=strdup(yytext);
return FLUXP;}


<glb_lexer_flux>@flux_file_m           {
yylval.name=strdup(yytext);
return FLUXM;}




<glb_lexer_nuflux>@flux_file          {
yylval.name=strdup(yytext);
return NUFLUX;}


<glb_lexer_nuflux>@flux_file_m           {
yylval.name=strdup(yytext);
return NUFLUX;}


:        {return RULESEP;}
@        {return RULEMULT;}



<glb_lexer_defr>#{ID}        {glb_namerec *s;
s = glb_getname (yytext,"rule");
if (s == 0)
s = glb_putname (yytext,"rule",NAME);
BEGIN(INITIAL);
BEGIN(glb_lexer_rule);
if(strcmp(s->context,"rule")!=0)
{yylval.nameptr=s;return RDF;}

yylval.nameptr = s;

return s->type;
}

<glb_lexer_defc>#{ID}        {glb_namerec *s;
s = glb_getname (yytext,"channel");
if (s == 0)
s = glb_putname (yytext,"channel",NAME);
BEGIN(INITIAL);
BEGIN(glb_lexer_chl);
if(strcmp(s->context,"channel")!=0)
{yylval.nameptr=s;return RDF;}

yylval.nameptr = s;

return s->type;
}

<glb_lexer_defe>#{ID}        {glb_namerec *s;
s = glb_getname (yytext,"energy");
if (s == 0)
s = glb_putname (yytext,"energy",NAME);
BEGIN(INITIAL);
BEGIN(glb_lexer_energy);
if(strcmp(s->context,"energy")!=0)
{yylval.nameptr=s;return RDF;}

yylval.nameptr = s;

return s->type;
}

<glb_lexer_defx>#{ID}        {glb_namerec *s;
s = glb_getname (yytext,"cross");
if (s == 0)
s = glb_putname (yytext,"cross",NAME);
BEGIN(INITIAL);
BEGIN(glb_lexer_cross);
if(strcmp(s->context,"cross")!=0)
{yylval.nameptr=s;return RDF;}

yylval.nameptr = s;

return s->type;
}


<glb_lexer_deff>#{ID}        {glb_namerec *s;
s = glb_getname (yytext,"flux");
if (s == 0)
s = glb_putname (yytext,"flux",NAME);
BEGIN(INITIAL);
BEGIN(glb_lexer_flux);
if(strcmp(s->context,"flux")!=0)
{yylval.nameptr=s;return RDF;}

yylval.nameptr = s;

return s->type;
}

<glb_lexer_defnf>#{ID}        {glb_namerec *s;
s = glb_getname (yytext,"nuflux");
if (s == 0)
s = glb_putname (yytext,"nuflux",NAME);
BEGIN(INITIAL);
BEGIN(glb_lexer_nuflux);
if(strcmp(s->context,"nuflux")!=0)
{yylval.nameptr=s;return RDF;}

yylval.nameptr = s;

return s->type;
}

#{ID}        {glb_namerec *s;
s = glb_getname (yytext,"rule");
if (s == 0) { yylval.name=strdup(yytext); return NDEF; }
yylval.nameptr = s;
return s->type;
}

<glb_lexer_rule,glb_lexer_chl,glb_lexer_energy,glb_lexer_cross,glb_lexer_flux,glb_lexer_nuflux>[>]        {
BEGIN(INITIAL);
yylval.in=-1;
return GRPCLOSE;}


<glb_lexer_chl,glb_lexer_rule,glb_lexer_energy,glb_lexer_cross,glb_lexer_flux,glb_lexer_nuflux>@{ID} {
yylval.name=strdup(yytext);
return IDN;
}




{DIGIT}+    {yylval.val=atof(yytext);
return NUM;
}

{IVAL}  {yylval.name=strdup(yytext);return IDN;}

{DIGIT}+"."{DIGIT}*        {
yylval.val=atof(yytext);
return NUM;
}

 "."{DIGIT}+       {
 yylval.val=atof(yytext);
 return NUM;
}

{DIGIT}+"."{DIGIT}*[eE][-+]?{DIGIT}+     {
yylval.val=atof(yytext);
return NUM;
}

"."{DIGIT}+[eE][-+]?{DIGIT}+     {
yylval.val=atof(yytext);
return NUM;
}

{DIGIT}+[eE][-+]?{DIGIT}+     {
yylval.val=atof(yytext);
return NUM;
}




{ID}        {glb_symrec *s;
s = glb_getsym (yytext);
if (s == 0)
s = glb_putsym (yytext, VAR);
yylval.tptr = s;
return s->type;
}


{LID}        {glb_symrec *s;
s = glb_getsym (yytext);
if (s == 0)
s = glb_putsym (yytext, LVAR);
yylval.tptr = s;
return s->type;
}





[ \t]*          /* eat up whitespace */

"%!GLoBES" /* empty */

.           return (int) yytext[0];


\n         /* yylloc.first_line++;*/ glb_line_num++; /*return '\n';*/
%%


/* OLD CODE, DOES NOT RUN WITH NEWER FLEX VERSIONS ANYMORE [WW, JK]:

void glb_lexer_cleanup() {
     if( yy_current_buffer != NULL )
     {
         yy_delete_buffer( yy_current_buffer );
         yy_current_buffer = 0;
     }
}
*/



void glb_lexer_cleanup() {
      if( YY_CURRENT_BUFFER != NULL )
          yy_delete_buffer( YY_CURRENT_BUFFER );
}


