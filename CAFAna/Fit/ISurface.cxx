#include "TCanvas.h"
#include "TGraph.h"
#include "TKey.h"
#include "TMarker.h"
#include "TH2.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TVectorD.h"

#include "OscLib/IOscCalc.h"

#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Fit/ISurface.h"

namespace ana
{

//---------------------------------------------------------------------
  void ISurface::Draw() const
  {
    EnsureAxes();

    fHist->Draw("colz same");

    // colz obliterated them
    gPad->RedrawAxis();

    gPad->Update();
  }

//---------------------------------------------------------------------
  void ISurface::DrawBestFit(Color_t color, Int_t marker) const
  {
    EnsureAxes();

    TMarker *mark = new TMarker(fBestFitX, fBestFitY, marker);
    mark->SetMarkerSize(1.5);
    mark->SetMarkerColor(color);
    mark->Draw();
    gPad->Update();
  }

//----------------------------------------------------------------------
  void ISurface::DrawContour(TH2 *fc, Style_t style, Color_t color,
                                  double minchi)
  {
    EnsureAxes();

    std::vector<TGraph *> gs = GetGraphs(fc, minchi);

    for (TGraph *g: gs)
    {
      g->SetLineWidth(3);//2);
      g->SetLineStyle(style);
      g->SetLineColor(color);
      g->Draw("l");
    }

    gPad->Update();
  }

  //---------------------------------------------------------------------
  void ISurface::EnsureAxes() const
  {
    // Could have a file temporarily open
    DontAddDirectory guard;

    // If this pad has already been drawn in, already has axes
    if (gPad && !gPad->GetListOfPrimitives()->IsEmpty()) return;

    // Old, hackier solution
    /*
    std::cout << gPad->GetListOfPrimitives()->GetEntries() << std::endl;
    // Which pads have we already drawn axes in? Never draw axes in them
    // again. Unfortunately UniqueID() never seems to be set. If that's the
    // case, set it to a random value and hope...
    static std::set<UInt_t> already;
    if(already.count(gPad->GetUniqueID())) return;
    if(gPad->GetUniqueID() == 0) gPad->SetUniqueID(rand());
    already.insert(gPad->GetUniqueID());
    */

    const TAxis *ax = fHist->GetXaxis();
    const TAxis *ay = fHist->GetYaxis();
    const double Nx = ax->GetNbins();
    const double Ny = ay->GetNbins();

    // Axes with limits where the user originally requested them, which we
    // adjusted to be the centres of the first and last bins.
    TH2 *axes = new TH2C(UniqueName().c_str(),
                         TString::Format(";%s;%s",
                                         ax->GetTitle(), ay->GetTitle()),
                         Nx - 1, ax->GetBinCenter(1), ax->GetBinCenter(Nx),
                         Ny - 1, ay->GetBinCenter(1), ay->GetBinCenter(Ny));
    axes->Draw();

    if(fHist){
      // "colz same" will reuse axis's min and max, so set them helpfully here
      axes->SetMinimum(fHist->GetMinimum());
      axes->SetMaximum(fHist->GetMaximum());
    }

    axes->SetTitle(fHist->GetTitle());
    axes->GetXaxis()->SetLabelSize(ax->GetLabelSize());
    axes->GetYaxis()->SetLabelSize(ay->GetLabelSize());
    axes->GetXaxis()->SetLabelOffset(ax->GetLabelOffset());
    axes->GetYaxis()->SetLabelOffset(ay->GetLabelOffset());
    axes->GetXaxis()->CenterTitle();
    axes->GetYaxis()->CenterTitle();
    gPad->Update();
  }

  //----------------------------------------------------------------------
  std::vector<TGraph*> ISurface::GetGraphs(TH2* fc, double minchi)
  {
    std::vector<TGraph*> ret;

    if(minchi < 0) minchi = fBestLikelihood;
    std::unique_ptr<TH2> surf((TH2*)fHist->Clone(UniqueName().c_str()));
    surf->Add(fc, -1);

    TVirtualPad* bak = gPad;

    const bool wasbatch = gROOT->IsBatch();
    gROOT->SetBatch(); // User doesn't want to see our temporary canvas
    TCanvas tmp;

    gStyle->SetOptStat(0);

    const double level = minchi-fBestLikelihood;
    surf->SetContour(1, &level);
    surf->Draw("cont list");

    tmp.Update();
    tmp.Paint();

    gROOT->SetBatch(wasbatch);
    gPad = bak;

    // The graphs we need (contained inside TLists, contained inside
    // TObjArrays) are in the list of specials. But we need to be careful about
    // types, because other stuff can get in here too (TDatabasePDG for
    // example).
    TCollection* specs = gROOT->GetListOfSpecials();

    TIter nextSpec(specs);
    while(TObject* spec = nextSpec()){
      if(!spec->InheritsFrom(TObjArray::Class())) continue;
      TObjArray* conts = (TObjArray*)spec;

      if(conts->IsEmpty()) continue;

      if(!conts->At(0)->InheritsFrom(TList::Class())) continue;
      TList* cont = (TList*)conts->At(0);

      TIter nextObj(cont);
      // Contour could be split into multiple pieces
      while(TObject* obj = nextObj()){
        if(!obj->InheritsFrom(TGraph::Class())) continue;

        ret.push_back((TGraph*)obj->Clone(UniqueName().c_str()));
      } // end for obj
    } // end for spec

    return ret;
  }

  //----------------------------------------------------------------------
  TH2 * ISurface::ToTH2(double minchi) const
  {
    // Could have a file temporarily open
    DontAddDirectory guard;

    TH2 *ret = new TH2F(*fHist);

    if (minchi >= 0)
    {
      for (int x = 0; x < ret->GetNbinsX() + 2; ++x)
      {
        for (int y = 0; y < ret->GetNbinsY() + 2; ++y)
        {
          ret->SetBinContent(x, y, ret->GetBinContent(x, y) + fBestLikelihood - minchi);
        }
      }
    }

    return ret;
  }

  //----------------------------------------------------------------------
  void ISurface::SetTitle(const char *str)
  {
    fHist->SetTitle(str);
  }

  //----------------------------------------------------------------------
  void ISurface::SaveToHelper(TDirectory *dir) const
  {
    TDirectory *oldDir = gDirectory;

    dir->cd();

    TVectorD v(3);
    v[0] = fBestLikelihood;
    v[1] = fBestFitX;
    v[2] = fBestFitY;
    v.Write("minValues");

    fHist->Write("hist");

    TVectorD s(fSeedValues.size(), &fSeedValues[0]);
    s.Write("seeds");

    if (oldDir)
      oldDir->cd();
  }

  //----------------------------------------------------------------------
  void ISurface::FillSurfObj(ISurface &surf, TDirectory *dir)
  {

    const TVectorD v = *(TVectorD *) dir->Get("minValues");
    const TVectorD s = *(TVectorD *) dir->Get("seeds");

    surf.fHist = (TH2F *) dir->Get("hist");
    surf.fBestLikelihood = v[0];
    surf.fBestFitX = v[1];
    surf.fBestFitY = v[2];

    for (int idx = 0; idx < s.GetNrows(); ++idx)
      surf.fSeedValues.push_back(s[idx]);

  }

  //----------------------------------------------------------------------
  /// Helper function for the gaussian approximation surfaces
  TH2* Flat(double level, const ISurface& s)
  {
    TH2* h = s.ToTH2();

    for(int x = 0; x < h->GetNbinsX()+2; ++x)
      for(int y = 0; y < h->GetNbinsY()+2; ++y)
        h->SetBinContent(x, y, level);

    return h;
  }


} // namespace ana
