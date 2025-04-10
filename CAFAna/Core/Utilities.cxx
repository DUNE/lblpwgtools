// Need to get a stan include in before the eigen ones in the header
#include "CAFAna/Core/Spectrum.h"

#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/Ratio.h"

#include "CAFAna/Core/MathUtil.h"

#include "TArrayD.h"
#include "TClass.h"
#include "TDirectory.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TMatrixD.h"
#include "TObjString.h"
#include "TString.h"
#include "TVector3.h"
#include "TVectorD.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include "sys/stat.h"
#include "wordexp.h"
#include <dirent.h>

namespace ana
{
  double LLPerBinFracSystErr::fgErr = -1;

  //----------------------------------------------------------------------
  FloatingExceptionOnNaN::FloatingExceptionOnNaN(bool enable)
  {
    // Don't want any pending FPEs to trigger when we flip exceptions
    // on. Whoever had them off previously had a reason.
    feclearexcept(FE_INVALID);

    fegetexceptflag(&fBackup, FE_INVALID);

#ifndef DARWINBUILD
    if(enable)
      feenableexcept(FE_INVALID);
    else
      fedisableexcept(FE_INVALID);
#else
    static bool first = true;
    if(first){
      std::cerr << "WARNING: CAFAna/Core/Utilities.cxx built on OS X, no feenableexcept available" << std::endl;
      first = false;
    }
#endif
  }

  //----------------------------------------------------------------------
  FloatingExceptionOnNaN::~FloatingExceptionOnNaN()
  {
    fesetexceptflag(&fBackup, FE_INVALID);
  }

  //----------------------------------------------------------------------
  // Reduce code duplication between CalcCovMx and CalcBiasMx
  Eigen::ArrayXd GetBinMeans(const std::vector<Eigen::ArrayXd>& binSets)
  {
    const unsigned int nBins(binSets.front().size());
    const unsigned int nUniv(binSets.size());

    Eigen::ArrayXd binMeans = Eigen::ArrayXd::Zero(nBins);
    for(unsigned int univIdx = 0; univIdx < nUniv; ++univIdx)
      binMeans += binSets[univIdx];

    binMeans *= 1./(nUniv);

    return binMeans;
  }

   //----------------------------------------------------------------------
  Eigen::MatrixXd CalcCovMx(const std::vector<Eigen::ArrayXd>& binSets)
  {
    if (binSets.size() < 2)
      return Eigen::MatrixXd(0,0);

    const unsigned int nUniv(binSets.size());
    const unsigned int nBins(binSets.front().size());

    const Eigen::ArrayXd binMeans = GetBinMeans(binSets);

    Eigen::MatrixXd covmx = Eigen::MatrixXd::Zero(nBins, nBins);

    for(unsigned int univIdx = 0; univIdx < nUniv; ++univIdx)
    {
      // Get a column vector of the difference from the mean in each bin
      Eigen::MatrixXd errs = (binSets[univIdx] - binMeans).matrix();

      // Calculate the variance from column * row
      covmx += errs * errs.transpose();
    } // for (univIdx)

    // now divide by N-1 to get sample covariance
    covmx *= 1./(nUniv-1);

    return covmx;
  }
  //----------------------------------------------------------------------
  Eigen::MatrixXd CalcBiasMx(const Eigen::ArrayXd& nom, const std::vector<Eigen::ArrayXd>& binSets)
  {
    if (binSets.size() < 2)
      return Eigen::MatrixXd(0,0);

    const Eigen::ArrayXd binMeans = GetBinMeans(binSets);

    // Get a column vector of the difference from the mean in each bin
    Eigen::MatrixXd errs = (nom - binMeans).matrix();

    // Calculate the variance from column * row
    return errs * errs.transpose();
  }

  //----------------------------------------------------------------------
  double LogLikelihood(const TH1* eh, const TH1* oh, bool useOverflow)
  {
    assert(eh->GetNbinsX() == oh->GetNbinsX());

    double chi = 0;

    int bufferBins = useOverflow? 2 : 1;

    for(int i = 0; i < eh->GetNbinsX()+bufferBins; ++i){
      const double e = eh->GetBinContent(i);
      const double o = oh->GetBinContent(i);

      chi += LogLikelihood(e, o);
    }

    return chi;
  }

