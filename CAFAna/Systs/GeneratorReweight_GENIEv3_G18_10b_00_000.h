#include "BDTReweighter.h"
class GeneratorReweight_GENIEv3_G18_10b_00_000 : public BDTReweighter {
 public :
 GeneratorReweight_GENIEv3_G18_10b_00_000() : BDTReweighter(1.0) {;}
  ~GeneratorReweight_GENIEv3_G18_10b_00_000() {;}


size_t get_num_output_group(void) {
  return 1;
}

size_t get_num_feature(void) {
  return 23;
}

static inline float pred_transform(float margin) {
  const float alpha = (float)1;
  return 1.0f / (1 + expf(-alpha * margin));
}

   float predict(union BDTReweighter::BDTReweighterFeature* data, int pred_margin) {

  float sum = 0.0f;
  unsigned int tmp;
  if (  (data[13].fvalue < 3.999999)) {
    if (  (data[13].fvalue < 0.99999899)) {
      if (  (data[10].fvalue < 0.3266536)) {
        if (  (data[5].fvalue < 0.53459638)) {
          if (  (data[11].fvalue < 0.31347501)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.479608;
            } else {
              sum += (float)-0.15421529;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.22279964;
            } else {
              sum += (float)-0.40148771;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.096527427)) {
            if (  (data[22].fvalue < 0.35841301)) {
              sum += (float)-0.080356792;
            } else {
              sum += (float)-0.021084951;
            }
          } else {
            if (  (data[22].fvalue < 0.39471692)) {
              sum += (float)-0.018980721;
            } else {
              sum += (float)0.0045265737;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 1.0798494)) {
          if (  (data[21].fvalue < 0.11727373)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.22534676;
            } else {
              sum += (float)0.053768083;
            }
          } else {
            if (  (data[21].fvalue < 0.33215699)) {
              sum += (float)-0.092430577;
            } else {
              sum += (float)0.015769789;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.43263882)) {
              sum += (float)-0.074740082;
            } else {
              sum += (float)0.49126133;
            }
          } else {
            if (  (data[11].fvalue < 0.23728426)) {
              sum += (float)-0.24625653;
            } else {
              sum += (float)-0.091560207;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[10].fvalue < 0.45693493)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.39471692)) {
              sum += (float)-0.15807526;
            } else {
              sum += (float)-0.10266355;
            }
          } else {
            if (  (data[22].fvalue < 0.11137914)) {
              sum += (float)-0.12604918;
            } else {
              sum += (float)-0.24153623;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.11137914)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.079301588;
            } else {
              sum += (float)0.048842855;
            }
          } else {
            if (  (data[22].fvalue < 0.35841301)) {
              sum += (float)-0.26378885;
            } else {
              sum += (float)-0.093196139;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.1390392)) {
          if (  (data[10].fvalue < 0.49091011)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.10791162;
            } else {
              sum += (float)-0.023860523;
            }
          } else {
            if (  (data[10].fvalue < 0.67122424)) {
              sum += (float)0.20108132;
            } else {
              sum += (float)0.30801472;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.35841301)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.059603598;
            } else {
              sum += (float)-0.22112399;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.031148406;
            } else {
              sum += (float)-0.047874164;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 9.999999)) {
      if (  (data[5].fvalue < 0.53459638)) {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[10].fvalue < 0.029676639)) {
              sum += (float)-0.27048969;
            } else {
              sum += (float)-0.051080648;
            }
          } else {
            if (  (data[11].fvalue < 0.53450954)) {
              sum += (float)-0.40934193;
            } else {
              sum += (float)-0.25697029;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[11].fvalue < 0.49810666)) {
              sum += (float)-0.62232381;
            } else {
              sum += (float)-0.44087893;
            }
          } else {
            if (  (data[11].fvalue < 0.63323027)) {
              sum += (float)-0.80278814;
            } else {
              sum += (float)-0.57400721;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.1390392)) {
          if (  (data[22].fvalue < 0.019597029)) {
            if (  (data[11].fvalue < 0.010542287)) {
              sum += (float)-0.11066065;
            } else {
              sum += (float)0.16172792;
            }
          } else {
            if (  (data[10].fvalue < 0.49091011)) {
              sum += (float)0.34705436;
            } else {
              sum += (float)0.58814579;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[22].fvalue < 0.32266095)) {
              sum += (float)-0.047351733;
            } else {
              sum += (float)0.078893788;
            }
          } else {
            if (  (data[21].fvalue < 0.023904111)) {
              sum += (float)0.084929854;
            } else {
              sum += (float)0.21144003;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.99999899)) {
        if (  (data[5].fvalue < 1.3349454)) {
          if (  (data[5].fvalue < 0.53459638)) {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.91910118;
            } else {
              sum += (float)-0.97664303;
            }
          } else {
            if (  (data[11].fvalue < 0.38546163)) {
              sum += (float)-0.83230895;
            } else {
              sum += (float)-0.1901685;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.022867812)) {
            if (  (data[11].fvalue < 0.010542287)) {
              sum += (float)-0.83340639;
            } else {
              sum += (float)-0.31834799;
            }
          } else {
            if (  (data[5].fvalue < 7.7357812)) {
              sum += (float)0.056601468;
            } else {
              sum += (float)0.30704466;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 13.999999)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)0.34017822;
            } else {
              sum += (float)0.4447754;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)0.49245173;
            } else {
              sum += (float)0.63496619;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.038016215)) {
              sum += (float)-0.28318059;
            } else {
              sum += (float)0.51030117;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.65105796;
            } else {
              sum += (float)0.8158744;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.94882452)) {
    if (  (data[18].fvalue < 0.18951811)) {
      if (  (data[18].fvalue < 0.0027355412)) {
        if (  (data[9].fvalue < 0.9217518)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.016403709;
            } else {
              sum += (float)-0.19557527;
            }
          } else {
            if (  (data[11].fvalue < 0.011326684)) {
              sum += (float)-0.41312763;
            } else {
              sum += (float)-0.21697083;
            }
          }
        } else {
          if (  (data[5].fvalue < 1.3966936)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.12521419;
            } else {
              sum += (float)-0.33719289;
            }
          } else {
            if (  (data[7].fvalue < 0.99983507)) {
              sum += (float)0.024689896;
            } else {
              sum += (float)-0.04240346;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.071185395)) {
          if (  (data[20].fvalue < 0.047182869)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.080504373;
            } else {
              sum += (float)-0.14678529;
            }
          } else {
            if (  (data[18].fvalue < 0.021533832)) {
              sum += (float)0.027071565;
            } else {
              sum += (float)-0.078261986;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.1419559)) {
            if (  (data[21].fvalue < 0.024135198)) {
              sum += (float)0.014037822;
            } else {
              sum += (float)0.1377719;
            }
          } else {
            if (  (data[21].fvalue < 0.330165)) {
              sum += (float)-0.13359512;
            } else {
              sum += (float)-0.014958254;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.3563442)) {
        if (  (data[18].fvalue < 0.29286841)) {
          if (  (data[21].fvalue < 0.1419559)) {
            if (  (data[20].fvalue < 0.13698576)) {
              sum += (float)0.11066214;
            } else {
              sum += (float)-0.020340059;
            }
          } else {
            if (  (data[21].fvalue < 0.26300621)) {
              sum += (float)-0.11141061;
            } else {
              sum += (float)-0.00075319275;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99988395)) {
            if (  (data[21].fvalue < 0.1419559)) {
              sum += (float)0.16384856;
            } else {
              sum += (float)0.062014308;
            }
          } else {
            if (  (data[9].fvalue < 1.2597686)) {
              sum += (float)0.41849422;
            } else {
              sum += (float)0.26525575;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.56022865)) {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.1419559)) {
              sum += (float)0.061926745;
            } else {
              sum += (float)0.030563105;
            }
          } else {
            if (  (data[21].fvalue < 0.1419559)) {
              sum += (float)0.52799124;
            } else {
              sum += (float)0.15819713;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.024135198)) {
            if (  (data[9].fvalue < 1.7054417)) {
              sum += (float)0.032776795;
            } else {
              sum += (float)-0.091131702;
            }
          } else {
            if (  (data[21].fvalue < 0.1191311)) {
              sum += (float)0.24170625;
            } else {
              sum += (float)0.0016809638;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.7690734)) {
      if (  (data[21].fvalue < 0.024135198)) {
        if (  (data[20].fvalue < 0.018096101)) {
          if (  (data[9].fvalue < 1.5295591)) {
            if (  (data[18].fvalue < 4.1766186)) {
              sum += (float)0.040054496;
            } else {
              sum += (float)0.22885543;
            }
          } else {
            if (  (data[18].fvalue < 2.2394111)) {
              sum += (float)-0.075088844;
            } else {
              sum += (float)0.037894677;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.10854869)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.51216137;
            } else {
              sum += (float)0.32278991;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.11037649;
            } else {
              sum += (float)0.51120937;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.1191311)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.24942209)) {
              sum += (float)0.28149554;
            } else {
              sum += (float)0.37930635;
            }
          } else {
            if (  (data[20].fvalue < 0.10854869)) {
              sum += (float)0.56026846;
            } else {
              sum += (float)0.40980509;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 2.7686458)) {
              sum += (float)0.11380812;
            } else {
              sum += (float)0.32409012;
            }
          } else {
            if (  (data[20].fvalue < 1.9955974)) {
              sum += (float)0.30527788;
            } else {
              sum += (float)0.033226296;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 0.024135198)) {
        if (  (data[20].fvalue < 2.4114335)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 3.3472011)) {
              sum += (float)-0.12681195;
            } else {
              sum += (float)-0.066490494;
            }
          } else {
            if (  (data[20].fvalue < 0.85878372)) {
              sum += (float)0.49633768;
            } else {
              sum += (float)0.28993848;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 3.1629159)) {
              sum += (float)-0.11238351;
            } else {
              sum += (float)-0.23585287;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.028912138;
            } else {
              sum += (float)0.41618139;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.1191311)) {
          if (  (data[20].fvalue < 0.13698576)) {
            if (  (data[20].fvalue < 0.018096101)) {
              sum += (float)0.16009857;
            } else {
              sum += (float)0.2955884;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.085541472;
            } else {
              sum += (float)0.25152069;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[20].fvalue < 1.6554426)) {
              sum += (float)-0.026361223;
            } else {
              sum += (float)-0.067754254;
            }
          } else {
            if (  (data[20].fvalue < 3.0544782)) {
              sum += (float)0.33750305;
            } else {
              sum += (float)0.059342433;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 2.999999)) {
    if (  (data[18].fvalue < 0.030772705)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < -1e-06)) {
            if (  (data[5].fvalue < 1.4149828)) {
              sum += (float)1.0025603;
            } else {
              sum += (float)1.0637152;
            }
          } else {
            if (  (data[5].fvalue < 0.52476287)) {
              sum += (float)-0.17646788;
            } else {
              sum += (float)0.11140263;
            }
          }
        } else {
          if (  (data[19].fvalue < -1e-06)) {
            if (  (data[18].fvalue < 0.0024532536)) {
              sum += (float)1.0177189;
            } else {
              sum += (float)1.1164577;
            }
          } else {
            if (  (data[19].fvalue < 0.035665169)) {
              sum += (float)-0.36600596;
            } else {
              sum += (float)-0.23306575;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[19].fvalue < 0.57676089)) {
              sum += (float)-0.18092468;
            } else {
              sum += (float)-0.29531959;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.56922245;
            } else {
              sum += (float)-0.89457148;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.021892056)) {
              sum += (float)0.24079154;
            } else {
              sum += (float)0.023596048;
            }
          } else {
            if (  (data[19].fvalue < 0.17770056)) {
              sum += (float)0.24385659;
            } else {
              sum += (float)0.69910431;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.99999899)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.060043264)) {
              sum += (float)-0.15409359;
            } else {
              sum += (float)0.048257448;
            }
          } else {
            if (  (data[18].fvalue < 0.082523264)) {
              sum += (float)-0.21086319;
            } else {
              sum += (float)0.13279769;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.13229947)) {
            if (  (data[5].fvalue < 0.52476287)) {
              sum += (float)0.27479702;
            } else {
              sum += (float)-0.10080185;
            }
          } else {
            if (  (data[18].fvalue < 7.4610353)) {
              sum += (float)-0.30810976;
            } else {
              sum += (float)-0.090869419;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[19].fvalue < -1e-06)) {
              sum += (float)1.0567441;
            } else {
              sum += (float)-0.26474449;
            }
          } else {
            if (  (data[18].fvalue < 0.10585126)) {
              sum += (float)-0.50057584;
            } else {
              sum += (float)-0.76052123;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.11788228)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.075032219;
            } else {
              sum += (float)0.31915084;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.25230932;
            } else {
              sum += (float)-0.02365054;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[12].fvalue < 5.999999)) {
        if (  (data[18].fvalue < 0.20660079)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.030772705)) {
              sum += (float)0.64574116;
            } else {
              sum += (float)0.15599351;
            }
          } else {
            if (  (data[18].fvalue < 0.14591818)) {
              sum += (float)0.57955086;
            } else {
              sum += (float)0.33490855;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.11897673)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.18216531;
            } else {
              sum += (float)0.21026286;
            }
          } else {
            if (  (data[18].fvalue < 0.5023756)) {
              sum += (float)-0.089090347;
            } else {
              sum += (float)-0.30839723;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[19].fvalue < 0.22615461)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.59220648;
            } else {
              sum += (float)-0.1912372;
            }
          } else {
            if (  (data[18].fvalue < 0.22268157)) {
              sum += (float)0.82360435;
            } else {
              sum += (float)-0.70726526;
            }
          }
        } else {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[18].fvalue < 0.27721941)) {
              sum += (float)0.058303833;
            } else {
              sum += (float)-0.64815181;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.91495258;
            } else {
              sum += (float)-0.76644099;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.030772705)) {
        if (  (data[19].fvalue < 0.1316677)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.035665169)) {
              sum += (float)-0.27196532;
            } else {
              sum += (float)0.42985249;
            }
          } else {
            if (  (data[20].fvalue < 0.018208068)) {
              sum += (float)-0.73011792;
            } else {
              sum += (float)-1.0797118;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.4397403)) {
              sum += (float)0.33731896;
            } else {
              sum += (float)0.85626286;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.31055582;
            } else {
              sum += (float)-0.88384587;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[19].fvalue < 0.19264764)) {
              sum += (float)0.35322279;
            } else {
              sum += (float)0.083981574;
            }
          } else {
            if (  (data[18].fvalue < 0.70910329)) {
              sum += (float)0.090359338;
            } else {
              sum += (float)-0.35143125;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.17770056)) {
            if (  (data[18].fvalue < 0.39829162)) {
              sum += (float)-0.88229334;
            } else {
              sum += (float)0.32522094;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.09896189;
            } else {
              sum += (float)0.29598755;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 3.999999)) {
    if (  (data[12].fvalue < 2.999999)) {
      if (  (data[13].fvalue < 6.999999)) {
        if (  (data[19].fvalue < -1e-06)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.69071627;
            } else {
              sum += (float)0.8764413;
            }
          } else {
            if (  (data[5].fvalue < 0.56420255)) {
              sum += (float)0.88824356;
            } else {
              sum += (float)0.99134022;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.046312194;
            } else {
              sum += (float)-0.14238447;
            }
          } else {
            if (  (data[19].fvalue < 0.004854891)) {
              sum += (float)-0.46426564;
            } else {
              sum += (float)-0.03062946;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.11935101)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.13360009)) {
              sum += (float)-0.6756193;
            } else {
              sum += (float)-0.1035207;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.31026277;
            } else {
              sum += (float)-0.60296512;
            }
          }
        } else {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.30483586;
            } else {
              sum += (float)-0.4385542;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.60353917;
            } else {
              sum += (float)-0.85816383;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.11798138;
            } else {
              sum += (float)-0.10357017;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.12473837;
            } else {
              sum += (float)-0.016156947;
            }
          }
        } else {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[19].fvalue < 0.10794852)) {
              sum += (float)-0.19974044;
            } else {
              sum += (float)-0.45698881;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.45239255;
            } else {
              sum += (float)-0.59247655;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.023284789)) {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.11736084;
            } else {
              sum += (float)-0.0011041239;
            }
          } else {
            if (  (data[19].fvalue < 0.41750884)) {
              sum += (float)-0.73396617;
            } else {
              sum += (float)-0.044610485;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.14161812)) {
            if (  (data[10].fvalue < 0.39550045)) {
              sum += (float)0.23875798;
            } else {
              sum += (float)0.38870865;
            }
          } else {
            if (  (data[19].fvalue < 0.13360009)) {
              sum += (float)0.24337557;
            } else {
              sum += (float)0.062321849;
            }
          }
        }
      }
    }
  } else {
    if (  (data[21].fvalue < 0.098392636)) {
      if (  (data[17].fvalue < 4.999999)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.15717396)) {
            if (  (data[10].fvalue < 0.038493548)) {
              sum += (float)-0.37548593;
            } else {
              sum += (float)-0.23482214;
            }
          } else {
            if (  (data[19].fvalue < 1.7559128)) {
              sum += (float)-0.085012786;
            } else {
              sum += (float)-0.2144829;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.34197265)) {
              sum += (float)-0.75400078;
            } else {
              sum += (float)-0.63548172;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.57534641;
            } else {
              sum += (float)-0.44748428;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 6.999999)) {
          if (  (data[10].fvalue < 0.49238232)) {
            if (  (data[19].fvalue < 1.0684278)) {
              sum += (float)-0.012663915;
            } else {
              sum += (float)-0.71335614;
            }
          } else {
            if (  (data[10].fvalue < 0.62774539)) {
              sum += (float)0.21644694;
            } else {
              sum += (float)0.43221888;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.96180648;
            } else {
              sum += (float)-0.88125145;
            }
          } else {
            sum += (float)-1.0847503;
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 5.999999)) {
        if (  (data[21].fvalue < 1.0268688)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.57745212;
            } else {
              sum += (float)-0.77190971;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.48567325;
            } else {
              sum += (float)-0.59100008;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.76157868;
            } else {
              sum += (float)-0.92579263;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.74424875;
            } else {
              sum += (float)-0.64000237;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.62774539)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[11].fvalue < 0.097243741)) {
              sum += (float)-0.53301609;
            } else {
              sum += (float)-0.93136197;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-1.1201735;
            } else {
              sum += (float)-1.4272337;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            sum += (float)-0.69885129;
          } else {
            if (  (data[11].fvalue < 0.28807724)) {
              sum += (float)0.69453442;
            } else {
              sum += (float)0.46711984;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 10.999999)) {
    if (  (data[12].fvalue < 13.999999)) {
      if (  (data[17].fvalue < 3.999999)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.039432198;
            } else {
              sum += (float)-0.036752824;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.18716307;
            } else {
              sum += (float)-0.40417448;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.14924987;
            } else {
              sum += (float)-0.34153163;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)0.046524975;
            } else {
              sum += (float)-0.10896211;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[21].fvalue < 0.44176149)) {
            if (  (data[17].fvalue < 6.999999)) {
              sum += (float)0.061470184;
            } else {
              sum += (float)-0.67305225;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.34991914;
            } else {
              sum += (float)-0.49956045;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.30506849)) {
              sum += (float)0.24084581;
            } else {
              sum += (float)0.38997436;
            }
          } else {
            if (  (data[17].fvalue < 4.999999)) {
              sum += (float)-0.48993728;
            } else {
              sum += (float)-0.65815401;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 15.999999)) {
        if (  (data[13].fvalue < 8.999999)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.76366031;
            } else {
              sum += (float)-0.65446323;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.68048996;
            } else {
              sum += (float)-0.42428854;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.023210909)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.34874812;
            } else {
              sum += (float)-0.73670483;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.32947698;
            } else {
              sum += (float)-0.04436807;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 16.999998)) {
          if (  (data[13].fvalue < 8.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.9078663;
            } else {
              sum += (float)-0.77742207;
            }
          } else {
            if (  (data[21].fvalue < 0.023210909)) {
              sum += (float)-0.85398912;
            } else {
              sum += (float)-0.4504756;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.60340208;
            } else {
              sum += (float)-0.8492865;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-1.107493;
            } else {
              sum += (float)-0.88736051;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 4.999999)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[13].fvalue < 13.999999)) {
          if (  (data[22].fvalue < 0.14425568)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.12370799;
            } else {
              sum += (float)-0.35868675;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.54868764;
            } else {
              sum += (float)-0.26748943;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.64260483;
            } else {
              sum += (float)-0.88368845;
            }
          } else {
            if (  (data[21].fvalue < 1.10543)) {
              sum += (float)0.042067047;
            } else {
              sum += (float)-0.30884707;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[21].fvalue < 0.101117)) {
            if (  (data[22].fvalue < 0.14425568)) {
              sum += (float)0.19755584;
            } else {
              sum += (float)-0.020310154;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.29346019;
            } else {
              sum += (float)0.054362539;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.101117)) {
              sum += (float)-0.31180307;
            } else {
              sum += (float)-0.72299486;
            }
          } else {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)-0.056936912;
            } else {
              sum += (float)-0.41253155;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 0.78307188)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[8].fvalue < 0.10192118)) {
              sum += (float)-0.14822046;
            } else {
              sum += (float)0.054994196;
            }
          } else {
            if (  (data[22].fvalue < 0.022190651)) {
              sum += (float)-0.29740393;
            } else {
              sum += (float)-0.013821842;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 17.999998)) {
              sum += (float)0.28358734;
            } else {
              sum += (float)0.45911741;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.11068805;
            } else {
              sum += (float)0.26399699;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 16.999998)) {
          if (  (data[12].fvalue < 15.999999)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.067072123;
            } else {
              sum += (float)0.22789225;
            }
          } else {
            if (  (data[21].fvalue < 0.023210909)) {
              sum += (float)-0.53350967;
            } else {
              sum += (float)0.0089473678;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.3609781;
            } else {
              sum += (float)0.28377822;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.070915312;
            } else {
              sum += (float)0.44038808;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 2.999999)) {
    if (  (data[17].fvalue < 1.999999)) {
      if (  (data[20].fvalue < 0.13723035)) {
        if (  (data[20].fvalue < 0.019497465)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.011770103;
            } else {
              sum += (float)0.020134164;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.26572368;
            } else {
              sum += (float)0.023920817;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 0.04702973)) {
              sum += (float)-0.022779465;
            } else {
              sum += (float)0.075070865;
            }
          } else {
            if (  (data[20].fvalue < 0.10738677)) {
              sum += (float)0.25864056;
            } else {
              sum += (float)0.12186044;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.27527452)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 0.24513911)) {
              sum += (float)-0.1316562;
            } else {
              sum += (float)-0.081333376;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.019909695;
            } else {
              sum += (float)0.29029128;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.0055824388;
            } else {
              sum += (float)-0.039146669;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.01002985;
            } else {
              sum += (float)0.12412637;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.13723035)) {
        if (  (data[20].fvalue < 0.019497465)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.013333146;
            } else {
              sum += (float)-0.084035687;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.1003783;
            } else {
              sum += (float)-0.011257191;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[17].fvalue < 4.999999)) {
              sum += (float)0.018342631;
            } else {
              sum += (float)-0.32537544;
            }
          } else {
            if (  (data[8].fvalue < 0.84376985)) {
              sum += (float)0.049029909;
            } else {
              sum += (float)0.22263035;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 3.999999)) {
          if (  (data[8].fvalue < 0.30401385)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.18188441;
            } else {
              sum += (float)-0.46587446;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.075697877;
            } else {
              sum += (float)-0.15350342;
            }
          }
        } else {
          if (  (data[17].fvalue < 5.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.13807036;
            } else {
              sum += (float)-0.37375045;
            }
          } else {
            if (  (data[20].fvalue < 0.53073788)) {
              sum += (float)-0.45629424;
            } else {
              sum += (float)-0.58175457;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 4.999999)) {
      if (  (data[17].fvalue < 1.999999)) {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.2385751;
            } else {
              sum += (float)0.67915571;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.62951303;
            } else {
              sum += (float)1.0700365;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 2.8756466)) {
              sum += (float)0.11873085;
            } else {
              sum += (float)0.075099736;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.021874947;
            } else {
              sum += (float)0.096023366;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.22939578;
            } else {
              sum += (float)-0.12210154;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.44994944;
            } else {
              sum += (float)-0.18145882;
            }
          }
        } else {
          if (  (data[17].fvalue < 5.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.52664798;
            } else {
              sum += (float)-0.41660991;
            }
          } else {
            sum += (float)-0.58470345;
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.74212116)) {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[20].fvalue < 0.13723035)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.3811954;
            } else {
              sum += (float)0.58817953;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.18064941;
            } else {
              sum += (float)0.36824727;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.10738677)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.65643835;
            } else {
              sum += (float)0.77880079;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.45318758;
            } else {
              sum += (float)0.61172158;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.097422287;
            } else {
              sum += (float)-0.19155023;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.41005778;
            } else {
              sum += (float)0.1739504;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.31076255;
            } else {
              sum += (float)0.48332804;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.23268557;
            } else {
              sum += (float)0.2191406;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < -1e-06)) {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[8].fvalue < 0.099852636)) {
        if (  (data[8].fvalue < 0.062079813)) {
          sum += (float)0.6160363;
        } else {
          sum += (float)0.64700592;
        }
      } else {
        if (  (data[8].fvalue < 0.39517331)) {
          sum += (float)0.67507553;
        } else {
          sum += (float)0.71960366;
        }
      }
    } else {
      if (  (data[8].fvalue < 0.062079813)) {
        sum += (float)0.75997078;
      } else {
        sum += (float)0.7227627;
      }
    }
  } else {
    if (  (data[22].fvalue < 0.11296078)) {
      if (  (data[22].fvalue < 0.019309675)) {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.25517327)) {
              sum += (float)-0.0052848929;
            } else {
              sum += (float)0.020864207;
            }
          } else {
            if (  (data[20].fvalue < 0.17203109)) {
              sum += (float)0.0026295609;
            } else {
              sum += (float)-0.085492454;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.0058648223)) {
              sum += (float)-0.23930603;
            } else {
              sum += (float)-0.35416836;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.13293011;
            } else {
              sum += (float)0.097311601;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 8.999999)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 0.050846301)) {
              sum += (float)-0.017505586;
            } else {
              sum += (float)0.083636865;
            }
          } else {
            if (  (data[19].fvalue < 0.51731271)) {
              sum += (float)0.21973513;
            } else {
              sum += (float)0.076713748;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.26269296)) {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.21939252;
            } else {
              sum += (float)-0.75491446;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.28291082;
            } else {
              sum += (float)0.41103309;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.32609361)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[22].fvalue < 0.14525346)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.050703101;
            } else {
              sum += (float)0.10278875;
            }
          } else {
            if (  (data[22].fvalue < 0.2542235)) {
              sum += (float)-0.17641796;
            } else {
              sum += (float)-0.11267661;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[22].fvalue < 0.18144254)) {
              sum += (float)0.028416418;
            } else {
              sum += (float)-0.11501818;
            }
          } else {
            if (  (data[22].fvalue < 0.22057618)) {
              sum += (float)0.33236861;
            } else {
              sum += (float)0.1147039;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.48903832)) {
          if (  (data[13].fvalue < 12.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.0045225536;
            } else {
              sum += (float)0.10700276;
            }
          } else {
            if (  (data[19].fvalue < 0.35402796)) {
              sum += (float)-1.0436828;
            } else {
              sum += (float)-0.27166334;
            }
          }
        } else {
          if (  (data[13].fvalue < 12.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.032195631;
            } else {
              sum += (float)-0.080594361;
            }
          } else {
            if (  (data[13].fvalue < 15.999999)) {
              sum += (float)0.13755345;
            } else {
              sum += (float)0.28689688;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.49644634)) {
    if (  (data[19].fvalue < 0.37431177)) {
      if (  (data[19].fvalue < -1e-06)) {
        if (  (data[18].fvalue < 0.0025154212)) {
          sum += (float)0.55340999;
        } else {
          if (  (data[18].fvalue < 0.023110619)) {
            sum += (float)0.59201503;
          } else {
            sum += (float)0.60971367;
          }
        }
      } else {
        if (  (data[18].fvalue < 0.27366439)) {
          if (  (data[18].fvalue < 0.0025154212)) {
            if (  (data[19].fvalue < 0.082795329)) {
              sum += (float)-0.021758161;
            } else {
              sum += (float)0.036884263;
            }
          } else {
            if (  (data[19].fvalue < 0.027126394)) {
              sum += (float)-0.019111663;
            } else {
              sum += (float)-0.070199549;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.16342768)) {
            if (  (data[19].fvalue < 0.0056471461)) {
              sum += (float)0.0071740337;
            } else {
              sum += (float)0.064957753;
            }
          } else {
            if (  (data[21].fvalue < 0.12084951)) {
              sum += (float)0.023108417;
            } else {
              sum += (float)-0.083783217;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.0025154212)) {
        if (  (data[17].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.463153)) {
              sum += (float)-0.0026120383;
            } else {
              sum += (float)0.023868946;
            }
          } else {
            if (  (data[19].fvalue < 1.4780953)) {
              sum += (float)0.065654248;
            } else {
              sum += (float)0.011887442;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.12084951)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.0057874527;
            } else {
              sum += (float)-0.40525919;
            }
          } else {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.19583941;
            } else {
              sum += (float)-0.53717124;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.18987207)) {
          if (  (data[21].fvalue < 0.12084951)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.1563831;
            } else {
              sum += (float)0.069437958;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.046449959;
            } else {
              sum += (float)0.094386451;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.02281077)) {
            if (  (data[11].fvalue < 0.017424298)) {
              sum += (float)0.53848821;
            } else {
              sum += (float)0.23329738;
            }
          } else {
            if (  (data[19].fvalue < 0.64020312)) {
              sum += (float)-0.018520093;
            } else {
              sum += (float)0.041308627;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.60589868)) {
      if (  (data[19].fvalue < 0.41606781)) {
        if (  (data[19].fvalue < 0.0056471461)) {
          if (  (data[17].fvalue < 2.999999)) {
            if (  (data[21].fvalue < 0.3660872)) {
              sum += (float)-0.0084085325;
            } else {
              sum += (float)0.029828785;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.12954816;
            } else {
              sum += (float)-0.32839882;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.027126394)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.18725808;
            } else {
              sum += (float)0.094751813;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.0598315;
            } else {
              sum += (float)0.013576942;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.12084951)) {
          if (  (data[11].fvalue < 0.02281077)) {
            if (  (data[6].fvalue < 42.824749)) {
              sum += (float)0.87006342;
            } else {
              sum += (float)0.49038935;
            }
          } else {
            if (  (data[21].fvalue < 0.022903096)) {
              sum += (float)-0.024559379;
            } else {
              sum += (float)0.083019435;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.07823588)) {
              sum += (float)0.028129756;
            } else {
              sum += (float)-0.085088618;
            }
          } else {
            if (  (data[18].fvalue < 0.93790704)) {
              sum += (float)-0.064087994;
            } else {
              sum += (float)-0.008675945;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.78594446)) {
        if (  (data[18].fvalue < 0.62316185)) {
          if (  (data[11].fvalue < 0.031713799)) {
            if (  (data[11].fvalue < 0.02281077)) {
              sum += (float)0.94472289;
            } else {
              sum += (float)0.23591971;
            }
          } else {
            if (  (data[21].fvalue < 0.022903096)) {
              sum += (float)-0.13089356;
            } else {
              sum += (float)-0.073654234;
            }
          }
        } else {
          if (  (data[21].fvalue < 1.2632935)) {
            if (  (data[21].fvalue < 0.022903096)) {
              sum += (float)-0.18106161;
            } else {
              sum += (float)-0.079656497;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.19700782;
            } else {
              sum += (float)-0.052374993;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.94559008)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 1.1006616)) {
              sum += (float)-0.16189271;
            } else {
              sum += (float)-0.24687698;
            }
          } else {
            if (  (data[21].fvalue < 1.3470106)) {
              sum += (float)-0.041442063;
            } else {
              sum += (float)-0.13333468;
            }
          }
        } else {
          if (  (data[18].fvalue < 5.0279512)) {
            if (  (data[18].fvalue < 1.1340386)) {
              sum += (float)-0.24582033;
            } else {
              sum += (float)-0.34049562;
            }
          } else {
            if (  (data[18].fvalue < 7.6335864)) {
              sum += (float)-0.1665536;
            } else {
              sum += (float)-0.022719016;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 8.999999)) {
    if (  (data[12].fvalue < 9.999999)) {
      if ( (data[9].fvalue < 1.315008)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 1.0827523)) {
              sum += (float)-0.012006143;
            } else {
              sum += (float)-0.14005025;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.017707584;
            } else {
              sum += (float)0.21843328;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.033181164;
            } else {
              sum += (float)0.20379955;
            }
          } else {
            if (  (data[9].fvalue < 1.0990095)) {
              sum += (float)-0.17056392;
            } else {
              sum += (float)0.13561276;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 1.8441148)) {
              sum += (float)0.093055248;
            } else {
              sum += (float)0.026238794;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.074849717;
            } else {
              sum += (float)-0.30689168;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.0048820805;
            } else {
              sum += (float)-0.11056703;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.064716533;
            } else {
              sum += (float)0.02736677;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 4.999999)) {
        if (  (data[9].fvalue < 1.4768023)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.022154193)) {
              sum += (float)-0.4339267;
            } else {
              sum += (float)0.18532047;
            }
          } else {
            if (  (data[21].fvalue < 0.022154193)) {
              sum += (float)0.075987898;
            } else {
              sum += (float)0.44423148;
            }
          }
        } else {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[21].fvalue < 0.022154193)) {
              sum += (float)-0.59599084;
            } else {
              sum += (float)-0.37615895;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.62843543;
            } else {
              sum += (float)-0.54965216;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 12.999999)) {
          if (  (data[21].fvalue < 0.022154193)) {
            if (  (data[9].fvalue < 1.7706828)) {
              sum += (float)0.0063142232;
            } else {
              sum += (float)-0.26905042;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.065078564;
            } else {
              sum += (float)0.096790686;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.9164572)) {
            if (  (data[21].fvalue < 0.022154193)) {
              sum += (float)-0.003414623;
            } else {
              sum += (float)0.37728533;
            }
          } else {
            if (  (data[12].fvalue < 14.999999)) {
              sum += (float)-0.23064639;
            } else {
              sum += (float)-0.45726556;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      if ( (data[9].fvalue < 1.4768023)) {
        if (  (data[12].fvalue < 1.999999)) {
          if ( (data[9].fvalue < 1.2617604)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.13804944;
            } else {
              sum += (float)-0.063330285;
            }
          } else {
            if (  (data[9].fvalue < 1.3662932)) {
              sum += (float)-0.096005164;
            } else {
              sum += (float)-0.25313282;
            }
          }
        } else {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[9].fvalue < 1.030692)) {
              sum += (float)-0.092887849;
            } else {
              sum += (float)0.12882131;
            }
          } else {
            if ( (data[9].fvalue < 1.3662932)) {
              sum += (float)0.25365469;
            } else {
              sum += (float)0.07330779;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 12.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.32639894;
            } else {
              sum += (float)-0.3934502;
            }
          } else {
            if (  (data[13].fvalue < 14.999999)) {
              sum += (float)-0.50065738;
            } else {
              sum += (float)-0.58766305;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.12874252;
            } else {
              sum += (float)-0.35491884;
            }
          } else {
            if (  (data[21].fvalue < 0.1214781)) {
              sum += (float)-0.20717867;
            } else {
              sum += (float)-0.33228922;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.315008)) {
        if (  (data[12].fvalue < 7.999999)) {
          if (  (data[9].fvalue < 1.0990095)) {
            if (  (data[22].fvalue < 0.019968649)) {
              sum += (float)-0.26155001;
            } else {
              sum += (float)0.34218583;
            }
          } else {
            if (  (data[21].fvalue < 0.022154193)) {
              sum += (float)0.13782448;
            } else {
              sum += (float)-0.13624802;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0990095)) {
            if (  (data[22].fvalue < 0.019968649)) {
              sum += (float)-0.48876452;
            } else {
              sum += (float)0.28340894;
            }
          } else {
            if (  (data[10].fvalue < 0.72374517)) {
              sum += (float)-0.31983486;
            } else {
              sum += (float)0.16169341;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 12.999999)) {
            if (  (data[21].fvalue < 0.1214781)) {
              sum += (float)0.08865492;
            } else {
              sum += (float)-0.015836954;
            }
          } else {
            if ( (data[9].fvalue < 1.8441148)) {
              sum += (float)0.14457433;
            } else {
              sum += (float)-0.26487911;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.538918)) {
            if (  (data[10].fvalue < 0.51283032)) {
              sum += (float)-0.075414658;
            } else {
              sum += (float)0.2369049;
            }
          } else {
            if (  (data[13].fvalue < 18.999998)) {
              sum += (float)0.15074696;
            } else {
              sum += (float)0.30937836;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 1.999999)) {
    if (  (data[12].fvalue < 4.999999)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if ( (data[9].fvalue < 1.9193208)) {
            if (  (data[19].fvalue < 0.0072319144)) {
              sum += (float)0.25992891;
            } else {
              sum += (float)-0.0027230289;
            }
          } else {
            if (  (data[19].fvalue < 0.083015941)) {
              sum += (float)-0.085290179;
            } else {
              sum += (float)0.0055995015;
            }
          }
        } else {
          if ( (data[9].fvalue < 4.1358142)) {
            if (  (data[19].fvalue < 0.37542212)) {
              sum += (float)-0.023825129;
            } else {
              sum += (float)-0.074630529;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.055850532;
            } else {
              sum += (float)-0.018526679;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.1040857)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.0072319144)) {
              sum += (float)0.10611654;
            } else {
              sum += (float)0.19288179;
            }
          } else {
            if (  (data[19].fvalue < 0.1972436)) {
              sum += (float)-0.084187724;
            } else {
              sum += (float)0.19125511;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.0072319144)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0076648165;
            } else {
              sum += (float)-0.088019185;
            }
          } else {
            if (  (data[19].fvalue < 0.1227602)) {
              sum += (float)0.11323143;
            } else {
              sum += (float)0.0028408312;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.21396604)) {
        if (  (data[12].fvalue < 9.999999)) {
          if (  (data[8].fvalue < 0.84350973)) {
            if (  (data[19].fvalue < 0.10853867)) {
              sum += (float)-0.28720418;
            } else {
              sum += (float)-0.13267845;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0028095208;
            } else {
              sum += (float)-0.12238535;
            }
          }
        } else {
          if (  (data[12].fvalue < 12.999999)) {
            if (  (data[8].fvalue < 1.2271506)) {
              sum += (float)-0.48016196;
            } else {
              sum += (float)-0.24720243;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.56604749;
            } else {
              sum += (float)-0.76962888;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.84738)) {
          if (  (data[19].fvalue < 0.4450272)) {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)0.017900279;
            } else {
              sum += (float)-0.60946536;
            }
          } else {
            if ( (data[9].fvalue < 1.5347639)) {
              sum += (float)0.28225338;
            } else {
              sum += (float)0.12815027;
            }
          }
        } else {
          if (  (data[12].fvalue < 14.999999)) {
            if (  (data[19].fvalue < 0.75797492)) {
              sum += (float)-0.034936935;
            } else {
              sum += (float)-0.097444504;
            }
          } else {
            if (  (data[19].fvalue < 0.6095013)) {
              sum += (float)-0.646869;
            } else {
              sum += (float)-0.13646661;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[19].fvalue < 0.027418636)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.0072319144)) {
              sum += (float)0.061198611;
            } else {
              sum += (float)-0.11212045;
            }
          } else {
            if (  (data[19].fvalue < 0.0072319144)) {
              sum += (float)-0.0093973381;
            } else {
              sum += (float)0.17400338;
            }
          }
        } else {
          if (  (data[17].fvalue < 3.999999)) {
            if ( (data[9].fvalue < 2.633378)) {
              sum += (float)-0.54301029;
            } else {
              sum += (float)-0.030322043;
            }
          } else {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.22755569;
            } else {
              sum += (float)-0.52971822;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.071320437)) {
              sum += (float)-0.26406011;
            } else {
              sum += (float)0.036219507;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.60286051;
            } else {
              sum += (float)-0.091962211;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.16453829)) {
              sum += (float)-0.023571687;
            } else {
              sum += (float)-0.083047606;
            }
          } else {
            if (  (data[19].fvalue < 0.16453829)) {
              sum += (float)0.11611637;
            } else {
              sum += (float)-0.00081639917;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 4.0236754)) {
        if (  (data[15].fvalue < 2.999999)) {
          if ( (data[9].fvalue < 1.84738)) {
            if (  (data[19].fvalue < 0.35529378)) {
              sum += (float)0.25454235;
            } else {
              sum += (float)0.54711717;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)0.097223319;
            } else {
              sum += (float)0.18703945;
            }
          }
        } else {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[19].fvalue < 0.13684884)) {
              sum += (float)0.37865561;
            } else {
              sum += (float)0.20243968;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.39154905;
            } else {
              sum += (float)0.59064668;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[19].fvalue < 0.26359436)) {
            if (  (data[12].fvalue < 12.999999)) {
              sum += (float)0.159839;
            } else {
              sum += (float)-0.38727921;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.0061243204;
            } else {
              sum += (float)0.062971421;
            }
          }
        } else {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[19].fvalue < 0.24576315)) {
              sum += (float)0.33926409;
            } else {
              sum += (float)0.093145974;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.44669515;
            } else {
              sum += (float)0.25310597;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 8.999999)) {
    if (  (data[19].fvalue < 0.44447204)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[19].fvalue < 0.02759143)) {
          if (  (data[8].fvalue < 0.21632624)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.025345724;
            } else {
              sum += (float)-0.15164301;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.012533714;
            } else {
              sum += (float)0.095025681;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.059602015)) {
              sum += (float)-0.12633538;
            } else {
              sum += (float)0.051479913;
            }
          } else {
            if (  (data[9].fvalue < 1.1054093)) {
              sum += (float)0.063276425;
            } else {
              sum += (float)-0.11191754;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.15048638)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[8].fvalue < 0.72323906)) {
              sum += (float)0.090055123;
            } else {
              sum += (float)0.25044173;
            }
          } else {
            if (  (data[8].fvalue < 1.142853)) {
              sum += (float)-0.33484131;
            } else {
              sum += (float)0.17584385;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.020003978;
            } else {
              sum += (float)-0.33100659;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.03779681;
            } else {
              sum += (float)0.12976891;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 6.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.84178978)) {
              sum += (float)-0.0030627241;
            } else {
              sum += (float)0.052628789;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.049487807;
            } else {
              sum += (float)-0.30120251;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 1.3146704)) {
              sum += (float)0.010668987;
            } else {
              sum += (float)-0.11449938;
            }
          } else {
            if ( (data[9].fvalue < 1.426294)) {
              sum += (float)0.15443642;
            } else {
              sum += (float)-0.026703209;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.4338285;
            } else {
              sum += (float)-0.51773053;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.29435852;
            } else {
              sum += (float)-0.39973357;
            }
          }
        } else {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.11987194;
            } else {
              sum += (float)0.040328968;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.25500339;
            } else {
              sum += (float)-0.036243159;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.28228715)) {
      if (  (data[13].fvalue < 10.999999)) {
        if (  (data[19].fvalue < 0.13714275)) {
          if (  (data[9].fvalue < 1.1054093)) {
            if (  (data[19].fvalue < 0.059602015)) {
              sum += (float)-0.54933596;
            } else {
              sum += (float)-0.37162131;
            }
          } else {
            if (  (data[19].fvalue < 0.10938516)) {
              sum += (float)-0.74825001;
            } else {
              sum += (float)-0.55633515;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.7213612)) {
            if (  (data[19].fvalue < 0.23027763)) {
              sum += (float)-0.36961433;
            } else {
              sum += (float)-0.18392715;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.14488585;
            } else {
              sum += (float)-0.11226133;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.3146704)) {
          if (  (data[8].fvalue < 0.39513981)) {
            if (  (data[19].fvalue < 0.24689588)) {
              sum += (float)-0.52150828;
            } else {
              sum += (float)-0.28324232;
            }
          } else {
            if (  (data[19].fvalue < 0.18132052)) {
              sum += (float)-0.53124785;
            } else {
              sum += (float)-0.67328358;
            }
          }
        } else {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[19].fvalue < 0.2145368)) {
              sum += (float)-0.66008651;
            } else {
              sum += (float)-0.18472303;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.70549679;
            } else {
              sum += (float)-0.88531625;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 5.999999)) {
        if (  (data[19].fvalue < 0.8006624)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 1.3146704)) {
              sum += (float)0.14524537;
            } else {
              sum += (float)-0.083550714;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.10814727;
            } else {
              sum += (float)0.22419544;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if ( (data[9].fvalue < 1.6526527)) {
              sum += (float)0.1178373;
            } else {
              sum += (float)-0.2385599;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.00010552558;
            } else {
              sum += (float)0.24073359;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.37829402)) {
          if (  (data[13].fvalue < 11.999999)) {
            if ( (data[9].fvalue < 2.8021154)) {
              sum += (float)-0.011909992;
            } else {
              sum += (float)0.2488859;
            }
          } else {
            if (  (data[13].fvalue < 14.999999)) {
              sum += (float)-0.28947556;
            } else {
              sum += (float)-0.78742814;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.94836992)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.16426302;
            } else {
              sum += (float)0.3100197;
            }
          } else {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)0.11227797;
            } else {
              sum += (float)-0.030076606;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.41980961)) {
    if (  (data[13].fvalue < 13.999999)) {
      if (  (data[19].fvalue < 0.46600407)) {
        if (  (data[13].fvalue < 7.999999)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[19].fvalue < -1e-06)) {
              sum += (float)0.54528075;
            } else {
              sum += (float)0.011587;
            }
          } else {
            if (  (data[19].fvalue < 0.12320231)) {
              sum += (float)-0.48223862;
            } else {
              sum += (float)-0.04984542;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.49529529)) {
            if (  (data[18].fvalue < 0.17701173)) {
              sum += (float)-0.38216564;
            } else {
              sum += (float)0.050308313;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.36760256;
            } else {
              sum += (float)-0.69156212;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[18].fvalue < 0.023091778)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.019946568;
            } else {
              sum += (float)-0.046532009;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.064256392;
            } else {
              sum += (float)0.018623246;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.0023097256)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.028125511;
            } else {
              sum += (float)-0.13399236;
            }
          } else {
            if (  (data[18].fvalue < 0.13340418)) {
              sum += (float)0.25537318;
            } else {
              sum += (float)0.063424915;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.760396)) {
        if (  (data[8].fvalue < 0.78126526)) {
          if (  (data[9].fvalue < 1.3696969)) {
            if (  (data[18].fvalue < 0.22440796)) {
              sum += (float)-0.5241698;
            } else {
              sum += (float)0.21168815;
            }
          } else {
            if (  (data[18].fvalue < 0.27389413)) {
              sum += (float)-0.81544131;
            } else {
              sum += (float)-0.49326181;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.37609151)) {
            if (  (data[19].fvalue < 0.18057075)) {
              sum += (float)-0.62865567;
            } else {
              sum += (float)-0.8426308;
            }
          } else {
            if (  (data[13].fvalue < 16.999998)) {
              sum += (float)-1.0161875;
            } else {
              sum += (float)-1.8926915;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 1.1508534)) {
          if (  (data[13].fvalue < 17.999998)) {
            if ( (data[9].fvalue < 1.5385966)) {
              sum += (float)0.30161142;
            } else {
              sum += (float)-0.12188702;
            }
          } else {
            if (  (data[9].fvalue < 2.0687129)) {
              sum += (float)-0.38631836;
            } else {
              sum += (float)-1.2818018;
            }
          }
        } else {
          if (  (data[13].fvalue < 18.999998)) {
            if (  (data[18].fvalue < 0.01517366)) {
              sum += (float)-0.38613248;
            } else {
              sum += (float)0.20737968;
            }
          } else {
            if (  (data[18].fvalue < 0.22440796)) {
              sum += (float)-0.677396;
            } else {
              sum += (float)0.030231675;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.55082166)) {
      if (  (data[13].fvalue < 5.999999)) {
        if (  (data[19].fvalue < 0.23197308)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.10969972)) {
              sum += (float)-0.001346986;
            } else {
              sum += (float)-0.11618349;
            }
          } else {
            if (  (data[19].fvalue < 0.10969972)) {
              sum += (float)0.4328635;
            } else {
              sum += (float)0.13308711;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.37609151)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.10140058;
            } else {
              sum += (float)-0.0040726988;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.090035141;
            } else {
              sum += (float)-0.18166588;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.283757)) {
          if (  (data[18].fvalue < 0.7035026)) {
            if (  (data[19].fvalue < 0.071956061)) {
              sum += (float)-0.51970345;
            } else {
              sum += (float)0.25490537;
            }
          } else {
            if (  (data[19].fvalue < 0.21452466)) {
              sum += (float)0.5283426;
            } else {
              sum += (float)0.39372849;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[19].fvalue < 0.37609151)) {
              sum += (float)0.12569264;
            } else {
              sum += (float)0.01414037;
            }
          } else {
            if (  (data[18].fvalue < 0.66154444)) {
              sum += (float)0.073957019;
            } else {
              sum += (float)0.34408271;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 11.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 1.6094432)) {
            if (  (data[19].fvalue < 3.3620238)) {
              sum += (float)-0.060681108;
            } else {
              sum += (float)0.21555944;
            }
          } else {
            if (  (data[19].fvalue < 1.0093949)) {
              sum += (float)0.11348156;
            } else {
              sum += (float)0.38322282;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.74147981)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.20295943;
            } else {
              sum += (float)-0.023092527;
            }
          } else {
            if (  (data[19].fvalue < 0.80378556)) {
              sum += (float)-0.13866621;
            } else {
              sum += (float)-0.28409496;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 14.999999)) {
          if (  (data[19].fvalue < 1.0743825)) {
            if (  (data[18].fvalue < 1.0721058)) {
              sum += (float)0.14905389;
            } else {
              sum += (float)0.30533364;
            }
          } else {
            if (  (data[18].fvalue < 1.2090739)) {
              sum += (float)0.066662051;
            } else {
              sum += (float)-0.33697972;
            }
          }
        } else {
          if (  (data[13].fvalue < 17.999998)) {
            if (  (data[19].fvalue < 1.3841505)) {
              sum += (float)0.29553518;
            } else {
              sum += (float)0.096133605;
            }
          } else {
            if (  (data[18].fvalue < 0.7035026)) {
              sum += (float)0.13548307;
            } else {
              sum += (float)0.37196174;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 1.999999)) {
    if (  (data[10].fvalue < 0.18577367)) {
      if (  (data[9].fvalue < 0.96603429)) {
        if (  (data[10].fvalue < 0.018497651)) {
          if (  (data[0].fvalue < 0.99999899)) {
            sum += (float)0.14235303;
          } else {
            sum += (float)0.19502746;
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)1.0213933;
            } else {
              sum += (float)1.2528561;
            }
          } else {
            sum += (float)-0.36900687;
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.11884562)) {
              sum += (float)-0.038226616;
            } else {
              sum += (float)-0.1057109;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.028439438;
            } else {
              sum += (float)0.10835319;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99386013)) {
            if (  (data[5].fvalue < 0.53732628)) {
              sum += (float)-0.36077228;
            } else {
              sum += (float)-0.10509069;
            }
          } else {
            if (  (data[9].fvalue < 1.2632166)) {
              sum += (float)0.063877746;
            } else {
              sum += (float)-0.013831696;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.991658)) {
        if (  (data[8].fvalue < 3.7450464)) {
          if (  (data[8].fvalue < 0.025647022)) {
            if (  (data[9].fvalue < 0.96603429)) {
              sum += (float)0.91064841;
            } else {
              sum += (float)0.031529792;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.00019354637;
            } else {
              sum += (float)0.040569931;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 1.6510702)) {
              sum += (float)0.12422509;
            } else {
              sum += (float)0.063637942;
            }
          } else {
            if (  (data[22].fvalue < 2.2153983)) {
              sum += (float)0.48120359;
            } else {
              sum += (float)-0.21384907;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.040800314;
            } else {
              sum += (float)0.0076889354;
            }
          } else {
            if (  (data[9].fvalue < 5.1251287)) {
              sum += (float)-0.014381918;
            } else {
              sum += (float)0.018085266;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.1890562;
            } else {
              sum += (float)0.40466964;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.031454872;
            } else {
              sum += (float)0.24200243;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if ( (data[9].fvalue < 3.2473698)) {
          if (  (data[22].fvalue < 0.020284032)) {
            if ( (data[9].fvalue < 2.1502836)) {
              sum += (float)0.037932996;
            } else {
              sum += (float)-0.0067800777;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.047760613;
            } else {
              sum += (float)0.11518434;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.0035634895;
            } else {
              sum += (float)0.07784494;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.00064132194;
            } else {
              sum += (float)-0.082716092;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 2.2251999)) {
              sum += (float)0.31931153;
            } else {
              sum += (float)0.065503761;
            }
          } else {
            if ( (data[9].fvalue < 3.2473698)) {
              sum += (float)0.54194373;
            } else {
              sum += (float)0.361545;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.053868789;
            } else {
              sum += (float)0.01079014;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.2721813;
            } else {
              sum += (float)0.054285157;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[22].fvalue < 0.090131544)) {
            if ( (data[9].fvalue < 5.2955732)) {
              sum += (float)0.53796202;
            } else {
              sum += (float)0.43832558;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.48567614;
            } else {
              sum += (float)0.28342563;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 3.1766975)) {
              sum += (float)0.60226947;
            } else {
              sum += (float)0.72919589;
            }
          } else {
            if (  (data[22].fvalue < 4.882226)) {
              sum += (float)0.5720582;
            } else {
              sum += (float)0.44522405;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 5.999999)) {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 3.1766975)) {
              sum += (float)-0.29096916;
            } else {
              sum += (float)-0.057139706;
            }
          } else {
            if (  (data[22].fvalue < 4.2443595)) {
              sum += (float)-0.43834999;
            } else {
              sum += (float)-0.31076342;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.47347724)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.52991265;
            } else {
              sum += (float)-0.56791085;
            }
          } else {
            sum += (float)-0.31086269;
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 3.3797884)) {
    if (  (data[8].fvalue < 2.6067209)) {
      if (  (data[19].fvalue < -1e-06)) {
        if (  (data[8].fvalue < 0.91341436)) {
          sum += (float)0.5293473;
        } else {
          sum += (float)0.58212805;
        }
      } else {
        if (  (data[10].fvalue < 1.0598999)) {
          if (  (data[11].fvalue < 0.011627806)) {
            if (  (data[19].fvalue < 0.060309265)) {
              sum += (float)-0.036135148;
            } else {
              sum += (float)0.1340415;
            }
          } else {
            if (  (data[21].fvalue < 0.36869138)) {
              sum += (float)-0.0075375433;
            } else {
              sum += (float)0.0095496718;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.071106218)) {
            if (  (data[8].fvalue < 0.10529323)) {
              sum += (float)0.28324044;
            } else {
              sum += (float)-0.11051248;
            }
          } else {
            if (  (data[8].fvalue < 0.26013201)) {
              sum += (float)0.53334683;
            } else {
              sum += (float)0.0078029744;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 2.2551198)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[11].fvalue < 0.53259748)) {
            if (  (data[10].fvalue < 0.62748623)) {
              sum += (float)0.0031075813;
            } else {
              sum += (float)0.051789872;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.073126055;
            } else {
              sum += (float)0.034777403;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.17543046)) {
            if (  (data[19].fvalue < 0.13830504)) {
              sum += (float)0.033207152;
            } else {
              sum += (float)0.25076512;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.055171434;
            } else {
              sum += (float)0.12257235;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.025039088)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 1.2370762)) {
              sum += (float)-0.016178029;
            } else {
              sum += (float)-0.044562016;
            }
          } else {
            if (  (data[10].fvalue < 0.28801692)) {
              sum += (float)0.045107458;
            } else {
              sum += (float)0.1853047;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.11999086)) {
            if (  (data[19].fvalue < 0.083844446)) {
              sum += (float)0.030994523;
            } else {
              sum += (float)0.100226;
            }
          } else {
            if (  (data[21].fvalue < 0.33639342)) {
              sum += (float)-0.055277292;
            } else {
              sum += (float)0.0082073435;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 5.999999)) {
      if (  (data[20].fvalue < 4.444757)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.6792174)) {
            if (  (data[20].fvalue < 0.020102736)) {
              sum += (float)-0.037891965;
            } else {
              sum += (float)-0.016777456;
            }
          } else {
            if (  (data[20].fvalue < 0.020102736)) {
              sum += (float)-0.089964047;
            } else {
              sum += (float)-0.04215363;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 1.8906407)) {
              sum += (float)0.016144127;
            } else {
              sum += (float)-0.021080198;
            }
          } else {
            if (  (data[20].fvalue < 2.5701084)) {
              sum += (float)0.070581503;
            } else {
              sum += (float)0.017046446;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.16451302)) {
          if (  (data[19].fvalue < 0.6792174)) {
            if (  (data[21].fvalue < 5.3438497)) {
              sum += (float)-0.020453772;
            } else {
              sum += (float)-0.041376986;
            }
          } else {
            if (  (data[19].fvalue < 6.8117228)) {
              sum += (float)-0.07386943;
            } else {
              sum += (float)0.0038190633;
            }
          }
        } else {
          if (  (data[14].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.0084094629)) {
              sum += (float)-0.043537915;
            } else {
              sum += (float)-0.026309241;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.081012115;
            } else {
              sum += (float)-0.26423696;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 6.999999)) {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.5396502)) {
              sum += (float)-0.44438311;
            } else {
              sum += (float)-0.5068903;
            }
          } else {
            if (  (data[19].fvalue < 0.071106218)) {
              sum += (float)-0.34702182;
            } else {
              sum += (float)-0.43999285;
            }
          }
        } else {
          sum += (float)-0.52267808;
        }
      } else {
        sum += (float)-0.54344803;
      }
    }
  }
  if (  (data[12].fvalue < 16.999998)) {
    if (  (data[12].fvalue < 2.999999)) {
      if (  (data[9].fvalue < 0.99568439)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.92243314)) {
              sum += (float)-0.03260741;
            } else {
              sum += (float)0.034017447;
            }
          } else {
            if (  (data[8].fvalue < 5.7103686)) {
              sum += (float)-0.3380951;
            } else {
              sum += (float)-0.07961598;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.7134461)) {
            if (  (data[8].fvalue < 0.40018442)) {
              sum += (float)0.1225858;
            } else {
              sum += (float)0.16451661;
            }
          } else {
            if (  (data[8].fvalue < 7.8819757)) {
              sum += (float)-0.074422836;
            } else {
              sum += (float)0.18019737;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.063551791)) {
          if (  (data[9].fvalue < 1.2658522)) {
            if (  (data[11].fvalue < 0.823448)) {
              sum += (float)-0.014205394;
            } else {
              sum += (float)-0.45653668;
            }
          } else {
            if (  (data[9].fvalue < 1.9255176)) {
              sum += (float)-0.14725773;
            } else {
              sum += (float)0.0045430125;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.019640189)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.011039693;
            } else {
              sum += (float)-0.039353356;
            }
          } else {
            if (  (data[20].fvalue < 0.11314394)) {
              sum += (float)0.059066597;
            } else {
              sum += (float)-0.0032535628;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 2.999999)) {
        if ( (data[9].fvalue < 1.5984467)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.71903062)) {
              sum += (float)0.049912564;
            } else {
              sum += (float)-0.026056491;
            }
          } else {
            if (  (data[8].fvalue < 1.9421607)) {
              sum += (float)0.1054448;
            } else {
              sum += (float)0.26871654;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.040435113;
            } else {
              sum += (float)-0.0049014902;
            }
          } else {
            if (  (data[20].fvalue < 0.11314394)) {
              sum += (float)0.078882471;
            } else {
              sum += (float)0.012787944;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 6.0966005)) {
              sum += (float)-0.052932635;
            } else {
              sum += (float)0.022509703;
            }
          } else {
            if ( (data[9].fvalue < 5.5897017)) {
              sum += (float)0.098593503;
            } else {
              sum += (float)0.031206945;
            }
          }
        } else {
          if ( (data[9].fvalue < 5.5897017)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.17774077;
            } else {
              sum += (float)0.35664272;
            }
          } else {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.058094256;
            } else {
              sum += (float)0.32535073;
            }
          }
        }
      }
    }
  } else {
    if (  (data[15].fvalue < 2.999999)) {
      if (  (data[20].fvalue < 0.49647528)) {
        if ( (data[9].fvalue < 1.7800009)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[6].fvalue < 5.1340671)) {
              sum += (float)-0.34053144;
            } else {
              sum += (float)-0.50538987;
            }
          } else {
            sum += (float)0.061650783;
          }
        } else {
          if (  (data[9].fvalue < 2.8861718)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.1007209;
            } else {
              sum += (float)0.088676587;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.25810316;
            } else {
              sum += (float)-0.065800339;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 1.8526337)) {
            sum += (float)-0.83310956;
          } else {
            if (  (data[12].fvalue < 17.999998)) {
              sum += (float)-0.42765316;
            } else {
              sum += (float)-0.53037083;
            }
          }
        } else {
          if ( (data[9].fvalue < 5.4417357)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.29553017;
            } else {
              sum += (float)-0.04849799;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.49286705;
            } else {
              sum += (float)-0.36617494;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 3.999999)) {
        if ( (data[9].fvalue < 4.6223769)) {
          if (  (data[20].fvalue < 0.11314394)) {
            if ( (data[9].fvalue < 2.9731562)) {
              sum += (float)0.39716277;
            } else {
              sum += (float)0.26110342;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.0047278842;
            } else {
              sum += (float)0.20180269;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.11314394)) {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)0.19975853;
            } else {
              sum += (float)0.013353904;
            }
          } else {
            if (  (data[12].fvalue < 17.999998)) {
              sum += (float)-0.071266964;
            } else {
              sum += (float)-0.24078113;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 4.999999)) {
          if ( (data[9].fvalue < 5.1321669)) {
            if ( (data[9].fvalue < 4.8417025)) {
              sum += (float)0.40446612;
            } else {
              sum += (float)0.22388515;
            }
          } else {
            if (  (data[20].fvalue < 0.14266799)) {
              sum += (float)0.30410093;
            } else {
              sum += (float)0.018914374;
            }
          }
        } else {
          if ( (data[9].fvalue < 6.3575568)) {
            if (  (data[12].fvalue < 17.999998)) {
              sum += (float)0.59978557;
            } else {
              sum += (float)0.49671683;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)0.23762138;
            } else {
              sum += (float)0.4838317;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 8.999999)) {
    if (  (data[19].fvalue < -1e-06)) {
      sum += (float)0.51925409;
    } else {
      if (  (data[19].fvalue < 0.46784088)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.1433046)) {
              sum += (float)0.015678316;
            } else {
              sum += (float)-0.007940283;
            }
          } else {
            if (  (data[19].fvalue < 0.11075305)) {
              sum += (float)-0.03896556;
            } else {
              sum += (float)0.0063649644;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.0093334643)) {
            if (  (data[8].fvalue < 0.39782077)) {
              sum += (float)-0.4118917;
            } else {
              sum += (float)0.18223725;
            }
          } else {
            if (  (data[19].fvalue < 0.12447719)) {
              sum += (float)0.058822829;
            } else {
              sum += (float)0.0050878772;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 1.4646965)) {
            if (  (data[8].fvalue < 1.2244281)) {
              sum += (float)-0.0094263069;
            } else {
              sum += (float)0.030442109;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.015657822;
            } else {
              sum += (float)0.026944228;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.37253979)) {
            if (  (data[20].fvalue < 0.020533662)) {
              sum += (float)-0.068933308;
            } else {
              sum += (float)-0.028082754;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.019921295;
            } else {
              sum += (float)-0.015859824;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.23246369)) {
      if (  (data[19].fvalue < 0.12447719)) {
        if (  (data[10].fvalue < 0.49293539)) {
          if (  (data[6].fvalue < 0.2229248)) {
            if (  (data[19].fvalue < 0.060376432)) {
              sum += (float)-0.45908737;
            } else {
              sum += (float)0.18761143;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.493902;
            } else {
              sum += (float)-0.58903778;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.07162609)) {
            if (  (data[19].fvalue < 0.038333621)) {
              sum += (float)-0.50553244;
            } else {
              sum += (float)-0.39447603;
            }
          } else {
            if (  (data[8].fvalue < 0.18351075)) {
              sum += (float)0.12576069;
            } else {
              sum += (float)-0.25724018;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 11.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.16332866;
            } else {
              sum += (float)-0.31644642;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.079152726;
            } else {
              sum += (float)-0.15193814;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.62623596)) {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)-0.47575587;
            } else {
              sum += (float)-0.6045506;
            }
          } else {
            if (  (data[8].fvalue < 0.45140281)) {
              sum += (float)-0.0032438119;
            } else {
              sum += (float)-0.35197791;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 1.1510727)) {
        if (  (data[19].fvalue < 0.31936845)) {
          if (  (data[13].fvalue < 11.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.068995677;
            } else {
              sum += (float)0.10030964;
            }
          } else {
            if (  (data[13].fvalue < 15.999999)) {
              sum += (float)-0.21681184;
            } else {
              sum += (float)-0.57438087;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.022961391)) {
            if (  (data[11].fvalue < 0.010862721)) {
              sum += (float)0.79223007;
            } else {
              sum += (float)0.25087887;
            }
          } else {
            if (  (data[19].fvalue < 0.55189782)) {
              sum += (float)0.038560197;
            } else {
              sum += (float)0.093245633;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 16.999998)) {
          if (  (data[10].fvalue < 0.33867049)) {
            if (  (data[20].fvalue < 5.3595018)) {
              sum += (float)-0.0097087417;
            } else {
              sum += (float)-0.081531145;
            }
          } else {
            if (  (data[10].fvalue < 0.59074181)) {
              sum += (float)0.040114794;
            } else {
              sum += (float)0.13476381;
            }
          }
        } else {
          if (  (data[13].fvalue < 19.999998)) {
            if (  (data[22].fvalue < 0.57158267)) {
              sum += (float)0.23504853;
            } else {
              sum += (float)0.068716414;
            }
          } else {
            if (  (data[22].fvalue < 0.62117004)) {
              sum += (float)0.37283209;
            } else {
              sum += (float)0.26579225;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 14.999999)) {
    if (  (data[17].fvalue < 6.999999)) {
      if (  (data[13].fvalue < 7.999999)) {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0002567604;
            } else {
              sum += (float)0.023896152;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.10983653;
            } else {
              sum += (float)0.014127024;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.071978852)) {
              sum += (float)0.016644061;
            } else {
              sum += (float)-0.14014617;
            }
          } else {
            if (  (data[19].fvalue < 0.071978852)) {
              sum += (float)-0.44648466;
            } else {
              sum += (float)0.054440808;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[19].fvalue < 0.64867485)) {
            if (  (data[22].fvalue < 0.11906091)) {
              sum += (float)0.017868552;
            } else {
              sum += (float)-0.1128585;
            }
          } else {
            if (  (data[20].fvalue < 0.019858178)) {
              sum += (float)-0.32361498;
            } else {
              sum += (float)-0.13154526;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.13996577)) {
            if (  (data[19].fvalue < 0.097919546)) {
              sum += (float)-0.49951914;
            } else {
              sum += (float)-0.24979803;
            }
          } else {
            if (  (data[19].fvalue < 0.25005797)) {
              sum += (float)-0.089879453;
            } else {
              sum += (float)0.062326975;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.14341144)) {
        sum += (float)-0.57526636;
      } else {
        if (  (data[13].fvalue < 7.999999)) {
          sum += (float)-0.53582859;
        } else {
          sum += (float)-0.58595222;
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 12.999999)) {
      if (  (data[19].fvalue < 0.84890825)) {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.53313202;
            } else {
              sum += (float)-0.46765941;
            }
          } else {
            if (  (data[19].fvalue < 0.25005797)) {
              sum += (float)0.11341244;
            } else {
              sum += (float)-0.4157078;
            }
          }
        } else {
          if (  (data[12].fvalue < 17.999998)) {
            if (  (data[20].fvalue < 0.11347555)) {
              sum += (float)0.026796438;
            } else {
              sum += (float)-0.16100447;
            }
          } else {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)-0.34730399;
            } else {
              sum += (float)-0.53387737;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 17.999998)) {
          if (  (data[20].fvalue < 0.11347555)) {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.28755808;
            } else {
              sum += (float)-0.10599532;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.43364561;
            } else {
              sum += (float)-0.31756788;
            }
          }
        } else {
          if (  (data[13].fvalue < 10.999999)) {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)-0.51863855;
            } else {
              sum += (float)-0.60988325;
            }
          } else {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)-0.63181013;
            } else {
              sum += (float)-0.8359009;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.68308496)) {
        if (  (data[13].fvalue < 16.999998)) {
          if (  (data[19].fvalue < 0.55205613)) {
            if (  (data[20].fvalue < 0.091553435)) {
              sum += (float)-0.22703874;
            } else {
              sum += (float)-0.45315066;
            }
          } else {
            if (  (data[10].fvalue < 0.58998334)) {
              sum += (float)-0.033945184;
            } else {
              sum += (float)-0.52439296;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.091553435)) {
            if (  (data[19].fvalue < 0.58247894)) {
              sum += (float)-0.53686035;
            } else {
              sum += (float)-0.30667564;
            }
          } else {
            if (  (data[13].fvalue < 18.999998)) {
              sum += (float)-0.61757654;
            } else {
              sum += (float)-0.77749527;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 15.999999)) {
          if (  (data[20].fvalue < 0.17419179)) {
            if (  (data[12].fvalue < 17.999998)) {
              sum += (float)0.17368166;
            } else {
              sum += (float)-0.1142961;
            }
          } else {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)-0.20206514;
            } else {
              sum += (float)-0.02897408;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.90182447)) {
            if (  (data[20].fvalue < 0.25239393)) {
              sum += (float)0.066082627;
            } else {
              sum += (float)-0.17844546;
            }
          } else {
            if (  (data[10].fvalue < 0.66896027)) {
              sum += (float)0.24582396;
            } else {
              sum += (float)-0.046574399;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.46495309)) {
    if (  (data[13].fvalue < 15.999999)) {
      if (  (data[18].fvalue < 0.0040667518)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 1.1045133)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.26614732;
            } else {
              sum += (float)-0.086542889;
            }
          } else {
            if ( (data[9].fvalue < 1.7151518)) {
              sum += (float)0.070013016;
            } else {
              sum += (float)0.0059305471;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.1045133)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.16165809;
            } else {
              sum += (float)0.0091767283;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.055423982;
            } else {
              sum += (float)0.037614793;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.0324328)) {
              sum += (float)0.022770358;
            } else {
              sum += (float)-0.053754218;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.030698296;
            } else {
              sum += (float)-0.020676916;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.07189469)) {
            if (  (data[9].fvalue < 1.4311833)) {
              sum += (float)0.0052060685;
            } else {
              sum += (float)0.14399493;
            }
          } else {
            if ( (data[9].fvalue < 1.2674713)) {
              sum += (float)0.070070125;
            } else {
              sum += (float)0.014515103;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.19101185)) {
        if (  (data[14].fvalue < 2.999999)) {
          if ( (data[9].fvalue < 1.2674713)) {
            if (  (data[18].fvalue < 0.11900882)) {
              sum += (float)-0.50201184;
            } else {
              sum += (float)-0.21866684;
            }
          } else {
            if (  (data[13].fvalue < 19.999998)) {
              sum += (float)-0.61142367;
            } else {
              sum += (float)-0.89280653;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 16.999998)) {
              sum += (float)0.11229075;
            } else {
              sum += (float)-0.083908722;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.33674526;
            } else {
              sum += (float)-0.69208467;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 19.999998)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.074855283;
            } else {
              sum += (float)-0.21857843;
            }
          } else {
            if (  (data[18].fvalue < 0.30774596)) {
              sum += (float)-0.57699108;
            } else {
              sum += (float)-0.25061125;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 19.999998)) {
              sum += (float)-0.48451254;
            } else {
              sum += (float)-0.87079656;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.13766941;
            } else {
              sum += (float)-0.2260543;
            }
          }
        }
      }
    }
  } else {
    if (  (data[0].fvalue < 0.99999899)) {
      if (  (data[15].fvalue < 0.99999899)) {
        if (  (data[22].fvalue < 0.021227544)) {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.21329662;
            } else {
              sum += (float)-0.023061236;
            }
          } else {
            if (  (data[9].fvalue < 1.6546522)) {
              sum += (float)0.13532312;
            } else {
              sum += (float)-0.07183212;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.1539156)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0028591538;
            } else {
              sum += (float)0.29596251;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0045935679;
            } else {
              sum += (float)-0.098488003;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.222995)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 5.0233774)) {
              sum += (float)-0.047727693;
            } else {
              sum += (float)0.21737438;
            }
          } else {
            if ( (data[9].fvalue < 1.7784621)) {
              sum += (float)0.15705995;
            } else {
              sum += (float)0.041161917;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 4.9733868)) {
              sum += (float)-0.027215743;
            } else {
              sum += (float)0.01008629;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.098251626;
            } else {
              sum += (float)-0.044350442;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.073479369;
            } else {
              sum += (float)-0.02405511;
            }
          } else {
            if (  (data[9].fvalue < 1.9205526)) {
              sum += (float)0.16884434;
            } else {
              sum += (float)0.029922891;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.4311833)) {
              sum += (float)0.081928812;
            } else {
              sum += (float)0.0097094625;
            }
          } else {
            if (  (data[9].fvalue < 2.1539156)) {
              sum += (float)0.32545441;
            } else {
              sum += (float)0.094078593;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 9.999999)) {
          if (  (data[10].fvalue < 0.26258221)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.0032552555;
            } else {
              sum += (float)-0.040675476;
            }
          } else {
            if ( (data[9].fvalue < 2.1539156)) {
              sum += (float)0.027129985;
            } else {
              sum += (float)-0.0080454824;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.222995)) {
            if (  (data[18].fvalue < 0.61802018)) {
              sum += (float)0.049677424;
            } else {
              sum += (float)0.2402526;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.013883979;
            } else {
              sum += (float)0.08921928;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.58672142)) {
    if (  (data[19].fvalue < 0.30226681)) {
      if (  (data[18].fvalue < 0.096264027)) {
        if (  (data[19].fvalue < 0.085032962)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 0.96695232)) {
              sum += (float)0.25488743;
            } else {
              sum += (float)-0.0416364;
            }
          } else {
            if (  (data[9].fvalue < 1.1107861)) {
              sum += (float)-0.20169155;
            } else {
              sum += (float)-0.017894201;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0305673)) {
            if (  (data[8].fvalue < 0.10470852)) {
              sum += (float)0.56196845;
            } else {
              sum += (float)0.055838287;
            }
          } else {
            if ( (data[9].fvalue < 1.4863181)) {
              sum += (float)-0.032565657;
            } else {
              sum += (float)0.021766616;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.097607747)) {
          if ( (data[9].fvalue < 1.6022378)) {
            if (  (data[9].fvalue < 1.0305673)) {
              sum += (float)0.057252068;
            } else {
              sum += (float)-0.026125601;
            }
          } else {
            if (  (data[18].fvalue < 0.27515876)) {
              sum += (float)0.059778713;
            } else {
              sum += (float)0.025575204;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.30839947)) {
            if (  (data[9].fvalue < 1.1781564)) {
              sum += (float)0.44243032;
            } else {
              sum += (float)0.010503828;
            }
          } else {
            if (  (data[18].fvalue < 0.30990604)) {
              sum += (float)-0.094116025;
            } else {
              sum += (float)-0.0056349467;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.3185441)) {
        if (  (data[8].fvalue < 0.30839947)) {
          if (  (data[18].fvalue < 0.1205857)) {
            if (  (data[19].fvalue < 0.4433403)) {
              sum += (float)0.028452827;
            } else {
              sum += (float)0.38036948;
            }
          } else {
            if (  (data[9].fvalue < 1.2667135)) {
              sum += (float)0.66975898;
            } else {
              sum += (float)0.41018265;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.0689454)) {
            if (  (data[18].fvalue < 0.32983923)) {
              sum += (float)0.047713038;
            } else {
              sum += (float)0.3216387;
            }
          } else {
            if (  (data[18].fvalue < 0.20656194)) {
              sum += (float)0.020846773;
            } else {
              sum += (float)-0.16286281;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.8034062)) {
          if (  (data[20].fvalue < 0.020869028)) {
            if (  (data[22].fvalue < 0.021323999)) {
              sum += (float)-0.059268307;
            } else {
              sum += (float)0.0011717572;
            }
          } else {
            if (  (data[20].fvalue < 0.11193247)) {
              sum += (float)0.075389087;
            } else {
              sum += (float)0.0046415506;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.36952308)) {
            if (  (data[18].fvalue < 0.0038529495)) {
              sum += (float)0.015734611;
            } else {
              sum += (float)0.040298283;
            }
          } else {
            if (  (data[20].fvalue < 0.11193247)) {
              sum += (float)0.045486405;
            } else {
              sum += (float)-0.033292238;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.682419)) {
      if (  (data[19].fvalue < 0.46747339)) {
        if (  (data[19].fvalue < 0.0078343973)) {
          if ( (data[9].fvalue < 1.7823431)) {
            if (  (data[8].fvalue < 1.0689454)) {
              sum += (float)-0.10514248;
            } else {
              sum += (float)-0.034725979;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.0062486343;
            } else {
              sum += (float)0.06254936;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.4863181)) {
            if (  (data[8].fvalue < 0.78342205)) {
              sum += (float)0.2429228;
            } else {
              sum += (float)0.04656313;
            }
          } else {
            if (  (data[19].fvalue < 0.01922843)) {
              sum += (float)0.097633846;
            } else {
              sum += (float)0.014651002;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.020869028)) {
          if (  (data[8].fvalue < 0.78342205)) {
            if (  (data[9].fvalue < 1.6022378)) {
              sum += (float)0.45877442;
            } else {
              sum += (float)-0.042244561;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.11563046;
            } else {
              sum += (float)-0.037362922;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.11193247)) {
            if ( (data[9].fvalue < 1.9250609)) {
              sum += (float)0.24400179;
            } else {
              sum += (float)0.12131597;
            }
          } else {
            if ( (data[9].fvalue < 1.9250609)) {
              sum += (float)0.101393;
            } else {
              sum += (float)-0.023764819;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 6.8056116)) {
        if ( (data[9].fvalue < 2.3132899)) {
          if (  (data[20].fvalue < 0.020869028)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.18967685;
            } else {
              sum += (float)0.073042192;
            }
          } else {
            if ( (data[9].fvalue < 2.0000505)) {
              sum += (float)0.20600778;
            } else {
              sum += (float)0.047675181;
            }
          }
        } else {
          if (  (data[18].fvalue < 4.1847081)) {
            if (  (data[18].fvalue < 0.999107)) {
              sum += (float)-0.082291715;
            } else {
              sum += (float)-0.1593795;
            }
          } else {
            if (  (data[9].fvalue < 4.9820395)) {
              sum += (float)-0.087342918;
            } else {
              sum += (float)0.016455168;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.4756751)) {
          if (  (data[18].fvalue < 0.93537122)) {
            if ( (data[9].fvalue < 2.0000505)) {
              sum += (float)0.47953135;
            } else {
              sum += (float)0.28065419;
            }
          } else {
            if (  (data[20].fvalue < 0.020869028)) {
              sum += (float)0.54085296;
            } else {
              sum += (float)0.68010241;
            }
          }
        } else {
          if (  (data[9].fvalue < 5.5852466)) {
            if (  (data[20].fvalue < 0.11193247)) {
              sum += (float)0.11314935;
            } else {
              sum += (float)-0.039781224;
            }
          } else {
            if (  (data[18].fvalue < 1.2038399)) {
              sum += (float)0.098626338;
            } else {
              sum += (float)0.1967224;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 2.999999)) {
    if (  (data[13].fvalue < 5.999999)) {
      if (  (data[21].fvalue < 0.12019975)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[21].fvalue < 0.048946377)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.00720928;
            } else {
              sum += (float)-0.019508369;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.1242634;
            } else {
              sum += (float)0.019215688;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.048946377)) {
              sum += (float)0.013623185;
            } else {
              sum += (float)0.21984628;
            }
          } else {
            if ( (data[9].fvalue < 1.7217423)) {
              sum += (float)0.15091527;
            } else {
              sum += (float)0.07707718;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.30453381)) {
          if ( (data[9].fvalue < 1.179394)) {
            if ( (data[9].fvalue < 1.1127505)) {
              sum += (float)-0.31824866;
            } else {
              sum += (float)-0.17386445;
            }
          } else {
            if (  (data[21].fvalue < 0.14580519)) {
              sum += (float)-0.016757157;
            } else {
              sum += (float)-0.089002885;
            }
          }
        } else {
          if (  (data[21].fvalue < 1.7458463)) {
            if (  (data[8].fvalue < 0.10738851)) {
              sum += (float)-0.099522196;
            } else {
              sum += (float)0.012115167;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.013040693;
            } else {
              sum += (float)-0.09406434;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.4361773)) {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.6044398)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.20498943;
            } else {
              sum += (float)0.042552665;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.27824122;
            } else {
              sum += (float)-0.046173319;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.44823605;
            } else {
              sum += (float)0.23534325;
            }
          } else {
            if ( (data[9].fvalue < 1.2217414)) {
              sum += (float)0.69729251;
            } else {
              sum += (float)0.55461377;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[21].fvalue < 0.14580519)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.23067328;
            } else {
              sum += (float)-0.0834243;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.40217346;
            } else {
              sum += (float)-0.24701735;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.14580519)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.058159657;
            } else {
              sum += (float)0.26150557;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.091686368;
            } else {
              sum += (float)0.18706226;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.99999899)) {
      if (  (data[12].fvalue < 6.999999)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 1.0354961)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.26998237;
            } else {
              sum += (float)0.091107324;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.024045154;
            } else {
              sum += (float)-0.12968171;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if ( (data[9].fvalue < 4.6233377)) {
              sum += (float)-0.22383884;
            } else {
              sum += (float)-0.13001955;
            }
          } else {
            if (  (data[21].fvalue < 0.025078502)) {
              sum += (float)-0.6369251;
            } else {
              sum += (float)-0.24911667;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.3735124)) {
          if (  (data[8].fvalue < 0.79036391)) {
            if (  (data[9].fvalue < 1.179394)) {
              sum += (float)0.52750492;
            } else {
              sum += (float)0.069797412;
            }
          } else {
            if (  (data[8].fvalue < 1.8383089)) {
              sum += (float)-0.22574282;
            } else {
              sum += (float)-0.45712838;
            }
          }
        } else {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[8].fvalue < 0.85645694)) {
              sum += (float)-0.2248034;
            } else {
              sum += (float)-0.36355034;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.43681893;
            } else {
              sum += (float)-0.49732828;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[21].fvalue < 0.025078502)) {
              sum += (float)0.084952809;
            } else {
              sum += (float)0.13260347;
            }
          } else {
            if ( (data[9].fvalue < 1.3735124)) {
              sum += (float)0.21875782;
            } else {
              sum += (float)-0.012960151;
            }
          }
        } else {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[21].fvalue < 0.025078502)) {
              sum += (float)-0.21559699;
            } else {
              sum += (float)0.00089301023;
            }
          } else {
            if (  (data[21].fvalue < 0.025078502)) {
              sum += (float)-0.50855124;
            } else {
              sum += (float)-0.17571212;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[9].fvalue < 1.0354961)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.28523177;
            } else {
              sum += (float)-0.054370243;
            }
          } else {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)-0.014083039;
            } else {
              sum += (float)-0.27128825;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[21].fvalue < 0.12019975)) {
              sum += (float)0.026198115;
            } else {
              sum += (float)-0.048510358;
            }
          } else {
            if ( (data[9].fvalue < 2.3172297)) {
              sum += (float)0.0076588406;
            } else {
              sum += (float)0.062935919;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 5.353476)) {
    if (  (data[15].fvalue < 2.999999)) {
      if (  (data[12].fvalue < 12.999999)) {
        if (  (data[18].fvalue < 0.29308835)) {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0016831323;
            } else {
              sum += (float)0.059911903;
            }
          } else {
            if (  (data[18].fvalue < 0.16220574)) {
              sum += (float)-0.59863812;
            } else {
              sum += (float)-0.10543204;
            }
          }
        } else {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0063542621;
            } else {
              sum += (float)-0.022854459;
            }
          } else {
            if (  (data[18].fvalue < 0.88233733)) {
              sum += (float)0.11532243;
            } else {
              sum += (float)-0.026531534;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.3284156)) {
          if (  (data[18].fvalue < 0.24045052)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.60705614;
            } else {
              sum += (float)-0.76107943;
            }
          } else {
            if (  (data[21].fvalue < 0.17730172)) {
              sum += (float)-0.32775947;
            } else {
              sum += (float)-0.54147279;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.3783681)) {
            if (  (data[18].fvalue < 0.55272532)) {
              sum += (float)-0.13833305;
            } else {
              sum += (float)0.10133324;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.19901556;
            } else {
              sum += (float)-0.0041797459;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[18].fvalue < 0.191185)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.028565791;
            } else {
              sum += (float)-0.096845791;
            }
          } else {
            if (  (data[18].fvalue < 0.11911368)) {
              sum += (float)0.44108197;
            } else {
              sum += (float)0.22808589;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.031918265;
            } else {
              sum += (float)-0.15450336;
            }
          } else {
            if (  (data[21].fvalue < 1.0335258)) {
              sum += (float)-0.12102989;
            } else {
              sum += (float)-0.016429782;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 1.2701237)) {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[11].fvalue < 0.34165764)) {
              sum += (float)0.15633579;
            } else {
              sum += (float)0.30385274;
            }
          } else {
            if (  (data[18].fvalue < 0.25437012)) {
              sum += (float)-0.45229626;
            } else {
              sum += (float)0.39299688;
            }
          }
        } else {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 1.3783681)) {
              sum += (float)0.13523348;
            } else {
              sum += (float)0.040430825;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.42321914;
            } else {
              sum += (float)0.22515577;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 1.286895)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.023267528)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.18849508;
            } else {
              sum += (float)-0.068658285;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.0269442;
            } else {
              sum += (float)-0.31228408;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.58592194)) {
            if (  (data[18].fvalue < 0.02312611)) {
              sum += (float)-0.0068805581;
            } else {
              sum += (float)-0.041044593;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.0077095134;
            } else {
              sum += (float)0.02816575;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.16220574)) {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.08418566;
            } else {
              sum += (float)0.23104629;
            }
          } else {
            if (  (data[18].fvalue < 0.10728483)) {
              sum += (float)-0.6860401;
            } else {
              sum += (float)-0.23232596;
            }
          }
        } else {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.52036834)) {
              sum += (float)0.001048302;
            } else {
              sum += (float)-0.044682194;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.020240387;
            } else {
              sum += (float)0.14610492;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.0017416617;
            } else {
              sum += (float)-0.047723986;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.085046172;
            } else {
              sum += (float)-0.002048596;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.086277477;
            } else {
              sum += (float)-0.03107509;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.11136682;
            } else {
              sum += (float)-0.30106032;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 4.999999)) {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.07738436;
            } else {
              sum += (float)0.0038798626;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.22821783;
            } else {
              sum += (float)-0.065931171;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.27377912;
            } else {
              sum += (float)0.5876413;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)0.04071844;
            } else {
              sum += (float)0.33834088;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 4.4647369)) {
    if (  (data[18].fvalue < 0.16147757)) {
      if (  (data[12].fvalue < 6.999999)) {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.032802202;
            } else {
              sum += (float)-0.0074480702;
            }
          } else {
            if (  (data[8].fvalue < 0.85171264)) {
              sum += (float)-0.0067817145;
            } else {
              sum += (float)0.12700297;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.095604926)) {
            if (  (data[18].fvalue < 0.061698128)) {
              sum += (float)-0.53568953;
            } else {
              sum += (float)-0.30327389;
            }
          } else {
            if (  (data[8].fvalue < 1.2417822)) {
              sum += (float)-0.084736735;
            } else {
              sum += (float)0.20000951;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.10747503)) {
          if (  (data[13].fvalue < 10.999999)) {
            if (  (data[18].fvalue < 0.084234402)) {
              sum += (float)-0.58213794;
            } else {
              sum += (float)-0.44787902;
            }
          } else {
            if (  (data[18].fvalue < 0.084234402)) {
              sum += (float)-0.71114004;
            } else {
              sum += (float)-0.61191189;
            }
          }
        } else {
          if (  (data[12].fvalue < 10.999999)) {
            if (  (data[8].fvalue < 1.4113655)) {
              sum += (float)-0.2718586;
            } else {
              sum += (float)-0.021000685;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.26631638;
            } else {
              sum += (float)-0.6030696;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[22].fvalue < 0.37274763)) {
            if (  (data[22].fvalue < 0.15170823)) {
              sum += (float)-0.011585976;
            } else {
              sum += (float)-0.078001373;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.017960902;
            } else {
              sum += (float)-0.02072702;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.73985809)) {
              sum += (float)-0.034724031;
            } else {
              sum += (float)0.04299432;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.22105505;
            } else {
              sum += (float)-0.085397638;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[18].fvalue < 0.59011745)) {
              sum += (float)-0.035716485;
            } else {
              sum += (float)-0.20322156;
            }
          } else {
            if (  (data[20].fvalue < 0.59135586)) {
              sum += (float)0.02561391;
            } else {
              sum += (float)-0.048418693;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.14430316)) {
            if (  (data[8].fvalue < 0.40009153)) {
              sum += (float)0.18975706;
            } else {
              sum += (float)0.075173773;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.018654563;
            } else {
              sum += (float)-0.0592051;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 3.8908658)) {
      if (  (data[22].fvalue < 0.11843292)) {
        if (  (data[20].fvalue < 0.11385651)) {
          if (  (data[18].fvalue < 0.023025975)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.17532355;
            } else {
              sum += (float)0.061608892;
            }
          } else {
            if (  (data[20].fvalue < 0.021357384)) {
              sum += (float)0.044089336;
            } else {
              sum += (float)0.13927935;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.020391991)) {
            if (  (data[8].fvalue < 9.32267)) {
              sum += (float)0.0026392355;
            } else {
              sum += (float)0.037404221;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.053844228;
            } else {
              sum += (float)0.16164881;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.01772012;
            } else {
              sum += (float)-0.12515268;
            }
          } else {
            if (  (data[18].fvalue < 0.023025975)) {
              sum += (float)0.11567043;
            } else {
              sum += (float)0.0054684407;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.014951448;
            } else {
              sum += (float)-0.1901444;
            }
          } else {
            if (  (data[18].fvalue < 0.17793359)) {
              sum += (float)0.185912;
            } else {
              sum += (float)0.055532303;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[18].fvalue < 0.46804228)) {
            if (  (data[18].fvalue < 0.023025975)) {
              sum += (float)-0.012242982;
            } else {
              sum += (float)-0.052093077;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.0042743012;
            } else {
              sum += (float)0.030789318;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.063093781;
            } else {
              sum += (float)-0.29014406;
            }
          } else {
            if (  (data[18].fvalue < 0.023025975)) {
              sum += (float)0.098222695;
            } else {
              sum += (float)-0.10125016;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.14740805)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.072380826)) {
              sum += (float)0.25819814;
            } else {
              sum += (float)0.022921786;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.48895299;
            } else {
              sum += (float)0.2017905;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.046186619;
            } else {
              sum += (float)0.0073198313;
            }
          } else {
            if (  (data[18].fvalue < 0.22317709)) {
              sum += (float)0.062228806;
            } else {
              sum += (float)-0.054706726;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 1.999999)) {
    if (  (data[20].fvalue < 3.4751437)) {
      if (  (data[22].fvalue < 6.36939)) {
        if (  (data[20].fvalue < 0.35337028)) {
          if (  (data[20].fvalue < 0.14477776)) {
            if (  (data[20].fvalue < 0.048999321)) {
              sum += (float)-0.0030293637;
            } else {
              sum += (float)0.05433441;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.0063701849;
            } else {
              sum += (float)-0.054276329;
            }
          }
        } else {
          if (  (data[17].fvalue < 3.999999)) {
            if (  (data[20].fvalue < 1.0245156)) {
              sum += (float)0.023452552;
            } else {
              sum += (float)0.0014898608;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.32627633;
            } else {
              sum += (float)-0.23487213;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.020000871)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.092309631;
            } else {
              sum += (float)-0.048190646;
            }
          } else {
            if (  (data[19].fvalue < 0.32061628)) {
              sum += (float)-0.014403992;
            } else {
              sum += (float)-0.050234199;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.014263)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.012616618;
            } else {
              sum += (float)0.020005992;
            }
          } else {
            if (  (data[20].fvalue < 0.11526544)) {
              sum += (float)0.027663579;
            } else {
              sum += (float)-0.03774536;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.71803397)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[17].fvalue < 5.999999)) {
            if (  (data[22].fvalue < 0.37247583)) {
              sum += (float)-0.028253416;
            } else {
              sum += (float)-0.01083848;
            }
          } else {
            if (  (data[11].fvalue < 0.74615842)) {
              sum += (float)-0.51926899;
            } else {
              sum += (float)-0.45164588;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.019041859)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.20541507;
            } else {
              sum += (float)-0.91999036;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.0044373036;
            } else {
              sum += (float)0.037840724;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 6.657023)) {
          if (  (data[19].fvalue < 6.8582368)) {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.028955851;
            } else {
              sum += (float)-0.46829617;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.014624284;
            } else {
              sum += (float)0.034211755;
            }
          }
        } else {
          if (  (data[19].fvalue < 4.9790459)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.043008283;
            } else {
              sum += (float)-0.069510512;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.04984213;
            } else {
              sum += (float)-0.00052119559;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if (  (data[22].fvalue < 0.021422332)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.015364179;
            } else {
              sum += (float)-0.01510383;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.025827358;
            } else {
              sum += (float)0.024582025;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.11052599;
            } else {
              sum += (float)0.22425939;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.065265886;
            } else {
              sum += (float)-0.19308697;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.092762239)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.097588651;
            } else {
              sum += (float)0.018839611;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)0.14494474;
            } else {
              sum += (float)0.25535107;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.33812737)) {
              sum += (float)-0.019938735;
            } else {
              sum += (float)0.0071365051;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.005231258;
            } else {
              sum += (float)0.032320693;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[19].fvalue < 0.019041859)) {
          if (  (data[20].fvalue < 1.2766423)) {
            sum += (float)0.21798788;
          } else {
            if (  (data[11].fvalue < 0.6959849)) {
              sum += (float)0.4599041;
            } else {
              sum += (float)0.33591381;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.46864098)) {
            if (  (data[5].fvalue < 31.444439)) {
              sum += (float)-0.01292506;
            } else {
              sum += (float)0.11119276;
            }
          } else {
            if (  (data[19].fvalue < 1.6133342)) {
              sum += (float)0.24669856;
            } else {
              sum += (float)0.15376379;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 5.999999)) {
          if (  (data[20].fvalue < 0.020000871)) {
            if (  (data[19].fvalue < 0.32061628)) {
              sum += (float)-0.30212337;
            } else {
              sum += (float)-0.42917567;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.082742989;
            } else {
              sum += (float)-0.293167;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 0.068273708)) {
              sum += (float)-0.37569928;
            } else {
              sum += (float)-0.46929273;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.49502072;
            } else {
              sum += (float)-0.57400393;
            }
          }
        }
      }
    }
  }
  if (  (data[10].fvalue < 1.0731571)) {
    if (  (data[13].fvalue < 15.999999)) {
      if (  (data[18].fvalue < 0.10813179)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.050980438)) {
              sum += (float)0.007252668;
            } else {
              sum += (float)0.03838516;
            }
          } else {
            if (  (data[20].fvalue < 0.020556014)) {
              sum += (float)-0.0042158188;
            } else {
              sum += (float)0.1178482;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.0023283537;
            } else {
              sum += (float)-0.013623146;
            }
          } else {
            if (  (data[20].fvalue < 0.28405795)) {
              sum += (float)-0.19672053;
            } else {
              sum += (float)-0.032269392;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 2.239372)) {
            if (  (data[10].fvalue < 0.24638648)) {
              sum += (float)-0.0047907038;
            } else {
              sum += (float)-0.02242806;
            }
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.050644342;
            } else {
              sum += (float)-0.019284196;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.6031805)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.0032131139;
            } else {
              sum += (float)0.01399244;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.015488169;
            } else {
              sum += (float)0.066683158;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.78046066)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 0.020556014)) {
              sum += (float)-0.067606136;
            } else {
              sum += (float)0.10874254;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.12712221;
            } else {
              sum += (float)-0.32375884;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.58876598)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.1905413;
            } else {
              sum += (float)-0.38531962;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.034327634;
            } else {
              sum += (float)-0.11528525;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.69786167)) {
          if (  (data[10].fvalue < 0.71945691)) {
            if (  (data[13].fvalue < 18.999998)) {
              sum += (float)0.20862399;
            } else {
              sum += (float)0.29836178;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.27680057;
            } else {
              sum += (float)0.19771905;
            }
          }
        } else {
          if (  (data[13].fvalue < 20.999998)) {
            if (  (data[18].fvalue < 2.239372)) {
              sum += (float)0.039850142;
            } else {
              sum += (float)0.17047548;
            }
          } else {
            if (  (data[18].fvalue < 1.2979497)) {
              sum += (float)0.20608182;
            } else {
              sum += (float)0.3470946;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.10813179)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.004289852)) {
              sum += (float)0.30964452;
            } else {
              sum += (float)0.0062228106;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.077732429;
            } else {
              sum += (float)-0.31666934;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.004289852)) {
              sum += (float)0.079829492;
            } else {
              sum += (float)0.16374937;
            }
          } else {
            if (  (data[18].fvalue < 0.004289852)) {
              sum += (float)0.083267465;
            } else {
              sum += (float)-0.089755245;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.022839174)) {
              sum += (float)-0.35492441;
            } else {
              sum += (float)-0.095280409;
            }
          } else {
            if (  (data[18].fvalue < 0.004289852)) {
              sum += (float)-0.12342709;
            } else {
              sum += (float)0.037274729;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[20].fvalue < 0.020556014)) {
              sum += (float)-0.2853483;
            } else {
              sum += (float)0.40337721;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.030015292;
            } else {
              sum += (float)0.48243079;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.016909793;
            } else {
              sum += (float)-0.23382598;
            }
          } else {
            if (  (data[18].fvalue < 4.2260351)) {
              sum += (float)-0.32037422;
            } else {
              sum += (float)0.090378627;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.24414927;
            } else {
              sum += (float)0.44217446;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.094873242;
            } else {
              sum += (float)0.47598517;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.020556014)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.11407308;
            } else {
              sum += (float)0.2689639;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.44506752;
            } else {
              sum += (float)0.14131176;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.0927618)) {
            if (  (data[18].fvalue < 0.52056158)) {
              sum += (float)0.60444909;
            } else {
              sum += (float)0.50293922;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.48041761;
            } else {
              sum += (float)0.33441564;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 1.3562676)) {
    if (  (data[8].fvalue < 6.0932956)) {
      if (  (data[21].fvalue < 0.12182908)) {
        if (  (data[21].fvalue < 0.051208165)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.029105304;
            } else {
              sum += (float)0.013472484;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.019144163;
            } else {
              sum += (float)-0.0051696887;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.10059769)) {
              sum += (float)0.065728799;
            } else {
              sum += (float)0.0065172473;
            }
          } else {
            if (  (data[21].fvalue < 0.10059769)) {
              sum += (float)0.14184111;
            } else {
              sum += (float)0.051217396;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.34409469)) {
          if (  (data[8].fvalue < 0.10651015)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.25333118;
            } else {
              sum += (float)-0.093172416;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.00048626596;
            } else {
              sum += (float)-0.049220432;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.1482069)) {
              sum += (float)-0.058766399;
            } else {
              sum += (float)0.02167993;
            }
          } else {
            if (  (data[21].fvalue < 0.61012393)) {
              sum += (float)0.018357199;
            } else {
              sum += (float)-0.016309328;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.78510064)) {
              sum += (float)0.059848979;
            } else {
              sum += (float)0.1627935;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.018561034;
            } else {
              sum += (float)-0.12232001;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.45812359)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.022760177;
            } else {
              sum += (float)-0.011393344;
            }
          } else {
            if (  (data[10].fvalue < 0.62777513)) {
              sum += (float)-0.0063990508;
            } else {
              sum += (float)-0.030307913;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.025130134)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 0.5938946)) {
              sum += (float)0.015851142;
            } else {
              sum += (float)-0.016332736;
            }
          } else {
            if (  (data[20].fvalue < 0.63937896)) {
              sum += (float)0.24762765;
            } else {
              sum += (float)0.03334466;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.10059769)) {
            if (  (data[10].fvalue < 0.6657455)) {
              sum += (float)0.13949616;
            } else {
              sum += (float)0.33690611;
            }
          } else {
            if (  (data[10].fvalue < 0.78510064)) {
              sum += (float)0.046183359;
            } else {
              sum += (float)0.25169954;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 4.999999)) {
      if (  (data[17].fvalue < 5.999999)) {
        if (  (data[10].fvalue < 0.71441287)) {
          if (  (data[11].fvalue < 0.82387865)) {
            if (  (data[11].fvalue < 0.10671575)) {
              sum += (float)0.014553377;
            } else {
              sum += (float)-0.0063556451;
            }
          } else {
            if (  (data[20].fvalue < 7.480145)) {
              sum += (float)0.014085963;
            } else {
              sum += (float)-0.0090925619;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0085003367;
            } else {
              sum += (float)-0.098971598;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.081689835;
            } else {
              sum += (float)-0.0047446527;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.42660677)) {
          if (  (data[21].fvalue < 3.3732803)) {
            if (  (data[8].fvalue < 8.5974064)) {
              sum += (float)-0.48882976;
            } else {
              sum += (float)-0.42858034;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.33130249;
            } else {
              sum += (float)-0.42076135;
            }
          }
        } else {
          sum += (float)-0.24262848;
        }
      }
    } else {
      if (  (data[14].fvalue < 4.999999)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[17].fvalue < 2.999999)) {
              sum += (float)-0.027961643;
            } else {
              sum += (float)-0.19574226;
            }
          } else {
            if (  (data[20].fvalue < 2.7602844)) {
              sum += (float)0.14806817;
            } else {
              sum += (float)-0.014970348;
            }
          }
        } else {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.051083028;
            } else {
              sum += (float)-0.012101034;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.091734134;
            } else {
              sum += (float)-0.037603702;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 7.480145)) {
          if (  (data[10].fvalue < 0.24646977)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.21257259;
            } else {
              sum += (float)-0.053489722;
            }
          } else {
            if (  (data[21].fvalue < 3.7991781)) {
              sum += (float)0.400235;
            } else {
              sum += (float)0.30169505;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 4.3208513)) {
              sum += (float)0.19321066;
            } else {
              sum += (float)0.065938763;
            }
          } else {
            if (  (data[10].fvalue < 0.19256866)) {
              sum += (float)-0.24693403;
            } else {
              sum += (float)0.040501788;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 5.6018453)) {
    if (  (data[9].fvalue < 0.99452138)) {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[10].fvalue < 0.72187054)) {
          if (  (data[9].fvalue < 0.96813226)) {
            if (  (data[10].fvalue < 0.34876424)) {
              sum += (float)0.66112828;
            } else {
              sum += (float)0.22443852;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.01745278;
            } else {
              sum += (float)0.19785362;
            }
          }
        } else {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[22].fvalue < 0.021096086)) {
              sum += (float)0.029316934;
            } else {
              sum += (float)-0.13161522;
            }
          } else {
            if (  (data[10].fvalue < 0.87954432)) {
              sum += (float)0.438737;
            } else {
              sum += (float)0.15478662;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.090565875)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.010845141;
            } else {
              sum += (float)-0.25644687;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.66233349;
            } else {
              sum += (float)-0.50940609;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0045250608;
            } else {
              sum += (float)0.080475867;
            }
          } else {
            if (  (data[10].fvalue < 0.41053516)) {
              sum += (float)-0.36137629;
            } else {
              sum += (float)-0.038548142;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 1.1116415)) {
              sum += (float)-0.077929117;
            } else {
              sum += (float)0.015848409;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0071702781;
            } else {
              sum += (float)-0.059829447;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.012428677;
            } else {
              sum += (float)-0.040451132;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.032852884;
            } else {
              sum += (float)-0.006560009;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[20].fvalue < 0.092095479)) {
              sum += (float)-0.24465233;
            } else {
              sum += (float)-0.00099638884;
            }
          } else {
            if (  (data[21].fvalue < 0.024029072)) {
              sum += (float)-0.36852306;
            } else {
              sum += (float)-0.10912556;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.59543484)) {
            if (  (data[10].fvalue < 0.51324081)) {
              sum += (float)0.090813555;
            } else {
              sum += (float)0.29224908;
            }
          } else {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)0.026633661;
            } else {
              sum += (float)-0.090829775;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 3.0768034)) {
      if (  (data[22].fvalue < 8.4455938)) {
        if (  (data[21].fvalue < 6.846559)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.0031884559;
            } else {
              sum += (float)0.055935215;
            }
          } else {
            if (  (data[9].fvalue < 6.351418)) {
              sum += (float)0.099314034;
            } else {
              sum += (float)0.15327476;
            }
          }
        } else {
          if (  (data[21].fvalue < 9.7010622)) {
            if (  (data[9].fvalue < 6.6567426)) {
              sum += (float)5.6414174e-05;
            } else {
              sum += (float)0.080436908;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.026163872;
            } else {
              sum += (float)-0.0010851835;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[22].fvalue < 14.48498)) {
            if (  (data[9].fvalue < 6.6567426)) {
              sum += (float)-0.012324777;
            } else {
              sum += (float)0.036107466;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.041933332;
            } else {
              sum += (float)-0.0061999061;
            }
          }
        } else {
          if (  (data[12].fvalue < 17.999998)) {
            if (  (data[21].fvalue < 0.024029072)) {
              sum += (float)-0.020704325;
            } else {
              sum += (float)0.019771578;
            }
          } else {
            if (  (data[13].fvalue < 17.999998)) {
              sum += (float)-0.32742578;
            } else {
              sum += (float)0.03046521;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 11.999999)) {
        if (  (data[13].fvalue < 11.999999)) {
          if (  (data[20].fvalue < 7.4581423)) {
            if (  (data[21].fvalue < 2.807312)) {
              sum += (float)0.032435536;
            } else {
              sum += (float)-0.0041786209;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.0028970051;
            } else {
              sum += (float)-0.020990556;
            }
          }
        } else {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)-0.23278934;
            } else {
              sum += (float)0.1399321;
            }
          } else {
            if (  (data[22].fvalue < 0.14854555)) {
              sum += (float)0.085280955;
            } else {
              sum += (float)-0.028542267;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 12.999999)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.14135033;
            } else {
              sum += (float)0.036749035;
            }
          } else {
            if (  (data[22].fvalue < 0.73974782)) {
              sum += (float)0.20279482;
            } else {
              sum += (float)0.11743136;
            }
          }
        } else {
          if (  (data[13].fvalue < 12.999999)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.11993526;
            } else {
              sum += (float)-0.29523414;
            }
          } else {
            if (  (data[12].fvalue < 15.999999)) {
              sum += (float)-0.012016958;
            } else {
              sum += (float)-0.14218222;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 1.2309989)) {
    if (  (data[19].fvalue < -1e-06)) {
      sum += (float)0.51291645;
    } else {
      if (  (data[12].fvalue < 12.999999)) {
        if (  (data[20].fvalue < 0.020385381)) {
          if (  (data[22].fvalue < 0.119918)) {
            if (  (data[22].fvalue < 0.051419266)) {
              sum += (float)0.0015848187;
            } else {
              sum += (float)0.075433016;
            }
          } else {
            if (  (data[22].fvalue < 0.33834574)) {
              sum += (float)-0.061593849;
            } else {
              sum += (float)-0.00463408;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.089933529)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.026003374;
            } else {
              sum += (float)0.11458247;
            }
          } else {
            if (  (data[20].fvalue < 0.32348341)) {
              sum += (float)-0.017027378;
            } else {
              sum += (float)0.006718134;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.69879019)) {
          if (  (data[19].fvalue < 0.80905902)) {
            if (  (data[8].fvalue < 0.79098541)) {
              sum += (float)-0.13564472;
            } else {
              sum += (float)-0.57006258;
            }
          } else {
            if (  (data[8].fvalue < 0.85026801)) {
              sum += (float)0.33001587;
            } else {
              sum += (float)-0.04972608;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.3947725)) {
            if (  (data[8].fvalue < 0.99696308)) {
              sum += (float)0.11075336;
            } else {
              sum += (float)-0.098903812;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.031833243;
            } else {
              sum += (float)0.11923932;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.65848863)) {
      if (  (data[12].fvalue < 6.999999)) {
        if (  (data[18].fvalue < 0.22532003)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.023105759)) {
              sum += (float)0.0010074109;
            } else {
              sum += (float)-0.088629231;
            }
          } else {
            if (  (data[18].fvalue < 0.1089355)) {
              sum += (float)0.45247418;
            } else {
              sum += (float)0.15223797;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.29561618)) {
              sum += (float)-0.074303322;
            } else {
              sum += (float)0.01984215;
            }
          } else {
            if (  (data[18].fvalue < 0.39289561)) {
              sum += (float)-0.038431849;
            } else {
              sum += (float)-0.11890884;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.35265219)) {
          if (  (data[18].fvalue < 0.27493706)) {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)0.52661753;
            } else {
              sum += (float)-0.46523899;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)0.30227321;
            } else {
              sum += (float)0.50590551;
            }
          }
        } else {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[18].fvalue < 0.52287763)) {
              sum += (float)0.1521747;
            } else {
              sum += (float)-0.015910204;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)0.36192566;
            } else {
              sum += (float)0.47960025;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[18].fvalue < 5.1084461)) {
          if (  (data[18].fvalue < 0.93748212)) {
            if (  (data[19].fvalue < 2.215575)) {
              sum += (float)0.10807184;
            } else {
              sum += (float)0.22470871;
            }
          } else {
            if (  (data[19].fvalue < 1.7597802)) {
              sum += (float)0.27439588;
            } else {
              sum += (float)0.40617743;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.78457534)) {
            if (  (data[18].fvalue < 7.6209679)) {
              sum += (float)0.21661592;
            } else {
              sum += (float)0.12322801;
            }
          } else {
            sum += (float)0.59447998;
          }
        }
      } else {
        if (  (data[18].fvalue < 1.2939249)) {
          if (  (data[12].fvalue < 10.999999)) {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.169384;
            } else {
              sum += (float)-0.032924965;
            }
          } else {
            if (  (data[18].fvalue < 0.93748212)) {
              sum += (float)0.29648444;
            } else {
              sum += (float)0.042449005;
            }
          }
        } else {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[19].fvalue < 2.784709)) {
              sum += (float)-0.089080982;
            } else {
              sum += (float)0.093676232;
            }
          } else {
            if (  (data[19].fvalue < 1.6095985)) {
              sum += (float)-0.16694471;
            } else {
              sum += (float)-0.29798296;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.023034617)) {
    if (  (data[18].fvalue < 0.015125559)) {
      if (  (data[8].fvalue < 0.26433063)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 1.9277366)) {
            if (  (data[22].fvalue < 0.052603312)) {
              sum += (float)-0.092255019;
            } else {
              sum += (float)-0.41023996;
            }
          } else {
            if (  (data[9].fvalue < 2.7140074)) {
              sum += (float)0.046789128;
            } else {
              sum += (float)-0.037337989;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.8599451)) {
            if (  (data[11].fvalue < 0.74812597)) {
              sum += (float)-0.023040131;
            } else {
              sum += (float)-0.19925129;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.071407318;
            } else {
              sum += (float)-0.009807067;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.7852913)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.6046897)) {
              sum += (float)0.57112229;
            } else {
              sum += (float)0.22148721;
            }
          } else {
            if (  (data[18].fvalue < 0.0036525587)) {
              sum += (float)-0.0069797747;
            } else {
              sum += (float)0.10549633;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.047419731;
            } else {
              sum += (float)0.0095848991;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.0057569789;
            } else {
              sum += (float)0.062771216;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 6.999999)) {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.31075317)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.10332718;
            } else {
              sum += (float)-0.045015275;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.16178451;
            } else {
              sum += (float)0.23152812;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 0.049774017)) {
              sum += (float)-0.25747252;
            } else {
              sum += (float)0.027295657;
            }
          } else {
            if (  (data[8].fvalue < 1.951239)) {
              sum += (float)0.073960029;
            } else {
              sum += (float)0.17585441;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.35542652)) {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[9].fvalue < 1.3199819)) {
              sum += (float)-0.21993022;
            } else {
              sum += (float)-0.091427185;
            }
          } else {
            if (  (data[9].fvalue < 1.1163487)) {
              sum += (float)-0.17010814;
            } else {
              sum += (float)-0.42829406;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.022376493;
            } else {
              sum += (float)-0.27948678;
            }
          } else {
            if (  (data[20].fvalue < 6.6605158)) {
              sum += (float)0.28697345;
            } else {
              sum += (float)0.10392576;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 0.14962114)) {
      if (  (data[9].fvalue < 1.2220738)) {
        if (  (data[18].fvalue < 0.051087309)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 0.99596602)) {
              sum += (float)0.11345472;
            } else {
              sum += (float)-0.0208575;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.16938058;
            } else {
              sum += (float)-0.034148052;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99596602)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.34591576;
            } else {
              sum += (float)2.3848877;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.11312902;
            } else {
              sum += (float)0.51950055;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.433418)) {
          if (  (data[18].fvalue < 0.37436688)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.038049027;
            } else {
              sum += (float)0.053936969;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.15202782;
            } else {
              sum += (float)0.47456649;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.19417688)) {
              sum += (float)0.038199816;
            } else {
              sum += (float)-0.031501688;
            }
          } else {
            if (  (data[18].fvalue < 0.33333904)) {
              sum += (float)-0.098253019;
            } else {
              sum += (float)0.014182334;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 7.3698812)) {
        if (  (data[9].fvalue < 9.6984138)) {
          if (  (data[18].fvalue < 0.22538392)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.00047765949;
            } else {
              sum += (float)-0.060703751;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.008626218;
            } else {
              sum += (float)0.0089045977;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.3346117)) {
            sum += (float)-0.74241275;
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.52394515;
            } else {
              sum += (float)-0.27841595;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 0.051087309)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.2701939;
            } else {
              sum += (float)-0.079645999;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.02695499;
            } else {
              sum += (float)-0.0056755538;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.085463569)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)0.10975093;
            } else {
              sum += (float)0.038628545;
            }
          } else {
            if ( (data[9].fvalue < 2.1479456)) {
              sum += (float)0.12026099;
            } else {
              sum += (float)0.01040183;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 3.999999)) {
    if (  (data[21].fvalue < 1.0306326)) {
      if (  (data[15].fvalue < 1.999999)) {
        if (  (data[21].fvalue < 0.12379868)) {
          if (  (data[21].fvalue < 0.024021026)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.00034694927;
            } else {
              sum += (float)0.075751089;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.021229163;
            } else {
              sum += (float)0.061376747;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.24532081)) {
            if (  (data[21].fvalue < 0.1494741)) {
              sum += (float)-0.0045186854;
            } else {
              sum += (float)-0.076966047;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.0083388984;
            } else {
              sum += (float)-0.027735932;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.24532081)) {
              sum += (float)0.10759106;
            } else {
              sum += (float)0.028612416;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.014269285;
            } else {
              sum += (float)-0.1040665;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.24532081)) {
            if (  (data[11].fvalue < 0.18987395)) {
              sum += (float)0.18965355;
            } else {
              sum += (float)0.32151195;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.03530407;
            } else {
              sum += (float)0.11797916;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 4.999999)) {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.011440572;
            } else {
              sum += (float)0.005667184;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.017266449;
            } else {
              sum += (float)-0.0038088276;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.27699387;
            } else {
              sum += (float)0.085275196;
            }
          } else {
            if (  (data[21].fvalue < 2.8212652)) {
              sum += (float)0.037450057;
            } else {
              sum += (float)0.0022638415;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 3.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.014103337;
            } else {
              sum += (float)-0.032121465;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.041025028;
            } else {
              sum += (float)0.010402462;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.48140767;
            } else {
              sum += (float)0.22630091;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.1671091;
            } else {
              sum += (float)0.015564945;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 1.999999)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.45729801;
            } else {
              sum += (float)0.34245107;
            }
          } else {
            sum += (float)0.67729574;
          }
        } else {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[11].fvalue < 0.61147839)) {
              sum += (float)0.13228913;
            } else {
              sum += (float)-0.017984586;
            }
          } else {
            if (  (data[11].fvalue < 0.63307011)) {
              sum += (float)0.47068807;
            } else {
              sum += (float)0.22124247;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.42974216;
            } else {
              sum += (float)0.21844555;
            }
          } else {
            sum += (float)0.56658196;
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[7].fvalue < 0.98772001)) {
              sum += (float)0.70023048;
            } else {
              sum += (float)0.60237175;
            }
          } else {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.4837577;
            } else {
              sum += (float)0.62378258;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.1340303;
            } else {
              sum += (float)0.48292953;
            }
          } else {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.018008903;
            } else {
              sum += (float)0.1446245;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.65584117)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.029486408;
            } else {
              sum += (float)-0.052209422;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.045355484;
            } else {
              sum += (float)-0.15934779;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.075429611;
            } else {
              sum += (float)0.20921692;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.29077676;
            } else {
              sum += (float)0.48495719;
            }
          }
        } else {
          if (  (data[13].fvalue < 11.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.022932168;
            } else {
              sum += (float)-0.04409726;
            }
          } else {
            if (  (data[21].fvalue < 2.2271156)) {
              sum += (float)0.35669881;
            } else {
              sum += (float)0.10313997;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 6.999999)) {
    if (  (data[19].fvalue < 1.0039189)) {
      if (  (data[12].fvalue < 16.999998)) {
        if (  (data[19].fvalue < -1e-06)) {
          sum += (float)0.50722712;
        } else {
          if (  (data[18].fvalue < 0.55784714)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)0.0024374237;
            } else {
              sum += (float)-0.13988806;
            }
          } else {
            if (  (data[19].fvalue < 0.51847851)) {
              sum += (float)0.012955697;
            } else {
              sum += (float)-0.035520609;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 1.0048467)) {
          if (  (data[8].fvalue < 1.1550796)) {
            if (  (data[9].fvalue < 1.543696)) {
              sum += (float)0.1133639;
            } else {
              sum += (float)-0.41964766;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.7648831;
            } else {
              sum += (float)-0.47257778;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.049804393;
            } else {
              sum += (float)-0.3570579;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.10785822;
            } else {
              sum += (float)0.10035226;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.55784714)) {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[18].fvalue < 0.14816393)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.010428437;
            } else {
              sum += (float)0.22698241;
            }
          } else {
            if (  (data[10].fvalue < 0.39735448)) {
              sum += (float)-0.063379228;
            } else {
              sum += (float)-0.0087671457;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.24295372)) {
            if (  (data[18].fvalue < 0.17953277)) {
              sum += (float)0.45235211;
            } else {
              sum += (float)0.2802974;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)0.031082653;
            } else {
              sum += (float)0.31417421;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if ( (data[9].fvalue < 4.257534)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.09003336;
            } else {
              sum += (float)-0.019609395;
            }
          } else {
            if (  (data[18].fvalue < 5.093472)) {
              sum += (float)0.15721542;
            } else {
              sum += (float)0.090300523;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if ( (data[9].fvalue < 2.0771267)) {
              sum += (float)-0.013260032;
            } else {
              sum += (float)-0.13456947;
            }
          } else {
            if (  (data[18].fvalue < 1.6041356)) {
              sum += (float)0.038251221;
            } else {
              sum += (float)-0.079373151;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 7.999999)) {
      sum += (float)-0.44151524;
    } else {
      sum += (float)-0.5281055;
    }
  }
  if (  (data[13].fvalue < 13.999999)) {
    if (  (data[12].fvalue < 16.999998)) {
      if (  (data[17].fvalue < 6.999999)) {
        if (  (data[12].fvalue < 12.999999)) {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.0001490474;
            } else {
              sum += (float)-0.050348733;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.14144231;
            } else {
              sum += (float)0.030667495;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.0720613)) {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)0.2305875;
            } else {
              sum += (float)0.016795116;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.15830567;
            } else {
              sum += (float)0.038626615;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 1.7473593)) {
          sum += (float)-0.50714982;
        } else {
          sum += (float)-0.44805813;
        }
      }
    } else {
      if (  (data[18].fvalue < 1.6199253)) {
        if (  (data[18].fvalue < 1.0060171)) {
          if (  (data[13].fvalue < 10.999999)) {
            if (  (data[8].fvalue < 2.1997163)) {
              sum += (float)0.3805196;
            } else {
              sum += (float)0.69392657;
            }
          } else {
            if (  (data[8].fvalue < 3.1425335)) {
              sum += (float)0.0051491414;
            } else {
              sum += (float)0.24409154;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.024840798)) {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.41508713;
            } else {
              sum += (float)-0.26078111;
            }
          } else {
            if (  (data[8].fvalue < 4.1903334)) {
              sum += (float)-0.046258509;
            } else {
              sum += (float)0.11573365;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 18.999998)) {
          if (  (data[21].fvalue < 0.024840798)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.44472498;
            } else {
              sum += (float)-0.54201394;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.21561319;
            } else {
              sum += (float)-0.35723194;
            }
          }
        } else {
          if (  (data[18].fvalue < 2.058249)) {
            if (  (data[8].fvalue < 3.7602396)) {
              sum += (float)-0.41331884;
            } else {
              sum += (float)-0.17769285;
            }
          } else {
            if (  (data[12].fvalue < 19.999998)) {
              sum += (float)-0.47735333;
            } else {
              sum += (float)-0.56476194;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.62665969)) {
      if (  (data[21].fvalue < 0.15017049)) {
        if (  (data[8].fvalue < 0.50060183)) {
          if (  (data[18].fvalue < 0.1223021)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.063947603;
            } else {
              sum += (float)-0.43437019;
            }
          } else {
            if (  (data[18].fvalue < 0.35544997)) {
              sum += (float)0.066514559;
            } else {
              sum += (float)0.24292241;
            }
          }
        } else {
          if (  (data[12].fvalue < 10.999999)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.19129069;
            } else {
              sum += (float)0.020216621;
            }
          } else {
            if (  (data[18].fvalue < 0.3152335)) {
              sum += (float)-0.37769601;
            } else {
              sum += (float)-0.080152281;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 11.999999)) {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.27101871;
            } else {
              sum += (float)-0.13590465;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.23599482;
            } else {
              sum += (float)0.12833901;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.2608794)) {
            if (  (data[18].fvalue < 0.17921938)) {
              sum += (float)-0.70622301;
            } else {
              sum += (float)-0.52791387;
            }
          } else {
            if (  (data[12].fvalue < 15.999999)) {
              sum += (float)-0.20606178;
            } else {
              sum += (float)-0.39168257;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 8.999999)) {
        if (  (data[13].fvalue < 17.999998)) {
          if (  (data[13].fvalue < 15.999999)) {
            if (  (data[21].fvalue < 0.066489801)) {
              sum += (float)0.056247264;
            } else {
              sum += (float)-0.13363028;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.37213346;
            } else {
              sum += (float)-0.05807079;
            }
          }
        } else {
          if (  (data[13].fvalue < 19.999998)) {
            if (  (data[21].fvalue < 0.084061265)) {
              sum += (float)-0.42202985;
            } else {
              sum += (float)-0.86044943;
            }
          } else {
            sum += (float)-1.3972744;
          }
        }
      } else {
        if (  (data[18].fvalue < 1.3029956)) {
          if (  (data[8].fvalue < 0.78821784)) {
            if (  (data[21].fvalue < 0.52610356)) {
              sum += (float)0.3347927;
            } else {
              sum += (float)0.11932384;
            }
          } else {
            if (  (data[21].fvalue < 0.57191449)) {
              sum += (float)0.12355995;
            } else {
              sum += (float)0.026931221;
            }
          }
        } else {
          if (  (data[13].fvalue < 18.999998)) {
            if (  (data[12].fvalue < 15.999999)) {
              sum += (float)0.23262571;
            } else {
              sum += (float)0.13247946;
            }
          } else {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)-0.055475887;
            } else {
              sum += (float)0.33737746;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.023316283)) {
    if (  (data[19].fvalue < 0.039450195)) {
      if (  (data[8].fvalue < 0.31188628)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.15109195)) {
            if (  (data[19].fvalue < 0.0081140026)) {
              sum += (float)-0.085125223;
            } else {
              sum += (float)-0.0063833049;
            }
          } else {
            if (  (data[8].fvalue < 0.06535767)) {
              sum += (float)0.11121935;
            } else {
              sum += (float)-0.032798443;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.44331786)) {
            if (  (data[8].fvalue < 0.06535767)) {
              sum += (float)-0.15232849;
            } else {
              sum += (float)0.01747982;
            }
          } else {
            if (  (data[8].fvalue < 0.06535767)) {
              sum += (float)-0.19692586;
            } else {
              sum += (float)-0.49635014;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.0081140026)) {
          if (  (data[10].fvalue < 0.51233321)) {
            if (  (data[8].fvalue < 0.99605578)) {
              sum += (float)0.040211979;
            } else {
              sum += (float)-0.015910512;
            }
          } else {
            if (  (data[8].fvalue < 3.5258744)) {
              sum += (float)0.36687288;
            } else {
              sum += (float)0.030435177;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.11396302)) {
            if (  (data[10].fvalue < 0.45799544)) {
              sum += (float)-0.2565707;
            } else {
              sum += (float)-0.4230051;
            }
          } else {
            if (  (data[18].fvalue < 0.014903174)) {
              sum += (float)-0.17074816;
            } else {
              sum += (float)-0.024233129;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.0031860927)) {
        if (  (data[8].fvalue < 0.06535767)) {
          if (  (data[10].fvalue < 0.097309753)) {
            if (  (data[19].fvalue < 0.084599152)) {
              sum += (float)0.095446423;
            } else {
              sum += (float)-0.028073411;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.082400031;
            } else {
              sum += (float)0.34208137;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.45799544)) {
              sum += (float)0.012631083;
            } else {
              sum += (float)-0.0052897804;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.039061759;
            } else {
              sum += (float)-0.037066687;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.061381128)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.10718937;
            } else {
              sum += (float)-0.063345827;
            }
          } else {
            if (  (data[21].fvalue < 0.21354659)) {
              sum += (float)0.12981391;
            } else {
              sum += (float)0.20188099;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.020872783)) {
            if (  (data[19].fvalue < 1.2886636)) {
              sum += (float)-0.21692753;
            } else {
              sum += (float)-0.32754484;
            }
          } else {
            if (  (data[19].fvalue < 0.46537173)) {
              sum += (float)0.080664553;
            } else {
              sum += (float)0.012429283;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 0.06535767)) {
      if (  (data[10].fvalue < 0.054210577)) {
        if (  (data[19].fvalue < 0.26480758)) {
          if (  (data[21].fvalue < 0.024994966)) {
            if (  (data[20].fvalue < 0.050054532)) {
              sum += (float)-0.058190957;
            } else {
              sum += (float)0.036465354;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.02670644;
            } else {
              sum += (float)-0.27926278;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.030186988)) {
            if (  (data[8].fvalue < 0.028187431)) {
              sum += (float)0.075014584;
            } else {
              sum += (float)-0.018333031;
            }
          } else {
            if (  (data[8].fvalue < 0.028187431)) {
              sum += (float)0.41818771;
            } else {
              sum += (float)0.11492307;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.050255146)) {
          if (  (data[18].fvalue < 0.042276252)) {
            if (  (data[10].fvalue < 0.45799544)) {
              sum += (float)-0.034430441;
            } else {
              sum += (float)0.19298226;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.0035456691;
            } else {
              sum += (float)0.15356706;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.11156087)) {
            if (  (data[8].fvalue < 0.028187431)) {
              sum += (float)0.55197924;
            } else {
              sum += (float)0.083494164;
            }
          } else {
            if (  (data[19].fvalue < 0.07295765)) {
              sum += (float)0.38754129;
            } else {
              sum += (float)0.57641453;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.028259896)) {
        if (  (data[18].fvalue < 0.062707469)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.72109145)) {
              sum += (float)-0.096430875;
            } else {
              sum += (float)0.025991149;
            }
          } else {
            if (  (data[10].fvalue < 0.28839105)) {
              sum += (float)-0.083870739;
            } else {
              sum += (float)-0.23925152;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.0081140026)) {
            if (  (data[10].fvalue < 0.32808667)) {
              sum += (float)0.012123263;
            } else {
              sum += (float)-0.0095379818;
            }
          } else {
            if (  (data[18].fvalue < 0.49693468)) {
              sum += (float)0.12990481;
            } else {
              sum += (float)0.040827267;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.32808667)) {
          if (  (data[8].fvalue < 3.327203)) {
            if (  (data[19].fvalue < 0.11121144)) {
              sum += (float)-0.031940967;
            } else {
              sum += (float)-0.012337173;
            }
          } else {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.0023797208;
            } else {
              sum += (float)-0.47241956;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.35564902)) {
            if (  (data[19].fvalue < 0.13903418)) {
              sum += (float)0.022624953;
            } else {
              sum += (float)0.24275103;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.0040389658;
            } else {
              sum += (float)0.066077411;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 13.999999)) {
    if (  (data[13].fvalue < 2.999999)) {
      if (  (data[18].fvalue < 0.52515125)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.0039785369)) {
            if (  (data[10].fvalue < 0.71503371)) {
              sum += (float)-0.0052882833;
            } else {
              sum += (float)0.05284417;
            }
          } else {
            if (  (data[10].fvalue < 0.4556714)) {
              sum += (float)0.00095200096;
            } else {
              sum += (float)-0.047588248;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.051632095)) {
            if (  (data[18].fvalue < 0.023147389)) {
              sum += (float)0.0035784014;
            } else {
              sum += (float)-0.025782403;
            }
          } else {
            if (  (data[11].fvalue < 0.011375202)) {
              sum += (float)0.075165853;
            } else {
              sum += (float)0.01027698;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 1.157299)) {
            if (  (data[10].fvalue < 0.097562291)) {
              sum += (float)0.00059031032;
            } else {
              sum += (float)-0.039151587;
            }
          } else {
            if (  (data[18].fvalue < 2.2642417)) {
              sum += (float)0.0085586309;
            } else {
              sum += (float)-0.013233209;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.34043401)) {
              sum += (float)-0.050190222;
            } else {
              sum += (float)-0.02776671;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.0022066613;
            } else {
              sum += (float)-0.052597564;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.29853976)) {
        if (  (data[8].fvalue < 1.7248255)) {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[5].fvalue < 5.5381336)) {
              sum += (float)-0.039815202;
            } else {
              sum += (float)-0.015872298;
            }
          } else {
            if (  (data[11].fvalue < 0.023031486)) {
              sum += (float)0.0773587;
            } else {
              sum += (float)-0.13241434;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.44228765)) {
            if (  (data[18].fvalue < 0.063262843)) {
              sum += (float)0.023778249;
            } else {
              sum += (float)-0.0071248286;
            }
          } else {
            if (  (data[18].fvalue < 0.086297318)) {
              sum += (float)0.11970612;
            } else {
              sum += (float)0.014315733;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.032146171)) {
          if (  (data[5].fvalue < 28.330605)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.16685773;
            } else {
              sum += (float)0.35230538;
            }
          } else {
            if (  (data[11].fvalue < 0.017751051)) {
              sum += (float)0.22248475;
            } else {
              sum += (float)0.043174412;
            }
          }
        } else {
          if (  (data[5].fvalue < 3.2926269)) {
            if (  (data[11].fvalue < 0.28779536)) {
              sum += (float)0.25854138;
            } else {
              sum += (float)0.077902146;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.0032941939;
            } else {
              sum += (float)0.022431433;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.94356263)) {
      if (  (data[8].fvalue < 0.99380189)) {
        if (  (data[18].fvalue < 0.26133087)) {
          if (  (data[18].fvalue < 0.063262843)) {
            if (  (data[10].fvalue < 0.44228765)) {
              sum += (float)-0.39145875;
            } else {
              sum += (float)-0.091372624;
            }
          } else {
            if (  (data[11].fvalue < 0.023031486)) {
              sum += (float)0.13585372;
            } else {
              sum += (float)-0.094244279;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.11162615)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.092997447;
            } else {
              sum += (float)-0.041445915;
            }
          } else {
            if (  (data[10].fvalue < 0.47278637)) {
              sum += (float)0.13816707;
            } else {
              sum += (float)0.45095703;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.55773878)) {
          if (  (data[18].fvalue < 0.42046195)) {
            if (  (data[10].fvalue < 0.87793648)) {
              sum += (float)-0.06559521;
            } else {
              sum += (float)0.18609716;
            }
          } else {
            if (  (data[13].fvalue < 16.999998)) {
              sum += (float)-0.13319926;
            } else {
              sum += (float)-0.2609207;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.59042513)) {
            if (  (data[13].fvalue < 16.999998)) {
              sum += (float)0.014487725;
            } else {
              sum += (float)-0.038104367;
            }
          } else {
            if (  (data[8].fvalue < 1.8320175)) {
              sum += (float)0.28961265;
            } else {
              sum += (float)-0.33709323;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 17.199821)) {
        if (  (data[8].fvalue < 2.1893091)) {
          if (  (data[5].fvalue < 7.9207025)) {
            if (  (data[11].fvalue < 0.38541204)) {
              sum += (float)0.47435692;
            } else {
              sum += (float)0.30721569;
            }
          } else {
            if (  (data[11].fvalue < 0.27473515)) {
              sum += (float)0.33483472;
            } else {
              sum += (float)0.14510401;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.2979816)) {
            if (  (data[10].fvalue < 0.59042513)) {
              sum += (float)0.059441056;
            } else {
              sum += (float)-0.1923189;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.13022307;
            } else {
              sum += (float)0.25950503;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.34043401)) {
          if (  (data[10].fvalue < 0.78245705)) {
            if (  (data[18].fvalue < 2.4426658)) {
              sum += (float)0.12085811;
            } else {
              sum += (float)0.23771213;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.34701049;
            } else {
              sum += (float)0.082852438;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.19986093)) {
            if (  (data[13].fvalue < 20.999998)) {
              sum += (float)0.020844975;
            } else {
              sum += (float)0.19879396;
            }
          } else {
            if (  (data[18].fvalue < 2.7747607)) {
              sum += (float)0.071273305;
            } else {
              sum += (float)0.15634011;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 1.999999)) {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[16].fvalue < 2.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 2.4747071)) {
            if (  (data[19].fvalue < 0.21550593)) {
              sum += (float)0.035420801;
            } else {
              sum += (float)0.0023601954;
            }
          } else {
            if (  (data[19].fvalue < 0.23192519)) {
              sum += (float)-0.032934748;
            } else {
              sum += (float)0.0060876058;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.012351623;
            } else {
              sum += (float)0.012402775;
            }
          } else {
            if (  (data[19].fvalue < 0.0084527954)) {
              sum += (float)-0.22678851;
            } else {
              sum += (float)-0.01989801;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.6305387)) {
          if (  (data[11].fvalue < 0.26208091)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.17968239;
            } else {
              sum += (float)0.099500246;
            }
          } else {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)0.057825282;
            } else {
              sum += (float)0.13641867;
            }
          }
        } else {
          if (  (data[16].fvalue < 4.999999)) {
            if (  (data[8].fvalue < 9.3025808)) {
              sum += (float)0.026837241;
            } else {
              sum += (float)-0.00025309643;
            }
          } else {
            if (  (data[8].fvalue < 10.884009)) {
              sum += (float)0.097976685;
            } else {
              sum += (float)0.029942367;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.99999899)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.0084527954)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.53765273;
            } else {
              sum += (float)0.92713386;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.03370684;
            } else {
              sum += (float)0.11670774;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.049683239)) {
            if (  (data[19].fvalue < 0.23192519)) {
              sum += (float)0.055181198;
            } else {
              sum += (float)-0.20239152;
            }
          } else {
            if (  (data[19].fvalue < 0.12641957)) {
              sum += (float)0.1255206;
            } else {
              sum += (float)0.0044443039;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.14064494)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.060987141)) {
              sum += (float)-0.22423314;
            } else {
              sum += (float)-0.068759769;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.051838037;
            } else {
              sum += (float)0.27212271;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.010892627;
            } else {
              sum += (float)-0.074191466;
            }
          } else {
            if (  (data[20].fvalue < 0.021460529)) {
              sum += (float)-0.28634363;
            } else {
              sum += (float)-0.077873714;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.99999899)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.13622946;
            } else {
              sum += (float)-0.072753452;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.47003993;
            } else {
              sum += (float)-0.17814417;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.021460529)) {
            if (  (data[11].fvalue < 0.49741739)) {
              sum += (float)0.15625235;
            } else {
              sum += (float)0.064841688;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.030073177;
            } else {
              sum += (float)0.042976931;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.021460529)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.050486382;
            } else {
              sum += (float)-0.0063235979;
            }
          } else {
            if (  (data[8].fvalue < 0.85814309)) {
              sum += (float)0.19806588;
            } else {
              sum += (float)0.017928945;
            }
          }
        } else {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[20].fvalue < 0.049683239)) {
              sum += (float)-0.16861619;
            } else {
              sum += (float)-0.028176518;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.29981452;
            } else {
              sum += (float)-0.093563899;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.0084527954)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 0.40505326)) {
            if (  (data[8].fvalue < 0.066163741)) {
              sum += (float)0.09544798;
            } else {
              sum += (float)0.22571503;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.32723686;
            } else {
              sum += (float)0.50323248;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.13773154;
            } else {
              sum += (float)-0.76150024;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.89676136;
            } else {
              sum += (float)-0.63666517;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.0395157)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 0.40505326)) {
              sum += (float)0.018498449;
            } else {
              sum += (float)0.14203632;
            }
          } else {
            if (  (data[19].fvalue < 0.028799616)) {
              sum += (float)-0.55352533;
            } else {
              sum += (float)-0.33699834;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.021400796;
            } else {
              sum += (float)-0.092402644;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.037975501;
            } else {
              sum += (float)0.0022996301;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 0.64663547)) {
    if (  (data[13].fvalue < 5.999999)) {
      if (  (data[11].fvalue < 0.30066735)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.22396807)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.39574927;
            } else {
              sum += (float)0.17652595;
            }
          } else {
            if (  (data[7].fvalue < 0.99720323)) {
              sum += (float)0.091747783;
            } else {
              sum += (float)-0.73740983;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.050380059;
            } else {
              sum += (float)0.14455429;
            }
          } else {
            if (  (data[11].fvalue < 0.13706805)) {
              sum += (float)-0.45854929;
            } else {
              sum += (float)-0.14295331;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.87749702)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.32816133)) {
              sum += (float)-0.00069878157;
            } else {
              sum += (float)-0.098060094;
            }
          } else {
            if (  (data[10].fvalue < 0.03875332)) {
              sum += (float)-0.046849981;
            } else {
              sum += (float)0.032231744;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.029993871)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.27949128;
            } else {
              sum += (float)-0.43288136;
            }
          } else {
            if (  (data[10].fvalue < 0.069666512)) {
              sum += (float)0.13734393;
            } else {
              sum += (float)-0.0065721576;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 9.999999)) {
          if (  (data[10].fvalue < 0.076144449)) {
            if (  (data[10].fvalue < 0.019011524)) {
              sum += (float)0.16594684;
            } else {
              sum += (float)0.37634179;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.53262126;
            } else {
              sum += (float)0.77549595;
            }
          }
        } else {
          sum += (float)1.2092235;
        }
      } else {
        if (  (data[13].fvalue < 8.999999)) {
          if (  (data[11].fvalue < 0.17834343)) {
            sum += (float)-0.25622752;
          } else {
            if (  (data[10].fvalue < 0.66705644)) {
              sum += (float)0.11378449;
            } else {
              sum += (float)0.39169887;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.51191455)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.2525934;
            } else {
              sum += (float)0.44353753;
            }
          } else {
            sum += (float)0.82907331;
          }
        }
      }
    }
  } else {
    if (  (data[15].fvalue < 1.999999)) {
      if (  (data[22].fvalue < 3.795037)) {
        if (  (data[22].fvalue < 0.022195136)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.32672828)) {
              sum += (float)-0.0057714893;
            } else {
              sum += (float)0.0060823015;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.16693178;
            } else {
              sum += (float)-0.043682043;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.093158506)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)0.047016315;
            } else {
              sum += (float)0.13150881;
            }
          } else {
            if (  (data[22].fvalue < 0.33943841)) {
              sum += (float)-0.027265022;
            } else {
              sum += (float)0.0053995862;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.05830938;
            } else {
              sum += (float)-0.031233368;
            }
          } else {
            if (  (data[10].fvalue < 0.87691569)) {
              sum += (float)-0.019161945;
            } else {
              sum += (float)-0.24950707;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.019568538;
            } else {
              sum += (float)-0.015660787;
            }
          } else {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)-0.0059602452;
            } else {
              sum += (float)-0.038365796;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.23832242)) {
        if (  (data[17].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 2.0998225)) {
              sum += (float)0.013182218;
            } else {
              sum += (float)-0.0047739372;
            }
          } else {
            if (  (data[22].fvalue < 1.7529539)) {
              sum += (float)-0.017418733;
            } else {
              sum += (float)0.0034929696;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 4.281548)) {
              sum += (float)0.06863708;
            } else {
              sum += (float)0.1723537;
            }
          } else {
            if (  (data[5].fvalue < 25.112162)) {
              sum += (float)-0.32584625;
            } else {
              sum += (float)-0.12248123;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[10].fvalue < 0.78473985)) {
            if (  (data[10].fvalue < 0.47346985)) {
              sum += (float)0.012431202;
            } else {
              sum += (float)-0.006817142;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.13415276;
            } else {
              sum += (float)0.0039042626;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.66705644)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.028622717;
            } else {
              sum += (float)0.2456079;
            }
          } else {
            if (  (data[10].fvalue < 0.87691569)) {
              sum += (float)0.131423;
            } else {
              sum += (float)0.34936455;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 0.99611908)) {
    if (  (data[9].fvalue < 0.92371672)) {
      if (  (data[10].fvalue < 1.0618154)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.55303156)) {
            if (  (data[21].fvalue < 0.025192421)) {
              sum += (float)0.087462097;
            } else {
              sum += (float)-0.00049457449;
            }
          } else {
            if (  (data[11].fvalue < 0.69614559)) {
              sum += (float)0.22693883;
            } else {
              sum += (float)0.45493165;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.0090708435;
            } else {
              sum += (float)-0.18056607;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.035745595;
            } else {
              sum += (float)0.25031751;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.11636178)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.017509125;
            } else {
              sum += (float)-0.17234853;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.043899767;
            } else {
              sum += (float)-0.029643387;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.33027825;
            } else {
              sum += (float)-0.20594425;
            }
          } else {
            if (  (data[20].fvalue < 0.50142002)) {
              sum += (float)0.33906093;
            } else {
              sum += (float)0.16380113;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.11636178)) {
        if (  (data[11].fvalue < 0.35580903)) {
          if (  (data[10].fvalue < 0.45790198)) {
            if (  (data[9].fvalue < 0.96905375)) {
              sum += (float)0.24257937;
            } else {
              sum += (float)0.041662414;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.02778005;
            } else {
              sum += (float)0.174918;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.96905375)) {
            if (  (data[10].fvalue < 0.28762618)) {
              sum += (float)0.70699215;
            } else {
              sum += (float)0.018536329;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.020421373;
            } else {
              sum += (float)0.043755405;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[20].fvalue < 0.32371026)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.34589314;
            } else {
              sum += (float)-0.2112231;
            }
          } else {
            if (  (data[11].fvalue < 0.054018155)) {
              sum += (float)-0.26599097;
            } else {
              sum += (float)-0.17720644;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.29076019;
            } else {
              sum += (float)0.12894052;
            }
          } else {
            sum += (float)0.41853413;
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 4.0349612)) {
      if (  (data[14].fvalue < 1.999999)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.1216774)) {
              sum += (float)-0.035927359;
            } else {
              sum += (float)0.0020957391;
            }
          } else {
            if (  (data[21].fvalue < 0.025192421)) {
              sum += (float)-0.016273765;
            } else {
              sum += (float)-0.0021196255;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.8627514)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.0761384;
            } else {
              sum += (float)0.26210195;
            }
          } else {
            if (  (data[9].fvalue < 2.4771614)) {
              sum += (float)-0.14213;
            } else {
              sum += (float)0.0040148655;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 0.049517568)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.28352377;
            } else {
              sum += (float)0.023156775;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.0084981928;
            } else {
              sum += (float)0.033070307;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 2.7159123)) {
              sum += (float)-0.6072892;
            } else {
              sum += (float)-0.14481108;
            }
          } else {
            if (  (data[9].fvalue < 2.2348089)) {
              sum += (float)0.53575605;
            } else {
              sum += (float)0.067817904;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 1.2849674)) {
        if (  (data[21].fvalue < 2.3499691)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.12405285)) {
              sum += (float)0.060367826;
            } else {
              sum += (float)0.035422839;
            }
          } else {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.001189049;
            } else {
              sum += (float)-0.49375165;
            }
          }
        } else {
          if (  (data[9].fvalue < 5.763803)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)-0.0072671683;
            } else {
              sum += (float)0.15789738;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.025171353;
            } else {
              sum += (float)-0.045223068;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.012208773;
            } else {
              sum += (float)-0.095481463;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.0049037845;
            } else {
              sum += (float)-0.011608279;
            }
          }
        } else {
          if (  (data[9].fvalue < 6.671382)) {
            if (  (data[9].fvalue < 5.1420555)) {
              sum += (float)-0.49794909;
            } else {
              sum += (float)-0.41096702;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.22964175;
            } else {
              sum += (float)-0.39545286;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 3.3716969)) {
    if (  (data[22].fvalue < 0.02358239)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.10804536)) {
              sum += (float)0.033042341;
            } else {
              sum += (float)-0.0023212638;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.043340486;
            } else {
              sum += (float)0.0018955118;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.66506737)) {
            if (  (data[8].fvalue < 0.85831708)) {
              sum += (float)-0.022963842;
            } else {
              sum += (float)-0.00013082835;
            }
          } else {
            if (  (data[18].fvalue < 0.015539871)) {
              sum += (float)-0.029988671;
            } else {
              sum += (float)0.021252126;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.085659191)) {
          if (  (data[8].fvalue < 0.55858201)) {
            if (  (data[10].fvalue < 0.51174974)) {
              sum += (float)-0.30776605;
            } else {
              sum += (float)-0.14386097;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.14502604;
            } else {
              sum += (float)-0.15049531;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.27095222)) {
            if (  (data[10].fvalue < 0.13779594)) {
              sum += (float)0.017228451;
            } else {
              sum += (float)0.1265959;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.033327486;
            } else {
              sum += (float)0.004458264;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.12108589)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[22].fvalue < 0.052961029)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.019677972;
            } else {
              sum += (float)0.026409302;
            }
          } else {
            if (  (data[20].fvalue < 0.3918578)) {
              sum += (float)0.010989022;
            } else {
              sum += (float)0.047611155;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.0976464)) {
            if (  (data[8].fvalue < 0.50490481)) {
              sum += (float)-0.28230613;
            } else {
              sum += (float)0.13559914;
            }
          } else {
            if (  (data[10].fvalue < 0.56120032)) {
              sum += (float)0.096002936;
            } else {
              sum += (float)0.20893976;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.33957627)) {
          if (  (data[10].fvalue < 0.4570044)) {
            if (  (data[22].fvalue < 0.14961903)) {
              sum += (float)0.0098059634;
            } else {
              sum += (float)-0.03088426;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0074583604;
            } else {
              sum += (float)0.083591811;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.075459644)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0058748596;
            } else {
              sum += (float)0.11823849;
            }
          } else {
            if (  (data[20].fvalue < 0.11556686)) {
              sum += (float)0.010777082;
            } else {
              sum += (float)-0.0045000864;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 2.999999)) {
      if (  (data[20].fvalue < 0.049424868)) {
        if (  (data[10].fvalue < 0.41032577)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.039003082;
            } else {
              sum += (float)0.0052269488;
            }
          } else {
            if (  (data[18].fvalue < 0.78693688)) {
              sum += (float)-0.046082113;
            } else {
              sum += (float)-0.071794905;
            }
          }
        } else {
          if (  (data[8].fvalue < 6.5283384)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0014061852;
            } else {
              sum += (float)0.057268567;
            }
          } else {
            if (  (data[8].fvalue < 16.958168)) {
              sum += (float)-0.028207397;
            } else {
              sum += (float)0.0026069225;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.09157525)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.71500421)) {
              sum += (float)0.07683558;
            } else {
              sum += (float)-0.090567075;
            }
          } else {
            if (  (data[18].fvalue < 0.063555166)) {
              sum += (float)-0.078647129;
            } else {
              sum += (float)0.045502841;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.35449886)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.015654616;
            } else {
              sum += (float)-0.054241709;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0027301903;
            } else {
              sum += (float)-0.0085205883;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.19602951)) {
        if (  (data[12].fvalue < 7.999999)) {
          if (  (data[18].fvalue < 0.015539871)) {
            if (  (data[10].fvalue < 0.17161429)) {
              sum += (float)-0.74793923;
            } else {
              sum += (float)-0.18808395;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.047887564;
            } else {
              sum += (float)0.10971967;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.12320928)) {
            if (  (data[18].fvalue < 0.085659191)) {
              sum += (float)-0.70325512;
            } else {
              sum += (float)-0.46737278;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.045912039;
            } else {
              sum += (float)-0.38189036;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 0.14997028)) {
            if (  (data[20].fvalue < 0.020148065)) {
              sum += (float)-0.0025509242;
            } else {
              sum += (float)0.062437639;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.02364058;
            } else {
              sum += (float)-0.10049924;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.8683666)) {
            if (  (data[20].fvalue < 0.11556686)) {
              sum += (float)0.054688748;
            } else {
              sum += (float)0.014007796;
            }
          } else {
            if (  (data[10].fvalue < 0.3484537)) {
              sum += (float)-0.018971521;
            } else {
              sum += (float)0.014400464;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 6.9670391)) {
    if (  (data[9].fvalue < 1.0363704)) {
      if (  (data[8].fvalue < 0.029373704)) {
        if (  (data[9].fvalue < 0.9695363)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.43167394)) {
              sum += (float)0.52903301;
            } else {
              sum += (float)0.7794984;
            }
          } else {
            if (  (data[5].fvalue < 0.65004188)) {
              sum += (float)-0.16889302;
            } else {
              sum += (float)0.34001574;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.14706422)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.021887576;
            } else {
              sum += (float)0.16470112;
            }
          } else {
            if (  (data[1].fvalue < 0.99999899)) {
              sum += (float)-0.03973157;
            } else {
              sum += (float)0.01964323;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.017716354)) {
          if (  (data[20].fvalue < 0.07044287)) {
            if (  (data[8].fvalue < 2.6232314)) {
              sum += (float)0.0083142836;
            } else {
              sum += (float)-0.054554138;
            }
          } else {
            if (  (data[8].fvalue < 6.5356336)) {
              sum += (float)-0.11240723;
            } else {
              sum += (float)0.086366519;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.1954987)) {
            if (  (data[8].fvalue < 0.92404294)) {
              sum += (float)-0.29517129;
            } else {
              sum += (float)-0.13202184;
            }
          } else {
            if (  (data[8].fvalue < 6.5356336)) {
              sum += (float)-0.061864112;
            } else {
              sum += (float)0.039352626;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.020779859)) {
        if ( (data[9].fvalue < 3.3409524)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.017716354)) {
              sum += (float)-0.013144901;
            } else {
              sum += (float)-0.0022261229;
            }
          } else {
            if (  (data[9].fvalue < 1.3232704)) {
              sum += (float)-0.13331084;
            } else {
              sum += (float)-0.038502816;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.763466)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.0046238662;
            } else {
              sum += (float)0.052770268;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.011565872;
            } else {
              sum += (float)-0.091370761;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.11744724)) {
          if (  (data[20].fvalue < 0.048416551)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.0057816994;
            } else {
              sum += (float)-0.121824;
            }
          } else {
            if (  (data[9].fvalue < 1.3232704)) {
              sum += (float)0.086572409;
            } else {
              sum += (float)0.036940098;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.25518915)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.031808212;
            } else {
              sum += (float)0.049995765;
            }
          } else {
            if (  (data[9].fvalue < 2.236655)) {
              sum += (float)0.018079223;
            } else {
              sum += (float)-0.0019527157;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 8.4823904)) {
      if (  (data[22].fvalue < 12.712636)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[20].fvalue < 0.82126004)) {
              sum += (float)-0.008414017;
            } else {
              sum += (float)0.0068948683;
            }
          } else {
            if (  (data[22].fvalue < 2.8311372)) {
              sum += (float)0.11228969;
            } else {
              sum += (float)0.028762672;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.9560231)) {
            if (  (data[11].fvalue < 0.90171909)) {
              sum += (float)-0.013802662;
            } else {
              sum += (float)0.052392546;
            }
          } else {
            if (  (data[20].fvalue < 5.3705378)) {
              sum += (float)0.06974902;
            } else {
              sum += (float)0.03163762;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 20.063005)) {
          if (  (data[9].fvalue < 7.8963203)) {
            if (  (data[20].fvalue < 3.4665604)) {
              sum += (float)0.011082249;
            } else {
              sum += (float)-0.0051570884;
            }
          } else {
            if (  (data[20].fvalue < 2.0207255)) {
              sum += (float)0.041976884;
            } else {
              sum += (float)0.015327062;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 7.8963203)) {
              sum += (float)-0.025053494;
            } else {
              sum += (float)-0.0079094209;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)0.017789187;
            } else {
              sum += (float)0.1605802;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 2.999999)) {
        if (  (data[22].fvalue < 0.093996204)) {
          if (  (data[20].fvalue < 19.909426)) {
            if (  (data[11].fvalue < 0.90171909)) {
              sum += (float)0.033127826;
            } else {
              sum += (float)0.067748539;
            }
          } else {
            if (  (data[8].fvalue < 2.7922244)) {
              sum += (float)-0.038038481;
            } else {
              sum += (float)0.018700367;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 15.75771)) {
              sum += (float)0.025276955;
            } else {
              sum += (float)0.0027983284;
            }
          } else {
            if (  (data[11].fvalue < 0.82510132)) {
              sum += (float)-0.010452367;
            } else {
              sum += (float)0.0030027532;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 5.3054223)) {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 0.23321545)) {
              sum += (float)-0.16991904;
            } else {
              sum += (float)-0.01652343;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.011211559;
            } else {
              sum += (float)-0.064562246;
            }
          }
        } else {
          if (  (data[14].fvalue < 5.999999)) {
            if (  (data[22].fvalue < 15.539835)) {
              sum += (float)-0.010487811;
            } else {
              sum += (float)0.0056374143;
            }
          } else {
            if (  (data[22].fvalue < 10.959826)) {
              sum += (float)0.066951878;
            } else {
              sum += (float)0.25055793;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 1.7261407)) {
    if (  (data[20].fvalue < 0.020824712)) {
      if (  (data[19].fvalue < -1e-06)) {
        sum += (float)0.50453973;
      } else {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[21].fvalue < 4.6736135)) {
            if (  (data[19].fvalue < 0.23385033)) {
              sum += (float)-0.0032035017;
            } else {
              sum += (float)0.0043179286;
            }
          } else {
            if (  (data[19].fvalue < 0.84168607)) {
              sum += (float)-0.021430915;
            } else {
              sum += (float)-0.081841789;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.0089542493)) {
            if (  (data[22].fvalue < 0.34414393)) {
              sum += (float)0.22926134;
            } else {
              sum += (float)0.01436401;
            }
          } else {
            if (  (data[22].fvalue < 1.533657)) {
              sum += (float)-0.095455058;
            } else {
              sum += (float)-0.021423971;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.091804281)) {
        if (  (data[19].fvalue < 0.086041592)) {
          if (  (data[19].fvalue < 0.0089542493)) {
            if (  (data[22].fvalue < 0.02180553)) {
              sum += (float)-0.00043499123;
            } else {
              sum += (float)0.044222776;
            }
          } else {
            if (  (data[20].fvalue < 0.049152415)) {
              sum += (float)-0.10523957;
            } else {
              sum += (float)-0.031305648;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.025120836)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.11717115;
            } else {
              sum += (float)0.025774017;
            }
          } else {
            if (  (data[21].fvalue < 0.27694169)) {
              sum += (float)-0.039101794;
            } else {
              sum += (float)0.028405834;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 5.999999)) {
          if (  (data[21].fvalue < 9.2892704)) {
            if (  (data[20].fvalue < 0.21925505)) {
              sum += (float)-0.007290734;
            } else {
              sum += (float)0.0046875207;
            }
          } else {
            if (  (data[19].fvalue < 0.88687968)) {
              sum += (float)-0.0051537724;
            } else {
              sum += (float)-0.017585935;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.020051323)) {
            if (  (data[21].fvalue < 1.973309)) {
              sum += (float)-0.47754437;
            } else {
              sum += (float)-0.22266752;
            }
          } else {
            if (  (data[20].fvalue < 0.82683712)) {
              sum += (float)0.10466823;
            } else {
              sum += (float)-0.32415077;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 0.091804281)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[22].fvalue < 0.15277924)) {
          if (  (data[21].fvalue < 0.025120836)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.036896352;
            } else {
              sum += (float)0.093739681;
            }
          } else {
            if (  (data[21].fvalue < 0.97027868)) {
              sum += (float)0.090185761;
            } else {
              sum += (float)-0.014709018;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99327523)) {
            if (  (data[5].fvalue < 23.559271)) {
              sum += (float)-0.0088622272;
            } else {
              sum += (float)0.042089634;
            }
          } else {
            if (  (data[5].fvalue < 32.200481)) {
              sum += (float)-0.034576118;
            } else {
              sum += (float)-0.0089930184;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.025120836)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.020824712)) {
              sum += (float)-0.03736639;
            } else {
              sum += (float)0.10454052;
            }
          } else {
            if (  (data[22].fvalue < 0.87375307)) {
              sum += (float)0.11787003;
            } else {
              sum += (float)0.0092451265;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.72093415)) {
            if (  (data[22].fvalue < 0.02180553)) {
              sum += (float)0.212896;
            } else {
              sum += (float)0.076498374;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.0075009838;
            } else {
              sum += (float)0.061939038;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 0.9937945)) {
        if (  (data[5].fvalue < 29.012362)) {
          if (  (data[17].fvalue < 2.999999)) {
            if (  (data[21].fvalue < 0.025120836)) {
              sum += (float)-0.028373603;
            } else {
              sum += (float)-0.013882403;
            }
          } else {
            if (  (data[21].fvalue < 0.57300192)) {
              sum += (float)-0.068909034;
            } else {
              sum += (float)-0.19066845;
            }
          }
        } else {
          if (  (data[21].fvalue < 6.8807673)) {
            if (  (data[20].fvalue < 5.8501496)) {
              sum += (float)0.02355692;
            } else {
              sum += (float)-0.0013603858;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.013967272;
            } else {
              sum += (float)-0.046305884;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.2880578)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.774041)) {
              sum += (float)0.010206744;
            } else {
              sum += (float)-0.07486432;
            }
          } else {
            if (  (data[21].fvalue < 0.21429731)) {
              sum += (float)-0.06839446;
            } else {
              sum += (float)-0.040907733;
            }
          }
        } else {
          if (  (data[20].fvalue < 1.1981101)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.0056658424;
            } else {
              sum += (float)-0.015436285;
            }
          } else {
            if (  (data[17].fvalue < 2.999999)) {
              sum += (float)-0.023534736;
            } else {
              sum += (float)-0.12690249;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 5.999999)) {
    if (  (data[15].fvalue < 2.999999)) {
      if (  (data[14].fvalue < 4.999999)) {
        if (  (data[14].fvalue < 3.999999)) {
          if (  (data[22].fvalue < 9.8497095)) {
            if ( (data[9].fvalue < 5.987576)) {
              sum += (float)-0.001130036;
            } else {
              sum += (float)0.016027696;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.016138893;
            } else {
              sum += (float)0.011730804;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.34921277;
            } else {
              sum += (float)0.15808912;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.17402039;
            } else {
              sum += (float)0.020368688;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if ( (data[9].fvalue < 6.3724442)) {
            if (  (data[18].fvalue < 0.063731238)) {
              sum += (float)0.5937593;
            } else {
              sum += (float)0.4371013;
            }
          } else {
            if (  (data[22].fvalue < 2.0931978)) {
              sum += (float)0.15241629;
            } else {
              sum += (float)0.38158834;
            }
          }
        } else {
          if (  (data[14].fvalue < 5.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.33717951;
            } else {
              sum += (float)0.12647143;
            }
          } else {
            if (  (data[11].fvalue < 0.69823581)) {
              sum += (float)0.52146131;
            } else {
              sum += (float)0.35220593;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 1.8704165)) {
              sum += (float)0.41178164;
            } else {
              sum += (float)0.29545021;
            }
          } else {
            if (  (data[18].fvalue < 1.6098343)) {
              sum += (float)0.21036154;
            } else {
              sum += (float)0.068019934;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 4.5043068)) {
              sum += (float)0.076696008;
            } else {
              sum += (float)-0.023808027;
            }
          } else {
            if (  (data[18].fvalue < 2.0461786)) {
              sum += (float)0.48299906;
            } else {
              sum += (float)0.3558422;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.12628534;
            } else {
              sum += (float)0.0053653885;
            }
          } else {
            if (  (data[6].fvalue < 8.2336969)) {
              sum += (float)-0.029628972;
            } else {
              sum += (float)-0.0041383137;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.0097484831;
            } else {
              sum += (float)0.14802359;
            }
          } else {
            if (  (data[18].fvalue < 2.7840202)) {
              sum += (float)0.001049313;
            } else {
              sum += (float)-0.04366358;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 2.999999)) {
      if ( (data[9].fvalue < 6.9718332)) {
        if (  (data[9].fvalue < 5.4515209)) {
          sum += (float)0.63543338;
        } else {
          sum += (float)0.5264529;
        }
      } else {
        sum += (float)0.36501035;
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 2.6809952)) {
            sum += (float)0.23073374;
          } else {
            sum += (float)0.3702113;
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[11].fvalue < 0.79665464)) {
              sum += (float)0.24397525;
            } else {
              sum += (float)-0.015224226;
            }
          } else {
            if (  (data[22].fvalue < 2.9896686)) {
              sum += (float)-0.17031454;
            } else {
              sum += (float)0.041628819;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 4.999999)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 2.0461786)) {
              sum += (float)0.36137289;
            } else {
              sum += (float)0.22260043;
            }
          } else {
            if (  (data[18].fvalue < 2.4299128)) {
              sum += (float)0.57128459;
            } else {
              sum += (float)0.44998387;
            }
          }
        } else {
          if (  (data[14].fvalue < 5.999999)) {
            if (  (data[18].fvalue < 2.0461786)) {
              sum += (float)0.23588426;
            } else {
              sum += (float)0.051840771;
            }
          } else {
            sum += (float)0.026576549;
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 17.999998)) {
    if (  (data[13].fvalue < 15.999999)) {
      if (  (data[12].fvalue < 10.999999)) {
        if (  (data[8].fvalue < 10.952457)) {
          if (  (data[18].fvalue < 2.245538)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.0020384716;
            } else {
              sum += (float)-0.005479096;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.015547524;
            } else {
              sum += (float)0.053497598;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.0094041)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.0052520931;
            } else {
              sum += (float)-0.025818482;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.013556917;
            } else {
              sum += (float)0.06519182;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[18].fvalue < 0.83336657)) {
              sum += (float)0.057298128;
            } else {
              sum += (float)-0.31859815;
            }
          } else {
            if (  (data[18].fvalue < 0.24473418)) {
              sum += (float)-0.37188688;
            } else {
              sum += (float)-0.059157394;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.21500215;
            } else {
              sum += (float)0.081057094;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.19851099;
            } else {
              sum += (float)-0.019517547;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 7.999999)) {
        if (  (data[18].fvalue < 0.33291027)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[18].fvalue < 0.1356626)) {
              sum += (float)0.12692554;
            } else {
              sum += (float)-0.21103208;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.10292294;
            } else {
              sum += (float)0.32485017;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.240511;
            } else {
              sum += (float)-0.004097383;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.49997705;
            } else {
              sum += (float)-0.192339;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.21125714)) {
          if (  (data[12].fvalue < 10.999999)) {
            if (  (data[18].fvalue < 0.099274628)) {
              sum += (float)-0.43240574;
            } else {
              sum += (float)0.053399727;
            }
          } else {
            if (  (data[10].fvalue < 0.51218551)) {
              sum += (float)-0.40305755;
            } else {
              sum += (float)0.094988152;
            }
          }
        } else {
          if (  (data[18].fvalue < 2.4445436)) {
            if (  (data[8].fvalue < 0.78723323)) {
              sum += (float)0.13408442;
            } else {
              sum += (float)0.037334904;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.025478525;
            } else {
              sum += (float)0.191285;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 15.999999)) {
      if (  (data[18].fvalue < 1.8762434)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 34.483994)) {
              sum += (float)-0.35490257;
            } else {
              sum += (float)-0.19989438;
            }
          } else {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)-0.19310679;
            } else {
              sum += (float)-0.046583299;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)0.13860939;
            } else {
              sum += (float)-0.020416163;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.1688703;
            } else {
              sum += (float)0.025914751;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.36942559;
            } else {
              sum += (float)-0.49183962;
            }
          } else {
            if (  (data[5].fvalue < 36.819763)) {
              sum += (float)-0.33615229;
            } else {
              sum += (float)-0.24517484;
            }
          }
        } else {
          if (  (data[12].fvalue < 18.999998)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.056638993;
            } else {
              sum += (float)-0.25061187;
            }
          } else {
            if (  (data[18].fvalue < 2.7705443)) {
              sum += (float)-0.24730061;
            } else {
              sum += (float)-0.39876994;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.94304329)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.15082951)) {
              sum += (float)-0.19793057;
            } else {
              sum += (float)-0.34728256;
            }
          } else {
            if (  (data[13].fvalue < 18.999998)) {
              sum += (float)-0.037848242;
            } else {
              sum += (float)-0.22542149;
            }
          }
        } else {
          if (  (data[8].fvalue < 4.7280464)) {
            sum += (float)-0.59410268;
          } else {
            sum += (float)-0.40144241;
          }
        }
      } else {
        if (  (data[13].fvalue < 21.999998)) {
          if (  (data[10].fvalue < 0.62572002)) {
            if (  (data[13].fvalue < 17.999998)) {
              sum += (float)-0.0011582632;
            } else {
              sum += (float)0.15441428;
            }
          } else {
            if (  (data[5].fvalue < 18.049164)) {
              sum += (float)-0.39534563;
            } else {
              sum += (float)-0.17140293;
            }
          }
        } else {
          sum += (float)0.47650838;
        }
      }
    }
  }
  if (  (data[15].fvalue < 3.999999)) {
    if (  (data[12].fvalue < 14.999999)) {
      if (  (data[21].fvalue < 2.222748)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[18].fvalue < 1.0061471)) {
              sum += (float)-0.00097240944;
            } else {
              sum += (float)0.012033713;
            }
          } else {
            if (  (data[18].fvalue < 0.12453312)) {
              sum += (float)-0.27937791;
            } else {
              sum += (float)-0.015327531;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.010122855;
            } else {
              sum += (float)-0.041856751;
            }
          } else {
            if (  (data[21].fvalue < 0.213636)) {
              sum += (float)0.25167286;
            } else {
              sum += (float)0.057471663;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.005557898;
            } else {
              sum += (float)-0.10801025;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.024114856;
            } else {
              sum += (float)-0.0095903045;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[18].fvalue < 1.3000454)) {
              sum += (float)-0.00021883036;
            } else {
              sum += (float)-0.021711065;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.15635444;
            } else {
              sum += (float)-0.010189678;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 18.999998)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.45824489;
            } else {
              sum += (float)-0.27632946;
            }
          } else {
            if (  (data[18].fvalue < 0.55676013)) {
              sum += (float)-0.26648352;
            } else {
              sum += (float)0.05172088;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.66838431)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.062186971;
            } else {
              sum += (float)0.19731773;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.18076891;
            } else {
              sum += (float)0.013146778;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 1.6086278)) {
            if (  (data[9].fvalue < 1.3232553)) {
              sum += (float)0.1245953;
            } else {
              sum += (float)-0.1099249;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.37204731;
            } else {
              sum += (float)-0.11503625;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.57366234)) {
            if (  (data[21].fvalue < 0.025099155)) {
              sum += (float)0.52540004;
            } else {
              sum += (float)0.15076461;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.059164263;
            } else {
              sum += (float)0.12729488;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 5.999999)) {
      if (  (data[13].fvalue < 5.999999)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[22].fvalue < 0.86936259)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.034825783;
            } else {
              sum += (float)-0.12408295;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.030516153;
            } else {
              sum += (float)-0.030834492;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.0061471)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.20261399;
            } else {
              sum += (float)0.027540928;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.10389689;
            } else {
              sum += (float)0.04010779;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 8.999999)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 1.1471711)) {
              sum += (float)-0.27024478;
            } else {
              sum += (float)-0.41682285;
            }
          } else {
            if (  (data[21].fvalue < 3.8015451)) {
              sum += (float)-0.0025569275;
            } else {
              sum += (float)-0.12324649;
            }
          }
        } else {
          if (  (data[13].fvalue < 11.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.28653249;
            } else {
              sum += (float)-0.42340261;
            }
          } else {
            if (  (data[9].fvalue < 5.7806931)) {
              sum += (float)-0.71694708;
            } else {
              sum += (float)-0.55579919;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 11.999999)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[18].fvalue < 1.2244309)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.58817542;
            } else {
              sum += (float)0.28081837;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.30007488;
            } else {
              sum += (float)0.097523101;
            }
          }
        } else {
          if (  (data[21].fvalue < 2.222748)) {
            if (  (data[9].fvalue < 2.7998538)) {
              sum += (float)-0.14359303;
            } else {
              sum += (float)0.2048146;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)0.013388305;
            } else {
              sum += (float)-0.17035523;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 4.999999)) {
          if (  (data[21].fvalue < 2.222748)) {
            if (  (data[21].fvalue < 0.90308493)) {
              sum += (float)0.49836746;
            } else {
              sum += (float)0.31670976;
            }
          } else {
            if (  (data[18].fvalue < 2.0543776)) {
              sum += (float)0.22691232;
            } else {
              sum += (float)0.075741269;
            }
          }
        } else {
          if (  (data[21].fvalue < 6.8513136)) {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.56687891;
            } else {
              sum += (float)0.41530582;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)0.2081643;
            } else {
              sum += (float)0.4641268;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 14.661214)) {
    if (  (data[15].fvalue < 3.999999)) {
      if (  (data[21].fvalue < 0.15073986)) {
        if (  (data[21].fvalue < 0.024894726)) {
          if (  (data[8].fvalue < 0.22524789)) {
            if (  (data[22].fvalue < 0.15069894)) {
              sum += (float)0.012069758;
            } else {
              sum += (float)-0.020246971;
            }
          } else {
            if (  (data[11].fvalue < 0.011939837)) {
              sum += (float)-0.032307643;
            } else {
              sum += (float)-0.0014824015;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.065566353;
            } else {
              sum += (float)0.02870512;
            }
          } else {
            if (  (data[22].fvalue < 0.68602866)) {
              sum += (float)-0.0051027574;
            } else {
              sum += (float)0.014843255;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.21522154)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.17907362)) {
              sum += (float)-0.037970029;
            } else {
              sum += (float)-0.057581332;
            }
          } else {
            if (  (data[21].fvalue < 0.17907362)) {
              sum += (float)0.15381843;
            } else {
              sum += (float)0.066590391;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.029949466)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.031892281;
            } else {
              sum += (float)-0.083655596;
            }
          } else {
            if (  (data[21].fvalue < 0.61950165)) {
              sum += (float)0.0047543291;
            } else {
              sum += (float)-0.0019188992;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 7.4775715)) {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[8].fvalue < 9.3097734)) {
              sum += (float)0.04748524;
            } else {
              sum += (float)0.14140747;
            }
          } else {
            if (  (data[21].fvalue < 4.6645284)) {
              sum += (float)0.2852695;
            } else {
              sum += (float)0.1618197;
            }
          }
        } else {
          if (  (data[8].fvalue < 7.8896003)) {
            if (  (data[22].fvalue < 2.6824741)) {
              sum += (float)-0.016520077;
            } else {
              sum += (float)0.028379913;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.074900314;
            } else {
              sum += (float)0.031611927;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 2.0971227)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.19559146;
            } else {
              sum += (float)0.027318884;
            }
          } else {
            if (  (data[11].fvalue < 0.55184954)) {
              sum += (float)0.006952777;
            } else {
              sum += (float)-0.074528575;
            }
          }
        } else {
          if (  (data[22].fvalue < 10.974582)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.046366718;
            } else {
              sum += (float)0.00025882595;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.16223633;
            } else {
              sum += (float)0.036603551;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 0.99999899)) {
      if (  (data[22].fvalue < 0.020877352)) {
        if (  (data[8].fvalue < 5.0205188)) {
          if (  (data[8].fvalue < 0.99315631)) {
            if (  (data[8].fvalue < 0.18632449)) {
              sum += (float)-0.031079765;
            } else {
              sum += (float)-0.10289556;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.4876931;
            } else {
              sum += (float)-0.040182266;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 9.3097734)) {
              sum += (float)0.11021661;
            } else {
              sum += (float)-0.03318001;
            }
          } else {
            if (  (data[8].fvalue < 10.911356)) {
              sum += (float)-0.11382319;
            } else {
              sum += (float)-0.051776547;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.23533554)) {
            if (  (data[7].fvalue < 0.98185521)) {
              sum += (float)0.0031036073;
            } else {
              sum += (float)-0.10595232;
            }
          } else {
            if (  (data[11].fvalue < 0.61015844)) {
              sum += (float)0.066972598;
            } else {
              sum += (float)0.013233427;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.53852874)) {
              sum += (float)-0.085923195;
            } else {
              sum += (float)0.00057600025;
            }
          } else {
            if (  (data[11].fvalue < 0.74466217)) {
              sum += (float)-0.023097647;
            } else {
              sum += (float)-0.070102662;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.82308346)) {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[11].fvalue < 0.65544367)) {
            if (  (data[7].fvalue < 0.97447634)) {
              sum += (float)-0.099861212;
            } else {
              sum += (float)-0.0043416144;
            }
          } else {
            if (  (data[7].fvalue < 0.97147)) {
              sum += (float)0.0024538753;
            } else {
              sum += (float)-0.018411417;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.18918228;
            } else {
              sum += (float)0.049778767;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.0084485449;
            } else {
              sum += (float)-0.022240249;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 4.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.0068964739;
            } else {
              sum += (float)0.19473992;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.028941961;
            } else {
              sum += (float)-0.014120839;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.93416536)) {
              sum += (float)0.037279826;
            } else {
              sum += (float)-0.049131807;
            }
          } else {
            if (  (data[22].fvalue < 2.4944887)) {
              sum += (float)-0.18695036;
            } else {
              sum += (float)-0.072206169;
            }
          }
        }
      }
    }
  }
  if (  (data[20].fvalue < 5.8692117)) {
    if ( (data[9].fvalue < 4.7284756)) {
      if (  (data[9].fvalue < 1.7230693)) {
        if (  (data[19].fvalue < 0.54673743)) {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.0030141722;
            } else {
              sum += (float)0.077979051;
            }
          } else {
            if (  (data[9].fvalue < 1.2721412)) {
              sum += (float)0.031033257;
            } else {
              sum += (float)-0.11122788;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.02866566;
            } else {
              sum += (float)-0.042918198;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.079159841;
            } else {
              sum += (float)0.23010704;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.31921268)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.0015553951;
            } else {
              sum += (float)-0.18291962;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.11280514;
            } else {
              sum += (float)0.01681447;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.0028342402;
            } else {
              sum += (float)-0.092189714;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.038920719;
            } else {
              sum += (float)-0.0056143957;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 2.8216486)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 2.4360292)) {
            if (  (data[19].fvalue < 0.79902667)) {
              sum += (float)0.032250647;
            } else {
              sum += (float)0.056214944;
            }
          } else {
            if (  (data[9].fvalue < 5.7618208)) {
              sum += (float)0.0070257173;
            } else {
              sum += (float)0.040094737;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.57547426)) {
              sum += (float)-0.0018122272;
            } else {
              sum += (float)-0.042391021;
            }
          } else {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.059653312;
            } else {
              sum += (float)-0.35362428;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 7.9128523)) {
          if (  (data[22].fvalue < 5.3249454)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.013404522;
            } else {
              sum += (float)-0.023605606;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.0075589949;
            } else {
              sum += (float)0.023660401;
            }
          }
        } else {
          if (  (data[22].fvalue < 19.974943)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.049142871;
            } else {
              sum += (float)-0.013404535;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.0057493644;
            } else {
              sum += (float)0.069850139;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 8.999999)) {
      if ( (data[9].fvalue < 2.0837638)) {
        if (  (data[19].fvalue < 3.3718932)) {
          if ( (data[9].fvalue < 1.5495671)) {
            if (  (data[19].fvalue < 1.8905268)) {
              sum += (float)-0.41828042;
            } else {
              sum += (float)-0.25398356;
            }
          } else {
            if (  (data[22].fvalue < 0.53455168)) {
              sum += (float)-0.12805073;
            } else {
              sum += (float)-0.25012895;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.093501322)) {
            if (  (data[19].fvalue < 6.8377628)) {
              sum += (float)0.041717079;
            } else {
              sum += (float)0.27208221;
            }
          } else {
            if ( (data[9].fvalue < 1.4889536)) {
              sum += (float)-0.30298179;
            } else {
              sum += (float)-0.13112131;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.1109878)) {
            if (  (data[19].fvalue < 0.019634686)) {
              sum += (float)-0.0080486881;
            } else {
              sum += (float)-0.050334226;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.001488025;
            } else {
              sum += (float)-0.024543177;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.072390698)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.18633969;
            } else {
              sum += (float)-0.39805925;
            }
          } else {
            if (  (data[22].fvalue < 1.3422291)) {
              sum += (float)-0.012539408;
            } else {
              sum += (float)0.004772658;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 5.999999)) {
        if (  (data[19].fvalue < 0.21720418)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.38362482;
            } else {
              sum += (float)-0.12956488;
            }
          } else {
            if (  (data[19].fvalue < 0.16982621)) {
              sum += (float)0.28388986;
            } else {
              sum += (float)0.094871461;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.84731001)) {
            if (  (data[19].fvalue < 0.30128276)) {
              sum += (float)-0.13310374;
            } else {
              sum += (float)-0.24514514;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.22192059;
            } else {
              sum += (float)-0.12005129;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.53455168)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.15113331)) {
              sum += (float)0.02237691;
            } else {
              sum += (float)-0.064499408;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.17388982;
            } else {
              sum += (float)0.30058831;
            }
          }
        } else {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.99884558)) {
              sum += (float)-0.017892204;
            } else {
              sum += (float)-0.077788927;
            }
          } else {
            if (  (data[22].fvalue < 1.6432198)) {
              sum += (float)0.178773;
            } else {
              sum += (float)0.0061331568;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 11.493268)) {
    if ( (data[9].fvalue < 7.2348089)) {
      if (  (data[10].fvalue < 0.13093586)) {
        if (  (data[17].fvalue < 3.999999)) {
          if (  (data[9].fvalue < 0.96968389)) {
            if (  (data[10].fvalue < 0.01915413)) {
              sum += (float)-0.65546358;
            } else {
              sum += (float)0.65995479;
            }
          } else {
            if (  (data[9].fvalue < 1.9388586)) {
              sum += (float)-0.014717633;
            } else {
              sum += (float)-0.00017217743;
            }
          }
        } else {
          if (  (data[9].fvalue < 4.7285876)) {
            if (  (data[9].fvalue < 1.7935021)) {
              sum += (float)-0.034472022;
            } else {
              sum += (float)-0.25446609;
            }
          } else {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.0095399646;
            } else {
              sum += (float)-0.35596535;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.22894238)) {
          if (  (data[9].fvalue < 1.039006)) {
            if (  (data[9].fvalue < 0.99756688)) {
              sum += (float)0.015359323;
            } else {
              sum += (float)-0.027996613;
            }
          } else {
            if (  (data[9].fvalue < 1.2694116)) {
              sum += (float)0.043652281;
            } else {
              sum += (float)-0.0020098304;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.049532261)) {
            if (  (data[21].fvalue < 0.02420206)) {
              sum += (float)-0.0077766869;
            } else {
              sum += (float)0.0030202172;
            }
          } else {
            if (  (data[8].fvalue < 0.85859656)) {
              sum += (float)0.024312209;
            } else {
              sum += (float)0.00095826294;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[20].fvalue < 9.4174566)) {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.99205935)) {
              sum += (float)0.085381977;
            } else {
              sum += (float)0.038050771;
            }
          } else {
            if (  (data[21].fvalue < 5.3743)) {
              sum += (float)0.018529413;
            } else {
              sum += (float)-0.059781998;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.98353422)) {
            if (  (data[8].fvalue < 3.5373538)) {
              sum += (float)-0.028493978;
            } else {
              sum += (float)0.018682851;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.0075884503;
            } else {
              sum += (float)0.028277406;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.35425079)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.073270649;
            } else {
              sum += (float)0.010406769;
            }
          } else {
            if (  (data[10].fvalue < 0.35997286)) {
              sum += (float)-0.015807465;
            } else {
              sum += (float)-0.1558861;
            }
          }
        } else {
          if (  (data[20].fvalue < 11.542469)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.023975937;
            } else {
              sum += (float)-0.022110131;
            }
          } else {
            if (  (data[17].fvalue < 3.999999)) {
              sum += (float)-0.0050918553;
            } else {
              sum += (float)0.094203755;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 1.999999)) {
      if (  (data[20].fvalue < 4.4365039)) {
        if (  (data[10].fvalue < 0.33808562)) {
          if (  (data[21].fvalue < 15.398313)) {
            if (  (data[9].fvalue < 7.2348089)) {
              sum += (float)-0.021647302;
            } else {
              sum += (float)0.0084280828;
            }
          } else {
            if (  (data[9].fvalue < 6.9701638)) {
              sum += (float)-0.026136305;
            } else {
              sum += (float)-0.044438198;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.87617534)) {
              sum += (float)-0.032036789;
            } else {
              sum += (float)0.14632329;
            }
          } else {
            if ( (data[9].fvalue < 2.3182139)) {
              sum += (float)-0.27418333;
            } else {
              sum += (float)0.0048282924;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 6.6389651)) {
          if (  (data[10].fvalue < 0.66605854)) {
            if (  (data[20].fvalue < 5.8730407)) {
              sum += (float)-0.00092529593;
            } else {
              sum += (float)0.012303286;
            }
          } else {
            if (  (data[9].fvalue < 2.8013978)) {
              sum += (float)-0.16363069;
            } else {
              sum += (float)-0.031930156;
            }
          }
        } else {
          if (  (data[20].fvalue < 7.4683805)) {
            if (  (data[9].fvalue < 7.2348089)) {
              sum += (float)-0.0095000742;
            } else {
              sum += (float)-0.022029074;
            }
          } else {
            if (  (data[7].fvalue < 0.99135011)) {
              sum += (float)-0.0052281017;
            } else {
              sum += (float)0.005550907;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 6.6389651)) {
        if (  (data[7].fvalue < 0.99380124)) {
          if (  (data[17].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 14.738995)) {
              sum += (float)0.040733356;
            } else {
              sum += (float)0.075348847;
            }
          } else {
            if (  (data[8].fvalue < 14.81647)) {
              sum += (float)0.10939611;
            } else {
              sum += (float)0.24717259;
            }
          }
        } else {
          if (  (data[9].fvalue < 6.173347)) {
            if (  (data[20].fvalue < 0.28720406)) {
              sum += (float)0.10239909;
            } else {
              sum += (float)0.16128372;
            }
          } else {
            if (  (data[21].fvalue < 14.738995)) {
              sum += (float)0.054681852;
            } else {
              sum += (float)0.11563219;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.99326348)) {
            if (  (data[20].fvalue < 4.4365039)) {
              sum += (float)-0.081055418;
            } else {
              sum += (float)-0.024543624;
            }
          } else {
            if (  (data[21].fvalue < 15.398313)) {
              sum += (float)-0.025116373;
            } else {
              sum += (float)0.044898015;
            }
          }
        } else {
          if (  (data[17].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 15.398313)) {
              sum += (float)-0.00082767714;
            } else {
              sum += (float)0.047230829;
            }
          } else {
            if (  (data[20].fvalue < 4.7888923)) {
              sum += (float)0.11690553;
            } else {
              sum += (float)0.29846421;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 1.041307)) {
    if (  (data[21].fvalue < 0.025317218)) {
      if (  (data[8].fvalue < 0.45292187)) {
        if (  (data[10].fvalue < 0.15768647)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.99664378)) {
              sum += (float)0.047782335;
            } else {
              sum += (float)-0.1317739;
            }
          } else {
            if (  (data[9].fvalue < 0.99664378)) {
              sum += (float)-0.050204042;
            } else {
              sum += (float)0.1821828;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.43979415)) {
            if (  (data[11].fvalue < 0.011758341)) {
              sum += (float)0.014025959;
            } else {
              sum += (float)-0.026535062;
            }
          } else {
            if (  (data[8].fvalue < 0.10783541)) {
              sum += (float)0.035762902;
            } else {
              sum += (float)-0.0022654973;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 1.0525928)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.050310135;
            } else {
              sum += (float)-0.041716781;
            }
          } else {
            if (  (data[8].fvalue < 8.5860968)) {
              sum += (float)-0.063052148;
            } else {
              sum += (float)0.084177233;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.7256072)) {
            if (  (data[5].fvalue < 10.267421)) {
              sum += (float)-0.0088483626;
            } else {
              sum += (float)0.028160512;
            }
          } else {
            if (  (data[5].fvalue < 4.0331402)) {
              sum += (float)-0.10001951;
            } else {
              sum += (float)-0.042374026;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 1.1486042)) {
        if (  (data[8].fvalue < 0.92155313)) {
          if (  (data[21].fvalue < 0.1032425)) {
            if (  (data[9].fvalue < 0.96943766)) {
              sum += (float)-0.4783074;
            } else {
              sum += (float)-0.28800038;
            }
          } else {
            if (  (data[8].fvalue < 0.78813815)) {
              sum += (float)-0.51521099;
            } else {
              sum += (float)-0.3862296;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.1032425)) {
            if (  (data[11].fvalue < 0.11637194)) {
              sum += (float)-0.14866294;
            } else {
              sum += (float)-0.05503954;
            }
          } else {
            if (  (data[21].fvalue < 0.25069517)) {
              sum += (float)-0.24996798;
            } else {
              sum += (float)-0.4385086;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.4783287)) {
          if (  (data[10].fvalue < 1.0525928)) {
            if (  (data[21].fvalue < 0.31144711)) {
              sum += (float)-0.093287207;
            } else {
              sum += (float)-0.16516756;
            }
          } else {
            if (  (data[21].fvalue < 0.082568616)) {
              sum += (float)0.098425806;
            } else {
              sum += (float)-0.026834464;
            }
          }
        } else {
          if (  (data[8].fvalue < 8.5860968)) {
            if (  (data[21].fvalue < 0.77725303)) {
              sum += (float)0.0022945018;
            } else {
              sum += (float)-0.087055452;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.090670049;
            } else {
              sum += (float)-0.0500286;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.1831313)) {
      if (  (data[10].fvalue < 0.36015028)) {
        if (  (data[8].fvalue < 0.029687384)) {
          if (  (data[11].fvalue < 0.74445921)) {
            if (  (data[21].fvalue < 0.082568616)) {
              sum += (float)-0.034655381;
            } else {
              sum += (float)-0.29236737;
            }
          } else {
            if (  (data[9].fvalue < 1.1216635)) {
              sum += (float)-0.51113385;
            } else {
              sum += (float)0.29515028;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.13217793;
            } else {
              sum += (float)-0.079196654;
            }
          } else {
            if (  (data[9].fvalue < 1.1216635)) {
              sum += (float)0.071181871;
            } else {
              sum += (float)-0.017524241;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 14.865647)) {
          if (  (data[9].fvalue < 1.1831313)) {
            if (  (data[21].fvalue < 0.082568616)) {
              sum += (float)-0.019165685;
            } else {
              sum += (float)-0.055856697;
            }
          } else {
            if (  (data[8].fvalue < 10.064178)) {
              sum += (float)-0.34033975;
            } else {
              sum += (float)-0.0065525472;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.29445177;
            } else {
              sum += (float)0.089198388;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.012414521;
            } else {
              sum += (float)-0.28939265;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.2721784)) {
        if (  (data[8].fvalue < 0.6093483)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.083007596;
            } else {
              sum += (float)0.016236713;
            }
          } else {
            if (  (data[21].fvalue < 0.051658329)) {
              sum += (float)0.0053748181;
            } else {
              sum += (float)0.038189352;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.025317218)) {
              sum += (float)-0.065721184;
            } else {
              sum += (float)0.033304974;
            }
          } else {
            if (  (data[21].fvalue < 0.025317218)) {
              sum += (float)0.047125496;
            } else {
              sum += (float)-0.09202683;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.28900704)) {
          if (  (data[9].fvalue < 1.9302404)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.016808644;
            } else {
              sum += (float)0.010006359;
            }
          } else {
            if (  (data[17].fvalue < 5.999999)) {
              sum += (float)-0.0005197518;
            } else {
              sum += (float)-0.42080912;
            }
          }
        } else {
          if (  (data[17].fvalue < 5.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.0033307115;
            } else {
              sum += (float)0.0051856027;
            }
          } else {
            if (  (data[9].fvalue < 1.4958819)) {
              sum += (float)-0.079490766;
            } else {
              sum += (float)0.16373947;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 1.0718364)) {
    if (  (data[13].fvalue < 11.999999)) {
      if (  (data[22].fvalue < 0.34516814)) {
        if (  (data[22].fvalue < 0.15248685)) {
          if (  (data[18].fvalue < 0.015266524)) {
            if (  (data[9].fvalue < 1.1215228)) {
              sum += (float)-0.029624099;
            } else {
              sum += (float)0.0017593757;
            }
          } else {
            if (  (data[9].fvalue < 1.0404998)) {
              sum += (float)0.031898655;
            } else {
              sum += (float)-0.00014063614;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if ( (data[9].fvalue < 1.1815494)) {
              sum += (float)-0.09220919;
            } else {
              sum += (float)-0.009787064;
            }
          } else {
            if ( (data[9].fvalue < 1.7941043)) {
              sum += (float)0.017041039;
            } else {
              sum += (float)-0.097174242;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.7941043)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.71931386)) {
              sum += (float)0.015451542;
            } else {
              sum += (float)-0.072801851;
            }
          } else {
            if (  (data[18].fvalue < 0.29683176)) {
              sum += (float)0.036511272;
            } else {
              sum += (float)0.14247319;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.58873725)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.00065199292;
            } else {
              sum += (float)0.0077932845;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.08659447;
            } else {
              sum += (float)-0.0084752403;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.4990969)) {
        if (  (data[9].fvalue < 1.3191146)) {
          if (  (data[18].fvalue < 0.052299369)) {
            if (  (data[9].fvalue < 1.1215228)) {
              sum += (float)0.19938837;
            } else {
              sum += (float)-0.40382573;
            }
          } else {
            if (  (data[10].fvalue < 0.44027793)) {
              sum += (float)0.22343907;
            } else {
              sum += (float)0.0092093451;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.975904)) {
            if (  (data[10].fvalue < 0.29812321)) {
              sum += (float)-0.092037424;
            } else {
              sum += (float)-0.17721254;
            }
          } else {
            if (  (data[22].fvalue < 0.19471969)) {
              sum += (float)0.022880292;
            } else {
              sum += (float)-0.062923878;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.58173639)) {
          if (  (data[10].fvalue < 0.53530574)) {
            if (  (data[9].fvalue < 1.548211)) {
              sum += (float)0.38988236;
            } else {
              sum += (float)0.053422015;
            }
          } else {
            if (  (data[22].fvalue < 0.020150697)) {
              sum += (float)-0.15738623;
            } else {
              sum += (float)0.040409401;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.5063951)) {
            if ( (data[9].fvalue < 1.7228293)) {
              sum += (float)-0.4160583;
            } else {
              sum += (float)0.0054960554;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.066324696;
            } else {
              sum += (float)0.036783665;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 9.999999)) {
      if (  (data[10].fvalue < 0.38279665)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 1.7628934)) {
              sum += (float)0.012711007;
            } else {
              sum += (float)-0.019165173;
            }
          } else {
            if (  (data[9].fvalue < 1.9327303)) {
              sum += (float)0.074773952;
            } else {
              sum += (float)-0.017995108;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 2.3798356)) {
              sum += (float)-0.042087827;
            } else {
              sum += (float)-0.0014655194;
            }
          } else {
            if (  (data[22].fvalue < 1.9881663)) {
              sum += (float)-0.19082108;
            } else {
              sum += (float)-0.043688439;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[17].fvalue < 3.999999)) {
            if (  (data[22].fvalue < 0.020150697)) {
              sum += (float)-0.013589833;
            } else {
              sum += (float)0.002761269;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.080421567;
            } else {
              sum += (float)0.38437253;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.11650492)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.019141262;
            } else {
              sum += (float)0.10698071;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.0054747621;
            } else {
              sum += (float)0.033517107;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 3.3438306)) {
        if (  (data[11].fvalue < 0.0624208)) {
          if (  (data[11].fvalue < 0.038507238)) {
            sum += (float)0.55165404;
          } else {
            if (  (data[18].fvalue < 1.4909638)) {
              sum += (float)0.24824297;
            } else {
              sum += (float)0.43513131;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.020150697)) {
            if (  (data[10].fvalue < 0.71931386)) {
              sum += (float)0.087124996;
            } else {
              sum += (float)-0.07269109;
            }
          } else {
            if (  (data[9].fvalue < 2.3990262)) {
              sum += (float)0.2105056;
            } else {
              sum += (float)0.12455741;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.58173639)) {
          if (  (data[13].fvalue < 12.999999)) {
            if (  (data[22].fvalue < 0.34516814)) {
              sum += (float)0.023011522;
            } else {
              sum += (float)0.08873225;
            }
          } else {
            if (  (data[22].fvalue < 0.020150697)) {
              sum += (float)0.056270726;
            } else {
              sum += (float)0.11804305;
            }
          }
        } else {
          if (  (data[13].fvalue < 13.999999)) {
            if (  (data[18].fvalue < 1.2983515)) {
              sum += (float)-0.01165491;
            } else {
              sum += (float)0.034237791;
            }
          } else {
            if (  (data[13].fvalue < 21.999998)) {
              sum += (float)-0.017954417;
            } else {
              sum += (float)0.20180407;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < -1e-06)) {
    sum += (float)0.50277317;
  } else {
    if (  (data[19].fvalue < 2.2032187)) {
      if (  (data[13].fvalue < 6.999999)) {
        if (  (data[18].fvalue < 0.6603784)) {
          if (  (data[19].fvalue < 0.18517193)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.0012893995;
            } else {
              sum += (float)-0.1476022;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.0026232023;
            } else {
              sum += (float)0.02108301;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.30131206)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.0027521315;
            } else {
              sum += (float)0.15185285;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.0217508;
            } else {
              sum += (float)-0.063418858;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.10007038)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.44160271)) {
              sum += (float)-0.22728886;
            } else {
              sum += (float)0.59690154;
            }
          } else {
            if (  (data[18].fvalue < 0.097828127)) {
              sum += (float)-0.55469292;
            } else {
              sum += (float)-0.29218799;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.99813676)) {
            if (  (data[18].fvalue < 0.74244511)) {
              sum += (float)-0.02196976;
            } else {
              sum += (float)0.094414383;
            }
          } else {
            if (  (data[18].fvalue < 0.35503581)) {
              sum += (float)0.069985203;
            } else {
              sum += (float)-0.066158153;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.94218332)) {
        if (  (data[13].fvalue < 13.999999)) {
          if (  (data[10].fvalue < 0.67044455)) {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.0061901594;
            } else {
              sum += (float)0.041278504;
            }
          } else {
            if (  (data[21].fvalue < 0.66690069)) {
              sum += (float)0.063898832;
            } else {
              sum += (float)0.01265399;
            }
          }
        } else {
          if (  (data[13].fvalue < 15.999999)) {
            if (  (data[10].fvalue < 0.56114513)) {
              sum += (float)0.09116672;
            } else {
              sum += (float)0.24177852;
            }
          } else {
            if (  (data[10].fvalue < 0.24652568)) {
              sum += (float)0.16694726;
            } else {
              sum += (float)0.26615009;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[18].fvalue < 2.0427325)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.083504491;
            } else {
              sum += (float)-0.039641909;
            }
          } else {
            if (  (data[10].fvalue < 0.71472192)) {
              sum += (float)0.07750985;
            } else {
              sum += (float)0.46300092;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.6117113)) {
            if (  (data[21].fvalue < 0.024948668)) {
              sum += (float)-0.19523999;
            } else {
              sum += (float)-0.084418431;
            }
          } else {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)-0.1762272;
            } else {
              sum += (float)-0.49497947;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 17.999998)) {
    if (  (data[8].fvalue < 0.22546355)) {
      if (  (data[19].fvalue < 0.019584082)) {
        if (  (data[18].fvalue < 0.043283988)) {
          if (  (data[10].fvalue < 0.30811942)) {
            if (  (data[21].fvalue < 0.051015478)) {
              sum += (float)-0.021270329;
            } else {
              sum += (float)-0.14578702;
            }
          } else {
            if (  (data[8].fvalue < 0.027533695)) {
              sum += (float)0.34104028;
            } else {
              sum += (float)-0.16159521;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.0577174)) {
            if (  (data[19].fvalue < 0.0094718561)) {
              sum += (float)0.0016558375;
            } else {
              sum += (float)0.1136213;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.43568245;
            } else {
              sum += (float)-0.033147909;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.61237299)) {
          if (  (data[19].fvalue < 0.050745692)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.10814612;
            } else {
              sum += (float)-0.07652723;
            }
          } else {
            if (  (data[18].fvalue < 0.0029184679)) {
              sum += (float)0.11019294;
            } else {
              sum += (float)0.29206425;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.25390288)) {
            if (  (data[19].fvalue < 0.35719037)) {
              sum += (float)-0.013552448;
            } else {
              sum += (float)0.039389882;
            }
          } else {
            if (  (data[18].fvalue < 0.13521656)) {
              sum += (float)0.050072759;
            } else {
              sum += (float)0.26113644;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.019584082)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.085658692)) {
              sum += (float)-0.023807285;
            } else {
              sum += (float)0.0072515514;
            }
          } else {
            if (  (data[18].fvalue < 0.023204908)) {
              sum += (float)-0.56909221;
            } else {
              sum += (float)-0.025354538;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.035879251;
            } else {
              sum += (float)0.0057016495;
            }
          } else {
            if (  (data[19].fvalue < 0.0094718561)) {
              sum += (float)0.025816159;
            } else {
              sum += (float)0.24812393;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.074372336)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.023204908)) {
              sum += (float)0.0013713974;
            } else {
              sum += (float)-0.039215654;
            }
          } else {
            if (  (data[19].fvalue < 0.21759257)) {
              sum += (float)-0.17517167;
            } else {
              sum += (float)0.21388875;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.16969208)) {
            if (  (data[18].fvalue < 0.16553926)) {
              sum += (float)-0.088190205;
            } else {
              sum += (float)0.021018036;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.020086339;
            } else {
              sum += (float)-0.0020213483;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 1.0033743)) {
      if (  (data[18].fvalue < 1.8702457)) {
        if (  (data[19].fvalue < 0.51587105)) {
          if (  (data[12].fvalue < 18.999998)) {
            if (  (data[19].fvalue < 0.20043615)) {
              sum += (float)-0.56010395;
            } else {
              sum += (float)-0.31021008;
            }
          } else {
            if (  (data[6].fvalue < 20.544209)) {
              sum += (float)-0.51275855;
            } else {
              sum += (float)-0.65831262;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.4108511)) {
            if (  (data[10].fvalue < 0.083583996)) {
              sum += (float)-0.22135338;
            } else {
              sum += (float)0.017017376;
            }
          } else {
            if (  (data[10].fvalue < 0.26275232)) {
              sum += (float)-0.23310988;
            } else {
              sum += (float)-0.4010103;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.26680121)) {
          sum += (float)-0.26868436;
        } else {
          if (  (data[12].fvalue < 19.999998)) {
            if (  (data[18].fvalue < 2.2370844)) {
              sum += (float)-0.042956144;
            } else {
              sum += (float)0.084592797;
            }
          } else {
            if (  (data[21].fvalue < 3.0030603)) {
              sum += (float)-0.048953239;
            } else {
              sum += (float)-0.1928685;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 2.2370844)) {
        if (  (data[19].fvalue < 1.6001613)) {
          if (  (data[8].fvalue < 2.4658694)) {
            if (  (data[19].fvalue < 1.0627631)) {
              sum += (float)-0.053310625;
            } else {
              sum += (float)0.12501779;
            }
          } else {
            if (  (data[21].fvalue < 0.024522539)) {
              sum += (float)-0.37964323;
            } else {
              sum += (float)-0.046142969;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.7426084)) {
            if (  (data[19].fvalue < 2.2044904)) {
              sum += (float)0.21579723;
            } else {
              sum += (float)0.37113541;
            }
          } else {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)-0.023910897;
            } else {
              sum += (float)0.15521131;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 1.7307231)) {
          if (  (data[19].fvalue < 1.4760045)) {
            if (  (data[18].fvalue < 4.20192)) {
              sum += (float)0.071882091;
            } else {
              sum += (float)-0.054229043;
            }
          } else {
            if (  (data[18].fvalue < 3.0236273)) {
              sum += (float)-0.053817198;
            } else {
              sum += (float)-0.16753213;
            }
          }
        } else {
          if (  (data[18].fvalue < 2.7575943)) {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)-0.29547122;
            } else {
              sum += (float)-0.17840943;
            }
          } else {
            if (  (data[19].fvalue < 2.4430962)) {
              sum += (float)-0.32556167;
            } else {
              sum += (float)-0.55893761;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 13.999999)) {
    if (  (data[12].fvalue < 11.999999)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-6.5196356e-05;
            } else {
              sum += (float)0.02794282;
            }
          } else {
            if (  (data[21].fvalue < 0.024971258)) {
              sum += (float)-0.23842432;
            } else {
              sum += (float)-0.024765229;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.099240519)) {
            if (  (data[18].fvalue < 0.19688253)) {
              sum += (float)-0.29136547;
            } else {
              sum += (float)0.270767;
            }
          } else {
            if (  (data[8].fvalue < 0.18993196)) {
              sum += (float)0.092334151;
            } else {
              sum += (float)0.012593497;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[21].fvalue < 0.31398576)) {
              sum += (float)0.0027031782;
            } else {
              sum += (float)-0.006554957;
            }
          } else {
            if (  (data[18].fvalue < 0.78438294)) {
              sum += (float)0.060850795;
            } else {
              sum += (float)-0.022937493;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.21642771)) {
              sum += (float)0.090387888;
            } else {
              sum += (float)-0.078077368;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.013480817;
            } else {
              sum += (float)0.024062227;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 1.0676457)) {
        if (  (data[19].fvalue < 0.60500908)) {
          if (  (data[13].fvalue < 8.999999)) {
            if (  (data[18].fvalue < 0.65623856)) {
              sum += (float)0.22905442;
            } else {
              sum += (float)0.017348398;
            }
          } else {
            if (  (data[20].fvalue < 0.020154577)) {
              sum += (float)-0.097223148;
            } else {
              sum += (float)-0.45355016;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.78438294)) {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)0.38807762;
            } else {
              sum += (float)0.2200845;
            }
          } else {
            if (  (data[21].fvalue < 0.024971258)) {
              sum += (float)-0.062346593;
            } else {
              sum += (float)0.13130838;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.71140623)) {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[19].fvalue < 0.24918951)) {
              sum += (float)0.073509768;
            } else {
              sum += (float)-0.20772761;
            }
          } else {
            if (  (data[18].fvalue < 1.7561784)) {
              sum += (float)0.029509848;
            } else {
              sum += (float)0.24440061;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.024971258)) {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.37997746;
            } else {
              sum += (float)-0.16553442;
            }
          } else {
            if (  (data[18].fvalue < 1.7561784)) {
              sum += (float)-0.030477712;
            } else {
              sum += (float)-0.21204592;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.51665723)) {
      if (  (data[18].fvalue < 1.2171398)) {
        if (  (data[12].fvalue < 8.999999)) {
          if (  (data[18].fvalue < 0.22887067)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.041088268;
            } else {
              sum += (float)-0.25949368;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.67666239;
            } else {
              sum += (float)0.35591632;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.049358021)) {
            if (  (data[18].fvalue < 0.37483695)) {
              sum += (float)-0.31000248;
            } else {
              sum += (float)-0.096918963;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.23247208;
            } else {
              sum += (float)-0.47020164;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.20233378)) {
          sum += (float)-0.59463048;
        } else {
          if (  (data[12].fvalue < 12.999999)) {
            if (  (data[19].fvalue < 0.35688046)) {
              sum += (float)0.6627087;
            } else {
              sum += (float)0.50304067;
            }
          } else {
            if (  (data[18].fvalue < 1.7561784)) {
              sum += (float)0.087686501;
            } else {
              sum += (float)0.41258794;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 7.999999)) {
        if (  (data[19].fvalue < 1.1349299)) {
          if (  (data[19].fvalue < 0.75383526)) {
            if (  (data[13].fvalue < 16.999998)) {
              sum += (float)0.31629533;
            } else {
              sum += (float)0.64063227;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.15362716;
            } else {
              sum += (float)-0.059345208;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.27954081)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.11701792;
            } else {
              sum += (float)0.13140312;
            }
          } else {
            if (  (data[20].fvalue < 0.020154577)) {
              sum += (float)-0.45848948;
            } else {
              sum += (float)-0.20030753;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.72272956)) {
          if (  (data[20].fvalue < 0.50493795)) {
            if (  (data[19].fvalue < 0.63837969)) {
              sum += (float)0.05933075;
            } else {
              sum += (float)0.22754973;
            }
          } else {
            if (  (data[19].fvalue < 0.71140623)) {
              sum += (float)-0.15690608;
            } else {
              sum += (float)0.073569402;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.098444693)) {
            if (  (data[18].fvalue < 0.075699948)) {
              sum += (float)-0.82957971;
            } else {
              sum += (float)-0.40022865;
            }
          } else {
            if (  (data[19].fvalue < 0.63837969)) {
              sum += (float)-0.052667413;
            } else {
              sum += (float)0.065205701;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 2.3314245)) {
    if (  (data[18].fvalue < 1.0669031)) {
      if (  (data[19].fvalue < -1e-06)) {
        sum += (float)0.50027055;
      } else {
        if (  (data[19].fvalue < 0.061415698)) {
          if (  (data[18].fvalue < 0.086533472)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.0072973315;
            } else {
              sum += (float)-0.042102627;
            }
          } else {
            if (  (data[18].fvalue < 0.25993553)) {
              sum += (float)0.022635253;
            } else {
              sum += (float)0.0013372563;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.65709084)) {
            if (  (data[18].fvalue < 0.0025070766)) {
              sum += (float)0.02290382;
            } else {
              sum += (float)0.16045016;
            }
          } else {
            if (  (data[11].fvalue < 0.018042279)) {
              sum += (float)0.025372695;
            } else {
              sum += (float)-0.0011305026;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.71363342)) {
        if (  (data[19].fvalue < 0.039745476)) {
          if (  (data[8].fvalue < 0.10863077)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.12322478;
            } else {
              sum += (float)-0.2251002;
            }
          } else {
            if (  (data[11].fvalue < 0.14713581)) {
              sum += (float)-0.028308693;
            } else {
              sum += (float)-0.011934367;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 1.4848044)) {
              sum += (float)0.0082155103;
            } else {
              sum += (float)0.047601908;
            }
          } else {
            if (  (data[11].fvalue < 0.062748328)) {
              sum += (float)0.11098053;
            } else {
              sum += (float)-0.001935926;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[19].fvalue < 1.4719162)) {
            if (  (data[18].fvalue < 3.050884)) {
              sum += (float)-0.077143215;
            } else {
              sum += (float)-0.024184981;
            }
          } else {
            if (  (data[19].fvalue < 4.0357203)) {
              sum += (float)-0.14146332;
            } else {
              sum += (float)-2.5100915e-05;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.15764578)) {
            if (  (data[19].fvalue < 0.89191085)) {
              sum += (float)0.04694717;
            } else {
              sum += (float)0.15434623;
            }
          } else {
            if (  (data[5].fvalue < 10.21869)) {
              sum += (float)0.11138149;
            } else {
              sum += (float)-0.013456035;
            }
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 0.99999899)) {
      if (  (data[19].fvalue < 0.51691431)) {
        if (  (data[18].fvalue < 0.0025070766)) {
          if (  (data[19].fvalue < 0.0096343979)) {
            if (  (data[11].fvalue < 0.11622651)) {
              sum += (float)0.12128407;
            } else {
              sum += (float)0.010787757;
            }
          } else {
            if (  (data[19].fvalue < 0.30122921)) {
              sum += (float)-0.082138248;
            } else {
              sum += (float)-0.012743305;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.0096343979)) {
            if (  (data[18].fvalue < 0.14980549)) {
              sum += (float)-0.050094709;
            } else {
              sum += (float)0.0075202985;
            }
          } else {
            if (  (data[18].fvalue < 0.033046115)) {
              sum += (float)0.18333979;
            } else {
              sum += (float)0.029568406;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.47203031)) {
          if (  (data[18].fvalue < 0.0025070766)) {
            if (  (data[5].fvalue < 14.108461)) {
              sum += (float)-0.016442316;
            } else {
              sum += (float)0.0004415694;
            }
          } else {
            if (  (data[18].fvalue < 0.02313076)) {
              sum += (float)0.084294379;
            } else {
              sum += (float)0.026733205;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.18984027)) {
            if (  (data[11].fvalue < 0.069953099)) {
              sum += (float)-0.19253641;
            } else {
              sum += (float)-0.093326718;
            }
          } else {
            if (  (data[5].fvalue < 14.923023)) {
              sum += (float)-0.084478639;
            } else {
              sum += (float)-0.022965733;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.94592178)) {
        if (  (data[18].fvalue < 0.55507857)) {
          if (  (data[19].fvalue < 0.49003839)) {
            if (  (data[19].fvalue < 0.019670935)) {
              sum += (float)-0.010380906;
            } else {
              sum += (float)-0.030032102;
            }
          } else {
            if (  (data[18].fvalue < 0.25993553)) {
              sum += (float)0.015185862;
            } else {
              sum += (float)-0.025906155;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.64085245)) {
            if (  (data[18].fvalue < 2.7598901)) {
              sum += (float)0.011938851;
            } else {
              sum += (float)-0.0017609685;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.06515713;
            } else {
              sum += (float)-0.020137129;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.52524567)) {
          if (  (data[18].fvalue < 0.075500511)) {
            if (  (data[19].fvalue < 3.1611595)) {
              sum += (float)0.0085483054;
            } else {
              sum += (float)-0.0037462297;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.02168043;
            } else {
              sum += (float)-0.0062644565;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 4.0357203)) {
              sum += (float)-0.052667882;
            } else {
              sum += (float)-0.0088703213;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.0079302089;
            } else {
              sum += (float)0.01720304;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.067919351)) {
    if (  (data[10].fvalue < 0.71688956)) {
      if (  (data[19].fvalue < 0.0092751607)) {
        if (  (data[10].fvalue < 0.1187181)) {
          if (  (data[20].fvalue < 0.18000932)) {
            if (  (data[11].fvalue < 0.59292507)) {
              sum += (float)0.030146707;
            } else {
              sum += (float)-0.090090312;
            }
          } else {
            if (  (data[20].fvalue < 1.1960074)) {
              sum += (float)-0.15172718;
            } else {
              sum += (float)-0.0043137958;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.51232857)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.11812087;
            } else {
              sum += (float)-0.050662436;
            }
          } else {
            if (  (data[8].fvalue < 0.029412504)) {
              sum += (float)0.43397066;
            } else {
              sum += (float)0.028681777;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.13799344)) {
          if (  (data[19].fvalue < 0.33889779)) {
            if (  (data[21].fvalue < 0.063691154)) {
              sum += (float)-0.014351149;
            } else {
              sum += (float)0.039118163;
            }
          } else {
            if (  (data[10].fvalue < 0.046196684)) {
              sum += (float)0.0061651776;
            } else {
              sum += (float)0.2172519;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.051086213)) {
            if (  (data[19].fvalue < 0.020348283)) {
              sum += (float)0.011221678;
            } else {
              sum += (float)0.086445175;
            }
          } else {
            if (  (data[10].fvalue < 0.30755922)) {
              sum += (float)0.13508387;
            } else {
              sum += (float)0.33667898;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.039535251)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.7831164)) {
            if (  (data[11].fvalue < 0.22423895)) {
              sum += (float)0.53043824;
            } else {
              sum += (float)0.37487894;
            }
          } else {
            if (  (data[19].fvalue < 0.020348283)) {
              sum += (float)0.78357476;
            } else {
              sum += (float)0.58286095;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.88077915)) {
            if (  (data[19].fvalue < 0.0092751607)) {
              sum += (float)0.2799955;
            } else {
              sum += (float)-0.013850415;
            }
          } else {
            if (  (data[11].fvalue < 0.17899556)) {
              sum += (float)0.56256568;
            } else {
              sum += (float)0.74104112;
            }
          }
        }
      } else {
        sum += (float)-0.82167453;
      }
    }
  } else {
    if (  (data[15].fvalue < 2.999999)) {
      if (  (data[20].fvalue < 9.4795647)) {
        if (  (data[11].fvalue < 0.7729156)) {
          if (  (data[10].fvalue < 0.030396646)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.040734857;
            } else {
              sum += (float)0.0049385955;
            }
          } else {
            if (  (data[20].fvalue < 0.049106818)) {
              sum += (float)-0.0035090314;
            } else {
              sum += (float)0.00060860097;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.7831164)) {
              sum += (float)0.025526054;
            } else {
              sum += (float)0.34379575;
            }
          } else {
            if (  (data[17].fvalue < 3.999999)) {
              sum += (float)0.00010706795;
            } else {
              sum += (float)0.064946026;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[21].fvalue < 0.34898412)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.030864784;
            } else {
              sum += (float)0.066835962;
            }
          } else {
            if (  (data[19].fvalue < 0.84698886)) {
              sum += (float)-0.0034390511;
            } else {
              sum += (float)-0.015472733;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 3.999999)) {
              sum += (float)0.04946059;
            } else {
              sum += (float)0.22418681;
            }
          } else {
            if (  (data[17].fvalue < 3.999999)) {
              sum += (float)0.0070097088;
            } else {
              sum += (float)0.083070442;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.71324295)) {
        if (  (data[19].fvalue < 0.26668423)) {
          if (  (data[20].fvalue < 0.42570597)) {
            if (  (data[20].fvalue < 0.14986025)) {
              sum += (float)0.0075041638;
            } else {
              sum += (float)-0.049439806;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-4.4483986e-05;
            } else {
              sum += (float)0.016844984;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.49256814)) {
            if (  (data[20].fvalue < 0.97036976)) {
              sum += (float)-0.072937325;
            } else {
              sum += (float)-0.028572984;
            }
          } else {
            if (  (data[21].fvalue < 0.28118098)) {
              sum += (float)0.18551068;
            } else {
              sum += (float)-0.0071859518;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 3.8219602)) {
          if (  (data[8].fvalue < 3.7605636)) {
            if (  (data[21].fvalue < 0.34898412)) {
              sum += (float)0.19156939;
            } else {
              sum += (float)0.021687245;
            }
          } else {
            if (  (data[21].fvalue < 0.48847902)) {
              sum += (float)0.26807129;
            } else {
              sum += (float)0.076291107;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.20737495)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.00086409575;
            } else {
              sum += (float)-0.065910064;
            }
          } else {
            if (  (data[20].fvalue < 0.11797054)) {
              sum += (float)0.13336846;
            } else {
              sum += (float)0.024707898;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 6.999999)) {
    if (  (data[2].fvalue < 0.99999899)) {
      if (  (data[21].fvalue < 1.1180192)) {
        if (  (data[21].fvalue < 0.35053465)) {
          if (  (data[21].fvalue < 0.10294019)) {
            if (  (data[21].fvalue < 0.064070106)) {
              sum += (float)0.0011400452;
            } else {
              sum += (float)0.026733825;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.013616586;
            } else {
              sum += (float)0.037372421;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.14737095)) {
            if (  (data[21].fvalue < 0.57366794)) {
              sum += (float)0.025286155;
            } else {
              sum += (float)0.0070753107;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.0071850521;
            } else {
              sum += (float)0.0075148512;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.89185762)) {
          if (  (data[10].fvalue < 0.53537744)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.0079947207;
            } else {
              sum += (float)0.00088564743;
            }
          } else {
            if (  (data[19].fvalue < 0.24990973)) {
              sum += (float)-0.001259812;
            } else {
              sum += (float)-0.05981743;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.020851074;
            } else {
              sum += (float)-0.0039783176;
            }
          } else {
            if (  (data[10].fvalue < 0.32856143)) {
              sum += (float)0.0037909381;
            } else {
              sum += (float)0.039759986;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 3.3491616)) {
        if (  (data[19].fvalue < 0.0098799095)) {
          if (  (data[5].fvalue < 1.7072802)) {
            if (  (data[10].fvalue < 0.19279246)) {
              sum += (float)-0.02380701;
            } else {
              sum += (float)-0.04440926;
            }
          } else {
            if (  (data[10].fvalue < 0.88170791)) {
              sum += (float)-0.0018981157;
            } else {
              sum += (float)-0.050392561;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.6261884)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.014093855;
            } else {
              sum += (float)0.017193429;
            }
          } else {
            if (  (data[6].fvalue < 0.2378113)) {
              sum += (float)0.12955737;
            } else {
              sum += (float)0.015347822;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.019723676)) {
          if (  (data[20].fvalue < 1.2004679)) {
            if (  (data[10].fvalue < 0.88170791)) {
              sum += (float)0.0098951692;
            } else {
              sum += (float)-0.031036869;
            }
          } else {
            if (  (data[21].fvalue < 0.025737789)) {
              sum += (float)-0.032412745;
            } else {
              sum += (float)-0.0051427996;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.039068785)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.0048648645;
            } else {
              sum += (float)-0.036002569;
            }
          } else {
            if (  (data[10].fvalue < 0.77907133)) {
              sum += (float)-0.0025067229;
            } else {
              sum += (float)0.015158414;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 1.0039138)) {
      sum += (float)0.23678149;
    } else {
      sum += (float)0.50562543;
    }
  }
  if (  (data[13].fvalue < 10.999999)) {
    if ( (data[9].fvalue < 4.3752074)) {
      if (  (data[18].fvalue < 1.2901533)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.0025192956)) {
            if ( (data[9].fvalue < 1.5549303)) {
              sum += (float)0.19774893;
            } else {
              sum += (float)-0.001075627;
            }
          } else {
            if (  (data[18].fvalue < 0.01518415)) {
              sum += (float)-0.10426489;
            } else {
              sum += (float)0.00053077081;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0411751)) {
            if (  (data[18].fvalue < 0.0025192956)) {
              sum += (float)-0.020321673;
            } else {
              sum += (float)0.051333554;
            }
          } else {
            if (  (data[20].fvalue < 0.32848126)) {
              sum += (float)-0.0090895146;
            } else {
              sum += (float)0.0032134335;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.49230275)) {
          if (  (data[18].fvalue < 3.5268757)) {
            if (  (data[9].fvalue < 2.9768634)) {
              sum += (float)-0.020220587;
            } else {
              sum += (float)-0.0061611217;
            }
          } else {
            if (  (data[18].fvalue < 7.6832042)) {
              sum += (float)-0.026124332;
            } else {
              sum += (float)-0.074202731;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if ( (data[9].fvalue < 1.7929066)) {
              sum += (float)-0.026416907;
            } else {
              sum += (float)0.0065333592;
            }
          } else {
            if ( (data[9].fvalue < 2.3169887)) {
              sum += (float)0.072050914;
            } else {
              sum += (float)-0.0016944472;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 2.0245306)) {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 1.0647129)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.029474147;
            } else {
              sum += (float)0.026425619;
            }
          } else {
            if (  (data[18].fvalue < 7.6832042)) {
              sum += (float)0.073679097;
            } else {
              sum += (float)-0.012060012;
            }
          }
        } else {
          if (  (data[18].fvalue < 7.6832042)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.014412005;
            } else {
              sum += (float)0.0034088329;
            }
          } else {
            if (  (data[20].fvalue < 0.35506529)) {
              sum += (float)-0.029193902;
            } else {
              sum += (float)-0.0021172948;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.52413189)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.075486705)) {
              sum += (float)-0.0092278058;
            } else {
              sum += (float)0.016793741;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.019258525;
            } else {
              sum += (float)-0.0067039924;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99697119)) {
            if (  (data[5].fvalue < 31.386238)) {
              sum += (float)-0.014454192;
            } else {
              sum += (float)-0.0011197025;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.033083789;
            } else {
              sum += (float)-0.017829945;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.55268162)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[18].fvalue < 0.11198606)) {
          if (  (data[9].fvalue < 1.0411751)) {
            if (  (data[18].fvalue < 0.032808889)) {
              sum += (float)0.034698308;
            } else {
              sum += (float)0.40139067;
            }
          } else {
            if (  (data[20].fvalue < 0.020588096)) {
              sum += (float)-0.19644065;
            } else {
              sum += (float)-0.022489186;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.4339765)) {
            if (  (data[10].fvalue < 0.51196074)) {
              sum += (float)0.20096867;
            } else {
              sum += (float)-0.031450476;
            }
          } else {
            if (  (data[20].fvalue < 0.020588096)) {
              sum += (float)-0.07078293;
            } else {
              sum += (float)0.0298256;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.2707949)) {
          if (  (data[5].fvalue < 1.6699382)) {
            if (  (data[11].fvalue < 0.35521656)) {
              sum += (float)0.81020594;
            } else {
              sum += (float)0.22180091;
            }
          } else {
            if (  (data[11].fvalue < 0.01217295)) {
              sum += (float)0.59495217;
            } else {
              sum += (float)0.026546918;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.7977333)) {
            if (  (data[18].fvalue < 0.35278216)) {
              sum += (float)-0.19248269;
            } else {
              sum += (float)-0.0840443;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.0031381908;
            } else {
              sum += (float)-0.075498514;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 1.2123431)) {
        if (  (data[9].fvalue < 1.939113)) {
          if (  (data[10].fvalue < 0.40979412)) {
            if (  (data[9].fvalue < 1.6647612)) {
              sum += (float)0.33319706;
            } else {
              sum += (float)0.15039615;
            }
          } else {
            if (  (data[9].fvalue < 1.4339765)) {
              sum += (float)0.051317692;
            } else {
              sum += (float)-0.093899123;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)0.020860834;
            } else {
              sum += (float)-0.023453346;
            }
          } else {
            if ( (data[9].fvalue < 2.7977333)) {
              sum += (float)0.10407694;
            } else {
              sum += (float)0.022086473;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 3.5349591)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 2.0769577)) {
              sum += (float)0.15130256;
            } else {
              sum += (float)0.062642813;
            }
          } else {
            if (  (data[9].fvalue < 3.0678368)) {
              sum += (float)0.22662748;
            } else {
              sum += (float)0.15388536;
            }
          }
        } else {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 22.999998)) {
              sum += (float)0.034345146;
            } else {
              sum += (float)0.33146995;
            }
          } else {
            if ( (data[9].fvalue < 5.1310816)) {
              sum += (float)0.13395511;
            } else {
              sum += (float)0.053321663;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 0.024235075)) {
    if (  (data[16].fvalue < 0.99999899)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[8].fvalue < 0.031868856)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.44796932)) {
              sum += (float)0.068922348;
            } else {
              sum += (float)-0.0022362366;
            }
          } else {
            if (  (data[18].fvalue < 0.015436516)) {
              sum += (float)-0.06212569;
            } else {
              sum += (float)0.032830104;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.37732571)) {
            if (  (data[18].fvalue < 0.29711893)) {
              sum += (float)0.0040401882;
            } else {
              sum += (float)-0.021294668;
            }
          } else {
            if (  (data[18].fvalue < 0.0034345249)) {
              sum += (float)-0.019494191;
            } else {
              sum += (float)0.019187577;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[18].fvalue < 0.0034345249)) {
            if (  (data[8].fvalue < 0.11124025)) {
              sum += (float)-0.10690702;
            } else {
              sum += (float)-6.8936941e-05;
            }
          } else {
            if (  (data[8].fvalue < 0.071627937)) {
              sum += (float)0.046364546;
            } else {
              sum += (float)-0.00068450556;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.24375324)) {
            if (  (data[8].fvalue < 1.1575534)) {
              sum += (float)-0.086476311;
            } else {
              sum += (float)0.0069584525;
            }
          } else {
            if (  (data[8].fvalue < 0.60692227)) {
              sum += (float)0.044809468;
            } else {
              sum += (float)-0.0056982068;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[18].fvalue < 0.2607564)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.0034345249)) {
              sum += (float)0.17657013;
            } else {
              sum += (float)-0.2176737;
            }
          } else {
            if (  (data[18].fvalue < 0.032943893)) {
              sum += (float)-0.28793222;
            } else {
              sum += (float)-0.11520611;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.036815092;
            } else {
              sum += (float)-0.14643614;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)0.0036505293;
            } else {
              sum += (float)0.16621624;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 1.0742893)) {
            sum += (float)-0.30695876;
          } else {
            sum += (float)-0.50762945;
          }
        } else {
          if (  (data[18].fvalue < 0.52539599)) {
            if (  (data[21].fvalue < 0.44962806)) {
              sum += (float)-0.7588039;
            } else {
              sum += (float)-0.64316589;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.69649297;
            } else {
              sum += (float)-0.39113319;
            }
          }
        }
      }
    }
  } else {
    if (  (data[22].fvalue < 0.094337337)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[18].fvalue < 0.086812638)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.0034345249)) {
              sum += (float)0.13087913;
            } else {
              sum += (float)-0.12364202;
            }
          } else {
            if (  (data[18].fvalue < 0.0034345249)) {
              sum += (float)-0.11559036;
            } else {
              sum += (float)0.038133431;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.023932304)) {
              sum += (float)0.035096534;
            } else {
              sum += (float)0.071730956;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0072829132;
            } else {
              sum += (float)0.043453995;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[22].fvalue < 0.073060699)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.20754346;
            } else {
              sum += (float)-0.40088236;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.10138523;
            } else {
              sum += (float)-0.67526805;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.053510286)) {
            if (  (data[18].fvalue < 0.29711893)) {
              sum += (float)-0.42183113;
            } else {
              sum += (float)-0.095084585;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.10001142;
            } else {
              sum += (float)-0.41718695;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.062889174)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.0034345249)) {
            if (  (data[22].fvalue < 1.0158697)) {
              sum += (float)-0.26947516;
            } else {
              sum += (float)-0.076940686;
            }
          } else {
            if (  (data[18].fvalue < 0.023838803)) {
              sum += (float)0.4422338;
            } else {
              sum += (float)0.0029255059;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.41751611)) {
            if (  (data[22].fvalue < 0.37645954)) {
              sum += (float)0.010925206;
            } else {
              sum += (float)0.030982787;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.01069547;
            } else {
              sum += (float)0.09393806;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 6.3686147)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.0034345249)) {
              sum += (float)-0.14311972;
            } else {
              sum += (float)0.0071446151;
            }
          } else {
            if (  (data[11].fvalue < 0.46434611)) {
              sum += (float)0.031515889;
            } else {
              sum += (float)0.0063415407;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.023838803)) {
              sum += (float)0.0035686572;
            } else {
              sum += (float)-0.0054294062;
            }
          } else {
            if (  (data[21].fvalue < 1.9806685)) {
              sum += (float)0.017209658;
            } else {
              sum += (float)-0.0024735387;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 15.526923)) {
    if ( (data[9].fvalue < 6.183589)) {
      if (  (data[15].fvalue < 4.999999)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 1.5894406)) {
            if ( (data[9].fvalue < 3.5284832)) {
              sum += (float)-0.00016642705;
            } else {
              sum += (float)0.013550146;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.011782473;
            } else {
              sum += (float)0.0025468371;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.0523111)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.012065763;
            } else {
              sum += (float)-0.045273706;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.0032143921;
            } else {
              sum += (float)-0.023224188;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.54627007)) {
          if (  (data[20].fvalue < 2.8890398)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.12526883;
            } else {
              sum += (float)0.044891987;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.010006994;
            } else {
              sum += (float)0.10764386;
            }
          }
        } else {
          if (  (data[15].fvalue < 5.999999)) {
            if (  (data[20].fvalue < 0.092058659)) {
              sum += (float)0.40844551;
            } else {
              sum += (float)0.10966758;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.14534652;
            } else {
              sum += (float)0.35099462;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 6.6725183)) {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 6.832747)) {
              sum += (float)0.034831926;
            } else {
              sum += (float)-0.027697526;
            }
          } else {
            if (  (data[20].fvalue < 0.35786194)) {
              sum += (float)-0.020144088;
            } else {
              sum += (float)0.015389901;
            }
          }
        } else {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[20].fvalue < 0.021387782)) {
              sum += (float)-0.060737003;
            } else {
              sum += (float)0.0039317282;
            }
          } else {
            if (  (data[22].fvalue < 4.6394019)) {
              sum += (float)-0.05692381;
            } else {
              sum += (float)0.015717309;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 4.9508376)) {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.0098234266;
            } else {
              sum += (float)0.0057004336;
            }
          } else {
            if (  (data[19].fvalue < 0.84846091)) {
              sum += (float)0.01901163;
            } else {
              sum += (float)-0.040303927;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.15352507)) {
              sum += (float)0.031828057;
            } else {
              sum += (float)-0.02341775;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.099618435;
            } else {
              sum += (float)0.024548821;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[19].fvalue < 0.49198806)) {
        if (  (data[19].fvalue < 0.099544071)) {
          if (  (data[19].fvalue < 0.020079695)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.03750908;
            } else {
              sum += (float)-0.0074646398;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.015306701;
            } else {
              sum += (float)-0.066081159;
            }
          }
        } else {
          if (  (data[8].fvalue < 14.795654)) {
            if (  (data[9].fvalue < 5.1357155)) {
              sum += (float)0.10748436;
            } else {
              sum += (float)0.026444241;
            }
          } else {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)0.0071129841;
            } else {
              sum += (float)-0.054289311;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 12.03955)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.077141829;
            } else {
              sum += (float)-0.043392722;
            }
          } else {
            if (  (data[20].fvalue < 0.14454098)) {
              sum += (float)0.025679357;
            } else {
              sum += (float)-0.025825284;
            }
          }
        } else {
          if (  (data[20].fvalue < 1.5894406)) {
            if (  (data[9].fvalue < 7.2329273)) {
              sum += (float)0.15526702;
            } else {
              sum += (float)0.035213731;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.008004819;
            } else {
              sum += (float)0.095588095;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.073656343)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[8].fvalue < 14.795654)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.1358629;
            } else {
              sum += (float)0.027336581;
            }
          } else {
            if (  (data[19].fvalue < 0.050829265)) {
              sum += (float)0.33084971;
            } else {
              sum += (float)0.19254611;
            }
          }
        } else {
          if (  (data[8].fvalue < 12.03955)) {
            if (  (data[19].fvalue < 0.050829265)) {
              sum += (float)-0.59131724;
            } else {
              sum += (float)-0.24278046;
            }
          } else {
            sum += (float)0.053685647;
          }
        }
      } else {
        if (  (data[20].fvalue < 0.35786194)) {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.25003836)) {
              sum += (float)-0.001722994;
            } else {
              sum += (float)-0.044737954;
            }
          } else {
            if (  (data[20].fvalue < 0.11858462)) {
              sum += (float)0.1746818;
            } else {
              sum += (float)0.00010586361;
            }
          }
        } else {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.011061803;
            } else {
              sum += (float)0.051587135;
            }
          } else {
            if (  (data[8].fvalue < 24.681461)) {
              sum += (float)-0.010972301;
            } else {
              sum += (float)0.030670011;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 2.999999)) {
    if (  (data[12].fvalue < 7.999999)) {
      if (  (data[13].fvalue < 8.999999)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.00093995378;
            } else {
              sum += (float)0.007054748;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.092168689;
            } else {
              sum += (float)-0.066760786;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if ( (data[9].fvalue < 1.548823)) {
              sum += (float)0.021774177;
            } else {
              sum += (float)-0.057138938;
            }
          } else {
            if (  (data[19].fvalue < 0.28440723)) {
              sum += (float)-0.023750614;
            } else {
              sum += (float)0.03023701;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.89265972)) {
          if (  (data[19].fvalue < 0.46531886)) {
            if (  (data[9].fvalue < 2.3939669)) {
              sum += (float)0.017425792;
            } else {
              sum += (float)0.119619;
            }
          } else {
            if (  (data[9].fvalue < 1.3760586)) {
              sum += (float)0.13156681;
            } else {
              sum += (float)-0.026196057;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 1.7895162)) {
              sum += (float)-0.021525502;
            } else {
              sum += (float)-0.19201767;
            }
          } else {
            if ( (data[9].fvalue < 1.9400611)) {
              sum += (float)0.24009004;
            } else {
              sum += (float)-0.058608197;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 4.999999)) {
        if (  (data[19].fvalue < 0.19972858)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.17335162;
            } else {
              sum += (float)-0.36447957;
            }
          } else {
            if (  (data[19].fvalue < 0.05105624)) {
              sum += (float)0.19050243;
            } else {
              sum += (float)0.016762307;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.4355525)) {
            if (  (data[8].fvalue < 1.508549)) {
              sum += (float)0.30052766;
            } else {
              sum += (float)-0.08056204;
            }
          } else {
            if (  (data[19].fvalue < 0.46531886)) {
              sum += (float)-0.17128563;
            } else {
              sum += (float)-0.042979676;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.062881224)) {
            if ( (data[9].fvalue < 1.0396821)) {
              sum += (float)0.12027124;
            } else {
              sum += (float)-0.34998539;
            }
          } else {
            if (  (data[9].fvalue < 1.3219386)) {
              sum += (float)0.066552997;
            } else {
              sum += (float)-0.0022196388;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.24907617)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.22027333;
            } else {
              sum += (float)-0.028098803;
            }
          } else {
            if ( (data[9].fvalue < 2.0136685)) {
              sum += (float)0.19600143;
            } else {
              sum += (float)0.037229799;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 6.999999)) {
      if (  (data[13].fvalue < 3.999999)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.074377291;
            } else {
              sum += (float)-0.0028252441;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.024994036;
            } else {
              sum += (float)-0.033621237;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 5.0204759)) {
              sum += (float)-0.11013352;
            } else {
              sum += (float)-0.054230824;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.041109253;
            } else {
              sum += (float)-0.11146841;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[19].fvalue < 0.24907617)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.37972659;
            } else {
              sum += (float)0.18380567;
            }
          } else {
            if (  (data[19].fvalue < 1.3707325)) {
              sum += (float)0.11880898;
            } else {
              sum += (float)0.062321585;
            }
          }
        } else {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.00054371747;
            } else {
              sum += (float)-0.10684231;
            }
          } else {
            if (  (data[19].fvalue < 1.8930757)) {
              sum += (float)0.083595864;
            } else {
              sum += (float)-0.0032744235;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 7.999999)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.46531886)) {
              sum += (float)-0.24198377;
            } else {
              sum += (float)-0.17166445;
            }
          } else {
            if (  (data[19].fvalue < 0.46531886)) {
              sum += (float)-0.36462858;
            } else {
              sum += (float)-0.28322107;
            }
          }
        } else {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[19].fvalue < 1.3707325)) {
              sum += (float)-0.042329472;
            } else {
              sum += (float)-0.14534631;
            }
          } else {
            if (  (data[19].fvalue < 0.3578465)) {
              sum += (float)-0.057370644;
            } else {
              sum += (float)-0.20750719;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.54622948)) {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[9].fvalue < 2.8039587)) {
              sum += (float)-0.35835242;
            } else {
              sum += (float)-0.038594164;
            }
          } else {
            if (  (data[9].fvalue < 3.3416903)) {
              sum += (float)-0.61246645;
            } else {
              sum += (float)-0.20291565;
            }
          }
        } else {
          if (  (data[12].fvalue < 12.999999)) {
            if (  (data[19].fvalue < 2.0118678)) {
              sum += (float)0.061763305;
            } else {
              sum += (float)-0.025876366;
            }
          } else {
            if (  (data[13].fvalue < 18.999998)) {
              sum += (float)-0.11093896;
            } else {
              sum += (float)0.17331366;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 8.0678453)) {
    if (  (data[15].fvalue < 3.999999)) {
      if (  (data[22].fvalue < 4.9112377)) {
        if (  (data[11].fvalue < 0.84914833)) {
          if (  (data[13].fvalue < 13.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0027972199;
            } else {
              sum += (float)-0.00028918736;
            }
          } else {
            if (  (data[22].fvalue < 0.53812802)) {
              sum += (float)0.043405607;
            } else {
              sum += (float)-0.03044674;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.7587415)) {
              sum += (float)-0.025039259;
            } else {
              sum += (float)-0.0022485794;
            }
          } else {
            if (  (data[5].fvalue < 15.722701)) {
              sum += (float)0.00033707669;
            } else {
              sum += (float)0.024972415;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.017535932;
            } else {
              sum += (float)-0.0088318568;
            }
          } else {
            if (  (data[20].fvalue < 0.28832811)) {
              sum += (float)-0.010913631;
            } else {
              sum += (float)0.004117209;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.39720216)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.0085471058;
            } else {
              sum += (float)-0.05841567;
            }
          } else {
            if (  (data[20].fvalue < 0.1158648)) {
              sum += (float)0.11716484;
            } else {
              sum += (float)0.025783747;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 1.9007492)) {
        if (  (data[20].fvalue < 0.020438548)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 1.5463803)) {
              sum += (float)-0.072407924;
            } else {
              sum += (float)0.12974443;
            }
          } else {
            if (  (data[18].fvalue < 2.2612212)) {
              sum += (float)0.24460974;
            } else {
              sum += (float)0.12658529;
            }
          }
        } else {
          if (  (data[18].fvalue < 2.7582738)) {
            if (  (data[20].fvalue < 0.70508003)) {
              sum += (float)-0.027438326;
            } else {
              sum += (float)0.01346582;
            }
          } else {
            if (  (data[21].fvalue < 1.0431862)) {
              sum += (float)0.089593813;
            } else {
              sum += (float)-0.07535772;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 3.000999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[5].fvalue < 21.178059)) {
              sum += (float)-0.0078128437;
            } else {
              sum += (float)0.061578698;
            }
          } else {
            if (  (data[18].fvalue < 0.15046066)) {
              sum += (float)-0.13611615;
            } else {
              sum += (float)0.15658614;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.12431321)) {
              sum += (float)0.14498271;
            } else {
              sum += (float)0.051530689;
            }
          } else {
            if (  (data[18].fvalue < 3.5381866)) {
              sum += (float)0.031044321;
            } else {
              sum += (float)-0.043804362;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 0.39336967)) {
      if (  (data[22].fvalue < 0.23418362)) {
        if (  (data[13].fvalue < 7.999999)) {
          if (  (data[18].fvalue < 5.0525794)) {
            if (  (data[21].fvalue < 11.493921)) {
              sum += (float)-0.050138321;
            } else {
              sum += (float)-0.078433312;
            }
          } else {
            if (  (data[20].fvalue < 0.020438548)) {
              sum += (float)0.023809146;
            } else {
              sum += (float)-0.026492927;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.15046066)) {
            if (  (data[20].fvalue < 0.047858771)) {
              sum += (float)-0.19292939;
            } else {
              sum += (float)-0.055763096;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)0.035973869;
            } else {
              sum += (float)0.12729448;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.14954273)) {
          if (  (data[20].fvalue < 0.047858771)) {
            if (  (data[18].fvalue < 0.93564266)) {
              sum += (float)0.0059482865;
            } else {
              sum += (float)-0.021220349;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.011038661;
            } else {
              sum += (float)0.064782053;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0027247637;
            } else {
              sum += (float)-0.02707609;
            }
          } else {
            if (  (data[18].fvalue < 0.47089186)) {
              sum += (float)-0.090927131;
            } else {
              sum += (float)-0.036465682;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[18].fvalue < 0.2770344)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0343128;
            } else {
              sum += (float)0.0039534434;
            }
          } else {
            if (  (data[22].fvalue < 0.9457258)) {
              sum += (float)-0.027727732;
            } else {
              sum += (float)-0.00065156253;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.015705392)) {
            if (  (data[5].fvalue < 25.845785)) {
              sum += (float)-0.016877113;
            } else {
              sum += (float)0.11907328;
            }
          } else {
            if (  (data[18].fvalue < 0.15046066)) {
              sum += (float)-0.034842856;
            } else {
              sum += (float)0.035178501;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[18].fvalue < 0.015705392)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0093563013;
            } else {
              sum += (float)-0.040145814;
            }
          } else {
            if (  (data[18].fvalue < 0.58535302)) {
              sum += (float)0.018811375;
            } else {
              sum += (float)-0.0080329981;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.16715807)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.04940277;
            } else {
              sum += (float)-0.15471506;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)-0.014845515;
            } else {
              sum += (float)0.15613651;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 15.999999)) {
    if (  (data[9].fvalue < 0.99778092)) {
      if (  (data[19].fvalue < 0.0094259009)) {
        if (  (data[18].fvalue < 0.015425072)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.96873528)) {
              sum += (float)-0.047582634;
            } else {
              sum += (float)-0.17052731;
            }
          } else {
            if (  (data[18].fvalue < 0.002882109)) {
              sum += (float)-0.6094296;
            } else {
              sum += (float)0.13955553;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.11055576)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.12700367;
            } else {
              sum += (float)-0.09343262;
            }
          } else {
            if (  (data[18].fvalue < 0.087459847)) {
              sum += (float)-0.13666746;
            } else {
              sum += (float)-0.015008251;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.14871012)) {
          if (  (data[19].fvalue < 0.051111981)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.08465033;
            } else {
              sum += (float)-0.093909509;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.23935917;
            } else {
              sum += (float)0.68585086;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.098760359)) {
            if (  (data[18].fvalue < 0.11168665)) {
              sum += (float)-0.089645527;
            } else {
              sum += (float)0.090415068;
            }
          } else {
            if (  (data[8].fvalue < 0.27218574)) {
              sum += (float)0.19707967;
            } else {
              sum += (float)0.019245202;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 0.14871012)) {
        if (  (data[18].fvalue < 0.07547354)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 1.8655012)) {
              sum += (float)-0.034620304;
            } else {
              sum += (float)0.054182537;
            }
          } else {
            if (  (data[19].fvalue < 0.11238117)) {
              sum += (float)-0.30661011;
            } else {
              sum += (float)0.01170864;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0427505)) {
            if (  (data[8].fvalue < 0.070236541)) {
              sum += (float)0.38732943;
            } else {
              sum += (float)0.0017622481;
            }
          } else {
            if (  (data[19].fvalue < 0.23387814)) {
              sum += (float)-0.019064704;
            } else {
              sum += (float)0.037123036;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.020309351)) {
          if (  (data[21].fvalue < 0.31344247)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.034128871;
            } else {
              sum += (float)0.0033654852;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0014546491;
            } else {
              sum += (float)0.024433294;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.039208438)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.058652379;
            } else {
              sum += (float)0.090098307;
            }
          } else {
            if (  (data[18].fvalue < 0.098389529)) {
              sum += (float)0.0027416686;
            } else {
              sum += (float)-0.0027118521;
            }
          }
        }
      }
    }
  } else {
    if (  (data[15].fvalue < 1.999999)) {
      if (  (data[19].fvalue < 0.21626553)) {
        if (  (data[19].fvalue < 0.13882875)) {
          if (  (data[8].fvalue < 0.7267018)) {
            sum += (float)-0.55289876;
          } else {
            sum += (float)-0.47071385;
          }
        } else {
          if (  (data[8].fvalue < 1.509528)) {
            if (  (data[9].fvalue < 1.608768)) {
              sum += (float)-0.3129901;
            } else {
              sum += (float)-0.50607181;
            }
          } else {
            sum += (float)-0.24958254;
          }
        }
      } else {
        if (  (data[18].fvalue < 1.4904979)) {
          if (  (data[19].fvalue < 1.4806999)) {
            if (  (data[9].fvalue < 1.3253462)) {
              sum += (float)0.11630391;
            } else {
              sum += (float)-0.18511423;
            }
          } else {
            if (  (data[18].fvalue < 1.1408186)) {
              sum += (float)0.2577405;
            } else {
              sum += (float)0.047080543;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.595418)) {
            if (  (data[18].fvalue < 2.2451112)) {
              sum += (float)-0.12708499;
            } else {
              sum += (float)-0.025603063;
            }
          } else {
            if (  (data[18].fvalue < 2.049257)) {
              sum += (float)-0.18153191;
            } else {
              sum += (float)-0.43414277;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 3.999999)) {
        if (  (data[21].fvalue < 0.72536975)) {
          if (  (data[19].fvalue < 0.49054068)) {
            if (  (data[9].fvalue < 2.9742756)) {
              sum += (float)-0.28534609;
            } else {
              sum += (float)0.14375141;
            }
          } else {
            if (  (data[21].fvalue < 0.34832168)) {
              sum += (float)0.30238661;
            } else {
              sum += (float)0.1411107;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.54694825)) {
            if (  (data[18].fvalue < 1.6063153)) {
              sum += (float)-0.27911356;
            } else {
              sum += (float)-0.014874416;
            }
          } else {
            if (  (data[18].fvalue < 1.8569163)) {
              sum += (float)0.074669831;
            } else {
              sum += (float)-0.072575346;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 3.0290704)) {
          if (  (data[18].fvalue < 0.82856762)) {
            sum += (float)-0.081442982;
          } else {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.24482523;
            } else {
              sum += (float)0.38307723;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.595418)) {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)0.20374064;
            } else {
              sum += (float)-0.0052362131;
            }
          } else {
            if (  (data[8].fvalue < 7.9099936)) {
              sum += (float)-0.19685847;
            } else {
              sum += (float)0.067154616;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.030944705)) {
    if (  (data[9].fvalue < 0.96941102)) {
      if (  (data[19].fvalue < 0.051476147)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.018497452)) {
            if (  (data[11].fvalue < 0.20106991)) {
              sum += (float)-0.57831216;
            } else {
              sum += (float)0.061549511;
            }
          } else {
            if (  (data[10].fvalue < 0.24618384)) {
              sum += (float)0.7790035;
            } else {
              sum += (float)0.32400236;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.26172906)) {
            if (  (data[19].fvalue < 0.00896772)) {
              sum += (float)-0.39202106;
            } else {
              sum += (float)-1.0574203;
            }
          } else {
            if (  (data[1].fvalue < 0.99999899)) {
              sum += (float)0.27699813;
            } else {
              sum += (float)-0.16185148;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.074057914)) {
          sum += (float)-1.6977452;
        } else {
          sum += (float)-2.7085147;
        }
      }
    } else {
      if (  (data[19].fvalue < 0.019939326)) {
        if (  (data[9].fvalue < 1.1821265)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.99884558)) {
              sum += (float)-0.030783331;
            } else {
              sum += (float)-0.12569614;
            }
          } else {
            if (  (data[11].fvalue < 0.31352645)) {
              sum += (float)-0.04091879;
            } else {
              sum += (float)0.10101955;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.2246578)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.21364737;
            } else {
              sum += (float)-0.13864663;
            }
          } else {
            if (  (data[9].fvalue < 1.7898797)) {
              sum += (float)-0.021289151;
            } else {
              sum += (float)0.051303752;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.0424691)) {
          if (  (data[19].fvalue < 0.06214771)) {
            if (  (data[9].fvalue < 0.99884558)) {
              sum += (float)0.14175701;
            } else {
              sum += (float)0.03533753;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.3104389;
            } else {
              sum += (float)1.0614461;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.5473604)) {
            if (  (data[19].fvalue < 0.33875757)) {
              sum += (float)-0.023764355;
            } else {
              sum += (float)0.12602672;
            }
          } else {
            if (  (data[19].fvalue < 0.18551745)) {
              sum += (float)-0.0048095356;
            } else {
              sum += (float)-0.11473338;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.1821265)) {
      if (  (data[19].fvalue < 0.23453139)) {
        if ( (data[9].fvalue < 0.96941102)) {
          if (  (data[8].fvalue < 0.3152138)) {
            if (  (data[19].fvalue < 0.12547807)) {
              sum += (float)0.026873663;
            } else {
              sum += (float)0.35578692;
            }
          } else {
            if (  (data[19].fvalue < 0.15335163)) {
              sum += (float)-0.047212899;
            } else {
              sum += (float)3.4480614e-05;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.11785509)) {
            if (  (data[8].fvalue < 0.92506301)) {
              sum += (float)-0.033191301;
            } else {
              sum += (float)0.01347624;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.21058734;
            } else {
              sum += (float)-0.088181913;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.78978962)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.0424691)) {
              sum += (float)0.076612338;
            } else {
              sum += (float)-0.043903746;
            }
          } else {
            if (  (data[8].fvalue < 0.45490792)) {
              sum += (float)0.29753822;
            } else {
              sum += (float)0.104515;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.40029594)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.067086764;
            } else {
              sum += (float)-0.20356126;
            }
          } else {
            if (  (data[8].fvalue < 1.4143466)) {
              sum += (float)0.062265348;
            } else {
              sum += (float)-0.024778051;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.18551745)) {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[19].fvalue < 0.06214771)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.00035175259;
            } else {
              sum += (float)0.045799576;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.025211897;
            } else {
              sum += (float)-0.024215246;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.025610503)) {
            if (  (data[20].fvalue < 0.55099869)) {
              sum += (float)-0.065700367;
            } else {
              sum += (float)-0.19694398;
            }
          } else {
            if (  (data[20].fvalue < 0.11785509)) {
              sum += (float)0.050564066;
            } else {
              sum += (float)-0.040816974;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 0.3152138)) {
            if (  (data[19].fvalue < 0.49266705)) {
              sum += (float)-0.042855382;
            } else {
              sum += (float)-0.013016449;
            }
          } else {
            if (  (data[20].fvalue < 0.35591221)) {
              sum += (float)-0.010227899;
            } else {
              sum += (float)0.0011028078;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.3806549)) {
            if (  (data[8].fvalue < 0.55400968)) {
              sum += (float)0.12199273;
            } else {
              sum += (float)-0.021710282;
            }
          } else {
            if (  (data[21].fvalue < 0.025610503)) {
              sum += (float)-0.01215984;
            } else {
              sum += (float)0.0075978171;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 0.83560491)) {
    if (  (data[0].fvalue < 0.99999899)) {
      if (  (data[12].fvalue < 8.999999)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[21].fvalue < 0.38080555)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.0030638399;
            } else {
              sum += (float)-0.0091069201;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0068466212;
            } else {
              sum += (float)0.034736525;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.083398953)) {
            if (  (data[8].fvalue < 1.3257307)) {
              sum += (float)-0.24919724;
            } else {
              sum += (float)-0.13243249;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.043812908;
            } else {
              sum += (float)-0.028269319;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 0.11832555)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.043075711;
            } else {
              sum += (float)0.19653586;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.045014095;
            } else {
              sum += (float)0.13283426;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 12.999999)) {
              sum += (float)-0.090759471;
            } else {
              sum += (float)-0.220273;
            }
          } else {
            if (  (data[20].fvalue < 0.021322679)) {
              sum += (float)0.1224774;
            } else {
              sum += (float)-0.055630408;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 5.999999)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.02024102;
            } else {
              sum += (float)-0.0010872484;
            }
          } else {
            if (  (data[8].fvalue < 0.11258709)) {
              sum += (float)0.083119445;
            } else {
              sum += (float)-0.039592426;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.15238225)) {
            if (  (data[5].fvalue < 0.72300988)) {
              sum += (float)-0.063536756;
            } else {
              sum += (float)-0.01505375;
            }
          } else {
            if (  (data[5].fvalue < 8.7994986)) {
              sum += (float)-0.00584814;
            } else {
              sum += (float)0.0038928168;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.86339778)) {
            if (  (data[22].fvalue < 0.022944188)) {
              sum += (float)-0.0015998845;
            } else {
              sum += (float)0.056176823;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.093469881;
            } else {
              sum += (float)-0.017825453;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.024338227)) {
            if (  (data[20].fvalue < 0.021322679)) {
              sum += (float)0.46813157;
            } else {
              sum += (float)0.29973388;
            }
          } else {
            if (  (data[20].fvalue < 0.021322679)) {
              sum += (float)0.14855751;
            } else {
              sum += (float)0.028125972;
            }
          }
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 0.15795119)) {
      if (  (data[12].fvalue < 8.999999)) {
        if (  (data[8].fvalue < 0.86339778)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 0.021322679)) {
              sum += (float)-0.0062513528;
            } else {
              sum += (float)0.046363235;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.060773246;
            } else {
              sum += (float)0.0021734568;
            }
          }
        } else {
          if (  (data[8].fvalue < 4.461761)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0079799304;
            } else {
              sum += (float)-0.0056176698;
            }
          } else {
            if (  (data[21].fvalue < 3.3967302)) {
              sum += (float)-0.02459421;
            } else {
              sum += (float)0.033181876;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 1.8612585)) {
          if (  (data[8].fvalue < 1.7292063)) {
            if (  (data[20].fvalue < 0.092805982)) {
              sum += (float)0.00025195934;
            } else {
              sum += (float)-0.093510285;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.20499636;
            } else {
              sum += (float)-0.05594366;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.14744355)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.030890087;
            } else {
              sum += (float)0.25509748;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.087490462;
            } else {
              sum += (float)0.020536434;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 36.879173)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.72819185)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.011194326;
            } else {
              sum += (float)0.034958385;
            }
          } else {
            if (  (data[11].fvalue < 0.51495463)) {
              sum += (float)0.00085472281;
            } else {
              sum += (float)-0.0062982026;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.050194781)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.0083074961;
            } else {
              sum += (float)-0.044051897;
            }
          } else {
            if (  (data[20].fvalue < 0.14744355)) {
              sum += (float)0.021665819;
            } else {
              sum += (float)-0.0064902948;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.0039488897;
            } else {
              sum += (float)0.0094706183;
            }
          } else {
            if (  (data[20].fvalue < 0.021322679)) {
              sum += (float)-0.12757193;
            } else {
              sum += (float)-0.0063308869;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 0.54887593)) {
              sum += (float)0.0061334819;
            } else {
              sum += (float)-0.039335296;
            }
          } else {
            if (  (data[20].fvalue < 0.14744355)) {
              sum += (float)0.14012165;
            } else {
              sum += (float)0.033702791;
            }
          }
        }
      }
    }
  }
  if (  (data[10].fvalue < 1.0588498)) {
    if (  (data[10].fvalue < 0.45704979)) {
      if (  (data[21].fvalue < 0.15102972)) {
        if (  (data[19].fvalue < 0.31967869)) {
          if (  (data[21].fvalue < 0.050275665)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.0015962257;
            } else {
              sum += (float)-0.0336993;
            }
          } else {
            if (  (data[19].fvalue < 0.086310051)) {
              sum += (float)0.001539377;
            } else {
              sum += (float)0.03245987;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.011463)) {
            if (  (data[19].fvalue < 0.40012345)) {
              sum += (float)0.036702812;
            } else {
              sum += (float)0.27943245;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.0072945245;
            } else {
              sum += (float)0.061496511;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.63898373)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.044998005)) {
              sum += (float)0.015764106;
            } else {
              sum += (float)-0.001784335;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.017918101;
            } else {
              sum += (float)0.0036465451;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.3081553)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.010237398;
            } else {
              sum += (float)0.018336093;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.0055309837;
            } else {
              sum += (float)0.015321459;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.94497502)) {
        if (  (data[19].fvalue < 0.0289919)) {
          if (  (data[11].fvalue < 0.017960323)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.077742279;
            } else {
              sum += (float)0.0036579375;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.0090012802;
            } else {
              sum += (float)-0.0065430338;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.10411409)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.016048769;
            } else {
              sum += (float)0.0063555385;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.023408616;
            } else {
              sum += (float)0.021094153;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.55120242)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.030166617;
            } else {
              sum += (float)0.034096919;
            }
          } else {
            if (  (data[21].fvalue < 0.10411409)) {
              sum += (float)0.14345442;
            } else {
              sum += (float)0.034962315;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.0028926914;
            } else {
              sum += (float)0.14683595;
            }
          } else {
            if (  (data[21].fvalue < 2.2459557)) {
              sum += (float)0.040939614;
            } else {
              sum += (float)0.0044021779;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.2677727)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[11].fvalue < 0.011463)) {
          if (  (data[19].fvalue < 0.1265699)) {
            if (  (data[19].fvalue < 0.0289919)) {
              sum += (float)-0.029830623;
            } else {
              sum += (float)0.028557576;
            }
          } else {
            if (  (data[19].fvalue < 0.20059446)) {
              sum += (float)0.092226163;
            } else {
              sum += (float)0.13447002;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.063047186)) {
            if (  (data[19].fvalue < 0.1265699)) {
              sum += (float)-0.040556122;
            } else {
              sum += (float)-0.017060449;
            }
          } else {
            if (  (data[19].fvalue < 0.073700048)) {
              sum += (float)0.010362471;
            } else {
              sum += (float)-0.013050332;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.039226286)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.07877101)) {
              sum += (float)-0.0031083247;
            } else {
              sum += (float)0.025731878;
            }
          } else {
            if (  (data[20].fvalue < 0.32837108)) {
              sum += (float)-0.34587112;
            } else {
              sum += (float)-0.16727944;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.011463)) {
            if (  (data[19].fvalue < 0.099740647)) {
              sum += (float)0.071193583;
            } else {
              sum += (float)0.095907234;
            }
          } else {
            if (  (data[19].fvalue < 0.1700016)) {
              sum += (float)0.042112734;
            } else {
              sum += (float)-0.011535602;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[11].fvalue < 0.011463)) {
          if (  (data[19].fvalue < 0.37646818)) {
            if (  (data[19].fvalue < 0.35775137)) {
              sum += (float)0.18033974;
            } else {
              sum += (float)0.25121242;
            }
          } else {
            if (  (data[19].fvalue < 0.44059867)) {
              sum += (float)0.31117204;
            } else {
              sum += (float)0.38917625;
            }
          }
        } else {
          if (  (data[19].fvalue < 6.8679843)) {
            if (  (data[21].fvalue < 0.083284512)) {
              sum += (float)0.059559491;
            } else {
              sum += (float)-0.020455593;
            }
          } else {
            sum += (float)0.43823007;
          }
        }
      } else {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.42073113)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.0085020708;
            } else {
              sum += (float)0.23474167;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.091944784;
            } else {
              sum += (float)0.02754003;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.75721347)) {
            if (  (data[20].fvalue < 0.11861373)) {
              sum += (float)0.29493013;
            } else {
              sum += (float)0.1707802;
            }
          } else {
            sum += (float)-0.047819573;
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 12.999999)) {
    if (  (data[12].fvalue < 8.999999)) {
      if (  (data[18].fvalue < 0.93736881)) {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0029802348;
            } else {
              sum += (float)0.0074991575;
            }
          } else {
            if (  (data[20].fvalue < 0.020158213)) {
              sum += (float)-0.041402835;
            } else {
              sum += (float)0.0054744263;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.51716101)) {
            if (  (data[18].fvalue < 0.27907526)) {
              sum += (float)-0.16224612;
            } else {
              sum += (float)0.032660924;
            }
          } else {
            if (  (data[18].fvalue < 0.29637158)) {
              sum += (float)0.27229753;
            } else {
              sum += (float)-0.0043682847;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.39930493)) {
              sum += (float)0.0055030864;
            } else {
              sum += (float)0.17681774;
            }
          } else {
            if (  (data[19].fvalue < 0.35767233)) {
              sum += (float)-0.029767506;
            } else {
              sum += (float)-0.11948694;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.086299323)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.19305703;
            } else {
              sum += (float)0.35088614;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.027962821;
            } else {
              sum += (float)-0.016080989;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 1.0007063)) {
        if (  (data[19].fvalue < 0.51716101)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.0010469556;
            } else {
              sum += (float)0.092434019;
            }
          } else {
            if (  (data[18].fvalue < 0.47162211)) {
              sum += (float)-0.28246915;
            } else {
              sum += (float)-0.057192396;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.44555178)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.38224027;
            } else {
              sum += (float)0.1980484;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)0.021085108;
            } else {
              sum += (float)0.10909998;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.75517499)) {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[19].fvalue < 0.26771548)) {
              sum += (float)0.031135315;
            } else {
              sum += (float)-0.14206629;
            }
          } else {
            if (  (data[19].fvalue < 0.37733191)) {
              sum += (float)0.32508725;
            } else {
              sum += (float)0.0997511;
            }
          }
        } else {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[18].fvalue < 1.4941858)) {
              sum += (float)-0.12947586;
            } else {
              sum += (float)-0.21625845;
            }
          } else {
            if (  (data[19].fvalue < 1.3080961)) {
              sum += (float)-0.010553969;
            } else {
              sum += (float)-0.12647381;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.5751856)) {
      if (  (data[18].fvalue < 0.88228482)) {
        if (  (data[12].fvalue < 9.999999)) {
          if (  (data[18].fvalue < 0.3168191)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.0038258005;
            } else {
              sum += (float)-0.27049631;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)0.41056865;
            } else {
              sum += (float)0.15860096;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.35599834)) {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)-0.49552551;
            } else {
              sum += (float)-0.29923138;
            }
          } else {
            if (  (data[19].fvalue < 0.37733191)) {
              sum += (float)-0.054885063;
            } else {
              sum += (float)-0.24225745;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 12.999999)) {
          if (  (data[19].fvalue < 0.37733191)) {
            if (  (data[22].fvalue < 0.30818656)) {
              sum += (float)0.33624732;
            } else {
              sum += (float)0.6655705;
            }
          } else {
            if (  (data[18].fvalue < 1.3840066)) {
              sum += (float)0.33639252;
            } else {
              sum += (float)0.44871759;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.4941858)) {
            if (  (data[19].fvalue < 0.21818832)) {
              sum += (float)-0.56336272;
            } else {
              sum += (float)0.001310077;
            }
          } else {
            if (  (data[18].fvalue < 7.64259)) {
              sum += (float)0.33024135;
            } else {
              sum += (float)-0.22646277;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.30818656)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[18].fvalue < 0.3168191)) {
              sum += (float)0.060822584;
            } else {
              sum += (float)-0.08005251;
            }
          } else {
            if (  (data[18].fvalue < 0.077431962)) {
              sum += (float)-0.55633104;
            } else {
              sum += (float)0.083758011;
            }
          }
        } else {
          if (  (data[12].fvalue < 10.999999)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.019939844;
            } else {
              sum += (float)0.21015236;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.32057396;
            } else {
              sum += (float)0.45213675;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[18].fvalue < 0.37854674)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.09118437;
            } else {
              sum += (float)0.0506845;
            }
          } else {
            if (  (data[19].fvalue < 1.7416577)) {
              sum += (float)-0.097420029;
            } else {
              sum += (float)-0.2682775;
            }
          }
        } else {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.69708002)) {
              sum += (float)0.032709468;
            } else {
              sum += (float)-0.027449669;
            }
          } else {
            if (  (data[22].fvalue < 1.5451623)) {
              sum += (float)0.20183688;
            } else {
              sum += (float)0.030283766;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 17.999998)) {
    if (  (data[21].fvalue < 0.10320821)) {
      if (  (data[18].fvalue < 0.93277717)) {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.0081128711;
            } else {
              sum += (float)0.0021986214;
            }
          } else {
            if (  (data[18].fvalue < 0.0028197626)) {
              sum += (float)0.012275182;
            } else {
              sum += (float)-0.041180484;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.023061957)) {
            if (  (data[11].fvalue < 0.012306822)) {
              sum += (float)0.1852739;
            } else {
              sum += (float)0.0057438202;
            }
          } else {
            if (  (data[18].fvalue < 0.13700832)) {
              sum += (float)-0.12956624;
            } else {
              sum += (float)0.11795484;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[21].fvalue < 0.023061957)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.046605371;
            } else {
              sum += (float)-0.00091616745;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.023247695;
            } else {
              sum += (float)-0.033229049;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.023061957)) {
            if (  (data[11].fvalue < 0.038155869)) {
              sum += (float)0.14434774;
            } else {
              sum += (float)-0.053088542;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.055054568;
            } else {
              sum += (float)-0.029003508;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.1820697)) {
        if (  (data[12].fvalue < 7.999999)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[21].fvalue < 1.2700592)) {
              sum += (float)0.0086543066;
            } else {
              sum += (float)-0.0043292707;
            }
          } else {
            if (  (data[18].fvalue < 0.023777112)) {
              sum += (float)-0.32260805;
            } else {
              sum += (float)0.056158256;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.087233588)) {
            if (  (data[18].fvalue < 0.063975617)) {
              sum += (float)-0.72065115;
            } else {
              sum += (float)-0.47305474;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.11495231;
            } else {
              sum += (float)-0.3297568;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.34860289)) {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0085542845;
            } else {
              sum += (float)-0.0386407;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.030295135;
            } else {
              sum += (float)-0.037743542;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.018664878;
            } else {
              sum += (float)0.024072997;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0023531152;
            } else {
              sum += (float)-0.0080147404;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 1.999999)) {
      if (  (data[21].fvalue < 0.023061957)) {
        if (  (data[10].fvalue < 0.71512759)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)-0.038058285;
            } else {
              sum += (float)-0.19004628;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.31700221;
            } else {
              sum += (float)-0.143163;
            }
          }
        } else {
          sum += (float)-0.36886162;
        }
      } else {
        if (  (data[12].fvalue < 18.999998)) {
          if (  (data[11].fvalue < 0.41601753)) {
            if (  (data[21].fvalue < 0.18193634)) {
              sum += (float)0.10064293;
            } else {
              sum += (float)-0.061289016;
            }
          } else {
            if (  (data[18].fvalue < 1.1388479)) {
              sum += (float)0.13159394;
            } else {
              sum += (float)0.026154222;
            }
          }
        } else {
          if (  (data[16].fvalue < 3.999999)) {
            if (  (data[10].fvalue < 0.62483156)) {
              sum += (float)-0.097858854;
            } else {
              sum += (float)0.15876813;
            }
          } else {
            if (  (data[11].fvalue < 0.63076562)) {
              sum += (float)0.03356849;
            } else {
              sum += (float)0.13349967;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 4.999999)) {
        if (  (data[18].fvalue < 1.6004008)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[11].fvalue < 0.82484806)) {
              sum += (float)-0.25829095;
            } else {
              sum += (float)-0.059037454;
            }
          } else {
            sum += (float)-0.42762148;
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 3.0076134)) {
              sum += (float)-0.3322472;
            } else {
              sum += (float)-0.22923233;
            }
          } else {
            if (  (data[10].fvalue < 0.084090397)) {
              sum += (float)-0.21118307;
            } else {
              sum += (float)-0.10328359;
            }
          }
        }
      } else {
        sum += (float)0.093287788;
      }
    }
  }
  if (  (data[9].fvalue < 1.6662719)) {
    if (  (data[21].fvalue < 0.02527253)) {
      if (  (data[14].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.46625811;
            } else {
              sum += (float)-0.25156474;
            }
          } else {
            if (  (data[9].fvalue < 0.99999267)) {
              sum += (float)0.019844439;
            } else {
              sum += (float)-0.03360961;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.1188014)) {
              sum += (float)0.0047231209;
            } else {
              sum += (float)-0.07083118;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0032842169;
            } else {
              sum += (float)0.090259708;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.29764426)) {
              sum += (float)-0.0021678861;
            } else {
              sum += (float)-0.046831734;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.06680046;
            } else {
              sum += (float)0.12650013;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.35455295)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.098618969;
            } else {
              sum += (float)0.027963851;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.16470817;
            } else {
              sum += (float)0.31212404;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[9].fvalue < 1.1188014)) {
            if (  (data[9].fvalue < 1.0446749)) {
              sum += (float)-0.11741451;
            } else {
              sum += (float)-0.060141962;
            }
          } else {
            if (  (data[9].fvalue < 1.3252524)) {
              sum += (float)0.021430146;
            } else {
              sum += (float)-0.017577063;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.014532793;
            } else {
              sum += (float)-0.20811898;
            }
          } else {
            if (  (data[9].fvalue < 1.4980677)) {
              sum += (float)0.15914246;
            } else {
              sum += (float)0.093689047;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.2124514)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.1562763;
            } else {
              sum += (float)0.089908518;
            }
          } else {
            if (  (data[18].fvalue < 0.064213082)) {
              sum += (float)-0.18168524;
            } else {
              sum += (float)-0.040054273;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.62151915)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.078228675;
            } else {
              sum += (float)0.15296222;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.054967739;
            } else {
              sum += (float)-0.119568;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.18185493)) {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[18].fvalue < 0.023926541)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.0076147909;
            } else {
              sum += (float)0.010374099;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.013019141;
            } else {
              sum += (float)0.15230206;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.0036949415;
            } else {
              sum += (float)-0.029092442;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.088379964;
            } else {
              sum += (float)0.0071879537;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[18].fvalue < 0.12532476)) {
            if (  (data[21].fvalue < 0.6664589)) {
              sum += (float)0.30123177;
            } else {
              sum += (float)0.18311988;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.047186214;
            } else {
              sum += (float)0.27408159;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.53441542)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)0.071587957;
            } else {
              sum += (float)-0.090918012;
            }
          } else {
            if (  (data[9].fvalue < 2.5580318)) {
              sum += (float)-0.13934164;
            } else {
              sum += (float)-0.030950632;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 2.9795921)) {
        if (  (data[21].fvalue < 1.7561225)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.02527253)) {
              sum += (float)0.0077888463;
            } else {
              sum += (float)-0.013028814;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.047351699;
            } else {
              sum += (float)-0.0099650398;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0025758946;
            } else {
              sum += (float)0.04138371;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.069373079;
            } else {
              sum += (float)0.011808435;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.084508374)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.092593186;
            } else {
              sum += (float)-0.037520476;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.0052038804;
            } else {
              sum += (float)0.046008717;
            }
          }
        } else {
          if (  (data[12].fvalue < 7.999999)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.0032701073;
            } else {
              sum += (float)-0.054376274;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.031308066;
            } else {
              sum += (float)-0.017234191;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.86152047)) {
    if (  (data[19].fvalue < 0.073154308)) {
      if (  (data[9].fvalue < 1.9421518)) {
        if (  (data[8].fvalue < 0.03131035)) {
          if (  (data[9].fvalue < 0.97029734)) {
            if (  (data[19].fvalue < 0.028954707)) {
              sum += (float)0.24808343;
            } else {
              sum += (float)-0.32939383;
            }
          } else {
            if (  (data[19].fvalue < 0.028954707)) {
              sum += (float)-0.01255145;
            } else {
              sum += (float)0.034099597;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.55771995)) {
            if (  (data[9].fvalue < 1.0434575)) {
              sum += (float)-0.023042589;
            } else {
              sum += (float)-0.011264076;
            }
          } else {
            if (  (data[19].fvalue < 0.020356141)) {
              sum += (float)0.0070447628;
            } else {
              sum += (float)-0.018322028;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.23148546)) {
          if (  (data[20].fvalue < 1.3758334)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.075234726;
            } else {
              sum += (float)0.034076061;
            }
          } else {
            if (  (data[19].fvalue < 0.0095118508)) {
              sum += (float)-0.018520575;
            } else {
              sum += (float)0.047990698;
            }
          }
        } else {
          if (  (data[20].fvalue < 2.5788484)) {
            if (  (data[21].fvalue < 0.3135089)) {
              sum += (float)0.017828222;
            } else {
              sum += (float)0.0053359522;
            }
          } else {
            if (  (data[21].fvalue < 0.024364661)) {
              sum += (float)-0.088555098;
            } else {
              sum += (float)-0.0064262156;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.3834983)) {
        if (  (data[19].fvalue < 0.57695824)) {
          if (  (data[8].fvalue < 0.1913271)) {
            if (  (data[9].fvalue < 0.99788791)) {
              sum += (float)0.21982211;
            } else {
              sum += (float)0.039509211;
            }
          } else {
            if (  (data[19].fvalue < 0.20120034)) {
              sum += (float)-0.019872788;
            } else {
              sum += (float)0.022098655;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.36090395)) {
            if (  (data[19].fvalue < 0.64037335)) {
              sum += (float)0.40177682;
            } else {
              sum += (float)0.67405367;
            }
          } else {
            if (  (data[19].fvalue < 0.7535156)) {
              sum += (float)0.11545949;
            } else {
              sum += (float)0.39630058;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.15522709)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.4557935)) {
              sum += (float)0.025512753;
            } else {
              sum += (float)0.048434723;
            }
          } else {
            if (  (data[21].fvalue < 0.12552293)) {
              sum += (float)-0.21422456;
            } else {
              sum += (float)-0.0014661306;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.32866788)) {
            if (  (data[21].fvalue < 0.28175724)) {
              sum += (float)-0.023199324;
            } else {
              sum += (float)0.010523042;
            }
          } else {
            if (  (data[9].fvalue < 2.1576674)) {
              sum += (float)0.035409585;
            } else {
              sum += (float)-0.0047092545;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.020356141)) {
      if (  (data[21].fvalue < 0.06480062)) {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 0.020493697)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.032602105;
            } else {
              sum += (float)-0.083682984;
            }
          } else {
            if ( (data[9].fvalue < 1.3834983)) {
              sum += (float)0.075560041;
            } else {
              sum += (float)-0.03630954;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0434575)) {
            if (  (data[8].fvalue < 1.9625542)) {
              sum += (float)-0.24743444;
            } else {
              sum += (float)-0.12461573;
            }
          } else {
            if ( (data[9].fvalue < 3.1601756)) {
              sum += (float)-0.025763202;
            } else {
              sum += (float)-0.0075841993;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.3135089)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.12552293)) {
              sum += (float)-0.0022293425;
            } else {
              sum += (float)0.030349771;
            }
          } else {
            if (  (data[19].fvalue < 0.0095118508)) {
              sum += (float)0.073724806;
            } else {
              sum += (float)-0.026945531;
            }
          }
        } else {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 0.96630865)) {
              sum += (float)0.0073665292;
            } else {
              sum += (float)-0.0037720602;
            }
          } else {
            if (  (data[9].fvalue < 4.8497596)) {
              sum += (float)0.033107549;
            } else {
              sum += (float)0.0097402306;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.4404984)) {
        if (  (data[19].fvalue < 0.57695824)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 1.2336248)) {
              sum += (float)-0.032222506;
            } else {
              sum += (float)-0.068465993;
            }
          } else {
            if (  (data[20].fvalue < 0.96630865)) {
              sum += (float)-0.015837738;
            } else {
              sum += (float)-0.17894171;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.7292387)) {
            if (  (data[9].fvalue < 1.1238766)) {
              sum += (float)0.10321514;
            } else {
              sum += (float)0.024869733;
            }
          } else {
            if (  (data[19].fvalue < 0.84415776)) {
              sum += (float)-0.09230952;
            } else {
              sum += (float)0.0067089098;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.49164933)) {
            if (  (data[19].fvalue < 0.29876569)) {
              sum += (float)0.0068724775;
            } else {
              sum += (float)0.025739374;
            }
          } else {
            if (  (data[19].fvalue < 1.2991951)) {
              sum += (float)-0.0069873054;
            } else {
              sum += (float)0.0031065859;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.15522709)) {
            if (  (data[8].fvalue < 2.4827781)) {
              sum += (float)-0.026164254;
            } else {
              sum += (float)-0.0080837449;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.0045143813;
            } else {
              sum += (float)0.0017574243;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.015513585)) {
    if (  (data[19].fvalue < 0.10016205)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 1.4511689)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.0033880014;
            } else {
              sum += (float)0.037788428;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.065586694;
            } else {
              sum += (float)-0.010949575;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.0028615454)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0089458348;
            } else {
              sum += (float)-0.24001585;
            }
          } else {
            if (  (data[19].fvalue < 0.009783837)) {
              sum += (float)-0.068039462;
            } else {
              sum += (float)0.14775105;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.009783837)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.35392258;
            } else {
              sum += (float)0.12551609;
            }
          } else {
            if (  (data[18].fvalue < 0.0028615454)) {
              sum += (float)-0.72762239;
            } else {
              sum += (float)-0.88453031;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.0028615454)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.12346066;
            } else {
              sum += (float)0.36903825;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.083400942;
            } else {
              sum += (float)-0.33453166;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.0049190009;
            } else {
              sum += (float)-0.042889562;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.049756773;
            } else {
              sum += (float)0.076167025;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.44008386)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.090685561;
            } else {
              sum += (float)0.031465825;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.035624448;
            } else {
              sum += (float)0.16911219;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.21773295)) {
          if (  (data[19].fvalue < 0.12734728)) {
            sum += (float)0.36936739;
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.32754737;
            } else {
              sum += (float)-0.057357393;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.35894507)) {
              sum += (float)-0.93832695;
            } else {
              sum += (float)-1.5107554;
            }
          } else {
            if (  (data[19].fvalue < 0.46557683)) {
              sum += (float)-0.54461628;
            } else {
              sum += (float)-0.86074978;
            }
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 1.999999)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.010172738;
            } else {
              sum += (float)-0.0046802023;
            }
          } else {
            if (  (data[19].fvalue < 0.12734728)) {
              sum += (float)0.10129347;
            } else {
              sum += (float)0.012217605;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.94613409)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.024371704;
            } else {
              sum += (float)0.027841315;
            }
          } else {
            if (  (data[18].fvalue < 0.054232646)) {
              sum += (float)0.07257764;
            } else {
              sum += (float)-0.070853606;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.28055385)) {
              sum += (float)0.16970572;
            } else {
              sum += (float)0.0060615889;
            }
          } else {
            if (  (data[19].fvalue < 0.20189366)) {
              sum += (float)-0.066323012;
            } else {
              sum += (float)-0.017984373;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.064373724)) {
            if (  (data[19].fvalue < 0.67613679)) {
              sum += (float)-0.18724801;
            } else {
              sum += (float)0.22907655;
            }
          } else {
            if (  (data[22].fvalue < 0.096490972)) {
              sum += (float)0.016668761;
            } else {
              sum += (float)-0.0039118924;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[22].fvalue < 0.88379264)) {
          if (  (data[19].fvalue < 0.009783837)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.1111936;
            } else {
              sum += (float)-0.091806091;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.071248725;
            } else {
              sum += (float)-0.2205462;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.0057737976;
            } else {
              sum += (float)0.0050539174;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.10575433;
            } else {
              sum += (float)-0.032936424;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.23353525)) {
          if (  (data[22].fvalue < 0.071489148)) {
            if (  (data[22].fvalue < 0.05371926)) {
              sum += (float)-0.11018963;
            } else {
              sum += (float)0.047428716;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.11381564;
            } else {
              sum += (float)0.22288859;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.21357298)) {
            if (  (data[19].fvalue < 0.33776876)) {
              sum += (float)-0.30583712;
            } else {
              sum += (float)0.1929388;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.021489836;
            } else {
              sum += (float)0.013258911;
            }
          }
        }
      }
    }
  }
  if (  (data[10].fvalue < 1.0523143)) {
    if (  (data[22].fvalue < 0.34694687)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[22].fvalue < 0.15587614)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.025974041)) {
              sum += (float)-0.000574712;
            } else {
              sum += (float)0.02409365;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.025061073;
            } else {
              sum += (float)-0.00042125554;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.23692091)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.02561307;
            } else {
              sum += (float)-0.063070834;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.00044424756;
            } else {
              sum += (float)-0.046059933;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.09605632)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[22].fvalue < 0.054447688)) {
              sum += (float)-0.25133479;
            } else {
              sum += (float)-0.094014451;
            }
          } else {
            if (  (data[10].fvalue < 0.36050522)) {
              sum += (float)-0.0034570934;
            } else {
              sum += (float)0.17770928;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.31570634;
            } else {
              sum += (float)-0.058087092;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.00074743148;
            } else {
              sum += (float)0.091153435;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.53813004)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.012631832;
            } else {
              sum += (float)-0.02018149;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.059688758;
            } else {
              sum += (float)0.013286819;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0051930086;
            } else {
              sum += (float)-0.093249932;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.036119357;
            } else {
              sum += (float)0.11198623;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 8.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 1.555611)) {
              sum += (float)0.0090124765;
            } else {
              sum += (float)-0.0056846277;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.019561032;
            } else {
              sum += (float)-0.0033590598;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.11484563;
            } else {
              sum += (float)-0.017687537;
            }
          } else {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.073199682;
            } else {
              sum += (float)0.21735232;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 4.999999)) {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.025974041)) {
              sum += (float)-0.0061205006;
            } else {
              sum += (float)-0.22531645;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.076857232;
            } else {
              sum += (float)0.244482;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.024307942;
            } else {
              sum += (float)0.22813183;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.15504587;
            } else {
              sum += (float)0.3592025;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.096891843;
            } else {
              sum += (float)0.33196622;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.3351576;
            } else {
              sum += (float)0.090470709;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.6379596)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.73049623;
            } else {
              sum += (float)0.43396708;
            }
          } else {
            sum += (float)0.17396019;
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.012449993)) {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 0.99858218)) {
              sum += (float)-0.044568483;
            } else {
              sum += (float)0.11168901;
            }
          } else {
            if (  (data[7].fvalue < 0.99976003)) {
              sum += (float)0.1830564;
            } else {
              sum += (float)0.28947294;
            }
          }
        } else {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[7].fvalue < 0.99983275)) {
              sum += (float)0.27677259;
            } else {
              sum += (float)0.57540321;
            }
          } else {
            sum += (float)0.83910722;
          }
        }
      } else {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.99814034)) {
            if (  (data[11].fvalue < 0.023508264)) {
              sum += (float)0.18620372;
            } else {
              sum += (float)0.027970282;
            }
          } else {
            if (  (data[11].fvalue < 0.023508264)) {
              sum += (float)-0.047976039;
            } else {
              sum += (float)-0.13280366;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.58645195)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.38255763;
            } else {
              sum += (float)0.18716629;
            }
          } else {
            if (  (data[11].fvalue < 0.31345612)) {
              sum += (float)0.0037216619;
            } else {
              sum += (float)0.16848911;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 1.2745235)) {
    if (  (data[12].fvalue < 5.999999)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.45686394)) {
            if (  (data[9].fvalue < 0.97038966)) {
              sum += (float)0.14115392;
            } else {
              sum += (float)0.018157028;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.00034479453;
            } else {
              sum += (float)-0.18658227;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.1224594)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.021103214;
            } else {
              sum += (float)-0.052617971;
            }
          } else {
            if (  (data[10].fvalue < 0.42575443)) {
              sum += (float)-0.063653603;
            } else {
              sum += (float)-0.010439328;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.13129728)) {
          if (  (data[10].fvalue < 0.053909924)) {
            if (  (data[9].fvalue < 0.99824041)) {
              sum += (float)0.015094463;
            } else {
              sum += (float)0.12665549;
            }
          } else {
            if (  (data[7].fvalue < 0.99992293)) {
              sum += (float)0.088626206;
            } else {
              sum += (float)0.018039575;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.018446323;
            } else {
              sum += (float)0.41055861;
            }
          } else {
            if (  (data[22].fvalue < 0.9497546)) {
              sum += (float)0.39098582;
            } else {
              sum += (float)0.091970809;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.47308585)) {
        if (  (data[9].fvalue < 1.0472143)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)0.59393841;
            } else {
              sum += (float)1.113134;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)0.21172169;
            } else {
              sum += (float)0.72019267;
            }
          }
        } else {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[10].fvalue < 0.13796192)) {
              sum += (float)0.20919582;
            } else {
              sum += (float)0.080606826;
            }
          } else {
            if (  (data[9].fvalue < 1.2251807)) {
              sum += (float)0.60745889;
            } else {
              sum += (float)0.40987328;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.1224594)) {
          if (  (data[10].fvalue < 0.8763091)) {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)0.084084988;
            } else {
              sum += (float)0.40627983;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.02521475;
            } else {
              sum += (float)0.1627548;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.71746433)) {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)0.036750957;
            } else {
              sum += (float)0.28259546;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.084666915;
            } else {
              sum += (float)0.28695795;
            }
          }
        }
      }
    }
  } else {
    if (  (data[22].fvalue < 0.34730217)) {
      if ( (data[9].fvalue < 3.2532282)) {
        if (  (data[21].fvalue < 0.28246963)) {
          if ( (data[9].fvalue < 2.1684504)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.02572141;
            } else {
              sum += (float)0.001889659;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.04878813;
            } else {
              sum += (float)-0.02025116;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.0873525)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0024863924;
            } else {
              sum += (float)0.036284279;
            }
          } else {
            if (  (data[21].fvalue < 1.36522)) {
              sum += (float)-0.018498467;
            } else {
              sum += (float)0.00064340618;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.084190771)) {
          if (  (data[9].fvalue < 5.5949621)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.048641205;
            } else {
              sum += (float)-0.023305211;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.074460894;
            } else {
              sum += (float)-0.030989973;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.1240638)) {
              sum += (float)0.00098761404;
            } else {
              sum += (float)-0.016552744;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.011666125;
            } else {
              sum += (float)0.10844171;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.6148094)) {
        if (  (data[22].fvalue < 1.4574125)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.2540518)) {
              sum += (float)-0.026440721;
            } else {
              sum += (float)0.016734768;
            }
          } else {
            if (  (data[10].fvalue < 0.30810371)) {
              sum += (float)0.074526288;
            } else {
              sum += (float)0.020137932;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[21].fvalue < 0.024290379)) {
              sum += (float)-0.029928267;
            } else {
              sum += (float)-0.16808729;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.31963614;
            } else {
              sum += (float)-0.58479208;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.78219873)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0055919373;
            } else {
              sum += (float)0.0057383189;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.0069253258;
            } else {
              sum += (float)-0.017228331;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.084190771)) {
              sum += (float)0.037184302;
            } else {
              sum += (float)-0.040770609;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.10022701;
            } else {
              sum += (float)0.0014426581;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 1.6847129)) {
    if (  (data[13].fvalue < 4.999999)) {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[11].fvalue < 0.063116804)) {
            if (  (data[11].fvalue < 0.032417551)) {
              sum += (float)0.26124933;
            } else {
              sum += (float)0.065448351;
            }
          } else {
            if (  (data[11].fvalue < 0.43249899)) {
              sum += (float)0.014512574;
            } else {
              sum += (float)-0.0030252067;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.228669;
            } else {
              sum += (float)0.65141916;
            }
          } else {
            if (  (data[22].fvalue < 0.024178868)) {
              sum += (float)0.055634849;
            } else {
              sum += (float)0.38840872;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.028848026;
            } else {
              sum += (float)0.037334334;
            }
          } else {
            if (  (data[11].fvalue < 0.61036408)) {
              sum += (float)0.16974692;
            } else {
              sum += (float)0.021829424;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.7320047)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.10312783;
            } else {
              sum += (float)-0.26570377;
            }
          } else {
            if (  (data[22].fvalue < 0.024178868)) {
              sum += (float)-0.041830409;
            } else {
              sum += (float)0.1258589;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.4115693)) {
              sum += (float)-0.062766925;
            } else {
              sum += (float)0.062418785;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.11148331;
            } else {
              sum += (float)0.0058799605;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.7320047)) {
            if (  (data[11].fvalue < 0.61036408)) {
              sum += (float)0.51497453;
            } else {
              sum += (float)0.22552052;
            }
          } else {
            if (  (data[11].fvalue < 0.20234896)) {
              sum += (float)0.41475031;
            } else {
              sum += (float)0.12865962;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 9.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.66944152)) {
              sum += (float)0.19635014;
            } else {
              sum += (float)-0.0041583688;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.012681132;
            } else {
              sum += (float)0.13753565;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.38443398)) {
            if (  (data[10].fvalue < 0.66944152)) {
              sum += (float)0.17667431;
            } else {
              sum += (float)0.53454608;
            }
          } else {
            if (  (data[22].fvalue < 0.024178868)) {
              sum += (float)0.089587137;
            } else {
              sum += (float)0.31125462;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 22.999998)) {
      if (  (data[12].fvalue < 15.999999)) {
        if (  (data[14].fvalue < 5.999999)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.00043499196;
            } else {
              sum += (float)-0.013672861;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.0038438463;
            } else {
              sum += (float)0.014962562;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.072429165;
            } else {
              sum += (float)0.073399611;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.52052605;
            } else {
              sum += (float)0.22432427;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.17629533;
            } else {
              sum += (float)-0.017085366;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.31583455;
            } else {
              sum += (float)0.075791761;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 17.999998)) {
              sum += (float)-0.28181162;
            } else {
              sum += (float)-0.020384781;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.040298086;
            } else {
              sum += (float)-0.22142793;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 16.999998)) {
        if (  (data[13].fvalue < 23.999998)) {
          if (  (data[12].fvalue < 15.999999)) {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)-0.038085151;
            } else {
              sum += (float)0.17655499;
            }
          } else {
            sum += (float)-0.20259044;
          }
        } else {
          if (  (data[10].fvalue < 0.17159836)) {
            sum += (float)0.294471;
          } else {
            sum += (float)0.3533574;
          }
        }
      } else {
        sum += (float)0.4586516;
      }
    }
  }
  if (  (data[8].fvalue < 0.11013398)) {
    if (  (data[18].fvalue < 0.033776689)) {
      if (  (data[10].fvalue < 1.0523684)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.45696822)) {
              sum += (float)-0.0034461513;
            } else {
              sum += (float)0.054522093;
            }
          } else {
            if (  (data[10].fvalue < 0.03026147)) {
              sum += (float)0.017678238;
            } else {
              sum += (float)-0.042227056;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.15162523)) {
            if (  (data[7].fvalue < 0.9491353)) {
              sum += (float)-0.097460836;
            } else {
              sum += (float)-0.50239003;
            }
          } else {
            if (  (data[18].fvalue < 0.015469597)) {
              sum += (float)0.20855291;
            } else {
              sum += (float)-0.16186245;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 0.073428959)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.37514201;
            } else {
              sum += (float)0.76666188;
            }
          } else {
            if (  (data[18].fvalue < 0.015469597)) {
              sum += (float)0.25229973;
            } else {
              sum += (float)0.64824569;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.97154289)) {
            sum += (float)-0.39602503;
          } else {
            sum += (float)-0.24063073;
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.069495507)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 0.83632851)) {
            if (  (data[11].fvalue < 0.74545163)) {
              sum += (float)0.23753142;
            } else {
              sum += (float)-0.026861154;
            }
          } else {
            if (  (data[22].fvalue < 0.023207521)) {
              sum += (float)0.00073339225;
            } else {
              sum += (float)0.031574931;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.28116342)) {
            if (  (data[10].fvalue < 0.03026147)) {
              sum += (float)-0.01346555;
            } else {
              sum += (float)0.10810461;
            }
          } else {
            if (  (data[22].fvalue < 0.19878252)) {
              sum += (float)-0.19597551;
            } else {
              sum += (float)-0.058091599;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.33740714)) {
          if (  (data[8].fvalue < 0.032485384)) {
            if (  (data[10].fvalue < 0.32813323)) {
              sum += (float)0.17168073;
            } else {
              sum += (float)-1.0363803;
            }
          } else {
            if (  (data[10].fvalue < 0.53438979)) {
              sum += (float)0.02597541;
            } else {
              sum += (float)0.19331807;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[10].fvalue < 0.11194737)) {
              sum += (float)0.051137306;
            } else {
              sum += (float)0.21301702;
            }
          } else {
            if (  (data[10].fvalue < 0.11805031)) {
              sum += (float)0.25566891;
            } else {
              sum += (float)0.47715208;
            }
          }
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 0.01190765)) {
      if (  (data[18].fvalue < 0.15323727)) {
        if (  (data[10].fvalue < 1.0523684)) {
          if (  (data[18].fvalue < 0.0030596713)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.052919637;
            } else {
              sum += (float)-0.23657271;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.013086624;
            } else {
              sum += (float)-0.078434058;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.36000034)) {
            if (  (data[18].fvalue < 0.077148244)) {
              sum += (float)0.03970949;
            } else {
              sum += (float)0.25704426;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.038771704;
            } else {
              sum += (float)-0.15262875;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.61752522)) {
          if (  (data[10].fvalue < 0.51223373)) {
            if (  (data[18].fvalue < 0.39922774)) {
              sum += (float)0.0058380258;
            } else {
              sum += (float)0.19535615;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.080189109;
            } else {
              sum += (float)0.31552976;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.23103404)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.096368544;
            } else {
              sum += (float)-0.29491362;
            }
          } else {
            if (  (data[8].fvalue < 1.1492229)) {
              sum += (float)0.017661428;
            } else {
              sum += (float)-0.32238907;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.024139926)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[18].fvalue < 0.015469597)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.0017498579;
            } else {
              sum += (float)0.0078204283;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.048056487;
            } else {
              sum += (float)-0.0081262626;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.4913289)) {
            if (  (data[10].fvalue < 0.39535537)) {
              sum += (float)-0.70565164;
            } else {
              sum += (float)-0.37960246;
            }
          } else {
            if (  (data[18].fvalue < 0.0030596713)) {
              sum += (float)0.85279346;
            } else {
              sum += (float)0.30306533;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.033776689)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.040204775;
            } else {
              sum += (float)-0.096546806;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.15176852;
            } else {
              sum += (float)-0.14785038;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.45507342)) {
            if (  (data[10].fvalue < 1.0523684)) {
              sum += (float)0.0059239971;
            } else {
              sum += (float)0.09624777;
            }
          } else {
            if (  (data[10].fvalue < 0.87990373)) {
              sum += (float)-0.001287562;
            } else {
              sum += (float)-0.024383906;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 2.7506363)) {
    if (  (data[6].fvalue < 42.77692)) {
      if (  (data[19].fvalue < 0.11323642)) {
        if (  (data[13].fvalue < 7.999999)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0025201389;
            } else {
              sum += (float)0.04320053;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.058806762;
            } else {
              sum += (float)-0.072811671;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.59037185)) {
              sum += (float)-0.1785658;
            } else {
              sum += (float)0.38474673;
            }
          } else {
            if (  (data[19].fvalue < 0.074368335)) {
              sum += (float)-0.53454733;
            } else {
              sum += (float)-0.25646269;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[21].fvalue < 0.023448672)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.0046354495;
            } else {
              sum += (float)-0.30827469;
            }
          } else {
            if (  (data[21].fvalue < 0.08448793)) {
              sum += (float)0.066261843;
            } else {
              sum += (float)0.013521215;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.08448793)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.0040229163;
            } else {
              sum += (float)0.018047767;
            }
          } else {
            if (  (data[11].fvalue < 0.038696662)) {
              sum += (float)0.070591718;
            } else {
              sum += (float)-0.0092460262;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.99999899)) {
        if (  (data[21].fvalue < 0.023448672)) {
          if (  (data[11].fvalue < 0.038696662)) {
            if (  (data[19].fvalue < 0.84591609)) {
              sum += (float)-0.038909301;
            } else {
              sum += (float)0.034244969;
            }
          } else {
            if (  (data[10].fvalue < 0.18580724)) {
              sum += (float)0.025703175;
            } else {
              sum += (float)-0.015744345;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.09936671)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.010459239;
            } else {
              sum += (float)-0.15307553;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.019300489;
            } else {
              sum += (float)-0.023693567;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 13.999999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[10].fvalue < 0.66127247)) {
              sum += (float)0.0006927576;
            } else {
              sum += (float)-0.050714169;
            }
          } else {
            if (  (data[19].fvalue < 0.12654905)) {
              sum += (float)-0.1248649;
            } else {
              sum += (float)0.025879782;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.038696662)) {
            if (  (data[19].fvalue < 0.71182132)) {
              sum += (float)0.29724109;
            } else {
              sum += (float)0.52071154;
            }
          } else {
            if (  (data[19].fvalue < 0.71182132)) {
              sum += (float)-0.14914607;
            } else {
              sum += (float)0.10778092;
            }
          }
        }
      }
    }
  } else {
    if (  (data[10].fvalue < 0.71408916)) {
      if (  (data[14].fvalue < 1.999999)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.015970705;
            } else {
              sum += (float)0.042530932;
            }
          } else {
            if (  (data[19].fvalue < 6.8650918)) {
              sum += (float)-0.016935676;
            } else {
              sum += (float)-0.061599813;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.014391959;
            } else {
              sum += (float)-0.012032552;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.041512258;
            } else {
              sum += (float)0.27261207;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.076960593;
            } else {
              sum += (float)-0.47459462;
            }
          } else {
            if (  (data[10].fvalue < 0.47237909)) {
              sum += (float)-0.029721173;
            } else {
              sum += (float)0.012219503;
            }
          }
        } else {
          if (  (data[19].fvalue < 4.9806566)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.034087475;
            } else {
              sum += (float)-0.010308244;
            }
          } else {
            if (  (data[21].fvalue < 0.31489649)) {
              sum += (float)-0.019377079;
            } else {
              sum += (float)0.014405548;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 1.999999)) {
        if (  (data[19].fvalue < 4.9806566)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.088992253)) {
              sum += (float)-0.1323002;
            } else {
              sum += (float)0.034875553;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.056676224;
            } else {
              sum += (float)0.12554903;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.87469041)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.052046183;
            } else {
              sum += (float)0.14602962;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.30297485;
            } else {
              sum += (float)0.15775438;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 10.999999)) {
          if (  (data[10].fvalue < 0.87469041)) {
            if (  (data[21].fvalue < 0.67282289)) {
              sum += (float)-0.033233073;
            } else {
              sum += (float)0.012665647;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.09749686;
            } else {
              sum += (float)0.034769636;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            sum += (float)0.38087788;
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)0.062222254;
            } else {
              sum += (float)0.22592078;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 5.999999)) {
    if ( (data[9].fvalue < 5.303443)) {
      if (  (data[17].fvalue < 2.999999)) {
        if (  (data[9].fvalue < 0.97096908)) {
          if (  (data[10].fvalue < 0.25448972)) {
            if (  (data[10].fvalue < 0.018615294)) {
              sum += (float)-0.43555766;
            } else {
              sum += (float)0.51066774;
            }
          } else {
            if (  (data[7].fvalue < 0.99992061)) {
              sum += (float)0.0015061715;
            } else {
              sum += (float)0.065739989;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.74239451)) {
            if (  (data[9].fvalue < 0.99835074)) {
              sum += (float)0.0037666538;
            } else {
              sum += (float)-0.033347033;
            }
          } else {
            if (  (data[6].fvalue < 32.96944)) {
              sum += (float)-0.00019040074;
            } else {
              sum += (float)-0.0042317649;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.7212741)) {
          if (  (data[9].fvalue < 1.9475006)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.055964828;
            } else {
              sum += (float)0.15571956;
            }
          } else {
            if ( (data[9].fvalue < 2.4046669)) {
              sum += (float)-0.53150737;
            } else {
              sum += (float)-0.22647084;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.089126587;
            } else {
              sum += (float)-0.012583095;
            }
          } else {
            if (  (data[5].fvalue < 19.615154)) {
              sum += (float)-0.068806842;
            } else {
              sum += (float)0.029950127;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.0046406239;
            } else {
              sum += (float)-0.0026119009;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0057421746;
            } else {
              sum += (float)0.033018555;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 7.8773785)) {
              sum += (float)0.0090352548;
            } else {
              sum += (float)0.024533395;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.0098655103;
            } else {
              sum += (float)-0.021484513;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 3.999999)) {
              sum += (float)0.0067937351;
            } else {
              sum += (float)0.15319057;
            }
          } else {
            if (  (data[6].fvalue < 17.010714)) {
              sum += (float)-0.018567635;
            } else {
              sum += (float)-0.00022026927;
            }
          }
        } else {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[17].fvalue < 3.999999)) {
              sum += (float)-0.065421097;
            } else {
              sum += (float)-0.23856089;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.27845192;
            } else {
              sum += (float)-0.15486023;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 2.2384701)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[9].fvalue < 1.3821974)) {
          if (  (data[10].fvalue < 0.78174257)) {
            if (  (data[9].fvalue < 1.3252488)) {
              sum += (float)-0.48885441;
            } else {
              sum += (float)-0.26133674;
            }
          } else {
            if (  (data[9].fvalue < 1.2739542)) {
              sum += (float)-0.25688735;
            } else {
              sum += (float)0.02850545;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.39659441)) {
            if (  (data[9].fvalue < 2.0128682)) {
              sum += (float)0.0007970556;
            } else {
              sum += (float)-0.54170555;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.028264083;
            } else {
              sum += (float)0.12507962;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 3.999999)) {
            if ( (data[9].fvalue < 1.7990019)) {
              sum += (float)0.26180547;
            } else {
              sum += (float)0.49190456;
            }
          } else {
            if ( (data[9].fvalue < 1.8753376)) {
              sum += (float)0.027651606;
            } else {
              sum += (float)0.40059558;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.6734129)) {
            sum += (float)0.77925557;
          } else {
            sum += (float)1.0336173;
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 5.303443)) {
        if (  (data[10].fvalue < 0.58861548)) {
          if (  (data[9].fvalue < 4.1423993)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.61916101;
            } else {
              sum += (float)-0.50688428;
            }
          } else {
            if (  (data[9].fvalue < 4.8573422)) {
              sum += (float)-0.39877647;
            } else {
              sum += (float)-0.28966704;
            }
          }
        } else {
          sum += (float)0.10664338;
        }
      } else {
        if (  (data[10].fvalue < 0.28849366)) {
          if (  (data[15].fvalue < 0.99999899)) {
            sum += (float)-0.10375661;
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.193872;
            } else {
              sum += (float)-0.01018318;
            }
          }
        } else {
          sum += (float)-0.2117077;
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.42253888)) {
    if (  (data[13].fvalue < 8.999999)) {
      if ( (data[9].fvalue < 1.6120039)) {
        if (  (data[22].fvalue < 0.054066144)) {
          if (  (data[20].fvalue < 0.071340829)) {
            if ( (data[9].fvalue < 1.2271862)) {
              sum += (float)-0.0044989162;
            } else {
              sum += (float)-0.026373528;
            }
          } else {
            if (  (data[18].fvalue < 0.015596257)) {
              sum += (float)-0.010153078;
            } else {
              sum += (float)0.031855851;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if ( (data[9].fvalue < 1.1262256)) {
              sum += (float)-0.093178183;
            } else {
              sum += (float)0.0058898898;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.022205176;
            } else {
              sum += (float)0.080013722;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[9].fvalue < 2.8052673)) {
            if (  (data[8].fvalue < 3.1430266)) {
              sum += (float)0.010921775;
            } else {
              sum += (float)-0.008160688;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.011272294;
            } else {
              sum += (float)0.0021194243;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.054723401)) {
              sum += (float)0.036584761;
            } else {
              sum += (float)-0.0062972778;
            }
          } else {
            if (  (data[9].fvalue < 2.7279544)) {
              sum += (float)-0.10951871;
            } else {
              sum += (float)-0.02091057;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 1.999999)) {
        if (  (data[9].fvalue < 2.0148766)) {
          if (  (data[8].fvalue < 0.79719651)) {
            if (  (data[18].fvalue < 0.24582341)) {
              sum += (float)-0.077720352;
            } else {
              sum += (float)0.084390618;
            }
          } else {
            if (  (data[20].fvalue < 0.020934384)) {
              sum += (float)-0.15758331;
            } else {
              sum += (float)-0.05545371;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.033498481;
            } else {
              sum += (float)0.10478126;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.067786127;
            } else {
              sum += (float)-0.051956598;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 3.0725448)) {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 1.5536484)) {
              sum += (float)0.055588964;
            } else {
              sum += (float)-0.22573414;
            }
          } else {
            if (  (data[9].fvalue < 2.642679)) {
              sum += (float)-0.63159746;
            } else {
              sum += (float)-0.40768287;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.015309261;
            } else {
              sum += (float)-0.11290847;
            }
          } else {
            if (  (data[18].fvalue < 0.15222144)) {
              sum += (float)-0.2747725;
            } else {
              sum += (float)-0.16831581;
            }
          }
        }
      }
    }
  } else {
    if (  (data[9].fvalue < 1.444195)) {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[8].fvalue < 0.23315534)) {
          if (  (data[9].fvalue < 1.2748474)) {
            if (  (data[9].fvalue < 1.2271862)) {
              sum += (float)0.74289107;
            } else {
              sum += (float)0.38300648;
            }
          } else {
            if (  (data[18].fvalue < 0.52520496)) {
              sum += (float)-0.030985933;
            } else {
              sum += (float)0.23970826;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.1830831)) {
            if (  (data[8].fvalue < 1.325528)) {
              sum += (float)0.063329838;
            } else {
              sum += (float)-0.016765803;
            }
          } else {
            if (  (data[21].fvalue < 0.050538581)) {
              sum += (float)-0.031454418;
            } else {
              sum += (float)0.0091168983;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.325528)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[18].fvalue < 0.70153755)) {
              sum += (float)0.14362369;
            } else {
              sum += (float)0.29721338;
            }
          } else {
            if (  (data[18].fvalue < 0.62548184)) {
              sum += (float)0.26234218;
            } else {
              sum += (float)0.42854279;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.74229306)) {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.04003232;
            } else {
              sum += (float)-0.25035268;
            }
          } else {
            if (  (data[8].fvalue < 2.4837198)) {
              sum += (float)0.19659728;
            } else {
              sum += (float)-0.025068682;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 1.5131234)) {
        if (  (data[13].fvalue < 9.999999)) {
          if (  (data[9].fvalue < 1.5536484)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.019595362;
            } else {
              sum += (float)0.094234467;
            }
          } else {
            if (  (data[8].fvalue < 0.50819671)) {
              sum += (float)-0.023165375;
            } else {
              sum += (float)-0.0093785049;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.0148766)) {
            if (  (data[18].fvalue < 0.88395482)) {
              sum += (float)0.058700591;
            } else {
              sum += (float)0.29983181;
            }
          } else {
            if (  (data[18].fvalue < 0.55845165)) {
              sum += (float)-0.040914226;
            } else {
              sum += (float)0.011152971;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 2.8877213)) {
              sum += (float)-0.0040015234;
            } else {
              sum += (float)0.014192089;
            }
          } else {
            if (  (data[18].fvalue < 1.1416628)) {
              sum += (float)-0.016563985;
            } else {
              sum += (float)0.0015434851;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.28450593)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.004938866;
            } else {
              sum += (float)0.14165033;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.0010221854;
            } else {
              sum += (float)0.016518328;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.073179133)) {
    if (  (data[9].fvalue < 0.99832171)) {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[11].fvalue < 0.012232913)) {
          if (  (data[9].fvalue < 0.97036344)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.1779834;
            } else {
              sum += (float)0.050903875;
            }
          } else {
            if (  (data[5].fvalue < 4.8969607)) {
              sum += (float)0.27032036;
            } else {
              sum += (float)0.055267297;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.97036344)) {
            if (  (data[11].fvalue < 0.28852665)) {
              sum += (float)0.088930488;
            } else {
              sum += (float)0.00014475729;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0074067623;
            } else {
              sum += (float)0.039158698;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[9].fvalue < 0.97036344)) {
            if (  (data[8].fvalue < 0.030568235)) {
              sum += (float)-0.38052955;
            } else {
              sum += (float)-0.25499603;
            }
          } else {
            if (  (data[8].fvalue < 0.030568235)) {
              sum += (float)-0.15966576;
            } else {
              sum += (float)0.022221241;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.97036344)) {
            if (  (data[11].fvalue < 0.20111854)) {
              sum += (float)-0.13109107;
            } else {
              sum += (float)-0.029210284;
            }
          } else {
            if (  (data[8].fvalue < 0.030568235)) {
              sum += (float)0.00054835476;
            } else {
              sum += (float)0.099468157;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 3.999999)) {
        if (  (data[9].fvalue < 1.1270936)) {
          if (  (data[9].fvalue < 1.0470368)) {
            if (  (data[11].fvalue < 0.90264785)) {
              sum += (float)-0.012215774;
            } else {
              sum += (float)-0.34877691;
            }
          } else {
            if (  (data[21].fvalue < 0.065469623)) {
              sum += (float)-0.11019841;
            } else {
              sum += (float)-0.79792726;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 1.2257434)) {
              sum += (float)0.060851507;
            } else {
              sum += (float)0.0020194736;
            }
          } else {
            if (  (data[9].fvalue < 1.2726862)) {
              sum += (float)-0.082490407;
            } else {
              sum += (float)0.0056153624;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.5026759)) {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[9].fvalue < 1.2257434)) {
              sum += (float)0.10583787;
            } else {
              sum += (float)0.04371278;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.17581424;
            } else {
              sum += (float)0.7155636;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.023661416)) {
              sum += (float)-0.13651484;
            } else {
              sum += (float)0.015028662;
            }
          } else {
            if (  (data[9].fvalue < 1.7334849)) {
              sum += (float)0.056914054;
            } else {
              sum += (float)-0.038554147;
            }
          }
        }
      }
    }
  } else {
    if (  (data[5].fvalue < 3.3740244)) {
      if (  (data[21].fvalue < 0.023661416)) {
        if (  (data[13].fvalue < 9.999999)) {
          if (  (data[8].fvalue < 0.50950336)) {
            if (  (data[9].fvalue < 0.9245047)) {
              sum += (float)0.022190863;
            } else {
              sum += (float)-0.0089593939;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.011457234;
            } else {
              sum += (float)-0.051609926;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.14734687)) {
            if (  (data[9].fvalue < 1.0470368)) {
              sum += (float)0.48931092;
            } else {
              sum += (float)0.19768599;
            }
          } else {
            if (  (data[11].fvalue < 0.4982236)) {
              sum += (float)0.070736147;
            } else {
              sum += (float)-0.021416757;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.010488001;
            } else {
              sum += (float)0.025125481;
            }
          } else {
            if (  (data[8].fvalue < 0.50950336)) {
              sum += (float)0.054825474;
            } else {
              sum += (float)-0.0059922123;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.7334849)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.031990122;
            } else {
              sum += (float)0.12650871;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.020698724;
            } else {
              sum += (float)-0.023081711;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 21.999998)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.0075786482;
            } else {
              sum += (float)0.0047003864;
            }
          } else {
            if (  (data[9].fvalue < 2.9851692)) {
              sum += (float)0.059757032;
            } else {
              sum += (float)0.016139118;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.00029644548;
            } else {
              sum += (float)0.0093348166;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.0011627658;
            } else {
              sum += (float)-0.011896899;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 23.999998)) {
          if (  (data[21].fvalue < 0.53435242)) {
            if ( (data[9].fvalue < 2.0148511)) {
              sum += (float)-0.011573785;
            } else {
              sum += (float)0.14043535;
            }
          } else {
            if (  (data[21].fvalue < 5.3823676)) {
              sum += (float)-0.0055443584;
            } else {
              sum += (float)0.10569239;
            }
          }
        } else {
          sum += (float)0.25413623;
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.23034529)) {
    if (  (data[19].fvalue < 0.31949496)) {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[18].fvalue < 0.044491399)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.051419828)) {
              sum += (float)-0.013937319;
            } else {
              sum += (float)0.0068409909;
            }
          } else {
            if (  (data[19].fvalue < 0.12731713)) {
              sum += (float)-0.24313262;
            } else {
              sum += (float)-0.062658988;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.0097012743)) {
            if (  (data[8].fvalue < 3.5221686)) {
              sum += (float)0.014827651;
            } else {
              sum += (float)-0.051052023;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.067513086;
            } else {
              sum += (float)-0.012827924;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[19].fvalue < 0.18582121)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.010628316;
            } else {
              sum += (float)-0.14068605;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.19022737;
            } else {
              sum += (float)0.63764709;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.12731713)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.1698824;
            } else {
              sum += (float)-0.45983046;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.027650585;
            } else {
              sum += (float)-0.24397598;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[18].fvalue < 0.064503692)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.0035075406)) {
              sum += (float)0.0026413761;
            } else {
              sum += (float)-0.026762318;
            }
          } else {
            if (  (data[18].fvalue < 0.044491399)) {
              sum += (float)0.15347192;
            } else {
              sum += (float)0.049695082;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.2167956)) {
            if (  (data[19].fvalue < 0.37611941)) {
              sum += (float)0.0095700864;
            } else {
              sum += (float)-0.053638492;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.020653851;
            } else {
              sum += (float)-0.02589613;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.10103067)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.39294159;
            } else {
              sum += (float)0.24424656;
            }
          } else {
            if (  (data[19].fvalue < 0.63840127)) {
              sum += (float)-0.062727161;
            } else {
              sum += (float)0.16485791;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.11299786;
            } else {
              sum += (float)-0.0070001329;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.20374969;
            } else {
              sum += (float)0.035663575;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.42064834)) {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[19].fvalue < 0.020148121)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.00046178018;
            } else {
              sum += (float)0.047011513;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.14813331;
            } else {
              sum += (float)0.35358918;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.30100968)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.039881088;
            } else {
              sum += (float)-0.00091475126;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.061507959;
            } else {
              sum += (float)-0.037850097;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.18582121)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.27205348;
            } else {
              sum += (float)0.53917414;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.04714343;
            } else {
              sum += (float)0.20523821;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.017537147;
            } else {
              sum += (float)-0.018226076;
            }
          } else {
            if (  (data[18].fvalue < 0.37922871)) {
              sum += (float)-0.096727461;
            } else {
              sum += (float)0.11332123;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.55528337)) {
              sum += (float)0.1179224;
            } else {
              sum += (float)0.048824333;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.062057588;
            } else {
              sum += (float)-0.04268793;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.10880189;
            } else {
              sum += (float)-0.32013372;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.00062783936;
            } else {
              sum += (float)0.1543382;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 1.3909905)) {
              sum += (float)-0.077468701;
            } else {
              sum += (float)-0.0099416608;
            }
          } else {
            if (  (data[18].fvalue < 0.74332029)) {
              sum += (float)-0.031102635;
            } else {
              sum += (float)0.0088226264;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.4727194)) {
            if (  (data[19].fvalue < 0.67519712)) {
              sum += (float)0.0052694511;
            } else {
              sum += (float)0.14943106;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.044944972;
            } else {
              sum += (float)0.01636567;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 10.999999)) {
    if (  (data[19].fvalue < 2.0154223)) {
      if (  (data[14].fvalue < 2.999999)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 2.8863332)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-9.6742639e-05;
            } else {
              sum += (float)0.010055623;
            }
          } else {
            if (  (data[19].fvalue < 0.46671915)) {
              sum += (float)0.00062073977;
            } else {
              sum += (float)-0.01264071;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.2238183)) {
            if (  (data[10].fvalue < 0.19264351)) {
              sum += (float)0.0057303878;
            } else {
              sum += (float)0.039993919;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.017482774;
            } else {
              sum += (float)-0.034621559;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 5.4025278)) {
            if (  (data[18].fvalue < 0.7427128)) {
              sum += (float)0.014802353;
            } else {
              sum += (float)0.041817822;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.0065346221;
            } else {
              sum += (float)-0.03127883;
            }
          }
        } else {
          if (  (data[22].fvalue < 3.5703855)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.047503129;
            } else {
              sum += (float)-0.093422398;
            }
          } else {
            if (  (data[18].fvalue < 5.0826058)) {
              sum += (float)0.044435639;
            } else {
              sum += (float)-0.20063399;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.5600608)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.26444331)) {
              sum += (float)0.00093549531;
            } else {
              sum += (float)0.037224691;
            }
          } else {
            if (  (data[18].fvalue < 2.4264238)) {
              sum += (float)-0.0094456291;
            } else {
              sum += (float)0.050290972;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[22].fvalue < 7.5459614)) {
              sum += (float)-0.032712843;
            } else {
              sum += (float)0.032315567;
            }
          } else {
            if (  (data[19].fvalue < 4.0702281)) {
              sum += (float)-0.11430344;
            } else {
              sum += (float)-0.32741049;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.023178956)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.045177586;
            } else {
              sum += (float)-0.003503656;
            }
          } else {
            if (  (data[19].fvalue < 3.3922939)) {
              sum += (float)-0.0016249574;
            } else {
              sum += (float)0.015965573;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.76121491)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.024817228;
            } else {
              sum += (float)0.098940149;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.053952556;
            } else {
              sum += (float)-0.025146198;
            }
          }
        }
      }
    }
  } else {
    if (  (data[0].fvalue < 0.99999899)) {
      if (  (data[19].fvalue < 0.67557955)) {
        if (  (data[18].fvalue < 1.3870455)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.26234865)) {
              sum += (float)-0.10806705;
            } else {
              sum += (float)-0.22464862;
            }
          } else {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)-0.30888426;
            } else {
              sum += (float)-0.5343616;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 2.2812922)) {
              sum += (float)-0.029739797;
            } else {
              sum += (float)0.053697918;
            }
          } else {
            if (  (data[12].fvalue < 15.999999)) {
              sum += (float)-0.080036558;
            } else {
              sum += (float)-0.33563;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.83422178)) {
          if (  (data[19].fvalue < 0.94784617)) {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)0.092128441;
            } else {
              sum += (float)-0.10730013;
            }
          } else {
            if (  (data[18].fvalue < 0.14032473)) {
              sum += (float)-0.64049715;
            } else {
              sum += (float)0.16703041;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.46741706)) {
            if (  (data[18].fvalue < 1.2238183)) {
              sum += (float)0.01383016;
            } else {
              sum += (float)-0.063763611;
            }
          } else {
            if (  (data[19].fvalue < 1.8897928)) {
              sum += (float)-0.088641077;
            } else {
              sum += (float)-0.17729376;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.26444331)) {
        if (  (data[19].fvalue < 1.5948989)) {
          if (  (data[10].fvalue < 0.78386122)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.19308333;
            } else {
              sum += (float)-0.39417785;
            }
          } else {
            if (  (data[18].fvalue < 0.19922201)) {
              sum += (float)0.32854778;
            } else {
              sum += (float)0.098325223;
            }
          }
        } else {
          if (  (data[19].fvalue < 4.0702281)) {
            if (  (data[10].fvalue < 0.23801041)) {
              sum += (float)0.24834105;
            } else {
              sum += (float)0.49342072;
            }
          } else {
            sum += (float)-0.2179682;
          }
        }
      } else {
        if (  (data[19].fvalue < 0.94784617)) {
          if (  (data[18].fvalue < 0.88549376)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.032332424;
            } else {
              sum += (float)-0.2188565;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.079593599;
            } else {
              sum += (float)0.012638446;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.83422178)) {
            if (  (data[19].fvalue < 1.5948989)) {
              sum += (float)0.11235432;
            } else {
              sum += (float)0.24246638;
            }
          } else {
            if (  (data[18].fvalue < 1.4933081)) {
              sum += (float)0.054714527;
            } else {
              sum += (float)-0.045825008;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 0.074660748)) {
    if (  (data[4].fvalue < 0.99999899)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[18].fvalue < 0.016532347)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.33882961)) {
              sum += (float)-0.0408158;
            } else {
              sum += (float)0.032869179;
            }
          } else {
            if (  (data[18].fvalue < 0.00361316)) {
              sum += (float)0.17657603;
            } else {
              sum += (float)0.0055631883;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.023638863)) {
            if (  (data[10].fvalue < 0.53487217)) {
              sum += (float)-0.13545783;
            } else {
              sum += (float)-0.20930977;
            }
          } else {
            if (  (data[18].fvalue < 0.089118384)) {
              sum += (float)-0.056121904;
            } else {
              sum += (float)0.0069373064;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.065309756)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.00361316)) {
              sum += (float)-0.0074790847;
            } else {
              sum += (float)0.14628467;
            }
          } else {
            if (  (data[18].fvalue < 0.00361316)) {
              sum += (float)0.84791374;
            } else {
              sum += (float)0.18490757;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.87436229)) {
            if (  (data[5].fvalue < 0.69100571)) {
              sum += (float)0.10014311;
            } else {
              sum += (float)-0.011313171;
            }
          } else {
            if (  (data[5].fvalue < 1.7017171)) {
              sum += (float)-0.24154547;
            } else {
              sum += (float)-0.054804668;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.016532347)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.25440297)) {
            if (  (data[5].fvalue < 0.69100571)) {
              sum += (float)-0.077910312;
            } else {
              sum += (float)0.01196883;
            }
          } else {
            if (  (data[5].fvalue < 1.7017171)) {
              sum += (float)0.039985329;
            } else {
              sum += (float)0.0036332519;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.00361316)) {
            if (  (data[5].fvalue < 0.69100571)) {
              sum += (float)-0.52698559;
            } else {
              sum += (float)-0.073892131;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.085550644;
            } else {
              sum += (float)-0.52227056;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.69100571)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 0.62443912)) {
              sum += (float)0.056921974;
            } else {
              sum += (float)0.098523587;
            }
          } else {
            if (  (data[18].fvalue < 0.065309756)) {
              sum += (float)-0.26236096;
            } else {
              sum += (float)0.11492825;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.022240927)) {
            if (  (data[18].fvalue < 0.33454004)) {
              sum += (float)-0.043736663;
            } else {
              sum += (float)-0.0015438237;
            }
          } else {
            if (  (data[22].fvalue < 0.07320901)) {
              sum += (float)0.077840343;
            } else {
              sum += (float)0.0072539491;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 6.999999)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.21479385)) {
              sum += (float)-0.022391574;
            } else {
              sum += (float)0.00098353322;
            }
          } else {
            if (  (data[18].fvalue < 0.033658046)) {
              sum += (float)0.39865479;
            } else {
              sum += (float)0.037550259;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.025107946)) {
              sum += (float)0.019926697;
            } else {
              sum += (float)0.0052284524;
            }
          } else {
            if (  (data[18].fvalue < 0.023638863)) {
              sum += (float)-0.60111541;
            } else {
              sum += (float)-0.016000509;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 1.0547156)) {
              sum += (float)0.0051978482;
            } else {
              sum += (float)0.055254556;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0052808938;
            } else {
              sum += (float)-0.014327294;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.025107946)) {
            if (  (data[22].fvalue < 0.022240927)) {
              sum += (float)-0.017297825;
            } else {
              sum += (float)0.0034621868;
            }
          } else {
            if (  (data[21].fvalue < 0.084781766)) {
              sum += (float)0.086143441;
            } else {
              sum += (float)0.013966928;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.089118384)) {
            if (  (data[10].fvalue < 0.49285224)) {
              sum += (float)-0.17970912;
            } else {
              sum += (float)0.96501577;
            }
          } else {
            if (  (data[21].fvalue < 0.025107946)) {
              sum += (float)0.014897891;
            } else {
              sum += (float)0.052821122;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.077393934)) {
            if (  (data[18].fvalue < 0.05488326)) {
              sum += (float)-0.65754056;
            } else {
              sum += (float)-0.37981513;
            }
          } else {
            if (  (data[18].fvalue < 0.41912642)) {
              sum += (float)-0.020336125;
            } else {
              sum += (float)-0.070947796;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 10.999999)) {
            if (  (data[18].fvalue < 0.11369088)) {
              sum += (float)0.19359;
            } else {
              sum += (float)-0.039666601;
            }
          } else {
            if (  (data[21].fvalue < 1.984776)) {
              sum += (float)-0.14270613;
            } else {
              sum += (float)-0.071902215;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.025107946)) {
            if (  (data[18].fvalue < 0.065309756)) {
              sum += (float)-0.54854888;
            } else {
              sum += (float)-0.0017657052;
            }
          } else {
            if (  (data[18].fvalue < 2.0411768)) {
              sum += (float)0.066415489;
            } else {
              sum += (float)0.025838951;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.073989823)) {
    if (  (data[10].fvalue < 0.78089231)) {
      if (  (data[13].fvalue < 4.999999)) {
        if (  (data[18].fvalue < 0.0034503797)) {
          if (  (data[10].fvalue < 0.13835016)) {
            if (  (data[21].fvalue < 0.025649723)) {
              sum += (float)0.024496406;
            } else {
              sum += (float)-0.040126804;
            }
          } else {
            if (  (data[10].fvalue < 0.45666471)) {
              sum += (float)-0.053866845;
            } else {
              sum += (float)0.056640677;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.12704305)) {
              sum += (float)0.046969984;
            } else {
              sum += (float)-0.014806572;
            }
          } else {
            if (  (data[21].fvalue < 0.025649723)) {
              sum += (float)-0.030058375;
            } else {
              sum += (float)0.061723277;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.090692066)) {
          if (  (data[18].fvalue < 0.15273146)) {
            if (  (data[21].fvalue < 0.025649723)) {
              sum += (float)-0.029061977;
            } else {
              sum += (float)0.10381657;
            }
          } else {
            if (  (data[10].fvalue < 0.038170449)) {
              sum += (float)0.039084591;
            } else {
              sum += (float)0.17899784;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.024335787)) {
            if (  (data[18].fvalue < 0.0034503797)) {
              sum += (float)0.076185077;
            } else {
              sum += (float)-0.08184284;
            }
          } else {
            if (  (data[10].fvalue < 0.15836105)) {
              sum += (float)0.17636377;
            } else {
              sum += (float)0.42000213;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.034251589)) {
        if (  (data[18].fvalue < 0.0034503797)) {
          if (  (data[10].fvalue < 0.88241512)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.27348736;
            } else {
              sum += (float)0.453601;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.60356396;
            } else {
              sum += (float)0.33491427;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.88241512)) {
              sum += (float)0.18827191;
            } else {
              sum += (float)0.44569212;
            }
          } else {
            sum += (float)-0.29661611;
          }
        }
      } else {
        sum += (float)-1.307748;
      }
    }
  } else {
    if (  (data[13].fvalue < 5.999999)) {
      if (  (data[21].fvalue < 0.06650354)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.28075054)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.008733497;
            } else {
              sum += (float)0.024484856;
            }
          } else {
            if (  (data[18].fvalue < 1.1417936)) {
              sum += (float)-0.039772369;
            } else {
              sum += (float)0.012141288;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.23123063)) {
              sum += (float)-0.0037645376;
            } else {
              sum += (float)0.0086584985;
            }
          } else {
            if (  (data[22].fvalue < 3.8101039)) {
              sum += (float)-0.029916994;
            } else {
              sum += (float)0.011943104;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.74391061)) {
          if (  (data[10].fvalue < 0.66869152)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0015846833;
            } else {
              sum += (float)0.0085213492;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.095160596;
            } else {
              sum += (float)-0.012504712;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.42620531)) {
            if (  (data[18].fvalue < 1.4931175)) {
              sum += (float)-0.0035587354;
            } else {
              sum += (float)-0.010662474;
            }
          } else {
            if (  (data[8].fvalue < 3.7574)) {
              sum += (float)0.022892343;
            } else {
              sum += (float)0.0067348974;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[8].fvalue < 0.56345814)) {
          if (  (data[18].fvalue < 0.13985376)) {
            if (  (data[20].fvalue < 0.021443423)) {
              sum += (float)-0.021941829;
            } else {
              sum += (float)0.073412895;
            }
          } else {
            if (  (data[10].fvalue < 0.097810805)) {
              sum += (float)0.0071896943;
            } else {
              sum += (float)0.11777116;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.021443423)) {
            if (  (data[18].fvalue < 0.55684644)) {
              sum += (float)-0.034144051;
            } else {
              sum += (float)0.013413636;
            }
          } else {
            if (  (data[20].fvalue < 0.1456012)) {
              sum += (float)0.09195701;
            } else {
              sum += (float)0.014534294;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.55684644)) {
          if (  (data[10].fvalue < 0.88241512)) {
            if (  (data[18].fvalue < 0.15273146)) {
              sum += (float)-0.052088749;
            } else {
              sum += (float)-0.019045206;
            }
          } else {
            if (  (data[8].fvalue < 0.72868246)) {
              sum += (float)0.31725645;
            } else {
              sum += (float)-0.05610602;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.025649723)) {
            if (  (data[8].fvalue < 2.9633932)) {
              sum += (float)0.04373664;
            } else {
              sum += (float)0.0078561902;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.013502377;
            } else {
              sum += (float)-0.01463408;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.032586489)) {
    if (  (data[10].fvalue < 0.38371462)) {
      if (  (data[19].fvalue < 0.0095704524)) {
        if (  (data[18].fvalue < 0.003230379)) {
          if (  (data[10].fvalue < 0.18680285)) {
            if (  (data[9].fvalue < 1.1813492)) {
              sum += (float)-0.068335548;
            } else {
              sum += (float)0.0052661994;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.039831948;
            } else {
              sum += (float)-0.55744135;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99851626)) {
            if (  (data[18].fvalue < 0.033951152)) {
              sum += (float)0.015038132;
            } else {
              sum += (float)0.15812793;
            }
          } else {
            if (  (data[10].fvalue < 0.069515049)) {
              sum += (float)0.021249996;
            } else {
              sum += (float)-0.10079879;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.0402095)) {
          if (  (data[19].fvalue < 0.03970417)) {
            if (  (data[18].fvalue < 0.05536573)) {
              sum += (float)0.014349287;
            } else {
              sum += (float)0.33817208;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.028463038;
            } else {
              sum += (float)0.27022561;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.087251686)) {
            if (  (data[18].fvalue < 0.003230379)) {
              sum += (float)0.0466718;
            } else {
              sum += (float)-0.093930691;
            }
          } else {
            if (  (data[9].fvalue < 1.3842986)) {
              sum += (float)0.053131308;
            } else {
              sum += (float)-0.026010638;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.044495989)) {
        if (  (data[19].fvalue < 0.029203219)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.39636496)) {
              sum += (float)0.50108403;
            } else {
              sum += (float)0.63742191;
            }
          } else {
            if (  (data[10].fvalue < 0.41239741)) {
              sum += (float)0.057932872;
            } else {
              sum += (float)0.57201827;
            }
          }
        } else {
          sum += (float)-0.22534995;
        }
      } else {
        sum += (float)-1.0120287;
      }
    }
  } else {
    if (  (data[19].fvalue < 6.8376908)) {
      if (  (data[19].fvalue < 0.42101094)) {
        if (  (data[18].fvalue < 0.47269586)) {
          if (  (data[9].fvalue < 1.9462849)) {
            if (  (data[8].fvalue < 0.72957635)) {
              sum += (float)-0.0084377155;
            } else {
              sum += (float)-0.028068619;
            }
          } else {
            if (  (data[10].fvalue < 0.32855251)) {
              sum += (float)0.0047887741;
            } else {
              sum += (float)-0.019584831;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.3257058)) {
            if (  (data[8].fvalue < 0.4086366)) {
              sum += (float)0.2927801;
            } else {
              sum += (float)0.028162198;
            }
          } else {
            if (  (data[18].fvalue < 7.6723623)) {
              sum += (float)0.0042225886;
            } else {
              sum += (float)-0.018882316;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.497735)) {
          if (  (data[8].fvalue < 0.4086366)) {
            if (  (data[18].fvalue < 0.2153728)) {
              sum += (float)0.065491907;
            } else {
              sum += (float)0.39102122;
            }
          } else {
            if (  (data[8].fvalue < 1.241287)) {
              sum += (float)0.03777384;
            } else {
              sum += (float)-0.0010085838;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.33581373)) {
            if (  (data[8].fvalue < 0.23274727)) {
              sum += (float)-0.025038175;
            } else {
              sum += (float)0.0031114276;
            }
          } else {
            if ( (data[9].fvalue < 5.6039867)) {
              sum += (float)-0.013936775;
            } else {
              sum += (float)0.0029196809;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.003230379)) {
        if (  (data[9].fvalue < 5.985846)) {
          if (  (data[10].fvalue < 0.78358245)) {
            if (  (data[9].fvalue < 4.25986)) {
              sum += (float)-0.0305629;
            } else {
              sum += (float)0.0045353002;
            }
          } else {
            if (  (data[9].fvalue < 0.96994513)) {
              sum += (float)0.31124666;
            } else {
              sum += (float)0.088368624;
            }
          }
        } else {
          if (  (data[9].fvalue < 6.9739275)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.038534738;
            } else {
              sum += (float)0.013247737;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.078182928;
            } else {
              sum += (float)0.047430247;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.24657516)) {
          if (  (data[8].fvalue < 16.944098)) {
            if (  (data[18].fvalue < 0.033951152)) {
              sum += (float)-0.11320975;
            } else {
              sum += (float)-0.070343196;
            }
          } else {
            if ( (data[9].fvalue < 2.2377949)) {
              sum += (float)0.21342249;
            } else {
              sum += (float)-0.01733076;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.2713519)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.053751402;
            } else {
              sum += (float)0.0048335488;
            }
          } else {
            if ( (data[9].fvalue < 2.6432858)) {
              sum += (float)0.12274949;
            } else {
              sum += (float)-0.023936627;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 7.5592799)) {
    if (  (data[9].fvalue < 1.2272228)) {
      if (  (data[13].fvalue < 4.999999)) {
        if (  (data[22].fvalue < 0.053822957)) {
          if (  (data[10].fvalue < 0.62667406)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.008672893;
            } else {
              sum += (float)0.023293929;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0026828777;
            } else {
              sum += (float)0.015322101;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.126569)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.039728642;
            } else {
              sum += (float)0.11088207;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.040086478;
            } else {
              sum += (float)-0.11379986;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.58835262)) {
              sum += (float)0.18403858;
            } else {
              sum += (float)0.10995883;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.063608147;
            } else {
              sum += (float)0.3422122;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.019237852;
            } else {
              sum += (float)0.10731197;
            }
          } else {
            if (  (data[22].fvalue < 0.58751673)) {
              sum += (float)0.15046564;
            } else {
              sum += (float)-0.088002495;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 4.0399075)) {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[10].fvalue < 0.2623699)) {
              sum += (float)-0.0029344892;
            } else {
              sum += (float)0.002109915;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.020822098;
            } else {
              sum += (float)0.0045082346;
            }
          }
        } else {
          if (  (data[13].fvalue < 5.999999)) {
            if ( (data[9].fvalue < 2.1621006)) {
              sum += (float)-0.07422708;
            } else {
              sum += (float)0.0018622845;
            }
          } else {
            if (  (data[9].fvalue < 2.7305937)) {
              sum += (float)-0.022031873;
            } else {
              sum += (float)0.094509102;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.3480179)) {
              sum += (float)-0.030353555;
            } else {
              sum += (float)-0.010919565;
            }
          } else {
            if (  (data[22].fvalue < 0.95116317)) {
              sum += (float)0.010046176;
            } else {
              sum += (float)-0.0022470374;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.024757097;
            } else {
              sum += (float)0.0046177357;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.010162503;
            } else {
              sum += (float)0.0068519767;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[16].fvalue < 6.999999)) {
        if (  (data[14].fvalue < 0.99999899)) {
          if ( (data[9].fvalue < 5.5888491)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.034912787;
            } else {
              sum += (float)-0.0017989581;
            }
          } else {
            if (  (data[22].fvalue < 19.995344)) {
              sum += (float)0.031687338;
            } else {
              sum += (float)0.0028984861;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.0149949)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.05532473;
            } else {
              sum += (float)-0.21889912;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.007547203;
            } else {
              sum += (float)-0.01398046;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 19.995344)) {
          if (  (data[10].fvalue < 0.56059307)) {
            if (  (data[22].fvalue < 8.47754)) {
              sum += (float)0.13777347;
            } else {
              sum += (float)0.066051073;
            }
          } else {
            sum += (float)-0.078138344;
          }
        } else {
          if (  (data[16].fvalue < 8.999999)) {
            if (  (data[6].fvalue < 6.8427315)) {
              sum += (float)-0.015267508;
            } else {
              sum += (float)0.017564563;
            }
          } else {
            if (  (data[6].fvalue < 5.5952377)) {
              sum += (float)0.03751339;
            } else {
              sum += (float)0.13304025;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 1.999999)) {
        if ( (data[9].fvalue < 6.3597546)) {
          if (  (data[9].fvalue < 4.0399075)) {
            if (  (data[10].fvalue < 0.72064829)) {
              sum += (float)0.018395299;
            } else {
              sum += (float)-0.056882042;
            }
          } else {
            if (  (data[6].fvalue < 16.980328)) {
              sum += (float)-0.021239595;
            } else {
              sum += (float)-0.0092404168;
            }
          }
        } else {
          if (  (data[22].fvalue < 19.995344)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.0063926843;
            } else {
              sum += (float)0.015932243;
            }
          } else {
            if (  (data[10].fvalue < 0.019043254)) {
              sum += (float)0.040921345;
            } else {
              sum += (float)-0.016086562;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.31788576)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.016307579;
            } else {
              sum += (float)0.070323072;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)-0.0039945841;
            } else {
              sum += (float)0.05464109;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 0.45719054)) {
              sum += (float)0.021274459;
            } else {
              sum += (float)-0.010709522;
            }
          } else {
            if (  (data[9].fvalue < 4.8554006)) {
              sum += (float)0.065341301;
            } else {
              sum += (float)0.028002674;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.27508029)) {
    if (  (data[9].fvalue < 0.92523086)) {
      if (  (data[19].fvalue < 0.051642027)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.19003858)) {
            if (  (data[8].fvalue < 0.11420116)) {
              sum += (float)0.7849232;
            } else {
              sum += (float)0.22068274;
            }
          } else {
            if (  (data[19].fvalue < 0.029790901)) {
              sum += (float)-0.025310684;
            } else {
              sum += (float)0.14895771;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.11420116)) {
              sum += (float)0.46529734;
            } else {
              sum += (float)0.057194028;
            }
          } else {
            if (  (data[19].fvalue < 0.029790901)) {
              sum += (float)-0.24761362;
            } else {
              sum += (float)-0.041895442;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 0.23201124)) {
            if (  (data[19].fvalue < 0.08736109)) {
              sum += (float)0.51725292;
            } else {
              sum += (float)0.051801756;
            }
          } else {
            if (  (data[19].fvalue < 0.075100042)) {
              sum += (float)0.14443;
            } else {
              sum += (float)0.46823245;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.11404352)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.014665101;
            } else {
              sum += (float)0.24394056;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.57883561;
            } else {
              sum += (float)0.84082711;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.11404352)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[19].fvalue < 0.0097674355)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0012227357;
            } else {
              sum += (float)-0.097001828;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.036902428;
            } else {
              sum += (float)-0.023878463;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.1184397)) {
              sum += (float)0.091175318;
            } else {
              sum += (float)-0.15653048;
            }
          } else {
            if (  (data[19].fvalue < 0.019914769)) {
              sum += (float)-0.59815001;
            } else {
              sum += (float)-0.12512344;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.0428706)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.11875051;
            } else {
              sum += (float)0.90694559;
            }
          } else {
            if (  (data[8].fvalue < 0.1511084)) {
              sum += (float)0.46125436;
            } else {
              sum += (float)0.16212893;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 2.8095393)) {
              sum += (float)-0.021235829;
            } else {
              sum += (float)0.00728931;
            }
          } else {
            if (  (data[9].fvalue < 1.6174923)) {
              sum += (float)0.049744539;
            } else {
              sum += (float)-0.020997815;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.4977953)) {
      if (  (data[19].fvalue < 0.67550403)) {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[22].fvalue < 0.053352259)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0024661247;
            } else {
              sum += (float)-0.013533713;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.00038470645;
            } else {
              sum += (float)0.042121444;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.1826264)) {
            if (  (data[8].fvalue < 0.61788434)) {
              sum += (float)0.086104795;
            } else {
              sum += (float)-0.06318669;
            }
          } else {
            if (  (data[19].fvalue < 0.33791384)) {
              sum += (float)-0.14969462;
            } else {
              sum += (float)-0.045710191;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.61788434)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.1738098;
            } else {
              sum += (float)0.08054246;
            }
          } else {
            if (  (data[9].fvalue < 1.3256357)) {
              sum += (float)0.57474154;
            } else {
              sum += (float)0.23683326;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.640063)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)0.015183696;
            } else {
              sum += (float)0.14022185;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.00025457356;
            } else {
              sum += (float)-0.19586922;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.99999899)) {
        if (  (data[9].fvalue < 3.2464077)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.022981351)) {
              sum += (float)0.0066006207;
            } else {
              sum += (float)-0.011470846;
            }
          } else {
            if (  (data[19].fvalue < 0.075100042)) {
              sum += (float)0.14066984;
            } else {
              sum += (float)0.0022634827;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.80014968)) {
              sum += (float)0.0045593502;
            } else {
              sum += (float)0.063613541;
            }
          } else {
            if (  (data[19].fvalue < 0.28549051)) {
              sum += (float)0.051788501;
            } else {
              sum += (float)-0.017589528;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.0097674355)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.022981351)) {
              sum += (float)-0.011304404;
            } else {
              sum += (float)0.0067655114;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.08492016;
            } else {
              sum += (float)-0.47642446;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.019914769)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.030755324;
            } else {
              sum += (float)-0.67122155;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.0048936526;
            } else {
              sum += (float)0.0011294526;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < -1e-06)) {
    sum += (float)0.49746004;
  } else {
    if (  (data[12].fvalue < 18.999998)) {
      if (  (data[20].fvalue < 0.32863018)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.23091568)) {
              sum += (float)-0.0036829701;
            } else {
              sum += (float)0.0041179815;
            }
          } else {
            if (  (data[19].fvalue < 0.010167383)) {
              sum += (float)0.0040709134;
            } else {
              sum += (float)-0.060620375;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.03077573;
            } else {
              sum += (float)0.014003245;
            }
          } else {
            if (  (data[20].fvalue < 0.1199268)) {
              sum += (float)0.040363017;
            } else {
              sum += (float)-0.022094671;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.012817401;
            } else {
              sum += (float)0.0028484794;
            }
          } else {
            if (  (data[19].fvalue < 0.010167383)) {
              sum += (float)0.0040965807;
            } else {
              sum += (float)-0.022140667;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.3177335)) {
              sum += (float)0.016542058;
            } else {
              sum += (float)-0.037671737;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.0034769061;
            } else {
              sum += (float)0.036285177;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.75445044)) {
        if (  (data[19].fvalue < 0.30115774)) {
          sum += (float)-0.43984795;
        } else {
          if (  (data[18].fvalue < 0.78844196)) {
            sum += (float)0.056601804;
          } else {
            if (  (data[18].fvalue < 2.2547889)) {
              sum += (float)-0.31407312;
            } else {
              sum += (float)-0.074818812;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 4.999999)) {
          if (  (data[20].fvalue < 0.090488806)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.016895354;
            } else {
              sum += (float)0.10380322;
            }
          } else {
            if (  (data[20].fvalue < 2.0225008)) {
              sum += (float)-0.15714379;
            } else {
              sum += (float)-0.018351704;
            }
          }
        } else {
          sum += (float)0.20169498;
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.11512933)) {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[17].fvalue < 1.999999)) {
        if (  (data[11].fvalue < 0.65651757)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.0010067836;
            } else {
              sum += (float)-0.12268621;
            }
          } else {
            if (  (data[20].fvalue < 0.05146062)) {
              sum += (float)0.0073508611;
            } else {
              sum += (float)0.040794592;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.14783177;
            } else {
              sum += (float)-0.016864613;
            }
          } else {
            if (  (data[20].fvalue < 0.05146062)) {
              sum += (float)-0.057414319;
            } else {
              sum += (float)0.011776428;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.25680313)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.096661739;
            } else {
              sum += (float)0.12908547;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.32921657;
            } else {
              sum += (float)0.024706822;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.073946163)) {
              sum += (float)-0.20768233;
            } else {
              sum += (float)-0.052413665;
            }
          } else {
            if (  (data[22].fvalue < 0.27387226)) {
              sum += (float)-0.49476615;
            } else {
              sum += (float)-0.22202618;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.021328654)) {
        if (  (data[22].fvalue < 0.024833372)) {
          if (  (data[11].fvalue < 0.011360062)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.031378236;
            } else {
              sum += (float)0.11435544;
            }
          } else {
            if (  (data[7].fvalue < 0.99992031)) {
              sum += (float)0.017331844;
            } else {
              sum += (float)-0.035261951;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.17993543;
            } else {
              sum += (float)0.36773166;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)0.051991068;
            } else {
              sum += (float)0.15701978;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.024833372)) {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.05455561)) {
              sum += (float)0.21144937;
            } else {
              sum += (float)0.10133661;
            }
          } else {
            if (  (data[20].fvalue < 0.42751908)) {
              sum += (float)-0.096802682;
            } else {
              sum += (float)-0.41744491;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.078448415;
            } else {
              sum += (float)0.019833637;
            }
          } else {
            if (  (data[11].fvalue < 0.57415432)) {
              sum += (float)0.040119685;
            } else {
              sum += (float)-0.059041433;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 0.05146062)) {
      if (  (data[12].fvalue < 9.999999)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 0.024833372)) {
              sum += (float)0.0013916718;
            } else {
              sum += (float)-0.0051188637;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.0081215501;
            } else {
              sum += (float)0.043666638;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.011360062)) {
              sum += (float)0.050669733;
            } else {
              sum += (float)0.00029797299;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.090688437;
            } else {
              sum += (float)-0.007360816;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)-0.1631081;
            } else {
              sum += (float)-0.38744909;
            }
          } else {
            if (  (data[11].fvalue < 0.20176663)) {
              sum += (float)-0.023381665;
            } else {
              sum += (float)0.029288054;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)-0.0018140612;
            } else {
              sum += (float)-0.27463502;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)0.059639499;
            } else {
              sum += (float)0.018977238;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 6.999999)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.0044860314;
            } else {
              sum += (float)-0.013031027;
            }
          } else {
            if (  (data[20].fvalue < 0.96822947)) {
              sum += (float)-0.032244567;
            } else {
              sum += (float)0.006779864;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.024833372)) {
            if (  (data[20].fvalue < 0.55179155)) {
              sum += (float)0.038079523;
            } else {
              sum += (float)0.013255219;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.022467984;
            } else {
              sum += (float)0.0028988621;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.17744045;
            } else {
              sum += (float)-0.10208456;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.14491704;
            } else {
              sum += (float)-0.02832038;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.070577689;
            } else {
              sum += (float)0.0034574282;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)0.023969984;
            } else {
              sum += (float)0.0058237179;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 6.9011502)) {
    if (  (data[19].fvalue < 0.18518725)) {
      if (  (data[18].fvalue < 0.11338972)) {
        if (  (data[10].fvalue < 0.32817623)) {
          if (  (data[8].fvalue < 0.93016005)) {
            if (  (data[8].fvalue < 0.11149464)) {
              sum += (float)-0.00071879313;
            } else {
              sum += (float)0.017279625;
            }
          } else {
            if (  (data[21].fvalue < 0.067076162)) {
              sum += (float)-0.029774759;
            } else {
              sum += (float)-0.0073652216;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.065619156)) {
              sum += (float)0.034028124;
            } else {
              sum += (float)-0.080867253;
            }
          } else {
            if (  (data[8].fvalue < 0.93016005)) {
              sum += (float)-0.068237729;
            } else {
              sum += (float)-0.0062909094;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.72167653)) {
          if (  (data[11].fvalue < 0.2373137)) {
            if (  (data[19].fvalue < 0.03044528)) {
              sum += (float)0.25597957;
            } else {
              sum += (float)0.73026615;
            }
          } else {
            if (  (data[11].fvalue < 0.4825545)) {
              sum += (float)0.08532916;
            } else {
              sum += (float)0.014083613;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.14154747)) {
            if (  (data[19].fvalue < 0.020022415)) {
              sum += (float)0.00082990783;
            } else {
              sum += (float)0.010940866;
            }
          } else {
            if (  (data[21].fvalue < 0.024284255)) {
              sum += (float)-0.028435713;
            } else {
              sum += (float)-0.0012152652;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.28014457)) {
            if (  (data[22].fvalue < 0.055600204)) {
              sum += (float)-0.03674724;
            } else {
              sum += (float)-0.0030883269;
            }
          } else {
            if (  (data[8].fvalue < 0.61728096)) {
              sum += (float)0.053095467;
            } else {
              sum += (float)0.0077954833;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.20083812)) {
            if (  (data[18].fvalue < 0.033660073)) {
              sum += (float)0.007591011;
            } else {
              sum += (float)0.031735204;
            }
          } else {
            if (  (data[19].fvalue < 1.4862428)) {
              sum += (float)0.00631586;
            } else {
              sum += (float)-0.030029517;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.33706772)) {
            if (  (data[8].fvalue < 0.23360424)) {
              sum += (float)0.068640389;
            } else {
              sum += (float)0.015389977;
            }
          } else {
            if (  (data[18].fvalue < 7.7151065)) {
              sum += (float)-0.0085470323;
            } else {
              sum += (float)0.065124348;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.1345401)) {
            if (  (data[18].fvalue < 0.08946912)) {
              sum += (float)0.0014630681;
            } else {
              sum += (float)-0.017149163;
            }
          } else {
            if (  (data[18].fvalue < 0.23027362)) {
              sum += (float)-0.0089272391;
            } else {
              sum += (float)0.025439763;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 7.7151065)) {
      if (  (data[19].fvalue < 0.89274544)) {
        if (  (data[19].fvalue < 0.020022415)) {
          if (  (data[18].fvalue < 0.47262147)) {
            if (  (data[8].fvalue < 2.4884729)) {
              sum += (float)-0.014339504;
            } else {
              sum += (float)0.015103307;
            }
          } else {
            if (  (data[10].fvalue < 0.2708483)) {
              sum += (float)-0.023783749;
            } else {
              sum += (float)-0.0028084633;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.33884293)) {
            if (  (data[18].fvalue < 0.18347137)) {
              sum += (float)-0.0070750061;
            } else {
              sum += (float)0.010456619;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0059452332;
            } else {
              sum += (float)-0.021374164;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.88564098)) {
          if (  (data[18].fvalue < 0.23027362)) {
            if (  (data[8].fvalue < 5.3615255)) {
              sum += (float)-0.024940606;
            } else {
              sum += (float)-0.01010396;
            }
          } else {
            if (  (data[22].fvalue < 0.27402386)) {
              sum += (float)-0.017410889;
            } else {
              sum += (float)0.0099195521;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.0292531)) {
            if (  (data[22].fvalue < 0.023122644)) {
              sum += (float)-0.058816005;
            } else {
              sum += (float)-0.0049383896;
            }
          } else {
            if (  (data[18].fvalue < 2.4203618)) {
              sum += (float)-0.046764955;
            } else {
              sum += (float)-0.08384759;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.26793301)) {
        if (  (data[10].fvalue < 0.78295451)) {
          if (  (data[5].fvalue < 29.083847)) {
            if (  (data[19].fvalue < 0.0099035725)) {
              sum += (float)-0.087455913;
            } else {
              sum += (float)0.0071034785;
            }
          } else {
            if (  (data[11].fvalue < 0.95861417)) {
              sum += (float)0.0012800267;
            } else {
              sum += (float)0.040421776;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.055600204)) {
            sum += (float)0.32913578;
          } else {
            if (  (data[5].fvalue < 42.346771)) {
              sum += (float)0.12028163;
            } else {
              sum += (float)0.0099970298;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.2386079)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.089504659;
            } else {
              sum += (float)0.15925273;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.14599589;
            } else {
              sum += (float)0.074305639;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 20.317694)) {
              sum += (float)0.045904905;
            } else {
              sum += (float)0.13503991;
            }
          } else {
            if (  (data[10].fvalue < 0.49211803)) {
              sum += (float)0.050261609;
            } else {
              sum += (float)-0.034379501;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.41217861)) {
    if (  (data[18].fvalue < 0.055917609)) {
      if (  (data[19].fvalue < 0.029762171)) {
        if (  (data[8].fvalue < 0.030907577)) {
          if (  (data[11].fvalue < 0.46375757)) {
            if (  (data[6].fvalue < 0.2521854)) {
              sum += (float)0.026863074;
            } else {
              sum += (float)0.0029124455;
            }
          } else {
            if (  (data[18].fvalue < 0.034228016)) {
              sum += (float)-0.039054539;
            } else {
              sum += (float)0.064184487;
            }
          }
        } else {
          if (  (data[5].fvalue < 3.3330686)) {
            if (  (data[11].fvalue < 0.63018405)) {
              sum += (float)-0.06868922;
            } else {
              sum += (float)0.10102201;
            }
          } else {
            if (  (data[11].fvalue < 0.031970873)) {
              sum += (float)-0.052509017;
            } else {
              sum += (float)0.0072762165;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.25232339)) {
          if (  (data[6].fvalue < 1.0480512)) {
            if (  (data[11].fvalue < 0.098732665)) {
              sum += (float)0.17740372;
            } else {
              sum += (float)0.034106553;
            }
          } else {
            if (  (data[19].fvalue < 0.07344725)) {
              sum += (float)-0.017800635;
            } else {
              sum += (float)0.016010156;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.5186699)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.05255492;
            } else {
              sum += (float)-0.011997423;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.03204326;
            } else {
              sum += (float)-0.016018175;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.011681868)) {
        if (  (data[5].fvalue < 22.858538)) {
          if (  (data[19].fvalue < 0.0098728165)) {
            if (  (data[6].fvalue < 10.140042)) {
              sum += (float)0.251867;
            } else {
              sum += (float)0.047441803;
            }
          } else {
            if (  (data[19].fvalue < 0.12718335)) {
              sum += (float)0.22441702;
            } else {
              sum += (float)0.62810999;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.28573713)) {
            if (  (data[5].fvalue < 32.913998)) {
              sum += (float)0.05693097;
            } else {
              sum += (float)0.0019549427;
            }
          } else {
            if (  (data[5].fvalue < 38.357212)) {
              sum += (float)0.50341594;
            } else {
              sum += (float)0.18450487;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 8.76229)) {
          if (  (data[11].fvalue < 0.063159063)) {
            if (  (data[19].fvalue < 0.26843178)) {
              sum += (float)0.056759387;
            } else {
              sum += (float)0.33242926;
            }
          } else {
            if (  (data[19].fvalue < 0.33905676)) {
              sum += (float)0.0029313171;
            } else {
              sum += (float)0.049525343;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.0054955254;
            } else {
              sum += (float)0.014556852;
            }
          } else {
            if (  (data[19].fvalue < 0.0098728165)) {
              sum += (float)0.010360973;
            } else {
              sum += (float)-0.032060642;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.024517402)) {
      if (  (data[18].fvalue < 0.016654005)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.0098728165)) {
              sum += (float)0.01023737;
            } else {
              sum += (float)0.0014749521;
            }
          } else {
            if (  (data[18].fvalue < 0.0032225708)) {
              sum += (float)0.046474524;
            } else {
              sum += (float)-0.12200721;
            }
          }
        } else {
          if (  (data[8].fvalue < 4.7487888)) {
            if (  (data[19].fvalue < 1.1365873)) {
              sum += (float)0.014219501;
            } else {
              sum += (float)-0.010582577;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.016615521;
            } else {
              sum += (float)0.015523223;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.44306633)) {
          if (  (data[19].fvalue < 0.0098728165)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.074185655;
            } else {
              sum += (float)-0.024648163;
            }
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.06416402;
            } else {
              sum += (float)0.23251782;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.791623)) {
              sum += (float)-0.0007851554;
            } else {
              sum += (float)0.046869069;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.015003812;
            } else {
              sum += (float)0.029050736;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.24682829)) {
        if (  (data[19].fvalue < 0.63842118)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.071233705)) {
              sum += (float)-0.040383589;
            } else {
              sum += (float)-0.013908749;
            }
          } else {
            if (  (data[19].fvalue < 0.0098728165)) {
              sum += (float)-0.030728096;
            } else {
              sum += (float)0.069402173;
            }
          }
        } else {
          if (  (data[19].fvalue < 3.3667257)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.021322777;
            } else {
              sum += (float)0.0046862662;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.032953233;
            } else {
              sum += (float)-0.002080695;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.57653165)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.0098728165)) {
              sum += (float)-0.0013302482;
            } else {
              sum += (float)0.019179139;
            }
          } else {
            if (  (data[19].fvalue < 0.25232339)) {
              sum += (float)0.00017882026;
            } else {
              sum += (float)-0.0099858604;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.37969887)) {
              sum += (float)0.00026940712;
            } else {
              sum += (float)-0.03317273;
            }
          } else {
            if (  (data[18].fvalue < 0.55696076)) {
              sum += (float)0.012720998;
            } else {
              sum += (float)-0.0060453918;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 0.97087723)) {
    if (  (data[8].fvalue < 0.1897624)) {
      if (  (data[18].fvalue < 0.045036186)) {
        if (  (data[19].fvalue < 0.051680531)) {
          if (  (data[10].fvalue < 0.22284786)) {
            if (  (data[10].fvalue < 0.019156201)) {
              sum += (float)-0.15540777;
            } else {
              sum += (float)0.65496868;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.071323365;
            } else {
              sum += (float)-0.060002003;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 0.074779786)) {
              sum += (float)-0.53703618;
            } else {
              sum += (float)0.27787426;
            }
          } else {
            if (  (data[19].fvalue < 0.099065639)) {
              sum += (float)-0.070495695;
            } else {
              sum += (float)0.35649192;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.32863104)) {
          sum += (float)-1.8621082;
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.0222327;
            } else {
              sum += (float)0.56384856;
            }
          } else {
            if (  (data[8].fvalue < 0.074779786)) {
              sum += (float)-0.16894026;
            } else {
              sum += (float)0.33203939;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.087170266)) {
        if (  (data[18].fvalue < 0.12838377)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.015478903;
            } else {
              sum += (float)0.25595117;
            }
          } else {
            if (  (data[18].fvalue < 0.055683035)) {
              sum += (float)-0.26613384;
            } else {
              sum += (float)-0.080608234;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 0.51275522)) {
              sum += (float)0.081290312;
            } else {
              sum += (float)-0.016848765;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.68408436;
            } else {
              sum += (float)0.2495366;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.46049932)) {
          if (  (data[18].fvalue < 0.10036854)) {
            if (  (data[19].fvalue < 0.15606141)) {
              sum += (float)0.021689676;
            } else {
              sum += (float)0.19320184;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.26552176;
            } else {
              sum += (float)0.73145306;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.25134873)) {
            if (  (data[18].fvalue < 0.12838377)) {
              sum += (float)-0.076804534;
            } else {
              sum += (float)0.041714542;
            }
          } else {
            if (  (data[8].fvalue < 1.0047239)) {
              sum += (float)0.1028831;
            } else {
              sum += (float)-0.003636783;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.18645018)) {
      if (  (data[13].fvalue < 10.999999)) {
        if (  (data[18].fvalue < 0.11326115)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[9].fvalue < 1.1243187)) {
              sum += (float)-0.023847533;
            } else {
              sum += (float)0.0070938081;
            }
          } else {
            if (  (data[9].fvalue < 1.0497897)) {
              sum += (float)-0.038698319;
            } else {
              sum += (float)-0.23317795;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.099065639)) {
              sum += (float)-1.6301276e-05;
            } else {
              sum += (float)-0.026531421;
            }
          } else {
            if (  (data[18].fvalue < 0.3356804)) {
              sum += (float)-0.03058834;
            } else {
              sum += (float)0.14875333;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.1243187)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.087170266)) {
              sum += (float)-0.17938289;
            } else {
              sum += (float)0.89445746;
            }
          } else {
            if (  (data[19].fvalue < 0.14084777)) {
              sum += (float)-0.46967965;
            } else {
              sum += (float)-0.18116511;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.14084777)) {
            if (  (data[18].fvalue < 0.40057424)) {
              sum += (float)-0.43185759;
            } else {
              sum += (float)-0.65961039;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.16306724;
            } else {
              sum += (float)-0.44705546;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.2275474)) {
        if (  (data[8].fvalue < 0.41136503)) {
          if (  (data[18].fvalue < 0.16882686)) {
            if (  (data[19].fvalue < 0.37711838)) {
              sum += (float)0.0053730747;
            } else {
              sum += (float)0.30874071;
            }
          } else {
            if (  (data[18].fvalue < 0.23159117)) {
              sum += (float)0.31028003;
            } else {
              sum += (float)0.54624307;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.3803387)) {
            if (  (data[19].fvalue < 0.3382799)) {
              sum += (float)-0.046045762;
            } else {
              sum += (float)0.014803547;
            }
          } else {
            if (  (data[8].fvalue < 0.86458313)) {
              sum += (float)0.40040219;
            } else {
              sum += (float)0.015410251;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.5653773)) {
          if (  (data[18].fvalue < 0.32007971)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.030772613;
            } else {
              sum += (float)0.0054738275;
            }
          } else {
            if (  (data[9].fvalue < 1.4397238)) {
              sum += (float)0.23535638;
            } else {
              sum += (float)-0.012973593;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.12838377)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.0042745909;
            } else {
              sum += (float)-0.035054598;
            }
          } else {
            if (  (data[9].fvalue < 1.6178802)) {
              sum += (float)-0.025468741;
            } else {
              sum += (float)-0.00074592925;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 1.0474184)) {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[9].fvalue < 0.99899215)) {
        if (  (data[5].fvalue < 0.75562662)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.97193152)) {
              sum += (float)0.024368122;
            } else {
              sum += (float)-0.0010178628;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.021558402;
            } else {
              sum += (float)0.070711844;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.021364789)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.0053866999;
            } else {
              sum += (float)-0.059608374;
            }
          } else {
            if (  (data[20].fvalue < 0.35876906)) {
              sum += (float)-0.1351923;
            } else {
              sum += (float)-0.017858859;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[5].fvalue < 1.7834095)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.038013089;
            } else {
              sum += (float)-0.13051158;
            }
          } else {
            if (  (data[21].fvalue < 0.15178548)) {
              sum += (float)-0.058940317;
            } else {
              sum += (float)0.019186208;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 0.75562662)) {
              sum += (float)0.051282536;
            } else {
              sum += (float)-0.017270777;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0083473632;
            } else {
              sum += (float)0.065312743;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 0.99899215)) {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 0.021364789)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.013133436;
            } else {
              sum += (float)0.097132891;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.0025659325;
            } else {
              sum += (float)0.19719684;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.086390361)) {
              sum += (float)0.17483038;
            } else {
              sum += (float)0.065865174;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.039428703;
            } else {
              sum += (float)0.38992071;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[5].fvalue < 0.75562662)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.018827777;
            } else {
              sum += (float)0.06854964;
            }
          } else {
            if (  (data[20].fvalue < 0.021364789)) {
              sum += (float)0.094750509;
            } else {
              sum += (float)-0.030392682;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.045881689;
            } else {
              sum += (float)0.0069714542;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.20894089;
            } else {
              sum += (float)0.072563738;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 0.021364789)) {
      if (  (data[16].fvalue < 0.99999899)) {
        if (  (data[21].fvalue < 0.026179697)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.035729505;
            } else {
              sum += (float)0.060595453;
            }
          } else {
            if ( (data[9].fvalue < 1.9489263)) {
              sum += (float)-0.055696942;
            } else {
              sum += (float)0.052694365;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.3859444)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.022046257;
            } else {
              sum += (float)0.10062524;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.019119006;
            } else {
              sum += (float)0.0083838319;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.9489263)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.0026378492;
            } else {
              sum += (float)0.013782986;
            }
          } else {
            if ( (data[9].fvalue < 1.5063721)) {
              sum += (float)0.073808104;
            } else {
              sum += (float)0.022544807;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.086390361)) {
            if (  (data[9].fvalue < 3.1587074)) {
              sum += (float)-0.006779111;
            } else {
              sum += (float)0.021373611;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.0065882169;
            } else {
              sum += (float)-0.030507335;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.3295317)) {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.040100269;
            } else {
              sum += (float)-0.050583996;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.1220926;
            } else {
              sum += (float)0.15327966;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.55197781)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.26523495;
            } else {
              sum += (float)0.15157899;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.11780725;
            } else {
              sum += (float)-0.19360767;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0066057825;
            } else {
              sum += (float)0.0014018412;
            }
          } else {
            if (  (data[21].fvalue < 0.10418153)) {
              sum += (float)-0.11166758;
            } else {
              sum += (float)0.0033309862;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.026179697)) {
            if (  (data[20].fvalue < 0.55197781)) {
              sum += (float)0.064655632;
            } else {
              sum += (float)0.022801351;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.0062926398;
            } else {
              sum += (float)-0.013162413;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.11575454)) {
    if (  (data[9].fvalue < 0.9994393)) {
      if (  (data[9].fvalue < 0.92663133)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[5].fvalue < 10.356125)) {
            if (  (data[5].fvalue < 0.75361478)) {
              sum += (float)0.40545025;
            } else {
              sum += (float)0.4998875;
            }
          } else {
            sum += (float)0.39627615;
          }
        } else {
          if (  (data[5].fvalue < 3.3676803)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.25885305;
            } else {
              sum += (float)-0.037519094;
            }
          } else {
            sum += (float)0.35036328;
          }
        }
      } else {
        if (  (data[5].fvalue < 10.356125)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.014090097;
            } else {
              sum += (float)0.041793138;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.0060867937;
            } else {
              sum += (float)-0.073983125;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.97104293)) {
              sum += (float)0.070237748;
            } else {
              sum += (float)-0.0094471117;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.094298013;
            } else {
              sum += (float)0.055797409;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 3.999999)) {
        if (  (data[9].fvalue < 1.87833)) {
          if (  (data[20].fvalue < 0.55320168)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.0097858272;
            } else {
              sum += (float)0.069223553;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.2118119;
            } else {
              sum += (float)0.050617959;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.011439413;
            } else {
              sum += (float)0.054811008;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.23129742;
            } else {
              sum += (float)-0.3196665;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.3312784)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.071658686)) {
              sum += (float)0.055916365;
            } else {
              sum += (float)0.18003139;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.13530518;
            } else {
              sum += (float)0.68349522;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.020645525)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.063905381;
            } else {
              sum += (float)0.028998503;
            }
          } else {
            if (  (data[9].fvalue < 1.8064851)) {
              sum += (float)0.14053142;
            } else {
              sum += (float)-0.036407404;
            }
          }
        }
      }
    }
  } else {
    if (  (data[5].fvalue < 0.75361478)) {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[9].fvalue < 0.9994393)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.070092082;
            } else {
              sum += (float)-0.13117753;
            }
          } else {
            if (  (data[8].fvalue < 0.27586213)) {
              sum += (float)-0.450791;
            } else {
              sum += (float)-0.16312538;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 1.1291921)) {
              sum += (float)-0.02630716;
            } else {
              sum += (float)0.049938675;
            }
          } else {
            if (  (data[9].fvalue < 1.1860346)) {
              sum += (float)0.076297902;
            } else {
              sum += (float)-0.049628019;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 0.97104293)) {
          if (  (data[8].fvalue < 0.36498752)) {
            if (  (data[9].fvalue < 0.92663133)) {
              sum += (float)0.13840348;
            } else {
              sum += (float)0.017829567;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.087878615;
            } else {
              sum += (float)0.087019093;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.020645525)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.042632978;
            } else {
              sum += (float)0.10793936;
            }
          } else {
            if (  (data[9].fvalue < 1.1291921)) {
              sum += (float)0.19467311;
            } else {
              sum += (float)0.08787071;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 5.999999)) {
        if (  (data[9].fvalue < 0.92663133)) {
          if (  (data[8].fvalue < 0.31895345)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.090975977;
            } else {
              sum += (float)0.01605233;
            }
          } else {
            if (  (data[8].fvalue < 0.73183835)) {
              sum += (float)0.017020188;
            } else {
              sum += (float)-0.01250939;
            }
          }
        } else {
          if ( (data[9].fvalue < 0.97104293)) {
            if (  (data[8].fvalue < 0.23498324)) {
              sum += (float)0.0022614591;
            } else {
              sum += (float)-0.030613881;
            }
          } else {
            if (  (data[9].fvalue < 1.1291921)) {
              sum += (float)0.0080995345;
            } else {
              sum += (float)-0.00048869965;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.3274477)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.039612841;
            } else {
              sum += (float)0.051376808;
            }
          } else {
            if (  (data[20].fvalue < 0.020645525)) {
              sum += (float)0.14380945;
            } else {
              sum += (float)0.59919721;
            }
          }
        } else {
          if (  (data[9].fvalue < 4.7360883)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.46662277;
            } else {
              sum += (float)-0.27068442;
            }
          } else {
            if (  (data[20].fvalue < 0.020645525)) {
              sum += (float)-0.22947483;
            } else {
              sum += (float)-0.032647371;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.0042268499)) {
    if ( (data[9].fvalue < 1.0466024)) {
      if (  (data[8].fvalue < 0.79688311)) {
        if (  (data[9].fvalue < 0.92521137)) {
          if (  (data[8].fvalue < 0.61594027)) {
            if (  (data[8].fvalue < 0.11287482)) {
              sum += (float)0.32232037;
            } else {
              sum += (float)0.0090926699;
            }
          } else {
            if (  (data[6].fvalue < 1.9439018)) {
              sum += (float)-0.083363876;
            } else {
              sum += (float)-0.053152207;
            }
          }
        } else {
          if (  (data[6].fvalue < 1.3122926)) {
            if (  (data[9].fvalue < 0.997899)) {
              sum += (float)-0.014032979;
            } else {
              sum += (float)-0.042945761;
            }
          } else {
            if (  (data[8].fvalue < 0.27709091)) {
              sum += (float)-0.053532906;
            } else {
              sum += (float)-0.025520196;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 7.9185834)) {
            if (  (data[9].fvalue < 1.0466024)) {
              sum += (float)0.021846313;
            } else {
              sum += (float)-0.38810444;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.28866419;
            } else {
              sum += (float)0.044412836;
            }
          }
        } else {
          if (  (data[8].fvalue < 4.7498345)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.082918026;
            } else {
              sum += (float)-0.23707394;
            }
          } else {
            if (  (data[9].fvalue < 1.0466024)) {
              sum += (float)0.031346105;
            } else {
              sum += (float)-0.15223616;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.32789)) {
        if (  (data[9].fvalue < 1.1273714)) {
          if (  (data[6].fvalue < 0.24975705)) {
            if (  (data[8].fvalue < 0.030858561)) {
              sum += (float)-0.39473438;
            } else {
              sum += (float)-0.11536852;
            }
          } else {
            if (  (data[8].fvalue < 0.15390247)) {
              sum += (float)0.046274256;
            } else {
              sum += (float)-0.0026514633;
            }
          }
        } else {
          if (  (data[6].fvalue < 1.0260253)) {
            if (  (data[22].fvalue < 0.073066674)) {
              sum += (float)0.10414159;
            } else {
              sum += (float)0.0034973444;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.028402273;
            } else {
              sum += (float)-0.013520484;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.0011099412;
            } else {
              sum += (float)-0.012604224;
            }
          } else {
            if (  (data[5].fvalue < 25.205538)) {
              sum += (float)-0.27614957;
            } else {
              sum += (float)-0.16116005;
            }
          }
        } else {
          if (  (data[9].fvalue < 3.0764954)) {
            if (  (data[8].fvalue < 2.6373713)) {
              sum += (float)0.054817945;
            } else {
              sum += (float)0.0019610133;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.0017968364;
            } else {
              sum += (float)0.036241431;
            }
          }
        }
      }
    }
  } else {
    if (  (data[9].fvalue < 1.1273714)) {
      if (  (data[8].fvalue < 0.86681569)) {
        if (  (data[18].fvalue < 0.21510224)) {
          if (  (data[8].fvalue < 0.27709091)) {
            if (  (data[18].fvalue < 0.10152833)) {
              sum += (float)0.019748019;
            } else {
              sum += (float)0.071198896;
            }
          } else {
            if (  (data[18].fvalue < 0.065913819)) {
              sum += (float)0.026042158;
            } else {
              sum += (float)-0.030462936;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.19545773)) {
            if (  (data[18].fvalue < 0.26349536)) {
              sum += (float)0.30020675;
            } else {
              sum += (float)0.65423346;
            }
          } else {
            if (  (data[9].fvalue < 0.92521137)) {
              sum += (float)0.14194614;
            } else {
              sum += (float)0.05928063;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.49852362)) {
          if (  (data[18].fvalue < 0.077563688)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.035743348;
            } else {
              sum += (float)-0.042501241;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.078155525;
            } else {
              sum += (float)0.023144957;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.35078)) {
            if (  (data[18].fvalue < 0.94452792)) {
              sum += (float)0.02883189;
            } else {
              sum += (float)0.13157347;
            }
          } else {
            if (  (data[18].fvalue < 1.0679243)) {
              sum += (float)-0.1175022;
            } else {
              sum += (float)0.0036251077;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.32789)) {
        if (  (data[18].fvalue < 0.10152833)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.055600218)) {
              sum += (float)-0.11222913;
            } else {
              sum += (float)0.010696947;
            }
          } else {
            if (  (data[22].fvalue < 0.02589949)) {
              sum += (float)-0.0054199328;
            } else {
              sum += (float)-0.16745523;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.67395025)) {
              sum += (float)0.0083878469;
            } else {
              sum += (float)-0.044925641;
            }
          } else {
            if (  (data[8].fvalue < 0.79688311)) {
              sum += (float)0.066508971;
            } else {
              sum += (float)0.023979185;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.12711051)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.065913819)) {
              sum += (float)0.056556717;
            } else {
              sum += (float)0.0089729177;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.019067302;
            } else {
              sum += (float)-0.0082114013;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.49852362)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.0028452657;
            } else {
              sum += (float)-0.026226362;
            }
          } else {
            if (  (data[9].fvalue < 1.7373387)) {
              sum += (float)0.015170337;
            } else {
              sum += (float)0.00054571388;
            }
          }
        }
      }
    }
  }
  sum = sum + (float)(-0);
  if (!pred_margin) {
    return pred_transform(sum);
  } else {
    return sum;
  }
}
};
