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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <setjmp.h>
#include <globes/globes.h>


#include "glb_types.h"
#include "glb_smear.h"
#include "glb_probability.h"
#include "glb_multiex.h"
#include "glb_fluxes.h"
#include "glb_error.h"
#include "glb_rate_engine.h"


#define PI 3.1415
#define EARTH_RADIUS 6371
#define NUCLEON_MASS 1.6726E-27
#define CROSS_SECTION 0.67E-45
#define CROSS_SECTION_ANTI 0.34E-45

#define FLOAT double


static double baseline = 1000 ;   // baseline in km
static double target_mass = 10;   // target mass in kt;
static double years = 1;          // running time in years
static int bins = 20;          // number of energy bins
static double treshold = 5.0;     // treshold energy

static double Probs[3][3];
static double ProbsAnti[3][3];
double** glb_calc_smearing[32];
int* glb_calc_uprange[32];
int* glb_calc_lowrange[32];
static int* rules[32];
static int rule_length[32];
static double* rule_coeff[32];
static int* BGrules[32];
static int BGrule_length[32];
static double* BGrule_coeff[32];
 double* glb_calc_signal_rates[32];
 double* glb_calc_bg_rates[32];
// ---------------------------------
double* glb_calc_rates_0[32];
double* glb_calc_rates_1[32];
double* glb_calc_rates_1BG[32];
double* glb_calc_energy_tab;
double glb_bg_norm_center[32];
double glb_bg_tilt_center[32];


int glb_calc_simbins;
double glb_calc_simtresh;
double glb_calc_simbeam;

static double* smm;
static int exptype = 1;
static int channel_list[32][6];
static int num_of_ch = 1;
int glb_num_of_rules=1;


/* storing the channel result before glb_calc_smearing (glb_calc_simbins) */
double *glb_calc_chrb_0[32];
double *glb_calc_chrb_1[32];

/* storing the channel result after glb_calc_smearing (bins) */
double *glb_calc_chra_0[32];
double *glb_calc_chra_1[32];

double *glb_calc_chr_template[32];

//binwise glb_calc_efficiencies
double* glb_calc_efficiencies[32];
//constant background
double* glb_calc_const_background[32];


double *glb_calc_user_pre_sm_channel[32];
double *glb_calc_user_post_sm_channel[32];
double *glb_calc_user_pre_sm_background[32];
double *glb_calc_user_post_sm_background[32];

/* the new glb_calc_smearing structure */

glb_smear *glb_calc_smear_data[32];


glb_flux *glb_calc_fluxes[32];
glb_xsec *glb_calc_xsecs[32];



// ------------------------------------------
// -----      Initialize Parameters      ----
// ------------------------------------------
// some of these functions may be superflous

void glb_set_type(int type)
{
  exptype =type;
}

int glb_get_type()
{
  return exptype;
}


void glb_set_baseline(double l)
{
  struct glb_experiment *e = glb_experiment_list[glb_current_exp];
  double sum,*ll;
  size_t s,i;
  s = e->psteps;
  ll = e->lengthtab;
  sum=0;
  if(s<=0)
    {
      glb_error("No baseline data to change");
      return;
    }
  for(i=0;i<s;i++) sum += ll[i];
  if(fabs(sum-l)>1E-8)
    {
      glb_error("Sum of lengths of layers does not match baseline");
      return;
    }
  baseline = sum;
}

double glb_check_baseline()
{
  return baseline;
}



/* in kt */
void glb_set_target_mass(double mass)
{
  target_mass = mass;
}

double glb_check_target_mass()
{
  return target_mass;
}

void glb_set_years(double year)
{
  years = year;
}

double glb_check_years()
{
  return years;
}



void glb_set_energy_treshold(double etresh)
{
  treshold = etresh;
}

double glb_check_energy_treshold()
{
  return treshold;
}


void glb_set_number_of_bins(int nr)
{
  bins = nr;
}

int glb_check_number_of_bins()
{
  return bins;
}

int glb_get_number_of_bins()
{
  return bins;
}