  //----------------------------------------------------------------------
  double LogLikelihood(const Eigen::ArrayXd& ea, const Eigen::ArrayXd& oa, bool useOverflow)
  {
    assert(ea.size() == oa.size());

    double chi = 0;

    const int bufferBins = useOverflow ? 0 : -1;

    for(int i = 0; i < ea.size()+bufferBins; ++i){
      chi += LogLikelihood(ea[i], oa[i]);
    }

    return chi;
  }

  //----------------------------------------------------------------------
  Eigen::MatrixXd EigenMatrixXdFromTMatrixD(const TMatrixD* mat)
  {
    Eigen::MatrixXd ret(mat->GetNrows(), mat->GetNcols());
    // TMatrixD doesn't appear to have a GetArray()
    for(int i = 0; i < mat->GetNrows(); ++i){
      for(int j = 0; j < mat->GetNcols(); ++j){
        ret.coeffRef(i, j) = (*mat)(i, j);
      }
    }
    return ret;
  }

  //----------------------------------------------------------------------
  TMatrixD TMatrixDFromEigenMatrixXd(const Eigen::MatrixXd& mat)
  {
    TMatrixD ret(mat.rows(), mat.cols());
    // TMatrixD doesn't appear to have a GetArray()
    for(int i = 0; i < mat.rows(); ++i){
      for(int j = 0; j < mat.cols(); ++j){
        ret(i, j) = mat.coeffRef(i, j);
      }
    }
    return ret;
  }

  //----------------------------------------------------------------------
  TH2F * EigenMatrixToTH2(Eigen::MatrixXd m)
  {
    TH2F* h = new TH2F("", "", m.rows(), 0, m.rows(),  m.cols() , 0 ,  m.cols() );  
    for (int x = 0 ; x< m.rows();x++)
      for (int y = 0 ; y< m.rows();y++)
        h->SetBinContent(x,y,m(x,y));

    return h;
  }
  //----------------------------------------------------------------------
  double Chi2CovMx(const Eigen::ArrayXd& e, const Eigen::ArrayXd& o, const Eigen::MatrixXd& covmxinv)
  {
    assert(e.size() == covmxinv.rows()+2);

    if(e.size() != o.size()){
      std::cout << "Chi2CovMx() mismatched expectation and observed sizes: " << e.size() << " vs " << o.size() << std::endl;
      abort();
    }

    if(e.size() != covmxinv.rows()+2){
      std::cout << "Chi2CovMx() expected " << e.size()-2 << "x" << e.size()-2 << " covariance matrix. Got " << covmxinv.rows() << "x" << covmxinv.cols() << std::endl;
      abort();
    }

    Eigen::ArrayXd diff = e-o;
    // Drop underflow and overflow bins
    const Eigen::ArrayXd diffSub(Eigen::Map<Eigen::ArrayXd>(diff.data()+1, diff.size()-2));
    // dot collapses things down to a single number
    return diffSub.matrix().dot(covmxinv*diffSub.matrix());
  }

