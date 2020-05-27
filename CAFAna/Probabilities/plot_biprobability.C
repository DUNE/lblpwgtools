//////////////////////////////////////////////////////////////////
// Imported from NOvA CAFAna and adapted by Diana Mendez        //
// Original authors: Erika Catano and Chris Backhouse           //
//                                                              //
// Plots numu->nue appearance biprobability canvases including  //
// matter, hierarchy, delta cp and octant effects one at a time //
// Additionally plots 1-dimensional appearance probabilities in //
// terms of neutrino energy                                     //
//////////////////////////////////////////////////////////////////

#include "CAFAna/Analysis/Style.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Analysis/Fit.h"
#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Core/Utilities.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "OscLib/func/IOscCalculator.h"
#include "Utilities/func/MathUtil.h"

using namespace ana;

#include "TFile.h"
#include "TH2.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TColor.h"
#include "TStyle.h"
#include "TPaveText.h"

#include <iostream>
#include <iomanip>

const double maxprob = 0.1001;
const double x1_dcp = 0.55;
const double x2_dcp = 0.89;
const double y1_dcp = 0.50;
const double y2_dcp = 0.61;

TCanvas * ProbabilityCanvas(double maxprob, bool isFHC, TString title=""){
  maxprob=maxprob*100;
  auto c1=  new TCanvas(UniqueName().c_str(), UniqueName().c_str(), 700, 500);
  c1->SetLeftMargin(0.15);
  c1->SetRightMargin(0.10);
  c1->SetBottomMargin(0.15);
  c1->SetFillStyle(0);
  TH2* axes = new TH2F(UniqueName().c_str(), ";Neutrino energy (GeV)", 
		       20, 0, 5, 100, 0, maxprob);
  axes->SetTitle(title);
  TString ystr = "P#left(#nu_{#mu}#rightarrow#nu_{e}#right) %";
  if (!isFHC) ystr = "P#left(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}#right) %";

  axes->GetYaxis()->SetTitle(ystr);
  axes->GetXaxis()->CenterTitle();
  axes->GetYaxis()->CenterTitle();
  
  axes->GetXaxis()->SetTitleSize(0.9*kBlessedTitleFontSize);
  axes->GetYaxis()->SetTitleSize(0.9*kBlessedTitleFontSize);
  axes->GetXaxis()->SetLabelSize(kBlessedLabelFontSize);
  axes->GetYaxis()->SetLabelSize(kBlessedLabelFontSize);
  axes->GetXaxis()->SetTitleOffset(0.9);
  axes->GetYaxis()->SetTitleOffset(0.8);
  axes->Draw();
  
  return c1;
}

TCanvas * BiprobabilityCanvas(double maxprob = 0.10, TString title=""){
  maxprob=maxprob*100;
  auto c1=  new TCanvas(UniqueName().c_str(), UniqueName().c_str(), 500, 500);
  c1->SetLeftMargin(0.15);
  c1->SetRightMargin(0.10);
  c1->SetBottomMargin(0.15);
  c1->SetFillStyle(0);
  TH2* axes = new TH2F(UniqueName().c_str(), ";P_{#mue};P_{#bar{#mu}#bar{e}}", 
		       100, 0, maxprob, 100, 0, maxprob);
  axes->SetTitle(title);

  axes->GetXaxis()->SetTitle("P#left(#nu_{#mu}#rightarrow#nu_{e}#right) %");
  axes->GetYaxis()->SetTitle("P#left(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}#right) %");
  axes->GetXaxis()->CenterTitle();
  axes->GetYaxis()->CenterTitle();

  axes->GetXaxis()->SetTitleSize(0.9*kBlessedTitleFontSize);
  axes->GetYaxis()->SetTitleSize(0.9*kBlessedTitleFontSize);
  axes->GetXaxis()->SetLabelSize(kBlessedLabelFontSize);
  axes->GetYaxis()->SetLabelSize(kBlessedLabelFontSize);
  axes->GetXaxis()->SetTitleOffset(0.9);
  axes->GetYaxis()->SetTitleOffset(0.8);
  axes->Draw();

  TGraph* one = new TGraph;
  one->SetPoint(0, 0, 0);
  one->SetPoint(1, maxprob, maxprob);
  one->SetLineWidth(2);
//  one->SetLineStyle(7);
  one->SetLineColor(kGray);
//  one->Draw("l");
  return c1;
}

