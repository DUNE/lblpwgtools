#ifndef TH2JAGGED_IMPL_HXX_SEEN
#define TH2JAGGED_IMPL_HXX_SEEN

#include "TH1.h"
#include "TH2.h"

#include <map>
#include <string>
#include <vector>

class TH2Poly;

template <typename THT> struct TH2TypeTraits {};

template <> struct TH2TypeTraits<double> {
  using TH1Type = TH1D;
  using TH2Type = TH2D;
};

template <> struct TH2TypeTraits<float> {
  using TH1Type = TH1F;
  using TH2Type = TH2F;
};

template <> struct TH2TypeTraits<int> {
  using TH1Type = TH1I;
  using TH2Type = TH2I;
};

template <> struct TH2TypeTraits<char> {
  using TH1Type = TH1C;
  using TH2Type = TH2C;
};

struct JBinId {
  Int_t UniBin;
  Int_t NonUniBin;
};
bool operator<(JBinId const &l, JBinId const &r);

template <typename ST> class TH2Jagged : public TH2 {

  using T1T = typename TH2TypeTraits<ST>::TH1Type;
  using T2T = typename TH2TypeTraits<ST>::TH2Type;

public:
  std::map<JBinId, Int_t> fBinMappingToFlat;
  std::map<JBinId, Int_t> fBinMappingToNonFlowFlat;
  std::vector<Int_t> fBinMappingNonFlowToWithFlowFlat;

  TAxis fUniformAxis;
  std::vector<TAxis> fNonUniformAxes;
  std::vector<ST> fBinContent;
  std::vector<ST> fBinError;
  std::vector<ST> fBinSumW2;

  bool fXIsUniform;
  double fMinX, fMaxX;
  double fMinY, fMaxY;
  std::string fOTitle;

private:
  // Builds mappings between 'flat' and X/Y bin numbers
  void BuildBinMappings();

  template <typename T> T GetUniformAxisT(T x, T y) const {
    return fXIsUniform ? x : y;
  }

  template <typename T> T GetNonUniformAxisT(T x, T y) const {
    return fXIsUniform ? y : x;
  }

  template <typename T> T GetXAxisT(T u, T nu) const {
    return fXIsUniform ? u : nu;
  }

  template <typename T> T GetYAxisT(T u, T nu) const {
    return fXIsUniform ? nu : u;
  }

  bool CheckConsistency(const TH2Jagged *h);

protected:
  TH1D *DoProjection(bool onX, const char *name, Int_t firstbin, Int_t lastbin,
                     Option_t *option) const;

public:
  TH2Jagged();

  TH2Jagged(char const *name, char const *title, Int_t NUbins, Double_t UMin,
            Double_t UMax, Int_t const *NNUbins, Double_t const *NUMin,
            Double_t const *NUMax, bool XIsUniform);

  TH2Jagged(char const *name, char const *title, Int_t NUbins,
            Double_t const *UBinEdges, Int_t const *NNUbins,
            Double_t const **NUBinEdges, bool XIsUniform);

  TH2Jagged(char const *name, char const *title, Int_t NXbins, Double_t XMin,
            Double_t XMax, Int_t const *NYbins, Double_t const *YMin,
            Double_t const *YMax);

  TH2Jagged(char const *name, char const *title, Int_t const *NXbins,
            Double_t const *XMin, Double_t const *XMax, Int_t NYbins,
            Double_t YMin, Double_t YMax);

  TH2Jagged(char const *name, char const *title, Int_t NXbins,
            Double_t const *XBinEdges, Int_t const *NYbins,
            Double_t const **YBinEdges);

  TH2Jagged(char const *name, char const *title, Int_t const *NXbins,
            Double_t const **XBinEdges, Int_t NYbins,
            Double_t const *YBinEdges);

  // Shuts up compiler warning but pulls methods that will do nothing into scope
  using TH2::Fill;
  Int_t Fill(Double_t x, Double_t y);
  Int_t Fill(Double_t x, Double_t y, Double_t w);
  Int_t FillKnownBin(Int_t gbin, Double_t w);

  Int_t GetNbins() const;
  Int_t GetNbinsNonFlow() const;
  Int_t GetGBinFromNonFlowIndex(Int_t nfidx);

  Int_t GetBin(Int_t binx, Int_t biny, Int_t binz = 0) const;
  void GetBinXYZ(Int_t gbin, Int_t &binx, Int_t &biny, Int_t &binz) const;

  TAxis const *GetNonUniformAxis(Int_t gbin) const;

  // Shuts up compiler warning but pulls methods that will do nothing into scope
  using TH2::FindFixBin;
  Int_t FindFixBin(Double_t x, Double_t y, Double_t z = 0) const;
  // Gets the 1D bin number of a 2D bin which ignores flow bins.
  // N.B. This is not a gbin, this is useful for mapping non-flow bins defined
  // by their 2D position to a 1D global bin axis which ignores flow bins.
  Int_t FindFixBinNoFlow(Double_t x, Double_t y, Double_t z = 0) const;
  bool IsFlowBin(Int_t gbin) const;

  void Scale(Double_t c = 1, Option_t *option = "");

  // Shuts up compiler warning but pulls methods that will do nothing into scope
  using TH2::GetBinContent;
  Double_t GetBinContent(Int_t gbin) const;
  Double_t GetBinContent(Int_t binx, Int_t biny) const;

  // Shuts up compiler warning but pulls methods that will do nothing into scope
  using TH2::SetBinContent;
  void SetBinContent(Int_t gbin, ST c);
  void SetBinContent(Int_t binx, Int_t biny, ST c);

  // Shuts up compiler warning but pulls methods that will do nothing into scope
  using TH2::GetBinError;
  Double_t GetBinError(Int_t gbin) const;
  Double_t GetBinError(Int_t binx, Int_t biny) const;

  // Shuts up compiler warning but pulls methods that will do nothing into scope
  using TH2::SetBinError;
  void SetBinError(Int_t gbin, ST c);
  void SetBinError(Int_t binx, Int_t biny, ST c);

  void Reset(Option_t *option = "");
  void RecalculateErrors(Option_t *option = "");

  Double_t Integral(Option_t *option = "") const;

  // Shuts up compiler warning but pulls methods that will do nothing into scope
  using TH2::Add;
  Bool_t Add(const TH2Jagged<ST> *h1, Double_t c1 = 1);
  Bool_t Add(const TH2Jagged<ST> *h1, const TH2Jagged<ST> *h2, Double_t c1,
             Double_t c2);
  Bool_t Add(const TH1 *h1, Double_t c1 = 1);
  Bool_t Add(const TH1 *h1, const TH1 *h2, Double_t c1 = 1, Double_t c2 = 1);

  TObject *Clone(char const *newname = 0) const;
  void Draw(Option_t *option = "");

  T2T *ToUniformTH2(Option_t *option = "") const;
  TH2Poly *ToTH2Poly() const;

  T1T *NonUniformSlice(Int_t ubin) const;

  T1T *ToFlatTH1() const;
  void SetBinContentFromFlatTH1(T1T const *h);

  void ResetUniformAxis();

  virtual ~TH2Jagged() {}

  ClassDef(TH2Jagged, 1);
};

// This is required to get rootcling to generate the streamer information, but
// causes clang to emit errors due to template specialization after
// instantiation when compiling the dictionary so must only be visible to
// rootcling and not the compiler
#ifdef __CLING__
template class TH2Jagged<double>;
template class TH2Jagged<float>;
template class TH2Jagged<int>;
template class TH2Jagged<char>;
#endif

// These are the only specializations that will be built for, for others, add
// them above, to the linkdef and to the explicit specializations in the
// implementation file
using TH2JaggedD = TH2Jagged<double>;
using TH2JaggedF = TH2Jagged<float>;
using TH2JaggedI = TH2Jagged<int>;
using TH2JaggedC = TH2Jagged<char>;

TH2JaggedF *ToTHJaggedF(TH2JaggedD const *d, char const *newname = 0);

#endif
