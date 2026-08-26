////////////////////////////////////////////////////////////////////////
//  Script to read a root file and plot a pdf w/ frequentist contours //
//                                                                    //
//  Luiz Prais - praislo@ucmail.uc.edu                                //
////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include "CAFAna/Core/LoadFromFile.h"
#include "PlotUtils.h"

using namespace ana;
using namespace PlotUtils;

using std::string;
using std::vector;

void PlotTwoDetFit( std::string FileInput   = "",     // full/path/to/file
                    std::string FileInput2  = "",     // full/path/to/file (optional, to compare w/ FileInput)
                    std::string FileInput3  = ""
                       
)

{

 // this really can be improved, but not now.... 
 bool UnderstoodCL = false; 
 std::string PlotCL  = "90CL"; // "1D" or "90CL" or "123Sigmas" 
 
 if(FileInput.find("prof") != std::string::npos){
  PlotCL  = "1D";
 }

 if(PlotCL == "1D" || PlotCL == "90CL" || PlotCL == "123Sigmas"){
  UnderstoodCL = true;
 }

 if(!UnderstoodCL){
  std::cout << "\nCouldn't select between 1D or 90CL or 123Sigmas" << std::endl;
  exit(1);
 }

 // read a file, and use that latter for plot name
 if(FileInput.find(".root") != std::string::npos){
  FileInput.erase(FileInput.length() - 5);
 }

 // Read the file name and use that to pass the plotting style
 TString SurfName = FileInput;
 PlotUtils::SetSurfaceStyle(SurfName);
 PlotUtils::SetSurfaceType(SurfName); 

 auto Canvas = MakeCanvas();

 PlotUtils::DrawAxis();


 //#****************************************************#
 //#                  inputs & outputs                  #
 //#****************************************************#  

 // create here a folder to store plots
 TString FileDir      = "plots/";
 gSystem->MakeDirectory(FileDir);
 TString PlotOutput  = FileDir+FileInput;

 TString Input;
 TFile *File;


//#*************************************************#
//#                                                 #
//#                  Plotting!                      #
//#                                                 #
//#*************************************************#


 std::cout << "\nLoading input files" << std::endl;

 Input        = FileInput + ".root";
 File         = new TFile (Input,"read");
 auto surface = *FrequentistSurface::LoadFrom(File, ("surface"));

 //#****************************************#
 //#                                        #
 //#              1D PROFILES               #
 //#                                        #
 //#****************************************#

 TGraph* profile;

 if(PlotCL=="1D"){
  std::cout << "\nPlotting the 1D profile..." << std::endl;

  std::cout << "\nStarting the process of converting the FitVarForProf1D 'Surface' into a 1D Profile..." << std::endl;

  auto hist = surface.ToTH2(surface.BestLikelihood());

  profile = MakeTGraphFromSurface(hist, surface.GetBestFitX());

  // This is ToDo
  //std::cout << "\nNow taking the sqrt() of the profiles for a significance curve in sigmas..\n" << std::endl;
  //profile = SqrtProfile(profile);

  profile[0].Draw("AC");
  profile[0].SetLineStyle(kSolid);
  profile[0].SetLineColor(kFillColor90); 

  profile[0].GetXaxis()->CenterTitle();
  profile[0].GetYaxis()->CenterTitle();

  profile[0].GetXaxis()->SetTitle(Xtitle);
  profile[0].GetYaxis()->SetTitle(Ytitle);

  profile[0].GetXaxis()->SetRangeUser(Xmin, Xmax);
  profile[0].GetYaxis()->SetRangeUser(Ymin, Ymax);

  profile[0].SetTitle("");
 }

 //#****************************************#
 //#                                        #
 //#              2D SURFACES               #
 //#                                        #
 //#****************************************# 

 if(PlotCL=="90CL"){
  std::cout << "\nPlotting a 90% CL contour..." << std::endl;

  surface.DrawContour(Gaussian90Percent2D(surface), kLine90CL, kFillColor90, -1);
 }
  else if(PlotCL=="123Sigmas"){
   std::cout << "\nPlotting 1,2,3 sigma contours..." << std::endl;

   surface.DrawContour(Gaussian68Percent2D(surface), kLine1Sigma, kFillColor1Sigma, -1);
   surface.DrawContour(Gaussian2Sigma2D(surface),    kLine2Sigma, kFillColor2Sigma, -1);
   surface.DrawContour(Gaussian3Sigma2D(surface),    kLine3Sigma, kFillColor3Sigma, -1);
 }
 

 //if(SurfName.Contains("NO")){
  surface.DrawBestFit(kBestFitColor, kBestFitMark);
 //}

 Canvas->cd();        

 if(PlotCL=="90CL"){
  DrawLegend90CL();
 }
  else if(PlotCL=="123Sigmas"){
   DrawLegendSigmas();
  }
 DrawLegendMassOrdering(SurfName);
 DrawLegendBestFit(SurfName);
 DrawLegendExposure();
 Simulation();
 CornerLabel("No Systs. FD-only");

 std::cout << "\n" << std::endl;
 gPad->Print(PlotOutput + "_" + PlotCL + ".pdf");
 gPad->Print(PlotOutput + ".png");

} // end of code