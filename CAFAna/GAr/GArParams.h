#pragma once

#include <vector>

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                           Useful numerical values                          */
    /* -------------------------------------------------------------------------- */

    // Centre of the HPgTPC, in the ND hall coordinate system
    const double kHPgTPCX = 0.0;    // cm
    const double kHPgTPCY = -150.0; // cm
    const double kHPgTPCZ = 1486.0; // cm

    // Radius and half lenght of the HPgTPC
    const double kHPgTPCRadius = 277.02; // cm
    const double kHPgTPCLength = 259.00; // cm

    // Standard HPgTPC fiducial volume (FV) cuts (from the ND CDR)
    const double kHPgTPCFidCutR = 50.0; // cm
    const double kHPgTPCFidCutX = 30.0; // cm

    // Radius and half lenght of the HPgTPC FV (ND CDR standard)
    const double kHPgTPCFidRadius = kHPgTPCRadius - kHPgTPCFidCutR;
    const double kHPgTPCFidLength = kHPgTPCLength - kHPgTPCFidCutX;

    // Centre positions of left and right HPgTPC faces
    const std::vector<double> kHPgTPCLeftFace  = {kHPgTPCX-kHPgTPCLength, kHPgTPCY, kHPgTPCZ};
    const std::vector<double> kHPgTPCRightFace = {kHPgTPCX+kHPgTPCLength, kHPgTPCY, kHPgTPCZ};

    // Centre positions of left and right HPgTPC FV faces (ND CDR standard)
    const std::vector<double> kHPgTPCFidLeftFace  = {kHPgTPCX-kHPgTPCFidLength, kHPgTPCY, kHPgTPCZ};
    const std::vector<double> kHPgTPCFidRightFace = {kHPgTPCX+kHPgTPCFidLength, kHPgTPCY, kHPgTPCZ};

    // Masses of the particles
    const double kMassElectron     =   0.511*1e-3; // GeV/c2
    const double kMassMuon         = 105.658*1e-3; // GeV/c2
    const double kMassNeutralPion  = 134.977*1e-3; // GeV/c2
    const double kMassPion         = 139.570*1e-3; // GeV/c2
    const double kMassKaon         = 493.677*1e-3; // GeV/c2
    const double kMassProton       = 938.272*1e-3; // GeV/c2

    // Single nucleon separation energies for Ar40
    const double kSeparationEProton  = 12.52*1e-3; // GeV
    const double kSeparationENeutron = 9.869*1e-3; // GeV

    /* ----------------------- ALEPH dE/dx parametrisation ---------------------- */
    const float kALEPHp1 = 3.30;
    const float kALEPHp2 = 8.80;
    const float kALEPHp3 = 0.27;
    const float kALEPHp4 = 0.75;
    const float kALEPHp5 = 0.82;

}