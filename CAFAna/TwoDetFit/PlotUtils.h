////////////////////////////////////////////////////////////////////////
//  Plot utilities: legends, captions, colors, and alikes             //
//                                                                    //
//  Luiz Prais - praislo@ucmail.uc.edu                                //
////////////////////////////////////////////////////////////////////////

#include "CAFAna/Analysis/Style.h"
#include "CAFAna/Core/rootlogon.C"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"


#include "TCanvas.h"
#include "TMarker.h"
#include "TBox.h"
#include "TLatex.h"
#include "TColor.h"
#include "TGraph.h"
#include "TVectorD.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TGaxis.h"
#include "TChain.h"
#include "TString.h"
#include "TH1.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TH2.h"
#include "TH2F.h"

using namespace ana;

namespace PlotUtils {

//#**************************************#
//#                Colors                #
//#**************************************#

 inline Int_t kFillColor1Sigma;
 inline Int_t kFillColor2Sigma;
 inline Int_t kFillColor3Sigma;
 inline Int_t kFillColor5Sigma;
 inline Int_t kFillColor90;

//#**************************************#
//#            Line Styles               #
//#**************************************#

 inline Int_t kLine1Sigma;
 inline Int_t kLine2Sigma;
 inline Int_t kLine3Sigma;
 inline Int_t kLine5Sigma;
 inline Int_t kLine90CL;

 Int_t kBestFitMark;
 Int_t kBestFitColor;


//#**************************************#
//#     Plot Vars, Titles, Ranges..      #
//#**************************************# 

 TString AxisLabel;

 double Xmin, Xmax;
 double Ymin, Ymax;

 TString Xtitle, Ytitle;

