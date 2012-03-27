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
#ifndef GLB_TYPES_H
#define GLB_TYPES_H 1

#if HAVE_CONFIG_H   /* config.h should come before any other includes */
#  include "config.h"
#endif

#include "globes/globes.h"

/* This is a close relative of glb_namerec, which is needed in order
 * to facilitate the cooperation between AEDL and C.
 */

/* this serves to handle the 5.2 fudge factor in versions prior to 3.0
 */
#define GLB_OLD_NORM -99

struct glb_naming
{
  char *name; /* name of symbol      */   
  char *context;  /* context rule, channel etc.           */
  int value;  /* e.g. rule number */
  struct glb_naming *next;    /* link field              */
};

typedef struct glb_naming glb_naming;

/* Data structure for handling fluxes and built-in fluxes */
#define GLB_FLUX_COLUMNS   7           /* Number of columns in flux file         */
typedef struct {
  int builtin;
  double time;
  double parent_energy;
  double stored_muons;
  double target_power;
  double norm;
  double gamma;
  double end_point;

  int n_lines;                         /* Number of columns in flux file         */
  char *file_name;                     /* Name of flux file                      */
  double *flux_data[GLB_FLUX_COLUMNS]; /* Flux data (7 x n_lines array)          */
} glb_flux;

/* Data structure for handling X-sections and built-in X-sections,
 * which will be added later.
 */
#define GLB_XSEC_COLUMNS   7           /* Number of columns in xsec file         */
typedef struct {
  int builtin;
  int n_lines;                         /* Number of lines in cross section file  */
  char *file_name;                     /* Name of cross section file             */
  double *xsec_data[GLB_XSEC_COLUMNS]; /* Cross section data (7 x n_lines array) */
} glb_xsec;



/* This are the additional data needed for type B matrices */

typedef struct {
  double corr_fac;
  double confidence_level;
  int offset;
  double low_bound;
  double up_bound;
} glb_option_type;


/* This is a pointer to the function which computes sigma(E,params) */
typedef double (*sigfun)(double, double* );

/* That is a first attempt for a data structure containing all
   the necessary information for computing a smear matrix. Right
   now it is sufficient for Type A matrices but lacks support
   for type B matrices */

#define GLB_TYPE_A 1
#define GLB_TYPE_B 2
#define GLB_TYPE_C 2 /* That is correct since there are now only two types */

typedef struct {
  int type;
  int numofbins;
  int simbins;
  double e_min; 
  double e_max;
  double e_sim_min;
  double e_sim_max;

    

  /* Pointer to the paramters for sig_f */
  double *sigma;
  int num_of_params;
  /* Here is the pointer to sigma(E,params) */
  sigfun sig_f;
  
  /* This is the binsize array -- unused so far.
   * In any case it is probably more useful to store the bin sizes
   * in a simple array.
   */
  
  double *binsize;

  double *simbinsize;

  /* Lookup tables */
  double *bincenter;
  double *simbincenter;

  /* Options for type B matrices */
  glb_option_type *options;
} glb_smear;


/* Data structure containing information about systematics functions */
typedef struct glb_systematic
{
  glb_chi_function chi_func;   /* Pointer to the chi^2 function                      */
  int dim;                     /* Number of systematics parameters                   */
  char *name;                  /* Unique name of this chi^2 routine                  */
  void *user_data;             /* Arbitrary user-defined parameter for chi^2 routine */
  struct glb_systematic *next; /* Pointer to next entry in glb_sys_list              */
} glb_systematic;



/** This structure contains a large part of the information for defining
 * an experiment.
 * It has fixed size since it contains only pointers to dynamic objects.
 * You have to  make sure that the memory for dynamic objects is allocated
 * (and freed !) elsewhere !
 * Since this structure is large and will grow, care should be taken to pass
 * only pointers to it (esp. in functions which are called in each step or 
 * cycle).
 * The plan is, that it should at some point contain really all information
 * for describing an experiment like fluxes etc. ( or at least pointers to
 * the place where those things are)
 */

struct glb_experiment {

  /* Version string */
  char *version;

  /* A string containing citation information for this experiment */
  char *citation;

  /* Name of AEDL file on which this experiment is based */
  char *filename;

  /* This contains the parsing meta-information like names of rules etc. */
  glb_naming *names;
  
  /** The beam spectrum is loaded with glb_flux_loader(file_name, flux_ident)
   */

  /** That is a new way to define and load fluxes */
  glb_flux *fluxes[32];
  int num_of_fluxes;

