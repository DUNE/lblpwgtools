#include "common_fit_definitions.C"

void bin_splines(std::string stateFname="common_state_mcc11v3.root",
		 std::string outputFname="bin_splines_mcc11v3.root"){
  
  gROOT->SetBatch(1);
  
  // Get the prediction interpolators
  std::vector<unique_ptr<PredictionInterp> > return_list = GetPredictionInterps(stateFname, GetListOfSysts());
  PredictionInterp& predInterpFDNumuFHC = *return_list[0].release();
  PredictionInterp& predInterpFDNueFHC  = *return_list[1].release();
  PredictionInterp& predInterpFDNumuRHC = *return_list[2].release();
  PredictionInterp& predInterpFDNueRHC  = *return_list[3].release();
  PredictionInterp& predInterpNDNumuFHC = *return_list[4].release();
  PredictionInterp& predInterpNDNumuRHC = *return_list[5].release();

  // Get the systematics that the PredictionInterps use
  std::vector<const ISyst*> systlist = predInterpFDNumuFHC.GetAllSysts();

  // There must be a better way
  std::vector<std::tuple<std::string, PredictionInterp*, double> > sampleList = {
    {"FD_FHC_numu", &predInterpFDNumuFHC, pot_fd},
    {"FD_RHC_numu", &predInterpFDNumuRHC, pot_fd},
    {"FD_FHC_nue", &predInterpFDNueFHC, pot_fd},
    {"FD_RHC_nue", &predInterpFDNueRHC, pot_fd},
    {"ND_FHC", &predInterpNDNumuFHC, pot_nd},
    {"ND_RHC", &predInterpNDNumuRHC, pot_nd}   
  };

  // Open 
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  osc::IOscCalculatorAdjustable* inputOsc = NuFitOscCalc(+1);

  // Loopy loopy loopy loop
  // Loop over the samples
  for (auto & sample : sampleList){    

    // ARGH, why aren't they self-aware?! :'(
    double this_pot = std::get<2>(sample);
    
    // Get a dummy spectrum for binning...
    TH1* fNom = std::get<1>(sample)->PredictSyst(inputOsc, kNoShift).FakeData(this_pot).ToTH1ProjectX(this_pot);
    fNom->SetName("nom");
    TAxis* xBins = fNom->GetXaxis();

    // Do this by hand because GetArray had some weird issues...
    double xBinArr[xBins->GetNbins()+1];
    for (int x=0; x<xBins->GetNbins(); ++x){
      xBinArr[x] = xBins->GetBinLowEdge(x+1);
    }
    xBinArr[xBins->GetNbins()]=xBins->GetBinUpEdge(xBins->GetNbins()+1);
    
    // Loop over the systematics and make dial validations for each one
    for (auto & syst : systlist){

      // Name for this bin spline
      std::string thisName = std::get<0>(sample)+"_"+syst->ShortName();
      std::cout << "Having a bash at: " << thisName << std::endl;
      
      // Make a new TH2D
      TH2D* thisSyst = new TH2D(thisName.c_str(), (thisName+"; E_{#nu} (GeV); Rate syst. shift/nominal").c_str(),
				xBins->GetNbins(), xBinArr, 300, -3, 3);
	
      // Loop over points 
      for (int yBin=0; yBin<thisSyst->GetNbinsY(); ++yBin){
	
	SystShifts thisShift(syst, thisSyst->GetYaxis()->GetBinCenter(yBin));
	// Actually calculate the bin content
	 TH1* thisMod = std::get<1>(sample)->PredictSyst(inputOsc, thisShift).FakeData(this_pot).ToTH1ProjectX(this_pot);
	 thisMod->SetName("thisMod");
	 
	// Need to loop over the bins here...
	for (int xBin=0; xBin<thisSyst->GetNbinsX(); ++xBin){
	  double nomVal = fNom->GetBinContent(xBin+1);
	  double modVal = 0;
	  if (nomVal != 0) modVal = thisMod->GetBinContent(xBin+1)/nomVal;
	  thisSyst->SetBinContent(xBin+1, yBin+1, modVal);
	}
	delete thisMod;
      }
      // Save the histogram, then give up
      thisSyst->Write();
      delete thisSyst;
    }
    delete fNom;
    // Now do OA parameters!
    // Either as a slice, or do between some limits...
  }
  
  fout->Close();
}