  //----------------------------------------------------------------------
  double LogLikelihoodCovMx(const Eigen::ArrayXd& e,
                            const Eigen::ArrayXd& o,
                            const Eigen::MatrixXd& M,
                            std::vector<double>* hint)
  {
    // Don't use under/overflow bins (the covariance matrix doesn't have them)
    const double* m0 = e.data()+1;
    const double* d = o.data()+1;
    const unsigned int N = e.size()-2;

    assert(M.rows() == int(N));

    // We're trying to solve for the best expectation in each bin 'm'

    // if no hint is provided, use this as our working area
    std::vector<double> localm;
    // The hint is hopefully our m's from a similar problem that was previously
    // posed, which should be a good starting point.
    std::vector<double>& mv = hint ? *hint : localm;
    // If not...
    if(mv.size() != N){
      mv.resize(N);
      // A good seed value is the nominal MC
      for(unsigned int i = 0; i < N; ++i) mv[i] = m0[i];
    }
    double* m = mv.data();

    double prev = -999;
    double ret = 0;

    // Normally converges in ~200 iterations
    for(int n = 0; n < 1000; ++n){
      // The derivatives of the chisq are quadratic functions, so it's not easy
      // to solved for all the variables simultaneously. Instead, we iterate
      // through the m's and solve them holding all the others fixed, and
      // repeat until we converge.
      for(unsigned int k = 0; k < N; ++k){
        if(m0[k] == 0) continue;
        // Coefficients for the quadratic formula for this term
        const double a = M(k, k);
        double b = 1 - m0[k]*M(k, k);
        for(unsigned int i = 0; i < N; ++i){
          if(m0[i] == 0) continue;
          if(i != k){
            b += (m[i]-m0[i])*M(i, k);
          }
        }
        const double c = -d[k];

        if(a == 0){
          m[k] = -c/b;
        }
        else{
          const double desc = b*b-4*a*c;
          assert(desc >= 0);
          // Empirically the other solution is always negative
          m[k] = ( -b + sqrt(desc) ) / (2*a);
          //          assert(( -b - sqrt(desc) ) / (2*a) < 0);
        }

        // Only physically meaningful to have a positive prediction (or
        // potentially zero is OK if there's zero data). If this goes wrong it
        // probably means your input matrix is badly formed.
        assert(m[k] > 0 || (d[k] == 0 && m[k] == 0));
      } // end for k

      // Update the chisq
      prev = ret;
      ret = 0;
      // There's the LL of the data to the updated prediction...
      for(unsigned int i = 0; i < N; ++i) ret += LogLikelihood(m[i], d[i]);

      // ...plus the penalty the prediction picks up from its covariance
      for(unsigned int i = 0; i < N; ++i)
        for(unsigned int j = 0; j < N; ++j)
          ret += (m[i]-m0[i]) * M(i, j) * (m[j]-m0[j]);

      // If the updates didn't change anything at all then we're done
      if(ret == prev) return ret;
    } // end for n

    // We're most likely flipping between two extremely similar numbers at the
    // <1e-14 scale.
    if(fabs(ret-prev) < 1e-6){
      //      std::cout << "Warning: partially stalled LLCovMx "
      //                << fabs(ret-prev) << std::endl;
      return ret;
    }

    // If not, we have an actual problem. Print out the bins to give some hints
    std::cout << "LogLikelihoodCovMx stalled" << std::endl;
    for(unsigned int i = 0; i < N; ++i){
      std::cout << i << " " << d[i] << " " << m0[i] << " -> " << m[i] << " " << LogLikelihood(m[i], d[i]) << std::endl;
    }
    abort();
  }

  //----------------------------------------------------------------------
  TH2F* ExpandedHistogram(const std::string& title,
                          int nbinsx, double xmin, double xmax,
                          int nbinsy, double ymin, double ymax)
  {
    DontAddDirectory guard;

    // How wide the bins will be once we're done
    const double xwidth = (xmax-xmin)/(nbinsx-1);
    const double ywidth = (ymax-ymin)/(nbinsy-1);

    // Move the bin edges so that the limits occur at the centres
    xmin -= xwidth/2; ymin -= ywidth/2;
    xmax += xwidth/2; ymax += ywidth/2;

    return new TH2F(UniqueName().c_str(), title.c_str(),
                    nbinsx, xmin, xmax,
                    nbinsy, ymin, ymax);
  }


