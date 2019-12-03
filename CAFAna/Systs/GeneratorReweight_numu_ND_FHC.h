#include "BDTReweighter.h"
class GeneratorReweight_numu_ND_FHC : public BDTReweighter {
 public :
 GeneratorReweight_numu_ND_FHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_numu_ND_FHC() {;}


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
  if (  (data[7].fvalue < 5.999999)) {
    if (  (data[14].fvalue < 0.057129312)) {
      if (  (data[5].fvalue < 1.2172848)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.14997767;
            } else {
              sum += (float)0.025356177;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.070313998;
            } else {
              sum += (float)-0.17197761;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[3].fvalue < 0.32721299)) {
              sum += (float)-0.27160397;
            } else {
              sum += (float)-0.43982735;
            }
          } else {
            if (  (data[13].fvalue < 0.16096152)) {
              sum += (float)0.36699805;
            } else {
              sum += (float)-0.17371146;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.00093649235)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.031246755)) {
              sum += (float)0.30248091;
            } else {
              sum += (float)0.090613306;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.52962863;
            } else {
              sum += (float)-0.42301092;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.56486142;
            } else {
              sum += (float)-0.077838928;
            }
          } else {
            if (  (data[3].fvalue < 0.58176112)) {
              sum += (float)0.54103553;
            } else {
              sum += (float)-0.11701439;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.016661067)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.51366985;
            } else {
              sum += (float)-0.15056391;
            }
          } else {
            if (  (data[13].fvalue < 0.0030827401)) {
              sum += (float)0.3216055;
            } else {
              sum += (float)0.057167944;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.030473394;
            } else {
              sum += (float)0.27830476;
            }
          } else {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)-0.23665468;
            } else {
              sum += (float)0.24864407;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[4].fvalue < 1.0807618)) {
            if (  (data[14].fvalue < 0.33721414)) {
              sum += (float)-0.0074866256;
            } else {
              sum += (float)0.21205896;
            }
          } else {
            if (  (data[5].fvalue < 0.3160871)) {
              sum += (float)0.28877777;
            } else {
              sum += (float)0.20028253;
            }
          }
        } else {
          if (  (data[8].fvalue < 5.999999)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.11553273;
            } else {
              sum += (float)-0.060255118;
            }
          } else {
            if (  (data[3].fvalue < 0.90736192)) {
              sum += (float)0.36032882;
            } else {
              sum += (float)0.14917029;
            }
          }
        }
      }
    }
  } else {
    if (  (data[7].fvalue < 7.999999)) {
      if (  (data[13].fvalue < 0.1708945)) {
        if (  (data[3].fvalue < 0.71154231)) {
          if (  (data[8].fvalue < 4.999999)) {
            if (  (data[4].fvalue < 1.0988882)) {
              sum += (float)0.55327094;
            } else {
              sum += (float)0.25666857;
            }
          } else {
            if (  (data[14].fvalue < 0.28326705)) {
              sum += (float)0.57350874;
            } else {
              sum += (float)0.44035479;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.49449614)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.033401508;
            } else {
              sum += (float)0.42958555;
            }
          } else {
            if (  (data[8].fvalue < 7.999999)) {
              sum += (float)-0.29908353;
            } else {
              sum += (float)0.11208184;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.00011035864)) {
          if (  (data[14].fvalue < 0.1545233)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.44588301;
            } else {
              sum += (float)0.15753505;
            }
          } else {
            if (  (data[3].fvalue < 1.0768251)) {
              sum += (float)0.38657227;
            } else {
              sum += (float)0.24751805;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.4702513)) {
            if ( (data[4].fvalue < 1.3647449)) {
              sum += (float)-0.38229707;
            } else {
              sum += (float)-0.12529825;
            }
          } else {
            if (  (data[15].fvalue < 0.10988031)) {
              sum += (float)0.028461473;
            } else {
              sum += (float)0.20751174;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.00011035864)) {
        if (  (data[3].fvalue < 0.98719937)) {
          if (  (data[8].fvalue < 6.999999)) {
            if (  (data[7].fvalue < 8.999999)) {
              sum += (float)0.47874901;
            } else {
              sum += (float)0.55537212;
            }
          } else {
            if (  (data[4].fvalue < 1.7119626)) {
              sum += (float)0.5938912;
            } else {
              sum += (float)0.53545284;
            }
          }
        } else {
          if (  (data[7].fvalue < 9.999999)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.52766883;
            } else {
              sum += (float)0.36892506;
            }
          } else {
            if (  (data[7].fvalue < 10.999999)) {
              sum += (float)0.48821428;
            } else {
              sum += (float)0.5661279;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.63303083)) {
          if (  (data[7].fvalue < 10.999999)) {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.36060813;
            } else {
              sum += (float)0.10622905;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.54030091;
            } else {
              sum += (float)0.34839764;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.2967535)) {
            if (  (data[16].fvalue < 0.083921686)) {
              sum += (float)-0.35101739;
            } else {
              sum += (float)-0.14373703;
            }
          } else {
            if (  (data[7].fvalue < 8.999999)) {
              sum += (float)-0.088118881;
            } else {
              sum += (float)0.15378727;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 4.999999)) {
    if (  (data[14].fvalue < 0.13973314)) {
      if (  (data[13].fvalue < 1.4410467)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.021003332)) {
            if (  (data[17].fvalue < 0.015847785)) {
              sum += (float)-0.031837139;
            } else {
              sum += (float)-0.13279861;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.31651512;
            } else {
              sum += (float)0.093299463;
            }
          }
        } else {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[3].fvalue < 0.3612977)) {
              sum += (float)-0.11806736;
            } else {
              sum += (float)-0.26066431;
            }
          } else {
            if (  (data[13].fvalue < 0.18144064)) {
              sum += (float)0.32093111;
            } else {
              sum += (float)-0.063603088;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 6.999999)) {
          if ( (data[4].fvalue < 1.4698123)) {
            if (  (data[17].fvalue < 0.015847785)) {
              sum += (float)-0.034048092;
            } else {
              sum += (float)-0.24047217;
            }
          } else {
            if (  (data[17].fvalue < 0.72700417)) {
              sum += (float)-0.33351487;
            } else {
              sum += (float)0.013685898;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            sum += (float)0.44065702;
          } else {
            if (  (data[7].fvalue < 8.999999)) {
              sum += (float)-0.080748662;
            } else {
              sum += (float)0.33462623;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 0.32736281)) {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[14].fvalue < 1.1469393)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.12160573;
            } else {
              sum += (float)0.011194061;
            }
          } else {
            if (  (data[15].fvalue < 0.50199038)) {
              sum += (float)-0.15821035;
            } else {
              sum += (float)0.17980659;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.29854167)) {
            if (  (data[13].fvalue < 0.19182053)) {
              sum += (float)-0.052155629;
            } else {
              sum += (float)0.10890615;
            }
          } else {
            if (  (data[7].fvalue < 6.999999)) {
              sum += (float)0.26321101;
            } else {
              sum += (float)0.39100894;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.22325212)) {
              sum += (float)-0.27545348;
            } else {
              sum += (float)-0.1317832;
            }
          } else {
            if (  (data[14].fvalue < 0.30781469)) {
              sum += (float)-0.069106758;
            } else {
              sum += (float)0.16623259;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.16620027;
            } else {
              sum += (float)0.055045851;
            }
          } else {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)-0.066255853;
            } else {
              sum += (float)0.1191671;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 7.999999)) {
      if (  (data[3].fvalue < 0.6317234)) {
        if (  (data[4].fvalue < 1.1371123)) {
          if (  (data[3].fvalue < 0.4151867)) {
            if (  (data[4].fvalue < 1.0990177)) {
              sum += (float)0.49288869;
            } else {
              sum += (float)0.42862278;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.23997256;
            } else {
              sum += (float)0.44311029;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.11020821)) {
            if (  (data[7].fvalue < 6.999999)) {
              sum += (float)0.17036095;
            } else {
              sum += (float)0.34257531;
            }
          } else {
            if (  (data[4].fvalue < 1.7406275)) {
              sum += (float)0.41029701;
            } else {
              sum += (float)0.22786906;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.30781469)) {
          if (  (data[13].fvalue < 0.35814297)) {
            if (  (data[7].fvalue < 5.999999)) {
              sum += (float)0.037335027;
            } else {
              sum += (float)0.28732148;
            }
          } else {
            if (  (data[7].fvalue < 8.999999)) {
              sum += (float)-0.16822782;
            } else {
              sum += (float)0.17168449;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.28516257)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.26674095;
            } else {
              sum += (float)-0.0069638537;
            }
          } else {
            if (  (data[3].fvalue < 1.8272345)) {
              sum += (float)0.24897242;
            } else {
              sum += (float)0.11340993;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 1.5205561)) {
        if (  (data[14].fvalue < 0.25888821)) {
          if (  (data[13].fvalue < 0.55333406)) {
            if (  (data[13].fvalue < 0.3421641)) {
              sum += (float)0.46583679;
            } else {
              sum += (float)0.43229106;
            }
          } else {
            if (  (data[7].fvalue < 8.999999)) {
              sum += (float)0.18778943;
            } else {
              sum += (float)0.42358115;
            }
          }
        } else {
          if (  (data[8].fvalue < 9.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.49719775;
            } else {
              sum += (float)0.32664615;
            }
          } else {
            if (  (data[4].fvalue < 1.878581)) {
              sum += (float)0.455284;
            } else {
              sum += (float)0.38447544;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 10.999999)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 9.999999)) {
              sum += (float)0.19934201;
            } else {
              sum += (float)0.34133467;
            }
          } else {
            if (  (data[17].fvalue < 0.18922465)) {
              sum += (float)-0.14311557;
            } else {
              sum += (float)0.18782246;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.43213305;
            } else {
              sum += (float)0.29464483;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.34540015;
            } else {
              sum += (float)0.11309882;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 6.999999)) {
    if (  (data[8].fvalue < 4.999999)) {
      if (  (data[14].fvalue < 0.19837107)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[16].fvalue < 0.00012143653)) {
            if (  (data[5].fvalue < 1.336078)) {
              sum += (float)-0.038433701;
            } else {
              sum += (float)0.10390264;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.16930728;
            } else {
              sum += (float)0.071011297;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            sum += (float)0.43916529;
          } else {
            sum += (float)0.63110781;
          }
        }
      } else {
        if (  (data[13].fvalue < 0.023842676)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.078645155;
            } else {
              sum += (float)0.10960889;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.16158339;
            } else {
              sum += (float)-0.15088047;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.36535096)) {
              sum += (float)0.10609157;
            } else {
              sum += (float)0.18670949;
            }
          } else {
            if (  (data[13].fvalue < 0.31326839)) {
              sum += (float)-0.046525463;
            } else {
              sum += (float)0.10532603;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.83708608)) {
        if (  (data[4].fvalue < 1.0979815)) {
          if (  (data[3].fvalue < 0.47373575)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.3387436;
            } else {
              sum += (float)0.41883254;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.13744214;
            } else {
              sum += (float)0.3557235;
            }
          }
        } else {
          if (  (data[8].fvalue < 6.999999)) {
            if (  (data[15].fvalue < 0.15041244)) {
              sum += (float)0.094631046;
            } else {
              sum += (float)0.22347358;
            }
          } else {
            if (  (data[3].fvalue < 0.27927029)) {
              sum += (float)0.34224376;
            } else {
              sum += (float)0.24858631;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.54576063)) {
          if (  (data[15].fvalue < 0.24826165)) {
            if (  (data[14].fvalue < 0.21629746)) {
              sum += (float)-0.24588211;
            } else {
              sum += (float)-0.03823908;
            }
          } else {
            if (  (data[3].fvalue < 1.6099625)) {
              sum += (float)0.18704778;
            } else {
              sum += (float)0.010699825;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.39314547)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.18856131;
            } else {
              sum += (float)-0.007330331;
            }
          } else {
            if (  (data[17].fvalue < 0.01498397)) {
              sum += (float)0.28013265;
            } else {
              sum += (float)0.12200342;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.90890855)) {
      if (  (data[7].fvalue < 8.999999)) {
        if (  (data[4].fvalue < 1.1544807)) {
          if (  (data[3].fvalue < 0.5599075)) {
            sum += (float)0.41303885;
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.22636434;
            } else {
              sum += (float)0.39762923;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.34294131)) {
              sum += (float)0.14200093;
            } else {
              sum += (float)0.40771642;
            }
          } else {
            if (  (data[8].fvalue < 6.999999)) {
              sum += (float)0.201819;
            } else {
              sum += (float)0.33297014;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8777696)) {
          if (  (data[7].fvalue < 9.999999)) {
            if (  (data[4].fvalue < 1.36316)) {
              sum += (float)0.39803898;
            } else {
              sum += (float)0.35141531;
            }
          } else {
            sum += (float)0.40360653;
          }
        } else {
          if (  (data[7].fvalue < 9.999999)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.36913496;
            } else {
              sum += (float)0.22239341;
            }
          } else {
            if (  (data[7].fvalue < 11.999999)) {
              sum += (float)0.33108062;
            } else {
              sum += (float)0.38994628;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 8.999999)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.12523855)) {
              sum += (float)0.33857718;
            } else {
              sum += (float)0.45093107;
            }
          } else {
            if (  (data[14].fvalue < 0.046502858)) {
              sum += (float)-0.23121412;
            } else {
              sum += (float)0.28378817;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.30316502)) {
              sum += (float)0.11737287;
            } else {
              sum += (float)0.29440701;
            }
          } else {
            if (  (data[16].fvalue < 0.14399788)) {
              sum += (float)-0.20488085;
            } else {
              sum += (float)0.086979501;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.00012143653)) {
          if (  (data[7].fvalue < 11.999999)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.40339002;
            } else {
              sum += (float)0.29391453;
            }
          } else {
            sum += (float)0.39559591;
          }
        } else {
          if (  (data[16].fvalue < 0.14399788)) {
            if (  (data[15].fvalue < 0.10980193)) {
              sum += (float)-0.077185281;
            } else {
              sum += (float)0.19144119;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.30540025;
            } else {
              sum += (float)0.04720084;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 5.999999)) {
    if (  (data[5].fvalue < 0.23274937)) {
      if (  (data[14].fvalue < 0.030476868)) {
        if (  (data[13].fvalue < 0.10589658)) {
          if (  (data[17].fvalue < 0.72685146)) {
            if (  (data[15].fvalue < 0.025333395)) {
              sum += (float)0.018129988;
            } else {
              sum += (float)-0.1217233;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.34724241;
            } else {
              sum += (float)-0.097298905;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.95965207)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.058602799;
            } else {
              sum += (float)-0.28869411;
            }
          } else {
            if (  (data[17].fvalue < 0.59135062)) {
              sum += (float)-0.21285874;
            } else {
              sum += (float)0.11607762;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 0.0095176278)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.023495456;
            } else {
              sum += (float)0.15415558;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.19232847;
            } else {
              sum += (float)0.060646996;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.23204264)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.11113723;
            } else {
              sum += (float)0.088784397;
            }
          } else {
            if (  (data[13].fvalue < 0.16255157)) {
              sum += (float)-0.065594085;
            } else {
              sum += (float)0.13581042;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.015698707)) {
        if (  (data[8].fvalue < 4.999999)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 1.0345912)) {
              sum += (float)-0.039592128;
            } else {
              sum += (float)0.0079927519;
            }
          } else {
            if (  (data[6].fvalue < 0.095452517)) {
              sum += (float)0.15171589;
            } else {
              sum += (float)-0.11206579;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.1336273)) {
            if ( (data[4].fvalue < 1.1163424)) {
              sum += (float)0.33872676;
            } else {
              sum += (float)0.12656556;
            }
          } else {
            if (  (data[8].fvalue < 6.999999)) {
              sum += (float)0.045905057;
            } else {
              sum += (float)0.18146092;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.012352951)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.16733542;
            } else {
              sum += (float)-0.32713711;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.12748499;
            } else {
              sum += (float)-0.3210237;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.14430641)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.078106821;
            } else {
              sum += (float)-0.27498233;
            }
          } else {
            if (  (data[13].fvalue < 0.023697749)) {
              sum += (float)-0.10513473;
            } else {
              sum += (float)0.033623088;
            }
          }
        }
      }
    }
  } else {
    if (  (data[7].fvalue < 7.999999)) {
      if (  (data[6].fvalue < 0.19207829)) {
        if ( (data[4].fvalue < 1.1163424)) {
          if (  (data[3].fvalue < 1.2378706)) {
            if (  (data[3].fvalue < 0.39719066)) {
              sum += (float)0.38611969;
            } else {
              sum += (float)0.32847899;
            }
          } else {
            sum += (float)0.064799041;
          }
        } else {
          if (  (data[8].fvalue < 5.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.090229839;
            } else {
              sum += (float)0.31126598;
            }
          } else {
            if (  (data[3].fvalue < 0.51596576)) {
              sum += (float)0.36010239;
            } else {
              sum += (float)0.22370814;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.58391386)) {
            if (  (data[14].fvalue < 0.044757191)) {
              sum += (float)0.0085837226;
            } else {
              sum += (float)0.2692776;
            }
          } else {
            if (  (data[14].fvalue < 0.037387919)) {
              sum += (float)0.25647432;
            } else {
              sum += (float)0.36478704;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.00012361853)) {
            if (  (data[15].fvalue < 0.17410329)) {
              sum += (float)0.084862746;
            } else {
              sum += (float)0.22988084;
            }
          } else {
            if (  (data[15].fvalue < 0.12913059)) {
              sum += (float)-0.12310501;
            } else {
              sum += (float)0.084375702;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.00012361853)) {
        if (  (data[13].fvalue < 0.26096839)) {
          if (  (data[14].fvalue < 0.79530185)) {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.28985718;
            } else {
              sum += (float)0.36615396;
            }
          } else {
            sum += (float)0.098353013;
          }
        } else {
          if (  (data[7].fvalue < 9.999999)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.33746347;
            } else {
              sum += (float)0.20653732;
            }
          } else {
            if (  (data[3].fvalue < 1.7099254)) {
              sum += (float)0.34727639;
            } else {
              sum += (float)0.26953268;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.10992859)) {
          if (  (data[7].fvalue < 10.999999)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.24049971;
            } else {
              sum += (float)-0.055382531;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.26724324;
            } else {
              sum += (float)-0.03829651;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.39733616)) {
              sum += (float)0.33779234;
            } else {
              sum += (float)0.23677886;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.19838285;
            } else {
              sum += (float)-0.023288043;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 6.999999)) {
    if (  (data[13].fvalue < 1.208162)) {
      if (  (data[5].fvalue < 0.36360505)) {
        if (  (data[14].fvalue < 0.022984799)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.025747402;
            } else {
              sum += (float)-0.1500717;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.11595495;
            } else {
              sum += (float)-0.12405499;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0089658573)) {
            if (  (data[14].fvalue < 0.19412199)) {
              sum += (float)-0.10949469;
            } else {
              sum += (float)0.032443907;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.12219676;
            } else {
              sum += (float)0.022688044;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 1.5371808)) {
          if (  (data[16].fvalue < 0.00011977331)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.09594243;
            } else {
              sum += (float)-0.013454294;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.16182871;
            } else {
              sum += (float)0.06147467;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.49038312;
            } else {
              sum += (float)0.32152331;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.14159042;
            } else {
              sum += (float)0.36404699;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.91675007)) {
        if ( (data[4].fvalue < 1.6041737)) {
          if (  (data[17].fvalue < 0.012046435)) {
            if (  (data[15].fvalue < 0.025956627)) {
              sum += (float)0.031777341;
            } else {
              sum += (float)-0.099385582;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.21189234;
            } else {
              sum += (float)-0.043632589;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.39688841)) {
            if (  (data[15].fvalue < 0.9946475)) {
              sum += (float)-0.22107618;
            } else {
              sum += (float)0.049469735;
            }
          } else {
            if (  (data[14].fvalue < 0.48571998)) {
              sum += (float)0.0051664147;
            } else {
              sum += (float)0.20413636;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 1.726335)) {
          if (  (data[15].fvalue < 0.025956627)) {
            if (  (data[5].fvalue < 0.5276646)) {
              sum += (float)0.19701256;
            } else {
              sum += (float)-0.01907384;
            }
          } else {
            if (  (data[15].fvalue < 0.40781075)) {
              sum += (float)-0.10790686;
            } else {
              sum += (float)0.094876163;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.9161693)) {
            sum += (float)0.0018328071;
          } else {
            if (  (data[15].fvalue < 0.025956627)) {
              sum += (float)0.36490446;
            } else {
              sum += (float)0.15879762;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.22518955)) {
      if (  (data[8].fvalue < 5.999999)) {
        if (  (data[14].fvalue < 0.25258401)) {
          if ( (data[4].fvalue < 1.0960296)) {
            if (  (data[3].fvalue < 0.74300694)) {
              sum += (float)0.34162128;
            } else {
              sum += (float)0.087337278;
            }
          } else {
            if (  (data[15].fvalue < 0.0010326608)) {
              sum += (float)0.043859646;
            } else {
              sum += (float)0.30957368;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.63279521)) {
            sum += (float)0.002324855;
          } else {
            sum += (float)-0.27925259;
          }
        }
      } else {
        if (  (data[14].fvalue < 0.54139096)) {
          if (  (data[8].fvalue < 7.999999)) {
            if (  (data[14].fvalue < 0.23139744)) {
              sum += (float)0.33024013;
            } else {
              sum += (float)0.13375165;
            }
          } else {
            sum += (float)0.34798604;
          }
        } else {
          if (  (data[8].fvalue < 8.999999)) {
            sum += (float)-0.15382928;
          } else {
            if (  (data[3].fvalue < 1.0330559)) {
              sum += (float)0.30819449;
            } else {
              sum += (float)0.051803958;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 9.999999)) {
        if (  (data[11].fvalue < 0.99999899)) {
          if (  (data[3].fvalue < 0.37926909)) {
            if (  (data[4].fvalue < 1.2649728)) {
              sum += (float)0.31971529;
            } else {
              sum += (float)0.22476231;
            }
          } else {
            if (  (data[14].fvalue < 0.27566776)) {
              sum += (float)0.089907669;
            } else {
              sum += (float)0.1914248;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.14162752)) {
            if (  (data[5].fvalue < 0.61706227)) {
              sum += (float)-0.054765422;
            } else {
              sum += (float)-0.41833529;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.1486387;
            } else {
              sum += (float)-0.037061643;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 1.2998965)) {
              sum += (float)0.32793528;
            } else {
              sum += (float)0.26083964;
            }
          } else {
            if (  (data[3].fvalue < 1.8259989)) {
              sum += (float)0.2038154;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 0.5276646)) {
              sum += (float)0.22625518;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[8].fvalue < 6.999999)) {
              sum += (float)0.14304152;
            } else {
              sum += (float)-0.18078111;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 6.999999)) {
    if (  (data[12].fvalue < 2.999999)) {
      if (  (data[7].fvalue < 4.999999)) {
        if (  (data[7].fvalue < 2.999999)) {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.1622057)) {
              sum += (float)-0.031392768;
            } else {
              sum += (float)0.014466756;
            }
          } else {
            if (  (data[14].fvalue < 0.0059564905)) {
              sum += (float)-0.010746552;
            } else {
              sum += (float)0.10958231;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.14947815)) {
            if (  (data[6].fvalue < 0.075883895)) {
              sum += (float)0.12503971;
            } else {
              sum += (float)-0.10426325;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.18311284;
            } else {
              sum += (float)0.011613661;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.12368894)) {
          if ( (data[4].fvalue < 1.1319689)) {
            if (  (data[3].fvalue < 0.36200368)) {
              sum += (float)0.4010345;
            } else {
              sum += (float)0.27830926;
            }
          } else {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.012213498;
            } else {
              sum += (float)0.22054921;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.14947815)) {
            if (  (data[16].fvalue < 0.00011725561)) {
              sum += (float)0.060090784;
            } else {
              sum += (float)-0.093514159;
            }
          } else {
            if (  (data[17].fvalue < 0.0098641431)) {
              sum += (float)0.18630588;
            } else {
              sum += (float)0.067430355;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 0.99999899)) {
        sum += (float)0.62359858;
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          sum += (float)0.31050333;
        } else {
          sum += (float)0.47874358;
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 1.2984897)) {
      if (  (data[4].fvalue < 1.3222233)) {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[3].fvalue < 0.51637369)) {
            if (  (data[3].fvalue < 0.31178051)) {
              sum += (float)0.32041827;
            } else {
              sum += (float)0.22226952;
            }
          } else {
            if (  (data[14].fvalue < 0.43736684)) {
              sum += (float)0.057646599;
            } else {
              sum += (float)0.26922902;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.74302506)) {
            if (  (data[4].fvalue < 1.2629613)) {
              sum += (float)0.3381471;
            } else {
              sum += (float)0.30633956;
            }
          } else {
            if (  (data[4].fvalue < 1.0942624)) {
              sum += (float)0.33849448;
            } else {
              sum += (float)0.23380251;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 9.999999)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.3004052)) {
              sum += (float)0.21488068;
            } else {
              sum += (float)0.33877861;
            }
          } else {
            if (  (data[14].fvalue < 0.13348223)) {
              sum += (float)0.29116505;
            } else {
              sum += (float)0.12215634;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.9122213)) {
            if (  (data[8].fvalue < 11.999999)) {
              sum += (float)0.29647937;
            } else {
              sum += (float)0.33400097;
            }
          } else {
            if (  (data[13].fvalue < 0.39135337)) {
              sum += (float)0.29185197;
            } else {
              sum += (float)0.16460857;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 11.999999)) {
        if (  (data[7].fvalue < 2.999999)) {
          if (  (data[5].fvalue < 0.70595419)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.32900932;
            } else {
              sum += (float)0.18432748;
            }
          } else {
            if (  (data[14].fvalue < 0.79532552)) {
              sum += (float)-0.13389486;
            } else {
              sum += (float)0.16586645;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.40562317)) {
            if (  (data[14].fvalue < 0.79532552)) {
              sum += (float)-0.088658452;
            } else {
              sum += (float)0.061239205;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)0.17321005;
            } else {
              sum += (float)-0.013873655;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 2.9451458)) {
          if (  (data[7].fvalue < 12.999999)) {
            if (  (data[13].fvalue < 0.52107668)) {
              sum += (float)0.27493659;
            } else {
              sum += (float)0.14194851;
            }
          } else {
            sum += (float)0.32389438;
          }
        } else {
          if (  (data[17].fvalue < 0.044985481)) {
            sum += (float)0.19972856;
          } else {
            sum += (float)-0.039100524;
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 5.999999)) {
    if (  (data[14].fvalue < 0.27385524)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 0.044136398)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.014191343;
            } else {
              sum += (float)-0.26374808;
            }
          } else {
            if (  (data[14].fvalue < 0.0048945644)) {
              sum += (float)-0.10900851;
            } else {
              sum += (float)0.023204546;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3061023)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.049353939;
            } else {
              sum += (float)-0.074959695;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.13419649;
            } else {
              sum += (float)0.029314505;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.5396775)) {
          if (  (data[15].fvalue < 0.12842003)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.089679614;
            } else {
              sum += (float)0.074623361;
            }
          } else {
            if (  (data[13].fvalue < 0.29934499)) {
              sum += (float)0.15734202;
            } else {
              sum += (float)-0.032584511;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.085488319)) {
            if (  (data[15].fvalue < 0.12842003)) {
              sum += (float)-0.29167253;
            } else {
              sum += (float)-0.14279766;
            }
          } else {
            if (  (data[5].fvalue < 0.46952468)) {
              sum += (float)-0.029441634;
            } else {
              sum += (float)-0.1312632;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.39229697)) {
        if (  (data[14].fvalue < 1.1420002)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.0086144926)) {
              sum += (float)0.023634616;
            } else {
              sum += (float)0.11421425;
            }
          } else {
            if (  (data[13].fvalue < 0.062047642)) {
              sum += (float)-0.26814818;
            } else {
              sum += (float)0.014644591;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.63639939)) {
            if ( (data[4].fvalue < 1.3061023)) {
              sum += (float)0.058617141;
            } else {
              sum += (float)-0.14851134;
            }
          } else {
            if (  (data[17].fvalue < 0.011870918)) {
              sum += (float)0.18482992;
            } else {
              sum += (float)-0.0018095408;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.43647268)) {
          if (  (data[3].fvalue < 1.5213182)) {
            if (  (data[4].fvalue < 1.8325977)) {
              sum += (float)0.16038524;
            } else {
              sum += (float)0.027395619;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.092798367;
            } else {
              sum += (float)-0.03745874;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.011870918)) {
            if (  (data[13].fvalue < 0.4964509)) {
              sum += (float)0.1666446;
            } else {
              sum += (float)0.27377316;
            }
          } else {
            if (  (data[5].fvalue < 0.64032519)) {
              sum += (float)0.12676047;
            } else {
              sum += (float)-0.075997062;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.47600871)) {
      if (  (data[4].fvalue < 1.2626905)) {
        if (  (data[7].fvalue < 3.999999)) {
          if (  (data[4].fvalue < 1.1315701)) {
            if (  (data[3].fvalue < 0.34581053)) {
              sum += (float)0.32140273;
            } else {
              sum += (float)0.19002965;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.060380552;
            } else {
              sum += (float)0.36686927;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1968741)) {
            sum += (float)0.32906961;
          } else {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)0.15233546;
            } else {
              sum += (float)0.30741745;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 8.999999)) {
          if (  (data[4].fvalue < 1.912244)) {
            if (  (data[15].fvalue < 0.17322388)) {
              sum += (float)0.12880352;
            } else {
              sum += (float)0.27350608;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.21194945;
            } else {
              sum += (float)-0.021141114;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.912244)) {
            sum += (float)0.30825591;
          } else {
            if (  (data[14].fvalue < 0.3590056)) {
              sum += (float)0.26786977;
            } else {
              sum += (float)0.10707994;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 8.999999)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.23713154)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.32624686;
            } else {
              sum += (float)0.14143467;
            }
          } else {
            sum += (float)0.34066322;
          }
        } else {
          if (  (data[3].fvalue < 1.5213182)) {
            if ( (data[4].fvalue < 0.96923494)) {
              sum += (float)0.28428134;
            } else {
              sum += (float)0.042829882;
            }
          } else {
            if (  (data[5].fvalue < 0.62016612)) {
              sum += (float)0.01077732;
            } else {
              sum += (float)-0.16080607;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 2.1189046)) {
          if (  (data[8].fvalue < 11.999999)) {
            if (  (data[14].fvalue < 0.29930288)) {
              sum += (float)0.26777482;
            } else {
              sum += (float)0.1501721;
            }
          } else {
            if ( (data[4].fvalue < 2.0649953)) {
              sum += (float)0.31515312;
            } else {
              sum += (float)0.22074874;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 13.999999)) {
              sum += (float)0.041063204;
            } else {
              sum += (float)0.24108262;
            }
          } else {
            if (  (data[17].fvalue < 0.9179244)) {
              sum += (float)-0.31737164;
            } else {
              sum += (float)0.0762624;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 7.999999)) {
    if (  (data[5].fvalue < 0.13957335)) {
      if (  (data[13].fvalue < 1.054369)) {
        if (  (data[4].fvalue < 1.2487134)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[15].fvalue < 0.12785132)) {
              sum += (float)-0.035153463;
            } else {
              sum += (float)0.093711302;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.10837125;
            } else {
              sum += (float)0.41797173;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7633935)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.084691793;
            } else {
              sum += (float)0.3813439;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.020567004;
            } else {
              sum += (float)0.43258867;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.48871461)) {
          if (  (data[16].fvalue < 0.50784296)) {
            if (  (data[4].fvalue < 1.9116518)) {
              sum += (float)0.044411302;
            } else {
              sum += (float)-0.20232883;
            }
          } else {
            if (  (data[16].fvalue < 0.85846478)) {
              sum += (float)0.056402102;
            } else {
              sum += (float)0.25808176;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.2085447)) {
            if (  (data[4].fvalue < 2.2066295)) {
              sum += (float)0.19784236;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[13].fvalue < 1.7971228)) {
              sum += (float)0.22734454;
            } else {
              sum += (float)0.047892172;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.047112945)) {
        if (  (data[14].fvalue < 0.32497072)) {
          if (  (data[15].fvalue < 0.84379405)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.0069958833;
            } else {
              sum += (float)-0.048645362;
            }
          } else {
            if (  (data[13].fvalue < 0.16238075)) {
              sum += (float)-0.22182527;
            } else {
              sum += (float)0.035854742;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.35811365)) {
            if (  (data[15].fvalue < 0.36702505)) {
              sum += (float)-0.0029229287;
            } else {
              sum += (float)0.081529185;
            }
          } else {
            if (  (data[14].fvalue < 0.53779525)) {
              sum += (float)0.088322632;
            } else {
              sum += (float)0.17593239;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.14231625)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.054369)) {
              sum += (float)-0.094711788;
            } else {
              sum += (float)-0.21055871;
            }
          } else {
            if (  (data[5].fvalue < 0.56538641)) {
              sum += (float)-0.15551493;
            } else {
              sum += (float)-0.34411144;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.0091597373)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.10145722;
            } else {
              sum += (float)0.063697144;
            }
          } else {
            if (  (data[13].fvalue < 0.029739093)) {
              sum += (float)-0.061363876;
            } else {
              sum += (float)0.041428991;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.80683964)) {
      if (  (data[13].fvalue < 0.19291621)) {
        if (  (data[14].fvalue < 0.48166907)) {
          sum += (float)0.30928031;
        } else {
          sum += (float)0.18451387;
        }
      } else {
        if (  (data[7].fvalue < 8.999999)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.34232736)) {
              sum += (float)-0.023375357;
            } else {
              sum += (float)0.25286943;
            }
          } else {
            if (  (data[4].fvalue < 1.2900219)) {
              sum += (float)0.24461539;
            } else {
              sum += (float)0.051369362;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 10.999999)) {
              sum += (float)0.22089584;
            } else {
              sum += (float)0.2949973;
            }
          } else {
            sum += (float)-0.053894825;
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 11.999999)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 0.79811877)) {
            if (  (data[4].fvalue < 1.4601591)) {
              sum += (float)-0.0436428;
            } else {
              sum += (float)0.19141628;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.32707849;
            } else {
              sum += (float)0.22069612;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 8.999999)) {
              sum += (float)0.017995168;
            } else {
              sum += (float)0.119722;
            }
          } else {
            if (  (data[16].fvalue < 0.1336994)) {
              sum += (float)-0.24846618;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 13.999999)) {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.25248525;
            } else {
              sum += (float)0.074701302;
            }
          } else {
            sum += (float)0.30946565;
          }
        } else {
          sum += (float)0.11023773;
        }
      }
    }
  }
  if (  (data[8].fvalue < 7.999999)) {
    if (  (data[12].fvalue < 2.999999)) {
      if (  (data[13].fvalue < 1.7878599)) {
        if (  (data[14].fvalue < 0.032738503)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 1.1383147)) {
              sum += (float)-0.016984746;
            } else {
              sum += (float)0.058361121;
            }
          } else {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.198331;
            } else {
              sum += (float)0.20624344;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.037140433)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.015945667;
            } else {
              sum += (float)-0.16965336;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.079206854;
            } else {
              sum += (float)0.0095811896;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 5.999999)) {
          if (  (data[17].fvalue < 0.48466554)) {
            if ( (data[4].fvalue < 1.793969)) {
              sum += (float)-0.069237016;
            } else {
              sum += (float)-0.23287566;
            }
          } else {
            if (  (data[15].fvalue < 0.042994745)) {
              sum += (float)0.043675184;
            } else {
              sum += (float)-0.082908906;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.33123448;
            } else {
              sum += (float)0.13364246;
            }
          } else {
            if (  (data[14].fvalue < 0.22741343)) {
              sum += (float)-0.26355752;
            } else {
              sum += (float)0.12444961;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 3.999999)) {
        sum += (float)0.26462251;
      } else {
        sum += (float)0.40484491;
      }
    }
  } else {
    if (  (data[14].fvalue < 0.24834599)) {
      if (  (data[13].fvalue < 0.34100154)) {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 0.15193398)) {
            if (  (data[14].fvalue < 0.158557)) {
              sum += (float)0.31418902;
            } else {
              sum += (float)0.19220293;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if (  (data[13].fvalue < 0.14223532)) {
            sum += (float)0.31672347;
          } else {
            if (  (data[7].fvalue < 5.999999)) {
              sum += (float)0.1278419;
            } else {
              sum += (float)0.29553974;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 1.0310453)) {
          if (  (data[4].fvalue < 1.3730074)) {
            sum += (float)0.29830152;
          } else {
            if (  (data[7].fvalue < 10.999999)) {
              sum += (float)-0.045758571;
            } else {
              sum += (float)0.26113334;
            }
          }
        } else {
          if (  (data[7].fvalue < 9.999999)) {
            sum += (float)-0.28302327;
          } else {
            sum += (float)0.058739174;
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.00012143653)) {
        if (  (data[3].fvalue < 1.3649348)) {
          if (  (data[4].fvalue < 1.8292582)) {
            if (  (data[8].fvalue < 9.999999)) {
              sum += (float)0.16442414;
            } else {
              sum += (float)0.26666218;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.24667452;
            } else {
              sum += (float)0.055615406;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.1724568)) {
              sum += (float)0.047427427;
            } else {
              sum += (float)0.17204359;
            }
          } else {
            if (  (data[15].fvalue < 0.44946742)) {
              sum += (float)-0.36579296;
            } else {
              sum += (float)0.037784208;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.13333407)) {
          if (  (data[3].fvalue < 2.3148661)) {
            if (  (data[14].fvalue < 0.79161358)) {
              sum += (float)-0.17900506;
            } else {
              sum += (float)0.013709437;
            }
          } else {
            if (  (data[15].fvalue < 0.50033569)) {
              sum += (float)-0.37010473;
            } else {
              sum += (float)-0.21111093;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 3.615206)) {
              sum += (float)0.13586903;
            } else {
              sum += (float)-0.060990542;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.032439966;
            } else {
              sum += (float)-0.25574014;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 4.999999)) {
    if (  (data[7].fvalue < 2.999999)) {
      if (  (data[4].fvalue < 1.0575577)) {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.47148144;
            } else {
              sum += (float)0.053283196;
            }
          } else {
            if (  (data[14].fvalue < 0.074342899)) {
              sum += (float)-0.10331567;
            } else {
              sum += (float)0.22772004;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0076979748)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.23781328;
            } else {
              sum += (float)-0.29718995;
            }
          } else {
            if (  (data[14].fvalue < 0.0037479014)) {
              sum += (float)-0.0070769363;
            } else {
              sum += (float)-0.090451002;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.98840928)) {
          if (  (data[13].fvalue < 0.11395478)) {
            if (  (data[14].fvalue < 0.074342899)) {
              sum += (float)-0.069793485;
            } else {
              sum += (float)0.031201014;
            }
          } else {
            if (  (data[13].fvalue < 0.79233599)) {
              sum += (float)0.06812714;
            } else {
              sum += (float)-0.071249403;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.025791492;
            } else {
              sum += (float)-0.19699499;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.07130152;
            } else {
              sum += (float)-0.033839524;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[8].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 0.061085276)) {
            if (  (data[13].fvalue < 0.029087944)) {
              sum += (float)-0.49179471;
            } else {
              sum += (float)-0.2169991;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.017340725;
            } else {
              sum += (float)0.11020324;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.013925886)) {
            if (  (data[6].fvalue < 0.036576688)) {
              sum += (float)0.28443894;
            } else {
              sum += (float)-0.27344763;
            }
          } else {
            if (  (data[6].fvalue < 0.056222621)) {
              sum += (float)0.33233204;
            } else {
              sum += (float)-0.073748857;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.15136638)) {
          if ( (data[4].fvalue < 1.1094809)) {
            if (  (data[3].fvalue < 0.41646445)) {
              sum += (float)0.41451067;
            } else {
              sum += (float)0.20903395;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.016021252;
            } else {
              sum += (float)0.33003068;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.1797718)) {
            if (  (data[4].fvalue < 1.0575577)) {
              sum += (float)0.21489556;
            } else {
              sum += (float)0.021292768;
            }
          } else {
            if (  (data[14].fvalue < 0.29672801)) {
              sum += (float)-0.16500618;
            } else {
              sum += (float)-0.026627997;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.15136638)) {
      if ( (data[4].fvalue < 1.1283106)) {
        if (  (data[0].fvalue < 2.9680226)) {
          if (  (data[7].fvalue < 5.999999)) {
            if (  (data[0].fvalue < 2.0648737)) {
              sum += (float)0.43655893;
            } else {
              sum += (float)0.35291994;
            }
          } else {
            sum += (float)0.32382789;
          }
        } else {
          if (  (data[6].fvalue < 0.075495176)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.22661924;
            } else {
              sum += (float)0.33828729;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.021154627;
            } else {
              sum += (float)0.23830116;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 4.999999)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.31153008)) {
              sum += (float)-0.097178057;
            } else {
              sum += (float)0.10999217;
            }
          } else {
            if (  (data[4].fvalue < 1.2879084)) {
              sum += (float)0.36908203;
            } else {
              sum += (float)0.066077352;
            }
          }
        } else {
          if (  (data[8].fvalue < 5.999999)) {
            if (  (data[3].fvalue < 0.28035492)) {
              sum += (float)0.19179349;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[3].fvalue < 0.31222159)) {
              sum += (float)0.28990826;
            } else {
              sum += (float)0.17654745;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 8.999999)) {
        if (  (data[3].fvalue < 1.2367082)) {
          if (  (data[4].fvalue < 1.0908196)) {
            if (  (data[3].fvalue < 0.60784161)) {
              sum += (float)0.33438021;
            } else {
              sum += (float)0.14602959;
            }
          } else {
            if (  (data[15].fvalue < 0.1275858)) {
              sum += (float)0.01357032;
            } else {
              sum += (float)0.13399254;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if ( (data[4].fvalue < 1.3354285)) {
              sum += (float)-0.050466578;
            } else {
              sum += (float)0.12195735;
            }
          } else {
            if (  (data[5].fvalue < 0.54973459)) {
              sum += (float)-0.016918559;
            } else {
              sum += (float)-0.13620606;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 11.999999)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.47017738)) {
              sum += (float)0.10920984;
            } else {
              sum += (float)0.30299821;
            }
          } else {
            if (  (data[13].fvalue < 0.34085214)) {
              sum += (float)0.22649142;
            } else {
              sum += (float)0.046809755;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.48127487)) {
            if (  (data[13].fvalue < 0.49367404)) {
              sum += (float)0.30791435;
            } else {
              sum += (float)0.25129211;
            }
          } else {
            if (  (data[17].fvalue < 0.0084189409)) {
              sum += (float)0.24192369;
            } else {
              sum += (float)0.085291475;
            }
          }
        }
      }
    }
  }
  if (  (data[16].fvalue < 0.00013045447)) {
    if (  (data[8].fvalue < 4.999999)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 0.17320599)) {
              sum += (float)0.030889587;
            } else {
              sum += (float)-0.0040718825;
            }
          } else {
            if ( (data[4].fvalue < 1.5720959)) {
              sum += (float)0.55082172;
            } else {
              sum += (float)-0.070190482;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.47635776)) {
            if (  (data[4].fvalue < 1.7288349)) {
              sum += (float)0.091564253;
            } else {
              sum += (float)-0.17990345;
            }
          } else {
            if ( (data[4].fvalue < 1.6204982)) {
              sum += (float)-0.12689324;
            } else {
              sum += (float)-0.29161629;
            }
          }
        }
      } else {
        sum += (float)0.36797273;
      }
    } else {
      if (  (data[6].fvalue < 0.2007087)) {
        if ( (data[4].fvalue < 1.1291536)) {
          if (  (data[3].fvalue < 0.32971203)) {
            if (  (data[3].fvalue < 0.13278225)) {
              sum += (float)0.33416131;
            } else {
              sum += (float)0.29365921;
            }
          } else {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)0.10998391;
            } else {
              sum += (float)0.26647699;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.27163979)) {
              sum += (float)0.0018965057;
            } else {
              sum += (float)0.17739379;
            }
          } else {
            if (  (data[0].fvalue < 5.2039194)) {
              sum += (float)0.28539622;
            } else {
              sum += (float)0.094156377;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 0.024900595)) {
            if (  (data[14].fvalue < 0.43449309)) {
              sum += (float)0.14813241;
            } else {
              sum += (float)0.39821142;
            }
          } else {
            if (  (data[5].fvalue < 0.622917)) {
              sum += (float)0.24891011;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[8].fvalue < 9.999999)) {
            if (  (data[17].fvalue < 0.72569776)) {
              sum += (float)0.021755418;
            } else {
              sum += (float)0.18300167;
            }
          } else {
            if (  (data[14].fvalue < 0.29702669)) {
              sum += (float)0.27296057;
            } else {
              sum += (float)0.14721254;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 1.999999)) {
      if ( (data[4].fvalue < 1.4344263)) {
        if (  (data[16].fvalue < 0.13222089)) {
          if (  (data[13].fvalue < 0.3730104)) {
            if (  (data[15].fvalue < 0.092178032)) {
              sum += (float)-0.3489002;
            } else {
              sum += (float)-0.17796636;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.040189996;
            } else {
              sum += (float)-0.26809973;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.45044407;
            } else {
              sum += (float)-0.14715293;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.12628327;
            } else {
              sum += (float)-0.14733912;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.0077199889)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 2.1256375)) {
              sum += (float)-0.18722904;
            } else {
              sum += (float)-0.043887824;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.085090041;
            } else {
              sum += (float)-0.24314374;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.13222089)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.0097697601;
            } else {
              sum += (float)-0.15757236;
            }
          } else {
            if (  (data[17].fvalue < 0.0094669107)) {
              sum += (float)0.087365575;
            } else {
              sum += (float)-0.0085627884;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.8279247)) {
        if (  (data[14].fvalue < 0.025221534)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 0.140175)) {
              sum += (float)0.65510726;
            } else {
              sum += (float)0.81908166;
            }
          } else {
            sum += (float)0.50169396;
          }
        } else {
          sum += (float)0.48462406;
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[3].fvalue < 0.60968989)) {
            if (  (data[13].fvalue < 0.27251494)) {
              sum += (float)0.33127642;
            } else {
              sum += (float)0.51882118;
            }
          } else {
            if (  (data[13].fvalue < 0.42774838)) {
              sum += (float)0.047288459;
            } else {
              sum += (float)0.34284198;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.4328897)) {
            sum += (float)-0.30881876;
          } else {
            sum += (float)0.059133645;
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 0.04547397)) {
    if (  (data[14].fvalue < 0.2487476)) {
      if (  (data[15].fvalue < 2.0125463)) {
        if (  (data[5].fvalue < 1.3365645)) {
          if (  (data[4].fvalue < 1.0311819)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.035169244;
            } else {
              sum += (float)0.16860552;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.025138363;
            } else {
              sum += (float)-0.042277001;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.38456729;
            } else {
              sum += (float)0.060674839;
            }
          } else {
            if (  (data[3].fvalue < 0.98882633)) {
              sum += (float)0.29844716;
            } else {
              sum += (float)0.060073014;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.23667143)) {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.20497276;
            } else {
              sum += (float)-0.3528851;
            }
          } else {
            if (  (data[4].fvalue < 2.6549335)) {
              sum += (float)0.032838371;
            } else {
              sum += (float)-0.16178794;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.57673192)) {
            if (  (data[5].fvalue < 0.40480116)) {
              sum += (float)0.071559064;
            } else {
              sum += (float)-0.14261433;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.20191123;
            } else {
              sum += (float)-0.073077589;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.47050044)) {
        if (  (data[15].fvalue < 0.30176917)) {
          if ( (data[4].fvalue < 1.4335012)) {
            if (  (data[16].fvalue < 0.00013230079)) {
              sum += (float)0.043019816;
            } else {
              sum += (float)-0.16883639;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.13933842;
            } else {
              sum += (float)-0.0080931019;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.47223696)) {
            if (  (data[15].fvalue < 0.84287995)) {
              sum += (float)0.081994675;
            } else {
              sum += (float)0.13417165;
            }
          } else {
            if (  (data[13].fvalue < 0.029363135)) {
              sum += (float)-0.0450029;
            } else {
              sum += (float)0.056147449;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.024865227)) {
          if (  (data[14].fvalue < 0.53872895)) {
            if (  (data[3].fvalue < 1.7077603)) {
              sum += (float)0.13461876;
            } else {
              sum += (float)0.011733068;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.24974284;
            } else {
              sum += (float)0.099166997;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.60641176)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.12794481;
            } else {
              sum += (float)-0.02446761;
            }
          } else {
            if ( (data[4].fvalue < 1.057933)) {
              sum += (float)-0.17830366;
            } else {
              sum += (float)0.12429244;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 0.18525134)) {
      if (  (data[13].fvalue < 0.93739712)) {
        if (  (data[13].fvalue < 0.02182267)) {
          if (  (data[15].fvalue < 0.99431294)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.19661106;
            } else {
              sum += (float)0.060810659;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.0056026429;
            } else {
              sum += (float)-0.2213587;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.0077727591)) {
              sum += (float)-0.06422393;
            } else {
              sum += (float)-0.00048149243;
            }
          } else {
            if (  (data[5].fvalue < 0.68705273)) {
              sum += (float)-0.10063421;
            } else {
              sum += (float)-0.32049337;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.0029174669)) {
            if (  (data[15].fvalue < 1.1944302)) {
              sum += (float)-0.26542771;
            } else {
              sum += (float)0.019389808;
            }
          } else {
            if ( (data[4].fvalue < 1.8653271)) {
              sum += (float)0.057446759;
            } else {
              sum += (float)-0.14984103;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.062106017;
            } else {
              sum += (float)-0.081295334;
            }
          } else {
            if (  (data[14].fvalue < 0.35682496)) {
              sum += (float)-0.16934809;
            } else {
              sum += (float)0.016722562;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 0.45800707)) {
        if (  (data[13].fvalue < 0.15258546)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.0077727591)) {
              sum += (float)-0.14196669;
            } else {
              sum += (float)-0.044535153;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.060107131;
            } else {
              sum += (float)0.21203412;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.9182722)) {
            if (  (data[4].fvalue < 1.698823)) {
              sum += (float)0.071946353;
            } else {
              sum += (float)-0.030568451;
            }
          } else {
            if (  (data[13].fvalue < 0.37325388)) {
              sum += (float)0.025352126;
            } else {
              sum += (float)0.18319671;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 2.999999)) {
          if (  (data[16].fvalue < 0.00013230079)) {
            if (  (data[13].fvalue < 0.39045361)) {
              sum += (float)-0.065370977;
            } else {
              sum += (float)-0.016912298;
            }
          } else {
            if (  (data[16].fvalue < 0.12766758)) {
              sum += (float)-0.20350187;
            } else {
              sum += (float)-0.087007925;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.2062664)) {
            if (  (data[15].fvalue < 0.043906093)) {
              sum += (float)-0.35300374;
            } else {
              sum += (float)-0.1872531;
            }
          } else {
            if (  (data[4].fvalue < 1.8653271)) {
              sum += (float)-0.14566046;
            } else {
              sum += (float)0.026223829;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 2.999999)) {
    if (  (data[13].fvalue < 1.7496111)) {
      if (  (data[5].fvalue < 0.064140119)) {
        if (  (data[4].fvalue < 1.8249311)) {
          if (  (data[4].fvalue < 1.4319881)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.015323889;
            } else {
              sum += (float)0.071299925;
            }
          } else {
            if (  (data[5].fvalue < 0.029220195)) {
              sum += (float)0.19664292;
            } else {
              sum += (float)0.11366575;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.043796796)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.017557004;
            } else {
              sum += (float)0.21815215;
            }
          } else {
            if (  (data[17].fvalue < 0.58814204)) {
              sum += (float)-0.09417019;
            } else {
              sum += (float)0.003890103;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 6.999999)) {
          if (  (data[5].fvalue < 1.13708)) {
            if (  (data[4].fvalue < 2.1227016)) {
              sum += (float)-0.0092550572;
            } else {
              sum += (float)0.035542063;
            }
          } else {
            if (  (data[3].fvalue < 0.088209093)) {
              sum += (float)-0.46095744;
            } else {
              sum += (float)0.038937241;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.38019368)) {
            if (  (data[4].fvalue < 1.1638186)) {
              sum += (float)0.2972599;
            } else {
              sum += (float)0.14497459;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.16719271;
            } else {
              sum += (float)0.014186492;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.85327441)) {
        if (  (data[15].fvalue < 0.025110144)) {
          if (  (data[17].fvalue < 0.40660086)) {
            if ( (data[4].fvalue < 2.0000348)) {
              sum += (float)-0.01617492;
            } else {
              sum += (float)-0.16912535;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.11198742;
            } else {
              sum += (float)-0.098980181;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.49960119)) {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.23024417;
            } else {
              sum += (float)-0.055811394;
            }
          } else {
            if (  (data[15].fvalue < 1.9948802)) {
              sum += (float)-0.058627333;
            } else {
              sum += (float)0.19821966;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.47235873)) {
          if (  (data[3].fvalue < 1.2349588)) {
            sum += (float)0.11592998;
          } else {
            if (  (data[15].fvalue < 0.6340853)) {
              sum += (float)0.21342428;
            } else {
              sum += (float)0.32726777;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            sum += (float)-0;
          } else {
            sum += (float)0.10612398;
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      sum += (float)0.19444783;
    } else {
      sum += (float)0.34583777;
    }
  }
  if (  (data[16].fvalue < 0.024908822)) {
    if (  (data[7].fvalue < 7.999999)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[5].fvalue < 0.4177894)) {
            if (  (data[13].fvalue < 1.3673959)) {
              sum += (float)0.015422096;
            } else {
              sum += (float)-0.075322643;
            }
          } else {
            if (  (data[15].fvalue < 0.025439011)) {
              sum += (float)0.0046583042;
            } else {
              sum += (float)-0.040414751;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.5214645)) {
            if ( (data[4].fvalue < 1.3509684)) {
              sum += (float)0.48081595;
            } else {
              sum += (float)0.27156055;
            }
          } else {
            if (  (data[15].fvalue < 0.14974262)) {
              sum += (float)-0.19729295;
            } else {
              sum += (float)0.2041395;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[4].fvalue < 1.6682292)) {
            if (  (data[14].fvalue < 0.012626503)) {
              sum += (float)-0.088064313;
            } else {
              sum += (float)0.19466084;
            }
          } else {
            if (  (data[4].fvalue < 2.4287155)) {
              sum += (float)-0.24948668;
            } else {
              sum += (float)0.035687756;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            sum += (float)0.19005229;
          } else {
            sum += (float)0.3355341;
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 10.999999)) {
        if (  (data[6].fvalue < 0.33515856)) {
          if (  (data[0].fvalue < 2.5631108)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.16737968;
            } else {
              sum += (float)0.27560693;
            }
          } else {
            if (  (data[6].fvalue < 0.16142857)) {
              sum += (float)0.22171412;
            } else {
              sum += (float)0.078855671;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.60681713)) {
              sum += (float)0.083393753;
            } else {
              sum += (float)0.25038865;
            }
          } else {
            if (  (data[15].fvalue < 0.36649361)) {
              sum += (float)-0.0061144829;
            } else {
              sum += (float)0.15513203;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 1.3608738)) {
          if (  (data[4].fvalue < 1.7558018)) {
            sum += (float)0.28645748;
          } else {
            if (  (data[7].fvalue < 11.999999)) {
              sum += (float)0.071535155;
            } else {
              sum += (float)0.24409945;
            }
          }
        } else {
          if (  (data[8].fvalue < 5.999999)) {
            sum += (float)0.2560944;
          } else {
            if (  (data[17].fvalue < 0.0084958216)) {
              sum += (float)0.16050987;
            } else {
              sum += (float)-0.0038375785;
            }
          }
        }
      }
    }
  } else {
    if (  (data[9].fvalue < 0.99999899)) {
      if (  (data[16].fvalue < 0.12680653)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 0.99999899)) {
            sum += (float)0.11668721;
          } else {
            if (  (data[4].fvalue < 1.9030589)) {
              sum += (float)-0.42468604;
            } else {
              sum += (float)-0.2849527;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.022178989;
            } else {
              sum += (float)-0.18722986;
            }
          } else {
            if (  (data[13].fvalue < 0.60681713)) {
              sum += (float)-0.27462304;
            } else {
              sum += (float)-0.14644691;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)1.000766;
            } else {
              sum += (float)0.047288295;
            }
          } else {
            if (  (data[4].fvalue < 2.1214108)) {
              sum += (float)-0.3711642;
            } else {
              sum += (float)-0.099171005;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.07213676;
            } else {
              sum += (float)-0.054942366;
            }
          } else {
            if ( (data[4].fvalue < 2.1214108)) {
              sum += (float)-0.41564634;
            } else {
              sum += (float)-0.23966023;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.0025252451)) {
            if (  (data[15].fvalue < 0.63473558)) {
              sum += (float)-0.14141652;
            } else {
              sum += (float)-0.031713258;
            }
          } else {
            if (  (data[13].fvalue < 0.037116371)) {
              sum += (float)-0.04917033;
            } else {
              sum += (float)0.063866518;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.045045327)) {
              sum += (float)-0.18328738;
            } else {
              sum += (float)0.07283324;
            }
          } else {
            if (  (data[16].fvalue < 0.12680653)) {
              sum += (float)-0.10536077;
            } else {
              sum += (float)0.0093336599;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8618798)) {
          if (  (data[4].fvalue < 1.7249188)) {
            sum += (float)0.49466357;
          } else {
            sum += (float)0.38383585;
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.0084958216)) {
              sum += (float)0.2583248;
            } else {
              sum += (float)-0.052950218;
            }
          } else {
            if (  (data[13].fvalue < 0.60681713)) {
              sum += (float)-0.19061272;
            } else {
              sum += (float)0.071764812;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.10491022)) {
    if (  (data[8].fvalue < 1.999999)) {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[15].fvalue < 0.043442145)) {
          if (  (data[13].fvalue < 0.0077233044)) {
            if (  (data[4].fvalue < 1.566506)) {
              sum += (float)0.037730634;
            } else {
              sum += (float)0.51269066;
            }
          } else {
            if (  (data[4].fvalue < 1.0568366)) {
              sum += (float)-0.015769336;
            } else {
              sum += (float)0.038914714;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.029178759)) {
            if (  (data[5].fvalue < 0.16271955)) {
              sum += (float)-0.078994617;
            } else {
              sum += (float)-0.21092166;
            }
          } else {
            if (  (data[14].fvalue < 0.00755713)) {
              sum += (float)-0.033709478;
            } else {
              sum += (float)0.129159;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 0.087217569)) {
              sum += (float)-0.1718722;
            } else {
              sum += (float)0.0030127112;
            }
          } else {
            if (  (data[13].fvalue < 0.54291415)) {
              sum += (float)0.011470919;
            } else {
              sum += (float)0.24221632;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.012859875)) {
            if (  (data[3].fvalue < 0.23443998)) {
              sum += (float)-0.16961047;
            } else {
              sum += (float)-0.28419602;
            }
          } else {
            if (  (data[7].fvalue < 5.999999)) {
              sum += (float)-0.091770612;
            } else {
              sum += (float)0.11246664;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.21359959)) {
        if (  (data[7].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 0.024884131)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.25430593;
            } else {
              sum += (float)-0.023358259;
            }
          } else {
            if (  (data[3].fvalue < 0.20482685)) {
              sum += (float)0.026043693;
            } else {
              sum += (float)-0.11026496;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.12358314)) {
            if (  (data[3].fvalue < 0.23443998)) {
              sum += (float)0.29898813;
            } else {
              sum += (float)0.09889897;
            }
          } else {
            if ( (data[4].fvalue < 1.126757)) {
              sum += (float)0.16744801;
            } else {
              sum += (float)-0.10149709;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.18775165;
            } else {
              sum += (float)0.012092205;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.21068616;
            } else {
              sum += (float)-0.29950449;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.17028809)) {
            if (  (data[4].fvalue < 1.1625551)) {
              sum += (float)0.11414506;
            } else {
              sum += (float)-0.13572265;
            }
          } else {
            if (  (data[14].fvalue < 0.04655131)) {
              sum += (float)-0.2437336;
            } else {
              sum += (float)-0.14817178;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.45467219)) {
      if (  (data[4].fvalue < 1.1453537)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.40915513;
            } else {
              sum += (float)0.13382183;
            }
          } else {
            if (  (data[5].fvalue < 0.4184638)) {
              sum += (float)0.24736138;
            } else {
              sum += (float)0.049781926;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.044812281)) {
            if (  (data[3].fvalue < 0.21948282)) {
              sum += (float)0.33763602;
            } else {
              sum += (float)0.11044889;
            }
          } else {
            if (  (data[4].fvalue < 1.0895423)) {
              sum += (float)0.43108457;
            } else {
              sum += (float)0.23172493;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 0.25939175)) {
            if (  (data[14].fvalue < 0.32401243)) {
              sum += (float)-0.079290941;
            } else {
              sum += (float)0.023810064;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.024719747;
            } else {
              sum += (float)0.11681401;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.10308149)) {
            if (  (data[4].fvalue < 1.7227137)) {
              sum += (float)0.20385613;
            } else {
              sum += (float)0.050226945;
            }
          } else {
            if (  (data[17].fvalue < 0.043838792)) {
              sum += (float)0.064270385;
            } else {
              sum += (float)-0.024318878;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.91661632)) {
        if (  (data[17].fvalue < 0.0083308937)) {
          if (  (data[13].fvalue < 0.24747276)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.010637911;
            } else {
              sum += (float)-0.063246913;
            }
          } else {
            if (  (data[14].fvalue < 0.39175367)) {
              sum += (float)0.017643915;
            } else {
              sum += (float)0.076153524;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.44888103)) {
            if (  (data[13].fvalue < 0.0077233044)) {
              sum += (float)-0.16068764;
            } else {
              sum += (float)-0.035076246;
            }
          } else {
            if (  (data[15].fvalue < 0.98972744)) {
              sum += (float)-0.0061187968;
            } else {
              sum += (float)0.046225697;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.29603726)) {
          if (  (data[13].fvalue < 0.0077233044)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.12943558;
            } else {
              sum += (float)0.085269272;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)0.07787326;
            } else {
              sum += (float)-0.089234933;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.017572669)) {
              sum += (float)0.16357979;
            } else {
              sum += (float)0.030151388;
            }
          } else {
            if (  (data[16].fvalue < 0.017572669)) {
              sum += (float)0.067404613;
            } else {
              sum += (float)-0.052732754;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 0.04497071)) {
    if (  (data[15].fvalue < 0.17075258)) {
      if (  (data[15].fvalue < 0.041862756)) {
        if (  (data[13].fvalue < 0.0078835208)) {
          if (  (data[14].fvalue < 0.0020100581)) {
            if (  (data[4].fvalue < 1.2846009)) {
              sum += (float)0.19797121;
            } else {
              sum += (float)0.50635231;
            }
          } else {
            if (  (data[14].fvalue < 0.43324739)) {
              sum += (float)-0.054119561;
            } else {
              sum += (float)0.12983669;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.00094237365;
            } else {
              sum += (float)-0.10960115;
            }
          } else {
            if (  (data[5].fvalue < 0.60615849)) {
              sum += (float)0.12967676;
            } else {
              sum += (float)-0.039437715;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.18459637)) {
          if (  (data[13].fvalue < 0.037202384)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.075391397;
            } else {
              sum += (float)0.029144032;
            }
          } else {
            if (  (data[15].fvalue < 0.10745863)) {
              sum += (float)-0.0027745361;
            } else {
              sum += (float)0.072785564;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.12627371)) {
            if (  (data[13].fvalue < 0.20280541)) {
              sum += (float)-0.11974837;
            } else {
              sum += (float)-0.055313468;
            }
          } else {
            if ( (data[4].fvalue < 1.4503863)) {
              sum += (float)0.010203133;
            } else {
              sum += (float)-0.082219966;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.021911537)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.27090064)) {
            if (  (data[3].fvalue < 0.36311671)) {
              sum += (float)-0.022602521;
            } else {
              sum += (float)-0.16659701;
            }
          } else {
            if (  (data[15].fvalue < 0.40365109)) {
              sum += (float)-0.054211635;
            } else {
              sum += (float)0.034460749;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.031140182)) {
              sum += (float)-0.078868635;
            } else {
              sum += (float)0.13513789;
            }
          } else {
            if (  (data[3].fvalue < 0.29687801)) {
              sum += (float)0.11331239;
            } else {
              sum += (float)-0.12612097;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.28097549)) {
          if (  (data[4].fvalue < 1.7225404)) {
            if (  (data[14].fvalue < 0.0068418346)) {
              sum += (float)0.12063935;
            } else {
              sum += (float)0.19832189;
            }
          } else {
            if (  (data[15].fvalue < 0.40365109)) {
              sum += (float)-0.031671368;
            } else {
              sum += (float)0.07927613;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.012159523)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.033987463;
            } else {
              sum += (float)0.10307142;
            }
          } else {
            if (  (data[4].fvalue < 1.6934785)) {
              sum += (float)0.10146317;
            } else {
              sum += (float)0.023687808;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 0.10412074)) {
      if (  (data[15].fvalue < 0.83783364)) {
        if (  (data[13].fvalue < 0.06159395)) {
          if (  (data[15].fvalue < 0.022511421)) {
            if (  (data[13].fvalue < 0.0078835208)) {
              sum += (float)-0.27219075;
            } else {
              sum += (float)-0.16543242;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.15183434;
            } else {
              sum += (float)-0.022092024;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.086497411;
            } else {
              sum += (float)-0.005649589;
            }
          } else {
            if (  (data[5].fvalue < 0.60615849)) {
              sum += (float)-0.089702234;
            } else {
              sum += (float)-0.24316037;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 0.014668128)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.066859156;
            } else {
              sum += (float)0.1160199;
            }
          } else {
            if (  (data[15].fvalue < 1.1858221)) {
              sum += (float)0.031114992;
            } else {
              sum += (float)0.13234425;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.28404188)) {
            if (  (data[14].fvalue < 0.93405634)) {
              sum += (float)-0.17061922;
            } else {
              sum += (float)-0.038761094;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.039245091;
            } else {
              sum += (float)-0.13217656;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.19054288)) {
        if (  (data[13].fvalue < 0.0078835208)) {
          if (  (data[14].fvalue < 0.024251973)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.42474881;
            } else {
              sum += (float)-0.23091917;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.1059385;
            } else {
              sum += (float)0.15894172;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.022511421)) {
            if (  (data[3].fvalue < 0.057108123)) {
              sum += (float)0.23244797;
            } else {
              sum += (float)0.11824526;
            }
          } else {
            if (  (data[14].fvalue < 0.0020100581)) {
              sum += (float)-0.11212739;
            } else {
              sum += (float)0.069877021;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.0068418346)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.21351425)) {
              sum += (float)-0.14318915;
            } else {
              sum += (float)-0.041662652;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.066704124;
            } else {
              sum += (float)-0.2062871;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.59143776)) {
            if (  (data[13].fvalue < 0.0078835208)) {
              sum += (float)-0.076313719;
            } else {
              sum += (float)0.0041407743;
            }
          } else {
            if (  (data[13].fvalue < 0.12370386)) {
              sum += (float)0.012345468;
            } else {
              sum += (float)0.06710735;
            }
          }
        }
      }
    }
  }
  if (  (data[0].fvalue < 2.2274072)) {
    if (  (data[12].fvalue < 0.99999899)) {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.30612937;
            } else {
              sum += (float)0.081954964;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.033134039;
            } else {
              sum += (float)0.11168706;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.14527741)) {
            if (  (data[15].fvalue < 0.040412392)) {
              sum += (float)0.010043972;
            } else {
              sum += (float)-0.042014204;
            }
          } else {
            if (  (data[14].fvalue < 0.012274939)) {
              sum += (float)0.0072054067;
            } else {
              sum += (float)0.084749654;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.25065359)) {
          if ( (data[4].fvalue < 1.1048889)) {
            if (  (data[13].fvalue < 0.045584641)) {
              sum += (float)0.19263743;
            } else {
              sum += (float)0.31786913;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.047540776;
            } else {
              sum += (float)0.32521293;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.19282314)) {
            if (  (data[14].fvalue < 0.29631051)) {
              sum += (float)-0.049382582;
            } else {
              sum += (float)0.067119621;
            }
          } else {
            if (  (data[6].fvalue < 0.53107959)) {
              sum += (float)0.26617143;
            } else {
              sum += (float)0.091098793;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.3458002)) {
        if (  (data[6].fvalue < 0.63074851)) {
          sum += (float)0.41060802;
        } else {
          sum += (float)0.23328462;
        }
      } else {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.046288002)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.030970376;
            } else {
              sum += (float)0.30733654;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.065032713;
            } else {
              sum += (float)0.23990083;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7840695)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.21664046;
            } else {
              sum += (float)0.37603751;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.12923642;
            } else {
              sum += (float)-0.059019919;
            }
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 0.83975661)) {
      if (  (data[15].fvalue < 0.3626484)) {
        if (  (data[15].fvalue < 0.040412392)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.000919502;
            } else {
              sum += (float)-0.11350006;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.17011987;
            } else {
              sum += (float)-0.051400706;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.92802876)) {
            if (  (data[14].fvalue < 0.93266344)) {
              sum += (float)-0.014337745;
            } else {
              sum += (float)-0.10598474;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.16886786;
            } else {
              sum += (float)-0.079847828;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.50513268)) {
          if (  (data[13].fvalue < 0.079171307)) {
            if (  (data[14].fvalue < 0.19035533)) {
              sum += (float)-0.073406897;
            } else {
              sum += (float)0.039131694;
            }
          } else {
            if (  (data[16].fvalue < 0.014029334)) {
              sum += (float)0.058022086;
            } else {
              sum += (float)0.013412894;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.8340143)) {
            if (  (data[16].fvalue < 0.014029334)) {
              sum += (float)0.0010855224;
            } else {
              sum += (float)-0.081658199;
            }
          } else {
            if (  (data[14].fvalue < 0.60355103)) {
              sum += (float)-0.10777763;
            } else {
              sum += (float)0.0053611491;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.3551124)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 0.057363294)) {
            if (  (data[17].fvalue < 0.13862924)) {
              sum += (float)0.12602662;
            } else {
              sum += (float)-0.039378144;
            }
          } else {
            if (  (data[13].fvalue < 0.23603208)) {
              sum += (float)-0.10045872;
            } else {
              sum += (float)0.0011536187;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.071889013)) {
            if (  (data[15].fvalue < 1.469471)) {
              sum += (float)0.1497703;
            } else {
              sum += (float)0.0028683606;
            }
          } else {
            if (  (data[14].fvalue < 0.60355103)) {
              sum += (float)-0.044847019;
            } else {
              sum += (float)0.10622741;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8984779)) {
          if (  (data[14].fvalue < 0.012274939)) {
            if (  (data[15].fvalue < 0.8340143)) {
              sum += (float)-0.035667587;
            } else {
              sum += (float)0.20082417;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.20533146;
            } else {
              sum += (float)0.048812278;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.64175969)) {
              sum += (float)0.11959244;
            } else {
              sum += (float)0.22343028;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.12228319;
            } else {
              sum += (float)-0.077814877;
            }
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.78812814)) {
    if (  (data[7].fvalue < 1.999999)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.19298966)) {
          if (  (data[14].fvalue < 0.0019282821)) {
            if (  (data[3].fvalue < 0.65631235)) {
              sum += (float)0.24455301;
            } else {
              sum += (float)0.38921449;
            }
          } else {
            if (  (data[3].fvalue < 1.4295372)) {
              sum += (float)0.13614221;
            } else {
              sum += (float)0.25814706;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.73927563)) {
            sum += (float)-0.31181377;
          } else {
            if (  (data[5].fvalue < 1.5298955)) {
              sum += (float)0.10320015;
            } else {
              sum += (float)0.20580207;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.60322344)) {
            if (  (data[6].fvalue < 0.21853788)) {
              sum += (float)0.018426428;
            } else {
              sum += (float)-0.062669352;
            }
          } else {
            sum += (float)0.26285037;
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.19008695)) {
              sum += (float)-0.21520232;
            } else {
              sum += (float)-0.047008645;
            }
          } else {
            sum += (float)-0.43591112;
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 0.99999899)) {
        if (  (data[8].fvalue < 0.99999899)) {
          if (  (data[3].fvalue < 1.356832)) {
            if (  (data[13].fvalue < 0.14346352)) {
              sum += (float)0.14921053;
            } else {
              sum += (float)-0.033685271;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.19896917;
            } else {
              sum += (float)-0.024873156;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 1.5298955)) {
              sum += (float)-0.27960372;
            } else {
              sum += (float)-0.10776067;
            }
          } else {
            if (  (data[3].fvalue < 1.9446853)) {
              sum += (float)0.094288133;
            } else {
              sum += (float)-0.16896355;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.68222874)) {
          sum += (float)-0.45998511;
        } else {
          sum += (float)-0.22183283;
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 1.4295372)) {
      if (  (data[7].fvalue < 8.999999)) {
        if (  (data[13].fvalue < 0.21413669)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.013246153;
            } else {
              sum += (float)0.054240331;
            }
          } else {
            if (  (data[6].fvalue < 0.084834158)) {
              sum += (float)0.25284296;
            } else {
              sum += (float)0.020760156;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.58847117)) {
            if (  (data[14].fvalue < 0.3552596)) {
              sum += (float)0.0044960389;
            } else {
              sum += (float)0.064102694;
            }
          } else {
            if (  (data[15].fvalue < 0.039661087)) {
              sum += (float)0.11243001;
            } else {
              sum += (float)0.015289831;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.403356)) {
          if (  (data[8].fvalue < 7.999999)) {
            if (  (data[3].fvalue < 0.63062239)) {
              sum += (float)0.23433867;
            } else {
              sum += (float)0.063936703;
            }
          } else {
            sum += (float)0.29120016;
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 0.54247451)) {
              sum += (float)0.019646186;
            } else {
              sum += (float)0.25061914;
            }
          } else {
            if (  (data[7].fvalue < 11.999999)) {
              sum += (float)0.02278013;
            } else {
              sum += (float)0.19800122;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[4].fvalue < 1.8182888)) {
          if (  (data[15].fvalue < 1.1773133)) {
            if (  (data[17].fvalue < 0.007467323)) {
              sum += (float)-0.00071381591;
            } else {
              sum += (float)-0.046667509;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.1354529;
            } else {
              sum += (float)0.17534328;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.7160066)) {
            if (  (data[13].fvalue < 0.4671565)) {
              sum += (float)-0.0070109502;
            } else {
              sum += (float)0.080979757;
            }
          } else {
            if (  (data[17].fvalue < 0.40675887)) {
              sum += (float)-0.10852413;
            } else {
              sum += (float)0.038372058;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.20778067)) {
          if (  (data[13].fvalue < 0.27194026)) {
            if (  (data[4].fvalue < 1.345088)) {
              sum += (float)-0.26157773;
            } else {
              sum += (float)0.079242058;
            }
          } else {
            if (  (data[14].fvalue < 0.13017301)) {
              sum += (float)-0.30732033;
            } else {
              sum += (float)-0.18741548;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.68222874)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.1112248;
            } else {
              sum += (float)-0.090975024;
            }
          } else {
            if (  (data[14].fvalue < 0.60322344)) {
              sum += (float)-0.017509509;
            } else {
              sum += (float)0.11412009;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 3.999999)) {
    if (  (data[11].fvalue < 2.999999)) {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[4].fvalue < 1.0545269)) {
          if (  (data[14].fvalue < 0.0016813673)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.0061032204;
            } else {
              sum += (float)0.33328295;
            }
          } else {
            if (  (data[14].fvalue < 0.017467104)) {
              sum += (float)-0.17815746;
            } else {
              sum += (float)-0.031247336;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.039379708)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.03764303;
            } else {
              sum += (float)-0.042887993;
            }
          } else {
            if (  (data[15].fvalue < 0.14432779)) {
              sum += (float)-0.035052698;
            } else {
              sum += (float)0.0082191387;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[6].fvalue < 0.046588216)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.037284836;
            } else {
              sum += (float)0.33600992;
            }
          } else {
            if (  (data[14].fvalue < 0.15742411)) {
              sum += (float)-0.059404612;
            } else {
              sum += (float)-0.0076839929;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.11320767)) {
            if (  (data[0].fvalue < 3.6407745)) {
              sum += (float)0.272057;
            } else {
              sum += (float)0.079600044;
            }
          } else {
            if (  (data[13].fvalue < 0.78143227)) {
              sum += (float)-0.00068731804;
            } else {
              sum += (float)0.074139699;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.8181117)) {
        if (  (data[14].fvalue < 0.0016813673)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.7238766)) {
              sum += (float)-0.39807859;
            } else {
              sum += (float)-0.28148222;
            }
          } else {
            if (  (data[4].fvalue < 1.6624359)) {
              sum += (float)-0.27203891;
            } else {
              sum += (float)-0.04160551;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.5617003)) {
            if ( (data[4].fvalue < 1.3823683)) {
              sum += (float)-0.34574282;
            } else {
              sum += (float)-0.2202431;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.11410188;
            } else {
              sum += (float)0.026367998;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.58910352)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.32815248;
            } else {
              sum += (float)-0.060395934;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)-0.013965395;
            } else {
              sum += (float)-0.27906531;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[11].fvalue < 3.999999)) {
              sum += (float)0.050825495;
            } else {
              sum += (float)-0.066190004;
            }
          } else {
            if (  (data[4].fvalue < 2.6472743)) {
              sum += (float)-0.051016949;
            } else {
              sum += (float)-0.23842959;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.31784499;
  }
  if (  (data[0].fvalue < 1.7367369)) {
    if (  (data[6].fvalue < 0.025925241)) {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[0].fvalue < 1.5785881)) {
          sum += (float)-0.50779122;
        } else {
          if (  (data[13].fvalue < 0.0087521737)) {
            sum += (float)-0.044026125;
          } else {
            sum += (float)-0.46727499;
          }
        }
      } else {
        sum += (float)0.22456099;
      }
    } else {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[5].fvalue < 0.10563336)) {
            if (  (data[6].fvalue < 0.53083342)) {
              sum += (float)0.024051396;
            } else {
              sum += (float)0.13200851;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.046160221;
            } else {
              sum += (float)0.010623677;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[6].fvalue < 0.77862906)) {
              sum += (float)0.34667122;
            } else {
              sum += (float)0.10988923;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.033535134;
            } else {
              sum += (float)0.36928499;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.34495556)) {
          if ( (data[4].fvalue < 1.1423405)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.162304;
            } else {
              sum += (float)0.30304617;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.016878162;
            } else {
              sum += (float)0.31808519;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.14545175)) {
            if (  (data[14].fvalue < 0.53494769)) {
              sum += (float)-0.0087899985;
            } else {
              sum += (float)0.16033767;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)0.18366645;
            } else {
              sum += (float)-0.11464909;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 0.0086841192)) {
      if (  (data[11].fvalue < 0.99999899)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 1.9807705)) {
            if (  (data[6].fvalue < 0.085071802)) {
              sum += (float)-0.021914367;
            } else {
              sum += (float)0.018373366;
            }
          } else {
            if (  (data[14].fvalue < 0.60230589)) {
              sum += (float)-0.13438945;
            } else {
              sum += (float)0.12068473;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.0035819691;
            } else {
              sum += (float)0.10515501;
            }
          } else {
            if (  (data[14].fvalue < 0.011606862)) {
              sum += (float)-0.19455658;
            } else {
              sum += (float)-0.016869629;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.40106514)) {
              sum += (float)0.21290976;
            } else {
              sum += (float)0.33599558;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.0030318892;
            } else {
              sum += (float)0.16461088;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.60762089)) {
            if (  (data[15].fvalue < 0.7191152)) {
              sum += (float)0.047181074;
            } else {
              sum += (float)0.23052645;
            }
          } else {
            sum += (float)-0.21546136;
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.0062875636)) {
        if (  (data[7].fvalue < 1.999999)) {
          if ( (data[4].fvalue < 1.9932089)) {
            if (  (data[11].fvalue < 2.999999)) {
              sum += (float)-0.080651052;
            } else {
              sum += (float)-0.27493545;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.27474573;
            } else {
              sum += (float)-0.016495463;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.013399726)) {
              sum += (float)0.081393726;
            } else {
              sum += (float)-0.027514325;
            }
          } else {
            if (  (data[5].fvalue < 0.35884923)) {
              sum += (float)-0.12894447;
            } else {
              sum += (float)-0.25734773;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.48795766)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.0087521737)) {
              sum += (float)-0.08097101;
            } else {
              sum += (float)0.029235747;
            }
          } else {
            if (  (data[5].fvalue < 0.73960942)) {
              sum += (float)-0.050778702;
            } else {
              sum += (float)-0.25321886;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.013399726)) {
            if (  (data[13].fvalue < 0.054304298)) {
              sum += (float)0.013079521;
            } else {
              sum += (float)0.057469476;
            }
          } else {
            if (  (data[13].fvalue < 0.84457737)) {
              sum += (float)-0.052013412;
            } else {
              sum += (float)0.051634319;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 1.5267079)) {
    if (  (data[14].fvalue < 0.11733091)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.24812546)) {
          if (  (data[17].fvalue < 0.72042096)) {
            if (  (data[15].fvalue < 0.83191639)) {
              sum += (float)0.00078882923;
            } else {
              sum += (float)-0.12829201;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.21947537;
            } else {
              sum += (float)0.06485749;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.0177396)) {
            if (  (data[15].fvalue < 0.98080248)) {
              sum += (float)-0.0037066664;
            } else {
              sum += (float)0.087132007;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.095906697;
            } else {
              sum += (float)-0.03505522;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.23490962)) {
          if (  (data[4].fvalue < 1.1420659)) {
            if (  (data[13].fvalue < 0.022828773)) {
              sum += (float)-0.092004947;
            } else {
              sum += (float)0.24071898;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.12513216;
            } else {
              sum += (float)0.070816226;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.14542305)) {
            if (  (data[14].fvalue < 0.038159158)) {
              sum += (float)-0.19700256;
            } else {
              sum += (float)-0.09083499;
            }
          } else {
            if (  (data[13].fvalue < 0.27220017)) {
              sum += (float)0.080300435;
            } else {
              sum += (float)-0.071770318;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 1.5070499)) {
        if (  (data[3].fvalue < 0.20528416)) {
          if (  (data[4].fvalue < 1.1420659)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.054582749;
            } else {
              sum += (float)0.2961196;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.010940369;
            } else {
              sum += (float)0.073197037;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.43245944)) {
            if (  (data[3].fvalue < 1.1186571)) {
              sum += (float)0.011070312;
            } else {
              sum += (float)-0.036749739;
            }
          } else {
            if (  (data[13].fvalue < 0.57147884)) {
              sum += (float)0.016599258;
            } else {
              sum += (float)0.08951465;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.98080248)) {
          if (  (data[13].fvalue < 0.5418793)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.06197032;
            } else {
              sum += (float)-0.16709736;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.17614663;
            } else {
              sum += (float)-0.022954125;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if ( (data[4].fvalue < 1.9928045)) {
              sum += (float)0.04765651;
            } else {
              sum += (float)0.25645342;
            }
          } else {
            if (  (data[13].fvalue < 0.15338056)) {
              sum += (float)-0.074149005;
            } else {
              sum += (float)0.090686329;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.11820966)) {
      sum += (float)-0.46867618;
    } else {
      if (  (data[7].fvalue < 3.999999)) {
        if (  (data[3].fvalue < 1.2876749)) {
          if (  (data[13].fvalue < 0.030411109)) {
            if (  (data[14].fvalue < 0.0015152892)) {
              sum += (float)0.21125741;
            } else {
              sum += (float)0.013465484;
            }
          } else {
            if (  (data[3].fvalue < 0.23490962)) {
              sum += (float)-0.30655277;
            } else {
              sum += (float)0.050490927;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.939513)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.2533527;
            } else {
              sum += (float)0.074799262;
            }
          } else {
            sum += (float)0.38918346;
          }
        }
      } else {
        if (  (data[3].fvalue < 0.49425825)) {
          sum += (float)0.37071985;
        } else {
          if (  (data[14].fvalue < 0.038159158)) {
            sum += (float)0.039732665;
          } else {
            if (  (data[13].fvalue < 0.10649632)) {
              sum += (float)0.20069654;
            } else {
              sum += (float)0.38062784;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 2.999999)) {
    if (  (data[8].fvalue < 10.999999)) {
      if (  (data[5].fvalue < 1.0740857)) {
        if (  (data[4].fvalue < 1.028551)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.082654558)) {
              sum += (float)-0.35267127;
            } else {
              sum += (float)-0.042097442;
            }
          } else {
            if (  (data[13].fvalue < 0.046214733)) {
              sum += (float)0.039246269;
            } else {
              sum += (float)-0.034001086;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.017160071)) {
              sum += (float)-0.0059105228;
            } else {
              sum += (float)0.016601833;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.11168425;
            } else {
              sum += (float)-0.026263634;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 0.0010246859)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.025159484;
            } else {
              sum += (float)0.28219527;
            }
          } else {
            if (  (data[14].fvalue < 0.011351111)) {
              sum += (float)-0.15243892;
            } else {
              sum += (float)-0.00033207558;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.2870455)) {
            if (  (data[3].fvalue < 0.41606918)) {
              sum += (float)0.37503794;
            } else {
              sum += (float)0.18439734;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.034635909;
            } else {
              sum += (float)-0.40603894;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.5416019)) {
        if (  (data[14].fvalue < 0.43188247)) {
          sum += (float)0.27641022;
        } else {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[14].fvalue < 0.79200327)) {
              sum += (float)0.16925308;
            } else {
              sum += (float)0.30874556;
            }
          } else {
            if (  (data[8].fvalue < 12.999999)) {
              sum += (float)-0;
            } else {
              sum += (float)0.19071995;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 14.999999)) {
          if (  (data[7].fvalue < 5.999999)) {
            sum += (float)0.20162772;
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.0057981969;
            } else {
              sum += (float)-0.20882283;
            }
          }
        } else {
          sum += (float)0.19741921;
        }
      }
    }
  } else {
    if (  (data[0].fvalue < 5.6493883)) {
      sum += (float)0.30673909;
    } else {
      sum += (float)0.10978869;
    }
  }
  if (  (data[7].fvalue < 5.999999)) {
    if (  (data[16].fvalue < 0.012921839)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.34053764)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.32928261)) {
              sum += (float)0.0063358578;
            } else {
              sum += (float)-0.01203311;
            }
          } else {
            if ( (data[4].fvalue < 1.4447538)) {
              sum += (float)0.37185913;
            } else {
              sum += (float)0.039617166;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.1977491)) {
            if (  (data[14].fvalue < 0.39139438)) {
              sum += (float)0.005830978;
            } else {
              sum += (float)0.074973345;
            }
          } else {
            if ( (data[4].fvalue < 1.7815737)) {
              sum += (float)-0.06325949;
            } else {
              sum += (float)0.15229821;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.65648079)) {
          if (  (data[4].fvalue < 1.7815737)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.057202246;
            } else {
              sum += (float)0.39539543;
            }
          } else {
            if (  (data[4].fvalue < 2.1924694)) {
              sum += (float)-0.2499273;
            } else {
              sum += (float)0.091721445;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.037907448)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.21019475;
            } else {
              sum += (float)0.29204625;
            }
          } else {
            if ( (data[4].fvalue < 1.8167447)) {
              sum += (float)0.091916732;
            } else {
              sum += (float)-0.070959665;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[16].fvalue < 0.31988394)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.26987687;
            } else {
              sum += (float)-0.077666603;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.075773008;
            } else {
              sum += (float)0.012434113;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.011484351)) {
            if (  (data[4].fvalue < 2.0491309)) {
              sum += (float)-0.043336801;
            } else {
              sum += (float)0.027301352;
            }
          } else {
            if (  (data[17].fvalue < 0.0031362833)) {
              sum += (float)0.067983136;
            } else {
              sum += (float)0.0013276868;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7486103)) {
          if (  (data[15].fvalue < 0.021176932)) {
            sum += (float)0.21184698;
          } else {
            sum += (float)0.41314188;
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.28517771)) {
              sum += (float)0.050898913;
            } else {
              sum += (float)0.26334324;
            }
          } else {
            if (  (data[4].fvalue < 2.6462989)) {
              sum += (float)-0.1045256;
            } else {
              sum += (float)0.11085909;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.28112659)) {
      if (  (data[4].fvalue < 1.2033892)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[4].fvalue < 1.1234787)) {
            if (  (data[0].fvalue < 1.8794926)) {
              sum += (float)0.32989627;
            } else {
              sum += (float)0.17313716;
            }
          } else {
            if (  (data[13].fvalue < 0.27260047)) {
              sum += (float)-0.1178829;
            } else {
              sum += (float)0.25262704;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.1834553)) {
            if (  (data[4].fvalue < 1.1752337)) {
              sum += (float)0.2745198;
            } else {
              sum += (float)0.11403165;
            }
          } else {
            sum += (float)0.36960864;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8547969)) {
          if (  (data[13].fvalue < 0.42590767)) {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.0037724755;
            } else {
              sum += (float)0.189601;
            }
          } else {
            if (  (data[4].fvalue < 1.4229535)) {
              sum += (float)0.28595465;
            } else {
              sum += (float)0.12044251;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 6.999999)) {
              sum += (float)0.039382182;
            } else {
              sum += (float)0.14333126;
            }
          } else {
            if (  (data[13].fvalue < 0.32572025)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.14676744;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[13].fvalue < 0.64039767)) {
          if (  (data[14].fvalue < 0.063068397)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.04524824;
            } else {
              sum += (float)-0.12773524;
            }
          } else {
            if (  (data[13].fvalue < 0.38903031)) {
              sum += (float)-0.052377045;
            } else {
              sum += (float)0.081684887;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.063068397)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.22942165;
            } else {
              sum += (float)-0.041108362;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.20041123;
            } else {
              sum += (float)-0.016099887;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.30755082)) {
          if ( (data[4].fvalue < 1.2290891)) {
            if (  (data[3].fvalue < 0.86772668)) {
              sum += (float)0.20339827;
            } else {
              sum += (float)0.072416179;
            }
          } else {
            if (  (data[15].fvalue < 0.10636506)) {
              sum += (float)-0.046976026;
            } else {
              sum += (float)0.20951264;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.40097573)) {
              sum += (float)-0.048014387;
            } else {
              sum += (float)0.063427277;
            }
          } else {
            if (  (data[16].fvalue < 0.31988394)) {
              sum += (float)-0.33787674;
            } else {
              sum += (float)-0.14520785;
            }
          }
        }
      }
    }
  }
  if (  (data[11].fvalue < 2.999999)) {
    if (  (data[7].fvalue < 10.999999)) {
      if (  (data[5].fvalue < 0.029715875)) {
        if (  (data[4].fvalue < 1.4891746)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.032145996;
            } else {
              sum += (float)0.050626587;
            }
          } else {
            if (  (data[13].fvalue < 0.046089027)) {
              sum += (float)0.0027233171;
            } else {
              sum += (float)0.19045648;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8540242)) {
            if (  (data[13].fvalue < 0.57088369)) {
              sum += (float)0.15186246;
            } else {
              sum += (float)0.048763346;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.065893114;
            } else {
              sum += (float)-0.058911394;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 0.79021728)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.094146736;
            } else {
              sum += (float)-0.072063625;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.0084829181;
            } else {
              sum += (float)-0.29694611;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.6945235)) {
            if (  (data[13].fvalue < 0.18310289)) {
              sum += (float)-0.0066998354;
            } else {
              sum += (float)0.0083264662;
            }
          } else {
            if (  (data[14].fvalue < 0.68836832)) {
              sum += (float)-0.029139755;
            } else {
              sum += (float)0.011714745;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.43300807)) {
        if (  (data[13].fvalue < 0.44536293)) {
          sum += (float)0.27797416;
        } else {
          if (  (data[8].fvalue < 4.999999)) {
            sum += (float)0.27896351;
          } else {
            if (  (data[7].fvalue < 12.999999)) {
              sum += (float)-0.0089603551;
            } else {
              sum += (float)0.22826719;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 6.999999)) {
          if (  (data[13].fvalue < 1.3540187)) {
            sum += (float)0.11643886;
          } else {
            sum += (float)0.28499806;
          }
        } else {
          if (  (data[7].fvalue < 14.999999)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.040617306;
            } else {
              sum += (float)-0.13041158;
            }
          } else {
            sum += (float)0.20938948;
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.7483333)) {
      if (  (data[9].fvalue < 0.99999899)) {
        if (  (data[17].fvalue < 0.90880471)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.7169733)) {
              sum += (float)-0.31584412;
            } else {
              sum += (float)-0.21000513;
            }
          } else {
            if (  (data[14].fvalue < 0.011769691)) {
              sum += (float)-0.25787702;
            } else {
              sum += (float)-0.12314989;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 1.1976812)) {
              sum += (float)-0.18128358;
            } else {
              sum += (float)-0.047346834;
            }
          } else {
            sum += (float)-0.010862342;
          }
        }
      } else {
        if (  (data[5].fvalue < 0.55437529)) {
          if (  (data[2].fvalue < 0.52584088)) {
            sum += (float)0.15190601;
          } else {
            sum += (float)-0.045214724;
          }
        } else {
          sum += (float)-0.18052061;
        }
      }
    } else {
      if (  (data[17].fvalue < 0.58311188)) {
        if (  (data[8].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.60340953)) {
              sum += (float)-0.14535631;
            } else {
              sum += (float)-0.31136644;
            }
          } else {
            sum += (float)-0.09874358;
          }
        } else {
          if (  (data[14].fvalue < 0.20883699)) {
            if (  (data[4].fvalue < 1.9405928)) {
              sum += (float)-0.026387146;
            } else {
              sum += (float)0.12287962;
            }
          } else {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)-0.072261684;
            } else {
              sum += (float)-0.22692001;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 0.15340051)) {
            if (  (data[14].fvalue < 0.011769691)) {
              sum += (float)-0.19043811;
            } else {
              sum += (float)0.0098696453;
            }
          } else {
            if (  (data[14].fvalue < 0.10597068)) {
              sum += (float)0.071323149;
            } else {
              sum += (float)-0.038535018;
            }
          }
        } else {
          if (  (data[8].fvalue < 5.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.1488536;
            } else {
              sum += (float)-0.0082963873;
            }
          } else {
            sum += (float)-0.29108819;
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 3.999999)) {
    if (  (data[16].fvalue < 0.49133667)) {
      if (  (data[16].fvalue < 0.007767838)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.0039870343;
            } else {
              sum += (float)0.019309815;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.02897366;
            } else {
              sum += (float)-0.020739134;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.017385038)) {
            if (  (data[13].fvalue < 0.046061378)) {
              sum += (float)-0.41904244;
            } else {
              sum += (float)-0.098149434;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.098480515;
            } else {
              sum += (float)-0.096700057;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.084814355;
            } else {
              sum += (float)-0.020940775;
            }
          } else {
            if (  (data[4].fvalue < 2.0477767)) {
              sum += (float)0.075009465;
            } else {
              sum += (float)-0.024442667;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8540308)) {
            if (  (data[15].fvalue < 0.021357367)) {
              sum += (float)0.11175963;
            } else {
              sum += (float)0.35795328;
            }
          } else {
            if (  (data[3].fvalue < 0.32898539)) {
              sum += (float)0.26982594;
            } else {
              sum += (float)-0.024077794;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.05785168)) {
        if (  (data[10].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.094153546)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.012890964;
            } else {
              sum += (float)0.13365728;
            }
          } else {
            if (  (data[17].fvalue < 0.040186368)) {
              sum += (float)0.19232105;
            } else {
              sum += (float)0.078524016;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.27150577)) {
              sum += (float)-0.24083944;
            } else {
              sum += (float)-0.048274841;
            }
          } else {
            if (  (data[13].fvalue < 0.44442171)) {
              sum += (float)-0.12413416;
            } else {
              sum += (float)0.035137136;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.28415248)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.067662604;
            } else {
              sum += (float)0.015800614;
            }
          } else {
            if (  (data[4].fvalue < 2.6452277)) {
              sum += (float)0.09175396;
            } else {
              sum += (float)-0.017436961;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.38380194)) {
            if (  (data[10].fvalue < 2.999999)) {
              sum += (float)0.069226243;
            } else {
              sum += (float)-0.13622583;
            }
          } else {
            if (  (data[15].fvalue < 0.49556738)) {
              sum += (float)-0.037639722;
            } else {
              sum += (float)0.022587478;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 4.999999)) {
      sum += (float)0.18378589;
    } else {
      sum += (float)0.30956727;
    }
  }
  if (  (data[8].fvalue < 6.999999)) {
    if (  (data[13].fvalue < 0.28387925)) {
      if (  (data[17].fvalue < 1.719871)) {
        if (  (data[14].fvalue < 0.0015405169)) {
          if (  (data[13].fvalue < 0.0378359)) {
            if (  (data[3].fvalue < 0.98011655)) {
              sum += (float)0.066327408;
            } else {
              sum += (float)0.42973381;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.017205337;
            } else {
              sum += (float)0.15075289;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.062304776)) {
            if (  (data[14].fvalue < 0.046355993)) {
              sum += (float)-0.10653134;
            } else {
              sum += (float)-0.015498375;
            }
          } else {
            if (  (data[15].fvalue < 0.12457076)) {
              sum += (float)-0.001989326;
            } else {
              sum += (float)0.050654139;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.3567889)) {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.31967819;
            } else {
              sum += (float)-0.11971026;
            }
          } else {
            if (  (data[13].fvalue < 0.0378359)) {
              sum += (float)-0.093059376;
            } else {
              sum += (float)0.087433353;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.5380249)) {
            if (  (data[3].fvalue < 2.2920532)) {
              sum += (float)0.072394155;
            } else {
              sum += (float)-0.074903212;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)0.18613608;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.9787783)) {
        if (  (data[17].fvalue < 0.71606195)) {
          if ( (data[4].fvalue < 1.7802848)) {
            if (  (data[14].fvalue < 0.1597739)) {
              sum += (float)0.0015309266;
            } else {
              sum += (float)0.043423794;
            }
          } else {
            if (  (data[16].fvalue < 0.48896706)) {
              sum += (float)-0.070383124;
            } else {
              sum += (float)0.05058651;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.6327397)) {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.079170346;
            } else {
              sum += (float)0.05252035;
            }
          } else {
            if (  (data[15].fvalue < 0.020705741)) {
              sum += (float)0.099294275;
            } else {
              sum += (float)0.012274968;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.999999)) {
          if (  (data[4].fvalue < 1.8946503)) {
            if (  (data[14].fvalue < 0.0015405169)) {
              sum += (float)-0.041213349;
            } else {
              sum += (float)0.13316017;
            }
          } else {
            if (  (data[16].fvalue < 0.059689827)) {
              sum += (float)0.17314944;
            } else {
              sum += (float)0.063170478;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.3472692)) {
              sum += (float)0.035090636;
            } else {
              sum += (float)-0.045239542;
            }
          } else {
            if (  (data[0].fvalue < 5.643609)) {
              sum += (float)-0.019962706;
            } else {
              sum += (float)-0.13381253;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 0.10643132)) {
      if (  (data[13].fvalue < 0.63802171)) {
        if (  (data[14].fvalue < 0.06386251)) {
          sum += (float)0.2970545;
        } else {
          if (  (data[15].fvalue < 0.10607895)) {
            if (  (data[3].fvalue < 0.51443416)) {
              sum += (float)0.208617;
            } else {
              sum += (float)0.015493508;
            }
          } else {
            sum += (float)0.29329631;
          }
        }
      } else {
        sum += (float)0.02244281;
      }
    } else {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 0.045749046)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.23068863;
            } else {
              sum += (float)0.064113133;
            }
          } else {
            if (  (data[14].fvalue < 0.29767922)) {
              sum += (float)-0.0084952982;
            } else {
              sum += (float)-0.26909298;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.27242854)) {
            if (  (data[13].fvalue < 0.12445728)) {
              sum += (float)0.015827486;
            } else {
              sum += (float)-0.084031522;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.24599722;
            } else {
              sum += (float)0.16815931;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.38269651)) {
          if ( (data[4].fvalue < 1.325336)) {
            if (  (data[3].fvalue < 0.70847237)) {
              sum += (float)0.21349353;
            } else {
              sum += (float)0.097435474;
            }
          } else {
            if (  (data[15].fvalue < 0.12457076)) {
              sum += (float)-0.018490074;
            } else {
              sum += (float)0.17659421;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.26975974)) {
              sum += (float)-0.028466212;
            } else {
              sum += (float)0.061596174;
            }
          } else {
            if (  (data[17].fvalue < 0.33642936)) {
              sum += (float)-0.26882896;
            } else {
              sum += (float)-0.065500498;
            }
          }
        }
      }
    }
  }
  if (  (data[3].fvalue < 1.1162862)) {
    if (  (data[15].fvalue < 0.21790862)) {
      if (  (data[15].fvalue < 0.057897571)) {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.17894286;
            } else {
              sum += (float)-0.17998113;
            }
          } else {
            if ( (data[4].fvalue < 1.1738977)) {
              sum += (float)0.013508103;
            } else {
              sum += (float)-0.013079657;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.021429816)) {
            if (  (data[5].fvalue < 0.55510616)) {
              sum += (float)0.16250649;
            } else {
              sum += (float)0.019293237;
            }
          } else {
            if (  (data[14].fvalue < 0.011628685)) {
              sum += (float)-0.051388722;
            } else {
              sum += (float)0.037430663;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4416704)) {
          if (  (data[15].fvalue < 0.10622855)) {
            if (  (data[13].fvalue < 0.15351726)) {
              sum += (float)-0.082927622;
            } else {
              sum += (float)-0.014918397;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.020769233;
            } else {
              sum += (float)0.20035987;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.0187185)) {
              sum += (float)-0.076602638;
            } else {
              sum += (float)-0.18517828;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.0061510294;
            } else {
              sum += (float)0.21303709;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.045995768)) {
        if (  (data[14].fvalue < 0.038170822)) {
          if (  (data[5].fvalue < 0.16411258)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.043580573;
            } else {
              sum += (float)0.35635769;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.19744803;
            } else {
              sum += (float)0.23147681;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.19054298)) {
            if (  (data[4].fvalue < 1.7461725)) {
              sum += (float)0.14305659;
            } else {
              sum += (float)-0.017104777;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.0017093974;
            } else {
              sum += (float)0.049884032;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7152838)) {
          if (  (data[14].fvalue < 0.006201467)) {
            if (  (data[5].fvalue < 0.27091476)) {
              sum += (float)0.079434767;
            } else {
              sum += (float)-0.010625051;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.086851344;
            } else {
              sum += (float)0.19804585;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.55495793)) {
            if (  (data[16].fvalue < 0.11384682)) {
              sum += (float)-0.073703811;
            } else {
              sum += (float)0.010148946;
            }
          } else {
            if (  (data[14].fvalue < 0.054460406)) {
              sum += (float)0.070117682;
            } else {
              sum += (float)0.015327378;
            }
          }
        }
      }
    }
  } else {
    if (  (data[7].fvalue < 2.999999)) {
      if (  (data[15].fvalue < 0.021429816)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.15926529)) {
              sum += (float)0.37696269;
            } else {
              sum += (float)-0.036437549;
            }
          } else {
            if (  (data[17].fvalue < 0.040140834)) {
              sum += (float)0.035150159;
            } else {
              sum += (float)-0.012020539;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.2531285)) {
            if (  (data[3].fvalue < 1.8028798)) {
              sum += (float)0.36172688;
            } else {
              sum += (float)0.10129799;
            }
          } else {
            if (  (data[14].fvalue < 0.35641906)) {
              sum += (float)-0.18699563;
            } else {
              sum += (float)0.070732333;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.025117522;
            } else {
              sum += (float)0.037944067;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.078822017;
            } else {
              sum += (float)-0.082397744;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.2531285)) {
            if (  (data[15].fvalue < 0.12492318)) {
              sum += (float)-0.34890601;
            } else {
              sum += (float)-0.19182132;
            }
          } else {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)-0.074563891;
            } else {
              sum += (float)0.092837684;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.17311081)) {
        if (  (data[14].fvalue < 0.32496464)) {
          if (  (data[4].fvalue < 1.2531285)) {
            if (  (data[15].fvalue < 0.26975098)) {
              sum += (float)-0.34523964;
            } else {
              sum += (float)-0.079309873;
            }
          } else {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.077691257;
            } else {
              sum += (float)0.2217938;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.097434625)) {
            if (  (data[13].fvalue < 0.062511511)) {
              sum += (float)-0.41927922;
            } else {
              sum += (float)-0.32067916;
            }
          } else {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.28482103;
            } else {
              sum += (float)-0.11908006;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.20893955)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)0.0064166482;
            } else {
              sum += (float)0.12017643;
            }
          } else {
            if (  (data[14].fvalue < 0.038170822)) {
              sum += (float)-0.20479076;
            } else {
              sum += (float)-0.077796742;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.77833104)) {
            if (  (data[15].fvalue < 0.44462487)) {
              sum += (float)-0.046917293;
            } else {
              sum += (float)0.043945923;
            }
          } else {
            if (  (data[15].fvalue < 0.021429816)) {
              sum += (float)0.11454625;
            } else {
              sum += (float)-0.005038939;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.35676852)) {
    if (  (data[8].fvalue < 8.999999)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[15].fvalue < 0.97806597)) {
          if (  (data[14].fvalue < 0.017448474)) {
            if (  (data[14].fvalue < 0.0011115728)) {
              sum += (float)0.0020726484;
            } else {
              sum += (float)-0.060434759;
            }
          } else {
            if (  (data[4].fvalue < 1.0281364)) {
              sum += (float)-0.026392387;
            } else {
              sum += (float)0.033844765;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.31093881)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.11832366;
            } else {
              sum += (float)0.072054036;
            }
          } else {
            if (  (data[5].fvalue < 0.36030641)) {
              sum += (float)0.10445696;
            } else {
              sum += (float)0.024237217;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.13122579)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.030783184)) {
              sum += (float)-0.13143784;
            } else {
              sum += (float)0.052795224;
            }
          } else {
            if (  (data[6].fvalue < 0.04681395)) {
              sum += (float)0.3768709;
            } else {
              sum += (float)0.090897232;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.1237866)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.070124514;
            } else {
              sum += (float)-0.012312425;
            }
          } else {
            if (  (data[13].fvalue < 0.51286107)) {
              sum += (float)0.028859835;
            } else {
              sum += (float)-0.059477869;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.63700181)) {
        if (  (data[13].fvalue < 0.35468093)) {
          if (  (data[7].fvalue < 5.999999)) {
            if (  (data[14].fvalue < 0.20925894)) {
              sum += (float)0.24235918;
            } else {
              sum += (float)0.088390388;
            }
          } else {
            if (  (data[14].fvalue < 0.32522708)) {
              sum += (float)0.26366982;
            } else {
              sum += (float)0.1629779;
            }
          }
        } else {
          if (  (data[7].fvalue < 8.999999)) {
            if (  (data[14].fvalue < 0.22851562)) {
              sum += (float)0.11131003;
            } else {
              sum += (float)-0.15369031;
            }
          } else {
            sum += (float)0.21411614;
          }
        }
      } else {
        if (  (data[14].fvalue < 0.22851562)) {
          sum += (float)0.0016914663;
        } else {
          sum += (float)-0.22171707;
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.3770771)) {
      if (  (data[3].fvalue < 2.0884917)) {
        if (  (data[14].fvalue < 0.53777403)) {
          if (  (data[3].fvalue < 0.73469341)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.064665005;
            } else {
              sum += (float)0.16354442;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.053277977;
            } else {
              sum += (float)-0.016966237;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.01954376)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.19341941;
            } else {
              sum += (float)0.0644494;
            }
          } else {
            if (  (data[3].fvalue < 0.93590921)) {
              sum += (float)0.12351857;
            } else {
              sum += (float)0.043569434;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.79444349)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.828722)) {
              sum += (float)0.011809603;
            } else {
              sum += (float)-0.14771797;
            }
          } else {
            if (  (data[13].fvalue < 0.91684091)) {
              sum += (float)-0.16400784;
            } else {
              sum += (float)0.012993059;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.22285382;
            } else {
              sum += (float)0.014849388;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.0025267908;
            } else {
              sum += (float)-0.1138685;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.62725931)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.79444349)) {
            if (  (data[17].fvalue < 0.90062243)) {
              sum += (float)-0.022474211;
            } else {
              sum += (float)0.088763639;
            }
          } else {
            if (  (data[17].fvalue < 1.1866728)) {
              sum += (float)-0.11423539;
            } else {
              sum += (float)0.085518554;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.0041109235)) {
              sum += (float)0.14019164;
            } else {
              sum += (float)-0.026940713;
            }
          } else {
            if (  (data[13].fvalue < 0.27206066)) {
              sum += (float)-0.041000389;
            } else {
              sum += (float)0.045077424;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.0041109235)) {
          if (  (data[5].fvalue < 0.53918904)) {
            if (  (data[15].fvalue < 0.97806597)) {
              sum += (float)0.036001198;
            } else {
              sum += (float)0.1117175;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.045906778;
            } else {
              sum += (float)0.057421621;
            }
          }
        } else {
          if (  (data[11].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 2.1104071)) {
              sum += (float)0.026194265;
            } else {
              sum += (float)-0.030315885;
            }
          } else {
            if (  (data[13].fvalue < 0.33937779)) {
              sum += (float)-0.13629462;
            } else {
              sum += (float)-0.29946214;
            }
          }
        }
      }
    }
  }
  if (  (data[0].fvalue < 2.0533338)) {
    if (  (data[14].fvalue < 0.17391777)) {
      if (  (data[7].fvalue < 0.99999899)) {
        if (  (data[5].fvalue < 0.26030692)) {
          if (  (data[4].fvalue < 1.5792733)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.27178741;
            } else {
              sum += (float)-0.030735238;
            }
          } else {
            if (  (data[15].fvalue < 0.01917948)) {
              sum += (float)0.32002425;
            } else {
              sum += (float)0.014851125;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 1.0532882)) {
              sum += (float)-0.32623878;
            } else {
              sum += (float)-0.12762894;
            }
          } else {
            if (  (data[6].fvalue < 0.066047698)) {
              sum += (float)0.42261204;
            } else {
              sum += (float)-0.0339702;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.0029239187)) {
          if (  (data[4].fvalue < 1.06795)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.30281165;
            } else {
              sum += (float)-0.24069792;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.27897325;
            } else {
              sum += (float)-0.053901706;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0095299445)) {
            if (  (data[14].fvalue < 0.00046978804)) {
              sum += (float)0.040761098;
            } else {
              sum += (float)-0.17058659;
            }
          } else {
            if (  (data[6].fvalue < 0.026124306)) {
              sum += (float)-0.25119272;
            } else {
              sum += (float)0.0057142014;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.54056668)) {
        if (  (data[6].fvalue < 0.35472575)) {
          if (  (data[13].fvalue < 0.15448961)) {
            if (  (data[6].fvalue < 0.18738583)) {
              sum += (float)0.13878217;
            } else {
              sum += (float)0.022998383;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.066329956;
            } else {
              sum += (float)0.24211374;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.43139216)) {
              sum += (float)0.013979386;
            } else {
              sum += (float)0.054507375;
            }
          } else {
            if (  (data[13].fvalue < 0.072111487)) {
              sum += (float)-0.17433646;
            } else {
              sum += (float)0.0014257237;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 0.35473549)) {
            if (  (data[5].fvalue < 0.36046806)) {
              sum += (float)-0.027567696;
            } else {
              sum += (float)0.050288733;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.068513356;
            } else {
              sum += (float)0.19491266;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.5591892)) {
            if (  (data[13].fvalue < 0.60185891)) {
              sum += (float)0.1928194;
            } else {
              sum += (float)0.31160265;
            }
          } else {
            if (  (data[0].fvalue < 1.4553447)) {
              sum += (float)0.23959272;
            } else {
              sum += (float)0.090498239;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[8].fvalue < 5.999999)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 2.999999)) {
              sum += (float)0.0004570662;
            } else {
              sum += (float)0.12803094;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.078123562;
            } else {
              sum += (float)-0.063716494;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.18355381;
            } else {
              sum += (float)0.069081724;
            }
          } else {
            if (  (data[5].fvalue < 0.5231806)) {
              sum += (float)0.0032215214;
            } else {
              sum += (float)-0.2614755;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.026124306)) {
          if (  (data[14].fvalue < 0.00046978804)) {
            if (  (data[13].fvalue < 0.039047349)) {
              sum += (float)-0.075204223;
            } else {
              sum += (float)0.23990361;
            }
          } else {
            if (  (data[0].fvalue < 4.0569854)) {
              sum += (float)0.41206628;
            } else {
              sum += (float)0.22102769;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.54056668)) {
            if (  (data[15].fvalue < 0.19110058)) {
              sum += (float)-0.045917854;
            } else {
              sum += (float)0.027155777;
            }
          } else {
            if (  (data[14].fvalue < 0.19043778)) {
              sum += (float)-0.017233942;
            } else {
              sum += (float)0.041581377;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        sum += (float)0.17605935;
      } else {
        sum += (float)0.30485395;
      }
    }
  }
  if (  (data[5].fvalue < 0.048756268)) {
    if (  (data[4].fvalue < 0.97689062)) {
      sum += (float)-0.41682419;
    } else {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[4].fvalue < 1.8906713)) {
          if (  (data[17].fvalue < 0.18046533)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.0088273501;
            } else {
              sum += (float)0.052617338;
            }
          } else {
            if (  (data[13].fvalue < 0.0032054388)) {
              sum += (float)-0.12376823;
            } else {
              sum += (float)0.1522824;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 0.0030770793)) {
              sum += (float)0.10650545;
            } else {
              sum += (float)-0.043588258;
            }
          } else {
            if (  (data[17].fvalue < 0.71241897)) {
              sum += (float)-0.071536414;
            } else {
              sum += (float)0.0065752799;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.057264652)) {
          sum += (float)0.3055321;
        } else {
          if (  (data[1].fvalue < 2.1463163)) {
            if (  (data[4].fvalue < 2.2848668)) {
              sum += (float)0.11832014;
            } else {
              sum += (float)0.23927304;
            }
          } else {
            sum += (float)0.014353563;
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.40606833)) {
      if (  (data[8].fvalue < 8.999999)) {
        if (  (data[3].fvalue < 0.41445398)) {
          if (  (data[17].fvalue < 0.13630393)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.002096564;
            } else {
              sum += (float)0.14957109;
            }
          } else {
            if (  (data[13].fvalue < 0.016737098)) {
              sum += (float)-0.080665216;
            } else {
              sum += (float)0.085763425;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.056593023)) {
              sum += (float)0.019939974;
            } else {
              sum += (float)-0.011209661;
            }
          } else {
            if (  (data[13].fvalue < 0.047510233)) {
              sum += (float)-0.2145232;
            } else {
              sum += (float)-0.03245784;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.27021083)) {
          if (  (data[7].fvalue < 7.999999)) {
            if (  (data[3].fvalue < 0.70587838)) {
              sum += (float)0.21784762;
            } else {
              sum += (float)0.054544926;
            }
          } else {
            sum += (float)0.26322964;
          }
        } else {
          if (  (data[8].fvalue < 12.999999)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.15946133;
            } else {
              sum += (float)0.046868939;
            }
          } else {
            sum += (float)0.23257965;
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.57969642)) {
        if (  (data[17].fvalue < 0.040657535)) {
          if (  (data[4].fvalue < 1.1019902)) {
            if (  (data[3].fvalue < 0.6024971)) {
              sum += (float)-0.17242579;
            } else {
              sum += (float)-0.014580289;
            }
          } else {
            if ( (data[4].fvalue < 1.5547348)) {
              sum += (float)0.031986561;
            } else {
              sum += (float)0.00024594559;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.72422951)) {
            if (  (data[15].fvalue < 0.01966445)) {
              sum += (float)0.02320469;
            } else {
              sum += (float)-0.034010082;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.089579791;
            } else {
              sum += (float)-0.012055516;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 1.999999)) {
          if (  (data[5].fvalue < 0.53937858)) {
            if (  (data[15].fvalue < 0.01966445)) {
              sum += (float)0.098567866;
            } else {
              sum += (float)0.0240617;
            }
          } else {
            if (  (data[13].fvalue < 0.54044276)) {
              sum += (float)-0.06493146;
            } else {
              sum += (float)0.011479601;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[17].fvalue < 0.90137899)) {
              sum += (float)-0.0062178709;
            } else {
              sum += (float)-0.14484622;
            }
          } else {
            if (  (data[7].fvalue < 6.999999)) {
              sum += (float)-0.29294783;
            } else {
              sum += (float)-0.16158131;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.093240462)) {
    if (  (data[8].fvalue < 5.999999)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.0031888126)) {
          if (  (data[15].fvalue < 0.019316489)) {
            if (  (data[17].fvalue < 0.0026453012)) {
              sum += (float)0.19881271;
            } else {
              sum += (float)-0.1516721;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.022000136;
            } else {
              sum += (float)-0.13310096;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.00982456)) {
            if (  (data[4].fvalue < 0.98641157)) {
              sum += (float)-0.0037425046;
            } else {
              sum += (float)-0.18342286;
            }
          } else {
            if (  (data[4].fvalue < 1.0396721)) {
              sum += (float)-0.016131686;
            } else {
              sum += (float)0.0040352801;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.036920041)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.075088613;
            } else {
              sum += (float)0.28543547;
            }
          } else {
            if (  (data[0].fvalue < 3.8213775)) {
              sum += (float)0.46558467;
            } else {
              sum += (float)0.16164535;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.76432568)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.061248723;
            } else {
              sum += (float)0.034014054;
            }
          } else {
            if (  (data[5].fvalue < 0.46321851)) {
              sum += (float)-0.043974712;
            } else {
              sum += (float)-0.17410421;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.090117224)) {
        if (  (data[4].fvalue < 1.2513477)) {
          if (  (data[3].fvalue < 0.34456816)) {
            if (  (data[14].fvalue < 0.044912357)) {
              sum += (float)0.29560858;
            } else {
              sum += (float)0.18965611;
            }
          } else {
            sum += (float)0.06543617;
          }
        } else {
          sum += (float)0.32728148;
        }
      } else {
        if (  (data[14].fvalue < 0.053304952)) {
          if (  (data[3].fvalue < 0.65246326)) {
            if (  (data[14].fvalue < 0.037031718)) {
              sum += (float)0.32456189;
            } else {
              sum += (float)0.19679551;
            }
          } else {
            sum += (float)0.049753577;
          }
        } else {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[3].fvalue < 0.28030813)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.22765315;
            }
          } else {
            if (  (data[3].fvalue < 0.76432568)) {
              sum += (float)0.12161893;
            } else {
              sum += (float)-0.091609806;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.20708382)) {
      if (  (data[7].fvalue < 3.999999)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.15226094;
            } else {
              sum += (float)0.018681677;
            }
          } else {
            if (  (data[15].fvalue < 0.019316489)) {
              sum += (float)0.18708871;
            } else {
              sum += (float)0.042523149;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.0837853)) {
            if (  (data[3].fvalue < 0.65246326)) {
              sum += (float)0.23708592;
            } else {
              sum += (float)0.077836975;
            }
          } else {
            if (  (data[14].fvalue < 0.22685605)) {
              sum += (float)-0.072342776;
            } else {
              sum += (float)0.084129758;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.1201539)) {
          if (  (data[0].fvalue < 2.8116262)) {
            if (  (data[13].fvalue < 0.10800626)) {
              sum += (float)0.24608353;
            } else {
              sum += (float)0.38845488;
            }
          } else {
            if (  (data[3].fvalue < 0.76432568)) {
              sum += (float)0.22746773;
            } else {
              sum += (float)0.066275626;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.21578085)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.057611931;
            } else {
              sum += (float)0.20194769;
            }
          } else {
            if (  (data[0].fvalue < 2.9035029)) {
              sum += (float)0.28821468;
            } else {
              sum += (float)0.090060413;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 1.3906208)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[5].fvalue < 0.47759742)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.061735339;
            } else {
              sum += (float)-0.0099651245;
            }
          } else {
            if (  (data[13].fvalue < 0.16458565)) {
              sum += (float)-0.029072577;
            } else {
              sum += (float)0.034417007;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.54288006)) {
            if (  (data[13].fvalue < 0.098999307)) {
              sum += (float)0.058337703;
            } else {
              sum += (float)0.20817888;
            }
          } else {
            if (  (data[14].fvalue < 0.38988465)) {
              sum += (float)0.011930653;
            } else {
              sum += (float)0.128346;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.00982456)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.023175877;
            } else {
              sum += (float)0.043334652;
            }
          } else {
            if (  (data[14].fvalue < 0.53430116)) {
              sum += (float)0.021575261;
            } else {
              sum += (float)0.070363052;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.081338845)) {
            if (  (data[13].fvalue < 0.024144141)) {
              sum += (float)-0.25967839;
            } else {
              sum += (float)-0.098902993;
            }
          } else {
            if (  (data[16].fvalue < 0.1141341)) {
              sum += (float)-0.013781737;
            } else {
              sum += (float)0.045198955;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 0.26730961)) {
    if (  (data[15].fvalue < 0.039246444)) {
      if (  (data[9].fvalue < 0.99999899)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.068011962;
            } else {
              sum += (float)-0.00053513475;
            }
          } else {
            if (  (data[14].fvalue < 0.47677323)) {
              sum += (float)0.0010516483;
            } else {
              sum += (float)0.10288456;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 1.9859338)) {
              sum += (float)-0.28028861;
            } else {
              sum += (float)-0.025441047;
            }
          } else {
            if (  (data[16].fvalue < 0.11421526)) {
              sum += (float)-0.078443728;
            } else {
              sum += (float)0.0098030269;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.019396033)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.004651343)) {
              sum += (float)0.02834115;
            } else {
              sum += (float)0.18454877;
            }
          } else {
            if (  (data[5].fvalue < 0.65000343)) {
              sum += (float)0.03552806;
            } else {
              sum += (float)-0.21369199;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.01010752)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.15222777;
            } else {
              sum += (float)0.12768298;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.073205084;
            } else {
              sum += (float)-0.072403751;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.14694053)) {
        if (  (data[15].fvalue < 0.10492856)) {
          if (  (data[13].fvalue < 0.17517692)) {
            if (  (data[14].fvalue < 0.015982576)) {
              sum += (float)-0.094524257;
            } else {
              sum += (float)0.033272602;
            }
          } else {
            if (  (data[4].fvalue < 1.3958175)) {
              sum += (float)0.11628488;
            } else {
              sum += (float)0.0080114147;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.01727964)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.0085397447;
            } else {
              sum += (float)0.4062337;
            }
          } else {
            if (  (data[4].fvalue < 1.682722)) {
              sum += (float)0.091179416;
            } else {
              sum += (float)-0.044947654;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.004651343)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.25620437;
            } else {
              sum += (float)-0.07380344;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.014745549;
            } else {
              sum += (float)-0.22457057;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.1653212)) {
              sum += (float)-0.024638936;
            } else {
              sum += (float)0.048913721;
            }
          } else {
            if (  (data[5].fvalue < 0.71720344)) {
              sum += (float)-0.040922306;
            } else {
              sum += (float)-0.18287067;
            }
          }
        }
      }
    }
  } else {
    if (  (data[5].fvalue < 0.59162617)) {
      if (  (data[13].fvalue < 0.13655253)) {
        if (  (data[14].fvalue < 0.015982576)) {
          if (  (data[5].fvalue < 0.23903048)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.030831676;
            } else {
              sum += (float)0.38247779;
            }
          } else {
            if (  (data[3].fvalue < 2.5388947)) {
              sum += (float)-0.13113779;
            } else {
              sum += (float)0.3132602;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.31157446)) {
            if (  (data[4].fvalue < 1.682722)) {
              sum += (float)0.10315713;
            } else {
              sum += (float)0.012698201;
            }
          } else {
            if (  (data[14].fvalue < 0.42933303)) {
              sum += (float)-0.0047006463;
            } else {
              sum += (float)0.028308505;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.6286725)) {
          if (  (data[3].fvalue < 0.49178818)) {
            if (  (data[13].fvalue < 0.26168445)) {
              sum += (float)0.076184496;
            } else {
              sum += (float)0.15540135;
            }
          } else {
            if (  (data[14].fvalue < 0.01010752)) {
              sum += (float)0.010015905;
            } else {
              sum += (float)0.086590543;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.62290418)) {
            if (  (data[13].fvalue < 0.84001434)) {
              sum += (float)0.0014789139;
            } else {
              sum += (float)-0.05112993;
            }
          } else {
            if (  (data[16].fvalue < 0.060344797)) {
              sum += (float)0.050745513;
            } else {
              sum += (float)0.011330098;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.22833945)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.004651343)) {
            if ( (data[4].fvalue < 1.5785064)) {
              sum += (float)-0.085025474;
            } else {
              sum += (float)-0.0026129535;
            }
          } else {
            if (  (data[13].fvalue < 0.29930648)) {
              sum += (float)-0.037664618;
            } else {
              sum += (float)0.10649471;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.095501848;
            } else {
              sum += (float)0.013707665;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.022949591;
            } else {
              sum += (float)-0.27937603;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 1.4939885)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.073955067;
            } else {
              sum += (float)-0.017664397;
            }
          } else {
            if (  (data[2].fvalue < 1.4221085)) {
              sum += (float)0.013536903;
            } else {
              sum += (float)-0.097012825;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[0].fvalue < 3.4757988)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.088557802;
            }
          } else {
            sum += (float)-0.20287716;
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 1.020699)) {
    if (  (data[4].fvalue < 1.0275097)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.0047336998)) {
              sum += (float)-0.40412524;
            } else {
              sum += (float)-0.34427431;
            }
          } else {
            if (  (data[15].fvalue < 0.49115342)) {
              sum += (float)0.10307113;
            } else {
              sum += (float)0.0013615161;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0035347666)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.1844117;
            } else {
              sum += (float)-0.25633121;
            }
          } else {
            if (  (data[14].fvalue < 0.0047336998)) {
              sum += (float)-0.013516408;
            } else {
              sum += (float)-0.06011742;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.16153644)) {
          if (  (data[4].fvalue < 0.99594635)) {
            if (  (data[14].fvalue < 0.029166052)) {
              sum += (float)0.32437643;
            } else {
              sum += (float)0.50795722;
            }
          } else {
            if (  (data[14].fvalue < 0.052871097)) {
              sum += (float)0.058442552;
            } else {
              sum += (float)0.38504753;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.056409553)) {
            if (  (data[14].fvalue < 0.071547449)) {
              sum += (float)-0.37177312;
            } else {
              sum += (float)-0.022760879;
            }
          } else {
            if (  (data[3].fvalue < 0.62689227)) {
              sum += (float)0.26833218;
            } else {
              sum += (float)-0.012238483;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 0.073304221)) {
          if (  (data[4].fvalue < 1.7424679)) {
            if (  (data[14].fvalue < 0.061871652)) {
              sum += (float)-0.06515085;
            } else {
              sum += (float)-0.0045032543;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.12145067;
            } else {
              sum += (float)0.0012522595;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.0047336998)) {
            if (  (data[3].fvalue < 1.2785095)) {
              sum += (float)0.0088091018;
            } else {
              sum += (float)-0.022514692;
            }
          } else {
            if (  (data[14].fvalue < 0.061871652)) {
              sum += (float)0.066635698;
            } else {
              sum += (float)0.011255676;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1192164)) {
          if (  (data[3].fvalue < 0.37858161)) {
            if (  (data[13].fvalue < 0.11775614)) {
              sum += (float)0.0086401971;
            } else {
              sum += (float)0.28378806;
            }
          } else {
            if (  (data[13].fvalue < 0.16521317)) {
              sum += (float)-0.16552985;
            } else {
              sum += (float)-0.010071571;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.57004946)) {
            if (  (data[15].fvalue < 0.10416728)) {
              sum += (float)-0.054659404;
            } else {
              sum += (float)0.026447212;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.028557237;
            } else {
              sum += (float)-0.046787471;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 2.999999)) {
      if (  (data[7].fvalue < 0.99999899)) {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.0047336998)) {
            sum += (float)-0.46124923;
          } else {
            sum += (float)-0.35065442;
          }
        } else {
          if (  (data[15].fvalue < 0.087445483)) {
            if (  (data[14].fvalue < 0.42957297)) {
              sum += (float)-0.038701177;
            } else {
              sum += (float)0.044773281;
            }
          } else {
            if (  (data[15].fvalue < 0.49115342)) {
              sum += (float)0.215325;
            } else {
              sum += (float)0.033978771;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.0035347666)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.11532263;
            } else {
              sum += (float)0.25378048;
            }
          } else {
            sum += (float)-0.26774952;
          }
        } else {
          if (  (data[14].fvalue < 0.0047336998)) {
            if (  (data[13].fvalue < 0.056409553)) {
              sum += (float)0.10311691;
            } else {
              sum += (float)0.0014419203;
            }
          } else {
            if (  (data[13].fvalue < 0.01726944)) {
              sum += (float)-0.11808333;
            } else {
              sum += (float)-0.014255023;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.9748919)) {
        if (  (data[13].fvalue < 0.073304221)) {
          if (  (data[3].fvalue < 0.36130682)) {
            if (  (data[14].fvalue < 0.061871652)) {
              sum += (float)0.23823972;
            } else {
              sum += (float)0.49584302;
            }
          } else {
            if (  (data[14].fvalue < 0.0819932)) {
              sum += (float)-0.28666124;
            } else {
              sum += (float)0.092589617;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.67861813)) {
            if (  (data[14].fvalue < 0.061871652)) {
              sum += (float)0.26125747;
            } else {
              sum += (float)0.47232953;
            }
          } else {
            if (  (data[14].fvalue < 0.10461599)) {
              sum += (float)-0;
            } else {
              sum += (float)0.32890916;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.1733166)) {
            if (  (data[13].fvalue < 0.21642955)) {
              sum += (float)-0.37843725;
            } else {
              sum += (float)-0.093075514;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.094717957;
            } else {
              sum += (float)-0.27949315;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.6821537)) {
            if (  (data[15].fvalue < 0.19024463)) {
              sum += (float)-0.3541832;
            } else {
              sum += (float)-0.21015522;
            }
          } else {
            sum += (float)-0.12876253;
          }
        }
      }
    }
  }
  if (  (data[11].fvalue < 2.999999)) {
    if (  (data[17].fvalue < 0.23025598)) {
      if (  (data[11].fvalue < 1.999999)) {
        if (  (data[17].fvalue < 0.041363213)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 1.0684409)) {
              sum += (float)-0.0077596758;
            } else {
              sum += (float)0.015192795;
            }
          } else {
            if ( (data[4].fvalue < 1.1391644)) {
              sum += (float)0.015507737;
            } else {
              sum += (float)-0.016991576;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.396409)) {
            if (  (data[17].fvalue < 0.13747832)) {
              sum += (float)-0.025134474;
            } else {
              sum += (float)0.084251493;
            }
          } else {
            if (  (data[14].fvalue < 0.0044755018)) {
              sum += (float)-0.082245417;
            } else {
              sum += (float)-0.029486159;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.3764484)) {
          if ( (data[4].fvalue < 1.2010118)) {
            sum += (float)-0.38224414;
          } else {
            if (  (data[13].fvalue < 0.34190875)) {
              sum += (float)-0.28412849;
            } else {
              sum += (float)-0.14150737;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.9871728)) {
              sum += (float)-0.17012173;
            } else {
              sum += (float)0.046256408;
            }
          } else {
            if (  (data[15].fvalue < 0.19073598)) {
              sum += (float)0.040512256;
            } else {
              sum += (float)-0.084534593;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[11].fvalue < 1.999999)) {
          if (  (data[17].fvalue < 1.7142825)) {
            if (  (data[16].fvalue < 0.0036855168)) {
              sum += (float)0.00805673;
            } else {
              sum += (float)-0.05287056;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.18368557;
            } else {
              sum += (float)0.0086166468;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.90773183)) {
            if (  (data[14].fvalue < 0.68353891)) {
              sum += (float)0.014892952;
            } else {
              sum += (float)-0.1387644;
            }
          } else {
            if (  (data[15].fvalue < 0.018616036)) {
              sum += (float)0.17744413;
            } else {
              sum += (float)0.010369319;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.45069274)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.0520836;
            } else {
              sum += (float)0.15285663;
            }
          } else {
            if (  (data[4].fvalue < 1.7134085)) {
              sum += (float)0.23218217;
            } else {
              sum += (float)0.068638079;
            }
          }
        } else {
          if (  (data[0].fvalue < 2.7677886)) {
            if (  (data[17].fvalue < 0.58419341)) {
              sum += (float)0.037547059;
            } else {
              sum += (float)0.12755279;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.072600469;
            } else {
              sum += (float)-0.012087122;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.7134085)) {
      if (  (data[9].fvalue < 0.99999899)) {
        if (  (data[14].fvalue < 0.0099193742)) {
          if (  (data[17].fvalue < 0.90773183)) {
            sum += (float)-0.25131634;
          } else {
            if (  (data[0].fvalue < 3.2161009)) {
              sum += (float)-0.19187285;
            } else {
              sum += (float)-0.012037186;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.90773183)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.19417717;
            } else {
              sum += (float)-0.084170461;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if (  (data[14].fvalue < 0.22672716)) {
          sum += (float)0.089000821;
        } else {
          sum += (float)-0.071234226;
        }
      }
    } else {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[17].fvalue < 0.90773183)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.16592418)) {
              sum += (float)-0.064182244;
            } else {
              sum += (float)-0.23189589;
            }
          } else {
            if (  (data[14].fvalue < 0.19002005)) {
              sum += (float)0.18221904;
            } else {
              sum += (float)-0.033245757;
            }
          }
        } else {
          if (  (data[11].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 0.27450803)) {
              sum += (float)-0.052509818;
            } else {
              sum += (float)0.058981344;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.14888078;
            } else {
              sum += (float)0.021158135;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[3].fvalue < 1.0177687)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.062234141;
            } else {
              sum += (float)0.12793085;
            }
          } else {
            if (  (data[13].fvalue < 0.37313721)) {
              sum += (float)0.057832401;
            } else {
              sum += (float)-0.02717139;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.39004591)) {
            if (  (data[15].fvalue < 0.62403303)) {
              sum += (float)0.019283656;
            } else {
              sum += (float)-0.13488296;
            }
          } else {
            if (  (data[0].fvalue < 3.1595726)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.15147385;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 4.999999)) {
    if (  (data[8].fvalue < 4.999999)) {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[4].fvalue < 1.0403178)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.19086118;
            } else {
              sum += (float)-0.005502061;
            }
          } else {
            if (  (data[13].fvalue < 0.032837749)) {
              sum += (float)-0.0099466685;
            } else {
              sum += (float)0.011115641;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.019138267)) {
              sum += (float)0.13242188;
            } else {
              sum += (float)-0.00029103321;
            }
          } else {
            if (  (data[6].fvalue < 0.076037571)) {
              sum += (float)0.11451717;
            } else {
              sum += (float)-0.066299208;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1203114)) {
          if (  (data[3].fvalue < 0.41549292)) {
            if (  (data[13].fvalue < 0.073958918)) {
              sum += (float)-0.016102603;
            } else {
              sum += (float)0.18065168;
            }
          } else {
            if (  (data[13].fvalue < 0.10947056)) {
              sum += (float)-0.19163574;
            } else {
              sum += (float)-0.013801075;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.13018449)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.075380132;
            } else {
              sum += (float)0.013068731;
            }
          } else {
            if (  (data[13].fvalue < 0.18591873)) {
              sum += (float)-0.10177125;
            } else {
              sum += (float)0.021492384;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.68378478)) {
        if (  (data[3].fvalue < 0.22043788)) {
          if (  (data[4].fvalue < 1.7764193)) {
            if (  (data[17].fvalue < 0.069229923)) {
              sum += (float)0.074873783;
            } else {
              sum += (float)0.25218609;
            }
          } else {
            if (  (data[17].fvalue < 0.0993881)) {
              sum += (float)-0.16277555;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.18010576)) {
            if (  (data[15].fvalue < 0.49270356)) {
              sum += (float)-0.040193524;
            } else {
              sum += (float)0.078595325;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.095590606;
            } else {
              sum += (float)-0.018250341;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.12790324)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.14459294;
            } else {
              sum += (float)-0.029569307;
            }
          } else {
            if (  (data[13].fvalue < 0.073958918)) {
              sum += (float)-0.28423092;
            } else {
              sum += (float)-0.04342201;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.20284308;
            } else {
              sum += (float)0.10777444;
            }
          } else {
            if (  (data[13].fvalue < 0.49032754)) {
              sum += (float)0.044043679;
            } else {
              sum += (float)-0.12037358;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.76491582)) {
      if (  (data[4].fvalue < 1.0841999)) {
        if (  (data[3].fvalue < 0.36218667)) {
          if (  (data[13].fvalue < 0.1563219)) {
            if (  (data[3].fvalue < 0.14762343)) {
              sum += (float)0.32989755;
            } else {
              sum += (float)0.18325298;
            }
          } else {
            sum += (float)0.36147141;
          }
        } else {
          if (  (data[13].fvalue < 0.18591873)) {
            if (  (data[14].fvalue < 0.1050178)) {
              sum += (float)-0.17624415;
            } else {
              sum += (float)0.13123924;
            }
          } else {
            if (  (data[14].fvalue < 0.02223485)) {
              sum += (float)0.084746495;
            } else {
              sum += (float)0.30507022;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.49032754)) {
          if (  (data[17].fvalue < 0.0993881)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.046250202;
            } else {
              sum += (float)0.090676382;
            }
          } else {
            if (  (data[4].fvalue < 1.4387419)) {
              sum += (float)0.31834465;
            } else {
              sum += (float)0.05874303;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8116845)) {
            if (  (data[4].fvalue < 1.4171076)) {
              sum += (float)0.18655235;
            } else {
              sum += (float)0.094276503;
            }
          } else {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.081475288;
            } else {
              sum += (float)-0.078726046;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.84043491)) {
        if (  (data[5].fvalue < 1.2195216)) {
          if (  (data[15].fvalue < 0.44236705)) {
            if (  (data[17].fvalue < 0.48143271)) {
              sum += (float)-0.069385894;
            } else {
              sum += (float)0.043702234;
            }
          } else {
            if (  (data[14].fvalue < 0.43080136)) {
              sum += (float)0.094271094;
            } else {
              sum += (float)-0.035457741;
            }
          }
        } else {
          if (  (data[3].fvalue < 2.2810247)) {
            if (  (data[13].fvalue < 0.1563219)) {
              sum += (float)-0.074535705;
            } else {
              sum += (float)0.23104371;
            }
          } else {
            sum += (float)-0.065345146;
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 0.071846351)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.089981735;
            } else {
              sum += (float)-0.10142609;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.18215558;
            } else {
              sum += (float)0.092931233;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.15831473)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)-0.15822911;
            } else {
              sum += (float)-0.2887336;
            }
          } else {
            if (  (data[14].fvalue < 0.78855926)) {
              sum += (float)-0.028202707;
            } else {
              sum += (float)0.11859836;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.22685337)) {
    if (  (data[8].fvalue < 1.999999)) {
      if (  (data[13].fvalue < 0.37334052)) {
        if (  (data[17].fvalue < 0.90419012)) {
          if (  (data[15].fvalue < 1.1677235)) {
            if (  (data[4].fvalue < 1.7770309)) {
              sum += (float)-0.0036482648;
            } else {
              sum += (float)0.099960841;
            }
          } else {
            if (  (data[5].fvalue < 0.29293719)) {
              sum += (float)-0.040778056;
            } else {
              sum += (float)-0.15564147;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.20076653;
            } else {
              sum += (float)-0.045696229;
            }
          } else {
            if (  (data[11].fvalue < 2.999999)) {
              sum += (float)0.11140398;
            } else {
              sum += (float)-0.064807706;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.0095198471)) {
          if (  (data[4].fvalue < 2.188637)) {
            if (  (data[3].fvalue < 0.26578966)) {
              sum += (float)-0.041671842;
            } else {
              sum += (float)0.00067497295;
            }
          } else {
            if (  (data[13].fvalue < 1.691057)) {
              sum += (float)0.086879872;
            } else {
              sum += (float)-0.056962136;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.0041941451)) {
              sum += (float)0.095665164;
            } else {
              sum += (float)-0.036457133;
            }
          } else {
            if (  (data[14].fvalue < 0.036058623)) {
              sum += (float)-0.040726837;
            } else {
              sum += (float)0.033550713;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.29716927)) {
        if (  (data[4].fvalue < 1.1564795)) {
          if (  (data[13].fvalue < 0.048983194)) {
            if (  (data[4].fvalue < 0.97710925)) {
              sum += (float)0.15357269;
            } else {
              sum += (float)-0.056039352;
            }
          } else {
            if (  (data[4].fvalue < 0.98681986)) {
              sum += (float)0.39117286;
            } else {
              sum += (float)0.15877533;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.089361303;
            } else {
              sum += (float)0.076961085;
            }
          } else {
            if (  (data[4].fvalue < 1.2781765)) {
              sum += (float)0.14754966;
            } else {
              sum += (float)-0.017974908;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.044004966)) {
          if (  (data[8].fvalue < 4.999999)) {
            if (  (data[14].fvalue < 0.021728154)) {
              sum += (float)-0.18210453;
            } else {
              sum += (float)-0.1009101;
            }
          } else {
            if (  (data[14].fvalue < 0.021728154)) {
              sum += (float)0.30713436;
            } else {
              sum += (float)0.04116378;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.10542707)) {
            if (  (data[13].fvalue < 0.040862609)) {
              sum += (float)-0.15432286;
            } else {
              sum += (float)-0.030545203;
            }
          } else {
            if (  (data[13].fvalue < 0.3275975)) {
              sum += (float)0.047444552;
            } else {
              sum += (float)-0.027916567;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.3396304)) {
      if (  (data[3].fvalue < 0.89858228)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.010672548;
            } else {
              sum += (float)0.066042133;
            }
          } else {
            if (  (data[13].fvalue < 0.03303672)) {
              sum += (float)-0.10977918;
            } else {
              sum += (float)0.087214686;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.14694437)) {
            if (  (data[4].fvalue < 1.1206696)) {
              sum += (float)0.18670055;
            } else {
              sum += (float)0.016110228;
            }
          } else {
            if (  (data[4].fvalue < 1.1872342)) {
              sum += (float)0.3355512;
            } else {
              sum += (float)0.19115807;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.60008913)) {
          if (  (data[3].fvalue < 2.282207)) {
            if (  (data[13].fvalue < 0.44669265)) {
              sum += (float)-0.01738645;
            } else {
              sum += (float)0.070785165;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.10350496;
            } else {
              sum += (float)-0.29780832;
            }
          }
        } else {
          if (  (data[3].fvalue < 2.5406952)) {
            if (  (data[15].fvalue < 0.019891638)) {
              sum += (float)0.10221995;
            } else {
              sum += (float)0.042673182;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.029472925;
            } else {
              sum += (float)-0.08157596;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.71467966)) {
        if (  (data[15].fvalue < 0.44296363)) {
          if (  (data[13].fvalue < 0.42674831)) {
            if (  (data[4].fvalue < 1.6295563)) {
              sum += (float)-0.0083119106;
            } else {
              sum += (float)-0.064280525;
            }
          } else {
            if (  (data[15].fvalue < 0.019891638)) {
              sum += (float)0.060506981;
            } else {
              sum += (float)-0.020108854;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.0041941451)) {
              sum += (float)0.027555745;
            } else {
              sum += (float)-0.011615501;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)-0.051240075;
            } else {
              sum += (float)-0.1548472;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.999999)) {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.072867915)) {
              sum += (float)0.11239831;
            } else {
              sum += (float)0.057374544;
            }
          } else {
            if (  (data[17].fvalue < 1.1899587)) {
              sum += (float)-0.017910574;
            } else {
              sum += (float)0.038091365;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[5].fvalue < 0.32600406)) {
              sum += (float)-0.042256989;
            } else {
              sum += (float)0.069316901;
            }
          } else {
            if (  (data[9].fvalue < 2.999999)) {
              sum += (float)-0.11179151;
            } else {
              sum += (float)-0.36542729;
            }
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.84582448)) {
    if (  (data[5].fvalue < 1.5161723)) {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.20681725)) {
            if (  (data[3].fvalue < 0.55664629)) {
              sum += (float)-0.078118935;
            } else {
              sum += (float)0.13896056;
            }
          } else {
            if (  (data[3].fvalue < 0.65244734)) {
              sum += (float)-0.47931403;
            } else {
              sum += (float)0.030560814;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 1.9294418)) {
              sum += (float)0.016092964;
            } else {
              sum += (float)0.1895285;
            }
          } else {
            if (  (data[13].fvalue < 0.049013041)) {
              sum += (float)-0.27119151;
            } else {
              sum += (float)-0.11426906;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.10058854)) {
          if (  (data[14].fvalue < 0.12980793)) {
            if (  (data[3].fvalue < 0.67889833)) {
              sum += (float)-0.24703269;
            } else {
              sum += (float)-0.42791495;
            }
          } else {
            if (  (data[3].fvalue < 0.7952978)) {
              sum += (float)0.11061107;
            } else {
              sum += (float)-0.21651924;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.022478081;
            } else {
              sum += (float)0.079217069;
            }
          } else {
            if (  (data[13].fvalue < 0.77688384)) {
              sum += (float)-0.24956915;
            } else {
              sum += (float)0.001090723;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.13738611)) {
        if (  (data[8].fvalue < 0.99999899)) {
          if (  (data[3].fvalue < 0.47235462)) {
            if (  (data[13].fvalue < 0.10965623)) {
              sum += (float)0.11904897;
            } else {
              sum += (float)-0.37620801;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.31705356;
            } else {
              sum += (float)0.13125682;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 0.0040067928)) {
              sum += (float)0.17186221;
            } else {
              sum += (float)0.033663206;
            }
          } else {
            if (  (data[3].fvalue < 0.7952978)) {
              sum += (float)0.28051561;
            } else {
              sum += (float)0.12243403;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.5345245)) {
          if (  (data[13].fvalue < 0.14687875)) {
            sum += (float)-0.2872687;
          } else {
            sum += (float)-0.55222261;
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 1.4161476)) {
              sum += (float)0.024047805;
            } else {
              sum += (float)0.24860758;
            }
          } else {
            if (  (data[14].fvalue < 0.052238014)) {
              sum += (float)0.037527889;
            } else {
              sum += (float)0.34139186;
            }
          }
        }
      }
    }
  } else {
    if (  (data[0].fvalue < 1.6286153)) {
      if (  (data[6].fvalue < 0.02637342)) {
        if (  (data[13].fvalue < 0.017837601)) {
          sum += (float)-0.39426503;
        } else {
          sum += (float)-0.16495325;
        }
      } else {
        if (  (data[5].fvalue < 0.28224182)) {
          if (  (data[17].fvalue < 0.13452329)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.0039128112;
            } else {
              sum += (float)0.052133363;
            }
          } else {
            if (  (data[13].fvalue < 0.040881261)) {
              sum += (float)-0.005843455;
            } else {
              sum += (float)0.11231823;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.44644871)) {
              sum += (float)-0.0070067002;
            } else {
              sum += (float)0.020367978;
            }
          } else {
            if (  (data[6].fvalue < 0.29328355)) {
              sum += (float)0.15210654;
            } else {
              sum += (float)0.0055014589;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[7].fvalue < 11.999999)) {
          if (  (data[14].fvalue < 0.39003178)) {
            if (  (data[15].fvalue < 1.949092)) {
              sum += (float)-0.0052017011;
            } else {
              sum += (float)-0.053102992;
            }
          } else {
            if (  (data[6].fvalue < 0.35445878)) {
              sum += (float)0.041106209;
            } else {
              sum += (float)-0.00071422983;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.60030895)) {
            if (  (data[7].fvalue < 13.999999)) {
              sum += (float)0.15201597;
            } else {
              sum += (float)0.2527374;
            }
          } else {
            if (  (data[8].fvalue < 8.999999)) {
              sum += (float)0.15456592;
            } else {
              sum += (float)-0.022892613;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 4.999999)) {
          sum += (float)0.17515618;
        } else {
          sum += (float)0.30041939;
        }
      }
    }
  }
  if (  (data[3].fvalue < 0.11847575)) {
    if (  (data[8].fvalue < 2.999999)) {
      if (  (data[4].fvalue < 1.0403618)) {
        if (  (data[13].fvalue < 0.065571301)) {
          if (  (data[7].fvalue < 0.99999899)) {
            sum += (float)-0.33829248;
          } else {
            if (  (data[14].fvalue < 0.0038697275)) {
              sum += (float)0.068601921;
            } else {
              sum += (float)-0.0571118;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.10956272)) {
              sum += (float)-0.14039968;
            } else {
              sum += (float)-0.31416622;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.04937784;
            } else {
              sum += (float)0.31194204;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1204469)) {
          if (  (data[4].fvalue < 1.0680735)) {
            if (  (data[13].fvalue < 0.14684781)) {
              sum += (float)0.10503726;
            } else {
              sum += (float)-0.24378189;
            }
          } else {
            if (  (data[13].fvalue < 0.1187454)) {
              sum += (float)0.062574856;
            } else {
              sum += (float)0.25744447;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.12480508)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.022753727;
            } else {
              sum += (float)0.050544254;
            }
          } else {
            if (  (data[13].fvalue < 0.01779319)) {
              sum += (float)0.01019984;
            } else {
              sum += (float)0.075168774;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1388772)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.071278602)) {
            if (  (data[4].fvalue < 0.98682672)) {
              sum += (float)0.32011893;
            } else {
              sum += (float)-0.12607311;
            }
          } else {
            if (  (data[4].fvalue < 1.0285456)) {
              sum += (float)0.4471882;
            } else {
              sum += (float)0.18483075;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0068662)) {
            sum += (float)0.41555259;
          } else {
            if (  (data[13].fvalue < 0.048953261)) {
              sum += (float)0.10800903;
            } else {
              sum += (float)0.34651199;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 0.27061692)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.1101466;
            } else {
              sum += (float)0.081442542;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.11247028;
            } else {
              sum += (float)-0.012984631;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3226175)) {
            if (  (data[13].fvalue < 0.2625103)) {
              sum += (float)0.1248386;
            } else {
              sum += (float)0.44475812;
            }
          } else {
            if (  (data[14].fvalue < 0.38956356)) {
              sum += (float)0.037039898;
            } else {
              sum += (float)0.16630787;
            }
          }
        }
      }
    }
  } else {
    if (  (data[5].fvalue < 1.0188305)) {
      if (  (data[13].fvalue < 0.30002579)) {
        if (  (data[4].fvalue < 1.7126694)) {
          if (  (data[3].fvalue < 0.60270989)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.0053694765;
            } else {
              sum += (float)0.15074337;
            }
          } else {
            if (  (data[14].fvalue < 0.35410422)) {
              sum += (float)-0.062176723;
            } else {
              sum += (float)-0.00058578199;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.7885291)) {
              sum += (float)0.019503519;
            } else {
              sum += (float)-0.015812274;
            }
          } else {
            if (  (data[14].fvalue < 0.2080445)) {
              sum += (float)-0.31158048;
            } else {
              sum += (float)0.06401813;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.5071068)) {
          if (  (data[5].fvalue < 0.62973636)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.050327264;
            } else {
              sum += (float)0.010073207;
            }
          } else {
            if (  (data[3].fvalue < 0.49235961)) {
              sum += (float)-0.12362996;
            } else {
              sum += (float)-0.0012694256;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.47899881)) {
            if (  (data[15].fvalue < 0.442866)) {
              sum += (float)-0.028794972;
            } else {
              sum += (float)0.018107275;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.031071946;
            } else {
              sum += (float)-0.10507915;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 3.999999)) {
        if (  (data[13].fvalue < 0.074124366)) {
          if (  (data[14].fvalue < 0.0038697275)) {
            if (  (data[3].fvalue < 0.20531709)) {
              sum += (float)0.01412098;
            } else {
              sum += (float)0.14518921;
            }
          } else {
            if (  (data[13].fvalue < 0.0039844126)) {
              sum += (float)0.092496634;
            } else {
              sum += (float)-0.029700229;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.28040931)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.2482892;
            } else {
              sum += (float)0.28906366;
            }
          } else {
            if (  (data[13].fvalue < 0.12797374)) {
              sum += (float)0.062678881;
            } else {
              sum += (float)-0.0021692121;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 1.5814255)) {
          if (  (data[3].fvalue < 0.60270989)) {
            if (  (data[14].fvalue < 0.028410621)) {
              sum += (float)0.15239327;
            } else {
              sum += (float)0.34731585;
            }
          } else {
            if (  (data[13].fvalue < 0.10956272)) {
              sum += (float)-0.22859667;
            } else {
              sum += (float)0.14246355;
            }
          }
        } else {
          if (  (data[5].fvalue < 1.3273108)) {
            if (  (data[13].fvalue < 0.38983944)) {
              sum += (float)-0.22704272;
            } else {
              sum += (float)-0.058559433;
            }
          } else {
            if (  (data[13].fvalue < 0.35717177)) {
              sum += (float)0.0075831879;
            } else {
              sum += (float)0.1907497;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.15860073)) {
    if (  (data[8].fvalue < 1.999999)) {
      if (  (data[15].fvalue < 0.041202661)) {
        if (  (data[13].fvalue < 0.0040684324)) {
          if (  (data[4].fvalue < 1.5306977)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.26318994;
            } else {
              sum += (float)0.11163538;
            }
          } else {
            if (  (data[17].fvalue < 0.70975721)) {
              sum += (float)0.3337234;
            } else {
              sum += (float)-0.19895077;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.057292227)) {
            if (  (data[13].fvalue < 0.057410929)) {
              sum += (float)0.022976762;
            } else {
              sum += (float)-0.062908724;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.0060028401;
            } else {
              sum += (float)-0.040396757;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.12503956)) {
          if (  (data[5].fvalue < 0.30368778)) {
            if (  (data[17].fvalue < 0.001945457)) {
              sum += (float)-0.0011727752;
            } else {
              sum += (float)-0.075242124;
            }
          } else {
            if (  (data[14].fvalue < 0.0093576815)) {
              sum += (float)-0.08293841;
            } else {
              sum += (float)-0.017530158;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.18659729)) {
            if (  (data[3].fvalue < 0.23568016)) {
              sum += (float)0.017079893;
            } else {
              sum += (float)-0.046726905;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)8.2480234e-05;
            } else {
              sum += (float)0.039729811;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.044005439)) {
        if (  (data[3].fvalue < 0.089323886)) {
          if (  (data[4].fvalue < 0.99707228)) {
            if (  (data[13].fvalue < 0.010832585)) {
              sum += (float)0.10687152;
            } else {
              sum += (float)0.48710248;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.079728745;
            } else {
              sum += (float)0.0897476;
            }
          }
        } else {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[15].fvalue < 0.089769512)) {
              sum += (float)-0.14940423;
            } else {
              sum += (float)-0.04735443;
            }
          } else {
            if (  (data[13].fvalue < 0.1099491)) {
              sum += (float)0.15385053;
            } else {
              sum += (float)-0.068222828;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.066886984)) {
          if (  (data[0].fvalue < 3.0518281)) {
            if (  (data[13].fvalue < 0.017908439)) {
              sum += (float)0.10671502;
            } else {
              sum += (float)0.37030235;
            }
          } else {
            if (  (data[13].fvalue < 0.025433021)) {
              sum += (float)-0.095384926;
            } else {
              sum += (float)0.10033191;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.025433021)) {
              sum += (float)-0.14249542;
            } else {
              sum += (float)-0.02202085;
            }
          } else {
            if (  (data[5].fvalue < 0.82601488)) {
              sum += (float)0.032209262;
            } else {
              sum += (float)-0.18402454;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.3070065)) {
      if (  (data[3].fvalue < 0.57969564)) {
        if (  (data[13].fvalue < 0.12840904)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.010052879;
            } else {
              sum += (float)0.087100595;
            }
          } else {
            if (  (data[4].fvalue < 1.0851244)) {
              sum += (float)0.16083889;
            } else {
              sum += (float)0.021115756;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.0092251012;
            } else {
              sum += (float)0.10759128;
            }
          } else {
            if (  (data[3].fvalue < 0.20609763)) {
              sum += (float)0.32685548;
            } else {
              sum += (float)0.16396751;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.0057159192)) {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.47878042)) {
              sum += (float)0.0011036363;
            } else {
              sum += (float)0.047278434;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.13005884;
            } else {
              sum += (float)-0.26423648;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.074353516)) {
              sum += (float)-0.19664969;
            } else {
              sum += (float)-0.024427852;
            }
          } else {
            if (  (data[7].fvalue < 5.999999)) {
              sum += (float)-0.23449655;
            } else {
              sum += (float)-0.050320018;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.11589101)) {
        if (  (data[15].fvalue < 0.32819605)) {
          if (  (data[17].fvalue < 0.89539492)) {
            if (  (data[13].fvalue < 0.3736496)) {
              sum += (float)-0.037909299;
            } else {
              sum += (float)0.011739697;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.080861226;
            } else {
              sum += (float)0.0014833446;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[5].fvalue < 0.52331567)) {
              sum += (float)0.02608799;
            } else {
              sum += (float)-0.0138638;
            }
          } else {
            if (  (data[4].fvalue < 1.8904034)) {
              sum += (float)0.017353775;
            } else {
              sum += (float)-0.065806471;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8492935)) {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.010832585)) {
              sum += (float)-0.040549174;
            } else {
              sum += (float)0.10045953;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.061478581;
            } else {
              sum += (float)-0.23951282;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.04890475;
            } else {
              sum += (float)0.044501491;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.069607437;
            } else {
              sum += (float)0.0029273322;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.47868997)) {
    if (  (data[3].fvalue < 1.0171808)) {
      if (  (data[17].fvalue < 0.18021454)) {
        if (  (data[15].fvalue < 0.16963744)) {
          if (  (data[4].fvalue < 1.2140476)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.001363766;
            } else {
              sum += (float)0.065850317;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.0077003967;
            } else {
              sum += (float)-0.063693047;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.039635237)) {
            if (  (data[13].fvalue < 1.1475776)) {
              sum += (float)0.018519405;
            } else {
              sum += (float)-0.078516051;
            }
          } else {
            if (  (data[14].fvalue < 0.0037592205)) {
              sum += (float)-0.095977291;
            } else {
              sum += (float)-0.017183028;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.00036695891;
            } else {
              sum += (float)0.088044778;
            }
          } else {
            if (  (data[4].fvalue < 1.9357387)) {
              sum += (float)-0.19119106;
            } else {
              sum += (float)-0.066536389;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8115366)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.056162581;
            } else {
              sum += (float)0.14881305;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.015544404;
            } else {
              sum += (float)0.035281666;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[15].fvalue < 0.97005123)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.071541831)) {
              sum += (float)0.36877471;
            } else {
              sum += (float)0.0024095296;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.0004190394;
            } else {
              sum += (float)-0.083522178;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.11970133)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.13011827;
            } else {
              sum += (float)0.067252964;
            }
          } else {
            if ( (data[4].fvalue < 1.7431667)) {
              sum += (float)-0.062722839;
            } else {
              sum += (float)0.027232563;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.59205705)) {
          if (  (data[13].fvalue < 0.30126026)) {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.0038955959;
            } else {
              sum += (float)0.14063032;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.0066817976;
            } else {
              sum += (float)-0.057346687;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.1479857)) {
            if (  (data[15].fvalue < 0.16963744)) {
              sum += (float)-0.22536756;
            } else {
              sum += (float)-0.10627297;
            }
          } else {
            if (  (data[14].fvalue < 0.15894218)) {
              sum += (float)-0.12548761;
            } else {
              sum += (float)-0.033770043;
            }
          }
        }
      }
    }
  } else {
    if (  (data[15].fvalue < 1.1636969)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 2.1872735)) {
              sum += (float)-0.34366581;
            } else {
              sum += (float)-0.093662582;
            }
          } else {
            if (  (data[15].fvalue < 0.27016535)) {
              sum += (float)-0.076324917;
            } else {
              sum += (float)-0.00570664;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.21852383)) {
            if ( (data[4].fvalue < 1.4834219)) {
              sum += (float)0.10028314;
            } else {
              sum += (float)0.0057332348;
            }
          } else {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)-0.067230806;
            } else {
              sum += (float)0.22028145;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.7761897)) {
          if (  (data[3].fvalue < 0.8974902)) {
            if (  (data[13].fvalue < 0.30126026)) {
              sum += (float)0.071705252;
            } else {
              sum += (float)0.16133773;
            }
          } else {
            if (  (data[14].fvalue < 0.68381345)) {
              sum += (float)-0.024511511;
            } else {
              sum += (float)0.043095753;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.11593736)) {
            if (  (data[13].fvalue < 0.6042363)) {
              sum += (float)-0.033043873;
            } else {
              sum += (float)0.05656416;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.092618465;
            } else {
              sum += (float)0.0058731502;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.0059074159)) {
        if ( (data[4].fvalue < 1.7431667)) {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.01132212)) {
              sum += (float)-0.075827681;
            } else {
              sum += (float)0.11610252;
            }
          } else {
            sum += (float)0.13823232;
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.68381345)) {
              sum += (float)0.072368309;
            } else {
              sum += (float)0.17422293;
            }
          } else {
            if (  (data[13].fvalue < 0.074940227)) {
              sum += (float)-0.10713975;
            } else {
              sum += (float)0.041365452;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 0.49557316)) {
            if (  (data[13].fvalue < 0.0045321425)) {
              sum += (float)-0.058591656;
            } else {
              sum += (float)0.0065587931;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.051488291;
            } else {
              sum += (float)-0.033758923;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 6.2839046)) {
              sum += (float)0.068097644;
            } else {
              sum += (float)-0.089724541;
            }
          } else {
            sum += (float)-0.20087741;
          }
        }
      }
    }
  }
  if (  (data[0].fvalue < 2.6774843)) {
    if (  (data[14].fvalue < 0.081647009)) {
      if (  (data[13].fvalue < 0.011619776)) {
        if (  (data[6].fvalue < 0.047696657)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.32011604;
            } else {
              sum += (float)0.15800494;
            }
          } else {
            if (  (data[14].fvalue < 0.0035815598)) {
              sum += (float)0.11134032;
            } else {
              sum += (float)-0.009163809;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.5765102)) {
            if (  (data[3].fvalue < 0.13357213)) {
              sum += (float)-0.041175686;
            } else {
              sum += (float)-0.11520413;
            }
          } else {
            if (  (data[15].fvalue < 0.1458872)) {
              sum += (float)0.19330455;
            } else {
              sum += (float)-0.016440779;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.026554016)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.026298255)) {
              sum += (float)0.003491234;
            } else {
              sum += (float)-0.35337937;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.030244481;
            } else {
              sum += (float)0.4936392;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 2.999999)) {
              sum += (float)0.00052588235;
            } else {
              sum += (float)-0.12933004;
            }
          } else {
            if (  (data[6].fvalue < 0.42098457)) {
              sum += (float)0.28596649;
            } else {
              sum += (float)0.016477695;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.17703964)) {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[4].fvalue < 1.1204329)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.0042677769;
            } else {
              sum += (float)0.24756318;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.014930474;
            } else {
              sum += (float)0.062292509;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3381561)) {
            if (  (data[13].fvalue < 0.22967546)) {
              sum += (float)0.11187709;
            } else {
              sum += (float)0.37332425;
            }
          } else {
            if (  (data[13].fvalue < 0.31473282)) {
              sum += (float)-0.022710839;
            } else {
              sum += (float)0.14088173;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.51600206)) {
          if (  (data[6].fvalue < 0.28150439)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.0082193706;
            } else {
              sum += (float)0.10414688;
            }
          } else {
            if (  (data[15].fvalue < 0.19287536)) {
              sum += (float)-0.017379142;
            } else {
              sum += (float)0.016032912;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.46431139)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.060483363;
            } else {
              sum += (float)0.19535188;
            }
          } else {
            if (  (data[14].fvalue < 0.59814155)) {
              sum += (float)0.021154344;
            } else {
              sum += (float)0.15772727;
            }
          }
        }
      }
    }
  } else {
    if (  (data[7].fvalue < 2.999999)) {
      if (  (data[0].fvalue < 4.0505695)) {
        if (  (data[9].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 1.0148511)) {
            if (  (data[4].fvalue < 2.0408549)) {
              sum += (float)-0.0070771971;
            } else {
              sum += (float)0.020702301;
            }
          } else {
            if (  (data[16].fvalue < 0.060500346)) {
              sum += (float)-0.013160417;
            } else {
              sum += (float)-0.10016886;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.59814155)) {
              sum += (float)-0.15041082;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.18152696;
            } else {
              sum += (float)0.3212781;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.061351053)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.17503667;
            } else {
              sum += (float)-0.080453947;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.010245403;
            } else {
              sum += (float)-0.064084291;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.531021)) {
            if (  (data[15].fvalue < 1.9391958)) {
              sum += (float)-0.004078602;
            } else {
              sum += (float)-0.096460938;
            }
          } else {
            if (  (data[15].fvalue < 1.1570772)) {
              sum += (float)-0.0034775599;
            } else {
              sum += (float)0.05102573;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.14818558)) {
        if (  (data[15].fvalue < 0.08963941)) {
          if (  (data[17].fvalue < 0.098769933)) {
            if (  (data[3].fvalue < 0.13357213)) {
              sum += (float)-0.027138421;
            } else {
              sum += (float)-0.17504112;
            }
          } else {
            if (  (data[14].fvalue < 0.22686189)) {
              sum += (float)0.20925863;
            } else {
              sum += (float)-0.14061773;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.22686189)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.0021372163;
            } else {
              sum += (float)0.23562239;
            }
          } else {
            if (  (data[13].fvalue < 0.083922543)) {
              sum += (float)-0.22707587;
            } else {
              sum += (float)-0.068112142;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.081647009)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 5.999999)) {
              sum += (float)0.0050146515;
            } else {
              sum += (float)0.13619104;
            }
          } else {
            if (  (data[14].fvalue < 0.028155738)) {
              sum += (float)-0.11615371;
            } else {
              sum += (float)-0.042906757;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.1501634)) {
            if (  (data[0].fvalue < 3.9255548)) {
              sum += (float)0.19836192;
            } else {
              sum += (float)0.025209194;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.010400543;
            } else {
              sum += (float)-0.025988253;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.29494584)) {
    if (  (data[7].fvalue < 8.999999)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[7].fvalue < 5.999999)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[17].fvalue < 0.039655741)) {
              sum += (float)0.0037837515;
            } else {
              sum += (float)-0.0089399843;
            }
          } else {
            if (  (data[4].fvalue < 1.5048995)) {
              sum += (float)-0.030312797;
            } else {
              sum += (float)0.02017979;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.39114511)) {
            if (  (data[4].fvalue < 1.7108588)) {
              sum += (float)-0.068820775;
            } else {
              sum += (float)0.14585119;
            }
          } else {
            if (  (data[3].fvalue < 0.4336741)) {
              sum += (float)0.16339569;
            } else {
              sum += (float)0.063621745;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 0.26397365)) {
            if (  (data[4].fvalue < 0.98699957)) {
              sum += (float)0.034923498;
            } else {
              sum += (float)-0.053396955;
            }
          } else {
            if (  (data[4].fvalue < 1.3061078)) {
              sum += (float)-0.051778551;
            } else {
              sum += (float)-0.17648563;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.2639664)) {
            if (  (data[15].fvalue < 0.073519327)) {
              sum += (float)0.031584594;
            } else {
              sum += (float)0.25080591;
            }
          } else {
            if (  (data[13].fvalue < 0.14829694)) {
              sum += (float)0.08708024;
            } else {
              sum += (float)-0.051064886;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.72579479)) {
          sum += (float)0.13567472;
        } else {
          sum += (float)0.27699611;
        }
      } else {
        if (  (data[13].fvalue < 0.44765058)) {
          if (  (data[8].fvalue < 6.999999)) {
            if (  (data[14].fvalue < 0.14352207)) {
              sum += (float)0.11818887;
            } else {
              sum += (float)-0.058586229;
            }
          } else {
            if (  (data[8].fvalue < 7.999999)) {
              sum += (float)0.16210669;
            } else {
              sum += (float)0.25631097;
            }
          }
        } else {
          if (  (data[8].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 0.77832019)) {
              sum += (float)0.021660242;
            } else {
              sum += (float)0.15092744;
            }
          } else {
            if (  (data[6].fvalue < 0.40715507)) {
              sum += (float)0.11593997;
            } else {
              sum += (float)-0.085404664;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.4817715)) {
      if (  (data[3].fvalue < 1.33957)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 0.16854234)) {
            if (  (data[13].fvalue < 0.40892294)) {
              sum += (float)0.011072608;
            } else {
              sum += (float)0.087534763;
            }
          } else {
            if (  (data[15].fvalue < 0.55259407)) {
              sum += (float)0.084786505;
            } else {
              sum += (float)-0.0072955368;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.1676483)) {
            if (  (data[4].fvalue < 1.0842272)) {
              sum += (float)0.15543178;
            } else {
              sum += (float)0.0049410951;
            }
          } else {
            if (  (data[4].fvalue < 1.1560931)) {
              sum += (float)0.278144;
            } else {
              sum += (float)0.12690942;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 1.3264986)) {
          if (  (data[14].fvalue < 0.68007416)) {
            if (  (data[13].fvalue < 0.72579479)) {
              sum += (float)-0.045993477;
            } else {
              sum += (float)0.044005942;
            }
          } else {
            if (  (data[4].fvalue < 1.0405129)) {
              sum += (float)0.068593808;
            } else {
              sum += (float)0.0031704553;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.12017038)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.13375515;
            } else {
              sum += (float)0.0023319162;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.13549472;
            } else {
              sum += (float)0.29107043;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.96400523)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.39974758)) {
              sum += (float)-0.049856871;
            } else {
              sum += (float)-0.00179892;
            }
          } else {
            if (  (data[4].fvalue < 1.8477618)) {
              sum += (float)0.23360418;
            } else {
              sum += (float)0.077913672;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.24287708)) {
              sum += (float)0.10275697;
            } else {
              sum += (float)0.034929968;
            }
          } else {
            if (  (data[13].fvalue < 0.0048959721)) {
              sum += (float)-0.28308919;
            } else {
              sum += (float)0.0018181287;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.78447413)) {
          if (  (data[4].fvalue < 2.4191027)) {
            if (  (data[5].fvalue < 0.39753869)) {
              sum += (float)0.05516918;
            } else {
              sum += (float)-0.028433368;
            }
          } else {
            if (  (data[0].fvalue < 6.2851658)) {
              sum += (float)-0.029293826;
            } else {
              sum += (float)-0.11115577;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 0.4637149)) {
              sum += (float)0.17173691;
            } else {
              sum += (float)0.047908336;
            }
          } else {
            if (  (data[15].fvalue < 1.4390252)) {
              sum += (float)-0.08466588;
            } else {
              sum += (float)0.021963015;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.24071257)) {
    if (  (data[5].fvalue < 0.97300833)) {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[14].fvalue < 0.2263637)) {
          if (  (data[17].fvalue < 1.1830046)) {
            if (  (data[4].fvalue < 1.6812593)) {
              sum += (float)-0.011342375;
            } else {
              sum += (float)0.025546763;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.13920866;
            } else {
              sum += (float)0.0032113674;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.034240946)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.00025467513;
            } else {
              sum += (float)-0.17063805;
            }
          } else {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.023636423;
            } else {
              sum += (float)0.093940206;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.099558055)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.1015362)) {
              sum += (float)0.0063673588;
            } else {
              sum += (float)-0.12179845;
            }
          } else {
            if (  (data[14].fvalue < 0.3524352)) {
              sum += (float)0.05222474;
            } else {
              sum += (float)-0.075602472;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.29437545)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.043750178;
            } else {
              sum += (float)0.16722396;
            }
          } else {
            if (  (data[13].fvalue < 0.10202324)) {
              sum += (float)-0.16196859;
            } else {
              sum += (float)-0.024668386;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.073909402)) {
        if (  (data[13].fvalue < 0.019041637)) {
          sum += (float)-0.51179367;
        } else {
          sum += (float)-0.17740746;
        }
      } else {
        if (  (data[4].fvalue < 0.9336099)) {
          if (  (data[3].fvalue < 0.10445584)) {
            if (  (data[13].fvalue < 0.011939428)) {
              sum += (float)0.013830125;
            } else {
              sum += (float)-0.39189395;
            }
          } else {
            if (  (data[3].fvalue < 0.39647382)) {
              sum += (float)-0.0062066144;
            } else {
              sum += (float)0.023518968;
            }
          }
        } else {
          if (  (data[5].fvalue < 1.0190537)) {
            if (  (data[13].fvalue < 0.093061052)) {
              sum += (float)0.048134629;
            } else {
              sum += (float)0.0015653528;
            }
          } else {
            if (  (data[3].fvalue < 0.36158022)) {
              sum += (float)0.11992554;
            } else {
              sum += (float)0.31080773;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 0.11657921)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[4].fvalue < 1.0838896)) {
          if (  (data[3].fvalue < 0.60184717)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.062220674;
            } else {
              sum += (float)0.19756643;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.002835216;
            } else {
              sum += (float)0.36631301;
            }
          }
        } else {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[3].fvalue < 0.8256768)) {
              sum += (float)0.018756786;
            } else {
              sum += (float)-0.00098239479;
            }
          } else {
            if (  (data[13].fvalue < 0.72515434)) {
              sum += (float)-0.066142507;
            } else {
              sum += (float)0.052114971;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.2770581)) {
          if (  (data[3].fvalue < 0.51267737)) {
            if (  (data[4].fvalue < 1.1381605)) {
              sum += (float)0.43849054;
            } else {
              sum += (float)0.12741458;
            }
          } else {
            if (  (data[3].fvalue < 1.0139085)) {
              sum += (float)-0.013481789;
            } else {
              sum += (float)-0.20999858;
            }
          }
        } else {
          if (  (data[7].fvalue < 5.999999)) {
            if (  (data[4].fvalue < 1.374213)) {
              sum += (float)-0.10490111;
            } else {
              sum += (float)-0.19023374;
            }
          } else {
            if (  (data[8].fvalue < 7.999999)) {
              sum += (float)-0.079776622;
            } else {
              sum += (float)0.19548032;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.37988657)) {
        if (  (data[0].fvalue < 2.3439467)) {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.042250983;
            } else {
              sum += (float)0.16448016;
            }
          } else {
            if (  (data[6].fvalue < 0.2700772)) {
              sum += (float)0.33635685;
            } else {
              sum += (float)0.14406408;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.23781388)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.016645437;
            } else {
              sum += (float)0.096138522;
            }
          } else {
            if (  (data[13].fvalue < 0.49132174)) {
              sum += (float)-0.013279554;
            } else {
              sum += (float)0.055417441;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 1.3226033)) {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.036479734;
            } else {
              sum += (float)0.14003043;
            }
          } else {
            if (  (data[6].fvalue < 0.50939655)) {
              sum += (float)0.27521771;
            } else {
              sum += (float)0.090872772;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.67889243)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.004599629;
            } else {
              sum += (float)-0.034104139;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.045925297;
            } else {
              sum += (float)0.064758748;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.18754862)) {
    if (  (data[6].fvalue < 0.14996856)) {
      if (  (data[7].fvalue < 0.99999899)) {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[5].fvalue < 0.065147422)) {
              sum += (float)-0.20421334;
            } else {
              sum += (float)-0.33863354;
            }
          } else {
            if (  (data[14].fvalue < 0.035121504)) {
              sum += (float)-0.32899004;
            } else {
              sum += (float)0.066574097;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[0].fvalue < 2.0941396)) {
              sum += (float)0.1217163;
            } else {
              sum += (float)-0.019829201;
            }
          } else {
            if (  (data[14].fvalue < 0.070644028)) {
              sum += (float)-0.018977843;
            } else {
              sum += (float)0.11838556;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 0.0031090376)) {
              sum += (float)0.01449328;
            } else {
              sum += (float)-0.0071920571;
            }
          } else {
            if (  (data[0].fvalue < 2.0941396)) {
              sum += (float)0.24684957;
            } else {
              sum += (float)0.015767021;
            }
          }
        } else {
          sum += (float)0.32839993;
        }
      }
    } else {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[15].fvalue < 1.9412248)) {
          if (  (data[14].fvalue < 0.0084790904)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.045632504;
            } else {
              sum += (float)-0.034410268;
            }
          } else {
            if (  (data[13].fvalue < 0.0049618906)) {
              sum += (float)-0.0056944122;
            } else {
              sum += (float)0.018446229;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.47466639)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.11650055;
            } else {
              sum += (float)-0.01492287;
            }
          } else {
            if (  (data[14].fvalue < 0.92183995)) {
              sum += (float)-0.00071605179;
            } else {
              sum += (float)0.089491345;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.52442008)) {
          if (  (data[13].fvalue < 0.042082358)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.095814109;
            } else {
              sum += (float)0.14990716;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.024895366;
            } else {
              sum += (float)-0.022513457;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.058480889)) {
            if (  (data[0].fvalue < 0.72983438)) {
              sum += (float)-0.028879784;
            } else {
              sum += (float)-0.19212773;
            }
          } else {
            if (  (data[3].fvalue < 0.39693704)) {
              sum += (float)0.039266467;
            } else {
              sum += (float)-0.079397917;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.094970047)) {
      if (  (data[8].fvalue < 0.99999899)) {
        if (  (data[0].fvalue < 2.6355095)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[0].fvalue < 2.4262514)) {
              sum += (float)-0.48053843;
            } else {
              sum += (float)-0.24670045;
            }
          } else {
            if (  (data[3].fvalue < 0.20621631)) {
              sum += (float)0.049144268;
            } else {
              sum += (float)-0.13843067;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.076333798)) {
            if (  (data[0].fvalue < 3.5498731)) {
              sum += (float)-0.22195946;
            } else {
              sum += (float)-0.031345826;
            }
          } else {
            if (  (data[13].fvalue < 0.21887432)) {
              sum += (float)0.1429451;
            } else {
              sum += (float)-0.041789658;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 4.3656688)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.0031090376)) {
              sum += (float)0.26269144;
            } else {
              sum += (float)-0.060634192;
            }
          } else {
            if (  (data[0].fvalue < 2.9501753)) {
              sum += (float)0.5002377;
            } else {
              sum += (float)0.18551613;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.057372384)) {
            if (  (data[4].fvalue < 1.1378702)) {
              sum += (float)0.18520363;
            } else {
              sum += (float)-0.0083859516;
            }
          } else {
            if (  (data[14].fvalue < 0.15775004)) {
              sum += (float)-0.012721588;
            } else {
              sum += (float)0.098590016;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.4587419)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.17317839)) {
              sum += (float)0.0052281404;
            } else {
              sum += (float)0.031032514;
            }
          } else {
            if ( (data[4].fvalue < 1.2254707)) {
              sum += (float)-0.21069674;
            } else {
              sum += (float)-0.084749386;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[6].fvalue < 0.52533948)) {
              sum += (float)0.34594986;
            } else {
              sum += (float)0.17893755;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.053628638;
            } else {
              sum += (float)0.35401061;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.40072501)) {
          if (  (data[16].fvalue < 0.85068077)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.031362232;
            } else {
              sum += (float)-0.0016932812;
            }
          } else {
            if (  (data[15].fvalue < 0.073922262)) {
              sum += (float)0.11134514;
            } else {
              sum += (float)0.028022302;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.04103392)) {
              sum += (float)0.038090535;
            } else {
              sum += (float)-0.00095333875;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.18601266;
            } else {
              sum += (float)0.023022795;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 4.999999)) {
    if (  (data[1].fvalue < 0.2614356)) {
      if (  (data[3].fvalue < 0.2502543)) {
        if (  (data[0].fvalue < 1.6269902)) {
          if (  (data[4].fvalue < 1.2379655)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.036579844;
            } else {
              sum += (float)0.14252678;
            }
          } else {
            if (  (data[3].fvalue < 0.10440835)) {
              sum += (float)0.16679965;
            } else {
              sum += (float)0.096952163;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.06898737)) {
              sum += (float)0.1048187;
            } else {
              sum += (float)-0.023130773;
            }
          } else {
            if (  (data[2].fvalue < 0.47481516)) {
              sum += (float)-0.025050087;
            } else {
              sum += (float)-0.11326092;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.38543785)) {
          if (  (data[3].fvalue < 0.45215958)) {
            if (  (data[13].fvalue < 0.34391281)) {
              sum += (float)0.017770579;
            } else {
              sum += (float)0.071401864;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.012586487;
            } else {
              sum += (float)-0.056986455;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.51659232)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.019008443;
            } else {
              sum += (float)-0.036642414;
            }
          } else {
            if (  (data[3].fvalue < 0.76181704)) {
              sum += (float)0.091847949;
            } else {
              sum += (float)0.0035502624;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 11.999999)) {
        if (  (data[10].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.42748019)) {
            if (  (data[6].fvalue < 0.25893256)) {
              sum += (float)0.0025865482;
            } else {
              sum += (float)-0.005388665;
            }
          } else {
            if ( (data[4].fvalue < 1.5278995)) {
              sum += (float)0.025438791;
            } else {
              sum += (float)-0.001498899;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.31101832;
            } else {
              sum += (float)-0.11550429;
            }
          } else {
            if ( (data[4].fvalue < 2.6419845)) {
              sum += (float)0.094329558;
            } else {
              sum += (float)-0.087418303;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 2.0791318)) {
          if (  (data[13].fvalue < 0.57240748)) {
            if (  (data[8].fvalue < 12.999999)) {
              sum += (float)0.16885112;
            } else {
              sum += (float)0.24761502;
            }
          } else {
            if (  (data[0].fvalue < 3.3345866)) {
              sum += (float)0.15385547;
            } else {
              sum += (float)-0.0016180737;
            }
          }
        } else {
          if (  (data[7].fvalue < 7.999999)) {
            sum += (float)0.087726444;
          } else {
            if (  (data[14].fvalue < 0.92016745)) {
              sum += (float)-0.012188071;
            } else {
              sum += (float)-0.13937631;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.2971316;
  }
  if (  (data[3].fvalue < 2.2745664)) {
    if (  (data[14].fvalue < 0.20654373)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[7].fvalue < 5.999999)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[11].fvalue < 2.999999)) {
              sum += (float)0.0022258954;
            } else {
              sum += (float)-0.068102464;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.0067393747;
            } else {
              sum += (float)-0.030663641;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.034868598)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.10261179;
            } else {
              sum += (float)-0.059131432;
            }
          } else {
            if (  (data[13].fvalue < 0.27682441)) {
              sum += (float)-0.053265464;
            } else {
              sum += (float)0.12540904;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.32827052)) {
          if (  (data[4].fvalue < 0.99650592)) {
            if (  (data[14].fvalue < 0.070230886)) {
              sum += (float)0.098684251;
            } else {
              sum += (float)0.3237932;
            }
          } else {
            if (  (data[14].fvalue < 0.0083063729)) {
              sum += (float)0.40300825;
            } else {
              sum += (float)0.0006114025;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.2175477)) {
            if (  (data[5].fvalue < 1.2170676)) {
              sum += (float)-0.077546135;
            } else {
              sum += (float)0.077464968;
            }
          } else {
            if (  (data[13].fvalue < 0.23059702)) {
              sum += (float)0.12041542;
            } else {
              sum += (float)-0.074979015;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.2502806)) {
        if (  (data[3].fvalue < 0.41471016)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.025010109;
            } else {
              sum += (float)0.12702601;
            }
          } else {
            if (  (data[13].fvalue < 0.1025662)) {
              sum += (float)0.11381099;
            } else {
              sum += (float)0.23092964;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.017453866;
            } else {
              sum += (float)-0.019003704;
            }
          } else {
            if (  (data[13].fvalue < 0.1025662)) {
              sum += (float)-0.031283237;
            } else {
              sum += (float)0.082078494;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.57245547)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.005216565)) {
              sum += (float)-0.13913341;
            } else {
              sum += (float)-0.012937693;
            }
          } else {
            if (  (data[15].fvalue < 0.021204676)) {
              sum += (float)0.078145117;
            } else {
              sum += (float)0.0035341436;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8870399)) {
            if (  (data[14].fvalue < 0.59508908)) {
              sum += (float)0.054803345;
            } else {
              sum += (float)0.14153367;
            }
          } else {
            if (  (data[14].fvalue < 0.59508908)) {
              sum += (float)-0.043495703;
            } else {
              sum += (float)0.067183807;
            }
          }
        }
      }
    }
  } else {
    if (  (data[7].fvalue < 2.999999)) {
      if (  (data[13].fvalue < 0.60457057)) {
        if (  (data[13].fvalue < 0.16841817)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.026822975)) {
              sum += (float)0.43190688;
            } else {
              sum += (float)-0.072773404;
            }
          } else {
            if (  (data[15].fvalue < 1.9279569)) {
              sum += (float)-0.0055893492;
            } else {
              sum += (float)-0.068075769;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.0029498057)) {
              sum += (float)-0.072604626;
            } else {
              sum += (float)-0.017854258;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.25733382;
            } else {
              sum += (float)-0.080485217;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 1.6853542)) {
              sum += (float)-0.026437057;
            } else {
              sum += (float)0.090053625;
            }
          } else {
            if (  (data[14].fvalue < 0.18894492)) {
              sum += (float)0.10788717;
            } else {
              sum += (float)-0.014218361;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 0.038313966)) {
              sum += (float)0.18331397;
            } else {
              sum += (float)0.066143632;
            }
          } else {
            if (  (data[14].fvalue < 0.014164939)) {
              sum += (float)-0.20635411;
            } else {
              sum += (float)-0.01521624;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 0.82770103)) {
        if (  (data[11].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.043239102;
            } else {
              sum += (float)0.0060290839;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.038307842;
            } else {
              sum += (float)-0.14333597;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.59291494)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.035315759;
            } else {
              sum += (float)-0.13897556;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.029735828;
            } else {
              sum += (float)-0.2496185;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.1448429)) {
          if (  (data[14].fvalue < 1.1226948)) {
            if (  (data[5].fvalue < 1.2170676)) {
              sum += (float)-0.18500352;
            } else {
              sum += (float)-0.064610451;
            }
          } else {
            if (  (data[14].fvalue < 1.4794351)) {
              sum += (float)-0.064489149;
            } else {
              sum += (float)0.018486919;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.046109758;
            } else {
              sum += (float)0.081779331;
            }
          } else {
            sum += (float)-0.07079155;
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 0.22720532)) {
    if (  (data[11].fvalue < 1.999999)) {
      if (  (data[16].fvalue < 0.20203495)) {
        if ( (data[4].fvalue < 1.1860809)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.00010142028;
            } else {
              sum += (float)0.030106872;
            }
          } else {
            if (  (data[6].fvalue < 0.46341497)) {
              sum += (float)0.35127711;
            } else {
              sum += (float)0.17304415;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.42747909)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.0063232007;
            } else {
              sum += (float)-0.040656038;
            }
          } else {
            if (  (data[17].fvalue < 0.068568982)) {
              sum += (float)0.014327352;
            } else {
              sum += (float)-0.038566854;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.015498686;
            } else {
              sum += (float)0.027781609;
            }
          } else {
            if (  (data[0].fvalue < 4.362215)) {
              sum += (float)0.23065588;
            } else {
              sum += (float)0.062081959;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.887375)) {
            if (  (data[3].fvalue < 1.7854778)) {
              sum += (float)0.11977877;
            } else {
              sum += (float)-0.00010037465;
            }
          } else {
            if (  (data[16].fvalue < 0.84913838)) {
              sum += (float)-0.0027304236;
            } else {
              sum += (float)0.06319695;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 0.99999899)) {
        if ( (data[4].fvalue < 2.0399184)) {
          if ( (data[4].fvalue < 1.1997373)) {
            sum += (float)-0.26391542;
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.10521137;
            } else {
              sum += (float)-0.19016498;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.39117518)) {
            sum += (float)0.33038658;
          } else {
            if (  (data[4].fvalue < 2.2822165)) {
              sum += (float)-0.17979223;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.24352114)) {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 0.50860763)) {
              sum += (float)0.046356879;
            } else {
              sum += (float)-0.18462542;
            }
          } else {
            if (  (data[6].fvalue < 0.66020256)) {
              sum += (float)0.36862615;
            } else {
              sum += (float)0.19772224;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.1143549)) {
            if (  (data[17].fvalue < 0.068568982)) {
              sum += (float)0.031579539;
            } else {
              sum += (float)-0.11700414;
            }
          } else {
            if (  (data[13].fvalue < 0.83921719)) {
              sum += (float)-0.026182281;
            } else {
              sum += (float)0.1531323;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.31615818)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 0.0050816932)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.37021825;
            } else {
              sum += (float)-0.1227349;
            }
          } else {
            if (  (data[5].fvalue < 0.304039)) {
              sum += (float)-0.015745688;
            } else {
              sum += (float)0.11002979;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.28051081)) {
            if (  (data[4].fvalue < 1.3375001)) {
              sum += (float)0.14683288;
            } else {
              sum += (float)0.06474749;
            }
          } else {
            if ( (data[4].fvalue < 1.1997373)) {
              sum += (float)0.082968146;
            } else {
              sum += (float)0.017948026;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.3551117)) {
          if (  (data[3].fvalue < 1.2107862)) {
            sum += (float)0.42379826;
          } else {
            sum += (float)0.088881508;
          }
        } else {
          if (  (data[17].fvalue < 0.39994758)) {
            if (  (data[0].fvalue < 5.1584363)) {
              sum += (float)0.072272412;
            } else {
              sum += (float)-0.059601311;
            }
          } else {
            if (  (data[4].fvalue < 1.6525202)) {
              sum += (float)0.23295395;
            } else {
              sum += (float)0.044883065;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.0028565631)) {
        if (  (data[13].fvalue < 0.084581949)) {
          if (  (data[17].fvalue < 1.1730525)) {
            if (  (data[4].fvalue < 1.9325851)) {
              sum += (float)-0.13581893;
            } else {
              sum += (float)0.14889508;
            }
          } else {
            if (  (data[4].fvalue < 2.641005)) {
              sum += (float)-0.23474304;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.40020493)) {
            if (  (data[15].fvalue < 0.022147235)) {
              sum += (float)0.00702486;
            } else {
              sum += (float)-0.052242156;
            }
          } else {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)0.065862201;
            } else {
              sum += (float)-0.03532793;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7733487)) {
          if (  (data[3].fvalue < 0.73199856)) {
            if (  (data[13].fvalue < 0.0050816932)) {
              sum += (float)0.0087606423;
            } else {
              sum += (float)0.087536819;
            }
          } else {
            if (  (data[15].fvalue < 0.16956425)) {
              sum += (float)0.005842994;
            } else {
              sum += (float)0.062327079;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.89185244)) {
            if (  (data[15].fvalue < 0.32851553)) {
              sum += (float)-0.041469406;
            } else {
              sum += (float)0.0074490537;
            }
          } else {
            if (  (data[4].fvalue < 2.2822165)) {
              sum += (float)0.050965652;
            } else {
              sum += (float)-0.013418122;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 0.43677908)) {
    if (  (data[4].fvalue < 0.96709114)) {
      if (  (data[3].fvalue < 0.023036065)) {
        if (  (data[13].fvalue < 0.019088721)) {
          sum += (float)-0.57788312;
        } else {
          sum += (float)-0.27961767;
        }
      } else {
        if (  (data[13].fvalue < 0.011936744)) {
          if (  (data[0].fvalue < 2.0936136)) {
            sum += (float)0.083520681;
          } else {
            sum += (float)0.21130244;
          }
        } else {
          sum += (float)-0.3159;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1549374)) {
        if (  (data[13].fvalue < 0.18790555)) {
          if (  (data[14].fvalue < 0.080722384)) {
            if (  (data[4].fvalue < 0.99647462)) {
              sum += (float)0.058370896;
            } else {
              sum += (float)-0.0198333;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.040311322;
            } else {
              sum += (float)0.13392933;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0832254)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.19520028;
            } else {
              sum += (float)0.11272289;
            }
          } else {
            if (  (data[4].fvalue < 1.1373935)) {
              sum += (float)0.20483555;
            } else {
              sum += (float)0.092967935;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.39948842)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.02424055;
            } else {
              sum += (float)0.016028188;
            }
          } else {
            if (  (data[15].fvalue < 0.022399345)) {
              sum += (float)0.082165606;
            } else {
              sum += (float)0.003070642;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.46843639)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.0034679936;
            } else {
              sum += (float)0.036476567;
            }
          } else {
            if (  (data[15].fvalue < 0.022399345)) {
              sum += (float)0.0588351;
            } else {
              sum += (float)0.0077244244;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.040962704)) {
      if (  (data[4].fvalue < 0.95682096)) {
        if (  (data[13].fvalue < 0.019088721)) {
          sum += (float)-0.53589904;
        } else {
          sum += (float)-0.22761793;
        }
      } else {
        if (  (data[13].fvalue < 0.011936744)) {
          sum += (float)0.15570836;
        } else {
          sum += (float)-0.2594358;
        }
      }
    } else {
      if (  (data[14].fvalue < 0.0028918544)) {
        if (  (data[13].fvalue < 0.034451026)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[5].fvalue < 1.2166699)) {
              sum += (float)0.17168796;
            } else {
              sum += (float)0.051961102;
            }
          } else {
            if (  (data[3].fvalue < 0.11892065)) {
              sum += (float)-0.11873167;
            } else {
              sum += (float)-0.4572522;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.026543856)) {
            if (  (data[0].fvalue < 3.2727284)) {
              sum += (float)-0.29889745;
            } else {
              sum += (float)-0.079062246;
            }
          } else {
            if (  (data[13].fvalue < 0.084564127)) {
              sum += (float)0.072840072;
            } else {
              sum += (float)-0.0049535064;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.0082360599)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 0.57755357)) {
              sum += (float)-0.086533062;
            } else {
              sum += (float)0.042349957;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.17374444;
            } else {
              sum += (float)0.28531435;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.11146415)) {
            if (  (data[14].fvalue < 0.11612953)) {
              sum += (float)-0.056142267;
            } else {
              sum += (float)-0.0088705886;
            }
          } else {
            if (  (data[3].fvalue < 0.36116943)) {
              sum += (float)0.12328831;
            } else {
              sum += (float)-0.0036549997;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 4.999999)) {
    if (  (data[7].fvalue < 13.999999)) {
      if (  (data[14].fvalue < 0.014097645)) {
        if (  (data[14].fvalue < 0.0028241896)) {
          if (  (data[15].fvalue < 0.022063889)) {
            if (  (data[13].fvalue < 0.042449869)) {
              sum += (float)0.062173355;
            } else {
              sum += (float)0.00043330275;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.025351185;
            } else {
              sum += (float)0.032493901;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.026754811)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.13380668;
            } else {
              sum += (float)0.3271493;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.042444892;
            } else {
              sum += (float)-0.055459663;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.073557317)) {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)0.029144054;
            } else {
              sum += (float)0.085404351;
            }
          } else {
            if (  (data[4].fvalue < 1.6253773)) {
              sum += (float)0.15014128;
            } else {
              sum += (float)-0.013377267;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.093579985)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.0024624048;
            } else {
              sum += (float)-0.040093549;
            }
          } else {
            if (  (data[6].fvalue < 0.1498441)) {
              sum += (float)0.049772874;
            } else {
              sum += (float)0.0032969848;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.73485458)) {
        sum += (float)0.24787553;
      } else {
        sum += (float)0.11448532;
      }
    }
  } else {
    sum += (float)0.28784457;
  }
  if (  (data[15].fvalue < 0.1941748)) {
    if (  (data[4].fvalue < 2.1053803)) {
      if (  (data[15].fvalue < 0.058683075)) {
        if (  (data[14].fvalue < 0.52970958)) {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 2.999999)) {
              sum += (float)-0.0019603448;
            } else {
              sum += (float)-0.075702913;
            }
          } else {
            if (  (data[9].fvalue < 0.99999899)) {
              sum += (float)-0.22386779;
            } else {
              sum += (float)-0.075130343;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.042204592;
            } else {
              sum += (float)-0.037617866;
            }
          } else {
            if (  (data[3].fvalue < 0.67708373)) {
              sum += (float)0.13867821;
            } else {
              sum += (float)0.034273304;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.10739959)) {
          if (  (data[5].fvalue < 0.54062009)) {
            if (  (data[9].fvalue < 1.999999)) {
              sum += (float)-0.029597817;
            } else {
              sum += (float)0.097360045;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.042288654;
            } else {
              sum += (float)-0.11535714;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3933644)) {
            if (  (data[14].fvalue < 0.0083883004)) {
              sum += (float)-0.011072738;
            } else {
              sum += (float)0.04376103;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.042903386;
            } else {
              sum += (float)0.0099598151;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 1.335865)) {
        if (  (data[17].fvalue < 0.33371902)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.52970958)) {
              sum += (float)0.18623672;
            } else {
              sum += (float)0.017626647;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.16093513;
            } else {
              sum += (float)-0.13471107;
            }
          }
        } else {
          if (  (data[17].fvalue < 1.1694435)) {
            if (  (data[4].fvalue < 2.4169273)) {
              sum += (float)-0.078717038;
            } else {
              sum += (float)0.036419608;
            }
          } else {
            if (  (data[13].fvalue < 0.63919473)) {
              sum += (float)-0.002324278;
            } else {
              sum += (float)0.11719213;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 1.6787697)) {
          if (  (data[16].fvalue < 0.84846681)) {
            if (  (data[14].fvalue < 0.59644955)) {
              sum += (float)-0.081711188;
            } else {
              sum += (float)0.12430289;
            }
          } else {
            if (  (data[15].fvalue < 0.10739959)) {
              sum += (float)0.14908297;
            } else {
              sum += (float)0.055560365;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.30340764;
            } else {
              sum += (float)0.0068652323;
            }
          } else {
            if (  (data[0].fvalue < 6.2760739)) {
              sum += (float)-0.026391465;
            } else {
              sum += (float)0.081851289;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.36632967)) {
      if (  (data[14].fvalue < 0.0083883004)) {
        if (  (data[13].fvalue < 0.026773537)) {
          if (  (data[5].fvalue < 0.20753248)) {
            if (  (data[4].fvalue < 1.2505453)) {
              sum += (float)0.1154287;
            } else {
              sum += (float)-0.056198668;
            }
          } else {
            if (  (data[3].fvalue < 0.23571102)) {
              sum += (float)-0.065646522;
            } else {
              sum += (float)-0.23928913;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.37337556)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.038377389;
            } else {
              sum += (float)-0.040392227;
            }
          } else {
            if (  (data[13].fvalue < 0.34359092)) {
              sum += (float)-0.035482377;
            } else {
              sum += (float)-0.0052634631;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if ( (data[4].fvalue < 1.3209336)) {
            if (  (data[3].fvalue < 0.62576902)) {
              sum += (float)0.11722378;
            } else {
              sum += (float)0.036332216;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.018579697;
            } else {
              sum += (float)0.061359048;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.4803556)) {
            if ( (data[4].fvalue < 1.3374346)) {
              sum += (float)0.28345147;
            } else {
              sum += (float)0.16950533;
            }
          } else {
            if (  (data[15].fvalue < 0.62562144)) {
              sum += (float)-0.0038517865;
            } else {
              sum += (float)0.16005866;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 1.7297096)) {
        if (  (data[13].fvalue < 0.15865642)) {
          if (  (data[14].fvalue < 0.014323181)) {
            if (  (data[13].fvalue < 0.026773537)) {
              sum += (float)-0.10855756;
            } else {
              sum += (float)-0.020297324;
            }
          } else {
            if (  (data[3].fvalue < 0.39678955)) {
              sum += (float)0.059595391;
            } else {
              sum += (float)0.0046231323;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.43662724)) {
            if (  (data[0].fvalue < 1.390094)) {
              sum += (float)0.11270534;
            } else {
              sum += (float)0.046541709;
            }
          } else {
            if (  (data[14].fvalue < 0.027538367)) {
              sum += (float)-0.021487348;
            } else {
              sum += (float)0.08917734;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.27777407)) {
          if (  (data[15].fvalue < 0.40122041)) {
            if ( (data[4].fvalue < 1.5752088)) {
              sum += (float)0.0065852683;
            } else {
              sum += (float)-0.034201171;
            }
          } else {
            if (  (data[13].fvalue < 0.28899902)) {
              sum += (float)-0.0086270059;
            } else {
              sum += (float)0.015160188;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.0394948)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.030319352;
            } else {
              sum += (float)0.37891841;
            }
          } else {
            if (  (data[14].fvalue < 0.020644879)) {
              sum += (float)0.022618279;
            } else {
              sum += (float)-0.013495499;
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
