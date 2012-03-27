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


/* Units and Normalization */

/* Energies GeV */
/* Length km */
/* dm^2 eV^2 */
/* densitie g/cm^3 */
/* X-section 10^-38 cm^2 */
/* Targetmass kiloton (10^6 kg) */
/* Fluxes are normalized to a baseline of 1km */
/* stored_muons is for NuFact only */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "glb_types.h"
#include "glb_error.h"
#include "glb_fluxes.h"
#include "glb_multiex.h"

#define PI 3.1415
#define NORM 6.02204*1E-12 //10^-38/u (atomic mass unit)
#define GLB_ELECTRON_MASS 0.511E-3

static double mmu = 0.1056;           // muon mass in GeV
static double beam_pol = 0;           // beam polaristaion
static double beam_en = 20;           // beam energy in GeV
static double beta,rgamma;             // relativistic quantities
static double stored_muons = 2.0E20;  // stored muons per year
// or protons on target
// care must be taken with this, it is a different number for each experiment
static double power;
static int    channel = 1;            // channel
static int FLUX_STEPS = 500;          // Number of sampling points for builtin fluxes



/***************************************************************************
 *                    H E L P E R   F U N C T I O N S                      *
 ***************************************************************************/

/***************************************************************************
 * Function glb_load_n_columns                                             *
 ***************************************************************************
 * Load data from file. The expected format is n_columns columns and an    *
 * arbitray number of rows. The resulting number of rows will be stored in *
 * n_lines, while for the actual data, arrays will be created (one for     *
 * each column), i.e. data[] should have length n_columns                  *
 ***************************************************************************/
int glb_load_n_columns(const char *file_name, const int n_columns, int *n_lines, double *data[])
{
  if (!file_name || !n_lines || !data)
    return GLBERR_UNINITIALIZED;

  int j;
  int nl;  // Counter for "real" lines, including blank lines and comments
  int buf_length = 100;

  /* Allocate buffer */
  for (j=0; j < n_columns; j++)
    data[j] = glb_malloc(buf_length*sizeof(data[j][0]));

  FILE *f = glb_fopen(file_name, "r");
  if (!f)
  {
    glb_error("glb_load_n_columns: Cannot open file %s", file_name);
    return GLBERR_FILE_NOT_FOUND;
  }

  /* Read from file */
  const int max_line = 1024;
  char this_line[max_line];
  *n_lines = nl = 0;
  while (fgets(this_line, max_line, f))
  {
    nl++;
    if (strlen(this_line) > max_line - 2)
    {
      glb_error("glb_load_n_columns: Line %d too long in file %s",
                nl, file_name);
      return GLBERR_INVALID_FILE_FORMAT;
    }

    if (this_line[0] != '#'  &&  this_line[strspn(this_line, " \t")] != '\n')
    {                               /* Ignore comments and blank lines */
      if (*n_lines >= buf_length)   /* If necessary, resize x-sec buffer */
      {
        buf_length *= 2;
        for (j=0; j < n_columns; j++)
          data[j] = glb_realloc(data[j], buf_length*sizeof(data[j][0]));
      }

      char *p = strtok(this_line, " \t,");
      for (j=0; j < n_columns; j++)
      {
        if (!p)
        {
          glb_error("glb_load_n_columns: Line %d too short in file %s",
                    nl, file_name);
          return GLBERR_INVALID_FILE_FORMAT;
        }
        if (sscanf(p, "%lf", &data[j][*n_lines]) != 1)
        {
          glb_error("glb_load_n_columns: Line %d invalid in file %s",
                    nl, file_name);
          return GLBERR_INVALID_FILE_FORMAT;
        }
        p = strtok(NULL, " \t,");
      }

//      if (*n_lines > 1  &&   /* Check if log10(E) support points are equidistant */
//          fabs(data[0][*n_lines] - 2*data[0][*n_lines-1]
//               + data[0][*n_lines-2])/fabs(data[0][*n_lines]) > 1e-10)
//      {
//        glb_error("glb_load_n_columns: Error in %s, line %d: log10(E) sampling points "
//                  "should be equidistant", file_name, nl);
//        return GLBERR_INVALID_FILE_FORMAT;
//      }
      (*n_lines)++;
    }
  };

  fclose(f);
  return GLB_SUCCESS;
}



/***************************************************************************
 *                            F L U X E S                                  *
 ***************************************************************************/

//this part is by Martin and for the Nufact only

void glb_set_max_energy(double en)
{
  beam_en = en;
  rgamma = en/mmu;
  beta = -sqrt(1-1/rgamma/rgamma);
}

double glb_get_max_energy()
{
  return beam_en;
}

