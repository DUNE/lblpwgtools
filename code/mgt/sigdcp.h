#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif

double DoChiSquareCPR(double x, void *dummy);
void ComputeCPVCurve(double osc[]);
void ComputeMHSigCurve(double osc[]);
void ComputeMHSigCurve_v2(double osc[], double dcpin);
void ComputeAllowedCP(double osc[]);
void ComputeCPResolutionCurve(double osc[]);
void ComputeCPResolutionCurve2(double osc[]);
void OscProbs(double osc[]);
void EventRates(double osc[]);
void dchisquared(double osc[], int proj, double xrange[], int dcpPreScan);
void ComputeRFCurve(double osc[], int truefitspectra);
void TrueFitSpectraMinOutput(double osc[]);
