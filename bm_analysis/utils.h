#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

namespace utils{

    struct RatioOpts {
    double rmin = 0.5;              // TRatio plot min range
    double rmax = 1.5;              // TRatio plot max range
    std::string varLabel = "Varied";
    std::string statFile = "";      // file where KS/AD are written
    bool dumpBins = false;          // if true, bin per bin printing
    };

    struct RatioOut {
    double N_n_raw = 0, err_n_raw = 0, N_v_raw = 0, err_v_raw = 0;
    double N_n_sc = 0, err_n_sc = 0, N_v_sc = 0, err_v_sc = 0;
    double R = 0, err_R = 0;
    };

    static int gCanvasId = 0;
    std::unique_ptr<TCanvas> NewCanvas()
    {
    const TString name = Form("c_%d", ++gCanvasId);
    return std::make_unique<TCanvas>(name, name);
    }

    TLegend* MakeLegend(double pot)
    {
    auto leg = new TLegend(0.65, 0.75, 0.88, 0.88);
    leg->AddEntry((TObject*)0, Form("POT = %.3g", pot), "");
    leg->SetTextFont(42);
    leg->SetTextSize(0.03);
    leg->SetBorderSize(0);
    leg->SetFillStyle(1);
    return leg;
    }

    void WriteStatTests(const std::string& path, TH1* h_n, TH1* h_v, double pot_spec_n, double pot_spec_v, double pot_n)
    {
    std::ofstream out(path, std::ios::app);

    // KS
    const double pKS = h_n->KolmogorovTest(h_v);
    const double dKS = h_n->KolmogorovTest(h_v, "M");
    const double pKSmc = h_n->KolmogorovTest(h_v, "X");
    const double pKSnorm = h_n->KolmogorovTest(h_v, "N");

    // AD
    const double pAD = h_n->AndersonDarlingTest(h_v);
    const double tAD = h_n->AndersonDarlingTest(h_v, "T");

    out << "=============== POT " << pot_n << " ===============" << std::endl;
    out << "Nominal POT: " << pot_spec_n << '\n';
    out << "Varied POT: "  << pot_spec_v << '\n';
    out << "Nominal POT used to rescale histograms: " << pot_n << '\n';
    out << "Nominal POT / Varied POT: " << pot_n / pot_spec_v << std::endl;
    out << "Nominal: int=" << h_n->Integral() << " entries=" << h_n->GetEntries()
        << " eff=" << h_n->GetEffectiveEntries() << '\n';
    out << "Varied: int="  << h_v->Integral() << " entries=" << h_v->GetEntries()
        << " eff=" << h_v->GetEffectiveEntries() << '\n';
    out << "Kolmogorov-Smirnov p-value: " << pKS
        << " Kolmogorov-Smirnov distance (D): " << dKS
        << " Kolmogorov-Smirnov pseudo-experiment: " << pKSmc
        << " Kolmogorov-Smirnov normalized: " << pKSnorm << std::endl;
    out << "Anderson-Darling p-value: " << pAD
        << " Anderson-Darling test statistic (T): " << tAD << std::endl;
    out << "=============================================" << std::endl;
    }

    void PrintBins(const TH1* h_n, const TH1* h_v)
    {
    std::cout << "\nbin  center[GeV]  N_n(sc)  err_n  N_v(sc)  err_v\n";
    for (int i = 1; i <= h_n->GetNbinsX(); ++i)
        std::cout << std::setw(3) << i << "  "
                << std::setw(8) << h_n->GetBinCenter(i) << "  "
                << std::setw(9) << h_n->GetBinContent(i) << "  "
                << std::setw(7) << h_n->GetBinError(i) << "  "
                << std::setw(9) << h_v->GetBinContent(i) << "  "
                << std::setw(7) << h_v->GetBinError(i) << "\n";
    std::cout << std::endl;
    }

    void PrintIntegrals(const RatioOut& r, const std::string& title, double pot_n)
    {
    std::cout << std::fixed << std::setprecision(2)
            << "\n=== " << title << ": integral events (with under/overflow) ===\n"
            << "Raw counts:\n"
            << "  nominal: " << r.N_n_raw << " +- " << r.err_n_raw << "\n"
            << "  varied : " << r.N_v_raw << " +- " << r.err_v_raw << "\n"
            << "Rescaled to nominal POT (" << std::scientific << pot_n << std::fixed << "):\n"
            << "  nominal: " << r.N_n_sc << " +- " << r.err_n_sc << "\n"
            << "  varied : " << r.N_v_sc << " +- " << r.err_v_sc << "\n"
            << "Ratio varied/nominal (rescaled): " << std::setprecision(4)
            << r.R << " +- " << r.err_R << std::endl;
    }

