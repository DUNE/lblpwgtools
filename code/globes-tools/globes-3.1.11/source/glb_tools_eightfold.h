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


#ifndef GLB_TOOLS_EIGHTFOLD_H
#define GLB_TOOLS_EIGHTFOLD_H 1

#define YES 1
#define NO 0
#define FORCE 2

#define FORWARD 1
#define FWDTRACK 2
#define BACKWARD -1
#define BACKTRACK -2

typedef struct result_type *result;

result alloc_result();
void free_result(result stale);
result set_forward(result in,result p);
result set_backward(result in,result p);
result get_forward(result in);
result get_backward(result in);
result set_minimum(result in, glb_params p);
glb_params get_minimum(result in, glb_params p);
result set_diagnosis(result in,int p);
int get_diagnosis(result in);
result set_chi(result in,double p);
double get_chi(result in);
result set_chi_sys(result in,double p);
double get_chi_sys(result in);
result set_chi_no_sys(result in,double p);
double get_chi_no_sys(result in);
result set_xval(result in,double p);
double get_xval(result in);
result rewind_chain(result in);
result setup_chain(double Xrange[2],double Resolution);
void free_chain(result stale);
void print_result(FILE *fp,result in);
result find_nearest_neighbor(result in, double xval);
result find_jump(result in, int fwd);
result find_split(result in);

int eightfold(const glb_params true_values,
	      const glb_params input_errors,
	      int hier,
	      int oct,
	      double xrange[2],
	      double yrange[2],
	      int resol[2],
	      glb_params **r_list,
	      double **r_chi);

void free_list(glb_params *stale, size_t n);


typedef double (*conversion)(double);

typedef struct {
  int which;
  conversion f;
  conversion inverse_f;
  glb_projection direction;
} projection;

typedef struct {
  projection X;
  projection Y;
} plane;


result track_in_chain(result in, const glb_params initial_values, const glb_params chi_sys, int fwd,
		      projection *p, FILE *fp);
result backpropagate_in_chain(result in, const glb_params chi_sys, int fwd, projection *p, FILE *fp);
FILE* goto_end(const char* filename,int flag);
int read_position(const char *filename, double *posx, double *posy,int flag);
double identity(double x);
double log10IIrad(double x);
double radIIlog10(double x);
double degIIrad(double x);
double radIIdeg(double x);
int setup_axis(projection *axis, int which);
void free_axis(projection *stale);

int myprior_init();
void myprior_clean();
double myprior_prior(const glb_params in,void *user_data);
int myprior_starting_values(const glb_params in, void *user_data);
int myprior_input_errors(const glb_params in, void *user_data);

#endif /* GLB_TOOLS_EIGHTFOLD_H */
