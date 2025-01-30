#ifndef FITQUNCONVERTER_H
#define FITQUNCONVERTER_H

#include <vector>
#include <functional>
#include <iostream>

// Constants for particle masses
const float MASS_ELECTRON = 0.000511; // GeV/c^2
const float MASS_MUON = 0.10566;      // GeV/c^2
const float MASS_PION = 0.13957;      // GeV/c^2
const float MASS_PROTON = 0.93827;    // GeV/c^2
const float MASS_NEUTRON = 0.93957;   // GeV/c^2

// Structs for particle data
struct ParticleCounts {
    int nP;       // Number of protons
    int nN;       // Number of neutrons
    int nipi0;    // Number of π0
    int nipip;    // Number of π+
    int nipim;    // Number of π−
    int niem;     // Number of electromagnetic particles
};

struct ParticleEnergies {
    float eP;      // Energy of outgoing protons (GeV)
    float eN;      // Energy of outgoing neutrons (GeV)
    float ePip;    // Energy of outgoing π+ (GeV)
    float ePim;    // Energy of outgoing π− (GeV)
    float ePi0;    // Energy of outgoing π0 (GeV)
    float eOther;  // Energy of other outgoing particles (GeV)
};

struct TrueMomentums {
    float NuMomX;        // Neutrino x-component momentum
    float NuMomY;        // Neutrino y-component momentum
    float NuMomZ;        // Neutrino z-component momentum
    float LepMomX;       // Lepton x-component momentum
    float LepMomY;       // Lepton y-component momentum
    float LepMomZ;       // Lepton z-component momentum
    float LepE;          // Lepton energy
    float LepNuAngle;    // Angle between neutrino and lepton
};

struct VertexCoordinates {
    float vtx_x; // True x-coordinate of the interaction vertex (cm)
    float vtx_y; // True y-coordinate of the interaction vertex (cm)
    float vtx_z; // True z-coordinate of the interaction vertex (cm)
};

// Main FitqunConverter class
class FitqunConverter {
public:
    // Reconstruction functions
    static float GetEvReco(int fqmrpcflg[], int fqmrnring, float fqmrmom[][6], int fqmrpid[][6]);
    static float GetEvRecoNue(int fqnmrfit, int fqmrpcflg[], int fqmrnring[], int fqmrpid[][6], float fqmrmom[][6]);
    static float GetEvRecoNumu(int fqnmrfit, int fqmrpcflg[], int fqmrnring[], int fqmrpid[][6], float fqmrmom[][6]);
    static float GetElepReco(int fqmrpcflg[], int fqmrnring, int fqmrpid[6], float fqmrmom[6]);
    static float GetThetaReco(int fqmrpcflg[], int fqmrnring, int fqmrpid[6], float fqmrdir[6][3], float fqmrmom[6]);
    static float GetRecoLepEnNue(int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]);
    static float GetRecoHadEnNue(float totalRecoEnergy, int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]);
    static float GetRecoLepEnNumu(int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]);
    static float GetRecoHadEnNumu(float totalRecoEnergy, int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]);

    static float CalculateTrueThetaAngle(float dirnu[][50], int neutrinoIndex, int leptonIndex);

    static int CalculatePID(int fqmrpcflg[], int fqmrnring, int fqmrpid[6], float fqmrmom[6]);
    static int ConvertInteractionMode(int mode);

    // True energy and PDG functions
    static float GetEvTrue(float pnu[10]);
    static float GetElepTrue(float pnu[10]);
    static int GetNuPDG(int ipnu[]);
    static int GetLepPDG(int ipnu[]);

    // GENIE interaction mode
    static int GetIsCC(int mode);

    // Outgoing particle functions
    static ParticleCounts CountOutgoingParticles(int numnu, const int ipnu[]);
    static ParticleEnergies CalculateParticleEnergies(int numnu, const int ipnu[], const float pnu[]);

    // Kinematics calculations
    static float GetQ2(float lepEnergy, float nuEnergy, float lepAngle);
    static float GetW(float nuEnergy, float lepEnergy, float lepAngle);
    static float GetX(float Q2, float nuEnergy, float lepEnergy);
    static float GetY(float nuEnergy, float lepEnergy);

    // True momentum calculations
    static TrueMomentums CalculateTrueMomentums(int numnu, const float pnu[], const float dirnu[][50], const int ipnu[]);

    // Vertex coordinates
    static VertexCoordinates GetVertexCoordinates(const float vertex[]);

    // Fixed values
    static int GetRunNumber();
    static int GetIsFD();
    static int GetIsFHC();

    // Default value
    static float GetDefaultValue();

    // Containment assumption
    static int IsLongestTrackContained();
};

#endif // FITQUNCONVERTER_H