static void set_beam_polarisation(double pol)
{
  beam_pol = pol;
}

static double check_beam_polarisation()
{
  return beam_pol;
}

static void set_stored_muons(double n)
{
  stored_muons = n;
}

static double check_stored_muons()
{
  return stored_muons;
}


static void set_power(double n)
{
  power = n;
}

static double check_power()
{
  return power;
}


static void set_channel(double ch)
{
  channel = ch;
}

static double flux_mu(double en, double baseline, double cost_lab)
{
  double x_lab, cost, x, xmax;
  baseline *=1000;

  x_lab = 2*en/mmu;
  cost = (beta+cost_lab)/(1+beta*cost_lab);
  x = x_lab/(rgamma*(1-beta*cost_lab));
  xmax = rgamma*(1-beta*cost_lab);

  if (x_lab < xmax)
    return rgamma*(1 - beta*cost) /baseline/baseline/mmu * stored_muons * x*x/PI * ((3-2*x)-channel*(1-2*x)*beam_pol*cost);
  else
    return 0.0;
}

static double flux_e(double en, double baseline, double cost_lab)
{
  double x_lab, cost, x, xmax;
  baseline *=1000;

  x_lab = 2*en/mmu;
  cost = (beta+cost_lab)/(1+beta*cost_lab);
  x = x_lab/(rgamma*(1-beta*cost_lab));
  xmax = rgamma*(1-beta*cost_lab);

  if (x_lab< xmax)
    return rgamma*(1 - beta*cost) /baseline/baseline/mmu * stored_muons* 6*x*x/PI * ((1-x)-channel*(1-x)*beam_pol*cost);
  else
    return 0.0;
}

static double fastflux_mu(double en, double baseline)
{
  double x_lab, x, xmax;
  baseline *=1000;

  x_lab = 2*en/mmu;
  x = x_lab/(rgamma*(1-beta));
  xmax = rgamma*(1-beta);

  if (x_lab < xmax)
    return rgamma*(1 - beta) /baseline/baseline/mmu * x*x/PI * (3-2*x);
  else
    return 0.0;
}

static double fastflux_e(double en, double baseline)
{
  double x_lab, x, xmax;
  baseline *=1000;

  x_lab = 2*en/mmu;
  x = x_lab/(rgamma*(1-beta));
  xmax = rgamma*(1-beta);

  if (x_lab < xmax)
    return rgamma*(1 - beta) /baseline/baseline/mmu *  6*x*x/PI * (1-x);
  else
    return 0.0;
}
//here ends the NuFact part


/* This type used for all builtin fluxes
 * The need to take the energy, the polarity (ie. neutrinos/anti-neutrinos
 * and they have to take the flavour argument. The need to return at least
 * zero for any valid input.
 */
typedef double (*flux_calc)(glb_flux *data, double en, int polarity, int l, int anti);

static double nufact_flux(glb_flux *data, double en, int polarity, int l, int anti)
{
  double ergebnis;
  double baseline=1.0;
  ergebnis=0.0;
  if (l==1 && anti == 1 && polarity == 1)
    {
      ergebnis=fastflux_e(en,baseline);
    }
  if (l==2 && anti == -1 && polarity == 1)
    {
      ergebnis=fastflux_mu(en,baseline);
    }
  if (l==1 && anti == -1 && polarity == -1)
    {
      ergebnis=fastflux_e(en,baseline);
    }
  if (l==2 && anti == 1 && polarity == -1)
    {
      ergebnis=fastflux_mu(en,baseline);
    }


  return ergebnis;
}

static double bb_spectrum(double gamma, double end_point, double enu)
{
  double part;
  double result,root;
  double gye,ye;
  double baseline=1000.;
  ye=GLB_ELECTRON_MASS/(end_point+GLB_ELECTRON_MASS);
  gye=(1./60)*(sqrt(1-pow(ye,2))*(2-9*pow(ye,2)-8*pow(ye,4))+15*pow(ye,4)*log10(ye/(1-sqrt(1-pow(ye,2)))));
  part=enu/(2*gamma*(end_point+GLB_ELECTRON_MASS));
  if(part>=1) return 0.0;
  /*  sign is now correct */
  root=(1-part)*(1-part) - ye*ye;
  if(root<=0) return 0.0;
  result=(1./PI/gye/pow(baseline,2))*(gamma*gamma/enu)*pow(part,3)*(1-part)*sqrt(root);
  return result;
}

