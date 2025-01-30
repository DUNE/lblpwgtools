#include "FitqunConverter.h"
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

// Function to calculate energy from momentum and PID
float ConvertMomentumToEnergy(float momentum, int pid) {
    float mass = 0.0;
    switch (pid) {
        case 1:  mass = MASS_ELECTRON; break;  // Electron
        case 2:  mass = MASS_MUON; break;     // Muon
        case 3:  mass = MASS_PION; break;     // Pion
        case 4:  mass = MASS_PROTON; break;   // Proton
        case 5:  mass = MASS_NEUTRON; break;  // Neutron
        default: mass = 0.0; break;           // Unknown
    }
    return std::sqrt(momentum * momentum + mass * mass);
}

// 1. Total reconstructed neutrino energy
float FitqunConverter::GetEvReco(int fqmrpcflg[], int fqmrnring, float fqmrmom[][6], int fqmrpid[][6]) {
    if (fqmrpcflg[0] < 0) return -999.0; // Ensure the best-fit attempt converged

    float totalEnergy = 0.0;
    for (int i = 0; i < fqmrnring; ++i) {
        totalEnergy += ConvertMomentumToEnergy(fqmrmom[0][i]/1000., fqmrpid[0][i]);
    }
    return totalEnergy;
}

// 2. Reconstructed energy assuming a νe interaction
float FitqunConverter::GetEvRecoNue(int fqnmrfit, int fqmrpcflg[], int fqmrnring[], int fqmrpid[][6], float fqmrmom[][6]) {
    float Ev_reco_nue = -999.0;
    for (int fitIdx = 0; fitIdx < fqnmrfit; ++fitIdx) {
        if (fqmrpcflg[fitIdx] >= 0) { // Ensure fit converged
	    if (fqmrnring[fitIdx] > 10) return Ev_reco_nue;
            int brightestRing = -1;
            float maxEnergy = -1.0;
            for (int ringIdx = 0; ringIdx < fqmrnring[fitIdx]; ++ringIdx) {
                float energy = ConvertMomentumToEnergy(fqmrmom[fitIdx][ringIdx]/1000., fqmrpid[fitIdx][ringIdx]);
                //std::cout<<"fitIdx, ringIdx, fqmrpid, energy "<<fitIdx<<" "<<ringIdx<<" "<<fqmrpid[fitIdx][ringIdx]<<" "<<energy<<std::endl;
                if (energy > maxEnergy) {
                    maxEnergy = energy;
                    brightestRing = ringIdx;
                }
            }
            if (brightestRing != -1 && fqmrpid[fitIdx][brightestRing] == 1) { // Electron
                Ev_reco_nue = maxEnergy;
                break; // Found a reasonable fit with the desired PID
            }
        }
    }
    return Ev_reco_nue;
}

// 3. Reconstructed energy assuming a νμ interaction
float FitqunConverter::GetEvRecoNumu(int fqnmrfit, int fqmrpcflg[], int fqmrnring[], int fqmrpid[][6], float fqmrmom[][6]) {
    float Ev_reco_numu = -999.0;
    for (int fitIdx = 0; fitIdx < fqnmrfit; ++fitIdx) {
        if (fqmrpcflg[fitIdx] >= 0) { // Ensure fit converged
            if (fqmrnring[fitIdx] > 10) return Ev_reco_numu;
            int brightestRing = -1;
            float maxEnergy = -1.0;
            for (int ringIdx = 0; ringIdx < fqmrnring[fitIdx]; ++ringIdx) {
                float energy = ConvertMomentumToEnergy(fqmrmom[fitIdx][ringIdx]/1000., fqmrpid[fitIdx][ringIdx]);
                //std::cout<<"fitIdx, ringIdx, fqmrpid, energy "<<fitIdx<<" "<<ringIdx<<" "<<fqmrpid[fitIdx][ringIdx]<<" "<<energy<<std::endl;
                if (energy > maxEnergy) {
                    maxEnergy = energy;
                    brightestRing = ringIdx;
                }
            }
            if (brightestRing != -1 && fqmrpid[fitIdx][brightestRing] == 2) { // Muon
                Ev_reco_numu = maxEnergy;
                break; // Found a reasonable fit with the desired PID
            }
        }
    }
    return Ev_reco_numu;
}

