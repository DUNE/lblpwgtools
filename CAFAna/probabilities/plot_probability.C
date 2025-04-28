//////////////////////////////////////////////////////////////////
// Plots numu->nue and numu->numu oscillation probabilities     //
// with different mh, theta23 and matter densities              //
// in terms of neutrino energy or fixed baseline/energy         //
//////////////////////////////////////////////////////////////////

#include "CAFAna/Analysis/Calcs.h"

#include "OscLib/IOscCalc.h"

#include "TGraph.h"
#include "TLegend.h"
#include "TH2.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TMath.h"

#include <string>

using namespace ana;
using namespace std;

bool DianaIsLazy = true;
double baseline = 1295.;

/*
void CornerLabel(std::string& s)
{
  TLatex* corner = new TLatex(.90, .94, s.c_str());
  corner->SetTextColor(kGray+1);
  corner->SetNDC();
  corner->SetTextSize(1.5/43.);
  corner->SetTextAlign(32);
  corner->Draw();
}
*/

void plot_probability(bool plotLE = false)
{


  std::string tag = "E";
  const char* labels  = "";
  const char* titles = ";Neutrino Energy (GeV); P(#nu_{#mu} #rightarrow #nu_{e})";
  if(plotLE){
    tag = "LE";
    labels = ";Baseline / Neutrino Energy (km/GeV); P(#nu_{#mu} #rightarrow #nu_{e})";
  }

  std::vector<double> sinVal   = {0.504, 0.506, 0.508, 0.510, 0.512, 0.514, 0.516, 0.518};
  std::vector<string> sinLabel = {"0.504", "0.506", "0.508", "0.510", "0.512", "0.514", "0.516", "0.518"};
  // std::vector<double> sinVal   = {0.42, 0.46, 0.50, 0.52, 0.54, 0.58, 0.62};
  // std::vector<string> sinLabel = {"0.42", "0.46", "0.50", "0.52", "0.54", "0.58", "0.62"};
  const int sinTot = sinVal.size();

  std::vector<double> rhoVal   = {2.8, 0.0};
  std::vector<string> rhoLabel = {"2.8", "0.0"};
  const int rhoTot = rhoVal.size();

  std::vector<double> deltaVal = {0.00245, -0.00251};
  std::vector<string> deltaLabel = {"nh", "ih"};
  const int deltaTot = deltaVal.size();



  // styles = rhoTot
  // colors = sinTot
  std::vector<Style_t> style = {1, 2, 3, 4};
  std::vector<Color_t> colore = {kBlue-7, kRed-4, kCyan+1, kGray+2, kBlue+1, kOrange+1, kGreen+1, kMagenta};
  std::vector<Color_t> colormu = {//kMagenta, 
    kRed-4, kGreen+1, kOrange+1, kBlue+1, kGray+2, kCyan+1, kRed-4, kBlue-7};


  std::vector< std::vector < std::vector<osc::IOscCalcAdjustable*> > > calc; 
  std::vector< std::vector < std::vector<TGraph*> > > gmue;
  std::vector< std::vector < std::vector<TGraph*> > > gmumu;
  std::vector< std::vector < std::vector<TGraph*> > > gmutau;

  for(int sinId = 0; sinId<sinTot; sinId++){

    std::vector< std::vector<osc::IOscCalcAdjustable*> > calcDouble;
    std::vector< std::vector<TGraph*> > gmueDouble;
    std::vector< std::vector<TGraph*> > gmumuDouble;
    std::vector< std::vector<TGraph*> > gmutauDouble;

    for(int rhoId = 0; rhoId<rhoTot; rhoId++){

      std::vector<osc::IOscCalcAdjustable*> calcSingle;
      std::vector<TGraph*> gmueSingle;
      std::vector<TGraph*> gmumuSingle;
      std::vector<TGraph*> gmutauSingle;

      for(int deltaId = 0; deltaId<deltaTot; deltaId++){	
	
      	osc::IOscCalcAdjustable* tempCalc = DefaultOscCalc();
        ResetOscCalcToDefault(tempCalc);
      	tempCalc->SetRho(rhoVal[rhoId]); 
      	tempCalc->SetDmsq32(deltaVal[deltaId]);
      	tempCalc->SetTh23(asin(sqrt(sinVal[sinId])));
      	calcSingle.push_back(tempCalc); 
      	
      	gmueSingle.push_back(new TGraph);
      	gmumuSingle.push_back(new TGraph);
      	gmutauSingle.push_back(new TGraph);

      } //delta

      calcDouble.push_back(calcSingle);
      gmueDouble.push_back( gmueSingle );
      gmumuDouble.push_back( gmumuSingle );
      gmutauDouble.push_back( gmutauSingle );

    } //rho

    calc.push_back( calcDouble );
    gmue.push_back( gmueDouble );
    gmumu.push_back( gmumuDouble );
    gmutau.push_back( gmutauDouble );

  } //sin
  


  for(int sinId = 0; sinId<sinTot; sinId++){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){

      	for(double E = .1; E < 3.1; E += .0015){
      	  const double Pmue  = calc[sinId][rhoId][deltaId]->P(14, 12, E);
      	  const double Pmumu = calc[sinId][rhoId][deltaId]->P(14, 14, E);
      	  const double Pmutau = calc[sinId][rhoId][deltaId]->P(14, 16, E);

      	  double x=E;
      	  if(plotLE) x=baseline/E;
      	  gmue[sinId][rhoId][deltaId]->SetPoint(gmue[sinId][rhoId][deltaId]->GetN(), x, Pmue);
      	  gmumu[sinId][rhoId][deltaId]->SetPoint(gmumu[sinId][rhoId][deltaId]->GetN(), x, Pmumu);
      	  gmutau[sinId][rhoId][deltaId]->SetPoint(gmutau[sinId][rhoId][deltaId]->GetN(), x, Pmutau);
      	} // for energies or L/E
	
      } //delta
    } //rho
  } //sin



  // ==============
  // var hierarchy
  // ==============
  for(int sinId = 0; sinId<sinTot; sinId++){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){
        gmue[sinId][rhoId][deltaId]->SetLineWidth(2);
        gmue[sinId][rhoId][deltaId]->SetLineColor(kRed-4);
        gmue[sinId][rhoId][deltaId]->SetLineStyle(style[deltaId]);
        gmumu[sinId][rhoId][deltaId]->SetLineWidth(2);
      	gmumu[sinId][rhoId][deltaId]->SetLineColor(kBlue-7);
        gmumu[sinId][rhoId][deltaId]->SetLineStyle(style[deltaId]);
      	gmutau[sinId][rhoId][deltaId]->SetLineWidth(2);
        gmutau[sinId][rhoId][deltaId]->SetLineColor(kOrange+1);
        gmutau[sinId][rhoId][deltaId]->SetLineStyle(style[deltaId]);
      } //delta
    } //rho
  }//sin   


  if(DianaIsLazy){
    //for(int sinId = 0; sinId<sinTot; sinId++){
    //for(int rhoId = 0; rhoId<rhoTot; rhoId++){

	double xmin = 0.1, xmax = 3.;
	double ymin = -0.01, ymax = 0.2;
	if(plotLE){
	  	xmin = baseline/xmin;
		xmax = baseline/xmax;
	}

    //std::string thisLabel = "numutoe__rho"+rhoLabel[rhoId]+"_sin"+sinLabel[sinId];
    TH2* axes = new TH2F("", labels, 60, xmin, xmax, 100, ymin, ymax);
    //CenterTitles(axes);
    axes->Draw();

    gPad->SetFillStyle(0);
    gmue[0][0][0]->Draw("l same");
	/*
        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          gmue[sinId][rhoId][deltaId]->Draw("l same");
        }
        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("Hierarchy","C");
        leg->SetFillStyle(0);
        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          leg->AddEntry(gmumu[sinId][rhoId][deltaId], Form("%s", deltaLabel[deltaId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);
	*/
    gPad->Print(("oscprob_numutoe_"+tag+".png").c_str());
    gPad->Print(("oscprob_numutoe_"+tag+".pdf").c_str());
	//}
	//}
  }


  if(DianaIsLazy){
    //for(int sinId = 0; sinId<sinTot; sinId++){
    //for(int rhoId = 0; rhoId<rhoTot; rhoId++){

	double xmin = 0.1, xmax = 3.;
	double ymin = -0.01, ymax = 1.01;
	if(plotLE){
	  	xmin = baseline/xmin;
		xmax = baseline/xmax;
	}

    //std::string thisLabel = "numutomu__rho"+rhoLabel[rhoId]+"_sin"+sinLabel[sinId];
    TH2* axes = new TH2F("", labels, 60, xmin, xmax, 100, ymin, ymax);
    //CenterTitles(axes);
    axes->Draw();

    gPad->SetFillStyle(0);
  	gmumu[0][0][0]->Draw("l same");   
	/*
        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          gmumu[sinId][rhoId][deltaId]->Draw("l same");
        }

        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("Hierarchy","C");
        leg->SetFillStyle(0);
        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          leg->AddEntry(gmue[sinId][rhoId][deltaId], Form("%s", deltaLabel[deltaId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);
	*/
        gPad->Print(("oscprob_numutomu_"+tag+".png").c_str());
	gPad->Print(("oscprob_numutomu_"+tag+".pdf").c_str());
	//}
	//}
  }


  if(DianaIsLazy){

	double xmin = 0.1, xmax = 3.;
	double ymin = -0.01, ymax = 1.01;
	if(plotLE){
	  	xmin = baseline/xmin;
		xmax = baseline/xmax;
	}
    //std::string thisLabel = "both__rho"+rhoLabel[rhoId]+"_sin"+sinLabel[sinId];
    TH2* axes = new TH2F("", labels, 60, xmin, xmax, 100, ymin, ymax);
    //CenterTitles(axes);
    axes->Draw();

    gPad->SetFillStyle(0);

  	gmumu[0][0][0]->Draw("l same");
  	gmue[0][0][0]->Draw("l same");
  	//gmutau[0][0][0]->Draw("l same");

    TLegend* leg = new TLegend(0.6, 0.65, 0.8, 0.85);
    //leg->SetHeader("Probability","C");
    leg->SetFillStyle(0);
  	leg->AddEntry(gmue[0][0][0], "P(#nu_{#mu} #rightarrow #nu_{e})", "l");
  	leg->AddEntry(gmumu[0][0][0], "P(#nu_{#mu} #rightarrow #nu_{#mu})", "l");
  	//leg->AddEntry(gmutau[0][0][0], "P(#nu_{#mu} #rightarrow #nu_{#tau})", "l");
    leg->Draw("same");
    //CornerLabel(thisLabel);

    gPad->Print(("oscprob_both_"+tag+".png").c_str());
    gPad->Print(("oscprob_both_"+tag+".pdf").c_str());
  }


}