/* Initialize the controlstructure for signal and backgrounds
 * Here I introduce the concept of channels. In general there are
 * three flavours in a beam, with to CP-signs, thus there six initial
 * states, which can oscillate to six final states. The reaction in
 * the detector can be anything, e.g. CC. This are altogether 72
 * possibilities, of which you usually need only a few. Therefore we
 * define channels.
 * A channel is defined by a flux, the CP-sign, an initial and final state,
 * the cross section and the energy resolution function.
 */


void glb_set_channel(int i, int polarity, int anti, int l, int m, int cc,
		int energysmear)
{
  channel_list[i][0]=polarity;
  channel_list[i][1]=anti;
  channel_list[i][2]=l;
  channel_list[i][3]=m;
  channel_list[i][4]=cc;
  channel_list[i][5]=energysmear;
}

double* glb_get_channel(int i)
{
  double* erg;
  int k;
  erg=(double*) glb_malloc(6 * sizeof(double));
  for (k=0;k<6;k++) erg[k]=channel_list[i][k];
  return erg;
}



void glb_set_num_of_channels(int cc)
{
 num_of_ch=cc;
}


int glb_calc_check_num_of_channels()
{
 return num_of_ch;
}

/* Here we define rules, how to convert the channels into the observable
 * signal and background. A rule has variable length, a list of channel
 * numbers and the coefficients for each channel, like the efficiency.
 */

void glb_set_number_of_rules(int cn)
{
 glb_num_of_rules=cn;
}

int glb_calc_check_num_of_rules()
{
  return glb_num_of_rules;
}

void glb_set_rule(int i, int cn, int *rule, double *coeff)
{
  rule_coeff[i]= coeff;
  rule_length[i] = cn;
  rules[i]=rule;
}



int glb_calc_glb_calc_check_rule_length(int i, int bg)
{
  if (bg==1)
    {
      return rule_length[i];
    }
  else
  {
    return BGrule_length[i];
  }
}

double* glb_calc_glb_calc_check_rule_coeff(int i, int bg)
{
  if (bg==1)
    {
      return rule_coeff[i];
    }
  else
    {
      return BGrule_coeff[i];
    }
}
int* glb_calc_check_rule(int i,int  bg)
{
  if (bg==1)
    {
      return rules[i];
    }
  else
    {
      return BGrules[i];
    }
}

void glb_set_bg_rule(int i, int cn, int *rule, double *coeff)
{
  BGrule_coeff[i]= coeff;
  BGrule_length[i] = cn;
  BGrules[i]=rule;
}



// ----------------------------------------
// -----      Binned Rate Vectors      ----
// ----------------------------------------




// adaption to Martins version of oszprob.cc

static void CalcAllProbs(double en, double baseline)
{
  struct glb_experiment *e = glb_experiment_list[glb_current_exp];
  int i, j;
  int status;

  if ((status=glb_hook_probability_matrix(Probs, +1, en, e->psteps, e->lengthtab, e->densitybuffer,
          (e->filter_state == GLB_ON) ? e->filter_value : -1.0, glb_probability_user_data)) != GLB_SUCCESS)
    glb_error("Calculation of oscillation probabilities failed.");

  if ((status=glb_hook_probability_matrix(ProbsAnti, -1, en, e->psteps, e->lengthtab, e->densitybuffer,
          (e->filter_state == GLB_ON) ? e->filter_value : -1.0, glb_probability_user_data)) != GLB_SUCCESS)
    glb_error("Calculation of oscillation probabilities failed.");
}

static double RatesNOSC(double en, double baseline,
		 int polarity, int anti, int l, int m,int ident)
{
  double ergebnis;

  ergebnis=glb_get_xsec(en,m,anti,glb_calc_xsecs[ident])
    *glb_get_flux(en,baseline,l,anti,glb_calc_fluxes[polarity])
    *target_mass;

  return ergebnis;
}

// Rates for arbitrary XSection

static double RatesXX(double en, double baseline, int polarity, int anti, int l, int m,int ident)
{
  double ergebnis;
  if (anti == 1)
    {
      ergebnis=glb_get_xsec(en,m,anti,glb_calc_xsecs[ident])*
	glb_get_flux(en,baseline,l,anti,glb_calc_fluxes[polarity])
	*Probs[l-1][m-1]*target_mass;
    }
  else
    {
      ergebnis=glb_get_xsec(en,m,anti,glb_calc_xsecs[ident])
	*glb_get_flux(en,baseline,l,anti,glb_calc_fluxes[polarity])
	*ProbsAnti[l-1][m-1]*target_mass;

    }
  return ergebnis;
}



