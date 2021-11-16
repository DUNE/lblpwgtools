/////////////////////////////////////////////////////////////////////////////////
// CovarianceMatrix.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, April 2021
//
// Class to make and handle covariances matrices encoding bin-to-bin correlations
// between different systematics and data samples.
// Adapted from NOvA CAFAna framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

// framework
#include "CAFAna/Core/Sample.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Systs/CovMxSysts.h"
#include "CAFAna/Prediction/IPrediction.h"
#include "OscLib/IOscCalc.h"

// root
#include "TH2D.h"
#include "TObjString.h"

// stl
#include <memory>
#include <vector>
#include <tuple>

#include <Eigen/Dense>

namespace ana {

  namespace covmx {

    typedef std::tuple<Flavors::Flavors_t, Current::Current_t, Sign::Sign_t> Component;
    std::vector<Component> GetComponents(Sample sample);

    /// A class for generating a covariance matrices as a function of oscillation parameters and systematics as nuisance parameters
    class CovarianceMatrix {
    public:

      CovarianceMatrix(std::vector<Sample> samples, std::vector<const ISyst*> systs,
		       int nUniverses=1e5);

      CovarianceMatrix(Eigen::MatrixXd fullCovMx, Eigen::MatrixXd covMxAbsolute,
		       std::vector<std::pair<Binning, unsigned int>> bins);

      CovarianceMatrix(Eigen::MatrixXd fullCovMx, std::vector<Sample> samples);

      CovarianceMatrix(std::vector<Sample> samples);

      virtual ~CovarianceMatrix(){}

      Eigen::MatrixXd Predict(std::vector<Sample> samples,
			      osc::IOscCalcAdjustable* calc,
			      const SystShifts& systs=SystShifts::Nominal());

      Eigen::MatrixXd Predict(std::vector<double> vBetaMu);

      // Add matrices together
      void AddMatrix(CovarianceMatrix* gen);

      static Eigen::MatrixXd ForcePosDef(Eigen::MatrixXd mx, double epsilon=0.1);
      static TMatrixD ForcePosDefROOT(Eigen::MatrixXd mx, double epsilon=0.1);

      // So many different types of matrix...
      Eigen::MatrixXd GetFullCovMx() { return fFullCovMx; }
      Eigen::MatrixXd GetCovMxAbsolute() { return fCovMxAbsolute; }
      Eigen::MatrixXd GetCovMxRelative(std::vector<Sample>& samples,
				       osc::IOscCalcAdjustable* calc);
      Eigen::MatrixXd GetFullCorrMx();

      void SetName(std::string name) { fName = name; }
      std::string GetName() { return fName; }

      std::vector<covmx::Sample> GetSamples();
      std::vector<Binning> GetBinnings();
      Binning GetBinning();
      Binning GetFullBinning();
      std::vector<unsigned int> GetNBins();
      std::vector<unsigned int> GetNComponents();

      void SetNUniverses(unsigned int i) { fNUniverses = i; }

      // Full covariance matrix
      TH2D* GetFullCovMxTH2();

      // Oscillated covariance matrix
      TH2D* GetCovMxAbsoluteTH2();
      TH2D* GetCovMxRelativeTH2(std::vector<Sample>& samples,
				osc::IOscCalcAdjustable* calc);

      TH2D* GetCorrMxTH2();
      TH2D* GetFullCorrMxTH2();

      virtual void SaveTo(TDirectory* dir, const std::string& name) const;
      static std::unique_ptr<CovarianceMatrix> LoadFrom(TDirectory* dir, const std::string& name);

      CovarianceMatrix() = delete;

    protected:

      CovarianceMatrix(Eigen::MatrixXd fullCovMx, Eigen::MatrixXd covMxAbsolute,
		       std::vector<std::vector<double>> edges, std::vector<unsigned int> comps);

      void BuildFullCovMx(std::vector<Sample> samples,
			  std::vector<const ISyst*> systs);

      void SetBinning(std::vector<Sample> samples);

      std::string fName; ///< String name identifier
      std::vector<std::pair<Binning, unsigned int>> fBins;

      Eigen::MatrixXd fFullCovMx; ///< Full covariance matrix
      Eigen::MatrixXd fCovMxAbsolute; ///< Oscillated absolute covariance matrix

      unsigned int fNUniverses; ///< Number of universes to simulate

    private:

      friend class CovMxManager;

    };

  } // namespace covmx

} // namespace ana