static double bb_flux(glb_flux *data, double en, int polarity, int l, int anti)
{
  double ergebnis;
  ergebnis=0.0;
  if (l==1 && anti == 1 && polarity == 1)
    {
      ergebnis=bb_spectrum(data->gamma,data->end_point,en);
    }
  if (l==1 && anti == -1 && polarity == -1)
    {
      ergebnis=bb_spectrum(data->gamma,data->end_point,en);
    }


  return ergebnis;
}

static void builtin_flux_setup(glb_flux *data, flux_calc flx, int pl)
{
  double smb,pb,de,emax;
  int i, j;

  if (data->builtin == 1  ||  data->builtin == 2)
    de=data->parent_energy/(FLUX_STEPS+1);
  else if (data->builtin == 3  ||  data->builtin == 4)
    de=2 * data->gamma * (data->end_point + GLB_ELECTRON_MASS)/(FLUX_STEPS+1);
  smb=stored_muons;
  pb=power;
  stored_muons=data->stored_muons;
  power=data->target_power;
  glb_set_max_energy(data->parent_energy);
  for (j=0; j < GLB_FLUX_COLUMNS; j++)
    data->flux_data[j] = glb_malloc((FLUX_STEPS+1) * sizeof(data->flux_data[j][0]));
  data->n_lines = FLUX_STEPS + 1;
  for(i=0; i <= FLUX_STEPS; i++)
  {
    data->flux_data[0][i] = i*de;
    data->flux_data[1][i] = flx(data,i*de,pl,1,+1);
    data->flux_data[2][i] = flx(data,i*de,pl,2,+1);
    data->flux_data[3][i] = flx(data,i*de,pl,3,+1);
    data->flux_data[4][i] = flx(data,i*de,pl,1,-1);
    data->flux_data[5][i] = flx(data,i*de,pl,2,-1);
    data->flux_data[6][i] = flx(data,i*de,pl,3,-1);
  }
  power=pb;
  stored_muons=smb;
}


// Here we settle the normalization issue for all beam types
static double norm2(int type)
{
  double erg;
  switch (type)
    {
    case 1: erg=NORM*100; //NuFact
      break;
    case 2: erg=NORM*100; //NuFact
      break;
    case 3: erg=NORM*100;//NUMI Beam (9@712)
      break;
    case 4: erg=NORM*100; //NUMI Beam (9@712)
      break;
    case GLB_OLD_NORM: erg=NORM*295*295*9.92033*1E6;
      /* here we try to eliminate the stupide 5.2 factor
       *    default: erg=NORM*295*295*9.92033*1E6; */
      break;
    default: erg=1.0 ;

      break;
    }
  return erg;
}


/***************************************************************************
 * Function glb_init_flux                                                  *
 ***************************************************************************
 * Initializes a glb_flux structure after it has been partially filled by  *
 * the parser with the information from the glb file. If a built-in flux   *
 * definition is to be used, the fluxes are computed, otherwise they are   *
 * laoded from a file                                                      *
 ***************************************************************************/
int glb_init_flux(glb_flux *flux)
{
  if (!flux)
    return GLBERR_UNINITIALIZED;

  if (flux->time < 0)          flux->time         = 1.0;
  if (flux->target_power < 0)  flux->target_power = 1.0;
  if (flux->stored_muons < 0)  flux->stored_muons = 1.0;
  if (flux->norm < 0)          flux->norm         = 1.0;

  /* Initialize flux tables according to user specification */
  switch (flux->builtin)
  {
    case 1:                       /* Neutrino factory, \mu^+ */
    case 2:                       /* Neutrino factory, \mu^- */
      if (flux->parent_energy < 0)
      {
        glb_error("No parent muon energy defined for NuFact flux");
        return GLBERR_INVALID_FILE_FORMAT;
      }
      if (flux->builtin == 1)
        builtin_flux_setup(flux, &nufact_flux, +1);
      else
        builtin_flux_setup(flux, &nufact_flux, -1);
      break;

    case 3:                       /* Beta beam, beta^+ decay */
    case 4:                       /* Beta beam, beta^- decay */
      if (flux->gamma < 0)
      {
        glb_error("No gamma factor specified for Beta Beam");
        return GLBERR_INVALID_FILE_FORMAT;
      }
      if (flux->end_point < 0)
      {
        glb_error("No endpoint energy specified for Beta Beam");
        return GLBERR_INVALID_FILE_FORMAT;
      }

      if (flux->builtin == 3)
        builtin_flux_setup(flux, &bb_flux, +1);
      else
        builtin_flux_setup(flux, &bb_flux, -1);
      break;

    default:
      if (flux->file_name == NULL || strlen(flux->file_name) <= 0)
      {
        glb_error("No flux file specified");
        return GLBERR_INVALID_FILE_FORMAT;
      }
      return glb_load_n_columns(flux->file_name, GLB_FLUX_COLUMNS,
                                &flux->n_lines, flux->flux_data);
      break;
  }

  return GLB_SUCCESS;
}