double glb_calc_channel(int i, double en, double baseline)
{
  double ergebnis;
  int final_flavour,initial_flavour;
  int nosc=0;
  if(channel_list[i][3]>9) {final_flavour=channel_list[i][3]-10;nosc=1;}
  else {final_flavour=channel_list[i][3];}
  if(channel_list[i][2]>9) {initial_flavour=channel_list[i][2]-10;nosc=1;}
  else {initial_flavour=channel_list[i][2];}


    if(nosc!=0)
      {
	if(final_flavour!=initial_flavour) {return 0;}
	ergebnis=RatesNOSC(en,baseline,channel_list[i][0],
			   channel_list[i][1],initial_flavour,
			   final_flavour,channel_list[i][4]);
      }
    else
      {
	ergebnis=RatesXX(en,baseline,channel_list[i][0],
			 channel_list[i][1],initial_flavour,
			 final_flavour,channel_list[i][4]);
      }
  return ergebnis;
}


static double Signal(int cn, double en, double baseline)
{
  int k;
  double ergebnis=0.0;
  for (k=0;k<rule_length[cn];k++)
    {
      ergebnis += rule_coeff[cn][k] * glb_calc_channel(rules[cn][k],en,baseline);


    }
  return ergebnis;
}

static double Background(int cn, double en, double baseline)
{
  int k;
  double ergebnis=0.0;
  for (k=0;k<BGrule_length[cn];k++)
    {
      ergebnis += BGrule_coeff[cn][k] * glb_calc_channel(BGrules[cn][k],en,baseline);
    }
  return ergebnis;
}


inline static double BinEnergy(int i)
{
  /* This fixed a bug with energy window stuff for variable bin width */
  return glb_bin_center(i,glb_calc_smear_data[0]);
}


/***************************************************************************
 * Function glb_set_rates                                                  *
 ***************************************************************************
 * Calculate the "true" event rates for the current experiment.            *
 ***************************************************************************/
void glb_set_rates()
{
  int i, j, k, s;
  int ew_low, ew_high;

  for(j=0; j < bins; j++)
    glb_calc_energy_tab[j] = BinEnergy(j);

  /* Calculate pre-smearing rates for all channels */
  for (j=0; j < glb_calc_simbins; j++)
  {
    /* Calculate probability matrix */
    CalcAllProbs(glb_sbin_center(j, glb_calc_smear_data[0]), baseline);
    for(i=0; i < num_of_ch; i++)
    {
      /* Calculate rate and incorporare pre-smearing efficiencies and
       * pre-smearing backgrounds */
      s = channel_list[i][5];
      glb_calc_chrb_0[i][j] = glb_calc_channel(i, glb_sbin_center(j,glb_calc_smear_data[s]),
                                               baseline)
              * glb_calc_user_pre_sm_channel[i][j]
              * glb_calc_smear_data[s]->simbinsize[j]
              + glb_calc_user_pre_sm_background[i][j];

      /* If NOSC-flag was set in glb-file, the "true" and fitted rates
       * are identical --> store them also in glb_calc_chrb_1 */
      if ((channel_list[i][2] > 9 || channel_list[i][3] > 9))
        glb_calc_chrb_1[i][j] = glb_calc_chrb_0[i][j];
    }
  }

  /* Calculate post-smearing rates for all channels */
  for(i=0; i < num_of_ch; i++)
  {
    s = channel_list[i][5];
    for(j=0; j < bins; j++)
    {
      /* Apply smearing matrix */
      glb_calc_chra_0[i][j] = 0;
      for(k=glb_calc_lowrange[s][j]; k < glb_calc_uprange[s][j]+1; k++)
      {
        glb_calc_chra_0[i][j] += glb_calc_smearing[s][j][k-glb_calc_lowrange[s][j]]
                                   * glb_calc_chrb_0[i][k];
      }

      /* Apply post-smearing efficiencies and post-smearing backgrounds */
      glb_calc_chra_0[i][j] = glb_calc_chra_0[i][j] * glb_calc_user_post_sm_channel[i][j]
                                  + glb_calc_user_post_sm_background[i][j];
    }

    /* If NOSC-flag was set in glb-file, the "true" and fitted rates
     * are identical --> store them also in glb_calc_chra_1 */
    if ((channel_list[i][2] > 9 || channel_list[i][3] > 9))
      for(j=0; j < bins; j++)
        glb_calc_chra_1[i][j] = glb_calc_chra_0[i][j];
  }

  /* Merge channel rates into signal, background, and total rates */
  for(i=0; i < glb_num_of_rules; i++)
  {
    for(j=0; j < bins; j++)
    {
      /* Background */
      glb_calc_bg_rates[i][j] = 0.0;
      for (k=0; k < BGrule_length[i]; k++)
        glb_calc_bg_rates[i][j] += BGrule_coeff[i][k] * glb_calc_chra_0[BGrules[i][k]][j];

      /* Signal */
      glb_calc_signal_rates[i][j] = 0.0;
      for (k=0; k < rule_length[i]; k++)
        glb_calc_signal_rates[i][j] += rule_coeff[i][k] * glb_calc_chra_0[rules[i][k]][j];
    }

    for (j=0; j < bins; j++)
    {
      /* Total */
      glb_calc_rates_0[i][j] = glb_calc_signal_rates[i][j]
                               + glb_calc_bg_rates[i][j]*glb_bg_norm_center[i];
    }
  }
}


