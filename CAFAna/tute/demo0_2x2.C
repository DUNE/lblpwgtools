// Make a simple spectrum plot
// cafe demo0.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/MultiVar.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TPad.h"
#include "TFile.h"
using namespace ana;

typedef _HistAxis<MultiVar> MultiVarHistAxis;


void demo0_2x2()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/minirun4/notruth/outputCAF_notruth_*";
  //"/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_FHC_nonswap.root";

  // Source of events
  SpectrumLoader loader(fname);

  // A Var is a little snippet of code that takes a record representing the
  // event record and returns a single number to plot.
  const Var kTrueEnergy([](const caf::SRProxy* sr)
                        {
                          return sr->mc.nu[0].E;
                        });


// number Particles reconstructed by DeepLearnPhysics machine learning stack
  const Var kNumberNDLP([](const caf::SRProxy* sr)
                        {
                          //std::cout<< "Found "<< sr->common.ixn.ndlp <<" interactions" << std::endl;
                          return sr->common.ixn.ndlp;
                        });// = SIMPLEVAR(common.ixn.ndlp);

  const Var kIxnVtxX([](const caf::SRProxy* sr)
                        {
                          //std::cout<< "Found "<< sr->common.ixn.ndlp <<" interactions" << std::endl;
                          if (sr->common.ixn.ndlp>0){
                            return (float)sr->common.ixn.dlp[0].vtx.x;
                          } 
                          return -1.f;
                        });// = SIMPLEVAR(common.ixn.ndlp);

  const MultiVar kAllIxnVtxX([](const caf::SRProxy* sr){

      std::vector<double> vtxs = {-5000.};
       if (sr->common.ixn.ndlp>0){
        for (int i=0; i<sr->common.ixn.ndlp; i++){
          vtxs.push_back(sr->common.ixn.dlp[i].vtx.x);
        }
       }
      
      return vtxs; 

  });


  // For such a simple variable you can use a shortcut like this
  // const Var kCVNNumu = SIMPLEVAR(cvnnumu);

  // Define a spectrum, ie a histogram with associated POT information
  const Binning binsNDLP = Binning::Simple(10, 0, 10);
  const Binning binsvtx = Binning::Simple(100, -100., 100);

  const HistAxis axNDLP("Number of LP", binsNDLP, kNumberNDLP);
  const HistAxis axVtx("Vertex X Position for 1st ixn (VAR)", binsvtx, kIxnVtxX);
  const MultiVarHistAxis axAllVtx("Vertex X Position for all ixn (MULTIVAR)", binsvtx, kAllIxnVtxX);
  // kIsNumuCC here is a "Cut". Same as a Var but returning a boolean. In this
  // case, we're only keeping events that are truly numu CC interactions.

  Spectrum sNDLP(loader, axNDLP, kNoCut);
  Spectrum sVtxX(loader, axVtx, kNoCut);
  Spectrum sAllVtxX(loader, axAllVtx, kNoCut);

//  Spectrum sEnergyNC(loader, axEnergy, kIsNC);


  // This is the call that actually fills in those spectra
  loader.Go();
  std::cout<< sNDLP.POT()<< " 1st POT\n";
  std::cout<< "Overriding POT\n";
  std::cout<< sVtxX.POT()<< " 2nd POT\n";
  std::cout<< "Overriding POT\n";

  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  sNDLP.OverridePOT(pot);
   sVtxX.OverridePOT(pot);
   sAllVtxX.OverridePOT(pot);
  std::cout<< sNDLP.POT()<< " POT\n";

  std::string out_file= "first_2x2_plots.root";
  std::cout<<" Save spectra in " <<out_file;

  TFile *file_o = new TFile(out_file.c_str(),"Recreate");
  // save Spectrum objs and norm hists for quick view

  sNDLP.SaveTo(file_o,"sNDLP");
  sVtxX.SaveTo(file_o,"sVtxX");
  sAllVtxX.SaveTo(file_o,"sAllVtxX");

  file_o->Print();
  file_o->Close(); 


  // For plotting purposes we can convert to TH1s
  new TCanvas;
  sNDLP.ToTH1(pot, kBlue)->Draw("hist");

  new TCanvas;
  sVtxX.ToTH1(pot, kBlue)->Draw("hist");

  new TCanvas;
  sAllVtxX.ToTH1(pot, kBlue)->Draw("hist");


}
