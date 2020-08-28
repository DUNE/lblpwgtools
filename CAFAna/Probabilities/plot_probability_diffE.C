////////////////////////////////////////////////////////////////////
// Plots numu and numubar disappearance probabilities, and        //
// their difference for a set of mh, theta23 and matter densities //
// in terms of neutrino energy                                    //
//                                                                //
// useful to graphicaly see the matter and octant effects, and    //
// how they differently affect the nu and nubar oscillations      //
////////////////////////////////////////////////////////////////////

#include "CAFAna/Analysis/Calcs.h"

#include "OscLib/IOscCalc.h"

#include "TGraph.h"
#include "TLegend.h"
#include "TH2.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TMath.h"

using namespace ana;

bool DianaIsLazy = true;

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

void plot_probability_diffE()
{

  //std::vector<double> sinVal   = {0.504, 0.506, 0.508, 0.510, 0.512, 0.514, 0.516, 0.518};
  //std::vector<string> sinLabel = {"0.504", "0.506", "0.508", "0.510", "0.512", "0.514", "0.516", "0.518"};
  std::vector<double> sinVal   = {0.42, 0.46, 0.50, 0.52, 0.54, 0.58, 0.62};
  std::vector<string> sinLabel = {"0.42", "0.46", "0.50", "0.52", "0.54", "0.58", "0.62"};
  const int sinTot = sinVal.size();

  std::vector<double> rhoVal   = {0.0, 2.8, 4.0, 5.0, 6.0, 7.0};
  std::vector<string> rhoLabel = {"0.0", "2.8", "4.0", "5.0", "6.0", "7.0"};
  const int rhoTot = rhoVal.size();

  std::vector<double> deltaVal = {0.00245, -0.00251};
  std::vector<string> deltaLabel = {"nh", "ih"};
  const int deltaTot = deltaVal.size();



  // styles = rhoTot
  // colors = sinTot
  std::vector<Style_t> style = {3, kSolid, 2, 5, 6, 8};
  std::vector<Color_t> color = {kMagenta, kGreen+1, kOrange+1, kGray+2, kCyan+1, kRed-4, kBlue-7, kBlue+1};


  std::vector< std::vector < std::vector<osc::IOscCalcAdjustable*> > > calc; 
  std::vector< std::vector < std::vector<TGraph*> > > gmumu;
  std::vector< std::vector < std::vector<TGraph*> > > gmumubar;
  std::vector< std::vector < std::vector<TGraph*> > > gmudif;

  for(int sinId = 0; sinId<sinTot; sinId++){

    std::vector< std::vector<osc::IOscCalcAdjustable*> > calcDouble;
    std::vector< std::vector<TGraph*> > gmumuDouble;
    std::vector< std::vector<TGraph*> > gmumubarDouble;
    std::vector< std::vector<TGraph*> > gmudifDouble;

    for(int rhoId = 0; rhoId<rhoTot; rhoId++){

      std::vector<osc::IOscCalcAdjustable*> calcSingle;
      std::vector<TGraph*> gmumuSingle;
      std::vector<TGraph*> gmumubarSingle;
      std::vector<TGraph*> gmudifSingle;

      for(int deltaId = 0; deltaId<deltaTot; deltaId++){	
	
      	osc::IOscCalcAdjustable* tempCalc = DefaultOscCalc();
        ResetOscCalcToDefault(tempCalc);
      	tempCalc->SetRho(rhoVal[rhoId]); 
      	tempCalc->SetDmsq32(deltaVal[deltaId]);
      	tempCalc->SetTh23(asin(sqrt(sinVal[sinId])));
      	calcSingle.push_back(tempCalc); 
	
      	gmumuSingle.push_back(new TGraph);
      	gmumubarSingle.push_back(new TGraph);
      	gmudifSingle.push_back(new TGraph);
      
      } //delta

      calcDouble.push_back( calcSingle );
      gmumuDouble.push_back( gmumuSingle );
      gmumubarDouble.push_back( gmumubarSingle );
      gmudifDouble.push_back( gmudifSingle );

    } //rho

    calc.push_back( calcDouble );
    gmumu.push_back( gmumuDouble );
    gmumubar.push_back( gmumubarDouble );
    gmudif.push_back( gmudifDouble );

  } //sin
  


  for(int sinId = 0; sinId<sinTot; sinId++){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){
      	for(double E = .25; E < 3.1; E += .0025){
      	  const double Pmumu    = calc[sinId][rhoId][deltaId]->P(14, 14, E);
      	  const double Pmumubar = calc[sinId][rhoId][deltaId]->P(-14, -14, E);
      	  const double Pmudif   = Pmumu - Pmumubar;
      	  
      	  gmumu[sinId][rhoId][deltaId]->SetPoint(gmumu[sinId][rhoId][deltaId]->GetN(), E, Pmumu);
      	  gmumubar[sinId][rhoId][deltaId]->SetPoint(gmumubar[sinId][rhoId][deltaId]->GetN(), E, Pmumubar);
      	  gmudif[sinId][rhoId][deltaId]->SetPoint(gmudif[sinId][rhoId][deltaId]->GetN(), E, Pmudif);
      	} // for energies
	
      } //delta
    } //rho
  } //sin


  /*
  // ==============
  // var hierarchy
  // ==============
  for(int sinId = 0; sinId<sinTot; sinId++){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){
        gmumu[sinId][rhoId][deltaId]->SetLineWidth(2);
        gmumu[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
        gmumu[sinId][rhoId][deltaId]->SetLineStyle(style[deltaId]);
        gmumubar[sinId][rhoId][deltaId]->SetLineWidth(2);
      	gmumubar[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
        gmumubar[sinId][rhoId][deltaId]->SetLineStyle(style[deltaId]);
        gmudif[sinId][rhoId][deltaId]->SetLineWidth(2);
        gmudif[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
        gmudif[sinId][rhoId][deltaId]->SetLineStyle(style[deltaId]);
      } //delta
    } //rho
  }//sin   


  if(DianaIsLazy){
    for(int sinId = 0; sinId<sinTot; sinId++){
      for(int rhoId = 0; rhoId<rhoTot; rhoId++){

      	std::string thisLabel = "numu__rho"+rhoLabel[rhoId]+"_sin"+sinLabel[sinId];
        TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#nu_{#mu} #rightarrow #nu_{#mu})", 60, 1., 2., 100, -0.01, 1.01);
        CenterTitles(axes);
        axes->Draw();

        gPad->SetFillStyle(0);

        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          gmumu[sinId][rhoId][deltaId]->Draw("l same");
        }

        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("Hierarchy","C");
        leg->SetFillStyle(0);
        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          leg->AddEntry(gmumu[sinId][rhoId][deltaId], Form("%s", deltaLabel[deltaId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);

        //gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("Poscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }


  if(DianaIsLazy){
    for(int sinId = 0; sinId<sinTot; sinId++){
      for(int rhoId = 0; rhoId<rhoTot; rhoId++){

      	std::string thisLabel = "numubar__rho"+rhoLabel[rhoId]+"_sin"+sinLabel[sinId];
        TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#mu})", 60, 1., 2., 100, -0.01, 1.01);
        CenterTitles(axes);
        axes->Draw();

        gPad->SetFillStyle(0);

        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          gmumubar[sinId][rhoId][deltaId]->Draw("l same");
        }

        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("Hierarchy","C");
        leg->SetFillStyle(0);
        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          leg->AddEntry(gmumubar[sinId][rhoId][deltaId], Form("%s", deltaLabel[deltaId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);

        //gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }


  if(DianaIsLazy){
    for(int sinId = 0; sinId<sinTot; sinId++){
      for(int rhoId = 0; rhoId<rhoTot; rhoId++){

      	std::string thisLabel = "signdif__rho"+rhoLabel[rhoId]+"_sin"+sinLabel[sinId];
        TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#nu_{#mu}) - P(#bar{#nu}_{#mu})", 60, 1., 2., 100, -0.001, 0.001);
        CenterTitles(axes);
        axes->Draw();

        gPad->SetFillStyle(0);

        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          gmudif[sinId][rhoId][deltaId]->Draw("l same");
        }

        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("Hierarchy","C");
        leg->SetFillStyle(0);
        for(int deltaId = 0; deltaId < deltaTot; deltaId++){
          leg->AddEntry(gmudif[sinId][rhoId][deltaId], Form("%s", deltaLabel[deltaId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);

        //gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }

*/


  /*
  // ========
  // var rho
  // ========
  for(int sinId = 0; sinId<sinTot; sinId++){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){
      	gmumu[sinId][rhoId][deltaId]->SetLineWidth(2);
      	gmumu[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
      	gmumu[sinId][rhoId][deltaId]->SetLineStyle(style[rhoId]);
      	gmumubar[sinId][rhoId][deltaId]->SetLineWidth(2);
      	gmumubar[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
      	gmumubar[sinId][rhoId][deltaId]->SetLineStyle(style[rhoId]);
      	gmudif[sinId][rhoId][deltaId]->SetLineWidth(2);
      	gmudif[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
      	gmudif[sinId][rhoId][deltaId]->SetLineStyle(style[rhoId]);
      } //delta
    } //rho
  }//sin                                                                                                                                                                            

  if(DianaIsLazy){
    for(int sinId = 0; sinId<sinTot; sinId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){
	
      	std::string thisLabel = "numu__"+deltaLabel[deltaId]+"_sin"+sinLabel[sinId];
        TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#nu_{#mu} #rightarrow #nu_{#mu})", 60, 1., 2., 100, -0.01, 1.01);
        CenterTitles(axes);
        axes->Draw();
	
        gPad->SetFillStyle(0);
	
        for(int rhoId = 0; rhoId < rhoTot; rhoId++){
          gmumu[sinId][rhoId][deltaId]->Draw("l same");
        }
	
        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("#rho","C");
        leg->SetFillStyle(0);
        for(int rhoId = 0; rhoId < rhoTot; rhoId++){
          leg->AddEntry(gmumu[sinId][rhoId][deltaId], Form("%s", rhoLabel[rhoId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);
	
        //gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }
  


  if(DianaIsLazy){
    for(int sinId = 0; sinId<sinTot; sinId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){

      	std::string thisLabel = "numubar__"+deltaLabel[deltaId]+"_sin"+sinLabel[sinId];
        TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#mu})", 60, 1., 2., 100, -0.01, 1.01);
        CenterTitles(axes);
        axes->Draw();

        gPad->SetFillStyle(0);

        for(int rhoId = 0; rhoId < rhoTot; rhoId++){
          gmumubar[sinId][rhoId][deltaId]->Draw("l same");
        }

        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("#rho","C");
        leg->SetFillStyle(0);
        for(int rhoId = 0; rhoId < rhoTot; rhoId++){
          leg->AddEntry(gmumubar[sinId][rhoId][deltaId], Form("%s", rhoLabel[rhoId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);

        gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }


  if(DianaIsLazy){
    for(int sinId = 0; sinId<sinTot; sinId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){

      	std::string thisLabel = "signdif__"+deltaLabel[deltaId]+"_sin"+sinLabel[sinId];
      	TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#nu_{#mu}) - P(#bar{#nu}_{#mu})", 60, 1., 2., 100, -0.001, 0.01);
        CenterTitles(axes);
        axes->Draw();

        gPad->SetFillStyle(0);

        for(int rhoId = 0; rhoId < rhoTot; rhoId++){
          gmudif[sinId][rhoId][deltaId]->Draw("l same");
        }

        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("#rho","C");
        leg->SetFillStyle(0);
        for(int rhoId = 0; rhoId < rhoTot; rhoId++){
          leg->AddEntry(gmudif[sinId][rhoId][deltaId], Form("%s", rhoLabel[rhoId].c_str()), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);

        gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }
  */


  // ================
  // var sin2theta23 
  // ================
  for(int sinId = 0; sinId<sinTot; sinId++){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){
        gmumu[sinId][rhoId][deltaId]->SetLineWidth(2);
        gmumu[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
        gmumu[sinId][rhoId][deltaId]->SetLineStyle(style[rhoId]);
        gmumubar[sinId][rhoId][deltaId]->SetLineWidth(2);
        gmumubar[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
        gmumubar[sinId][rhoId][deltaId]->SetLineStyle(style[rhoId]);
        gmudif[sinId][rhoId][deltaId]->SetLineWidth(2);
        gmudif[sinId][rhoId][deltaId]->SetLineColor(color[sinId]);
        gmudif[sinId][rhoId][deltaId]->SetLineStyle(style[rhoId]);
      } //delta
    } //rho
  }//sin

  if(DianaIsLazy){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){

      	std::string thisLabel = "numu__"+deltaLabel[deltaId]+"_rho"+rhoLabel[rhoId];
      	TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#nu_{#mu} #rightarrow #nu_{#mu})", 60, .0, 3., 100, -0.01, 1.01);
      	CenterTitles(axes);
      	axes->Draw();
      	
      	gPad->SetFillStyle(0);
      	
      	for(int sinId = 0; sinId<sinTot; sinId++){
      	  gmumu[sinId][rhoId][deltaId]->Draw("l same");
      	}
      	
      	TLegend* leg = new TLegend(0.7, 0.45, 0.9, 0.85);  
      	leg->SetHeader("sin^{2}#theta_{23}","C");
      	leg->SetFillStyle(0);
      	for(int sinId = 0; sinId<sinTot; sinId++){
      	  leg->AddEntry(gmumu[sinId][rhoId][deltaId], Form("%s", sinLabel[sinId].c_str()), "l");
      	} 
      	leg->Draw("same");
      	//CornerLabel(thisLabel);
      	
      	//gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
      	gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }



  if(DianaIsLazy){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){

      	std::string thisLabel = "numubar__"+deltaLabel[deltaId]+"_rho"+rhoLabel[rhoId];
        TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#mu})", 60, .0, 3., 100, -0.01, 1.01);
        CenterTitles(axes);
        axes->Draw();

        gPad->SetFillStyle(0);

        for(int sinId = 0; sinId<sinTot; sinId++){
          gmumubar[sinId][rhoId][deltaId]->Draw("l same");
        }

        TLegend* leg = new TLegend(0.7, 0.45, 0.9, 0.85);
        leg->SetHeader("sin^{2}#theta_{23}","C");
        leg->SetFillStyle(0);
        for(int sinId = 0; sinId<sinTot; sinId++){
          leg->AddEntry(gmumubar[sinId][rhoId][deltaId], Form("%s", sinLabel[sinId].c_str()), "l");
        }
        leg->Draw("same");
        //CornerLabel(thisLabel);

        //gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }


  if(DianaIsLazy){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      for(int deltaId = 0; deltaId<deltaTot; deltaId++){

      	std::string thisLabel = "signdif__"+deltaLabel[deltaId]+"_rho"+rhoLabel[rhoId]+"";
      	TH2* axes = new TH2F("", ";Neutrino energy (GeV); P(#nu_{#mu}) - P(#bar{#nu}_{#mu})", 60, 0., 3., 100, -0.01, 0.04);
        CenterTitles(axes);
        axes->Draw();
        gPad->SetFillStyle(0);
      	gmudif[3][0][deltaId]->Draw("l same");
      	gmudif[3][1][deltaId]->Draw("l same");
      	gmudif[3][2][deltaId]->Draw("l same");
        gmudif[3][3][deltaId]->Draw("l same");
      	gmudif[3][4][deltaId]->Draw("l same");
      	gmudif[3][5][deltaId]->Draw("l same");
        for(int sinId = 0; sinId<sinTot; sinId++){
          gmudif[sinId][rhoId][deltaId]->Draw("l same");
        }

      	TLegend* legrho = new TLegend(0.55, 0.50, 0.73, 0.85);
        legrho->SetHeader("#rho (gr/cm^{3})","L");
        legrho->SetFillStyle(0);
      	legrho->AddEntry(gmudif[3][0][deltaId], "0.0", "l");
      	legrho->AddEntry(gmudif[3][1][deltaId], "2.8", "l");
      	legrho->AddEntry(gmudif[3][2][deltaId], "4.0", "l");
      	legrho->AddEntry(gmudif[3][3][deltaId], "5.0", "l");
      	legrho->AddEntry(gmudif[3][4][deltaId], "6.0", "l");
      	legrho->AddEntry(gmudif[3][5][deltaId], "7.0", "l");
      	legrho->Draw("same");
        TLegend* leg = new TLegend(0.75, 0.45, 0.9, 0.85);
        leg->SetHeader("sin^{2}#theta_{23}","L");
        leg->SetFillStyle(0);
        for(int sinId = 0; sinId<sinTot; sinId++){
          leg->AddEntry(gmudif[sinId][rhoId][deltaId], Form("%s", sinLabel[sinId].c_str()), "l");
        }
        leg->Draw("same");
        //CornerLabel(thisLabel);

        //gPad->Print(Form("oscprob__%s.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s.pdf",thisLabel.c_str()));
      }
    }
  }


}