/***************************************************************************
 * Function glb_get_flux                                                   *
 ***************************************************************************
 * Return the flux of neutrinos of flavor f and energy E at the baseline L *
 ***************************************************************************/
double glb_get_flux(double E, double L, int f, int cp_sign, const glb_flux *flux)
{
// JK 2010-11-27 We now allow non-equidistant support points 
//  int col = (int) f + 3*(1-cp_sign)/2;  /* Which column of the flux table? */
//  int n_steps = flux->n_lines - 1;
//  double E_binsize = (flux->flux_data[0][n_steps] - flux->flux_data[0][0]) / n_steps;
//  double result;
//
//  int k = floor((E - flux->flux_data[0][0]) / E_binsize);
//  if (k < 0 || k >= n_steps)
//    return 0.0;
//  else
//  {
//    double E_lo    = flux->flux_data[0][k];
//    double flux_lo = flux->flux_data[col][k];
//    double flux_up = flux->flux_data[col][k+1];
//    result  = flux_lo + (E - E_lo)*(flux_up - flux_lo)/E_binsize;
//    result *= norm2(flux->builtin) * flux->time * flux->target_power
//                * flux->stored_muons * flux->norm / (L*L);
//  }
//  return result;

  int col = (int) f + 3*(1-cp_sign)/2;  /* Which column of the flux table? */
  int n_steps = flux->n_lines - 1;
  double result;

  int k=0;
  while (k <= n_steps  &&  flux->flux_data[0][k] < E)
    k++;
  if (k <= 0 || k > n_steps)
    return 0.0;
  else
  {
    double E_lo    = flux->flux_data[0][k-1];
    double E_up    = flux->flux_data[0][k];
    double flux_lo = flux->flux_data[col][k-1];
    double flux_up = flux->flux_data[col][k];
    result  = flux_lo + (E - E_lo)*(flux_up - flux_lo)/(E_up - E_lo);
    result *= norm2(flux->builtin) * flux->time * flux->target_power
                * flux->stored_muons * flux->norm / (L*L);
  }
  return result;
}


/***************************************************************************
 * Function glb_free_flux                                                  *
 ***************************************************************************
 * Free flux data structue                                                 *
 ***************************************************************************/
int glb_free_flux(glb_flux *flux)
{
  int status;
  status = glb_reset_flux(flux);
  glb_free(flux);
  return status;
}


/***************************************************************************
 * Function glb_reset_flux                                                 *
 ***************************************************************************
 * Initialize flux data structrue to dummy valus. Any previously allocated *
 * memory for flux->file_name or flux->flux_data is freed.                 *
 ***************************************************************************/
int glb_reset_flux(glb_flux *flux)
{
  int i;
  if (flux)
  {
    flux->builtin       = -1;
    flux->time          = -1;
    flux->target_power  = -1;
    flux->stored_muons  = -1;
    flux->parent_energy = -1;
    flux->gamma         = -1;
    flux->end_point     = -1;
    flux->norm          = -1;
    flux->n_lines       = 0;
    for (i=0; i < GLB_FLUX_COLUMNS; i++)
    {
      if (flux->flux_data[i])
      {
        glb_free(flux->flux_data[i]);
        flux->flux_data[i] = NULL;
      }
    }
    if (flux->file_name)
    {
      glb_free(flux->file_name);
      flux->file_name = NULL;
    }
  }

  return GLB_SUCCESS;
}


/***************************************************************************
 * Function glb_copy_flux                                                  *
 ***************************************************************************
 * Create a new flux data structure as a copy of an existing one           *
 ***************************************************************************/
int glb_copy_flux(glb_flux *dest, const glb_flux *src)
{
  int i, j;

  if (!dest)
    return GLBERR_INVALID_ARGS;

  memcpy(dest, src, sizeof(*src));
  if (src->file_name)
  {
    dest->file_name = strdup(src->file_name);
    if (!dest->file_name)
      glb_fatal("glb_copy_flux: Cannot copy name of cross section file");
  }
  for (j=0; j < GLB_FLUX_COLUMNS; j++)
  {
    if (src->flux_data[j])
    {
      dest->flux_data[j] = glb_malloc(src->n_lines*sizeof(src->flux_data[j][0]));
      for (i=0; i < dest->n_lines; i++)
        dest->flux_data[j][i] = src->flux_data[j][i];
    }
  }

  return GLB_SUCCESS;
}


/***************************************************************************
 *                     C R O S S   S E C T I O N S                         *
 ***************************************************************************/

