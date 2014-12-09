#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif

double DoChiSquareCPR(double x, void *dummy);
void ComputeCPVCurve(double osc[]);
void ComputeMHSigCurve(double osc[]);
void ComputeMHSigCurve_v2(double osc[], double dcpin, int disableScanSysts);
void ComputeAllowedCP(double osc[]);
void ComputeCPResolutionCurve(double osc[]);
void ComputeCPResolutionCurve2(double osc[]);
void OscProbs(double osc[]);
void EventRates(double osc[]);
void dchisquared(double osc[], int proj, double xrange[], int dcpPreScan);
void ComputeRFCurve(double osc[], int truefitspectra);
void ComputeRFCurve2d(double osc[]);
void TrueFitSpectraMinOutput(double osc[]);
void FCMap_1D(double osc[], int proj, double xrange[]);
void ComputeNuisanceReponse(double osc[], int nuis);
