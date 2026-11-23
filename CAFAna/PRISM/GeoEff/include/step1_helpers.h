#include <iostream>
#include <fstream>
#include <algorithm>    // std::max
#include <stdlib.h>
#include <iomanip>
#include "TSystemDirectory.h"

bool verbose = false; // default to false, true for debugging, a lot of printouts
bool myfileVerbose = false; // change to true if we need check data
bool throwfileVerbose = false;
bool hadronhitVerbose = false;
bool ntupleVerbose = false;

unsigned long N_throws = 64*64; // Use multiple of 64

// Active volume for FD: based on ntuple hadron hit x/y/z histogram
float FDActiveVol_min[] = {-370., -600.,    0.};
float FDActiveVol_max[] = {370.,   600., 1400.};
// float FDActiveVol_min[] = {-370., -601.,    -1.};
// float FDActiveVol_max[] = {370.,   601., 1400.};

// Active volume for ND
float NDActiveVol_min[] = {-350., -150.,   0.};
float NDActiveVol_max[] = { 350.,  150., 500.};
float NDLAr_OnAxis_offset[]    = { 0.,     5.5, 411.0}; // this offset is only for ND MC when On-Axis, use 0 for FD MC

// Fiducial volume for ND
//float ND_FV_min[] = {-300., -100., 50.};
//float ND_FV_max[] = { 300.,  100., 350.};
//preprocessed CAFs for PRISM have NDFV between -200, 200 in vtxX
float ND_FV_min[] = {-200., -100., 50.};
float ND_FV_max[] = { 200.,  100., 350.};

// Fiducial volume for FD ( minus the same amount from FDActiveVol)
// float FD_FV_min[] = {-240., -470., 130.};
// float FD_FV_max[] = { 240.,  470., 1170.};
// FV: 99% pass Eveto cut
//float FD_FV_min[] = {-145., -375., 100.};
//float FD_FV_max[] = { 145., 375., 960.};
//Allison Larger FV -> Match what is used in NearToFar
float FD_FV_min[] = {-310., -550.,   50.};
float FD_FV_max[] = { 310.,  550., 1244.};
// float FD_FV_min[] = {-320., -550., 50.};
// float FD_FV_max[] = { 320.,  550., 1250.};


bool random_ND_off_axis_pos     = false; // Set to true will only use a random ND off axis position per event in runGeoEffFDEvtSim
double ND_off_axis_pos_stepsize = 2.5;   // unit meters
// Average neutrino decay position in beam coordinates as a function of off axis x (from Luke)
// This is used to interpolate the decay position
double meanPDPZ[]       = {57.7352, 59.1433, 60.8336, 62.4821, 65.005, 67.5822, 69.8348, 73.0865, 76.6664, 81.1443, 85.6266, 90.346, 93.362, 93.6072, 93.362,  90.346, 85.6266, 81.1443, 76.6664, 73.0865, 69.8348, 67.5822, 65.005, 62.4821, 60.8336, 59.1433, 57.7352}; // unit: meters
double OffAxisPoints[]  = {-30.5,   -28,    -25.5,   -23,     -20.5,   -18,     -15.5,   -13,    -10.5,   -8,      -5.5,    -3,    -0.5,     0,       0.5,    3,     5.5,    8,      10.5,   13,     15.5,   18,     20.5,  23,     25.5,   28,     30.5};
double NDLarPos_new1[]  = {0., -28.}; // unit meter
double NDLarPos_new2[]  = {-1.75, -25.75}; // unit meter
double ND_Lar_dtctr_pos_new_stepsize = 14; // unit meter

bool random_vtx_vx         = false; // Set to true will only use a random vtx x per event in runGeoEffFDEvtSim
double ND_local_x_stepsize = 48.;   // unit cm, must be a positive number below 200
double ND_local_x_min      = -312.;
double ND_local_x_max      = 312.;

namespace FDEffCalc_cfg {

  // This ND FV cut function is copied from CAFAna:
  // https://github.com/DUNE/lblpwgtools/blob/master/CAFAna/Cuts/TruthCuts.h#L65-L98
  // To confirm: 1.6cm or 1.3cm dead region b/t modules in x?
  // Function for on-axis!!! geo Eff
    inline bool IsInNDFV(double pos_x_cm, double pos_y_cm, double pos_z_cm) {
    bool inDeadRegion = false;
    for (int i = -3; i <= 3; ++i) {
      // 0.5cm cathode in the middle of each module, plus 0.5cm buffer
      double cathode_center = i * 102.1;
      if (pos_x_cm > cathode_center - 0.75 && pos_x_cm < cathode_center + 0.75)
        inDeadRegion = true;

      // 1.6cm dead region between modules (0.5cm module wall and 0.3cm pixel
      // plane, x2) don't worry about outer boundary because events are only
      // generated in active Ar + insides
      double module_boundary = i * 102.1 + 51.05;
      if (i <= 2 && pos_x_cm > module_boundary - 1.3 &&
          pos_x_cm < module_boundary + 1.3)
        inDeadRegion = true;
    }
    for (int i = 1; i <= 4; ++i) {
      // module boundaries in z are 1.8cm (0.4cm ArCLight plane + 0.5cm module
      // wall, x2) module is 102.1cm wide, but only 101.8cm long due to cathode
      // (0.5cm) being absent in length but ArCLight is 0.1cm thicker than pixel
      // plane so it's 0.3cm difference positions are off-set by 0.6 because I
      // defined 0 to be the upstream edge based on the active volume by
      // inspecting a plot, and aparently missed by 3 mm, but whatever add 8mm =
      // 2 pad buffer due to worse position resolution in spatial dimension z
      // compared to timing direction x so total FV gap will be 1.8 + 2*0.8
      // = 3.4cm
      double module_boundary = i * 101.8 - 0.6;
      if (pos_z_cm > module_boundary - 1.7 && pos_z_cm < module_boundary + 1.7)
        inDeadRegion = true;
    }
    // return fiducial volume
    return (abs(pos_x_cm) < ND_FV_max[0] && abs(pos_y_cm) < ND_FV_max[1] && pos_z_cm > ND_FV_min[2] &&
            pos_z_cm < ND_FV_max[2] && !inDeadRegion);
  } // end ND FV cut

} // end namespace