  //----------------------------------------------------------------------
  std::unique_ptr<TMatrixD> SymmMxInverse(const TMatrixD& mx)
  {
    // check if there are any null rows/columns.
    // if there are, they make the matrix singular.
    // we will remove them temporarily,
    // invert the matrix, then put them back afterwards.
    std::set<int> nullRows;
    for (auto row = mx.GetRowLwb(); row <= mx.GetRowUpb(); row++)
    {
      bool rowIsNull = true;
      for (auto col = mx.GetColLwb(); col <= mx.GetColUpb(); col++)
      {
        if (mx[row][col] != 0.)
        {
          rowIsNull = false;
          break;
        }
      }

      if (rowIsNull)
        nullRows.insert(row);
    }

    std::cerr << " Notice: covariance matrix '" << mx.GetName() << "' has " << nullRows.size() << " null rows.\n"
        << "They will be removed before inverting and added back afterwards." << std::endl;

    // create a new matrix for inverting, skipping the null rows
    auto invMx = std::make_unique<TMatrixD>(mx.GetRowLwb(), mx.GetRowUpb() - nullRows.size(),
                                           mx.GetColLwb(), mx.GetColUpb() - nullRows.size());
    unsigned int skippedRows = 0;
    for (auto row = mx.GetRowLwb(); row <= mx.GetRowUpb(); row++)
    {
      if (nullRows.find(row) != nullRows.end())
      {
        skippedRows++;
        continue;
      }
      unsigned int skippedCols = 0;
      for (auto col = mx.GetColLwb(); col <= mx.GetColUpb(); col++)
      {
        // since we assumed the matrix is symmetric,
        // we can just use the null rows list here
        if (nullRows.find(col) != nullRows.end())
        {
          skippedCols++;
          continue;
        }

        (*invMx)[col-skippedCols][row-skippedRows] = (*invMx)[row-skippedRows][col-skippedCols] = mx[row][col];
      }
    }

    invMx->Invert();

    // put back the empty rows if there were any
    if (nullRows.size())
    {
      skippedRows = 0;
      auto retMx = std::make_unique<TMatrixD>(mx.GetRowLwb(), mx.GetRowUpb(),
                                              mx.GetColLwb(), mx.GetColUpb());
      for (auto row = mx.GetRowLwb(); row <= mx.GetRowUpb(); row++)
      {
        if (nullRows.find(row) != nullRows.end())
        {
          skippedRows++;
          continue;
        }

        unsigned int skippedCols = skippedRows;
        for (auto col = row; col <= mx.GetColUpb(); col++)
        {
          if (nullRows.find(col) != nullRows.end())
          {
            skippedCols++;
            continue;
          }

          (*retMx)[col][row] = (*retMx)[row][col] = (*invMx)[row-skippedRows][col-skippedCols];
        }
      }

      return retMx;
    }

    return invMx;
  }

  //----------------------------------------------------------------------
  std::string FindCAFAnaDir()
  {
    #ifdef USE_SRT_CONTEXT
    const char* pub = getenv("SRT_PUBLIC_CONTEXT");
    const char* priv = getenv("SRT_PRIVATE_CONTEXT");

    if(priv && priv != std::string(".")){
      const std::string ret = std::string(priv)+"/CAFAna/";
      struct stat junk;
      if(stat(ret.c_str(), &junk) == 0) return ret;
    }
    assert(pub);
    return std::string(pub)+"/CAFAna/";
    #else
    const char *cafana = getenv("CAFANA");
    assert(cafana);
    struct stat junk;
    if(stat(cafana, &junk) == 0) {return cafana;}
    assert(false);
    return "";//to stop the compiler error.
    #endif
  }

  //----------------------------------------------------------------------
  std::vector<std::string> LoadFileList(const std::string& listfile)
  {
    std::vector<std::string> ret;

    std::ifstream is(listfile);
    if(!is.good()){
      std::cerr << "Can't open file list '" << listfile << "'. Aborting." << std::endl;
      abort();
    }

    while(!is.eof()){
      std::string fname;
      is >> fname;
      if(!fname.empty()) ret.push_back(fname);
    }
    return ret;
  }

  //----------------------------------------------------------------------
  std::map<std::string, std::string> GetCAFMetadata(TDirectory* dir)
  {
    std::map<std::string, std::string> ret;

    TIter next(dir->GetListOfKeys());
    while(TObject* key = next()){
      TObject* obj = dir->Get(key->GetName());
      assert(obj);

      const TString className = obj->ClassName();
      if(className == "TObjString"){
        ret[key->GetName()] = ((TObjString*)obj)->GetString();
      }
      else{
        std::cerr << "Unexpected object " << key->GetName() << " of type " << className << " while looking for metadata. Ignoring" << std::endl;
      }
    }

    return ret;
  }

