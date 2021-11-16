/////////////////////////////////////////////////////////////////////////////////
// CovarianceMatrix.cxx
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, April 2021
//
// Class to make and handle covariances matrices encoding bin-to-bin correlations
// between different systematics and data samples.
// Adapted from NOvA CAFAna framework.
/////////////////////////////////////////////////////////////////////////////////

// framework
#include "CAFAna/Prediction/CovarianceMatrix.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "OscLib/OscCalcSterile.h"

// root
#include "TH1D.h"
#include "TRandom3.h"
#include "TDirectory.h"
#include "TString.h"
#include "TMatrixD.h"

// stl
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <sstream>

using std::back_inserter;
using std::begin;
using std::cout;
using std::end;
using std::endl;
using std::get;
using std::make_pair;
using std::make_tuple;
using std::make_unique;
using std::ostringstream;
using std::pair;
using std::runtime_error;
using std::transform;
using std::unique_ptr;
using std::vector;

using Eigen::SelfAdjointEigenSolver;
using Eigen::Map;
using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace ana {

  namespace covmx {

    //-------------------------------------------------------------------------
    vector<Component> GetComponents(Sample sample) {

      vector<Component> ret;

      // Define vectors of flavor and sign
      vector<Flavors::Flavors_t> flavors = { Flavors::kNuEToNuE,
					     Flavors::kNuMuToNuMu };
      if (sample.detector == kFarDet) {
	vector<Flavors::Flavors_t> fdFlavors = { Flavors::kNuEToNuMu,
						 Flavors::kNuEToNuTau,
						 Flavors::kNuMuToNuE,
						 Flavors::kNuMuToNuTau };
	flavors.insert(end(flavors), begin(fdFlavors), end(fdFlavors));
      }
      vector<Sign::Sign_t> signs = { Sign::kNu, Sign::kAntiNu };

      // Add each combination of flavor and sign
      for (auto & flavor: flavors) {
	for (auto & sign: signs) {
	  ret.push_back(make_tuple(flavor, Current::kCC, sign));
	} // for flavor
      } // for sign

      // Add the neutral currents
      ret.push_back(make_tuple(Flavors::kAll, Current::kNC, Sign::kBoth));
      return ret;

    } // function GetComponents

    //-------------------------------------------------------------------------
    CovarianceMatrix::CovarianceMatrix(vector<Sample> samples,
				       vector<const ISyst*> systs, int nUniverses)
      : fNUniverses(nUniverses) {
      BuildFullCovMx(samples, systs);
    } // function CovarianceMatrix::CovarianceMatrix

    //-------------------------------------------------------------------------
    CovarianceMatrix::CovarianceMatrix(MatrixXd fullCovMx,
				       MatrixXd covMxAbsolute, vector<pair<Binning, unsigned int>> bins)
      : fBins(bins), fFullCovMx(fullCovMx), fCovMxAbsolute(covMxAbsolute),
	fNUniverses(1e5) {
    } // CovarianceMatrix constructor

    //-------------------------------------------------------------------------
    CovarianceMatrix::CovarianceMatrix(MatrixXd fullCovMx,
				       vector<Sample> samples)
      : fFullCovMx(fullCovMx), fNUniverses(1e5) {
      SetBinning(samples);

      unsigned int nBins = GetBinning().NBins();
      fCovMxAbsolute.resize(nBins, nBins);
      fCovMxAbsolute.setZero();

    } // CovarianceMatrix constructor

    //-------------------------------------------------------------------------
    /// \brief Constructor that makes an empty matrix from given bins
    CovarianceMatrix::CovarianceMatrix(vector<Sample> samples)
      : fNUniverses(1e5) {
      SetBinning(samples);

      // Create empty covariance matrices
      unsigned int nBinsFull = GetFullBinning().NBins();
      fFullCovMx.resize(nBinsFull, nBinsFull);
      fFullCovMx.setZero();
      unsigned int nBins = GetBinning().NBins();
      fCovMxAbsolute.resize(nBins, nBins);
      fCovMxAbsolute.setZero();

    } // CovarianceMatrix constructor

    //----------------------------------------------------------------------
    void CovarianceMatrix::BuildFullCovMx(vector<Sample> samples,
					  vector<const ISyst*> systs) {

      osc::IOscCalcAdjustable* calc = new osc::OscCalcSterileTrivial();

      // Set matrix binning
      if (!fBins.empty()) {
	throw runtime_error("Cannot reset matrix binning!");
      }
      for (Sample s: samples) {
	Binning bins = s.GetPrediction()->Predict(calc).GetBinnings()[0];
	unsigned int nComps = GetComponents(s).size();
	fBins.push_back(make_pair(bins, nComps));
      }

      // Instantiate empty matrices
      vector<Binning> bins = GetBinnings();
      unsigned int nBinsFull = GetFullBinning().NBins();
      fFullCovMx.resize(nBinsFull, nBinsFull);
      fFullCovMx.setZero();
      unsigned int nBins = GetBinning().NBins();
      fCovMxAbsolute.resize(nBins, nBins);
      fCovMxAbsolute.setZero();
      vector<double> nominalPrediction(nBinsFull);
      vector<double> fractionalShift(nBinsFull);

      size_t i = 0;
      // Loop over samples
      for (size_t iPred = 0; iPred < samples.size(); ++iPred) {
	// Loop over components
	for (auto& component: GetComponents(samples[iPred])) {
	  // Get spectrum and use it to set nominal prediction
	  Spectrum spec
	    = samples[iPred].GetPrediction()->PredictComponent(calc,
							       get<0>(component), get<1>(component), get<2>(component));
	  TH1D* hist = spec.ToTH1(samples[iPred].GetPOT());
	  for (size_t iBin = 1; iBin <= (size_t)bins[iPred].NBins(); ++iBin) {
	    nominalPrediction[i] = hist->GetBinContent(iBin);
	    std::cout << "Nominal prediction for bin " << i << " is " << nominalPrediction[i] << std::endl;
	    ++i;
	  }
	  delete hist;
	}
      }

      TRandom3 rnd(0);

      Progress progress("Constructing the full covariance matrix");
      SystShifts shifts;

      // Randomly throw many universes with systematic shifts
      for (Long64_t iThrow = 0; iThrow < fNUniverses; ++iThrow) {
	progress.SetProgress(double(iThrow)/fNUniverses);

	// Set random Gaussian shifts for all systematics
	for (const ISyst* syst : systs) {
	  double shift = rnd.Gaus();
	  const KeySyst* tmp = dynamic_cast<const KeySyst*>(syst); // Cast to key syst for one-sidedness
	  if (tmp && tmp->IsOneSided()) shift = fabs(shift);
	  shifts.SetShift(syst, shift);
	}

	// Loop over components
	i = 0;
	for (size_t iPred = 0; iPred < samples.size(); ++iPred) {
	  SystShifts sampleShifts = samples[iPred].GetSystShifts(shifts);
	  auto comps = covmx::GetComponents(samples[iPred]);
	  for (auto& component : comps) {
	    Spectrum shiftedSpec
	      = samples[iPred].GetPrediction()->PredictComponentSyst(calc,
								     sampleShifts,
								     get<0>(component), get<1>(component), get<2>(component));
	    auto shiftedHist = shiftedSpec.ToTH1(samples[iPred].GetPOT());

	    // Get fractional shift in each energy bin
	    for (size_t iBin = 1; iBin <= (size_t)bins[iPred].NBins(); ++iBin) {

	      // Get systematic-shifted prediction
	      double shiftedPrediction = shiftedHist->GetBinContent(iBin);
	      if (std::isnan(shiftedPrediction))
		throw runtime_error("NaN value found in shifted prediction.");

	      // Set fractional shift for this bin
	      if (nominalPrediction[i] > 0) {
		fractionalShift[i] = ((shiftedPrediction+0.01)/(nominalPrediction[i]+0.01)) - 1;
		std::cout << "Fractional shift for bin " << i << " is " << fractionalShift[i] << std::endl;
	      }
	      else
		fractionalShift[i] = 0;
	      ++i;
	    }
	    delete shiftedHist;
	  } // for prediction
	} // for component

	// Add fractional shifts to covariance matrix
	for (size_t iBin = 0; iBin < nBinsFull; ++iBin) {
	  for (size_t jBin = 0; jBin < nBinsFull; ++jBin) {
	    fFullCovMx(iBin, jBin) += fractionalShift[iBin]*fractionalShift[jBin]*(1.0/(fNUniverses));
	  }
	}

      } // for universes

      progress.Done();

      delete calc;

    } // function CovarianceMatrix::BuildFullCovMx

    //-------------------------------------------------------------------------
    MatrixXd CovarianceMatrix::Predict(vector<Sample> samples,
				       osc::IOscCalcAdjustable* calc,
				       const SystShifts& shifts) {
      DontAddDirectory guard;

      fCovMxAbsolute.setZero();

      // Prepare oscillated prediction vector
      size_t nBinsFull = GetFullBinning().NBins();
      vector<double> oscPrediction(nBinsFull);

      size_t i = 0;
      for (size_t iPred = 0; iPred < samples.size(); ++iPred) {
	auto comps = covmx::GetComponents(samples[iPred]);
	for (auto& component : comps) {
	  SystShifts sampleShifts = samples[iPred].GetSystShifts(shifts);
	  Spectrum spec
	    = samples[iPred].GetPrediction()->PredictComponentSyst(calc,
								   sampleShifts,
								   get<0>(component), get<1>(component), get<2>(component));
	  TH1D* hist = spec.ToTH1(samples[iPred].GetPOT());
	  for (int iBin = 1; iBin <= spec.GetBinnings()[0].NBins(); ++iBin) {
	    oscPrediction[i] = hist->GetBinContent(iBin);
	    ++i;
	  }
	  delete hist;
	} // for component
      } // for sample

      // Loop over prediction, component & bin in i
      size_t iOffset(0), iFullOffset(0);
      vector<unsigned int> nComps = GetNComponents();
      for (size_t iPred = 0; iPred < samples.size(); ++iPred) {
	size_t iBins = fBins[iPred].first.NBins();
	for (size_t iComp = 0; iComp < nComps[iPred]; ++iComp) {
	  // Loop over prediction, component & bin in j
	  for (size_t iBin = 0; iBin < iBins; ++iBin) {
	    size_t jOffset(0), jFullOffset(0);
	    for (size_t jPred = 0; jPred < samples.size(); ++jPred) {
	      size_t jBins = fBins[jPred].first.NBins();
	      for (size_t jComp = 0; jComp < nComps[jPred]; ++jComp) {
		for (size_t jBin = 0; jBin < jBins; ++jBin) {
		  size_t i(iOffset+iBin), iFull(iFullOffset+iBin),
		    j(jOffset+jBin), jFull(jFullOffset+jBin);
		  fCovMxAbsolute(i, j) += fFullCovMx(iFull, jFull) * oscPrediction[iFull] * oscPrediction[jFull];
		} // for jBin
		jFullOffset += jBins;
	      } // for jComp
	      jOffset += jBins;
	    } // for jPred
	  } // for iBin
	  iFullOffset += iBins;
	} // for iComp
	iOffset += iBins;
      }// for iPred

      return fCovMxAbsolute;

    } // function CovarianceMatrix::Predict

    //----------------------------------------------------------------------
    MatrixXd CovarianceMatrix::Predict(vector<double> vBetaMu) {
      DontAddDirectory guard;

      MatrixXd ret(vBetaMu.size(), vBetaMu.size());
      ret.setZero();
      size_t iOffset(0), iFullOffset(0);
      vector<unsigned int> nComps = GetNComponents();
      for (size_t iPred = 0; iPred < fBins.size(); ++iPred) {
	size_t iBins = fBins[iPred].first.NBins();
	for (size_t iComp = 0; iComp < nComps[iPred]; ++iComp) {
	  // Loop over prediction, component & bin in j
	  for (size_t iBin = 0; iBin < iBins; ++iBin) {
	    size_t jOffset(0), jFullOffset(0);
	    for (size_t jPred = 0; jPred < fBins.size(); ++jPred) {
	      size_t jBins = fBins[jPred].first.NBins();
	      for (size_t jComp = 0; jComp < nComps[jPred]; ++jComp) {
		for (size_t jBin = 0; jBin < jBins; ++jBin) {
		  size_t i(iOffset+iBin), iFull(iFullOffset+iBin),
		    j(jOffset+jBin), jFull(jFullOffset+jBin);
		  ret(i, j) += fFullCovMx(iFull, jFull) * vBetaMu[iFull] * vBetaMu[jFull];
		} // for jBin
		jFullOffset += jBins;
	      } // for jComp
	      jOffset += jBins;
	    } // for jPred
	  } // for iBin
	  iFullOffset += iBins;
	} // for iComp
	iOffset += iBins;
      }// for iPred

      return ret;

    } // function CovarianceMatrix::Predict

    //----------------------------------------------------------------------
    void CovarianceMatrix::AddMatrix(CovarianceMatrix* gen) {

      unsigned int nBins = GetBinning().NBins();
      unsigned int nBinsComp = gen->GetBinning().NBins();
      unsigned int nBinsFull = GetFullBinning().NBins();
      unsigned int nBinsFullComp = gen->GetFullBinning().NBins();

      // Sanity check
      if (nBins != nBinsComp || nBinsFull != nBinsFullComp) {
	throw runtime_error("Cannot add matrices; bins do not match.");
      }

      fFullCovMx += gen->GetFullCovMx();
      fCovMxAbsolute += gen->GetCovMxAbsolute();

    } // function CovarianceMatrix::AddMatrix

    //----------------------------------------------------------------------
    MatrixXd CovarianceMatrix::ForcePosDef(MatrixXd mx, double epsilon) {

      SelfAdjointEigenSolver<MatrixXd> es(mx);
      MatrixXd eigenvectors = es.eigenvectors();
      MatrixXd eigenvalues = es.eigenvalues().asDiagonal();

      for (int i = 0; i < eigenvalues.rows(); ++i)
	if (eigenvalues(i, i) < epsilon)
	  eigenvalues(i, i) = epsilon;

      return eigenvectors * eigenvalues * eigenvectors.transpose();

    } // function CovarianceMatrix::ForcePosDef

    //---------------------------------------------------------------------------
    vector<Binning> CovarianceMatrix::GetBinnings() {

      vector<Binning> ret;
      transform(begin(fBins), end(fBins), back_inserter(ret),
		[](auto const& pair){ return pair.first; });
      return ret;

    } // function CovarianceMatrix::GetBinnings

    //---------------------------------------------------------------------------
    Binning CovarianceMatrix::GetBinning() {  

      // Concatenate together all the individual binning schemes
      vector<double> mergedBins = { 0. };
      vector<Binning> bins = GetBinnings();
      for (Binning b : bins) {
	for (size_t i = 1; i < b.Edges().size(); ++i) {
	  double width = b.Edges()[i] - b.Edges()[i-1];
	  mergedBins.push_back(mergedBins.back() + width);
	} // for bin
      } // for sample
      return Binning::Custom(mergedBins);

    } // function CovarianceMatrix::GetBinning

    //---------------------------------------------------------------------------
    Binning CovarianceMatrix::GetFullBinning() {

      // Concatenate together all the binning schemes component-wise
      vector<double> mergedBins = { 0. };
      auto bins = fBins;
      for (auto const& b : bins) {
	vector<double> edges = b.first.Edges();
	for (size_t iComp = 0; iComp < b.second; ++iComp) {
	  for (size_t iBin = 1; iBin < edges.size(); ++iBin) {
	    double width = edges[iBin] - edges[iBin-1];
	    mergedBins.push_back(mergedBins.back() + width);
	  } // for iBin
	} // for iComp
      } // for sample
      return Binning::Custom(mergedBins);

    } // function CovarianceMatrix::GetFullBinning

    //---------------------------------------------------------------------------
    vector<unsigned int> CovarianceMatrix::GetNBins() {

      vector<unsigned int> ret;
      transform(begin(fBins), end(fBins), back_inserter(ret),
		[](auto const& pair){ return pair.first.NBins(); });
      return ret;

    } // function CovarianceMatrix::GetNBins

    //---------------------------------------------------------------------------
    vector<unsigned int> CovarianceMatrix::GetNComponents() {

      vector<unsigned int> ret;
      transform(begin(fBins), end(fBins), back_inserter(ret),
		[](auto const& pair){ return pair.second; });
      return ret;

    } // function CovarianceMatrix::GetNComponents

    //---------------------------------------------------------------------------
    MatrixXd CovarianceMatrix::GetCovMxRelative(vector<Sample>& samples,
						osc::IOscCalcAdjustable* calc) {

      Predict(samples, calc);
      MatrixXd ret(fCovMxAbsolute);
      int c1 = 0;
      for (Sample& s1 : samples) {
	TH1D* h1 = s1.GetPrediction()->Predict(calc).ToTH1(s1.GetPOT());
	for (int b1 = 1; b1 <= h1->GetNbinsX(); ++b1) {
	  double iVal = h1->GetBinContent(b1);
	  int c2 = 0;
	  for (Sample& s2 : samples) {
	    TH1D* h2 = s2.GetPrediction()->Predict(calc).ToTH1(s2.GetPOT());
	    for (int b2 = 1; b2 <= h2->GetNbinsX(); ++b2) {
	      double jVal = h2->GetBinContent(b2);
	      ret(c1, c2) /= iVal * jVal;
	      ++c2;
	    } // for bin b2
	    delete h2;
	  } // for sample s2
	  ++c1;
	} // for bin b1
	delete h1;
      } // for sample s1

      return ret;

    } // function CovarianceMatrix::GetCovMxRelative

    //----------------------------------------------------------------------
    MatrixXd CovarianceMatrix::GetFullCorrMx() {

      // Populate the histogram with the elements of the full correlation
      // matrix
      MatrixXd corrMx = fFullCovMx;
      corrMx.setZero();
      for (int i = 0; i < corrMx.rows(); ++i) {
	for (int j = 0; j < corrMx.cols(); ++j) {
	  double cov = fFullCovMx(i, j);
	  double sigma_1 = fFullCovMx(i, i);
	  double sigma_2 = fFullCovMx(j, j);
	  if (sigma_1 > 0 && sigma_2 > 0) {
	    corrMx(i, j) = cov/(sqrt(sigma_1) * sqrt(sigma_2));
	  } else {
	    corrMx(i, j) = 0;
	  }
	}
      }

      return corrMx;

    } // function CovarianceMatrix::GetFullCorrMx

    //----------------------------------------------------------------------
    TH2D* CovarianceMatrix::GetFullCovMxTH2() {

      DontAddDirectory guard;

      // Get full binning and create histogram
      Binning bins = GetFullBinning();
      size_t nBins = bins.NBins();
      vector<double> edges = bins.Edges();
      TH2D* hist = new TH2D(UniqueName().c_str(),
			    ";Reco E bin (components);Reco E bin (components)",
			    nBins, &edges[0], nBins, &edges[0]);

      // Populate histogram from matrix
      for (size_t i = 0; i < nBins; ++i) {
	for (size_t j = 0; j < nBins; ++j) {
	  hist->SetBinContent(i+1, j+1, fFullCovMx(i,j));
	}
      }
      return hist;

    } // function CovarianceMatrix::GetFullCovMxTH2

    //----------------------------------------------------------------------
    TH2D* CovarianceMatrix::GetCovMxAbsoluteTH2() {

      DontAddDirectory guard;

      // Get full binning and create histogram
      Binning bins = GetBinning();
      size_t nBins = bins.NBins();
      vector<double> edges = bins.Edges();
      TH2D* hist = new TH2D(UniqueName().c_str(),
			    ";Reco E bin;Reco E bin",
			    nBins, &edges[0], nBins, &edges[0]);

      // Populate histogram from matrix
      for (size_t i = 0; i < nBins; ++i) {
	for (size_t j = 0; j < nBins; ++j) {
	  hist->SetBinContent(i+1, j+1, fCovMxAbsolute(i,j));
	}
      }
      return hist;

    } // function CovarianceMatrix::GetOscCovMxAbsTH2

    //----------------------------------------------------------------------
    TH2D* CovarianceMatrix::GetCovMxRelativeTH2(vector<Sample>& samples,
						osc::IOscCalcAdjustable* calc) {

      DontAddDirectory guard;

      // Get full binning and create histogram
      Binning bins = GetBinning();
      size_t nBins = bins.NBins();
      vector<double> edges = bins.Edges();
      TH2D* hist = new TH2D(UniqueName().c_str(),
			    ";Reco E bin;Reco E bin",
			    nBins, &edges[0], nBins, &edges[0]);

      // Populate histogram from matrix
      MatrixXd mx = GetCovMxRelative(samples, calc);
      for (size_t i = 0; i < nBins; ++i) {
	for (size_t j = 0; j < nBins; ++j) {
	  hist->SetBinContent(i+1, j+1, mx(i,j));
	}
      }
      return hist;

    } // function CovarianceMatrix::GetOscCovMxRelTH2

    //----------------------------------------------------------------------
    TH2D* CovarianceMatrix::GetCorrMxTH2() {

      DontAddDirectory guard;

      Binning mxBins = GetBinning();
      TH2D* hist = new TH2D(UniqueName().c_str(),
			    ";Reco E bin;Reco E bin",
			    mxBins.NBins(), &mxBins.Edges()[0],
			    mxBins.NBins(), &mxBins.Edges()[0]);

      for (int i = 0; i < mxBins.NBins(); ++i) {
	for (int j = 0; j < mxBins.NBins(); ++j) {
	  double cov = fCovMxAbsolute(i,j);
	  double sigma_1 = fCovMxAbsolute(i,i);
	  double sigma_2 = fCovMxAbsolute(j,j);
	  if (sigma_1 > 0 && sigma_2 > 0)
	    hist->SetBinContent(i+1, j+1, cov/(sqrt(sigma_1)*sqrt(sigma_2)));
	  else hist->SetBinContent(i+1, j+1, 0);
	}
      }
      return hist;

    } // function CovarianceMatrix::GetCorrMxTH2

    //----------------------------------------------------------------------
    TH2D* CovarianceMatrix::GetFullCorrMxTH2() {

      DontAddDirectory guard;

      // Get the binning of the full matrix, and then instantiate an empty
      // 2D histogram
      Binning mxBins = GetFullBinning();
      TH2D* hist = new TH2D(UniqueName().c_str(),
			    ";Reco E bin (components);Reco E bin (components)",
			    mxBins.NBins(), &mxBins.Edges()[0],
			    mxBins.NBins(), &mxBins.Edges()[0]);

      // Populate the histogram with the elements of the full correlation
      // matrix
      MatrixXd corrMx = GetFullCorrMx();
      for (int i = 0; i < mxBins.NBins(); ++i) {
	for (int j = 0; j < mxBins.NBins(); ++j) {
	  hist->SetBinContent(i+1, j+1, corrMx(i, j));
	}
      }

      return hist;

    } // function CovarianceMatrix::GetFullCorrMxTH2

    //-------------------------------------------------------------------------
    void CovarianceMatrix::SetBinning(vector<Sample> samples) {

      // Binning only gets set once, don't let it be reset
      if (!fBins.empty()) {
	throw runtime_error("Cannot reset matrix binning!");
      }

      // Loop over samples and fill binning scheme & number of beam
      // components for each one
      for (Sample& s : samples) {
	fBins.push_back(make_pair(s.GetBinning(),
				  GetComponents(s).size()));
      }

    } // function CovarianceMatrix::SetBinning

    //----------------------------------------------------------------------
    void CovarianceMatrix::SaveTo(TDirectory* dir,
				  const std::string& name) const {

      TDirectory* tmp = gDirectory;

      dir = dir->mkdir(name.c_str()); // switch to subdir
      dir->cd();

      TObjString("CovarianceMatrix").Write("type");

      TMatrixD fullCovMx(fFullCovMx.rows(), fFullCovMx.cols(),
			 fFullCovMx.data());
      fullCovMx.Write("fullcovmx");
      TMatrixD covMxAbsolute(fCovMxAbsolute.rows(), fCovMxAbsolute.cols());
      for (int i = 0; i < fCovMxAbsolute.rows(); ++i) {
	for (int j = 0; j < fCovMxAbsolute.cols(); ++j) {
	  covMxAbsolute(i, j) = fCovMxAbsolute(i, j);
	}
      }
      covMxAbsolute.Write("covmxabsolute");

      // Save binning information
      vector<vector<double>> edges(fBins.size());
      vector<unsigned int> nComps(fBins.size(), 0);
      for (size_t it = 0; it < fBins.size(); ++it) {
	edges[it] = fBins[it].first.Edges();
	nComps[it] = fBins[it].second;
      }
      dir->WriteObject(&edges, "binedges");
      dir->WriteObject(&nComps, "ncomps");

      dir->Write();
      delete dir;
      tmp->cd();

    } // function CovarianceMatrix::SaveTo

    //----------------------------------------------------------------------                                                               
    unique_ptr<CovarianceMatrix> CovarianceMatrix::LoadFrom(TDirectory* dir,
							    const std::string& name) {

      dir = dir->GetDirectory(name.c_str()); // switch to subdir
      assert(dir); 

      TObjString* tag = (TObjString*)dir->Get("type");
      assert(tag);
      assert(tag->GetString() == "CovarianceMatrix");

      // Load matrices
      TMatrixD* tmp = (TMatrixD*)dir->Get("fullcovmx");
      assert(tmp);
      MatrixXd fullCovMx = Map<MatrixXd>(tmp->GetMatrixArray(),
					 tmp->GetNrows(), tmp->GetNcols());
      delete tmp;
      tmp = (TMatrixD*)dir->Get("covmxabsolute");
      assert(tmp);
      MatrixXd covMxAbsolute = Map<MatrixXd>(tmp->GetMatrixArray(),
					     tmp->GetNrows(), tmp->GetNcols());
      delete tmp;

      // Load binning information
      vector<vector<double>>* edges;
      vector<unsigned int>* nComps;
      vector<pair<Binning, unsigned int>> bins;
      dir->GetObject("binedges", edges);
      dir->GetObject("ncomps", nComps);
      for (size_t it = 0; it < edges->size(); ++it) {
	bins.push_back(make_pair(Binning::Custom(edges->at(it)),
				 nComps->at(it)));
      }

      unique_ptr<CovarianceMatrix> ret(make_unique<CovarianceMatrix>
				       (fullCovMx, covMxAbsolute, bins));

      delete dir;
      return ret;

    } // function CovarianceMatrix::LoadFrom

  } // namespace covmx

} // namespace ana
