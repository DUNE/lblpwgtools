// To set this as default, you need a .rootrc file in your home directory,
// containing the following line:
// Rint.Logon: /full/path/to/rootlogon.C

#ifndef ROOTLOGON_C
#define ROOTLOGON_C

#include "TColor.h"
#include "TH1.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"

void rootlogon()
{
  printf("Welcome to the ROOT of all evils \n");

  // Defaults to classic style, but that's OK, we can fix it
  TStyle* duneStyle = new TStyle("duneStyle", "DUNE Style");

  // Centre title
  duneStyle->SetTitleAlign(22);
  duneStyle->SetTitleX(.5);
  duneStyle->SetTitleY(.95);
  duneStyle->SetTitleBorderSize(0);

  // No info box
  duneStyle->SetOptStat(0);

  //set the background color to white
  duneStyle->SetFillColor(10);
  duneStyle->SetFrameFillColor(10);
  duneStyle->SetCanvasColor(10);
  duneStyle->SetPadColor(10);
  duneStyle->SetTitleFillColor(0);
  duneStyle->SetStatColor(10);

  // Don't put a colored frame around the plots
  duneStyle->SetFrameBorderMode(0);
  duneStyle->SetCanvasBorderMode(0);
  duneStyle->SetPadBorderMode(0);

  // Set the default line color for a fit function to be red
  duneStyle->SetFuncColor(kRed);

  // Marker settings
  //  duneStyle->SetMarkerStyle(kFullCircle);

  // No border on legends
  duneStyle->SetLegendBorderSize(0);

  // Axis titles
  duneStyle->SetTitleSize(.055, "xyz");
  duneStyle->SetTitleOffset(.8, "xyz");
  // More space for y-axis to avoid clashing with big numbers
  duneStyle->SetTitleOffset(.9, "y");
  // This applies the same settings to the overall plot title
  duneStyle->SetTitleSize(.055, "");
  duneStyle->SetTitleOffset(.8, "");
  // Axis labels (numbering)
  duneStyle->SetLabelSize(.04, "xyz");
  duneStyle->SetLabelOffset(.005, "xyz");

  // Prevent ROOT from occasionally automatically zero-suppressing
  duneStyle->SetHistMinimumZero();

  // Thicker lines
  duneStyle->SetHistLineWidth(2);
  duneStyle->SetFrameLineWidth(2);
  duneStyle->SetFuncWidth(2);

  // Set the number of tick marks to show
  duneStyle->SetNdivisions(506, "xyz");

  // Set the tick mark style
  duneStyle->SetPadTickX(1);
  duneStyle->SetPadTickY(1);

  // Fonts
  const int kDuneFont = 42;
  duneStyle->SetStatFont(kDuneFont);
  duneStyle->SetLabelFont(kDuneFont, "xyz");
  duneStyle->SetTitleFont(kDuneFont, "xyz");
  duneStyle->SetTitleFont(kDuneFont, ""); // Apply same setting to plot titles
  duneStyle->SetTextFont(kDuneFont);
  duneStyle->SetLegendFont(kDuneFont);

  // Get moodier colours for colz
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  duneStyle->SetNumberContours(NCont);

  gROOT->SetStyle("duneStyle");

  // Uncomment this line if you want to force all plots loaded from files
  // to use this same style
  //gROOT->ForceStyle();
}

// Put a "DUNE Preliminary" tag in the corner
void Preliminary()
{
  TLatex* prelim = new TLatex(.9, .95, "DUNE Preliminary");
  prelim->SetTextColor(kBlue);
  prelim->SetNDC();
  prelim->SetTextSize(2/30.);
  prelim->SetTextAlign(32);
  prelim->Draw();
}

// Put a "DUNE Preliminary" tag on the right
void PreliminarySide()
{
  TLatex* prelim = new TLatex(.93, .9, "DUNE Preliminary");
  prelim->SetTextColor(kBlue);
  prelim->SetNDC();
  prelim->SetTextSize(2/30.);
  prelim->SetTextAngle(270);
  prelim->SetTextAlign(12);
  prelim->Draw();
}

// Put a "DUNE Simulation" tag in the corner
void Simulation()
{
  TLatex* prelim = new TLatex(.9, .95, "DUNE Simulation");
  prelim->SetTextColor(kGray+1);
  prelim->SetNDC();
  prelim->SetTextSize(2/30.);
  prelim->SetTextAlign(32);
  prelim->Draw();
}

// Put a "DUNE Simulation" tag on the right
void SimulationSide()
{
  TLatex* prelim = new TLatex(.93, .9, "DUNE Simulation");
  prelim->SetTextColor(kGray+1);
  prelim->SetNDC();
  prelim->SetTextSize(2/30.);
  prelim->SetTextAngle(270);
  prelim->SetTextAlign(12);
  prelim->Draw();
}

// Put a "DUNE Fake Data" tag in the corner
void FakeData()
{
  TLatex* prelim = new TLatex(.9, .95, "DUNE Fake Data");
  prelim->SetTextColor(kBlue);
  prelim->SetNDC();
  prelim->SetTextSize(2/30.);
  prelim->SetTextAlign(32);
  prelim->Draw();
}

// Add a label in top left corner
// Especially useful for "Neutrino Beam" and "Antineutrino Beam" labels
void CornerLabel(std::string Str) {
  TLatex* CornLab = new TLatex(.1, .93, Str.c_str());
  CornLab->SetTextColor(kGray+1);
  CornLab->SetNDC();
  CornLab->SetTextSize (2/30.);
  CornLab->SetTextAlign(11);
  CornLab->Draw();
}

void CenterTitles(TH1* histo)
{
  histo->GetXaxis()->CenterTitle();
  histo->GetYaxis()->CenterTitle();
  histo->GetZaxis()->CenterTitle();  
}

#endif
