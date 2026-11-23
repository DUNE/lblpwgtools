#include <TH1.h>
#include <iostream>
void cloneAndAddBinsToCoeffs(TH1* hist, int nExtraBins, double extraBinContent1, double extraBinContent2) {
    // Get the original histogram properties
    int nBinsOriginal = hist->GetNbinsX();
    double xMin = hist->GetXaxis()->GetXmin();
    double xMax = hist->GetXaxis()->GetXmax();
    // Calculate new x-axis range to include additional bins
    double binWidth = (xMax - xMin) / nBinsOriginal;
    double newXMax = xMax + nExtraBins * binWidth;
    // Create a new histogram with additional bins
    TH1* newHist = new TH1F("newHist", hist->GetTitle(), nBinsOriginal + nExtraBins, xMin, newXMax);
    // Copy contents from original histogram to the new histogram
    for (int i = 1; i <= nBinsOriginal; ++i) {
        newHist->SetBinContent(i, hist->GetBinContent(i));
    }
    // Set contents of the additional bins
    newHist->SetBinContent(nBinsOriginal + 1, extraBinContent1);
    newHist->SetBinContent(nBinsOriginal + 2, extraBinContent2);
    // Print to confirm
    for (int i = 1; i <= newHist->GetNbinsX(); ++i) {
        std::cout << "Bin " << i << ": Content = " << newHist->GetBinContent(i) << std::endl;
    }
}