TMarker * GetMarker_dCP (int delta, Int_t color)
{
  TMarker * mark = new TMarker;
  mark->SetMarkerColor(color);
  Int_t style;
  if(delta==0)   style = kOpenCircle;
  if(delta==1)   style = kFullCircle; 
  if(delta==2)   style = kOpenSquare; 
  if(delta==3)   style = kFullSquare; 
  mark->SetMarkerStyle(style);
  return mark;
}

TGraph * TheoryLine_E (osc::IOscCalculatorAdjustable * calc, 
		       const bool isFHC,
		       Int_t color=1, Int_t style = kSolid)
{
  gStyle->SetLineStyleString(11,"8 16");

  int pdgfrom = (isFHC ? 14:-14);
  int pdgto   = (isFHC ? 12:-12);
  double scale = 100.;
  TGraph* gmue = new TGraph();
  for(double E = .25; E < 5.1; E += .0025){
    const double Pmue = calc->P(pdgfrom, pdgto, E);
    gmue->SetPoint(gmue->GetN(),E,Pmue*scale);
  }
  gmue->SetLineWidth(2);
  gmue->SetLineColor(color);
  gmue->SetLineStyle(style);
  return gmue;
}


TGraph*  TheoryEllipse_dCP (osc::IOscCalculatorAdjustable * calc, double E=2.5,
			    int maxDegree = 360,
			    TString label="", Int_t color=1, Int_t style = kSolid, 
			    bool markers=true)
{
  double sc=100;
  TGraph* g = new TGraph;
  double avgx = 0, avgy = 0;
  
  for(int dIdx = 0; dIdx <= maxDegree; ++dIdx){
    double delta = dIdx*2./360.;
    calc->SetdCP(delta*M_PI);
    const double P = calc->P(14, 12, E)*sc;
    const double Pbar = calc->P(-14, -12, E)*sc;
    g->SetPoint(g->GetN(), P, Pbar);

    if(markers){
      if(dIdx%90==0)
	GetMarker_dCP(dIdx/90,color)->DrawMarker(P,Pbar);
    }
    avgx += P;
    avgy += Pbar;
  }
  avgx=avgx/100.;
  avgy=avgy/100.;
  TLatex* ltx = new TLatex(avgx, avgy,label); 
  ltx->SetTextAlign(22);
  ltx->SetTextSize(0.04);
  ltx->SetTextColor(color);
  ltx->Draw();

  g->SetLineWidth(2);
  g->SetLineColor(color);
  g->SetLineStyle(style);
  g->Draw("l");
  return g;
}
TLegend * DefaultLegend(double x1, double y1,double x2, double y2)
{
  auto leg = new TLegend(x1,y1,x2,y2);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.75*kBlessedLabelFontSize);
  return leg;
}

TLegend * Legend_dCP(double x1, double y1,double x2, double y2)
{
  auto leg = new TLegend(x1,y1,x2,y2);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.75*kBlessedLabelFontSize);
    leg->AddEntry(GetMarker_dCP(0,1),"#delta=0","p");
    leg->AddEntry(GetMarker_dCP(1,1),"#delta=#pi/2","p");
    leg->AddEntry(GetMarker_dCP(2,1),"#delta=#pi","p");
    leg->AddEntry(GetMarker_dCP(3,1),"#delta=3#pi/2","p");
    leg->SetNColumns(2);
    return leg;
}

TLatex * HieLabel(int hie, Int_t color, double x, double y)
{
  auto ltx = new TLatex();
  ltx->SetTextSize(0.75*kBlessedLabelFontSize);
  ltx->SetTextColor(color);
  ltx->SetTextAlign(22);
  if(hie>0) ltx->DrawLatexNDC(x,y,"#Deltam_{32}^{2}>0");
  if(hie<0) ltx->DrawLatexNDC(x,y,"#Deltam_{32}^{2}<0");

  return ltx;
}

