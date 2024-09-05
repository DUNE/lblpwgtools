
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/Navigate.h"
#include "duneanaobj/StandardRecord/SREnums.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

namespace ana{
 class DummySyst: public ISyst
  {
  public:
  DummySyst(const std::string& shortName, const std::string& longName)
      :ISyst(shortName, longName) {}
      virtual void Shift(double, caf::SRInteractionProxy*, double&) const {}
  };

  // declare a particular systematic and names
  // varied the ionization potential (W_ion) parameter in larnd-sim to +/-3 sigma 
  // (to 25.1 and 22.7 eV respectively) from its nominal value (23.6 eV).
  extern const DummySyst kWIonSyst("WIon", "Ionization Potential"); 

  std::vector<int> GetDummyShifts(const ISyst* s)
  {
    // do for each different file syst, some may be one-sided, etc...
    // not sure if should label it as +/- 1 or 3.. anyways it is easily changed
    if (s == &kWIonSyst ) return {-1,0,1};
    else return {0};
  };


// Modify the record of reconstructed muon particles that match with a true muon
// scale by say 5% at 1 sigma.
 class EnergyScaleTrueMu: public ISyst {
 public:
   EnergyScaleTrueMu() : ISyst("EnergyScaleTrueMu", "Muon Energy Scale Syst") {}
   void Shift(double sigma,
              caf::SRInteractionProxy* ixn, double& weight) const override;
 };
  void EnergyScaleTrueMu::Shift(double sigma,
                            caf::SRInteractionProxy* ixn, double& weight) const {
   double scale = .05 * sigma;

   // loop through particles and change record to shift energy
   // only if there is a matched true muon
    for (int i=0; i<ixn->part.ndlp; i++){
      if (! ixn->part.dlp[0].truth.empty()){ 
             // get top level ancestpr of this particle
             const caf::SRProxy * top = ixn->part.Ancestor<caf::SRProxy>();
             if ( abs(ixn->part.dlp[i].pdg)==13 &&   // check if identified as a muon
                  // we scale only if the top match has the same pdg as reco particle
                  (caf::FindParticle(top->mc, ixn->part.dlp[0].truth[0]) ->pdg == ixn->part.dlp[i].pdg) ) 
              {         
                ixn->part.dlp[i].E *= 1. + scale;
              }
            }
    }
 }
 
 const EnergyScaleTrueMu kEnergyScaleTrueMu;


//// lets think about a MEC scaling systematic...
/////// Lets try a systematic that can affect both true and reco:: i.e. MEC flat scale
 class MECScaleSyst : public ISyst
 {
 public: 
   MECScaleSyst(double scale, const std::string shortName, const std::string latexName)
     : ISyst(shortName, latexName), fScale(scale) {}
 
     void Shift( double sigma, caf::SRInteractionProxy* ixn, double& weight) const override;
 
   private:
   double fScale;
 };
 void MECScaleSyst::Shift(double sigma, caf::SRInteractionProxy* ixn, double& weight) const
 {
   if (!ixn->truth.empty()){
      const caf::SRProxy * top = ixn->Ancestor<caf::SRProxy>();
      if ((caf::FindInteraction(top->mc, ixn->truth[0]) )->mode == caf::ScatteringMode::kMEC)
        weight = 1 + sigma * fScale;}
   else 
     weight=1;
 }
 const MECScaleSyst kMECScaleSyst(0.10, "MECScaleSyst", "MEC scale systematic");

}