 bool UseLogX = false;
 bool UseLogY = false;

// This handles all the contours colors, and switches between NO and IO style preferences
inline void SetSurfaceStyle(const TString& SurfName) {

 // same line for every mass ordering
 kLine1Sigma = kDashed;
 kLine2Sigma = kSolid;
 kLine3Sigma = kDotted;   
 kLine90CL   = kSolid;

 if (SurfName.Contains("NO")) {
  kBestFitMark     = kFullStar;
  kBestFitColor    = kBlack;

  kFillColor1Sigma = k1SigmaConfidenceColorNH;
  kFillColor2Sigma = k2SigmaConfidenceColorNH;
  kFillColor3Sigma = k3SigmaConfidenceColorNH;
  kFillColor5Sigma = k90PercConfidenceColorNH;
  kFillColor90     = k90PercConfidenceColorNH;
 }
  else{
   kBestFitMark     = kFullStar;
   kBestFitColor    = kBlack;

   kFillColor1Sigma = k1SigmaConfidenceColorIH;
   kFillColor2Sigma = k2SigmaConfidenceColorIH;
   kFillColor3Sigma = k3SigmaConfidenceColorIH;
   kFillColor5Sigma = k90PercConfidenceColorIH;
   kFillColor90     = k90PercConfidenceColorIH;
  }

} // end SetSurfaceStyle

// Here we figure out the variables being plotted
inline void SetSurfaceType(const TString& SurfName) {

 ////////////////////////////////////////////////////////////////////
 //                1D Profiles - Std Osc 3 Flavor                  //
 ////////////////////////////////////////////////////////////////////
 
 if (SurfName.Contains("prof_deltaCP")) {
   Xmin = 0.0, Xmax = 2.0;
   Ymin = 0, Ymax = 10;
   Xtitle = "#delta_{CP}";
   //Ytitle = "Significance ( #sqrt{#Delta#chi^{2}} )";
   Ytitle = "#Delta#chi^{2}";
   //AxisLabel = "Significance ( #sqrt{#Delta#chi^{2}} );sin^{2}(#theta_{24});#Deltam_{41}^{2} (eV^{2})";
}  

if (SurfName.Contains("prof_ssth23")) {
   Xmin = 0.25, Xmax = 0.75;
   Ymin = 0, Ymax = 10;
   Xtitle = "sin^{2}(#theta_{23})";
   //Ytitle = "Significance ( #sqrt{#Delta#chi^{2}} )";
   Ytitle = "#Delta#chi^{2}";
}  

if (SurfName.Contains("prof_dmsq32")) {
   Xmin = 2.4, Xmax = 2.5;
   Ymin = 0, Ymax = 10;
   Xtitle = "#Deltam_{32}^{2} (10^{-3} eV^{2})";
   //Ytitle = "Significance ( #sqrt{#Delta#chi^{2}} )";
   Ytitle = "#Delta#chi^{2}";
   //AxisLabel = "Significance ( #sqrt{#Delta#chi^{2}} );sin^{2}(#theta_{24});#Deltam_{41}^{2} (eV^{2})";
}  

 ////////////////////////////////////////////////////////////////////
 //               2D Surfaces - Std Osc 3 Flavor                   //
 ////////////////////////////////////////////////////////////////////

 //#*****************#
 //#  ssth23_deltaCP #
 //#*****************#
 if (SurfName.Contains("surf_ssth23_deltaCP")) {
  Xmin = 0.0,   Xmax = 2.0;  // deltaCP range
  Ymin = 0.3,  Ymax = 0.7;   // ssth23 range

  AxisLabel = ";#delta_{CP} / #pi;sin^{2}(#theta_{23})";

 }

 //#*****************#
 //#  ssth23_dmsq32  #
 //#*****************#
 if (SurfName.Contains("surf_ssth23_dmsq32")) {
  Xmin = 0.3, Xmax = 0.7;  // ssth23 range
  Ymin = 2.4,  Ymax = 2.5; // dmsq32 range

  AxisLabel = ";sin^{2}(#theta_{23});#Deltam_{32}^{2} (10^{-3} eV^{2})";

 }

 ////////////////////////////////////////////////////////////////////
 //                     2D Surfaces - NSI                          //
 ////////////////////////////////////////////////////////////////////
 
 if (SurfName.Contains("surf_epsemu_deltaemu")) {
   Xmin = 0.0, Xmax = 2.0;
   Ymin = 0.0, Ymax = 0.25;
   AxisLabel = ";#delta_{e#mu} / #pi;#left|#varepsilon_{e#mu}#right|";
}

if (SurfName.Contains("surf_epsetau_deltaetau")) {
   Xmin = 0.0, Xmax = 2.0;
   Ymin = 0.0, Ymax = 0.25;
   AxisLabel = ";#delta_{e#tau} / #pi;#left|#varepsilon_{e#tau}#right|";
}

if (SurfName.Contains("surf_epsmutau_deltamutau")) {
   Xmin = 0.0, Xmax = 2.0;
   Ymin = 0.0, Ymax = 0.60;
   AxisLabel = ";#delta_{#mu#tau} / #pi;#left|#varepsilon_{#mu#tau}#right|";
} 

 ////////////////////////////////////////////////////////////////////
 //                     1D Profiles - Sterile                      //
 ////////////////////////////////////////////////////////////////////
 
 if (SurfName.Contains("prof_ssth24")) {
   Xmin = 1e-4, Xmax = 1.0;
   Ymin = 0, Ymax = 10;
   Xtitle = "sin^{2}(#theta_{24})";
   //Ytitle = "Significance ( #sqrt{#Delta#chi^{2}} )";
   Ytitle = "#Delta#chi^{2}";
   //AxisLabel = "Significance ( #sqrt{#Delta#chi^{2}} );sin^{2}(#theta_{24});#Deltam_{41}^{2} (eV^{2})";

   UseLogX = true;
   UseLogY = false;
}

 ////////////////////////////////////////////////////////////////////
 //                     2D Surfaces - Sterile                      //
 ////////////////////////////////////////////////////////////////////
 
 if (SurfName.Contains("surf_ssth14_dmsq41")) {
   Xmin = 1e-4, Xmax = 1.0;
   Ymin = 1e-4, Ymax = 1e2;
   AxisLabel = ";sin^{2}(#theta_{14});#Deltam_{41}^{2} (eV^{2})";

   UseLogX = true;
   UseLogY = true;
}

 if (SurfName.Contains("surf_ssth24_dmsq41")) {
   Xmin = 1e-5, Xmax = 1.0;
   Ymin = 1e-4, Ymax = 1e2;
   AxisLabel = ";sin^{2}(#theta_{24});#Deltam_{41}^{2} (eV^{2})";

   UseLogX = true;
   UseLogY = true;
}

} // end SetSurfaceType


//#*******************************************************************#
//#              Pre-construct canvas, Draw later                     #      
//#*******************************************************************#

