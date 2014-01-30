#ifndef MGT_HELPER_H
#include "mgt_helper.h"
#endif

double DoChiSquareT13(double x, void *dummy);
double DoChiSquareMH(double x, void *dummy);
double DoChiSquareMH_deg(double x, void *dummy);
double DoChiSquareCPRSVL(double x, void *dummy);
double DoChiSquareNP(double x, void *dummy);
void ComputeResolutionCurve(double osc[]);
void ComputeCPResolutionCurveSVL(double osc[]);
void ComputeNonZeroT13(double osc[]);
void ComputeCPVSigFracCurve(double osc[]);
void ComputeCPBubblesCurve(double osc[]);
void ComputeOctCurve(double osc[]);
void ComputeNMQCurve(double osc[]);
void ComputeCPResolutionCurve2SVL(double osc[]);
void ComputeMHSigCurve_glbchiAll(double osc[]);
void ComputeMHSigCurveSVL(double osc[]);
void ComputeMHSigCurve_bruteForce(double osc[]);
void ComputeMHSensitivityCurve(double osc[]);
void ComputeNPResCurve(double osc[], double in_xup_hi, double deviation, double in_xdown_lo);
void ComputeT13SensitivityCurve(double osc[]);

