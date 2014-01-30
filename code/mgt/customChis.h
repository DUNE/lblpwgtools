
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif

#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix_char.h>                 
#include <gsl/gsl_matrix_double.h>  
#include <gsl/gsl_matrix_long_double.h>  
#include <gsl/gsl_matrix_uint.h>
#include <gsl/gsl_matrix_complex_double.h>       
#include <gsl/gsl_matrix_float.h>   
#include <gsl/gsl_matrix_long.h>         
#include <gsl/gsl_matrix_ulong.h>
#include <gsl/gsl_matrix_complex_float.h>        
#include <gsl/gsl_matrix.h>         
#include <gsl/gsl_matrix_short.h>        
#include <gsl/gsl_matrix_ushort.h>
#include <gsl/gsl_matrix_complex_long_double.h>  
#include <gsl/gsl_matrix_int.h>     
#include <gsl/gsl_matrix_uchar.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_spline.h>

double xmin[2][32]; //stores minimization results
double fchi1,fchi2; //stores stat and penalty terms respectively
double priors[4];
double sigma_binbin;
double ts[5][100];
double gpriors[7][4];
double chiSpectrumTiltSplitBG(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);
double chiSpectrumTiltCustom(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);
double chiSpectrumTiltCustom_BtoB(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);
double chiSpectrumSplitBG(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);
double chiSpectrumSplitBG_BtoB(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);
double chiNormPerBin(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);

//Error matrix
double* errorMatrix[32];
gsl_matrix* Total_Input_Matrix[32];
gsl_matrix* Inv_Matrix[32];
double *prior_fit_rates[32];
double chiSpectrum_ErrorMatrix(int exp, int rule, int n_params, double *x, double *errors, void *user_data);
double CalcDeterminant( float **mat, int order);
int GetMinor(float **src, float **dest, int row, int col, int order);
void MatrixInversion(float **A, int order, float **Y);

//Response functions
gsl_interp_accel **rf_acc[32];
gsl_spline **rf_spline[32];
int rfCols[32][32]; //number of sigma entries for RFs
int penalty[32][32]; //whether or not there is a penalty term
double** mult_presmear_effs[32]; //presmearing effs from response functions
double* rfCovMatrix; //covariance matrix for reponse functions
void LoadRFCovMatrix();
double chi_ResponseFunction(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);
double chi_ResponseFunctionCov(int exp, int rule, int n_params, double *x, double *errors,
                          void *user_data);
                          
//Other
double my_prior_t23_opoct(const glb_params in, void* user_data);



