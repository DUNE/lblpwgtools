
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif

#include <gsl/gsl_spline.h>


//splines
gsl_interp_accel *priors_acc[12];
gsl_spline *priors_spline[12];
double priors_splinerange[12][2];

double my_prior_t23_opoct(const glb_params in, void* user_data);
double my_prior_Capozzi13(const glb_params in, void* user_data);
double my_prior_defglobes(const glb_params in, void* user_data);
void initCapozzi13(int,int);
void test_priors(double osc[], int proj, double xrange[]);
void registerprior();