  //----------------------------------------------------------------------
  void CombineMetadata(std::map<std::string, std::string>& base,
                       const std::map<std::string, std::string>& add,
                       std::set<std::string>& mask)
  {
    for(auto it: add){
      const std::string& key = it.first;

      // Needs special handling anyway, leave it blank.
      if(key == "parents") continue;

      // Accumulate the runs list
      if(key == "runs"){
        const std::string& r1 = base[key];
        const std::string& r2 = it.second;

        assert(!r2.empty());

        if(r1.empty()){
          base[key] = r2;
          continue;
        }

        // "[foo]" + "[bar]"
        std::string sum = r1+&r2[1]; // "[foo]bar]"
        sum[r1.size()-1] = ',';      // "[foo,bar]"
        base[key] = sum;
        continue;
      }

      if(base.find(key) == base.end()){
        // If it's new, add it
        base[key] = it.second;
      }
      else{
        if(key == "simulated.number_of_spills" ||
           key == "event_count" ||
           key == "online.totalevents"){
          // These two fields should be accumulated
          base[key] = TString::Format("%d",
                                      atoi(base[key].c_str()) +
                                      atoi(it.second.c_str())).Data();
        }
        else{
          // If it's a clash, record it
          if(base[key] != it.second) mask.insert(key);
        }
      }
    }
  }


  //----------------------------------------------------------------------
  void WriteCAFMetadata(TDirectory* dir,
                        const std::map<std::string, std::string>& meta)
  {
    TDirectory* tmp = gDirectory;
    dir->cd();

    for(auto it: meta){
      TObjString str(it.second.c_str());
      str.Write(it.first.c_str());
    }

    dir->Save();

    tmp->cd();
  }


  //----------------------------------------------------------------------
  bool RunningOnGrid()
  {
    return (getenv("_CONDOR_SCRATCH_DIR") != 0);
  }

  //----------------------------------------------------------------------
  FitToFourier::FitToFourier(TH1* h, double xlo, double xhi, int NOsc)
    : fHist(h), fxlo(xlo), fxhi(xhi), fNOsc(NOsc)
  {
  }

  //----------------------------------------------------------------------
  FitToFourier::~FitToFourier()
  {
  }

  //----------------------------------------------------------------------
  double FitToFourier::operator()(double *x, double *par) const
  {
    double x0 = x[0];
    double val = par[0];
    for (int i = 1; i <= fNOsc; i++)
      val += par[2*i-1]*sin(i*M_PI*x0) + par[2*i]*cos(i*M_PI*x0);
    return val;
  }

  //----------------------------------------------------------------------
  TF1* FitToFourier::Fit() const
  {
    double s[fNOsc];
    std::fill_n(s,fNOsc,0);
    double c[fNOsc];
    std::fill_n(c,fNOsc,0);
    
    int nBins = 0;
    for(int i = 1; i <= fHist->GetNbinsX(); ++i){
      const double x = M_PI * fHist->GetXaxis()->GetBinCenter(i);
      const double y = fHist->GetBinContent(i);

      if(y == 0) continue;
      ++nBins;

      for(int n = 0; n <= fNOsc; ++n){
        s[n] += y * sin(n*x);
        c[n] += y * cos(n*x);
      }
    }

    for(int n = 0; n <= fNOsc; ++n){
      s[n] *= 2./nBins;
      c[n] *= 2./nBins;
    }

    TF1* f = new TF1(UniqueName().c_str(), this, fxlo, fxhi, 2*fNOsc+1);

    f->SetParameter(0, c[0]/2);
    for(int n = 1; n <= fNOsc; ++n){
      f->SetParameter(n*2-1, s[n]);
      f->SetParameter(n*2,   c[n]);
    }

    // Because ROOT is having problems drawing f if I don't
    double min = fHist->GetMinimum();
    double max = fHist->GetMaximum();
    f->GetYaxis()->SetRangeUser(0.8*min, 1.2*max);
    return f;
  }