/***************************************************************************
 * Function glb_init_xsec                                                  *
 ***************************************************************************
 * Load cross sections from the file specified in xs->file_name and        *
 * store them in xs->xsec_data                                             *
 ***************************************************************************/
int glb_init_xsec(glb_xsec *xs)
{
  if (!xs || !xs->file_name)
    return GLBERR_UNINITIALIZED;

  return glb_load_n_columns(xs->file_name, GLB_XSEC_COLUMNS, &xs->n_lines, xs->xsec_data);
}


/***************************************************************************
 * Function glb_get_xsec                                                   *
 ***************************************************************************
 * Returns the cross section for neutrinos of flavor f from the cross      *
 * section data set in xs                                                  *
 ***************************************************************************/
double glb_get_xsec(double E, int f, int cp_sign, const glb_xsec *xs)
{
// JK 2010-11-27 We now allow non-equidistant support points 
//  double logE = log10(E);
//  int col = (int) f + 3*(1-cp_sign)/2;  /* Which column of the xsec table? */
//  int n_steps = xs->n_lines - 1;
//  double logE_binsize = (xs->xsec_data[0][n_steps] - xs->xsec_data[0][0]) / n_steps;
//  
//  int k = floor((logE - xs->xsec_data[0][0]) / logE_binsize);
//  if (k < 0 || k >= n_steps)
//    return 0.0;
//  else
//  {
//    double logE_lo = xs->xsec_data[0][k];
//    double xs_lo   = xs->xsec_data[col][k];
//    double xs_up   = xs->xsec_data[col][k+1];
//    return E * ( xs_lo + (logE - logE_lo)*(xs_up - xs_lo)/logE_binsize );
//  }

  double logE = log10(E);
  int col = (int) f + 3*(1-cp_sign)/2;  /* Which column of the xsec table? */
  int n_steps = xs->n_lines - 1;
  
  int k=0;
  while (k <= n_steps  &&  xs->xsec_data[0][k] < logE)
    k++;
  if (k <= 0 || k > n_steps)
    return 0.0;
  else
  {
    double logE_lo = xs->xsec_data[0][k-1];
    double logE_up = xs->xsec_data[0][k];
    double xs_lo   = xs->xsec_data[col][k-1];
    double xs_up   = xs->xsec_data[col][k];
    return E * ( xs_lo + (logE - logE_lo)*(xs_up - xs_lo)/(logE_up - logE_lo) );
  }
}


/***************************************************************************
 * Function glb_free_xsec                                                  *
 ***************************************************************************
 * Free cross section data structures                                      *
 ***************************************************************************/
int glb_free_xsec(glb_xsec *xs)
{
  int status;
  status = glb_reset_xsec(xs);
  glb_free(xs);
  return status;
}


/***************************************************************************
 * Function glb_reset_xsec                                                 *
 ***************************************************************************
 * Initialize cross section structrue to dummy valus. Any previously       *
 * allocated memory for xs->file_name or xs->xsec_data is freed.           *
 ***************************************************************************/
int glb_reset_xsec(glb_xsec *xs)
{
  int i;
  if (xs)
  {
    xs->n_lines   = 0;
    xs->builtin   = -1;
    for (i=0; i < GLB_XSEC_COLUMNS; i++)
    {
      if (xs->xsec_data[i])
      {
        glb_free(xs->xsec_data[i]);
        xs->xsec_data[i] = NULL;
      }
    }
    if (xs->file_name)
    {
      glb_free(xs->file_name);
      xs->file_name = NULL;
    }
  }

  return GLB_SUCCESS;
}


/***************************************************************************
 * Function glb_copy_xsec                                                  *
 ***************************************************************************
 * Create a new cross section structure as a copy of an existing one       *
 ***************************************************************************/
int glb_copy_xsec(glb_xsec *dest, const glb_xsec *src)
{
  int i, j;

  if (!dest)
    return GLBERR_INVALID_ARGS;

  memcpy(dest, src, sizeof(*src));
  if (src->file_name)
  {
    dest->file_name = strdup(src->file_name);
    if (!dest->file_name)
      glb_fatal("glb_copy_xsec: Cannot copy name of cross section file");
  }
  for (j=0; j < GLB_XSEC_COLUMNS; j++)
  {
    if (src->xsec_data[j])
    {
      dest->xsec_data[j] = glb_malloc(src->n_lines*sizeof(src->xsec_data[j][0]));
      for (i=0; i < dest->n_lines; i++)
        dest->xsec_data[j][i] = src->xsec_data[j][i];
    }
  }

  return GLB_SUCCESS;
}