 TCanvas* MakeCanvas() {
  auto Canvas = new TCanvas(ana::UniqueName().c_str());
  Canvas->SetLeftMargin(0.11);
  Canvas->SetBottomMargin(0.13);
  if (UseLogX) Canvas->SetLogx();
  if (UseLogY) Canvas->SetLogy();
  return Canvas;
 }

 void DrawAxis(){
  TH2* Axis = new TH2F();
  Axis = new TH2F("", AxisLabel, 100, Xmin, Xmax, 100, Ymin, Ymax);
  Axis->GetXaxis()->SetTitleSize(.055);
  Axis->GetYaxis()->SetTitleSize(.055);
  Axis->GetXaxis()->SetLabelSize(.04);
  Axis->GetYaxis()->SetLabelSize(.04);
  Axis->GetXaxis()->SetTitleOffset(0.8);
  Axis->GetYaxis()->SetTitleOffset(0.9);
  Axis->GetXaxis()->SetLabelOffset(.005);
  Axis->GetYaxis()->SetLabelOffset(.005);
  Axis->GetXaxis()->CenterTitle();
  Axis->GetYaxis()->CenterTitle();
  Axis->Draw("Axis");
 }
  

 //#*******************************************************************#
 //#                          Legends                                  #
 //#*******************************************************************#
 
 //#***********************#
 //#     Leg for 90CL      #
 //#***********************#
 void DrawLegend90CL(){

   TLegend *Legend90CL = new TLegend(0.13,0.72,0.44,0.90,NULL,"brNDC");
            Legend90CL->SetNColumns(3);
            Legend90CL->SetMargin(0.6);
            Legend90CL->SetColumnSeparation(0.1);
            Legend90CL->SetTextSize(0.040);
            Legend90CL->SetBorderSize(0);
            Legend90CL->SetLineColor(1);
            Legend90CL->SetLineStyle(1);
            Legend90CL->SetLineWidth(0);
            Legend90CL->SetFillColor(10);
            Legend90CL->SetFillStyle(0);
  
   TLegendEntry *EntryLegend90CL = Legend90CL->AddEntry("NULL","","h");
                 EntryLegend90CL = Legend90CL->AddEntry("h","90% CL","f");
                 EntryLegend90CL->SetLineColor(kFillColor90);
                 EntryLegend90CL->SetLineStyle(kLine90CL);
                 EntryLegend90CL->SetLineWidth(3);
                 EntryLegend90CL->SetTextFont(42);

   Legend90CL->Draw();               

 } // end DrawLegend90CL

 
 //#***********************#
 //#  Leg for 1,2,3 sigmas #
 //#***********************#
 void DrawLegendSigmas(){

   TLegend *LegendSigmas = new TLegend(0.13,0.72,0.44,0.90,NULL,"brNDC");
            LegendSigmas->SetNColumns(3);
            LegendSigmas->SetMargin(0.6);
            LegendSigmas->SetColumnSeparation(0.1);
            LegendSigmas->SetTextSize(0.040);
            LegendSigmas->SetBorderSize(0);
            LegendSigmas->SetLineColor(1);
            LegendSigmas->SetLineStyle(1);
            LegendSigmas->SetLineWidth(0);
            LegendSigmas->SetFillColor(10);
            LegendSigmas->SetFillStyle(0);
  
   TLegendEntry *EntryLegendSigmas = LegendSigmas->AddEntry("NULL","","h");
                 EntryLegendSigmas = LegendSigmas->AddEntry("h","1#sigma","f");
                 EntryLegendSigmas->SetLineColor(kFillColor1Sigma);
                 EntryLegendSigmas->SetLineStyle(kDashed);
                 EntryLegendSigmas->SetLineWidth(3);
                 EntryLegendSigmas->SetTextFont(42);//

                 EntryLegendSigmas = LegendSigmas->AddEntry("h","2#sigma","f");
                 EntryLegendSigmas->SetLineColor(kFillColor2Sigma);
                 EntryLegendSigmas->SetLineStyle(kSolid);
                 EntryLegendSigmas->SetLineWidth(3);
                 EntryLegendSigmas->SetTextFont(42);

                 EntryLegendSigmas = LegendSigmas->AddEntry("h","3#sigma","f");
                 EntryLegendSigmas->SetLineColor(kFillColor3Sigma);
                 EntryLegendSigmas->SetLineStyle(kDotted);
                 EntryLegendSigmas->SetLineWidth(3);
                 EntryLegendSigmas->SetTextFont(42);

   LegendSigmas->Draw();               

 } // end DrawLegendSigmas

 
 