// 4. Outgoing lepton energy (Brightest valid ring energy) for the best-fit attempt
float FitqunConverter::GetElepReco(int fqmrpcflg[], int fqmrnring, int fqmrpid[6], float fqmrmom[6]) {
    if (fqmrpcflg[0] < 0) return -999.0; // Check if the best-fit attempt converged

    // Create a vector of {energy, ringIndex} pairs for sorting
    std::vector<std::pair<float, int>> energyRingPairs;
    for (int ringIdx = 0; ringIdx < fqmrnring; ++ringIdx) {
        float energy = ConvertMomentumToEnergy(fqmrmom[ringIdx]/1000., fqmrpid[ringIdx]);
        energyRingPairs.emplace_back(energy, ringIdx);
    }

    // Sort rings by energy in descending order
    std::sort(energyRingPairs.begin(), energyRingPairs.end(),
          [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
              return a.first > b.first;
          });

    // Find the first valid lepton (electron or muon) in sorted order
    for (const auto& pair : energyRingPairs) {
      float energy = pair.first;   // Access the energy (first element of the pair)
      int ringIdx = pair.second;   // Access the ring index (second element of the pair)

      if (fqmrpid[ringIdx] == 1 || fqmrpid[ringIdx] == 2) { // Electron or muon
          return energy; // Return the energy of the brightest valid lepton ring
      }
    }

    return -999.0; // No valid lepton ring found
}

// 5. Reconstructed lepton scattering angle for the best-fit attempt
float FitqunConverter::GetThetaReco(int fqmrpcflg[], int fqmrnring, int fqmrpid[6], float fqmrdir[6][3], float fqmrmom[6]) {
    if (fqmrpcflg[0] < 0) return -999.0; // Check if the best-fit attempt converged

    // Create a vector of {energy, ringIndex} pairs for sorting
    std::vector<std::pair<float, int>> energyRingPairs;
    for (int ringIdx = 0; ringIdx < fqmrnring; ++ringIdx) {
        float energy = ConvertMomentumToEnergy(fqmrmom[ringIdx]/1000., fqmrpid[ringIdx]);
        energyRingPairs.emplace_back(energy, ringIdx);
    }

    // Sort rings by energy in descending order
    std::sort(energyRingPairs.begin(), energyRingPairs.end(),
              [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
                  return a.first > b.first;
              });
    
    // Find the first valid lepton (electron or muon) in sorted order
    for (const auto& pair : energyRingPairs) {
      float energy = pair.first;   // Access the energy (first element of the pair)
      int ringIdx = pair.second;   // Access the ring index (second element of the pair)

      if (fqmrpid[ringIdx] == 1 || fqmrpid[ringIdx] == 2) { // Electron or muon
          float dirZ = fqmrdir[ringIdx][2];
          return std::acos(dirZ); // Return the angle of the brightest valid lepton ring
      }
    }


    return -999.0; // No valid lepton ring found
}

// 8. Reconstructed lepton energy for νe
float FitqunConverter::GetRecoLepEnNue(int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]) {
    for (int fitIdx = 0; fitIdx < fqnmrfit; ++fitIdx) {
        if (fqmrpcflg[fitIdx] >= 0 && fqmrpid[fitIdx][0] == 1) {
            return ConvertMomentumToEnergy(fqmrmom[fitIdx][0]/1000., fqmrpid[fitIdx][0]);
        }
    }
    return -999.0;
}

// 9. Reconstructed hadronic energy for νe
float FitqunConverter::GetRecoHadEnNue(float totalRecoEnergy, int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]) {
    float recoLepEnNue = GetRecoLepEnNue(fqnmrfit, fqmrpcflg, fqmrpid, fqmrmom);
    if (recoLepEnNue < 0) return -999.0;
    return totalRecoEnergy - recoLepEnNue;
}