  //----------------------------------------------------------------------
  void EnsurePositiveDefinite(TH2* mat)
  {
    // Convert histogram to a proper matrix
    assert(mat->GetNbinsX() == mat->GetNbinsY());
    const int N = mat->GetNbinsX();
    TMatrixD m(N, N);
    for(int i = 0; i < N; ++i)
      for(int j = 0; j < N; ++j)
        m(i, j) = mat->GetBinContent(i+1, j+1);

    // Decompose it
    TVectorD evals;
    TMatrixD evecs = m.EigenVectors(evals);
    TMatrixD evalmat(N, N);
    // Force any negative eigenvalues slightly positive (floating point errors)
    for(int i = 0; i < N; ++i) evalmat(i, i) = std::max(1e-14, evals[i]);

    // Put the original matrix back together
    const TMatrixD evecs_inv(TMatrixD::kTransposed, evecs);
    m = evecs*evalmat*evecs_inv;

    // Decompose again to check for floating point problems
    m.EigenVectors(evals);
    for(int i = 0; i < N; ++i) assert(evals[i] > 0);

    // Copy the new matrix contents back into the histogram
    for(int i = 0; i < N; ++i)
      for(int j = 0; j < N; ++j)
        mat->SetBinContent(i+1, j+1, m(i, j));
  }

  //----------------------------------------------------------------------
  // Note that this does not work for 3D!
  Eigen::ArrayXd GetMaskArray(const Spectrum& s, double xmin, double xmax, double ymin, double ymax)
  {
    if (s.NDimensions() > 2){
      std::cout << "Error: unable to apply a mask in " << s.GetBinnings().size() << " dimensions" << std::endl;
      abort();
    }

    if(s.NDimensions() == 1 && ymax > ymin){
      std::cout << "Error: GetMaskArray(): can't specify y range for 1D spectrum" << std::endl;
      abort();
    }

    const Binning* xbins = &s.GetBinnings()[0];
    const Binning* ybins = (s.NDimensions() == 2) ? &s.GetBinnings()[1] : 0;

    const int Nx = xbins->NBins();
    const int Ny = ybins ? ybins->NBins() : 1;

    // The 1D flattening of 2D binning is pretty confusing. The bins are packed
    // densely, without under/overflow, *except* there is a single underflow at
    // 0 and single overflow at Nx*Ny+1. So we do our calculations as if there
    // were no under/overflow and then add 1 to the output index to account.

    Eigen::ArrayXd ret(Nx*Ny+2);

    // Include underflow and overflow if mask disabled, otherwise exclude
    ret[0] = ret[Nx*Ny+1] = ((xmin < xmax || ymin < ymax) ? 0 : 1);

    for(int i = 0; i < Nx*Ny; ++i){

      const int ix = i / Ny;
      const int iy = i % Ny;

      bool isMask = false;

      if (xmin < xmax){
	if (xbins->Edges()[ix  ] < xmin) isMask = true;
	if (xbins->Edges()[ix+1] > xmax) isMask = true;
      }

      if (ymin < ymax){
	if (ybins->Edges()[iy  ] < ymin) isMask = true;
	if (ybins->Edges()[iy+1] > ymax) isMask = true;
      }

      ret[i+1] = isMask ? 0 : 1;
    }

    return ret;
  }

  //----------------------------------------------------------------------
  double FindQuantile(double frac, std::vector<double>& xs)
  {
    // This turns out to be a much more fraught issue than you would naively
    // expect. This algorithm is equivalent to R-6 here:
    // https://en.wikipedia.org/wiki/Quantile#Estimating_quantiles_from_a_sample

    // In principle we could use std::nth_element(). Probably doesn't matter
    // much in practice since this is only for plotting.
    std::sort(xs.begin(), xs.end());

    const int N = xs.size();
    // The index we would ideally be sampling at
    const double h = frac*(N+1);
    // The indices on either side where we have to actually evaluate
    const unsigned int h0 = std::floor(h);
    const unsigned int h1 = std::ceil(h);
    if(h0 == 0) return xs[0]; // Don't underflow indexing
    if(h1 > xs.size()) return xs.back(); // Don't overflow indexing
    // The values at those indices
    const double x0 = xs[h0-1]; // wikipedia is using 1-based indexing
    const double x1 = xs[h1-1];

    if(h0 == h1) return x0;

    // Linear interpolation
    return (h1-h)*x0 + (h-h0)*x1;
  }
  
}