TPaveText * PaveParams (std::vector<TString> txts, 
			double x1, double y1, double x2, double y2) 
{
  auto pave = new TPaveText(x1,y1,x2,y2,"NDC NB");
  pave->SetTextAlign(12);
  pave->SetFillStyle(0);
  pave->SetLineColorAlpha(kWhite,0);
  pave->SetTextSize(0.7*kBlessedLabelFontSize);
  for (auto txt:txts){
    pave->AddText(txt);
  }
  return pave;
}

TPaveText * PaveParams (double x1, double y1, double x2, double y2) 
{
  std::vector <TString> txts = {
    "sin^{2}#theta_{13}=2.18#times10^{-2}",
    "#Deltam_{32}^{2}=2.67#times10^{-3}eV^{2}" ,
    "sin^{2}#theta_{23}=0.404,0.623"
  };
  auto pave = PaveParams(txts, x1,y1,x2,y2);
  return pave;
}


void plot_biprobability()
{
  double P, Pbar;
  double E=2.5;
  std::vector <TString> txts;

// Biprobability canvas
  auto c_biprob = BiprobabilityCanvas(maxprob,"DUNE: L=1285 km, E=2.5 GeV");
  auto c_prob_f = ProbabilityCanvas(maxprob,true,"DUNE: L=1285 km");
  auto c_prob_r = ProbabilityCanvas(maxprob,false,"DUNE: L=1285 km");

  TString dout = "plots";
  c_biprob->Print(dout+"/plot_biprob_0.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_0.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_0.pdf");

// Add NH 0.5 0 no matter 
  auto calc = DefaultOscCalc();
  calc->SetRho(0);
  txts = {"No matter effects",
	  "sin^{2}#theta_{13}=2.18#times10^{-2}",
	  "|#Deltam_{32}^{2}|=2.44#times10^{-3}eV^{2}",
	  "sin^{2}#theta_{23}=0.5"};

  auto leg =DefaultLegend(0.63,0.60,0.89, 0.68);

  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;
  c_prob_f->cd();
  auto gr1 = TheoryLine_E(calc, true, kPrimColorNH);
  gr1->Draw("same");
  GetMarker_dCP(0,kPrimColorNH)->DrawMarker(E,P);
  PaveParams(txts, 0.63,0.68,0.89,0.885)->Draw();
  leg->AddEntry(gr1->Clone(),"NH #delta=0","l");
  leg->Draw();

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorNH)->Draw("same");
  GetMarker_dCP(0,kPrimColorNH)->DrawMarker(E,Pbar);
  PaveParams(txts, 0.63,0.68,0.89,0.885)->Draw();
  leg->Draw();

  c_biprob->cd();
  GetMarker_dCP(0,kPrimColorNH)->DrawMarker(P,Pbar);
  Legend_dCP(x1_dcp,y1_dcp,x2_dcp,y2_dcp)->Draw();
  PaveParams(txts, 0.53,0.68,0.89,0.885)->Draw();

  c_biprob->Print(dout+"/plot_biprob_1.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_1.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_1.pdf");

// Add IH 0.5 0 no matter 
  calc->SetDmsq32(-1*fabs(calc->GetDmsq32()));
  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;
  c_prob_f->cd();
  gr1 = TheoryLine_E(calc, true, kPrimColorIH);
  gr1->Draw("same");
  GetMarker_dCP(0,kPrimColorIH)->DrawMarker(E,P);
  leg->SetY1NDC(0.58);
  leg->AddEntry(gr1->Clone(),"IH #delta=0","l");
  gPad->Modified();

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorIH)->Draw("same");
  GetMarker_dCP(0,kPrimColorIH)->DrawMarker(E,Pbar);
  gPad->Modified();

  c_biprob->cd();
  GetMarker_dCP(0,kPrimColorIH)->DrawMarker(P,Pbar);

  c_biprob->Print(dout+"/plot_biprob_2.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_2.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_2.pdf");