// 10. Reconstructed lepton energy for νμ
float FitqunConverter::GetRecoLepEnNumu(int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]) {
    for (int fitIdx = 0; fitIdx < fqnmrfit; ++fitIdx) {
        if (fqmrpcflg[fitIdx] >= 0 && fqmrpid[fitIdx][0] == 2) {
            return ConvertMomentumToEnergy(fqmrmom[fitIdx][0]/1000., fqmrpid[fitIdx][0]);
        }
    }
    return -999.0;
}

// 11. Reconstructed hadronic energy for νμ
float FitqunConverter::GetRecoHadEnNumu(float totalRecoEnergy, int fqnmrfit, int fqmrpcflg[], int fqmrpid[][6], float fqmrmom[][6]) {
    float recoLepEnNumu = GetRecoLepEnNumu(fqnmrfit, fqmrpcflg, fqmrpid, fqmrmom);
    if (recoLepEnNumu < 0) return -999.0;
    return totalRecoEnergy - recoLepEnNumu;
}


// 12. True neutrino energy
float FitqunConverter::GetEvTrue(float pnu[10]) {
    return pnu[0];
}

// 13. True lepton energy
float FitqunConverter::GetElepTrue(float pnu[10]) {
    return pnu[2];
}

// 14. True neutrino PDG
int FitqunConverter::GetNuPDG(int ipnu[]) {
    return ipnu[0];
}

// 15. True lepton PDG
int FitqunConverter::GetLepPDG(int ipnu[]) {
    return ipnu[2];
}

// 16. GENIE interaction mode
int FitqunConverter::GetIsCC(int mode) {
    return (std::abs(mode)<30 ) ? 1 : 0;
}

// Function to count outgoing particles
ParticleCounts FitqunConverter::CountOutgoingParticles(int numnu, const int ipnu[]) {
    ParticleCounts counts = {0, 0, 0, 0, 0, 0}; // Initialize counts

    // Loop through particles at the neutrino interaction
    for (int i = 0; i < numnu; ++i) {
        switch (ipnu[i]) {
            case 2212: // Proton
                ++counts.nP;
                break;
            case 2112: // Neutron
                ++counts.nN;
                break;
            case 111: // π0
                ++counts.nipi0;
                break;
            case 211: // π+
                ++counts.nipip;
                break;
            case -211: // π−
                ++counts.nipim;
                break;
            case 22: // Photon (γ)
            case 11: // Electron (e−)
            case -11: // Positron (e+)
                ++counts.niem;
                break;
            default:
                // Other particles ignored
                break;
        }
    }

    return counts; // Return the struct containing the counts
}


// 17. Four-momentum transfer squared (Q²)
float FitqunConverter::GetQ2(float lepEnergy, float nuEnergy, float lepAngle) {
    return 2 * nuEnergy * lepEnergy * (1 - std::cos(lepAngle));
}

// 18. Invariant mass of the hadronic system (W)
float FitqunConverter::GetW(float nuEnergy, float lepEnergy, float lepAngle) {
    float Q2 = GetQ2(lepEnergy, nuEnergy, lepAngle);
    if (0.938 * 0.938 + 2 * 0.938 * (nuEnergy - lepEnergy) - Q2 < 0) 
      return 0;
    return std::sqrt(0.938 * 0.938 + 2 * 0.938 * (nuEnergy - lepEnergy) - Q2);
}

// 19. Bjorken x
float FitqunConverter::GetX(float Q2, float nuEnergy, float lepEnergy) {
    return Q2 / (2 * 0.938 * (nuEnergy - lepEnergy));
}

// 20. Inelasticity (Y)
float FitqunConverter::GetY(float nuEnergy, float lepEnergy) {
    return (nuEnergy - lepEnergy) / nuEnergy;
}


// Function to normalize a 3D vector
void NormalizeVector(float& x, float& y, float& z) {
    float magnitude = std::sqrt(x * x + y * y + z * z);
    if (magnitude > 0) {
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }
}

