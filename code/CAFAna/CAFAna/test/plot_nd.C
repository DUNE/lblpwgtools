#ifdef __CINT__
void plot_nd(){}
#else

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Cuts/TruthCuts.h"
using namespace ana;

#include "Utilities/rootlogon.C"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TH1.h"

void plot_nd()
{
  rootlogon(); // style

  SpectrumLoader loaderFHC("/dune/data/users/marshalc/NDTF_FGT_FHC.root");
  SpectrumLoader loaderRHC("/dune/data/users/marshalc/NDTF_FGT_RHC.root");

  // TODO - this whole dance isn't really necessary for the saner ND
  // files. Work on SpectrumLoader so that it can just count all the POT in
  // this case without the intermediaries.
  auto* loaderFHCPOT = loaderFHC.LoaderForRunPOT(1);
  auto* loaderRHCPOT = loaderRHC.LoaderForRunPOT(1);

  const Var kRecoE = SIMPLEVAR(dune.Ev_reco);
  const Var kPID = SIMPLEVAR(dune.mvaresult);

  const HistAxis axis("Reconstructed energy (GeV)",
                      Binning::Simple(40, 0, 10),
                      kRecoE);

  const HistAxis axisPID("mvaresult",
                         Binning::Simple(30, -1.5, +1.5),
                         kPID);

  Spectrum sPIDFHCNumu(*loaderFHCPOT, axisPID, kIsNumuCC);
  Spectrum sPIDFHCNC(*loaderFHCPOT, axisPID, kIsNC);
  Spectrum sPIDFHCNue(*loaderFHCPOT, axisPID, kIsBeamNue);
  
  Spectrum sRecoEFHCNumuSelTot(*loaderFHCPOT, axis, kPID > 0.5);
  Spectrum sRecoEFHCNumuSelNumu(*loaderFHCPOT, axis, kIsNumuCC && kPID > 0.5);
  Spectrum sRecoEFHCNumuSelNC(*loaderFHCPOT, axis, kIsNC && kPID > 0.5);
  Spectrum sRecoEFHCNumuSelNue(*loaderFHCPOT, axis, kIsBeamNue && kPID > 0.5);

  Spectrum sRecoEFHCNueSelTot(*loaderFHCPOT, axis, kPID < -0.5);
  Spectrum sRecoEFHCNueSelNumu(*loaderFHCPOT, axis, kIsNumuCC && kPID < -0.5);
  Spectrum sRecoEFHCNueSelNC(*loaderFHCPOT, axis, kIsNC && kPID < -0.5);
  Spectrum sRecoEFHCNueSelNue(*loaderFHCPOT, axis, kIsBeamNue && kPID < -0.5);

  loaderFHC.Go();

  new TCanvas;
  sPIDFHCNumu.ToTH1(1.47e21, kRed)->Draw("hist");
  sPIDFHCNC.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sPIDFHCNue.ToTH1(1.47e21, kMagenta)->Draw("hist same");

  new TCanvas;
  sRecoEFHCNumuSelTot.ToTH1(1.47e21)->Draw("hist");
  sRecoEFHCNumuSelNumu.ToTH1(1.47e21, kRed)->Draw("hist same");
  sRecoEFHCNumuSelNC.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sRecoEFHCNumuSelNue.ToTH1(1.47e21, kMagenta)->Draw("hist same");

  new TCanvas;
  sRecoEFHCNueSelTot.ToTH1(1.47e21)->Draw("hist");
  sRecoEFHCNueSelNumu.ToTH1(1.47e21, kRed)->Draw("hist same");
  sRecoEFHCNueSelNC.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sRecoEFHCNueSelNue.ToTH1(1.47e21, kMagenta)->Draw("hist same");

}

#endif