/***************************************************************************
 * Function glb_set_new_rates                                              *
 ***************************************************************************
 * Calculate the fitted event rates for the current experiment.            *
 ***************************************************************************
 * Parameters:                                                             *
 *   fast: if set to GLB_FAST_RATES, the computation is accelerated by     *
 *         using precomputed  prefactors from chr_template                 *
 ***************************************************************************/
void glb_set_new_rates(int fast)
{
  int i, j, k, s;

  /* Calculate pre-smearing rates for all channels */
  if (fast == GLB_FAST_RATES)
  {
    for (j=0; j < glb_calc_simbins; j++)
    {
      int probs = 0; /* Probabilities already computed? */
      for(i=0; i < num_of_ch; i++)
      {
        int fi = channel_list[i][2]; /* Initial flavour */
        int ff = channel_list[i][3]; /* Final flavour   */

        /* Recalculate rates only if NOSC-flag was not set in glb-file */
        if (fi <= 9 && ff <= 9)
        {
          if (!probs)
          {
            CalcAllProbs(glb_sbin_center(j, glb_calc_smear_data[0]), baseline);
            probs = 1;
          }
          int anti = channel_list[i][1];
          if (anti == 1)
            glb_calc_chrb_1[i][j] = glb_calc_chr_template[i][j] * Probs[fi-1][ff-1]
                                       + glb_calc_user_pre_sm_background[i][j];
          else
            glb_calc_chrb_1[i][j] = glb_calc_chr_template[i][j] * ProbsAnti[fi-1][ff-1]
                                       + glb_calc_user_pre_sm_background[i][j];
        }
      } /* for(i) */
    } /* for(j) */
  }
  else
  {
    for (j=0; j < glb_calc_simbins; j++)
    {
      /* Calculate probability matrix */
      CalcAllProbs(glb_sbin_center(j, glb_calc_smear_data[0]), baseline);
      for(i=0; i < num_of_ch; i++)
      {
        /* Recalculate rates only if NOSC-flag was not set in glb-file */
        if (channel_list[i][2] <= 9 && channel_list[i][3] <= 9)
        {
          s = channel_list[i][5];
          glb_calc_chrb_1[i][j] = glb_calc_channel(i, glb_sbin_center(j,glb_calc_smear_data[s]),
                                                   baseline)
                  * glb_calc_user_pre_sm_channel[i][j]
                  * glb_calc_smear_data[s]->simbinsize[j]
                  + glb_calc_user_pre_sm_background[i][j];
        }
      } /* for(i) */
    } /* for(j) */
  } /* if(fast) */

  /* Calculate post-smearing rates for all channels */
  for(i=0; i < num_of_ch; i++)
  {
    /* Recalculate rates only if NOSC-flag was not set in glb-file */
    if (channel_list[i][2] <= 9 && channel_list[i][3] <= 9)
    {
      s = channel_list[i][5];
      for(j=0; j < bins; j++)
      {
        glb_calc_chra_1[i][j] = 0.0;
        int k_low  = glb_calc_lowrange[s][j];
        int k_high = glb_calc_uprange[s][j] + 1;
        for(k=k_low; k < k_high; k++)
          glb_calc_chra_1[i][j] += glb_calc_smearing[s][j][k-k_low]
                                     * glb_calc_chrb_1[i][k];
        glb_calc_chra_1[i][j] = glb_calc_chra_1[i][j] * glb_calc_user_post_sm_channel[i][j]
                                    + glb_calc_user_post_sm_background[i][j];
      }
    }
  }

  /* Merge channel rates into signal and background rates */
  for(i=0; i < glb_num_of_rules; i++)
  {
    for(j=0; j < bins; j++)
    {
      glb_calc_rates_1BG[i][j] = 0.0;
      for (k=0; k < BGrule_length[i]; k++)
        glb_calc_rates_1BG[i][j] += BGrule_coeff[i][k] * glb_calc_chra_1[BGrules[i][k]][j];

      glb_calc_rates_1[i][j] = 0.0;
      for (k=0; k < rule_length[i]; k++)
        glb_calc_rates_1[i][j] += rule_coeff[i][k] * glb_calc_chra_1[rules[i][k]][j];
    }
  }
}