// Function to calculate true neutrino and lepton momenta
TrueMomentums FitqunConverter::CalculateTrueMomentums(int numnu, const float pnu[], const float dirnu[][50], const int ipnu[]) {
    TrueMomentums result = {0, 0, 0, 0, 0, 0, -999, -999}; // Initialize with defaults

    // Define neutrino and lepton indices based on the `numnu` array
    int neutrinoIndex = -1;
    int leptonIndex = -1;

    for (int i = 0; i < numnu; ++i) {
        if (ipnu[i] == 12 || ipnu[i] == -12 || ipnu[i] == 14 || ipnu[i] == -14 || ipnu[i] == 16 || ipnu[i] == -16) {
            neutrinoIndex = i; // Identify the neutrino
        } else if (std::abs(ipnu[i]) == 11 || std::abs(ipnu[i]) == 13 || std::abs(ipnu[i]) == 15) {
            leptonIndex = i; // Identify the lepton
        }
    }

    // Ensure neutrino and lepton indices are found
    if (neutrinoIndex != -1) {
        // Normalize the direction vector for the neutrino
        float dirX = dirnu[0][neutrinoIndex];
        float dirY = dirnu[1][neutrinoIndex];
        float dirZ = dirnu[2][neutrinoIndex];
        NormalizeVector(dirX, dirY, dirZ);

        result.NuMomX = pnu[neutrinoIndex] * dirX;
        result.NuMomY = pnu[neutrinoIndex] * dirY;
        result.NuMomZ = pnu[neutrinoIndex] * dirZ;
    }

    if (leptonIndex != -1) {
        // Normalize the direction vector for the lepton
        float dirX = dirnu[0][leptonIndex];
        float dirY = dirnu[1][leptonIndex];
        float dirZ = dirnu[2][leptonIndex];
        NormalizeVector(dirX, dirY, dirZ);

        result.LepMomX = pnu[leptonIndex] * dirX;
        result.LepMomY = pnu[leptonIndex] * dirY;
        result.LepMomZ = pnu[leptonIndex] * dirZ;

        // Calculate the lepton energy using the lepton mass
        float leptonMass = 0.0;
        if (std::abs(ipnu[leptonIndex]) == 11) { // Electron
            leptonMass = 0.000511;
        } else if (std::abs(ipnu[leptonIndex]) == 13) { // Muon
            leptonMass = 0.10566;
        } else if (std::abs(ipnu[leptonIndex]) == 15) { // Tau
            leptonMass = 1.77686;
        }
        result.LepE = std::sqrt(std::pow(pnu[leptonIndex], 2) + std::pow(leptonMass, 2));
    }

    // Calculate the angle between the neutrino and lepton momenta
    if (neutrinoIndex != -1 && leptonIndex != -1) {
        float dirNuX = dirnu[0][neutrinoIndex];
        float dirNuY = dirnu[1][neutrinoIndex];
        float dirNuZ = dirnu[2][neutrinoIndex];
        NormalizeVector(dirNuX, dirNuY, dirNuZ);

        float dirLepX = dirnu[0][leptonIndex];
        float dirLepY = dirnu[1][leptonIndex];
        float dirLepZ = dirnu[2][leptonIndex];
        NormalizeVector(dirLepX, dirLepY, dirLepZ);

        float dotProduct = dirNuX * dirLepX + dirNuY * dirLepY + dirNuZ * dirLepZ;
        result.LepNuAngle = std::acos(dotProduct);
    }

    return result;
}


// Function to extract vertex coordinates
VertexCoordinates FitqunConverter::GetVertexCoordinates(const float vertex[]) {
    VertexCoordinates result;
    result.vtx_x = 0;
    result.vtx_y = 0;
    result.vtx_z = 0;
    return result;
}

// Function to calculate outgoing particle energies
ParticleEnergies FitqunConverter::CalculateParticleEnergies(int numnu, const int ipnu[], const float pnu[]) {
    ParticleEnergies result = {0, 0, 0, 0, 0, 0}; // Initialize to zero

    for (int i = 0; i < numnu; ++i) {
        float energy = pnu[i]; // Energy is directly given in pnu

        switch (ipnu[i]) {
            case 2212: // Proton
                result.eP += energy;
                break;
            case 2112: // Neutron
                result.eN += energy;
                break;
            case 211: // π+
                result.ePip += energy;
                break;
            case -211: // π−
                result.ePim += energy;
                break;
            case 111: // π0
                result.ePi0 += energy;
                break;
            default: // Other particles
                result.eOther += energy;
                break;
        }
    }

    return result;
}

