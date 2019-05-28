#include "BDTReweighter.h"
class GeneratorReweight_nuebar_FD_RHC : public BDTReweighter {
 public :
 GeneratorReweight_nuebar_FD_RHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_nuebar_FD_RHC() {;}


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
  //  unsigned int tmp;
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[13].fvalue < 0.053468198)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.014939306)) {
              sum += (float)-0.13955763;
            } else {
              sum += (float)-0.37289721;
            }
          } else {
            if ( (data[13].fvalue < 2.720001e-06)) {
              sum += (float)0.58904111;
            } else {
              sum += (float)-0.0021044689;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.3146125e-05)) {
            if (  (data[4].fvalue < 1.3045831)) {
              sum += (float)0.0064836442;
            } else {
              sum += (float)0.1221429;
            }
          } else {
            if ( (data[13].fvalue < 0.014029884)) {
              sum += (float)-0.29998043;
            } else {
              sum += (float)-0.050657403;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[14].fvalue < 0.23586041)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.33117634;
            } else {
              sum += (float)0.031567831;
            }
          } else {
            if ( (data[13].fvalue < 0.032475658)) {
              sum += (float)-0.46941778;
            } else {
              sum += (float)-0.33761793;
            }
          }
        } else {
          sum += (float)0.16896529;
        }
      }
    } else {
      if ( (data[7].fvalue < 5.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.024179304)) {
            if ( (data[17].fvalue < 0.018638004)) {
              sum += (float)0.035124492;
            } else {
              sum += (float)-0.14383021;
            }
          } else {
            if (  (data[4].fvalue < 1.0595379)) {
              sum += (float)-0.073591724;
            } else {
              sum += (float)0.16621861;
            }
          }
        } else {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.099813052;
            } else {
              sum += (float)0.048478104;
            }
          } else {
            if ( (data[3].fvalue < 0.8635065)) {
              sum += (float)0.19362865;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.48111436)) {
          if ( (data[3].fvalue < 0.54647785)) {
            sum += (float)0.34740177;
          } else {
            sum += (float)0.16200975;
          }
        } else {
          sum += (float)0.44504315;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 7.5)) {
      if ( (data[14].fvalue < 0.13373187)) {
        if ( (data[13].fvalue < 0.12544234)) {
          sum += (float)0.52929801;
        } else {
          sum += (float)0.32383877;
        }
      } else {
        if ( (data[15].fvalue < 0.0024088034)) {
          if ( (data[8].fvalue < 6.5)) {
            if ( (data[13].fvalue < 0.13121396)) {
              sum += (float)0.11995758;
            } else {
              sum += (float)0.26538214;
            }
          } else {
            if ( (data[3].fvalue < 1.0572777)) {
              sum += (float)0.33822921;
            } else {
              sum += (float)0.15755971;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.037539922)) {
            sum += (float)-0.067898557;
          } else {
            sum += (float)0.11091594;
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.00063737942)) {
        if ( (data[7].fvalue < 5.5)) {
          if ( (data[3].fvalue < 0.94019878)) {
            if (  (data[4].fvalue < 1.1348317)) {
              sum += (float)0.57178146;
            } else {
              sum += (float)0.45412311;
            }
          } else {
            sum += (float)0.28210902;
          }
        } else {
          if ( (data[3].fvalue < 1.2380285)) {
            sum += (float)0.58121645;
          } else {
            sum += (float)0.39268404;
          }
        }
      } else {
        sum += (float)0.28468505;
      }
    }
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[13].fvalue < 0.18662131)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.024445895)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.0006458991)) {
              sum += (float)-0.083286896;
            } else {
              sum += (float)-0.26703832;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.034459535;
            } else {
              sum += (float)-0.17994949;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.10174308)) {
              sum += (float)-0.26988751;
            } else {
              sum += (float)-0.1085596;
            }
          } else {
            if ( (data[14].fvalue < 4.9061885e-07)) {
              sum += (float)0.56914914;
            } else {
              sum += (float)0.062184062;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.11900643)) {
          if ( (data[6].fvalue < 0.10283117)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.16920909;
            } else {
              sum += (float)0.26284015;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.025959287;
            } else {
              sum += (float)-0.24310039;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.39435607)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.013337661;
            } else {
              sum += (float)0.2371781;
            }
          } else {
            if ( (data[14].fvalue < 0.67172498)) {
              sum += (float)-0.0944997;
            } else {
              sum += (float)-0.25072816;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.23260674)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.011177133)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.039581258;
            } else {
              sum += (float)0.2419716;
            }
          } else {
            if ( (data[6].fvalue < 0.13799331)) {
              sum += (float)0.019270705;
            } else {
              sum += (float)0.20938252;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.11321426)) {
              sum += (float)-0.019892802;
            } else {
              sum += (float)0.17722732;
            }
          } else {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)-0.19845344;
            } else {
              sum += (float)0.085862719;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.30170658)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[2].fvalue < 0.45671421)) {
              sum += (float)0.19264232;
            } else {
              sum += (float)0.075361088;
            }
          } else {
            if ( (data[3].fvalue < 0.3230336)) {
              sum += (float)0.10440511;
            } else {
              sum += (float)-0.030742794;
            }
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.23040946;
            } else {
              sum += (float)0.097385637;
            }
          } else {
            if ( (data[16].fvalue < 0.0013824929)) {
              sum += (float)0.34858096;
            } else {
              sum += (float)0.17738959;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 7.5)) {
      if ( (data[14].fvalue < 0.17479908)) {
        if (  (data[4].fvalue < 1.1243914)) {
          sum += (float)0.43866712;
        } else {
          if ( (data[10].fvalue < 0.5)) {
            sum += (float)0.10241541;
          } else {
            sum += (float)0.38211364;
          }
        }
      } else {
        if ( (data[7].fvalue < 5.5)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)0.30227214;
          } else {
            if ( (data[13].fvalue < 0.041677304)) {
              sum += (float)-0.090147473;
            } else {
              sum += (float)0.14206459;
            }
          }
        } else {
          sum += (float)0.29809093;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.25339049)) {
        sum += (float)0.45115086;
      } else {
        if ( (data[8].fvalue < 9.5)) {
          if ( (data[17].fvalue < 0.018073427)) {
            sum += (float)0.30310887;
          } else {
            sum += (float)0.11692616;
          }
        } else {
          sum += (float)0.41531768;
        }
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[13].fvalue < 0.0713965)) {
      if ( (data[14].fvalue < 0.15510675)) {
        if ( (data[8].fvalue < 3.5)) {
          if (  (data[4].fvalue < 0.92243004)) {
            if ( (data[13].fvalue < 3.2581404e-05)) {
              sum += (float)0.10376317;
            } else {
              sum += (float)-0.12427405;
            }
          } else {
            if ( (data[17].fvalue < 0.72391284)) {
              sum += (float)-0.10147549;
            } else {
              sum += (float)-0.26076558;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[6].fvalue < 0.06060914)) {
              sum += (float)0.30504039;
            } else {
              sum += (float)0.0061145178;
            }
          } else {
            if ( (data[14].fvalue < 0.10920455)) {
              sum += (float)0.38601607;
            } else {
              sum += (float)0.19010276;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.36147386)) {
            if ( (data[14].fvalue < 1.158653)) {
              sum += (float)0.038270224;
            } else {
              sum += (float)-0.17250313;
            }
          } else {
            if ( (data[6].fvalue < 0.061209541)) {
              sum += (float)-0.19728744;
            } else {
              sum += (float)-0.032965094;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1201085)) {
            if ( (data[3].fvalue < 0.46130332)) {
              sum += (float)0.20636284;
            } else {
              sum += (float)-0.28211418;
            }
          } else {
            if ( (data[16].fvalue < 0.0097043607)) {
              sum += (float)-0.30150226;
            } else {
              sum += (float)-0.14255115;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 5.5)) {
        if ( (data[5].fvalue < 0.41413051)) {
          if ( (data[13].fvalue < 1.2180533)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.10001516;
            } else {
              sum += (float)0.019133691;
            }
          } else {
            if ( (data[16].fvalue < 0.47918367)) {
              sum += (float)-0.21059273;
            } else {
              sum += (float)0.034302097;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.14483802;
            } else {
              sum += (float)0.052971248;
            }
          } else {
            if ( (data[3].fvalue < 0.39757934)) {
              sum += (float)0.051108617;
            } else {
              sum += (float)-0.096207991;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.98122656)) {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[3].fvalue < 0.39584684)) {
              sum += (float)0.2316293;
            } else {
              sum += (float)0.12622029;
            }
          } else {
            sum += (float)0.32400486;
          }
        } else {
          sum += (float)0.086180612;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[14].fvalue < 0.19095588)) {
        sum += (float)0.34647739;
      } else {
        if ( (data[15].fvalue < 0.002976781)) {
          if ( (data[3].fvalue < 0.85963559)) {
            if (  (data[4].fvalue < 1.2078906)) {
              sum += (float)0.31257829;
            } else {
              sum += (float)0.18847789;
            }
          } else {
            if ( (data[13].fvalue < 0.22179338)) {
              sum += (float)0.0021785586;
            } else {
              sum += (float)0.15096416;
            }
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.7964876)) {
        if ( (data[3].fvalue < 1.1489236)) {
          sum += (float)0.38666651;
        } else {
          sum += (float)0.22970746;
        }
      } else {
        sum += (float)0.21598843;
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[13].fvalue < 0.15188712)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.14250174)) {
            if (  (data[4].fvalue < 0.90075409)) {
              sum += (float)0.076426484;
            } else {
              sum += (float)-0.063765712;
            }
          } else {
            if ( (data[13].fvalue < 0.0096126441)) {
              sum += (float)-0.020836562;
            } else {
              sum += (float)0.065186791;
            }
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.1329517)) {
              sum += (float)-0.076890454;
            } else {
              sum += (float)-0.21437939;
            }
          } else {
            if ( (data[3].fvalue < 0.27615368)) {
              sum += (float)0.065327294;
            } else {
              sum += (float)-0.11121502;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.027078653)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.016383249)) {
              sum += (float)0.045885786;
            } else {
              sum += (float)0.14354439;
            }
          } else {
            if ( (data[14].fvalue < 0.13439894)) {
              sum += (float)-0.14861794;
            } else {
              sum += (float)0.051459827;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0069289641)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.21688412;
            } else {
              sum += (float)0.065666236;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.099340461;
            } else {
              sum += (float)-0.07624843;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.10135483)) {
        sum += (float)0.3237471;
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.42029387)) {
            if ( (data[13].fvalue < 0.02341703)) {
              sum += (float)-0;
            } else {
              sum += (float)0.13865508;
            }
          } else {
            if ( (data[8].fvalue < 6.5)) {
              sum += (float)0.1635143;
            } else {
              sum += (float)0.28670388;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.31485528)) {
            if (  (data[4].fvalue < 1.2038591)) {
              sum += (float)0.27031881;
            } else {
              sum += (float)0.099159732;
            }
          } else {
            if ( (data[13].fvalue < 0.05659692)) {
              sum += (float)-0.14081059;
            } else {
              sum += (float)0.051162686;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 6.5)) {
      if ( (data[6].fvalue < 0.18291447)) {
        if ( (data[8].fvalue < 4.5)) {
          sum += (float)0.14957914;
        } else {
          sum += (float)0.34386384;
        }
      } else {
        if ( (data[3].fvalue < 1.4608059)) {
          if ( (data[15].fvalue < 0.0025445854)) {
            if ( (data[3].fvalue < 0.21182667)) {
              sum += (float)0.22754724;
            } else {
              sum += (float)0.12119963;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)-0.051380739;
        }
      }
    } else {
      if ( (data[4].fvalue < 1.8800788)) {
        if ( (data[3].fvalue < 1.2718959)) {
          sum += (float)0.34958315;
        } else {
          sum += (float)0.15250404;
        }
      } else {
        sum += (float)0.17912978;
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[13].fvalue < 0.035361897)) {
      if ( (data[15].fvalue < 0.037278406)) {
        if ( (data[13].fvalue < 1.9009443e-06)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.047862511;
            } else {
              sum += (float)0.038771227;
            }
          } else {
            sum += (float)0.48948824;
          }
        } else {
          if ( (data[13].fvalue < 0.0045359256)) {
            if ( (data[13].fvalue < 0.0009284009)) {
              sum += (float)-0.28053224;
            } else {
              sum += (float)-0.18676935;
            }
          } else {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.082714431;
            } else {
              sum += (float)0.046986967;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.81765205)) {
            if ( (data[16].fvalue < 0.80909109)) {
              sum += (float)-0.16941573;
            } else {
              sum += (float)-0.019759905;
            }
          } else {
            sum += (float)-0.31082243;
          }
        } else {
          if ( (data[16].fvalue < 0.0019005891)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.061271921;
            } else {
              sum += (float)-0.24588086;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.036883838;
            } else {
              sum += (float)-0.083320469;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.31134319)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.018508639)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.022792909;
            } else {
              sum += (float)0.12215832;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.16370074;
            } else {
              sum += (float)0.048484199;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.06362889;
            } else {
              sum += (float)0.047232296;
            }
          } else {
            if ( (data[15].fvalue < 0.0084545948)) {
              sum += (float)0.080878735;
            } else {
              sum += (float)-0.053235095;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.078511417)) {
          if ( (data[14].fvalue < 4.4401897e-08)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.06269224;
            } else {
              sum += (float)0.48345521;
            }
          } else {
            if ( (data[14].fvalue < 0.027222907)) {
              sum += (float)-0.16356707;
            } else {
              sum += (float)-0.03494085;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.21885186)) {
            sum += (float)0.26617023;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.076985672;
            } else {
              sum += (float)-0.019400384;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[14].fvalue < 0.17034815)) {
        sum += (float)0.27833015;
      } else {
        if ( (data[3].fvalue < 0.48455292)) {
          if (  (data[4].fvalue < 1.2531986)) {
            sum += (float)0.23649989;
          } else {
            if ( (data[14].fvalue < 0.40542555)) {
              sum += (float)0.034983318;
            } else {
              sum += (float)0.16592579;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.14896879)) {
            if ( (data[6].fvalue < 0.32178965)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.070539787;
            }
          } else {
            if ( (data[14].fvalue < 0.61389291)) {
              sum += (float)0.031272992;
            } else {
              sum += (float)0.14542891;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.37425852)) {
        sum += (float)0.32705921;
      } else {
        if ( (data[4].fvalue < 1.7491878)) {
          if ( (data[3].fvalue < 0.87021327)) {
            sum += (float)0.27891174;
          } else {
            sum += (float)0.15300521;
          }
        } else {
          sum += (float)0.10824003;
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[8].fvalue < 4.5)) {
      if (  (data[4].fvalue < 1.0381787)) {
        if ( (data[14].fvalue < 9.1076854e-06)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.075000972)) {
              sum += (float)-0.43842453;
            } else {
              sum += (float)-0.093172863;
            }
          } else {
            sum += (float)0.50990671;
          }
        } else {
          if ( (data[13].fvalue < 1.3638012e-06)) {
            if ( (data[14].fvalue < 0.033590823)) {
              sum += (float)0.17991111;
            } else {
              sum += (float)-0.056531381;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.15324467;
            } else {
              sum += (float)0.10620418;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.015041737)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.0070252917)) {
              sum += (float)-0.012676404;
            } else {
              sum += (float)0.051430658;
            }
          } else {
            if ( (data[13].fvalue < 1.3173534e-05)) {
              sum += (float)0.040627062;
            } else {
              sum += (float)-0.073801838;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3849974)) {
            if ( (data[14].fvalue < 0.29217917)) {
              sum += (float)-0.30939192;
            } else {
              sum += (float)-0.10293832;
            }
          } else {
            if ( (data[14].fvalue < 0.96104574)) {
              sum += (float)-0.045449726;
            } else {
              sum += (float)-0.20647097;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1074004)) {
        if ( (data[3].fvalue < 0.68035185)) {
          if ( (data[3].fvalue < 0.42090166)) {
            sum += (float)0.39163914;
          } else {
            sum += (float)0.23635837;
          }
        } else {
          if ( (data[3].fvalue < 1.0790057)) {
            sum += (float)0.068740822;
          } else {
            sum += (float)-0.082409613;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.13516247)) {
          if ( (data[14].fvalue < 0.51691747)) {
            if ( (data[17].fvalue < 0.11920072)) {
              sum += (float)-0.089533009;
            } else {
              sum += (float)0.11543332;
            }
          } else {
            if ( (data[13].fvalue < 0.24484827)) {
              sum += (float)0.053298313;
            } else {
              sum += (float)0.22891477;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.37258005)) {
            sum += (float)0.26607904;
          } else {
            if ( (data[0].fvalue < 2.8627501)) {
              sum += (float)0.15010719;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[6].fvalue < 0.14393592)) {
        if ( (data[4].fvalue < 1.1357874)) {
          sum += (float)0.32992482;
        } else {
          sum += (float)0.16365188;
        }
      } else {
        if ( (data[3].fvalue < 0.9178502)) {
          if (  (data[4].fvalue < 1.8616962)) {
            if ( (data[16].fvalue < 0.14112607)) {
              sum += (float)0.11330549;
            } else {
              sum += (float)0.24820565;
            }
          } else {
            sum += (float)-0.029630074;
          }
        } else {
          if ( (data[13].fvalue < 0.57463771)) {
            if ( (data[5].fvalue < 0.50683582)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.13308732;
            }
          } else {
            sum += (float)0.086560264;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.8827888)) {
        sum += (float)0.3075411;
      } else {
        sum += (float)0.17974937;
      }
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[12].fvalue < 0.5)) {
      if (  (data[4].fvalue < 1.2957215)) {
        if ( (data[8].fvalue < 4.5)) {
          if (  (data[4].fvalue < 0.87741685)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.11271588;
            } else {
              sum += (float)-0.032155171;
            }
          } else {
            if ( (data[14].fvalue < 0.47856697)) {
              sum += (float)-0.034108784;
            } else {
              sum += (float)0.034430221;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1250546)) {
            if ( (data[3].fvalue < 0.64494956)) {
              sum += (float)0.29449105;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[16].fvalue < 0.0026102215)) {
              sum += (float)-0.053685874;
            } else {
              sum += (float)0.19215521;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.026313227)) {
          if ( (data[15].fvalue < 0.048780166)) {
            if (  (data[4].fvalue < 1.5085891)) {
              sum += (float)0.025190147;
            } else {
              sum += (float)0.080771215;
            }
          } else {
            if ( (data[4].fvalue < 1.5354198)) {
              sum += (float)-0.14284827;
            } else {
              sum += (float)-0.00062880217;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.024046769)) {
            if ( (data[3].fvalue < 0.15558518)) {
              sum += (float)0.16483112;
            } else {
              sum += (float)0.021537209;
            }
          } else {
            if ( (data[13].fvalue < 0.0019788854)) {
              sum += (float)-0.18636443;
            } else {
              sum += (float)-0.054397956;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.7111827)) {
        if ( (data[3].fvalue < 0.47343898)) {
          sum += (float)0.20025492;
        } else {
          sum += (float)-0.12078128;
        }
      } else {
        if (  (data[4].fvalue < 2.1403043)) {
          sum += (float)-0.43308142;
        } else {
          sum += (float)-0.26159698;
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.0020633303)) {
      if ( (data[8].fvalue < 9.5)) {
        if ( (data[14].fvalue < 0.19339104)) {
          sum += (float)0.25951913;
        } else {
          if ( (data[3].fvalue < 0.54621351)) {
            if (  (data[4].fvalue < 1.3467323)) {
              sum += (float)0.20098247;
            } else {
              sum += (float)0.096933492;
            }
          } else {
            sum += (float)0.052197341;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.40429696)) {
          sum += (float)0.3101674;
        } else {
          sum += (float)0.21003248;
        }
      }
    } else {
      sum += (float)-0.036323041;
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.20214903)) {
      if ( (data[13].fvalue < 0.01464794)) {
        if ( (data[16].fvalue < 0.03053578)) {
          if ( (data[14].fvalue < 3.1136352e-07)) {
            if (  (data[4].fvalue < 1.5564907)) {
              sum += (float)0.13244261;
            } else {
              sum += (float)0.59703571;
            }
          } else {
            if ( (data[5].fvalue < 1.1064051)) {
              sum += (float)-0.039421596;
            } else {
              sum += (float)0.042960145;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.11013614)) {
            if ( (data[14].fvalue < 0.11608142)) {
              sum += (float)-0.20739746;
            } else {
              sum += (float)-0.084740639;
            }
          } else {
            if ( (data[3].fvalue < 0.18497705)) {
              sum += (float)-0.015281324;
            } else {
              sum += (float)-0.1154241;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.21468395)) {
          if ( (data[16].fvalue < 0.0012189348)) {
            if (  (data[4].fvalue < 1.13899)) {
              sum += (float)0.051643938;
            } else {
              sum += (float)-0.086439446;
            }
          } else {
            if (  (data[4].fvalue < 1.7411844)) {
              sum += (float)0.12733305;
            } else {
              sum += (float)-0.073511891;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0094984649)) {
              sum += (float)-0.038601;
            } else {
              sum += (float)0.04988141;
            }
          } else {
            if ( (data[14].fvalue < 0.047608398)) {
              sum += (float)-0.25771055;
            } else {
              sum += (float)-0.035813477;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.84940302)) {
        if ( (data[14].fvalue < 1.3752277)) {
          if ( (data[13].fvalue < 0.26212198)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.021020615;
            } else {
              sum += (float)-0.061012842;
            }
          } else {
            if ( (data[14].fvalue < 0.47684324)) {
              sum += (float)0.047023781;
            } else {
              sum += (float)0.15717767;
            }
          }
        } else {
          if ( (data[4].fvalue < 2.0463395)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.080438912;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[17].fvalue < 0.28236204)) {
              sum += (float)-0.26238939;
            } else {
              sum += (float)-0.10012009;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.39247331)) {
          if ( (data[17].fvalue < 0.038563371)) {
            if ( (data[14].fvalue < 0.46647924)) {
              sum += (float)0.18072662;
            } else {
              sum += (float)0.33344397;
            }
          } else {
            sum += (float)0.099110045;
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.12560222)) {
      sum += (float)0.28410441;
    } else {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[3].fvalue < 1.3607588)) {
          if (  (data[4].fvalue < 1.864507)) {
            if ( (data[13].fvalue < 0.55001026)) {
              sum += (float)0.084638551;
            } else {
              sum += (float)0.18337372;
            }
          } else {
            sum += (float)-0.020900952;
          }
        } else {
          sum += (float)-0.084385179;
        }
      } else {
        sum += (float)0.2589753;
      }
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[6].fvalue < 0.012761859)) {
      sum += (float)-0.26017925;
    } else {
      if ( (data[15].fvalue < 0.0082080103)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[13].fvalue < 1.0871977)) {
              sum += (float)0.0017248821;
            } else {
              sum += (float)-0.13489379;
            }
          } else {
            if ( (data[6].fvalue < 0.12394325)) {
              sum += (float)0.20491177;
            } else {
              sum += (float)0.039340261;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.763171)) {
            if ( (data[3].fvalue < 0.50577933)) {
              sum += (float)0.069041885;
            } else {
              sum += (float)-0.12744792;
            }
          } else {
            sum += (float)-0.33126205;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.0002705174)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.36111248)) {
              sum += (float)-0.11182468;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 1.592679)) {
              sum += (float)-0.30220741;
            } else {
              sum += (float)-0.13001904;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.0028321492)) {
              sum += (float)-0.11741991;
            } else {
              sum += (float)0.004881857;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.067638144;
            } else {
              sum += (float)-0.074598514;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.2406991)) {
      if ( (data[7].fvalue < 3.5)) {
        sum += (float)0.13200414;
      } else {
        sum += (float)0.27641812;
      }
    } else {
      if ( (data[8].fvalue < 10.5)) {
        if ( (data[0].fvalue < 2.7215157)) {
          sum += (float)0.11251238;
        } else {
          if ( (data[5].fvalue < 0.3278949)) {
            sum += (float)0.035749123;
          } else {
            sum += (float)-0.049361233;
          }
        }
      } else {
        sum += (float)0.21924888;
      }
    }
  }
  if ( (data[7].fvalue < 7.5)) {
    if ( (data[14].fvalue < 0.098928772)) {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[17].fvalue < 0.044767424)) {
          if ( (data[13].fvalue < 0.22185472)) {
            if ( (data[13].fvalue < 7.1504623e-08)) {
              sum += (float)-0.00033522287;
            } else {
              sum += (float)-0.06160428;
            }
          } else {
            if ( (data[16].fvalue < 0.37012678)) {
              sum += (float)-0.013179835;
            } else {
              sum += (float)0.089557543;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0040907268)) {
            if ( (data[14].fvalue < 0.031834252)) {
              sum += (float)-0.26965049;
            } else {
              sum += (float)-0.087775648;
            }
          } else {
            if ( (data[14].fvalue < 0.0048699658)) {
              sum += (float)-0.10033291;
            } else {
              sum += (float)0.021286776;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.14961755)) {
          if ( (data[3].fvalue < 0.15751995)) {
            if (  (data[4].fvalue < 1.0136926)) {
              sum += (float)0.38183558;
            } else {
              sum += (float)0.20483287;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.049117699;
            } else {
              sum += (float)0.21648704;
            }
          }
        } else {
          sum += (float)-0.14614221;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.40598342)) {
        if (  (data[4].fvalue < 1.0302576)) {
          if ( (data[3].fvalue < 0.23334379)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.20229881;
            } else {
              sum += (float)0.025077909;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.020653144;
            } else {
              sum += (float)0.11872197;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.1381704)) {
            if ( (data[3].fvalue < 0.34122929)) {
              sum += (float)0.13474539;
            } else {
              sum += (float)0.013658056;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.024276286;
            } else {
              sum += (float)-0.03360242;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.024687968)) {
          if ( (data[5].fvalue < 0.33698821)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.17307128;
            } else {
              sum += (float)0.067297213;
            }
          } else {
            if ( (data[5].fvalue < 0.50684333)) {
              sum += (float)0.055679895;
            } else {
              sum += (float)-0.031858642;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.41512367)) {
            if ( (data[14].fvalue < 0.66506517)) {
              sum += (float)-0.021482291;
            } else {
              sum += (float)-0.19063607;
            }
          } else {
            if ( (data[16].fvalue < 0.60100949)) {
              sum += (float)-0;
            } else {
              sum += (float)0.14608033;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.22904891)) {
      sum += (float)0.28387427;
    } else {
      sum += (float)0.15380833;
    }
  }
  if ( (data[8].fvalue < 9.5)) {
    if ( (data[5].fvalue < 0.17534037)) {
      if ( (data[14].fvalue < 0.99061966)) {
        if (  (data[4].fvalue < 1.2350227)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[16].fvalue < 0.097108081)) {
              sum += (float)-0.059328578;
            } else {
              sum += (float)0.031642847;
            }
          } else {
            sum += (float)0.11088911;
          }
        } else {
          if ( (data[14].fvalue < 0.032344706)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.02275015;
            } else {
              sum += (float)0.081695564;
            }
          } else {
            if ( (data[16].fvalue < 0.16086408)) {
              sum += (float)0.033757575;
            } else {
              sum += (float)0.08756046;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.034200434)) {
          if ( (data[8].fvalue < 3.5)) {
            sum += (float)-0.054982807;
          } else {
            sum += (float)0.012421543;
          }
        } else {
          if ( (data[16].fvalue < 0.45421177)) {
            if ( (data[17].fvalue < 0.044499464)) {
              sum += (float)-0.16486353;
            } else {
              sum += (float)-0.30075365;
            }
          } else {
            sum += (float)0.0082530035;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 1.2299583)) {
        if ( (data[14].fvalue < 0.39595717)) {
          if (  (data[4].fvalue < 1.1394824)) {
            if (  (data[4].fvalue < 1.0359542)) {
              sum += (float)-0.024732692;
            } else {
              sum += (float)0.040714469;
            }
          } else {
            if ( (data[13].fvalue < 0.029199325)) {
              sum += (float)-0.065177262;
            } else {
              sum += (float)-0.014380643;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.42931062)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.014124682;
            } else {
              sum += (float)-0.044685408;
            }
          } else {
            if ( (data[3].fvalue < 1.8157423)) {
              sum += (float)0.20718907;
            } else {
              sum += (float)0.034952465;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[13].fvalue < 1.679614e-06)) {
            if ( (data[3].fvalue < 0.50179976)) {
              sum += (float)0.014389261;
            } else {
              sum += (float)0.12815094;
            }
          } else {
            if ( (data[13].fvalue < 0.015198941)) {
              sum += (float)-0.12682188;
            } else {
              sum += (float)0.00068496261;
            }
          }
        } else {
          sum += (float)0.20259853;
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.40340984)) {
      sum += (float)0.24829651;
    } else {
      sum += (float)0.069132119;
    }
  }
  if ( (data[15].fvalue < 0.0085463654)) {
    if ( (data[8].fvalue < 7.5)) {
      if ( (data[5].fvalue < 0.26393372)) {
        if ( (data[14].fvalue < 0.033358615)) {
          if ( (data[13].fvalue < 0.11140387)) {
            if ( (data[16].fvalue < 0.027162574)) {
              sum += (float)0.030424604;
            } else {
              sum += (float)-0.074970029;
            }
          } else {
            if ( (data[16].fvalue < 0.38565451)) {
              sum += (float)-0.024558399;
            } else {
              sum += (float)0.10089101;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0352505)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.23417167;
            } else {
              sum += (float)0.013418135;
            }
          } else {
            if (  (data[4].fvalue < 1.1157539)) {
              sum += (float)0.23048858;
            } else {
              sum += (float)0.026321106;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0249012)) {
              sum += (float)-0.36861557;
            } else {
              sum += (float)-0.07188677;
            }
          } else {
            if ( (data[13].fvalue < 0.25851569)) {
              sum += (float)-0.083681785;
            } else {
              sum += (float)0.23231591;
            }
          }
        } else {
          if ( (data[14].fvalue < 3.1136352e-07)) {
            sum += (float)0.43031919;
          } else {
            if ( (data[14].fvalue < 0.0056407806)) {
              sum += (float)-0.17199981;
            } else {
              sum += (float)-0.0069680423;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.5517174)) {
        if ( (data[14].fvalue < 0.20091647)) {
          sum += (float)0.25272211;
        } else {
          if ( (data[6].fvalue < 0.39312014)) {
            if ( (data[14].fvalue < 0.38298482)) {
              sum += (float)0.08135321;
            } else {
              sum += (float)0.19123986;
            }
          } else {
            sum += (float)0.040235989;
          }
        }
      } else {
        if ( (data[6].fvalue < 0.3310625)) {
          sum += (float)0.065804131;
        } else {
          sum += (float)-0;
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.4328921)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)-0.18593886;
        } else {
          sum += (float)-0.018627707;
        }
      } else {
        sum += (float)-0.25307193;
      }
    } else {
      if ( (data[17].fvalue < 0.035940051)) {
        if ( (data[14].fvalue < 0.022453062)) {
          sum += (float)-0.1096267;
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.94943029)) {
              sum += (float)-0.031007329;
            } else {
              sum += (float)0.12454473;
            }
          } else {
            if (  (data[4].fvalue < 2.0201039)) {
              sum += (float)0.13498802;
            } else {
              sum += (float)0.0026935127;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.6713071)) {
          sum += (float)0.016758777;
        } else {
          if ( (data[16].fvalue < 0.71414375)) {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)-0.15848428;
            } else {
              sum += (float)-0.0637495;
            }
          } else {
            sum += (float)-0.0040868274;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[5].fvalue < 0.02593736)) {
      if (  (data[4].fvalue < 1.8198864)) {
        if (  (data[4].fvalue < 1.4284527)) {
          if ( (data[16].fvalue < 0.29055128)) {
            if ( (data[16].fvalue < 0.11189031)) {
              sum += (float)0.022646561;
            } else {
              sum += (float)0.14739665;
            }
          } else {
            sum += (float)-0.072521277;
          }
        } else {
          if ( (data[14].fvalue < 0.48425004)) {
            if ( (data[14].fvalue < 0.0032688677)) {
              sum += (float)0.090544961;
            } else {
              sum += (float)0.20381914;
            }
          } else {
            sum += (float)0.038907271;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.70484161)) {
          sum += (float)-0.11583201;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[15].fvalue < 0.023134923)) {
        if ( (data[13].fvalue < 1.4287186)) {
          if ( (data[13].fvalue < 0.23077042)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.0035038525;
            } else {
              sum += (float)-0.03407814;
            }
          } else {
            if ( (data[14].fvalue < 0.35638523)) {
              sum += (float)0.014921524;
            } else {
              sum += (float)0.088804133;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.40271544)) {
            if (  (data[4].fvalue < 2.0338197)) {
              sum += (float)-0.068177521;
            } else {
              sum += (float)-0.27969617;
            }
          } else {
            sum += (float)0.014807778;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.11505893)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.0018518671)) {
              sum += (float)-0.1304397;
            } else {
              sum += (float)-0.017330425;
            }
          } else {
            if ( (data[16].fvalue < 0.051949322)) {
              sum += (float)-0.28441748;
            } else {
              sum += (float)-0.1522132;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.0019681093)) {
              sum += (float)-0.12143931;
            } else {
              sum += (float)0.026238959;
            }
          } else {
            if ( (data[17].fvalue < 0.046394058)) {
              sum += (float)0.037451822;
            } else {
              sum += (float)-0.031427242;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.25718385)) {
      if ( (data[14].fvalue < 0.17063084)) {
        sum += (float)0.25963327;
      } else {
        sum += (float)0.13599069;
      }
    } else {
      if ( (data[16].fvalue < 0.010281635)) {
        sum += (float)0.12111998;
      } else {
        sum += (float)-0.0096478211;
      }
    }
  }
  if ( (data[8].fvalue < 10.5)) {
    if ( (data[6].fvalue < 0.013820498)) {
      sum += (float)-0.17122923;
    } else {
      if ( (data[14].fvalue < 1.7770241)) {
        if ( (data[16].fvalue < 0.14664538)) {
          if ( (data[16].fvalue < 0.049816482)) {
            if ( (data[17].fvalue < 0.15271834)) {
              sum += (float)-0.0048747407;
            } else {
              sum += (float)0.060600989;
            }
          } else {
            if ( (data[3].fvalue < 0.30934668)) {
              sum += (float)-0.018027226;
            } else {
              sum += (float)-0.080576032;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.047612749)) {
            if ( (data[14].fvalue < 0.025663307)) {
              sum += (float)-0.0080575189;
            } else {
              sum += (float)0.056575049;
            }
          } else {
            if ( (data[16].fvalue < 1.1278701)) {
              sum += (float)-0.073383473;
            } else {
              sum += (float)0.070621178;
            }
          }
        }
      } else {
        if ( (data[2].fvalue < 0.5738712)) {
          sum += (float)-0.16626875;
        } else {
          sum += (float)-0.023000496;
        }
      }
    }
  } else {
    sum += (float)0.19857821;
  }
  if ( (data[7].fvalue < 8.5)) {
    if ( (data[17].fvalue < 0.043345638)) {
      if ( (data[11].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.29323414)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[4].fvalue < 0.73612475)) {
              sum += (float)0.167703;
            } else {
              sum += (float)0.0015207019;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.07179179;
            } else {
              sum += (float)0.018475473;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.45688599)) {
            if ( (data[16].fvalue < 0.52380848)) {
              sum += (float)0.0018419209;
            } else {
              sum += (float)0.079002038;
            }
          } else {
            if ( (data[13].fvalue < 0.43902308)) {
              sum += (float)0.060000051;
            } else {
              sum += (float)0.19697325;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[3].fvalue < 0.69731033)) {
            if ( (data[17].fvalue < 0.014697154)) {
              sum += (float)0.2514995;
            } else {
              sum += (float)0.10252309;
            }
          } else {
            if ( (data[17].fvalue < 0.022542097)) {
              sum += (float)0.040873867;
            } else {
              sum += (float)-0.039257433;
            }
          }
        } else {
          sum += (float)-0.086934343;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.12598929)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.01867079)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.15034974;
            } else {
              sum += (float)-0.045683894;
            }
          } else {
            if ( (data[14].fvalue < 0.057802122)) {
              sum += (float)-0.10431653;
            } else {
              sum += (float)-0.017562531;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.34204704)) {
            sum += (float)-0.099244222;
          } else {
            sum += (float)-0.2741276;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.010884786)) {
          if ( (data[14].fvalue < 0.002160816)) {
            sum += (float)-0.27603248;
          } else {
            if ( (data[3].fvalue < 0.21211627)) {
              sum += (float)0.13467397;
            } else {
              sum += (float)0.014372726;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.003220784)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.15151243;
            } else {
              sum += (float)0.033355284;
            }
          } else {
            if ( (data[13].fvalue < 0.80608761)) {
              sum += (float)0.011060107;
            } else {
              sum += (float)-0.15683797;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.18335471;
  }
  if ( (data[3].fvalue < 1.5904534)) {
    if ( (data[13].fvalue < 0.040055178)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[5].fvalue < 1.0491358)) {
          if ( (data[14].fvalue < 0.10368639)) {
            if ( (data[16].fvalue < 0.034243651)) {
              sum += (float)0.003331858;
            } else {
              sum += (float)-0.056008805;
            }
          } else {
            if (  (data[4].fvalue < 1.0458784)) {
              sum += (float)-0.036939155;
            } else {
              sum += (float)0.013815001;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.04371582)) {
            if ( (data[3].fvalue < 0.13851632)) {
              sum += (float)-0.053573523;
            } else {
              sum += (float)0.18003154;
            }
          } else {
            if ( (data[3].fvalue < 0.22167644)) {
              sum += (float)-0.21559627;
            } else {
              sum += (float)0.025542142;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[3].fvalue < 0.28487134)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.13285665;
            } else {
              sum += (float)0.024040041;
            }
          } else {
            if ( (data[13].fvalue < 0.025396854)) {
              sum += (float)-0.25585878;
            } else {
              sum += (float)-0.11395458;
            }
          }
        } else {
          sum += (float)0.16167277;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.8647854)) {
        if ( (data[16].fvalue < 0.13090906)) {
          if ( (data[14].fvalue < 0.49519563)) {
            if ( (data[17].fvalue < 0.13886046)) {
              sum += (float)-0.0084069129;
            } else {
              sum += (float)0.069279604;
            }
          } else {
            if ( (data[13].fvalue < 0.36589062)) {
              sum += (float)0.053838268;
            } else {
              sum += (float)0.19233696;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0090044681)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0086604515;
            } else {
              sum += (float)0.16227548;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.14055052;
            } else {
              sum += (float)0.071517743;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.89410549)) {
          if ( (data[17].fvalue < 0.77349389)) {
            if ( (data[15].fvalue < 0.27078965)) {
              sum += (float)-0.13214301;
            } else {
              sum += (float)-0.013901875;
            }
          } else {
            if ( (data[17].fvalue < 1.1410592)) {
              sum += (float)-0;
            } else {
              sum += (float)0.1305663;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.36495623)) {
            if ( (data[14].fvalue < 0.0091642654)) {
              sum += (float)-0.029143535;
            } else {
              sum += (float)0.084644035;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.28127956;
            } else {
              sum += (float)0.011482524;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.8865595)) {
      if ( (data[8].fvalue < 2.5)) {
        if (  (data[4].fvalue < 1.156198)) {
          if ( (data[16].fvalue < 0.016699523)) {
            if ( (data[14].fvalue < 0.84986651)) {
              sum += (float)0.090526596;
            } else {
              sum += (float)0.0046469891;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[6].fvalue < 0.62836957)) {
            if ( (data[13].fvalue < 0.50874144)) {
              sum += (float)-0.074518502;
            } else {
              sum += (float)-0.00070385792;
            }
          } else {
            sum += (float)-0.16536346;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.8784318)) {
          if ( (data[5].fvalue < 0.65588045)) {
            sum += (float)-0.078438044;
          } else {
            sum += (float)-0.2794489;
          }
        } else {
          sum += (float)-0.018868478;
        }
      }
    } else {
      if ( (data[6].fvalue < 0.81330401)) {
        if ( (data[7].fvalue < 0.5)) {
          sum += (float)0.088671304;
        } else {
          if ( (data[13].fvalue < 0.40105927)) {
            sum += (float)-0.053339433;
          } else {
            sum += (float)0.0276442;
          }
        }
      } else {
        sum += (float)0.16074084;
      }
    }
  }
  if ( (data[12].fvalue < 0.5)) {
    if ( (data[13].fvalue < 0.020096729)) {
      if ( (data[13].fvalue < 1.7547034e-06)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.022274032)) {
              sum += (float)0.009817292;
            } else {
              sum += (float)-0.058756657;
            }
          } else {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.0099496935;
            } else {
              sum += (float)0.086036816;
            }
          }
        } else {
          sum += (float)0.40752968;
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          sum += (float)-0.32010317;
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.045672886)) {
              sum += (float)-0.068947107;
            } else {
              sum += (float)0.12732938;
            }
          } else {
            if ( (data[3].fvalue < 0.35449106)) {
              sum += (float)-0.036172166;
            } else {
              sum += (float)-0.19811879;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[6].fvalue < 0.065583423)) {
          if ( (data[14].fvalue < 1.329573e-06)) {
            sum += (float)0.039683495;
          } else {
            if (  (data[4].fvalue < 1.0301754)) {
              sum += (float)-0.18897249;
            } else {
              sum += (float)0.039040357;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.010286564)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.019415593;
            } else {
              sum += (float)0.084149331;
            }
          } else {
            if ( (data[13].fvalue < 0.049407147)) {
              sum += (float)0.17685564;
            } else {
              sum += (float)0.056605753;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.081955776)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.031561084)) {
              sum += (float)-0.1309192;
            } else {
              sum += (float)0.10976402;
            }
          } else {
            if (  (data[4].fvalue < 1.091403)) {
              sum += (float)0.28740984;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.022875827)) {
            sum += (float)-0.24106009;
          } else {
            if ( (data[3].fvalue < 0.93295461)) {
              sum += (float)-0.0033192188;
            } else {
              sum += (float)-0.070288427;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.741307)) {
      if ( (data[3].fvalue < 0.54211861)) {
        sum += (float)0.10271928;
      } else {
        sum += (float)-0.060785107;
      }
    } else {
      if ( (data[0].fvalue < 3.8167896)) {
        sum += (float)-0.29706031;
      } else {
        sum += (float)-0.16027512;
      }
    }
  }
  if ( (data[6].fvalue < 0.91347873)) {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[14].fvalue < 1.0627028e-06)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.061642028;
            } else {
              sum += (float)-0.053207967;
            }
          } else {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.16123526;
            } else {
              sum += (float)0.16158275;
            }
          }
        } else {
          sum += (float)0.39530671;
        }
      } else {
        if ( (data[14].fvalue < 0.0055625597)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.04192524)) {
              sum += (float)-0.20099895;
            } else {
              sum += (float)0.014420985;
            }
          } else {
            sum += (float)-0.25088832;
          }
        } else {
          if ( (data[16].fvalue < 0.117387)) {
            if ( (data[13].fvalue < 0.48477614)) {
              sum += (float)-0.012966076;
            } else {
              sum += (float)0.047742032;
            }
          } else {
            if ( (data[3].fvalue < 0.075152084)) {
              sum += (float)0.12553939;
            } else {
              sum += (float)0.004540788;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.17498122)) {
        if ( (data[4].fvalue < 1.1293752)) {
          if ( (data[0].fvalue < 3.0238445)) {
            sum += (float)0.27430439;
          } else {
            sum += (float)0.12921569;
          }
        } else {
          if ( (data[14].fvalue < 0.33170226)) {
            if ( (data[13].fvalue < 0.066693455)) {
              sum += (float)-0.047059704;
            } else {
              sum += (float)0.039234117;
            }
          } else {
            sum += (float)0.18034644;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.68198544)) {
            if ( (data[13].fvalue < 0.030136786)) {
              sum += (float)-0.007855094;
            } else {
              sum += (float)0.11754028;
            }
          } else {
            sum += (float)0.19119401;
          }
        } else {
          if ( (data[0].fvalue < 1.5225142)) {
            sum += (float)0.11982981;
          } else {
            if ( (data[16].fvalue < 0.19740944)) {
              sum += (float)-0.049297784;
            } else {
              sum += (float)0.063548841;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.63973641)) {
      sum += (float)0.024468249;
    } else {
      sum += (float)0.25658697;
    }
  }
  if ( (data[11].fvalue < 1.5)) {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[14].fvalue < 0.31714308)) {
        if ( (data[3].fvalue < 0.47554141)) {
          if ( (data[16].fvalue < 0.16126946)) {
            if (  (data[4].fvalue < 1.148533)) {
              sum += (float)0.0086607849;
            } else {
              sum += (float)-0.028461952;
            }
          } else {
            if ( (data[13].fvalue < 0.0066866092)) {
              sum += (float)-0.0019314705;
            } else {
              sum += (float)0.064404249;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.03984201)) {
              sum += (float)0.048785903;
            } else {
              sum += (float)-0.021099135;
            }
          } else {
            if ( (data[14].fvalue < 0.086678147)) {
              sum += (float)-0.19016328;
            } else {
              sum += (float)-0.053454287;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.56881511)) {
          if ( (data[13].fvalue < 0.43292928)) {
            if ( (data[4].fvalue < 1.7659335)) {
              sum += (float)0.01085364;
            } else {
              sum += (float)-0.056715768;
            }
          } else {
            if ( (data[17].fvalue < 0.013483977)) {
              sum += (float)0.12887722;
            } else {
              sum += (float)0.00068590359;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.37870502)) {
            if ( (data[16].fvalue < 0.013490647)) {
              sum += (float)0.23106477;
            } else {
              sum += (float)0.077170141;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      sum += (float)0.16950451;
    }
  } else {
    if ( (data[17].fvalue < 1.1982687)) {
      if ( (data[5].fvalue < 0.46065146)) {
        if (  (data[4].fvalue < 1.7537839)) {
          if ( (data[17].fvalue < 0.20709294)) {
            sum += (float)-0.082685106;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.00046208623;
            } else {
              sum += (float)0.086031832;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.58016747)) {
            if ( (data[0].fvalue < 3.3057842)) {
              sum += (float)-0.082843632;
            } else {
              sum += (float)-0.18849652;
            }
          } else {
            if (  (data[4].fvalue < 1.9487066)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.093649879;
            }
          }
        }
      } else {
        sum += (float)-0.20037717;
      }
    } else {
      if ( (data[14].fvalue < 0.24388236)) {
        sum += (float)-0.031015804;
      } else {
        sum += (float)0.13534623;
      }
    }
  }
  if ( (data[16].fvalue < 0.43847516)) {
    if ( (data[4].fvalue < 1.7136707)) {
      if ( (data[16].fvalue < 3.2933731e-05)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.25890619)) {
            sum += (float)-0.32865804;
          } else {
            sum += (float)0.10925159;
          }
        } else {
          if ( (data[14].fvalue < 4.4401897e-08)) {
            sum += (float)0.36002582;
          } else {
            if ( (data[13].fvalue < 0.43899286)) {
              sum += (float)-0.0099734971;
            } else {
              sum += (float)0.070963204;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.0059614065)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[5].fvalue < 0.33627373)) {
              sum += (float)-0.020535972;
            } else {
              sum += (float)-0.099433623;
            }
          } else {
            if ( (data[14].fvalue < 0.053080283)) {
              sum += (float)-0.13289598;
            } else {
              sum += (float)0.078715444;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3703783)) {
            if ( (data[5].fvalue < 0.62666464)) {
              sum += (float)0.10357231;
            } else {
              sum += (float)-0.020158656;
            }
          } else {
            if ( (data[14].fvalue < 0.19749179)) {
              sum += (float)-0.034093935;
            } else {
              sum += (float)0.0474937;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.062577501)) {
        if ( (data[8].fvalue < 0.5)) {
          sum += (float)0.25625196;
        } else {
          if ( (data[17].fvalue < 0.65044427)) {
            if ( (data[17].fvalue < 0.053131059)) {
              sum += (float)0.029177893;
            } else {
              sum += (float)-0.089942783;
            }
          } else {
            if ( (data[14].fvalue < 0.26007092)) {
              sum += (float)-0.020784106;
            } else {
              sum += (float)0.11691999;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.74825537)) {
          if ( (data[9].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.9599952)) {
              sum += (float)-0.11356391;
            } else {
              sum += (float)-0.21013035;
            }
          } else {
            if ( (data[1].fvalue < 1.1337177)) {
              sum += (float)-0.10300605;
            } else {
              sum += (float)-0.026385201;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.47631055)) {
            sum += (float)-0.051388498;
          } else {
            if ( (data[17].fvalue < 1.2906363)) {
              sum += (float)-0.0021399725;
            } else {
              sum += (float)0.098832257;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.033385731)) {
      if ( (data[13].fvalue < 0.27155936)) {
        if ( (data[3].fvalue < 0.30785954)) {
          if (  (data[4].fvalue < 1.9095104)) {
            if (  (data[4].fvalue < 1.4355884)) {
              sum += (float)-0.070081025;
            } else {
              sum += (float)0.053847391;
            }
          } else {
            if ( (data[0].fvalue < 3.6439648)) {
              sum += (float)-0.071839496;
            } else {
              sum += (float)-0.0095690433;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.56135952)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.14797181;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.8854854)) {
            if ( (data[3].fvalue < 0.60581726)) {
              sum += (float)0.10924184;
            } else {
              sum += (float)-0.0025008894;
            }
          } else {
            if ( (data[16].fvalue < 0.79988754)) {
              sum += (float)0.10235394;
            } else {
              sum += (float)0.23028551;
            }
          }
        } else {
          if ( (data[16].fvalue < 1.0624112)) {
            sum += (float)-0.092092924;
          } else {
            sum += (float)0.074970305;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 1.3378172)) {
        if ( (data[3].fvalue < 0.33707571)) {
          if (  (data[4].fvalue < 1.9218626)) {
            if ( (data[13].fvalue < 0.0005875983)) {
              sum += (float)0.071309157;
            } else {
              sum += (float)0.16783775;
            }
          } else {
            if ( (data[16].fvalue < 0.74588734)) {
              sum += (float)-0.081517965;
            } else {
              sum += (float)0.039502434;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0054644444)) {
            if ( (data[14].fvalue < 0.30266875)) {
              sum += (float)-0.075097181;
            } else {
              sum += (float)0.04694948;
            }
          } else {
            if (  (data[4].fvalue < 2.1808681)) {
              sum += (float)0.060558658;
            } else {
              sum += (float)-0.047259998;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.33032703)) {
          if ( (data[13].fvalue < 0.0010840528)) {
            sum += (float)-0.0011756296;
          } else {
            sum += (float)0.093964122;
          }
        } else {
          sum += (float)0.21365893;
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.84465492)) {
    if ( (data[13].fvalue < 5.5666419e-06)) {
      if ( (data[8].fvalue < 1.5)) {
        if (  (data[4].fvalue < 0.75130522)) {
          sum += (float)0.20486669;
        } else {
          if ( (data[3].fvalue < 0.67668509)) {
            sum += (float)0.013254405;
          } else {
            if ( (data[14].fvalue < 0.40171307)) {
              sum += (float)0.13712944;
            } else {
              sum += (float)0.017582746;
            }
          }
        }
      } else {
        sum += (float)0.0096609164;
      }
    } else {
      if ( (data[3].fvalue < 1.3016343)) {
        if ( (data[5].fvalue < 1.4295859)) {
          sum += (float)0.0057081506;
        } else {
          sum += (float)0.071616851;
        }
      } else {
        sum += (float)-0.086109094;
      }
    }
  } else {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[6].fvalue < 0.87474459)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 3.5)) {
            if (  (data[4].fvalue < 1.0195446)) {
              sum += (float)-0.021805845;
            } else {
              sum += (float)0.0086943209;
            }
          } else {
            if (  (data[4].fvalue < 1.1073718)) {
              sum += (float)0.12101733;
            } else {
              sum += (float)-0.08820717;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.090043694)) {
              sum += (float)-0.099461533;
            } else {
              sum += (float)0.11994072;
            }
          } else {
            if ( (data[6].fvalue < 0.053838879)) {
              sum += (float)0.151969;
            } else {
              sum += (float)-0.03793085;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.64049041)) {
          if ( (data[13].fvalue < 0.09891367)) {
            sum += (float)0.017733948;
          } else {
            sum += (float)-0.031477019;
          }
        } else {
          sum += (float)0.17429164;
        }
      }
    } else {
      if ( (data[3].fvalue < 1.2802994)) {
        if (  (data[4].fvalue < 1.1306469)) {
          if ( (data[3].fvalue < 0.6061368)) {
            sum += (float)0.23295344;
          } else {
            sum += (float)0.062176969;
          }
        } else {
          if ( (data[3].fvalue < 0.15185674)) {
            if ( (data[14].fvalue < 0.27265483)) {
              sum += (float)0.024001405;
            } else {
              sum += (float)0.1194355;
            }
          } else {
            if ( (data[14].fvalue < 0.76867449)) {
              sum += (float)-0.013945109;
            } else {
              sum += (float)0.087744482;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.77959955)) {
          sum += (float)-0.1705934;
        } else {
          sum += (float)-0.00026328524;
        }
      }
    }
  }
  if ( (data[13].fvalue < 1.9368734)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[13].fvalue < 0.010081796)) {
            if ( (data[13].fvalue < 1.7840924e-06)) {
              sum += (float)0.0029677027;
            } else {
              sum += (float)-0.065326631;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.027903581;
            } else {
              sum += (float)0.03801319;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0863481)) {
            if ( (data[3].fvalue < 0.26857793)) {
              sum += (float)0.20348303;
            } else {
              sum += (float)-0.035895254;
            }
          } else {
            if ( (data[16].fvalue < 0.13235581)) {
              sum += (float)-0.10365845;
            } else {
              sum += (float)0.063879445;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[16].fvalue < 0.0095666163)) {
            sum += (float)-0.096173726;
          } else {
            if ( (data[13].fvalue < 0.12099656)) {
              sum += (float)-0;
            } else {
              sum += (float)0.11214466;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.015296618)) {
            if ( (data[3].fvalue < 0.11899258)) {
              sum += (float)-0.027342733;
            } else {
              sum += (float)-0.18972635;
            }
          } else {
            if ( (data[6].fvalue < 0.068972677)) {
              sum += (float)0.12601133;
            } else {
              sum += (float)-0.029398492;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.0711025)) {
        if ( (data[3].fvalue < 0.93645197)) {
          if ( (data[3].fvalue < 0.41906843)) {
            sum += (float)0.28137264;
          } else {
            sum += (float)0.11969124;
          }
        } else {
          sum += (float)-0.031669188;
        }
      } else {
        if ( (data[16].fvalue < 0.1125633)) {
          if ( (data[14].fvalue < 0.42094439)) {
            if ( (data[17].fvalue < 0.10858653)) {
              sum += (float)-0.080834381;
            } else {
              sum += (float)0.11403905;
            }
          } else {
            if ( (data[17].fvalue < 0.011383613)) {
              sum += (float)0.060359221;
            } else {
              sum += (float)-0.051747885;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.72054)) {
            if ( (data[3].fvalue < 0.90088034)) {
              sum += (float)0.20181401;
            } else {
              sum += (float)0.032096621;
            }
          } else {
            if ( (data[16].fvalue < 0.54133165)) {
              sum += (float)-0.064492598;
            } else {
              sum += (float)0.047430925;
            }
          }
        }
      }
    }
  } else {
    sum += (float)-0.12780596;
  }
  if ( (data[5].fvalue < 0.0038151923)) {
    if ( (data[6].fvalue < 0.19796938)) {
      sum += (float)0.028121037;
    } else {
      sum += (float)0.13666812;
    }
  } else {
    if ( (data[13].fvalue < 0.12546146)) {
      if ( (data[7].fvalue < 4.5)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[6].fvalue < 0.015257308)) {
            if ( (data[0].fvalue < 3.753984)) {
              sum += (float)-0.28501806;
            } else {
              sum += (float)1.1370954e-05;
            }
          } else {
            if ( (data[14].fvalue < 0.23287517)) {
              sum += (float)-0.0087559381;
            } else {
              sum += (float)0.0063346727;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.029249392)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.15094046;
            } else {
              sum += (float)0.03506428;
            }
          } else {
            if ( (data[6].fvalue < 0.076990731)) {
              sum += (float)0.099013224;
            } else {
              sum += (float)-0.030334283;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.1405855)) {
          sum += (float)0.21888478;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[6].fvalue < 0.34324086)) {
        if ( (data[16].fvalue < 0.22145101)) {
          if ( (data[14].fvalue < 0.29682219)) {
            if (  (data[4].fvalue < 1.2981272)) {
              sum += (float)0.0274093;
            } else {
              sum += (float)-0.028727908;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.012724809;
            } else {
              sum += (float)0.11030065;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6006525)) {
            if ( (data[3].fvalue < 0.19190262)) {
              sum += (float)0.17910744;
            } else {
              sum += (float)0.084557436;
            }
          } else {
            if ( (data[16].fvalue < 0.45696819)) {
              sum += (float)-0.014136256;
            } else {
              sum += (float)0.058009084;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.89588475)) {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.25142729)) {
              sum += (float)-0.027147675;
            } else {
              sum += (float)0.023223042;
            }
          } else {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.063266777;
            } else {
              sum += (float)-0.23452309;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6349249)) {
            sum += (float)-0.10952754;
          } else {
            if ( (data[13].fvalue < 0.49229532)) {
              sum += (float)0.025975086;
            } else {
              sum += (float)0.14173691;
            }
          }
        }
      }
    }
  }
  if ( (data[3].fvalue < 0.18441872)) {
    if (  (data[4].fvalue < 1.0533041)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[14].fvalue < 0.045505106)) {
          if ( (data[13].fvalue < 1.0011885e-05)) {
            if ( (data[6].fvalue < 0.019627057)) {
              sum += (float)-0.016635574;
            } else {
              sum += (float)0.19158381;
            }
          } else {
            if ( (data[14].fvalue < 3.1136352e-07)) {
              sum += (float)0.0467963;
            } else {
              sum += (float)-0.17070106;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.028796297)) {
            if ( (data[0].fvalue < 3.4594092)) {
              sum += (float)-0.34719619;
            } else {
              sum += (float)-0.16209793;
            }
          } else {
            if ( (data[14].fvalue < 0.12403978)) {
              sum += (float)-0.034800403;
            } else {
              sum += (float)-0.22575609;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99142802)) {
          sum += (float)0.33026677;
        } else {
          if ( (data[14].fvalue < 0.063058794)) {
            sum += (float)-0.094113253;
          } else {
            sum += (float)0.20277739;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1331488)) {
        if ( (data[13].fvalue < 0.009436395)) {
          if ( (data[14].fvalue < 0.1013193)) {
            sum += (float)-0.037182439;
          } else {
            if ( (data[5].fvalue < 0.33275104)) {
              sum += (float)0.12666094;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.13411394)) {
            if ( (data[13].fvalue < 0.042545326)) {
              sum += (float)0.06748911;
            } else {
              sum += (float)0.21168308;
            }
          } else {
            sum += (float)0.32177219;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.00091253128)) {
          if ( (data[11].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.24497116)) {
              sum += (float)-0.058654744;
            } else {
              sum += (float)0.087507732;
            }
          } else {
            if ( (data[14].fvalue < 0.018485092)) {
              sum += (float)-0.12076461;
            } else {
              sum += (float)0.084099218;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.085264221)) {
            if ( (data[13].fvalue < 0.019521117)) {
              sum += (float)-0.015858388;
            } else {
              sum += (float)0.048570186;
            }
          } else {
            if (  (data[4].fvalue < 1.7769601)) {
              sum += (float)0.080940165;
            } else {
              sum += (float)-0.047821365;
            }
          }
        }
      }
    }
  } else {
    if ( (data[12].fvalue < 0.5)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[16].fvalue < 0.044112027)) {
            if ( (data[14].fvalue < 8.1779235e-06)) {
              sum += (float)0.11817855;
            } else {
              sum += (float)0.012297988;
            }
          } else {
            if ( (data[13].fvalue < 0.0030040271)) {
              sum += (float)-0.035412658;
            } else {
              sum += (float)0.01467586;
            }
          }
        } else {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.018805614;
            } else {
              sum += (float)-0.12751862;
            }
          } else {
            if ( (data[13].fvalue < 0.57292181)) {
              sum += (float)0.0076750019;
            } else {
              sum += (float)0.22725385;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.15187323)) {
          if ( (data[14].fvalue < 0.58821404)) {
            if (  (data[4].fvalue < 1.0707268)) {
              sum += (float)0.013442249;
            } else {
              sum += (float)-0.067034163;
            }
          } else {
            if ( (data[13].fvalue < 0.14849779)) {
              sum += (float)-0.0025554681;
            } else {
              sum += (float)0.08508572;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.37181354)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.04209242;
            } else {
              sum += (float)0.13591103;
            }
          } else {
            if ( (data[13].fvalue < 0.25851929)) {
              sum += (float)0.035710711;
            } else {
              sum += (float)-0.039983384;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.6141317)) {
        sum += (float)0.051857479;
      } else {
        if (  (data[4].fvalue < 1.8267105)) {
          sum += (float)-0.10648677;
        } else {
          sum += (float)-0.22325887;
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.93097657)) {
    if ( (data[14].fvalue < 0.27923411)) {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[6].fvalue < 0.05484692)) {
              sum += (float)-0.029219843;
            } else {
              sum += (float)0.0031668127;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.063565813;
            } else {
              sum += (float)-0.11342349;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.080349267)) {
            if ( (data[14].fvalue < 0.047736578)) {
              sum += (float)-0.067291193;
            } else {
              sum += (float)0.11121839;
            }
          } else {
            if ( (data[17].fvalue < 0.13047057)) {
              sum += (float)-0.075429775;
            } else {
              sum += (float)0.074137539;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.71251392)) {
          if (  (data[4].fvalue < 1.158103)) {
            if ( (data[3].fvalue < 0.35114288)) {
              sum += (float)0.25027138;
            } else {
              sum += (float)0.12185927;
            }
          } else {
            if ( (data[16].fvalue < 0.052396134)) {
              sum += (float)-0;
            } else {
              sum += (float)0.07077574;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.30874211)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.018859426;
            } else {
              sum += (float)-0.13228902;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.62500346)) {
        if ( (data[3].fvalue < 2.0415792)) {
          if ( (data[13].fvalue < 0.41809323)) {
            if ( (data[3].fvalue < 0.077040106)) {
              sum += (float)0.049436949;
            } else {
              sum += (float)0.0051756678;
            }
          } else {
            if (  (data[4].fvalue < 1.687654)) {
              sum += (float)0.11599609;
            } else {
              sum += (float)0.010498065;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)-0.042233471;
          } else {
            sum += (float)-0.13697301;
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.33719951)) {
            if ( (data[13].fvalue < 0.0014534872)) {
              sum += (float)0.18342362;
            } else {
              sum += (float)0.039928868;
            }
          } else {
            sum += (float)0.00088027987;
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.021592049)) {
      if ( (data[16].fvalue < 0.43321174)) {
        if ( (data[16].fvalue < 0.046841998)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[5].fvalue < 0.23216352)) {
              sum += (float)-0.10235501;
            } else {
              sum += (float)0.022579361;
            }
          } else {
            sum += (float)0.080558665;
          }
        } else {
          if ( (data[16].fvalue < 0.2898773)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.12844704;
            } else {
              sum += (float)-0.040954266;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[0].fvalue < 3.9709358)) {
          sum += (float)0.13622923;
        } else {
          sum += (float)0.013602892;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.49113244)) {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)-0.20842263;
        } else {
          if (  (data[4].fvalue < 1.9162886)) {
            sum += (float)-0;
          } else {
            sum += (float)-0.12617514;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.7570051)) {
          sum += (float)-0.0048416927;
        } else {
          sum += (float)0.10036238;
        }
      }
    }
  }
  if ( (data[11].fvalue < 1.5)) {
    if ( (data[14].fvalue < 0.24636625)) {
      if ( (data[3].fvalue < 0.71553886)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[13].fvalue < 0.74294853)) {
            if ( (data[16].fvalue < 0.14111303)) {
              sum += (float)-0.0076122875;
            } else {
              sum += (float)0.017340725;
            }
          } else {
            if ( (data[16].fvalue < 0.39126378)) {
              sum += (float)-0.11517908;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0911751)) {
            sum += (float)0.19956048;
          } else {
            if ( (data[17].fvalue < 0.16356423)) {
              sum += (float)-0;
            } else {
              sum += (float)0.1566955;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8178703)) {
          if ( (data[13].fvalue < 0.25552845)) {
            if ( (data[5].fvalue < 1.2768042)) {
              sum += (float)-0.11079844;
            } else {
              sum += (float)0.0025685609;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.0057224464;
            } else {
              sum += (float)-0.14791602;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.039311834)) {
            sum += (float)0.09168902;
          } else {
            if ( (data[16].fvalue < 0.66366565)) {
              sum += (float)-0.037621498;
            } else {
              sum += (float)0.051709294;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.51219285)) {
        if ( (data[16].fvalue < 0.56804872)) {
          if ( (data[17].fvalue < 0.56584096)) {
            if ( (data[4].fvalue < 1.7413123)) {
              sum += (float)0.0068760938;
            } else {
              sum += (float)-0.054820675;
            }
          } else {
            if ( (data[5].fvalue < 0.30882061)) {
              sum += (float)0.10836343;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.49683169)) {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.10180745;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.025303394;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.47932744)) {
          if (  (data[4].fvalue < 1.9288206)) {
            if ( (data[3].fvalue < 1.2301891)) {
              sum += (float)0.10347005;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.039315559;
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            sum += (float)0.18114483;
          } else {
            sum += (float)0.065189868;
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.4255922)) {
      sum += (float)-0.15712264;
    } else {
      if ( (data[14].fvalue < 0.65256989)) {
        if ( (data[13].fvalue < 0.31439394)) {
          if ( (data[13].fvalue < 0.004698752)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.062856421;
            } else {
              sum += (float)0.036489721;
            }
          } else {
            if ( (data[13].fvalue < 0.056673642)) {
              sum += (float)0.11076844;
            } else {
              sum += (float)0.012880043;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.55806905)) {
            sum += (float)-0.037653163;
          } else {
            sum += (float)-0.1207253;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)-0.17411165;
        } else {
          sum += (float)-0.029800924;
        }
      }
    }
  }
  if ( (data[3].fvalue < 2.1578174)) {
    if ( (data[5].fvalue < 0.0090467166)) {
      if ( (data[6].fvalue < 0.22536269)) {
        if ( (data[13].fvalue < 0.0078653647)) {
          sum += (float)-0.0032357685;
        } else {
          sum += (float)0.066310555;
        }
      } else {
        if (  (data[4].fvalue < 1.6906099)) {
          if ( (data[0].fvalue < 1.7760606)) {
            sum += (float)0.059532236;
          } else {
            sum += (float)0.14760503;
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.17197555)) {
        if ( (data[8].fvalue < 8.5)) {
          if ( (data[6].fvalue < 0.81609124)) {
            if ( (data[3].fvalue < 0.48691308)) {
              sum += (float)0.0016645868;
            } else {
              sum += (float)-0.013380154;
            }
          } else {
            if ( (data[13].fvalue < 0.024381723)) {
              sum += (float)0.094760418;
            } else {
              sum += (float)-0.0076334365;
            }
          }
        } else {
          sum += (float)0.1036939;
        }
      } else {
        if ( (data[14].fvalue < 0.15358503)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.011542729;
            } else {
              sum += (float)-0.046588734;
            }
          } else {
            if (  (data[4].fvalue < 1.2596779)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.14585875;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3638463)) {
            if ( (data[3].fvalue < 1.1415542)) {
              sum += (float)0.09357737;
            } else {
              sum += (float)-0.012373867;
            }
          } else {
            if ( (data[15].fvalue < 0.11445294)) {
              sum += (float)0.0096804248;
            } else {
              sum += (float)0.086733676;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 0.46349609)) {
      sum += (float)0.01966944;
    } else {
      if ( (data[5].fvalue < 0.91196346)) {
        if ( (data[13].fvalue < 0.72234166)) {
          if ( (data[14].fvalue < 1.3920879)) {
            if ( (data[16].fvalue < 0.55999047)) {
              sum += (float)-0.15343571;
            } else {
              sum += (float)-0.066889212;
            }
          } else {
            sum += (float)-0.024028243;
          }
        } else {
          sum += (float)-0.0175202;
        }
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[4].fvalue < 1.1549996)) {
    if ( (data[7].fvalue < 2.5)) {
      if (  (data[4].fvalue < 1.0595841)) {
        if ( (data[13].fvalue < 1.0458019e-05)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.023927039)) {
              sum += (float)0.13844149;
            } else {
              sum += (float)-0.01169515;
            }
          } else {
            sum += (float)0.36845145;
          }
        } else {
          if ( (data[14].fvalue < 0.051105306)) {
            if ( (data[14].fvalue < 3.07398e-05)) {
              sum += (float)0.043276969;
            } else {
              sum += (float)-0.099903941;
            }
          } else {
            if ( (data[3].fvalue < 0.14980033)) {
              sum += (float)0.1196954;
            } else {
              sum += (float)-0.0276605;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.28434938)) {
          if ( (data[13].fvalue < 0.012951963)) {
            if ( (data[14].fvalue < 0.12251777)) {
              sum += (float)-0.064564027;
            } else {
              sum += (float)0.074652545;
            }
          } else {
            if (  (data[4].fvalue < 1.1376574)) {
              sum += (float)0.15504973;
            } else {
              sum += (float)0.029214343;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.23026535)) {
              sum += (float)-0.0015232109;
            } else {
              sum += (float)0.079714112;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.082575247;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.33463961)) {
        if (  (data[4].fvalue < 1.0141098)) {
          sum += (float)0.30899847;
        } else {
          if ( (data[14].fvalue < 0.10372598)) {
            sum += (float)0.052136187;
          } else {
            sum += (float)0.25074762;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.085625589)) {
          sum += (float)-0.11780179;
        } else {
          if ( (data[3].fvalue < 0.77610511)) {
            if (  (data[4].fvalue < 1.0613046)) {
              sum += (float)0.21763359;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[3].fvalue < 1.3366585)) {
              sum += (float)0.0023330653;
            } else {
              sum += (float)-0.092108324;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.29492265)) {
      if ( (data[16].fvalue < 0.11020148)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[16].fvalue < 0.057139315)) {
            if ( (data[17].fvalue < 0.33956087)) {
              sum += (float)0.03379669;
            } else {
              sum += (float)-0.044449322;
            }
          } else {
            if ( (data[13].fvalue < 0.0018555908)) {
              sum += (float)-0.094857253;
            } else {
              sum += (float)-0.028512655;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.10656445)) {
            if ( (data[13].fvalue < 0.3253085)) {
              sum += (float)-0.11705009;
            } else {
              sum += (float)-0.012010936;
            }
          } else {
            if ( (data[13].fvalue < 0.24683066)) {
              sum += (float)0.091678277;
            } else {
              sum += (float)-0.068787411;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 3.5)) {
          if (  (data[4].fvalue < 1.2333884)) {
            if ( (data[3].fvalue < 0.085392825)) {
              sum += (float)0.082412779;
            } else {
              sum += (float)0.017840989;
            }
          } else {
            if ( (data[14].fvalue < 0.17766425)) {
              sum += (float)-0.016537171;
            } else {
              sum += (float)0.025223285;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.31546789)) {
            if ( (data[14].fvalue < 0.17968725)) {
              sum += (float)0.18754609;
            } else {
              sum += (float)0.069398567;
            }
          } else {
            sum += (float)-0.11567355;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.99641597)) {
        if (  (data[4].fvalue < 2.0168457)) {
          if ( (data[16].fvalue < 0.30390143)) {
            if (  (data[4].fvalue < 1.3884215)) {
              sum += (float)0.0192526;
            } else {
              sum += (float)-0.013480564;
            }
          } else {
            if (  (data[4].fvalue < 1.3545153)) {
              sum += (float)-0.054221261;
            } else {
              sum += (float)0.053922821;
            }
          }
        } else {
          if ( (data[16].fvalue < 1.0814219)) {
            if ( (data[15].fvalue < 0.55590653)) {
              sum += (float)-0.089487925;
            } else {
              sum += (float)0.025490001;
            }
          } else {
            sum += (float)0.092678308;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.34307304)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.18370888;
          } else {
            sum += (float)0.063287482;
          }
        } else {
          sum += (float)-0.0022798295;
        }
      }
    }
  }
  if ( (data[10].fvalue < 1.5)) {
    if ( (data[13].fvalue < 0.33151615)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[6].fvalue < 0.022356786)) {
          if ( (data[14].fvalue < 0.016589358)) {
            if ( (data[14].fvalue < 0.0056519751)) {
              sum += (float)-0.026198534;
            } else {
              sum += (float)0.13351156;
            }
          } else {
            if ( (data[0].fvalue < 3.217164)) {
              sum += (float)-0.27210814;
            } else {
              sum += (float)-0.046195548;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.0561614)) {
            if ( (data[16].fvalue < 0.85930991)) {
              sum += (float)0.0053526592;
            } else {
              sum += (float)-0.043364242;
            }
          } else {
            if ( (data[5].fvalue < 0.063145623)) {
              sum += (float)-0.016749443;
            } else {
              sum += (float)0.061710987;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 2.6873212)) {
          if ( (data[6].fvalue < 0.20399159)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.16550441;
            }
          } else {
            if ( (data[17].fvalue < 0.25136346)) {
              sum += (float)-0.023194095;
            } else {
              sum += (float)0.13140908;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.062227979)) {
            if ( (data[14].fvalue < 0.12283217)) {
              sum += (float)-0.020224724;
            } else {
              sum += (float)-0.13403291;
            }
          } else {
            if ( (data[6].fvalue < 0.095782578)) {
              sum += (float)0.070220649;
            } else {
              sum += (float)-0.030070957;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.3460634)) {
        if ( (data[3].fvalue < 0.92561346)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[16].fvalue < 0.0028070915)) {
              sum += (float)0.04322983;
            } else {
              sum += (float)0.11957019;
            }
          } else {
            sum += (float)0.18568373;
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0760479)) {
              sum += (float)0.14386994;
            } else {
              sum += (float)0.040234849;
            }
          } else {
            if ( (data[14].fvalue < 0.28497806)) {
              sum += (float)-0.082189031;
            } else {
              sum += (float)0.012363602;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.33016577)) {
          if ( (data[14].fvalue < 0.072609603)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.081279457;
            } else {
              sum += (float)0.0010349401;
            }
          } else {
            if ( (data[14].fvalue < 0.58213323)) {
              sum += (float)0.012200532;
            } else {
              sum += (float)0.096671388;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.0067581092)) {
            if ( (data[5].fvalue < 0.35814631)) {
              sum += (float)0.094871536;
            } else {
              sum += (float)0.0042055808;
            }
          } else {
            if ( (data[17].fvalue < 0.28373682)) {
              sum += (float)-0.056681745;
            } else {
              sum += (float)0.053829018;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 0.5)) {
      if ( (data[13].fvalue < 0.034295842)) {
        sum += (float)-0.20229162;
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if (  (data[4].fvalue < 1.9151251)) {
            if ( (data[14].fvalue < 0.00088953075)) {
              sum += (float)-0.015599127;
            } else {
              sum += (float)0.036633022;
            }
          } else {
            if ( (data[16].fvalue < 0.97988605)) {
              sum += (float)-0.15242292;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.35314417)) {
            sum += (float)-0.053624369;
          } else {
            sum += (float)-0.14708859;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.0044494)) {
        if ( (data[13].fvalue < 0.055671971)) {
          sum += (float)0.15587357;
        } else {
          sum += (float)0.011423824;
        }
      } else {
        if ( (data[15].fvalue < 0.32837152)) {
          sum += (float)-0.065272383;
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.16502815)) {
    if ( (data[4].fvalue < 0.86054069)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.091386095)) {
          sum += (float)0.13247181;
        } else {
          if ( (data[3].fvalue < 0.59345555)) {
            sum += (float)-0.04043157;
          } else {
            if ( (data[14].fvalue < 0.24339856)) {
              sum += (float)0.15194392;
            } else {
              sum += (float)0.013917086;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.76077187)) {
          sum += (float)0.053928662;
        } else {
          if ( (data[14].fvalue < 0.3124541)) {
            sum += (float)-0.080660455;
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.34149671)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[16].fvalue < 0.12749568)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.014486833;
            } else {
              sum += (float)0.031010469;
            }
          } else {
            if ( (data[17].fvalue < 0.0085131824)) {
              sum += (float)0.024299167;
            } else {
              sum += (float)-0.071764767;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1698797)) {
            if ( (data[14].fvalue < 0.17040072)) {
              sum += (float)0.12667307;
            } else {
              sum += (float)0.24285112;
            }
          } else {
            if ( (data[16].fvalue < 0.0010196135)) {
              sum += (float)-0.016692709;
            } else {
              sum += (float)0.12517118;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 2.5)) {
            if (  (data[4].fvalue < 1.9336777)) {
              sum += (float)-0.00570472;
            } else {
              sum += (float)0.039080888;
            }
          } else {
            sum += (float)-0.15335611;
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.23120108)) {
              sum += (float)-0.12005283;
            } else {
              sum += (float)0.032072376;
            }
          } else {
            if ( (data[13].fvalue < 0.0592466)) {
              sum += (float)-0.14601456;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.2981272)) {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[10].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.52440959)) {
            if ( (data[14].fvalue < 0.004385238)) {
              sum += (float)-0.10415509;
            } else {
              sum += (float)-0.00069113902;
            }
          } else {
            sum += (float)0.10653091;
          }
        } else {
          if ( (data[3].fvalue < 0.76667398)) {
            if (  (data[4].fvalue < 1.2110181)) {
              sum += (float)0.15297996;
            } else {
              sum += (float)0.084697045;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.062616438;
            } else {
              sum += (float)-0.062417481;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.96089351)) {
          if ( (data[14].fvalue < 0.20629096)) {
            sum += (float)0.085553922;
          } else {
            sum += (float)0.2098477;
          }
        } else {
          if ( (data[14].fvalue < 0.41634136)) {
            sum += (float)-0.11624509;
          } else {
            sum += (float)0.068313621;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.88491046)) {
        if ( (data[14].fvalue < 0.43092978)) {
          if ( (data[16].fvalue < 0.25027108)) {
            if ( (data[3].fvalue < 0.29684564)) {
              sum += (float)0.00085841957;
            } else {
              sum += (float)-0.055194627;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.029948123;
            } else {
              sum += (float)-0.01683614;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.36253962)) {
              sum += (float)-0.061499909;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.1028887;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.3172829)) {
          sum += (float)0.12271292;
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.010285337)) {
    if ( (data[17].fvalue < 0.01206424)) {
      if ( (data[16].fvalue < 0.038580857)) {
        if ( (data[13].fvalue < 2.9630704e-05)) {
          if ( (data[6].fvalue < 0.20507646)) {
            if ( (data[14].fvalue < 0.0050170301)) {
              sum += (float)-0.030975081;
            } else {
              sum += (float)0.18385752;
            }
          } else {
            sum += (float)0.39012203;
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.26913726;
            } else {
              sum += (float)0.10236745;
            }
          } else {
            if ( (data[14].fvalue < 3.07398e-05)) {
              sum += (float)0.32962155;
            } else {
              sum += (float)-0.076896183;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[5].fvalue < 0.17188159)) {
              sum += (float)0.002072844;
            } else {
              sum += (float)-0.034046113;
            }
          } else {
            if ( (data[13].fvalue < 0.46521688)) {
              sum += (float)0.0086331256;
            } else {
              sum += (float)0.10868111;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.031652585)) {
            if ( (data[13].fvalue < 0.00017650455)) {
              sum += (float)-0.26879489;
            } else {
              sum += (float)-0.13313824;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.033788811;
            } else {
              sum += (float)-0.13372368;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.017211445)) {
        sum += (float)-0.19875968;
      } else {
        if ( (data[17].fvalue < 0.34191704)) {
          if ( (data[13].fvalue < 0.64185655)) {
            if ( (data[0].fvalue < 2.6255441)) {
              sum += (float)-0.14065188;
            } else {
              sum += (float)-0.019979058;
            }
          } else {
            sum += (float)-0.16960804;
          }
        } else {
          if ( (data[16].fvalue < 0.55018771)) {
            if (  (data[4].fvalue < 1.8201172)) {
              sum += (float)0.0065785763;
            } else {
              sum += (float)-0.058292508;
            }
          } else {
            sum += (float)0.047861766;
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.13980815)) {
      if ( (data[13].fvalue < 0.57447505)) {
        if ( (data[4].fvalue < 1.1835542)) {
          if ( (data[3].fvalue < 0.057490572)) {
            if (  (data[4].fvalue < 0.9717797)) {
              sum += (float)-0.26574969;
            } else {
              sum += (float)-0.014708434;
            }
          } else {
            if ( (data[5].fvalue < 0.36439008)) {
              sum += (float)0.043414626;
            } else {
              sum += (float)0.0040492667;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.17626703)) {
            if ( (data[14].fvalue < 0.24380618)) {
              sum += (float)-0.050352316;
            } else {
              sum += (float)-0.0084160436;
            }
          } else {
            if ( (data[13].fvalue < 0.0032239477)) {
              sum += (float)-0.0070243995;
            } else {
              sum += (float)0.064929329;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[1].fvalue < 1.9310405)) {
              sum += (float)0.0011240138;
            } else {
              sum += (float)0.12132971;
            }
          } else {
            if (  (data[4].fvalue < 1.4892663)) {
              sum += (float)0.032695696;
            } else {
              sum += (float)-0.039697893;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            sum += (float)0.1606002;
          } else {
            if ( (data[3].fvalue < 0.78925073)) {
              sum += (float)0.060604949;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.00412302)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[11].fvalue < 0.5)) {
            if ( (data[5].fvalue < 0.41293165)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.059947334;
            }
          } else {
            if ( (data[16].fvalue < 0.44059911)) {
              sum += (float)-0.1185775;
            } else {
              sum += (float)-0.0065798885;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.063649513)) {
            sum += (float)-0.0056364215;
          } else {
            if ( (data[2].fvalue < 0.21047258)) {
              sum += (float)0.09996099;
            } else {
              sum += (float)0.04894042;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7995541)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.060117658)) {
              sum += (float)0.12637807;
            } else {
              sum += (float)0.057905246;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.0016623337;
            } else {
              sum += (float)0.084627382;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.10480154)) {
            if ( (data[13].fvalue < 0.078077167)) {
              sum += (float)0.01400883;
            } else {
              sum += (float)-0.062300775;
            }
          } else {
            if ( (data[17].fvalue < 0.12915386)) {
              sum += (float)-0.0070610722;
            } else {
              sum += (float)0.076752506;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.1912026)) {
    if ( (data[3].fvalue < 2.1366205)) {
      if ( (data[6].fvalue < 0.013314223)) {
        sum += (float)-0.128994;
      } else {
        if ( (data[7].fvalue < 7.5)) {
          if ( (data[14].fvalue < 0.37610817)) {
            if ( (data[3].fvalue < 0.58601213)) {
              sum += (float)-0.00082303089;
            } else {
              sum += (float)-0.02162209;
            }
          } else {
            if (  (data[4].fvalue < 1.8357409)) {
              sum += (float)0.011795166;
            } else {
              sum += (float)-0.023082282;
            }
          }
        } else {
          sum += (float)0.078524157;
        }
      }
    } else {
      if ( (data[5].fvalue < 0.42739475)) {
        sum += (float)0.021757599;
      } else {
        if ( (data[6].fvalue < 0.71557415)) {
          if ( (data[13].fvalue < 0.73585689)) {
            if ( (data[14].fvalue < 0.70794475)) {
              sum += (float)-0.093555182;
            } else {
              sum += (float)-0.02721725;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)-0.12901308;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[14].fvalue < 0.052967161)) {
        if ( (data[13].fvalue < 0.00025702035)) {
          if ( (data[3].fvalue < 0.17330584)) {
            sum += (float)-0.048351716;
          } else {
            if ( (data[5].fvalue < 1.5634723)) {
              sum += (float)0.29499659;
            } else {
              sum += (float)0.097043283;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.3238039)) {
            sum += (float)-0.065935381;
          } else {
            sum += (float)0.015332664;
          }
        }
      } else {
        if ( (data[3].fvalue < 0.27377218)) {
          sum += (float)-0.26157933;
        } else {
          if ( (data[14].fvalue < 0.096171528)) {
            if ( (data[5].fvalue < 1.5025493)) {
              sum += (float)0.19160698;
            } else {
              sum += (float)0.0076647047;
            }
          } else {
            if ( (data[3].fvalue < 0.41426528)) {
              sum += (float)-0.22222224;
            } else {
              sum += (float)0.033468667;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.54142368)) {
        sum += (float)0.21911325;
      } else {
        sum += (float)0.02448024;
      }
    }
  }
  if ( (data[1].fvalue < 0.24946746)) {
    if ( (data[3].fvalue < 0.56297565)) {
      if ( (data[3].fvalue < 0.13288918)) {
        sum += (float)0.084468529;
      } else {
        if ( (data[1].fvalue < 0.17496812)) {
          sum += (float)-0.01633459;
        } else {
          sum += (float)0.019645425;
        }
      }
    } else {
      sum += (float)0.12226593;
    }
  } else {
    if ( (data[6].fvalue < 0.42526707)) {
      if ( (data[13].fvalue < 0.30514324)) {
        if ( (data[17].fvalue < 0.13968687)) {
          if ( (data[17].fvalue < 0.048464779)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.0019692343;
            } else {
              sum += (float)0.074489146;
            }
          } else {
            if ( (data[3].fvalue < 0.5665614)) {
              sum += (float)-0.023943005;
            } else {
              sum += (float)-0.094071813;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.0027393412)) {
              sum += (float)-0.055660862;
            } else {
              sum += (float)0.084654719;
            }
          } else {
            if (  (data[4].fvalue < 1.7055451)) {
              sum += (float)0.095665611;
            } else {
              sum += (float)0.0084298141;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.53351)) {
          if ( (data[13].fvalue < 0.63237041)) {
            if ( (data[3].fvalue < 0.18772987)) {
              sum += (float)0.10622088;
            } else {
              sum += (float)0.029699681;
            }
          } else {
            if ( (data[16].fvalue < 0.02939738)) {
              sum += (float)0.15326512;
            } else {
              sum += (float)0.040090002;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.093416937)) {
            if ( (data[16].fvalue < 0.2313851)) {
              sum += (float)-0.047965281;
            } else {
              sum += (float)0.014157702;
            }
          } else {
            if ( (data[14].fvalue < 0.034053843)) {
              sum += (float)-0;
            } else {
              sum += (float)0.07669051;
            }
          }
        }
      }
    } else {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[5].fvalue < 0.37897846)) {
          if ( (data[16].fvalue < 0.03301087)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.16659611;
            } else {
              sum += (float)0.010072807;
            }
          } else {
            if ( (data[16].fvalue < 0.3585203)) {
              sum += (float)-0.032784469;
            } else {
              sum += (float)0.0092322184;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.70112097)) {
            if ( (data[13].fvalue < 0.55747044)) {
              sum += (float)-0.031135755;
            } else {
              sum += (float)0.011995836;
            }
          } else {
            if ( (data[5].fvalue < 0.46386772)) {
              sum += (float)-0.017505527;
            } else {
              sum += (float)-0.12619928;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7750218)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.17089731;
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.97333491)) {
    if (  (data[4].fvalue < 0.96448362)) {
      if ( (data[3].fvalue < 0.056462754)) {
        sum += (float)-0.22080229;
      } else {
        if ( (data[14].fvalue < 0.037722677)) {
          if ( (data[13].fvalue < 6.009293e-06)) {
            if ( (data[3].fvalue < 0.090080865)) {
              sum += (float)0.057268146;
            } else {
              sum += (float)0.25821471;
            }
          } else {
            sum += (float)-0.050456077;
          }
        } else {
          if ( (data[3].fvalue < 0.12633348)) {
            sum += (float)-0.26704806;
          } else {
            if ( (data[14].fvalue < 0.066669762)) {
              sum += (float)0.13107838;
            } else {
              sum += (float)-0.064360157;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.10268666)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.46124467)) {
            if ( (data[13].fvalue < 0.10749485)) {
              sum += (float)-0.020950092;
            } else {
              sum += (float)0.01170036;
            }
          } else {
            if ( (data[13].fvalue < 2.4540277e-05)) {
              sum += (float)0.1212319;
            } else {
              sum += (float)0.0054320814;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.1359897)) {
            if ( (data[13].fvalue < 0.0050363061)) {
              sum += (float)-0.032972559;
            } else {
              sum += (float)0.053576514;
            }
          } else {
            if ( (data[14].fvalue < 0.039636806)) {
              sum += (float)-0.15790011;
            } else {
              sum += (float)-0.047599167;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 3.0621171)) {
          if ( (data[6].fvalue < 0.05811847)) {
            sum += (float)-0.15430078;
          } else {
            if ( (data[16].fvalue < 0.00030358965)) {
              sum += (float)0.0092573352;
            } else {
              sum += (float)0.032260235;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[6].fvalue < 0.038575456)) {
              sum += (float)-0.13606058;
            } else {
              sum += (float)0.0027409662;
            }
          } else {
            if ( (data[13].fvalue < 0.091693982)) {
              sum += (float)-0.055205878;
            } else {
              sum += (float)-0.003078101;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[13].fvalue < 1.679614e-06)) {
        if ( (data[14].fvalue < 0.073668085)) {
          if ( (data[3].fvalue < 0.18377376)) {
            if ( (data[14].fvalue < 0.033120841)) {
              sum += (float)0.12883516;
            } else {
              sum += (float)-0.17252888;
            }
          } else {
            if ( (data[5].fvalue < 1.4599135)) {
              sum += (float)0.27493897;
            } else {
              sum += (float)0.052400298;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.26891419)) {
            sum += (float)-0.22632144;
          } else {
            if ( (data[14].fvalue < 0.11212131)) {
              sum += (float)0.14224255;
            } else {
              sum += (float)0.0022891716;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.0023953007;
            } else {
              sum += (float)-0.14856401;
            }
          } else {
            if ( (data[5].fvalue < 1.2842715)) {
              sum += (float)-0.13152127;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)0.036185678;
        }
      }
    } else {
      if ( (data[3].fvalue < 1.0867692)) {
        if ( (data[14].fvalue < 0.15671435)) {
          sum += (float)0.073498487;
        } else {
          sum += (float)0.2547873;
        }
      } else {
        sum += (float)-0.019481774;
      }
    }
  }
  if ( (data[11].fvalue < 1.5)) {
    if ( (data[14].fvalue < 0.4867245)) {
      if ( (data[5].fvalue < 0.065929204)) {
        if (  (data[4].fvalue < 1.7038792)) {
          if (  (data[4].fvalue < 1.2129896)) {
            if ( (data[16].fvalue < 0.065011367)) {
              sum += (float)-0.04026033;
            } else {
              sum += (float)0.042845462;
            }
          } else {
            if ( (data[17].fvalue < 0.12973711)) {
              sum += (float)0.033549514;
            } else {
              sum += (float)0.096275941;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.068979457)) {
            sum += (float)0.049642824;
          } else {
            if ( (data[16].fvalue < 0.78722113)) {
              sum += (float)-0.060164012;
            } else {
              sum += (float)-0.001436267;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.1641692)) {
          if ( (data[8].fvalue < 3.5)) {
            if (  (data[4].fvalue < 1.0624343)) {
              sum += (float)-0.0036102545;
            } else {
              sum += (float)0.0241368;
            }
          } else {
            if ( (data[3].fvalue < 0.43909562)) {
              sum += (float)0.12134401;
            } else {
              sum += (float)-0.023792548;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.48972663)) {
            if ( (data[16].fvalue < 0.00047089026)) {
              sum += (float)-0.02387543;
            } else {
              sum += (float)0.00096254057;
            }
          } else {
            if ( (data[13].fvalue < 0.50602221)) {
              sum += (float)-0.082020007;
            } else {
              sum += (float)0.010944824;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.87439746)) {
        if ( (data[17].fvalue < 0.021285664)) {
          if ( (data[15].fvalue < 0.037682191)) {
            if ( (data[16].fvalue < 0.34310618)) {
              sum += (float)0.015434419;
            } else {
              sum += (float)0.08813215;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.12147491;
            } else {
              sum += (float)0.025674833;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.022654511)) {
            if ( (data[17].fvalue < 0.24513158)) {
              sum += (float)-0.021785146;
            } else {
              sum += (float)0.050809428;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.12874027;
            } else {
              sum += (float)-0.01812922;
            }
          }
        }
      } else {
        sum += (float)0.14076495;
      }
    }
  } else {
    if (  (data[4].fvalue < 1.4320502)) {
      sum += (float)-0.12090307;
    } else {
      if ( (data[6].fvalue < 0.32958642)) {
        sum += (float)0.061368041;
      } else {
        if ( (data[14].fvalue < 0.44986406)) {
          if ( (data[14].fvalue < 0.020285832)) {
            sum += (float)-0.086398773;
          } else {
            if ( (data[17].fvalue < 0.50036895)) {
              sum += (float)-0.018616598;
            } else {
              sum += (float)0.017947033;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.80699766)) {
            if ( (data[13].fvalue < 0.0045514228)) {
              sum += (float)-0.12530378;
            } else {
              sum += (float)-0.056375302;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.20602702)) {
    if ( (data[13].fvalue < 7.1504623e-08)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[16].fvalue < 0.030335486)) {
            if ( (data[14].fvalue < 0.022360142)) {
              sum += (float)0.10367686;
            } else {
              sum += (float)-0.0021511009;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.032324012;
            } else {
              sum += (float)0.034136847;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.7147336)) {
            if ( (data[4].fvalue < 1.2777598)) {
              sum += (float)0.080845967;
            } else {
              sum += (float)0.0014076341;
            }
          } else {
            sum += (float)0.13168804;
          }
        }
      } else {
        sum += (float)0.34050292;
      }
    } else {
      if ( (data[16].fvalue < 0.00044789264)) {
        if ( (data[8].fvalue < 0.5)) {
          sum += (float)-0.23035762;
        } else {
          if ( (data[14].fvalue < 0.00042230904)) {
            sum += (float)0.24321401;
          } else {
            if ( (data[17].fvalue < 0.00027592495)) {
              sum += (float)-0.039322667;
            } else {
              sum += (float)0.024637759;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0067486009)) {
          if ( (data[13].fvalue < 0.083209231)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.12762536;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 1.514773)) {
              sum += (float)0.026119456;
            } else {
              sum += (float)-0.058651134;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.12325548)) {
              sum += (float)0.031461205;
            } else {
              sum += (float)0.12578899;
            }
          } else {
            if (  (data[4].fvalue < 1.2611538)) {
              sum += (float)0.050380722;
            } else {
              sum += (float)-0.024745489;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.4471393)) {
      if ( (data[3].fvalue < 0.96766567)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.047862403;
            } else {
              sum += (float)0.033356264;
            }
          } else {
            if ( (data[16].fvalue < 0.24407238)) {
              sum += (float)0.053942215;
            } else {
              sum += (float)0.12212319;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.077087536)) {
            sum += (float)-0;
          } else {
            if ( (data[3].fvalue < 0.36229128)) {
              sum += (float)0.19303428;
            } else {
              sum += (float)0.058772624;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.38763571)) {
            sum += (float)-0.021975739;
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.010649754;
            } else {
              sum += (float)0.10571736;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.38132566)) {
            if ( (data[16].fvalue < 0.10668024)) {
              sum += (float)-0.11995837;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.0040558963;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.60016763)) {
        if ( (data[14].fvalue < 0.14902687)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.043815561;
            } else {
              sum += (float)0.021974064;
            }
          } else {
            sum += (float)-0.1575297;
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[15].fvalue < 0.00063365191)) {
              sum += (float)0.013918065;
            } else {
              sum += (float)0.10455287;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.068927571;
            } else {
              sum += (float)-0.072012603;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.39129019)) {
            if ( (data[5].fvalue < 0.33888412)) {
              sum += (float)0.031174514;
            } else {
              sum += (float)-0.034392789;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.094312616;
            } else {
              sum += (float)0.0054163327;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.18717328)) {
            sum += (float)-0.068426013;
          } else {
            sum += (float)0.018568022;
          }
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.12890738)) {
    if ( (data[10].fvalue < 1.5)) {
      if ( (data[16].fvalue < 0.043402277)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[3].fvalue < 0.10330267)) {
            if (  (data[4].fvalue < 0.96840715)) {
              sum += (float)-0.1440661;
            } else {
              sum += (float)-0.0060019731;
            }
          } else {
            if ( (data[14].fvalue < 0.07683482)) {
              sum += (float)0.055526339;
            } else {
              sum += (float)0.0049562054;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.026526108)) {
            if ( (data[14].fvalue < 0.033252724)) {
              sum += (float)0.035363574;
            } else {
              sum += (float)0.21352024;
            }
          } else {
            if ( (data[14].fvalue < 0.047789097)) {
              sum += (float)-0.07923951;
            } else {
              sum += (float)-0.0055360077;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.034117281)) {
          if ( (data[14].fvalue < 0.090928867)) {
            if (  (data[4].fvalue < 1.1879168)) {
              sum += (float)0.080699258;
            } else {
              sum += (float)-0.048975669;
            }
          } else {
            sum += (float)0.11197017;
          }
        } else {
          if ( (data[14].fvalue < 0.13626519)) {
            if (  (data[4].fvalue < 1.7858142)) {
              sum += (float)-0.061213288;
            } else {
              sum += (float)0.06606032;
            }
          } else {
            if ( (data[3].fvalue < 0.55203593)) {
              sum += (float)0.013139424;
            } else {
              sum += (float)-0.051100746;
            }
          }
        }
      }
    } else {
      sum += (float)0.099676378;
    }
  } else {
    if ( (data[3].fvalue < 0.10510646)) {
      if ( (data[13].fvalue < 0.012046631)) {
        if (  (data[4].fvalue < 1.2672775)) {
          if ( (data[16].fvalue < 0.18353371)) {
            if ( (data[14].fvalue < 0.023892861)) {
              sum += (float)-0.04580671;
            } else {
              sum += (float)0.043361545;
            }
          } else {
            sum += (float)0.14102712;
          }
        } else {
          if ( (data[14].fvalue < 0.099570505)) {
            if ( (data[16].fvalue < 0.51905555)) {
              sum += (float)-0.060564946;
            } else {
              sum += (float)0.03353145;
            }
          } else {
            if (  (data[4].fvalue < 1.4062876)) {
              sum += (float)0.086497836;
            } else {
              sum += (float)0.012194086;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8028271)) {
          if ( (data[16].fvalue < 0.24636)) {
            if (  (data[4].fvalue < 1.3755356)) {
              sum += (float)0.11011746;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.15991068;
          }
        } else {
          sum += (float)-0.018202756;
        }
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[17].fvalue < 0.58489811)) {
          if ( (data[14].fvalue < 0.090016395)) {
            if ( (data[13].fvalue < 0.20564754)) {
              sum += (float)-0.032127373;
            } else {
              sum += (float)0.0036497565;
            }
          } else {
            if ( (data[6].fvalue < 0.2042833)) {
              sum += (float)0.045684855;
            } else {
              sum += (float)-0.00094963406;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.24683687)) {
            sum += (float)-0.037106868;
          } else {
            if ( (data[6].fvalue < 0.67215121)) {
              sum += (float)0.093587816;
            } else {
              sum += (float)0.034014262;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4451258)) {
          if ( (data[3].fvalue < 0.60595089)) {
            sum += (float)0.2021222;
          } else {
            sum += (float)0.06075554;
          }
        } else {
          if ( (data[16].fvalue < 0.54699397)) {
            if ( (data[15].fvalue < 0.14443082)) {
              sum += (float)-0.062689938;
            } else {
              sum += (float)0.05091108;
            }
          } else {
            if ( (data[14].fvalue < 0.31160486)) {
              sum += (float)0.15102364;
            } else {
              sum += (float)-0.0052385088;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.041981068)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[5].fvalue < 1.5111109)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[13].fvalue < 0.007492858)) {
            if ( (data[13].fvalue < 1.9018934e-06)) {
              sum += (float)0.00012944736;
            } else {
              sum += (float)-0.049462374;
            }
          } else {
            if ( (data[14].fvalue < 0.055167556)) {
              sum += (float)-0.048253182;
            } else {
              sum += (float)0.056617636;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.3034659e-05)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.10048179;
            }
          } else {
            if ( (data[14].fvalue < 0.18308485)) {
              sum += (float)0.010197669;
            } else {
              sum += (float)-0.1265863;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.35874376)) {
          if ( (data[14].fvalue < 0.036830895)) {
            sum += (float)-0.011942942;
          } else {
            sum += (float)-0.18993291;
          }
        } else {
          if ( (data[14].fvalue < 0.10939272)) {
            sum += (float)0.095152281;
          } else {
            sum += (float)-0.084811293;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.23797947)) {
        if (  (data[4].fvalue < 0.99203217)) {
          sum += (float)0.16482706;
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.026804598)) {
              sum += (float)-0.13787189;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 1.250881)) {
              sum += (float)0.095770121;
            } else {
              sum += (float)-0.068619557;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.1482825)) {
          if ( (data[14].fvalue < 0.18194234)) {
            sum += (float)-0.17936964;
          } else {
            if ( (data[6].fvalue < 0.20495063)) {
              sum += (float)0.0063058478;
            } else {
              sum += (float)-0.1697375;
            }
          }
        } else {
          sum += (float)-0.0063876789;
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.39354944)) {
      if (  (data[4].fvalue < 1.3397512)) {
        if ( (data[14].fvalue < 0.29976499)) {
          if ( (data[16].fvalue < 0.0021850797)) {
            if (  (data[4].fvalue < 1.1301453)) {
              sum += (float)0.046092264;
            } else {
              sum += (float)-0.034855161;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.054596346;
            } else {
              sum += (float)0.26728532;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[13].fvalue < 0.07326562)) {
              sum += (float)-0;
            } else {
              sum += (float)0.105716;
            }
          } else {
            sum += (float)0.2940138;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.7630492)) {
            if ( (data[16].fvalue < 0.2535283)) {
              sum += (float)0.016889513;
            } else {
              sum += (float)0.058944769;
            }
          } else {
            if ( (data[16].fvalue < 0.15467475)) {
              sum += (float)0.031284522;
            } else {
              sum += (float)-0.037346892;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.40171087)) {
            if ( (data[13].fvalue < 0.43556038)) {
              sum += (float)-0.068692088;
            } else {
              sum += (float)0.016497495;
            }
          } else {
            if (  (data[4].fvalue < 1.4410968)) {
              sum += (float)0.13356325;
            } else {
              sum += (float)-0.001778719;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.16561393)) {
        if ( (data[15].fvalue < 0.0077390643)) {
          if ( (data[17].fvalue < 0.72666204)) {
            if ( (data[13].fvalue < 0.4807438)) {
              sum += (float)-0.015639788;
            } else {
              sum += (float)0.014208148;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.090459108;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.037397765)) {
            sum += (float)-0.18451613;
          } else {
            if ( (data[16].fvalue < 0.2371304)) {
              sum += (float)0.026665939;
            } else {
              sum += (float)-0.072615854;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)-0.055205643;
          } else {
            sum += (float)0.06188285;
          }
        } else {
          if (  (data[4].fvalue < 2.1422129)) {
            sum += (float)0.11528585;
          } else {
            sum += (float)0.024570068;
          }
        }
      }
    }
  }
  if ( (data[3].fvalue < 0.28330207)) {
    if ( (data[13].fvalue < 0.0062046871)) {
      if ( (data[14].fvalue < 0.064208329)) {
        if (  (data[4].fvalue < 1.0287714)) {
          if ( (data[3].fvalue < 0.036666222)) {
            if (  (data[4].fvalue < 0.9752686)) {
              sum += (float)-0.21541686;
            } else {
              sum += (float)0.013581585;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.11844816;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.090476178)) {
            if (  (data[4].fvalue < 1.3972751)) {
              sum += (float)-0.13759115;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 1.5874028)) {
              sum += (float)-0.01675478;
            } else {
              sum += (float)0.036502615;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.0143383)) {
          if ( (data[3].fvalue < 0.15089116)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.21927215;
            } else {
              sum += (float)-0.0019648725;
            }
          } else {
            if ( (data[14].fvalue < 0.12190869)) {
              sum += (float)0.039511897;
            } else {
              sum += (float)-0.14714848;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.6752159)) {
            if (  (data[4].fvalue < 1.7392941)) {
              sum += (float)0.012997559;
            } else {
              sum += (float)-0.038373869;
            }
          } else {
            if ( (data[16].fvalue < 0.10654432)) {
              sum += (float)-0.090380609;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.024207849)) {
        if ( (data[16].fvalue < 0.22890046)) {
          if ( (data[13].fvalue < 0.11788338)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.10907106;
            } else {
              sum += (float)-0.03354957;
            }
          } else {
            if ( (data[14].fvalue < 0.0063882088)) {
              sum += (float)-0.035840243;
            } else {
              sum += (float)0.056688122;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6294678)) {
            if ( (data[3].fvalue < 0.16193087)) {
              sum += (float)0.10476272;
            } else {
              sum += (float)0.017452445;
            }
          } else {
            if ( (data[16].fvalue < 0.62984085)) {
              sum += (float)-0.079986975;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1239884)) {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0401832)) {
              sum += (float)-0.093350567;
            } else {
              sum += (float)0.071884051;
            }
          } else {
            if ( (data[3].fvalue < 0.12238677)) {
              sum += (float)0.21885362;
            } else {
              sum += (float)0.099390917;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.061843112)) {
              sum += (float)0.13541056;
            } else {
              sum += (float)0.023649134;
            }
          } else {
            if ( (data[13].fvalue < 0.085774295)) {
              sum += (float)-0.024064982;
            } else {
              sum += (float)0.021764681;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[16].fvalue < 0.035356071)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.18379262)) {
            if ( (data[17].fvalue < 0.32014629)) {
              sum += (float)0.059119929;
            } else {
              sum += (float)-0.092677176;
            }
          } else {
            if ( (data[6].fvalue < 0.094796613)) {
              sum += (float)-0.077217467;
            } else {
              sum += (float)0.01431786;
            }
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[14].fvalue < 0.063438535)) {
              sum += (float)-0.10973611;
            } else {
              sum += (float)-0.021522265;
            }
          } else {
            sum += (float)0.066340819;
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.003028264)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.033204362;
            } else {
              sum += (float)0.026585571;
            }
          } else {
            if ( (data[14].fvalue < 0.17119779)) {
              sum += (float)-0.0066024214;
            } else {
              sum += (float)0.039043665;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.14887699;
            } else {
              sum += (float)-0.00041392955;
            }
          } else {
            sum += (float)0.010267437;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.099362686)) {
        if ( (data[16].fvalue < 0.14016293)) {
          if ( (data[1].fvalue < 2.1474848)) {
            sum += (float)-0.090324171;
          } else {
            sum += (float)-0.22308432;
          }
        } else {
          sum += (float)0.036466688;
        }
      } else {
        if ( (data[13].fvalue < 0.083031595)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.64732885)) {
              sum += (float)-0.014592965;
            } else {
              sum += (float)0.052107479;
            }
          } else {
            if ( (data[13].fvalue < 0.0071132099)) {
              sum += (float)-0.20759678;
            } else {
              sum += (float)-0.060463313;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.235368)) {
            if ( (data[3].fvalue < 0.90255809)) {
              sum += (float)0.10760319;
            } else {
              sum += (float)-0.015346278;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.010073547;
            } else {
              sum += (float)-0.036946516;
            }
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.21470183)) {
    if ( (data[7].fvalue < 6.5)) {
      if (  (data[4].fvalue < 1.1162243)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.281443;
            } else {
              sum += (float)0.071170777;
            }
          } else {
            if ( (data[14].fvalue < 2.5550557e-06)) {
              sum += (float)0.3086831;
            } else {
              sum += (float)5.0382096e-05;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.50227934)) {
            if (  (data[4].fvalue < 1.0160632)) {
              sum += (float)0.22607726;
            } else {
              sum += (float)0.026684763;
            }
          } else {
            sum += (float)-0.04793686;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.097075433)) {
          if (  (data[4].fvalue < 1.5087216)) {
            if ( (data[13].fvalue < 0.41670549)) {
              sum += (float)-0.049406756;
            } else {
              sum += (float)0.032359581;
            }
          } else {
            if ( (data[17].fvalue < 0.40801781)) {
              sum += (float)0.076342262;
            } else {
              sum += (float)-0.018077772;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.2281964)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.021843046;
            } else {
              sum += (float)0.098421723;
            }
          } else {
            if ( (data[16].fvalue < 0.21018559)) {
              sum += (float)-0.027729331;
            } else {
              sum += (float)0.00021839933;
            }
          }
        }
      }
    } else {
      sum += (float)0.091286525;
    }
  } else {
    if ( (data[5].fvalue < 0.40894926)) {
      if (  (data[4].fvalue < 1.4257172)) {
        if ( (data[16].fvalue < 0.00067305932)) {
          if ( (data[11].fvalue < 0.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.066424541;
            } else {
              sum += (float)0.031536058;
            }
          } else {
            if (  (data[4].fvalue < 1.3387263)) {
              sum += (float)0.15120031;
            } else {
              sum += (float)0.060271364;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.3005349)) {
            if ( (data[3].fvalue < 0.14432169)) {
              sum += (float)0.12657;
            } else {
              sum += (float)0.020662703;
            }
          } else {
            if ( (data[5].fvalue < 0.32292345)) {
              sum += (float)0.16500752;
            } else {
              sum += (float)0.047239531;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.28158438)) {
          if ( (data[15].fvalue < 0.47089848)) {
            if ( (data[13].fvalue < 0.41199574)) {
              sum += (float)-0.017038787;
            } else {
              sum += (float)0.039136119;
            }
          } else {
            if ( (data[1].fvalue < 1.395134)) {
              sum += (float)0.010229912;
            } else {
              sum += (float)0.11971312;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7201872)) {
            if ( (data[5].fvalue < 0.29615223)) {
              sum += (float)0.11366136;
            } else {
              sum += (float)0.015252093;
            }
          } else {
            if ( (data[17].fvalue < 1.1067803)) {
              sum += (float)-0.015221833;
            } else {
              sum += (float)0.075676307;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.2767573)) {
        sum += (float)-0.10548358;
      } else {
        if ( (data[4].fvalue < 1.2358189)) {
          if ( (data[8].fvalue < 4.5)) {
            if (  (data[4].fvalue < 1.0212026)) {
              sum += (float)-0.019323129;
            } else {
              sum += (float)0.020014105;
            }
          } else {
            if ( (data[3].fvalue < 1.3115435)) {
              sum += (float)0.10535302;
            } else {
              sum += (float)-0.023475265;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.70474696)) {
            if ( (data[16].fvalue < 0.48069006)) {
              sum += (float)-0.054542303;
            } else {
              sum += (float)0.032737985;
            }
          } else {
            if ( (data[17].fvalue < 0.035957456)) {
              sum += (float)0.043628469;
            } else {
              sum += (float)-0.036756229;
            }
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.67912)) {
    sum += (float)0.10027708;
  } else {
    if ( (data[3].fvalue < 1.5988467)) {
      if ( (data[14].fvalue < 0.52439433)) {
        if (  (data[4].fvalue < 1.2231448)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[16].fvalue < 0.096943378)) {
              sum += (float)-0.0023607893;
            } else {
              sum += (float)0.032367207;
            }
          } else {
            if ( (data[3].fvalue < 0.53170109)) {
              sum += (float)0.10350663;
            } else {
              sum += (float)-0.013103207;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.19948941)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.0024996817;
            } else {
              sum += (float)-0.038314074;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.00047751158;
            } else {
              sum += (float)0.04738304;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4972861)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.042000555;
            } else {
              sum += (float)-0.0038537763;
            }
          } else {
            if ( (data[3].fvalue < 0.28609782)) {
              sum += (float)0.17371146;
            } else {
              sum += (float)0.047729112;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.54058576)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.0048281951;
            } else {
              sum += (float)-0.079676718;
            }
          } else {
            sum += (float)0.11119596;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.8114586)) {
        if ( (data[1].fvalue < 1.2586815)) {
          if ( (data[14].fvalue < 0.58731478)) {
            sum += (float)-0.11279758;
          } else {
            sum += (float)-0.035066295;
          }
        } else {
          if ( (data[14].fvalue < 0.74316096)) {
            if ( (data[14].fvalue < 0.12503482)) {
              sum += (float)0.00014561503;
            } else {
              sum += (float)-0.068181753;
            }
          } else {
            if ( (data[6].fvalue < 0.52224576)) {
              sum += (float)0.015292047;
            } else {
              sum += (float)-0.03691528;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 1.058154)) {
          if ( (data[13].fvalue < 0.063861579)) {
            sum += (float)0.02830261;
          } else {
            if ( (data[13].fvalue < 0.83635437)) {
              sum += (float)-0.052952688;
            } else {
              sum += (float)0.0036360717;
            }
          }
        } else {
          sum += (float)0.069548666;
        }
      }
    }
  }
  if ( (data[11].fvalue < 1.5)) {
    if ( (data[14].fvalue < 0.43721214)) {
      if ( (data[17].fvalue < 0.14768642)) {
        if ( (data[4].fvalue < 1.2730505)) {
          if ( (data[13].fvalue < 0.19976753)) {
            if ( (data[16].fvalue < 0.16181612)) {
              sum += (float)-0.0027519334;
            } else {
              sum += (float)0.031288311;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.013513709;
            } else {
              sum += (float)0.068605043;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.28139162)) {
              sum += (float)0.0018228634;
            } else {
              sum += (float)-0.023231387;
            }
          } else {
            if ( (data[16].fvalue < 0.37911934)) {
              sum += (float)-0.064473614;
            } else {
              sum += (float)0.065619193;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[13].fvalue < 0.0025803163)) {
            if ( (data[14].fvalue < 0.062355369)) {
              sum += (float)-0.12200011;
            } else {
              sum += (float)0.0083160065;
            }
          } else {
            if (  (data[4].fvalue < 1.6830454)) {
              sum += (float)0.058998145;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.22910354)) {
            sum += (float)0.20367528;
          } else {
            sum += (float)0.043034066;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.96296644)) {
        if ( (data[17].fvalue < 0.67987597)) {
          if ( (data[4].fvalue < 1.8688431)) {
            if ( (data[3].fvalue < 1.5094924)) {
              sum += (float)0.019775519;
            } else {
              sum += (float)-0.022262316;
            }
          } else {
            if ( (data[15].fvalue < 0.62611687)) {
              sum += (float)-0.061836865;
            } else {
              sum += (float)0.074371308;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.76764655)) {
            sum += (float)0.028667951;
          } else {
            sum += (float)0.14673485;
          }
        }
      } else {
        sum += (float)0.10179096;
      }
    }
  } else {
    if ( (data[8].fvalue < 1.5)) {
      if (  (data[4].fvalue < 1.721725)) {
        if (  (data[4].fvalue < 1.5419136)) {
          sum += (float)-0.066981316;
        } else {
          sum += (float)0.032419391;
        }
      } else {
        if ( (data[3].fvalue < 0.89615297)) {
          sum += (float)-0.10939402;
        } else {
          sum += (float)-0.018840816;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.34708846)) {
        if ( (data[6].fvalue < 0.37730706)) {
          sum += (float)-0.0048138266;
        } else {
          sum += (float)-0.070283927;
        }
      } else {
        if (  (data[4].fvalue < 2.263397)) {
          if ( (data[17].fvalue < 0.78877872)) {
            if ( (data[3].fvalue < 0.54149532)) {
              sum += (float)0.043719187;
            } else {
              sum += (float)-0.0016756132;
            }
          } else {
            sum += (float)0.088137113;
          }
        } else {
          sum += (float)-0.020937242;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.11119878)) {
    if (  (data[4].fvalue < 1.8577597)) {
      if ( (data[5].fvalue < 0.052760586)) {
        if ( (data[6].fvalue < 0.61685514)) {
          if ( (data[17].fvalue < 0.10152535)) {
            if ( (data[16].fvalue < 0.0021736322)) {
              sum += (float)-0.024023993;
            } else {
              sum += (float)0.023594182;
            }
          } else {
            if ( (data[3].fvalue < 0.028907085)) {
              sum += (float)0.12640633;
            } else {
              sum += (float)0.0017826061;
            }
          }
        } else {
          sum += (float)0.093238525;
        }
      } else {
        if (  (data[4].fvalue < 1.1560581)) {
          if ( (data[8].fvalue < 0.5)) {
            sum += (float)-0.096428044;
          } else {
            if ( (data[14].fvalue < 0.00064291881)) {
              sum += (float)0.17679752;
            } else {
              sum += (float)-0.0012994248;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.047553554)) {
            if (  (data[4].fvalue < 1.5446782)) {
              sum += (float)-0.065441497;
            } else {
              sum += (float)0.029976612;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.0052850237;
            } else {
              sum += (float)-0.067318209;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.050354466)) {
        if ( (data[16].fvalue < 1.7478865)) {
          if ( (data[17].fvalue < 0.060365073)) {
            if ( (data[2].fvalue < 0.19738433)) {
              sum += (float)0.017529918;
            } else {
              sum += (float)0.13201605;
            }
          } else {
            if ( (data[14].fvalue < 0.66816396)) {
              sum += (float)0.045583367;
            } else {
              sum += (float)-0.03106329;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.64512175)) {
            sum += (float)-0.064061992;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.074119918)) {
          sum += (float)-0.094250284;
        } else {
          if ( (data[14].fvalue < 0.66135222)) {
            if ( (data[15].fvalue < 0.85548687)) {
              sum += (float)0.05958556;
            } else {
              sum += (float)-0.071676888;
            }
          } else {
            sum += (float)-0.05524455;
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.071139038)) {
      if (  (data[4].fvalue < 1.6661365)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[14].fvalue < 0.06780152)) {
            sum += (float)0.10267798;
          } else {
            if ( (data[14].fvalue < 0.17294776)) {
              sum += (float)-0.017047109;
            } else {
              sum += (float)0.043399081;
            }
          }
        } else {
          sum += (float)0.15847936;
        }
      } else {
        sum += (float)-0;
      }
    } else {
      if ( (data[6].fvalue < 0.6877315)) {
        if ( (data[14].fvalue < 0.14554136)) {
          if ( (data[16].fvalue < 0.091871411)) {
            if ( (data[17].fvalue < 0.48977375)) {
              sum += (float)-0.024796501;
            } else {
              sum += (float)0.056959059;
            }
          } else {
            if (  (data[4].fvalue < 1.3832157)) {
              sum += (float)0.050139233;
            } else {
              sum += (float)-0.00097047916;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.19852421)) {
            if ( (data[0].fvalue < 3.0409389)) {
              sum += (float)0.15014322;
            } else {
              sum += (float)0.025568988;
            }
          } else {
            if ( (data[13].fvalue < 0.59805822)) {
              sum += (float)0.0026673011;
            } else {
              sum += (float)0.057430733;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 1.2549273)) {
          if ( (data[14].fvalue < 0.11854933)) {
            if ( (data[16].fvalue < 0.50582147)) {
              sum += (float)-0.10442007;
            } else {
              sum += (float)-0.020753972;
            }
          } else {
            if ( (data[13].fvalue < 0.47614747)) {
              sum += (float)-0.051481638;
            } else {
              sum += (float)0.012031805;
            }
          }
        } else {
          sum += (float)0.058202099;
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.57907987)) {
    if ( (data[3].fvalue < 0.13952471)) {
      if (  (data[4].fvalue < 0.95638943)) {
        if ( (data[14].fvalue < 0.031127084)) {
          if ( (data[3].fvalue < 0.079694368)) {
            sum += (float)-0.17462182;
          } else {
            sum += (float)0.091447771;
          }
        } else {
          sum += (float)-0.19333191;
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[16].fvalue < 0.00021499579)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.0074891062;
            } else {
              sum += (float)0.05374093;
            }
          } else {
            if (  (data[4].fvalue < 1.2888795)) {
              sum += (float)0.046495825;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3920805)) {
            if ( (data[14].fvalue < 0.1560591)) {
              sum += (float)0.047666155;
            } else {
              sum += (float)0.18616369;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[4].fvalue < 1.1546243)) {
          if ( (data[14].fvalue < 0.15808822)) {
            if ( (data[13].fvalue < 0.041118249)) {
              sum += (float)0.065748282;
            } else {
              sum += (float)-0.007099546;
            }
          } else {
            if ( (data[3].fvalue < 0.45783871)) {
              sum += (float)-0.054713897;
            } else {
              sum += (float)0.032399651;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.8109834)) {
              sum += (float)-0.018014766;
            } else {
              sum += (float)0.011906411;
            }
          } else {
            if ( (data[14].fvalue < 0.18486123)) {
              sum += (float)0.018598821;
            } else {
              sum += (float)0.098986462;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.10415462)) {
          if ( (data[14].fvalue < 0.063359469)) {
            if ( (data[16].fvalue < 0.40364042)) {
              sum += (float)-0.091745064;
            } else {
              sum += (float)0.011893085;
            }
          } else {
            if ( (data[3].fvalue < 0.58268678)) {
              sum += (float)-0.0046770107;
            } else {
              sum += (float)-0.046069801;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.56667042)) {
            if (  (data[4].fvalue < 1.2900095)) {
              sum += (float)0.15762402;
            } else {
              sum += (float)0.02669639;
            }
          } else {
            sum += (float)-0.070926011;
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.58246279)) {
      if ( (data[4].fvalue < 1.4970796)) {
        if ( (data[13].fvalue < 0.030778058)) {
          if ( (data[4].fvalue < 1.3178798)) {
            if ( (data[5].fvalue < 0.75433952)) {
              sum += (float)0.065662809;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.033824757;
            } else {
              sum += (float)0.020852325;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.0191462)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.056612998;
            } else {
              sum += (float)0.16525939;
            }
          } else {
            if ( (data[13].fvalue < 0.22051163)) {
              sum += (float)-0.0075963;
            } else {
              sum += (float)0.039899901;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 1.1616751)) {
          if ( (data[13].fvalue < 0.56083506)) {
            if ( (data[16].fvalue < 0.92478484)) {
              sum += (float)-0.019966766;
            } else {
              sum += (float)0.081967473;
            }
          } else {
            sum += (float)0.089584097;
          }
        } else {
          sum += (float)0.11574052;
        }
      }
    } else {
      sum += (float)0.11561219;
    }
  }
  if ( (data[4].fvalue < 1.2142067)) {
    if ( (data[8].fvalue < 3.5)) {
      if (  (data[4].fvalue < 0.97655404)) {
        if ( (data[14].fvalue < 0.056910992)) {
          if ( (data[3].fvalue < 0.085367031)) {
            if ( (data[14].fvalue < 0.020739412)) {
              sum += (float)0.070304982;
            } else {
              sum += (float)-0.20682114;
            }
          } else {
            if ( (data[13].fvalue < 0.00010885099)) {
              sum += (float)0.10497756;
            } else {
              sum += (float)-0.020051075;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.20527241)) {
            if ( (data[14].fvalue < 0.085177496)) {
              sum += (float)-0.11870155;
            } else {
              sum += (float)-0.27872154;
            }
          } else {
            if ( (data[14].fvalue < 0.11564232)) {
              sum += (float)0.054889657;
            } else {
              sum += (float)-0.021217586;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.41348663)) {
          if ( (data[14].fvalue < 0.022816833)) {
            if (  (data[4].fvalue < 0.98937589)) {
              sum += (float)0.094156466;
            } else {
              sum += (float)-0.045819294;
            }
          } else {
            if ( (data[3].fvalue < 0.050261792)) {
              sum += (float)-0.021908831;
            } else {
              sum += (float)0.031227522;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.050563272)) {
              sum += (float)0.024422182;
            } else {
              sum += (float)-0.04701459;
            }
          } else {
            if ( (data[17].fvalue < 0.0010702491)) {
              sum += (float)-0.055610787;
            } else {
              sum += (float)0.017372638;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.060000792)) {
        if (  (data[4].fvalue < 1.0164926)) {
          if ( (data[3].fvalue < 0.83233821)) {
            if ( (data[14].fvalue < 0.15765062)) {
              sum += (float)0.084552065;
            } else {
              sum += (float)0.24948713;
            }
          } else {
            sum += (float)0.0072073289;
          }
        } else {
          if ( (data[14].fvalue < 0.24111661)) {
            if ( (data[3].fvalue < 0.24111584)) {
              sum += (float)-0.00314821;
            } else {
              sum += (float)-0.15859172;
            }
          } else {
            if ( (data[3].fvalue < 0.64920294)) {
              sum += (float)0.10161248;
            } else {
              sum += (float)-0.036897305;
            }
          }
        }
      } else {
        sum += (float)0.2125338;
      }
    }
  } else {
    if ( (data[5].fvalue < 0.23167077)) {
      if ( (data[6].fvalue < 0.094652683)) {
        if ( (data[1].fvalue < 4.243053)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.10391674;
        }
      } else {
        if (  (data[4].fvalue < 1.3375773)) {
          if ( (data[14].fvalue < 0.12207926)) {
            if ( (data[16].fvalue < 0.18677062)) {
              sum += (float)-0.04213994;
            } else {
              sum += (float)0.0412855;
            }
          } else {
            if ( (data[16].fvalue < 0.000485403)) {
              sum += (float)0.010318435;
            } else {
              sum += (float)0.096092112;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.6438842)) {
            if ( (data[17].fvalue < 0.41021353)) {
              sum += (float)-0.0031405643;
            } else {
              sum += (float)0.018038716;
            }
          } else {
            sum += (float)-0.083870873;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.31095076)) {
        if ( (data[15].fvalue < 0.11644231)) {
          if ( (data[14].fvalue < 0.7344979)) {
            if ( (data[16].fvalue < 0.0012884452)) {
              sum += (float)-0.046162825;
            } else {
              sum += (float)-0.014064003;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.020053767;
            } else {
              sum += (float)-0.094234221;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[3].fvalue < 1.0445652)) {
              sum += (float)0.024048476;
            } else {
              sum += (float)-0.041457154;
            }
          } else {
            if ( (data[15].fvalue < 0.24935323)) {
              sum += (float)0.12565351;
            } else {
              sum += (float)0.035767235;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.29390028)) {
          if ( (data[13].fvalue < 0.40969503)) {
            if ( (data[3].fvalue < 0.45424727)) {
              sum += (float)0.053212181;
            } else {
              sum += (float)-0.005649603;
            }
          } else {
            if ( (data[0].fvalue < 4.6226883)) {
              sum += (float)0.092076376;
            } else {
              sum += (float)0.010014731;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.19196826)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.082412429;
            } else {
              sum += (float)0.0016699373;
            }
          } else {
            if ( (data[3].fvalue < 2.5113537)) {
              sum += (float)0.028091721;
            } else {
              sum += (float)-0.059057757;
            }
          }
        }
      }
    }
  }
  if ( (data[17].fvalue < 0.22588709)) {
    if ( (data[17].fvalue < 0.046874404)) {
      if ( (data[17].fvalue < 8.5235086e-05)) {
        if ( (data[14].fvalue < 0.81132376)) {
          if ( (data[13].fvalue < 0.13399488)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0040559112;
            } else {
              sum += (float)-0.015130854;
            }
          } else {
            if (  (data[4].fvalue < 1.3885801)) {
              sum += (float)0.026610311;
            } else {
              sum += (float)-0.0083904462;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[16].fvalue < 0.31802985)) {
              sum += (float)0.0020975124;
            } else {
              sum += (float)0.056236036;
            }
          } else {
            if (  (data[4].fvalue < 1.8361225)) {
              sum += (float)0.13674214;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.014547203)) {
          if ( (data[16].fvalue < 0.037985429)) {
            sum += (float)0.20301814;
          } else {
            sum += (float)0.035002559;
          }
        } else {
          if ( (data[16].fvalue < 0.0234765)) {
            if ( (data[5].fvalue < 0.37093839)) {
              sum += (float)0.092160501;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[1].fvalue < 2.0515454)) {
              sum += (float)-0.037792176;
            } else {
              sum += (float)0.013869799;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.4084752)) {
        if ( (data[14].fvalue < 0.025817832)) {
          sum += (float)-0.10277473;
        } else {
          if ( (data[5].fvalue < 0.21702954)) {
            if ( (data[13].fvalue < 0.0030262519)) {
              sum += (float)0.025600849;
            } else {
              sum += (float)0.13396275;
            }
          } else {
            if ( (data[17].fvalue < 0.097707883)) {
              sum += (float)-0.048808243;
            } else {
              sum += (float)0.020453684;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.0076952339)) {
          if ( (data[16].fvalue < 1.0134678)) {
            if ( (data[14].fvalue < 0.10489917)) {
              sum += (float)-0.01288161;
            } else {
              sum += (float)-0.070530415;
            }
          } else {
            sum += (float)0.054895189;
          }
        } else {
          if (  (data[4].fvalue < 1.7334192)) {
            sum += (float)0.11905202;
          } else {
            if ( (data[15].fvalue < 0.27038801)) {
              sum += (float)-0.047218475;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.369609)) {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[5].fvalue < 0.54488486)) {
          if ( (data[14].fvalue < 0.018392455)) {
            sum += (float)-0.014942684;
          } else {
            if ( (data[16].fvalue < 0.024056904)) {
              sum += (float)0.062038999;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)-0.033309381;
        }
      } else {
        sum += (float)0.13785425;
      }
    } else {
      if ( (data[0].fvalue < 2.0578814)) {
        if ( (data[14].fvalue < 0.088233232)) {
          sum += (float)-0;
        } else {
          if ( (data[2].fvalue < 0.71981108)) {
            sum += (float)0.089434713;
          } else {
            sum += (float)0.011786741;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.33740044)) {
          if ( (data[14].fvalue < 0.44759238)) {
            if ( (data[13].fvalue < 0.0033616028)) {
              sum += (float)-0.070384741;
            } else {
              sum += (float)-0.0080445278;
            }
          } else {
            if ( (data[5].fvalue < 0.074329674)) {
              sum += (float)-0.070369162;
            } else {
              sum += (float)0.017409863;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.37043017)) {
              sum += (float)-0;
            } else {
              sum += (float)0.055293884;
            }
          } else {
            sum += (float)-0.018481789;
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.68161505)) {
    sum += (float)0.095727749;
  } else {
    if ( (data[14].fvalue < 0.26809418)) {
      if ( (data[5].fvalue < 1.2999303)) {
        if (  (data[4].fvalue < 1.0925674)) {
          if ( (data[8].fvalue < 0.5)) {
            sum += (float)-0.089296728;
          } else {
            if ( (data[14].fvalue < 0.0006753807)) {
              sum += (float)0.13503052;
            } else {
              sum += (float)0.0066779787;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.00024528091)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.0023358916;
            } else {
              sum += (float)-0.073919497;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.0057439995;
            } else {
              sum += (float)0.073685423;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[3].fvalue < 0.38106441)) {
            if ( (data[14].fvalue < 0.06981051)) {
              sum += (float)-0.05706409;
            } else {
              sum += (float)-0.29710397;
            }
          } else {
            if ( (data[14].fvalue < 0.13203487)) {
              sum += (float)0.081127822;
            } else {
              sum += (float)-0.048402704;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.020242404)) {
            if ( (data[3].fvalue < 0.38280338)) {
              sum += (float)0.0074704736;
            } else {
              sum += (float)-0.043776844;
            }
          } else {
            sum += (float)0.14014019;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.072575167)) {
        if (  (data[4].fvalue < 1.3913095)) {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)-0.010938823;
          } else {
            if ( (data[14].fvalue < 0.3438555)) {
              sum += (float)0.065127201;
            } else {
              sum += (float)0.16925944;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.090344623)) {
            if ( (data[13].fvalue < 0.082560524)) {
              sum += (float)-0.040498149;
            } else {
              sum += (float)0.0042562173;
            }
          } else {
            sum += (float)0.04823979;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.24472246)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[2].fvalue < 0.97584498)) {
              sum += (float)0.0010806567;
            } else {
              sum += (float)0.042446062;
            }
          } else {
            if (  (data[4].fvalue < 1.3338652)) {
              sum += (float)0.059183281;
            } else {
              sum += (float)-0.13566247;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.048526976;
            } else {
              sum += (float)0.050503317;
            }
          } else {
            if (  (data[4].fvalue < 1.4857953)) {
              sum += (float)0.090718105;
            } else {
              sum += (float)0.016380938;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.2055403)) {
    if ( (data[16].fvalue < 0.28001755)) {
      if ( (data[14].fvalue < 4.9061885e-07)) {
        if (  (data[4].fvalue < 1.6424549)) {
          if (  (data[4].fvalue < 1.2544501)) {
            if ( (data[5].fvalue < 0.11467703)) {
              sum += (float)0.060862504;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[0].fvalue < 2.8106434)) {
              sum += (float)-0.052162547;
            } else {
              sum += (float)0.0022504914;
            }
          }
        } else {
          sum += (float)0.21707414;
        }
      } else {
        if ( (data[14].fvalue < 0.0058861081)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.17040598;
          } else {
            if ( (data[14].fvalue < 0.0031515588)) {
              sum += (float)-0.08103767;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.3275751)) {
            if ( (data[17].fvalue < 0.00019042919)) {
              sum += (float)-0.0034126663;
            } else {
              sum += (float)0.03753344;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.0090042241;
            } else {
              sum += (float)-0.085060731;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.030526437)) {
        if (  (data[4].fvalue < 1.4174639)) {
          if ( (data[16].fvalue < 0.32216632)) {
            sum += (float)-0;
          } else {
            if ( (data[13].fvalue < 0.010693802)) {
              sum += (float)-0.12382372;
            } else {
              sum += (float)-0.032959282;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.1726317)) {
            if ( (data[16].fvalue < 1.526989)) {
              sum += (float)0.026500309;
            } else {
              sum += (float)-0.030175358;
            }
          } else {
            if ( (data[16].fvalue < 0.75213063)) {
              sum += (float)0.01460409;
            } else {
              sum += (float)-0.08325208;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.048976198)) {
          if ( (data[5].fvalue < 0.43244341)) {
            if (  (data[4].fvalue < 1.3751113)) {
              sum += (float)0.095136873;
            } else {
              sum += (float)0.036302704;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.024328509;
            } else {
              sum += (float)0.03680348;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.27135524)) {
            if (  (data[4].fvalue < 1.8281269)) {
              sum += (float)-0.0096329097;
            } else {
              sum += (float)-0.10017896;
            }
          } else {
            if ( (data[6].fvalue < 0.66141558)) {
              sum += (float)0.047828455;
            } else {
              sum += (float)-0.030373003;
            }
          }
        }
      }
    }
  } else {
    if ( (data[0].fvalue < 2.3698852)) {
      if ( (data[6].fvalue < 0.42008173)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[10].fvalue < 0.5)) {
            sum += (float)-0.020681696;
          } else {
            sum += (float)0.067123041;
          }
        } else {
          if ( (data[6].fvalue < 0.26079866)) {
            sum += (float)0.18080264;
          } else {
            if ( (data[14].fvalue < 0.17127833)) {
              sum += (float)0.019546147;
            } else {
              sum += (float)0.1241959;
            }
          }
        }
      } else {
        if ( (data[2].fvalue < 1.0154142)) {
          if ( (data[1].fvalue < 1.1203253)) {
            if ( (data[13].fvalue < 0.58585238)) {
              sum += (float)0.022567524;
            } else {
              sum += (float)0.066826515;
            }
          } else {
            sum += (float)-0.042489331;
          }
        } else {
          sum += (float)-0.047333442;
        }
      }
    } else {
      if ( (data[6].fvalue < 0.2088502)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.03172477;
            } else {
              sum += (float)0.030654242;
            }
          } else {
            if ( (data[0].fvalue < 4.9150724)) {
              sum += (float)0.074764743;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.14441553)) {
            sum += (float)0.010322653;
          } else {
            sum += (float)0.15273409;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.6627503)) {
          if ( (data[14].fvalue < 0.35728788)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.0046482333;
            } else {
              sum += (float)-0.092819884;
            }
          } else {
            if ( (data[16].fvalue < 0.0023450088)) {
              sum += (float)0.043827578;
            } else {
              sum += (float)-0.018647669;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.3358288)) {
            if (  (data[4].fvalue < 1.566148)) {
              sum += (float)-0.034871176;
            } else {
              sum += (float)0.056088235;
            }
          } else {
            sum += (float)-0.035028763;
          }
        }
      }
    }
  }
  if ( (data[2].fvalue < 0.10524724)) {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[0].fvalue < 2.0081205)) {
        if ( (data[14].fvalue < 0.069465727)) {
          if ( (data[2].fvalue < 0.069706842)) {
            sum += (float)-0.093240708;
          } else {
            sum += (float)-0.01825379;
          }
        } else {
          sum += (float)0.0080899745;
        }
      } else {
        if ( (data[14].fvalue < 0.04706154)) {
          if (  (data[4].fvalue < 1.0073001)) {
            if ( (data[13].fvalue < 1.890045e-06)) {
              sum += (float)0.10782901;
            } else {
              sum += (float)-0.054879952;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.013470134;
            } else {
              sum += (float)-0.056509756;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.006115)) {
            if ( (data[3].fvalue < 0.098005578)) {
              sum += (float)-0.2310483;
            } else {
              sum += (float)-0.0090225674;
            }
          } else {
            if ( (data[5].fvalue < 0.410887)) {
              sum += (float)0.0073596062;
            } else {
              sum += (float)0.11325244;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.0355065)) {
        sum += (float)0.16950884;
      } else {
        if ( (data[14].fvalue < 0.18955463)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.10794954)) {
              sum += (float)-0.10832588;
            } else {
              sum += (float)-0.012199439;
            }
          } else {
            sum += (float)0.097852521;
          }
        } else {
          if (  (data[4].fvalue < 1.2976103)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.060670372;
            } else {
              sum += (float)0.21941458;
            }
          } else {
            if ( (data[13].fvalue < 0.088026688)) {
              sum += (float)-0.043877423;
            } else {
              sum += (float)0.05226649;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.023976948)) {
      if ( (data[14].fvalue < 0.025715012)) {
        sum += (float)-0.025542684;
      } else {
        sum += (float)-0.14366737;
      }
    } else {
      if ( (data[0].fvalue < 2.0058169)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[6].fvalue < 0.10107568)) {
            if ( (data[14].fvalue < 0.11624429)) {
              sum += (float)-0.0065168464;
            } else {
              sum += (float)-0.15558138;
            }
          } else {
            if ( (data[2].fvalue < 1.0246161)) {
              sum += (float)0.012227327;
            } else {
              sum += (float)-0.020001071;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.3064096)) {
            if ( (data[6].fvalue < 0.20309424)) {
              sum += (float)0.19932628;
            } else {
              sum += (float)0.096124671;
            }
          } else {
            if ( (data[0].fvalue < 1.5078208)) {
              sum += (float)0.047059659;
            } else {
              sum += (float)-0.014000255;
            }
          }
        }
      } else {
        if ( (data[1].fvalue < 0.27999443)) {
          sum += (float)0.065025054;
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.014427393)) {
              sum += (float)0.0075244303;
            } else {
              sum += (float)-0.025014475;
            }
          } else {
            if ( (data[14].fvalue < 0.037177648)) {
              sum += (float)-0.10098613;
            } else {
              sum += (float)-0.0068419124;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.044413168)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[5].fvalue < 1.8259976)) {
        if ( (data[16].fvalue < 0.039104722)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[14].fvalue < 4.2073218e-05)) {
              sum += (float)0.10473479;
            } else {
              sum += (float)0.0055716028;
            }
          } else {
            if (  (data[4].fvalue < 0.99267602)) {
              sum += (float)0.085520655;
            } else {
              sum += (float)-0.057342872;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.24181621)) {
            if ( (data[16].fvalue < 0.091909543)) {
              sum += (float)-0.065134518;
            } else {
              sum += (float)-0.01498379;
            }
          } else {
            if ( (data[17].fvalue < 0.0013009573)) {
              sum += (float)0.016962793;
            } else {
              sum += (float)-0.042141918;
            }
          }
        }
      } else {
        sum += (float)-0.10525071;
      }
    } else {
      if ( (data[3].fvalue < 0.08697886)) {
        if (  (data[4].fvalue < 1.242208)) {
          sum += (float)0.074842438;
        } else {
          sum += (float)-0.050070751;
        }
      } else {
        if ( (data[17].fvalue < 0.11714539)) {
          if ( (data[0].fvalue < 1.5370777)) {
            sum += (float)0.016777162;
          } else {
            if ( (data[16].fvalue < 0.11080125)) {
              sum += (float)-0.11435796;
            } else {
              sum += (float)-0.011315997;
            }
          }
        } else {
          sum += (float)0.041252211;
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.88843787)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[5].fvalue < 0.53867769)) {
          if (  (data[4].fvalue < 1.226265)) {
            if (  (data[4].fvalue < 1.0536039)) {
              sum += (float)-0.04334927;
            } else {
              sum += (float)0.048883069;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0099351695;
            } else {
              sum += (float)-0.022782538;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.036706321)) {
              sum += (float)0.0048327525;
            } else {
              sum += (float)-0.090305462;
            }
          } else {
            if (  (data[4].fvalue < 1.0459046)) {
              sum += (float)0.047776993;
            } else {
              sum += (float)-0.03642983;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.084898)) {
          if ( (data[3].fvalue < 0.56118882)) {
            if ( (data[14].fvalue < 0.13623455)) {
              sum += (float)0.13562763;
            } else {
              sum += (float)0.28043926;
            }
          } else {
            sum += (float)0.031635679;
          }
        } else {
          if ( (data[14].fvalue < 0.32496297)) {
            if ( (data[16].fvalue < 0.021193497)) {
              sum += (float)-0.042097449;
            } else {
              sum += (float)0.061800476;
            }
          } else {
            if (  (data[4].fvalue < 1.352308)) {
              sum += (float)0.10341667;
            } else {
              sum += (float)0.02188368;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.70166743)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.70433295)) {
            if ( (data[13].fvalue < 0.37493449)) {
              sum += (float)-0.040537387;
            } else {
              sum += (float)0.003581994;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.090645328;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.3678093)) {
            if ( (data[4].fvalue < 1.7829399)) {
              sum += (float)-0.09697967;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.029286053;
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.36604202)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.0054276027;
            } else {
              sum += (float)0.086070985;
            }
          } else {
            if ( (data[5].fvalue < 0.43981034)) {
              sum += (float)0.10696011;
            } else {
              sum += (float)0.033911023;
            }
          }
        } else {
          sum += (float)-0.045584127;
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
