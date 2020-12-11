#include "OscLib/EarthModel.h"

#include "TH2.h"
#include "TLine.h"

#include <iostream>
#include <list>

void test_earth_model()
{
  osc::EarthModel earth("PREM", .1);
  //  osc::EarthModel earth("STACEY", .1);

  (new TH2F("", "", 100, -7500, +7500, 100, -15000, +1000))->Draw();

  for(double cosQ = -1; cosQ <= +1; cosQ += .002){
    std::cout << cosQ << std::endl;

    std::list<double> Ls, Ns;
    earth.LineProfile(15, cosQ, 0, Ls, Ns);

    // Pretty sure the layers are in the direction the neutrino travels, but it
    // doesn't matter much for this visualization because for all the up-coming
    // ones the sequence is symmetric.

    const double sinQ = sqrt(1-cosQ*cosQ);

    int col = 0;

    double L = 0;
    for(auto Lit = Ls.begin(), Nit = Ns.begin(); Lit != Ls.end(); ++Lit, ++Nit){
      const double dL = *Lit;
      std::cout << "  " << dL << " " << *Nit << std::endl;
      // NB the Ns are electron mole/cm^3 ~ 0.5*density

      for(int sign: {-1, +1}){
        TLine* line = new TLine(sign*sinQ*L, cosQ*L, sign*sinQ*(L+dL), cosQ*(L+dL));
        line->SetLineWidth(2);
        line->SetLineColor((col%8)+1);
        line->Draw();
      }
      ++col;
      L += dL;
    }
  }
}
