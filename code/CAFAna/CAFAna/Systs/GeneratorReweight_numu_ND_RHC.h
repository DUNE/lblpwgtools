#include "BDTReweighter.h"
class GeneratorReweight_numu_ND_RHC : public BDTReweighter {
 public :
 GeneratorReweight_numu_ND_RHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_numu_ND_RHC() {;}


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
    if ( (data[14].fvalue < 0.051786631)) {
      if ( (data[13].fvalue < 1.5951825)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.016907644)) {
              sum += (float)-0.0672208;
            } else {
              sum += (float)-0.20513101;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.091460764;
            } else {
              sum += (float)-0.16698104;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[3].fvalue < 0.39083302)) {
              sum += (float)-0.26812521;
            } else {
              sum += (float)-0.44557726;
            }
          } else {
            if ( (data[13].fvalue < 0.15709737)) {
              sum += (float)0.38262376;
            } else {
              sum += (float)-0.14100049;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.66181099)) {
          if ( (data[4].fvalue < 1.4949071)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.19990243;
            } else {
              sum += (float)0.02099568;
            }
          } else {
            if ( (data[16].fvalue < 0.77434272)) {
              sum += (float)-0.41748255;
            } else {
              sum += (float)-0.098494552;
            }
          }
        } else {
          if ( (data[17].fvalue < 1.8728104)) {
            if ( (data[15].fvalue < 0.029783502)) {
              sum += (float)-0.06899444;
            } else {
              sum += (float)-0.19650048;
            }
          } else {
            if ( (data[3].fvalue < 4.1198692)) {
              sum += (float)0.21391362;
            } else {
              sum += (float)0.069358394;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.011561666)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.25348628)) {
              sum += (float)-0.29680526;
            } else {
              sum += (float)-0.11716918;
            }
          } else {
            if ( (data[13].fvalue < 1.105268e-06)) {
              sum += (float)0.58927155;
            } else {
              sum += (float)-0.038963862;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.22914501)) {
              sum += (float)0.026600769;
            } else {
              sum += (float)0.29578966;
            }
          } else {
            if ( (data[3].fvalue < 0.15616754)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.30820733;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0659292)) {
            if ( (data[3].fvalue < 0.94412613)) {
              sum += (float)-0.048044115;
            } else {
              sum += (float)0.13080464;
            }
          } else {
            if ( (data[5].fvalue < 0.31575578)) {
              sum += (float)0.28206947;
            } else {
              sum += (float)0.19408612;
            }
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.12995501;
            } else {
              sum += (float)-0.04620095;
            }
          } else {
            if ( (data[3].fvalue < 0.66269159)) {
              sum += (float)0.37536612;
            } else {
              sum += (float)0.20324621;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[13].fvalue < 0.18481001)) {
        if ( (data[14].fvalue < 0.39309663)) {
          if ( (data[8].fvalue < 3.5)) {
            sum += (float)0.27977288;
          } else {
            sum += (float)0.53943694;
          }
        } else {
          sum += (float)0.093557;
        }
      } else {
        if ( (data[16].fvalue < 0.0036452687)) {
          if ( (data[14].fvalue < 0.14207357)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.44455299;
            } else {
              sum += (float)0.13555874;
            }
          } else {
            if ( (data[3].fvalue < 1.2858946)) {
              sum += (float)0.37840083;
            } else {
              sum += (float)0.2549181;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.572916)) {
            sum += (float)-0.18651326;
          } else {
            if ( (data[16].fvalue < 0.14772248)) {
              sum += (float)0.014091654;
            } else {
              sum += (float)0.19959402;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.0023943358)) {
        if ( (data[3].fvalue < 1.2088463)) {
          if ( (data[8].fvalue < 6.5)) {
            sum += (float)0.50633776;
          } else {
            sum += (float)0.58276993;
          }
        } else {
          if ( (data[7].fvalue < 9.5)) {
            if ( (data[4].fvalue < 1.431373)) {
              sum += (float)0.28247565;
            } else {
              sum += (float)0.42807937;
            }
          } else {
            sum += (float)0.52360886;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.56178665)) {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[7].fvalue < 9.5)) {
              sum += (float)0.3269617;
            } else {
              sum += (float)0.49961683;
            }
          } else {
            if ( (data[17].fvalue < 0.0022938552)) {
              sum += (float)0.30617023;
            } else {
              sum += (float)0.025533866;
            }
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.19248587)) {
      if ( (data[13].fvalue < 1.7742975)) {
        if ( (data[5].fvalue < 1.3182293)) {
          if ( (data[12].fvalue < 2.5)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.065019764;
            } else {
              sum += (float)0.19588572;
            }
          } else {
            sum += (float)0.55996382;
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.40011427;
          } else {
            if ( (data[13].fvalue < 0.00011086044)) {
              sum += (float)0.54605275;
            } else {
              sum += (float)0.13305795;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 1.0736948)) {
          if ( (data[15].fvalue < 1.3376842)) {
            if ( (data[4].fvalue < 1.5767817)) {
              sum += (float)-0.10994183;
            } else {
              sum += (float)-0.32743153;
            }
          } else {
            if ( (data[15].fvalue < 2.1061234)) {
              sum += (float)-0.096030578;
            } else {
              sum += (float)0.11098531;
            }
          }
        } else {
          if ( (data[17].fvalue < 2.1952558)) {
            if ( (data[13].fvalue < 2.3197212)) {
              sum += (float)0.056187332;
            } else {
              sum += (float)-0.081268318;
            }
          } else {
            sum += (float)0.14479095;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.026234429)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.32348064)) {
              sum += (float)-0.19776094;
            } else {
              sum += (float)-0.036212903;
            }
          } else {
            if ( (data[13].fvalue < 1.9583429e-06)) {
              sum += (float)0.44311401;
            } else {
              sum += (float)0.10300502;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[5].fvalue < 0.40971905)) {
              sum += (float)0.25568265;
            } else {
              sum += (float)0.12007219;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.052670456;
            } else {
              sum += (float)-0.32576716;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[13].fvalue < 1.1817234)) {
              sum += (float)0.19172174;
            } else {
              sum += (float)-0.035124432;
            }
          } else {
            sum += (float)0.41156232;
          }
        } else {
          if ( (data[5].fvalue < 0.45257437)) {
            if ( (data[8].fvalue < 6.5)) {
              sum += (float)0.097427495;
            } else {
              sum += (float)0.26506868;
            }
          } else {
            if ( (data[13].fvalue < 0.29755789)) {
              sum += (float)-0.091000371;
            } else {
              sum += (float)0.030013787;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[13].fvalue < 0.16698426)) {
        if ( (data[14].fvalue < 0.33418733)) {
          if ( (data[8].fvalue < 3.5)) {
            sum += (float)0.28764382;
          } else {
            sum += (float)0.42573509;
          }
        } else {
          sum += (float)0.024821294;
        }
      } else {
        if ( (data[16].fvalue < 0.0010600748)) {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[14].fvalue < 0.27798137)) {
              sum += (float)0.099223234;
            } else {
              sum += (float)0.23393032;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.41587636;
            } else {
              sum += (float)0.26524034;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.12125735)) {
            if ( (data[5].fvalue < 0.41818702)) {
              sum += (float)0.021436784;
            } else {
              sum += (float)-0.18464452;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.21057981;
            } else {
              sum += (float)-0.038495298;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.0024303771)) {
        if ( (data[7].fvalue < 9.5)) {
          if ( (data[13].fvalue < 0.2264856)) {
            sum += (float)0.44560853;
          } else {
            if ( (data[17].fvalue < 0.0059124618)) {
              sum += (float)0.35765377;
            } else {
              sum += (float)0.26945096;
            }
          }
        } else {
          sum += (float)0.44744399;
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.1286846)) {
            if ( (data[5].fvalue < 0.28712443)) {
              sum += (float)0.27287802;
            } else {
              sum += (float)0.010566039;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.38307446;
            } else {
              sum += (float)0.17850615;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.38684875)) {
            sum += (float)0.1798315;
          } else {
            sum += (float)-0;
          }
        }
      }
    }
  }
  if ( (data[8].fvalue < 4.5)) {
    if ( (data[14].fvalue < 0.027358949)) {
      if ( (data[16].fvalue < 0.012749119)) {
        if ( (data[14].fvalue < 4.3494828e-07)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.060488783;
            } else {
              sum += (float)0.084293351;
            }
          } else {
            sum += (float)0.61960453;
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.4608615;
            } else {
              sum += (float)-0.33169949;
            }
          } else {
            if ( (data[13].fvalue < 1.7813337e-06)) {
              sum += (float)0.57184631;
            } else {
              sum += (float)-0.12194943;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[12].fvalue < 1.5)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.27060786;
            } else {
              sum += (float)-0.082043156;
            }
          } else {
            if ( (data[3].fvalue < 0.93445349)) {
              sum += (float)0.46195009;
            } else {
              sum += (float)0.12398922;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.050687172;
            } else {
              sum += (float)0.16603066;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.3804543;
            } else {
              sum += (float)-0.1479287;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.021183586)) {
        if ( (data[14].fvalue < 0.21232179)) {
          if ( (data[15].fvalue < 0.95120108)) {
            if ( (data[5].fvalue < 0.20441756)) {
              sum += (float)0.0048246817;
            } else {
              sum += (float)-0.12452979;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.39035198;
            } else {
              sum += (float)-0.060636826;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.36107773)) {
            if ( (data[17].fvalue < 0.034130566)) {
              sum += (float)0.10923973;
            } else {
              sum += (float)-0.031255782;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.099826492;
            } else {
              sum += (float)0.095501423;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0528984)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.016775891;
            } else {
              sum += (float)-0.11437724;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.19529961;
            } else {
              sum += (float)0.036957517;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.27972978)) {
            if ( (data[7].fvalue < 6.5)) {
              sum += (float)-0.082794823;
            } else {
              sum += (float)0.18153802;
            }
          } else {
            if ( (data[13].fvalue < 0.33777779)) {
              sum += (float)0.011917424;
            } else {
              sum += (float)0.1591965;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.98448837)) {
      if ( (data[8].fvalue < 7.5)) {
        if (  (data[4].fvalue < 1.1242685)) {
          if ( (data[3].fvalue < 0.5382328)) {
            sum += (float)0.4289135;
          } else {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.048295815;
            } else {
              sum += (float)0.32788232;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.27055845)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.19455077;
            } else {
              sum += (float)0.30951342;
            }
          } else {
            if ( (data[14].fvalue < 0.33537239)) {
              sum += (float)0.069619492;
            } else {
              sum += (float)0.18604481;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.9784994)) {
          sum += (float)0.39581171;
        } else {
          if ( (data[8].fvalue < 10.5)) {
            if ( (data[13].fvalue < 0.34184182)) {
              sum += (float)0.270401;
            } else {
              sum += (float)0.1563344;
            }
          } else {
            sum += (float)0.34031901;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 7.5)) {
        if ( (data[14].fvalue < 0.26408535)) {
          if ( (data[13].fvalue < 0.35534191)) {
            if (  (data[4].fvalue < 1.3350165)) {
              sum += (float)-0.12671688;
            } else {
              sum += (float)0.17416869;
            }
          } else {
            if ( (data[14].fvalue < 0.17048785)) {
              sum += (float)-0.44800752;
            } else {
              sum += (float)-0.18958129;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.58497524)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.19271831;
            } else {
              sum += (float)0.0058535971;
            }
          } else {
            if ( (data[14].fvalue < 0.51979637)) {
              sum += (float)0.051037844;
            } else {
              sum += (float)0.2453265;
            }
          }
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[7].fvalue < 11.5)) {
              sum += (float)0.22924477;
            } else {
              sum += (float)0.37500316;
            }
          } else {
            sum += (float)0.00084802619;
          }
        } else {
          if ( (data[17].fvalue < 0.60317999)) {
            sum += (float)-0.15249185;
          } else {
            if ( (data[3].fvalue < 2.5784559)) {
              sum += (float)0.21977541;
            } else {
              sum += (float)0.019876862;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[5].fvalue < 0.24356468)) {
      if ( (data[14].fvalue < 0.016520187)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[12].fvalue < 2.5)) {
            if ( (data[13].fvalue < 1.2255807)) {
              sum += (float)-0.041216042;
            } else {
              sum += (float)-0.19354236;
            }
          } else {
            sum += (float)0.41779026;
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.044176705;
            } else {
              sum += (float)0.15457486;
            }
          } else {
            if ( (data[13].fvalue < 0.20685226)) {
              sum += (float)-0.042263418;
            } else {
              sum += (float)-0.19917907;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.0072089951)) {
          if ( (data[14].fvalue < 0.12508063)) {
            if ( (data[15].fvalue < 1.849823)) {
              sum += (float)-0.10456046;
            } else {
              sum += (float)-0.35671562;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0017344645;
            } else {
              sum += (float)0.17434295;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.1798313;
            } else {
              sum += (float)0.065345466;
            }
          } else {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)-0.014339193;
            } else {
              sum += (float)0.12530619;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.025589816)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[5].fvalue < 1.3725964)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.033565152;
            } else {
              sum += (float)0.095456116;
            }
          } else {
            if ( (data[14].fvalue < 1.0229714e-06)) {
              sum += (float)0.1723486;
            } else {
              sum += (float)0.018543374;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.34684843)) {
            if (  (data[4].fvalue < 1.1514754)) {
              sum += (float)0.36971384;
            } else {
              sum += (float)0.12123525;
            }
          } else {
            if ( (data[8].fvalue < 6.5)) {
              sum += (float)0.05181266;
            } else {
              sum += (float)0.19503699;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.15699834)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.9991771)) {
              sum += (float)-0.09889745;
            } else {
              sum += (float)-0.27333012;
            }
          } else {
            if ( (data[5].fvalue < 0.67047596)) {
              sum += (float)-0.22742514;
            } else {
              sum += (float)-0.43666282;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.014807447)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.14771003;
            } else {
              sum += (float)0.041000292;
            }
          } else {
            if ( (data[13].fvalue < 0.007218237)) {
              sum += (float)-0.09281975;
            } else {
              sum += (float)0.026477545;
            }
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.0021760492)) {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[13].fvalue < 0.21123347)) {
          if ( (data[14].fvalue < 0.24880677)) {
            sum += (float)0.34900451;
          } else {
            sum += (float)0.19883686;
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.324056;
          } else {
            if ( (data[14].fvalue < 0.098847985)) {
              sum += (float)0.037805725;
            } else {
              sum += (float)0.1876456;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 1.3928223)) {
          sum += (float)0.35031325;
        } else {
          if ( (data[7].fvalue < 11.5)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)0.29974774;
            } else {
              sum += (float)0.17028001;
            }
          } else {
            sum += (float)0.3377012;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)0.37763199;
        } else {
          sum += (float)0.21020694;
        }
      } else {
        if ( (data[7].fvalue < 10.5)) {
          if ( (data[16].fvalue < 0.14166701)) {
            if ( (data[5].fvalue < 0.36891943)) {
              sum += (float)-0.015570091;
            } else {
              sum += (float)-0.24957833;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.139585;
            } else {
              sum += (float)-0.087344564;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.014425119)) {
            sum += (float)0.32454315;
          } else {
            sum += (float)0.19013323;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[13].fvalue < 2.1287036)) {
      if ( (data[8].fvalue < 5.5)) {
        if ( (data[12].fvalue < 3.5)) {
          if ( (data[5].fvalue < 0.30560154)) {
            if ( (data[14].fvalue < 0.013933551)) {
              sum += (float)-0.021085924;
            } else {
              sum += (float)0.045287199;
            }
          } else {
            if ( (data[16].fvalue < 0.0031433604)) {
              sum += (float)-0.016603189;
            } else {
              sum += (float)-0.097778;
            }
          }
        } else {
          sum += (float)0.46973041;
        }
      } else {
        if ( (data[3].fvalue < 0.81940377)) {
          if (  (data[4].fvalue < 1.1947994)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.21752955;
            } else {
              sum += (float)0.34014037;
            }
          } else {
            if ( (data[8].fvalue < 7.5)) {
              sum += (float)0.11963364;
            } else {
              sum += (float)0.23731852;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.62946951)) {
              sum += (float)0.23514017;
            } else {
              sum += (float)0.044493519;
            }
          } else {
            if ( (data[8].fvalue < 8.5)) {
              sum += (float)0.001890355;
            } else {
              sum += (float)0.15277651;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.45683059)) {
        if ( (data[16].fvalue < 0.59715927)) {
          if ( (data[4].fvalue < 1.7537789)) {
            if ( (data[17].fvalue < 0.017469492)) {
              sum += (float)-0.037392765;
            } else {
              sum += (float)-0.20588724;
            }
          } else {
            if ( (data[15].fvalue < 0.69151747)) {
              sum += (float)-0.32030833;
            } else {
              sum += (float)-0.14918827;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.4551411)) {
            sum += (float)-0.15874183;
          } else {
            sum += (float)0.095084563;
          }
        }
      } else {
        if ( (data[4].fvalue < 2.9505792)) {
          if ( (data[17].fvalue < 1.5285497)) {
            if ( (data[15].fvalue < 0.39720291)) {
              sum += (float)-0.11291275;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.062333412;
          }
        } else {
          if ( (data[16].fvalue < 0.073110029)) {
            sum += (float)0.048611347;
          } else {
            sum += (float)0.19766501;
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.24442756)) {
      if ( (data[14].fvalue < 0.45364541)) {
        if ( (data[8].fvalue < 5.5)) {
          if (  (data[4].fvalue < 1.1001997)) {
            sum += (float)0.31397706;
          } else {
            sum += (float)0.10694725;
          }
        } else {
          sum += (float)0.34513062;
        }
      } else {
        sum += (float)0.047243524;
      }
    } else {
      if ( (data[7].fvalue < 9.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if (  (data[4].fvalue < 1.3390409)) {
            sum += (float)0.10525217;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.31926557;
            } else {
              sum += (float)0.21067126;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.00374729)) {
            if ( (data[14].fvalue < 0.27537519)) {
              sum += (float)0.064998545;
            } else {
              sum += (float)0.17435756;
            }
          } else {
            if ( (data[17].fvalue < 0.1225443)) {
              sum += (float)-0.15777585;
            } else {
              sum += (float)0.029825736;
            }
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[17].fvalue < 0.012466306)) {
            sum += (float)0.30903393;
          } else {
            if ( (data[7].fvalue < 11.5)) {
              sum += (float)0.12901846;
            } else {
              sum += (float)0.26748505;
            }
          }
        } else {
          sum += (float)0.033419844;
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[14].fvalue < 0.014655476)) {
        if ( (data[15].fvalue < 0.029784579)) {
          if ( (data[13].fvalue < 2.935139e-06)) {
            if (  (data[4].fvalue < 1.5258316)) {
              sum += (float)0.085437313;
            } else {
              sum += (float)0.56622881;
            }
          } else {
            if ( (data[14].fvalue < 7.992252e-06)) {
              sum += (float)0.0073095271;
            } else {
              sum += (float)-0.15677977;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[12].fvalue < 0.5)) {
              sum += (float)-0.10291187;
            } else {
              sum += (float)0.063109316;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.1070288;
            } else {
              sum += (float)-0.13474855;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.043060981)) {
          if ( (data[14].fvalue < 0.11655725)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.21408629;
            } else {
              sum += (float)-0.06089339;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.034096088;
            } else {
              sum += (float)0.054243885;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0550865)) {
              sum += (float)0.008672283;
            } else {
              sum += (float)0.14573996;
            }
          } else {
            if ( (data[5].fvalue < 0.6013658)) {
              sum += (float)0.057154935;
            } else {
              sum += (float)-0.10413545;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[15].fvalue < 0.12411164)) {
          if ( (data[6].fvalue < 0.045882784)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.099919468;
            } else {
              sum += (float)0.37272006;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.05180366;
            } else {
              sum += (float)-0.16878781;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.34486854)) {
              sum += (float)-0;
            } else {
              sum += (float)0.15309542;
            }
          } else {
            if ( (data[14].fvalue < 0.016220747)) {
              sum += (float)-0.24833943;
            } else {
              sum += (float)-0.015879562;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.071191311)) {
          if ( (data[0].fvalue < 4.9321775)) {
            sum += (float)0.41730583;
          } else {
            sum += (float)0.18005309;
          }
        } else {
          if ( (data[8].fvalue < 7.5)) {
            if ( (data[3].fvalue < 0.4467389)) {
              sum += (float)0.050172724;
            } else {
              sum += (float)-0.045274727;
            }
          } else {
            if ( (data[3].fvalue < 2.2031877)) {
              sum += (float)0.19476956;
            } else {
              sum += (float)0.026989631;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.1316064)) {
      if ( (data[14].fvalue < 0.25460735)) {
        if ( (data[3].fvalue < 0.56963921)) {
          if (  (data[4].fvalue < 1.0734992)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.46981022;
            } else {
              sum += (float)0.33768034;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.19826579;
            } else {
              sum += (float)0.35915473;
            }
          }
        } else {
          if ( (data[8].fvalue < 6.5)) {
            sum += (float)0.074520357;
          } else {
            sum += (float)0.3257308;
          }
        }
      } else {
        if ( (data[8].fvalue < 6.5)) {
          sum += (float)-0.29371724;
        } else {
          sum += (float)0.087818459;
        }
      }
    } else {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[16].fvalue < 0.003460099)) {
          if ( (data[14].fvalue < 0.11480078)) {
            if (  (data[4].fvalue < 1.1213363)) {
              sum += (float)0.19938092;
            } else {
              sum += (float)-0.026045254;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.23724642;
            } else {
              sum += (float)0.091033578;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.13847558)) {
            if ( (data[15].fvalue < 0.12309301)) {
              sum += (float)-0.25784519;
            } else {
              sum += (float)-0.034550246;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.073504694;
            } else {
              sum += (float)-0.11385006;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.0049823998)) {
          if ( (data[3].fvalue < 1.3928223)) {
            if ( (data[7].fvalue < 10.5)) {
              sum += (float)0.23917565;
            } else {
              sum += (float)0.32070553;
            }
          } else {
            if (  (data[4].fvalue < 1.4979845)) {
              sum += (float)0.079678811;
            } else {
              sum += (float)0.20822845;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.43990955)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.18234569;
            } else {
              sum += (float)0.0096641965;
            }
          } else {
            sum += (float)-0.0443382;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[13].fvalue < 1.9860877)) {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[16].fvalue < 0.0064164503)) {
          if ( (data[15].fvalue < 2.8458762)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.0010291815;
            } else {
              sum += (float)0.092440672;
            }
          } else {
            if ( (data[14].fvalue < 0.72072428)) {
              sum += (float)-0.25627637;
            } else {
              sum += (float)0.20320457;
            }
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[16].fvalue < 0.43626252)) {
              sum += (float)-0.23526393;
            } else {
              sum += (float)0.0074273515;
            }
          } else {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)-0.047673266;
            } else {
              sum += (float)0.22274947;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.018288597)) {
          if ( (data[4].fvalue < 1.3194494)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.040004339;
            } else {
              sum += (float)-0.18325414;
            }
          } else {
            if ( (data[14].fvalue < 1.6596099)) {
              sum += (float)0.11987524;
            } else {
              sum += (float)-0.048855815;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.005343203;
            } else {
              sum += (float)0.092202276;
            }
          } else {
            if ( (data[5].fvalue < 0.44391757)) {
              sum += (float)-0.035954386;
            } else {
              sum += (float)-0.20457844;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.53921986)) {
        if ( (data[16].fvalue < 0.99352396)) {
          if ( (data[14].fvalue < 0.4330298)) {
            if ( (data[4].fvalue < 1.8521986)) {
              sum += (float)-0.079762742;
            } else {
              sum += (float)-0.23409599;
            }
          } else {
            sum += (float)0.12177717;
          }
        } else {
          sum += (float)0.11449473;
        }
      } else {
        if ( (data[17].fvalue < 1.6558681)) {
          if ( (data[15].fvalue < 0.71383959)) {
            if ( (data[15].fvalue < 0.023879815)) {
              sum += (float)0.010252341;
            } else {
              sum += (float)-0.11547924;
            }
          } else {
            sum += (float)0.1146036;
          }
        } else {
          if ( (data[15].fvalue < 0.0043796445)) {
            sum += (float)0.14415544;
          } else {
            sum += (float)0.031593066;
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.56119531)) {
      if (  (data[4].fvalue < 1.1419713)) {
        sum += (float)0.3277854;
      } else {
        if ( (data[8].fvalue < 7.5)) {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[15].fvalue < 0.12470393)) {
              sum += (float)0.029699603;
            } else {
              sum += (float)0.18053824;
            }
          } else {
            if ( (data[3].fvalue < 0.1781413)) {
              sum += (float)0.2109551;
            } else {
              sum += (float)0.12983729;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8640591)) {
            sum += (float)0.31066683;
          } else {
            sum += (float)0.10413584;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.060308211)) {
          if ( (data[8].fvalue < 0.5)) {
            sum += (float)0.29305637;
          } else {
            sum += (float)0.050838646;
          }
        } else {
          sum += (float)0.31969911;
        }
      } else {
        if ( (data[7].fvalue < 10.5)) {
          if ( (data[14].fvalue < 0.07954368)) {
            if ( (data[13].fvalue < 0.3640756)) {
              sum += (float)0.038638111;
            } else {
              sum += (float)-0.33665705;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.062524274;
            } else {
              sum += (float)-0.094018765;
            }
          }
        } else {
          if ( (data[3].fvalue < 2.6660829)) {
            if ( (data[16].fvalue < 0.011145351)) {
              sum += (float)0.28076574;
            } else {
              sum += (float)0.1716273;
            }
          } else {
            sum += (float)0.077275909;
          }
        }
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[12].fvalue < 3.5)) {
      if ( (data[14].fvalue < 0.32568836)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.17737216)) {
            if ( (data[15].fvalue < 0.8197493)) {
              sum += (float)-0.002704164;
            } else {
              sum += (float)-0.18349355;
            }
          } else {
            if ( (data[14].fvalue < 0.0079452125)) {
              sum += (float)-0.0063854447;
            } else {
              sum += (float)0.095213205;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.08194945)) {
            if ( (data[13].fvalue < 0.26375687)) {
              sum += (float)-0.058445834;
            } else {
              sum += (float)-0.25041494;
            }
          } else {
            if ( (data[3].fvalue < 0.61704075)) {
              sum += (float)0.02101744;
            } else {
              sum += (float)-0.069103599;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.40977377)) {
          if ( (data[14].fvalue < 1.5388713)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.051053673;
            } else {
              sum += (float)-0.040725164;
            }
          } else {
            if ( (data[15].fvalue < 0.65722746)) {
              sum += (float)-0.19747731;
            } else {
              sum += (float)0.09443263;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.015388092)) {
            if ( (data[14].fvalue < 0.43572477)) {
              sum += (float)0.093843997;
            } else {
              sum += (float)0.23025574;
            }
          } else {
            if ( (data[5].fvalue < 0.51870829)) {
              sum += (float)0.0997454;
            } else {
              sum += (float)-0.072422482;
            }
          }
        }
      }
    } else {
      sum += (float)0.40135366;
    }
  } else {
    if ( (data[14].fvalue < 0.17135502)) {
      if ( (data[13].fvalue < 0.37702361)) {
        sum += (float)0.30061406;
      } else {
        sum += (float)0.075858936;
      }
    } else {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[8].fvalue < 9.5)) {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.27247259;
          } else {
            if ( (data[3].fvalue < 1.5522021)) {
              sum += (float)0.083632745;
            } else {
              sum += (float)-0.014285059;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.60845125)) {
            if ( (data[3].fvalue < 1.7628601)) {
              sum += (float)0.27389455;
            } else {
              sum += (float)0.14560308;
            }
          } else {
            if ( (data[14].fvalue < 0.91522157)) {
              sum += (float)-0.0013544471;
            } else {
              sum += (float)0.17680083;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.78537977)) {
          if ( (data[3].fvalue < 1.9264481)) {
            sum += (float)-0.098044738;
          } else {
            sum += (float)-0.24320091;
          }
        } else {
          if ( (data[11].fvalue < 2.5)) {
            sum += (float)0.10448073;
          } else {
            sum += (float)-0.10139435;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 7.5)) {
    if ( (data[13].fvalue < 1.3839498)) {
      if ( (data[5].fvalue < 0.3973226)) {
        if ( (data[13].fvalue < 0.13044611)) {
          if ( (data[17].fvalue < 0.027699467)) {
            if ( (data[15].fvalue < 3.1827884)) {
              sum += (float)0.02295588;
            } else {
              sum += (float)-0.19372082;
            }
          } else {
            if ( (data[14].fvalue < 0.008867573)) {
              sum += (float)-0.16419065;
            } else {
              sum += (float)-0.021718983;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.84888411)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.0045098728;
            } else {
              sum += (float)0.05896062;
            }
          } else {
            if ( (data[13].fvalue < 0.34747428)) {
              sum += (float)0.012351993;
            } else {
              sum += (float)0.16995868;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.1435831)) {
          if ( (data[17].fvalue < 0.022160698)) {
            if (  (data[4].fvalue < 1.0174839)) {
              sum += (float)-0.03768478;
            } else {
              sum += (float)-0.0025725162;
            }
          } else {
            if (  (data[4].fvalue < 1.9880149)) {
              sum += (float)-0.093842074;
            } else {
              sum += (float)-0.012299621;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.657671)) {
            if ( (data[14].fvalue < 1.6163134e-05)) {
              sum += (float)0.083574861;
            } else {
              sum += (float)-0.0023514989;
            }
          } else {
            if ( (data[3].fvalue < 0.18263507)) {
              sum += (float)-0;
            } else {
              sum += (float)0.17983387;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.7130306)) {
        if ( (data[17].fvalue < 0.92552578)) {
          if ( (data[15].fvalue < 1.5728755)) {
            if ( (data[14].fvalue < 0.56880391)) {
              sum += (float)-0.15115172;
            } else {
              sum += (float)0.16778615;
            }
          } else {
            if ( (data[15].fvalue < 2.0028918)) {
              sum += (float)0.089067757;
            } else {
              sum += (float)0.26367804;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.017347753)) {
            if ( (data[5].fvalue < 0.56217515)) {
              sum += (float)0.22147594;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[14].fvalue < 0.0095711779)) {
              sum += (float)-0.078647368;
            } else {
              sum += (float)0.047739916;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 3.1979532)) {
          if ( (data[13].fvalue < 2.2779467)) {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.22758164;
            } else {
              sum += (float)0.062929332;
            }
          } else {
            if (  (data[4].fvalue < 3.1214142)) {
              sum += (float)0.014926494;
            } else {
              sum += (float)0.19198291;
            }
          }
        } else {
          if ( (data[4].fvalue < 3.1350713)) {
            if ( (data[17].fvalue < 0.29022402)) {
              sum += (float)-0.2335337;
            } else {
              sum += (float)-0.039561346;
            }
          } else {
            sum += (float)0.012458405;
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.0024303771)) {
      if ( (data[3].fvalue < 0.71990472)) {
        if (  (data[4].fvalue < 1.3326906)) {
          sum += (float)0.2903434;
        } else {
          if ( (data[7].fvalue < 10.5)) {
            if (  (data[4].fvalue < 1.7919836)) {
              sum += (float)0.17576806;
            } else {
              sum += (float)0.08842285;
            }
          } else {
            sum += (float)0.27219823;
          }
        }
      } else {
        if ( (data[7].fvalue < 11.5)) {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)0.23394601;
          } else {
            if ( (data[15].fvalue < 0.35406116)) {
              sum += (float)0.039722707;
            } else {
              sum += (float)0.18097766;
            }
          }
        } else {
          sum += (float)0.2442368;
        }
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[15].fvalue < 0.17452049)) {
          sum += (float)0.10721712;
        } else {
          sum += (float)0.26485804;
        }
      } else {
        if ( (data[13].fvalue < 0.42377886)) {
          sum += (float)0.14405045;
        } else {
          if ( (data[5].fvalue < 0.43252999)) {
            if ( (data[7].fvalue < 11.5)) {
              sum += (float)-0.053678263;
            } else {
              sum += (float)0.11742833;
            }
          } else {
            sum += (float)-0.20064941;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[12].fvalue < 3.5)) {
        if ( (data[12].fvalue < 1.5)) {
          if ( (data[17].fvalue < 0.043817021)) {
            if (  (data[4].fvalue < 1.0492215)) {
              sum += (float)-0.014221772;
            } else {
              sum += (float)0.028561307;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.034032851;
            } else {
              sum += (float)0.04016649;
            }
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[3].fvalue < 0.32865492)) {
              sum += (float)0.057552926;
            } else {
              sum += (float)-0.24292572;
            }
          } else {
            if (  (data[4].fvalue < 1.8708262)) {
              sum += (float)0.43750736;
            } else {
              sum += (float)0.023039358;
            }
          }
        }
      } else {
        sum += (float)0.35976949;
      }
    } else {
      if ( (data[6].fvalue < 0.059148796)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[0].fvalue < 3.2432604)) {
            sum += (float)0.2713744;
          } else {
            if ( (data[13].fvalue < 0.090960048)) {
              sum += (float)-0.12567405;
            } else {
              sum += (float)0.054965932;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.041016135)) {
            sum += (float)0.44576126;
          } else {
            sum += (float)0.19985521;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.17436893)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[4].fvalue < 1.3835964)) {
              sum += (float)-0.040698022;
            } else {
              sum += (float)0.041880824;
            }
          } else {
            if ( (data[13].fvalue < 0.13916346)) {
              sum += (float)0.027781924;
            } else {
              sum += (float)-0.15940809;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.10081249)) {
            if ( (data[14].fvalue < 0.69558692)) {
              sum += (float)-0.20942359;
            } else {
              sum += (float)-0.42577395;
            }
          } else {
            if ( (data[13].fvalue < 0.3247292)) {
              sum += (float)-0.025912495;
            } else {
              sum += (float)0.059783924;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.10499007)) {
      if ( (data[14].fvalue < 0.2145673)) {
        if ( (data[14].fvalue < 0.11438721)) {
          sum += (float)0.31924954;
        } else {
          sum += (float)0.18659608;
        }
      } else {
        sum += (float)-0.06212135;
      }
    } else {
      if ( (data[17].fvalue < 0.0019896948)) {
        if ( (data[7].fvalue < 8.5)) {
          if ( (data[15].fvalue < 0.18534353)) {
            if (  (data[4].fvalue < 1.0995693)) {
              sum += (float)0.14214863;
            } else {
              sum += (float)0.0019247807;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.21741249;
            } else {
              sum += (float)0.098954797;
            }
          }
        } else {
          if ( (data[7].fvalue < 11.5)) {
            if ( (data[3].fvalue < 1.1704688)) {
              sum += (float)0.19294769;
            } else {
              sum += (float)0.092585668;
            }
          } else {
            sum += (float)0.26083729;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.15192421)) {
          if ( (data[5].fvalue < 0.51152682)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)-0.045264278;
            } else {
              sum += (float)-0.17747891;
            }
          } else {
            sum += (float)-0.2979742;
          }
        } else {
          if ( (data[11].fvalue < 2.5)) {
            if ( (data[17].fvalue < 0.71124101)) {
              sum += (float)0.011080694;
            } else {
              sum += (float)0.1167603;
            }
          } else {
            if ( (data[14].fvalue < 0.48870778)) {
              sum += (float)-0.053945467;
            } else {
              sum += (float)-0.24708776;
            }
          }
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.011992119)) {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[5].fvalue < 0.10521321)) {
        if ( (data[13].fvalue < 1.1706507)) {
          if (  (data[4].fvalue < 1.2446287)) {
            if (  (data[4].fvalue < 0.99839628)) {
              sum += (float)0.100453;
            } else {
              sum += (float)-0.025383221;
            }
          } else {
            if (  (data[4].fvalue < 1.7827599)) {
              sum += (float)0.10711499;
            } else {
              sum += (float)0.023490151;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.52380574)) {
            if ( (data[6].fvalue < 0.65892422)) {
              sum += (float)-0.25707278;
            } else {
              sum += (float)-0.11018778;
            }
          } else {
            sum += (float)0.084134042;
          }
        }
      } else {
        if ( (data[12].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.22739057)) {
            if ( (data[15].fvalue < 3.7292035)) {
              sum += (float)-0.011093837;
            } else {
              sum += (float)-0.22963281;
            }
          } else {
            if ( (data[17].fvalue < 0.97400558)) {
              sum += (float)0.0098702926;
            } else {
              sum += (float)0.1043916;
            }
          }
        } else {
          if ( (data[12].fvalue < 2.5)) {
            if (  (data[4].fvalue < 2.5368662)) {
              sum += (float)-0.22112459;
            } else {
              sum += (float)0.0049823979;
            }
          } else {
            sum += (float)0.30279824;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.40573525)) {
        if ( (data[14].fvalue < 0.53504765)) {
          sum += (float)0.27373865;
        } else {
          if ( (data[7].fvalue < 4.5)) {
            sum += (float)0.21585992;
          } else {
            sum += (float)0.02592854;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.0036664996)) {
          if ( (data[14].fvalue < 0.65310693)) {
            sum += (float)0.054371849;
          } else {
            sum += (float)0.18137959;
          }
        } else {
          if ( (data[17].fvalue < 0.4285807)) {
            sum += (float)-0.059669882;
          } else {
            sum += (float)0.023391675;
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.12569493)) {
      if ( (data[15].fvalue < 0.00034854392)) {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.8740029)) {
            sum += (float)-0.43601063;
          } else {
            sum += (float)-0.29850593;
          }
        } else {
          if ( (data[13].fvalue < 0.38197887)) {
            if (  (data[4].fvalue < 1.669767)) {
              sum += (float)-0.30162656;
            } else {
              sum += (float)-0.13893312;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.040159527;
            } else {
              sum += (float)-0.14402115;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4145958)) {
          if ( (data[13].fvalue < 0.14663878)) {
            sum += (float)-0.33672306;
          } else {
            if (  (data[4].fvalue < 1.2674651)) {
              sum += (float)-0.2198507;
            } else {
              sum += (float)-0.11551362;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.080034286;
            } else {
              sum += (float)0.037739452;
            }
          } else {
            if ( (data[14].fvalue < 0.61444056)) {
              sum += (float)-0.16175757;
            } else {
              sum += (float)-0.072287187;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.0093186824)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[4].fvalue < 2.1466365)) {
            if ( (data[15].fvalue < 0.61881089)) {
              sum += (float)-0.20691842;
            } else {
              sum += (float)-0.07915917;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.32859927;
            } else {
              sum += (float)-0.040894914;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.28374159)) {
              sum += (float)-0.03001263;
            } else {
              sum += (float)0.08546263;
            }
          } else {
            if ( (data[5].fvalue < 0.25005692)) {
              sum += (float)-0.10592443;
            } else {
              sum += (float)-0.27823323;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.56795406)) {
          if ( (data[4].fvalue < 1.3874457)) {
            if ( (data[15].fvalue < 0.00093268871)) {
              sum += (float)-0.25993088;
            } else {
              sum += (float)-0.1063388;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.024049064;
            } else {
              sum += (float)-0.083165452;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.03780745)) {
            if ( (data[15].fvalue < 2.8432341)) {
              sum += (float)0.12928033;
            } else {
              sum += (float)-0.005738724;
            }
          } else {
            if ( (data[13].fvalue < 0.90824342)) {
              sum += (float)-0.003626541;
            } else {
              sum += (float)0.11076597;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 2.5738652)) {
    if ( (data[3].fvalue < 1.0387001)) {
      if ( (data[13].fvalue < 1.0362978)) {
        if ( (data[8].fvalue < 6.5)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.17577942)) {
              sum += (float)0.00062357076;
            } else {
              sum += (float)0.033802893;
            }
          } else {
            if (  (data[4].fvalue < 1.5840545)) {
              sum += (float)0.39001378;
            } else {
              sum += (float)0.0012623556;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3376523)) {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)0.16791952;
            } else {
              sum += (float)0.27281168;
            }
          } else {
            if ( (data[14].fvalue < 0.18340066)) {
              sum += (float)0.21264242;
            } else {
              sum += (float)0.049195394;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.64823645)) {
          if ( (data[7].fvalue < 4.5)) {
            if (  (data[4].fvalue < 2.5759616)) {
              sum += (float)-0.18941104;
            } else {
              sum += (float)0.053009719;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.19333209;
            } else {
              sum += (float)-0.014483516;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.03644415)) {
            sum += (float)0.1822644;
          } else {
            sum += (float)0.05399771;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.0631742)) {
        if ( (data[5].fvalue < 1.2423271)) {
          if ( (data[14].fvalue < 0.39997461)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.039865233;
            } else {
              sum += (float)-0.15668213;
            }
          } else {
            if ( (data[13].fvalue < 0.53470278)) {
              sum += (float)-0.029484861;
            } else {
              sum += (float)0.098245971;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 0.69449443)) {
              sum += (float)0.22785778;
            } else {
              sum += (float)0.10128225;
            }
          } else {
            if ( (data[5].fvalue < 1.4104347)) {
              sum += (float)-0.095529854;
            } else {
              sum += (float)0.042587262;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.50203919)) {
          if ( (data[14].fvalue < 0.45264474)) {
            if ( (data[17].fvalue < 2.74019)) {
              sum += (float)-0.03657461;
            } else {
              sum += (float)-0.22519189;
            }
          } else {
            if ( (data[14].fvalue < 2.1899819)) {
              sum += (float)0.076672956;
            } else {
              sum += (float)-0.073492169;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 1.6807635)) {
              sum += (float)0.12557016;
            } else {
              sum += (float)0.016313091;
            }
          } else {
            if ( (data[14].fvalue < 0.16048837)) {
              sum += (float)-0.25129446;
            } else {
              sum += (float)0.026478138;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 3.0051446)) {
      if ( (data[14].fvalue < 0.38467443)) {
        if ( (data[17].fvalue < 0.43648779)) {
          if (  (data[4].fvalue < 2.0822897)) {
            if (  (data[4].fvalue < 1.5856028)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.10792796;
            }
          } else {
            if ( (data[16].fvalue < 0.47097158)) {
              sum += (float)-0.23299281;
            } else {
              sum += (float)-0.08102724;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.029859275)) {
              sum += (float)0.043765821;
            } else {
              sum += (float)-0.090075165;
            }
          } else {
            sum += (float)-0.15637045;
          }
        }
      } else {
        sum += (float)0.01925317;
      }
    } else {
      if ( (data[16].fvalue < 0.01946282)) {
        sum += (float)-0.052098837;
      } else {
        sum += (float)0.094323926;
      }
    }
  }
  if ( (data[16].fvalue < 0.0074223615)) {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[7].fvalue < 2.5)) {
        if ( (data[13].fvalue < 0.20067149)) {
          if ( (data[17].fvalue < 3.1296594)) {
            if ( (data[15].fvalue < 3.2988293)) {
              sum += (float)-0.0041898079;
            } else {
              sum += (float)-0.13356595;
            }
          } else {
            if ( (data[14].fvalue < 0.47596431)) {
              sum += (float)-0.28171945;
            } else {
              sum += (float)0.090382367;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0552847)) {
            if ( (data[3].fvalue < 0.43868721)) {
              sum += (float)-0.10537584;
            } else {
              sum += (float)0.0023186058;
            }
          } else {
            if ( (data[13].fvalue < 1.1388578)) {
              sum += (float)0.049186584;
            } else {
              sum += (float)-0.0315524;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.0808005)) {
          if ( (data[3].fvalue < 0.40284026)) {
            if ( (data[14].fvalue < 0.014136162)) {
              sum += (float)0.035643265;
            } else {
              sum += (float)0.32382345;
            }
          } else {
            if ( (data[14].fvalue < 0.11532162)) {
              sum += (float)-0.13886742;
            } else {
              sum += (float)0.034690194;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.21357772)) {
            if ( (data[15].fvalue < 0.13525555)) {
              sum += (float)-0.11104428;
            } else {
              sum += (float)0.029361559;
            }
          } else {
            if ( (data[13].fvalue < 0.15323061)) {
              sum += (float)-0.15151964;
            } else {
              sum += (float)0.041925002;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.065775372)) {
        if (  (data[4].fvalue < 1.0804597)) {
          sum += (float)0.35300505;
        } else {
          sum += (float)0.16374831;
        }
      } else {
        if ( (data[15].fvalue < 0.21161732)) {
          if ( (data[7].fvalue < 9.5)) {
            if (  (data[4].fvalue < 1.0667578)) {
              sum += (float)0.12028908;
            } else {
              sum += (float)0.009298618;
            }
          } else {
            if ( (data[3].fvalue < 1.3936653)) {
              sum += (float)0.22651248;
            } else {
              sum += (float)0.065101787;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7406249)) {
            if ( (data[3].fvalue < 1.6610912)) {
              sum += (float)0.26086044;
            } else {
              sum += (float)0.037422337;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.17894657;
            } else {
              sum += (float)0.019106068;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.5)) {
      if ( (data[4].fvalue < 2.3097453)) {
        if ( (data[17].fvalue < 0.015349269)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.30423349;
            } else {
              sum += (float)-0.077808559;
            }
          } else {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)-0.024570497;
            } else {
              sum += (float)0.25040886;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.8693575)) {
              sum += (float)-0.27327326;
            } else {
              sum += (float)-0.16380659;
            }
          } else {
            if ( (data[4].fvalue < 1.501411)) {
              sum += (float)-0.20464881;
            } else {
              sum += (float)-0.059984148;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.41675752)) {
          if ( (data[15].fvalue < 0.45538929)) {
            if ( (data[13].fvalue < 0.00014154564)) {
              sum += (float)0.19018665;
            } else {
              sum += (float)-0.022910742;
            }
          } else {
            if ( (data[14].fvalue < 0.14571592)) {
              sum += (float)-0.15530969;
            } else {
              sum += (float)-0.051860739;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[16].fvalue < 1.0023453)) {
              sum += (float)0.042044763;
            } else {
              sum += (float)0.16585577;
            }
          } else {
            if ( (data[13].fvalue < 1.2231023)) {
              sum += (float)-0.15408161;
            } else {
              sum += (float)-5.9408154e-05;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.0209185)) {
        if ( (data[9].fvalue < 2.5)) {
          if ( (data[16].fvalue < 0.427899)) {
            if ( (data[13].fvalue < 1.0540687)) {
              sum += (float)0.046604175;
            } else {
              sum += (float)-0.10301284;
            }
          } else {
            if ( (data[15].fvalue < 3.2229886)) {
              sum += (float)0.086031362;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.8497007)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.27415198;
            } else {
              sum += (float)0.11826501;
            }
          } else {
            sum += (float)-0.053873014;
          }
        }
      } else {
        if ( (data[11].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.0041378113)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.11773036;
            } else {
              sum += (float)0.065540761;
            }
          } else {
            if ( (data[14].fvalue < 0.0033712324)) {
              sum += (float)-0.061598841;
            } else {
              sum += (float)0.042080447;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.8980644)) {
            sum += (float)-0.048375733;
          } else {
            sum += (float)-0.24024656;
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.5261884)) {
    if ( (data[5].fvalue < 0.20798823)) {
      if ( (data[13].fvalue < 1.3965864)) {
        if ( (data[13].fvalue < 0.075610831)) {
          if ( (data[14].fvalue < 0.035621725)) {
            if ( (data[17].fvalue < 0.83037245)) {
              sum += (float)-0.028522031;
            } else {
              sum += (float)-0.23336835;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.070842467;
            } else {
              sum += (float)0.045423597;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0061119343)) {
              sum += (float)0.02814116;
            } else {
              sum += (float)0.097931176;
            }
          } else {
            if ( (data[14].fvalue < 0.12551995)) {
              sum += (float)-0.07991349;
            } else {
              sum += (float)0.036991429;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.8310616)) {
          if ( (data[17].fvalue < 0.38564062)) {
            if ( (data[16].fvalue < 0.42645442)) {
              sum += (float)-0.18621621;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[15].fvalue < 0.058081008)) {
              sum += (float)0.074246109;
            } else {
              sum += (float)-0.13169236;
            }
          }
        } else {
          if ( (data[13].fvalue < 2.1057434)) {
            if ( (data[3].fvalue < 0.94918573)) {
              sum += (float)0.068910159;
            } else {
              sum += (float)0.18902327;
            }
          } else {
            if ( (data[16].fvalue < 0.29948729)) {
              sum += (float)-0.041784927;
            } else {
              sum += (float)0.089757845;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.035197217)) {
        if ( (data[13].fvalue < 1.105268e-06)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.39658299;
            } else {
              sum += (float)-0.16017748;
            }
          } else {
            sum += (float)0.5154534;
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)0.0043646512;
            } else {
              sum += (float)-0.15238056;
            }
          } else {
            if ( (data[16].fvalue < 0.62284386)) {
              sum += (float)-0.11045908;
            } else {
              sum += (float)0.065907657;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.14810063)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.012507116)) {
              sum += (float)-0.11906951;
            } else {
              sum += (float)-0.039108764;
            }
          } else {
            if ( (data[3].fvalue < 0.49471399)) {
              sum += (float)0.027764738;
            } else {
              sum += (float)-0.20018987;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0080367792)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.065242566;
            } else {
              sum += (float)0.050706435;
            }
          } else {
            if ( (data[13].fvalue < 0.02038886)) {
              sum += (float)-0.02510906;
            } else {
              sum += (float)0.04895911;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[14].fvalue < 0.00010489565)) {
        if ( (data[13].fvalue < 0.021258667)) {
          sum += (float)0.26842934;
        } else {
          if ( (data[4].fvalue < 0.83750969)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.21081325;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.075867876)) {
              sum += (float)0.070500359;
            } else {
              sum += (float)-0.17653158;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 0.80096716)) {
          if ( (data[5].fvalue < 1.7354839)) {
            sum += (float)0.018332172;
          } else {
            sum += (float)0.16482803;
          }
        } else {
          if ( (data[13].fvalue < 0.00088397024)) {
            sum += (float)0.017960154;
          } else {
            if ( (data[13].fvalue < 0.01941425)) {
              sum += (float)-0.22201741;
            } else {
              sum += (float)-0.015964663;
            }
          }
        }
      }
    } else {
      sum += (float)0.31557241;
    }
  }
  if ( (data[12].fvalue < 3.5)) {
    if ( (data[15].fvalue < 4.2444382)) {
      if ( (data[14].fvalue < 0.29749227)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.045924798)) {
            if ( (data[13].fvalue < 1.4104437e-06)) {
              sum += (float)0.17597906;
            } else {
              sum += (float)0.01202828;
            }
          } else {
            if ( (data[13].fvalue < 0.099019289)) {
              sum += (float)-0.066689014;
            } else {
              sum += (float)-0.00068951002;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.043123893)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.17396277;
            } else {
              sum += (float)0.13369744;
            }
          } else {
            if ( (data[3].fvalue < 0.4438538)) {
              sum += (float)0.019140473;
            } else {
              sum += (float)-0.049038496;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 1.4048277)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.83433455)) {
              sum += (float)0.030075368;
            } else {
              sum += (float)0.11421976;
            }
          } else {
            if ( (data[17].fvalue < 0.98743367)) {
              sum += (float)-0.09842921;
            } else {
              sum += (float)0.026531877;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.90088558)) {
            if ( (data[13].fvalue < 0.4634718)) {
              sum += (float)-0.11440653;
            } else {
              sum += (float)0.1026005;
            }
          } else {
            if ( (data[15].fvalue < 1.6756978)) {
              sum += (float)0.036317743;
            } else {
              sum += (float)0.18736561;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.64214444)) {
        if ( (data[13].fvalue < 0.34955686)) {
          sum += (float)-0.24786009;
        } else {
          sum += (float)-0.044130422;
        }
      } else {
        sum += (float)0.033213548;
      }
    }
  } else {
    sum += (float)0.30491155;
  }
  if ( (data[13].fvalue < 3.0713181)) {
    if ( (data[7].fvalue < 10.5)) {
      if ( (data[5].fvalue < 0.48509461)) {
        if ( (data[14].fvalue < 0.011253109)) {
          if ( (data[14].fvalue < 3.2678938e-07)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.020666968;
            } else {
              sum += (float)0.057114191;
            }
          } else {
            if ( (data[13].fvalue < 0.017341629)) {
              sum += (float)-0.24307026;
            } else {
              sum += (float)-0.053044867;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.032548811)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.10369342;
            } else {
              sum += (float)0.0039781285;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.090396956;
            } else {
              sum += (float)0.0032496992;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.023985613)) {
          if ( (data[14].fvalue < 1.0229714e-06)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.0071714227;
            } else {
              sum += (float)0.46336576;
            }
          } else {
            if ( (data[14].fvalue < 0.016836267)) {
              sum += (float)-0.15730289;
            } else {
              sum += (float)-0.0070371367;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.12163385)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.071230769;
            } else {
              sum += (float)-0.21995836;
            }
          } else {
            if ( (data[15].fvalue < 2.817811)) {
              sum += (float)-0.025301553;
            } else {
              sum += (float)-0.11039337;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.3888815)) {
        sum += (float)0.2436952;
      } else {
        if ( (data[17].fvalue < 0.014004312)) {
          sum += (float)0.15003087;
        } else {
          sum += (float)-0;
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.31866306)) {
      if ( (data[4].fvalue < 1.9658117)) {
        sum += (float)-0.047729224;
      } else {
        if ( (data[16].fvalue < 0.35080555)) {
          if ( (data[15].fvalue < 0.047962707)) {
            sum += (float)-0.13286686;
          } else {
            if ( (data[15].fvalue < 0.39018482)) {
              sum += (float)-0.2852954;
            } else {
              sum += (float)-0.13733162;
            }
          }
        } else {
          sum += (float)-0.067512743;
        }
      }
    } else {
      if (  (data[4].fvalue < 2.9672191)) {
        if ( (data[17].fvalue < 0.70964807)) {
          sum += (float)-0.098348767;
        } else {
          sum += (float)-0.015185106;
        }
      } else {
        sum += (float)0.076640815;
      }
    }
  }
  if ( (data[12].fvalue < 2.5)) {
    if ( (data[17].fvalue < 4.258688)) {
      if ( (data[5].fvalue < 1.21494)) {
        if ( (data[5].fvalue < 0.6426366)) {
          if ( (data[17].fvalue < 0.68265337)) {
            if ( (data[17].fvalue < 0.033744529)) {
              sum += (float)0.010000386;
            } else {
              sum += (float)-0.028387576;
            }
          } else {
            if ( (data[13].fvalue < 0.51199216)) {
              sum += (float)-1.1547427e-05;
            } else {
              sum += (float)0.083199963;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.013330676)) {
            if ( (data[15].fvalue < 0.0303239)) {
              sum += (float)-0.0067976378;
            } else {
              sum += (float)-0.045425154;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.04212907;
            } else {
              sum += (float)-0.20654528;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.20701215;
          } else {
            if ( (data[13].fvalue < 1.3858262e-05)) {
              sum += (float)0.36925048;
            } else {
              sum += (float)0.030469451;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.0509553)) {
            sum += (float)0.34130889;
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[9].fvalue < 0.5)) {
          sum += (float)-0.33727714;
        } else {
          sum += (float)-0.18652616;
        }
      } else {
        if ( (data[15].fvalue < 0.03708867)) {
          sum += (float)0.042545304;
        } else {
          sum += (float)-0.090195239;
        }
      }
    }
  } else {
    sum += (float)0.26514506;
  }
  if ( (data[5].fvalue < 0.049403831)) {
    if (  (data[4].fvalue < 1.848645)) {
      if (  (data[4].fvalue < 1.4498918)) {
        if ( (data[17].fvalue < 0.12432323)) {
          if ( (data[9].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.1533968)) {
              sum += (float)0.077455804;
            } else {
              sum += (float)-0.057136714;
            }
          } else {
            if ( (data[14].fvalue < 0.0033369383)) {
              sum += (float)-0.002704011;
            } else {
              sum += (float)0.20455262;
            }
          }
        } else {
          sum += (float)0.21926464;
        }
      } else {
        if ( (data[5].fvalue < 0.021886986)) {
          if ( (data[15].fvalue < 0.57910031)) {
            if ( (data[13].fvalue < 0.41205674)) {
              sum += (float)0.29945228;
            } else {
              sum += (float)0.13621634;
            }
          } else {
            sum += (float)0.089920878;
          }
        } else {
          if ( (data[2].fvalue < 0.12064362)) {
            if (  (data[4].fvalue < 1.7130597)) {
              sum += (float)0.11367694;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.080181144)) {
              sum += (float)0.10543977;
            } else {
              sum += (float)0.23729436;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.41278398)) {
        if ( (data[15].fvalue < 0.47227877)) {
          if ( (data[3].fvalue < 0.10699923)) {
            sum += (float)-0.19598408;
          } else {
            sum += (float)-0.078333169;
          }
        } else {
          sum += (float)-0.022232533;
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[1].fvalue < 0.84979528)) {
            if ( (data[0].fvalue < 4.5075307)) {
              sum += (float)0.12176306;
            } else {
              sum += (float)0.014358443;
            }
          } else {
            if (  (data[4].fvalue < 2.296535)) {
              sum += (float)-0.042560879;
            } else {
              sum += (float)0.024233053;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.32902813)) {
            sum += (float)-0.16168825;
          } else {
            sum += (float)-0.020384921;
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[16].fvalue < 0.0074386345)) {
        if ( (data[12].fvalue < 1.5)) {
          if (  (data[4].fvalue < 2.1711841)) {
            if ( (data[15].fvalue < 1.8306136)) {
              sum += (float)0.0010555359;
            } else {
              sum += (float)-0.11741354;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.096598446;
            } else {
              sum += (float)0.0093053281;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7121912)) {
            if ( (data[13].fvalue < 0.69570959)) {
              sum += (float)0.066725917;
            } else {
              sum += (float)-0.19223003;
            }
          } else {
            if (  (data[4].fvalue < 2.3673198)) {
              sum += (float)-0.26677671;
            } else {
              sum += (float)-0.0091999834;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.13041982)) {
          if ( (data[4].fvalue < 1.3456414)) {
            if ( (data[13].fvalue < 0.27955371)) {
              sum += (float)-0.2669808;
            } else {
              sum += (float)-0.066729598;
            }
          } else {
            if ( (data[16].fvalue < 0.049667835)) {
              sum += (float)-0.009061425;
            } else {
              sum += (float)-0.079459719;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0081092594)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.23058382;
            } else {
              sum += (float)-0.038916387;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.030182187;
            } else {
              sum += (float)-0.042375986;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.20310995)) {
        sum += (float)0.23280634;
      } else {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[13].fvalue < 0.79680252)) {
            sum += (float)0.018587729;
          } else {
            sum += (float)0.23122473;
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[7].fvalue < 12.5)) {
              sum += (float)0.017439378;
            } else {
              sum += (float)0.18057324;
            }
          } else {
            sum += (float)-0.13148046;
          }
        }
      }
    }
  }
  if ( (data[17].fvalue < 0.028271861)) {
    if ( (data[14].fvalue < 0.38106894)) {
      if ( (data[15].fvalue < 2.255465)) {
        if ( (data[8].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.045234)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.42856634;
            } else {
              sum += (float)-0.0046282825;
            }
          } else {
            if ( (data[15].fvalue < 0.041565858)) {
              sum += (float)0.052383084;
            } else {
              sum += (float)0.0017443239;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.31931764)) {
            if (  (data[4].fvalue < 1.1489251)) {
              sum += (float)0.11255239;
            } else {
              sum += (float)-0.029140519;
            }
          } else {
            if ( (data[14].fvalue < 0.1118869)) {
              sum += (float)-0.13848718;
            } else {
              sum += (float)-0.018202348;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.31618237)) {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[5].fvalue < 0.13722968)) {
              sum += (float)-0.080955982;
            } else {
              sum += (float)-0.27861974;
            }
          } else {
            if ( (data[16].fvalue < 0.051726982)) {
              sum += (float)0.058076423;
            } else {
              sum += (float)-0.11803049;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.79453844)) {
            if ( (data[3].fvalue < 1.8513391)) {
              sum += (float)0.08923059;
            } else {
              sum += (float)-0.039470814;
            }
          } else {
            if (  (data[4].fvalue < 2.7054658)) {
              sum += (float)0.2385781;
            } else {
              sum += (float)0.09313041;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.38534802)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.62561864)) {
              sum += (float)-0.031939641;
            } else {
              sum += (float)0.084488131;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.17240515;
            } else {
              sum += (float)0.048298035;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.076794922)) {
            if ( (data[3].fvalue < 0.75521612)) {
              sum += (float)-0.13400383;
            } else {
              sum += (float)-0.34290066;
            }
          } else {
            if ( (data[14].fvalue < 1.0758095)) {
              sum += (float)0.022440206;
            } else {
              sum += (float)-0.12655474;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.60782325)) {
            if ( (data[3].fvalue < 1.757153)) {
              sum += (float)0.13187143;
            } else {
              sum += (float)-0.0014312515;
            }
          } else {
            if ( (data[13].fvalue < 0.57597095)) {
              sum += (float)0.099099971;
            } else {
              sum += (float)0.20946036;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.42816377)) {
            sum += (float)-0.14905295;
          } else {
            if ( (data[3].fvalue < 2.6993477)) {
              sum += (float)0.084070846;
            } else {
              sum += (float)-0.053761411;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.136237)) {
      if ( (data[15].fvalue < 1.0326779)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.66028845)) {
              sum += (float)-0.069144979;
            } else {
              sum += (float)-0.20389023;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.066085666;
            } else {
              sum += (float)-0.073093332;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.35130221)) {
            if ( (data[16].fvalue < 0.16351408)) {
              sum += (float)-0.11677451;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[17].fvalue < 0.10833011)) {
              sum += (float)-0.23283994;
            } else {
              sum += (float)-0.091339357;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[17].fvalue < 0.068203002)) {
              sum += (float)0.15789005;
            } else {
              sum += (float)0.045454137;
            }
          } else {
            sum += (float)-0.049889375;
          }
        } else {
          if (  (data[4].fvalue < 2.3532155)) {
            sum += (float)0.010861205;
          } else {
            if ( (data[14].fvalue < 0.83470815)) {
              sum += (float)-0.10990099;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.25355738)) {
        if (  (data[4].fvalue < 1.7862988)) {
          if ( (data[13].fvalue < 0.010812365)) {
            if ( (data[14].fvalue < 0.052159697)) {
              sum += (float)-0.20677832;
            } else {
              sum += (float)-0.023591843;
            }
          } else {
            if ( (data[15].fvalue < 0.042805187)) {
              sum += (float)0.19464903;
            } else {
              sum += (float)0.027137289;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.53077281)) {
            if ( (data[15].fvalue < 0.67788684)) {
              sum += (float)-0.13046327;
            } else {
              sum += (float)-0.0016802653;
            }
          } else {
            if ( (data[14].fvalue < 0.0074437782)) {
              sum += (float)-0.055641588;
            } else {
              sum += (float)0.05103682;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0065519116)) {
          if ( (data[13].fvalue < 0.23753276)) {
            if ( (data[17].fvalue < 0.71143836)) {
              sum += (float)-0.0032051655;
            } else {
              sum += (float)-0.18808316;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.027806625;
            } else {
              sum += (float)0.068974927;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0066958284)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.2204798;
            } else {
              sum += (float)-0.003940457;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.071970843;
            } else {
              sum += (float)-0.00018349935;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 2.445909)) {
    if ( (data[12].fvalue < 3.5)) {
      if ( (data[14].fvalue < 0.14988908)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.2126421)) {
            if ( (data[17].fvalue < 1.473022)) {
              sum += (float)-0.0092526553;
            } else {
              sum += (float)-0.19277067;
            }
          } else {
            if ( (data[15].fvalue < 0.9186008)) {
              sum += (float)0.006118489;
            } else {
              sum += (float)0.084739812;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.12322526)) {
            if (  (data[4].fvalue < 1.1204076)) {
              sum += (float)-0.0026808942;
            } else {
              sum += (float)-0.11207609;
            }
          } else {
            if ( (data[13].fvalue < 0.29653969)) {
              sum += (float)0.10785585;
            } else {
              sum += (float)-0.07062269;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 1.6625853)) {
          if ( (data[14].fvalue < 0.97452807)) {
            if (  (data[4].fvalue < 2.9285784)) {
              sum += (float)0.032564942;
            } else {
              sum += (float)-0.055887934;
            }
          } else {
            if ( (data[15].fvalue < 0.56767744)) {
              sum += (float)-0.096762277;
            } else {
              sum += (float)0.063751496;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.58618879)) {
            if ( (data[13].fvalue < 0.018569924)) {
              sum += (float)-0.089404389;
            } else {
              sum += (float)-0.019467037;
            }
          } else {
            if ( (data[15].fvalue < 1.2679876)) {
              sum += (float)-0.0118315;
            } else {
              sum += (float)0.073450603;
            }
          }
        }
      }
    } else {
      sum += (float)0.26218563;
    }
  } else {
    if ( (data[4].fvalue < 2.8151746)) {
      if ( (data[15].fvalue < 0.030142395)) {
        if ( (data[16].fvalue < 0.03246925)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[0].fvalue < 6.6735992)) {
              sum += (float)0.022699786;
            } else {
              sum += (float)-0.05096354;
            }
          } else {
            sum += (float)-0.0960088;
          }
        } else {
          sum += (float)-0.14067437;
        }
      } else {
        if ( (data[15].fvalue < 0.54801238)) {
          if ( (data[16].fvalue < 0.32674265)) {
            if (  (data[4].fvalue < 2.1537397)) {
              sum += (float)-0.11033724;
            } else {
              sum += (float)-0.22673783;
            }
          } else {
            sum += (float)-0.090223387;
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            sum += (float)0.012138239;
          } else {
            sum += (float)-0.098976143;
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.31548643)) {
        if ( (data[7].fvalue < 1.5)) {
          sum += (float)-0.13667829;
        } else {
          sum += (float)0.0094614886;
        }
      } else {
        if ( (data[17].fvalue < 0.12937646)) {
          sum += (float)0.0068889838;
        } else {
          sum += (float)0.1248538;
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.1231434)) {
    if ( (data[5].fvalue < 0.46278262)) {
      if ( (data[17].fvalue < 0.56298423)) {
        if ( (data[13].fvalue < 0.96552211)) {
          if ( (data[15].fvalue < 0.14378989)) {
            if (  (data[4].fvalue < 1.1426609)) {
              sum += (float)0.0326727;
            } else {
              sum += (float)-0.016552661;
            }
          } else {
            if ( (data[3].fvalue < 0.26109108)) {
              sum += (float)0.062350936;
            } else {
              sum += (float)0.0045807203;
            }
          }
        } else {
          if ( (data[15].fvalue < 1.4581311)) {
            if ( (data[16].fvalue < 0.90517271)) {
              sum += (float)-0.08385741;
            } else {
              sum += (float)0.10929438;
            }
          } else {
            if ( (data[16].fvalue < 0.028768821)) {
              sum += (float)0.19075158;
            } else {
              sum += (float)0.083092272;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.023983676)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 2.5017157)) {
              sum += (float)0.035344031;
            } else {
              sum += (float)-0.10330463;
            }
          } else {
            if ( (data[15].fvalue < 0.052421223)) {
              sum += (float)0.13446207;
            } else {
              sum += (float)0.022944417;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.98397565)) {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)-0.035941835;
            } else {
              sum += (float)-0.15472114;
            }
          } else {
            if ( (data[16].fvalue < 0.36503875)) {
              sum += (float)-0;
            } else {
              sum += (float)0.089485154;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.3620853)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[14].fvalue < 0.30815712)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.17478506;
            } else {
              sum += (float)-0.0088901808;
            }
          } else {
            if ( (data[13].fvalue < 0.21935496)) {
              sum += (float)0.049615823;
            } else {
              sum += (float)0.13549337;
            }
          }
        } else {
          sum += (float)0.38744459;
        }
      } else {
        if ( (data[13].fvalue < 0.94991195)) {
          if ( (data[14].fvalue < 0.57672817)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.046562806;
            } else {
              sum += (float)-0.12316964;
            }
          } else {
            if ( (data[17].fvalue < 1.4247432)) {
              sum += (float)-0.019421794;
            } else {
              sum += (float)0.10962248;
            }
          }
        } else {
          if ( (data[15].fvalue < 1.2052231)) {
            if ( (data[15].fvalue < 0.0092788637)) {
              sum += (float)0.019227125;
            } else {
              sum += (float)-0.043338031;
            }
          } else {
            if ( (data[14].fvalue < 0.042816631)) {
              sum += (float)0.11768938;
            } else {
              sum += (float)0.010688876;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[4].fvalue < 0.70754111)) {
            if ( (data[4].fvalue < 0.57961667)) {
              sum += (float)0.26178572;
            } else {
              sum += (float)0.15263024;
            }
          } else {
            if ( (data[13].fvalue < 0.034252968)) {
              sum += (float)0.1162696;
            } else {
              sum += (float)0.027828068;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.092571035)) {
              sum += (float)0.084404819;
            } else {
              sum += (float)-0.035759825;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.2131341;
            } else {
              sum += (float)-0.0048895874;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.021732815)) {
            sum += (float)-0.28240487;
          } else {
            if ( (data[3].fvalue < 0.85601401)) {
              sum += (float)-0.040052596;
            } else {
              sum += (float)-0.23833765;
            }
          }
        } else {
          sum += (float)0.065387607;
        }
      }
    } else {
      if ( (data[3].fvalue < 1.2681731)) {
        if ( (data[3].fvalue < 0.46499375)) {
          sum += (float)0.35079116;
        } else {
          if ( (data[13].fvalue < 0.057090618)) {
            sum += (float)-0.023180585;
          } else {
            if ( (data[14].fvalue < 0.12470823)) {
              sum += (float)0.13421737;
            } else {
              sum += (float)0.33074427;
            }
          }
        }
      } else {
        sum += (float)-0.11051719;
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[8].fvalue < 6.5)) {
        if ( (data[13].fvalue < 0.28645915)) {
          if ( (data[14].fvalue < 0.10117385)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0084106904;
            } else {
              sum += (float)-0.10964197;
            }
          } else {
            if ( (data[13].fvalue < 0.010391673)) {
              sum += (float)-0.0098848501;
            } else {
              sum += (float)0.026737133;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0031691156)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.011330784;
            } else {
              sum += (float)0.05284993;
            }
          } else {
            if ( (data[14].fvalue < 0.051325619)) {
              sum += (float)0.11230138;
            } else {
              sum += (float)0.011978747;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 7.5)) {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.17474911;
          } else {
            sum += (float)0.067527004;
          }
        } else {
          sum += (float)0.22582941;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1250831)) {
        if ( (data[3].fvalue < 0.23216601)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.145761)) {
              sum += (float)0.068036124;
            } else {
              sum += (float)0.34154886;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.25206152;
            } else {
              sum += (float)0.41539159;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.1007742)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.038848259;
            } else {
              sum += (float)0.13857341;
            }
          } else {
            if ( (data[14].fvalue < 0.13416742)) {
              sum += (float)-0.20205866;
            } else {
              sum += (float)-0.085102737;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.11284465)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.028517924;
            } else {
              sum += (float)-0.14920799;
            }
          } else {
            if ( (data[3].fvalue < 0.37238604)) {
              sum += (float)0.080768123;
            } else {
              sum += (float)-0.019817388;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.24504197)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.15069257;
            } else {
              sum += (float)-0.020650975;
            }
          } else {
            if ( (data[15].fvalue < 0.022381904)) {
              sum += (float)0.048258442;
            } else {
              sum += (float)-0.0015772838;
            }
          }
        }
      }
    }
  } else {
    if ( (data[10].fvalue < 1.5)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[13].fvalue < 0.51311946)) {
          if ( (data[14].fvalue < 0.053464603)) {
            if (  (data[4].fvalue < 1.2208915)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.14749521;
            }
          } else {
            if ( (data[13].fvalue < 0.36868674)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12277762;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.038474709)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.24455717;
            } else {
              sum += (float)-0.040327877;
            }
          } else {
            sum += (float)0.22940831;
          }
        }
      } else {
        if ( (data[6].fvalue < 0.21631266)) {
          if ( (data[4].fvalue < 1.2352128)) {
            if ( (data[3].fvalue < 0.76224828)) {
              sum += (float)0.25472492;
            } else {
              sum += (float)0.078561746;
            }
          } else {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12674366;
            }
          }
        } else {
          if ( (data[0].fvalue < 1.7749221)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.0075327414;
            } else {
              sum += (float)0.17103775;
            }
          } else {
            if ( (data[15].fvalue < 0.56067336)) {
              sum += (float)-0.032016374;
            } else {
              sum += (float)0.063387059;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[2].fvalue < 0.50722432)) {
          sum += (float)-0.099810109;
        } else {
          sum += (float)-0.20795929;
        }
      } else {
        sum += (float)-0.041107789;
      }
    }
  }
  if ( (data[15].fvalue < 3.8481243)) {
    if ( (data[11].fvalue < 3.5)) {
      if ( (data[7].fvalue < 4.5)) {
        if (  (data[4].fvalue < 2.1146126)) {
          if ( (data[11].fvalue < 2.5)) {
            if ( (data[14].fvalue < 1.0880063)) {
              sum += (float)-0.0017504072;
            } else {
              sum += (float)-0.048685495;
            }
          } else {
            if (  (data[4].fvalue < 1.8402524)) {
              sum += (float)-0.24960265;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[13].fvalue < 2.0634162)) {
            if ( (data[13].fvalue < 0.39099196)) {
              sum += (float)0.008607354;
            } else {
              sum += (float)0.047072873;
            }
          } else {
            if (  (data[4].fvalue < 2.6478391)) {
              sum += (float)-0.10513415;
            } else {
              sum += (float)-0.00082709437;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.092210799)) {
          if (  (data[4].fvalue < 1.1532278)) {
            if ( (data[0].fvalue < 4.5304832)) {
              sum += (float)0.32995778;
            } else {
              sum += (float)0.15070632;
            }
          } else {
            sum += (float)0.01804895;
          }
        } else {
          if ( (data[7].fvalue < 12.5)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.03982364;
            } else {
              sum += (float)-0.013123768;
            }
          } else {
            sum += (float)0.20592906;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.32322896)) {
        if ( (data[14].fvalue < 0.0054301918)) {
          sum += (float)-0.15573311;
        } else {
          sum += (float)-0;
        }
      } else {
        if ( (data[13].fvalue < 0.26567024)) {
          sum += (float)-0.089538813;
        } else {
          sum += (float)-0.2677547;
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.85643178)) {
      if ( (data[13].fvalue < 0.61249745)) {
        if ( (data[11].fvalue < 0.5)) {
          if ( (data[5].fvalue < 0.14227548)) {
            sum += (float)-0.071396418;
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.24639688;
            } else {
              sum += (float)-0.11511861;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            sum += (float)-0.0066885841;
          } else {
            sum += (float)-0.15081666;
          }
        }
      } else {
        sum += (float)0.034919363;
      }
    } else {
      sum += (float)0.069248073;
    }
  }
  if ( (data[10].fvalue < 2.5)) {
    if ( (data[8].fvalue < 10.5)) {
      if ( (data[12].fvalue < 2.5)) {
        if ( (data[12].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.025003657)) {
            if ( (data[14].fvalue < 2.7294812)) {
              sum += (float)0.0040646615;
            } else {
              sum += (float)-0.095269211;
            }
          } else {
            if ( (data[16].fvalue < 0.37514904)) {
              sum += (float)-0.039110616;
            } else {
              sum += (float)0.008428622;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7597135)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.017488992;
            } else {
              sum += (float)0.3694635;
            }
          } else {
            if (  (data[4].fvalue < 2.2520638)) {
              sum += (float)-0.24231297;
            } else {
              sum += (float)-0.032803714;
            }
          }
        }
      } else {
        sum += (float)0.21915916;
      }
    } else {
      if ( (data[13].fvalue < 0.58651942)) {
        if ( (data[14].fvalue < 0.54161656)) {
          sum += (float)0.27250189;
        } else {
          sum += (float)0.10284527;
        }
      } else {
        sum += (float)-0.0046764384;
      }
    }
  } else {
    sum += (float)-0.18655357;
  }
  if ( (data[14].fvalue < 0.032367606)) {
    if ( (data[13].fvalue < 1.3340787e-06)) {
      if ( (data[3].fvalue < 0.99547577)) {
        if ( (data[7].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.6003931)) {
            if ( (data[5].fvalue < 0.1767195)) {
              sum += (float)-0.10354482;
            } else {
              sum += (float)-0.34215283;
            }
          } else {
            if ( (data[15].fvalue < 0.55013382)) {
              sum += (float)0.29179236;
            } else {
              sum += (float)-0.042717595;
            }
          }
        } else {
          sum += (float)0.40536767;
        }
      } else {
        if ( (data[15].fvalue < 1.7240459)) {
          if ( (data[17].fvalue < 0.83092082)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.42003977;
            } else {
              sum += (float)0.61249232;
            }
          } else {
            sum += (float)0.18226646;
          }
        } else {
          sum += (float)-0.045209747;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.16846953)) {
        if ( (data[3].fvalue < 0.690817)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.84032869)) {
              sum += (float)-0.042080507;
            } else {
              sum += (float)0.016736023;
            }
          } else {
            if ( (data[3].fvalue < 0.14158759)) {
              sum += (float)0.33583057;
            } else {
              sum += (float)-0.014094296;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.91322899)) {
              sum += (float)-0.11159322;
            } else {
              sum += (float)-0.26870933;
            }
          } else {
            if ( (data[17].fvalue < 1.8024454)) {
              sum += (float)0.22667579;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.0098552108)) {
            if ( (data[14].fvalue < 3.3451855e-07)) {
              sum += (float)-7.2715135e-05;
            } else {
              sum += (float)-0.084800884;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.17973393;
            } else {
              sum += (float)-0.014642012;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.158463)) {
            if ( (data[3].fvalue < 0.42079067)) {
              sum += (float)0.23833691;
            } else {
              sum += (float)-0.16383646;
            }
          } else {
            if ( (data[3].fvalue < 0.26226169)) {
              sum += (float)-0.15703055;
            } else {
              sum += (float)-0.25093207;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.20786542)) {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)-0.35937381;
          } else {
            sum += (float)-0.097739354;
          }
        } else {
          if ( (data[13].fvalue < 1.105268e-06)) {
            sum += (float)0.44577122;
          } else {
            if (  (data[4].fvalue < 1.1437116)) {
              sum += (float)0.094842143;
            } else {
              sum += (float)-0.015482956;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.735455)) {
          if ( (data[15].fvalue < 0.13323954)) {
            if ( (data[15].fvalue < 0.036987476)) {
              sum += (float)0.23217225;
            } else {
              sum += (float)0.02762497;
            }
          } else {
            if ( (data[13].fvalue < 0.0056895767)) {
              sum += (float)0.10461568;
            } else {
              sum += (float)0.20005053;
            }
          }
        } else {
          if ( (data[1].fvalue < 0.9164716)) {
            sum += (float)0.061236367;
          } else {
            if ( (data[15].fvalue < 0.65762961)) {
              sum += (float)-0.087967589;
            } else {
              sum += (float)0.0059352056;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.064817734)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.042243775;
            } else {
              sum += (float)0.087225333;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.10575256;
            } else {
              sum += (float)-0.06940823;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.032965481)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.29696533;
            } else {
              sum += (float)-0.11717417;
            }
          } else {
            if ( (data[14].fvalue < 0.30050039)) {
              sum += (float)-0.037879497;
            } else {
              sum += (float)-0.19939297;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.46223873)) {
              sum += (float)-0.011281046;
            } else {
              sum += (float)0.051131077;
            }
          } else {
            if ( (data[15].fvalue < 0.043926843)) {
              sum += (float)0.16553678;
            } else {
              sum += (float)0.038154308;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.21362323)) {
            if ( (data[13].fvalue < 0.23323211)) {
              sum += (float)0.046339236;
            } else {
              sum += (float)-0.12519316;
            }
          } else {
            if ( (data[3].fvalue < 2.1996551)) {
              sum += (float)0.019983634;
            } else {
              sum += (float)-0.038561255;
            }
          }
        }
      }
    }
  }
  if ( (data[15].fvalue < 0.37569049)) {
    if ( (data[15].fvalue < 0.047975205)) {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[14].fvalue < 5.1353271e-07)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.036028307)) {
              sum += (float)0.15040995;
            } else {
              sum += (float)-0.0036628859;
            }
          } else {
            sum += (float)0.42178527;
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.30063251;
            } else {
              sum += (float)0.19711451;
            }
          } else {
            if ( (data[13].fvalue < 8.5506315e-07)) {
              sum += (float)0.38345256;
            } else {
              sum += (float)-0.015555819;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.01105745)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[5].fvalue < 0.58438599)) {
              sum += (float)0.27955982;
            } else {
              sum += (float)0.12480478;
            }
          } else {
            sum += (float)0.075519502;
          }
        } else {
          if ( (data[4].fvalue < 1.1354337)) {
            if ( (data[13].fvalue < 0.25014722)) {
              sum += (float)-0.11261985;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[14].fvalue < 0.0071197869)) {
              sum += (float)0.0020361044;
            } else {
              sum += (float)0.080853999;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.5940597)) {
        if ( (data[15].fvalue < 0.11360174)) {
          if ( (data[13].fvalue < 0.1431987)) {
            if ( (data[14].fvalue < 0.038258903)) {
              sum += (float)-0.16382368;
            } else {
              sum += (float)-0.076576836;
            }
          } else {
            if ( (data[3].fvalue < 0.08990854)) {
              sum += (float)0.093172699;
            } else {
              sum += (float)-0.034351688;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0039865887)) {
            if ( (data[5].fvalue < 0.29309088)) {
              sum += (float)0.040549025;
            } else {
              sum += (float)-0.038282573;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.07279285;
            } else {
              sum += (float)-0.070614815;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.81039947)) {
          if ( (data[14].fvalue < 0.70582414)) {
            if ( (data[13].fvalue < 0.10438852)) {
              sum += (float)0.048231699;
            } else {
              sum += (float)-0.033391435;
            }
          } else {
            if ( (data[17].fvalue < 0.99586749)) {
              sum += (float)-0.12435131;
            } else {
              sum += (float)0.073396474;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.91255736)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.16387947;
            } else {
              sum += (float)-0.04865551;
            }
          } else {
            if ( (data[16].fvalue < 1.7074518)) {
              sum += (float)0.0082902657;
            } else {
              sum += (float)0.11719308;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.51715875)) {
      if ( (data[13].fvalue < 0.018802021)) {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[15].fvalue < 1.9060923)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.050927546;
            } else {
              sum += (float)0.068601757;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.0058404761;
            } else {
              sum += (float)-0.1809537;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.6423407)) {
            if ( (data[14].fvalue < 0.43465936)) {
              sum += (float)0.11388317;
            } else {
              sum += (float)0.028109571;
            }
          } else {
            if ( (data[16].fvalue < 0.040346548)) {
              sum += (float)0.020174267;
            } else {
              sum += (float)-0.11340805;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.033480011)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.034612969;
            } else {
              sum += (float)-0.026120886;
            }
          } else {
            if ( (data[13].fvalue < 0.36425376)) {
              sum += (float)0.17985933;
            } else {
              sum += (float)0.0096326321;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.36554575)) {
              sum += (float)-0.055301894;
            } else {
              sum += (float)0.012698004;
            }
          } else {
            if (  (data[4].fvalue < 2.7093146)) {
              sum += (float)0.044105809;
            } else {
              sum += (float)-0.037496045;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[5].fvalue < 0.49673492)) {
          if ( (data[15].fvalue < 0.86542022)) {
            if ( (data[17].fvalue < 0.045850206)) {
              sum += (float)0.067385487;
            } else {
              sum += (float)-0.010355397;
            }
          } else {
            if ( (data[16].fvalue < 0.018688772)) {
              sum += (float)0.18073955;
            } else {
              sum += (float)-0.015930744;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.7920184)) {
              sum += (float)-0.045455568;
            } else {
              sum += (float)0.03504594;
            }
          } else {
            if ( (data[6].fvalue < 0.79108757)) {
              sum += (float)0.079263031;
            } else {
              sum += (float)-0.035667855;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.99298596)) {
          if ( (data[16].fvalue < 0.1082209)) {
            if ( (data[4].fvalue < 1.9676654)) {
              sum += (float)-0.0021084356;
            } else {
              sum += (float)-0.12467;
            }
          } else {
            if (  (data[4].fvalue < 2.2356722)) {
              sum += (float)0.066833384;
            } else {
              sum += (float)-0.020899072;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.1342897)) {
            if (  (data[4].fvalue < 2.0113916)) {
              sum += (float)0.16704422;
            } else {
              sum += (float)0.065133967;
            }
          } else {
            if ( (data[16].fvalue < 0.012276603)) {
              sum += (float)0.045218028;
            } else {
              sum += (float)-0.015193922;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.12607454)) {
    if ( (data[7].fvalue < 3.5)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.15709043)) {
          if ( (data[17].fvalue < 1.2835267)) {
            if ( (data[3].fvalue < 2.9739928)) {
              sum += (float)-0.017897705;
            } else {
              sum += (float)0.31413227;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.21169975;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.015440911;
            } else {
              sum += (float)0.053595502;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.08279638;
            } else {
              sum += (float)0.016414806;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.29067022)) {
          if (  (data[4].fvalue < 0.98366976)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.02521465;
            } else {
              sum += (float)0.29105088;
            }
          } else {
            if ( (data[13].fvalue < 0.05329515)) {
              sum += (float)-0.10406079;
            } else {
              sum += (float)0.012376822;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.042181876)) {
            if ( (data[5].fvalue < 1.2676985)) {
              sum += (float)-0.22927694;
            } else {
              sum += (float)-0.08257255;
            }
          } else {
            if ( (data[17].fvalue < 0.12731072)) {
              sum += (float)-0.082224116;
            } else {
              sum += (float)0.00051290536;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.14907101)) {
        if ( (data[17].fvalue < 0.1026983)) {
          if ( (data[15].fvalue < 0.091884941)) {
            if (  (data[4].fvalue < 1.0429037)) {
              sum += (float)0.18513584;
            } else {
              sum += (float)-0.089520447;
            }
          } else {
            sum += (float)0.26360506;
          }
        } else {
          sum += (float)0.3504189;
        }
      } else {
        if ( (data[14].fvalue < 0.57233357)) {
          if ( (data[8].fvalue < 3.5)) {
            sum += (float)-0.15494977;
          } else {
            if ( (data[15].fvalue < 0.10938239)) {
              sum += (float)-0.025999326;
            } else {
              sum += (float)0.10202299;
            }
          }
        } else {
          sum += (float)-0.2700288;
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.4547162)) {
      if ( (data[13].fvalue < 1.1801457)) {
        if (  (data[4].fvalue < 1.0244056)) {
          if ( (data[3].fvalue < 0.34674302)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.14268258;
            } else {
              sum += (float)0.15899685;
            }
          } else {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.0014942981;
            } else {
              sum += (float)0.18963742;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if (  (data[4].fvalue < 1.5000896)) {
              sum += (float)0.04234118;
            } else {
              sum += (float)0.0080617219;
            }
          } else {
            if ( (data[13].fvalue < 0.60481787)) {
              sum += (float)-0.015338898;
            } else {
              sum += (float)0.048872922;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.45154068)) {
          if (  (data[4].fvalue < 2.6159573)) {
            if ( (data[15].fvalue < 0.056570686)) {
              sum += (float)-0.01842415;
            } else {
              sum += (float)-0.15233159;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.045055211;
            } else {
              sum += (float)-0.072157204;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.045352429)) {
            sum += (float)0.18104334;
          } else {
            sum += (float)0.050608445;
          }
        }
      }
    } else {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.58083284)) {
          if ( (data[14].fvalue < 0.0079443362)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.093453854;
            } else {
              sum += (float)0.058573235;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.039914686;
            } else {
              sum += (float)-0.032053515;
            }
          }
        } else {
          if ( (data[17].fvalue < 1.5732454)) {
            if ( (data[14].fvalue < 0.68765223)) {
              sum += (float)-0.0051139151;
            } else {
              sum += (float)0.098947756;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.11138;
            } else {
              sum += (float)-0.085571133;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.677639)) {
          if (  (data[4].fvalue < 1.5218375)) {
            sum += (float)0.038319387;
          } else {
            if ( (data[15].fvalue < 0.12926306)) {
              sum += (float)-0.23816097;
            } else {
              sum += (float)-0.11321087;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.0269048)) {
            sum += (float)-0.12174479;
          } else {
            sum += (float)0.11816732;
          }
        }
      }
    }
  }
  if ( (data[17].fvalue < 4.7838478)) {
    if ( (data[5].fvalue < 0.035851192)) {
      if (  (data[4].fvalue < 1.8670019)) {
        if (  (data[4].fvalue < 1.450624)) {
          if ( (data[17].fvalue < 0.0086096469)) {
            if ( (data[15].fvalue < 0.33117729)) {
              sum += (float)0.026358435;
            } else {
              sum += (float)-0.10099036;
            }
          } else {
            sum += (float)0.14548425;
          }
        } else {
          if ( (data[6].fvalue < 0.39449239)) {
            if ( (data[3].fvalue < 0.047269195)) {
              sum += (float)0.12961203;
            } else {
              sum += (float)0.01866667;
            }
          } else {
            sum += (float)0.19065505;
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[17].fvalue < 0.063775122)) {
            if (  (data[4].fvalue < 2.1019213)) {
              sum += (float)-0.039003812;
            } else {
              sum += (float)0.060022447;
            }
          } else {
            if ( (data[3].fvalue < 0.12344623)) {
              sum += (float)-0.085345723;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)-0.11208149;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.9855199)) {
        if ( (data[11].fvalue < 2.5)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.008777895;
            } else {
              sum += (float)0.05763958;
            }
          } else {
            if ( (data[6].fvalue < 0.071682401)) {
              sum += (float)0.20255797;
            } else {
              sum += (float)0.022346908;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0058063958)) {
            if ( (data[17].fvalue < 0.95266223)) {
              sum += (float)-0.29694691;
            } else {
              sum += (float)-0.096085556;
            }
          } else {
            if ( (data[5].fvalue < 0.49612325)) {
              sum += (float)0.0107192;
            } else {
              sum += (float)-0.14745168;
            }
          }
        }
      } else {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[15].fvalue < 0.059674069)) {
            if ( (data[13].fvalue < 1.2248605e-05)) {
              sum += (float)0.17073122;
            } else {
              sum += (float)0.033630271;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.01429698;
            } else {
              sum += (float)-0.027929112;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.5431843)) {
            if ( (data[15].fvalue < 0.032504745)) {
              sum += (float)-0.25758129;
            } else {
              sum += (float)-0.06119867;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.11211471;
            } else {
              sum += (float)-0.033526931;
            }
          }
        }
      }
    }
  } else {
    sum += (float)-0.16104336;
  }
  if ( (data[4].fvalue < 0.85264659)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.17583272)) {
          if ( (data[3].fvalue < 0.51579142)) {
            if ( (data[13].fvalue < 0.097598985)) {
              sum += (float)0.11703884;
            } else {
              sum += (float)-0.061398163;
            }
          } else {
            if ( (data[14].fvalue < 0.082053587)) {
              sum += (float)0.27521345;
            } else {
              sum += (float)0.094598532;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.66454995)) {
            sum += (float)-0.20305862;
          } else {
            if ( (data[13].fvalue < 0.22654173)) {
              sum += (float)0.22339915;
            } else {
              sum += (float)0.054568451;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.24212824)) {
          sum += (float)-0.13468054;
        } else {
          sum += (float)-0.013437138;
        }
      }
    } else {
      if ( (data[5].fvalue < 1.4617739)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[3].fvalue < 1.503932)) {
            sum += (float)-0.064659044;
          } else {
            sum += (float)0.11424322;
          }
        } else {
          if ( (data[14].fvalue < 0.16438496)) {
            if ( (data[3].fvalue < 1.1708546)) {
              sum += (float)-0.086163819;
            } else {
              sum += (float)-0.28791818;
            }
          } else {
            if ( (data[3].fvalue < 2.0054815)) {
              sum += (float)0.059297305;
            } else {
              sum += (float)-0.16130589;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[5].fvalue < 1.7943342)) {
            sum += (float)-0.01981953;
          } else {
            sum += (float)0.06184965;
          }
        } else {
          sum += (float)0.20161535;
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.0317178)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[3].fvalue < 0.63679242)) {
            sum += (float)-0.3703416;
          } else {
            sum += (float)0.039725538;
          }
        } else {
          if ( (data[13].fvalue < 1.0350003e-06)) {
            sum += (float)0.35886249;
          } else {
            if ( (data[14].fvalue < 4.0385171e-06)) {
              sum += (float)-0.0073990882;
            } else {
              sum += (float)-0.070863821;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.74758017)) {
          if ( (data[13].fvalue < 0.032236874)) {
            if ( (data[3].fvalue < 0.15093385)) {
              sum += (float)0.28633046;
            } else {
              sum += (float)-0.048788037;
            }
          } else {
            if ( (data[14].fvalue < 0.095689245)) {
              sum += (float)0.16239645;
            } else {
              sum += (float)0.31165862;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.21693633)) {
            if ( (data[13].fvalue < 0.27386665)) {
              sum += (float)-0.311378;
            } else {
              sum += (float)-0.081628777;
            }
          } else {
            if ( (data[3].fvalue < 1.1728269)) {
              sum += (float)0.094013408;
            } else {
              sum += (float)-0.085481122;
            }
          }
        }
      }
    } else {
      if ( (data[0].fvalue < 1.6734006)) {
        if ( (data[13].fvalue < 0.038184144)) {
          if ( (data[14].fvalue < 0.092358835)) {
            if ( (data[5].fvalue < 0.18658157)) {
              sum += (float)-0.025639558;
            } else {
              sum += (float)-0.18715487;
            }
          } else {
            if ( (data[15].fvalue < 0.14919165)) {
              sum += (float)-0.010673532;
            } else {
              sum += (float)0.085993275;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.13627034)) {
            if ( (data[17].fvalue < 0.12384062)) {
              sum += (float)0.018091308;
            } else {
              sum += (float)0.092101775;
            }
          } else {
            if ( (data[5].fvalue < 0.16029343)) {
              sum += (float)0.2043073;
            } else {
              sum += (float)0.058149334;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1651514)) {
          if ( (data[3].fvalue < 0.37880367)) {
            if ( (data[13].fvalue < 0.070703253)) {
              sum += (float)-0.036274862;
            } else {
              sum += (float)0.087307982;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.032627866;
            } else {
              sum += (float)-0.040362976;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.39831194)) {
            if ( (data[14].fvalue < 1.9158487)) {
              sum += (float)-0.0081014046;
            } else {
              sum += (float)-0.086086541;
            }
          } else {
            if ( (data[16].fvalue < 2.4268112)) {
              sum += (float)0.0098381443;
            } else {
              sum += (float)-0.09855134;
            }
          }
        }
      }
    }
  }
  if ( (data[11].fvalue < 3.5)) {
    if ( (data[12].fvalue < 2.5)) {
      if ( (data[17].fvalue < 3.9371545)) {
        if ( (data[17].fvalue < 1.2589674)) {
          if ( (data[11].fvalue < 2.5)) {
            if ( (data[6].fvalue < 0.0068932758)) {
              sum += (float)-0.22326776;
            } else {
              sum += (float)-0.00026562676;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.16713691;
            } else {
              sum += (float)-0.0046051196;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.41640484)) {
              sum += (float)-0.041500069;
            } else {
              sum += (float)0.11181089;
            }
          } else {
            if ( (data[15].fvalue < 0.027986299)) {
              sum += (float)0.14337263;
            } else {
              sum += (float)0.026397245;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.79780459)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.14635384)) {
              sum += (float)-0.26491907;
            } else {
              sum += (float)-0.10117225;
            }
          } else {
            if ( (data[14].fvalue < 0.12621167)) {
              sum += (float)-0.075046234;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)0.080007866;
        }
      }
    } else {
      sum += (float)0.21525709;
    }
  } else {
    if (  (data[4].fvalue < 2.8552346)) {
      if ( (data[7].fvalue < 1.5)) {
        sum += (float)-0.11102124;
      } else {
        sum += (float)-0;
      }
    } else {
      sum += (float)-0.17817092;
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[4].fvalue < 0.79252237)) {
            if ( (data[5].fvalue < 1.7487364)) {
              sum += (float)0.044131566;
            } else {
              sum += (float)0.16369368;
            }
          } else {
            if (  (data[4].fvalue < 1.0592622)) {
              sum += (float)-0.011969379;
            } else {
              sum += (float)0.0066652596;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.10788132)) {
            if ( (data[3].fvalue < 0.14347175)) {
              sum += (float)0.011390179;
            } else {
              sum += (float)-0.17169991;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.027753439;
            } else {
              sum += (float)0.052632526;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[4].fvalue < 0.99611974)) {
            sum += (float)0.19821621;
          } else {
            if ( (data[17].fvalue < 0.14969249)) {
              sum += (float)0.034535948;
            } else {
              sum += (float)0.13837074;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.24425228)) {
              sum += (float)-0.11396497;
            } else {
              sum += (float)-0.031231048;
            }
          } else {
            if (  (data[4].fvalue < 1.1207547)) {
              sum += (float)0.084898412;
            } else {
              sum += (float)-0.012644538;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.62357211)) {
        if ( (data[15].fvalue < 0.19360405)) {
          if ( (data[3].fvalue < 0.40129602)) {
            if (  (data[4].fvalue < 1.3197342)) {
              sum += (float)0.12644202;
            } else {
              sum += (float)-0.0048560994;
            }
          } else {
            if ( (data[17].fvalue < 0.39869756)) {
              sum += (float)-0.10188575;
            } else {
              sum += (float)0.10884664;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7174547)) {
            sum += (float)0.18936774;
          } else {
            if ( (data[13].fvalue < 0.16660506)) {
              sum += (float)0.10446685;
            } else {
              sum += (float)-0.011802637;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.11411883)) {
              sum += (float)0.1054083;
            } else {
              sum += (float)0.24149384;
            }
          } else {
            if ( (data[13].fvalue < 0.3485297)) {
              sum += (float)0.028478134;
            } else {
              sum += (float)0.12819779;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            sum += (float)0.069507472;
          } else {
            sum += (float)-0.035210706;
          }
        }
      }
    }
  } else {
    if ( (data[11].fvalue < 1.5)) {
      if ( (data[13].fvalue < 0.67890251)) {
        if ( (data[3].fvalue < 0.445719)) {
          if (  (data[4].fvalue < 1.1419303)) {
            if (  (data[4].fvalue < 1.0832796)) {
              sum += (float)0.29983222;
            } else {
              sum += (float)0.16339886;
            }
          } else {
            if ( (data[15].fvalue < 0.033488512)) {
              sum += (float)-0;
            } else {
              sum += (float)0.11377368;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.39856428)) {
            if ( (data[17].fvalue < 0.10802019)) {
              sum += (float)-0.057230067;
            } else {
              sum += (float)0.052713718;
            }
          } else {
            if ( (data[14].fvalue < 0.46870151)) {
              sum += (float)0.11702011;
            } else {
              sum += (float)0.0063168039;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 5.5)) {
          if ( (data[14].fvalue < 0.32409102)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.10571463;
            } else {
              sum += (float)-0.043915652;
            }
          } else {
            if ( (data[13].fvalue < 0.8348738)) {
              sum += (float)0.10454327;
            } else {
              sum += (float)0.20186079;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.67325813)) {
            if (  (data[4].fvalue < 1.8650765)) {
              sum += (float)-0.089067489;
            } else {
              sum += (float)-0.21264093;
            }
          } else {
            if ( (data[16].fvalue < 0.028068161)) {
              sum += (float)0.16301528;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.27192205)) {
        sum += (float)-0.17179194;
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.26870206)) {
            sum += (float)0.099106297;
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.030005807;
            } else {
              sum += (float)-0.070397429;
            }
          }
        } else {
          sum += (float)-0.1552767;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.36287093)) {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[17].fvalue < 3.1723118)) {
        if ( (data[15].fvalue < 2.6753039)) {
          if (  (data[4].fvalue < 1.8978496)) {
            if ( (data[12].fvalue < 0.5)) {
              sum += (float)-0.0080698682;
            } else {
              sum += (float)0.15966247;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.038352247;
            } else {
              sum += (float)-0.006374856;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.44487453)) {
            if ( (data[5].fvalue < 0.2182323)) {
              sum += (float)-0.048806474;
            } else {
              sum += (float)-0.15488268;
            }
          } else {
            if ( (data[14].fvalue < 1.0648038)) {
              sum += (float)-0.0036716699;
            } else {
              sum += (float)0.086501524;
            }
          }
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.22783405)) {
            if (  (data[4].fvalue < 2.6782031)) {
              sum += (float)-0.30853635;
            } else {
              sum += (float)-0.17387915;
            }
          } else {
            sum += (float)-0.033230539;
          }
        } else {
          if ( (data[0].fvalue < 7.4883966)) {
            if ( (data[13].fvalue < 0.016329041)) {
              sum += (float)-0.0090931775;
            } else {
              sum += (float)0.044989772;
            }
          } else {
            sum += (float)-0.051486846;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.34071344)) {
        sum += (float)0.23343475;
      } else {
        if ( (data[7].fvalue < 3.5)) {
          sum += (float)0.15953349;
        } else {
          if ( (data[14].fvalue < 0.69625902)) {
            sum += (float)0.092636406;
          } else {
            sum += (float)-0.048527032;
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 1.1057847)) {
      if ( (data[15].fvalue < 0.053761147)) {
        if ( (data[14].fvalue < 0.24375153)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[17].fvalue < 0.36706412)) {
              sum += (float)0.0035793961;
            } else {
              sum += (float)0.05191822;
            }
          } else {
            if (  (data[4].fvalue < 1.3933289)) {
              sum += (float)0.015652426;
            } else {
              sum += (float)-0.11379712;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.55561095)) {
              sum += (float)0.047814697;
            } else {
              sum += (float)0.10725802;
            }
          } else {
            if ( (data[11].fvalue < 2.5)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.079346672;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.026502129)) {
          if ( (data[16].fvalue < 0.99973553)) {
            if ( (data[13].fvalue < 0.75845313)) {
              sum += (float)0.029343106;
            } else {
              sum += (float)-0.038408764;
            }
          } else {
            if ( (data[3].fvalue < 3.714772)) {
              sum += (float)0.13408437;
            } else {
              sum += (float)0.01864876;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.40109837)) {
            if ( (data[14].fvalue < 0.0015440063)) {
              sum += (float)-0.15805262;
            } else {
              sum += (float)-0.032229789;
            }
          } else {
            if ( (data[15].fvalue < 0.30686855)) {
              sum += (float)-0.055699605;
            } else {
              sum += (float)0.0049603488;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[3].fvalue < 2.1334248)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.53249121)) {
              sum += (float)0.078538045;
            } else {
              sum += (float)0.18284494;
            }
          } else {
            if ( (data[1].fvalue < 1.8700814)) {
              sum += (float)0.087655783;
            } else {
              sum += (float)-0.012065215;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.7879591)) {
            if ( (data[16].fvalue < 0.59435761)) {
              sum += (float)0.036924943;
            } else {
              sum += (float)-0.073790699;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.095217802;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.21557556)) {
              sum += (float)0.0017957434;
            } else {
              sum += (float)0.060193826;
            }
          } else {
            if ( (data[14].fvalue < 0.57929045)) {
              sum += (float)-0.066549353;
            } else {
              sum += (float)0.024962323;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.21130455)) {
            sum += (float)-0.029328071;
          } else {
            sum += (float)-0.15694571;
          }
        }
      }
    }
  }
  if ( (data[1].fvalue < 0.18506883)) {
    if ( (data[5].fvalue < 0.13442323)) {
      if ( (data[0].fvalue < 1.5778127)) {
        sum += (float)0.1950883;
      } else {
        if ( (data[1].fvalue < 0.14573342)) {
          sum += (float)0.020192904;
        } else {
          sum += (float)0.079966381;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.53764403)) {
        if ( (data[13].fvalue < 0.13432661)) {
          if ( (data[0].fvalue < 0.3631416)) {
            sum += (float)0.048479594;
          } else {
            if ( (data[3].fvalue < 0.70914519)) {
              sum += (float)-0.031197973;
            } else {
              sum += (float)0.034275178;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.47831962)) {
            sum += (float)0.097430468;
          } else {
            if ( (data[15].fvalue < 0.077609427)) {
              sum += (float)0.042523839;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        sum += (float)-0.027559729;
      }
    }
  } else {
    if ( (data[13].fvalue < 0.014386103)) {
      if ( (data[14].fvalue < 1.7758883e-05)) {
        if ( (data[3].fvalue < 1.446198)) {
          if (  (data[4].fvalue < 0.99053746)) {
            if ( (data[13].fvalue < 0.0015494567)) {
              sum += (float)-0;
            } else {
              sum += (float)0.23428324;
            }
          } else {
            if ( (data[13].fvalue < 6.889577e-06)) {
              sum += (float)0.023946837;
            } else {
              sum += (float)-0.26956984;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.032094922)) {
            sum += (float)0.29669023;
          } else {
            sum += (float)0.44954711;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.020548424)) {
          if ( (data[15].fvalue < 0.86990404)) {
            if (  (data[4].fvalue < 1.5208892)) {
              sum += (float)-0.17261176;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.27908748;
          }
        } else {
          if ( (data[13].fvalue < 1.0505208e-09)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.015430356;
            } else {
              sum += (float)0.33683571;
            }
          } else {
            if ( (data[13].fvalue < 0.0027895058)) {
              sum += (float)-0.22905952;
            } else {
              sum += (float)-0.028776789;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.0075324541)) {
        if ( (data[14].fvalue < 3.2678938e-07)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.018046409;
            } else {
              sum += (float)0.028666742;
            }
          } else {
            sum += (float)0.38165084;
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[4].fvalue < 1.1538911)) {
              sum += (float)-0.065457515;
            } else {
              sum += (float)0.12738688;
            }
          } else {
            if ( (data[3].fvalue < 0.20736188)) {
              sum += (float)-0.061169215;
            } else {
              sum += (float)-0.22063257;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.052582443)) {
            if (  (data[4].fvalue < 1.1029544)) {
              sum += (float)-0.025435856;
            } else {
              sum += (float)0.13660486;
            }
          } else {
            if ( (data[14].fvalue < 0.45253253)) {
              sum += (float)0.0098653408;
            } else {
              sum += (float)0.055558536;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.053555518)) {
            if ( (data[0].fvalue < 3.149219)) {
              sum += (float)0.29169315;
            } else {
              sum += (float)0.096788578;
            }
          } else {
            if ( (data[14].fvalue < 0.037565842)) {
              sum += (float)-0.058561441;
            } else {
              sum += (float)-0.0016758858;
            }
          }
        }
      }
    }
  }
  if ( (data[3].fvalue < 0.50061166)) {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[17].fvalue < 0.13948514)) {
          if ( (data[15].fvalue < 0.15038452)) {
            if (  (data[4].fvalue < 1.1729183)) {
              sum += (float)0.0014467646;
            } else {
              sum += (float)-0.030222559;
            }
          } else {
            if (  (data[4].fvalue < 1.7104025)) {
              sum += (float)0.044308454;
            } else {
              sum += (float)-0.022115517;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.037944898)) {
            if ( (data[13].fvalue < 0.0022140623)) {
              sum += (float)-0.23339209;
            } else {
              sum += (float)0.09308739;
            }
          } else {
            if ( (data[14].fvalue < 0.0020357252)) {
              sum += (float)-0.089108914;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4415126)) {
          sum += (float)0.40463847;
        } else {
          if ( (data[17].fvalue < 0.013276106)) {
            if ( (data[5].fvalue < 0.045627832)) {
              sum += (float)0.22123088;
            } else {
              sum += (float)0.036536399;
            }
          } else {
            sum += (float)-0.098581068;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.168615)) {
        if (  (data[4].fvalue < 1.0628202)) {
          if ( (data[14].fvalue < 0.080517009)) {
            sum += (float)0.14556651;
          } else {
            sum += (float)0.32445347;
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            sum += (float)0.0013499459;
          } else {
            sum += (float)0.24287076;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8162978)) {
          if ( (data[15].fvalue < 0.1408)) {
            if ( (data[14].fvalue < 0.3274425)) {
              sum += (float)-0.034366556;
            } else {
              sum += (float)0.1054543;
            }
          } else {
            if (  (data[4].fvalue < 1.4426785)) {
              sum += (float)0.30349219;
            } else {
              sum += (float)0.093448117;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.26142317)) {
            if ( (data[17].fvalue < 0.47660467)) {
              sum += (float)-0.11766373;
            } else {
              sum += (float)0.014889354;
            }
          } else {
            sum += (float)0.044088371;
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.44267827)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[4].fvalue < 1.1073086)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.2614646)) {
              sum += (float)0.11088716;
            } else {
              sum += (float)0.022786448;
            }
          } else {
            if ( (data[14].fvalue < 0.51928675)) {
              sum += (float)-0.14409095;
            } else {
              sum += (float)0.060628802;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.8915153)) {
              sum += (float)-0.043203257;
            } else {
              sum += (float)-0.0019733806;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.052734841;
            } else {
              sum += (float)0.0011165813;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.066521063)) {
          if ( (data[5].fvalue < 0.58124)) {
            if ( (data[14].fvalue < 0.29649001)) {
              sum += (float)-0.0051690293;
            } else {
              sum += (float)-0.1743491;
            }
          } else {
            sum += (float)-0.24321716;
          }
        } else {
          if ( (data[15].fvalue < 0.15929674)) {
            if ( (data[16].fvalue < 0.12588067)) {
              sum += (float)-0.051573645;
            } else {
              sum += (float)0.036525551;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.0044108769;
            } else {
              sum += (float)0.045603737;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.042902283)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.041367926)) {
            if (  (data[4].fvalue < 1.1140552)) {
              sum += (float)-0.0031341272;
            } else {
              sum += (float)0.062225666;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.047542702;
            } else {
              sum += (float)0.060552172;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.088504359)) {
            if ( (data[3].fvalue < 0.91637403)) {
              sum += (float)-0.055312462;
            } else {
              sum += (float)-0.18242517;
            }
          } else {
            if ( (data[14].fvalue < 0.78355986)) {
              sum += (float)-0.0045076627;
            } else {
              sum += (float)0.091661029;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.28186068)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0015026741)) {
              sum += (float)-0.14374703;
            } else {
              sum += (float)0.012228418;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.068558641;
            } else {
              sum += (float)-0.043741543;
            }
          }
        } else {
          if ( (data[17].fvalue < 1.2664711)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.0073647783;
            } else {
              sum += (float)-0.034996297;
            }
          } else {
            if ( (data[4].fvalue < 1.7392392)) {
              sum += (float)-0.08822377;
            } else {
              sum += (float)0.046108354;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.0454293)) {
    if ( (data[13].fvalue < 0.13823801)) {
      if ( (data[16].fvalue < 1.7632699)) {
        if (  (data[4].fvalue < 1.7552929)) {
          if ( (data[15].fvalue < 1.036437)) {
            if ( (data[12].fvalue < 0.5)) {
              sum += (float)-0.013685456;
            } else {
              sum += (float)0.16000891;
            }
          } else {
            if ( (data[17].fvalue < 0.0013579712)) {
              sum += (float)-0.13157342;
            } else {
              sum += (float)0.075639792;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 2.8034141e-05)) {
              sum += (float)0.16050568;
            } else {
              sum += (float)-0.065037891;
            }
          } else {
            if ( (data[17].fvalue < 0.80477411)) {
              sum += (float)-0.012059411;
            } else {
              sum += (float)0.02549416;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.2610817)) {
          if ( (data[14].fvalue < 0.0074582594)) {
            sum += (float)-0.29654062;
          } else {
            sum += (float)-0.15697452;
          }
        } else {
          sum += (float)0.040369939;
        }
      }
    } else {
      if ( (data[0].fvalue < 2.2657557)) {
        if ( (data[5].fvalue < 0.34932977)) {
          if (  (data[4].fvalue < 1.1306278)) {
            if (  (data[4].fvalue < 1.0828246)) {
              sum += (float)0.040248815;
            } else {
              sum += (float)0.25381899;
            }
          } else {
            if ( (data[15].fvalue < 0.19758418)) {
              sum += (float)0.021789946;
            } else {
              sum += (float)0.086993672;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.098276742)) {
            sum += (float)-0.16489701;
          } else {
            if ( (data[1].fvalue < 1.6791075)) {
              sum += (float)0.0064989342;
            } else {
              sum += (float)0.071313828;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.92175573)) {
          if ( (data[15].fvalue < 1.2386664)) {
            if (  (data[4].fvalue < 1.7193909)) {
              sum += (float)0.0027745347;
            } else {
              sum += (float)-0.02157194;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.033673778;
            } else {
              sum += (float)-0.080673061;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.763098)) {
            if ( (data[3].fvalue < 3.1255403)) {
              sum += (float)0.030762797;
            } else {
              sum += (float)-0.078354836;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.10469684;
            } else {
              sum += (float)0.013307308;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.10876973)) {
      sum += (float)-0.17629385;
    } else {
      if ( (data[15].fvalue < 0.0026916713)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.00010489565)) {
            if ( (data[13].fvalue < 0.047819726)) {
              sum += (float)0.15170349;
            } else {
              sum += (float)0.014588493;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.030649409;
            } else {
              sum += (float)0.081780665;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.4287609)) {
            if ( (data[14].fvalue < 0.073970318)) {
              sum += (float)0.025469093;
            } else {
              sum += (float)0.20324406;
            }
          } else {
            sum += (float)-0.031302873;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.19693623;
        }
      }
    }
  }
  if ( (data[15].fvalue < 3.3302336)) {
    if ( (data[6].fvalue < 0.0073407684)) {
      sum += (float)-0.16361563;
    } else {
      if ( (data[11].fvalue < 3.5)) {
        if ( (data[15].fvalue < 0.39889699)) {
          if ( (data[14].fvalue < 1.2074659)) {
            if ( (data[5].fvalue < 0.05998724)) {
              sum += (float)0.031470045;
            } else {
              sum += (float)-0.00041254604;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.10732242;
            } else {
              sum += (float)0.013592282;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.51758897)) {
            if ( (data[16].fvalue < 2.0691657)) {
              sum += (float)0.015628459;
            } else {
              sum += (float)-0.059963774;
            }
          } else {
            if ( (data[14].fvalue < 0.75777417)) {
              sum += (float)-0.022710778;
            } else {
              sum += (float)0.013857478;
            }
          }
        }
      } else {
        if ( (data[2].fvalue < 0.66637003)) {
          if ( (data[14].fvalue < 0.0019167874)) {
            sum += (float)-0.11917301;
          } else {
            if ( (data[14].fvalue < 0.22407188)) {
              sum += (float)0.069639467;
            } else {
              sum += (float)-0.062471773;
            }
          }
        } else {
          sum += (float)-0.13037151;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.4777903)) {
      if ( (data[14].fvalue < 0.6537962)) {
        if (  (data[4].fvalue < 2.5295436)) {
          sum += (float)-0.17237543;
        } else {
          if ( (data[5].fvalue < 0.0878627)) {
            sum += (float)-0;
          } else {
            if ( (data[14].fvalue < 0.29475623)) {
              sum += (float)-0.10547788;
            } else {
              sum += (float)-0.035419427;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 6.9054909)) {
          sum += (float)0.077652648;
        } else {
          if ( (data[16].fvalue < 0.022609169)) {
            sum += (float)2.8146276e-05;
          } else {
            sum += (float)-0.10434287;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.29447556)) {
        sum += (float)0.074637659;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[14].fvalue < 0.095682539)) {
    if ( (data[8].fvalue < 1.5)) {
      if (  (data[4].fvalue < 2.259773)) {
        if ( (data[15].fvalue < 0.041317232)) {
          if ( (data[13].fvalue < 1.0350003e-06)) {
            if ( (data[17].fvalue < 0.27531332)) {
              sum += (float)0.20043731;
            } else {
              sum += (float)-0.30102885;
            }
          } else {
            if ( (data[16].fvalue < 0.00047223375)) {
              sum += (float)0.0042113457;
            } else {
              sum += (float)-0.065092988;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.023654742)) {
            if ( (data[5].fvalue < 0.097799271)) {
              sum += (float)-0.044937015;
            } else {
              sum += (float)-0.13726239;
            }
          } else {
            if ( (data[14].fvalue < 0.0033056978)) {
              sum += (float)-0.027044874;
            } else {
              sum += (float)0.057030097;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 1.2920518)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 1.9736271)) {
              sum += (float)0.223046;
            } else {
              sum += (float)-0.061965454;
            }
          } else {
            if ( (data[13].fvalue < 0.39080569)) {
              sum += (float)-0.0083903624;
            } else {
              sum += (float)0.037817426;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.31295681)) {
            if ( (data[13].fvalue < 0.10977538)) {
              sum += (float)-0.22601108;
            } else {
              sum += (float)-0.10561103;
            }
          } else {
            if ( (data[13].fvalue < 0.71039164)) {
              sum += (float)-0.023521345;
            } else {
              sum += (float)0.068023682;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.15523006)) {
        if (  (data[4].fvalue < 0.98887563)) {
          if ( (data[13].fvalue < 0.014622675)) {
            sum += (float)0.05311387;
          } else {
            sum += (float)0.40903237;
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[15].fvalue < 0.087693304)) {
              sum += (float)-0.067343265;
            } else {
              sum += (float)0.089219108;
            }
          } else {
            if ( (data[13].fvalue < 0.10981517)) {
              sum += (float)0.24853468;
            } else {
              sum += (float)0.044092715;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.048474107)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.15476832;
            } else {
              sum += (float)-0.069964685;
            }
          } else {
            sum += (float)0.086074591;
          }
        } else {
          if ( (data[15].fvalue < 0.0010803952)) {
            if ( (data[13].fvalue < 0.024605576)) {
              sum += (float)-0.2390039;
            } else {
              sum += (float)-0.044522021;
            }
          } else {
            if ( (data[13].fvalue < 0.2707752)) {
              sum += (float)0.057738859;
            } else {
              sum += (float)-0.047918092;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.21624771)) {
      if (  (data[4].fvalue < 1.1453445)) {
        if ( (data[13].fvalue < 0.011072392)) {
          sum += (float)-0.035520803;
        } else {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.14050607;
          } else {
            sum += (float)0.28962728;
          }
        }
      } else {
        if ( (data[0].fvalue < 1.988888)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.29959905)) {
              sum += (float)0.001593123;
            } else {
              sum += (float)0.12934929;
            }
          } else {
            if ( (data[3].fvalue < 0.12806204)) {
              sum += (float)0.18782014;
            } else {
              sum += (float)0.065115385;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.08255659)) {
            if ( (data[14].fvalue < 0.27840304)) {
              sum += (float)0.017991809;
            } else {
              sum += (float)0.097929828;
            }
          } else {
            if ( (data[13].fvalue < 0.21725006)) {
              sum += (float)-0.021222327;
            } else {
              sum += (float)0.032302924;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.64978373)) {
        if ( (data[17].fvalue < 0.026418921)) {
          if ( (data[14].fvalue < 0.40973297)) {
            if ( (data[3].fvalue < 1.1608918)) {
              sum += (float)0.0035878608;
            } else {
              sum += (float)-0.035394508;
            }
          } else {
            if ( (data[4].fvalue < 1.3704255)) {
              sum += (float)0.072560482;
            } else {
              sum += (float)0.010245993;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0057926727)) {
            if ( (data[15].fvalue < 0.95275664)) {
              sum += (float)-0.12378342;
            } else {
              sum += (float)-0.00056508888;
            }
          } else {
            if ( (data[17].fvalue < 0.11389434)) {
              sum += (float)-0.041313808;
            } else {
              sum += (float)0.0066042203;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.047946)) {
          if ( (data[3].fvalue < 3.0753477)) {
            if ( (data[14].fvalue < 0.73969007)) {
              sum += (float)0.13578992;
            } else {
              sum += (float)0.0074449577;
            }
          } else {
            if ( (data[14].fvalue < 0.3925091)) {
              sum += (float)-0.037231255;
            } else {
              sum += (float)0.061210386;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.81858003)) {
              sum += (float)-0.013458361;
            } else {
              sum += (float)0.038229972;
            }
          } else {
            sum += (float)-0.18108745;
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 3.218745)) {
    if ( (data[10].fvalue < 2.5)) {
      if ( (data[7].fvalue < 6.5)) {
        if ( (data[15].fvalue < 0.18309811)) {
          if ( (data[15].fvalue < 0.05250876)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.0037026748;
            } else {
              sum += (float)0.043998953;
            }
          } else {
            if ( (data[3].fvalue < 0.09979631)) {
              sum += (float)0.038349412;
            } else {
              sum += (float)-0.033039581;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.2083538)) {
            if ( (data[14].fvalue < 0.0027130537)) {
              sum += (float)-4.9108563e-05;
            } else {
              sum += (float)0.073757499;
            }
          } else {
            if ( (data[13].fvalue < 0.93304384)) {
              sum += (float)0.0042514405;
            } else {
              sum += (float)-0.019335959;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 2.2005343)) {
          if ( (data[6].fvalue < 0.17337897)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)0.067193165;
            } else {
              sum += (float)0.23211147;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.088754244;
            } else {
              sum += (float)0.019509345;
            }
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[13].fvalue < 1.0756426)) {
              sum += (float)-0;
            } else {
              sum += (float)0.1382506;
            }
          } else {
            if ( (data[4].fvalue < 2.1612685)) {
              sum += (float)-0.17393719;
            } else {
              sum += (float)-0.035324309;
            }
          }
        }
      }
    } else {
      sum += (float)-0.12724273;
    }
  } else {
    if (  (data[4].fvalue < 3.0469413)) {
      if ( (data[17].fvalue < 0.16694674)) {
        if (  (data[4].fvalue < 2.2434027)) {
          sum += (float)-0.040003046;
        } else {
          sum += (float)-0.137137;
        }
      } else {
        if ( (data[3].fvalue < 4.6274042)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.11192521;
        }
      }
    } else {
      sum += (float)0.0035165499;
    }
  }
  if ( (data[7].fvalue < 12.5)) {
    if ( (data[15].fvalue < 5.1153903)) {
      if ( (data[0].fvalue < 1.0681558)) {
        if ( (data[14].fvalue < 0.31136021)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.09667933)) {
              sum += (float)-0.16001487;
            } else {
              sum += (float)0.0042143799;
            }
          } else {
            if ( (data[5].fvalue < 0.19243693)) {
              sum += (float)0.062581219;
            } else {
              sum += (float)0.012175303;
            }
          }
        } else {
          sum += (float)0.13712907;
        }
      } else {
        if ( (data[6].fvalue < 0.0070538213)) {
          sum += (float)-0.16399661;
        } else {
          if ( (data[13].fvalue < 0.26842338)) {
            if ( (data[8].fvalue < 9.5)) {
              sum += (float)-0.0050613666;
            } else {
              sum += (float)0.13674021;
            }
          } else {
            if ( (data[16].fvalue < 0.75145292)) {
              sum += (float)0.0018155145;
            } else {
              sum += (float)0.032873329;
            }
          }
        }
      }
    } else {
      sum += (float)-0.14663766;
    }
  } else {
    sum += (float)0.15151283;
  }
  if ( (data[17].fvalue < 3.32781)) {
    if ( (data[14].fvalue < 0.12840214)) {
      if ( (data[8].fvalue < 6.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)0.0023117203;
            } else {
              sum += (float)-0.054106876;
            }
          } else {
            if ( (data[13].fvalue < 0.80884469)) {
              sum += (float)-0.067965113;
            } else {
              sum += (float)0.028750317;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.097056195)) {
            if ( (data[6].fvalue < 0.025241848)) {
              sum += (float)0.16197827;
            } else {
              sum += (float)-0.0516156;
            }
          } else {
            if ( (data[3].fvalue < 1.3657317)) {
              sum += (float)0.053158481;
            } else {
              sum += (float)-0.071177311;
            }
          }
        }
      } else {
        sum += (float)0.19132932;
      }
    } else {
      if ( (data[0].fvalue < 1.1759112)) {
        if ( (data[13].fvalue < 0.21478793)) {
          if ( (data[6].fvalue < 0.38080817)) {
            if ( (data[13].fvalue < 0.03925804)) {
              sum += (float)0.018800763;
            } else {
              sum += (float)0.18298261;
            }
          } else {
            if ( (data[14].fvalue < 0.29699317)) {
              sum += (float)-0.020242674;
            } else {
              sum += (float)0.10219074;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3215712)) {
            if ( (data[6].fvalue < 0.56124139)) {
              sum += (float)0.22269419;
            } else {
              sum += (float)0.096256174;
            }
          } else {
            sum += (float)0.010075476;
          }
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.31460333)) {
            if ( (data[15].fvalue < 0.4368256)) {
              sum += (float)-0.012569711;
            } else {
              sum += (float)0.01911029;
            }
          } else {
            if (  (data[4].fvalue < 1.8007282)) {
              sum += (float)0.040106587;
            } else {
              sum += (float)0.0017390209;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.67390382)) {
            if (  (data[4].fvalue < 2.7958329)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.06183061;
            }
          } else {
            sum += (float)-0.18648756;
          }
        }
      }
    }
  } else {
    if ( (data[11].fvalue < 1.5)) {
      if (  (data[4].fvalue < 2.7243845)) {
        if ( (data[9].fvalue < 0.5)) {
          sum += (float)-0.24124964;
        } else {
          sum += (float)-0.067199744;
        }
      } else {
        if ( (data[17].fvalue < 3.9069555)) {
          sum += (float)0.032970414;
        } else {
          sum += (float)-0.068327107;
        }
      }
    } else {
      if (  (data[4].fvalue < 2.7699904)) {
        if ( (data[14].fvalue < 0.015207431)) {
          sum += (float)0.0094635813;
        } else {
          sum += (float)0.0909575;
        }
      } else {
        if ( (data[15].fvalue < 0.034063149)) {
          sum += (float)0.0049914867;
        } else {
          if ( (data[13].fvalue < 0.37786931)) {
            sum += (float)-0.0085533047;
          } else {
            sum += (float)-0.073270537;
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.13780126)) {
    if ( (data[5].fvalue < 0.88028061)) {
      if ( (data[14].fvalue < 0.069290042)) {
        if ( (data[15].fvalue < 1.2813141)) {
          if (  (data[4].fvalue < 1.7086029)) {
            if ( (data[3].fvalue < 0.28184795)) {
              sum += (float)-0.016564231;
            } else {
              sum += (float)-0.099872619;
            }
          } else {
            if ( (data[17].fvalue < 1.0128608)) {
              sum += (float)0.1540743;
            } else {
              sum += (float)-0.10454959;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.82335)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.16953783;
            } else {
              sum += (float)0.033574406;
            }
          } else {
            sum += (float)0.11773751;
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.0069543775)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.096455961;
            } else {
              sum += (float)6.5905377e-05;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.04016161;
            } else {
              sum += (float)-0.028781315;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.32979298)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.080683053;
            } else {
              sum += (float)0.089652613;
            }
          } else {
            if (  (data[4].fvalue < 1.3580644)) {
              sum += (float)-0.0042789639;
            } else {
              sum += (float)-0.1508822;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.24756578)) {
        if ( (data[13].fvalue < 0.062312435)) {
          if ( (data[5].fvalue < 1.3830235)) {
            if ( (data[14].fvalue < 2.1293507e-05)) {
              sum += (float)0.13820617;
            } else {
              sum += (float)-0.017496271;
            }
          } else {
            if ( (data[13].fvalue < 0.032425795)) {
              sum += (float)0.019271912;
            } else {
              sum += (float)-0.23374873;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.093122475)) {
            if ( (data[3].fvalue < 0.19087425)) {
              sum += (float)-0.30384043;
            } else {
              sum += (float)0.0070778141;
            }
          } else {
            sum += (float)-0.32265702;
          }
        }
      } else {
        if ( (data[14].fvalue < 1.4504767e-05)) {
          if ( (data[13].fvalue < 0.10058047)) {
            if (  (data[4].fvalue < 0.90740007)) {
              sum += (float)0.13458925;
            } else {
              sum += (float)0.29902694;
            }
          } else {
            if ( (data[3].fvalue < 0.2970911)) {
              sum += (float)-0.10271923;
            } else {
              sum += (float)0.11051393;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.51778543)) {
            if ( (data[3].fvalue < 0.59771633)) {
              sum += (float)0.015754119;
            } else {
              sum += (float)-0.071190938;
            }
          } else {
            if ( (data[15].fvalue < 0.32616401)) {
              sum += (float)0.1385375;
            } else {
              sum += (float)0.0014155732;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.6131046)) {
      if ( (data[17].fvalue < 1.1902514)) {
        if ( (data[4].fvalue < 1.773527)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[17].fvalue < 0.21723169)) {
              sum += (float)0.0048794765;
            } else {
              sum += (float)0.055486314;
            }
          } else {
            if (  (data[4].fvalue < 1.3568382)) {
              sum += (float)0.40181234;
            } else {
              sum += (float)0.081819504;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.6127665)) {
            if ( (data[16].fvalue < 0.73288953)) {
              sum += (float)-0.044373635;
            } else {
              sum += (float)0.049080819;
            }
          } else {
            if (  (data[4].fvalue < 2.5031025)) {
              sum += (float)0.042386994;
            } else {
              sum += (float)-0.016932597;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.8239007)) {
          if (  (data[4].fvalue < 1.8336847)) {
            sum += (float)-0.021754624;
          } else {
            if (  (data[4].fvalue < 2.3232796)) {
              sum += (float)0.13438395;
            } else {
              sum += (float)0.060082249;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.86388844)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.0082105706;
            } else {
              sum += (float)-0.10532142;
            }
          } else {
            if ( (data[17].fvalue < 1.9528186)) {
              sum += (float)-0;
            } else {
              sum += (float)0.10465475;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.75475264)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[5].fvalue < 0.55752146)) {
            if ( (data[3].fvalue < 2.6188531)) {
              sum += (float)0.005397439;
            } else {
              sum += (float)-0.033907801;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.023025775;
            } else {
              sum += (float)-0.10076322;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.20709616)) {
            if ( (data[5].fvalue < 0.47784135)) {
              sum += (float)-0.23453282;
            } else {
              sum += (float)-0.12408077;
            }
          } else {
            sum += (float)-0.0088341804;
          }
        }
      } else {
        if ( (data[17].fvalue < 1.4629935)) {
          if ( (data[16].fvalue < 1.8014904)) {
            if ( (data[15].fvalue < 1.9057261)) {
              sum += (float)-0.008345495;
            } else {
              sum += (float)0.054090574;
            }
          } else {
            if ( (data[17].fvalue < 0.20603028)) {
              sum += (float)0.12998736;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 3.0229421)) {
            if (  (data[4].fvalue < 1.7392392)) {
              sum += (float)-0.021400807;
            } else {
              sum += (float)0.094365552;
            }
          } else {
            if ( (data[13].fvalue < 1.3589225)) {
              sum += (float)-0.038435906;
            } else {
              sum += (float)0.0005771349;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 8.5)) {
    if ( (data[5].fvalue < 0.019661557)) {
      if ( (data[13].fvalue < 0.00046602875)) {
        if ( (data[15].fvalue < 0.24740379)) {
          sum += (float)0.029328717;
        } else {
          if ( (data[0].fvalue < 4.3131518)) {
            sum += (float)-0.067007184;
          } else {
            sum += (float)0.0010244147;
          }
        }
      } else {
        if ( (data[3].fvalue < 0.035946708)) {
          if ( (data[15].fvalue < 0.082829416)) {
            if ( (data[17].fvalue < 0.0047076382)) {
              sum += (float)-0;
            } else {
              sum += (float)0.17895126;
            }
          } else {
            if ( (data[13].fvalue < 0.099851355)) {
              sum += (float)0.11390714;
            } else {
              sum += (float)0.23651519;
            }
          }
        } else {
          sum += (float)-0.01948308;
        }
      }
    } else {
      if ( (data[2].fvalue < 0.91301548)) {
        if ( (data[1].fvalue < 0.32636008)) {
          if ( (data[15].fvalue < 1.8173097)) {
            if ( (data[13].fvalue < 0.82454979)) {
              sum += (float)0.086373456;
            } else {
              sum += (float)-0.0088358698;
            }
          } else {
            sum += (float)-0.06333109;
          }
        } else {
          if ( (data[14].fvalue < 0.0079530049)) {
            if ( (data[14].fvalue < 1.764129e-07)) {
              sum += (float)-0.0016501584;
            } else {
              sum += (float)-0.071318455;
            }
          } else {
            if ( (data[13].fvalue < 0.05657088)) {
              sum += (float)-0.0065586227;
            } else {
              sum += (float)0.0099567007;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.55961859)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.002954623;
            } else {
              sum += (float)-0.047024831;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0019354319;
            } else {
              sum += (float)0.040163077;
            }
          }
        } else {
          if ( (data[0].fvalue < 0.63515365)) {
            sum += (float)0.1439909;
          } else {
            if ( (data[14].fvalue < 0.20541444)) {
              sum += (float)-0.094925843;
            } else {
              sum += (float)-0.02769055;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[13].fvalue < 0.95773518)) {
        sum += (float)0.046811968;
      } else {
        sum += (float)0.21589649;
      }
    } else {
      if ( (data[13].fvalue < 0.42460215)) {
        sum += (float)0.12697819;
      } else {
        if ( (data[14].fvalue < 1.0491922)) {
          if ( (data[8].fvalue < 6.5)) {
            sum += (float)0.018050985;
          } else {
            if ( (data[15].fvalue < 0.24966913)) {
              sum += (float)-0.11620732;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)0.058205705;
        }
      }
    }
  }
  if ( (data[4].fvalue < 1.199543)) {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[9].fvalue < 0.5)) {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)-0.38118818;
          } else {
            sum += (float)0.097149581;
          }
        } else {
          if ( (data[14].fvalue < 0.0865646)) {
            if ( (data[3].fvalue < 0.046025276)) {
              sum += (float)0.019186884;
            } else {
              sum += (float)-0.051543359;
            }
          } else {
            if ( (data[15].fvalue < 0.51965874)) {
              sum += (float)0.12527977;
            } else {
              sum += (float)-0.030715564;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 1.105268e-06)) {
          sum += (float)0.32511654;
        } else {
          if ( (data[13].fvalue < 0.0020666607)) {
            if ( (data[13].fvalue < 0.0013596106)) {
              sum += (float)-0.17276885;
            } else {
              sum += (float)-0.073983297;
            }
          } else {
            if (  (data[4].fvalue < 1.0769454)) {
              sum += (float)0.00072589127;
            } else {
              sum += (float)0.021071982;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.87505615)) {
        if ( (data[13].fvalue < 0.19732438)) {
          if ( (data[15].fvalue < 0.0010532062)) {
            if (  (data[4].fvalue < 1.0723712)) {
              sum += (float)0.14256699;
            } else {
              sum += (float)-0.038145579;
            }
          } else {
            sum += (float)0.29035136;
          }
        } else {
          if ( (data[3].fvalue < 0.64807796)) {
            if ( (data[13].fvalue < 0.24908531)) {
              sum += (float)0.19930489;
            } else {
              sum += (float)0.35734069;
            }
          } else {
            sum += (float)0.1360264;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.17768607)) {
          sum += (float)-0.22580765;
        } else {
          if ( (data[3].fvalue < 1.8044455)) {
            if ( (data[13].fvalue < 0.18932119)) {
              sum += (float)-0.057667345;
            } else {
              sum += (float)0.11231937;
            }
          } else {
            if ( (data[14].fvalue < 0.61451936)) {
              sum += (float)-0.1498877;
            } else {
              sum += (float)-0.010482654;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.075483739)) {
      if ( (data[11].fvalue < 0.5)) {
        if ( (data[15].fvalue < 0.0011819196)) {
          if ( (data[14].fvalue < 0.12848788)) {
            if ( (data[13].fvalue < 0.25684738)) {
              sum += (float)-0.16729036;
            } else {
              sum += (float)-0.032223947;
            }
          } else {
            if ( (data[13].fvalue < 0.12423484)) {
              sum += (float)-0.051874395;
            } else {
              sum += (float)0.050135117;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.024482649)) {
            if ( (data[14].fvalue < 0.073283896)) {
              sum += (float)-0.042068448;
            } else {
              sum += (float)0.10302836;
            }
          } else {
            if ( (data[15].fvalue < 0.10684138)) {
              sum += (float)0.059346002;
            } else {
              sum += (float)0.1276883;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.021962902)) {
          if ( (data[13].fvalue < 0.039893247)) {
            sum += (float)-0;
          } else {
            if ( (data[0].fvalue < 4.7947507)) {
              sum += (float)0.19267781;
            } else {
              sum += (float)0.065880194;
            }
          }
        } else {
          sum += (float)-0.049969699;
        }
      }
    } else {
      if ( (data[15].fvalue < 0.29076749)) {
        if ( (data[13].fvalue < 0.38673624)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)-0.0056444351;
            } else {
              sum += (float)-0.091334112;
            }
          } else {
            if ( (data[17].fvalue < 0.1140213)) {
              sum += (float)-0.10001909;
            } else {
              sum += (float)0.045868576;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.059330516)) {
            if ( (data[16].fvalue < 0.047084112)) {
              sum += (float)0.017943673;
            } else {
              sum += (float)-0.031676795;
            }
          } else {
            if ( (data[14].fvalue < 0.003052304)) {
              sum += (float)-0.049754158;
            } else {
              sum += (float)-0.00051128102;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.038045987)) {
          if ( (data[5].fvalue < 0.50536609)) {
            if ( (data[14].fvalue < 0.35755271)) {
              sum += (float)0.0058513354;
            } else {
              sum += (float)0.031515393;
            }
          } else {
            if ( (data[13].fvalue < 0.72812295)) {
              sum += (float)-0.025879452;
            } else {
              sum += (float)0.021285446;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0051095448)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.1812315;
            } else {
              sum += (float)-0.031543974;
            }
          } else {
            if ( (data[14].fvalue < 1.7412877)) {
              sum += (float)-0.0039131707;
            } else {
              sum += (float)0.090793803;
            }
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.77158016)) {
    if ( (data[5].fvalue < 1.4853373)) {
      if ( (data[14].fvalue < 0.0005622243)) {
        if (  (data[4].fvalue < 0.7421844)) {
          if (  (data[4].fvalue < 0.66912138)) {
            sum += (float)0.064662121;
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)0.094422109;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[3].fvalue < 2.1522307)) {
            sum += (float)-0.036653135;
          } else {
            sum += (float)0.079548791;
          }
        } else {
          if ( (data[3].fvalue < 1.9841495)) {
            sum += (float)-0.035748951;
          } else {
            sum += (float)-0.14543687;
          }
        }
      }
    } else {
      if ( (data[2].fvalue < 0.29704612)) {
        sum += (float)0.021150671;
      } else {
        if ( (data[4].fvalue < 0.69465566)) {
          sum += (float)0.1891486;
        } else {
          sum += (float)0.089100473;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 3.49089)) {
      if ( (data[8].fvalue < 11.5)) {
        if ( (data[6].fvalue < 0.0076098284)) {
          sum += (float)-0.11704567;
        } else {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.0032608511;
            } else {
              sum += (float)0.030622059;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.032102056;
            } else {
              sum += (float)-0.01743733;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.2708271)) {
          sum += (float)0.1802157;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if (  (data[4].fvalue < 3.0332117)) {
        sum += (float)-0.10066213;
      } else {
        sum += (float)0.0076575209;
      }
    }
  }
  if ( (data[10].fvalue < 1.5)) {
    if ( (data[16].fvalue < 0.27289575)) {
      if ( (data[16].fvalue < 0.014223577)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[17].fvalue < 0.017794969)) {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)0.013467134;
            } else {
              sum += (float)-0.08237879;
            }
          } else {
            if ( (data[14].fvalue < 0.0019273354)) {
              sum += (float)-0.034003634;
            } else {
              sum += (float)0.010555943;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.13987821)) {
            if ( (data[4].fvalue < 1.142505)) {
              sum += (float)0.0076718936;
            } else {
              sum += (float)-0.029079657;
            }
          } else {
            if ( (data[3].fvalue < 0.40338635)) {
              sum += (float)0.10287308;
            } else {
              sum += (float)0.0090642944;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.01610183)) {
            if ( (data[13].fvalue < 0.07986477)) {
              sum += (float)-0.027541807;
            } else {
              sum += (float)-0.25487313;
            }
          } else {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)-0.053129341;
            } else {
              sum += (float)0.17161344;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.061572976;
            } else {
              sum += (float)-0.0044271974;
            }
          } else {
            if ( (data[16].fvalue < 0.10133202)) {
              sum += (float)-0.081562378;
            } else {
              sum += (float)0.01307271;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.21448132)) {
          if ( (data[16].fvalue < 2.7068861)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.050766978;
            } else {
              sum += (float)0.014841102;
            }
          } else {
            sum += (float)-0.19996302;
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.31479406;
            } else {
              sum += (float)0.020366289;
            }
          } else {
            if ( (data[14].fvalue < 0.19617528)) {
              sum += (float)0.029175647;
            } else {
              sum += (float)-0.084004566;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.11031613)) {
          if ( (data[3].fvalue < 1.9704137)) {
            if (  (data[4].fvalue < 2.0958059)) {
              sum += (float)0.038744319;
            } else {
              sum += (float)-0.047631167;
            }
          } else {
            sum += (float)-0.1200378;
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[16].fvalue < 1.1663661)) {
              sum += (float)0.064206824;
            } else {
              sum += (float)0.1479627;
            }
          } else {
            if (  (data[4].fvalue < 2.7223644)) {
              sum += (float)0.042022482;
            } else {
              sum += (float)-0.034623269;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 2.7602615)) {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[15].fvalue < 0.00037957961)) {
          sum += (float)-0.24311547;
        } else {
          if ( (data[15].fvalue < 0.52089846)) {
            if ( (data[14].fvalue < 0.078410201)) {
              sum += (float)-0.043290652;
            } else {
              sum += (float)0.023952447;
            }
          } else {
            sum += (float)-0.13249734;
          }
        }
      } else {
        if ( (data[9].fvalue < 2.5)) {
          if ( (data[15].fvalue < 1.295536)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.11706375;
            } else {
              sum += (float)-0.0053779478;
            }
          } else {
            sum += (float)-0.038181257;
          }
        } else {
          if ( (data[14].fvalue < 0.38978621)) {
            sum += (float)0.27215031;
          } else {
            sum += (float)0.072074026;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 1.7651031)) {
        if ( (data[7].fvalue < 3.5)) {
          if (  (data[4].fvalue < 2.8730044)) {
            sum += (float)-0.02556131;
          } else {
            if ( (data[3].fvalue < 1.9216667)) {
              sum += (float)-0.084298417;
            } else {
              sum += (float)-0.16244709;
            }
          }
        } else {
          sum += (float)-0.17696968;
        }
      } else {
        sum += (float)0.01013179;
      }
    }
  }
  if ( (data[13].fvalue < 0.26830959)) {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[3].fvalue < 0.54580551)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[14].fvalue < 0.80595267)) {
            if ( (data[17].fvalue < 0.13255732)) {
              sum += (float)-0.001428914;
            } else {
              sum += (float)0.032215938;
            }
          } else {
            if ( (data[15].fvalue < 0.87017751)) {
              sum += (float)-0.10264967;
            } else {
              sum += (float)0.057811741;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.675504)) {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)0.31702957;
            } else {
              sum += (float)0.098118;
            }
          } else {
            sum += (float)-0.031200022;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.49120396)) {
            if ( (data[15].fvalue < 0.77221012)) {
              sum += (float)0.0077443863;
            } else {
              sum += (float)-0.051612388;
            }
          } else {
            if ( (data[15].fvalue < 0.04401508)) {
              sum += (float)0.080596171;
            } else {
              sum += (float)0.0074034519;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3791196)) {
            if ( (data[3].fvalue < 0.81382084)) {
              sum += (float)-0.038139883;
            } else {
              sum += (float)-0.11648589;
            }
          } else {
            if ( (data[14].fvalue < 0.36406747)) {
              sum += (float)0.035976697;
            } else {
              sum += (float)-0.060059775;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.79360855)) {
        sum += (float)0.17228048;
      } else {
        sum += (float)0.044595815;
      }
    }
  } else {
    if ( (data[14].fvalue < 0.5128448)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[15].fvalue < 0.92005157)) {
          if ( (data[14].fvalue < 0.033421613)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.00012107907;
            } else {
              sum += (float)-0.12581153;
            }
          } else {
            if ( (data[14].fvalue < 0.19883728)) {
              sum += (float)0.028102769;
            } else {
              sum += (float)-0.009606367;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[4].fvalue < 1.8896434)) {
              sum += (float)-0.0082302317;
            } else {
              sum += (float)0.066889681;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.013230209;
            } else {
              sum += (float)0.055298425;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.3586245)) {
          if ( (data[3].fvalue < 0.6425876)) {
            if ( (data[3].fvalue < 0.19026762)) {
              sum += (float)0.2915591;
            } else {
              sum += (float)0.10131721;
            }
          } else {
            if ( (data[3].fvalue < 2.0259609)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.16085963;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.13263223)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.24209052;
            } else {
              sum += (float)-0.12212171;
            }
          } else {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)-0.0096463263;
            } else {
              sum += (float)-0.081146851;
            }
          }
        }
      }
    } else {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.60307658)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[15].fvalue < 0.0011319336)) {
              sum += (float)-0.10987861;
            } else {
              sum += (float)0.042662982;
            }
          } else {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)0.085081339;
            } else {
              sum += (float)-0.0040686848;
            }
          }
        } else {
          if ( (data[3].fvalue < 3.1822844)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.13894974;
            } else {
              sum += (float)0.022399267;
            }
          } else {
            if ( (data[13].fvalue < 1.1004896)) {
              sum += (float)-0.067130551;
            } else {
              sum += (float)0.071145266;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 2.3628502)) {
          if ( (data[1].fvalue < 1.8024287)) {
            if ( (data[2].fvalue < 0.49336755)) {
              sum += (float)0.070712186;
            } else {
              sum += (float)0.00012402685;
            }
          } else {
            if ( (data[5].fvalue < 0.20813701)) {
              sum += (float)-0.079450563;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.81588149)) {
            sum += (float)-0.14480759;
          } else {
            if (  (data[4].fvalue < 2.5820193)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.049950618;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.056345768)) {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[7].fvalue < 0.5)) {
        if (  (data[4].fvalue < 1.0073304)) {
          sum += (float)-0.31646299;
        } else {
          sum += (float)-0.043374266;
        }
      } else {
        if ( (data[13].fvalue < 1.7813337e-06)) {
          sum += (float)0.30042848;
        } else {
          if ( (data[13].fvalue < 0.048706308)) {
            if ( (data[14].fvalue < 1.6594247e-05)) {
              sum += (float)0.093016572;
            } else {
              sum += (float)-0.092610002;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.058770925;
            } else {
              sum += (float)0.032309234;
            }
          }
        }
      }
    } else {
      if ( (data[0].fvalue < 4.1071024)) {
        sum += (float)0.2549136;
      } else {
        if ( (data[14].fvalue < 0.12355441)) {
          if ( (data[6].fvalue < 0.034000918)) {
            sum += (float)0.076901637;
          } else {
            sum += (float)-0.11465309;
          }
        } else {
          sum += (float)0.1301647;
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.051305272)) {
      if ( (data[11].fvalue < 0.5)) {
        if ( (data[14].fvalue < 0.43811926)) {
          if ( (data[14].fvalue < 5.6341179e-07)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.0062294411;
            } else {
              sum += (float)0.34474719;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.027027449;
            } else {
              sum += (float)0.01048129;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.4655608)) {
            if ( (data[3].fvalue < 0.80532724)) {
              sum += (float)0.11969005;
            } else {
              sum += (float)0.032991104;
            }
          } else {
            if ( (data[15].fvalue < 1.0031657)) {
              sum += (float)-0.0083352616;
            } else {
              sum += (float)0.061914336;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.010373067)) {
          if ( (data[17].fvalue < 0.0054224594)) {
            sum += (float)0.2549333;
          } else {
            if ( (data[14].fvalue < 0.10720092)) {
              sum += (float)0.20022599;
            } else {
              sum += (float)0.080865324;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.10125592;
            }
          } else {
            if ( (data[17].fvalue < 0.021341491)) {
              sum += (float)-0.097129509;
            } else {
              sum += (float)-0.010662601;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.13896967)) {
        if ( (data[13].fvalue < 0.0078153946)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 1.4864657)) {
              sum += (float)-0.16954027;
            } else {
              sum += (float)-0.023777921;
            }
          } else {
            if ( (data[14].fvalue < 0.49176633)) {
              sum += (float)-0.062234677;
            } else {
              sum += (float)0.024356846;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.352874)) {
            if ( (data[17].fvalue < 0.10382164)) {
              sum += (float)-0.028046256;
            } else {
              sum += (float)0.048820645;
            }
          } else {
            if (  (data[4].fvalue < 2.1340361)) {
              sum += (float)-0.065074243;
            } else {
              sum += (float)-0.0051662941;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4001961)) {
          if ( (data[17].fvalue < 0.49815643)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.077546015;
            } else {
              sum += (float)-0.15191546;
            }
          } else {
            if ( (data[14].fvalue < 0.016232193)) {
              sum += (float)-0.096144445;
            } else {
              sum += (float)0.011723734;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.45548373)) {
            if ( (data[14].fvalue < 1.1808927)) {
              sum += (float)-0.016102547;
            } else {
              sum += (float)-0.10384553;
            }
          } else {
            if ( (data[16].fvalue < 0.0013581347)) {
              sum += (float)0.018218677;
            } else {
              sum += (float)-0.014826383;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 3.5)) {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[8].fvalue < 2.5)) {
        if (  (data[4].fvalue < 1.0469422)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.33689034;
            } else {
              sum += (float)0.071746446;
            }
          } else {
            if ( (data[13].fvalue < 2.0865095e-06)) {
              sum += (float)0.30494443;
            } else {
              sum += (float)-0.0093689924;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.16952807)) {
            if (  (data[4].fvalue < 1.1129913)) {
              sum += (float)0.113322;
            } else {
              sum += (float)0.011587962;
            }
          } else {
            if ( (data[13].fvalue < 0.3071948)) {
              sum += (float)-0.006648567;
            } else {
              sum += (float)0.0076796073;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[17].fvalue < 0.15953305)) {
            if ( (data[0].fvalue < 3.0699716)) {
              sum += (float)0.071078822;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 2.1056097)) {
              sum += (float)0.1686682;
            } else {
              sum += (float)0.045415312;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.021189842)) {
            if ( (data[3].fvalue < 0.29490733)) {
              sum += (float)-0.011486781;
            } else {
              sum += (float)-0.18683276;
            }
          } else {
            if (  (data[4].fvalue < 1.124959)) {
              sum += (float)0.043858085;
            } else {
              sum += (float)-0.032829445;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.26421413)) {
        if ( (data[13].fvalue < 0.059097759)) {
          sum += (float)0.083265282;
        } else {
          sum += (float)-0.10694327;
        }
      } else {
        if ( (data[15].fvalue < 0.0078423796)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.086624436)) {
              sum += (float)0.12493718;
            } else {
              sum += (float)0.24902146;
            }
          } else {
            if ( (data[5].fvalue < 0.3655504)) {
              sum += (float)0.14456348;
            } else {
              sum += (float)0.04717515;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.098050684)) {
            if ( (data[15].fvalue < 0.12822768)) {
              sum += (float)-0.058143176;
            } else {
              sum += (float)0.041188575;
            }
          } else {
            if ( (data[13].fvalue < 0.23740491)) {
              sum += (float)0.06686765;
            } else {
              sum += (float)0.18033804;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.0647068)) {
      if ( (data[3].fvalue < 0.73782384)) {
        if ( (data[13].fvalue < 0.071141355)) {
          sum += (float)0.025841322;
        } else {
          if ( (data[14].fvalue < 0.013085924)) {
            sum += (float)0.094299078;
          } else {
            if ( (data[3].fvalue < 0.5767324)) {
              sum += (float)0.35121623;
            } else {
              sum += (float)0.17877555;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.0873491)) {
          if ( (data[3].fvalue < 1.3375988)) {
            if ( (data[14].fvalue < 0.19297022)) {
              sum += (float)-0.086845972;
            } else {
              sum += (float)0.1529104;
            }
          } else {
            if ( (data[13].fvalue < 0.52985644)) {
              sum += (float)-0.18835783;
            } else {
              sum += (float)-0.030422037;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.2939615)) {
            sum += (float)0.17494462;
          } else {
            sum += (float)0.010497688;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.73826665)) {
        if ( (data[15].fvalue < 0.087298676)) {
          if ( (data[13].fvalue < 0.34923565)) {
            if ( (data[17].fvalue < 0.12237132)) {
              sum += (float)-0.083291307;
            } else {
              sum += (float)0.050360393;
            }
          } else {
            if ( (data[3].fvalue < 0.71158576)) {
              sum += (float)0.038541388;
            } else {
              sum += (float)-0.057002358;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.342921)) {
            if ( (data[3].fvalue < 1.2557781)) {
              sum += (float)0.23556173;
            } else {
              sum += (float)0.033687532;
            }
          } else {
            if ( (data[14].fvalue < 0.32980299)) {
              sum += (float)0.043744046;
            } else {
              sum += (float)-0.029341469;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 5.5)) {
          if ( (data[14].fvalue < 0.12467362)) {
            if ( (data[16].fvalue < 0.21839735)) {
              sum += (float)0.029622706;
            } else {
              sum += (float)-0.075160027;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.12582298;
            } else {
              sum += (float)0.038607392;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.4024561)) {
            if ( (data[4].fvalue < 1.8279526)) {
              sum += (float)-0.054597672;
            } else {
              sum += (float)-0.16017729;
            }
          } else {
            if ( (data[13].fvalue < 1.8844787)) {
              sum += (float)-0.0054570362;
            } else {
              sum += (float)0.11049468;
            }
          }
        }
      }
    }
  }
  if ( (data[15].fvalue < 0.033349156)) {
    if ( (data[9].fvalue < 0.5)) {
      if ( (data[10].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.30103904)) {
          if ( (data[13].fvalue < 1.160239e-06)) {
            if (  (data[4].fvalue < 1.5754726)) {
              sum += (float)-0.055386458;
            } else {
              sum += (float)0.17693424;
            }
          } else {
            if ( (data[4].fvalue < 1.1554103)) {
              sum += (float)0.0057403874;
            } else {
              sum += (float)-0.012370998;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.25702545;
            } else {
              sum += (float)0.13796376;
            }
          } else {
            if ( (data[13].fvalue < 0.63907385)) {
              sum += (float)0.020087413;
            } else {
              sum += (float)0.091881432;
            }
          }
        }
      } else {
        sum += (float)-0.17702961;
      }
    } else {
      if ( (data[15].fvalue < 0.0074158423)) {
        if ( (data[14].fvalue < 0.0027533346)) {
          sum += (float)0.095900893;
        } else {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.26050434;
          } else {
            sum += (float)0.13766681;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.5219214)) {
          if (  (data[4].fvalue < 1.3121679)) {
            if ( (data[14].fvalue < 0.062746592)) {
              sum += (float)0.064095631;
            } else {
              sum += (float)0.20501247;
            }
          } else {
            if ( (data[13].fvalue < 0.095892891)) {
              sum += (float)0.12664756;
            } else {
              sum += (float)0.017545855;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.62155944)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0094377659;
            } else {
              sum += (float)-0.098841839;
            }
          } else {
            sum += (float)0.06204563;
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.10904582)) {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[4].fvalue < 2.2703691)) {
          if ( (data[14].fvalue < 0.019887552)) {
            if ( (data[13].fvalue < 0.21076238)) {
              sum += (float)-0.10393299;
            } else {
              sum += (float)-0.039434575;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0046973261;
            } else {
              sum += (float)-0.054884374;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.36351472)) {
            if ( (data[13].fvalue < 1.29008)) {
              sum += (float)0.12276355;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[17].fvalue < 1.582556)) {
              sum += (float)-0.082290053;
            } else {
              sum += (float)0.023017604;
            }
          }
        }
      } else {
        if ( (data[10].fvalue < 0.5)) {
          sum += (float)0.0064838878;
        } else {
          sum += (float)0.18844648;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.4689037)) {
        if ( (data[14].fvalue < 0.0038932511)) {
          if ( (data[13].fvalue < 0.032403387)) {
            if ( (data[3].fvalue < 0.12145996)) {
              sum += (float)-0.028398946;
            } else {
              sum += (float)-0.22187911;
            }
          } else {
            if ( (data[5].fvalue < 0.43497041)) {
              sum += (float)0.037252426;
            } else {
              sum += (float)-0.043935087;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.79130006)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.065200962;
            } else {
              sum += (float)0.19276334;
            }
          } else {
            if ( (data[13].fvalue < 0.18695968)) {
              sum += (float)-0.020300984;
            } else {
              sum += (float)0.053373575;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.42697698)) {
          if ( (data[16].fvalue < 0.00031620916)) {
            if ( (data[17].fvalue < 0.75999844)) {
              sum += (float)-0.055230763;
            } else {
              sum += (float)0.0066834162;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0058042537;
            } else {
              sum += (float)0.046279188;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.057502232)) {
            if ( (data[9].fvalue < 2.5)) {
              sum += (float)0.013506321;
            } else {
              sum += (float)-0.13515574;
            }
          } else {
            if ( (data[16].fvalue < 0.18668064)) {
              sum += (float)-0.043105591;
            } else {
              sum += (float)-0.0012182103;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.12218556)) {
    if ( (data[4].fvalue < 0.99999428)) {
      if ( (data[14].fvalue < 1.79204e-05)) {
        if ( (data[13].fvalue < 0.036523454)) {
          if (  (data[4].fvalue < 0.96276033)) {
            if ( (data[3].fvalue < 0.050557598)) {
              sum += (float)-0.35568827;
            } else {
              sum += (float)0.12591217;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.27383706;
            } else {
              sum += (float)0.052317236;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.17820245)) {
            if ( (data[13].fvalue < 0.077523284)) {
              sum += (float)-0.053666789;
            } else {
              sum += (float)-0.2113128;
            }
          } else {
            if (  (data[4].fvalue < 0.95186031)) {
              sum += (float)0.050212767;
            } else {
              sum += (float)0.24758919;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.040675197)) {
          if ( (data[14].fvalue < 0.039154027)) {
            if ( (data[3].fvalue < 0.089600936)) {
              sum += (float)-0.035095014;
            } else {
              sum += (float)-0.1151219;
            }
          } else {
            if ( (data[3].fvalue < 0.13293383)) {
              sum += (float)0.14723347;
            } else {
              sum += (float)-0.011356464;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.15040985)) {
            sum += (float)0.21544765;
          } else {
            if ( (data[4].fvalue < 0.97115737)) {
              sum += (float)0.026269691;
            } else {
              sum += (float)-0.043249518;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.7472656)) {
        if ( (data[14].fvalue < 0.13427645)) {
          if ( (data[3].fvalue < 0.21210387)) {
            if ( (data[13].fvalue < 0.02841075)) {
              sum += (float)-0.048416212;
            } else {
              sum += (float)0.0043990687;
            }
          } else {
            if (  (data[4].fvalue < 1.0793205)) {
              sum += (float)-0.1928533;
            } else {
              sum += (float)-0.060497031;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.35503161)) {
            if (  (data[4].fvalue < 1.1315256)) {
              sum += (float)0.087388843;
            } else {
              sum += (float)0.01207211;
            }
          } else {
            if ( (data[15].fvalue < 0.19908971)) {
              sum += (float)-0.041085172;
            } else {
              sum += (float)-0.0019995614;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 2.1703904)) {
          if ( (data[15].fvalue < 0.081022725)) {
            if ( (data[14].fvalue < 0.36065856)) {
              sum += (float)0.11321057;
            } else {
              sum += (float)-0.040338542;
            }
          } else {
            if ( (data[2].fvalue < 0.24602978)) {
              sum += (float)-0.021604959;
            } else {
              sum += (float)0.0091161747;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.80047685)) {
            if ( (data[14].fvalue < 0.091289677)) {
              sum += (float)-0.17309676;
            } else {
              sum += (float)-0.052428816;
            }
          } else {
            if ( (data[15].fvalue < 0.50843132)) {
              sum += (float)0.18726328;
            } else {
              sum += (float)0.02010229;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.030823674)) {
      if ( (data[0].fvalue < 6.5107841)) {
        sum += (float)-0.19228451;
      } else {
        if ( (data[6].fvalue < 0.026308302)) {
          sum += (float)-0.11488754;
        } else {
          sum += (float)0.03474097;
        }
      }
    } else {
      if ( (data[4].fvalue < 1.6942713)) {
        if ( (data[11].fvalue < 2.5)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.23622321)) {
              sum += (float)0.0015307438;
            } else {
              sum += (float)0.035920378;
            }
          } else {
            if (  (data[4].fvalue < 1.3535383)) {
              sum += (float)0.32484445;
            } else {
              sum += (float)0.060011949;
            }
          }
        } else {
          sum += (float)-0.20857342;
        }
      } else {
        if ( (data[17].fvalue < 0.65674019)) {
          if ( (data[16].fvalue < 0.46583831)) {
            if ( (data[15].fvalue < 0.92118073)) {
              sum += (float)-0.037908524;
            } else {
              sum += (float)0.0093366113;
            }
          } else {
            if (  (data[4].fvalue < 2.6461062)) {
              sum += (float)0.021493746;
            } else {
              sum += (float)-0.016315611;
            }
          }
        } else {
          if ( (data[0].fvalue < 4.4665442)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.031636633;
            } else {
              sum += (float)0.088587634;
            }
          } else {
            if ( (data[6].fvalue < 0.61591291)) {
              sum += (float)0.03045601;
            } else {
              sum += (float)-0.012262833;
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
