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


#ifndef GLB_TOOLS_PARSER_H
#define GLB_TOOLS_PARSER_H 1

void parse_definition(const char *in);
int vector_read_double(const char *str, double *result, 
		       size_t n, void (*parse_error)(void));
int vector_read_int(const char *str, int *result, 
		    size_t n,void (*parse_error)(void));
void parse_error();
void my_print_params(FILE *fp,const glb_params iv);

char *get_output_filename(const char *basename);

#endif /* GLB_TOOLS_PARSER_H */