// Add NH IH 0.5 pi/2 no matter
  calc->SetdCP(0.5*M_PI);
  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;
  c_prob_f->cd();
  TheoryLine_E(calc, true, kPrimColorIH,11)->Draw("same");
  GetMarker_dCP(1,kPrimColorIH)->DrawMarker(E,P);

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorIH,11)->Draw("same");
  GetMarker_dCP(1,kPrimColorIH)->DrawMarker(E,Pbar);

  c_biprob->cd();
  TheoryEllipse_dCP(calc, E, 90, "",kPrimColorIH,kSolid,true)->Draw("same");

  calc->SetDmsq32(fabs(calc->GetDmsq32()));
  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;

  c_prob_f->cd();
  TheoryLine_E(calc, true, kPrimColorNH,11)->Draw("same");
  GetMarker_dCP(1,kPrimColorNH)->DrawMarker(E,P);

  TGraph* dummy = new TGraph();
  dummy->SetLineWidth(2); dummy->SetLineColor(kGray+2); 
  dummy->SetLineStyle(11);
  leg->AddEntry(dummy->Clone(),"#delta=#pi/2","l");
  leg->SetY1NDC(0.54);
  gPad->Modified();

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorNH,11)->Draw("same");
  GetMarker_dCP(1,kPrimColorNH)->DrawMarker(E,Pbar);
  gPad->Modified();

  c_biprob->cd();
  TheoryEllipse_dCP(calc, E, 90, "",kPrimColorNH,kSolid,true)->Draw("same");

  c_biprob->Print(dout+"/plot_biprob_3.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_3.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_3.pdf");