std::string EnsureTrailingSlash(std::string str) {
  if (!str.size()) {
    return str;
  }
  if (str.back() != '/') {
    return str + '/';
  }
  return str;
}

std::string parse_stdRegex_ErrorCode(std::regex_constants::error_type etype) {
  switch (etype) {
  case std::regex_constants::error_collate:
    return "error_collate: invalid collating element request";
  case std::regex_constants::error_ctype:
    return "error_ctype: invalid character class";
  case std::regex_constants::error_escape:
    return "error_escape: invalid escape character or trailing escape";
  case std::regex_constants::error_backref:
    return "error_backref: invalid back reference";
  case std::regex_constants::error_brack:
    return "error_brack: mismatched bracket([ or ])";
  case std::regex_constants::error_paren:
    return "error_paren: mismatched parentheses(( or ))";
  case std::regex_constants::error_brace:
    return "error_brace: mismatched brace({ or })";
  case std::regex_constants::error_badbrace:
    return "error_badbrace: invalid range inside a { }";
  case std::regex_constants::error_range:
    return "erro_range: invalid character range(e.g., [z-a])";
  case std::regex_constants::error_space:
    return "error_space: insufficient memory to handle this regular expression";
  case std::regex_constants::error_badrepeat:
    return "error_badrepeat: a repetition character (*, ?, +, or {) was not "
           "preceded by a valid regular expression";
  case std::regex_constants::error_complexity:
    return "error_complexity: the requested match is too complex";
  case std::regex_constants::error_stack:
    return "error_stack: insufficient memory to evaluate a match";
  default:
    return "";
  }
}

std::string DeGlobPattern(std::string const &pattern) {
  std::stringstream ss("");
  size_t next_asterisk = pattern.find_first_of('*');
  size_t next_to_add = 0;
  bool modified = false;
  while (next_asterisk != std::string::npos) {
    if ((pattern[next_asterisk - 1] !=
         ']') && // Try to allow valid uses of an asterisk without a preceding '.'
        (pattern[next_asterisk - 1] != '.')) {
      modified = true;
      // Add a .
      ss << pattern.substr(next_to_add, next_asterisk - next_to_add) << ".*";
      next_to_add = next_asterisk + 1;
      if (next_to_add >= pattern.size()) {
        next_to_add = std::string::npos;
      }
    }
    std::cout << "stuck" << std::endl;
    next_asterisk = pattern.find_first_of('*', next_to_add);
  }

  if (next_to_add != std::string::npos) {
    ss << pattern.substr(next_to_add);
  }

  if (modified) {
    std::cout << "[INFO]: DeGlobified input pattern: " << pattern
              << " to std::regex_friendly: " << ss.str() << std::endl;
  }

  return ss.str();
}

std::vector<std::string> GetMatchingFiles(std::string directory,
                                          std::string pattern,
                                          bool IncDir) {

  directory = EnsureTrailingSlash(directory);
  
  pattern = DeGlobPattern(pattern);

  std::cout << "[INFO]: Looking for files matching: \"" << pattern
            << "\" in directory: " << directory << std::endl;

  std::regex rpattern(pattern);

  std::vector<std::string> matches;

  DIR *dir;
  struct dirent *ent;
  dir = opendir(directory.c_str());
  if (dir != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (std::regex_match(ent->d_name, rpattern)) {
        matches.push_back(IncDir ? directory + std::string(ent->d_name)
                                 : std::string(ent->d_name));
      }
    }
  }
  return matches;
}
