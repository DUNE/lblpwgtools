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
#ifndef __GLOBES_H
#define __GLOBES_H 1

#include <stdio.h>

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif /* __cplusplus */


/* Constants */
/* --------- */

/* Activate compatibility mode, i.e. allow deprecated symbol names */
#define GLB_COMPAT

/* Number of neutrino flavours */
#define GLB_NU_FLAVOURS  3


enum glb_enum_oscillation_parameters
       { GLB_THETA_12 = 0, GLB_THETA_13 = 1, GLB_THETA_23 = 2,
         GLB_DELTA_CP = 3, GLB_DM_21 = 4, GLB_DM_31 = 5 };
enum glb_enum_switches
       { GLB_OFF=0, GLB_ON=1 };
enum glb_enum_rate_specs
       { GLB_W_EFF = 1, GLB_WO_EFF = 2,
         GLB_W_BG = 3, GLB_WO_BG = 4,
         GLB_W_COEFF = 5, GLB_WO_COEFF = 6 };
enum glb_enum_param_fixed_free
       { GLB_FIXED = 0, GLB_FREE = 1 };
enum glb_enum_efficiency_types
       { GLB_PRE = 1, GLB_POST = 2 };

/* Available minimization algorithms */
enum glb_enum_minimizers
       { GLB_MIN_NESTED_POWELL, GLB_MIN_POWELL };
#define GLB_MIN_DEFAULT  GLB_MIN_POWELL


#define GLB_EFF    1
#define GLB_BG     2
#define GLB_SIG    3

#define GLB_ALL   -1

#define GLB_EARTH_RADIUS 6371.0 /* km */

/* Unit conversion */

#define GLB_EV_TO_KM_FACTOR  1.9747235e-10
#define GLB_KM_TO_EV(x)      ((x) / GLB_EV_TO_KM_FACTOR)
#define GLB_EV_TO_KM(x)      ((x) * GLB_EV_TO_KM_FACTOR)


/* maximum number of experiments */

#define GLB_MAX_EXP 300




/* Data structures */
/* --------------- */


typedef struct glb_projection_type *glb_projection;
typedef struct glb_params_type  *glb_params;
typedef struct glb_experiment *glb_exp;

/* User-defined chi^2 function */
typedef double (*glb_chi_function)(int exp, int rule, int n_params,
                                   double *params, double *errors, void *user_data);


/* User-defined glb_probability_matrix and glb_set/get_oscillation_parameters */
typedef int (*glb_probability_matrix_function)(double P[3][3], int cp_sign, double E,
                  int psteps, const double *length, const double *density,
                  double filter_sigma, void *user_data);
typedef int (*glb_set_oscillation_parameters_function)(glb_params p, void *user_data);
typedef int (*glb_get_oscillation_parameters_function)(glb_params p, void *user_data);


/* External variables */
/* ------------------ */

extern char **glb_path_vector;
extern size_t glb_path_vector_length;
extern int glb_num_of_exps;

extern int glb_single_experiment_number;
#ifdef SWIG
/* I really have no clue why I have to 
 * declare this one to SWIG and the other global 
 * variables not
 */
glb_exp glb_experiment_list[GLB_MAX_EXP];
#else
extern glb_exp glb_experiment_list[GLB_MAX_EXP];
#endif
extern int glb_rule_number;


/* Function declarations */
/* --------------------- */
BEGIN_C_DECLS

/* Initialization */
void glbInit(char *name);


/* Loading of AEDL files */

void glbClearExperimentList();
void glbDefineAEDLVariable(const char *name, double value);
void glbDefineAEDLList(const char *name, double *list, size_t length);
double glbGetAEDLVariable(const char *name);
void glbClearAEDLVariables();
int glbInitExperiment(char *inf, glb_exp *in, int *counter);


int glbNameToValue(int exp, const char* context, const char *name);
const char *glbValueToName(int exp,const char* context, int value);


/* Version control / verbosity */
int glbTestReleaseVersion(const char *version);
int glbTestLibraryVersion(const char *version);
const char *glbVersionOfExperiment(int exp);
const char *glbGetFilenameOfExperiment(int exp);
const char *glbGetCitationForExperiment(int exp);
int glbSetVerbosityLevel(int level);
int glbGetVerbosityLevel();


