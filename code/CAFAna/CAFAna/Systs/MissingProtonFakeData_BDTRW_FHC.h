#include "BDTReweighter.h"
class MissingProtonFakeData_BDTRW_FHC : public BDTReweighter {
 public :
 MissingProtonFakeData_BDTRW_FHC() : BDTReweighter(1.0500955) {;}
  ~MissingProtonFakeData_BDTRW_FHC() {;}

size_t get_num_output_group(void) {
  return 1;
}

size_t get_num_feature(void) {
  return 8;
}

static inline float pred_transform(float margin) {
  return margin;
}

   float predict(union BDTReweighter::BDTReweighterFeature* data, int pred_margin) {
  float sum = 0.0f;
  unsigned int tmp;
  if ( (data[6].fvalue < 0.47068101)) {
    if ( (data[6].fvalue < 0.25608724)) {
      if ( (data[6].fvalue < 4.4372558e-05)) {
        if ( (data[5].fvalue < 7.2266006)) {
          if ( (data[7].fvalue < 0.1215435)) {
            sum += (float)0.13914905;
          } else {
            if ( (data[5].fvalue < 3.1871986)) {
              sum += (float)0.13517192;
            } else {
              sum += (float)0.13844235;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.18608165)) {
            sum += (float)0.13659452;
          } else {
            if ( (data[7].fvalue < 0.64311916)) {
              sum += (float)0.12561437;
            } else {
              sum += (float)0.13393238;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.11263568)) {
          if ( (data[6].fvalue < 0.04563611)) {
            if ( (data[6].fvalue < 0.020774078)) {
              sum += (float)0.072690256;
            } else {
              sum += (float)0.080006555;
            }
          } else {
            if ( (data[7].fvalue < 0.33503351)) {
              sum += (float)0.089415841;
            } else {
              sum += (float)0.081631191;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.20474464)) {
            if ( (data[7].fvalue < 0.3067213)) {
              sum += (float)0.10320866;
            } else {
              sum += (float)0.089219391;
            }
          } else {
            if ( (data[7].fvalue < 0.3340134)) {
              sum += (float)0.120858;
            } else {
              sum += (float)0.099879287;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.36848378)) {
        if ( (data[7].fvalue < 0.36063069)) {
          if ( (data[6].fvalue < 0.31652653)) {
            if ( (data[7].fvalue < 0.28630596)) {
              sum += (float)0.13944936;
            } else {
              sum += (float)0.12751807;
            }
          } else {
            if ( (data[5].fvalue < 6.4966545)) {
              sum += (float)0.15789859;
            } else {
              sum += (float)0.13490376;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.3152616)) {
            if ( (data[7].fvalue < 0.55451238)) {
              sum += (float)0.11242558;
            } else {
              sum += (float)0.10049332;
            }
          } else {
            if ( (data[7].fvalue < 0.49360123)) {
              sum += (float)0.13048214;
            } else {
              sum += (float)0.11754809;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.41792157)) {
          if ( (data[5].fvalue < 6.3387613)) {
            if ( (data[6].fvalue < 0.43877873)) {
              sum += (float)0.17492704;
            } else {
              sum += (float)0.19478516;
            }
          } else {
            if ( (data[7].fvalue < 0.17306647)) {
              sum += (float)0.15455094;
            } else {
              sum += (float)0.11998407;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.5987225)) {
            if ( (data[6].fvalue < 0.44779927)) {
              sum += (float)0.13776544;
            } else {
              sum += (float)0.15418524;
            }
          } else {
            if ( (data[5].fvalue < 7.4203672)) {
              sum += (float)0.1077815;
            } else {
              sum += (float)0.091523871;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.80551964)) {
      if ( (data[6].fvalue < 0.62075758)) {
        if ( (data[7].fvalue < 0.47078723)) {
          if ( (data[5].fvalue < 6.6648417)) {
            if ( (data[6].fvalue < 0.53313923)) {
              sum += (float)0.21671671;
            } else {
              sum += (float)0.24463004;
            }
          } else {
            if ( (data[7].fvalue < 0.18714291)) {
              sum += (float)0.17825513;
            } else {
              sum += (float)0.13395335;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.8889198)) {
            if ( (data[6].fvalue < 0.53600585)) {
              sum += (float)0.16562492;
            } else {
              sum += (float)0.1832072;
            }
          } else {
            if ( (data[5].fvalue < 7.9504929)) {
              sum += (float)0.12531874;
            } else {
              sum += (float)0.10220683;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.52192968)) {
          if ( (data[5].fvalue < 6.3045754)) {
            if ( (data[6].fvalue < 0.69509077)) {
              sum += (float)0.27638581;
            } else {
              sum += (float)0.31187955;
            }
          } else {
            if ( (data[7].fvalue < 0.18563268)) {
              sum += (float)0.21279411;
            } else {
              sum += (float)0.16476576;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.7627625)) {
            if ( (data[6].fvalue < 0.69644916)) {
              sum += (float)0.20288278;
            } else {
              sum += (float)0.22921598;
            }
          } else {
            if ( (data[5].fvalue < 7.9889665)) {
              sum += (float)0.15696418;
            } else {
              sum += (float)0.12490465;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.4043821)) {
        if ( (data[5].fvalue < 6.5130954)) {
          if ( (data[7].fvalue < 0.55956769)) {
            if ( (data[6].fvalue < 0.99766815)) {
              sum += (float)0.36014497;
            } else {
              sum += (float)0.43246442;
            }
          } else {
            if ( (data[6].fvalue < 1.0327339)) {
              sum += (float)0.27161935;
            } else {
              sum += (float)0.33318231;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.21259716)) {
            if ( (data[6].fvalue < 1.0138416)) {
              sum += (float)0.24791248;
            } else {
              sum += (float)0.29134613;
            }
          } else {
            if ( (data[5].fvalue < 8.466568)) {
              sum += (float)0.24032229;
            } else {
              sum += (float)0.18481678;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 6.5754733)) {
          if ( (data[6].fvalue < 1.8971554)) {
            if ( (data[7].fvalue < 0.59685993)) {
              sum += (float)0.54723823;
            } else {
              sum += (float)0.43487647;
            }
          } else {
            if ( (data[6].fvalue < 2.1562276)) {
              sum += (float)0.60915673;
            } else {
              sum += (float)0.71585542;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.0749121)) {
            if ( (data[7].fvalue < 0.19940588)) {
              sum += (float)0.34546763;
            } else {
              sum += (float)0.28087366;
            }
          } else {
            if ( (data[5].fvalue < 9.1354122)) {
              sum += (float)0.47374198;
            } else {
              sum += (float)0.36480212;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.478338)) {
    if ( (data[6].fvalue < 0.23451832)) {
      if ( (data[6].fvalue < 6.9702146e-05)) {
        if ( (data[5].fvalue < 7.0758486)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[5].fvalue < 3.0400524)) {
              sum += (float)0.094378464;
            } else {
              sum += (float)0.096652292;
            }
          } else {
            if ( (data[7].fvalue < 0.26497126)) {
              sum += (float)0.097876415;
            } else {
              sum += (float)0.093856543;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.19654441)) {
            if ( (data[5].fvalue < 11.309088)) {
              sum += (float)0.092937067;
            } else {
              sum += (float)0.096858203;
            }
          } else {
            if ( (data[5].fvalue < 12.067766)) {
              sum += (float)0.090460442;
            } else {
              sum += (float)0.085481837;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.10865524)) {
          if ( (data[6].fvalue < 0.04563684)) {
            if ( (data[6].fvalue < 0.020782411)) {
              sum += (float)0.050872806;
            } else {
              sum += (float)0.056006823;
            }
          } else {
            if ( (data[7].fvalue < 0.48942679)) {
              sum += (float)0.06204813;
            } else {
              sum += (float)0.054403551;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.40650886)) {
            if ( (data[6].fvalue < 0.18227872)) {
              sum += (float)0.069993749;
            } else {
              sum += (float)0.078744143;
            }
          } else {
            if ( (data[7].fvalue < 0.63640314)) {
              sum += (float)0.063341923;
            } else {
              sum += (float)0.055587213;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.34130394)) {
        if ( (data[7].fvalue < 0.34242293)) {
          if ( (data[6].fvalue < 0.28616846)) {
            if ( (data[5].fvalue < 6.6295543)) {
              sum += (float)0.093599461;
            } else {
              sum += (float)0.082784541;
            }
          } else {
            if ( (data[5].fvalue < 6.4716363)) {
              sum += (float)0.10463341;
            } else {
              sum += (float)0.092311643;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.44480616)) {
            if ( (data[5].fvalue < 6.9600925)) {
              sum += (float)0.085012384;
            } else {
              sum += (float)0.053193714;
            }
          } else {
            if ( (data[6].fvalue < 0.30093652)) {
              sum += (float)0.070286028;
            } else {
              sum += (float)0.078504927;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.36516404)) {
          if ( (data[5].fvalue < 6.7956781)) {
            if ( (data[6].fvalue < 0.40926629)) {
              sum += (float)0.11763472;
            } else {
              sum += (float)0.1349806;
            }
          } else {
            if ( (data[7].fvalue < 0.12955174)) {
              sum += (float)0.10835155;
            } else {
              sum += (float)0.083900899;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.41202605)) {
            if ( (data[5].fvalue < 7.2545605)) {
              sum += (float)0.092755407;
            } else {
              sum += (float)0.056449398;
            }
          } else {
            if ( (data[7].fvalue < 0.56179392)) {
              sum += (float)0.11204042;
            } else {
              sum += (float)0.097075008;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.85794139)) {
      if ( (data[6].fvalue < 0.65461373)) {
        if ( (data[7].fvalue < 0.42404658)) {
          if ( (data[5].fvalue < 6.3663473)) {
            if ( (data[6].fvalue < 0.57182014)) {
              sum += (float)0.15983477;
            } else {
              sum += (float)0.18732883;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.13205406;
            } else {
              sum += (float)0.10358728;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.1805058)) {
            if ( (data[7].fvalue < 0.54943311)) {
              sum += (float)0.13918917;
            } else {
              sum += (float)0.12159414;
            }
          } else {
            if ( (data[5].fvalue < 7.9211254)) {
              sum += (float)0.09480159;
            } else {
              sum += (float)0.072915345;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.47358334)) {
          if ( (data[5].fvalue < 5.408884)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.23200344;
            } else {
              sum += (float)0.19046542;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.16486792;
            } else {
              sum += (float)0.1283675;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.8639145)) {
            if ( (data[7].fvalue < 0.64031118)) {
              sum += (float)0.17436886;
            } else {
              sum += (float)0.15217182;
            }
          } else {
            if ( (data[5].fvalue < 7.4330626)) {
              sum += (float)0.12073398;
            } else {
              sum += (float)0.08862026;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 7.2027302)) {
        if ( (data[6].fvalue < 1.6432662)) {
          if ( (data[7].fvalue < 0.48355556)) {
            if ( (data[5].fvalue < 5.2737465)) {
              sum += (float)0.30629066;
            } else {
              sum += (float)0.22316048;
            }
          } else {
            if ( (data[6].fvalue < 1.1978276)) {
              sum += (float)0.20939289;
            } else {
              sum += (float)0.26776972;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.1064074)) {
            if ( (data[6].fvalue < 1.8054789)) {
              sum += (float)0.35294831;
            } else {
              sum += (float)0.41085035;
            }
          } else {
            if ( (data[6].fvalue < 2.3831277)) {
              sum += (float)0.46187449;
            } else {
              sum += (float)0.53262466;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 1.7673917)) {
          if ( (data[7].fvalue < 0.24737212)) {
            if ( (data[6].fvalue < 1.1947939)) {
              sum += (float)0.17679042;
            } else {
              sum += (float)0.22069423;
            }
          } else {
            if ( (data[6].fvalue < 1.1665056)) {
              sum += (float)0.11926364;
            } else {
              sum += (float)0.16145524;
            }
          }
        } else {
          if ( (data[5].fvalue < 15.534826)) {
            if ( (data[6].fvalue < 2.4492023)) {
              sum += (float)0.25138307;
            } else {
              sum += (float)0.31012404;
            }
          } else {
            if ( (data[7].fvalue < 0.19637951)) {
              sum += (float)0.26233342;
            } else {
              sum += (float)0.20657754;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.50068265)) {
    if ( (data[6].fvalue < 0.27706859)) {
      if ( (data[6].fvalue < 0.00011157226)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[5].fvalue < 7.6914597)) {
            if ( (data[5].fvalue < 2.9126132)) {
              sum += (float)0.065983169;
            } else {
              sum += (float)0.067551166;
            }
          } else {
            if ( (data[7].fvalue < 0.16775611)) {
              sum += (float)0.067001268;
            } else {
              sum += (float)0.061673895;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.96645987)) {
            if ( (data[5].fvalue < 0.51158923)) {
              sum += (float)0.075362787;
            } else {
              sum += (float)0.064765625;
            }
          } else {
            if ( (data[7].fvalue < 0.32075763)) {
              sum += (float)0.068757616;
            } else {
              sum += (float)0.065610185;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.18227521)) {
          if ( (data[6].fvalue < 0.05565875)) {
            if ( (data[6].fvalue < 0.029823057)) {
              sum += (float)0.036313172;
            } else {
              sum += (float)0.040333334;
            }
          } else {
            if ( (data[7].fvalue < 0.24497101)) {
              sum += (float)0.04733092;
            } else {
              sum += (float)0.041694358;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.29276124)) {
            if ( (data[6].fvalue < 0.2340036)) {
              sum += (float)0.055975929;
            } else {
              sum += (float)0.064694352;
            }
          } else {
            if ( (data[7].fvalue < 0.51666021)) {
              sum += (float)0.051702995;
            } else {
              sum += (float)0.044304349;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.39035034)) {
        if ( (data[7].fvalue < 0.31572258)) {
          if ( (data[6].fvalue < 0.32358059)) {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.071522705;
            } else {
              sum += (float)0.060421355;
            }
          } else {
            if ( (data[5].fvalue < 6.2989807)) {
              sum += (float)0.080952369;
            } else {
              sum += (float)0.066473566;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.64587939)) {
            if ( (data[5].fvalue < 6.8755007)) {
              sum += (float)0.062680289;
            } else {
              sum += (float)0.034331068;
            }
          } else {
            if ( (data[5].fvalue < 7.5365252)) {
              sum += (float)0.051193293;
            } else {
              sum += (float)0.02872839;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.3981939)) {
          if ( (data[5].fvalue < 6.3228722)) {
            if ( (data[6].fvalue < 0.44649827)) {
              sum += (float)0.089099512;
            } else {
              sum += (float)0.10160062;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.082151189;
            } else {
              sum += (float)0.062094335;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.4559164)) {
            if ( (data[7].fvalue < 0.62087357)) {
              sum += (float)0.076714464;
            } else {
              sum += (float)0.065824971;
            }
          } else {
            if ( (data[5].fvalue < 7.512785)) {
              sum += (float)0.051820789;
            } else {
              sum += (float)0.037472971;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.93234885)) {
      if ( (data[6].fvalue < 0.71832228)) {
        if ( (data[7].fvalue < 0.44662023)) {
          if ( (data[5].fvalue < 5.2397375)) {
            if ( (data[6].fvalue < 0.60039771)) {
              sum += (float)0.11753858;
            } else {
              sum += (float)0.14032294;
            }
          } else {
            if ( (data[7].fvalue < 0.21309504)) {
              sum += (float)0.094735004;
            } else {
              sum += (float)0.071592033;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.59422016)) {
            if ( (data[7].fvalue < 0.64459062)) {
              sum += (float)0.090713643;
            } else {
              sum += (float)0.077331573;
            }
          } else {
            if ( (data[5].fvalue < 5.8708973)) {
              sum += (float)0.1010256;
            } else {
              sum += (float)0.060844697;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 6.9944878)) {
          if ( (data[7].fvalue < 0.43784645)) {
            if ( (data[5].fvalue < 4.9350128)) {
              sum += (float)0.17832062;
            } else {
              sum += (float)0.13403779;
            }
          } else {
            if ( (data[7].fvalue < 0.62938184)) {
              sum += (float)0.13727365;
            } else {
              sum += (float)0.11470062;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.18183412)) {
            if ( (data[2].fvalue < 0.5)) {
              sum += (float)0.11327365;
            } else {
              sum += (float)0.12757266;
            }
          } else {
            if ( (data[5].fvalue < 9.4554014)) {
              sum += (float)0.090230897;
            } else {
              sum += (float)0.069469698;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 5.8140593)) {
        if ( (data[6].fvalue < 1.5715046)) {
          if ( (data[7].fvalue < 0.62880522)) {
            if ( (data[6].fvalue < 1.1663928)) {
              sum += (float)0.19775385;
            } else {
              sum += (float)0.25776517;
            }
          } else {
            if ( (data[6].fvalue < 1.2928765)) {
              sum += (float)0.14562219;
            } else {
              sum += (float)0.18279156;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.3399103)) {
            if ( (data[6].fvalue < 1.9443483)) {
              sum += (float)0.23462795;
            } else {
              sum += (float)0.29329282;
            }
          } else {
            if ( (data[6].fvalue < 1.8073957)) {
              sum += (float)0.27938831;
            } else {
              sum += (float)0.36359209;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 1.7825429)) {
          if ( (data[1].fvalue < 0.5)) {
            if ( (data[6].fvalue < 1.2979133)) {
              sum += (float)0.14744315;
            } else {
              sum += (float)0.19191056;
            }
          } else {
            if ( (data[6].fvalue < 1.1283876)) {
              sum += (float)0.096936427;
            } else {
              sum += (float)0.12255529;
            }
          }
        } else {
          if ( (data[5].fvalue < 8.9182873)) {
            if ( (data[6].fvalue < 3.0063231)) {
              sum += (float)0.22647519;
            } else {
              sum += (float)0.31232655;
            }
          } else {
            if ( (data[6].fvalue < 2.3449287)) {
              sum += (float)0.1538735;
            } else {
              sum += (float)0.18903945;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.45890385)) {
    if ( (data[6].fvalue < 0.23402457)) {
      if ( (data[6].fvalue < 0.00018325806)) {
        if ( (data[5].fvalue < 6.9073868)) {
          if ( (data[7].fvalue < 0.25893912)) {
            if ( (data[6].fvalue < 1.6326905e-05)) {
              sum += (float)0.047620069;
            } else {
              sum += (float)0.039805911;
            }
          } else {
            if ( (data[5].fvalue < 3.5789373)) {
              sum += (float)0.046057142;
            } else {
              sum += (float)0.04838204;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.14705966)) {
            if ( (data[5].fvalue < 10.722805)) {
              sum += (float)0.044234239;
            } else {
              sum += (float)0.048325893;
            }
          } else {
            if ( (data[7].fvalue < 0.71304899)) {
              sum += (float)0.04335548;
            } else {
              sum += (float)0.051424082;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.10865524)) {
          if ( (data[6].fvalue < 0.029823486)) {
            if ( (data[7].fvalue < 0.27960739)) {
              sum += (float)0.026266834;
            } else {
              sum += (float)0.021095052;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.029706931;
            } else {
              sum += (float)0.024576986;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.2638278)) {
            if ( (data[5].fvalue < 6.4860611)) {
              sum += (float)0.037847687;
            } else {
              sum += (float)0.030048763;
            }
          } else {
            if ( (data[5].fvalue < 6.9229641)) {
              sum += (float)0.031635053;
            } else {
              sum += (float)0.0093800444;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.33067924)) {
        if ( (data[7].fvalue < 0.3962481)) {
          if ( (data[5].fvalue < 1.2659242)) {
            if ( (data[5].fvalue < 0.89179498)) {
              sum += (float)0.030139217;
            } else {
              sum += (float)0.037145849;
            }
          } else {
            if ( (data[5].fvalue < 6.4513955)) {
              sum += (float)0.048558433;
            } else {
              sum += (float)0.040400166;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.72416043)) {
            if ( (data[5].fvalue < 6.1018763)) {
              sum += (float)0.037006728;
            } else {
              sum += (float)0.019861612;
            }
          } else {
            if ( (data[7].fvalue < 0.83198583)) {
              sum += (float)0.03007655;
            } else {
              sum += (float)0.02517066;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.44469404)) {
          if ( (data[5].fvalue < 6.302011)) {
            if ( (data[5].fvalue < 1.3712306)) {
              sum += (float)0.04502346;
            } else {
              sum += (float)0.059930336;
            }
          } else {
            if ( (data[7].fvalue < 0.19064879)) {
              sum += (float)0.051400725;
            } else {
              sum += (float)0.031604022;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.73502076)) {
            if ( (data[5].fvalue < 6.0251341)) {
              sum += (float)0.046215273;
            } else {
              sum += (float)0.026317673;
            }
          } else {
            if ( (data[5].fvalue < 1.5221118)) {
              sum += (float)0.050223328;
            } else {
              sum += (float)0.036402177;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.77482438)) {
      if ( (data[7].fvalue < 0.37940893)) {
        if ( (data[5].fvalue < 7.2749128)) {
          if ( (data[6].fvalue < 0.58934557)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.08351171;
            } else {
              sum += (float)0.066634312;
            }
          } else {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.10800886;
            } else {
              sum += (float)0.080538221;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.14440762)) {
            if ( (data[6].fvalue < 0.56305873)) {
              sum += (float)0.059860431;
            } else {
              sum += (float)0.070574388;
            }
          } else {
            if ( (data[5].fvalue < 11.97625)) {
              sum += (float)0.050555825;
            } else {
              sum += (float)0.037613049;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.56466204)) {
          if ( (data[5].fvalue < 5.3450708)) {
            if ( (data[7].fvalue < 0.59358859)) {
              sum += (float)0.061606221;
            } else {
              sum += (float)0.053062584;
            }
          } else {
            if ( (data[5].fvalue < 8.7530489)) {
              sum += (float)0.038619291;
            } else {
              sum += (float)0.022020677;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.2039847)) {
            if ( (data[7].fvalue < 0.58382237)) {
              sum += (float)0.080152296;
            } else {
              sum += (float)0.065846704;
            }
          } else {
            if ( (data[5].fvalue < 18.134373)) {
              sum += (float)0.03935875;
            } else {
              sum += (float)0.0227951;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.3035412)) {
        if ( (data[5].fvalue < 5.6611891)) {
          if ( (data[7].fvalue < 0.44000989)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.15049779;
            } else {
              sum += (float)0.10409986;
            }
          } else {
            if ( (data[7].fvalue < 0.73158467)) {
              sum += (float)0.11086159;
            } else {
              sum += (float)0.086294957;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.2908386)) {
            if ( (data[5].fvalue < 11.203178)) {
              sum += (float)0.10188195;
            } else {
              sum += (float)0.079141557;
            }
          } else {
            if ( (data[5].fvalue < 11.628622)) {
              sum += (float)0.063623093;
            } else {
              sum += (float)0.044000491;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 8.0481081)) {
          if ( (data[6].fvalue < 1.864112)) {
            if ( (data[7].fvalue < 0.70065004)) {
              sum += (float)0.18829064;
            } else {
              sum += (float)0.13186465;
            }
          } else {
            if ( (data[6].fvalue < 2.3430357)) {
              sum += (float)0.21012795;
            } else {
              sum += (float)0.26401761;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.0059447)) {
            if ( (data[7].fvalue < 0.19577211)) {
              sum += (float)0.12605651;
            } else {
              sum += (float)0.079787374;
            }
          } else {
            if ( (data[5].fvalue < 14.258011)) {
              sum += (float)0.14234321;
            } else {
              sum += (float)0.11208069;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.51029551)) {
    if ( (data[6].fvalue < 0.29958612)) {
      if ( (data[6].fvalue < 0.00033731078)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[7].fvalue < 0.76682729)) {
            if ( (data[5].fvalue < 7.7564945)) {
              sum += (float)0.032454394;
            } else {
              sum += (float)0.031237371;
            }
          } else {
            if ( (data[5].fvalue < 1.5580246)) {
              sum += (float)0.044659518;
            } else {
              sum += (float)0.03411543;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.0122902)) {
            if ( (data[5].fvalue < 0.51701725)) {
              sum += (float)0.038701862;
            } else {
              sum += (float)0.031312194;
            }
          } else {
            if ( (data[6].fvalue < 1.9683837e-05)) {
              sum += (float)0.033817891;
            } else {
              sum += (float)0.028610226;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.1817843)) {
          if ( (data[6].fvalue < 0.1132132)) {
            if ( (data[5].fvalue < 6.3026633)) {
              sum += (float)0.020077448;
            } else {
              sum += (float)0.015064712;
            }
          } else {
            if ( (data[5].fvalue < 1.1312826)) {
              sum += (float)0.016433029;
            } else {
              sum += (float)0.024389474;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.26061264)) {
            if ( (data[6].fvalue < 0.22485791)) {
              sum += (float)0.028021211;
            } else {
              sum += (float)0.032002587;
            }
          } else {
            if ( (data[5].fvalue < 6.960783)) {
              sum += (float)0.025022179;
            } else {
              sum += (float)0.0070010344;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.29597527)) {
        if ( (data[6].fvalue < 0.38196796)) {
          if ( (data[1].fvalue < 0.5)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.040073045;
            } else {
              sum += (float)0.035649195;
            }
          } else {
            if ( (data[5].fvalue < 5.2061529)) {
              sum += (float)0.039092805;
            } else {
              sum += (float)0.022447228;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.0339866)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.051753387;
            } else {
              sum += (float)0.042793725;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.041589223;
            } else {
              sum += (float)0.030589299;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.37868452)) {
          if ( (data[5].fvalue < 6.6825142)) {
            if ( (data[7].fvalue < 0.55946088)) {
              sum += (float)0.031687725;
            } else {
              sum += (float)0.025775461;
            }
          } else {
            if ( (data[5].fvalue < 12.108959)) {
              sum += (float)0.015321065;
            } else {
              sum += (float)0.0066570253;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.2915387)) {
            if ( (data[7].fvalue < 0.49728233)) {
              sum += (float)0.040370997;
            } else {
              sum += (float)0.033768192;
            }
          } else {
            if ( (data[5].fvalue < 11.102818)) {
              sum += (float)0.02324998;
            } else {
              sum += (float)0.015040659;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.96483409)) {
      if ( (data[7].fvalue < 0.39705807)) {
        if ( (data[5].fvalue < 4.9350128)) {
          if ( (data[6].fvalue < 0.73983508)) {
            if ( (data[5].fvalue < 3.0038095)) {
              sum += (float)0.057148326;
            } else {
              sum += (float)0.073191583;
            }
          } else {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.10315325;
            } else {
              sum += (float)0.068195723;
            }
          }
        } else {
          if ( (data[1].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.70299947)) {
              sum += (float)0.047855165;
            } else {
              sum += (float)0.06274242;
            }
          } else {
            if ( (data[7].fvalue < 0.12468089)) {
              sum += (float)0.052327856;
            } else {
              sum += (float)0.037536543;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.67341578)) {
          if ( (data[5].fvalue < 5.6740904)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.047225282;
            } else {
              sum += (float)0.033857789;
            }
          } else {
            if ( (data[5].fvalue < 7.9211254)) {
              sum += (float)0.030830832;
            } else {
              sum += (float)0.02016155;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.8666325)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.062111415;
            } else {
              sum += (float)0.037908319;
            }
          } else {
            if ( (data[5].fvalue < 8.0986586)) {
              sum += (float)0.03641412;
            } else {
              sum += (float)0.025023626;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 5.4834194)) {
        if ( (data[6].fvalue < 1.5272576)) {
          if ( (data[7].fvalue < 0.59565866)) {
            if ( (data[6].fvalue < 1.1071429)) {
              sum += (float)0.10113277;
            } else {
              sum += (float)0.13942966;
            }
          } else {
            if ( (data[7].fvalue < 0.7844547)) {
              sum += (float)0.086331099;
            } else {
              sum += (float)0.0634083;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.63205206)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.22886167;
            } else {
              sum += (float)0.11447877;
            }
          } else {
            if ( (data[6].fvalue < 2.086349)) {
              sum += (float)0.11882807;
            } else {
              sum += (float)0.17491136;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 1.677263)) {
          if ( (data[7].fvalue < 0.29260993)) {
            if ( (data[5].fvalue < 13.923812)) {
              sum += (float)0.08567223;
            } else {
              sum += (float)0.056771312;
            }
          } else {
            if ( (data[5].fvalue < 10.293782)) {
              sum += (float)0.058487367;
            } else {
              sum += (float)0.03485474;
            }
          }
        } else {
          if ( (data[1].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.27502564)) {
              sum += (float)0.13765615;
            } else {
              sum += (float)0.10775786;
            }
          } else {
            if ( (data[5].fvalue < 9.51439)) {
              sum += (float)0.10799015;
            } else {
              sum += (float)0.075538144;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.44921684)) {
    if ( (data[6].fvalue < 0.27246469)) {
      if ( (data[6].fvalue < 0.00042990112)) {
        if ( (data[7].fvalue < 0.22899902)) {
          if ( (data[6].fvalue < 2.0996093e-05)) {
            if ( (data[3].fvalue < 0.5)) {
              sum += (float)0.023514271;
            } else {
              sum += (float)0.022580812;
            }
          } else {
            if ( (data[6].fvalue < 4.4372558e-05)) {
              sum += (float)0.0059256237;
            } else {
              sum += (float)0.020458579;
            }
          }
        } else {
          if ( (data[5].fvalue < 12.249512)) {
            if ( (data[7].fvalue < 0.74919152)) {
              sum += (float)0.022487704;
            } else {
              sum += (float)0.0239689;
            }
          } else {
            if ( (data[3].fvalue < 0.5)) {
              sum += (float)0.018460641;
            } else {
              sum += (float)0.0070395046;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.13614474)) {
          if ( (data[7].fvalue < 0.54573905)) {
            if ( (data[6].fvalue < 0.020772705)) {
              sum += (float)0.011376643;
            } else {
              sum += (float)0.015041221;
            }
          } else {
            if ( (data[5].fvalue < 7.503334)) {
              sum += (float)0.0093978494;
            } else {
              sum += (float)-0.0038879702;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.49174935)) {
            if ( (data[5].fvalue < 1.1869564)) {
              sum += (float)0.012640766;
            } else {
              sum += (float)0.019710429;
            }
          } else {
            if ( (data[7].fvalue < 0.7131902)) {
              sum += (float)0.014197878;
            } else {
              sum += (float)0.0088345613;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.26519817)) {
        if ( (data[6].fvalue < 0.36715478)) {
          if ( (data[5].fvalue < 5.5655513)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.027805367;
            } else {
              sum += (float)0.024241908;
            }
          } else {
            if ( (data[7].fvalue < 0.10833412)) {
              sum += (float)0.025510976;
            } else {
              sum += (float)0.014181595;
            }
          }
        } else {
          if ( (data[5].fvalue < 4.8641939)) {
            if ( (data[5].fvalue < 2.9264505)) {
              sum += (float)0.029200442;
            } else {
              sum += (float)0.034751549;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.026837565;
            } else {
              sum += (float)0.020243596;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.47349894)) {
          if ( (data[5].fvalue < 1.304831)) {
            if ( (data[5].fvalue < 0.88091063)) {
              sum += (float)0.010175707;
            } else {
              sum += (float)0.018934537;
            }
          } else {
            if ( (data[5].fvalue < 4.9454823)) {
              sum += (float)0.026068764;
            } else {
              sum += (float)0.011682507;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.32123068)) {
            if ( (data[7].fvalue < 0.76918203)) {
              sum += (float)0.016669251;
            } else {
              sum += (float)0.011531655;
            }
          } else {
            if ( (data[7].fvalue < 0.79013932)) {
              sum += (float)0.021023668;
            } else {
              sum += (float)0.014525644;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.89537156)) {
      if ( (data[5].fvalue < 2.2530231)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.74833274)) {
            if ( (data[7].fvalue < 0.65776813)) {
              sum += (float)0.033466689;
            } else {
              sum += (float)0.026138594;
            }
          } else {
            if ( (data[7].fvalue < 0.61190248)) {
              sum += (float)0.051600892;
            } else {
              sum += (float)0.0383261;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.42830044)) {
            if ( (data[5].fvalue < 2.0032513)) {
              sum += (float)0.013592638;
            } else {
              sum += (float)0.023934685;
            }
          } else {
            if ( (data[5].fvalue < 1.7078722)) {
              sum += (float)0.020462312;
            } else {
              sum += (float)0.041480459;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.8832049)) {
          if ( (data[7].fvalue < 0.68764067)) {
            if ( (data[6].fvalue < 0.63831919)) {
              sum += (float)0.041952729;
            } else {
              sum += (float)0.058814924;
            }
          } else {
            if ( (data[7].fvalue < 0.77179909)) {
              sum += (float)0.033184614;
            } else {
              sum += (float)0.021295216;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.21656844)) {
            if ( (data[6].fvalue < 0.70952439)) {
              sum += (float)0.032012142;
            } else {
              sum += (float)0.043596826;
            }
          } else {
            if ( (data[5].fvalue < 8.0995102)) {
              sum += (float)0.028651429;
            } else {
              sum += (float)0.015955599;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 5.4362125)) {
        if ( (data[5].fvalue < 3.1941905)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[6].fvalue < 1.5167763)) {
              sum += (float)0.059117895;
            } else {
              sum += (float)0.091812223;
            }
          } else {
            if ( (data[7].fvalue < 0.52795231)) {
              sum += (float)0.012332303;
            } else {
              sum += (float)0.036710553;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.79457033)) {
            if ( (data[6].fvalue < 1.2198772)) {
              sum += (float)0.08208847;
            } else {
              sum += (float)0.12647569;
            }
          } else {
            if ( (data[6].fvalue < 1.6432662)) {
              sum += (float)0.035124913;
            } else {
              sum += (float)0.099138163;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 2.2967253)) {
          if ( (data[7].fvalue < 0.40392727)) {
            if ( (data[5].fvalue < 15.890483)) {
              sum += (float)0.05586113;
            } else {
              sum += (float)0.03479135;
            }
          } else {
            if ( (data[5].fvalue < 16.491074)) {
              sum += (float)0.031364653;
            } else {
              sum += (float)0.0083115837;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.7054283)) {
            if ( (data[5].fvalue < 15.99122)) {
              sum += (float)0.080664478;
            } else {
              sum += (float)0.054418456;
            }
          } else {
            if ( (data[5].fvalue < 6.6690178)) {
              sum += (float)0.19062984;
            } else {
              sum += (float)0.11057871;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.52230412)) {
    if ( (data[6].fvalue < 0.24109685)) {
      if ( (data[6].fvalue < 2.0996093e-05)) {
        if ( (data[7].fvalue < 0.92235482)) {
          if ( (data[3].fvalue < 0.5)) {
            if ( (data[5].fvalue < 2.819494)) {
              sum += (float)0.015818588;
            } else {
              sum += (float)0.016427394;
            }
          } else {
            if ( (data[7].fvalue < 0.25207454)) {
              sum += (float)0.01576823;
            } else {
              sum += (float)0.014048231;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.0814543)) {
            sum += (float)0.033330124;
          } else {
            if ( (data[5].fvalue < 4.1777363)) {
              sum += (float)0.015030156;
            } else {
              sum += (float)0.020987924;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.055658109)) {
          if ( (data[7].fvalue < 0.21688202)) {
            if ( (data[5].fvalue < 0.96276587)) {
              sum += (float)0.0063053616;
            } else {
              sum += (float)0.0095720496;
            }
          } else {
            if ( (data[5].fvalue < 6.9438019)) {
              sum += (float)0.0068148221;
            } else {
              sum += (float)-0.0075317146;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.0456519)) {
            if ( (data[5].fvalue < 0.77388966)) {
              sum += (float)0.0037688741;
            } else {
              sum += (float)0.0075643058;
            }
          } else {
            if ( (data[5].fvalue < 6.4124885)) {
              sum += (float)0.012526467;
            } else {
              sum += (float)0.0068278997;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.37638068)) {
        if ( (data[7].fvalue < 0.23070517)) {
          if ( (data[6].fvalue < 0.30288923)) {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.016737901;
            } else {
              sum += (float)0.010513661;
            }
          } else {
            if ( (data[4].fvalue < 0.5)) {
              sum += (float)0.018602826;
            } else {
              sum += (float)0.02149111;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.2550244)) {
            if ( (data[5].fvalue < 1.2338848)) {
              sum += (float)0.010666711;
            } else {
              sum += (float)0.015229788;
            }
          } else {
            if ( (data[7].fvalue < 0.3413738)) {
              sum += (float)-0.0056076935;
            } else {
              sum += (float)0.0049883565;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.23510103)) {
          if ( (data[5].fvalue < 6.6195621)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.027692517;
            } else {
              sum += (float)0.019905796;
            }
          } else {
            if ( (data[7].fvalue < 0.10301524)) {
              sum += (float)0.020113131;
            } else {
              sum += (float)0.014443697;
            }
          }
        } else {
          if ( (data[5].fvalue < 4.8847685)) {
            if ( (data[7].fvalue < 0.52342331)) {
              sum += (float)0.020003384;
            } else {
              sum += (float)0.015779531;
            }
          } else {
            if ( (data[5].fvalue < 13.412777)) {
              sum += (float)0.010737807;
            } else {
              sum += (float)0.0026607811;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.085613)) {
      if ( (data[2].fvalue < 0.5)) {
        if ( (data[5].fvalue < 8.1114454)) {
          if ( (data[5].fvalue < 2.8940439)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.024090478;
            } else {
              sum += (float)0.015613527;
            }
          } else {
            if ( (data[7].fvalue < 0.74366891)) {
              sum += (float)0.037204575;
            } else {
              sum += (float)0.016235054;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.15310092)) {
            if ( (data[6].fvalue < 0.83749616)) {
              sum += (float)0.021351295;
            } else {
              sum += (float)0.034738723;
            }
          } else {
            if ( (data[5].fvalue < 18.234131)) {
              sum += (float)0.013770043;
            } else {
              sum += (float)-0.0013873167;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.34215188)) {
          if ( (data[5].fvalue < 4.8379731)) {
            if ( (data[6].fvalue < 0.76097292)) {
              sum += (float)0.043249872;
            } else {
              sum += (float)0.073747471;
            }
          } else {
            if ( (data[6].fvalue < 0.75903052)) {
              sum += (float)0.025439279;
            } else {
              sum += (float)0.03634024;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.82461894)) {
            if ( (data[6].fvalue < 0.60165417)) {
              sum += (float)0.021812923;
            } else {
              sum += (float)0.028294777;
            }
          } else {
            if ( (data[7].fvalue < 0.65757847)) {
              sum += (float)0.045166355;
            } else {
              sum += (float)0.027748138;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 8.7420425)) {
        if ( (data[7].fvalue < 0.48110539)) {
          if ( (data[5].fvalue < 4.9272432)) {
            if ( (data[6].fvalue < 1.6129057)) {
              sum += (float)0.086027354;
            } else {
              sum += (float)0.18375108;
            }
          } else {
            if ( (data[6].fvalue < 1.5745625)) {
              sum += (float)0.051705137;
            } else {
              sum += (float)0.0817573;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.7189684)) {
            if ( (data[7].fvalue < 0.71726757)) {
              sum += (float)0.053625375;
            } else {
              sum += (float)0.036847297;
            }
          } else {
            if ( (data[5].fvalue < 6.1908283)) {
              sum += (float)0.082559928;
            } else {
              sum += (float)0.028641118;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.13941845)) {
          if ( (data[7].fvalue < 0.06802319)) {
            if ( (data[5].fvalue < 53.294701)) {
              sum += (float)0.083188638;
            } else {
              sum += (float)0.047178529;
            }
          } else {
            if ( (data[5].fvalue < 24.454714)) {
              sum += (float)0.065273516;
            } else {
              sum += (float)0.029949408;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.7585099)) {
            if ( (data[5].fvalue < 14.090948)) {
              sum += (float)0.029534992;
            } else {
              sum += (float)0.0063471943;
            }
          } else {
            if ( (data[5].fvalue < 19.383331)) {
              sum += (float)0.04733758;
            } else {
              sum += (float)0.018926319;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.56272304)) {
    if ( (data[6].fvalue < 0.21375763)) {
      if ( (data[6].fvalue < 0.0012567444)) {
        if ( (data[7].fvalue < 0.89573461)) {
          if ( (data[7].fvalue < 0.21309623)) {
            if ( (data[5].fvalue < 2.3815413)) {
              sum += (float)0.012077439;
            } else {
              sum += (float)0.011303023;
            }
          } else {
            if ( (data[5].fvalue < 7.7673721)) {
              sum += (float)0.011144668;
            } else {
              sum += (float)0.0088810036;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.0882895)) {
            sum += (float)0.032238655;
          } else {
            if ( (data[7].fvalue < 0.92235482)) {
              sum += (float)0.010862119;
            } else {
              sum += (float)0.016283341;
            }
          }
        }
      } else {
        if ( (data[1].fvalue < 0.5)) {
          if ( (data[5].fvalue < 1.0822949)) {
            if ( (data[5].fvalue < 0.88607365)) {
              sum += (float)0.0035174256;
            } else {
              sum += (float)0.0054875053;
            }
          } else {
            if ( (data[6].fvalue < 0.10864481)) {
              sum += (float)0.007129285;
            } else {
              sum += (float)0.0092108706;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.6888552)) {
            if ( (data[7].fvalue < 0.72512364)) {
              sum += (float)0.007079205;
            } else {
              sum += (float)0.00043858244;
            }
          } else {
            if ( (data[7].fvalue < 0.1429005)) {
              sum += (float)0.0037195154;
            } else {
              sum += (float)-0.0065366081;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.42258626)) {
        if ( (data[7].fvalue < 0.46690392)) {
          if ( (data[5].fvalue < 1.3215864)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.010323727;
            } else {
              sum += (float)0.0063403752;
            }
          } else {
            if ( (data[5].fvalue < 5.0050349)) {
              sum += (float)0.012938699;
            } else {
              sum += (float)0.0090255458;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.69547522)) {
            if ( (data[5].fvalue < 1.9152083)) {
              sum += (float)0.0075961133;
            } else {
              sum += (float)0.011627453;
            }
          } else {
            if ( (data[7].fvalue < 0.78961158)) {
              sum += (float)0.0066951802;
            } else {
              sum += (float)0.0038997817;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.30522555)) {
          if ( (data[5].fvalue < 7.6048317)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.020884512;
            } else {
              sum += (float)0.013292213;
            }
          } else {
            if ( (data[7].fvalue < 0.09998019)) {
              sum += (float)0.014768586;
            } else {
              sum += (float)0.0075306934;
            }
          }
        } else {
          if ( (data[5].fvalue < 4.8844166)) {
            if ( (data[7].fvalue < 0.72858489)) {
              sum += (float)0.014066709;
            } else {
              sum += (float)0.0083134416;
            }
          } else {
            if ( (data[5].fvalue < 4.9426136)) {
              sum += (float)-0.018259475;
            } else {
              sum += (float)0.0041638706;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.1454952)) {
      if ( (data[7].fvalue < 0.66299915)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[5].fvalue < 5.1127191)) {
            if ( (data[5].fvalue < 3.0443373)) {
              sum += (float)0.024384981;
            } else {
              sum += (float)0.036271587;
            }
          } else {
            if ( (data[7].fvalue < 0.1939303)) {
              sum += (float)0.021979311;
            } else {
              sum += (float)0.011337806;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.9139102)) {
            if ( (data[5].fvalue < 2.8687339)) {
              sum += (float)0.010206355;
            } else {
              sum += (float)-0.014414213;
            }
          } else {
            if ( (data[7].fvalue < 0.27263093)) {
              sum += (float)0.014683404;
            } else {
              sum += (float)0.038110979;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.79670179)) {
          if ( (data[6].fvalue < 0.70811129)) {
            if ( (data[5].fvalue < 2.6488442)) {
              sum += (float)0.010581581;
            } else {
              sum += (float)0.016707426;
            }
          } else {
            if ( (data[5].fvalue < 6.8659945)) {
              sum += (float)0.020079475;
            } else {
              sum += (float)-0.0018681457;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.1577978)) {
            if ( (data[7].fvalue < 0.94664657)) {
              sum += (float)0.0090258233;
            } else {
              sum += (float)0.054086175;
            }
          } else {
            if ( (data[7].fvalue < 0.86651313)) {
              sum += (float)0.01120422;
            } else {
              sum += (float)-0.012179174;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 6.084403)) {
        if ( (data[7].fvalue < 0.69269347)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[6].fvalue < 1.7747359)) {
              sum += (float)0.054715984;
            } else {
              sum += (float)0.11323411;
            }
          } else {
            if ( (data[5].fvalue < 5.4473524)) {
              sum += (float)0.018996488;
            } else {
              sum += (float)0.090043515;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.3845019)) {
            if ( (data[6].fvalue < 1.5036706)) {
              sum += (float)0.027213784;
            } else {
              sum += (float)0.040107273;
            }
          } else {
            if ( (data[5].fvalue < 2.8566074)) {
              sum += (float)-0.14804728;
            } else {
              sum += (float)0.0920913;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.28572953)) {
          if ( (data[5].fvalue < 13.83592)) {
            if ( (data[6].fvalue < 1.4748127)) {
              sum += (float)0.040138844;
            } else {
              sum += (float)0.060919687;
            }
          } else {
            if ( (data[7].fvalue < 0.13588738)) {
              sum += (float)0.044833116;
            } else {
              sum += (float)0.012768178;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.6694684)) {
            if ( (data[7].fvalue < 0.51886952)) {
              sum += (float)0.019035086;
            } else {
              sum += (float)0.0071911355;
            }
          } else {
            if ( (data[5].fvalue < 10.958895)) {
              sum += (float)0.06794998;
            } else {
              sum += (float)0.034475114;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.60614467)) {
    if ( (data[6].fvalue < 0.30734581)) {
      if ( (data[6].fvalue < 0.00072955323)) {
        if ( (data[3].fvalue < 0.5)) {
          if ( (data[5].fvalue < 2.9339337)) {
            if ( (data[7].fvalue < 0.40036583)) {
              sum += (float)0.008150137;
            } else {
              sum += (float)0.0071253465;
            }
          } else {
            if ( (data[5].fvalue < 6.5930285)) {
              sum += (float)0.008538167;
            } else {
              sum += (float)0.0075001428;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.5996528)) {
            if ( (data[5].fvalue < 1.1856854)) {
              sum += (float)0.007654699;
            } else {
              sum += (float)0.010602442;
            }
          } else {
            if ( (data[7].fvalue < 0.31383428)) {
              sum += (float)0.0074969386;
            } else {
              sum += (float)0.0049551185;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.60021389)) {
          if ( (data[6].fvalue < 0.16369042)) {
            if ( (data[6].fvalue < 0.020768708)) {
              sum += (float)0.0032338153;
            } else {
              sum += (float)0.0052934787;
            }
          } else {
            if ( (data[5].fvalue < 1.2885588)) {
              sum += (float)0.0041690222;
            } else {
              sum += (float)0.0074696494;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.20373687)) {
            if ( (data[5].fvalue < 2.4357367)) {
              sum += (float)0.00017439744;
            } else {
              sum += (float)0.0025838274;
            }
          } else {
            if ( (data[5].fvalue < 2.7323651)) {
              sum += (float)0.0028730936;
            } else {
              sum += (float)0.0056211706;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 2.1714282)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[7].fvalue < 0.52310777)) {
            if ( (data[6].fvalue < 0.35429037)) {
              sum += (float)0.0073877634;
            } else {
              sum += (float)0.01071483;
            }
          } else {
            if ( (data[7].fvalue < 0.87178504)) {
              sum += (float)0.006687576;
            } else {
              sum += (float)0.022633137;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.56332612)) {
            if ( (data[5].fvalue < 1.4455044)) {
              sum += (float)0.0027790451;
            } else {
              sum += (float)0.0061155166;
            }
          } else {
            if ( (data[5].fvalue < 1.1695893)) {
              sum += (float)0.0087411525;
            } else {
              sum += (float)0.013576853;
            }
          }
        }
      } else {
        if ( (data[1].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.43497884)) {
            if ( (data[4].fvalue < 0.5)) {
              sum += (float)0.010098945;
            } else {
              sum += (float)0.012653929;
            }
          } else {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.014619272;
            } else {
              sum += (float)0.010253911;
            }
          }
        } else {
          if ( (data[5].fvalue < 4.6599059)) {
            if ( (data[7].fvalue < 0.64922059)) {
              sum += (float)0.012397065;
            } else {
              sum += (float)0.0049036471;
            }
          } else {
            if ( (data[5].fvalue < 15.900288)) {
              sum += (float)0.0054076277;
            } else {
              sum += (float)-0.00076758052;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.0592072)) {
      if ( (data[7].fvalue < 0.74039853)) {
        if ( (data[5].fvalue < 7.457346)) {
          if ( (data[5].fvalue < 2.6784656)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.015765471;
            } else {
              sum += (float)0.0072583403;
            }
          } else {
            if ( (data[6].fvalue < 0.68216169)) {
              sum += (float)0.017291049;
            } else {
              sum += (float)0.024784589;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.11403225)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.019653406;
            } else {
              sum += (float)0.0057411646;
            }
          } else {
            if ( (data[5].fvalue < 23.55508)) {
              sum += (float)0.006542101;
            } else {
              sum += (float)-0.01261428;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.6619346)) {
          if ( (data[6].fvalue < 0.72732699)) {
            if ( (data[6].fvalue < 0.72204351)) {
              sum += (float)0.0097033065;
            } else {
              sum += (float)-0.017192693;
            }
          } else {
            if ( (data[5].fvalue < 1.6449666)) {
              sum += (float)0.021781113;
            } else {
              sum += (float)0.053162247;
            }
          }
        } else {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[5].fvalue < 3.4829497)) {
              sum += (float)0.0052797906;
            } else {
              sum += (float)0.0098835565;
            }
          } else {
            if ( (data[7].fvalue < 0.74829948)) {
              sum += (float)-0.018321622;
            } else {
              sum += (float)0.021168908;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 9.9804592)) {
        if ( (data[7].fvalue < 0.48110539)) {
          if ( (data[5].fvalue < 4.9855132)) {
            if ( (data[6].fvalue < 1.3190814)) {
              sum += (float)0.041534238;
            } else {
              sum += (float)0.078048073;
            }
          } else {
            if ( (data[6].fvalue < 1.2418258)) {
              sum += (float)0.01783747;
            } else {
              sum += (float)0.035725728;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.225831)) {
            if ( (data[7].fvalue < 0.81933284)) {
              sum += (float)0.024982238;
            } else {
              sum += (float)0.011270564;
            }
          } else {
            if ( (data[5].fvalue < 3.4910059)) {
              sum += (float)0.026767476;
            } else {
              sum += (float)0.066142261;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.17397517)) {
          if ( (data[6].fvalue < 1.7436891)) {
            if ( (data[7].fvalue < 0.094556168)) {
              sum += (float)0.037868425;
            } else {
              sum += (float)0.018133063;
            }
          } else {
            if ( (data[5].fvalue < 27.004757)) {
              sum += (float)0.060917605;
            } else {
              sum += (float)0.032091293;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.119148)) {
            if ( (data[5].fvalue < 19.082085)) {
              sum += (float)0.0093212109;
            } else {
              sum += (float)-0.0089074457;
            }
          } else {
            if ( (data[7].fvalue < 0.61714971)) {
              sum += (float)0.019583747;
            } else {
              sum += (float)0.041626263;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.62689722)) {
    if ( (data[6].fvalue < 0.30175352)) {
      if ( (data[5].fvalue < 1.0119178)) {
        if ( (data[5].fvalue < 0.49642283)) {
          if ( (data[5].fvalue < 0.4750334)) {
            if ( (data[6].fvalue < 0.045655333)) {
              sum += (float)0.010866119;
            } else {
              sum += (float)0.0068252711;
            }
          } else {
            if ( (data[7].fvalue < 0.10752691)) {
              sum += (float)0.00037870536;
            } else {
              sum += (float)0.0077464269;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.77559179)) {
            if ( (data[5].fvalue < 0.75713742)) {
              sum += (float)0.00015714992;
            } else {
              sum += (float)0.0020192617;
            }
          } else {
            if ( (data[7].fvalue < 0.79251581)) {
              sum += (float)0.014007646;
            } else {
              sum += (float)0.023504803;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 5.8500681)) {
          if ( (data[7].fvalue < 0.56507993)) {
            if ( (data[6].fvalue < 0.19213946)) {
              sum += (float)0.0042868699;
            } else {
              sum += (float)0.0058582854;
            }
          } else {
            if ( (data[6].fvalue < 5.0109862e-05)) {
              sum += (float)0.005210212;
            } else {
              sum += (float)0.0015056563;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.123477)) {
            if ( (data[5].fvalue < 10.690735)) {
              sum += (float)0.0029258267;
            } else {
              sum += (float)0.0048366627;
            }
          } else {
            if ( (data[6].fvalue < 0.00038232422)) {
              sum += (float)0.0050491542;
            } else {
              sum += (float)-0.005089757;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 2.5977371)) {
        if ( (data[2].fvalue < 0.5)) {
          if ( (data[4].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.73129356)) {
              sum += (float)0.0037232004;
            } else {
              sum += (float)0.00073400099;
            }
          } else {
            if ( (data[5].fvalue < 1.035159)) {
              sum += (float)0.0024603037;
            } else {
              sum += (float)0.0068071694;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.51246989)) {
            if ( (data[6].fvalue < 0.44007939)) {
              sum += (float)0.006574309;
            } else {
              sum += (float)0.010555658;
            }
          } else {
            if ( (data[7].fvalue < 0.87141323)) {
              sum += (float)0.0052480968;
            } else {
              sum += (float)0.023434823;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.6451063)) {
          if ( (data[6].fvalue < 0.43119329)) {
            if ( (data[5].fvalue < 3.0327721)) {
              sum += (float)0.0060887351;
            } else {
              sum += (float)0.0083497325;
            }
          } else {
            if ( (data[7].fvalue < 0.25738943)) {
              sum += (float)0.012745933;
            } else {
              sum += (float)0.0086380551;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.15398389)) {
            if ( (data[5].fvalue < 14.050909)) {
              sum += (float)0.008073071;
            } else {
              sum += (float)0.0044647194;
            }
          } else {
            if ( (data[5].fvalue < 14.690434)) {
              sum += (float)0.0030056501;
            } else {
              sum += (float)-0.0031350395;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.504777)) {
      if ( (data[2].fvalue < 0.5)) {
        if ( (data[5].fvalue < 5.2071371)) {
          if ( (data[5].fvalue < 3.1639857)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.0099795656;
            } else {
              sum += (float)0.0036578288;
            }
          } else {
            if ( (data[7].fvalue < 0.81851995)) {
              sum += (float)0.019325618;
            } else {
              sum += (float)0.0039004255;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.45909891)) {
            if ( (data[5].fvalue < 23.779013)) {
              sum += (float)0.0091592399;
            } else {
              sum += (float)-0.0039534294;
            }
          } else {
            if ( (data[7].fvalue < 0.87304008)) {
              sum += (float)0.00082058477;
            } else {
              sum += (float)-0.017347958;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.31534487)) {
          if ( (data[5].fvalue < 4.7648964)) {
            if ( (data[6].fvalue < 0.89529061)) {
              sum += (float)0.02735286;
            } else {
              sum += (float)0.064119786;
            }
          } else {
            if ( (data[7].fvalue < 0.23629099)) {
              sum += (float)0.01463965;
            } else {
              sum += (float)0.00057582004;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.90573478)) {
            if ( (data[5].fvalue < 1.623639)) {
              sum += (float)0.014417905;
            } else {
              sum += (float)0.0078698173;
            }
          } else {
            if ( (data[7].fvalue < 0.66210586)) {
              sum += (float)0.023580648;
            } else {
              sum += (float)0.012333114;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 6.1023269)) {
        if ( (data[7].fvalue < 0.81933284)) {
          if ( (data[6].fvalue < 2.1566005)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.037416492;
            } else {
              sum += (float)0.0069562267;
            }
          } else {
            if ( (data[5].fvalue < 3.5261173)) {
              sum += (float)0.039787769;
            } else {
              sum += (float)0.09022668;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.5106661)) {
            if ( (data[7].fvalue < 0.93300855)) {
              sum += (float)0.0066569666;
            } else {
              sum += (float)0.11394358;
            }
          } else {
            if ( (data[5].fvalue < 3.5480497)) {
              sum += (float)0.14917967;
            } else {
              sum += (float)0.02047918;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.23229238)) {
          if ( (data[5].fvalue < 18.999464)) {
            if ( (data[6].fvalue < 1.9625697)) {
              sum += (float)0.035983332;
            } else {
              sum += (float)0.05624881;
            }
          } else {
            if ( (data[6].fvalue < 2.0106106)) {
              sum += (float)0.006331895;
            } else {
              sum += (float)0.033801969;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.9708085)) {
            if ( (data[5].fvalue < 16.419945)) {
              sum += (float)0.011350259;
            } else {
              sum += (float)-0.0022333374;
            }
          } else {
            if ( (data[7].fvalue < 0.89627665)) {
              sum += (float)0.022063358;
            } else {
              sum += (float)0.099036403;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.74114048)) {
    if ( (data[6].fvalue < 0.35407943)) {
      if ( (data[5].fvalue < 2.0000434)) {
        if ( (data[6].fvalue < 0.0012280579)) {
          if ( (data[7].fvalue < 0.83116698)) {
            if ( (data[7].fvalue < 0.14451271)) {
              sum += (float)0.0049922527;
            } else {
              sum += (float)0.0039597955;
            }
          } else {
            if ( (data[5].fvalue < 1.2576714)) {
              sum += (float)0.024948901;
            } else {
              sum += (float)0.012245858;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.42865989)) {
            if ( (data[6].fvalue < 0.0018198242)) {
              sum += (float)-0.014100677;
            } else {
              sum += (float)0.0023017705;
            }
          } else {
            if ( (data[6].fvalue < 0.20524731)) {
              sum += (float)-3.6948448e-05;
            } else {
              sum += (float)0.0017234491;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.7387848)) {
          if ( (data[6].fvalue < 0.11327368)) {
            if ( (data[6].fvalue < 0.0012560119)) {
              sum += (float)0.0040844828;
            } else {
              sum += (float)0.0024846916;
            }
          } else {
            if ( (data[7].fvalue < 0.76693273)) {
              sum += (float)0.0044160276;
            } else {
              sum += (float)-0.0015877507;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.16009483)) {
            if ( (data[6].fvalue < 0.21906185)) {
              sum += (float)0.0023117433;
            } else {
              sum += (float)0.0045841858;
            }
          } else {
            if ( (data[6].fvalue < 0.00074407959)) {
              sum += (float)0.0041276943;
            } else {
              sum += (float)-0.0030792505;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.75882232)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[7].fvalue < 0.23672336)) {
            if ( (data[5].fvalue < 8.1091938)) {
              sum += (float)0.0090725059;
            } else {
              sum += (float)0.0032507137;
            }
          } else {
            if ( (data[5].fvalue < 4.891592)) {
              sum += (float)0.0053634583;
            } else {
              sum += (float)0.0016293545;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.302062)) {
            if ( (data[7].fvalue < 0.63142836)) {
              sum += (float)0.0022813156;
            } else {
              sum += (float)0.010399393;
            }
          } else {
            if ( (data[7].fvalue < 0.17125419)) {
              sum += (float)0.0036678936;
            } else {
              sum += (float)0.012772211;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.5021559)) {
          if ( (data[7].fvalue < 0.87282312)) {
            if ( (data[7].fvalue < 0.76586032)) {
              sum += (float)4.3985237e-05;
            } else {
              sum += (float)0.01264142;
            }
          } else {
            if ( (data[7].fvalue < 0.89223707)) {
              sum += (float)0.040772546;
            } else {
              sum += (float)0.014138274;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.7589922)) {
            if ( (data[7].fvalue < 0.91040838)) {
              sum += (float)-0.00027668074;
            } else {
              sum += (float)0.011841129;
            }
          } else {
            if ( (data[7].fvalue < 0.84043956)) {
              sum += (float)0.01080603;
            } else {
              sum += (float)-0.00087025744;
            }
          }
        }
      }
    }
  } else {
    if ( (data[0].fvalue < 0.5)) {
      if ( (data[5].fvalue < 5.2550626)) {
        if ( (data[7].fvalue < 0.30810189)) {
          if ( (data[6].fvalue < 0.87564999)) {
            if ( (data[5].fvalue < 4.1969156)) {
              sum += (float)0.028290233;
            } else {
              sum += (float)0.0087572671;
            }
          } else {
            if ( (data[5].fvalue < 4.9928303)) {
              sum += (float)0.043810796;
            } else {
              sum += (float)0.0095188683;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.1328545)) {
            if ( (data[7].fvalue < 0.66607028)) {
              sum += (float)0.012453941;
            } else {
              sum += (float)0.005183314;
            }
          } else {
            if ( (data[7].fvalue < 0.76257265)) {
              sum += (float)0.024214868;
            } else {
              sum += (float)0.0099184811;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.1552316)) {
          if ( (data[5].fvalue < 16.983055)) {
            if ( (data[6].fvalue < 1.2778418)) {
              sum += (float)0.018074028;
            } else {
              sum += (float)0.043442138;
            }
          } else {
            if ( (data[7].fvalue < 0.092381455)) {
              sum += (float)0.018693084;
            } else {
              sum += (float)-0.0021160161;
            }
          }
        } else {
          if ( (data[5].fvalue < 27.071068)) {
            if ( (data[6].fvalue < 1.5921015)) {
              sum += (float)0.0032237214;
            } else {
              sum += (float)0.012009268;
            }
          } else {
            if ( (data[6].fvalue < 0.79223943)) {
              sum += (float)0.014900718;
            } else {
              sum += (float)-0.01491687;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 2.1594541)) {
        if ( (data[6].fvalue < 2.0288682)) {
          if ( (data[6].fvalue < 1.9501026)) {
            if ( (data[7].fvalue < 0.76503253)) {
              sum += (float)0.002173488;
            } else {
              sum += (float)0.013509364;
            }
          } else {
            if ( (data[5].fvalue < 2.9543905)) {
              sum += (float)0.030038085;
            } else {
              sum += (float)0.099122614;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.6389947)) {
            sum += (float)-0.13935126;
          } else {
            if ( (data[6].fvalue < 2.0328851)) {
              sum += (float)-0.095792443;
            } else {
              sum += (float)0.0086613363;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.87332618)) {
          if ( (data[7].fvalue < 0.78867757)) {
            if ( (data[5].fvalue < 3.1136389)) {
              sum += (float)-0.022701444;
            } else {
              sum += (float)0.041565221;
            }
          } else {
            if ( (data[6].fvalue < 2.1827474)) {
              sum += (float)-0.019302836;
            } else {
              sum += (float)0.065295629;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.4116149)) {
            sum += (float)-0.13074806;
          } else {
            sum += (float)0.028661478;
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.54665303)) {
    if ( (data[6].fvalue < 0.20587613)) {
      if ( (data[6].fvalue < 0.0012567444)) {
        if ( (data[7].fvalue < 0.25096583)) {
          if ( (data[5].fvalue < 12.251728)) {
            if ( (data[5].fvalue < 2.4199419)) {
              sum += (float)0.0034624452;
            } else {
              sum += (float)0.0029812132;
            }
          } else {
            if ( (data[7].fvalue < 0.12302005)) {
              sum += (float)0.0049928203;
            } else {
              sum += (float)0.0021826364;
            }
          }
        } else {
          if ( (data[5].fvalue < 8.2745705)) {
            if ( (data[5].fvalue < 4.6007376)) {
              sum += (float)0.0026985491;
            } else {
              sum += (float)0.0047485759;
            }
          } else {
            if ( (data[7].fvalue < 0.4138869)) {
              sum += (float)-0.00066781149;
            } else {
              sum += (float)0.0029956079;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.0018201598)) {
          if ( (data[5].fvalue < 1.9980886)) {
            if ( (data[7].fvalue < 0.39365739)) {
              sum += (float)-0.010265947;
            } else {
              sum += (float)-0.016156416;
            }
          } else {
            if ( (data[6].fvalue < 0.0012577209)) {
              sum += (float)0.00064572424;
            } else {
              sum += (float)-0.015876215;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.18490857)) {
            if ( (data[6].fvalue < 0.045609713)) {
              sum += (float)0.0012370885;
            } else {
              sum += (float)0.0023473755;
            }
          } else {
            if ( (data[5].fvalue < 7.5758944)) {
              sum += (float)0.0011335593;
            } else {
              sum += (float)-0.0050111804;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 0.5)) {
        if ( (data[5].fvalue < 9.9098673)) {
          if ( (data[5].fvalue < 3.0340247)) {
            if ( (data[2].fvalue < 0.5)) {
              sum += (float)0.0014150434;
            } else {
              sum += (float)0.0028897403;
            }
          } else {
            if ( (data[6].fvalue < 0.43497884)) {
              sum += (float)0.0033382636;
            } else {
              sum += (float)0.0055306782;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.099180907)) {
            if ( (data[7].fvalue < 0.067688197)) {
              sum += (float)0.0032844702;
            } else {
              sum += (float)0.0011622868;
            }
          } else {
            if ( (data[5].fvalue < 25.831081)) {
              sum += (float)-0.0025739798;
            } else {
              sum += (float)-0.012493141;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 3.1668038)) {
          if ( (data[5].fvalue < 1.0512608)) {
            if ( (data[7].fvalue < 0.24278352)) {
              sum += (float)-0.0090109296;
            } else {
              sum += (float)0.00081215549;
            }
          } else {
            if ( (data[7].fvalue < 0.35663182)) {
              sum += (float)0.0033307739;
            } else {
              sum += (float)0.0063973023;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.17966065)) {
            if ( (data[7].fvalue < 0.17861444)) {
              sum += (float)0.0053704581;
            } else {
              sum += (float)-0.012476875;
            }
          } else {
            if ( (data[6].fvalue < 0.52145284)) {
              sum += (float)0.009699286;
            } else {
              sum += (float)0.022427049;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.7009304)) {
      if ( (data[7].fvalue < 0.75131428)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.91517282)) {
            if ( (data[7].fvalue < 0.3042053)) {
              sum += (float)0.0085721016;
            } else {
              sum += (float)0.0041679027;
            }
          } else {
            if ( (data[5].fvalue < 4.8205881)) {
              sum += (float)0.014524479;
            } else {
              sum += (float)0.0046592206;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.1747649)) {
            if ( (data[7].fvalue < 0.17404342)) {
              sum += (float)-0.0010918377;
            } else {
              sum += (float)-0.056262724;
            }
          } else {
            if ( (data[5].fvalue < 3.3525901)) {
              sum += (float)0.0017817563;
            } else {
              sum += (float)0.007597948;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.0171955)) {
          if ( (data[6].fvalue < 0.96261227)) {
            if ( (data[7].fvalue < 0.89534861)) {
              sum += (float)0.0022837108;
            } else {
              sum += (float)0.034117136;
            }
          } else {
            if ( (data[6].fvalue < 1.4072001)) {
              sum += (float)0.017290721;
            } else {
              sum += (float)-0.0034390218;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.6860154)) {
            if ( (data[6].fvalue < 1.6686467)) {
              sum += (float)0.00051879854;
            } else {
              sum += (float)0.036953844;
            }
          } else {
            if ( (data[7].fvalue < 0.89954412)) {
              sum += (float)-0.041989043;
            } else {
              sum += (float)0.11849456;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 6.1806545)) {
        if ( (data[7].fvalue < 0.81933284)) {
          if ( (data[5].fvalue < 4.0956316)) {
            if ( (data[5].fvalue < 3.979871)) {
              sum += (float)0.026745664;
            } else {
              sum += (float)-0.055902269;
            }
          } else {
            if ( (data[6].fvalue < 1.7154135)) {
              sum += (float)0.10986975;
            } else {
              sum += (float)0.04252382;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.760345)) {
            if ( (data[6].fvalue < 1.7033508)) {
              sum += (float)0.062706351;
            } else {
              sum += (float)-0.037424561;
            }
          } else {
            if ( (data[5].fvalue < 2.3140125)) {
              sum += (float)-0.080875017;
            } else {
              sum += (float)0.015661897;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.22795671)) {
          if ( (data[5].fvalue < 27.004757)) {
            if ( (data[7].fvalue < 0.12091023)) {
              sum += (float)0.054437146;
            } else {
              sum += (float)0.022757502;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.031047108;
            } else {
              sum += (float)0.0067133466;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.6338797)) {
            if ( (data[6].fvalue < 2.2269046)) {
              sum += (float)-0.039779782;
            } else {
              sum += (float)0.010633055;
            }
          } else {
            if ( (data[5].fvalue < 7.4928708)) {
              sum += (float)0.030162157;
            } else {
              sum += (float)0.0041957428;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.74114048)) {
    if ( (data[6].fvalue < 0.31036139)) {
      if ( (data[5].fvalue < 0.93739122)) {
        if ( (data[5].fvalue < 0.50023139)) {
          if ( (data[6].fvalue < 0.045261808)) {
            if ( (data[7].fvalue < 0.077749923)) {
              sum += (float)0.0016431871;
            } else {
              sum += (float)0.0062672985;
            }
          } else {
            if ( (data[7].fvalue < 0.16635615)) {
              sum += (float)-0.0053909523;
            } else {
              sum += (float)0.0037776951;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.77559179)) {
            if ( (data[5].fvalue < 0.71643913)) {
              sum += (float)-0.0012816645;
            } else {
              sum += (float)2.6761547e-05;
            }
          } else {
            if ( (data[5].fvalue < 0.9278363)) {
              sum += (float)0.015343748;
            } else {
              sum += (float)0.0066383849;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 6.7163944)) {
          if ( (data[5].fvalue < 2.9212327)) {
            if ( (data[7].fvalue < 0.66366124)) {
              sum += (float)0.0014253604;
            } else {
              sum += (float)-0.00042497602;
            }
          } else {
            if ( (data[7].fvalue < 0.83894765)) {
              sum += (float)0.0021080624;
            } else {
              sum += (float)-0.0015685742;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.095277779)) {
            if ( (data[5].fvalue < 10.111042)) {
              sum += (float)0.00039486567;
            } else {
              sum += (float)0.0020348122;
            }
          } else {
            if ( (data[6].fvalue < 0.00014794922)) {
              sum += (float)0.0014377007;
            } else {
              sum += (float)-0.0030134891;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.60481834)) {
        if ( (data[5].fvalue < 4.6433258)) {
          if ( (data[5].fvalue < 3.0043879)) {
            if ( (data[6].fvalue < 0.54792857)) {
              sum += (float)0.0018856011;
            } else {
              sum += (float)0.0039770175;
            }
          } else {
            if ( (data[7].fvalue < 0.45877126)) {
              sum += (float)0.0041115563;
            } else {
              sum += (float)0.0096375095;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.15558091)) {
            if ( (data[6].fvalue < 0.74104893)) {
              sum += (float)0.0023295402;
            } else {
              sum += (float)-0.064726122;
            }
          } else {
            if ( (data[7].fvalue < 0.15617418)) {
              sum += (float)-0.018677952;
            } else {
              sum += (float)-0.00012452401;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.3679399)) {
          if ( (data[7].fvalue < 0.83394289)) {
            if ( (data[7].fvalue < 0.62187195)) {
              sum += (float)-0.00013766489;
            } else {
              sum += (float)0.0041141552;
            }
          } else {
            if ( (data[6].fvalue < 0.66071504)) {
              sum += (float)0.022071574;
            } else {
              sum += (float)-0.007503619;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.5204799)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)-0.0001082483;
            } else {
              sum += (float)0.0060310569;
            }
          } else {
            if ( (data[7].fvalue < 0.84138608)) {
              sum += (float)0.0062557682;
            } else {
              sum += (float)-0.0021145577;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 0.3455894)) {
      if ( (data[5].fvalue < 11.555695)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[6].fvalue < 1.2059419)) {
            if ( (data[5].fvalue < 4.1969156)) {
              sum += (float)0.017242616;
            } else {
              sum += (float)0.0072022923;
            }
          } else {
            if ( (data[5].fvalue < 5.0879459)) {
              sum += (float)0.053535301;
            } else {
              sum += (float)0.021158583;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.17594829)) {
            if ( (data[5].fvalue < 4.8470755)) {
              sum += (float)-0.047494527;
            } else {
              sum += (float)-0.0068884972;
            }
          } else {
            if ( (data[5].fvalue < 2.5178616)) {
              sum += (float)0.03795284;
            } else {
              sum += (float)0.0037333036;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.11504978)) {
          if ( (data[6].fvalue < 0.93045831)) {
            if ( (data[6].fvalue < 0.92714494)) {
              sum += (float)0.0035499991;
            } else {
              sum += (float)-0.046259657;
            }
          } else {
            if ( (data[5].fvalue < 17.396626)) {
              sum += (float)0.023991155;
            } else {
              sum += (float)0.012202655;
            }
          }
        } else {
          if ( (data[5].fvalue < 27.027304)) {
            if ( (data[6].fvalue < 1.3005857)) {
              sum += (float)-0.0051177014;
            } else {
              sum += (float)0.0048512621;
            }
          } else {
            if ( (data[6].fvalue < 1.8505645)) {
              sum += (float)-0.02136007;
            } else {
              sum += (float)0.0011670765;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 5.3457413)) {
        if ( (data[6].fvalue < 1.9541292)) {
          if ( (data[6].fvalue < 1.9520195)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.0052100169;
            } else {
              sum += (float)0.00036784657;
            }
          } else {
            if ( (data[6].fvalue < 1.9532546)) {
              sum += (float)-0.1545392;
            } else {
              sum += (float)-0.047015496;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.65644109)) {
            if ( (data[6].fvalue < 2.0258818)) {
              sum += (float)0.034720559;
            } else {
              sum += (float)0.08583688;
            }
          } else {
            if ( (data[5].fvalue < 3.8905606)) {
              sum += (float)0.0089132506;
            } else {
              sum += (float)0.032974251;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 2.3431494)) {
          if ( (data[5].fvalue < 5.3574371)) {
            if ( (data[6].fvalue < 1.0168115)) {
              sum += (float)0.0057959659;
            } else {
              sum += (float)-0.07977502;
            }
          } else {
            if ( (data[7].fvalue < 0.88400674)) {
              sum += (float)-0.00097985554;
            } else {
              sum += (float)-0.015059976;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.61714971)) {
            if ( (data[6].fvalue < 2.6466875)) {
              sum += (float)-0.0094212564;
            } else {
              sum += (float)0.0088390075;
            }
          } else {
            if ( (data[6].fvalue < 2.5061648)) {
              sum += (float)0.064478464;
            } else {
              sum += (float)0.019746566;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.68197399)) {
    if ( (data[6].fvalue < 0.19923034)) {
      if ( (data[6].fvalue < 2.0996093e-05)) {
        if ( (data[7].fvalue < 0.74874711)) {
          if ( (data[7].fvalue < 0.56544113)) {
            if ( (data[5].fvalue < 6.5949335)) {
              sum += (float)0.0017890882;
            } else {
              sum += (float)0.001273569;
            }
          } else {
            if ( (data[5].fvalue < 3.8735721)) {
              sum += (float)0.00070296903;
            } else {
              sum += (float)0.0032745812;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.4229467)) {
            if ( (data[7].fvalue < 0.84752119)) {
              sum += (float)0.0066559929;
            } else {
              sum += (float)0.017720263;
            }
          } else {
            if ( (data[5].fvalue < 4.401073)) {
              sum += (float)0.0020224161;
            } else {
              sum += (float)0.005380393;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.045588106)) {
          if ( (data[6].fvalue < 0.0018217163)) {
            if ( (data[6].fvalue < 0.0012577209)) {
              sum += (float)0.0011312233;
            } else {
              sum += (float)-0.010506276;
            }
          } else {
            if ( (data[7].fvalue < 0.33587369)) {
              sum += (float)0.00032539526;
            } else {
              sum += (float)-0.0011445432;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.258909)) {
            if ( (data[5].fvalue < 0.69209713)) {
              sum += (float)-0.001938773;
            } else {
              sum += (float)0.00015584513;
            }
          } else {
            if ( (data[7].fvalue < 0.75078958)) {
              sum += (float)0.0011629822;
            } else {
              sum += (float)-0.0019754854;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 2.1640816)) {
        if ( (data[5].fvalue < 0.8452177)) {
          if ( (data[6].fvalue < 0.25351363)) {
            if ( (data[7].fvalue < 0.3337054)) {
              sum += (float)-0.0024923747;
            } else {
              sum += (float)2.7526734e-05;
            }
          } else {
            if ( (data[7].fvalue < 0.58171463)) {
              sum += (float)-0.0023302571;
            } else {
              sum += (float)0.0077884947;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.87059844)) {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.0011398449;
            } else {
              sum += (float)-0.00023371891;
            }
          } else {
            if ( (data[5].fvalue < 1.6321456)) {
              sum += (float)0.018947624;
            } else {
              sum += (float)0.004172436;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.5512161)) {
          if ( (data[7].fvalue < 0.81260538)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.0026200989;
            } else {
              sum += (float)0.0011608391;
            }
          } else {
            if ( (data[6].fvalue < 0.68065804)) {
              sum += (float)-0.0016944484;
            } else {
              sum += (float)-0.038371734;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.15543067)) {
            if ( (data[5].fvalue < 17.456581)) {
              sum += (float)0.0019341893;
            } else {
              sum += (float)-0.00025825106;
            }
          } else {
            if ( (data[7].fvalue < 0.39651203)) {
              sum += (float)-0.0015765157;
            } else {
              sum += (float)0.0012241543;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 0.58632487)) {
      if ( (data[5].fvalue < 4.7567029)) {
        if ( (data[6].fvalue < 1.3102739)) {
          if ( (data[7].fvalue < 0.29611593)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.013899636;
            } else {
              sum += (float)0.0039661126;
            }
          } else {
            if ( (data[7].fvalue < 0.46300429)) {
              sum += (float)0.0015312154;
            } else {
              sum += (float)0.0077554262;
            }
          }
        } else {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.38545316)) {
              sum += (float)0.068512894;
            } else {
              sum += (float)0.02446053;
            }
          } else {
            if ( (data[5].fvalue < 4.1703682)) {
              sum += (float)-0.024820171;
            } else {
              sum += (float)0.022543872;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 6.4946074)) {
          if ( (data[6].fvalue < 1.2395805)) {
            if ( (data[7].fvalue < 0.22501095)) {
              sum += (float)0.0035936316;
            } else {
              sum += (float)-0.010302868;
            }
          } else {
            if ( (data[5].fvalue < 6.3837152)) {
              sum += (float)0.013079406;
            } else {
              sum += (float)-0.05889507;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.4801397)) {
            if ( (data[7].fvalue < 0.56116867)) {
              sum += (float)0.017312763;
            } else {
              sum += (float)-0.032957368;
            }
          } else {
            if ( (data[7].fvalue < 0.096505702)) {
              sum += (float)0.0069397404;
            } else {
              sum += (float)0.00099314877;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.9071517)) {
        if ( (data[6].fvalue < 1.9033253)) {
          if ( (data[7].fvalue < 0.85556817)) {
            if ( (data[6].fvalue < 0.99870372)) {
              sum += (float)0.0005083789;
            } else {
              sum += (float)0.0039250762;
            }
          } else {
            if ( (data[6].fvalue < 1.8211007)) {
              sum += (float)-0.0020338336;
            } else {
              sum += (float)-0.052997537;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.79361427)) {
            sum += (float)-0.12633874;
          } else {
            sum += (float)-0.004127942;
          }
        }
      } else {
        if ( (data[6].fvalue < 1.9079535)) {
          sum += (float)0.14047247;
        } else {
          if ( (data[7].fvalue < 0.91403562)) {
            if ( (data[7].fvalue < 0.91346139)) {
              sum += (float)0.0083932793;
            } else {
              sum += (float)-0.17270249;
            }
          } else {
            if ( (data[7].fvalue < 0.92021829)) {
              sum += (float)0.082193062;
            } else {
              sum += (float)0.017993471;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.82701969)) {
    if ( (data[6].fvalue < 0.4322992)) {
      if ( (data[4].fvalue < 0.5)) {
        if ( (data[5].fvalue < 7.28092)) {
          if ( (data[5].fvalue < 2.9343703)) {
            if ( (data[7].fvalue < 0.46787992)) {
              sum += (float)0.0006092824;
            } else {
              sum += (float)-4.4577591e-05;
            }
          } else {
            if ( (data[5].fvalue < 4.4494824)) {
              sum += (float)0.0011635965;
            } else {
              sum += (float)0.00035143842;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.068820089)) {
            if ( (data[5].fvalue < 10.137572)) {
              sum += (float)-6.2679734e-05;
            } else {
              sum += (float)0.0012301427;
            }
          } else {
            if ( (data[6].fvalue < 0.057464384)) {
              sum += (float)0.00026855888;
            } else {
              sum += (float)-0.0021058542;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.10862149)) {
          if ( (data[5].fvalue < 1.1346766)) {
            if ( (data[7].fvalue < 0.14610606)) {
              sum += (float)0.00077182049;
            } else {
              sum += (float)-0.0012064372;
            }
          } else {
            if ( (data[5].fvalue < 10.684057)) {
              sum += (float)0.00072618795;
            } else {
              sum += (float)0.0028324374;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.2511652)) {
            if ( (data[5].fvalue < 1.2500038)) {
              sum += (float)0.00076228648;
            } else {
              sum += (float)-0.0095317746;
            }
          } else {
            if ( (data[7].fvalue < 0.23899484)) {
              sum += (float)0.0016437011;
            } else {
              sum += (float)0.0037171778;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.80571735)) {
        if ( (data[5].fvalue < 2.9797342)) {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.42147577)) {
              sum += (float)-0.001101402;
            } else {
              sum += (float)0.0010010704;
            }
          } else {
            if ( (data[7].fvalue < 0.28516027)) {
              sum += (float)0.0062539997;
            } else {
              sum += (float)0.0015549327;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.6050115)) {
            if ( (data[7].fvalue < 0.20609814)) {
              sum += (float)0.0041721319;
            } else {
              sum += (float)0.0021613133;
            }
          } else {
            if ( (data[6].fvalue < 0.47697556)) {
              sum += (float)0.0048962794;
            } else {
              sum += (float)-0.00098680088;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.80575705)) {
          if ( (data[7].fvalue < 0.33808646)) {
            if ( (data[5].fvalue < 6.7885542)) {
              sum += (float)-0.0047745313;
            } else {
              sum += (float)-0.045763277;
            }
          } else {
            sum += (float)-0.095465235;
          }
        } else {
          if ( (data[7].fvalue < 0.27667552)) {
            if ( (data[7].fvalue < 0.26997283)) {
              sum += (float)0.0010955122;
            } else {
              sum += (float)0.034415409;
            }
          } else {
            if ( (data[7].fvalue < 0.29322177)) {
              sum += (float)-0.024787104;
            } else {
              sum += (float)-0.0026723992;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 0.65646601)) {
      if ( (data[5].fvalue < 4.5331383)) {
        if ( (data[5].fvalue < 2.8971326)) {
          if ( (data[6].fvalue < 1.1578871)) {
            if ( (data[5].fvalue < 2.8871937)) {
              sum += (float)0.0048999842;
            } else {
              sum += (float)-0.037748285;
            }
          } else {
            if ( (data[7].fvalue < 0.63337851)) {
              sum += (float)-0.010251017;
            } else {
              sum += (float)0.0089127347;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.8905802)) {
            if ( (data[7].fvalue < 0.34044138)) {
              sum += (float)0.013562326;
            } else {
              sum += (float)0.0075909682;
            }
          } else {
            if ( (data[5].fvalue < 4.4084759)) {
              sum += (float)0.056440856;
            } else {
              sum += (float)-0.078353837;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.19806898)) {
          if ( (data[5].fvalue < 30.051121)) {
            if ( (data[6].fvalue < 1.3381361)) {
              sum += (float)0.0050261775;
            } else {
              sum += (float)0.015418177;
            }
          } else {
            if ( (data[7].fvalue < 0.059793964)) {
              sum += (float)0.013275771;
            } else {
              sum += (float)-0.012374862;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.0315224)) {
            if ( (data[5].fvalue < 4.5466642)) {
              sum += (float)-0.065571852;
            } else {
              sum += (float)-0.0050734365;
            }
          } else {
            if ( (data[5].fvalue < 14.578102)) {
              sum += (float)0.0041015651;
            } else {
              sum += (float)-0.0043673115;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.4554588)) {
        if ( (data[5].fvalue < 1.9139378)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[5].fvalue < 1.4487388)) {
              sum += (float)0.024941223;
            } else {
              sum += (float)0.0058827647;
            }
          } else {
            if ( (data[5].fvalue < 1.8256037)) {
              sum += (float)-0.0040949509;
            } else {
              sum += (float)0.013310165;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.9148498)) {
            sum += (float)-0.12300188;
          } else {
            if ( (data[6].fvalue < 1.4150997)) {
              sum += (float)-0.00080093544;
            } else {
              sum += (float)-0.012944817;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 1.46241)) {
          if ( (data[5].fvalue < 3.4927669)) {
            if ( (data[5].fvalue < 2.8682137)) {
              sum += (float)0.062709332;
            } else {
              sum += (float)-0.024047028;
            }
          } else {
            if ( (data[5].fvalue < 3.815222)) {
              sum += (float)0.14858824;
            } else {
              sum += (float)0.064307049;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.9379971)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.015991509;
            } else {
              sum += (float)-0.0099304914;
            }
          } else {
            if ( (data[5].fvalue < 3.1894298)) {
              sum += (float)-0.01621582;
            } else {
              sum += (float)0.0051010204;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 1.0434012)) {
    if ( (data[6].fvalue < 0.39954585)) {
      if ( (data[4].fvalue < 0.5)) {
        if ( (data[6].fvalue < 1.4038086e-05)) {
          if ( (data[7].fvalue < 0.72783923)) {
            if ( (data[7].fvalue < 0.64431131)) {
              sum += (float)0.00097138976;
            } else {
              sum += (float)0.00028030324;
            }
          } else {
            if ( (data[5].fvalue < 1.3277903)) {
              sum += (float)0.0072362111;
            } else {
              sum += (float)0.0014896611;
            }
          }
        } else {
          if ( (data[5].fvalue < 4.6395655)) {
            if ( (data[7].fvalue < 0.67267668)) {
              sum += (float)0.0004594152;
            } else {
              sum += (float)-0.00068698789;
            }
          } else {
            if ( (data[6].fvalue < 0.27258217)) {
              sum += (float)-0.00069452205;
            } else {
              sum += (float)0.000797769;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.11430316)) {
          if ( (data[5].fvalue < 1.1346766)) {
            if ( (data[7].fvalue < 0.088226445)) {
              sum += (float)0.0015057797;
            } else {
              sum += (float)-0.00067940168;
            }
          } else {
            if ( (data[6].fvalue < 0.04575897)) {
              sum += (float)0.00013413012;
            } else {
              sum += (float)0.00083902368;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.0162323)) {
            if ( (data[5].fvalue < 2.0155149)) {
              sum += (float)0.00075240358;
            } else {
              sum += (float)-0.015345694;
            }
          } else {
            if ( (data[7].fvalue < 0.28618711)) {
              sum += (float)0.0013552899;
            } else {
              sum += (float)0.0072153667;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.77146697)) {
        if ( (data[6].fvalue < 1.0417)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.20608914)) {
              sum += (float)0.0028689359;
            } else {
              sum += (float)0.0010964042;
            }
          } else {
            if ( (data[7].fvalue < 0.6308338)) {
              sum += (float)-0.00027771181;
            } else {
              sum += (float)0.0047437632;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.12098793)) {
            sum += (float)0.041369285;
          } else {
            if ( (data[5].fvalue < 1.7136738)) {
              sum += (float)0.021329984;
            } else {
              sum += (float)-0.027611122;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 3.4854727)) {
          if ( (data[7].fvalue < 0.89674532)) {
            if ( (data[5].fvalue < 1.7164017)) {
              sum += (float)0.0049960865;
            } else {
              sum += (float)-0.0027332359;
            }
          } else {
            if ( (data[6].fvalue < 0.82484478)) {
              sum += (float)0.0073375842;
            } else {
              sum += (float)0.03343631;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.84045225)) {
            if ( (data[5].fvalue < 10.524723)) {
              sum += (float)0.0074699908;
            } else {
              sum += (float)-0.0082179848;
            }
          } else {
            if ( (data[5].fvalue < 3.5662408)) {
              sum += (float)0.011239918;
            } else {
              sum += (float)-0.0029369409;
            }
          }
        }
      }
    }
  } else {
    if ( (data[0].fvalue < 0.5)) {
      if ( (data[7].fvalue < 0.43693525)) {
        if ( (data[5].fvalue < 8.9815521)) {
          if ( (data[6].fvalue < 1.2075679)) {
            if ( (data[7].fvalue < 0.33006084)) {
              sum += (float)0.017990373;
            } else {
              sum += (float)-0.00076085527;
            }
          } else {
            if ( (data[5].fvalue < 3.8155081)) {
              sum += (float)0.044819899;
            } else {
              sum += (float)0.01511019;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.13805255)) {
            if ( (data[5].fvalue < 26.256645)) {
              sum += (float)0.01408617;
            } else {
              sum += (float)-7.5937271e-05;
            }
          } else {
            if ( (data[7].fvalue < 0.43641716)) {
              sum += (float)-0.0012336215;
            } else {
              sum += (float)0.059779093;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 3.9557579)) {
          if ( (data[7].fvalue < 0.95801115)) {
            if ( (data[5].fvalue < 3.8905606)) {
              sum += (float)0.0033260903;
            } else {
              sum += (float)0.02340563;
            }
          } else {
            sum += (float)0.1219667;
          }
        } else {
          if ( (data[5].fvalue < 4.0814896)) {
            if ( (data[5].fvalue < 4.0596771)) {
              sum += (float)-0.014782586;
            } else {
              sum += (float)-0.068812907;
            }
          } else {
            if ( (data[6].fvalue < 2.1255953)) {
              sum += (float)-0.0029933753;
            } else {
              sum += (float)0.0097638294;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 2.9964342)) {
        if ( (data[7].fvalue < 0.51598775)) {
          if ( (data[6].fvalue < 1.0900087)) {
            if ( (data[6].fvalue < 1.0554385)) {
              sum += (float)-0.055451952;
            } else {
              sum += (float)0.043950267;
            }
          } else {
            if ( (data[7].fvalue < 0.46455604)) {
              sum += (float)-0.085563205;
            } else {
              sum += (float)-0.024788978;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.0188148)) {
            if ( (data[7].fvalue < 0.76503253)) {
              sum += (float)-0.0051775142;
            } else {
              sum += (float)0.0062421369;
            }
          } else {
            if ( (data[5].fvalue < 2.6602952)) {
              sum += (float)-0.091464639;
            } else {
              sum += (float)0.0063037421;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.4643262)) {
          if ( (data[5].fvalue < 3.1936607)) {
            if ( (data[6].fvalue < 1.1903117)) {
              sum += (float)-0.093748048;
            } else {
              sum += (float)0.025285;
            }
          } else {
            if ( (data[5].fvalue < 3.3256736)) {
              sum += (float)0.041975848;
            } else {
              sum += (float)-0.00057978794;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.9588737)) {
            if ( (data[5].fvalue < 3.8855195)) {
              sum += (float)0.01170912;
            } else {
              sum += (float)-0.099406697;
            }
          } else {
            if ( (data[6].fvalue < 1.4384863)) {
              sum += (float)0.020032968;
            } else {
              sum += (float)0.073472962;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 1.5760829)) {
    if ( (data[5].fvalue < 22.157017)) {
      if ( (data[6].fvalue < 0.49210599)) {
        if ( (data[5].fvalue < 0.90786707)) {
          if ( (data[5].fvalue < 0.52659476)) {
            if ( (data[7].fvalue < 0.26287711)) {
              sum += (float)0.0019851294;
            } else {
              sum += (float)0.0062540625;
            }
          } else {
            if ( (data[7].fvalue < 0.62534487)) {
              sum += (float)-0.000670682;
            } else {
              sum += (float)0.0068622869;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.4919779)) {
            if ( (data[6].fvalue < 0.047548801)) {
              sum += (float)0.00015171898;
            } else {
              sum += (float)0.00048107104;
            }
          } else {
            if ( (data[7].fvalue < 0.17953895)) {
              sum += (float)-0.04867319;
            } else {
              sum += (float)-0.0077439952;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.31537473)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[5].fvalue < 8.9803314)) {
              sum += (float)0.004095274;
            } else {
              sum += (float)-0.00077912846;
            }
          } else {
            if ( (data[7].fvalue < 0.24536887)) {
              sum += (float)-0.0025158613;
            } else {
              sum += (float)0.0032778601;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.44785431)) {
            if ( (data[7].fvalue < 0.31557631)) {
              sum += (float)-0.029077264;
            } else {
              sum += (float)-0.00096420443;
            }
          } else {
            if ( (data[7].fvalue < 0.53321373)) {
              sum += (float)0.0037810535;
            } else {
              sum += (float)0.00037662697;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.063137725)) {
        if ( (data[6].fvalue < 1.0683297)) {
          if ( (data[6].fvalue < 1.0407329)) {
            if ( (data[6].fvalue < 1.0012102)) {
              sum += (float)0.0010256118;
            } else {
              sum += (float)0.037274268;
            }
          } else {
            if ( (data[5].fvalue < 24.787445)) {
              sum += (float)0.0066882041;
            } else {
              sum += (float)-0.026927659;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.022582322)) {
            if ( (data[6].fvalue < 1.3468994)) {
              sum += (float)0.065483876;
            } else {
              sum += (float)0.0056003626;
            }
          } else {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.020068841;
            } else {
              sum += (float)-0.014739054;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.12210864)) {
          if ( (data[7].fvalue < 0.090731516)) {
            if ( (data[6].fvalue < 0.0032120666)) {
              sum += (float)0.001188977;
            } else {
              sum += (float)-0.0015331795;
            }
          } else {
            if ( (data[7].fvalue < 0.34476447)) {
              sum += (float)-0.0030041554;
            } else {
              sum += (float)-0.00030143955;
            }
          }
        } else {
          if ( (data[5].fvalue < 32.848827)) {
            if ( (data[7].fvalue < 0.30312473)) {
              sum += (float)-0.0071868962;
            } else {
              sum += (float)-0.0017524591;
            }
          } else {
            if ( (data[7].fvalue < 0.38689548)) {
              sum += (float)-0.013768196;
            } else {
              sum += (float)0.00017443561;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 2.9379971)) {
      if ( (data[5].fvalue < 2.9123554)) {
        if ( (data[1].fvalue < 0.5)) {
          if ( (data[6].fvalue < 2.0210493)) {
            if ( (data[6].fvalue < 1.9429822)) {
              sum += (float)-0.0029908142;
            } else {
              sum += (float)0.045383736;
            }
          } else {
            if ( (data[6].fvalue < 2.1318212)) {
              sum += (float)-0.064104043;
            } else {
              sum += (float)0.0032758706;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.8496955)) {
            if ( (data[5].fvalue < 2.5088077)) {
              sum += (float)0.0064233141;
            } else {
              sum += (float)0.032932494;
            }
          } else {
            if ( (data[5].fvalue < 2.4400165)) {
              sum += (float)0.069676436;
            } else {
              sum += (float)-0.012108189;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 1.7188635)) {
          if ( (data[6].fvalue < 1.6100035)) {
            if ( (data[6].fvalue < 1.589159)) {
              sum += (float)0.0064795609;
            } else {
              sum += (float)0.06199434;
            }
          } else {
            if ( (data[7].fvalue < 0.68921351)) {
              sum += (float)0.022490183;
            } else {
              sum += (float)-0.042789504;
            }
          }
        } else {
          if ( (data[6].fvalue < 2.0041592)) {
            sum += (float)0.17183152;
          } else {
            if ( (data[5].fvalue < 2.9342151)) {
              sum += (float)0.094477981;
            } else {
              sum += (float)-0.021338537;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 3.1894298)) {
        if ( (data[5].fvalue < 3.1617622)) {
          if ( (data[5].fvalue < 3.133215)) {
            if ( (data[6].fvalue < 1.8854606)) {
              sum += (float)-0.029850017;
            } else {
              sum += (float)0.013513858;
            }
          } else {
            if ( (data[6].fvalue < 2.0002685)) {
              sum += (float)0.068175785;
            } else {
              sum += (float)-0.050685178;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.74289691)) {
            if ( (data[6].fvalue < 1.7545691)) {
              sum += (float)0.031616274;
            } else {
              sum += (float)-0.048492383;
            }
          } else {
            if ( (data[6].fvalue < 2.3913264)) {
              sum += (float)-0.097836986;
            } else {
              sum += (float)0.010896113;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 3.2526295)) {
          if ( (data[7].fvalue < 0.59237146)) {
            sum += (float)-0.056670688;
          } else {
            if ( (data[6].fvalue < 2.1828494)) {
              sum += (float)0.060552787;
            } else {
              sum += (float)-0.0043228935;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.2648487)) {
            if ( (data[6].fvalue < 1.6643744)) {
              sum += (float)0.019058801;
            } else {
              sum += (float)-0.090654895;
            }
          } else {
            if ( (data[5].fvalue < 3.3143592)) {
              sum += (float)-0.021607144;
            } else {
              sum += (float)0.0037183489;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.7539854)) {
    if ( (data[5].fvalue < 1.4980621)) {
      if ( (data[7].fvalue < 0.8155272)) {
        if ( (data[6].fvalue < 0.65592498)) {
          if ( (data[6].fvalue < 0.63934994)) {
            if ( (data[7].fvalue < 0.13300318)) {
              sum += (float)0.00086524937;
            } else {
              sum += (float)-0.00026412818;
            }
          } else {
            if ( (data[7].fvalue < 0.52357006)) {
              sum += (float)-0.0053119855;
            } else {
              sum += (float)0.009239655;
            }
          }
        } else {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.66148007)) {
              sum += (float)-0.018282626;
            } else {
              sum += (float)-0.0049874685;
            }
          } else {
            if ( (data[5].fvalue < 1.4920894)) {
              sum += (float)0.0016865426;
            } else {
              sum += (float)-0.026160562;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.69092095)) {
          if ( (data[7].fvalue < 0.8402952)) {
            if ( (data[5].fvalue < 1.1651751)) {
              sum += (float)0.014126118;
            } else {
              sum += (float)0.0014431219;
            }
          } else {
            if ( (data[5].fvalue < 1.4373877)) {
              sum += (float)0.013518318;
            } else {
              sum += (float)0.0049330029;
            }
          }
        } else {
          sum += (float)-0.024917742;
        }
      }
    } else {
      if ( (data[7].fvalue < 0.80066657)) {
        if ( (data[7].fvalue < 0.36877048)) {
          if ( (data[7].fvalue < 0.20376039)) {
            if ( (data[6].fvalue < 0.2725803)) {
              sum += (float)0.00023960134;
            } else {
              sum += (float)0.00082440261;
            }
          } else {
            if ( (data[5].fvalue < 3.9196267)) {
              sum += (float)0.00018867246;
            } else {
              sum += (float)-0.0014481738;
            }
          }
        } else {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[5].fvalue < 2.4248776)) {
              sum += (float)0.00014878095;
            } else {
              sum += (float)0.0013736682;
            }
          } else {
            if ( (data[6].fvalue < 0.66399205)) {
              sum += (float)0.0036615166;
            } else {
              sum += (float)0.012742895;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.3211689)) {
          if ( (data[6].fvalue < 0.00017910766)) {
            if ( (data[5].fvalue < 1.7589484)) {
              sum += (float)0.0071704118;
            } else {
              sum += (float)0.00037237903;
            }
          } else {
            if ( (data[5].fvalue < 1.6200621)) {
              sum += (float)0.0073934798;
            } else {
              sum += (float)-0.0022045411;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.68807817)) {
            if ( (data[7].fvalue < 0.89699912)) {
              sum += (float)0.0025437819;
            } else {
              sum += (float)-0.00063436717;
            }
          } else {
            if ( (data[5].fvalue < 4.8193374)) {
              sum += (float)0.029371824;
            } else {
              sum += (float)0.01314662;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 4.5935202)) {
      if ( (data[7].fvalue < 0.95760417)) {
        if ( (data[7].fvalue < 0.26361865)) {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.25994554)) {
              sum += (float)-0.0013543456;
            } else {
              sum += (float)0.042446539;
            }
          } else {
            if ( (data[6].fvalue < 0.84783196)) {
              sum += (float)0.0099464059;
            } else {
              sum += (float)0.031592365;
            }
          }
        } else {
          if ( (data[1].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.26601481)) {
              sum += (float)-0.029887054;
            } else {
              sum += (float)0.00034938339;
            }
          } else {
            if ( (data[7].fvalue < 0.73464376)) {
              sum += (float)0.0066781454;
            } else {
              sum += (float)6.3984677e-05;
            }
          }
        }
      } else {
        sum += (float)0.094511822;
      }
    } else {
      if ( (data[5].fvalue < 4.5980387)) {
        if ( (data[1].fvalue < 0.5)) {
          sum += (float)-0.095158577;
        } else {
          if ( (data[6].fvalue < 0.77899611)) {
            sum += (float)0.011858189;
          } else {
            if ( (data[5].fvalue < 4.5963964)) {
              sum += (float)-0.050615747;
            } else {
              sum += (float)-0.0030254247;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.20804171)) {
          if ( (data[5].fvalue < 17.901981)) {
            if ( (data[6].fvalue < 1.3399045)) {
              sum += (float)0.0027958825;
            } else {
              sum += (float)0.013289818;
            }
          } else {
            if ( (data[7].fvalue < 0.096229464)) {
              sum += (float)0.0033955462;
            } else {
              sum += (float)-0.0075547793;
            }
          }
        } else {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.79470348)) {
              sum += (float)0.0077570188;
            } else {
              sum += (float)-0.00095063797;
            }
          } else {
            if ( (data[6].fvalue < 1.1733068)) {
              sum += (float)-0.012150537;
            } else {
              sum += (float)0.00081573427;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 2.9191022)) {
    if ( (data[5].fvalue < 16.942604)) {
      if ( (data[6].fvalue < 1.0115075)) {
        if ( (data[6].fvalue < 1.0113375)) {
          if ( (data[5].fvalue < 2.9892945)) {
            if ( (data[7].fvalue < 0.91051406)) {
              sum += (float)7.8121229e-05;
            } else {
              sum += (float)0.009011684;
            }
          } else {
            if ( (data[5].fvalue < 4.6433754)) {
              sum += (float)0.00067574502;
            } else {
              sum += (float)-0.00010664133;
            }
          }
        } else {
          if ( (data[4].fvalue < 0.5)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)-0.081506871;
            } else {
              sum += (float)-0.012197903;
            }
          } else {
            sum += (float)0.019815356;
          }
        }
      } else {
        if ( (data[7].fvalue < 0.40488213)) {
          if ( (data[5].fvalue < 4.0891743)) {
            if ( (data[6].fvalue < 1.2202214)) {
              sum += (float)0.011460129;
            } else {
              sum += (float)0.036139876;
            }
          } else {
            if ( (data[7].fvalue < 0.40458971)) {
              sum += (float)0.0023730639;
            } else {
              sum += (float)0.086772569;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.016022)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.019899659;
            } else {
              sum += (float)-0.0056042448;
            }
          } else {
            if ( (data[7].fvalue < 0.41180539)) {
              sum += (float)-0.015263488;
            } else {
              sum += (float)0.00026248203;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.081720605)) {
        if ( (data[6].fvalue < 1.2016118)) {
          if ( (data[6].fvalue < 1.1332954)) {
            if ( (data[2].fvalue < 0.5)) {
              sum += (float)0.0013304937;
            } else {
              sum += (float)-0.00078110443;
            }
          } else {
            if ( (data[7].fvalue < 0.023466175)) {
              sum += (float)0.040287733;
            } else {
              sum += (float)-0.024343098;
            }
          }
        } else {
          if ( (data[1].fvalue < 0.5)) {
            if ( (data[5].fvalue < 28.203501)) {
              sum += (float)-0.0096827503;
            } else {
              sum += (float)0.013949118;
            }
          } else {
            if ( (data[6].fvalue < 1.6098628)) {
              sum += (float)0.039555151;
            } else {
              sum += (float)0.0062278793;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.10711551)) {
          if ( (data[5].fvalue < 30.668865)) {
            if ( (data[7].fvalue < 0.16177155)) {
              sum += (float)0.00075434061;
            } else {
              sum += (float)-0.0012054015;
            }
          } else {
            if ( (data[7].fvalue < 0.29419506)) {
              sum += (float)-0.0037068392;
            } else {
              sum += (float)-0.0004268364;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.27744678)) {
            if ( (data[6].fvalue < 2.0278244)) {
              sum += (float)-0.0057547851;
            } else {
              sum += (float)0.0055165882;
            }
          } else {
            if ( (data[6].fvalue < 2.8350995)) {
              sum += (float)-0.0012098243;
            } else {
              sum += (float)-0.046106044;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 6.8403783)) {
      if ( (data[6].fvalue < 3.1949363)) {
        if ( (data[6].fvalue < 2.9601173)) {
          if ( (data[6].fvalue < 2.9446845)) {
            sum += (float)0.015513832;
          } else {
            sum += (float)0.046658888;
          }
        } else {
          if ( (data[7].fvalue < 0.83497053)) {
            if ( (data[6].fvalue < 3.0937128)) {
              sum += (float)0.031893659;
            } else {
              sum += (float)0.0084892465;
            }
          } else {
            if ( (data[6].fvalue < 3.1461296)) {
              sum += (float)-0.030422883;
            } else {
              sum += (float)0.016203977;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.8975352)) {
          sum += (float)0.073562048;
        } else {
          if ( (data[6].fvalue < 3.5519824)) {
            if ( (data[5].fvalue < 5.757719)) {
              sum += (float)0.0075828596;
            } else {
              sum += (float)-0.041755557;
            }
          } else {
            sum += (float)0.051299121;
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.89469624)) {
        if ( (data[7].fvalue < 0.27361441)) {
          if ( (data[6].fvalue < 5.7219563)) {
            if ( (data[7].fvalue < 0.2582508)) {
              sum += (float)0.020389186;
            } else {
              sum += (float)0.03782399;
            }
          } else {
            sum += (float)-0.009957022;
          }
        } else {
          if ( (data[5].fvalue < 7.249887)) {
            if ( (data[7].fvalue < 0.67897105)) {
              sum += (float)-0.0045397086;
            } else {
              sum += (float)-0.064523846;
            }
          } else {
            if ( (data[5].fvalue < 18.104607)) {
              sum += (float)0.0064607505;
            } else {
              sum += (float)-0.0071931379;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 7.5167561)) {
          sum += (float)0.11769107;
        } else {
          if ( (data[5].fvalue < 8.9746819)) {
            if ( (data[5].fvalue < 7.9147992)) {
              sum += (float)0.0097704297;
            } else {
              sum += (float)-0.00736148;
            }
          } else {
            if ( (data[5].fvalue < 9.5015955)) {
              sum += (float)0.041359946;
            } else {
              sum += (float)0.018481856;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 3.5320973)) {
    if ( (data[7].fvalue < 0.86017048)) {
      if ( (data[7].fvalue < 0.8601625)) {
        if ( (data[6].fvalue < 1.6252166)) {
          if ( (data[6].fvalue < 1.6196122)) {
            if ( (data[6].fvalue < 1.6173294)) {
              sum += (float)0.00015283631;
            } else {
              sum += (float)0.035346024;
            }
          } else {
            if ( (data[7].fvalue < 0.45322824)) {
              sum += (float)0.0069996803;
            } else {
              sum += (float)-0.041803539;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.8329754)) {
            if ( (data[5].fvalue < 5.8073616)) {
              sum += (float)0.0061456524;
            } else {
              sum += (float)0.12225568;
            }
          } else {
            if ( (data[7].fvalue < 0.29732546)) {
              sum += (float)0.0066847368;
            } else {
              sum += (float)-0.0032339483;
            }
          }
        }
      } else {
        sum += (float)0.105358;
      }
    } else {
      if ( (data[5].fvalue < 2.5346732)) {
        if ( (data[7].fvalue < 0.91045594)) {
          if ( (data[5].fvalue < 1.7477195)) {
            if ( (data[6].fvalue < 0.89329493)) {
              sum += (float)0.0095001198;
            } else {
              sum += (float)-0.040011059;
            }
          } else {
            if ( (data[6].fvalue < 1.6371822)) {
              sum += (float)0.00016662158;
            } else {
              sum += (float)-0.038655303;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.4753556)) {
            if ( (data[5].fvalue < 2.4188864)) {
              sum += (float)0.011745258;
            } else {
              sum += (float)-0.014192668;
            }
          } else {
            if ( (data[5].fvalue < 2.4818711)) {
              sum += (float)0.1442824;
            } else {
              sum += (float)0.023032047;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.93075287)) {
          if ( (data[6].fvalue < 0.91784292)) {
            if ( (data[5].fvalue < 7.0248718)) {
              sum += (float)-0.0025462571;
            } else {
              sum += (float)0.005734609;
            }
          } else {
            if ( (data[7].fvalue < 0.92687404)) {
              sum += (float)-0.0058518695;
            } else {
              sum += (float)-0.042878143;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.4440365)) {
            if ( (data[6].fvalue < 1.2917757)) {
              sum += (float)0.00045747912;
            } else {
              sum += (float)-0.029286949;
            }
          } else {
            if ( (data[7].fvalue < 0.94181156)) {
              sum += (float)0.063888311;
            } else {
              sum += (float)-0.0002363348;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 7.9938583)) {
      if ( (data[6].fvalue < 5.8079133)) {
        if ( (data[6].fvalue < 4.6416712)) {
          if ( (data[5].fvalue < 7.3844233)) {
            sum += (float)0.062194686;
          } else {
            if ( (data[5].fvalue < 7.6916199)) {
              sum += (float)-0.0091531277;
            } else {
              sum += (float)0.0037362457;
            }
          }
        } else {
          sum += (float)0.098281965;
        }
      } else {
        sum += (float)0.0083048707;
      }
    } else {
      if ( (data[5].fvalue < 9.201045)) {
        if ( (data[6].fvalue < 4.1365147)) {
          sum += (float)-0.033356566;
        } else {
          if ( (data[5].fvalue < 8.7206078)) {
            sum += (float)0.00057127478;
          } else {
            sum += (float)0.0072443965;
          }
        }
      } else {
        if ( (data[5].fvalue < 10.408293)) {
          if ( (data[5].fvalue < 10.328336)) {
            if ( (data[7].fvalue < 0.5579592)) {
              sum += (float)0.028962072;
            } else {
              sum += (float)0.0025447314;
            }
          } else {
            sum += (float)0.053287521;
          }
        } else {
          if ( (data[7].fvalue < 0.43561685)) {
            if ( (data[7].fvalue < 0.29932413)) {
              sum += (float)0.017846864;
            } else {
              sum += (float)0.00782221;
            }
          } else {
            if ( (data[5].fvalue < 16.887112)) {
              sum += (float)0.0080484096;
            } else {
              sum += (float)-0.013369814;
            }
          }
        }
      }
    }
  }
  sum = sum + (float)(0.5);
  if (!pred_margin) {
    return pred_transform(sum);
  } else {
    return sum;
  }
}
};
