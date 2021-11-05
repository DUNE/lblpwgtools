////////////////////////////////////////////////////////////////////
// Plots numu and numubar disappearance probabilities, and        //
// their difference for a set of mh and dcp in terms of sin2th23  //
//                                                                //
// useful to graphicaly see where the maximum dissapearance point //
// is deppending on the hierarchy and small dcp effect            //
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
Style_t lineNH = kSolid;
Style_t lineIH = kDotted;

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

void plot_probability_difftheta()
{

  std::vector<double> energyVal   = {1.625};
  std::vector<string> energyLabel = {"1625"};
  const int energyTot = energyVal.size();

  std::vector<double> deltaCPVal   = {0.0, 0.5, 1.0, 1.5};
  std::vector<string> deltaCPLabel = {"0.0","0.5", "1.0", "1.5"};
  const int deltaCPTot = deltaCPVal.size();

  std::vector<double> rhoVal   = {0.0, 2.8};
  std::vector<string> rhoLabel = {"0.0", "2.8"};
  const int rhoTot = rhoVal.size();

  std::vector<double> deltaVal = {0.00245, -0.00251};
  std::vector<string> deltaLabel = {"nh", "ih"};
  const int deltaTot = deltaVal.size();



  // styles = rhoTot
  // colors = sinTot
  std::vector<Style_t> style = {kSolid, kDotted, 3, 4};
  std::vector<Color_t> color = {kMagenta, kOrange+1, kGreen+1, kBlue-7};
  // kBlue+1, kGray+2, kCyan+1, kRed-4, kBlue-7};



  
  std::vector< std::vector < std::vector<osc::IOscCalcAdjustable*> > > calc;
  std::vector< std::vector < std::vector<TGraph*> > > gmumu;
  std::vector< std::vector < std::vector<TGraph*> > > gmumubar;
  std::vector< std::vector < std::vector<TGraph*> > > gmudif;
  std::vector< std::vector < std::vector<double> > > min_mumu;
  std::vector< std::vector < std::vector<double> > > min_mumubar;
  std::vector< std::vector < std::vector<double> > > min_mudif;

  for(int rhoId = 0; rhoId<rhoTot; rhoId++){
    
    std::vector< std::vector<osc::IOscCalcAdjustable*> > calcDouble;
    std::vector< std::vector<TGraph*> > gmumuDouble;
    std::vector< std::vector<TGraph*> > gmumubarDouble;
    std::vector< std::vector<TGraph*> > gmudifDouble;
    std::vector< std::vector<double> > min_mumuDouble;
    std::vector< std::vector<double> > min_mumubarDouble;
    std::vector< std::vector<double> > min_mudifDouble;

    for(int deltaId = 0; deltaId<deltaTot; deltaId++){
      std::vector<osc::IOscCalcAdjustable*> calcSingle;
      std::vector<TGraph*> gmumuSingle;
      std::vector<TGraph*> gmumubarSingle;
      std::vector<TGraph*> gmudifSingle;
      std::vector<double> min_mumuSingle;
      std::vector<double> min_mumubarSingle;
      std::vector<double> min_mudifSingle;

      for(int deltaCPId = 0; deltaCPId<deltaCPTot; deltaCPId++){
	
      	osc::IOscCalcAdjustable* tempCalc = DefaultOscCalc();
        ResetOscCalcToDefault(tempCalc);
        tempCalc->SetRho(rhoVal[rhoId]);
        tempCalc->SetDmsq32(deltaVal[deltaId]);
      	tempCalc->SetdCP(deltaCPVal[deltaCPId]*M_PI);
        calcSingle.push_back(tempCalc);

        gmumuSingle.push_back(new TGraph);
        gmumubarSingle.push_back(new TGraph);
        gmudifSingle.push_back(new TGraph);
      	min_mumuSingle.push_back(0.001);
        min_mumubarSingle.push_back(0.001);
        min_mudifSingle.push_back(0.001);

      } //deltaCP
 
      calcDouble.push_back(calcSingle);
      gmumuDouble.push_back( gmumuSingle );
      gmumubarDouble.push_back( gmumubarSingle );
      gmudifDouble.push_back( gmudifSingle );
      min_mumuDouble.push_back( min_mumuSingle );
      min_mumubarDouble.push_back( min_mumubarSingle );
      min_mudifDouble.push_back( min_mudifSingle );

    } //delta

    calc.push_back( calcDouble );
    gmumu.push_back( gmumuDouble );
    gmumubar.push_back( gmumubarDouble );
    gmudif.push_back( gmudifDouble );
    min_mumu.push_back( min_mumuDouble );
    min_mumubar.push_back( min_mumubarDouble );
    min_mudif.push_back( min_mudifDouble );

  } //rho


  double minSinVal = 0.49, maxSinVal = 0.53;
  double binWidthSin = 0.001;
  //for(int energyId = 0; energyId<energyTot; energyId++){
  for(int rhoId = 0; rhoId<rhoTot; rhoId++){
    for(int deltaId = 0; deltaId<deltaTot; deltaId++){
      for(int deltaCPId = 0; deltaCPId<deltaCPTot; deltaCPId++){
      	for(double sinVal = minSinVal; sinVal < maxSinVal; sinVal += binWidthSin){
	  
      	  calc[rhoId][deltaId][deltaCPId]->SetTh23(asin(sqrt(sinVal)));
      	  const double Pmumu    = calc[rhoId][deltaId][deltaCPId]->P(14, 14, energyVal[0]);
      	  const double Pmumubar = calc[rhoId][deltaId][deltaCPId]->P(-14, -14, energyVal[0]);
      	  const double Pmudif   = Pmumu - Pmumubar;
      	  
      	  gmumu[rhoId][deltaId][deltaCPId]->SetPoint(gmumu[rhoId][deltaId][deltaCPId]->GetN(), sinVal, Pmumu);
      	  gmumubar[rhoId][deltaId][deltaCPId]->SetPoint(gmumubar[rhoId][deltaId][deltaCPId]->GetN(), sinVal, Pmumubar);
      	  gmudif[rhoId][deltaId][deltaCPId]->SetPoint(gmudif[rhoId][deltaId][deltaCPId]->GetN(), sinVal, Pmudif);

      	} // for sin

      	min_mumu[rhoId][deltaId][deltaCPId]    = TMath::MinElement(gmumu[rhoId][deltaId][deltaCPId]->GetN(),gmumu[rhoId][deltaId][deltaCPId]->GetX());
      	min_mumubar[rhoId][deltaId][deltaCPId] = TMath::MinElement(gmumubar[rhoId][deltaId][deltaCPId]->GetN(),gmumubar[rhoId][deltaId][deltaCPId]->GetX());
      	min_mudif[rhoId][deltaId][deltaCPId]   = TMath::MinElement(gmudif[rhoId][deltaId][deltaCPId]->GetN(),gmudif[rhoId][deltaId][deltaCPId]->GetX());
      	min_mumu[rhoId][deltaId][deltaCPId]    = (min_mumu[rhoId][deltaId][deltaCPId]*binWidthSin)+minSinVal;
      	min_mumubar[rhoId][deltaId][deltaCPId] = (min_mumubar[rhoId][deltaId][deltaCPId]*binWidthSin)+minSinVal;
      	min_mudif[rhoId][deltaId][deltaCPId]   = (min_mudif[rhoId][deltaId][deltaCPId]*binWidthSin)+minSinVal;

      } //delta
    } //rho
  } //deltaCP
    //}


  // ==============
  // var rho
  // ==============
  for(int rhoId = 0; rhoId<rhoTot; rhoId++){
    for(int deltaId = 0; deltaId<deltaTot; deltaId++){
      for(int deltaCPId = 0; deltaCPId<deltaCPTot; deltaCPId++){
        gmumu[rhoId][deltaId][deltaCPId]->SetLineWidth(2);
        gmumu[rhoId][deltaId][deltaCPId]->SetLineColor(color[deltaCPId]);
        gmumu[rhoId][deltaId][deltaCPId]->SetLineStyle(style[deltaId]);
        gmumubar[rhoId][deltaId][deltaCPId]->SetLineWidth(2);
      	gmumubar[rhoId][deltaId][deltaCPId]->SetLineColor(color[deltaCPId]);
        gmumubar[rhoId][deltaId][deltaCPId]->SetLineStyle(style[deltaId]);
        gmudif[rhoId][deltaId][deltaCPId]->SetLineWidth(2);
        gmudif[rhoId][deltaId][deltaCPId]->SetLineColor(color[deltaCPId]);
        gmudif[rhoId][deltaId][deltaCPId]->SetLineStyle(style[deltaId]);
      } //delta
    } //delta
  }//rho



  TGraph * dummynh = new TGraph();
  TGraph * dummyih = new TGraph();
  dummynh->SetLineColor(kGray+2);
  dummynh->SetLineStyle(kSolid);
  dummynh->SetLineWidth(2);
  dummyih->SetLineColor(kGray+2);
  dummyih->SetLineStyle(kDotted);
  dummyih->SetLineWidth(2);

  TLegend* hie = new TLegend(0.35, 0.70, 0.60, 0.85);
  hie->SetFillStyle(0);
  hie->SetHeader("Hierarchy","L");
  hie->AddEntry(dummynh, "Normal", "l");
  hie->AddEntry(dummyih, "Inverted", "l");
  

  if(DianaIsLazy){
    
    //    for(int deltaId = 0; deltaId<deltaTot; deltaId++){
    for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      
      //std::string thisLabel = "numu__"+deltaLabel[deltaId]+"_rho"+rhoLabel[rhoId]+"_E"+energyLabel[0];
      std::string thisLabel = "numu__nhih_rho"+rhoLabel[rhoId]+"_E"+energyLabel[0];
      TH2* axes = new TH2F("", ";sin^{2}#theta_{23}; P(#nu_{#mu} #rightarrow #nu_{#mu})", 30, 0.495, 0.525, 100, -0.00001, 0.0008);
      CenterTitles(axes);
      axes->Draw();
      
      gPad->SetFillStyle(0);
      
      for(int deltaCPId = 0; deltaCPId < deltaCPTot; deltaCPId++){
      	gmumu[rhoId][0][deltaCPId]->Draw("l same");
      	gmumu[rhoId][1][deltaCPId]->Draw("l same");
      }
      
      TLegend* leg = new TLegend(0.55, 0.60, 0.65, 0.85);
      leg->SetHeader("#delta_{CP}","C");
      leg->SetFillStyle(0);
      for(int deltaCPId = 0; deltaCPId < deltaCPTot; deltaCPId++){
      	leg->AddEntry(gmumu[rhoId][0][deltaCPId], Form("%s #pi", deltaCPLabel[deltaCPId].c_str()), "l");
      }
      leg->Draw("same");
      hie->Draw("same");
      CornerLabel(thisLabel);
      
      //gPad->Print(Form("oscprob__%s__sinvsprob.png",thisLabel.c_str()));
      gPad->Print(Form("oscprob__%s__sinvsprob.pdf",thisLabel.c_str()));
    }
    //}//deltaId
  }
  
  if(DianaIsLazy){
    //for(int deltaId = 0; deltaId<deltaTot; deltaId++){   
      for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      
      	//std::string thisLabel = "numubar__"+deltaLabel[deltaId]+"_rho"+rhoLabel[rhoId]+"_E"+energyLabel[0];
      	std::string thisLabel = "numubar__nhih_rho"+rhoLabel[rhoId]+"_E"+energyLabel[0];
      	TH2* axes = new TH2F("", ";sin^{2}#theta_{23}; P(#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#mu})", 30, 0.495, 0.525, 100, -0.00001, 0.0008);
      	CenterTitles(axes);
      	axes->Draw();
      	
      	gPad->SetFillStyle(0);
	
      	for(int deltaCPId = 0; deltaCPId < deltaCPTot; deltaCPId++){
      	  gmumubar[rhoId][0][deltaCPId]->Draw("l same");
      	  gmumubar[rhoId][1][deltaCPId]->Draw("l same");
      	}
      	
      	TLegend* leg = new TLegend(0.55, 0.60, 0.65, 0.85);
      	leg->SetHeader("#delta_{CP}","C");
      	leg->SetFillStyle(0);
      	for(int deltaCPId = 0; deltaCPId < deltaCPTot; deltaCPId++){
      	  leg->AddEntry(gmumubar[rhoId][0][deltaCPId], Form("%s #pi", deltaCPLabel[deltaCPId].c_str()), "l");
      	}
      	hie->Draw("same");
      	leg->Draw("same");
      	CornerLabel(thisLabel);
      	
      	//gPad->Print(Form("oscprob__%s__sinvsprob.png",thisLabel.c_str()));
      	gPad->Print(Form("oscprob__%s__sinvsprob.pdf",thisLabel.c_str()));
      }
      //}//deltaId
  }
  
  if(DianaIsLazy){
   
    for(int deltaId = 0; deltaId<deltaTot; deltaId++){
      for(int rhoId = 0; rhoId<rhoTot; rhoId++){
      
        std::string thisLabel = "signdif__"+deltaLabel[deltaId]+"_rho"+rhoLabel[rhoId]+"_E"+energyLabel[0];
        TH2* axes = new TH2F("", ";sin^{2}#theta_{23}; P(#nu_{#mu}) - P(#bar{#nu}_{#mu})", 30, 0.50, 0.52, 100, -0.0005, 0.0005);
        CenterTitles(axes);
        axes->Draw();
        
        gPad->SetFillStyle(0);
        
        for(int deltaCPId = 0; deltaCPId < deltaCPTot; deltaCPId++){
      	  gmudif[rhoId][deltaId][deltaCPId]->Draw("l same");
        }
        
        TLegend* leg = new TLegend(0.7, 0.60, 0.9, 0.85);
        leg->SetHeader("#delta_{CP}, sin^{2}#theta_{23} min","C");
        leg->SetFillStyle(0);
        for(int deltaCPId = 0; deltaCPId < deltaCPTot; deltaCPId++){
        	leg->AddEntry(gmudif[rhoId][0][deltaCPId], Form("%s #pi, %.4f", deltaCPLabel[deltaCPId].c_str(), min_mumubar[rhoId][deltaId][deltaCPId]), "l");
        }
        leg->Draw("same");
        CornerLabel(thisLabel);
        
        gPad->Print(Form("oscprob__%s__sinvsprob.png",thisLabel.c_str()));
        gPad->Print(Form("oscprob__%s__sinvsprob.pdf",thisLabel.c_str()));
      }
    }
  }


}//the end
