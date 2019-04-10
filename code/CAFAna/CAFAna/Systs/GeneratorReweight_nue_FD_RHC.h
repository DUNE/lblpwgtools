#include "BDTReweighter.h"
class GeneratorReweight_nue_FD_RHC : public BDTReweighter {
 public :
 GeneratorReweight_nue_FD_RHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_nue_FD_RHC() {;}


size_t get_num_output_group(void) {
  return 1;
}

size_t get_num_feature(void) {
  return 22;
}

static inline float pred_transform(float margin) {
  const float alpha = (float)1;
  return 1.0f / (1 + expf(-alpha * margin));
}

   float predict(union BDTReweighter::BDTReweighterFeature* data, int pred_margin) {

  float sum = 0.0f;
  unsigned int tmp;
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.040689636)) {
      if ( (data[13].fvalue < 1.3590466)) {
        if ( (data[5].fvalue < 1.2952453)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.012271116)) {
              sum += (float)-0.097259551;
            } else {
              sum += (float)-0.23192705;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.091843516;
            } else {
              sum += (float)-0.22362699;
            }
          }
        } else {
          if ( (data[4].fvalue < 0.85937369)) {
            sum += (float)0.15528479;
          } else {
            sum += (float)0.036261037;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.77884632)) {
          if ( (data[15].fvalue < 0.71448171)) {
            if ( (data[4].fvalue < 1.6580031)) {
              sum += (float)-0.19911955;
            } else {
              sum += (float)-0.42932412;
            }
          } else {
            sum += (float)-0.17364188;
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.0099927057)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.20848191)) {
              sum += (float)-0.32417175;
            } else {
              sum += (float)-0.12067006;
            }
          } else {
            sum += (float)0.1438846;
          }
        } else {
          if ( (data[15].fvalue < 0.0095424671)) {
            sum += (float)-0.11710557;
          } else {
            if ( (data[14].fvalue < 0.2675122)) {
              sum += (float)0.061054248;
            } else {
              sum += (float)0.26569211;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0733681)) {
            if ( (data[3].fvalue < 0.85438192)) {
              sum += (float)-0.058146432;
            } else {
              sum += (float)0.11405853;
            }
          } else {
            if ( (data[5].fvalue < 0.3232668)) {
              sum += (float)0.2735903;
            } else {
              sum += (float)0.18040624;
            }
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.11748986;
            } else {
              sum += (float)-0.057537086;
            }
          } else {
            if ( (data[3].fvalue < 0.9780798)) {
              sum += (float)0.33479667;
            } else {
              sum += (float)0.17419252;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[10].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.18331042)) {
          sum += (float)0.45010692;
        } else {
          if ( (data[14].fvalue < 0.16011719)) {
            sum += (float)0.1595204;
          } else {
            sum += (float)0.3222909;
          }
        }
      } else {
        sum += (float)0.09596429;
      }
    } else {
      if ( (data[10].fvalue < 0.5)) {
        sum += (float)0.53283161;
      } else {
        sum += (float)0.3070429;
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.13165462)) {
      if ( (data[13].fvalue < 1.755169)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.015043844)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.089472815;
            } else {
              sum += (float)0.013736071;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.29336256;
            } else {
              sum += (float)0.1015104;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[14].fvalue < 0.042257652)) {
              sum += (float)-0.31582454;
            } else {
              sum += (float)-0.16521055;
            }
          } else {
            sum += (float)0.11548723;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.50334549)) {
          if ( (data[5].fvalue < 0.51655328)) {
            sum += (float)-0.33406124;
          } else {
            sum += (float)-0.20386706;
          }
        } else {
          sum += (float)-0.075161867;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.017352395)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.2648378)) {
            if ( (data[4].fvalue < 1.3391092)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.25265712;
            }
          } else {
            if ( (data[5].fvalue < 0.31478316)) {
              sum += (float)0.032593142;
            } else {
              sum += (float)-0.10443237;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.35704246)) {
              sum += (float)0.12375892;
            } else {
              sum += (float)0.23912565;
            }
          } else {
            sum += (float)-0.20409739;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.52578801)) {
            sum += (float)0.20146579;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.14485832;
            } else {
              sum += (float)0.017809853;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.34971878)) {
            if ( (data[8].fvalue < 6.5)) {
              sum += (float)0.095364623;
            } else {
              sum += (float)0.26185805;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.052678946;
            } else {
              sum += (float)0.090210155;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[16].fvalue < 0.010512327)) {
        if ( (data[13].fvalue < 0.13722633)) {
          sum += (float)0.3980023;
        } else {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[14].fvalue < 0.2260246)) {
              sum += (float)0.074782975;
            } else {
              sum += (float)0.22933574;
            }
          } else {
            sum += (float)0.27676421;
          }
        }
      } else {
        sum += (float)0.093043208;
      }
    } else {
      sum += (float)0.40225673;
    }
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.15402168)) {
      if ( (data[13].fvalue < 1.8948656)) {
        if ( (data[5].fvalue < 1.2833512)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0140515)) {
              sum += (float)-0.052671865;
            } else {
              sum += (float)0.037026096;
            }
          } else {
            if ( (data[3].fvalue < 0.30608496)) {
              sum += (float)-0.0028937443;
            } else {
              sum += (float)-0.19399442;
            }
          }
        } else {
          if ( (data[14].fvalue < 1.3875037e-07)) {
            if ( (data[5].fvalue < 1.4334176)) {
              sum += (float)0.078909725;
            } else {
              sum += (float)0.19401932;
            }
          } else {
            sum += (float)0.030640943;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.44870338)) {
          if ( (data[13].fvalue < 2.5017715)) {
            sum += (float)-0.17873684;
          } else {
            sum += (float)-0.30421466;
          }
        } else {
          sum += (float)-0.07873778;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.2696858)) {
        if ( (data[5].fvalue < 0.397385)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.0084789917)) {
              sum += (float)0.019206466;
            } else {
              sum += (float)0.11526278;
            }
          } else {
            sum += (float)-0.077425405;
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.1153038;
            } else {
              sum += (float)-0.010009492;
            }
          } else {
            sum += (float)0.074293956;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.39319557)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.16016865;
          } else {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)-0.035570115;
            } else {
              sum += (float)0.11086147;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.42946923)) {
            if ( (data[5].fvalue < 0.31176701)) {
              sum += (float)0.11386086;
            } else {
              sum += (float)0.027692921;
            }
          } else {
            if ( (data[3].fvalue < 2.4614525)) {
              sum += (float)0.25987831;
            } else {
              sum += (float)0.13673109;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.1534745)) {
      if ( (data[8].fvalue < 7.5)) {
        if ( (data[3].fvalue < 0.52931774)) {
          sum += (float)0.30063152;
        } else {
          sum += (float)0.14934529;
        }
      } else {
        sum += (float)0.37242371;
      }
    } else {
      if ( (data[8].fvalue < 8.5)) {
        if ( (data[5].fvalue < 0.43649375)) {
          sum += (float)0.1230713;
        } else {
          sum += (float)-0;
        }
      } else {
        sum += (float)0.20767944;
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[14].fvalue < 0.22469033)) {
      if ( (data[3].fvalue < 1.0992389)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[13].fvalue < 1.1434662)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.017605131;
            } else {
              sum += (float)0.12209728;
            }
          } else {
            if (  (data[4].fvalue < 2.3159537)) {
              sum += (float)-0.19421919;
            } else {
              sum += (float)-0.072029106;
            }
          }
        } else {
          sum += (float)0.18981405;
        }
      } else {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.32142392)) {
              sum += (float)-0.19259177;
            } else {
              sum += (float)-0.091733709;
            }
          } else {
            if ( (data[17].fvalue < 1.3071074)) {
              sum += (float)-0.081176557;
            } else {
              sum += (float)0.12807338;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.37785643)) {
            sum += (float)-0;
          } else {
            sum += (float)0.088635311;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.064230159)) {
        if ( (data[15].fvalue < 0.51043427)) {
          if ( (data[14].fvalue < 1.0627174)) {
            if ( (data[3].fvalue < 0.37416375)) {
              sum += (float)0.092953674;
            } else {
              sum += (float)-0.015226933;
            }
          } else {
            if ( (data[14].fvalue < 1.7923311)) {
              sum += (float)-0.13262214;
            } else {
              sum += (float)-0.23003544;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.39527911)) {
            if ( (data[3].fvalue < 1.28883)) {
              sum += (float)0.034668483;
            } else {
              sum += (float)-0.17389899;
            }
          } else {
            if ( (data[5].fvalue < 0.48500276)) {
              sum += (float)0.11155925;
            } else {
              sum += (float)-0.0031036751;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.0095990747)) {
          if ( (data[15].fvalue < 0.45459914)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)0.062880792;
            } else {
              sum += (float)0.1669983;
            }
          } else {
            sum += (float)0.17079026;
          }
        } else {
          if ( (data[17].fvalue < 0.18591926)) {
            if ( (data[3].fvalue < 1.1213143)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.11520944;
            }
          } else {
            if ( (data[5].fvalue < 0.43378896)) {
              sum += (float)0.095476694;
            } else {
              sum += (float)-0.0077595985;
            }
          }
        }
      }
    }
  } else {
    if ( (data[10].fvalue < 0.5)) {
      if ( (data[3].fvalue < 1.1157421)) {
        sum += (float)0.31497669;
      } else {
        sum += (float)0.18442465;
      }
    } else {
      sum += (float)0.11331459;
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[13].fvalue < 1.5891978)) {
      if ( (data[5].fvalue < 0.25955892)) {
        if ( (data[14].fvalue < 0.026893983)) {
          if ( (data[13].fvalue < 0.15470889)) {
            if ( (data[17].fvalue < 0.59648871)) {
              sum += (float)-0.030895663;
            } else {
              sum += (float)-0.26272422;
            }
          } else {
            if (  (data[4].fvalue < 2.207571)) {
              sum += (float)-0;
            } else {
              sum += (float)0.1066087;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0096203573)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.041679867;
            } else {
              sum += (float)0.14978999;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.13113989;
            } else {
              sum += (float)0.0074128653;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.035319187)) {
          if ( (data[5].fvalue < 1.1937547)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.017603017;
            } else {
              sum += (float)0.10285687;
            }
          } else {
            if ( (data[14].fvalue < 1.3875037e-07)) {
              sum += (float)0.11347409;
            } else {
              sum += (float)0.022651803;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0095086936)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.16298711;
            } else {
              sum += (float)0.037956741;
            }
          } else {
            if ( (data[17].fvalue < 0.15011016)) {
              sum += (float)-0.15319583;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.51831335)) {
        if ( (data[5].fvalue < 0.60403216)) {
          if ( (data[15].fvalue < 0.51286507)) {
            sum += (float)-0.24616383;
          } else {
            sum += (float)-0.073953703;
          }
        } else {
          sum += (float)-0.050236493;
        }
      } else {
        sum += (float)0.0056865565;
      }
    }
  } else {
    if ( (data[3].fvalue < 0.71645057)) {
      if ( (data[7].fvalue < 7.5)) {
        sum += (float)0.17219073;
      } else {
        sum += (float)0.29815435;
      }
    } else {
      if ( (data[7].fvalue < 9.5)) {
        if ( (data[8].fvalue < 3.5)) {
          sum += (float)0.13183966;
        } else {
          if ( (data[15].fvalue < 0.21167289)) {
            sum += (float)-0.046212997;
          } else {
            sum += (float)0.0519838;
          }
        }
      } else {
        sum += (float)0.23115627;
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[13].fvalue < 2.0742421)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.0064218142)) {
            if ( (data[15].fvalue < 2.6553397)) {
              sum += (float)0.013137585;
            } else {
              sum += (float)-0.18215476;
            }
          } else {
            if (  (data[4].fvalue < 2.1059413)) {
              sum += (float)-0.12122054;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.031999949)) {
            if ( (data[15].fvalue < 0.55941713)) {
              sum += (float)0.018069776;
            } else {
              sum += (float)0.12755093;
            }
          } else {
            if ( (data[6].fvalue < 0.70360655)) {
              sum += (float)0.050454337;
            } else {
              sum += (float)-0.0099557508;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.4849722)) {
          sum += (float)-0.2204894;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if (  (data[4].fvalue < 2.6698108)) {
        if ( (data[15].fvalue < 0.035263941)) {
          sum += (float)-0.11098749;
        } else {
          sum += (float)-0.21354119;
        }
      } else {
        sum += (float)-0.042353876;
      }
    }
  } else {
    if ( (data[17].fvalue < 0.0030513327)) {
      if ( (data[3].fvalue < 0.80283296)) {
        sum += (float)0.28199688;
      } else {
        sum += (float)0.15552948;
      }
    } else {
      sum += (float)0.046136353;
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[14].fvalue < 0.012273634)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.012876337)) {
            if ( (data[13].fvalue < 2.6308294e-06)) {
              sum += (float)0.39982381;
            } else {
              sum += (float)-0.012068179;
            }
          } else {
            if ( (data[5].fvalue < 0.098031044)) {
              sum += (float)-0.009629691;
            } else {
              sum += (float)-0.11594775;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.0558532)) {
              sum += (float)0.020081608;
            } else {
              sum += (float)0.13984168;
            }
          } else {
            sum += (float)-0.21403413;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.064650729)) {
          if ( (data[14].fvalue < 0.17585886)) {
            if ( (data[15].fvalue < 0.90311241)) {
              sum += (float)-0.069435827;
            } else {
              sum += (float)-0.25434297;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.021011189;
            } else {
              sum += (float)0.047086753;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0796812)) {
              sum += (float)-0.01307577;
            } else {
              sum += (float)0.15962628;
            }
          } else {
            if ( (data[14].fvalue < 0.32840508)) {
              sum += (float)-0.024027413;
            } else {
              sum += (float)0.082997873;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 4.5)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[4].fvalue < 1.3278413)) {
            sum += (float)-0.035701595;
          } else {
            sum += (float)0.073681511;
          }
        } else {
          if ( (data[14].fvalue < 0.062970325)) {
            if ( (data[13].fvalue < 0.17836662)) {
              sum += (float)-0.026905861;
            } else {
              sum += (float)-0.28601503;
            }
          } else {
            if (  (data[4].fvalue < 1.4560094)) {
              sum += (float)-0.13635877;
            } else {
              sum += (float)-0.04354351;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.52180576)) {
          sum += (float)0.14343819;
        } else {
          sum += (float)-0.036048148;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.12849584)) {
      sum += (float)0.26894066;
    } else {
      if ( (data[7].fvalue < 9.5)) {
        if ( (data[16].fvalue < 0.0031962437)) {
          if ( (data[14].fvalue < 0.096940547)) {
            sum += (float)-5.3307213e-05;
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.15953368;
            } else {
              sum += (float)0.065059744;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.15415356)) {
            sum += (float)-0.11595421;
          } else {
            if ( (data[17].fvalue < 0.0018610877)) {
              sum += (float)0.081876971;
            } else {
              sum += (float)-0.017476112;
            }
          }
        }
      } else {
        sum += (float)0.21640487;
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[17].fvalue < 0.03607389)) {
      if ( (data[14].fvalue < 0.30944133)) {
        if ( (data[14].fvalue < 1.3875037e-07)) {
          if ( (data[13].fvalue < 0.034253307)) {
            if ( (data[15].fvalue < 0.0090691466)) {
              sum += (float)0.33226505;
            } else {
              sum += (float)-0.042331401;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0086279586;
            } else {
              sum += (float)0.055614136;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0068663969)) {
            sum += (float)-0.20949307;
          } else {
            if ( (data[15].fvalue < 0.00038027513)) {
              sum += (float)-0.054823641;
            } else {
              sum += (float)0.026858931;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.36567882)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.0096478099)) {
              sum += (float)0.036987741;
            } else {
              sum += (float)0.12358491;
            }
          } else {
            if ( (data[13].fvalue < 0.13375133)) {
              sum += (float)-0.1703355;
            } else {
              sum += (float)-0.01061803;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.42411673)) {
            sum += (float)0.041438106;
          } else {
            sum += (float)0.17916641;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.20178522)) {
        if ( (data[15].fvalue < 1.3000481)) {
          if ( (data[13].fvalue < 0.98954058)) {
            if ( (data[5].fvalue < 0.14718664)) {
              sum += (float)-0.010709484;
            } else {
              sum += (float)-0.11083543;
            }
          } else {
            sum += (float)-0.21361491;
          }
        } else {
          sum += (float)-0;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.0072399704)) {
            if ( (data[13].fvalue < 0.20088932)) {
              sum += (float)-0.22084695;
            } else {
              sum += (float)-0.051848963;
            }
          } else {
            if ( (data[13].fvalue < 0.010115011)) {
              sum += (float)-0.041785929;
            } else {
              sum += (float)0.088733435;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.40111476)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.14469649;
            } else {
              sum += (float)0.039497983;
            }
          } else {
            if ( (data[13].fvalue < 0.40305382)) {
              sum += (float)-0.12701486;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.0021719397)) {
      if ( (data[7].fvalue < 8.5)) {
        sum += (float)0.11095574;
      } else {
        sum += (float)0.24014583;
      }
    } else {
      sum += (float)0.021344168;
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[13].fvalue < 2.540148)) {
      if ( (data[5].fvalue < 0.23420301)) {
        if ( (data[13].fvalue < 0.14062901)) {
          if ( (data[14].fvalue < 0.051971667)) {
            if ( (data[17].fvalue < 0.55613256)) {
              sum += (float)-0.0089019639;
            } else {
              sum += (float)-0.1538817;
            }
          } else {
            if (  (data[4].fvalue < 1.6947253)) {
              sum += (float)0.084526181;
            } else {
              sum += (float)-0.0014574522;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.011161009)) {
              sum += (float)0.04091084;
            } else {
              sum += (float)0.11631566;
            }
          } else {
            if ( (data[14].fvalue < 0.0875725)) {
              sum += (float)-0.17577443;
            } else {
              sum += (float)0.038428813;
            }
          }
        }
      } else {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.42931038)) {
            if ( (data[5].fvalue < 1.1251352)) {
              sum += (float)-0.028275451;
            } else {
              sum += (float)0.050650738;
            }
          } else {
            if (  (data[4].fvalue < 2.3079944)) {
              sum += (float)0.0066849771;
            } else {
              sum += (float)0.1190896;
            }
          }
        } else {
          sum += (float)-0.16555536;
        }
      }
    } else {
      sum += (float)-0.14214437;
    }
  } else {
    if ( (data[13].fvalue < 0.36711806)) {
      sum += (float)0.21669446;
    } else {
      sum += (float)0.018936079;
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[15].fvalue < 3.5674267)) {
      if ( (data[8].fvalue < 4.5)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.1719282)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.025423011;
            } else {
              sum += (float)0.055969555;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.0030939528;
            } else {
              sum += (float)0.063350156;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.06700927)) {
            sum += (float)0.11080123;
          } else {
            if ( (data[15].fvalue < 0.1638228)) {
              sum += (float)-0.088004351;
            } else {
              sum += (float)0.019842453;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.48306239)) {
          sum += (float)0.14461915;
        } else {
          if ( (data[14].fvalue < 0.30256695)) {
            sum += (float)-0.079061896;
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.1162967;
            } else {
              sum += (float)0.011660339;
            }
          }
        }
      }
    } else {
      sum += (float)-0.14374986;
    }
  } else {
    if ( (data[3].fvalue < 1.0575892)) {
      if ( (data[13].fvalue < 0.20999736)) {
        sum += (float)0.22144331;
      } else {
        if ( (data[13].fvalue < 0.59401858)) {
          sum += (float)0.04062495;
        } else {
          sum += (float)0.12659378;
        }
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        sum += (float)0.11535565;
      } else {
        if ( (data[14].fvalue < 0.48848525)) {
          sum += (float)-0.10041722;
        } else {
          sum += (float)0.0042810482;
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.0085390043)) {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[14].fvalue < 1.5825081)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[5].fvalue < 1.5178354)) {
            if ( (data[5].fvalue < 0.45162219)) {
              sum += (float)0.022264428;
            } else {
              sum += (float)-0.0027489769;
            }
          } else {
            sum += (float)0.11599333;
          }
        } else {
          sum += (float)-0.11062868;
        }
      } else {
        if ( (data[15].fvalue < 0.65361458)) {
          sum += (float)-0.14673543;
        } else {
          sum += (float)0.07460019;
        }
      }
    } else {
      sum += (float)0.18613474;
    }
  } else {
    if ( (data[15].fvalue < 0.00038481964)) {
      if ( (data[16].fvalue < 0.14099798)) {
        sum += (float)-0.26827073;
      } else {
        if ( (data[14].fvalue < 0.062101886)) {
          sum += (float)-0.14349957;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[5].fvalue < 0.41548389)) {
        if ( (data[17].fvalue < 0.037570558)) {
          if ( (data[14].fvalue < 0.0053999145)) {
            if ( (data[16].fvalue < 0.702402)) {
              sum += (float)-0.025438016;
            } else {
              sum += (float)0.037047107;
            }
          } else {
            if ( (data[16].fvalue < 0.13468027)) {
              sum += (float)-0.012082895;
            } else {
              sum += (float)0.095096789;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.48368621)) {
            if ( (data[15].fvalue < 0.29550079)) {
              sum += (float)-0.0027952397;
            } else {
              sum += (float)-0.089636378;
            }
          } else {
            if (  (data[4].fvalue < 2.3858242)) {
              sum += (float)-0.091773689;
            } else {
              sum += (float)0.066727862;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.76720065)) {
              sum += (float)-0.072473399;
            } else {
              sum += (float)-0.16143139;
            }
          } else {
            sum += (float)-0.031829651;
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[17].fvalue < 0.038570035)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[15].fvalue < 3.0457239)) {
        if ( (data[8].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0418169)) {
            if ( (data[13].fvalue < 3.1175825e-06)) {
              sum += (float)0.15768711;
            } else {
              sum += (float)-0.020443279;
            }
          } else {
            if ( (data[15].fvalue < 0.041176893)) {
              sum += (float)0.075746253;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.10319504)) {
            if ( (data[13].fvalue < 0.17076904)) {
              sum += (float)-0.0089277206;
            } else {
              sum += (float)-0.18216893;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.14097804;
            } else {
              sum += (float)-0.015516671;
            }
          }
        }
      } else {
        sum += (float)-0.12033379;
      }
    } else {
      if ( (data[6].fvalue < 0.12120566)) {
        sum += (float)0.25280806;
      } else {
        if ( (data[14].fvalue < 0.58703136)) {
          if ( (data[13].fvalue < 0.34931397)) {
            if ( (data[15].fvalue < 0.041134462)) {
              sum += (float)0.005786486;
            } else {
              sum += (float)0.14024301;
            }
          } else {
            sum += (float)-0.085325621;
          }
        } else {
          sum += (float)0.1016091;
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.6643908)) {
      if ( (data[13].fvalue < 0.89886415)) {
        if ( (data[14].fvalue < 0.30144411)) {
          if ( (data[17].fvalue < 0.13030012)) {
            if ( (data[15].fvalue < 0.06346114)) {
              sum += (float)-0.10687178;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[3].fvalue < 0.17924805)) {
              sum += (float)0.10913388;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.91635835)) {
            if ( (data[14].fvalue < 0.7339589)) {
              sum += (float)-0.074544184;
            } else {
              sum += (float)-0.1675536;
            }
          } else {
            if ( (data[3].fvalue < 1.9498144)) {
              sum += (float)0.050106071;
            } else {
              sum += (float)-0.0080079781;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 2.5532637)) {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)-0.19193321;
          } else {
            sum += (float)-0.086111173;
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.57131821)) {
        if ( (data[14].fvalue < 0.084390074)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 1.8716974)) {
              sum += (float)-0.097980708;
            } else {
              sum += (float)-0.28402159;
            }
          } else {
            sum += (float)0.058975037;
          }
        } else {
          if ( (data[16].fvalue < 0.0064067589)) {
            if ( (data[17].fvalue < 1.0642972)) {
              sum += (float)0.010376525;
            } else {
              sum += (float)0.0826241;
            }
          } else {
            sum += (float)-0.062519155;
          }
        }
      } else {
        if ( (data[17].fvalue < 1.2023606)) {
          if ( (data[5].fvalue < 0.30987328)) {
            sum += (float)0.055372894;
          } else {
            sum += (float)-0.03758103;
          }
        } else {
          if ( (data[15].fvalue < 0.044775479)) {
            sum += (float)0.20245542;
          } else {
            sum += (float)0.050225209;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[13].fvalue < 0.24646538)) {
        if ( (data[14].fvalue < 0.2892988)) {
          if ( (data[15].fvalue < 0.9910568)) {
            if ( (data[17].fvalue < 1.2957032)) {
              sum += (float)0.0011339841;
            } else {
              sum += (float)-0.15166287;
            }
          } else {
            if ( (data[5].fvalue < 0.18935713)) {
              sum += (float)-0.077080682;
            } else {
              sum += (float)-0.21745946;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.62021554)) {
            if ( (data[14].fvalue < 0.94481385)) {
              sum += (float)0.028130954;
            } else {
              sum += (float)-0.075749621;
            }
          } else {
            if ( (data[14].fvalue < 0.57475543)) {
              sum += (float)-0;
            } else {
              sum += (float)0.083854251;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 1.0441179)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[16].fvalue < 0.015615255)) {
              sum += (float)0.029392404;
            } else {
              sum += (float)-0.021274451;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.12682216;
            } else {
              sum += (float)0.0094565731;
            }
          }
        } else {
          if ( (data[15].fvalue < 1.194729)) {
            if ( (data[15].fvalue < 0.051899076)) {
              sum += (float)0.017967686;
            } else {
              sum += (float)-0.11051401;
            }
          } else {
            sum += (float)0.12538308;
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.14268719)) {
        if ( (data[4].fvalue < 1.1168356)) {
          if ( (data[3].fvalue < 0.35723007)) {
            sum += (float)0.20478736;
          } else {
            sum += (float)-0.10803524;
          }
        } else {
          if ( (data[14].fvalue < 0.21475583)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.0050293216;
            } else {
              sum += (float)-0.14483634;
            }
          } else {
            if ( (data[13].fvalue < 0.38089231)) {
              sum += (float)-0.078817673;
            } else {
              sum += (float)0.064560458;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.83585155)) {
          sum += (float)0.066034436;
        } else {
          if ( (data[5].fvalue < 0.4040814)) {
            if ( (data[16].fvalue < 0.13995993)) {
              sum += (float)-0.0076131332;
            } else {
              sum += (float)0.017729517;
            }
          } else {
            sum += (float)-0.069371849;
          }
        }
      }
    }
  } else {
    if ( (data[11].fvalue < 1.5)) {
      if (  (data[4].fvalue < 1.0638144)) {
        sum += (float)0.18096295;
      } else {
        if ( (data[15].fvalue < 0.21166803)) {
          if ( (data[13].fvalue < 0.54980707)) {
            if ( (data[3].fvalue < 0.34090126)) {
              sum += (float)0.033020247;
            } else {
              sum += (float)-0.073973931;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.15200754;
            } else {
              sum += (float)0.0053048129;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            sum += (float)0.15928432;
          } else {
            sum += (float)0.044031233;
          }
        }
      }
    } else {
      sum += (float)-0.080878608;
    }
  }
  if ( (data[16].fvalue < 0.024178986)) {
    if ( (data[17].fvalue < 1.0609968)) {
      if ( (data[8].fvalue < 7.5)) {
        if ( (data[14].fvalue < 1.4759295)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[14].fvalue < 3.2368527e-07)) {
              sum += (float)0.018430283;
            } else {
              sum += (float)-0.0077737062;
            }
          } else {
            sum += (float)-0.10687145;
          }
        } else {
          if ( (data[15].fvalue < 0.36230481)) {
            sum += (float)-0.15372245;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        sum += (float)0.12096106;
      }
    } else {
      if ( (data[13].fvalue < 0.32117218)) {
        if ( (data[14].fvalue < 0.15018728)) {
          if ( (data[17].fvalue < 2.1147332)) {
            sum += (float)-0.035514373;
          } else {
            sum += (float)-0.14312194;
          }
        } else {
          if ( (data[15].fvalue < 0.4196558)) {
            sum += (float)0.11576871;
          } else {
            sum += (float)0.010787671;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.43782425)) {
          if ( (data[15].fvalue < 0.025154214)) {
            sum += (float)0.22629669;
          } else {
            sum += (float)0.088486828;
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.10655414)) {
      if ( (data[15].fvalue < 0.10811269)) {
        sum += (float)-0.18040475;
      } else {
        if ( (data[3].fvalue < 1.1044214)) {
          sum += (float)-0.0040360666;
        } else {
          sum += (float)-0.10738388;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.020381488)) {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 2.1916769)) {
            if ( (data[13].fvalue < 0.63530278)) {
              sum += (float)-0.064878128;
            } else {
              sum += (float)-0.19801615;
            }
          } else {
            if ( (data[13].fvalue < 0.36237073)) {
              sum += (float)-0.072488688;
            } else {
              sum += (float)-0.00052728259;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.58714867)) {
            sum += (float)-0;
          } else {
            sum += (float)0.087252095;
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.52041805)) {
            if ( (data[13].fvalue < 0.065171033)) {
              sum += (float)-0.031774674;
            } else {
              sum += (float)0.035392672;
            }
          } else {
            if ( (data[13].fvalue < 0.53171253)) {
              sum += (float)0.040104434;
            } else {
              sum += (float)0.12569502;
            }
          }
        } else {
          sum += (float)-0.079443432;
        }
      }
    }
  }
  if ( (data[13].fvalue < 3.034405)) {
    if ( (data[5].fvalue < 0.11830419)) {
      if (  (data[4].fvalue < 1.8028394)) {
        if (  (data[4].fvalue < 1.439218)) {
          if ( (data[15].fvalue < 0.11017245)) {
            if ( (data[2].fvalue < 0.080647811)) {
              sum += (float)-0.027330946;
            } else {
              sum += (float)0.016006796;
            }
          } else {
            sum += (float)0.086642593;
          }
        } else {
          if ( (data[3].fvalue < 0.13554588)) {
            sum += (float)0.15223467;
          } else {
            sum += (float)0.052509625;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.54134154)) {
          if ( (data[17].fvalue < 0.63783658)) {
            if ( (data[6].fvalue < 0.52744764)) {
              sum += (float)-0.12369618;
            } else {
              sum += (float)-0.012372311;
            }
          } else {
            sum += (float)0.024186924;
          }
        } else {
          if ( (data[13].fvalue < 0.27493417)) {
            if ( (data[2].fvalue < 0.27380544)) {
              sum += (float)-0.018156622;
            } else {
              sum += (float)0.028527781;
            }
          } else {
            sum += (float)0.055502687;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.4156752)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[7].fvalue < 7.5)) {
            if ( (data[17].fvalue < 0.034619052)) {
              sum += (float)-0.00028678923;
            } else {
              sum += (float)-0.026291328;
            }
          } else {
            sum += (float)0.090734504;
          }
        } else {
          sum += (float)-0.12741163;
        }
      } else {
        if ( (data[13].fvalue < 0.60109663)) {
          if ( (data[14].fvalue < 0.77685738)) {
            if ( (data[0].fvalue < 5.61905)) {
              sum += (float)0.025515959;
            } else {
              sum += (float)-0.054719079;
            }
          } else {
            if ( (data[14].fvalue < 1.8750821)) {
              sum += (float)0.075257197;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 1.4611691)) {
              sum += (float)0.12847464;
            } else {
              sum += (float)0.056954626;
            }
          } else {
            sum += (float)0.0043434976;
          }
        }
      }
    }
  } else {
    sum += (float)-0.13544436;
  }
  if ( (data[1].fvalue < 0.21612874)) {
    if ( (data[1].fvalue < 0.13424338)) {
      sum += (float)0.10283191;
    } else {
      if ( (data[14].fvalue < 0.035954379)) {
        sum += (float)0.057157882;
      } else {
        sum += (float)-0;
      }
    }
  } else {
    if ( (data[13].fvalue < 2.0424464)) {
      if ( (data[5].fvalue < 0.33764493)) {
        if ( (data[13].fvalue < 1.1312149)) {
          if ( (data[13].fvalue < 0.13344274)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.012556681;
            } else {
              sum += (float)0.030976554;
            }
          } else {
            if ( (data[15].fvalue < 0.22006461)) {
              sum += (float)0.012426154;
            } else {
              sum += (float)0.054015547;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.4163449)) {
            sum += (float)-0.089634687;
          } else {
            sum += (float)0.031049965;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.0030836044)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[3].fvalue < 0.66590887)) {
              sum += (float)-0.22865675;
            } else {
              sum += (float)-0.019962283;
            }
          } else {
            if ( (data[13].fvalue < 0.0028666463)) {
              sum += (float)0.20321593;
            } else {
              sum += (float)0.0031905863;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.4288137)) {
            sum += (float)-0.16884826;
          } else {
            if ( (data[14].fvalue < 0.014809681)) {
              sum += (float)-0.060261138;
            } else {
              sum += (float)0.0032105099;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.39258802)) {
        if ( (data[5].fvalue < 0.38652015)) {
          sum += (float)-0.11903378;
        } else {
          sum += (float)-0.026028931;
        }
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[4].fvalue < 0.83391356)) {
    if ( (data[14].fvalue < 9.8347213e-05)) {
      sum += (float)0.11483604;
    } else {
      sum += (float)0.013824371;
    }
  } else {
    if ( (data[5].fvalue < 0.55461085)) {
      if ( (data[13].fvalue < 1.461728)) {
        if ( (data[17].fvalue < 3.2152598)) {
          if ( (data[8].fvalue < 8.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.012194067;
            } else {
              sum += (float)-0.01474426;
            }
          } else {
            sum += (float)0.095102333;
          }
        } else {
          sum += (float)-0.083488896;
        }
      } else {
        if ( (data[17].fvalue < 0.42987812)) {
          if ( (data[16].fvalue < 0.24062106)) {
            if (  (data[4].fvalue < 2.1299949)) {
              sum += (float)-0.046841227;
            } else {
              sum += (float)-0.13488291;
            }
          } else {
            sum += (float)-0.0046735741;
          }
        } else {
          sum += (float)0.041625299;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.025617162)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[14].fvalue < 1.3875037e-07)) {
            if ( (data[13].fvalue < 0.05005198)) {
              sum += (float)0.16070463;
            } else {
              sum += (float)-0.010337707;
            }
          } else {
            if ( (data[14].fvalue < 0.021587422)) {
              sum += (float)-0.16430542;
            } else {
              sum += (float)-0.03041066;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.87544966)) {
            sum += (float)0.19107826;
          } else {
            sum += (float)-0.030943671;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.27874687)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[3].fvalue < 1.8836782)) {
              sum += (float)-0.0039945114;
            } else {
              sum += (float)-0.10342967;
            }
          } else {
            sum += (float)-0.15531576;
          }
        } else {
          sum += (float)-0.0050091576;
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.29394138)) {
    if ( (data[13].fvalue < 0.023477724)) {
      if ( (data[14].fvalue < 7.2904527e-07)) {
        if ( (data[3].fvalue < 0.60823584)) {
          if (  (data[4].fvalue < 0.99856031)) {
            sum += (float)0.14766194;
          } else {
            if (  (data[4].fvalue < 1.5146091)) {
              sum += (float)-0.1490691;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)0.32923856;
        }
      } else {
        if ( (data[15].fvalue < 2.0606356)) {
          if ( (data[14].fvalue < 0.067906767)) {
            if ( (data[14].fvalue < 0.01820698)) {
              sum += (float)-0.19629736;
            } else {
              sum += (float)-0.088224173;
            }
          } else {
            if ( (data[5].fvalue < 0.18508026)) {
              sum += (float)0.042974643;
            } else {
              sum += (float)-0.053471662;
            }
          }
        } else {
          sum += (float)-0.23035078;
        }
      }
    } else {
      if ( (data[8].fvalue < 0.5)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.012858048)) {
            if ( (data[13].fvalue < 0.28842902)) {
              sum += (float)-0.042562455;
            } else {
              sum += (float)0.019736016;
            }
          } else {
            if ( (data[15].fvalue < 0.21784492)) {
              sum += (float)-0.10246833;
            } else {
              sum += (float)-0.03512029;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3186336)) {
            if (  (data[4].fvalue < 1.0413115)) {
              sum += (float)-0.0448578;
            } else {
              sum += (float)0.020388829;
            }
          } else {
            if ( (data[13].fvalue < 1.1200573)) {
              sum += (float)0.081353448;
            } else {
              sum += (float)0.15496129;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.014817763)) {
            sum += (float)0.20771877;
          } else {
            if ( (data[15].fvalue < 0.00041940066)) {
              sum += (float)0.011838936;
            } else {
              sum += (float)0.094096951;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0097963484)) {
            sum += (float)-0.17977242;
          } else {
            if ( (data[6].fvalue < 0.083859518)) {
              sum += (float)0.098905936;
            } else {
              sum += (float)-0.023103222;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 1.8807126)) {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.40487388)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.01869916;
            } else {
              sum += (float)0.08545053;
            }
          } else {
            if ( (data[13].fvalue < 0.092994072)) {
              sum += (float)-0.17526117;
            } else {
              sum += (float)-0.0041017989;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.010740262)) {
            if ( (data[14].fvalue < 0.46068409)) {
              sum += (float)0.069835275;
            } else {
              sum += (float)0.18585688;
            }
          } else {
            if ( (data[14].fvalue < 0.55225283)) {
              sum += (float)-0.011507972;
            } else {
              sum += (float)0.077540092;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[15].fvalue < 0.41019189)) {
            sum += (float)0.059368938;
          } else {
            sum += (float)-0.021564228;
          }
        } else {
          sum += (float)-0.090971939;
        }
      }
    } else {
      if ( (data[15].fvalue < 0.2984629)) {
        sum += (float)-0.10174253;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[5].fvalue < 0.048070967)) {
    if (  (data[4].fvalue < 1.750392)) {
      if (  (data[4].fvalue < 1.4499748)) {
        sum += (float)0.029194226;
      } else {
        sum += (float)0.15188703;
      }
    } else {
      if ( (data[3].fvalue < 0.16003835)) {
        sum += (float)-0.015856363;
      } else {
        sum += (float)0.01549636;
      }
    }
  } else {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[8].fvalue < 5.5)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[8].fvalue < 2.5)) {
            if (  (data[4].fvalue < 1.0469332)) {
              sum += (float)-0.028889164;
            } else {
              sum += (float)0.0087831886;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.12086949;
            } else {
              sum += (float)-0.092832305;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0155792)) {
            sum += (float)0.043507375;
          } else {
            if ( (data[14].fvalue < 0.21590225)) {
              sum += (float)-0.06629955;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.61985981)) {
          sum += (float)0.012836881;
        } else {
          sum += (float)0.1072865;
        }
      }
    } else {
      if ( (data[6].fvalue < 0.14033082)) {
        sum += (float)0.14847583;
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.64738345)) {
            sum += (float)-0;
          } else {
            sum += (float)0.15347609;
          }
        } else {
          if ( (data[3].fvalue < 0.66271412)) {
            sum += (float)0.047467273;
          } else {
            if ( (data[17].fvalue < 0.58899748)) {
              sum += (float)-0.061475877;
            } else {
              sum += (float)0.013219805;
            }
          }
        }
      }
    }
  }
  if ( (data[1].fvalue < 0.30139047)) {
    if ( (data[5].fvalue < 0.39924356)) {
      if ( (data[0].fvalue < 1.6099317)) {
        sum += (float)0.097714692;
      } else {
        if ( (data[6].fvalue < 0.95272255)) {
          sum += (float)-0;
        } else {
          sum += (float)0.065894559;
        }
      }
    } else {
      sum += (float)-0;
    }
  } else {
    if ( (data[13].fvalue < 0.040343046)) {
      if ( (data[14].fvalue < 1.3875037e-07)) {
        if ( (data[15].fvalue < 0.018873708)) {
          if ( (data[13].fvalue < 0.0039544851)) {
            sum += (float)0.22573915;
          } else {
            sum += (float)0.054571841;
          }
        } else {
          if (  (data[4].fvalue < 1.7373106)) {
            sum += (float)-0.089029506;
          } else {
            sum += (float)0.018834502;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.043491989)) {
          if (  (data[4].fvalue < 0.98736501)) {
            sum += (float)-0.033564158;
          } else {
            sum += (float)-0.15632409;
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[15].fvalue < 2.951453)) {
              sum += (float)-0.00099007809;
            } else {
              sum += (float)-0.11562903;
            }
          } else {
            sum += (float)-0.17457654;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.024766149)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.023369722;
            } else {
              sum += (float)0.048273385;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.11973526;
            } else {
              sum += (float)-0.030799367;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.4057262)) {
              sum += (float)-0.0019493763;
            } else {
              sum += (float)0.046678804;
            }
          } else {
            sum += (float)-0.16669185;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0396049)) {
            if ( (data[14].fvalue < 0.08607126)) {
              sum += (float)-0.075442605;
            } else {
              sum += (float)0.030256063;
            }
          } else {
            if ( (data[5].fvalue < 0.2297706)) {
              sum += (float)0.029069277;
            } else {
              sum += (float)0.074443907;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.073869027)) {
            sum += (float)0.14309724;
          } else {
            if ( (data[0].fvalue < 2.5499747)) {
              sum += (float)0.038636681;
            } else {
              sum += (float)-0.013244897;
            }
          }
        }
      }
    }
  }
  if ( (data[17].fvalue < 0.016576391)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[6].fvalue < 0.026574751)) {
        if ( (data[13].fvalue < 0.036000378)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.12296441;
        }
      } else {
        if ( (data[15].fvalue < 0.032319333)) {
          if ( (data[13].fvalue < 3.6043119e-08)) {
            if ( (data[4].fvalue < 1.0983121)) {
              sum += (float)0.08110512;
            } else {
              sum += (float)0.24956957;
            }
          } else {
            if ( (data[4].fvalue < 1.6835611)) {
              sum += (float)0.037033241;
            } else {
              sum += (float)-0.10671992;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.11329633)) {
            if ( (data[13].fvalue < 0.10987344)) {
              sum += (float)-0.10027227;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[3].fvalue < 0.24058396)) {
              sum += (float)0.065677151;
            } else {
              sum += (float)0.00096980605;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[16].fvalue < 0.11881718)) {
          if ( (data[3].fvalue < 0.22809911)) {
            if (  (data[4].fvalue < 1.1194191)) {
              sum += (float)0.063118458;
            } else {
              sum += (float)-0.046491042;
            }
          } else {
            if ( (data[13].fvalue < 0.08092466)) {
              sum += (float)-0.1450434;
            } else {
              sum += (float)-0.038920641;
            }
          }
        } else {
          if ( (data[1].fvalue < 1.3843608)) {
            sum += (float)-0;
          } else {
            sum += (float)0.07811369;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1205103)) {
          if ( (data[3].fvalue < 0.52894771)) {
            sum += (float)0.31085777;
          } else {
            sum += (float)-0.0058596982;
          }
        } else {
          if ( (data[14].fvalue < 0.072619081)) {
            if (  (data[4].fvalue < 1.3640273)) {
              sum += (float)-0.11243103;
            } else {
              sum += (float)-0.0063515543;
            }
          } else {
            if ( (data[13].fvalue < 0.35350016)) {
              sum += (float)-0.022519354;
            } else {
              sum += (float)0.058649398;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 1.2915914)) {
      if ( (data[13].fvalue < 0.9897567)) {
        if ( (data[13].fvalue < 0.0080967713)) {
          if (  (data[4].fvalue < 1.9965993)) {
            if ( (data[15].fvalue < 0.35951141)) {
              sum += (float)-0.17337692;
            } else {
              sum += (float)-0.028979082;
            }
          } else {
            if ( (data[14].fvalue < 0.97711718)) {
              sum += (float)0.030923346;
            } else {
              sum += (float)-0.1035338;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0059645521)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.054690361;
            } else {
              sum += (float)0.044752896;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.05959025;
            } else {
              sum += (float)-0.019831074;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.78648365)) {
          if ( (data[15].fvalue < 0.065581977)) {
            if ( (data[17].fvalue < 0.43426928)) {
              sum += (float)-0.086965322;
            } else {
              sum += (float)0.0010724614;
            }
          } else {
            sum += (float)-0.13995318;
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.78831059)) {
        if ( (data[13].fvalue < 0.50254518)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.17291652;
            } else {
              sum += (float)-0.0082152979;
            }
          } else {
            sum += (float)0.01624313;
          }
        } else {
          if ( (data[16].fvalue < 0.038908049)) {
            if ( (data[3].fvalue < 2.6234365)) {
              sum += (float)0.15224983;
            } else {
              sum += (float)0.048485406;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        sum += (float)0.1269916;
      }
    }
  }
  if ( (data[16].fvalue < 0.013136039)) {
    if ( (data[8].fvalue < 4.5)) {
      if (  (data[4].fvalue < 2.0503922)) {
        if ( (data[7].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.1165574)) {
            sum += (float)-0.24336572;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.046189759;
            } else {
              sum += (float)0.1069565;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0017337715)) {
            sum += (float)0.19246948;
          } else {
            if ( (data[13].fvalue < 0.013740265)) {
              sum += (float)-0.068428695;
            } else {
              sum += (float)0.0055160234;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[17].fvalue < 3.2421634)) {
            if ( (data[13].fvalue < 1.8327323)) {
              sum += (float)0.054346353;
            } else {
              sum += (float)-0.037124731;
            }
          } else {
            sum += (float)-0.039334323;
          }
        } else {
          sum += (float)-0.043902814;
        }
      }
    } else {
      if ( (data[6].fvalue < 0.21339056)) {
        sum += (float)0.12456641;
      } else {
        if ( (data[7].fvalue < 2.5)) {
          sum += (float)0.094400488;
        } else {
          if ( (data[17].fvalue < 0.20460784)) {
            if ( (data[0].fvalue < 2.9616547)) {
              sum += (float)0.019140963;
            } else {
              sum += (float)-0.050238367;
            }
          } else {
            sum += (float)0.052101817;
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.00028363819)) {
      if (  (data[4].fvalue < 2.2540317)) {
        if ( (data[14].fvalue < 0.13197099)) {
          sum += (float)-0.14820927;
        } else {
          sum += (float)-0.059190463;
        }
      } else {
        sum += (float)-0;
      }
    } else {
      if ( (data[17].fvalue < 0.084300093)) {
        if ( (data[13].fvalue < 1.5937555)) {
          if ( (data[13].fvalue < 0.46238801)) {
            if ( (data[15].fvalue < 1.5194215)) {
              sum += (float)0.023241632;
            } else {
              sum += (float)-0.040196374;
            }
          } else {
            if ( (data[16].fvalue < 0.92945004)) {
              sum += (float)0.022821799;
            } else {
              sum += (float)0.11365246;
            }
          }
        } else {
          sum += (float)-0.050678745;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.23722649)) {
            sum += (float)-0.11140028;
          } else {
            if ( (data[14].fvalue < 0.0019688082)) {
              sum += (float)-0.049359534;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.056250948;
          } else {
            if ( (data[0].fvalue < 5.9927578)) {
              sum += (float)-0.0060637114;
            } else {
              sum += (float)-0.074699089;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.5347933)) {
    if ( (data[17].fvalue < 0.035364829)) {
      if ( (data[14].fvalue < 0.33032516)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.43320349)) {
            if ( (data[15].fvalue < 1.2739511)) {
              sum += (float)0.0011390032;
            } else {
              sum += (float)-0.11563881;
            }
          } else {
            if ( (data[15].fvalue < 1.367979)) {
              sum += (float)0.022314433;
            } else {
              sum += (float)0.13948292;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.52474582)) {
            if ( (data[15].fvalue < 0.12486535)) {
              sum += (float)-0.024119308;
            } else {
              sum += (float)0.10418681;
            }
          } else {
            if ( (data[14].fvalue < 0.114622)) {
              sum += (float)-0.16637903;
            } else {
              sum += (float)-0.036196925;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.59945589)) {
          if ( (data[15].fvalue < 0.4046039)) {
            if ( (data[4].fvalue < 1.4279947)) {
              sum += (float)0.052060928;
            } else {
              sum += (float)-0.056292899;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.069691449;
            } else {
              sum += (float)-0.001863388;
            }
          }
        } else {
          sum += (float)0.088328436;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.12850809)) {
        if ( (data[8].fvalue < 2.5)) {
          if (  (data[4].fvalue < 2.1495388)) {
            if ( (data[17].fvalue < 0.062583774)) {
              sum += (float)-0.0056847087;
            } else {
              sum += (float)-0.080773868;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)-0.14531961;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.26020366)) {
              sum += (float)-0.08068835;
            } else {
              sum += (float)-0.018849509;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.087301105;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.29250097)) {
            sum += (float)0.10769111;
          } else {
            if ( (data[0].fvalue < 3.3160009)) {
              sum += (float)0.0570958;
            } else {
              sum += (float)-0.0020652039;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.075777084;
  }
  if ( (data[5].fvalue < 0.012779377)) {
    sum += (float)0.090048432;
  } else {
    if ( (data[16].fvalue < 0.0031561735)) {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[13].fvalue < 0.42442039)) {
          if ( (data[5].fvalue < 0.49632567)) {
            if ( (data[13].fvalue < 0.14160912)) {
              sum += (float)-0.0097012902;
            } else {
              sum += (float)0.018085185;
            }
          } else {
            if ( (data[5].fvalue < 0.96793175)) {
              sum += (float)-0.036638174;
            } else {
              sum += (float)0.0055671255;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.87860036)) {
            if ( (data[15].fvalue < 0.048158132)) {
              sum += (float)0.022349313;
            } else {
              sum += (float)-0.025736235;
            }
          } else {
            sum += (float)0.11715041;
          }
        }
      } else {
        if ( (data[6].fvalue < 0.10626426)) {
          sum += (float)0.18164954;
        } else {
          if ( (data[0].fvalue < 1.9475359)) {
            sum += (float)0.08259505;
          } else {
            if ( (data[14].fvalue < 0.52524614)) {
              sum += (float)-0.019413445;
            } else {
              sum += (float)0.045840014;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.00038481964)) {
        if ( (data[16].fvalue < 0.28100556)) {
          if ( (data[13].fvalue < 0.5679853)) {
            sum += (float)-0.18215202;
          } else {
            sum += (float)-0.029383408;
          }
        } else {
          sum += (float)-0.014889377;
        }
      } else {
        if ( (data[16].fvalue < 0.13845906)) {
          if ( (data[5].fvalue < 0.24897057)) {
            if ( (data[5].fvalue < 0.15371397)) {
              sum += (float)-0.02909478;
            } else {
              sum += (float)0.019855732;
            }
          } else {
            if ( (data[15].fvalue < 0.56238914)) {
              sum += (float)-0.097860724;
            } else {
              sum += (float)-0.027592747;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.33758336)) {
            if ( (data[2].fvalue < 0.62054378)) {
              sum += (float)-0.00025806253;
            } else {
              sum += (float)0.044271268;
            }
          } else {
            if ( (data[13].fvalue < 0.30566064)) {
              sum += (float)-0.05616479;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  }
  if ( (data[10].fvalue < 1.5)) {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[14].fvalue < 0.24129769)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.23647586)) {
            if ( (data[15].fvalue < 0.72819209)) {
              sum += (float)-0.0013867219;
            } else {
              sum += (float)-0.081030004;
            }
          } else {
            if ( (data[15].fvalue < 0.54878491)) {
              sum += (float)0.0010039272;
            } else {
              sum += (float)0.054999046;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.24167562)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.025135569;
            } else {
              sum += (float)0.13908531;
            }
          } else {
            if ( (data[14].fvalue < 0.07072103)) {
              sum += (float)-0.16989264;
            } else {
              sum += (float)-0.044839248;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 1.5005139)) {
          if ( (data[14].fvalue < 1.0157559)) {
            if ( (data[3].fvalue < 1.4747378)) {
              sum += (float)0.027814487;
            } else {
              sum += (float)-0.0023927167;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.076314151;
            } else {
              sum += (float)0.018683447;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.79145813)) {
            if ( (data[3].fvalue < 2.4535742)) {
              sum += (float)0.046983324;
            } else {
              sum += (float)-0.079415128;
            }
          } else {
            sum += (float)0.12569335;
          }
        }
      }
    } else {
      sum += (float)0.094337419;
    }
  } else {
    if ( (data[7].fvalue < 2.5)) {
      sum += (float)-0.0031562767;
    } else {
      sum += (float)-0.11779653;
    }
  }
  if ( (data[14].fvalue < 2.2417655)) {
    if ( (data[12].fvalue < 0.5)) {
      if ( (data[16].fvalue < 0.027121603)) {
        if ( (data[17].fvalue < 0.62746447)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[6].fvalue < 0.044877686)) {
              sum += (float)-0.030326661;
            } else {
              sum += (float)0.015261067;
            }
          } else {
            if ( (data[3].fvalue < 0.2986744)) {
              sum += (float)0.014076142;
            } else {
              sum += (float)-0.023358699;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.6068251)) {
            sum += (float)-0.053859506;
          } else {
            if ( (data[17].fvalue < 3.0026491)) {
              sum += (float)0.051728111;
            } else {
              sum += (float)-0.026659399;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.28205568)) {
            if ( (data[13].fvalue < 0.64192921)) {
              sum += (float)-0.032833956;
            } else {
              sum += (float)-0.14742562;
            }
          } else {
            if ( (data[14].fvalue < 0.001993103)) {
              sum += (float)-0.028333267;
            } else {
              sum += (float)0.021792296;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 1.2045745)) {
              sum += (float)0.028901514;
            } else {
              sum += (float)0.085850529;
            }
          } else {
            if ( (data[16].fvalue < 0.18701079)) {
              sum += (float)-0.09315443;
            } else {
              sum += (float)0.0010860144;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 2.4351258)) {
        sum += (float)-0.089406461;
      } else {
        sum += (float)0.0037826779;
      }
    }
  } else {
    sum += (float)-0.055622663;
  }
  if ( (data[13].fvalue < 3.6043119e-08)) {
    if ( (data[14].fvalue < 8.7487424e-06)) {
      if ( (data[3].fvalue < 0.36026779)) {
        sum += (float)0.0088654375;
      } else {
        sum += (float)0.27106151;
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if (  (data[4].fvalue < 1.1613784)) {
          sum += (float)0.10801073;
        } else {
          if ( (data[15].fvalue < 0.10559277)) {
            sum += (float)-0.10202795;
          } else {
            if ( (data[14].fvalue < 0.40354991)) {
              sum += (float)-0.052602131;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.53272521)) {
          sum += (float)-0;
        } else {
          sum += (float)0.10273051;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.0039318893)) {
      sum += (float)-0.24846469;
    } else {
      if ( (data[14].fvalue < 0.02992383)) {
        if ( (data[14].fvalue < 3.2368527e-07)) {
          if ( (data[15].fvalue < 0.048669428)) {
            if ( (data[13].fvalue < 0.068262972)) {
              sum += (float)0.059448827;
            } else {
              sum += (float)0.0038599905;
            }
          } else {
            if ( (data[4].fvalue < 2.1478212)) {
              sum += (float)-0.044941127;
            } else {
              sum += (float)0.017573893;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0047154161)) {
            sum += (float)-0.18109994;
          } else {
            if ( (data[15].fvalue < 0.0017400922)) {
              sum += (float)-0.051828384;
            } else {
              sum += (float)0.054397386;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0516374)) {
            if ( (data[3].fvalue < 0.43281507)) {
              sum += (float)-0.10212816;
            } else {
              sum += (float)0.025310403;
            }
          } else {
            if ( (data[13].fvalue < 0.058489934)) {
              sum += (float)0.098694138;
            } else {
              sum += (float)0.032823823;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.023473915)) {
            sum += (float)-0.10424425;
          } else {
            if ( (data[6].fvalue < 0.070866808)) {
              sum += (float)0.095725022;
            } else {
              sum += (float)-0.0015002708;
            }
          }
        }
      }
    }
  }
  if ( (data[15].fvalue < 3.5969448)) {
    if ( (data[15].fvalue < 0.22932622)) {
      if ( (data[15].fvalue < 0.043755725)) {
        if ( (data[15].fvalue < 0.00038735243)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.083297454;
            } else {
              sum += (float)0.006671187;
            }
          } else {
            if ( (data[14].fvalue < 0.21094382)) {
              sum += (float)-0.080042139;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.021939589)) {
            sum += (float)0.1330768;
          } else {
            if ( (data[5].fvalue < 0.34160557)) {
              sum += (float)0.056688294;
            } else {
              sum += (float)-0.0075121201;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.18439886)) {
          if ( (data[15].fvalue < 0.11057501)) {
            sum += (float)-0.009247385;
          } else {
            sum += (float)0.054207105;
          }
        } else {
          if ( (data[4].fvalue < 2.1601684)) {
            if (  (data[4].fvalue < 1.6053531)) {
              sum += (float)-0.030676914;
            } else {
              sum += (float)-0.11131018;
            }
          } else {
            if ( (data[16].fvalue < 0.15412644)) {
              sum += (float)-0.014605317;
            } else {
              sum += (float)0.040861677;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.73012698)) {
        if (  (data[4].fvalue < 1.7790911)) {
          if ( (data[13].fvalue < 0.070192896)) {
            if ( (data[14].fvalue < 0.019351102)) {
              sum += (float)-0.037477341;
            } else {
              sum += (float)0.070040688;
            }
          } else {
            if ( (data[14].fvalue < 0.0026611546)) {
              sum += (float)0.078795046;
            } else {
              sum += (float)0.15384698;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.39402872)) {
            sum += (float)-0.082347006;
          } else {
            if ( (data[5].fvalue < 0.064850204)) {
              sum += (float)-0.0044003422;
            } else {
              sum += (float)0.043750662;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.6868763)) {
          if ( (data[13].fvalue < 0.037096884)) {
            if ( (data[15].fvalue < 0.68445563)) {
              sum += (float)0.018739754;
            } else {
              sum += (float)-0.066701181;
            }
          } else {
            if ( (data[3].fvalue < 2.1600242)) {
              sum += (float)0.020171888;
            } else {
              sum += (float)-0.021637073;
            }
          }
        } else {
          if ( (data[15].fvalue < 1.3130009)) {
            if ( (data[14].fvalue < 1.3583494)) {
              sum += (float)0.020197401;
            } else {
              sum += (float)-0.025133882;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.13485779;
            } else {
              sum += (float)0.00048380968;
            }
          }
        }
      }
    }
  } else {
    sum += (float)-0.086118594;
  }
  if ( (data[10].fvalue < 1.5)) {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[13].fvalue < 2.6558433)) {
        if ( (data[7].fvalue < 2.5)) {
          if (  (data[4].fvalue < 1.0164555)) {
            if ( (data[14].fvalue < 1.3875037e-07)) {
              sum += (float)0.0013478824;
            } else {
              sum += (float)-0.057592962;
            }
          } else {
            if ( (data[15].fvalue < 0.057120599)) {
              sum += (float)0.026094269;
            } else {
              sum += (float)-0.001725488;
            }
          }
        } else {
          if ( (data[4].fvalue < 0.98903805)) {
            sum += (float)0.065946907;
          } else {
            if ( (data[13].fvalue < 0.19929565)) {
              sum += (float)-0.07172244;
            } else {
              sum += (float)-0.0057049086;
            }
          }
        }
      } else {
        sum += (float)-0.074725062;
      }
    } else {
      if ( (data[13].fvalue < 0.15933433)) {
        sum += (float)0.10747241;
      } else {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[14].fvalue < 0.21751772)) {
            if ( (data[13].fvalue < 0.63985598)) {
              sum += (float)-0.041142665;
            } else {
              sum += (float)0.048332475;
            }
          } else {
            sum += (float)0.12282597;
          }
        } else {
          if ( (data[6].fvalue < 0.46892935)) {
            sum += (float)0.016714552;
          } else {
            if ( (data[1].fvalue < 1.0896881)) {
              sum += (float)-0.0062348745;
            } else {
              sum += (float)-0.068515994;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 2.6673255)) {
      sum += (float)-0;
    } else {
      sum += (float)-0.099937089;
    }
  }
  if ( (data[15].fvalue < 0.14026725)) {
    if ( (data[15].fvalue < 0.061831728)) {
      if ( (data[13].fvalue < 6.1662735e-07)) {
        if ( (data[11].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.2412243)) {
            sum += (float)0.052882727;
          } else {
            sum += (float)0.18446708;
          }
        } else {
          sum += (float)-0.047087222;
        }
      } else {
        if ( (data[16].fvalue < 0.66536075)) {
          if ( (data[5].fvalue < 1.1036718)) {
            if ( (data[14].fvalue < 0.42440814)) {
              sum += (float)-0.015700428;
            } else {
              sum += (float)0.024212023;
            }
          } else {
            if ( (data[13].fvalue < 0.29955393)) {
              sum += (float)0.034110542;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)0.081470862;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.06879019)) {
        if ( (data[3].fvalue < 0.15557739)) {
          sum += (float)-0;
        } else {
          if ( (data[15].fvalue < 0.11910671)) {
            if ( (data[3].fvalue < 0.62992936)) {
              sum += (float)-0.058327902;
            } else {
              sum += (float)-0.12030024;
            }
          } else {
            sum += (float)-0.025982382;
          }
        }
      } else {
        sum += (float)0.00031017669;
      }
    }
  } else {
    if ( (data[0].fvalue < 1.5381873)) {
      sum += (float)0.094174117;
    } else {
      if ( (data[6].fvalue < 0.16442826)) {
        if ( (data[14].fvalue < 0.012090221)) {
          sum += (float)0.02532967;
        } else {
          sum += (float)0.11100001;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 2.0146761)) {
            if ( (data[14].fvalue < 0.0080396011)) {
              sum += (float)-0.054989401;
            } else {
              sum += (float)0.0032092643;
            }
          } else {
            if ( (data[13].fvalue < 1.0638585)) {
              sum += (float)0.021184549;
            } else {
              sum += (float)-0.034240197;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[17].fvalue < 0.026992166)) {
              sum += (float)0.10555205;
            } else {
              sum += (float)0.024974639;
            }
          } else {
            if ( (data[5].fvalue < 0.5684886)) {
              sum += (float)0.015336696;
            } else {
              sum += (float)-0.047621083;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.015337136)) {
    sum += (float)0.073725879;
  } else {
    if ( (data[12].fvalue < 0.5)) {
      if ( (data[1].fvalue < 4.2163391)) {
        if ( (data[6].fvalue < 0.30766308)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[15].fvalue < 0.27794522)) {
              sum += (float)0.0027897106;
            } else {
              sum += (float)0.052887678;
            }
          } else {
            if ( (data[0].fvalue < 3.5532112)) {
              sum += (float)0.13278618;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.76405025)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.0060308971;
            } else {
              sum += (float)-0.013646334;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.017340234;
            } else {
              sum += (float)-0.10365976;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.057062287;
          } else {
            if ( (data[14].fvalue < 0.15639564)) {
              sum += (float)-0.0028672779;
            } else {
              sum += (float)0.062420662;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.22173363)) {
            if ( (data[1].fvalue < 5.5832939)) {
              sum += (float)-0.033560243;
            } else {
              sum += (float)-0.12071408;
            }
          } else {
            if ( (data[6].fvalue < 0.21928518)) {
              sum += (float)0.019763039;
            } else {
              sum += (float)-0.020990083;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 2.4953222)) {
        sum += (float)-0.085535459;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[13].fvalue < 0.20596366)) {
    if ( (data[8].fvalue < 6.5)) {
      if ( (data[16].fvalue < 1.1740104)) {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[4].fvalue < 1.1005397)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.012581876;
            } else {
              sum += (float)0.11666792;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.012654341;
            } else {
              sum += (float)-0.053478699;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.71653169)) {
            if (  (data[4].fvalue < 2.3711662)) {
              sum += (float)0.094546586;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[16].fvalue < 0.081078269)) {
              sum += (float)-0.062502675;
            } else {
              sum += (float)0.029512363;
            }
          }
        }
      } else {
        sum += (float)-0.097732745;
      }
    } else {
      sum += (float)0.10218563;
    }
  } else {
    if ( (data[15].fvalue < 1.3680199)) {
      if ( (data[16].fvalue < 0.69197488)) {
        if ( (data[4].fvalue < 1.7453314)) {
          if ( (data[5].fvalue < 0.7079019)) {
            if (  (data[4].fvalue < 1.1824915)) {
              sum += (float)0.050196812;
            } else {
              sum += (float)0.0098992102;
            }
          } else {
            if ( (data[17].fvalue < 0.011873936)) {
              sum += (float)-0.010449135;
            } else {
              sum += (float)-0.075156763;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.88597894)) {
            if ( (data[14].fvalue < 0.41642776)) {
              sum += (float)-0.067317784;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 2.6341102)) {
              sum += (float)0.062188704;
            } else {
              sum += (float)-0.0014034708;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.86087441)) {
          if ( (data[0].fvalue < 5.2625399)) {
            sum += (float)0.049396314;
          } else {
            sum += (float)-0.021011908;
          }
        } else {
          sum += (float)0.10107671;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.66050065)) {
        if ( (data[2].fvalue < 0.43949685)) {
          sum += (float)0.050246891;
        } else {
          sum += (float)-0.024974221;
        }
      } else {
        if ( (data[16].fvalue < 0.17198265)) {
          sum += (float)0.12044296;
        } else {
          sum += (float)0.035734318;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.36550432)) {
    if ( (data[3].fvalue < 0.15774019)) {
      if (  (data[4].fvalue < 1.0332074)) {
        if ( (data[13].fvalue < 0.06812571)) {
          if ( (data[14].fvalue < 3.7842896e-05)) {
            if (  (data[4].fvalue < 0.9699077)) {
              sum += (float)-0.017437579;
            } else {
              sum += (float)0.11473183;
            }
          } else {
            sum += (float)-0.031662021;
          }
        } else {
          sum += (float)-0.1299729;
        }
      } else {
        if (  (data[4].fvalue < 1.1196005)) {
          sum += (float)0.1311079;
        } else {
          if (  (data[4].fvalue < 1.2210071)) {
            if ( (data[13].fvalue < 0.081872538)) {
              sum += (float)-0.0035415206;
            } else {
              sum += (float)-0.067731373;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.061384369;
            } else {
              sum += (float)-0.010530205;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 4.5)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.1363976;
            } else {
              sum += (float)-0.036903314;
            }
          } else {
            if ( (data[13].fvalue < 0.0099705197)) {
              sum += (float)-0.013575259;
            } else {
              sum += (float)0.032873899;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.048050232)) {
            sum += (float)-0.14611448;
          } else {
            if ( (data[3].fvalue < 0.87111127)) {
              sum += (float)-9.408842e-05;
            } else {
              sum += (float)-0.059836164;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.19672713)) {
          sum += (float)0.095127158;
        } else {
          if ( (data[7].fvalue < 3.5)) {
            sum += (float)0.038775656;
          } else {
            sum += (float)-0.034671705;
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.61837566)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[14].fvalue < 0.027320161)) {
          if ( (data[3].fvalue < 0.30288586)) {
            if ( (data[13].fvalue < 0.50081414)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.11710913;
            }
          } else {
            if (  (data[4].fvalue < 2.024992)) {
              sum += (float)-0.0019190813;
            } else {
              sum += (float)0.034569427;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1880097)) {
            sum += (float)-0;
          } else {
            if ( (data[14].fvalue < 0.27198184)) {
              sum += (float)0.065102555;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.24033503)) {
          if ( (data[14].fvalue < 0.13496169)) {
            sum += (float)-0.15561567;
          } else {
            sum += (float)-0.058596261;
          }
        } else {
          if ( (data[6].fvalue < 0.46645015)) {
            sum += (float)0.056384962;
          } else {
            if (  (data[4].fvalue < 2.2643323)) {
              sum += (float)-0.065941386;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.039859038)) {
        sum += (float)0.093344882;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[1].fvalue < 0.13375011)) {
    sum += (float)0.064257905;
  } else {
    if ( (data[10].fvalue < 1.5)) {
      if ( (data[7].fvalue < 7.5)) {
        if ( (data[4].fvalue < 0.89062053)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.00070572796)) {
              sum += (float)0.068923004;
            } else {
              sum += (float)0.00011730013;
            }
          } else {
            sum += (float)-0.032689691;
          }
        } else {
          if ( (data[5].fvalue < 0.81586683)) {
            if ( (data[13].fvalue < 0.32289696)) {
              sum += (float)-0.0068831551;
            } else {
              sum += (float)0.007663941;
            }
          } else {
            if ( (data[13].fvalue < 0.052419871)) {
              sum += (float)0.016331416;
            } else {
              sum += (float)-0.031845264;
            }
          }
        }
      } else {
        sum += (float)0.071502909;
      }
    } else {
      if ( (data[1].fvalue < 1.7135943)) {
        sum += (float)-0.07108105;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[4].fvalue < 0.74361342)) {
    sum += (float)0.071228556;
  } else {
    if ( (data[17].fvalue < 0.68006724)) {
      if ( (data[17].fvalue < 0.027106486)) {
        if ( (data[13].fvalue < 0.74305058)) {
          if ( (data[15].fvalue < 2.4774566)) {
            if ( (data[15].fvalue < 0.14028464)) {
              sum += (float)-0.0050571552;
            } else {
              sum += (float)0.019300099;
            }
          } else {
            if ( (data[14].fvalue < 0.27658641)) {
              sum += (float)-0.099777743;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.5274482)) {
            if ( (data[5].fvalue < 0.86032599)) {
              sum += (float)0.085579127;
            } else {
              sum += (float)0.0028916264;
            }
          } else {
            if ( (data[6].fvalue < 0.81223398)) {
              sum += (float)-0.017871972;
            } else {
              sum += (float)0.063284822;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.095790073)) {
          if ( (data[15].fvalue < 0.082802847)) {
            sum += (float)-0.0806517;
          } else {
            if ( (data[5].fvalue < 0.26501375)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.045186322;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6812326)) {
            if ( (data[3].fvalue < 0.41740674)) {
              sum += (float)0.07379698;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 2.5173209)) {
              sum += (float)-0.045526952;
            } else {
              sum += (float)0.015011049;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.29290336)) {
        if ( (data[14].fvalue < 0.14107719)) {
          if ( (data[13].fvalue < 0.098857403)) {
            sum += (float)-0.08502534;
          } else {
            if ( (data[3].fvalue < 1.5470629)) {
              sum += (float)0.062638454;
            } else {
              sum += (float)-0.0085365521;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.4861951)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.091545515;
            } else {
              sum += (float)0.025388299;
            }
          } else {
            if ( (data[17].fvalue < 2.0485559)) {
              sum += (float)-0.036385931;
            } else {
              sum += (float)0.032260258;
            }
          }
        }
      } else {
        if ( (data[2].fvalue < 0.53176373)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.06601768;
        }
      }
    }
  }
  if ( (data[1].fvalue < 0.25136572)) {
    if ( (data[13].fvalue < 0.10560602)) {
      sum += (float)0.0028360777;
    } else {
      if ( (data[0].fvalue < 1.5460368)) {
        sum += (float)0.078381561;
      } else {
        sum += (float)0.022790298;
      }
    }
  } else {
    if ( (data[4].fvalue < 1.1602894)) {
      if ( (data[7].fvalue < 0.5)) {
        sum += (float)-0.12137707;
      } else {
        if ( (data[13].fvalue < 0.00395374)) {
          sum += (float)0.13660324;
        } else {
          if (  (data[4].fvalue < 1.0886748)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.00010180336;
            } else {
              sum += (float)0.074095421;
            }
          } else {
            if ( (data[15].fvalue < 0.029731145)) {
              sum += (float)0.062653653;
            } else {
              sum += (float)-0.0088050794;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.00025646406)) {
        if ( (data[13].fvalue < 0.51741505)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.85824251)) {
              sum += (float)-0.030587405;
            } else {
              sum += (float)-0.11463673;
            }
          } else {
            if ( (data[14].fvalue < 0.070389934)) {
              sum += (float)0.071488895;
            } else {
              sum += (float)-0.014597444;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.29950917)) {
            if (  (data[4].fvalue < 1.2641063)) {
              sum += (float)0.076038413;
            } else {
              sum += (float)-0.016398801;
            }
          } else {
            sum += (float)0.10488401;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.041280724)) {
          if ( (data[13].fvalue < 0.33200455)) {
            sum += (float)0.12933519;
          } else {
            sum += (float)0.029208336;
          }
        } else {
          if ( (data[15].fvalue < 0.099327803)) {
            if ( (data[5].fvalue < 0.12308089)) {
              sum += (float)0.014213488;
            } else {
              sum += (float)-0.049104001;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0045743627;
            } else {
              sum += (float)0.021454405;
            }
          }
        }
      }
    }
  }
  if ( (data[10].fvalue < 1.5)) {
    if ( (data[5].fvalue < 0.5078426)) {
      if ( (data[15].fvalue < 0.88685477)) {
        if ( (data[14].fvalue < 0.73900998)) {
          if ( (data[13].fvalue < 0.78199923)) {
            if (  (data[4].fvalue < 1.4473038)) {
              sum += (float)-0.0047681816;
            } else {
              sum += (float)0.023831343;
            }
          } else {
            if ( (data[15].fvalue < 0.072202638)) {
              sum += (float)0.0043442948;
            } else {
              sum += (float)-0.075415865;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.35738227)) {
            if ( (data[17].fvalue < 0.77250648)) {
              sum += (float)-0.060832225;
            } else {
              sum += (float)0.0023990527;
            }
          } else {
            sum += (float)0.032575648;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.55497593)) {
          if ( (data[13].fvalue < 0.096602798)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.058565933;
            } else {
              sum += (float)0.060358785;
            }
          } else {
            if (  (data[4].fvalue < 2.3399448)) {
              sum += (float)0.059607606;
            } else {
              sum += (float)0.0036768753;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            sum += (float)0.12858702;
          } else {
            sum += (float)-8.401298e-05;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.46586195)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[3].fvalue < 0.32849067)) {
            if ( (data[13].fvalue < 0.11885531)) {
              sum += (float)0.0080281887;
            } else {
              sum += (float)-0.10236818;
            }
          } else {
            if ( (data[2].fvalue < 0.30564386)) {
              sum += (float)0.079091817;
            } else {
              sum += (float)-0.036003191;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.011626536)) {
            sum += (float)0.12954168;
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.019259853;
            } else {
              sum += (float)0.10460053;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.89981723)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.66079581)) {
              sum += (float)0.0062203202;
            } else {
              sum += (float)-0.074800007;
            }
          } else {
            if ( (data[14].fvalue < 0.11459932)) {
              sum += (float)-0.10508097;
            } else {
              sum += (float)-0.030249426;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[3].fvalue < 3.7723734)) {
              sum += (float)-0.016346633;
            } else {
              sum += (float)0.033708233;
            }
          } else {
            sum += (float)0.058219034;
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 2.5)) {
      sum += (float)0.0020788789;
    } else {
      sum += (float)-0.10523354;
    }
  }
  if ( (data[4].fvalue < 1.1352532)) {
    if ( (data[7].fvalue < 0.5)) {
      sum += (float)-0.12577946;
    } else {
      if ( (data[13].fvalue < 0.0039198198)) {
        sum += (float)0.11878562;
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if (  (data[4].fvalue < 1.0921345)) {
            if ( (data[14].fvalue < 0.00010778879)) {
              sum += (float)0.010790188;
            } else {
              sum += (float)-0.022580059;
            }
          } else {
            if ( (data[3].fvalue < 0.34843236)) {
              sum += (float)0.11618671;
            } else {
              sum += (float)0.0047388421;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.48818743)) {
            sum += (float)0.22687085;
          } else {
            sum += (float)-0.040396359;
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.10063415)) {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.2383676)) {
          sum += (float)-0.10266192;
        } else {
          sum += (float)-0.030866401;
        }
      } else {
        sum += (float)-0;
      }
    } else {
      if ( (data[3].fvalue < 0.096100658)) {
        if (  (data[4].fvalue < 1.2516797)) {
          sum += (float)-0.00443479;
        } else {
          if ( (data[13].fvalue < 0.02922741)) {
            sum += (float)-0;
          } else {
            if ( (data[2].fvalue < 0.083361544)) {
              sum += (float)0.024439633;
            } else {
              sum += (float)0.088983305;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 1.9718736e-06)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.10710009;
            } else {
              sum += (float)-0.015838744;
            }
          } else {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.03625996;
            } else {
              sum += (float)0.10030446;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.01127669)) {
            sum += (float)-0.12293602;
          } else {
            if ( (data[14].fvalue < 0.011338405)) {
              sum += (float)-0.01437029;
            } else {
              sum += (float)0.0038122952;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 5.36313e-07)) {
    if (  (data[4].fvalue < 0.99733233)) {
      sum += (float)0.12190759;
    } else {
      if (  (data[4].fvalue < 1.7393785)) {
        if ( (data[14].fvalue < 0.14371589)) {
          if ( (data[5].fvalue < 0.10293862)) {
            sum += (float)-0;
          } else {
            sum += (float)-0.11663146;
          }
        } else {
          if (  (data[4].fvalue < 1.4498333)) {
            if ( (data[3].fvalue < 0.89327443)) {
              sum += (float)0.072749548;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[14].fvalue < 0.54351687)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.04501877;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          sum += (float)0.10866819;
        } else {
          if ( (data[15].fvalue < 2.5046344)) {
            if ( (data[15].fvalue < 0.72421002)) {
              sum += (float)-0.0089730443;
            } else {
              sum += (float)0.05710493;
            }
          } else {
            sum += (float)-0.035130717;
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.0045174859)) {
      sum += (float)-0.15212487;
    } else {
      if ( (data[3].fvalue < 2.0566854)) {
        if ( (data[15].fvalue < 0.27814102)) {
          if ( (data[13].fvalue < 0.13417915)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.00083632889;
            } else {
              sum += (float)-0.039617784;
            }
          } else {
            if (  (data[4].fvalue < 1.375006)) {
              sum += (float)0.01174267;
            } else {
              sum += (float)-0.0052737938;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8869205)) {
            if ( (data[14].fvalue < 0.024310386)) {
              sum += (float)0.0027752516;
            } else {
              sum += (float)0.074995078;
            }
          } else {
            if ( (data[5].fvalue < 0.10290442)) {
              sum += (float)-0.034029517;
            } else {
              sum += (float)0.017718013;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.006643638)) {
              sum += (float)-0.043577969;
            } else {
              sum += (float)0.014945306;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.094332017;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.61045754)) {
            if ( (data[17].fvalue < 0.0081689367)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.038961485;
            }
          } else {
            sum += (float)-0.12298207;
          }
        }
      }
    }
  }
  if ( (data[1].fvalue < 3.5615506)) {
    if ( (data[0].fvalue < 4.8755598)) {
      if ( (data[0].fvalue < 4.2508936)) {
        if ( (data[13].fvalue < 0.036855996)) {
          if ( (data[14].fvalue < 0.10322651)) {
            if (  (data[4].fvalue < 1.0037417)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.066371702;
            }
          } else {
            if ( (data[5].fvalue < 0.33307856)) {
              sum += (float)0.025720667;
            } else {
              sum += (float)-0.026978897;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.3050354)) {
            if ( (data[6].fvalue < 0.066799536)) {
              sum += (float)-0.038063046;
            } else {
              sum += (float)0.013360847;
            }
          } else {
            if ( (data[14].fvalue < 0.69131577)) {
              sum += (float)-0.027266148;
            } else {
              sum += (float)0.043453977;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0058553359)) {
          if ( (data[13].fvalue < 0.61005539)) {
            sum += (float)-0.079074204;
          } else {
            sum += (float)-0.002640245;
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[17].fvalue < 0.16393779)) {
              sum += (float)0.018350374;
            } else {
              sum += (float)-0.01076595;
            }
          } else {
            sum += (float)-0.042585418;
          }
        }
      }
    } else {
      if ( (data[11].fvalue < 2.5)) {
        if ( (data[5].fvalue < 0.097577743)) {
          if ( (data[17].fvalue < 0.37966025)) {
            sum += (float)-0;
          } else {
            sum += (float)-0.03321027;
          }
        } else {
          if ( (data[15].fvalue < 3.5206249)) {
            if ( (data[13].fvalue < 0.0051381346)) {
              sum += (float)0.053459171;
            } else {
              sum += (float)0.01640518;
            }
          } else {
            sum += (float)-0.015542419;
          }
        }
      } else {
        sum += (float)-0.035629421;
      }
    }
  } else {
    if ( (data[4].fvalue < 0.99668628)) {
      if ( (data[13].fvalue < 0.065234959)) {
        sum += (float)0.045244258;
      } else {
        if ( (data[3].fvalue < 0.38149196)) {
          sum += (float)-0.050779589;
        } else {
          if ( (data[13].fvalue < 0.28336278)) {
            sum += (float)0.029258212;
          } else {
            sum += (float)-0.025415028;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.072333783)) {
        if ( (data[15].fvalue < 0.11788066)) {
          sum += (float)-0;
        } else {
          sum += (float)0.032263789;
        }
      } else {
        if ( (data[13].fvalue < 0.32206205)) {
          if ( (data[14].fvalue < 0.17710432)) {
            if ( (data[13].fvalue < 0.10125198)) {
              sum += (float)-0.073061906;
            } else {
              sum += (float)-0.023174418;
            }
          } else {
            if (  (data[4].fvalue < 1.2896161)) {
              sum += (float)0.022606703;
            } else {
              sum += (float)-0.022831496;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.35238135)) {
            if ( (data[14].fvalue < 0.090213686)) {
              sum += (float)-0.0060954015;
            } else {
              sum += (float)0.031827182;
            }
          } else {
            sum += (float)-0.039929911;
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 3.6043119e-08)) {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[15].fvalue < 2.7770159)) {
        if ( (data[17].fvalue < 0.0089311562)) {
          if (  (data[4].fvalue < 1.9547362)) {
            if ( (data[15].fvalue < 0.050788179)) {
              sum += (float)0.058484096;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[15].fvalue < 1.5678487)) {
              sum += (float)0.093379296;
            } else {
              sum += (float)0.017812656;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.2329299)) {
            if ( (data[0].fvalue < 4.7302027)) {
              sum += (float)-0.092750281;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[14].fvalue < 0.56010354)) {
              sum += (float)0.053754751;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        sum += (float)-0.054584913;
      }
    } else {
      sum += (float)0.092496224;
    }
  } else {
    if ( (data[13].fvalue < 0.0042027449)) {
      sum += (float)-0.14619793;
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.23647052)) {
            if ( (data[6].fvalue < 0.25626463)) {
              sum += (float)-0.0042854748;
            } else {
              sum += (float)-0.060728393;
            }
          } else {
            if ( (data[5].fvalue < 0.41585207)) {
              sum += (float)0.02003343;
            } else {
              sum += (float)-0.0022310223;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.0024527521)) {
              sum += (float)0.17432782;
            } else {
              sum += (float)0.024295164;
            }
          } else {
            if ( (data[4].fvalue < 1.5237575)) {
              sum += (float)-0.079513073;
            } else {
              sum += (float)0.0063048876;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.062071856)) {
          sum += (float)0.093983375;
        } else {
          if ( (data[13].fvalue < 0.040984068)) {
            if ( (data[2].fvalue < 0.25702938)) {
              sum += (float)-0.0013056153;
            } else {
              sum += (float)-0.12000505;
            }
          } else {
            if ( (data[14].fvalue < 0.040688675)) {
              sum += (float)-0.11052805;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 1.3875037e-07)) {
    if (  (data[4].fvalue < 2.0929294)) {
      if (  (data[4].fvalue < 1.6832483)) {
        if ( (data[17].fvalue < 0.63237524)) {
          if ( (data[15].fvalue < 0.60917133)) {
            if (  (data[4].fvalue < 0.97462642)) {
              sum += (float)-0.0054193274;
            } else {
              sum += (float)0.022594549;
            }
          } else {
            sum += (float)-0.050838012;
          }
        } else {
          sum += (float)-0.077716999;
        }
      } else {
        if ( (data[15].fvalue < 0.0078037884)) {
          if (  (data[4].fvalue < 1.8961964)) {
            sum += (float)-0.012394324;
          } else {
            sum += (float)0.03716024;
          }
        } else {
          if ( (data[15].fvalue < 0.47036868)) {
            sum += (float)-0.10716694;
          } else {
            if ( (data[15].fvalue < 0.99661374)) {
              sum += (float)0.023306672;
            } else {
              sum += (float)-0.039937954;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.031595305)) {
        if ( (data[15].fvalue < 0.43098542)) {
          if (  (data[4].fvalue < 2.4933567)) {
            sum += (float)0.038643681;
          } else {
            sum += (float)-0.0068628658;
          }
        } else {
          sum += (float)0.092074268;
        }
      } else {
        if ( (data[13].fvalue < 1.0895553)) {
          if ( (data[16].fvalue < 0.94492555)) {
            if (  (data[4].fvalue < 2.4837408)) {
              sum += (float)0.073476903;
            } else {
              sum += (float)0.010919124;
            }
          } else {
            sum += (float)-0.024905223;
          }
        } else {
          sum += (float)-0.028263537;
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.0047378992)) {
      sum += (float)-0.1529429;
    } else {
      if ( (data[16].fvalue < 0.71585321)) {
        if ( (data[3].fvalue < 0.35496131)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[15].fvalue < 0.00034099503)) {
              sum += (float)-0.013161267;
            } else {
              sum += (float)0.029149964;
            }
          } else {
            if (  (data[4].fvalue < 1.2654823)) {
              sum += (float)0.13525671;
            } else {
              sum += (float)0.017832302;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.43343776)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0042818002;
            } else {
              sum += (float)-0.029980106;
            }
          } else {
            if (  (data[4].fvalue < 2.0159431)) {
              sum += (float)0.048391249;
            } else {
              sum += (float)-0.011579415;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.29668438)) {
          if ( (data[0].fvalue < 6.0534592)) {
            sum += (float)0.095855162;
          } else {
            sum += (float)0.020708445;
          }
        } else {
          sum += (float)-0.016028607;
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.852723)) {
    if ( (data[14].fvalue < 0.00015932824)) {
      sum += (float)0.069201306;
    } else {
      sum += (float)-0;
    }
  } else {
    if ( (data[16].fvalue < 0.86982548)) {
      if ( (data[14].fvalue < 0.42560151)) {
        if ( (data[7].fvalue < 7.5)) {
          if ( (data[17].fvalue < 0.25724608)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.00583563;
            } else {
              sum += (float)-0.024811108;
            }
          } else {
            if ( (data[5].fvalue < 0.33478549)) {
              sum += (float)0.022021431;
            } else {
              sum += (float)-0.021967648;
            }
          }
        } else {
          sum += (float)0.06562902;
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.60368079)) {
            if ( (data[17].fvalue < 0.91842836)) {
              sum += (float)-7.2564013e-05;
            } else {
              sum += (float)0.078813672;
            }
          } else {
            sum += (float)0.090944119;
          }
        } else {
          if ( (data[14].fvalue < 1.0412917)) {
            sum += (float)-0.074946575;
          } else {
            sum += (float)-0.0047343038;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.73382688)) {
        if ( (data[0].fvalue < 6.2790933)) {
          if ( (data[13].fvalue < 0.2428275)) {
            sum += (float)-0;
          } else {
            sum += (float)0.044426378;
          }
        } else {
          sum += (float)-0.040805798;
        }
      } else {
        sum += (float)0.1107218;
      }
    }
  }
  if ( (data[17].fvalue < 2.9916217)) {
    if ( (data[4].fvalue < 0.78620386)) {
      sum += (float)0.042410232;
    } else {
      if ( (data[12].fvalue < 1.5)) {
        if ( (data[14].fvalue < 1.3875037e-07)) {
          if ( (data[3].fvalue < 1.1671426)) {
            if ( (data[13].fvalue < 0.53152692)) {
              sum += (float)0.0026311283;
            } else {
              sum += (float)-0.029830504;
            }
          } else {
            if ( (data[16].fvalue < 0.035123535)) {
              sum += (float)0.046900243;
            } else {
              sum += (float)-0.0085490467;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0057448237)) {
            sum += (float)-0.095026001;
          } else {
            if ( (data[15].fvalue < 0.00028596941)) {
              sum += (float)-0.011943366;
            } else {
              sum += (float)0.0059549562;
            }
          }
        }
      } else {
        sum += (float)0.039279874;
      }
    }
  } else {
    sum += (float)-0.049721666;
  }
  if ( (data[6].fvalue < 0.31578225)) {
    if ( (data[7].fvalue < 0.5)) {
      if ( (data[15].fvalue < 0.050316714)) {
        sum += (float)-0.17079338;
      } else {
        if (  (data[4].fvalue < 1.5329711)) {
          if ( (data[14].fvalue < 0.13933623)) {
            sum += (float)-0;
          } else {
            sum += (float)0.042444859;
          }
        } else {
          sum += (float)-0.013490111;
        }
      }
    } else {
      if ( (data[8].fvalue < 0.5)) {
        if ( (data[6].fvalue < 0.1410839)) {
          if ( (data[0].fvalue < 3.9773505)) {
            if ( (data[3].fvalue < 0.19662613)) {
              sum += (float)-0.075491324;
            } else {
              sum += (float)-0.012981195;
            }
          } else {
            if ( (data[13].fvalue < 0.43068135)) {
              sum += (float)0.012955491;
            } else {
              sum += (float)-0.049070273;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.6468072)) {
            if ( (data[2].fvalue < 0.18100017)) {
              sum += (float)0.063349128;
            } else {
              sum += (float)0.013270281;
            }
          } else {
            sum += (float)-0.026181871;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.002591216)) {
          sum += (float)0.19350807;
        } else {
          if ( (data[15].fvalue < 0.10619079)) {
            if ( (data[0].fvalue < 1.546813)) {
              sum += (float)0.082693838;
            } else {
              sum += (float)-0.0004039031;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.04252148;
            } else {
              sum += (float)0.12975948;
            }
          }
        }
      }
    }
  } else {
    if ( (data[12].fvalue < 0.5)) {
      if ( (data[17].fvalue < 0.020648696)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[6].fvalue < 0.45340487)) {
            if ( (data[6].fvalue < 0.41956046)) {
              sum += (float)0.00089188572;
            } else {
              sum += (float)-0.03946203;
            }
          } else {
            if ( (data[16].fvalue < 0.022903357)) {
              sum += (float)0.036740284;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.08732608)) {
            sum += (float)-0.068174198;
          } else {
            if ( (data[13].fvalue < 0.26919493)) {
              sum += (float)-0.029217869;
            } else {
              sum += (float)0.013955693;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.12415519)) {
          if ( (data[4].fvalue < 1.8920953)) {
            if ( (data[14].fvalue < 0.013635298)) {
              sum += (float)-0.034659784;
            } else {
              sum += (float)0.015127196;
            }
          } else {
            if ( (data[13].fvalue < 1.0966042)) {
              sum += (float)0.028008927;
            } else {
              sum += (float)-0.015360013;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.77182174)) {
              sum += (float)-0.057043586;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.010686059;
            } else {
              sum += (float)-0.03145859;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 2.5316787)) {
        sum += (float)-0.090646707;
      } else {
        sum += (float)0.0070772362;
      }
    }
  }
  if ( (data[6].fvalue < 0.14420068)) {
    if ( (data[7].fvalue < 0.5)) {
      sum += (float)-0.10851019;
    } else {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[6].fvalue < 0.079609334)) {
          if ( (data[13].fvalue < 0.086210079)) {
            if ( (data[14].fvalue < 5.9668244e-05)) {
              sum += (float)0.048721258;
            } else {
              sum += (float)-0.006473301;
            }
          } else {
            if (  (data[4].fvalue < 1.0542631)) {
              sum += (float)-0.038508981;
            } else {
              sum += (float)0.015731055;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.24072409)) {
              sum += (float)0.041295152;
            } else {
              sum += (float)0.007891939;
            }
          } else {
            sum += (float)-0.011149813;
          }
        }
      } else {
        sum += (float)0.073730879;
      }
    }
  } else {
    if ( (data[6].fvalue < 0.35085869)) {
      if ( (data[14].fvalue < 0.12778491)) {
        if ( (data[2].fvalue < 0.078101203)) {
          sum += (float)0.023786863;
        } else {
          if ( (data[13].fvalue < 0.036815457)) {
            sum += (float)-0.068607971;
          } else {
            if ( (data[6].fvalue < 0.31576729)) {
              sum += (float)-0.013345055;
            } else {
              sum += (float)-0.050192323;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 3.0523615)) {
          if ( (data[5].fvalue < 0.33100355)) {
            sum += (float)-0;
          } else {
            sum += (float)0.053092059;
          }
        } else {
          if ( (data[17].fvalue < 0.31292522)) {
            if ( (data[15].fvalue < 0.30034047)) {
              sum += (float)-0.035438944;
            } else {
              sum += (float)0.0042691114;
            }
          } else {
            sum += (float)0.038394332;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[7].fvalue < 4.5)) {
          if ( (data[17].fvalue < 0.066661224)) {
            if ( (data[17].fvalue < 0.001248247)) {
              sum += (float)0.0057188505;
            } else {
              sum += (float)0.060648873;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.014873658;
            } else {
              sum += (float)0.017343281;
            }
          }
        } else {
          sum += (float)0.075256281;
        }
      } else {
        if ( (data[15].fvalue < 0.005440868)) {
          if ( (data[14].fvalue < 0.19074795)) {
            sum += (float)-0.1023342;
          } else {
            if ( (data[7].fvalue < 5.5)) {
              sum += (float)0.072099432;
            } else {
              sum += (float)-0.0033493536;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.40974438)) {
            if ( (data[14].fvalue < 0.23245378)) {
              sum += (float)0.10139275;
            } else {
              sum += (float)-0.034804355;
            }
          } else {
            if ( (data[14].fvalue < 0.44988137)) {
              sum += (float)-0.10248099;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.22518671)) {
    if ( (data[17].fvalue < 1.8305614)) {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[4].fvalue < 0.92537248)) {
          if ( (data[3].fvalue < 0.30312985)) {
            sum += (float)-0.0096380739;
          } else {
            if ( (data[13].fvalue < 0.14121675)) {
              sum += (float)0.065861188;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[13].fvalue < 3.6043119e-08)) {
            if ( (data[15].fvalue < 1.5728747)) {
              sum += (float)0.016486038;
            } else {
              sum += (float)-0.033149455;
            }
          } else {
            if ( (data[13].fvalue < 0.0054077073)) {
              sum += (float)-0.073551975;
            } else {
              sum += (float)-0.011380354;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.29412872)) {
          sum += (float)0.10542042;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.40343314)) {
        sum += (float)-0.12526907;
      } else {
        sum += (float)0.013622577;
      }
    }
  } else {
    if ( (data[17].fvalue < 1.3816874)) {
      if ( (data[16].fvalue < 1.0861961)) {
        if ( (data[4].fvalue < 1.7511232)) {
          if ( (data[5].fvalue < 0.61062235)) {
            if ( (data[15].fvalue < 0.17276923)) {
              sum += (float)0.0089142574;
            } else {
              sum += (float)0.045589179;
            }
          } else {
            if ( (data[14].fvalue < 3.2368527e-07)) {
              sum += (float)0.0063934256;
            } else {
              sum += (float)-0.031827737;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.83715922)) {
            if ( (data[17].fvalue < 0.56207895)) {
              sum += (float)-0.049356051;
            } else {
              sum += (float)-0.0030315726;
            }
          } else {
            if (  (data[4].fvalue < 2.4542694)) {
              sum += (float)0.059227899;
            } else {
              sum += (float)-0.01472135;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.57295227)) {
          sum += (float)0.082704253;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.83341771)) {
        if ( (data[6].fvalue < 0.71579337)) {
          sum += (float)0.046647966;
        } else {
          sum += (float)-0.027374554;
        }
      } else {
        sum += (float)0.099184491;
      }
    }
  }
  if ( (data[3].fvalue < 1.7747189)) {
    if ( (data[12].fvalue < 1.5)) {
      if ( (data[14].fvalue < 0.073126249)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.095162407)) {
            if ( (data[15].fvalue < 0.30983746)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.097364835;
            }
          } else {
            if (  (data[4].fvalue < 0.98129874)) {
              sum += (float)-0.024570692;
            } else {
              sum += (float)0.011161962;
            }
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[5].fvalue < 0.24470821)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.12211179;
            }
          } else {
            sum += (float)0.037682287;
          }
        }
      } else {
        if ( (data[6].fvalue < 0.069930971)) {
          sum += (float)0.063639969;
        } else {
          if ( (data[0].fvalue < 1.7397563)) {
            if ( (data[3].fvalue < 0.63447225)) {
              sum += (float)0.055383541;
            } else {
              sum += (float)-0.0034901034;
            }
          } else {
            if ( (data[16].fvalue < 0.10752957)) {
              sum += (float)-0.0041444185;
            } else {
              sum += (float)0.031061083;
            }
          }
        }
      }
    } else {
      sum += (float)0.059888851;
    }
  } else {
    if ( (data[14].fvalue < 0.75560367)) {
      if ( (data[13].fvalue < 0.8520509)) {
        if ( (data[5].fvalue < 0.50068492)) {
          if ( (data[15].fvalue < 0.50933003)) {
            if ( (data[17].fvalue < 1.3707782)) {
              sum += (float)0.05242043;
            } else {
              sum += (float)-0.015307409;
            }
          } else {
            if ( (data[16].fvalue < 0.051200081)) {
              sum += (float)-0.0056034243;
            } else {
              sum += (float)-0.057585604;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.26266944)) {
            sum += (float)-0.017543266;
          } else {
            if ( (data[13].fvalue < 0.59527516)) {
              sum += (float)-0.1195496;
            } else {
              sum += (float)-0.045401853;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 3.20331)) {
          if ( (data[17].fvalue < 0.28836465)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.041032765;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.0031019801;
          }
        } else {
          if ( (data[3].fvalue < 3.9543581)) {
            sum += (float)0.046461716;
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 1.2155683)) {
        if ( (data[17].fvalue < 0.069087535)) {
          if ( (data[15].fvalue < 1.1245345)) {
            if ( (data[5].fvalue < 0.62160504)) {
              sum += (float)-0.03469098;
            } else {
              sum += (float)0.058205802;
            }
          } else {
            sum += (float)0.06950897;
          }
        } else {
          if ( (data[14].fvalue < 1.3244638)) {
            sum += (float)-0.00022265507;
          } else {
            sum += (float)-0.091030926;
          }
        }
      } else {
        sum += (float)0.079315446;
      }
    }
  }
  if ( (data[16].fvalue < 2.0483837)) {
    if ( (data[6].fvalue < 0.25224233)) {
      if ( (data[8].fvalue < 0.5)) {
        if ( (data[3].fvalue < 0.11881013)) {
          if ( (data[15].fvalue < 0.11389045)) {
            if ( (data[13].fvalue < 0.046976231)) {
              sum += (float)-0.019969665;
            } else {
              sum += (float)-0.097153731;
            }
          } else {
            sum += (float)0.042571306;
          }
        } else {
          if ( (data[6].fvalue < 0.21032149)) {
            if ( (data[13].fvalue < 0.1865845)) {
              sum += (float)0.018696722;
            } else {
              sum += (float)-0.012895505;
            }
          } else {
            if ( (data[13].fvalue < 0.45406613)) {
              sum += (float)0.061947402;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0027253907)) {
          sum += (float)0.13682358;
        } else {
          if ( (data[15].fvalue < 0.1342679)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.096738048;
            } else {
              sum += (float)0.011129284;
            }
          } else {
            if (  (data[4].fvalue < 1.4895756)) {
              sum += (float)0.076880582;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[1].fvalue < 1.0681129)) {
        if ( (data[3].fvalue < 0.25473335)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.024778301)) {
              sum += (float)0.021683836;
            } else {
              sum += (float)-0.013563649;
            }
          } else {
            sum += (float)0.058512386;
          }
        } else {
          if ( (data[3].fvalue < 0.50264823)) {
            if ( (data[14].fvalue < 0.16909647)) {
              sum += (float)-0.02745292;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[2].fvalue < 1.4531431)) {
              sum += (float)0.011665633;
            } else {
              sum += (float)-0.0095934393;
            }
          }
        }
      } else {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[14].fvalue < 0.15768269)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.0018610008;
            } else {
              sum += (float)0.051049184;
            }
          } else {
            if ( (data[0].fvalue < 2.6990509)) {
              sum += (float)0.020986576;
            } else {
              sum += (float)-0.0200895;
            }
          }
        } else {
          sum += (float)-0.054799374;
        }
      }
    }
  } else {
    sum += (float)-0.060591713;
  }
  if ( (data[13].fvalue < 5.4519899e-07)) {
    if ( (data[15].fvalue < 0.06775023)) {
      if ( (data[14].fvalue < 0.19563)) {
        sum += (float)0.11392852;
      } else {
        sum += (float)-0.0019951756;
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[0].fvalue < 6.8873844)) {
          if ( (data[5].fvalue < 0.34809726)) {
            if ( (data[14].fvalue < 0.27758011)) {
              sum += (float)-0.020966474;
            } else {
              sum += (float)0.020604536;
            }
          } else {
            if ( (data[4].fvalue < 1.7777331)) {
              sum += (float)-0.050655544;
            } else {
              sum += (float)-0.0029695679;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.49087995)) {
            sum += (float)0.070645891;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[4].fvalue < 2.0751815)) {
          sum += (float)0.056193296;
        } else {
          sum += (float)-0;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.0098033808)) {
      if ( (data[3].fvalue < 0.18260446)) {
        sum += (float)-0;
      } else {
        sum += (float)-0.096104093;
      }
    } else {
      if ( (data[6].fvalue < 0.80477846)) {
        if ( (data[17].fvalue < 0.12408566)) {
          if ( (data[17].fvalue < 0.041437704)) {
            if ( (data[17].fvalue < 7.333768e-05)) {
              sum += (float)-0.0028034043;
            } else {
              sum += (float)0.063359976;
            }
          } else {
            if ( (data[5].fvalue < 0.31065089)) {
              sum += (float)-0.0082135722;
            } else {
              sum += (float)-0.067531563;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.5620705)) {
            if ( (data[3].fvalue < 0.308231)) {
              sum += (float)0.10572737;
            } else {
              sum += (float)0.022961866;
            }
          } else {
            if ( (data[17].fvalue < 0.41348886)) {
              sum += (float)-0.019356601;
            } else {
              sum += (float)0.012428942;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.68929541)) {
          if ( (data[0].fvalue < 3.065599)) {
            sum += (float)-0;
          } else {
            if ( (data[14].fvalue < 0.032672614)) {
              sum += (float)-0.098211862;
            } else {
              sum += (float)-0.020265127;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if (  (data[4].fvalue < 2.223947)) {
              sum += (float)-0.014356248;
            } else {
              sum += (float)0.03770544;
            }
          } else {
            sum += (float)-0.041537583;
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
