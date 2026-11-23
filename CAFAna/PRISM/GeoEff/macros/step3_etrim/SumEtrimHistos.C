#include <TFile.h>
#include <TH1.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TList.h>
#include <TKey.h>
#include <TString.h>
#include <iostream>
#include <vector>
#include <map>
void SumEtrimHistos(const TString& folderPath, const std::vector<TString>& keywords) {
    // Open the directory
    TSystemDirectory dir(folderPath, folderPath);
    TList* files = dir.GetListOfFiles();
    if (!files) {
        std::cerr << "Error: Could not open folder " << folderPath << std::endl;
        return;
    }
    // Map to store summed histograms for each keyword
    std::map<TString, TH1*> summedHistograms;
    // Loop over files in the folder
    TIter next(files);
    TSystemFile* file;
    while ((file = dynamic_cast<TSystemFile*>(next()))) {
        
        TString fileName = file->GetName();
        // Skip directories and non-ROOT files
        if (file->IsDirectory() || !fileName.EndsWith(".root")) {
            continue;
        }
        TString filePath = folderPath + "/" + fileName;
        // Open the ROOT file
        TFile* inputFile = TFile::Open(filePath, "READ");
        cout<< " input file is: "<<inputFile->GetName()<<endl;
        if (!inputFile || inputFile->IsZombie()) {
            std::cerr << "Error: Could not open file " << filePath << std::endl;
            delete inputFile;
            continue;
        }
        // Loop over all objects in the file
        TIter keyList(inputFile->GetListOfKeys());
        TKey* key;
        while ((key = dynamic_cast<TKey*>(keyList()))) {
            // Get the object name
            TString objName = key->GetName();
	     //std::cout << "Found object: " << key->GetName() << std::endl;
            // Check if the histogram name matches any of the given keywords
            for (const auto& keyword : keywords) {
                //if (objName.Contains(keyword)) {
		if (objName == keyword || objName.BeginsWith(keyword)){
                    //cout<<" keyword: "<<keyword<<endl;
                    // Retrieve the histogram
                    TH1* hist = dynamic_cast<TH1*>(key->ReadObj());
                    if (!hist) {
                        std::cerr << "Warning: Object " << objName << " is not a histogram." << std::endl;
                        continue;
                    }
                    // Skip histograms with NaN or inf integral
                    double integral = hist->Integral();
                    if (!std::isfinite(integral)) {
                        std::cerr << "Skipping histogram " << objName
                                  << " due to non-finite integral: " << integral << std::endl;
                        delete hist;  // avoid memory leak
                        continue;
                    }
                    // Add the histogram to the corresponding summed histogram
                    if (!summedHistograms[keyword]) {
                        // First time this histogram is encountered for the keyword, clone it to initialize the sum
                        summedHistograms[keyword] = dynamic_cast<TH1*>(hist->Clone());
                        summedHistograms[keyword]->SetTitle(Form("Summed_histo_%s", keyword.Data()));
                        summedHistograms[keyword]->SetDirectory(0); // Detach from the file
                    } else {
                        // Add to the existing summed histogram
                        summedHistograms[keyword]->Add(hist);
                    }
                }
            }
        }
        // Clean up
        inputFile->Close();
        delete inputFile;
    }
    // Clean up file list
    delete files;





    // Save the summed histograms to a new ROOT file
    TFile outputFile("AllOAPosWithFDEvRateForEtrue_EtrimSummed_histograms_TEST_withMadisFiles.root", "RECREATE");
    std::cout << "Writing summed histograms to output file" << std::endl;
    for (const auto& [keyword, summedHist] : summedHistograms) {
        if (summedHist) {
            summedHist->Write(); // Write each summed histogram
        }
    }
    outputFile.Close();
    std::cout << "Summed histograms saved to AllOAPosWithFDEvRateForEtrue_EtrimSummed_histograms_NoCoeffsApplied_WithOscillatedSpectrum_CAFLikeWeight_2Dhistograms_withWeightPmuonChange.root" << std::endl;
}