int FitqunConverter::IsLongestTrackContained() { return 1; }

// 24. Fixed values
int FitqunConverter::GetRunNumber() { return 20000001; }
int FitqunConverter::GetIsFD() { return 1; }
int FitqunConverter::GetIsFHC() { return 999; }

// 25. Default value
float FitqunConverter::GetDefaultValue() { return -999.0; }

float FitqunConverter::CalculateTrueThetaAngle(float dirnu[][50], int neutrinoIndex, int leptonIndex) {
    // Ensure indices are valid
    if (neutrinoIndex < 0 || leptonIndex < 0) {
        return -999.0; // Invalid indices
    }

    // Normalize the direction vectors
    float nuDirMag = std::sqrt(std::pow(dirnu[0][neutrinoIndex], 2) +
                               std::pow(dirnu[1][neutrinoIndex], 2) +
                               std::pow(dirnu[2][neutrinoIndex], 2));
    float lepDirMag = std::sqrt(std::pow(dirnu[0][leptonIndex], 2) +
                                std::pow(dirnu[1][leptonIndex], 2) +
                                std::pow(dirnu[2][leptonIndex], 2));

    if (nuDirMag == 0 || lepDirMag == 0) {
        return -999.0; // Zero magnitude vector
    }

    float nuDirX = dirnu[0][neutrinoIndex] / nuDirMag;
    float nuDirY = dirnu[1][neutrinoIndex] / nuDirMag;
    float nuDirZ = dirnu[2][neutrinoIndex] / nuDirMag;

    float lepDirX = dirnu[0][leptonIndex] / lepDirMag;
    float lepDirY = dirnu[1][leptonIndex] / lepDirMag;
    float lepDirZ = dirnu[2][leptonIndex] / lepDirMag;

    // Dot product between the normalized vectors
    float dotProduct = nuDirX * lepDirX + nuDirY * lepDirY + nuDirZ * lepDirZ;

    // Compute the angle
    return std::acos(dotProduct); // Angle in radians
}

int FitqunConverter::CalculatePID(int fqmrpcflg[], int fqmrnring, int fqmrpid[6], float fqmrmom[6]) {

    if (fqmrpcflg[0] < 0) return -1; // Check if the best-fit attempt converged

    // Create a vector of {energy, ringIndex} pairs for sorting
    std::vector<std::pair<float, int>> energyRingPairs;
    for (int ringIdx = 0; ringIdx < fqmrnring; ++ringIdx) {
        float energy = ConvertMomentumToEnergy(fqmrmom[ringIdx]/1000., fqmrpid[ringIdx]);
        energyRingPairs.emplace_back(energy, ringIdx);
    }

    // Sort rings by energy in descending order
    std::sort(energyRingPairs.begin(), energyRingPairs.end(),
          [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
              return a.first > b.first;
          });

    // Find the first valid lepton (electron or muon) in sorted order
    for (const auto& pair : energyRingPairs) {
      float energy = pair.first;   // Access the energy (first element of the pair)
      int ringIdx = pair.second;   // Access the ring index (second element of the pair)
        if (fqmrpid[ringIdx] == 1 || fqmrpid[ringIdx] == 2) { // Electron or muon
            return fqmrpid[ringIdx]; // Return the PID of the first valid lepton ring
        }
    }
    return -1; // Return -1 if no valid lepton PID is found
}

int FitqunConverter::ConvertInteractionMode(int mode) {
    // Convert the interaction mode from the neutrino mode to simb definition
    switch (std::abs(mode)) {
        case 1:
            return 0; // kQE
        case 11: case 12: case 13:
            return 1; // kRes
        case 16:
            return 3; // kCoh
        case 21:
            return 2; // kDIS (interpreted as multi-pion production)
        case 27:
            return 11; // kDiffractive
        case 31: case 32: case 33: case 34:
            return 1; // kRes (NC single pi from delta resonance)
        case 36:
            return 3; // kCoh
        case 41:
            return 2; // kDIS (interpreted as multi-pion production)
        case 47:
            return 11; // kDiffractive
        case 51: case 52:
            return 4; // kCohElastic
        default:
            return -1; // kUnknownInteraction
    }
}