  /** That is a new way to define and load x-sections */
  glb_xsec *xsecs[32];
  int num_of_xsecs;

 
  /* Bin widths */
  double *binsize;
  double *simbinsize; 

  /** set the way a profile is computed or the baseline is changed */
  int density_profile_type;

  /** baseline is the baseline. It is a positive number. The unit
  * is km. For most cases it should not exceed 2*EARTHRADIUS.
  */
  double baseline;
  
  /** emin is the lower bound on the energy for the events used in the 
  * analysis.
  * It is a positive number and the unit is GeV.
  */ 
  double emin;

  /** emax is the upper bound on the energy for the  events in the analysis.
  * It is a positive number and the unit is GeV. It has to be larger than
  * emin.
  */
  double emax;

  /** Number of bins which divide the range from emin to emax for the analysis.
  * It is larger than zero.
  */
  int numofbins;


  /** Target mass in units of kt (=1000 tons). It is positive.
   */
  double targetmass;

  
  /** Number of channels. It is a number between 1 and 32.
   */
  int numofchannels;

  /** Strange construct. Holds the channel definition ( 5 int`s)
  * describing: beam polarity, initial state, final state, CP sign
  * cross section. The length was 5*numofchannnels. Not very nice!
  * will be changed for two reasons:
  * - it is ugly and the channel definition will be enlarged at least
  * by one number 
  * - the energy resolution function identifier.
  * This new defintion has now get numofchannels*sizeof(int) of memory.
  *
  * malloc!
  */
  int* listofchannels[6];

  /** Number of rules. Ranges from 1 to 32.
   */
  int numofrules;

  /** Tells for each rule how many channels are added to form the signal.
  * Ranges from 1 to 32.
  */
  int lengthofrules[32];

  /** Contains the pre-factor for each channel and rule which is applied to
  * each channel in computing the signal. Has length 
  * lengthofrules*sizeof(double). malloc!
  */
  double* rulescoeff[32];

  /** Contains the identifying number for each channel used in this rule.
  * Has got length lengthofrules*sizeof(int). Range is 0 to lengthofrule-1.
  * malloc!
  */
  int* rulechannellist[32];

  /** Tells for each rule how many channels are added to form the background.
  * Ranges from 1 to 32.
  */
  int lengthofbgrules[32];

  /** Contains the pre-factor for each channel and rule which is applied to
  * each channel in computing the background. Has length 
  * lengthofrules*sizeof(double). malloc!
  */
  double* bgrulescoeff[32];

  /** Contains the identifying number for each channel used in this rule.
  * Has got length lengthofrules*sizeof(int). Range is 0 to lengthofrule-1.
  * malloc!
  */
  int* bgrulechannellist[32];

  /** Number of smearing data types stored */
  int num_of_sm;

  /** Meta information for computing the smear matrix for each rule */
  glb_smear *smear_data[32];

  /** Thus holds the pointer to the place where the energy resolution
  * function is stored. The energy resolution function is stored as
  * a matrix where only the non-zero elements are used. This and the
  * fact that there is a numer of bins in the analysis which can be
  * different from the bins used in the calculation makes the game
  * a little tricky. Be careful! smear contains for each different
  * energy resolution numofbins pointers which point each to vector
  * of length simbins and at this point in the memory you will find
  * a double.
  *
  * \todo This is now only the user-defined mode. Need implemenation
  * for computing this matrix and the other quantities. Also
  * the meta information for this process has to go in here.
  */
  double** smear[32];

  /** For each analysis bin (numofbins) exists a lower index in the
  * range given by simbins for which smear contains non-zero entries
  * (remember only those are stored). Thus lowrange is positive (including
  * zero) and smaller than simbins and smaller than uprange (see below)
  * \todo This is now only the user-defined mode. Need implemenation
  * for computing this matrix and the other quantities. Also
  * the meta information for this process has to go in here.
  */
  int* lowrange[32];

  /** For each analysis bin (numofbins) exists a upper index in the
  * range given by simbins for which smear contains non-zero entries
  * (remember only those are stored). Thus uprange is positive and 
  * smaller than simbins and bigger than uprange.
  * \todo This is now only the user-defined mode. Need implemenation
  * for computing this matrix and the other quantities. Also
  * the meta information for this process has to go in here.
  */
  int* uprange[32];
  
  /** simtresh has the same function for the computation of events than
  * emin for the analysis. simtresh has to be positive and smaller than 
  * emin. The unit is GeV. 
  */ 
  double simtresh;