/* Handling of oscillation parameter data structure */
glb_params glbAllocParams();
void glbFreeParams(glb_params stale);
glb_params glbDefineParams(glb_params in, double theta12, double theta13,
                           double theta23, double delta, double dm21, double dm31);
glb_params glbCopyParams(const glb_params source, glb_params dest);
glb_params glbSetOscParams(glb_params in, double osc, int which);
glb_params glbSetDensityParams(glb_params in,double dens, int which);
glb_params glbSetIteration(glb_params in, int iter);
double glbGetOscParams(glb_params in, int which);
double glbGetDensityParams(glb_params in, int which);
int glbGetIteration(const glb_params in);
void glbPrintParams(FILE *stream, const glb_params in);

int glbClearParamNames();
int glbSetParamNames(char **names);
int glbSetParamName(const char *name, int i);
char *glbGetParamName(int i);
int glbFindParamByName(const char *name);
int glbSetOscParamByName(glb_params in, double value, const char *name);
double glbGetOscParamByName(const glb_params in, const char *name);

int glbSetOscillationParameters(const glb_params in);
int glbSetInputErrors(const glb_params in);
int glbGetOscillationParameters(glb_params in);
int glbGetInputErrors(glb_params in);

int glbSetCentralValues(const glb_params in);
int glbGetCentralValues(glb_params in);


/* Screen output */
int glbShowRuleRates(FILE *stream, int exp, int rule, int pos,
                     int effi, int bgi, int coeffi, int signal);
int glbShowChannelRates(FILE *stream, int exp, int channel, int smearing, int effi, int bgi);
int glbShowChannelProbs(FILE *stream, int exp, int channel, int smearing, int effi, int bgi);
void glbPrintDelimiter(FILE *stream, int character);
void *glbSetChannelPrintFunction(void *fp);
void glbSetPrintDelimiters(const char *left,const char *middle,
                           const char *right);


/* Event rate calculation */
void glbSetRates();
void glbSetNewRates();


/* chi^2 projections */
glb_projection glbAllocProjection();
void glbFreeProjection(glb_projection stale);
glb_projection glbDefineProjection(glb_projection in, int theta12,
    int theta13,int theta23, int delta, int dm21, int dm31);
glb_projection glbCopyProjection(const glb_projection source, glb_projection dest);
glb_projection glbSetProjectionFlag(glb_projection in, int flag, int which);
glb_projection glbSetDensityProjectionFlag(glb_projection in, int flag, int which);
int glbGetProjectionFlag(const glb_projection in, int which);
int glbGetDensityProjectionFlag(const glb_projection in, int which);
void glbPrintProjection(FILE *stream, const glb_projection in);
int glbSetProjectionFlagByName(glb_projection in, int flag, const char *name);
int glbGetProjectionFlagByName(const glb_projection in, const char *name);

int glbSetProjection(const glb_projection in);
int glbGetProjection(glb_projection in);

double glbChiSys(const glb_params in,int experiment, int rule);
double glbChiTheta13(const glb_params in, glb_params out, int exp);
double glbChiTheta12(const glb_params in, glb_params out, int exp);
double glbChiTheta23(const glb_params in, glb_params out, int exp);
double glbChiDelta(const glb_params in, glb_params out, int exp);
double glbChiDm21(const glb_params in, glb_params out, int exp);
double glbChiDm31(const glb_params in, glb_params out, int exp);
double glbChiTheta13Delta(const glb_params in, glb_params out, int exp);
double glbChiNP(const glb_params in, glb_params out, int exp);
double glbChiAll(const glb_params in, glb_params out, int exp);


/* Matter profile handling */
int glbLoadProfileData(const char* filename, size_t *layers, double **length,
                       double **density );
int glbStaceyProfile(double baseline, size_t layers, double **length,
                     double **density);
int glbAverageDensityProfile(double baseline, double **length,
                             double **density);
int glbGetProfileDataInExperiment(int exp,size_t *layers, double** length,
                                  double** density);
int glbSetProfileDataInExperiment(int exp, size_t layers,const double* length,
                                  const double* density);
int glbSetBaselineInExperiment(int exp, double baseline);
int glbGetProfileTypeInExperiment(int exp);
double glbGetBaselineInExperiment(int exp);