 //#*******************************#
 //#     print NO or IO info       #
 //#*******************************#

 void DrawLegendMassOrdering(const TString& SurfName){

  TLegend *LegendMassOrdering = new TLegend(0.13,0.75,0.36,0.95,NULL,"brNDC");
           LegendMassOrdering->SetTextSize(0.048);
           LegendMassOrdering->SetBorderSize(0);
           LegendMassOrdering->SetLineColor(1);
           LegendMassOrdering->SetLineStyle(1);
           LegendMassOrdering->SetLineWidth(0);
           LegendMassOrdering->SetFillColor(10);
           LegendMassOrdering->SetFillStyle(0);

  // put a big caption with DUNE info
  TString MassOrderingInfo;

   if(SurfName.Contains("NO")){
           MassOrderingInfo = "#bf{DUNE Work In Progress}";  
   }
   if(SurfName.Contains("IO")){
           MassOrderingInfo = "#bf{DUNE Work In Progress}";  
   }

  // now yes, add the info
  TLegendEntry *EntryMassOrdering = LegendMassOrdering->AddEntry("NULL",MassOrderingInfo,"h");
  LegendMassOrdering->Draw();               

 } // end DrawLegendMassOrdering
 
 //#*******************************#
 //#     print Exposure info       #
 //#*******************************#

 void DrawLegendExposure(){

   TLegend *LegendExposure = new TLegend(0.510,0.75,0.860,0.95,NULL,"brNDC");
            LegendExposure->SetTextSize(0.040);
            LegendExposure->SetBorderSize(0);
            LegendExposure->SetLineColor(1);
            LegendExposure->SetLineStyle(1);
            LegendExposure->SetLineWidth(0);
            LegendExposure->SetFillColor(10);
            LegendExposure->SetFillStyle(0);
          
 
   TLegendEntry //*EntryExposure = LegendExposure->AddEntry("NULL","","h");
                *EntryExposure = LegendExposure->AddEntry("NULL","336 kt-MW-years (7 yrs staged)","h");
                 //EntryExposure = LegendExposure->AddEntry("NULL","1.36#times10^{23} POT #nu-beam","h");
                 //EntryExposure = LegendExposure->AddEntry("NULL","1.36#times10^{23} POT #bar{#nu}-beam","h");
                 
   LegendExposure->Draw();

 } // end DrawLegendExposure



 //#*******************************#
 //#       print best fit          #
 //#*******************************#

 void DrawLegendBestFit(const TString& SurfName){
 
  TGraph* Marker = new TGraph();
  Marker->SetMarkerStyle(kBestFitMark);
  Marker->SetMarkerColor(kBestFitColor);
  Marker->SetMarkerSize(1.5);
 
  TLegend *LegendBF = new TLegend(0.120,0.60,0.350,0.70,NULL,"brNDC");
           LegendBF->SetTextSize(0.040);
           LegendBF->SetBorderSize(0);
           LegendBF->SetLineColor(1);
           LegendBF->SetLineStyle(1);
           LegendBF->SetLineWidth(0);
           LegendBF->SetFillColor(10);
           LegendBF->SetFillStyle(0);


  TLegendEntry *EntryBF      = LegendBF->AddEntry(Marker,"Best fit","p");
  TLegendEntry *EntryBFDummy = LegendBF->AddEntry("NULL","","");  // add a null entry just to keep correct styling 

 } // end DrawLegendBestFit

} // namespace PlotUtils