    RatioOut DrawRatio(const ana::Spectrum& s_n, const ana::Spectrum& s_v, double pot_n, const std::string& outpath, const RatioOpts& opt = RatioOpts())
    {
    auto c = NewCanvas();

    auto h_n = s_n.ToTH1(pot_n, kBlue);
    auto h_v = s_v.ToTH1(pot_n, kRed);

    if (h_n->GetSumw2N() == 0) h_n->Sumw2();
    if (h_v->GetSumw2N() == 0) h_v->Sumw2();
    h_n->SetLineWidth(2);
    h_v->SetLineWidth(2);

    auto h_n_raw = s_n.ToTH1(s_n.POT());
    auto h_v_raw = s_v.ToTH1(s_v.POT());

    auto ratio = new TRatioPlot(h_v, h_n);
    ratio->SetH1DrawOpt("hist E");
    ratio->SetH2DrawOpt("hist E");
    ratio->Draw();
    c->Update();
    ratio->GetLowerRefGraph()->SetMinimum(opt.rmin);
    ratio->GetLowerRefGraph()->SetMaximum(opt.rmax);
    ratio->GetLowYaxis()->SetNdivisions(505);

    ratio->GetUpperPad()->cd();
    auto leg = MakeLegend(pot_n);
    leg->AddEntry(h_n, Form("Nominal (%.0f ev)", h_n->Integral()), "le");
    leg->AddEntry(h_v, Form("%s (%.0f ev)", opt.varLabel.c_str(), h_v->Integral()), "le");
    leg->Draw();
    c->SaveAs(outpath.c_str());

    RatioOut r;
    const int nb = h_n->GetNbinsX();
    r.N_n_raw = h_n_raw->IntegralAndError(0, nb + 1, r.err_n_raw);
    r.N_v_raw = h_v_raw->IntegralAndError(0, nb + 1, r.err_v_raw);
    r.N_n_sc  = h_n->IntegralAndError(0, nb + 1, r.err_n_sc);
    r.N_v_sc  = h_v->IntegralAndError(0, nb + 1, r.err_v_sc); 

    r.R = r.N_v_sc / r.N_n_sc;
    r.err_R = r.R * std::sqrt(std::pow(r.err_v_sc / r.N_v_sc, 2) + std::pow(r.err_n_sc / r.N_n_sc, 2));

    if (opt.dumpBins) {PrintBins(h_n, h_v);}
    if (!opt.statFile.empty()) {WriteStatTests(opt.statFile, h_n, h_v, s_n.POT(), s_v.POT(), pot_n);}

    return r;
    }

    void DrawOverlay(const ana::Spectrum& s_a, const ana::Spectrum& s_b, const std::string& lab_a, const std::string& lab_b, 
                            double pot_n, const std::string& outpath, Color_t col_a = kBlack, Color_t col_b = kOrange)
    {
    auto c = NewCanvas();

    auto h_a = s_a.ToTH1(pot_n, col_a);
    auto h_b = s_b.ToTH1(pot_n, col_b);
    if (h_a->GetSumw2N() == 0) h_a->Sumw2();
    if (h_b->GetSumw2N() == 0) h_b->Sumw2();
    h_a->SetLineWidth(2);
    h_b->SetLineWidth(2);

    h_a->Draw("hist");
    h_b->Draw("same hist");
    c->Update();

    auto leg = MakeLegend(pot_n);
    leg->AddEntry(h_a, Form("%s (%.0f ev)", lab_a.c_str(), h_a->Integral()), "le");
    leg->AddEntry(h_b, Form("%s (%.0f ev)", lab_b.c_str(), h_b->Integral()), "le");
    leg->Draw();
    c->SaveAs(outpath.c_str());
    }

    void Draw1D(const ana::Spectrum& s, double pot_n, const std::string& outpath, Color_t col = kBlue)
    {
    auto c = NewCanvas();
    auto h = s.ToTH1(pot_n, col);
    h->Draw("hsit");
    c->SaveAs(outpath.c_str());
    }

    void Draw2D(const ana::Spectrum& s, double pot_n, const std::string& outpath)
    {
    auto c = NewCanvas();
    auto h = s.ToTH2(pot_n);
    h->Draw("COLZ");
    c->SaveAs(outpath.c_str());
    }

    void DrawRatio2D(const ana::Spectrum& s_n, const ana::Spectrum& s_v, double pot_n,
                    const std::string& outpath, double zmin = 0.5, double zmax = 1.5)
    {
    auto c = NewCanvas();
    auto h_n = s_n.ToTH2(pot_n);
    auto h_v = s_v.ToTH2(pot_n);
    // [FIX] nell'originale era h4_n / h4_v (nominal/varied), al contrario di tutte le ratio 1D
    // (varied/nominal). Ora e' sempre varied/nominal. Opzionale: Divide(h_n, "B") per errori binomiali.
    auto h_r = (TH2D*)h_v->Clone(Form("ratio2d_%d", gCanvasId));
    h_r->Divide(h_n);
    h_r->SetTitle("varied / nominal");
    h_r->SetMinimum(zmin);
    h_r->SetMaximum(zmax);
    h_r->Draw("COLZ");
    c->SaveAs(outpath.c_str());
    }

    void KSRebinScan(const ana::Spectrum& s_n, const ana::Spectrum& s_v, double pot_n,
                    int nbins_fine = 1000, std::initializer_list<int> factors = {1,2,5,10,20,40})
    {
    TH1D* fA = s_n.ToTH1(pot_n, ana::kPOT);
    TH1D* fB = s_v.ToTH1(pot_n, ana::kPOT);
    for (int k : factors) {
        TH1D* a = (TH1D*)fA->Clone(); a->Rebin(k);
        TH1D* b = (TH1D*)fB->Clone(); b->Rebin(k);
        printf("nbin=%d  D=%.5f  p=%g  pAD=%g\n", nbins_fine / k,
            a->KolmogorovTest(b, "M"), a->KolmogorovTest(b), a->AndersonDarlingTest(b));
        delete a; delete b;
    }
    }

}

#endif // UTILS_H