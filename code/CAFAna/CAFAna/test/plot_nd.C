#ifdef __CINT__
void plot_nd(){}
#else

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Systs/DUNEXSecSysts.h"
#include "CAFAna/Core/SystShifts.h"
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
  const Var kPIDmu = SIMPLEVAR(dune.numu_pid);
  const Var kPIDe = SIMPLEVAR(dune.nue_pid);

  const HistAxis axis("Reconstructed energy (GeV)",
                      Binning::Simple(40, 0, 10),
                      kRecoE);

  const HistAxis axisPIDmu("PIDmu",
                         Binning::Simple(30, -1.5, +1.5),
                         kPIDmu);
  const HistAxis axisPIDe("PIDe",
                         Binning::Simple(30, -1.5, +1.5),
                         kPIDe);

  Spectrum sMuPIDFHCNumu(*loaderFHCPOT, axisPIDmu, kIsNumuCC);
  Spectrum sMuPIDFHCNC(*loaderFHCPOT, axisPIDmu, kIsNC);
  Spectrum sMuPIDFHCNue(*loaderFHCPOT, axisPIDmu, kIsBeamNue);

  Spectrum sEPIDFHCNumu(*loaderFHCPOT, axisPIDe, kIsNumuCC);
  Spectrum sEPIDFHCNC(*loaderFHCPOT, axisPIDe, kIsNC);
  Spectrum sEPIDFHCNue(*loaderFHCPOT, axisPIDe, kIsBeamNue);
  
  Spectrum sRecoEFHCNumuSelTot(*loaderFHCPOT, axis, kPIDmu > 0.5);
  Spectrum sRecoEFHCNumuSelNumu(*loaderFHCPOT, axis, kIsNumuCC && kPIDmu > 0.5);
  Spectrum sRecoEFHCNumuSelNC(*loaderFHCPOT, axis, kIsNC && kPIDmu > 0.5);
  Spectrum sRecoEFHCNumuSelNue(*loaderFHCPOT, axis, kIsBeamNue && kPIDmu > 0.5);

  Spectrum sRecoEFHCNueSelTot(*loaderFHCPOT, axis, kPIDe > 0.5);
  Spectrum sRecoEFHCNueSelNumu(*loaderFHCPOT, axis, kIsNumuCC && kPIDe > 0.5);
  Spectrum sRecoEFHCNueSelNC(*loaderFHCPOT, axis, kIsNC && kPIDe > 0.5);
  Spectrum sRecoEFHCNueSelNue(*loaderFHCPOT, axis, kIsBeamNue && kPIDe > 0.5);

  DUNEXSecSyst shift(k_int_nu_MEC_dummy);
  SystShifts shifts( &shift, +1);
  Spectrum sRecoEFHCNumuSelTotUp(*loaderFHCPOT, axis, kPIDmu > 0.5, shifts);
  Spectrum sRecoEFHCNumuSelNumuUp(*loaderFHCPOT, axis, kIsNumuCC && kPIDmu > 0.5, shifts);
  Spectrum sRecoEFHCNumuSelNCUp(*loaderFHCPOT, axis, kIsNC && kPIDmu > 0.5, shifts);
  Spectrum sRecoEFHCNumuSelNueUp(*loaderFHCPOT, axis, kIsBeamNue && kPIDmu > 0.5, shifts);

  std::vector<Cut> truthcuts;
  for( int i = 0; i < 32; ++i ) {
    truthcuts.push_back( kVALORCategory == i );
  }

  PredictionScaleComp pred(*loaderFHCPOT, 
                            axis,
                            kPIDmu > 0.5,
                            truthcuts);

  loaderFHC.Go();

  new TCanvas;
  sMuPIDFHCNumu.ToTH1(1.47e21, kRed)->Draw("hist");
  sMuPIDFHCNC.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sMuPIDFHCNue.ToTH1(1.47e21, kMagenta)->Draw("hist same");
  gPad->Print( "numu_PID.png" );

  new TCanvas;
  sEPIDFHCNumu.ToTH1(1.47e21, kRed)->Draw("hist");
  sEPIDFHCNC.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sEPIDFHCNue.ToTH1(1.47e21, kMagenta)->Draw("hist same");
  gPad->Print( "nue_PID.png" );

  new TCanvas;
  sRecoEFHCNumuSelTot.ToTH1(1.47e21)->Draw("hist");
  sRecoEFHCNumuSelNumu.ToTH1(1.47e21, kRed)->Draw("hist same");
  sRecoEFHCNumuSelNC.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sRecoEFHCNumuSelNue.ToTH1(1.47e21, kMagenta)->Draw("hist same");
  gPad->Print( "numu_selection.png" );

  new TCanvas;
  sRecoEFHCNueSelTot.ToTH1(1.47e21)->Draw("hist");
  sRecoEFHCNueSelNumu.ToTH1(1.47e21, kRed)->Draw("hist same");
  sRecoEFHCNueSelNC.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sRecoEFHCNueSelNue.ToTH1(1.47e21, kMagenta)->Draw("hist same");
  gPad->Print( "nue_selection.png" );

  new TCanvas;
  sRecoEFHCNumuSelTotUp.ToTH1(1.47e21)->Draw("hist");
  sRecoEFHCNumuSelNumuUp.ToTH1(1.47e21, kRed)->Draw("hist same");
  sRecoEFHCNumuSelNCUp.ToTH1(1.47e21, kBlue)->Draw("hist same");
  sRecoEFHCNumuSelNueUp.ToTH1(1.47e21, kMagenta)->Draw("hist same");
  gPad->Print( "numu_selection_shift.png" );



}

#endif
