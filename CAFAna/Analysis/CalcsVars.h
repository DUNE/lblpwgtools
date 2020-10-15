#include "TMath.h"

//https://arxiv.org/pdf/1707.02322.pdf
const double kBaseline = 1284.9;     // km
const double kEarthDensity = 2.848;  // g/cm^3

// ====================================================== //
// http://pdg.lbl.gov/2019/tables/rpp2019-sum-leptons.pdf //
// PDG December 2019                                      //
// ====================================================== //
const double kPDGDmsq21 = 7.53e-5;
const double kPDGTh12 = asin(sqrt(0.307));
const double kPDGTh13 = asin(sqrt(2.18e-2));
const double kPDGDmsq32NH = 2.444e-3;
const double kPDGDmsq32IH = -2.55e-3;
const double kPDGTh23NH = asin(sqrt(0.512));
const double kPDGTh23IH= asin(sqrt(0.536));

// ================================== //
// http://www.nu-fit.org/?q=node/177  //
// NuFit November 2018                //
// ================================== //
const double kNuFitDmsq21CV = 7.39e-5;
const double kNuFitTh12CV = 33.82 * TMath::Pi()/180;

// Have to adjust for nu-fit's weird convention in NH
const double kNuFitDmsq32CVNH = +2.525e-3 - kNuFitDmsq21CV;
const double kNuFitTh23CVNH = 49.6 * TMath::Pi()/180;
const double kNuFitTh13CVNH = 8.61 * TMath::Pi()/180;
const double kNuFitdCPCVNH = 215 * TMath::Pi()/180;

const double kNuFitDmsq32CVIH = -2.512e-3;
const double kNuFitTh23CVIH = 49.8 * TMath::Pi()/180;
const double kNuFitTh13CVIH = 8.65 * TMath::Pi()/180;
const double kNuFitdCPCVIH = 284 * TMath::Pi()/180;

//Additional values of oscillation parameters
//Th23 variations
const double kNuFitTh23HiNH = 51.2 * TMath::Pi()/180;
const double kNuFitTh23LoNH = 47.3 * TMath::Pi()/180;
const double kNuFitTh23HiIH = 51.4 * TMath::Pi()/180;
const double kNuFitTh23LoIH = 47.8 * TMath::Pi()/180;
const double kNuFitTh23MM = 45 * TMath::Pi()/180;
const double kNuFitTh23MaxNH = 52.4 * TMath::Pi()/180;
const double kNuFitTh23MaxIH = 52.5 * TMath::Pi()/180;
const double kNuFitTh23MinNH = 40.3 * TMath::Pi()/180;
const double kNuFitTh23MinIH = 40.6 * TMath::Pi()/180;
//Th13 variations
const double kNuFitTh13MaxNH = 8.99 * TMath::Pi()/180;
const double kNuFitTh13MinNH = 8.22 * TMath::Pi()/180;
const double kNuFitTh13MaxIH = 9.03 * TMath::Pi()/180;
const double kNuFitTh13MinIH = 8.27 * TMath::Pi()/180;
//Dmsq32 variations
const double kNuFitDmsq32MaxNH = 2.625e-3 - kNuFitDmsq21CV;
const double kNuFitDmsq32MinNH = 2.427e-3 - kNuFitDmsq21CV;
const double kNuFitDmsq32MaxIH = -2.412e-3;
const double kNuFitDmsq32MinIH = -2.611e-3;

// Based on 1/6 of the +/- 3sigma error
const double kNuFitDmsq21Err = ((8.01-6.79)/6)*1e-5;
const double kNuFitTh12Err = ((36.27-31.61)/6) * TMath::Pi()/180;

const double kNuFitDmsq32ErrNH = ((2.625-2.427)/6)*1e-3;
const double kNuFitTh23ErrNH = ((52.4-40.3)/6) * TMath::Pi()/180;
const double kNuFitTh13ErrNH = ((8.99-8.22)/6) * TMath::Pi()/180;

const double kNuFitDmsq32ErrIH = ((2.611-2.412)/6)*1e-3;
const double kNuFitTh23ErrIH = ((52.5-40.6)/6) * TMath::Pi()/180;
const double kNuFitTh13ErrIH = ((9.03-8.27)/6) * TMath::Pi()/180;
