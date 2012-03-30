
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <argp.h>
#include <float.h>


#include "globes/globes.h"   /* GLoBES library */
double xmin[2][32];
double fchi1,fchi2;
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


int tmyprior_init();
//void tmyprior_clean();
//static double tsprior(double , double , double );
double tmyprior_prior(const glb_params , void *);
int tmyprior_input_errors(const glb_params , void *);
int tmyprior_starting_values(const glb_params , void *);
