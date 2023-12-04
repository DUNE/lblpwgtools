#include "BDTReweighter.h"
class GeneratorReweight_numu_FD_FHC : public BDTReweighter {
 public :
 GeneratorReweight_numu_FD_FHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_numu_FD_FHC() {;}


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
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.062635034)) {
      if ( (data[13].fvalue < 1.0892229)) {
        if ( (data[5].fvalue < 1.382669)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.10566148;
            } else {
              sum += (float)0.0037553655;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.3316409;
            } else {
              sum += (float)0.18007725;
            }
          }
        } else {
          if ( (data[14].fvalue < 2.0232133e-07)) {
            if ( (data[13].fvalue < 0.032124039)) {
              sum += (float)0.372866;
            } else {
              sum += (float)0.16446231;
            }
          } else {
            if ( (data[14].fvalue < 0.015759595)) {
              sum += (float)-0.15344575;
            } else {
              sum += (float)0.043914698;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.78618687)) {
          if ( (data[4].fvalue < 1.5275242)) {
            if ( (data[17].fvalue < 0.015043409)) {
              sum += (float)-0.096410148;
            } else {
              sum += (float)-0.2760472;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.38634944;
            } else {
              sum += (float)-0.18966506;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.43766028)) {
            sum += (float)0.064873524;
          } else {
            sum += (float)-0.10612065;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.013871467)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.24369098)) {
              sum += (float)-0.29911324;
            } else {
              sum += (float)-0.11743935;
            }
          } else {
            if ( (data[13].fvalue < 1.1462755e-05)) {
              sum += (float)0.57858384;
            } else {
              sum += (float)0.0021104617;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.0042550522;
            } else {
              sum += (float)0.2884348;
            }
          } else {
            if ( (data[5].fvalue < 0.30390358)) {
              sum += (float)-0.036506109;
            } else {
              sum += (float)-0.31690323;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0850785)) {
            if ( (data[14].fvalue < 0.36197948)) {
              sum += (float)-0.0079508927;
            } else {
              sum += (float)0.21355137;
            }
          } else {
            if ( (data[5].fvalue < 0.30345291)) {
              sum += (float)0.29496163;
            } else {
              sum += (float)0.21174178;
            }
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.11568608;
            } else {
              sum += (float)-0.051421616;
            }
          } else {
            if ( (data[3].fvalue < 0.97041202)) {
              sum += (float)0.35582078;
            } else {
              sum += (float)0.14441201;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[13].fvalue < 0.1483655)) {
        if ( (data[14].fvalue < 0.19872443)) {
          sum += (float)0.53891116;
        } else {
          sum += (float)0.29944077;
        }
      } else {
        if ( (data[16].fvalue < 0.0038565099)) {
          if ( (data[14].fvalue < 0.11587854)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.31872368;
            } else {
              sum += (float)0.08461608;
            }
          } else {
            if ( (data[3].fvalue < 1.2642398)) {
              sum += (float)0.37033153;
            } else {
              sum += (float)0.21950714;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.42321244)) {
            if ( (data[15].fvalue < 0.14771307)) {
              sum += (float)0.060294233;
            } else {
              sum += (float)0.24408525;
            }
          } else {
            sum += (float)-0.15531242;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.002017336)) {
        if ( (data[3].fvalue < 1.2913357)) {
          sum += (float)0.56392705;
        } else {
          sum += (float)0.45469502;
        }
      } else {
        if ( (data[5].fvalue < 0.39549816)) {
          sum += (float)0.4224543;
        } else {
          sum += (float)0.12125304;
        }
      }
    }
  }
  if ( (data[8].fvalue < 4.5)) {
    if ( (data[14].fvalue < 0.14919458)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.022174597)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.014144375)) {
              sum += (float)-0.057382997;
            } else {
              sum += (float)-0.19886886;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.083671235;
            } else {
              sum += (float)-0.27788615;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.74044037)) {
              sum += (float)-0.20615758;
            } else {
              sum += (float)-0.40386155;
            }
          } else {
            if (  (data[4].fvalue < 1.0568776)) {
              sum += (float)-0.033503037;
            } else {
              sum += (float)0.14278716;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.34966511)) {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[15].fvalue < 0.14450118)) {
              sum += (float)-0.13833837;
            } else {
              sum += (float)0.11346032;
            }
          } else {
            if (  (data[4].fvalue < 1.1295626)) {
              sum += (float)0.48400447;
            } else {
              sum += (float)0.012906319;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.048214629)) {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)-0.4015981;
            } else {
              sum += (float)-0.23148383;
            }
          } else {
            if ( (data[7].fvalue < 6.5)) {
              sum += (float)-0.20002656;
            } else {
              sum += (float)0.15810508;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.021802312)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[5].fvalue < 0.36396536)) {
              sum += (float)-0.030040242;
            } else {
              sum += (float)-0.17873915;
            }
          } else {
            if ( (data[13].fvalue < 6.9514715e-07)) {
              sum += (float)0.43648243;
            } else {
              sum += (float)0.090861171;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.20928486;
            }
          } else {
            if ( (data[3].fvalue < 0.39221135)) {
              sum += (float)-0.019445855;
            } else {
              sum += (float)-0.23687987;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.39329079)) {
            if ( (data[14].fvalue < 1.0177457)) {
              sum += (float)0.2310874;
            } else {
              sum += (float)0.0050112931;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.16562656;
            } else {
              sum += (float)0.038575701;
            }
          }
        } else {
          if ( (data[7].fvalue < 5.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.12798588;
            } else {
              sum += (float)-0.01985277;
            }
          } else {
            if ( (data[13].fvalue < 0.36840653)) {
              sum += (float)0.056552146;
            } else {
              sum += (float)0.29112756;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 6.5)) {
      if ( (data[3].fvalue < 0.73340636)) {
        if (  (data[4].fvalue < 1.1093391)) {
          if ( (data[3].fvalue < 0.44300082)) {
            sum += (float)0.48809439;
          } else {
            sum += (float)0.30624664;
          }
        } else {
          if ( (data[15].fvalue < 0.12497927)) {
            if ( (data[14].fvalue < 0.31353647)) {
              sum += (float)0.10181173;
            } else {
              sum += (float)0.26886973;
            }
          } else {
            if (  (data[4].fvalue < 1.691299)) {
              sum += (float)0.39247024;
            } else {
              sum += (float)0.19062723;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.33815864)) {
          if ( (data[13].fvalue < 0.34135222)) {
            if (  (data[4].fvalue < 1.4340749)) {
              sum += (float)-0;
            } else {
              sum += (float)0.20950355;
            }
          } else {
            if ( (data[14].fvalue < 0.20165847)) {
              sum += (float)-0.26824683;
            } else {
              sum += (float)-0.043797471;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.45311761)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.21667728;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[16].fvalue < 0.013456017)) {
              sum += (float)0.27648333;
            } else {
              sum += (float)0.092411324;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.1703756)) {
        if (  (data[4].fvalue < 1.8203205)) {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[3].fvalue < 0.52252448)) {
              sum += (float)0.42744046;
            } else {
              sum += (float)0.29286766;
            }
          } else {
            sum += (float)0.45970085;
          }
        } else {
          if ( (data[8].fvalue < 9.5)) {
            sum += (float)0.26453179;
          } else {
            sum += (float)0.38303781;
          }
        }
      } else {
        if ( (data[8].fvalue < 9.5)) {
          if ( (data[5].fvalue < 0.72102022)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)0.2312939;
            } else {
              sum += (float)0.085194893;
            }
          } else {
            sum += (float)0.028065134;
          }
        } else {
          sum += (float)0.3629255;
        }
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[13].fvalue < 1.464202)) {
        if ( (data[14].fvalue < 0.18786603)) {
          if ( (data[16].fvalue < 0.0070137936)) {
            if ( (data[5].fvalue < 1.2540343)) {
              sum += (float)-0.032317534;
            } else {
              sum += (float)0.095208436;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.1601956;
            } else {
              sum += (float)0.087888606;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.26168561)) {
            if ( (data[14].fvalue < 1.1485827)) {
              sum += (float)0.12454327;
            } else {
              sum += (float)-0.051162213;
            }
          } else {
            if ( (data[13].fvalue < 0.0088586481)) {
              sum += (float)-0.059414286;
            } else {
              sum += (float)0.036667775;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.49907508)) {
          if ( (data[4].fvalue < 1.6749251)) {
            if ( (data[17].fvalue < 0.034973718)) {
              sum += (float)-0.064379238;
            } else {
              sum += (float)-0.19981414;
            }
          } else {
            if ( (data[15].fvalue < 0.79906046)) {
              sum += (float)-0.3114956;
            } else {
              sum += (float)-0.06968265;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.034012314)) {
            if ( (data[5].fvalue < 0.42412889)) {
              sum += (float)0.1011875;
            } else {
              sum += (float)-0.022627434;
            }
          } else {
            sum += (float)-0.10735737;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.45906621)) {
        if (  (data[4].fvalue < 1.1139081)) {
          sum += (float)0.40762919;
        } else {
          if ( (data[8].fvalue < 7.5)) {
            if ( (data[15].fvalue < 0.00036315137)) {
              sum += (float)0.12847403;
            } else {
              sum += (float)0.23467296;
            }
          } else {
            sum += (float)0.33334136;
          }
        }
      } else {
        if ( (data[8].fvalue < 7.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)0.13667764;
            } else {
              sum += (float)0.29142565;
            }
          } else {
            if ( (data[15].fvalue < 0.24522036)) {
              sum += (float)-0.00074678526;
            } else {
              sum += (float)0.12307896;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            sum += (float)0.3549009;
          } else {
            if ( (data[3].fvalue < 1.5381074)) {
              sum += (float)0.23304041;
            } else {
              sum += (float)0.067265198;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[3].fvalue < 0.78480113)) {
        if (  (data[4].fvalue < 1.2144761)) {
          sum += (float)0.38476652;
        } else {
          if ( (data[16].fvalue < 0.0042661233)) {
            sum += (float)0.28903684;
          } else {
            sum += (float)0.15983608;
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          sum += (float)0.30903262;
        } else {
          if ( (data[16].fvalue < 0.0018053318)) {
            if ( (data[14].fvalue < 0.3638773)) {
              sum += (float)0.074562185;
            } else {
              sum += (float)0.18656532;
            }
          } else {
            sum += (float)-0.021227857;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 2.4467692)) {
        if ( (data[16].fvalue < 0.0057057841)) {
          sum += (float)0.3895075;
        } else {
          sum += (float)0.29124072;
        }
      } else {
        sum += (float)0.19254375;
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.24738063)) {
      if ( (data[3].fvalue < 1.0067992)) {
        if ( (data[7].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.1183683)) {
            if ( (data[14].fvalue < 0.076968245)) {
              sum += (float)-0.46402779;
            } else {
              sum += (float)-0.24320091;
            }
          } else {
            if ( (data[15].fvalue < 0.031136114)) {
              sum += (float)0.17040071;
            } else {
              sum += (float)-0.099537052;
            }
          }
        } else {
          if ( (data[13].fvalue < 6.112888e-08)) {
            sum += (float)0.55710483;
          } else {
            if ( (data[13].fvalue < 0.96869081)) {
              sum += (float)-0.003572192;
            } else {
              sum += (float)-0.15348995;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.037516989)) {
              sum += (float)-0.011466812;
            } else {
              sum += (float)-0.1013966;
            }
          } else {
            if ( (data[4].fvalue < 1.6190544)) {
              sum += (float)-0.19347568;
            } else {
              sum += (float)-0.35765767;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.55080914)) {
            if ( (data[13].fvalue < 1.1504166)) {
              sum += (float)-0.073443964;
            } else {
              sum += (float)-0.28797293;
            }
          } else {
            if ( (data[15].fvalue < 0.29310733)) {
              sum += (float)-0.32840759;
            } else {
              sum += (float)-0.17532767;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.42040515)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.59742159)) {
            if ( (data[13].fvalue < 0.0076057692)) {
              sum += (float)-0.1106917;
            } else {
              sum += (float)0.10540627;
            }
          } else {
            if ( (data[17].fvalue < 0.033011608)) {
              sum += (float)0.18314365;
            } else {
              sum += (float)0.038998105;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)0.1567331;
            } else {
              sum += (float)0.29453215;
            }
          } else {
            if ( (data[13].fvalue < 0.11035946)) {
              sum += (float)-0.13669646;
            } else {
              sum += (float)0.1168126;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.30260804)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.060508262;
            } else {
              sum += (float)0.090672448;
            }
          } else {
            if ( (data[13].fvalue < 0.076450333)) {
              sum += (float)-0.30956918;
            } else {
              sum += (float)-0.10206636;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.0077580176)) {
            if ( (data[14].fvalue < 0.40477014)) {
              sum += (float)0.033445321;
            } else {
              sum += (float)0.17831111;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.054085128;
            } else {
              sum += (float)-0.11667872;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[3].fvalue < 0.92166626)) {
        if (  (data[4].fvalue < 1.1066612)) {
          sum += (float)0.36085758;
        } else {
          if ( (data[8].fvalue < 6.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.24387899;
            } else {
              sum += (float)0.095602214;
            }
          } else {
            if (  (data[4].fvalue < 1.781919)) {
              sum += (float)0.30598348;
            } else {
              sum += (float)0.13591127;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)0.26647112;
        } else {
          if ( (data[14].fvalue < 0.17981111)) {
            if ( (data[13].fvalue < 0.53926688)) {
              sum += (float)0.013033868;
            } else {
              sum += (float)-0.20601974;
            }
          } else {
            if ( (data[17].fvalue < 0.01609966)) {
              sum += (float)0.12270305;
            } else {
              sum += (float)-0.0066696908;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.9073169)) {
        if ( (data[10].fvalue < 0.5)) {
          sum += (float)0.34639871;
        } else {
          sum += (float)0.24590185;
        }
      } else {
        sum += (float)0.18149789;
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[14].fvalue < 0.034330916)) {
      if ( (data[14].fvalue < 4.1931483e-07)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.0096186511)) {
              sum += (float)-0.01857714;
            } else {
              sum += (float)-0.12433513;
            }
          } else {
            if ( (data[15].fvalue < 0.13520412)) {
              sum += (float)0.048391081;
            } else {
              sum += (float)0.18453281;
            }
          }
        } else {
          sum += (float)0.61014056;
        }
      } else {
        if ( (data[14].fvalue < 0.0077552744)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.41652906;
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.13673949;
            } else {
              sum += (float)-0.27981123;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.050661035)) {
              sum += (float)-0.17056838;
            } else {
              sum += (float)0.049307693;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.22623429;
            } else {
              sum += (float)0.16657975;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.30624914)) {
        if ( (data[13].fvalue < 0.023746487)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.01284899)) {
              sum += (float)0.012944136;
            } else {
              sum += (float)-0.095906325;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.15573758;
            } else {
              sum += (float)-0.065148801;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.17027345;
            } else {
              sum += (float)0.027636424;
            }
          } else {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)-0.048560087;
            } else {
              sum += (float)0.12491883;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.03315258)) {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.30121112)) {
              sum += (float)-0.12787916;
            } else {
              sum += (float)-0.042088062;
            }
          } else {
            if (  (data[4].fvalue < 2.117413)) {
              sum += (float)0.14214739;
            } else {
              sum += (float)0.00032670039;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0677152)) {
              sum += (float)-0.0039180419;
            } else {
              sum += (float)0.11240707;
            }
          } else {
            if ( (data[3].fvalue < 0.26161978)) {
              sum += (float)0.26209161;
            } else {
              sum += (float)-0.038561359;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.4818457)) {
      if ( (data[14].fvalue < 0.21583283)) {
        if ( (data[13].fvalue < 0.31070149)) {
          sum += (float)0.34332141;
        } else {
          sum += (float)0.18905596;
        }
      } else {
        if ( (data[8].fvalue < 9.5)) {
          if (  (data[4].fvalue < 1.2551167)) {
            sum += (float)0.26680171;
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.235091;
            } else {
              sum += (float)0.1122633;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.901737)) {
            sum += (float)0.32360044;
          } else {
            sum += (float)0.17717059;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 10.5)) {
        if ( (data[14].fvalue < 0.83460522)) {
          if ( (data[15].fvalue < 0.30447686)) {
            sum += (float)-0.1138126;
          } else {
            sum += (float)0.087012462;
          }
        } else {
          sum += (float)0.11400463;
        }
      } else {
        sum += (float)0.20146753;
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[5].fvalue < 0.20855083)) {
      if ( (data[13].fvalue < 1.312284)) {
        if ( (data[14].fvalue < 0.018342888)) {
          if ( (data[13].fvalue < 0.11565894)) {
            if ( (data[17].fvalue < 0.66507542)) {
              sum += (float)-0.029645124;
            } else {
              sum += (float)-0.28075993;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.072378576;
            } else {
              sum += (float)-0.043967739;
            }
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[17].fvalue < 0.15538497)) {
              sum += (float)-0.0103809;
            } else {
              sum += (float)0.1121843;
            }
          } else {
            if ( (data[3].fvalue < 0.19954328)) {
              sum += (float)0.18777432;
            } else {
              sum += (float)0.061487909;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.42348504)) {
          if ( (data[15].fvalue < 0.44245446)) {
            if (  (data[4].fvalue < 2.0925117)) {
              sum += (float)-0.14875253;
            } else {
              sum += (float)-0.27018124;
            }
          } else {
            sum += (float)-0.04619791;
          }
        } else {
          sum += (float)0.055092052;
        }
      }
    } else {
      if ( (data[5].fvalue < 1.2357943)) {
        if ( (data[16].fvalue < 0.0023860123)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.014079587;
            } else {
              sum += (float)-0.074465737;
            }
          } else {
            if ( (data[6].fvalue < 0.14427325)) {
              sum += (float)0.27250227;
            } else {
              sum += (float)0.05877772;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.00021165059)) {
            if ( (data[16].fvalue < 0.4849571)) {
              sum += (float)-0.26965129;
            } else {
              sum += (float)-0.035455909;
            }
          } else {
            if ( (data[14].fvalue < 0.0071860328)) {
              sum += (float)-0.10798834;
            } else {
              sum += (float)0.017409386;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[14].fvalue < 1.0042978e-06)) {
            if ( (data[4].fvalue < 0.78683925)) {
              sum += (float)0.19627337;
            } else {
              sum += (float)0.071644634;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.045400284;
            } else {
              sum += (float)0.16703394;
            }
          }
        } else {
          sum += (float)0.31241378;
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 9.5)) {
      if ( (data[16].fvalue < 0.00039345748)) {
        if ( (data[13].fvalue < 0.20057566)) {
          sum += (float)0.27776542;
        } else {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.28224236;
          } else {
            if ( (data[17].fvalue < 0.0026534311)) {
              sum += (float)0.14859614;
            } else {
              sum += (float)0.02098343;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.11954658)) {
          sum += (float)-0.14737521;
        } else {
          if ( (data[15].fvalue < 0.12239914)) {
            sum += (float)-0;
          } else {
            sum += (float)0.13187085;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.0028745891)) {
        sum += (float)0.30909395;
      } else {
        sum += (float)0.18178555;
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[12].fvalue < 0.5)) {
        if (  (data[4].fvalue < 1.0497911)) {
          if ( (data[5].fvalue < 1.1194364)) {
            if ( (data[14].fvalue < 3.7129001e-05)) {
              sum += (float)-0.033721924;
            } else {
              sum += (float)-0.097774729;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.051761284;
            } else {
              sum += (float)-0.10300796;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.038207825)) {
            if ( (data[15].fvalue < 0.038251858)) {
              sum += (float)0.093507901;
            } else {
              sum += (float)0.0099026812;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.051035788;
            } else {
              sum += (float)0.058154933;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.6825289)) {
          if ( (data[3].fvalue < 0.62238514)) {
            sum += (float)0.17010915;
          } else {
            if ( (data[13].fvalue < 0.47105086)) {
              sum += (float)-0.051785626;
            } else {
              sum += (float)-0.20644785;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.4834449)) {
            if (  (data[4].fvalue < 1.8095337)) {
              sum += (float)-0.18874639;
            } else {
              sum += (float)-0.34936816;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[6].fvalue < 0.07378491)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.11055638)) {
              sum += (float)-0.065911621;
            } else {
              sum += (float)0.12314612;
            }
          } else {
            sum += (float)0.30547458;
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.045722697;
            } else {
              sum += (float)0.10586198;
            }
          } else {
            if ( (data[14].fvalue < 0.096019678)) {
              sum += (float)-0.20435539;
            } else {
              sum += (float)-0.07147447;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.096657291)) {
          sum += (float)0.36025044;
        } else {
          if ( (data[14].fvalue < 0.21068498)) {
            if ( (data[13].fvalue < 0.23967554)) {
              sum += (float)0.062991209;
            } else {
              sum += (float)-0.22747268;
            }
          } else {
            if ( (data[13].fvalue < 0.15700176)) {
              sum += (float)-0.10814221;
            } else {
              sum += (float)0.085196905;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.10738158)) {
      if (  (data[4].fvalue < 1.0917815)) {
        sum += (float)0.36725867;
      } else {
        sum += (float)0.14016633;
      }
    } else {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[15].fvalue < 0.14093536)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.60011327)) {
              sum += (float)0.02032664;
            } else {
              sum += (float)0.1309315;
            }
          } else {
            if ( (data[16].fvalue < 0.16416553)) {
              sum += (float)-0.22064818;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.849682)) {
            if (  (data[4].fvalue < 1.7731664)) {
              sum += (float)0.26274243;
            } else {
              sum += (float)0.11421073;
            }
          } else {
            if ( (data[15].fvalue < 0.73650432)) {
              sum += (float)-0.0059898961;
            } else {
              sum += (float)0.098834179;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 10.5)) {
          if ( (data[3].fvalue < 1.6640294)) {
            if ( (data[13].fvalue < 0.27008489)) {
              sum += (float)0.28377968;
            } else {
              sum += (float)0.14789982;
            }
          } else {
            sum += (float)0.0014085586;
          }
        } else {
          sum += (float)0.28104439;
        }
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[13].fvalue < 1.6203016)) {
      if ( (data[5].fvalue < 0.38087693)) {
        if ( (data[13].fvalue < 0.14772907)) {
          if ( (data[14].fvalue < 0.013853364)) {
            if ( (data[13].fvalue < 1.9607762e-06)) {
              sum += (float)0.044749636;
            } else {
              sum += (float)-0.098701999;
            }
          } else {
            if ( (data[13].fvalue < 0.011262781)) {
              sum += (float)-0.020585444;
            } else {
              sum += (float)0.058674179;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.75089598)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.00069585681;
            } else {
              sum += (float)0.060087174;
            }
          } else {
            if ( (data[13].fvalue < 0.34592676)) {
              sum += (float)0.037984636;
            } else {
              sum += (float)0.20254092;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.033555329)) {
          if ( (data[5].fvalue < 1.4795392)) {
            if ( (data[16].fvalue < 0.0065048025)) {
              sum += (float)-0.0059734816;
            } else {
              sum += (float)-0.087053277;
            }
          } else {
            if ( (data[4].fvalue < 0.80974436)) {
              sum += (float)0.15852493;
            } else {
              sum += (float)0.059310876;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.12238722)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.11474922;
            } else {
              sum += (float)-0.26134056;
            }
          } else {
            if ( (data[14].fvalue < 0.01428337)) {
              sum += (float)-0.092682548;
            } else {
              sum += (float)-0.013118842;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.65452272)) {
        if ( (data[4].fvalue < 1.8059099)) {
          if ( (data[17].fvalue < 0.019832678)) {
            if ( (data[15].fvalue < 0.021600001)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.082939804;
            }
          } else {
            sum += (float)-0.14981444;
          }
        } else {
          if ( (data[15].fvalue < 0.61288905)) {
            if ( (data[13].fvalue < 2.0814815)) {
              sum += (float)-0.15035433;
            } else {
              sum += (float)-0.25279978;
            }
          } else {
            sum += (float)-0.013892696;
          }
        }
      } else {
        sum += (float)0.052125294;
      }
    }
  } else {
    if ( (data[3].fvalue < 0.77054423)) {
      if ( (data[8].fvalue < 7.5)) {
        if (  (data[4].fvalue < 1.1380527)) {
          sum += (float)0.27107698;
        } else {
          if ( (data[13].fvalue < 0.51888621)) {
            if ( (data[3].fvalue < 0.39372933)) {
              sum += (float)0.13243097;
            } else {
              sum += (float)-0.0066510076;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.24824156;
            } else {
              sum += (float)0.10437247;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.6158162)) {
          sum += (float)0.30534762;
        } else {
          sum += (float)0.17006642;
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        sum += (float)0.21434322;
      } else {
        if ( (data[7].fvalue < 10.5)) {
          if ( (data[15].fvalue < 0.41352883)) {
            if ( (data[16].fvalue < 0.001878513)) {
              sum += (float)0.013721901;
            } else {
              sum += (float)-0.14058049;
            }
          } else {
            sum += (float)0.13162427;
          }
        } else {
          sum += (float)0.20219764;
        }
      }
    }
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[9].fvalue < 1.5)) {
      if ( (data[16].fvalue < 0.0046563763)) {
        if ( (data[15].fvalue < 2.1540651)) {
          if ( (data[14].fvalue < 1.4034814)) {
            if ( (data[12].fvalue < 0.5)) {
              sum += (float)0.0065659285;
            } else {
              sum += (float)-0.14089391;
            }
          } else {
            if ( (data[15].fvalue < 0.62937284)) {
              sum += (float)-0.16240306;
            } else {
              sum += (float)0.13761479;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.49698687)) {
            if ( (data[13].fvalue < 0.11474565)) {
              sum += (float)-0.31528857;
            } else {
              sum += (float)0.037168209;
            }
          } else {
            sum += (float)0.087403804;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[9].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.9687941)) {
              sum += (float)-0.43513435;
            } else {
              sum += (float)-0.16610217;
            }
          } else {
            if ( (data[14].fvalue < 0.0014845855)) {
              sum += (float)-0.14001048;
            } else {
              sum += (float)0.028254533;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.00016526016)) {
              sum += (float)-0.037039872;
            } else {
              sum += (float)0.097931407;
            }
          } else {
            if ( (data[4].fvalue < 1.4584398)) {
              sum += (float)-0.29107016;
            } else {
              sum += (float)-0.047685508;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 1.1885185)) {
        if ( (data[4].fvalue < 1.348515)) {
          sum += (float)-0.069136485;
        } else {
          if (  (data[4].fvalue < 2.797205)) {
            if ( (data[9].fvalue < 2.5)) {
              sum += (float)0.088481262;
            } else {
              sum += (float)0.24606472;
            }
          } else {
            sum += (float)-0.026722293;
          }
        }
      } else {
        if ( (data[0].fvalue < 4.3386869)) {
          sum += (float)-0.12164024;
        } else {
          sum += (float)0.045365788;
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.28438681)) {
      if ( (data[6].fvalue < 0.15861106)) {
        sum += (float)0.2779057;
      } else {
        if ( (data[0].fvalue < 2.3886602)) {
          sum += (float)0.27253386;
        } else {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[14].fvalue < 0.35285947)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12722862;
            }
          } else {
            sum += (float)0.21364628;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 2.020196)) {
        if ( (data[8].fvalue < 8.5)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.1579318)) {
              sum += (float)0.086535007;
            } else {
              sum += (float)0.19413911;
            }
          } else {
            if ( (data[0].fvalue < 1.5329181)) {
              sum += (float)0.14202477;
            } else {
              sum += (float)0.011921965;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.38243482)) {
            if ( (data[14].fvalue < 0.33303389)) {
              sum += (float)0.28452232;
            } else {
              sum += (float)0.15868051;
            }
          } else {
            sum += (float)0.097350515;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.74490249)) {
          if ( (data[16].fvalue < 0.0015539479)) {
            if ( (data[14].fvalue < 0.80534756)) {
              sum += (float)-0;
            } else {
              sum += (float)0.077307984;
            }
          } else {
            sum += (float)-0.090754524;
          }
        } else {
          sum += (float)-0.18689668;
        }
      }
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[5].fvalue < 0.13060015)) {
      if (  (data[4].fvalue < 1.7695901)) {
        if (  (data[4].fvalue < 1.4318584)) {
          if ( (data[15].fvalue < 0.12878487)) {
            if ( (data[17].fvalue < 0.11021094)) {
              sum += (float)-0.026375381;
            } else {
              sum += (float)0.16849078;
            }
          } else {
            if ( (data[13].fvalue < 0.01233456)) {
              sum += (float)-0.0022430024;
            } else {
              sum += (float)0.19434665;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.059301995)) {
            if ( (data[17].fvalue < 0.28919291)) {
              sum += (float)0.16062728;
            } else {
              sum += (float)0.28138122;
            }
          } else {
            if ( (data[13].fvalue < 0.045948885)) {
              sum += (float)0.026829382;
            } else {
              sum += (float)0.095789708;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.1524029)) {
          if ( (data[15].fvalue < 0.56743371)) {
            if ( (data[15].fvalue < 0.028594086)) {
              sum += (float)-0.0021895913;
            } else {
              sum += (float)-0.11350229;
            }
          } else {
            if ( (data[14].fvalue < 0.015945794)) {
              sum += (float)-0.060190435;
            } else {
              sum += (float)0.091849767;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.4191794)) {
            if ( (data[13].fvalue < 0.23698255)) {
              sum += (float)0.0042746514;
            } else {
              sum += (float)0.11102264;
            }
          } else {
            sum += (float)-0.11146637;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.32750106)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.20301965)) {
            if ( (data[15].fvalue < 0.76345348)) {
              sum += (float)-0.012670294;
            } else {
              sum += (float)-0.19680205;
            }
          } else {
            if ( (data[14].fvalue < 0.014261776)) {
              sum += (float)-0.010114887;
            } else {
              sum += (float)0.097788952;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.10489237)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.074046545;
            } else {
              sum += (float)0.22549978;
            }
          } else {
            if ( (data[15].fvalue < 0.13741168)) {
              sum += (float)-0.10563113;
            } else {
              sum += (float)0.014758808;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.39056093)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0024969466;
            } else {
              sum += (float)0.083691239;
            }
          } else {
            if ( (data[13].fvalue < 0.05292432)) {
              sum += (float)-0.31104302;
            } else {
              sum += (float)-0.045828257;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.010276685)) {
            if ( (data[14].fvalue < 0.48841289)) {
              sum += (float)0.099758878;
            } else {
              sum += (float)0.23075308;
            }
          } else {
            if ( (data[15].fvalue < 0.0078420779)) {
              sum += (float)0.10634913;
            } else {
              sum += (float)-0.015894417;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.24756826)) {
      if ( (data[13].fvalue < 0.21169847)) {
        sum += (float)0.28561568;
      } else {
        sum += (float)0.14442511;
      }
    } else {
      if ( (data[17].fvalue < 0.0050389944)) {
        if ( (data[8].fvalue < 10.5)) {
          if ( (data[14].fvalue < 0.67150474)) {
            if ( (data[6].fvalue < 0.41889584)) {
              sum += (float)0.13489288;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.17363194;
          }
        } else {
          sum += (float)0.20579134;
        }
      } else {
        if ( (data[17].fvalue < 0.22850698)) {
          sum += (float)-0.12649307;
        } else {
          sum += (float)0.048578601;
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[13].fvalue < 0.19514957)) {
        if ( (data[14].fvalue < 0.11396457)) {
          if ( (data[17].fvalue < 0.71592987)) {
            if ( (data[15].fvalue < 0.85809493)) {
              sum += (float)-0.0088511528;
            } else {
              sum += (float)-0.16413194;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.25072476;
            } else {
              sum += (float)0.03107092;
            }
          }
        } else {
          if ( (data[14].fvalue < 1.1221455)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.013265784;
            } else {
              sum += (float)0.091328502;
            }
          } else {
            if ( (data[15].fvalue < 0.87828499)) {
              sum += (float)-0.090663172;
            } else {
              sum += (float)0.14715056;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.007625625)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.0049383179;
            } else {
              sum += (float)-0.10459043;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.079098761;
            } else {
              sum += (float)-0.2005503;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.066610649;
            } else {
              sum += (float)0.16451591;
            }
          } else {
            if ( (data[14].fvalue < 0.055163704)) {
              sum += (float)-0.20592751;
            } else {
              sum += (float)0.011148344;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.05109835)) {
        if ( (data[14].fvalue < 0.013368303)) {
          sum += (float)0.063683286;
        } else {
          sum += (float)0.31735015;
        }
      } else {
        if ( (data[15].fvalue < 0.14101388)) {
          if ( (data[14].fvalue < 0.18265721)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.01561322;
            } else {
              sum += (float)-0.13612631;
            }
          } else {
            if ( (data[13].fvalue < 0.27778685)) {
              sum += (float)-0.10074966;
            } else {
              sum += (float)0.055511728;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.2609241)) {
            if ( (data[13].fvalue < 0.90942878)) {
              sum += (float)0.073431261;
            } else {
              sum += (float)-0.10722198;
            }
          } else {
            if ( (data[15].fvalue < 0.89905667)) {
              sum += (float)-0.061923373;
            } else {
              sum += (float)0.032491911;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.14503047)) {
      if (  (data[4].fvalue < 1.1027725)) {
        sum += (float)0.31108275;
      } else {
        if ( (data[8].fvalue < 3.5)) {
          sum += (float)-0.061121251;
        } else {
          sum += (float)0.1886449;
        }
      }
    } else {
      if ( (data[10].fvalue < 1.5)) {
        if ( (data[7].fvalue < 9.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.68687367)) {
              sum += (float)0.041152786;
            } else {
              sum += (float)0.27575353;
            }
          } else {
            if ( (data[14].fvalue < 0.049756214)) {
              sum += (float)-0.084904142;
            } else {
              sum += (float)0.043091197;
            }
          }
        } else {
          if ( (data[7].fvalue < 11.5)) {
            sum += (float)0.13794747;
          } else {
            sum += (float)0.25076565;
          }
        }
      } else {
        sum += (float)-0.20588519;
      }
    }
  }
  if ( (data[13].fvalue < 1.8679831)) {
    if ( (data[5].fvalue < 0.45384008)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.074107289)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[17].fvalue < 0.034352742)) {
              sum += (float)0.0087302327;
            } else {
              sum += (float)-0.050701182;
            }
          } else {
            if ( (data[14].fvalue < 0.11053301)) {
              sum += (float)0.30957171;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0248387)) {
            if ( (data[3].fvalue < 0.084471166)) {
              sum += (float)-0.28055066;
            } else {
              sum += (float)-0.070620015;
            }
          } else {
            if (  (data[4].fvalue < 1.1406999)) {
              sum += (float)0.1378924;
            } else {
              sum += (float)0.021022227;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7457318)) {
          if ( (data[3].fvalue < 0.43855238)) {
            sum += (float)0.22412263;
          } else {
            sum += (float)-0.011219276;
          }
        } else {
          if (  (data[4].fvalue < 2.280221)) {
            if (  (data[4].fvalue < 1.8655946)) {
              sum += (float)-0.15406398;
            } else {
              sum += (float)-0.3257134;
            }
          } else {
            sum += (float)-0.040898111;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.015426553)) {
        if ( (data[5].fvalue < 1.6706264)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.00040987032;
            } else {
              sum += (float)-0.039833117;
            }
          } else {
            if ( (data[3].fvalue < 0.56009722)) {
              sum += (float)0.23691066;
            } else {
              sum += (float)-0.0036002139;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.30342093)) {
            if ( (data[13].fvalue < 0.021533668)) {
              sum += (float)0.10199849;
            } else {
              sum += (float)-0.022668481;
            }
          } else {
            if ( (data[5].fvalue < 1.9462525)) {
              sum += (float)0.085122332;
            } else {
              sum += (float)0.20036812;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.12396558)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.063371085;
            } else {
              sum += (float)-0.1962996;
            }
          } else {
            sum += (float)-0.2916069;
          }
        } else {
          if ( (data[3].fvalue < 1.2945735)) {
            if ( (data[17].fvalue < 0.5176118)) {
              sum += (float)0.017744441;
            } else {
              sum += (float)-0.1261964;
            }
          } else {
            if ( (data[15].fvalue < 0.19326025)) {
              sum += (float)-0.084399574;
            } else {
              sum += (float)-0.019001149;
            }
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.013832189)) {
      if ( (data[17].fvalue < 0.4122214)) {
        if ( (data[17].fvalue < 0.025480352)) {
          if ( (data[4].fvalue < 1.9097583)) {
            sum += (float)0.0043728761;
          } else {
            sum += (float)-0.078934304;
          }
        } else {
          sum += (float)-0.14547332;
        }
      } else {
        sum += (float)0.046354719;
      }
    } else {
      if (  (data[4].fvalue < 2.6165581)) {
        if (  (data[4].fvalue < 2.0169263)) {
          sum += (float)-0.13504684;
        } else {
          sum += (float)-0.23987108;
        }
      } else {
        sum += (float)-0.018232258;
      }
    }
  }
  if ( (data[14].fvalue < 0.28840286)) {
    if ( (data[13].fvalue < 1.3281536)) {
      if ( (data[8].fvalue < 7.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 1.9330053)) {
            if (  (data[4].fvalue < 2.05352)) {
              sum += (float)0.00022104163;
            } else {
              sum += (float)0.071333297;
            }
          } else {
            if ( (data[13].fvalue < 0.28635192)) {
              sum += (float)-0.19604635;
            } else {
              sum += (float)0.08306282;
            }
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[14].fvalue < 0.045415927)) {
              sum += (float)-0.15327211;
            } else {
              sum += (float)-0.041072771;
            }
          } else {
            if (  (data[4].fvalue < 1.1036804)) {
              sum += (float)0.19233619;
            } else {
              sum += (float)-0.021895695;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.20444727)) {
          sum += (float)0.24856123;
        } else {
          sum += (float)0.090871587;
        }
      }
    } else {
      if ( (data[17].fvalue < 1.1136791)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[15].fvalue < 0.80332339)) {
              sum += (float)-0.097143918;
            } else {
              sum += (float)0.078900635;
            }
          } else {
            sum += (float)0.1101314;
          }
        } else {
          if ( (data[14].fvalue < 0.18270516)) {
            sum += (float)-0.30148703;
          } else {
            sum += (float)-0.10934947;
          }
        }
      } else {
        sum += (float)0.13927794;
      }
    }
  } else {
    if ( (data[13].fvalue < 0.44125479)) {
      if ( (data[14].fvalue < 1.4498571)) {
        if ( (data[17].fvalue < 0.81586403)) {
          if ( (data[17].fvalue < 0.022099325)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.04837466;
            } else {
              sum += (float)-0.0038001989;
            }
          } else {
            if ( (data[15].fvalue < 1.0544946)) {
              sum += (float)-0.060591888;
            } else {
              sum += (float)0.064803399;
            }
          }
        } else {
          if ( (data[4].fvalue < 2.6134152)) {
            if ( (data[3].fvalue < 2.6795926)) {
              sum += (float)0.14182046;
            } else {
              sum += (float)0.059887003;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.42822725)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.11793726;
            } else {
              sum += (float)-0.25148454;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[17].fvalue < 0.006340865)) {
            if ( (data[4].fvalue < 1.9114411)) {
              sum += (float)-0;
            } else {
              sum += (float)0.1480511;
            }
          } else {
            sum += (float)-0.076852649;
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.010910257)) {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.51386052)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.15808383;
            } else {
              sum += (float)7.6728924e-05;
            }
          } else {
            if ( (data[13].fvalue < 0.52044976)) {
              sum += (float)0.10342899;
            } else {
              sum += (float)0.23672655;
            }
          }
        } else {
          sum += (float)-0.031357724;
        }
      } else {
        if ( (data[14].fvalue < 0.49340653)) {
          if ( (data[15].fvalue < 0.31321055)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.0038751212;
            } else {
              sum += (float)-0.16669881;
            }
          } else {
            if ( (data[13].fvalue < 0.84299755)) {
              sum += (float)0.05974552;
            } else {
              sum += (float)-0.035830136;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.015046973)) {
            if ( (data[3].fvalue < 2.2031245)) {
              sum += (float)0.1614904;
            } else {
              sum += (float)0.027956378;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.047634464;
            } else {
              sum += (float)-0.050601494;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.16292998)) {
    if ( (data[13].fvalue < 0.8481822)) {
      if (  (data[4].fvalue < 1.2607174)) {
        if ( (data[8].fvalue < 3.5)) {
          if (  (data[4].fvalue < 0.99848443)) {
            sum += (float)0.10937103;
          } else {
            if ( (data[15].fvalue < 0.099463172)) {
              sum += (float)-0.050667606;
            } else {
              sum += (float)0.04172539;
            }
          }
        } else {
          sum += (float)0.12743074;
        }
      } else {
        if ( (data[3].fvalue < 0.061359011)) {
          if (  (data[4].fvalue < 1.4434354)) {
            if ( (data[13].fvalue < 0.012436949)) {
              sum += (float)-0.049753085;
            } else {
              sum += (float)0.087396458;
            }
          } else {
            if ( (data[15].fvalue < 0.52940261)) {
              sum += (float)0.18916799;
            } else {
              sum += (float)0.087639004;
            }
          }
        } else {
          if ( (data[0].fvalue < 1.6632193)) {
            if ( (data[6].fvalue < 0.42765895)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12163012;
            }
          } else {
            if ( (data[0].fvalue < 4.5364251)) {
              sum += (float)0.0314437;
            } else {
              sum += (float)-0.02171286;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.38667437)) {
        if (  (data[4].fvalue < 2.4052205)) {
          if ( (data[15].fvalue < 0.035681412)) {
            if ( (data[17].fvalue < 0.23250309)) {
              sum += (float)-0.064305864;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[15].fvalue < 0.37563637)) {
              sum += (float)-0.22348829;
            } else {
              sum += (float)-0.076470293;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.6802301)) {
            sum += (float)0.10087027;
          } else {
            sum += (float)-0.045459792;
          }
        }
      } else {
        sum += (float)0.13982865;
      }
    }
  } else {
    if ( (data[16].fvalue < 0.0018285098)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.3036198)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[17].fvalue < 2.15347)) {
              sum += (float)-0.012929033;
            } else {
              sum += (float)-0.15642509;
            }
          } else {
            if ( (data[6].fvalue < 0.16129734)) {
              sum += (float)0.15825149;
            } else {
              sum += (float)0.016607398;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.42788053)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.023208831;
            } else {
              sum += (float)-0.035308879;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.10576931;
            } else {
              sum += (float)-0.038776573;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.6029916)) {
          sum += (float)0.0021660416;
        } else {
          sum += (float)-0.19015168;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.13707085)) {
        if ( (data[15].fvalue < 0.00020348807)) {
          if ( (data[13].fvalue < 0.54895914)) {
            sum += (float)-0.28075716;
          } else {
            if ( (data[16].fvalue < 0.054213718)) {
              sum += (float)-0.028254874;
            } else {
              sum += (float)-0.12492825;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.083399802;
            } else {
              sum += (float)0.028683161;
            }
          } else {
            if ( (data[3].fvalue < 0.93608856)) {
              sum += (float)-0.05713946;
            } else {
              sum += (float)-0.20309873;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.007650075)) {
            if ( (data[15].fvalue < 0.71562493)) {
              sum += (float)-0.1267788;
            } else {
              sum += (float)0.0030693777;
            }
          } else {
            if ( (data[17].fvalue < 0.020820171)) {
              sum += (float)0.067072526;
            } else {
              sum += (float)-0.025795847;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[17].fvalue < 0.043741606)) {
              sum += (float)0.13049743;
            } else {
              sum += (float)0.034746908;
            }
          } else {
            if ( (data[15].fvalue < 0.00018352915)) {
              sum += (float)-0.069524348;
            } else {
              sum += (float)0.033348978;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 7.5)) {
    if ( (data[13].fvalue < 2.1730208)) {
      if (  (data[4].fvalue < 2.1448684)) {
        if ( (data[3].fvalue < 1.3510885)) {
          if ( (data[14].fvalue < 0.11976336)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.077912822;
            } else {
              sum += (float)-0.0015259054;
            }
          } else {
            if ( (data[15].fvalue < 0.35603708)) {
              sum += (float)0.0085983127;
            } else {
              sum += (float)0.069927037;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.02181299;
            } else {
              sum += (float)0.07547757;
            }
          } else {
            if ( (data[14].fvalue < 0.60895121)) {
              sum += (float)-0.13208431;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.36734068)) {
          if ( (data[17].fvalue < 2.5192442)) {
            if ( (data[15].fvalue < 2.569207)) {
              sum += (float)0.035965893;
            } else {
              sum += (float)-0.089148462;
            }
          } else {
            if ( (data[14].fvalue < 0.070385486)) {
              sum += (float)-0.23483841;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 1.3025273)) {
              sum += (float)0.12750089;
            } else {
              sum += (float)0.01447718;
            }
          } else {
            if ( (data[14].fvalue < 0.56472027)) {
              sum += (float)-0.062240329;
            } else {
              sum += (float)0.080988079;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.38090566)) {
        if ( (data[4].fvalue < 1.7764952)) {
          sum += (float)-0.044168934;
        } else {
          sum += (float)-0.15497567;
        }
      } else {
        sum += (float)-0;
      }
    }
  } else {
    if ( (data[3].fvalue < 0.55442655)) {
      if (  (data[4].fvalue < 1.6166761)) {
        sum += (float)0.21772556;
      } else {
        sum += (float)0.094147801;
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        sum += (float)0.16982321;
      } else {
        if ( (data[13].fvalue < 0.32601103)) {
          sum += (float)0.1276601;
        } else {
          if ( (data[17].fvalue < 0.0027774917)) {
            if ( (data[14].fvalue < 0.37211245)) {
              sum += (float)-0.028448779;
            } else {
              sum += (float)0.053827427;
            }
          } else {
            sum += (float)-0.125267;
          }
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.0072799334)) {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[7].fvalue < 2.5)) {
        if (  (data[4].fvalue < 1.0349442)) {
          if ( (data[13].fvalue < 5.8632645e-07)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.16645133;
            } else {
              sum += (float)0.46232876;
            }
          } else {
            if ( (data[14].fvalue < 3.3256074e-05)) {
              sum += (float)-0.0005612541;
            } else {
              sum += (float)-0.074064322;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.040747605)) {
            if ( (data[17].fvalue < 0.041539222)) {
              sum += (float)0.06339059;
            } else {
              sum += (float)0.0090969158;
            }
          } else {
            if ( (data[14].fvalue < 0.0070591029)) {
              sum += (float)-0.043730777;
            } else {
              sum += (float)0.014376616;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.096933)) {
          if ( (data[3].fvalue < 0.41109419)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.055512246;
            } else {
              sum += (float)0.31226128;
            }
          } else {
            if ( (data[14].fvalue < 0.10448601)) {
              sum += (float)-0.12462763;
            } else {
              sum += (float)0.012895637;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.24356762)) {
            if ( (data[17].fvalue < 0.13041413)) {
              sum += (float)-0.081446946;
            } else {
              sum += (float)0.022425886;
            }
          } else {
            if (  (data[4].fvalue < 1.8992943)) {
              sum += (float)0.079444543;
            } else {
              sum += (float)-0.014792267;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.11023705)) {
        if ( (data[14].fvalue < 0.13430823)) {
          sum += (float)0.25419101;
        } else {
          sum += (float)-0.0058092582;
        }
      } else {
        if ( (data[14].fvalue < 0.11135573)) {
          if ( (data[8].fvalue < 0.5)) {
            sum += (float)0.13456638;
          } else {
            if ( (data[4].fvalue < 1.0533838)) {
              sum += (float)0.098820366;
            } else {
              sum += (float)-0.07997603;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.40886533)) {
              sum += (float)-0;
            } else {
              sum += (float)0.23835993;
            }
          } else {
            if ( (data[6].fvalue < 0.28092343)) {
              sum += (float)0.13177782;
            } else {
              sum += (float)0.0083841933;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.4290524)) {
      if ( (data[13].fvalue < 0.54886019)) {
        if ( (data[16].fvalue < 0.1241099)) {
          if ( (data[13].fvalue < 0.23676437)) {
            sum += (float)-0.27790341;
          } else {
            sum += (float)-0.15285912;
          }
        } else {
          if ( (data[4].fvalue < 1.3477939)) {
            sum += (float)-0.14175156;
          } else {
            sum += (float)-0.041777775;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.00051934778)) {
          sum += (float)0.049229138;
        } else {
          sum += (float)-0.11943351;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.032609068)) {
        if ( (data[9].fvalue < 0.5)) {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)-0.25735623;
          } else {
            if ( (data[16].fvalue < 0.16119298)) {
              sum += (float)-0.11048243;
            } else {
              sum += (float)0.052928977;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.5437153)) {
            if ( (data[13].fvalue < 0.92510045)) {
              sum += (float)0.027324595;
            } else {
              sum += (float)-0.071197093;
            }
          } else {
            if ( (data[13].fvalue < 0.24471647)) {
              sum += (float)-0;
            } else {
              sum += (float)0.10053598;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.2984786)) {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.6647673)) {
              sum += (float)0.009754532;
            } else {
              sum += (float)-0.13804229;
            }
          } else {
            if ( (data[17].fvalue < 0.17020729)) {
              sum += (float)-0.10133489;
            } else {
              sum += (float)-0.0052317004;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.77279222)) {
            if ( (data[0].fvalue < 4.9150767)) {
              sum += (float)-0.0049866792;
            } else {
              sum += (float)-0.088387743;
            }
          } else {
            if ( (data[16].fvalue < 0.34248674)) {
              sum += (float)-0.017022351;
            } else {
              sum += (float)0.10335242;
            }
          }
        }
      }
    }
  }
  if ( (data[12].fvalue < 0.5)) {
    if ( (data[7].fvalue < 10.5)) {
      if ( (data[17].fvalue < 0.53030622)) {
        if ( (data[17].fvalue < 0.036910124)) {
          if ( (data[15].fvalue < 0.17386085)) {
            if ( (data[13].fvalue < 5.8632645e-07)) {
              sum += (float)0.09325327;
            } else {
              sum += (float)-0.0095741116;
            }
          } else {
            if ( (data[13].fvalue < 0.036381807)) {
              sum += (float)-0.015680639;
            } else {
              sum += (float)0.047217079;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.1422053)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.063714154;
            } else {
              sum += (float)-0.15763482;
            }
          } else {
            if (  (data[4].fvalue < 1.6768203)) {
              sum += (float)0.032515462;
            } else {
              sum += (float)-0.049626943;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0059953732)) {
          if ( (data[13].fvalue < 0.25515497)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.18248376;
            } else {
              sum += (float)0.051567648;
            }
          } else {
            if ( (data[5].fvalue < 0.42876449)) {
              sum += (float)0.080887266;
            } else {
              sum += (float)-0.046972428;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.056811258)) {
            if ( (data[13].fvalue < 0.031861223)) {
              sum += (float)0.022450302;
            } else {
              sum += (float)0.12260215;
            }
          } else {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)0.046857029;
            } else {
              sum += (float)-0.026839918;
            }
          }
        }
      }
    } else {
      sum += (float)0.16293266;
    }
  } else {
    if ( (data[4].fvalue < 1.7226522)) {
      if ( (data[13].fvalue < 0.53987968)) {
        if ( (data[6].fvalue < 0.44907224)) {
          sum += (float)0.11694961;
        } else {
          sum += (float)-0;
        }
      } else {
        sum += (float)-0.11664676;
      }
    } else {
      if (  (data[4].fvalue < 2.3377562)) {
        if (  (data[4].fvalue < 1.8401479)) {
          sum += (float)-0.099236436;
        } else {
          sum += (float)-0.26466334;
        }
      } else {
        sum += (float)-0.028498815;
      }
    }
  }
  if ( (data[5].fvalue < 1.1172254)) {
    if ( (data[5].fvalue < 0.60730231)) {
      if ( (data[13].fvalue < 0.16702187)) {
        if ( (data[5].fvalue < 0.076094486)) {
          if (  (data[4].fvalue < 1.2046155)) {
            if (  (data[4].fvalue < 1.1180205)) {
              sum += (float)0.038540345;
            } else {
              sum += (float)-0.059207808;
            }
          } else {
            if (  (data[4].fvalue < 1.7378222)) {
              sum += (float)0.070028134;
            } else {
              sum += (float)0.0035850983;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[8].fvalue < 6.5)) {
              sum += (float)-0.01914254;
            } else {
              sum += (float)0.11304352;
            }
          } else {
            if ( (data[14].fvalue < 0.23565492)) {
              sum += (float)0.10600147;
            } else {
              sum += (float)0.018720319;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 1.88676)) {
          if ( (data[15].fvalue < 0.12438933)) {
            if (  (data[4].fvalue < 1.1491737)) {
              sum += (float)0.093035474;
            } else {
              sum += (float)0.020303959;
            }
          } else {
            if (  (data[4].fvalue < 1.2721925)) {
              sum += (float)-0.020655734;
            } else {
              sum += (float)0.11995416;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.70681584)) {
            if ( (data[17].fvalue < 1.032584)) {
              sum += (float)-0.0032382021;
            } else {
              sum += (float)0.060323857;
            }
          } else {
            if ( (data[16].fvalue < 0.057354882)) {
              sum += (float)0.080111146;
            } else {
              sum += (float)0.012300171;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.013542728)) {
        if ( (data[14].fvalue < 0.52686691)) {
          if ( (data[14].fvalue < 4.8900932e-07)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.011743675;
            } else {
              sum += (float)0.47866806;
            }
          } else {
            if ( (data[14].fvalue < 0.019380409)) {
              sum += (float)-0.14766152;
            } else {
              sum += (float)-0.025828581;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.18214819;
          } else {
            if ( (data[13].fvalue < 0.29606766)) {
              sum += (float)-0.054920208;
            } else {
              sum += (float)0.080498166;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.12033853)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.010697405)) {
              sum += (float)-0.13467421;
            } else {
              sum += (float)-0.042862836;
            }
          } else {
            sum += (float)-0.28216305;
          }
        } else {
          if ( (data[15].fvalue < 0.84808457)) {
            if ( (data[14].fvalue < 0.0059286468)) {
              sum += (float)-0.058994859;
            } else {
              sum += (float)0.0064920415;
            }
          } else {
            if ( (data[13].fvalue < 0.57948852)) {
              sum += (float)-0.12545501;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 3.5)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[4].fvalue < 0.81405938)) {
          if ( (data[4].fvalue < 0.63533425)) {
            sum += (float)0.21855557;
          } else {
            if ( (data[13].fvalue < 0.18778467)) {
              sum += (float)0.14022109;
            } else {
              sum += (float)0.066153273;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.039965466)) {
            if ( (data[14].fvalue < 6.6465723e-06)) {
              sum += (float)0.19568732;
            } else {
              sum += (float)-0.015714563;
            }
          } else {
            if ( (data[3].fvalue < 0.23731235)) {
              sum += (float)-0.26460466;
            } else {
              sum += (float)0.02489656;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.33574155)) {
          sum += (float)0.095442705;
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.16588894)) {
              sum += (float)0.037736323;
            } else {
              sum += (float)-0.034675386;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.21650358;
            } else {
              sum += (float)-0.011544451;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.0601091)) {
        sum += (float)0.27498659;
      } else {
        sum += (float)-0.035013579;
      }
    }
  }
  if ( (data[15].fvalue < 0.035647426)) {
    if ( (data[13].fvalue < 6.9019086e-07)) {
      if ( (data[14].fvalue < 8.9874226e-07)) {
        if (  (data[4].fvalue < 1.5602541)) {
          sum += (float)0.042478088;
        } else {
          sum += (float)0.51308835;
        }
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.35405594;
            } else {
              sum += (float)0.1207009;
            }
          } else {
            if ( (data[14].fvalue < 0.26581341)) {
              sum += (float)0.11644231;
            } else {
              sum += (float)0.28083074;
            }
          }
        } else {
          sum += (float)0.39709187;
        }
      }
    } else {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[16].fvalue < 0.00082405389)) {
          if ( (data[13].fvalue < 0.0030031342)) {
            if (  (data[4].fvalue < 0.98211706)) {
              sum += (float)-0.053468242;
            } else {
              sum += (float)-0.24161246;
            }
          } else {
            if ( (data[17].fvalue < 0.17320696)) {
              sum += (float)-0.0023392914;
            } else {
              sum += (float)0.031950597;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.9520497)) {
              sum += (float)-0.32943231;
            } else {
              sum += (float)-0.154742;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.032884516;
            } else {
              sum += (float)-0.086286113;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.010783174)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0079675578)) {
              sum += (float)0.15627491;
            } else {
              sum += (float)0.29085028;
            }
          } else {
            sum += (float)0.12704317;
          }
        } else {
          if ( (data[5].fvalue < 0.35524225)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.1250136;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.017507348;
            } else {
              sum += (float)-0.066167809;
            }
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.12896238)) {
      if ( (data[5].fvalue < 0.2898466)) {
        if ( (data[15].fvalue < 0.057956863)) {
          if (  (data[4].fvalue < 1.2261778)) {
            sum += (float)-0.050798681;
          } else {
            if (  (data[4].fvalue < 1.3531923)) {
              sum += (float)0.11094159;
            } else {
              sum += (float)0.0036383616;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.058281109)) {
            if ( (data[13].fvalue < 0.08100605)) {
              sum += (float)-0.03777694;
            } else {
              sum += (float)0.077298313;
            }
          } else {
            if (  (data[4].fvalue < 2.1755698)) {
              sum += (float)-0.065800257;
            } else {
              sum += (float)0.0086511513;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.018553082)) {
            if ( (data[13].fvalue < 0.30700201)) {
              sum += (float)-0.14836955;
            } else {
              sum += (float)-0.069290727;
            }
          } else {
            if ( (data[13].fvalue < 0.1261096)) {
              sum += (float)-0.081602857;
            } else {
              sum += (float)0.0083741574;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.61918724)) {
            sum += (float)0.003229812;
          } else {
            if ( (data[5].fvalue < 0.65365446)) {
              sum += (float)-0.16769207;
            } else {
              sum += (float)-0.26634219;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.0094433483)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[3].fvalue < 0.19976723)) {
            if ( (data[13].fvalue < 0.052898265)) {
              sum += (float)-0.030810731;
            } else {
              sum += (float)0.089927733;
            }
          } else {
            if ( (data[13].fvalue < 0.16718543)) {
              sum += (float)-0.12047578;
            } else {
              sum += (float)-0.040144905;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.089707851)) {
              sum += (float)-0.03171749;
            } else {
              sum += (float)0.094977975;
            }
          } else {
            sum += (float)-0.12657177;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.008981497)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.20793705)) {
              sum += (float)-0.10974673;
            } else {
              sum += (float)-0.016983334;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.098332487;
            } else {
              sum += (float)-0.16376704;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.051778398)) {
              sum += (float)0.12095116;
            } else {
              sum += (float)0.019480798;
            }
          } else {
            if ( (data[6].fvalue < 0.35008168)) {
              sum += (float)0.095516697;
            } else {
              sum += (float)-0.006265854;
            }
          }
        }
      }
    }
  }
  if ( (data[3].fvalue < 0.10677437)) {
    if ( (data[17].fvalue < 0.13461939)) {
      if ( (data[15].fvalue < 0.10661761)) {
        if ( (data[7].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.0676082)) {
            sum += (float)-0.26105598;
          } else {
            sum += (float)-0.10205185;
          }
        } else {
          if ( (data[13].fvalue < 5.6968952e-07)) {
            sum += (float)0.35887358;
          } else {
            if ( (data[13].fvalue < 0.033503637)) {
              sum += (float)0.050302017;
            } else {
              sum += (float)-0.013189037;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.00721882)) {
          if ( (data[13].fvalue < 0.071900964)) {
            if ( (data[15].fvalue < 0.74902356)) {
              sum += (float)-0.018805858;
            } else {
              sum += (float)0.063118808;
            }
          } else {
            if ( (data[3].fvalue < 0.061369818)) {
              sum += (float)0.14309223;
            } else {
              sum += (float)0.065915279;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.643389)) {
            if ( (data[13].fvalue < 0.013231939)) {
              sum += (float)0.099074073;
            } else {
              sum += (float)0.18427227;
            }
          } else {
            sum += (float)0.041295104;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.042377062)) {
        sum += (float)-0;
      } else {
        if (  (data[4].fvalue < 1.6333814)) {
          sum += (float)0.21698122;
        } else {
          sum += (float)0.090164192;
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.034743983)) {
      if ( (data[13].fvalue < 0.454842)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[15].fvalue < 1.7248716)) {
            if (  (data[4].fvalue < 1.8468887)) {
              sum += (float)0.0038048988;
            } else {
              sum += (float)0.075937569;
            }
          } else {
            if ( (data[14].fvalue < 0.56288719)) {
              sum += (float)-0.13415137;
            } else {
              sum += (float)0.12633896;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.049943462)) {
            if ( (data[3].fvalue < 0.24326484)) {
              sum += (float)-0.0039299205;
            } else {
              sum += (float)-0.20663209;
            }
          } else {
            if ( (data[15].fvalue < 0.19296396)) {
              sum += (float)-0.023286099;
            } else {
              sum += (float)0.039740898;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.43432906)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[15].fvalue < 1.0172421)) {
              sum += (float)0.014892628;
            } else {
              sum += (float)0.12295619;
            }
          } else {
            if ( (data[14].fvalue < 0.17182034)) {
              sum += (float)-0.15972222;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.05108957)) {
            if ( (data[3].fvalue < 2.1439381)) {
              sum += (float)0.17971265;
            } else {
              sum += (float)0.065457538;
            }
          } else {
            if ( (data[15].fvalue < 0.21004994)) {
              sum += (float)-0.0031508491;
            } else {
              sum += (float)0.069783658;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.18186054)) {
        if ( (data[13].fvalue < 0.75990534)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.032088965)) {
              sum += (float)-0.075733736;
            } else {
              sum += (float)-0.0063877776;
            }
          } else {
            if ( (data[5].fvalue < 0.56823587)) {
              sum += (float)-0.082155257;
            } else {
              sum += (float)-0.21952832;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)-0.18322596;
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.011971571;
            } else {
              sum += (float)-0.090535305;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0059471084)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.11496589)) {
              sum += (float)-0.13854301;
            } else {
              sum += (float)-0.03691835;
            }
          } else {
            if ( (data[15].fvalue < 0.037437804)) {
              sum += (float)0.096950836;
            } else {
              sum += (float)-0.022464545;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0055673318)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.18712682;
            } else {
              sum += (float)-0.0026941306;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.085467175;
            } else {
              sum += (float)0.0016796406;
            }
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 1.6495583)) {
    if ( (data[14].fvalue < 0.037281226)) {
      if ( (data[14].fvalue < 5.8504845e-08)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.022880781)) {
            if ( (data[3].fvalue < 0.97655332)) {
              sum += (float)0.048113044;
            } else {
              sum += (float)0.40048996;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.022882659;
            } else {
              sum += (float)0.033758707;
            }
          }
        } else {
          sum += (float)0.43927303;
        }
      } else {
        if ( (data[14].fvalue < 0.0045798598)) {
          if ( (data[14].fvalue < 0.0018408641)) {
            sum += (float)-0.23020174;
          } else {
            if ( (data[15].fvalue < 0.10452928)) {
              sum += (float)-0.15169309;
            } else {
              sum += (float)-0.040189993;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.040155817)) {
              sum += (float)-0.11451985;
            } else {
              sum += (float)0.11397852;
            }
          } else {
            if ( (data[3].fvalue < 0.28444949)) {
              sum += (float)0.004724626;
            } else {
              sum += (float)-0.13730466;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.024564918)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.0380372;
            } else {
              sum += (float)0.086192988;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.070006147;
            } else {
              sum += (float)-0.075457491;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.29692706)) {
            sum += (float)-0.056658778;
          } else {
            sum += (float)-0.2495129;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 5.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.051073547;
            } else {
              sum += (float)-0.010241722;
            }
          } else {
            sum += (float)0.19564186;
          }
        } else {
          if ( (data[6].fvalue < 0.12576321)) {
            if ( (data[0].fvalue < 2.9017205)) {
              sum += (float)0.26505193;
            } else {
              sum += (float)0.023087492;
            }
          } else {
            if ( (data[0].fvalue < 1.4667078)) {
              sum += (float)0.062386449;
            } else {
              sum += (float)-0.011853629;
            }
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.37057924)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[7].fvalue < 0.5)) {
          sum += (float)-0.20027341;
        } else {
          sum += (float)-0.077462658;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          sum += (float)0.0061891656;
        } else {
          sum += (float)-0.081810676;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.058593102)) {
        if ( (data[4].fvalue < 2.0019884)) {
          sum += (float)0.0029627327;
        } else {
          sum += (float)0.072007187;
        }
      } else {
        sum += (float)-0.051036816;
      }
    }
  }
  if ( (data[8].fvalue < 8.5)) {
    if ( (data[4].fvalue < 0.8374629)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.29482722)) {
            if ( (data[3].fvalue < 0.76479876)) {
              sum += (float)0.058478311;
            } else {
              sum += (float)0.18240352;
            }
          } else {
            if ( (data[3].fvalue < 1.0573294)) {
              sum += (float)-0.039873503;
            } else {
              sum += (float)0.066481903;
            }
          }
        } else {
          sum += (float)-0.074809499;
        }
      } else {
        if ( (data[3].fvalue < 0.71178555)) {
          sum += (float)0.096637286;
        } else {
          if ( (data[14].fvalue < 0.12612183)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.010292752;
            } else {
              sum += (float)-0.18714522;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.60455)) {
        if ( (data[14].fvalue < 1.0409514)) {
          if ( (data[14].fvalue < 0.31977373)) {
            if ( (data[13].fvalue < 1.1502748)) {
              sum += (float)0.0022743968;
            } else {
              sum += (float)-0.041331515;
            }
          } else {
            if ( (data[13].fvalue < 0.27286381)) {
              sum += (float)0.013264782;
            } else {
              sum += (float)0.060520075;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.78263181)) {
            if ( (data[13].fvalue < 0.54021978)) {
              sum += (float)-0.078301653;
            } else {
              sum += (float)0.14335382;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.17757344;
            } else {
              sum += (float)-0.0035716919;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 1.8670219)) {
          if ( (data[14].fvalue < 0.46596962)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.11176466;
            } else {
              sum += (float)-0.0067113638;
            }
          } else {
            if (  (data[4].fvalue < 1.1816185)) {
              sum += (float)0.098996371;
            } else {
              sum += (float)0.0094706891;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.2334945)) {
            if ( (data[14].fvalue < 0.61373544)) {
              sum += (float)-0.084526256;
            } else {
              sum += (float)-0.011239542;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.028660418;
            } else {
              sum += (float)0.047816683;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.40815523)) {
      if ( (data[3].fvalue < 1.1133385)) {
        sum += (float)0.20366812;
      } else {
        sum += (float)0.062960945;
      }
    } else {
      sum += (float)-0.01240757;
    }
  }
  if ( (data[12].fvalue < 0.5)) {
    if ( (data[16].fvalue < 0.0092342217)) {
      if (  (data[4].fvalue < 2.0350289)) {
        if ( (data[15].fvalue < 1.5279436)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[13].fvalue < 0.37841886)) {
              sum += (float)-0.0044880551;
            } else {
              sum += (float)0.014012587;
            }
          } else {
            if ( (data[6].fvalue < 0.26270342)) {
              sum += (float)0.11123396;
            } else {
              sum += (float)0.010187782;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.54072118)) {
            if ( (data[14].fvalue < 0.076516509)) {
              sum += (float)-0.17229737;
            } else {
              sum += (float)-0.082957208;
            }
          } else {
            sum += (float)0.0070705046;
          }
        }
      } else {
        if ( (data[13].fvalue < 1.2412034)) {
          if ( (data[14].fvalue < 7.7178076e-05)) {
            if ( (data[17].fvalue < 1.8718557)) {
              sum += (float)0.16829723;
            } else {
              sum += (float)-0.034345865;
            }
          } else {
            if ( (data[15].fvalue < 2.802865)) {
              sum += (float)0.031842489;
            } else {
              sum += (float)-0.085755937;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.026960291)) {
            if ( (data[17].fvalue < 0.56267124)) {
              sum += (float)-0;
            } else {
              sum += (float)0.13395742;
            }
          } else {
            if ( (data[15].fvalue < 0.53115869)) {
              sum += (float)-0.19697164;
            } else {
              sum += (float)0.0048462357;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.12494614)) {
        if ( (data[15].fvalue < 0.00016526016)) {
          if ( (data[13].fvalue < 0.59432411)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.09988074;
            } else {
              sum += (float)-0.20490761;
            }
          } else {
            if ( (data[14].fvalue < 0.10695384)) {
              sum += (float)-0.061504982;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.51420653)) {
            if ( (data[13].fvalue < 0.12400706)) {
              sum += (float)-0.069657587;
            } else {
              sum += (float)0.018634781;
            }
          } else {
            if ( (data[15].fvalue < 0.99995875)) {
              sum += (float)-0.075992905;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.01591618)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[4].fvalue < 2.2239954)) {
              sum += (float)-0.07599169;
            } else {
              sum += (float)0.015698101;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.037550032;
            } else {
              sum += (float)-0.085341051;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.67457634)) {
              sum += (float)0.025860343;
            } else {
              sum += (float)0.094233587;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.14060754;
            } else {
              sum += (float)0.040079486;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.7773588)) {
      if ( (data[3].fvalue < 0.88414168)) {
        if ( (data[2].fvalue < 0.25997615)) {
          sum += (float)0.12376942;
        } else {
          sum += (float)0.030771879;
        }
      } else {
        sum += (float)-0.087647438;
      }
    } else {
      if (  (data[4].fvalue < 2.4423053)) {
        if ( (data[12].fvalue < 1.5)) {
          sum += (float)-0.23570876;
        } else {
          sum += (float)-0.12841013;
        }
      } else {
        sum += (float)-0.0015449525;
      }
    }
  }
  if ( (data[7].fvalue < 10.5)) {
    if ( (data[5].fvalue < 1.8725199)) {
      if ( (data[5].fvalue < 0.02601959)) {
        if (  (data[4].fvalue < 1.8526545)) {
          if ( (data[11].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.0025619664;
            } else {
              sum += (float)0.072244741;
            }
          } else {
            sum += (float)0.1526486;
          }
        } else {
          sum += (float)-0.014470419;
        }
      } else {
        if ( (data[15].fvalue < 0.25082618)) {
          if ( (data[15].fvalue < 0.046334684)) {
            if ( (data[13].fvalue < 6.9019086e-07)) {
              sum += (float)0.072614461;
            } else {
              sum += (float)-0.0020315524;
            }
          } else {
            if ( (data[3].fvalue < 0.3827225)) {
              sum += (float)0.0028557894;
            } else {
              sum += (float)-0.047617782;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.51636541)) {
            if (  (data[4].fvalue < 1.7510138)) {
              sum += (float)0.058849681;
            } else {
              sum += (float)-0.012494893;
            }
          } else {
            if ( (data[14].fvalue < 0.40682244)) {
              sum += (float)-0.011202288;
            } else {
              sum += (float)0.020705253;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.29996663)) {
        sum += (float)0.00048567145;
      } else {
        sum += (float)0.1280324;
      }
    }
  } else {
    sum += (float)0.13439874;
  }
  if ( (data[13].fvalue < 0.23002136)) {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[5].fvalue < 0.9814266)) {
        if (  (data[4].fvalue < 1.7900144)) {
          if ( (data[15].fvalue < 1.0486088)) {
            if ( (data[15].fvalue < 0.15125899)) {
              sum += (float)-0.022069154;
            } else {
              sum += (float)0.0065619429;
            }
          } else {
            if (  (data[4].fvalue < 1.7117027)) {
              sum += (float)-0.14388368;
            } else {
              sum += (float)-0.0076222029;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.20749657;
            } else {
              sum += (float)0.0098849218;
            }
          } else {
            if ( (data[14].fvalue < 0.0015938809)) {
              sum += (float)-0.093176737;
            } else {
              sum += (float)0.0012663355;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.15468396;
          } else {
            if ( (data[13].fvalue < 0.00089059101)) {
              sum += (float)0.24399211;
            } else {
              sum += (float)0.0088418005;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.92881542)) {
            if ( (data[3].fvalue < 0.49595344)) {
              sum += (float)0.28014687;
            } else {
              sum += (float)0.073595442;
            }
          } else {
            sum += (float)-0.13456452;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.17845708)) {
        if ( (data[15].fvalue < 0.019244455)) {
          sum += (float)0.092084564;
        } else {
          sum += (float)0.22499187;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          sum += (float)0.11109596;
        } else {
          if ( (data[14].fvalue < 0.36166769)) {
            if ( (data[15].fvalue < 0.03625397)) {
              sum += (float)-0.0068838568;
            } else {
              sum += (float)0.11876561;
            }
          } else {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.012572552;
            } else {
              sum += (float)-0.13414446;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 0.71746182)) {
      if (  (data[4].fvalue < 1.7025889)) {
        if ( (data[14].fvalue < 0.16821864)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.036533818;
            } else {
              sum += (float)-0.2365178;
            }
          } else {
            if (  (data[4].fvalue < 1.0962675)) {
              sum += (float)0.21187054;
            } else {
              sum += (float)-0.05216518;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.55189371)) {
            if (  (data[4].fvalue < 1.3263006)) {
              sum += (float)0.087527171;
            } else {
              sum += (float)0.014484928;
            }
          } else {
            if ( (data[3].fvalue < 1.51445)) {
              sum += (float)0.13266487;
            } else {
              sum += (float)0.029864952;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.89718604)) {
          if ( (data[16].fvalue < 0.6147055)) {
            if ( (data[15].fvalue < 0.73485446)) {
              sum += (float)-0.049880251;
            } else {
              sum += (float)0.042224262;
            }
          } else {
            if ( (data[13].fvalue < 0.67614192)) {
              sum += (float)0.03915751;
            } else {
              sum += (float)0.12180534;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.51215625)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.029022845;
            } else {
              sum += (float)0.068895504;
            }
          } else {
            if ( (data[15].fvalue < 0.0075555388)) {
              sum += (float)0.15836918;
            } else {
              sum += (float)0.027519768;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.42719457)) {
        sum += (float)-0.20453858;
      } else {
        if ( (data[6].fvalue < 0.082135752)) {
          sum += (float)-0.097518221;
        } else {
          if ( (data[13].fvalue < 0.29851428)) {
            if (  (data[4].fvalue < 1.0414099)) {
              sum += (float)0.042564366;
            } else {
              sum += (float)-0.045894202;
            }
          } else {
            if ( (data[3].fvalue < 0.7333352)) {
              sum += (float)-0.06949313;
            } else {
              sum += (float)-0.0057845693;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.011815616)) {
    sum += (float)-0.14030094;
  } else {
    if ( (data[3].fvalue < 0.095278218)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[9].fvalue < 0.5)) {
          sum += (float)-0.23123062;
        } else {
          if ( (data[14].fvalue < 0.075065836)) {
            if ( (data[3].fvalue < 0.031373702)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.065463014;
            }
          } else {
            sum += (float)0.089363888;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.038101055)) {
          if (  (data[4].fvalue < 1.0011518)) {
            if ( (data[14].fvalue < 7.2014627e-06)) {
              sum += (float)0.22740199;
            } else {
              sum += (float)0.021194296;
            }
          } else {
            if ( (data[14].fvalue < 0.078246057)) {
              sum += (float)-0.061339822;
            } else {
              sum += (float)0.054527156;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.98084486)) {
            sum += (float)-0.24570315;
          } else {
            if (  (data[4].fvalue < 1.0335793)) {
              sum += (float)-0.04101155;
            } else {
              sum += (float)0.039639305;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[7].fvalue < 5.5)) {
          if ( (data[14].fvalue < 0.84313345)) {
            if ( (data[14].fvalue < 0.013383096)) {
              sum += (float)-0.0036868302;
            } else {
              sum += (float)0.016492911;
            }
          } else {
            if ( (data[15].fvalue < 0.42989761)) {
              sum += (float)-0.10063137;
            } else {
              sum += (float)0.03279499;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.55313843)) {
            sum += (float)0.01577522;
          } else {
            sum += (float)0.18845198;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.083185323)) {
          if ( (data[3].fvalue < 0.30524468)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.057702459;
            } else {
              sum += (float)0.06694404;
            }
          } else {
            if ( (data[17].fvalue < 0.1878854)) {
              sum += (float)-0.1337304;
            } else {
              sum += (float)-0.022088844;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.48284304)) {
              sum += (float)0.020220701;
            } else {
              sum += (float)0.10799527;
            }
          } else {
            if ( (data[13].fvalue < 0.066640556)) {
              sum += (float)-0.070864633;
            } else {
              sum += (float)0.0034140977;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[13].fvalue < 0.0092907958)) {
          if ( (data[14].fvalue < 8.3113275e-07)) {
            if ( (data[15].fvalue < 0.004505198)) {
              sum += (float)0.17657079;
            } else {
              sum += (float)-0.049196426;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.12563637;
            } else {
              sum += (float)-0.013961722;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.011995384;
            } else {
              sum += (float)0.024794437;
            }
          } else {
            if ( (data[14].fvalue < 1.0948416e-07)) {
              sum += (float)0.38864338;
            } else {
              sum += (float)0.022640094;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[15].fvalue < 0.020183444)) {
            if ( (data[14].fvalue < 0.31944913)) {
              sum += (float)0.092278957;
            } else {
              sum += (float)0.19852383;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.011102794;
            } else {
              sum += (float)0.1040308;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[6].fvalue < 0.074790522)) {
              sum += (float)0.12809435;
            } else {
              sum += (float)-0.087890148;
            }
          } else {
            if ( (data[14].fvalue < 0.29112017)) {
              sum += (float)-0.04327292;
            } else {
              sum += (float)0.068827577;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1094429)) {
        if ( (data[3].fvalue < 0.36778033)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.10812177)) {
              sum += (float)-0.018958429;
            } else {
              sum += (float)0.21881475;
            }
          } else {
            sum += (float)0.30285159;
          }
        } else {
          if ( (data[3].fvalue < 1.3206105)) {
            if ( (data[14].fvalue < 0.089885592)) {
              sum += (float)-0.063006677;
            } else {
              sum += (float)0.095126435;
            }
          } else {
            if ( (data[13].fvalue < 0.56002176)) {
              sum += (float)-0.18875523;
            } else {
              sum += (float)-0.051114403;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.37536025)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[15].fvalue < 0.0013179998)) {
              sum += (float)-0.11634316;
            } else {
              sum += (float)-0.042013485;
            }
          } else {
            if ( (data[15].fvalue < 0.090477139)) {
              sum += (float)-0.014329968;
            } else {
              sum += (float)0.041800916;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.11737448)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.022444747;
            } else {
              sum += (float)-0.10941561;
            }
          } else {
            if ( (data[15].fvalue < 0.0049381685)) {
              sum += (float)0.069814414;
            } else {
              sum += (float)-0.011997903;
            }
          }
        }
      }
    }
  } else {
    if ( (data[10].fvalue < 1.5)) {
      if ( (data[13].fvalue < 0.70410442)) {
        if (  (data[4].fvalue < 1.1562897)) {
          if ( (data[3].fvalue < 0.99450475)) {
            if ( (data[3].fvalue < 0.47801155)) {
              sum += (float)0.24243717;
            } else {
              sum += (float)0.10933684;
            }
          } else {
            if ( (data[3].fvalue < 1.3886683)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.14440614;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.095181376)) {
            if ( (data[17].fvalue < 0.17912367)) {
              sum += (float)-0.060357694;
            } else {
              sum += (float)0.046833895;
            }
          } else {
            if ( (data[14].fvalue < 0.3166247)) {
              sum += (float)0.11627392;
            } else {
              sum += (float)-0.010183646;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[14].fvalue < 0.077083305)) {
            if ( (data[14].fvalue < 0.0013052688)) {
              sum += (float)0.15035547;
            } else {
              sum += (float)-0.013990679;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.21503972;
            } else {
              sum += (float)0.11599536;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.60495394)) {
            if (  (data[4].fvalue < 1.7669898)) {
              sum += (float)-0.0126164;
            } else {
              sum += (float)-0.16514942;
            }
          } else {
            sum += (float)0.0633744;
          }
        }
      }
    } else {
      sum += (float)-0.15942472;
    }
  }
  if ( (data[13].fvalue < 0.30359894)) {
    if ( (data[8].fvalue < 9.5)) {
      if ( (data[17].fvalue < 2.6414928)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[6].fvalue < 0.012112184)) {
              sum += (float)-0.17823833;
            } else {
              sum += (float)-0.00018135134;
            }
          } else {
            if ( (data[3].fvalue < 0.41864926)) {
              sum += (float)0.0034984627;
            } else {
              sum += (float)-0.061145332;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.084271267)) {
            sum += (float)0.18684226;
          } else {
            if ( (data[0].fvalue < 2.1549845)) {
              sum += (float)0.053265948;
            } else {
              sum += (float)-0.0059626284;
            }
          }
        }
      } else {
        sum += (float)-0.13152903;
      }
    } else {
      sum += (float)0.18454255;
    }
  } else {
    if ( (data[16].fvalue < 0.86103022)) {
      if ( (data[16].fvalue < 0.028221879)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[14].fvalue < 0.57630205)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.014687491;
            } else {
              sum += (float)-0.023091147;
            }
          } else {
            if ( (data[13].fvalue < 0.54829347)) {
              sum += (float)0.023832001;
            } else {
              sum += (float)0.12053475;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8150054)) {
            if ( (data[3].fvalue < 1.1099316)) {
              sum += (float)0.0056722336;
            } else {
              sum += (float)-0.10469186;
            }
          } else {
            sum += (float)-0.15277581;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.0014059527)) {
            sum += (float)-0.2457812;
          } else {
            if ( (data[13].fvalue < 0.85861766)) {
              sum += (float)-0.0055992529;
            } else {
              sum += (float)-0.099827424;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if (  (data[4].fvalue < 1.8244734)) {
              sum += (float)0.049870618;
            } else {
              sum += (float)-0.002468735;
            }
          } else {
            if ( (data[14].fvalue < 0.13373293)) {
              sum += (float)-0.17847753;
            } else {
              sum += (float)-0.011141063;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.36669439)) {
        if ( (data[13].fvalue < 0.67206156)) {
          if ( (data[3].fvalue < 1.7508348)) {
            sum += (float)0.0892708;
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[16].fvalue < 1.3867495)) {
            sum += (float)0.1039876;
          } else {
            sum += (float)0.21466616;
          }
        }
      } else {
        sum += (float)-0.012242374;
      }
    }
  }
  if ( (data[15].fvalue < 3.5657978)) {
    if ( (data[4].fvalue < 0.58859277)) {
      sum += (float)0.1097046;
    } else {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.0041451277;
            } else {
              sum += (float)-0.043730736;
            }
          } else {
            if ( (data[14].fvalue < 1.0194894e-07)) {
              sum += (float)0.35514411;
            } else {
              sum += (float)0.011131259;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.00015112598;
            } else {
              sum += (float)0.070373535;
            }
          } else {
            if ( (data[13].fvalue < 0.039156973)) {
              sum += (float)-0.1233281;
            } else {
              sum += (float)-0.017705375;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.0637943)) {
          sum += (float)0.25685769;
        } else {
          if ( (data[14].fvalue < 0.053228363)) {
            if ( (data[13].fvalue < 0.128682)) {
              sum += (float)0.1247287;
            } else {
              sum += (float)-0.059921362;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.045971125;
            } else {
              sum += (float)-0.0074796542;
            }
          }
        }
      }
    }
  } else {
    sum += (float)-0.13641156;
  }
  if ( (data[13].fvalue < 0.011708329)) {
    if ( (data[13].fvalue < 1.4610923e-06)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[6].fvalue < 0.074738026)) {
          sum += (float)-0.18771513;
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[3].fvalue < 0.65280187)) {
              sum += (float)-0.0090823136;
            } else {
              sum += (float)0.26886365;
            }
          } else {
            if ( (data[15].fvalue < 9.7895434e-05)) {
              sum += (float)-0.13957071;
            } else {
              sum += (float)-0.010182547;
            }
          }
        }
      } else {
        sum += (float)0.35040581;
      }
    } else {
      if ( (data[13].fvalue < 0.0012720437)) {
        sum += (float)-0.24622338;
      } else {
        if ( (data[6].fvalue < 0.022873562)) {
          sum += (float)0.094737284;
        } else {
          if ( (data[14].fvalue < 0.15106231)) {
            if ( (data[3].fvalue < 0.062537864)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.12081233;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.018935243;
            } else {
              sum += (float)-0.10884184;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.014955322)) {
      if ( (data[14].fvalue < 5.8504845e-08)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.0034773841)) {
              sum += (float)-4.5909877e-05;
            } else {
              sum += (float)-0.03186053;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.056291513;
            }
          }
        } else {
          sum += (float)0.3293801;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.1199659)) {
            if ( (data[3].fvalue < 0.32136312)) {
              sum += (float)-0.14258599;
            } else {
              sum += (float)-0.0030762146;
            }
          } else {
            if ( (data[14].fvalue < 0.0031520277)) {
              sum += (float)-0.03772977;
            } else {
              sum += (float)0.11334007;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.39602324)) {
            if (  (data[4].fvalue < 1.1347187)) {
              sum += (float)0.0062577748;
            } else {
              sum += (float)-0.099925496;
            }
          } else {
            if ( (data[4].fvalue < 1.4907877)) {
              sum += (float)-0.20290682;
            } else {
              sum += (float)-0.092711113;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.062712982)) {
          if ( (data[13].fvalue < 0.11519948)) {
            if (  (data[4].fvalue < 1.0716851)) {
              sum += (float)-0.081596807;
            } else {
              sum += (float)0.04654881;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.075903051;
            } else {
              sum += (float)0.17670968;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.46473324)) {
            if ( (data[15].fvalue < 0.00014959852)) {
              sum += (float)-0.017125865;
            } else {
              sum += (float)0.02382125;
            }
          } else {
            if ( (data[15].fvalue < 0.056995757)) {
              sum += (float)0.1171193;
            } else {
              sum += (float)0.014655732;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.053643864)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.06259916;
          } else {
            sum += (float)0.29308298;
          }
        } else {
          if ( (data[13].fvalue < 0.072908238)) {
            if ( (data[3].fvalue < 0.21165058)) {
              sum += (float)0.0085719377;
            } else {
              sum += (float)-0.094527371;
            }
          } else {
            if ( (data[6].fvalue < 0.11704345)) {
              sum += (float)0.072752945;
            } else {
              sum += (float)-0.0046379627;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.5593363)) {
    if ( (data[0].fvalue < 1.7982597)) {
      if ( (data[13].fvalue < 0.30589753)) {
        if ( (data[3].fvalue < 0.358998)) {
          if ( (data[15].fvalue < 0.12382218)) {
            if (  (data[4].fvalue < 1.1351762)) {
              sum += (float)0.037745621;
            } else {
              sum += (float)-0.018535651;
            }
          } else {
            if ( (data[14].fvalue < 0.014429778)) {
              sum += (float)0.024188638;
            } else {
              sum += (float)0.11401518;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.27411503)) {
            if (  (data[4].fvalue < 1.5360379)) {
              sum += (float)-0.06112295;
            } else {
              sum += (float)0.021937458;
            }
          } else {
            if (  (data[4].fvalue < 1.4040432)) {
              sum += (float)0.073930964;
            } else {
              sum += (float)-0.025782188;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[3].fvalue < 0.59314436)) {
            sum += (float)0.11536492;
          } else {
            if ( (data[13].fvalue < 0.48711401)) {
              sum += (float)-0;
            } else {
              sum += (float)0.068896979;
            }
          }
        } else {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[15].fvalue < 0.00086163031)) {
              sum += (float)-0.00084683544;
            } else {
              sum += (float)0.073131613;
            }
          } else {
            if ( (data[3].fvalue < 0.4748933)) {
              sum += (float)0.13230933;
            } else {
              sum += (float)0.01647866;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.37639371)) {
        if ( (data[14].fvalue < 0.79652894)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 2.1681328)) {
              sum += (float)-0.0024141048;
            } else {
              sum += (float)-0.13512456;
            }
          } else {
            if ( (data[13].fvalue < 0.76789784)) {
              sum += (float)0.060171645;
            } else {
              sum += (float)-0.039326023;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.58194423)) {
              sum += (float)-0.11952709;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.38674122)) {
              sum += (float)-0.01551239;
            } else {
              sum += (float)0.088105097;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 1.6388793)) {
          if ( (data[13].fvalue < 0.32794505)) {
            if ( (data[14].fvalue < 0.020957358)) {
              sum += (float)-0.041757647;
            } else {
              sum += (float)0.02195002;
            }
          } else {
            if ( (data[14].fvalue < 0.062330782)) {
              sum += (float)0.059601795;
            } else {
              sum += (float)-0.0066329013;
            }
          }
        } else {
          sum += (float)-0.10863557;
        }
      }
    }
  } else {
    if ( (data[11].fvalue < 1.5)) {
      if ( (data[15].fvalue < 0.018731739)) {
        if ( (data[17].fvalue < 0.7604804)) {
          if ( (data[14].fvalue < 0.62979496)) {
            if ( (data[14].fvalue < 0.00010301608)) {
              sum += (float)0.009749433;
            } else {
              sum += (float)-0.019990033;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.1506846;
            } else {
              sum += (float)0.016776361;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.62942111)) {
            sum += (float)-0.1546969;
          } else {
            sum += (float)-0.026506912;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.10860267)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[1].fvalue < 1.1207283)) {
              sum += (float)-0.07321988;
            } else {
              sum += (float)-0.011095805;
            }
          } else {
            if ( (data[13].fvalue < 0.59535134)) {
              sum += (float)-0.16770037;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1587615)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.10333528;
            } else {
              sum += (float)0.0018889054;
            }
          } else {
            if ( (data[3].fvalue < 1.3603663)) {
              sum += (float)0.0051934714;
            } else {
              sum += (float)-0.034390543;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.54367113)) {
        sum += (float)-0.21870367;
      } else {
        sum += (float)-0.02256351;
      }
    }
  }
  if ( (data[17].fvalue < 0.25941253)) {
    if ( (data[17].fvalue < 0.051165551)) {
      if ( (data[14].fvalue < 0.32897621)) {
        if ( (data[11].fvalue < 0.5)) {
          if ( (data[15].fvalue < 1.9409255)) {
            if ( (data[7].fvalue < 8.5)) {
              sum += (float)-0.0024773246;
            } else {
              sum += (float)0.13894907;
            }
          } else {
            if ( (data[13].fvalue < 0.062124211)) {
              sum += (float)-0.15436685;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.010626702)) {
            sum += (float)0.19699676;
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.050070543;
            } else {
              sum += (float)-0.098411441;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.3701544)) {
          if ( (data[3].fvalue < 0.9906534)) {
            if ( (data[13].fvalue < 0.26546755)) {
              sum += (float)0.073116727;
            } else {
              sum += (float)0.20100652;
            }
          } else {
            if ( (data[14].fvalue < 0.74697357)) {
              sum += (float)-0.0080893654;
            } else {
              sum += (float)0.070466973;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.32384357)) {
            if ( (data[13].fvalue < 0.32827407)) {
              sum += (float)-0.05238042;
            } else {
              sum += (float)0.046714213;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.047870379;
            } else {
              sum += (float)-0.013173839;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.69554317)) {
        if ( (data[13].fvalue < 0.0044204276)) {
          if ( (data[15].fvalue < 1.3012264)) {
            if ( (data[14].fvalue < 0.19038239)) {
              sum += (float)-0.010986422;
            } else {
              sum += (float)-0.16456796;
            }
          } else {
            sum += (float)0.044129912;
          }
        } else {
          if ( (data[17].fvalue < 0.1168322)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.015645692;
            } else {
              sum += (float)-0.071958348;
            }
          } else {
            if (  (data[4].fvalue < 1.3599079)) {
              sum += (float)0.077998415;
            } else {
              sum += (float)0.00030752696;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.0025484799)) {
            sum += (float)-0.17315792;
          } else {
            sum += (float)-0.037152737;
          }
        } else {
          if (  (data[4].fvalue < 1.8208969)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.038374245;
            } else {
              sum += (float)-0.047935866;
            }
          } else {
            if ( (data[16].fvalue < 0.011347784)) {
              sum += (float)-0.12497879;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 0.46697414)) {
      if (  (data[4].fvalue < 1.7190807)) {
        if ( (data[13].fvalue < 0.021536123)) {
          if ( (data[14].fvalue < 0.054028496)) {
            sum += (float)-0.17908524;
          } else {
            if ( (data[17].fvalue < 0.47743163)) {
              sum += (float)-0;
            } else {
              sum += (float)0.064370371;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.63122272)) {
            if ( (data[13].fvalue < 0.13985702)) {
              sum += (float)0.058536585;
            } else {
              sum += (float)0.14099213;
            }
          } else {
            if ( (data[14].fvalue < 0.037035204)) {
              sum += (float)-0.010695498;
            } else {
              sum += (float)0.07910531;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.67222571)) {
          if ( (data[14].fvalue < 0.91604996)) {
            if ( (data[15].fvalue < 0.40542674)) {
              sum += (float)-0.020714872;
            } else {
              sum += (float)0.027225479;
            }
          } else {
            if (  (data[4].fvalue < 2.2871809)) {
              sum += (float)-0.14418015;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.013603093)) {
            if ( (data[17].fvalue < 2.7104597)) {
              sum += (float)0.051929351;
            } else {
              sum += (float)-0.043151952;
            }
          } else {
            if ( (data[13].fvalue < 0.56293058)) {
              sum += (float)-0.066388533;
            } else {
              sum += (float)0.017438063;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.61096621)) {
        if ( (data[14].fvalue < 0.13917685)) {
          if ( (data[17].fvalue < 0.8381952)) {
            if ( (data[13].fvalue < 0.19703779)) {
              sum += (float)-0.10132497;
            } else {
              sum += (float)-0.0054769916;
            }
          } else {
            if ( (data[13].fvalue < 0.38883519)) {
              sum += (float)-0.18321674;
            } else {
              sum += (float)-0.053855881;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.95100915)) {
            if ( (data[3].fvalue < 1.7956915)) {
              sum += (float)0.021636995;
            } else {
              sum += (float)-0.062688492;
            }
          } else {
            sum += (float)0.07407745;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.50637174)) {
          sum += (float)0.055868398;
        } else {
          if ( (data[3].fvalue < 3.5212255)) {
            if ( (data[13].fvalue < 1.0921671)) {
              sum += (float)-0.0052789939;
            } else {
              sum += (float)0.057556823;
            }
          } else {
            sum += (float)-0.033091202;
          }
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.42264798)) {
    if ( (data[16].fvalue < 0.025099352)) {
      if ( (data[4].fvalue < 0.74838543)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.38824177)) {
            sum += (float)0.12258675;
          } else {
            if ( (data[3].fvalue < 1.9138769)) {
              sum += (float)0.0039888076;
            } else {
              sum += (float)0.069279522;
            }
          }
        } else {
          sum += (float)-0.011058679;
        }
      } else {
        if ( (data[17].fvalue < 0.44534296)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[7].fvalue < 9.5)) {
              sum += (float)-0.0022143486;
            } else {
              sum += (float)0.12015795;
            }
          } else {
            if ( (data[5].fvalue < 0.19963878)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.10229492;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0054801055)) {
            if ( (data[13].fvalue < 0.093713254)) {
              sum += (float)-0.17405128;
            } else {
              sum += (float)0.012427104;
            }
          } else {
            if (  (data[4].fvalue < 2.0869641)) {
              sum += (float)0.049994942;
            } else {
              sum += (float)0.00052478199;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.10024041)) {
        if ( (data[4].fvalue < 1.3913764)) {
          if ( (data[13].fvalue < 0.29093403)) {
            sum += (float)-0.19802728;
          } else {
            sum += (float)-0.07438314;
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.064955913;
            } else {
              sum += (float)0.0073967972;
            }
          } else {
            if ( (data[5].fvalue < 0.2189126)) {
              sum += (float)-0.03893172;
            } else {
              sum += (float)-0.12515701;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.71350193)) {
            if ( (data[15].fvalue < 0.025850978)) {
              sum += (float)-0.089348271;
            } else {
              sum += (float)-0.0030724169;
            }
          } else {
            if ( (data[15].fvalue < 0.76121747)) {
              sum += (float)-0.12203801;
            } else {
              sum += (float)-0.01027048;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.7069752)) {
              sum += (float)0.06130014;
            } else {
              sum += (float)0.0043429169;
            }
          } else {
            sum += (float)-0.070885368;
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 1.5)) {
      if ( (data[13].fvalue < 0.14791958)) {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.84254587)) {
            sum += (float)-0.033996269;
          } else {
            sum += (float)-0.15176237;
          }
        } else {
          sum += (float)0.029106928;
        }
      } else {
        if ( (data[5].fvalue < 0.34506357)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.12386001)) {
              sum += (float)-0;
            } else {
              sum += (float)0.081014909;
            }
          } else {
            if (  (data[4].fvalue < 2.2356749)) {
              sum += (float)0.045436256;
            } else {
              sum += (float)-0.031216875;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.87974852)) {
            if ( (data[15].fvalue < 0.39440197)) {
              sum += (float)-0.066373304;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.67442966)) {
              sum += (float)-0;
            } else {
              sum += (float)0.069687068;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.605989)) {
        if ( (data[14].fvalue < 0.12566376)) {
          sum += (float)-0;
        } else {
          if ( (data[15].fvalue < 0.085006267)) {
            sum += (float)0.15091284;
          } else {
            if (  (data[4].fvalue < 2.1931753)) {
              sum += (float)0.097065359;
            } else {
              sum += (float)0.013350033;
            }
          }
        }
      } else {
        sum += (float)-0.028178412;
      }
    }
  }
  if ( (data[1].fvalue < 0.21307091)) {
    if ( (data[3].fvalue < 0.2378726)) {
      sum += (float)0.12796122;
    } else {
      if ( (data[5].fvalue < 0.27282596)) {
        if ( (data[15].fvalue < 0.12481305)) {
          if ( (data[17].fvalue < 0.19310787)) {
            sum += (float)0.10151371;
          } else {
            sum += (float)0.023598056;
          }
        } else {
          if ( (data[5].fvalue < 0.17013839)) {
            sum += (float)-0;
          } else {
            sum += (float)0.0334181;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          sum += (float)0.00049889076;
        } else {
          sum += (float)-0.041379545;
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 0.85802937)) {
      if ( (data[5].fvalue < 1.3809553)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.5068773)) {
            if ( (data[0].fvalue < 2.5961351)) {
              sum += (float)0.014177831;
            } else {
              sum += (float)0.089381352;
            }
          } else {
            if ( (data[3].fvalue < 1.5640666)) {
              sum += (float)-0.073219031;
            } else {
              sum += (float)0.028599175;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            sum += (float)0.001304908;
          } else {
            if ( (data[3].fvalue < 1.7234133)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.1882439;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 0.68091297)) {
          sum += (float)0.11724019;
        } else {
          if ( (data[13].fvalue < 0.23341934)) {
            if ( (data[14].fvalue < 2.548345e-05)) {
              sum += (float)0.066090435;
            } else {
              sum += (float)0.010695562;
            }
          } else {
            sum += (float)-0.020390084;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.50643206)) {
        if ( (data[15].fvalue < 0.17208776)) {
          if ( (data[15].fvalue < 0.057864323)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.0017891903;
            } else {
              sum += (float)0.052463122;
            }
          } else {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.039068237;
            } else {
              sum += (float)0.015867757;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.5150104)) {
            if ( (data[15].fvalue < 0.59513938)) {
              sum += (float)0.059968546;
            } else {
              sum += (float)-0.07629405;
            }
          } else {
            if ( (data[15].fvalue < 0.40191483)) {
              sum += (float)-0.036235068;
            } else {
              sum += (float)0.010830037;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 1.1955689)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.65442014)) {
              sum += (float)-0.0097336303;
            } else {
              sum += (float)0.08007513;
            }
          } else {
            if ( (data[3].fvalue < 0.38956833)) {
              sum += (float)0.18400577;
            } else {
              sum += (float)0.02480902;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.70205975)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.024076371;
            } else {
              sum += (float)-0.094191872;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.014808388;
            } else {
              sum += (float)0.030961508;
            }
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.40719765)) {
    if ( (data[8].fvalue < 1.5)) {
      if ( (data[15].fvalue < 0.81755888)) {
        if (  (data[4].fvalue < 1.0679972)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.26727879;
          } else {
            if ( (data[13].fvalue < 6.9019086e-07)) {
              sum += (float)0.32905313;
            } else {
              sum += (float)-0.0091489116;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.038220711)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.025199257;
            } else {
              sum += (float)-0.012603976;
            }
          } else {
            if ( (data[5].fvalue < 0.28529572)) {
              sum += (float)0.015807373;
            } else {
              sum += (float)-0.028290335;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.097968921)) {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[3].fvalue < 0.21777394)) {
              sum += (float)0.020677311;
            } else {
              sum += (float)-0.12601648;
            }
          } else {
            if ( (data[16].fvalue < 0.0054341573)) {
              sum += (float)0.11893562;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.52406687)) {
            if ( (data[5].fvalue < 0.32860935)) {
              sum += (float)0.026984664;
            } else {
              sum += (float)-0.075791493;
            }
          } else {
            if ( (data[15].fvalue < 1.1953208)) {
              sum += (float)0.0048012836;
            } else {
              sum += (float)0.103747;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.029901745)) {
        sum += (float)0.28920311;
      } else {
        if ( (data[14].fvalue < 0.077469021)) {
          if ( (data[15].fvalue < 0.11867264)) {
            if ( (data[3].fvalue < 0.17082036)) {
              sum += (float)-0.017716173;
            } else {
              sum += (float)-0.1106462;
            }
          } else {
            if ( (data[13].fvalue < 0.17706269)) {
              sum += (float)0.10192198;
            } else {
              sum += (float)-0.066789195;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.20351619)) {
            if ( (data[0].fvalue < 2.4714961)) {
              sum += (float)0.13314788;
            } else {
              sum += (float)0.0032734682;
            }
          } else {
            if ( (data[17].fvalue < 0.23785049)) {
              sum += (float)-0.026337635;
            } else {
              sum += (float)0.027825503;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.3541195)) {
      if ( (data[3].fvalue < 2.2737093)) {
        if ( (data[13].fvalue < 0.14543182)) {
          if ( (data[5].fvalue < 0.6278758)) {
            if ( (data[14].fvalue < 0.45239753)) {
              sum += (float)0.017968599;
            } else {
              sum += (float)0.099829696;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.051293485;
            } else {
              sum += (float)-0.024171354;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)0.069848023;
          } else {
            if ( (data[3].fvalue < 1.4546226)) {
              sum += (float)0.19861719;
            } else {
              sum += (float)0.077471398;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.92027211)) {
          if ( (data[15].fvalue < 0.16766985)) {
            sum += (float)-0.11374559;
          } else {
            sum += (float)-0.024229698;
          }
        } else {
          if ( (data[6].fvalue < 0.62179625)) {
            sum += (float)0.11215982;
          } else {
            sum += (float)0.016354728;
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.92527413)) {
        if ( (data[17].fvalue < 1.0940474)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.19099824)) {
              sum += (float)-0.07841213;
            } else {
              sum += (float)-0.02644844;
            }
          } else {
            if ( (data[13].fvalue < 0.22645085)) {
              sum += (float)-0.015022854;
            } else {
              sum += (float)0.040033128;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.63149595)) {
              sum += (float)0.086770885;
            } else {
              sum += (float)0.19331487;
            }
          } else {
            sum += (float)-0.0066891531;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.014334613)) {
          if ( (data[5].fvalue < 0.49768835)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.13660485;
            } else {
              sum += (float)0.011743435;
            }
          } else {
            if ( (data[14].fvalue < 0.87424755)) {
              sum += (float)-0.040120512;
            } else {
              sum += (float)0.041425511;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.016270654)) {
            sum += (float)-0.091010191;
          } else {
            sum += (float)-0.01290328;
          }
        }
      }
    }
  }
  if ( (data[0].fvalue < 1.2198628)) {
    if ( (data[7].fvalue < 3.5)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[14].fvalue < 0.1201043)) {
          sum += (float)-0.13501206;
        } else {
          sum += (float)0.0096454918;
        }
      } else {
        if ( (data[5].fvalue < 0.27183378)) {
          if (  (data[4].fvalue < 1.0401638)) {
            sum += (float)-0.036735993;
          } else {
            if (  (data[4].fvalue < 1.1586077)) {
              sum += (float)0.1645118;
            } else {
              sum += (float)0.040163193;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[3].fvalue < 0.20832363)) {
              sum += (float)-0.020970928;
            } else {
              sum += (float)0.0078826547;
            }
          } else {
            sum += (float)0.064482339;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.103641)) {
        sum += (float)0.23506136;
      } else {
        if ( (data[8].fvalue < 3.5)) {
          sum += (float)-0.0084150471;
        } else {
          sum += (float)0.1002681;
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.27715805)) {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[17].fvalue < 1.3652304)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.037158769)) {
              sum += (float)0.0055195354;
            } else {
              sum += (float)-0.011287931;
            }
          } else {
            if ( (data[6].fvalue < 0.044624195)) {
              sum += (float)0.14526013;
            } else {
              sum += (float)-0.022343354;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.45487973)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.20048009;
            } else {
              sum += (float)-0.044628177;
            }
          } else {
            if ( (data[4].fvalue < 1.9866586)) {
              sum += (float)-0.013284423;
            } else {
              sum += (float)0.13251157;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 1.1477034)) {
          if ( (data[13].fvalue < 0.50944906)) {
            if (  (data[4].fvalue < 1.4418969)) {
              sum += (float)-0.091672808;
            } else {
              sum += (float)0.069050908;
            }
          } else {
            if ( (data[16].fvalue < 0.070174292)) {
              sum += (float)-0.068656884;
            } else {
              sum += (float)0.048600249;
            }
          }
        } else {
          if ( (data[11].fvalue < 2.5)) {
            if ( (data[15].fvalue < 0.0095218895)) {
              sum += (float)0.20094588;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 2.2283375)) {
              sum += (float)-0.050403059;
            } else {
              sum += (float)0.033517454;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.37066358)) {
        if ( (data[0].fvalue < 3.8741345)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.012167017;
            } else {
              sum += (float)0.051719245;
            }
          } else {
            if ( (data[13].fvalue < 0.21886252)) {
              sum += (float)0.046622001;
            } else {
              sum += (float)0.17533594;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.63833374)) {
            if ( (data[15].fvalue < 0.44301918)) {
              sum += (float)-0.024235928;
            } else {
              sum += (float)0.025727935;
            }
          } else {
            if ( (data[4].fvalue < 1.6007943)) {
              sum += (float)0.066527382;
            } else {
              sum += (float)-0.013526143;
            }
          }
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[17].fvalue < 1.0507479)) {
            if ( (data[16].fvalue < 0.26893651)) {
              sum += (float)-0.0045838659;
            } else {
              sum += (float)0.050474528;
            }
          } else {
            if ( (data[14].fvalue < 0.45653766)) {
              sum += (float)0.024582732;
            } else {
              sum += (float)0.14028852;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if (  (data[4].fvalue < 1.9785256)) {
              sum += (float)0.01825816;
            } else {
              sum += (float)-0.031433161;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.059434056;
            } else {
              sum += (float)-0.17065148;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.012044878)) {
    sum += (float)-0.1192479;
  } else {
    if ( (data[1].fvalue < 3.8680711)) {
      if ( (data[14].fvalue < 0.029914996)) {
        if ( (data[14].fvalue < 5.8504845e-08)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.049730763)) {
              sum += (float)0.043124035;
            } else {
              sum += (float)-0.0024781022;
            }
          } else {
            sum += (float)0.31429383;
          }
        } else {
          if ( (data[14].fvalue < 0.0020186831)) {
            if ( (data[3].fvalue < 0.15609127)) {
              sum += (float)-0.059540376;
            } else {
              sum += (float)-0.17714927;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.15714936;
            } else {
              sum += (float)-0.015136606;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.48563835)) {
          if (  (data[4].fvalue < 1.1593853)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0050191758;
            } else {
              sum += (float)0.1150514;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.013659565;
            } else {
              sum += (float)0.035988662;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.61842716)) {
            if ( (data[15].fvalue < 0.16382414)) {
              sum += (float)-0.015132658;
            } else {
              sum += (float)0.004265781;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.040197771;
            } else {
              sum += (float)-0.0075681037;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[3].fvalue < 0.26612389)) {
          if ( (data[13].fvalue < 0.012253281)) {
            sum += (float)-0.081065111;
          } else {
            if ( (data[13].fvalue < 0.059742901)) {
              sum += (float)0.034209829;
            } else {
              sum += (float)-0.046205282;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.12800495)) {
            if (  (data[4].fvalue < 0.98039985)) {
              sum += (float)-0.010065358;
            } else {
              sum += (float)0.038602348;
            }
          } else {
            if ( (data[13].fvalue < 0.36204168)) {
              sum += (float)-0.03025447;
            } else {
              sum += (float)0.0044553154;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.084833607)) {
          if ( (data[13].fvalue < 0.26992249)) {
            sum += (float)-0.15226461;
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.15775543)) {
    if (  (data[4].fvalue < 1.7918608)) {
      if (  (data[4].fvalue < 0.98676085)) {
        if ( (data[13].fvalue < 0.05331425)) {
          if ( (data[14].fvalue < 6.8807407e-05)) {
            if ( (data[3].fvalue < 0.055455927)) {
              sum += (float)-0.037847083;
            } else {
              sum += (float)0.15093806;
            }
          } else {
            if ( (data[13].fvalue < 1.9351479e-05)) {
              sum += (float)0.072582692;
            } else {
              sum += (float)-0.048687723;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.1669239)) {
            if ( (data[13].fvalue < 0.076026991)) {
              sum += (float)-0.10344789;
            } else {
              sum += (float)-0.30001143;
            }
          } else {
            if ( (data[3].fvalue < 0.28098503)) {
              sum += (float)-0.0375905;
            } else {
              sum += (float)0.038529396;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.10315731)) {
          if ( (data[13].fvalue < 0.027955573)) {
            if (  (data[4].fvalue < 1.5677545)) {
              sum += (float)-0.097291023;
            } else {
              sum += (float)0.036672045;
            }
          } else {
            if ( (data[3].fvalue < 0.27021813)) {
              sum += (float)0.0023927987;
            } else {
              sum += (float)-0.059939828;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[3].fvalue < 0.33202052)) {
              sum += (float)0.032038648;
            } else {
              sum += (float)-0.0073403618;
            }
          } else {
            if ( (data[3].fvalue < 0.40774894)) {
              sum += (float)-0.012787382;
            } else {
              sum += (float)-0.12816198;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.080819234)) {
        if ( (data[17].fvalue < 0.26631182)) {
          if ( (data[14].fvalue < 0.21634018)) {
            if ( (data[13].fvalue < 0.0030710544)) {
              sum += (float)0.32288575;
            } else {
              sum += (float)0.10648639;
            }
          } else {
            sum += (float)-0.011801375;
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.093714923;
            } else {
              sum += (float)0.0083205793;
            }
          } else {
            sum += (float)0.077797867;
          }
        }
      } else {
        if ( (data[15].fvalue < 2.5226145)) {
          if ( (data[16].fvalue < 1.2809494)) {
            if ( (data[3].fvalue < 0.37347183)) {
              sum += (float)-0.017425412;
            } else {
              sum += (float)0.017712472;
            }
          } else {
            sum += (float)-0.07589905;
          }
        } else {
          if ( (data[14].fvalue < 0.42119026)) {
            sum += (float)-0.098922767;
          } else {
            sum += (float)0.027000969;
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.06261012)) {
      if (  (data[4].fvalue < 1.0416613)) {
        if ( (data[0].fvalue < 3.9078345)) {
          sum += (float)-0.23623855;
        } else {
          if ( (data[6].fvalue < 0.049791798)) {
            sum += (float)-0.13033199;
          } else {
            sum += (float)-0.0028502408;
          }
        }
      } else {
        sum += (float)-0;
      }
    } else {
      if (  (data[4].fvalue < 1.0101383)) {
        if ( (data[3].fvalue < 0.32165906)) {
          sum += (float)-0.21150893;
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[3].fvalue < 0.51221061)) {
              sum += (float)-0.067110337;
            } else {
              sum += (float)-0.00088113826;
            }
          } else {
            if ( (data[3].fvalue < 1.4006503)) {
              sum += (float)0.1478066;
            } else {
              sum += (float)-0.054735556;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.47047)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[5].fvalue < 0.289783)) {
              sum += (float)0.086519033;
            } else {
              sum += (float)0.02432307;
            }
          } else {
            if ( (data[14].fvalue < 0.14036036)) {
              sum += (float)-0.0095614707;
            } else {
              sum += (float)0.033350147;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.40425551)) {
            if ( (data[15].fvalue < 0.036369152)) {
              sum += (float)0.0026031798;
            } else {
              sum += (float)-0.033098716;
            }
          } else {
            if (  (data[4].fvalue < 1.7907484)) {
              sum += (float)0.053863142;
            } else {
              sum += (float)0.0019514429;
            }
          }
        }
      }
    }
  }
  if ( (data[1].fvalue < 2.3461409)) {
    if ( (data[6].fvalue < 0.025521986)) {
      sum += (float)-0.13880241;
    } else {
      if ( (data[3].fvalue < 0.72993034)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[13].fvalue < 0.70363891)) {
            if ( (data[13].fvalue < 0.15224892)) {
              sum += (float)-0.0028851391;
            } else {
              sum += (float)0.017114731;
            }
          } else {
            if ( (data[17].fvalue < 0.46697831)) {
              sum += (float)-0.0670368;
            } else {
              sum += (float)0.048780758;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0481863)) {
            sum += (float)0.22673668;
          } else {
            if ( (data[13].fvalue < 0.39651692)) {
              sum += (float)-7.68469e-06;
            } else {
              sum += (float)0.064505443;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if (  (data[4].fvalue < 1.8647425)) {
            if ( (data[15].fvalue < 1.3396347)) {
              sum += (float)-0.0020038683;
            } else {
              sum += (float)-0.092245907;
            }
          } else {
            if ( (data[13].fvalue < 1.5388637)) {
              sum += (float)0.022591574;
            } else {
              sum += (float)-0.037920758;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.66713262)) {
              sum += (float)0.028153336;
            } else {
              sum += (float)0.10216116;
            }
          } else {
            if ( (data[13].fvalue < 0.07463681)) {
              sum += (float)-0.1204275;
            } else {
              sum += (float)-0.017333569;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.15251376)) {
      if ( (data[15].fvalue < 0.11999053)) {
        if (  (data[4].fvalue < 1.1583021)) {
          if ( (data[14].fvalue < 0.18739212)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.13852967;
            } else {
              sum += (float)-0.0069120312;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.015297132;
            } else {
              sum += (float)0.091202237;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.40917647)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.033599861;
            } else {
              sum += (float)-0.092779309;
            }
          } else {
            if ( (data[14].fvalue < 0.035790827)) {
              sum += (float)-0.017681988;
            } else {
              sum += (float)0.021949662;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.25362587)) {
          if ( (data[14].fvalue < 0.002966132)) {
            if ( (data[5].fvalue < 0.32285911)) {
              sum += (float)0.012796828;
            } else {
              sum += (float)-0.058942359;
            }
          } else {
            if ( (data[6].fvalue < 0.13373138)) {
              sum += (float)0.14268588;
            } else {
              sum += (float)0.056219276;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.50119394)) {
            if ( (data[6].fvalue < 0.29519874)) {
              sum += (float)0.00011540574;
            } else {
              sum += (float)-0.037512343;
            }
          } else {
            if (  (data[4].fvalue < 1.6561823)) {
              sum += (float)0.036515363;
            } else {
              sum += (float)-0.014066196;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.31716526)) {
        if ( (data[13].fvalue < 0.010559637)) {
          sum += (float)-0.049432658;
        } else {
          if ( (data[3].fvalue < 0.31735128)) {
            if ( (data[14].fvalue < 0.0048948675)) {
              sum += (float)0.065740019;
            } else {
              sum += (float)0.16062085;
            }
          } else {
            if ( (data[14].fvalue < 0.091489166)) {
              sum += (float)0.015302546;
            } else {
              sum += (float)0.079580992;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.36410588)) {
          sum += (float)-0.071066067;
        } else {
          if ( (data[3].fvalue < 0.93434882)) {
            if ( (data[3].fvalue < 0.50371903)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.066505782;
            }
          } else {
            if ( (data[2].fvalue < 0.38746062)) {
              sum += (float)0.025828324;
            } else {
              sum += (float)-0.033206832;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.0081551466)) {
    if ( (data[13].fvalue < 6.112888e-08)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[3].fvalue < 0.52977622)) {
              sum += (float)-0.0071053808;
            } else {
              sum += (float)0.18623756;
            }
          } else {
            if ( (data[15].fvalue < 0.0009376544)) {
              sum += (float)-0.17391536;
            } else {
              sum += (float)-0.020785745;
            }
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.33745828;
            } else {
              sum += (float)0.10604057;
            }
          } else {
            if ( (data[16].fvalue < 0.0039994237)) {
              sum += (float)0.0558463;
            } else {
              sum += (float)-0.065198235;
            }
          }
        }
      } else {
        sum += (float)0.31193036;
      }
    } else {
      if ( (data[13].fvalue < 0.0021344621)) {
        if (  (data[4].fvalue < 1.0117745)) {
          sum += (float)-0.075290427;
        } else {
          sum += (float)-0.1999899;
        }
      } else {
        if (  (data[4].fvalue < 0.9893899)) {
          if ( (data[14].fvalue < 7.2014627e-06)) {
            sum += (float)0.18324731;
          } else {
            sum += (float)-0.072333515;
          }
        } else {
          if ( (data[14].fvalue < 0.0075997706)) {
            sum += (float)-0.19565228;
          } else {
            if ( (data[17].fvalue < 0.10621199)) {
              sum += (float)-0.056515187;
            } else {
              sum += (float)0.051707532;
            }
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.19773528)) {
      if ( (data[14].fvalue < 0.53675073)) {
        if ( (data[17].fvalue < 0.15631866)) {
          if ( (data[4].fvalue < 1.2010605)) {
            if ( (data[14].fvalue < 0.11920546)) {
              sum += (float)-0.0037886556;
            } else {
              sum += (float)0.02422221;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.0075643621;
            } else {
              sum += (float)-0.058833268;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.43550918)) {
            if (  (data[4].fvalue < 1.8205745)) {
              sum += (float)0.087517969;
            } else {
              sum += (float)-0.027191026;
            }
          } else {
            if ( (data[14].fvalue < 0.013477817)) {
              sum += (float)-0.017428223;
            } else {
              sum += (float)0.018728372;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.5208491)) {
          if ( (data[3].fvalue < 2.2362914)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.055412263;
            } else {
              sum += (float)0.11900104;
            }
          } else {
            if ( (data[14].fvalue < 0.9396596)) {
              sum += (float)-0.08013425;
            } else {
              sum += (float)0.064145185;
            }
          }
        } else {
          if ( (data[13].fvalue < 1.0035574)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.042632528;
            } else {
              sum += (float)0.022493757;
            }
          } else {
            sum += (float)0.14796437;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.7010342)) {
        if ( (data[14].fvalue < 0.0028055925)) {
          if ( (data[15].fvalue < 0.969661)) {
            if ( (data[13].fvalue < 0.049215794)) {
              sum += (float)-0.10671547;
            } else {
              sum += (float)0.015829144;
            }
          } else {
            sum += (float)-0.12897329;
          }
        } else {
          if ( (data[3].fvalue < 1.4316218)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.065271899;
            } else {
              sum += (float)0.16793707;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.045430336;
            } else {
              sum += (float)-0.03242749;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.58923602)) {
          if ( (data[16].fvalue < 0.10211192)) {
            if ( (data[17].fvalue < 0.43160918)) {
              sum += (float)-0.094483756;
            } else {
              sum += (float)0.002613473;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0075482903;
            } else {
              sum += (float)0.073450573;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.2538881)) {
            if ( (data[17].fvalue < 0.45868057)) {
              sum += (float)0.023637624;
            } else {
              sum += (float)0.079859011;
            }
          } else {
            if ( (data[15].fvalue < 1.710644)) {
              sum += (float)-0.043821838;
            } else {
              sum += (float)0.0058050295;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.31237057)) {
    if ( (data[8].fvalue < 6.5)) {
      if ( (data[9].fvalue < 2.5)) {
        if (  (data[4].fvalue < 1.1364635)) {
          if ( (data[5].fvalue < 0.35529155)) {
            if (  (data[4].fvalue < 1.0673832)) {
              sum += (float)-0.010317377;
            } else {
              sum += (float)0.14255357;
            }
          } else {
            if ( (data[14].fvalue < 0.65609109)) {
              sum += (float)-0.001667415;
            } else {
              sum += (float)0.09095256;
            }
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.0085137244;
            } else {
              sum += (float)-0.073699787;
            }
          } else {
            if ( (data[15].fvalue < 0.021100901)) {
              sum += (float)0.12522383;
            } else {
              sum += (float)-0.0035673385;
            }
          }
        }
      } else {
        sum += (float)0.11149878;
      }
    } else {
      if ( (data[14].fvalue < 0.19135626)) {
        sum += (float)0.14265116;
      } else {
        if ( (data[7].fvalue < 3.5)) {
          sum += (float)0.066161267;
        } else {
          if ( (data[14].fvalue < 0.54019034)) {
            sum += (float)0.056449428;
          } else {
            sum += (float)-0.095924884;
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 1.0403484)) {
      if ( (data[16].fvalue < 0.042319603)) {
        if (  (data[4].fvalue < 1.0786629)) {
          if ( (data[3].fvalue < 0.60712075)) {
            if ( (data[13].fvalue < 0.33199662)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.1407747;
            }
          } else {
            if ( (data[13].fvalue < 0.43986744)) {
              sum += (float)0.030882539;
            } else {
              sum += (float)-0.017635649;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.29966527)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.06387607;
            } else {
              sum += (float)0.012486867;
            }
          } else {
            if ( (data[17].fvalue < 1.4590526)) {
              sum += (float)0.002502236;
            } else {
              sum += (float)0.065405928;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.00020372888)) {
          if ( (data[14].fvalue < 0.12423396)) {
            if ( (data[0].fvalue < 2.4272623)) {
              sum += (float)-0.0013434489;
            } else {
              sum += (float)-0.10670893;
            }
          } else {
            if ( (data[16].fvalue < 0.16192836)) {
              sum += (float)-0.054909129;
            } else {
              sum += (float)0.02191098;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.5382545)) {
            if (  (data[4].fvalue < 1.7845116)) {
              sum += (float)0.11077405;
            } else {
              sum += (float)0.002339582;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.028977605;
            } else {
              sum += (float)0.0038573006;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.81427526)) {
        if ( (data[0].fvalue < 4.4063911)) {
          sum += (float)0.061752897;
        } else {
          sum += (float)-0.028743003;
        }
      } else {
        sum += (float)0.13731846;
      }
    }
  }
  if ( (data[3].fvalue < 0.051357061)) {
    if (  (data[4].fvalue < 0.96516621)) {
      sum += (float)-0.17194058;
    } else {
      if ( (data[13].fvalue < 0.042669125)) {
        if (  (data[4].fvalue < 1.0044532)) {
          if ( (data[13].fvalue < 0.026597857)) {
            if ( (data[13].fvalue < 0.0087600071)) {
              sum += (float)0.090291709;
            } else {
              sum += (float)0.22212493;
            }
          } else {
            sum += (float)0.048696905;
          }
        } else {
          if ( (data[15].fvalue < 0.089683175)) {
            if ( (data[14].fvalue < 0.047273982)) {
              sum += (float)-0.079878479;
            } else {
              sum += (float)0.045277476;
            }
          } else {
            if (  (data[4].fvalue < 1.2898116)) {
              sum += (float)0.10882273;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.0073061)) {
          sum += (float)-0.19952892;
        } else {
          if ( (data[15].fvalue < 0.00068829628)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.021792594;
            } else {
              sum += (float)0.1315304;
            }
          } else {
            if ( (data[13].fvalue < 0.082659215)) {
              sum += (float)0.017809322;
            } else {
              sum += (float)0.11704996;
            }
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.46435627)) {
      if ( (data[17].fvalue < 0.36477971)) {
        if ( (data[4].fvalue < 0.76670426)) {
          if ( (data[5].fvalue < 1.5953541)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.036950912;
            } else {
              sum += (float)-0.02549877;
            }
          } else {
            sum += (float)0.098155603;
          }
        } else {
          if ( (data[14].fvalue < 0.17587596)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0047416366;
            } else {
              sum += (float)-0.028100014;
            }
          } else {
            if ( (data[6].fvalue < 0.21820052)) {
              sum += (float)0.039166756;
            } else {
              sum += (float)-0.00332959;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.044796187)) {
          if ( (data[14].fvalue < 0.0027246377)) {
            sum += (float)-0.18405861;
          } else {
            if ( (data[17].fvalue < 0.65835202)) {
              sum += (float)-0.021287652;
            } else {
              sum += (float)0.018562656;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.76360244)) {
            if ( (data[15].fvalue < 0.045935873)) {
              sum += (float)0.057153162;
            } else {
              sum += (float)-0.0041295192;
            }
          } else {
            if ( (data[13].fvalue < 0.64822733)) {
              sum += (float)-0.0095770396;
            } else {
              sum += (float)0.027992254;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.76070052)) {
        if ( (data[14].fvalue < 0.012639052)) {
          if ( (data[13].fvalue < 0.26973999)) {
            if ( (data[16].fvalue < 0.75843412)) {
              sum += (float)0.0058784792;
            } else {
              sum += (float)-0.081789106;
            }
          } else {
            if ( (data[15].fvalue < 0.26886457)) {
              sum += (float)-0;
            } else {
              sum += (float)0.06013843;
            }
          }
        } else {
          if ( (data[0].fvalue < 4.6185083)) {
            if ( (data[3].fvalue < 1.0251639)) {
              sum += (float)0.086808756;
            } else {
              sum += (float)0.03827912;
            }
          } else {
            if ( (data[14].fvalue < 0.46964335)) {
              sum += (float)-0.053884011;
            } else {
              sum += (float)0.054278933;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.76684439)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.090865418;
        }
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[9].fvalue < 2.5)) {
        if (  (data[4].fvalue < 2.1973114)) {
          if ( (data[15].fvalue < 2.0429134)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.0024547921;
            } else {
              sum += (float)0.023132471;
            }
          } else {
            if ( (data[14].fvalue < 0.26679385)) {
              sum += (float)-0.13594118;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.56488323)) {
            if ( (data[17].fvalue < 0.52093786)) {
              sum += (float)0.076298982;
            } else {
              sum += (float)0.0036956239;
            }
          } else {
            if ( (data[16].fvalue < 0.037656724)) {
              sum += (float)0.020878902;
            } else {
              sum += (float)-0.036838517;
            }
          }
        }
      } else {
        sum += (float)0.10202255;
      }
    } else {
      if ( (data[3].fvalue < 0.37588519)) {
        if (  (data[4].fvalue < 1.12903)) {
          sum += (float)0.18085536;
        } else {
          if (  (data[4].fvalue < 1.872575)) {
            if ( (data[13].fvalue < 0.26368526)) {
              sum += (float)0.02231165;
            } else {
              sum += (float)0.13582358;
            }
          } else {
            sum += (float)-0.061125349;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.75047654)) {
          if ( (data[3].fvalue < 0.89310598)) {
            if ( (data[0].fvalue < 3.752203)) {
              sum += (float)0.024738582;
            } else {
              sum += (float)-0.066512696;
            }
          } else {
            if ( (data[15].fvalue < 0.62436438)) {
              sum += (float)-0.061060902;
            } else {
              sum += (float)0.031014426;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.19796598)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.030306539;
            } else {
              sum += (float)-0.072200797;
            }
          } else {
            if ( (data[3].fvalue < 2.3236256)) {
              sum += (float)0.1502445;
            } else {
              sum += (float)0.030403394;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[13].fvalue < 0.60995042)) {
        if ( (data[13].fvalue < 0.29064161)) {
          sum += (float)-0.030108919;
        } else {
          if ( (data[3].fvalue < 0.57700312)) {
            sum += (float)0.088282339;
          } else {
            sum += (float)-0.0061914986;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.95823455)) {
          if ( (data[3].fvalue < 0.81258428)) {
            sum += (float)0.13398884;
          } else {
            sum += (float)0.017738871;
          }
        } else {
          if ( (data[6].fvalue < 0.64472342)) {
            sum += (float)0.20663856;
          } else {
            sum += (float)0.10314346;
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.2422798)) {
        if ( (data[0].fvalue < 3.2225657)) {
          sum += (float)0.19920985;
        } else {
          sum += (float)0.015079401;
        }
      } else {
        if ( (data[0].fvalue < 2.1529527)) {
          if ( (data[6].fvalue < 0.51556432)) {
            sum += (float)0.12008513;
          } else {
            sum += (float)-0.02167042;
          }
        } else {
          if ( (data[15].fvalue < 0.13462749)) {
            if ( (data[13].fvalue < 0.93700069)) {
              sum += (float)-0.10171944;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 1.8664525)) {
              sum += (float)0.076161087;
            } else {
              sum += (float)-0.03966077;
            }
          }
        }
      }
    }
  }
  if ( (data[15].fvalue < 3.095675)) {
    if ( (data[5].fvalue < 0.035234444)) {
      if ( (data[1].fvalue < 0.90566111)) {
        if ( (data[6].fvalue < 0.64766324)) {
          sum += (float)0.047960855;
        } else {
          sum += (float)0.13081957;
        }
      } else {
        if ( (data[5].fvalue < 0.0096536092)) {
          sum += (float)0.049707361;
        } else {
          if (  (data[4].fvalue < 1.7964991)) {
            if ( (data[6].fvalue < 0.27454501)) {
              sum += (float)-0;
            } else {
              sum += (float)0.044803943;
            }
          } else {
            sum += (float)-0.037462667;
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.99875456)) {
        if ( (data[1].fvalue < 0.29606301)) {
          if ( (data[3].fvalue < 0.13049227)) {
            sum += (float)0.10867601;
          } else {
            if ( (data[5].fvalue < 0.6201458)) {
              sum += (float)0.0068230913;
            } else {
              sum += (float)0.072353698;
            }
          }
        } else {
          if ( (data[14].fvalue < 2.0460303)) {
            if (  (data[4].fvalue < 2.3180265)) {
              sum += (float)-0.0036928945;
            } else {
              sum += (float)0.021810347;
            }
          } else {
            if ( (data[0].fvalue < 3.7977378)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.072754428;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.6540439)) {
          if ( (data[3].fvalue < 2.5643024)) {
            if ( (data[15].fvalue < 1.1955274)) {
              sum += (float)-0.062237784;
            } else {
              sum += (float)-0.17077014;
            }
          } else {
            sum += (float)0.0083943969;
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.64735293)) {
              sum += (float)0.017700143;
            } else {
              sum += (float)0.074631706;
            }
          } else {
            if ( (data[13].fvalue < 0.17803156)) {
              sum += (float)-0.011457484;
            } else {
              sum += (float)-0.0860807;
            }
          }
        }
      }
    }
  } else {
    sum += (float)-0.075118408;
  }
  if ( (data[13].fvalue < 2.5788393)) {
    if ( (data[0].fvalue < 3.3696904)) {
      if ( (data[6].fvalue < 0.016511545)) {
        sum += (float)-0.16200943;
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.0010084329;
            } else {
              sum += (float)0.031079836;
            }
          } else {
            if ( (data[14].fvalue < 0.56496608)) {
              sum += (float)0.010895003;
            } else {
              sum += (float)0.11726977;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.26046759)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.041755162;
            } else {
              sum += (float)0.11765842;
            }
          } else {
            if ( (data[13].fvalue < 0.59926027)) {
              sum += (float)-0.019006662;
            } else {
              sum += (float)0.047386568;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[7].fvalue < 5.5)) {
          if (  (data[4].fvalue < 2.2929218)) {
            if ( (data[5].fvalue < 0.19864789)) {
              sum += (float)-0.022401072;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.53894961)) {
              sum += (float)-0.00036085959;
            } else {
              sum += (float)0.058100618;
            }
          }
        } else {
          sum += (float)0.076899633;
        }
      } else {
        if ( (data[16].fvalue < 0.223864)) {
          if ( (data[17].fvalue < 1.2404342)) {
            if (  (data[4].fvalue < 1.8968431)) {
              sum += (float)-0.02559142;
            } else {
              sum += (float)-0.075589284;
            }
          } else {
            sum += (float)0.038739465;
          }
        } else {
          if (  (data[4].fvalue < 2.5856428)) {
            if ( (data[13].fvalue < 0.34735692)) {
              sum += (float)0.091174744;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.058561217;
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 2.3830819)) {
      sum += (float)-0.026667589;
    } else {
      sum += (float)-0.10203294;
    }
  }
  if ( (data[14].fvalue < 0.21208259)) {
    if ( (data[8].fvalue < 1.5)) {
      if ( (data[15].fvalue < 0.032011066)) {
        if ( (data[13].fvalue < 6.9019086e-07)) {
          if ( (data[17].fvalue < 0.013867188)) {
            if (  (data[4].fvalue < 1.3505163)) {
              sum += (float)0.020970805;
            } else {
              sum += (float)0.32881728;
            }
          } else {
            sum += (float)-0.12625644;
          }
        } else {
          if ( (data[14].fvalue < 5.8504845e-08)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.006683276;
            } else {
              sum += (float)0.3030425;
            }
          } else {
            if ( (data[14].fvalue < 0.0097877756)) {
              sum += (float)-0.08359693;
            } else {
              sum += (float)1.3499409e-05;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.09348546)) {
          if ( (data[5].fvalue < 0.22837448)) {
            if ( (data[15].fvalue < 0.11003503)) {
              sum += (float)-0.080750152;
            } else {
              sum += (float)-0.0059450525;
            }
          } else {
            if ( (data[14].fvalue < 0.078674614)) {
              sum += (float)-0.11619873;
            } else {
              sum += (float)-0.037685633;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0014845931)) {
            if ( (data[15].fvalue < 0.37828717)) {
              sum += (float)-0.027652439;
            } else {
              sum += (float)0.018033326;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.060329493;
            } else {
              sum += (float)-0.0074342499;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[6].fvalue < 0.065017298)) {
          if ( (data[0].fvalue < 2.9267592)) {
            sum += (float)0.23177414;
          } else {
            if ( (data[13].fvalue < 0.029925521)) {
              sum += (float)-0.086442389;
            } else {
              sum += (float)0.060631867;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.094616108)) {
            if ( (data[13].fvalue < 0.15522227)) {
              sum += (float)-0.14138043;
            } else {
              sum += (float)-0.029896697;
            }
          } else {
            if ( (data[13].fvalue < 0.23858136)) {
              sum += (float)0.12468655;
            } else {
              sum += (float)-0.053548954;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.23665246)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.013476375;
            } else {
              sum += (float)0.099702798;
            }
          } else {
            if ( (data[13].fvalue < 0.12342853)) {
              sum += (float)0.22219336;
            } else {
              sum += (float)0.097012423;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.6808124)) {
              sum += (float)0.1766803;
            } else {
              sum += (float)0.04697727;
            }
          } else {
            if ( (data[14].fvalue < 0.083852492)) {
              sum += (float)-0.15318856;
            } else {
              sum += (float)-0.028594105;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.14558288)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[3].fvalue < 0.47153896)) {
          sum += (float)0.016628958;
        } else {
          sum += (float)-0.024593955;
        }
      } else {
        if (  (data[4].fvalue < 1.2219138)) {
          sum += (float)0.21576668;
        } else {
          sum += (float)0.072773926;
        }
      }
    } else {
      if ( (data[0].fvalue < 2.3845153)) {
        if ( (data[13].fvalue < 0.20102654)) {
          if ( (data[3].fvalue < 0.22331545)) {
            if ( (data[14].fvalue < 0.3182857)) {
              sum += (float)0.015180496;
            } else {
              sum += (float)0.095368363;
            }
          } else {
            if ( (data[6].fvalue < 0.29100943)) {
              sum += (float)0.052078214;
            } else {
              sum += (float)-0.0051227775;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.53718865)) {
              sum += (float)0.00012675824;
            } else {
              sum += (float)0.08707615;
            }
          } else {
            if ( (data[6].fvalue < 0.44547576)) {
              sum += (float)0.18333058;
            } else {
              sum += (float)0.04721804;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 1.2832055)) {
          if ( (data[15].fvalue < 1.194365)) {
            if (  (data[4].fvalue < 2.0725069)) {
              sum += (float)-0.0015063422;
            } else {
              sum += (float)-0.046568282;
            }
          } else {
            if ( (data[16].fvalue < 0.037727334)) {
              sum += (float)0.043892987;
            } else {
              sum += (float)-0.049845029;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.59459674)) {
            if ( (data[15].fvalue < 0.33784544)) {
              sum += (float)0.047777172;
            } else {
              sum += (float)-0.064317085;
            }
          } else {
            if ( (data[13].fvalue < 0.016835522)) {
              sum += (float)0.15249659;
            } else {
              sum += (float)0.01047861;
            }
          }
        }
      }
    }
  }
  if ( (data[0].fvalue < 1.224025)) {
    if ( (data[7].fvalue < 3.5)) {
      if (  (data[4].fvalue < 1.0186105)) {
        if ( (data[13].fvalue < 0.067834422)) {
          if ( (data[6].fvalue < 0.079881757)) {
            sum += (float)-0.022192337;
          } else {
            if ( (data[14].fvalue < 2.1315629e-05)) {
              sum += (float)0.16670528;
            } else {
              sum += (float)0.00054406305;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.23770048)) {
            if ( (data[13].fvalue < 0.095507726)) {
              sum += (float)-0.057048738;
            } else {
              sum += (float)-0.15378809;
            }
          } else {
            if ( (data[13].fvalue < 0.21709974)) {
              sum += (float)0.02585292;
            } else {
              sum += (float)-0.027744282;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.05569388)) {
          if ( (data[15].fvalue < 0.11078575)) {
            if ( (data[14].fvalue < 0.10456235)) {
              sum += (float)-0.13862912;
            } else {
              sum += (float)-0.01951718;
            }
          } else {
            sum += (float)0.022426708;
          }
        } else {
          if ( (data[6].fvalue < 0.33239138)) {
            if ( (data[14].fvalue < 0.010568677)) {
              sum += (float)0.03246443;
            } else {
              sum += (float)0.16623753;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.045000728;
            } else {
              sum += (float)-0.0069084638;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.60048097)) {
        if (  (data[4].fvalue < 1.1052384)) {
          sum += (float)0.15357502;
        } else {
          sum += (float)0.056960359;
        }
      } else {
        sum += (float)-0;
      }
    }
  } else {
    if ( (data[3].fvalue < 3.3630157)) {
      if ( (data[8].fvalue < 9.5)) {
        if ( (data[13].fvalue < 0.34417731)) {
          if ( (data[6].fvalue < 0.16124216)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.054749746;
            } else {
              sum += (float)0.0066055409;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.00045865096;
            } else {
              sum += (float)-0.021820344;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.5002141)) {
            if ( (data[14].fvalue < 0.094421834)) {
              sum += (float)0.0061586881;
            } else {
              sum += (float)0.043689035;
            }
          } else {
            if ( (data[14].fvalue < 0.3560074)) {
              sum += (float)-0.011481752;
            } else {
              sum += (float)0.025370764;
            }
          }
        }
      } else {
        sum += (float)0.065962195;
      }
    } else {
      if ( (data[14].fvalue < 0.084435225)) {
        if ( (data[13].fvalue < 1.8854594)) {
          if ( (data[16].fvalue < 0.42893708)) {
            if ( (data[17].fvalue < 1.2290587)) {
              sum += (float)0.063276276;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.014191427;
          }
        } else {
          sum += (float)-0.034804966;
        }
      } else {
        if ( (data[14].fvalue < 1.0966072)) {
          if ( (data[16].fvalue < 0.17222877)) {
            if ( (data[2].fvalue < 1.3556702)) {
              sum += (float)-0.055559643;
            } else {
              sum += (float)-0.12879108;
            }
          } else {
            sum += (float)-0.00078996358;
          }
        } else {
          if ( (data[14].fvalue < 2.1038747)) {
            if (  (data[4].fvalue < 1.6591057)) {
              sum += (float)-0.0020069752;
            } else {
              sum += (float)0.047354538;
            }
          } else {
            sum += (float)-0.062489603;
          }
        }
      }
    }
  }
  if ( (data[8].fvalue < 10.5)) {
    if ( (data[17].fvalue < 2.984832)) {
      if ( (data[2].fvalue < 1.6557391)) {
        if ( (data[14].fvalue < 0.43931109)) {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.00061690016;
            } else {
              sum += (float)-0.0091271522;
            }
          } else {
            if ( (data[3].fvalue < 0.43760294)) {
              sum += (float)0.087239467;
            } else {
              sum += (float)0.0042533507;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6639574)) {
            if ( (data[3].fvalue < 1.4159126)) {
              sum += (float)0.040345833;
            } else {
              sum += (float)0.0079832738;
            }
          } else {
            if ( (data[13].fvalue < 0.63281387)) {
              sum += (float)-0.016388329;
            } else {
              sum += (float)0.071177699;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.87301099)) {
          if ( (data[13].fvalue < 0.26113033)) {
            if ( (data[14].fvalue < 0.62393636)) {
              sum += (float)-0.069537163;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[6].fvalue < 0.81491613)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.050569534;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.56657064)) {
            if (  (data[4].fvalue < 1.7456627)) {
              sum += (float)0.053239819;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.03669133;
          }
        }
      }
    } else {
      sum += (float)-0.072471723;
    }
  } else {
    sum += (float)0.10109039;
  }
  if ( (data[15].fvalue < 0.15036997)) {
    if ( (data[15].fvalue < 0.052211408)) {
      if ( (data[14].fvalue < 0.54459977)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[3].fvalue < 0.26820877)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.20279987;
            } else {
              sum += (float)-0.0072389143;
            }
          } else {
            if ( (data[13].fvalue < 0.033370171)) {
              sum += (float)0.056937054;
            } else {
              sum += (float)0.0072083622;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0032296)) {
            if ( (data[3].fvalue < 0.16130139)) {
              sum += (float)0.26527548;
            } else {
              sum += (float)0.0098776743;
            }
          } else {
            if ( (data[13].fvalue < 0.2703169)) {
              sum += (float)-0.048528008;
            } else {
              sum += (float)0.0004072774;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.33657858)) {
          if ( (data[4].fvalue < 1.4515364)) {
            if ( (data[3].fvalue < 0.9225018)) {
              sum += (float)0.14818127;
            } else {
              sum += (float)0.069554336;
            }
          } else {
            sum += (float)0.04359059;
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.0034520272)) {
              sum += (float)-0.011631642;
            } else {
              sum += (float)0.083204836;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.07814467;
            } else {
              sum += (float)0.023121452;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.093297139)) {
        if ( (data[16].fvalue < 0.019513272)) {
          if ( (data[3].fvalue < 0.042848654)) {
            sum += (float)0.032699488;
          } else {
            if ( (data[17].fvalue < 0.87821567)) {
              sum += (float)-0.057680622;
            } else {
              sum += (float)0.0068726963;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.080829844)) {
            if ( (data[13].fvalue < 0.62742543)) {
              sum += (float)0.04019472;
            } else {
              sum += (float)-0.011994979;
            }
          } else {
            sum += (float)-0.036538973;
          }
        }
      } else {
        if ( (data[4].fvalue < 1.4300088)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.016872721;
            } else {
              sum += (float)-0.033369318;
            }
          } else {
            sum += (float)0.084242508;
          }
        } else {
          if ( (data[16].fvalue < 0.0048678191)) {
            if (  (data[4].fvalue < 2.1182978)) {
              sum += (float)-0.067133196;
            } else {
              sum += (float)0.021137264;
            }
          } else {
            if ( (data[16].fvalue < 0.37913549)) {
              sum += (float)0.061797995;
            } else {
              sum += (float)-0.042882163;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.25364897)) {
      if ( (data[14].fvalue < 0.0033459673)) {
        if ( (data[5].fvalue < 0.34877747)) {
          if ( (data[13].fvalue < 0.10556209)) {
            if ( (data[15].fvalue < 0.30019611)) {
              sum += (float)0.036651906;
            } else {
              sum += (float)-0.059585325;
            }
          } else {
            if ( (data[3].fvalue < 0.29396155)) {
              sum += (float)0.10618393;
            } else {
              sum += (float)0.0240405;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1891551)) {
            sum += (float)-0;
          } else {
            sum += (float)-0.059237763;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.2718915)) {
          if ( (data[3].fvalue < 0.6229893)) {
            sum += (float)0.1516169;
          } else {
            sum += (float)0.043685667;
          }
        } else {
          if ( (data[15].fvalue < 0.24867654)) {
            if ( (data[13].fvalue < 0.071841076)) {
              sum += (float)-0.050819915;
            } else {
              sum += (float)0.023497673;
            }
          } else {
            if ( (data[0].fvalue < 5.3263464)) {
              sum += (float)0.074144907;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[0].fvalue < 2.8522243)) {
        if ( (data[14].fvalue < 0.0045096623)) {
          if ( (data[13].fvalue < 0.066151299)) {
            if ( (data[5].fvalue < 0.1621183)) {
              sum += (float)-0.04009594;
            } else {
              sum += (float)-0.18054806;
            }
          } else {
            if (  (data[4].fvalue < 1.3231804)) {
              sum += (float)-0.024993485;
            } else {
              sum += (float)0.020779012;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.40572801)) {
            if ( (data[13].fvalue < 0.053171515)) {
              sum += (float)0.041333653;
            } else {
              sum += (float)0.11370983;
            }
          } else {
            if ( (data[13].fvalue < 0.0048685251)) {
              sum += (float)-0.001459562;
            } else {
              sum += (float)0.026753617;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.34410816)) {
          if ( (data[16].fvalue < 0.086914942)) {
            if ( (data[13].fvalue < 0.88484532)) {
              sum += (float)-0.023712307;
            } else {
              sum += (float)-0.097375646;
            }
          } else {
            if ( (data[4].fvalue < 2.2113335)) {
              sum += (float)-0.0065265647;
            } else {
              sum += (float)0.066047229;
            }
          }
        } else {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.65268624)) {
              sum += (float)0.010991215;
            } else {
              sum += (float)-0.0086129615;
            }
          } else {
            sum += (float)-0.10292496;
          }
        }
      }
    }
  }
  if ( (data[3].fvalue < 0.055443682)) {
    if (  (data[4].fvalue < 0.96358562)) {
      sum += (float)-0.14000979;
    } else {
      if ( (data[14].fvalue < 0.018933937)) {
        if ( (data[13].fvalue < 0.042880081)) {
          if (  (data[4].fvalue < 1.0081202)) {
            if (  (data[4].fvalue < 0.9764142)) {
              sum += (float)0.011823922;
            } else {
              sum += (float)0.13013598;
            }
          } else {
            if ( (data[15].fvalue < 0.093559913)) {
              sum += (float)-0.07616242;
            } else {
              sum += (float)0.005763662;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0059025)) {
            sum += (float)-0.16600911;
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.015710903;
            } else {
              sum += (float)0.043129019;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.1087451)) {
            sum += (float)0.09849859;
          } else {
            if ( (data[13].fvalue < 0.23501332)) {
              sum += (float)-0.0250092;
            } else {
              sum += (float)0.078084201;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3540525)) {
            sum += (float)0.13507032;
          } else {
            sum += (float)0.023521636;
          }
        }
      }
    }
  } else {
    if ( (data[0].fvalue < 1.3080487)) {
      if ( (data[8].fvalue < 2.5)) {
        if (  (data[4].fvalue < 1.0314021)) {
          if ( (data[13].fvalue < 0.1102067)) {
            if ( (data[14].fvalue < 9.8353543e-05)) {
              sum += (float)0.045719046;
            } else {
              sum += (float)-0.034520183;
            }
          } else {
            if ( (data[6].fvalue < 0.16007268)) {
              sum += (float)-0.16653781;
            } else {
              sum += (float)-0.017649833;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.13439026)) {
            if ( (data[13].fvalue < 0.096715316)) {
              sum += (float)-0.061249558;
            } else {
              sum += (float)0.029536579;
            }
          } else {
            if ( (data[13].fvalue < 0.10481284)) {
              sum += (float)0.075603962;
            } else {
              sum += (float)0.023372803;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.33430833)) {
          sum += (float)0.20654754;
        } else {
          if ( (data[13].fvalue < 0.30328417)) {
            if ( (data[0].fvalue < 0.87451971)) {
              sum += (float)0.052071147;
            } else {
              sum += (float)-0.056867022;
            }
          } else {
            sum += (float)0.1135701;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 2.9654303)) {
        if ( (data[14].fvalue < 0.75974131)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 5.5)) {
              sum += (float)-0.0016808443;
            } else {
              sum += (float)0.050004784;
            }
          } else {
            if ( (data[6].fvalue < 0.21677732)) {
              sum += (float)0.028129855;
            } else {
              sum += (float)-0.025937201;
            }
          }
        } else {
          if ( (data[15].fvalue < 1.4702119)) {
            if ( (data[17].fvalue < 0.076681167)) {
              sum += (float)0.013602039;
            } else {
              sum += (float)-0.024238313;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.1305439;
            } else {
              sum += (float)0.041433297;
            }
          }
        }
      } else {
        sum += (float)-0.07264439;
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