// Add NH IH 0.5 pi no matter
  calc->SetdCP(1.*M_PI);
  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;
  c_prob_f->cd();
  TheoryLine_E(calc, true, kPrimColorNH,9)->Draw("same");
  GetMarker_dCP(2,kPrimColorNH)->DrawMarker(E,P);

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorNH,9)->Draw("same");
  GetMarker_dCP(2,kPrimColorNH)->DrawMarker(E,Pbar);

  c_biprob->cd();
  TheoryEllipse_dCP(calc, E, 180, "",kPrimColorNH,kSolid,true)->Draw("same");

  calc->SetDmsq32(-fabs(calc->GetDmsq32()));
  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;

  c_prob_f->cd();
  TheoryLine_E(calc, true, kPrimColorIH,9)->Draw("same");
  GetMarker_dCP(2,kPrimColorIH)->DrawMarker(E,P);

  dummy->SetLineStyle(9);
  leg->AddEntry(dummy->Clone(),"#delta=#pi","l");
  leg->SetY1NDC(0.5);
  gPad->Modified();

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorIH,9)->Draw("same");
  GetMarker_dCP(2,kPrimColorIH)->DrawMarker(E,Pbar);
  gPad->Modified();

  c_biprob->cd();
  TheoryEllipse_dCP(calc, E, 180, "",kPrimColorIH,kSolid,true)->Draw("same");

  c_biprob->Print(dout+"/plot_biprob_4.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_4.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_4.pdf");


  // Add NH IH 0.5 ellipse no matter
  calc->SetDmsq32(+fabs(calc->GetDmsq32()));
  calc->SetdCP(1.5*M_PI);
  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;
  c_prob_f->cd();
  TheoryLine_E(calc, true, kPrimColorNH,3)->Draw("same");
  GetMarker_dCP(3,kPrimColorNH)->DrawMarker(E,P);

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorNH,3)->Draw("same");
  GetMarker_dCP(3,kPrimColorNH)->DrawMarker(E,Pbar);

  c_biprob->cd();
  TheoryEllipse_dCP(calc, E, 360, "",kPrimColorNH,kSolid,true)->Draw("same");

  calc->SetDmsq32(-fabs(calc->GetDmsq32()));
  calc->SetdCP(1.5*M_PI);

  P = calc->P(14, 12, E)*100.;
  Pbar = calc->P(-14, -12, E)*100.;

  c_prob_f->cd();
  TheoryLine_E(calc, true, kPrimColorIH,3)->Draw("same");
  GetMarker_dCP(3,kPrimColorIH)->DrawMarker(E,P);

  dummy->SetLineStyle(3);
  leg->AddEntry(dummy->Clone(),"#delta=3#pi/2","l");
  leg->SetY1NDC(0.46);
  gPad->Modified();

  c_prob_r->cd();
  TheoryLine_E(calc, false, kPrimColorIH,3)->Draw("same");
  GetMarker_dCP(3,kPrimColorIH)->DrawMarker(E,Pbar);
  gPad->Modified();

  c_biprob->cd();
  TheoryEllipse_dCP(calc, E, 360, "",kPrimColorIH,kSolid,true)->Draw("same");

  c_biprob->Print(dout+"/plot_biprob_5.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_5.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_5.pdf");

  // Remove no matter add matter
  c_biprob = BiprobabilityCanvas(maxprob,"DUNE: L=1285 km, E=2.5 GeV");
  c_prob_f = ProbabilityCanvas(maxprob,true,"DUNE: L=1285 km");
  c_prob_r = ProbabilityCanvas(maxprob,false,"DUNE: L=1285 km");
  calc = DefaultOscCalc();
  txts = {
	  "sin^{2}#theta_{13}=2.18#times10^{-2}",
	  "|#Deltam_{32}^{2}|=2.44#times10^{-3}eV^{2}",
	  "sin^{2}#theta_{23}=0.5"};

  for(int hie:{+1,-1}){
    int color = (hie>0? kPrimColorNH:kPrimColorIH);
    for(int del:{0,1,/*2,*/3}){
      calc->SetDmsq32(hie*fabs(calc->GetDmsq32()));
      calc->SetdCP(del*M_PI/2);
      P = calc->P(14, 12, E)*100.;
      Pbar = calc->P(-14, -12, E)*100.;
      c_prob_f->cd();
      int style = kSolid; 
      if(del==1) style = 11;
      if(del==2) style = 9;
      if(del==3) style = 3;
      TheoryLine_E(calc, true, color,style)->Draw("same");
      GetMarker_dCP(del,color)->DrawMarker(E,P);      
      
      c_prob_r->cd();
      TheoryLine_E(calc, false, color,style)->Draw("same");
      GetMarker_dCP(del,color)->DrawMarker(E,Pbar);
    }
    c_biprob->cd();
    TheoryEllipse_dCP(calc, E, 360, "",color,kSolid,true)->Draw("same");
  }
  leg->Clear();
  dummy->SetLineStyle(kSolid);
  dummy->SetLineColor(kPrimColorNH);leg->AddEntry(dummy->Clone(),"NH #delta=0","l");
  dummy->SetLineColor(kPrimColorIH);leg->AddEntry(dummy->Clone(),"IH #delta=0","l");
  dummy->SetLineColor(kGray+2);
  dummy->SetLineStyle(11);leg->AddEntry(dummy->Clone(),"#delta=#pi/2","l");
  dummy->SetLineStyle( 3);leg->AddEntry(dummy->Clone(),"#delta=3#pi/2","l");

  for(auto c:{c_biprob, c_prob_f, c_prob_r}){
    c->cd();
    if(c==c_biprob){
      PaveParams(txts, 0.53,0.68,0.89,0.885)->Draw();
      Legend_dCP(x1_dcp,y1_dcp,x2_dcp,y2_dcp)->Draw(); 
    }
    else{
      PaveParams(txts, 0.63,0.68,0.89,0.885)->Draw();
      leg->Draw();
    }
  }
  c_biprob->Print(dout+"/plot_biprob_6.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_6.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_6.pdf");

  // Remove 0.5 add 0.4 0.6
  c_prob_f = ProbabilityCanvas(0.085,true,"NOvA: L=810 km");
  c_prob_r = ProbabilityCanvas(0.085,false,"NOvA: L=810 km");
  txts = {
    "sin^{2}#theta_{13}=2.18#times10^{-2}",
    "|#Deltam_{32}^{2}|=2.44#times10^{-3}eV^{2}",
    //  "sin^{2}#theta_{23}=0.404,0.623"
      "#delta_{CP}=0"
  };

  calc = DefaultOscCalc();

  for(int hie:{+1,-1}){
    for(double ssth23:{0.40,0.5, 0.60}) {
      int color = (hie>0? kPrimColorNH:kPrimColorIH);
      int style = kSolid;
      if(ssth23<0.41) {style = 2; color = color -1;}
      if(ssth23>0.59) {style = 9; color = color -1;}

      calc->SetDmsq32(hie*fabs(calc->GetDmsq32()));
      kFitSinSqTheta23.SetValue(calc, ssth23);
      P = calc->P(14, 12, E)*100.;
      Pbar = calc->P(-14, -12, E)*100.;
      c_prob_f->cd();
      TheoryLine_E(calc, true, color,style)->Draw("same");
      
      c_prob_r->cd();
      TheoryLine_E(calc, false, color,style)->Draw("same");
    }//end th23
  }//end hie

  leg->Clear();
  dummy->SetLineStyle(kSolid);
  dummy->SetLineColor(kPrimColorNH);leg->AddEntry(dummy->Clone(),
						  "NH sin^{2}#theta_{23}=0.5","l");
  dummy->SetLineColor(kPrimColorIH);leg->AddEntry(dummy->Clone(),
						  "IH sin^{2}#theta_{23}=0.5","l");
  dummy->SetLineColor(kGray+2);
  dummy->SetLineStyle(2);leg->AddEntry(dummy->Clone(),"sin^{2}#theta_{23}=0.40","l");
  dummy->SetLineStyle(9);leg->AddEntry(dummy->Clone(),"sin^{2}#theta_{23}=0.60","l");

  for(auto c:{c_prob_f, c_prob_r}){
    c->cd();
    PaveParams(txts, 0.63,0.68,0.89,0.885)->Draw();
    leg->Draw();
  }
  
  c_biprob = BiprobabilityCanvas(maxprob,"DUNE: L=1285 km, E=2.5 GeV");
  txts = {
    "sin^{2}#theta_{13}=2.18#times10^{-2}",
    "|#Deltam_{32}^{2}|=2.44#times10^{-3}eV^{2}",
    "sin^{2}#theta_{23}=0.40,0.60"
  };
  PaveParams(txts, 0.53,0.68,0.89,0.885)->Draw();
  Legend_dCP(x1_dcp,y1_dcp,x2_dcp,y2_dcp)->Draw(); 

  calc = DefaultOscCalc();
  for(int hie:{+1,-1}){
    for(double ssth23:{0.40,0.60}) {
      int color = (hie>0? kPrimColorNH:kPrimColorIH);
      calc->SetDmsq32(hie*fabs(calc->GetDmsq32()));
      kFitSinSqTheta23.SetValue(calc, ssth23);
      TheoryEllipse_dCP(calc, E, 360, 
			TString::Format("%.3f",ssth23),
			  color,kSolid,true)->Draw("same");
    
    }
  }
  c_biprob->Print(dout+"/plot_biprob_7.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_7.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_7.pdf");


  // Switch to higher dmsq
  // Remove 0.5 add 0.4 0.6
  c_prob_f = ProbabilityCanvas(maxprob,true,"DUNE: L=1285 km");
  c_prob_r = ProbabilityCanvas(maxprob,false,"DUNE: L=1285 km");
  txts = {
	  "sin^{2}#theta_{13}=2.18#times10^{-2}",
	  "|#Deltam_{32}^{2}|=2.67#times10^{-3}eV^{2}",
//	  "sin^{2}#theta_{23}=0.404,0.623"
	  "#delta_{CP}=0"
  };

  calc = DefaultOscCalc();

  for(int hie:{+1,-1}){
    for(double ssth23:{0.40,0.5, 0.60}) {

      int style = kSolid;
      for(double dmsq:{2.44e-3, 2.67e-3}){
	calc->SetDmsq32(dmsq);
	int color = (hie>0? kPrimColorNH:kPrimColorIH);
	if(ssth23<0.41) {style = 2; color = color -1;}
	if(ssth23>0.59) {style = 9; color = color -1;}

	if(dmsq<2.5e-3) color = kGray;

	calc->SetDmsq32(hie*fabs(calc->GetDmsq32()));
	kFitSinSqTheta23.SetValue(calc, ssth23);
	P = calc->P(14, 12, E)*100.;
	Pbar = calc->P(-14, -12, E)*100.;
	c_prob_f->cd();
	TheoryLine_E(calc, true, color,style)->Draw("same");
	
	c_prob_r->cd();
	TheoryLine_E(calc, false, color,style)->Draw("same");
      }//end dmsq
    }//end th23
  }//end hie

  leg->Clear();
  dummy->SetLineStyle(kSolid);
  dummy->SetLineColor(kPrimColorNH);leg->AddEntry(dummy->Clone(),
						  "NH sin^{2}#theta_{23}=0.5","l");
  dummy->SetLineColor(kPrimColorIH);leg->AddEntry(dummy->Clone(),
						  "IH sin^{2}#theta_{23}=0.5","l");
  dummy->SetLineColor(kGray+2);
  dummy->SetLineStyle(2);leg->AddEntry(dummy->Clone(),"sin^{2}#theta_{23}=0.40","l");
  dummy->SetLineStyle(9);leg->AddEntry(dummy->Clone(),"sin^{2}#theta_{23}=0.60","l");

  for(auto c:{c_prob_f, c_prob_r}){
    c->cd();
    PaveParams(txts, 0.63,0.68,0.89,0.885)->Draw();
    leg->Draw();
  }
  
  c_biprob = BiprobabilityCanvas(maxprob,"DUNE: L=1285 km, E=2.5 GeV");
  txts = {
    "sin^{2}#theta_{13}=2.18#times10^{-2}",
    "|#Deltam_{32}^{2}|=2.67#times10^{-3}eV^{2}",
    "sin^{2}#theta_{23}=0.40,0.60"
  };
  PaveParams(txts, 0.53,0.68,0.89,0.885)->Draw();
  Legend_dCP(x1_dcp,y1_dcp,x2_dcp,y2_dcp)->Draw(); 

  calc = DefaultOscCalc();
  calc->SetDmsq32(2.67e-3);
  for(int hie:{+1,-1}){

    for(double ssth23:{0.404,0.623}) {
      for(double dmsq:{2.44e-3, 2.67e-3}){
	calc->SetDmsq32(dmsq);
	int color = (hie>0? kPrimColorNH:kPrimColorIH);
	if(dmsq<2.5e-3) color = kGray+1;

      calc->SetDmsq32(hie*fabs(calc->GetDmsq32()));
      kFitSinSqTheta23.SetValue(calc, ssth23);
      TheoryEllipse_dCP(calc, E, 360, 
			TString::Format("%.3f",ssth23),
			color,kSolid,true)->Draw("same");
      }
    }
  }
  c_biprob->Print(dout+"/plot_biprob_8.pdf");
  c_prob_f->Print(dout+"/plot_prob_f_8.pdf");
  c_prob_r->Print(dout+"/plot_prob_r_8.pdf");


  //Hyperspace!

  for(int i=0; i<=10; i++){
    c_biprob = BiprobabilityCanvas(maxprob,"DUNE: L=1285 km, E=2.5 GeV");
    double dmsq = (2.4+(2.75-2.4)/10.*i);
    txts = {
      "sin^{2}2#theta_{13}=0.085#pm0.005",
      TString::Format("|#Deltam_{32}^{2}|=%.2f#times10^{-3}eV^{2}",dmsq),
      "sin^{2}#theta_{23}=0.3-0.7"
    };
    PaveParams(txts, 0.51,0.68,0.89,0.885)->Draw();
    Legend_dCP(x1_dcp,y1_dcp,x2_dcp,y2_dcp)->Draw(); 
    
    calc = DefaultOscCalc();
    for(int hie:{+1,-1}){
      for(double ssth23:{0.404,0.5,0.623,0.42,0.44,0.46,0.48,0.52,0.54,0.56,0.58,0.60,0.3,0.32,0.34,0.36,0.38,0.64,0.66,0.68,0.7}) {
	for(double ss2th13:{0.080,0.085,0.090}){
	  int color = (hie>0? kPrimColorNH:kPrimColorIH);
	  calc->SetDmsq32(dmsq*1e-3);
	  calc->SetDmsq32(hie*fabs(calc->GetDmsq32()));
	  kFitSinSq2Theta13.SetValue(calc,ss2th13);
	  kFitSinSqTheta23.SetValue(calc, ssth23);
	  TheoryEllipse_dCP(calc, E, 360, 
			    "",
			    color,kSolid,true)->Draw("same");
	}
      }
    }
    
    c_biprob->Print((TString)dout+"/plot_biprob_hyp_"+std::to_string(i)+".pdf");
  }//end i

}
