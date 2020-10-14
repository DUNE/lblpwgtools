#pragma once

#include <map>

namespace ana

{

  //Beam power POT / year
  //*======================================================================================================*/
  //*                                     Beam Power Configurations                                        */
  //*                            (Use this chart to detemine @power in units of pot/yr)                    */
  //*  MI prot/pulse    Energy (GeV)    Cycle time    Beam power (MW)    Uptime&efficiency    pot/year     */
  //*     7.50E+13          120            1.2           1.20E+00              0.56           1.10E+21     */
  //*     7.50E+13           80            0.9           1.07E+00              0.56           1.47E+21     */
  //*     7.50E+13           60            0.7           1.03E+00              0.56           1.89E+21     */
  //*                                                                                                      */
  //*======================================================================================================*/
  
  // POT per year for the MI accelerating them to the given energy (120, 80 or 60 GeV)
  const double POT120 = 1.1e21;
  const double POT80  = 1.47e21;
  const double POT60  = 1.89e21;

  // Scale FD POT as MC wasnt produced with as much stats as the ND
  const double kFakeFDPOT  = POT120 * 40 / 1.13;
  const double kFakeNDPOT  = POT120;
  const double kExposure7Years  = 336.;
  const double kExposure10Years = 624.;
  const double kExposure15Years = 1104.;

  // This is for the nominal exposure, which is 7 years staged 
  const double kNominalExposure = kExposure7Years;

  std::map<int,float> kFDPOT = {
    {7,  kFakeFDPOT},
    {10, kFakeFDPOT * kExposure10Years / kNominalExposure},
    {15, kFakeFDPOT * kExposure15Years / kNominalExposure}
  };

  std::map<int,float> kNDPOT = {
    {7,  kFakeNDPOT},
    {10, kFakeNDPOT * kExposure10Years / kNominalExposure},
    {15, kFakeNDPOT * kExposure15Years / kNominalExposure}
  };

  // // POT per year assuming beam intensity at 7 years
  // const double kFDPOT = kFakeFDPOT * kExposure7Years / kNominalExposure;
  // const double kNDPOT = kFakeNDPOT * kExposure7Years / kNominalExposure;

}