  /** simbeam has the same function for the computation of events than
  * emax for the analysis. simtresh has to be positive and bigger than 
  * emax and bigger than simtresh. The unit is GeV.
  */ 
  double simbeam;

  /** simbins is the number of bins dividing (equi-distant) 
  * the range from simtresh to
  * simbeam. It is a positive number.
  * \todo This is now only the user-defined mode. Need implemenation
  * for computing this matrix and the other quantities. Also
  * the meta information for this process has to go in here.
  */
  int simbins; 

  /** Changed.
  * filter_state is either "GLB_ON" or "GLB_OFF".
  */
  int filter_state;

  /** Positive number.
   */
  double filter_value;

  /** The number of layers for the matter profile. It is a number greater
  * than 0. Default is 1.
  */
  int psteps;

  /** glb_List of the thickness of each matter layer. Unit is km and positive. 
  * Has length
  * psteps. The sum of all length in lengthtab should equal baseline.
  * Here a mechanism for ensuring this relation has to be implemented.
  * 
  */
  double* lengthtab;

  /** glb_List of the matter densities for each layer. It is a positive number
  * and the unit is g cm^-3. Has length psteps.
  * 
  */
  double* densitytab;

  /** A buffer needed in the computation of the matter profile uncertainty.
  * Does not appear in any config-file, but has to be malloced on
  * initialization. Has length psteps. Determined internally.
  */
  double* densitybuffer;

  /** New.
  * Contains a number for each simbin which is multiplied with each
  * channel before doing the smearing. Has length simbins. Positive
  * including zero.
  */
  double* user_pre_smearing_channel[32];

  /** New.
  * Contains a number for each bin which is multiplied with each
  * channel after doing the smearing. Has length bins. Positive
  * including zero.
  */
  double* user_post_smearing_channel[32];

  /** New.
  * Contains a number for each bin which is added to each
  * bin before doing the smearing. Has length simbins. Positive
  * including zero.
  */
  double* user_pre_smearing_background[32];

  /** New.
  * Contains a number for each bin which is added to each
  * bin after doing the smearing. Has length bins. Positive
  * including zero.
  */
  double* user_post_smearing_background[32];

  /** New.
  * Store pre-computed background. Has length numofbins. 
  * Determined internally.
  */
  double* no_osc_background[32];

  /** Energy range for each rule in which the events are used to compute
  * the chi^2. Lower limit and upper limit.
  */
  double energy_window[32][2];    /* Energy range */
  int energy_window_bins[32][2];  /* Bin range    */

  /** Has length numofbins */
  double* energy_tab;


  /** Now comes a bunch of pointers which finally are vectors containing
  * the different parts of event vectors needed during computation.
  * All mallocing has to be done at intialization of a given experiment!
  */
  double *chrb_0[32], *chrb_1[32]; /* True and fitted pre-smearing rates by channel   */
  double *chra_0[32], *chra_1[32]; /* True and fitted post-smearing rates by channel */
  double *chr_template[32];   /* Products of fluxes, cross sections, and prefactors by channel */
  double* SignalRates[32];    /* "True" signal event rates for all rules */
  double* BackgroundRates[32];/* "True" background event rates for all rules */
  double* rates0[32];         /* "True" event rates for all rules */
  double* rates1[32];         /* Fitted signal rates for all rules */
  double* rates1BG[32];       /* Fitted background rates for all rules */

  /** Systematics functions and on/off states */
  int sys_on_off[32];         /* Systematics switch (GLB_ON/GLB_OFF)     */
  glb_systematic *sys_on[32]; /* The chi^2 functions of the rules        */
  glb_systematic *sys_off[32]; 
  char *sys_on_strings[32];   /* The errordim strings from the AEDL file */
  char *sys_off_strings[32];

  /** Systematical error and starting values (GLoBES 3.0 scheme) */
  double *sys_on_errors[32];     /* Systematical errors */
  double *sys_on_startvals[32];  /* Starting values for systematics minimization */
  double *sys_off_errors[32];    /* Systematical errors */
  double *sys_off_startvals[32]; /* Starting values for systematics minimization */

  /** Systematical error and starting values (GLoBES 2.0.11 scheme) */
  double signal_errors[2][32];   /* Signal norm/energy errors for old chi^2 functions */
  double signal_startvals[2][32];/* Signal starting values for old chi^2 functions */
  double bg_errors[2][32];       /* Background norm/energy errors for old chi^2 functions */
  double bg_startvals[2][32];    /* BG starting values for old chi^2 functions */
  double bg_centers[2][32];      /* BG central values for old chi^2 functions */
};

#endif /* GLB_TYPES_H 1 */