/***************************************************************************
 * Function glb_rate_template                                              *
 ***************************************************************************
 * Calculate products of cross sections, fluxes, and prefactors for all    *
 * channels in current experiment. This helps to speed up later            *
 * computations                                                            *
 ***************************************************************************/
void glb_rate_template()
{
  int i, j, s;
  for (i=0; i < 3; i++)
    for (j=0; j < 3; j++)
      Probs[i][j] = ProbsAnti[i][j] = 1.0;

  /* Calculate pre-smearing rates for all channels */
  for (j=0; j < glb_calc_simbins; j++)
  {
    for(i=0; i < num_of_ch; i++)
    {
      /* Recalculate rates only if NOSC-flag was not set in glb-file */
      if (channel_list[i][2] <= 9 && channel_list[i][3] <= 9)
      {
        s = channel_list[i][5];
        glb_calc_chr_template[i][j] =
          glb_calc_channel(i, glb_sbin_center(j,glb_calc_smear_data[s]), baseline)
                * glb_calc_user_pre_sm_channel[i][j]
                * glb_calc_smear_data[s]->simbinsize[j];
      }
    }
  }
}



/* // ---------------------------------------- */
/* // -----      Statistics Functions      ---- */
/* // ---------------------------------------- */

double* glb_calc_check_bg_center(int i)
{
  double* erg;
  erg=(double*) glb_malloc(2*sizeof(double));
  erg[0]=glb_bg_norm_center[i];
  erg[1]=glb_bg_tilt_center[i];
  return erg;
}

void glb_set_bg_center(int i,double norm, double tilt)
{
  glb_bg_norm_center[i]=norm;
  glb_bg_tilt_center[i]=tilt;
}



// function for avoiding malloc problems
void glb_remove_calc_pointers()
{
  int k;
  for(k=0;k<32;k++)
    {
      glb_calc_smear_data[k]=NULL;
      glb_calc_smearing[k]=NULL;
      rules[k]=NULL;
      rule_coeff[k]=NULL;
      BGrules[k]=NULL;
      BGrule_coeff[k]=NULL;
      glb_calc_signal_rates[k]=NULL;
      glb_calc_bg_rates[k]=NULL;
      glb_calc_rates_0[k]=NULL;
      glb_calc_rates_1[k]=NULL;
      glb_calc_rates_1BG[k]=NULL;
      glb_calc_energy_tab=NULL;
      glb_calc_efficiencies[k]=NULL;
      glb_calc_const_background[k]=NULL;
      glb_calc_chra_0[k]=NULL;
      glb_calc_chra_1[k]=NULL;
      glb_calc_chrb_0[k]=NULL;
      glb_calc_chrb_1[k]=NULL;
    }
  smm=NULL;

}
