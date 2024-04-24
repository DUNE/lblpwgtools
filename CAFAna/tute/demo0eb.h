#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/ISyst.h"
#include "TVector3.h"

namespace ana{

//-------------- Two examples dummy systematics

// Setup a dummy systematic that shifts Particle (Muon) Enegy by a +/-  percent

 class EnergyScaleMu: public ISyst {
 public:
   EnergyScaleMu() : ISyst("EnergyScaleMu", "Muon Energy Scale Syst") {}
   void Shift(double sigma,
              caf::SRInteractionProxy* ixn, double& weight) const override;
 };
  void EnergyScaleMu::Shift(double sigma,
                            caf::SRInteractionProxy* ixn, double& weight) const {
   double scale = .05 * sigma;

   // loop through particles and change record to shift energy 
    for (int i=0; i<ixn->part.ndlp; i++){
       if ( abs(ixn->part.dlp[i].pdg)==13) {
          ixn->part.dlp[i].E *= 1. + scale;
        }
    }
 }
 
 const EnergyScaleMu kEnergyScaleMu;

//----------------------------------------------------------------------
//// A second dummy syst to have a multiverse with multiple systs
// A reweight systematic... multiply times [x] all events that pass [ some ] criteria
  class MuonRwgtSyst : public ISyst
  {
  public :
    MuonRwgtSyst(double scale, const std::string shortName, const std::string latexName)
       : ISyst( shortName , latexName ), fScale(scale) {}

     void Shift( double sigma,  caf::SRInteractionProxy* ixn, double& weight ) const override;
    
  private:
    double fScale;

  };
  void MuonRwgtSyst::Shift( double sigma, caf::SRInteractionProxy* ixn, double& weight ) const
   {
    bool hasMuon = false;
   // loop through particles see if theres a muon
    for (int i=0; i<ixn->part.ndlp; i++){
       if ( abs(ixn->part.dlp[i].pdg)==13) {
           hasMuon= true;
        }
    }
    if (hasMuon){
        weight = 1 + sigma * fScale; 
        //std::cout<< " Found a Muon, wgt = "<<weight<<std::endl;
      }
     else
      weight = 1;
  }
  const MuonRwgtSyst kMuonRwgtSyst(0.10, "MuonRwgtSyst", "Muon weight Scale Syst" );

//-------------- ----------------------------------------------------------------------

// Some cuts and vars used in example

  
// Var that takes energy of the most energetic muon in interaction
const Var kEnergyInteractionMu([](const caf::SRInteractionProxy* ixn)
  { 
      double e = -5.;

      int numMuons = 0;
      for (int i=0; i<ixn->part.ndlp; i++){
        // check if muon
        //std::cout<< "we are in particle "<< i<<std::endl;
        if ( abs(ixn->part.dlp[i].pdg)==13) {
          numMuons ++;          
          //this muons energy
          double tempE = ixn->part.dlp[i].E;
          //std::cout<< "found muon # "<< numMuons<< ", idx "<<i<<" with energy "<< tempE<<std::endl;
          // in case there is another muon,           
          if( numMuons>1 ){
            // check which one has more energy
            if (tempE>e)
              e=tempE; // store if this muon has more energy
          }else{ e = tempE;}
        }
                    
      }              
      return e;     
  });

// Var that sums up Energy of all tracks
// Var that takes energy of the most energetic muon in interaction
const Var kEnergyInteractionAll([](const caf::SRInteractionProxy* ixn)
  { 
      double e = 0.;
      for (int i=0; i<ixn->part.ndlp; i++){
          double tempE = ixn->part.dlp[i].E;  
          e = e + tempE;                              
      }// in the weird case interaction had nothing??              
      //if (e == 0) return -5.;
      return e;     
  });
// Var that takes energy of the most energetic muon in interaction
const Var kEnergyInteractionButMu([](const caf::SRInteractionProxy* ixn)
  { 
      return    kEnergyInteractionAll(ixn) - kEnergyInteractionMu(ixn);
  });


  // A simple selection cut at the level of vertices: i.e. containment
  const Cut kContainment([](const caf::SRInteractionProxy* ixn)
                      {

                        double x = ixn->vtx.x;
                        double y = ixn->vtx.y;
                        double z = ixn->vtx.z;
                        bool cont =  abs(x)<50 && 
                                     abs(x)>10 && 
                                     abs(y)<50 &&
                                     abs(z)>10 && 
                                     abs(z)<50 ;
                        return cont;
                      });

// Has only one muon 
  const Cut kHas1RecoMuon([](const caf::SRInteractionProxy* ixn)
                      {
                        int nMuons= 0;
                        for (int i=0; i<ixn->part.ndlp; i++){
                           if ( abs(ixn->part.dlp[i].pdg)==13) {
                               nMuons++;
                            }
                        }
                        return (nMuons==1);
                      });


 const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
 // Select particles that have a pdg of muon 
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == 13 ;
                      });

}