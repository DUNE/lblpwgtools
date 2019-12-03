
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif


void bestfitmc(double osc[]);
void nofitmc(double osc[], int priorson, int pf);
void EventSpectraOutputTruePrePost(double osctrue[], double osctest[], int fittype);