/* Access to miscellaneous experiment parameters */
int glbSetTargetMass(int experiment, double mass);
int glbSetSourcePower(int experiment, int flux_ident, double power);
int glbSetRunningTime(int experiment, int flux_ident, double time);
int glbSetFilterStateInExperiment(int experiment,int on_off);
int glbSetFilterInExperiment(int experiment,double filter);
int glbCompensateFilterInExperiment(int experiment);
int glbOptimizeSmearingMatrixInExperiment(int experiment);

double glbGetTargetMass(int experiment);
double glbGetSourcePower(int experiment, int flux_ident);
double glbGetRunningTime(int experiment, int flux_ident);
int glbGetFilterStateInExperiment(int experiment); 
double glbGetFilterInExperiment(int experiment);
int glbGetEminEmax(int experiment, double *emin, double *emax);
int glbGetEnergyWindow(int experiment, int rule, double *low, double *high);
int glbGetEnergyWindowBins(int experiment, int rule, int *low_bin, int *high_bin);
int glbSetEnergyWindow(int experiment, int rule, double low, double high);
int glbGetNumberOfSamplingPoints(int exp);
int glbGetNumberOfBins(int exp);
int glbGetNumberOfRules(int exp);
int glbGetNumberOfChannels(int exp);
double *glbGetBinSizeListPtr(int exp);
double *glbGetBinCentersListPtr(int exp);
double *glbGetSamplingStepsizeListPtr(int exp);
double *glbGetSamplingPointsListPtr(int exp);
int glbGetLengthOfRule(int exp, int rule, int signal);
int glbGetChannelInRule(int exp, int rule, int pos, int signal);
double glbGetCoefficientInRule(int exp, int rule, int pos, int signal);
int glbSetCoefficientInRule(int exp, int rule, int pos, int signal, double coeff);
int glbGetChannelFlux(int exp, int channel);
int glbGetNumberOfFluxes(int exp);
double glbFlux(int experiment, int flux_ident, 
        double energy, double distance, int flavour, int anti);
double glbXSection(int experiment, int xsec_ident,double energy,int flavour,
                   int anti);


/* User-defined Systematics */
int glbDefineChiFunction(glb_chi_function chi_func, int dim, const char *name, void *user_data);
int glbSetChiFunction(int exp, int rule, int on_off, const char *sys_id, double *errors); 
int glbSwitchSystematics(int exp, int rule, int on_off);
int glbSetSignalErrors(int exp, int rule, double norm, double tilt);
int glbSetSignalStartingValues(int exp, int rule, double norm, double tilt);
int glbSetBGErrors(int exp, int rule, double norm, double tilt);
int glbSetBGStartingValues(int exp, int rule, double norm, double tilt);
int glbSetSysErrorsList(int exp, int rule, int on_off, const double *sys_list);
int glbSetSysStartingValuesList(int exp, int rule, int on_off, const double *sys_list);

int glbGetSysDim(const char *name);
int glbGetSysDimInExperiment(int exp, int rule, int on_off);
int glbGetChiFunction(int exp, int rule, int on_off, char *sys_id, size_t max_len);
glb_chi_function glbGetChiFunctionPtr(const char *name);
glb_chi_function glbGetChiFunctionPtrInExperiment(int exp, int rule, int on_off);
int glbGetSysOnOffState(int exp, int rule);
int glbGetSignalErrors(int exp, int rule, double *norm, double *tilt);
int glbGetSignalStartingValues(int exp, int rule, double *norm, double *tilt);
int glbGetBGErrors(int exp, int rule, double *norm, double *tilt);
int glbGetBGStartingValues(int exp, int rule, double *norm, double *tilt);
double *glbGetSysErrorsListPtr(int exp, int rule, int on_off);
double *glbGetSysStartingValuesListPtr(int exp, int rule, int on_off);

void glbShiftEnergyScale(double g, double *rates_in, double *rates_out,
                         int n_bins, double emin, double emax);


#ifndef SWIG
int glbRegisterPriorFunction(double (*prior)(const glb_params, void *user_data),
                             int (*starting)(const glb_params, void *user_data),
                             int (*error)(const glb_params, void *user_data),
                             void *user_data);
