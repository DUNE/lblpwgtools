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
#ifndef GLB_PARSER_DATA_H
#define GLB_PARSER_DATA_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include "glb_types.h"

typedef struct glb_list {
  struct glb_list *next;    /* chain forward pointer*/
  double entry;       /* incase you want to use raw glb_Lists */
} glb_List;


/* Functions type.                                   */
typedef double (*func_t) (double);

/* Functions type.                                   */
typedef glb_List *(*lfunc_t) (void);

/* Data type for links in the chain of symbols.      */
struct glb_symrec
{
  char *name;  /* name of symbol                     */
  int type;    /* type of symbol: either VAR or FNCT */
  union
  {
    double var;                  /* value of a VAR   */
    func_t fnctptr;              /* value of a FNCT  */
    /* this field is added for the case of threadable function */
    lfunc_t lfnctptr; 
  } value;
  glb_List *list;
  int destroy;
  int reverse;
  struct glb_symrec *next;    /* link field              */
};

typedef struct glb_symrec glb_symrec;




/* Managing the names for rules etc...               */
/* Data type for links in the chain of symbols.      */
struct glb_namerec
{
  char *name; /* name of symbol      */   
  char *context;  /* context rule, channel etc.           */
  int type;   /* type of symbol: either VAR or FNCT */
  int value;  /* e.g. rule number */
  sigfun sf;
  struct glb_namerec *next;    /* link field              */
};

typedef struct glb_namerec glb_namerec;




glb_symrec *glb_getsym (const char *sym_name);
glb_symrec *glb_putsym (char *sym_name, int sym_type);


glb_namerec *glb_getname (const char *sym_name, char* context);
glb_namerec *glb_putname (char *sym_name, char *context, int sym_type);
     
/* stuff needed for doing multiple experiments in one file */

/* they are defined gls_parser.y */
void glb_copy_buff();
void glbReset(); 


#endif /* GLB_PARSER_DATA_H */
