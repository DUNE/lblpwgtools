#include "RandHisto.h"

#include "TMath.h"

RandHisto::RandHisto(CLHEP::HepRandomEngine& engine) : fFlat(engine)
{}

void RandHisto::GetRandom(TH1* histo, double& x, double& y, double& z)
{
  int dim = histo->GetDimension();
  int nbins = histo->GetNbinsX();
  int nbinsy = 0;
  int nbinsz = 0;
  int nbinsx = histo->GetNbinsX();
  if (dim > 1){
    nbinsy = histo->GetNbinsY();
    nbins *= nbinsy;
  }
  if (dim > 2){
    nbinsz = histo->GetNbinsZ(); 
    nbins *= nbinsz;
  }
  
  int ibinX(0), ibinY(0), ibinZ(0);
  //GetIntegral is normalized to 1 when it computes the bin integral array
  int ibinG = TMath::BinarySearch(nbins, histo->GetIntegral(), fFlat.fire());
  //histo->GetBinXYZ(ibinG, ibinX, ibinY, ibinZ);
  //NOTE the GetIntegral computes its bin integral array without overflow/underflow bins (indexing starting at 0).  So GetBinXYZ cannot be used to convert
  //the global bin index.  At best it will only be off by 1 for 1d histograms.  At worst it will be very wrong for 2d and 3d histograms.
  //To avoid this compute the indexing by hand.
  if (dim == 1) ibinX = ibinG + 1;
  if (dim == 2){
    ibinY = (ibinG / nbinsx) + 1;
    ibinX = (ibinG % nbinsx) + 1;
  }
  if (dim == 3){
    ibinZ = ibinG / (nbinsx*nbinsy);
    ibinY = ((ibinG - (nbinsx*nbinsy*ibinZ)) / nbinsx) + 1;
    ibinX = ((ibinG - (nbinsx*nbinsy*ibinZ)) % nbinsx) + 1;
    ibinZ++;
  }
    

  //x = histo->GetXaxis()->GetBinCenter(ibinX);
  double xCenter = histo->GetXaxis()->GetBinCenter(ibinX);
  double xWidth = histo->GetXaxis()->GetBinWidth(ibinX);
  x = (xCenter-0.5*xWidth) + xWidth*fFlat.fire();

  if (dim > 1)
    {
      //y = histo->GetYaxis()->GetBinCenter(ibinY);
      double yCenter = histo->GetYaxis()->GetBinCenter(ibinY);
      double yWidth = histo->GetYaxis()->GetBinWidth(ibinY);
      y = (yCenter-0.5*yWidth) + yWidth*fFlat.fire();
    }

  if (dim > 2)
    {
      //z = histo->GetZaxis()->GetBinCenter(ibinZ);
      double zCenter = histo->GetZaxis()->GetBinCenter(ibinZ);
      double zWidth = histo->GetZaxis()->GetBinWidth(ibinZ);
      z = (zCenter-0.5*zWidth) + zWidth*fFlat.fire();
    }

  return;
}