#endif /* SWIG */


/* Implementation of non-standard physics */
int glbRegisterProbabilityEngine(int n_parameters,
                 glb_probability_matrix_function prob_func,
                 glb_set_oscillation_parameters_function set_params_func,
                 glb_get_oscillation_parameters_function get_params_func,
                 void *user_data);
int glbGetNumOfOscParams();


/* Access to event rate vectors */
double glbTotalRuleRate(int exp, int rule, int pos,int effi, 
			int bgi, int coeffi, int signal);
double *glbGetChannelRatePtr(int exp, int channel, int pre_post);
double *glbGetRuleRatePtr(int exp, int rule);
double *glbGetSignalRatePtr(int exp, int rule);
double *glbGetBGRatePtr(int exp, int rule);
double *glbGetChannelFitRatePtr(int exp, int channel, int pre_post);
double *glbGetSignalFitRatePtr(int exp, int rule);
double *glbGetBGFitRatePtr(int exp, int rule);

double *glbGetEfficiencyPtr(int exp, int channel, int pre_post);
double *glbGetBackgroundPtr(int exp, int channel, int pre_post);


/* Access to the probablity engine */
double glbVacuumProbability(int initial_flavour, int final_flavour,
                            int cp_sign, double E, double L);
double glbConstantDensityProbability(int initial_flavour, int final_flavour,
                            int cp_sign, double E, double L, double rho);
double glbProfileProbability(int exp,int initial_flavour, int final_flavour,
                            int panti, double energy);
double glbFilteredConstantDensityProbability(int exp,int initial_flavour,
                            int final_flavour, int panti, double energy);

/* Selecting a minization algorithm */
int glbSelectMinimizer(int minimizer_ID);



/* Some symbols which are provided for compatibility with */
/* older versiond of GLoBES                               */
#ifdef GLB_COMPAT
  #define GLB_DM_SOL  GLB_DM_21   /* Redefined to avoid confusion of dm31 and dm32 */
  #define GLB_DM_ATM  GLB_DM_31 

 #define GLB_OSCP (glbGetNumOfOscParams())
    
  /* Replaced by glbChiTheta13 */
  double glbChiTheta(const glb_params in, glb_params out, int exp);

  /* Replaced by glbChiDm21 */
  double glbChiDms(const glb_params in, glb_params out, int exp);

  /* Replaced by glbChiDm31 */
  double glbChiDm(const glb_params in, glb_params out, int exp);

  /* Replaced by glbChiTheta13Delta */
  double glbChiThetaDelta(const glb_params in, glb_params out, int exp);


  /* Replaced by glbGetEfficiencyPtr/glbGetBackgroundPtr */
  int glbGetUserData(double **data, size_t *length,
                     int exp, int channel,int smearing, int bgeff);

  /* Replaced by glbChannelRatePtr */
  int glbGetChannelRates(double **data, size_t *length,
                         int exp, int channel,int smearing);
  
  /* Replaced by glbGetProfileDataInExperiment */
  int glbGetProfileData(size_t *layers, double** length, double** density);
  
  
  void glbResetRateStack();                   /* Obsolete */
  
  int glbSetFilterState(int on_off);          /* No longer implemented */
  int glbGetFilterState();                   
  int glbSetFilter(double filter);
  double glbGetFilter();


  /* Replaced by glbSet/GetChiFunction */
  int glbSetErrorDim(int experiment, int rule, int on_off, int errordim);
  int glbGetErrorDim(int experiment, int rule, int on_off);

  /* No longer supported since @backgroundcenter is deprecated */
  double glbGetNormalizationInRule(int exp, int rule, int signal);
  int glbSetBGCenters(int exp, int rule, double norm, double tilt);
  int glbGetBGCenters(int exp, int rule, double *norm, double *tilt);

  /* Renamed to glbSet/GetCentralValues */
  int glbSetStartingValues(const glb_params in);
  int glbGetStartingValues(glb_params in);

  /* Renamed to glbGetNumberOfSamplingPoints */
  int glbGetNumberOfSimBins(int exp);
#endif  /* #ifdef GLB_COMPAT */


END_C_DECLS

#endif /* #ifndef __GLOBES_H */


