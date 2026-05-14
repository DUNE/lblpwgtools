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

 std::string PlotCL  = "90CL"; // "90CL" or "123Sigmas"   

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
  else{
   std::cout << "\nCouldn't select between 90CL or 1,2,3 sigma contours"<<std::endl;
   exit(1);
  }
 

 //if(SurfName.Contains("NO")){
  surface.DrawBestFit(kBestFitColor, kBestFitMark);
 //}

 Canvas->cd();        

 if(PlotCL=="90CL"){
  DrawLegend90CL();
 }
  else{
   DrawLegendSigmas();
  }
 DrawLegendMassOrdering(SurfName);
 DrawLegendBestFit(SurfName);
 DrawLegendExposure();
 Simulation();

 std::cout << "\n" << std::endl;
 gPad->Print(PlotOutput + "_" + PlotCL + ".pdf");
 //gPad->Print(PlotOutput + ".png");

} // end of code