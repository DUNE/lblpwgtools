#include "BDTReweighter.h"
class GeneratorReweight_NEUT : public BDTReweighter {
 public :
 GeneratorReweight_NEUT() : BDTReweighter(1.0) {;}
  ~GeneratorReweight_NEUT() {;}


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
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[12].fvalue < 5.999999)) {
      if (  (data[10].fvalue < 0.021820793)) {
        if (  (data[9].fvalue < 0.91017288)) {
          sum += (float)0.99997556;
        } else {
          if (  (data[9].fvalue < 1.7701931)) {
            if (  (data[6].fvalue < 0.25681219)) {
              sum += (float)-0.737005;
            } else {
              sum += (float)-0.046601817;
            }
          } else {
            if (  (data[8].fvalue < 0.18987454)) {
              sum += (float)0.51912302;
            } else {
              sum += (float)0.27938268;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 11.002527)) {
          if (  (data[6].fvalue < 0.25681219)) {
            if (  (data[8].fvalue < 0.090420641)) {
              sum += (float)-0.42920664;
            } else {
              sum += (float)0.039266437;
            }
          } else {
            if (  (data[22].fvalue < 1.2157236)) {
              sum += (float)0.072692178;
            } else {
              sum += (float)-0.0074601858;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.46351376)) {
            if (  (data[19].fvalue < 2.8669026)) {
              sum += (float)0.16291232;
            } else {
              sum += (float)0.0051992438;
            }
          } else {
            if (  (data[11].fvalue < 0.81177199)) {
              sum += (float)0.23414925;
            } else {
              sum += (float)0.44349283;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.30476224)) {
        if (  (data[12].fvalue < 7.999999)) {
          if (  (data[19].fvalue < 0.12673262)) {
            if (  (data[8].fvalue < 0.38750172)) {
              sum += (float)-0.89836955;
            } else {
              sum += (float)-0.69752145;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.35283458;
            } else {
              sum += (float)-0.55044103;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.16585025)) {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.90060478;
            } else {
              sum += (float)-0.97792721;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.71426296;
            } else {
              sum += (float)-0.93077183;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 8.999999)) {
          if (  (data[8].fvalue < 7.4023657)) {
            if (  (data[19].fvalue < 2.1442318)) {
              sum += (float)-0.18571039;
            } else {
              sum += (float)-0.44529673;
            }
          } else {
            if (  (data[19].fvalue < 2.5263844)) {
              sum += (float)0.039537501;
            } else {
              sum += (float)-0.25129062;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.63475084)) {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.5365907;
            } else {
              sum += (float)-0.8138541;
            }
          } else {
            if (  (data[22].fvalue < 0.01899909)) {
              sum += (float)-0.55752647;
            } else {
              sum += (float)-0.25737518;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 2.999999)) {
      if ( (data[9].fvalue < 4.7955155)) {
        if ( (data[9].fvalue < 1.8360838)) {
          sum += (float)-0.99854487;
        } else {
          if (  (data[8].fvalue < 12.099914)) {
            if (  (data[9].fvalue < 2.3650348)) {
              sum += (float)-0.877864;
            } else {
              sum += (float)-0.95313805;
            }
          } else {
            if (  (data[8].fvalue < 28.968685)) {
              sum += (float)-0.92098367;
            } else {
              sum += (float)-0.85164726;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 15.309718)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[8].fvalue < 0.22615947)) {
              sum += (float)-0.81310248;
            } else {
              sum += (float)-0.92371857;
            }
          } else {
            if (  (data[19].fvalue < 0.35792974)) {
              sum += (float)-0.98226726;
            } else {
              sum += (float)-0.94019496;
            }
          }
        } else {
          if (  (data[8].fvalue < 28.968685)) {
            if (  (data[22].fvalue < 0.71767873)) {
              sum += (float)-0.91829622;
            } else {
              sum += (float)-0.88866389;
            }
          } else {
            if (  (data[10].fvalue < 0.60450453)) {
              sum += (float)-0.86149305;
            } else {
              sum += (float)-0.79590589;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 3.999999)) {
        if (  (data[8].fvalue < 11.002527)) {
          if (  (data[9].fvalue < 4.6576619)) {
            sum += (float)-0.98155969;
          } else {
            sum += (float)-0.96655548;
          }
        } else {
          if (  (data[22].fvalue < 0.064506061)) {
            sum += (float)-0.97239453;
          } else {
            sum += (float)-0.9423064;
          }
        }
      } else {
        sum += (float)-0.99974841;
      }
    }
  }
  if (  (data[13].fvalue < 5.999999)) {
    if (  (data[21].fvalue < 0.11041)) {
      if (  (data[21].fvalue < 0.022479948)) {
        if (  (data[22].fvalue < 0.1046379)) {
          if (  (data[20].fvalue < 0.12806837)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.12131818;
            } else {
              sum += (float)-0.12508729;
            }
          } else {
            if (  (data[20].fvalue < 3.766726)) {
              sum += (float)-0.038311072;
            } else {
              sum += (float)0.23716262;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.054906085)) {
            if (  (data[11].fvalue < 0.035772935)) {
              sum += (float)-0.14484316;
            } else {
              sum += (float)0.1446275;
            }
          } else {
            if (  (data[20].fvalue < 0.019331466)) {
              sum += (float)-0.041142128;
            } else {
              sum += (float)0.032490503;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 1.0508498)) {
            if (  (data[22].fvalue < 0.022004282)) {
              sum += (float)0.33852425;
            } else {
              sum += (float)0.4748646;
            }
          } else {
            if (  (data[11].fvalue < 0.18837525)) {
              sum += (float)0.10130495;
            } else {
              sum += (float)0.27024916;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.10180762)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.1930791;
            } else {
              sum += (float)0.35816899;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.18532544;
            } else {
              sum += (float)0.062144395;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[22].fvalue < 0.022004282)) {
          if (  (data[20].fvalue < 0.019331466)) {
            if (  (data[21].fvalue < 3.7664945)) {
              sum += (float)-0.089830793;
            } else {
              sum += (float)0.17691748;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.1051246;
            } else {
              sum += (float)-0.10890621;
            }
          }
        } else {
          if (  (data[21].fvalue < 1.141682)) {
            if (  (data[22].fvalue < 0.97362435)) {
              sum += (float)0.19740571;
            } else {
              sum += (float)0.045411501;
            }
          } else {
            if (  (data[10].fvalue < 0.012721796)) {
              sum += (float)0.31122288;
            } else {
              sum += (float)0.024092367;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[20].fvalue < 0.10180762)) {
            if (  (data[11].fvalue < 0.042501107)) {
              sum += (float)-0.27514109;
            } else {
              sum += (float)0.035166249;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.17889179;
            } else {
              sum += (float)-0.044960789;
            }
          }
        } else {
          if (  (data[21].fvalue < 4.6067448)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.74339169;
            } else {
              sum += (float)0.42234585;
            }
          } else {
            if (  (data[10].fvalue < 0.24704632)) {
              sum += (float)0.092229933;
            } else {
              sum += (float)0.41044623;
            }
          }
        }
      }
    }
  } else {
    if (  (data[15].fvalue < 2.999999)) {
      if (  (data[14].fvalue < 2.999999)) {
        if (  (data[11].fvalue < 0.049701825)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[11].fvalue < 0.016360058)) {
              sum += (float)-0.73196971;
            } else {
              sum += (float)-0.41517389;
            }
          } else {
            if (  (data[11].fvalue < 0.030089391)) {
              sum += (float)-0.75922501;
            } else {
              sum += (float)-0.63029969;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[21].fvalue < 1.6203732)) {
              sum += (float)-0.268805;
            } else {
              sum += (float)-0.38216218;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.48111555;
            } else {
              sum += (float)-0.27060527;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 3.999999)) {
          if (  (data[20].fvalue < 3.0925648)) {
            if (  (data[22].fvalue < 0.022004282)) {
              sum += (float)-0.1127735;
            } else {
              sum += (float)0.22475511;
            }
          } else {
            if (  (data[21].fvalue < 2.0953672)) {
              sum += (float)-0.11532111;
            } else {
              sum += (float)-0.36674985;
            }
          }
        } else {
          if (  (data[20].fvalue < 4.5313349)) {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)0.56238461;
            } else {
              sum += (float)0.9197436;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)0.11480348;
            } else {
              sum += (float)0.67444414;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 3.999999)) {
        if (  (data[21].fvalue < 2.8961833)) {
          if (  (data[22].fvalue < 0.022004282)) {
            if (  (data[20].fvalue < 0.10180762)) {
              sum += (float)0.42507896;
            } else {
              sum += (float)-0.017743517;
            }
          } else {
            if (  (data[10].fvalue < 0.28308567)) {
              sum += (float)0.36097398;
            } else {
              sum += (float)0.65023857;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.30092615)) {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)-0.16849315;
            } else {
              sum += (float)0.45565385;
            }
          } else {
            if (  (data[10].fvalue < 0.44789481)) {
              sum += (float)0.11265558;
            } else {
              sum += (float)0.38341367;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 4.9550996)) {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[10].fvalue < 0.20631875)) {
              sum += (float)0.60918444;
            } else {
              sum += (float)0.83101779;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.94386488;
            } else {
              sum += (float)1.0547369;
            }
          }
        } else {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[10].fvalue < 0.25506303)) {
              sum += (float)0.13866509;
            } else {
              sum += (float)0.54310483;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)0.66291189;
            } else {
              sum += (float)0.97754598;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 4.999999)) {
    if (  (data[12].fvalue < 6.999999)) {
      if (  (data[19].fvalue < 0.089106344)) {
        if (  (data[18].fvalue < 0.0288326)) {
          if (  (data[19].fvalue < 0.023097081)) {
            if (  (data[6].fvalue < 0.23597084)) {
              sum += (float)-0.30695203;
            } else {
              sum += (float)-0.64613563;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.082768239;
            } else {
              sum += (float)-0.4192673;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.2073219;
            } else {
              sum += (float)-0.35516956;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.45003161;
            } else {
              sum += (float)-0.017558426;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.23623341;
            } else {
              sum += (float)-0.30031765;
            }
          } else {
            if (  (data[18].fvalue < 0.0093634585)) {
              sum += (float)-0.33587554;
            } else {
              sum += (float)0.0073010148;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.30334505)) {
              sum += (float)0.053457182;
            } else {
              sum += (float)-0.24700953;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.32214415;
            } else {
              sum += (float)-0.042813823;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[19].fvalue < 0.40156615)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.65343022;
            } else {
              sum += (float)-0.30038038;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.30202281;
            } else {
              sum += (float)-0.53505194;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.8780219;
            } else {
              sum += (float)-0.65998298;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.77457398;
            } else {
              sum += (float)-0.57545942;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.7472828)) {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.27231741;
            } else {
              sum += (float)0.12174074;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.59358472;
            } else {
              sum += (float)-0.32873821;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.51942652;
            } else {
              sum += (float)-0.73193568;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.17259328;
            } else {
              sum += (float)-0.66241419;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.20927577)) {
      if (  (data[19].fvalue < 0.2573106)) {
        if (  (data[19].fvalue < 0.1814808)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[19].fvalue < 0.14025822)) {
              sum += (float)-0.86251438;
            } else {
              sum += (float)-0.72480017;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.67000377;
            } else {
              sum += (float)-0.62006372;
            }
          }
        } else {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[18].fvalue < 0.0093634585)) {
              sum += (float)-0.78767776;
            } else {
              sum += (float)-0.35225445;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.58782274;
            } else {
              sum += (float)-0.70583344;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[18].fvalue < 0.0093634585)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.4835923;
            } else {
              sum += (float)-0.69875115;
            }
          } else {
            if (  (data[19].fvalue < 1.6447685)) {
              sum += (float)-0.065254509;
            } else {
              sum += (float)-0.25901815;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.0093634585)) {
              sum += (float)-0.81531245;
            } else {
              sum += (float)-0.66573167;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.37012625;
            } else {
              sum += (float)-0.62637889;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.44583085)) {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.51976901)) {
              sum += (float)-0.34909782;
            } else {
              sum += (float)-0.75809717;
            }
          } else {
            if (  (data[19].fvalue < 0.2573106)) {
              sum += (float)-0.37154835;
            } else {
              sum += (float)-0.061518561;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.55037826)) {
              sum += (float)-0.61397314;
            } else {
              sum += (float)-0.83481127;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.36311075;
            } else {
              sum += (float)-0.6203661;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.52955616;
            } else {
              sum += (float)-0.041774645;
            }
          } else {
            if (  (data[18].fvalue < 0.51976901)) {
              sum += (float)-0.48315942;
            } else {
              sum += (float)-0.7408123;
            }
          }
        } else {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)0.065948471;
            } else {
              sum += (float)-0.24554263;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.11699804;
            } else {
              sum += (float)0.39112481;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[13].fvalue < 5.999999)) {
      if (  (data[16].fvalue < 2.999999)) {
        if ( (data[9].fvalue < 3.1025703)) {
          if (  (data[21].fvalue < 0.92572206)) {
            if (  (data[5].fvalue < 0.76382542)) {
              sum += (float)-0.1503077;
            } else {
              sum += (float)0.03726618;
            }
          } else {
            if (  (data[9].fvalue < 2.5251637)) {
              sum += (float)-0.15234217;
            } else {
              sum += (float)-0.028605947;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.10772782)) {
              sum += (float)0.34516186;
            } else {
              sum += (float)0.15541241;
            }
          } else {
            if (  (data[19].fvalue < 1.7940042)) {
              sum += (float)0.068142392;
            } else {
              sum += (float)-0.10989222;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 3.1025703)) {
            if (  (data[9].fvalue < 1.9182783)) {
              sum += (float)0.024537725;
            } else {
              sum += (float)-0.36831948;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.060300045;
            } else {
              sum += (float)-0.25659671;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 3.9688013)) {
              sum += (float)0.053307801;
            } else {
              sum += (float)-0.069666386;
            }
          } else {
            if (  (data[22].fvalue < 3.7116601)) {
              sum += (float)0.292247;
            } else {
              sum += (float)0.083203308;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.38266596)) {
        if (  (data[19].fvalue < 0.21148542)) {
          if (  (data[19].fvalue < 0.16751125)) {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.60979629;
            } else {
              sum += (float)-0.57190108;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.38949862;
            } else {
              sum += (float)-0.59018809;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if ( (data[9].fvalue < 1.0853559)) {
              sum += (float)0.22171977;
            } else {
              sum += (float)-0.30017447;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.46577445;
            } else {
              sum += (float)-0.57354569;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 3.999999)) {
          if (  (data[19].fvalue < 2.2930954)) {
            if (  (data[19].fvalue < 0.57973027)) {
              sum += (float)-0.15267757;
            } else {
              sum += (float)0.012445859;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.21250303;
            } else {
              sum += (float)0.16320579;
            }
          }
        } else {
          if (  (data[22].fvalue < 6.1362696)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.17457691;
            } else {
              sum += (float)0.46140876;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.015225742;
            } else {
              sum += (float)0.25132912;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if (  (data[16].fvalue < 1.999999)) {
        if ( (data[9].fvalue < 1.9182783)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.72776473;
            } else {
              sum += (float)-0.68499148;
            }
          } else {
            if (  (data[19].fvalue < 0.2597414)) {
              sum += (float)-0.57060963;
            } else {
              sum += (float)-0.61201614;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.99316561)) {
              sum += (float)-0.63559413;
            } else {
              sum += (float)-0.67610377;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.61884165;
            } else {
              sum += (float)-0.59018046;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 6.1248722)) {
          if (  (data[13].fvalue < 5.999999)) {
            if ( (data[9].fvalue < 4.2899919)) {
              sum += (float)-0.62508708;
            } else {
              sum += (float)-0.5956406;
            }
          } else {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)-0.56267399;
            } else {
              sum += (float)-0.45097786;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.64078724;
            } else {
              sum += (float)-0.49905375;
            }
          } else {
            if (  (data[22].fvalue < 7.1904988)) {
              sum += (float)-0.40292194;
            } else {
              sum += (float)-0.53629941;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[21].fvalue < 0.11299507)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.22630677)) {
              sum += (float)-0.72319102;
            } else {
              sum += (float)-0.73963499;
            }
          } else {
            if (  (data[9].fvalue < 2.2093379)) {
              sum += (float)-0.63481671;
            } else {
              sum += (float)-0.71261543;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.67624575)) {
              sum += (float)-0.7088443;
            } else {
              sum += (float)-0.72550344;
            }
          } else {
            if (  (data[22].fvalue < 0.10772782)) {
              sum += (float)-0.68506831;
            } else {
              sum += (float)-0.70663399;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.57973027)) {
              sum += (float)-0.68292075;
            } else {
              sum += (float)-0.65749168;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.70067054;
            } else {
              sum += (float)-0.66948247;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.44844791)) {
              sum += (float)-0.58565998;
            } else {
              sum += (float)-0.6399399;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.67627144;
            } else {
              sum += (float)-0.74382007;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[8].fvalue < 6.4594512)) {
      if (  (data[13].fvalue < 7.999999)) {
        if (  (data[21].fvalue < 0.091845781)) {
          if (  (data[21].fvalue < 0.021793496)) {
            if ( (data[9].fvalue < 2.5933688)) {
              sum += (float)-0.0021909578;
            } else {
              sum += (float)0.073967382;
            }
          } else {
            if (  (data[9].fvalue < 1.4819657)) {
              sum += (float)0.035125017;
            } else {
              sum += (float)0.17222898;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.3162035)) {
            if (  (data[9].fvalue < 1.8436004)) {
              sum += (float)-0.067806505;
            } else {
              sum += (float)0.21290886;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.017504521;
            } else {
              sum += (float)-0.074810117;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.7082946)) {
          if (  (data[8].fvalue < 1.1309613)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.37456807;
            } else {
              sum += (float)-0.53811997;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)0.027623605;
            } else {
              sum += (float)-0.34245467;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 1.8378544)) {
              sum += (float)-0.082937568;
            } else {
              sum += (float)-0.20022924;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)0.13207054;
            } else {
              sum += (float)0.38793841;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 2.2497003)) {
        if (  (data[8].fvalue < 20.605917)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.37705266)) {
              sum += (float)0.10495695;
            } else {
              sum += (float)0.17243901;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.0677469;
            } else {
              sum += (float)0.35841209;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.78803706)) {
            if (  (data[9].fvalue < 4.9139061)) {
              sum += (float)0.42226332;
            } else {
              sum += (float)0.25841212;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.17343424;
            } else {
              sum += (float)0.49983865;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 20.605917)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.056865465;
            } else {
              sum += (float)-0.078015678;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)-0.040649809;
            } else {
              sum += (float)0.29747832;
            }
          }
        } else {
          if (  (data[8].fvalue < 28.700102)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.13388574;
            } else {
              sum += (float)0.047994327;
            }
          } else {
            if (  (data[7].fvalue < 0.78803706)) {
              sum += (float)0.31016359;
            } else {
              sum += (float)0.14278781;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if ( (data[9].fvalue < 4.5138702)) {
        if ( (data[9].fvalue < 1.9143379)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[8].fvalue < 3.8600745)) {
              sum += (float)-0.60862297;
            } else {
              sum += (float)-0.52896982;
            }
          } else {
            sum += (float)-0.55021441;
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 2.3529406)) {
              sum += (float)-0.18605958;
            } else {
              sum += (float)-0.4973821;
            }
          } else {
            if (  (data[8].fvalue < 10.105608)) {
              sum += (float)-0.54338205;
            } else {
              sum += (float)-0.48893231;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 17.880157)) {
          if (  (data[21].fvalue < 3.0685601)) {
            if (  (data[9].fvalue < 5.6905909)) {
              sum += (float)-0.48390824;
            } else {
              sum += (float)-0.44759262;
            }
          } else {
            if (  (data[8].fvalue < 9.3488445)) {
              sum += (float)-0.51053452;
            } else {
              sum += (float)-0.48190337;
            }
          }
        } else {
          if (  (data[8].fvalue < 28.700102)) {
            if (  (data[21].fvalue < 4.0312967)) {
              sum += (float)-0.41976637;
            } else {
              sum += (float)-0.46401951;
            }
          } else {
            if (  (data[7].fvalue < 0.88839376)) {
              sum += (float)-0.35322946;
            } else {
              sum += (float)-0.40611649;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 1.999999)) {
        if ( (data[9].fvalue < 2.9208786)) {
          sum += (float)-0.61213827;
        } else {
          sum += (float)-0.6034615;
        }
      } else {
        if (  (data[13].fvalue < 5.999999)) {
          sum += (float)-0.5930807;
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            sum += (float)-0.57003045;
          } else {
            sum += (float)-0.6028465;
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[19].fvalue < 0.066052698)) {
      if (  (data[18].fvalue < 0.029834434)) {
        if (  (data[19].fvalue < 0.034161057)) {
          if (  (data[11].fvalue < 0.29290932)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.16214719;
            } else {
              sum += (float)-0.71617186;
            }
          } else {
            if (  (data[5].fvalue < 15.848475)) {
              sum += (float)-0.3564665;
            } else {
              sum += (float)-0.62651587;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.1206026;
            } else {
              sum += (float)-0.24818431;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.70684707;
            } else {
              sum += (float)-0.55603832;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)0.11065385;
            } else {
              sum += (float)-0.49627581;
            }
          } else {
            if (  (data[18].fvalue < 0.22760777)) {
              sum += (float)0.027886663;
            } else {
              sum += (float)-0.5214411;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.02296152)) {
              sum += (float)-0.21085396;
            } else {
              sum += (float)0.037131254;
            }
          } else {
            if (  (data[18].fvalue < 0.26116917)) {
              sum += (float)-0.72745013;
            } else {
              sum += (float)-0.39645869;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 2.1069853)) {
        if (  (data[19].fvalue < 1.2550548)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.076677047;
            } else {
              sum += (float)-0.073011979;
            }
          } else {
            if (  (data[18].fvalue < 0.048877973)) {
              sum += (float)-0.36211571;
            } else {
              sum += (float)0.099694997;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.8254658)) {
            if (  (data[21].fvalue < 1.5142939)) {
              sum += (float)0.049905933;
            } else {
              sum += (float)-0.068510674;
            }
          } else {
            if (  (data[18].fvalue < 0.16890348)) {
              sum += (float)-0.12519479;
            } else {
              sum += (float)-0.02401961;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[21].fvalue < 0.059958007)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.21950105;
            } else {
              sum += (float)-0.24016526;
            }
          } else {
            if (  (data[19].fvalue < 0.19721535)) {
              sum += (float)-0.20471059;
            } else {
              sum += (float)-0.022655856;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.34719414;
            } else {
              sum += (float)-0.54991543;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.1451509;
            } else {
              sum += (float)0.12947595;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[18].fvalue < 7.5044355)) {
          if (  (data[18].fvalue < 0.10318472)) {
            if (  (data[19].fvalue < 0.010796987)) {
              sum += (float)-0.52633834;
            } else {
              sum += (float)-0.45693082;
            }
          } else {
            if (  (data[11].fvalue < 0.34766185)) {
              sum += (float)-0.45513111;
            } else {
              sum += (float)-0.40824562;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.63381416)) {
            if (  (data[19].fvalue < 0.67372966)) {
              sum += (float)-0.45705187;
            } else {
              sum += (float)-0.27289543;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.32257006;
            } else {
              sum += (float)0.14876175;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 7.5044355)) {
          if (  (data[19].fvalue < 6.0948591)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.40758368;
            } else {
              sum += (float)-0.36193219;
            }
          } else {
            if (  (data[18].fvalue < 0.55059457)) {
              sum += (float)-0.30956575;
            } else {
              sum += (float)0.14527299;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.49823841)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.41787151;
            } else {
              sum += (float)-0.13639627;
            }
          } else {
            if (  (data[22].fvalue < 3.0604901)) {
              sum += (float)0.56415558;
            } else {
              sum += (float)-0.029785389;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 1.999999)) {
          sum += (float)-0.55969602;
        } else {
          sum += (float)-0.54133666;
        }
      } else {
        if (  (data[18].fvalue < 0.0089430669)) {
          sum += (float)-0.534895;
        } else {
          sum += (float)-0.55296916;
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[10].fvalue < 0.032984361)) {
      if (  (data[9].fvalue < 0.9100951)) {
        sum += (float)0.7530328;
      } else {
        if (  (data[9].fvalue < 1.7021977)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[9].fvalue < 1.0822394)) {
              sum += (float)-0.26954913;
            } else {
              sum += (float)0.037518043;
            }
          } else {
            if (  (data[18].fvalue < 0.14124727)) {
              sum += (float)-0.60946339;
            } else {
              sum += (float)-0.3039152;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.3279443)) {
            if (  (data[10].fvalue < 0.013425715)) {
              sum += (float)0.23372829;
            } else {
              sum += (float)0.13770784;
            }
          } else {
            if (  (data[21].fvalue < 1.9824846)) {
              sum += (float)0.091215365;
            } else {
              sum += (float)-0.10767299;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[18].fvalue < 0.81335896)) {
            if (  (data[18].fvalue < 0.07008671)) {
              sum += (float)-0.026929457;
            } else {
              sum += (float)0.039698396;
            }
          } else {
            if (  (data[21].fvalue < 0.98844093)) {
              sum += (float)-0.0013995582;
            } else {
              sum += (float)-0.11550176;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.080696151)) {
            if (  (data[18].fvalue < 0.059776176)) {
              sum += (float)-0.6397438;
            } else {
              sum += (float)-0.33841711;
            }
          } else {
            if (  (data[20].fvalue < 0.12266774)) {
              sum += (float)0.16331497;
            } else {
              sum += (float)0.037574172;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.35175517)) {
          if (  (data[18].fvalue < 0.16888289)) {
            if (  (data[18].fvalue < 0.12858205)) {
              sum += (float)-0.6201514;
            } else {
              sum += (float)-0.50885868;
            }
          } else {
            if ( (data[9].fvalue < 1.149509)) {
              sum += (float)0.077278219;
            } else {
              sum += (float)-0.36631581;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.40601987)) {
            if (  (data[18].fvalue < 1.6483128)) {
              sum += (float)-0.021724826;
            } else {
              sum += (float)-0.13121569;
            }
          } else {
            if (  (data[21].fvalue < 0.022159677)) {
              sum += (float)-0.033861756;
            } else {
              sum += (float)0.13570979;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if ( (data[9].fvalue < 4.2534671)) {
        if ( (data[9].fvalue < 1.8361735)) {
          if (  (data[10].fvalue < 0.74160361)) {
            if (  (data[9].fvalue < 1.763659)) {
              sum += (float)-0.53649443;
            } else {
              sum += (float)-0.50853497;
            }
          } else {
            sum += (float)-0.44285798;
          }
        } else {
          if (  (data[20].fvalue < 0.097395957)) {
            if (  (data[21].fvalue < 0.11343361)) {
              sum += (float)-0.22218555;
            } else {
              sum += (float)-0.38162747;
            }
          } else {
            if (  (data[10].fvalue < 0.52427739)) {
              sum += (float)-0.4121969;
            } else {
              sum += (float)-0.30835131;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 1.885052)) {
          if (  (data[18].fvalue < 7.3895168)) {
            if (  (data[21].fvalue < 1.8444644)) {
              sum += (float)-0.22744973;
            } else {
              sum += (float)-0.31171975;
            }
          } else {
            if (  (data[21].fvalue < 0.57594222)) {
              sum += (float)-0.3226746;
            } else {
              sum += (float)0.14154129;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.257265)) {
            if (  (data[9].fvalue < 6.0794616)) {
              sum += (float)-0.37482059;
            } else {
              sum += (float)-0.31163532;
            }
          } else {
            if (  (data[18].fvalue < 0.14124727)) {
              sum += (float)-0.32781205;
            } else {
              sum += (float)-0.24477465;
            }
          }
        }
      }
    } else {
      sum += (float)-0.53081268;
    }
  }
  if (  (data[8].fvalue < 4.6351666)) {
    if (  (data[10].fvalue < 0.056867085)) {
      if (  (data[9].fvalue < 0.96061605)) {
        if (  (data[9].fvalue < 0.9109062)) {
          sum += (float)0.61920416;
        } else {
          sum += (float)1.0305877;
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 1.7649562)) {
            if (  (data[9].fvalue < 1.0832281)) {
              sum += (float)-0.1879812;
            } else {
              sum += (float)-0.0079860305;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.15828863;
            } else {
              sum += (float)-0.0095669311;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.069741227)) {
            if (  (data[9].fvalue < 2.6514943)) {
              sum += (float)0.1146177;
            } else {
              sum += (float)0.35586739;
            }
          } else {
            if (  (data[18].fvalue < 0.0092144469)) {
              sum += (float)0.061161194;
            } else {
              sum += (float)-0.037271529;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 1.0887341)) {
        if (  (data[18].fvalue < 0.030769248)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 1.1048769)) {
              sum += (float)-0.079311401;
            } else {
              sum += (float)-0.24270539;
            }
          } else {
            if (  (data[18].fvalue < 0.0092144469)) {
              sum += (float)0.028131431;
            } else {
              sum += (float)-0.25877273;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0832281)) {
            if (  (data[8].fvalue < 0.098583311)) {
              sum += (float)0.28006357;
            } else {
              sum += (float)0.11836412;
            }
          } else {
            if (  (data[8].fvalue < 2.1975055)) {
              sum += (float)-0.044270277;
            } else {
              sum += (float)0.018937288;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 2.7303548)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.13128214;
            } else {
              sum += (float)0.21147056;
            }
          } else {
            if (  (data[18].fvalue < 1.9381553)) {
              sum += (float)0.046874918;
            } else {
              sum += (float)0.331476;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.7885091)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.08259289;
            } else {
              sum += (float)0.073521107;
            }
          } else {
            if ( (data[9].fvalue < 4.6099544)) {
              sum += (float)-0.21624321;
            } else {
              sum += (float)-0.084774837;
            }
          }
        }
      }
    }
  } else {
    if (  (data[15].fvalue < 3.999999)) {
      if (  (data[18].fvalue < 2.87449)) {
        if (  (data[18].fvalue < 0.14167194)) {
          if (  (data[18].fvalue < 0.0092144469)) {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)0.050162934;
            } else {
              sum += (float)-0.3529129;
            }
          } else {
            if (  (data[18].fvalue < 0.060312886)) {
              sum += (float)-0.19893642;
            } else {
              sum += (float)-0.037928808;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.31385237)) {
            if (  (data[18].fvalue < 1.4394701)) {
              sum += (float)0.059556939;
            } else {
              sum += (float)-0.015231849;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.089289978;
            } else {
              sum += (float)0.21887752;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.52637804)) {
          if (  (data[9].fvalue < 4.8736353)) {
            if (  (data[9].fvalue < 2.89798)) {
              sum += (float)0.13633864;
            } else {
              sum += (float)-0.16254309;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.08541704;
            } else {
              sum += (float)-0.054829128;
            }
          }
        } else {
          if (  (data[8].fvalue < 28.504236)) {
            if (  (data[8].fvalue < 10.193353)) {
              sum += (float)0.10324796;
            } else {
              sum += (float)-0.017581882;
            }
          } else {
            if (  (data[10].fvalue < 0.64029235)) {
              sum += (float)0.11396937;
            } else {
              sum += (float)0.21898195;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 1.999999)) {
        if (  (data[15].fvalue < 4.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 7.1668105)) {
              sum += (float)0.74730647;
            } else {
              sum += (float)0.49336433;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.65159136;
            } else {
              sum += (float)0.43420249;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.11545842)) {
            sum += (float)0.62422615;
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)0.79056364;
            } else {
              sum += (float)0.88820195;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 4.999999)) {
          if (  (data[10].fvalue < 0.33388183)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.030243529;
            } else {
              sum += (float)0.15062593;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.17218108;
            } else {
              sum += (float)0.35495734;
            }
          }
        } else {
          if ( (data[9].fvalue < 6.5529437)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.66275024;
            } else {
              sum += (float)0.46850735;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)0.20538379;
            } else {
              sum += (float)0.51621324;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 7.999999)) {
    if (  (data[13].fvalue < 8.999999)) {
      if (  (data[19].fvalue < 0.036761615)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.021315143)) {
            if ( (data[9].fvalue < 2.0417488)) {
              sum += (float)0.17773372;
            } else {
              sum += (float)-0.47564736;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.053687967;
            } else {
              sum += (float)-0.10414576;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.010855459)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.25593793;
            } else {
              sum += (float)-0.65994841;
            }
          } else {
            if (  (data[18].fvalue < 0.4964658)) {
              sum += (float)-0.0056947186;
            } else {
              sum += (float)-0.26111224;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.0283481)) {
          if (  (data[18].fvalue < 0.0091637233)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.057886671;
            } else {
              sum += (float)-0.81957442;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.4194974;
            } else {
              sum += (float)0.072470456;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.1205769;
            } else {
              sum += (float)-0.20461999;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.1153902;
            } else {
              sum += (float)0.040454891;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[13].fvalue < 10.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.0091637233)) {
              sum += (float)-0.61683345;
            } else {
              sum += (float)-0.55646056;
            }
          } else {
            if (  (data[18].fvalue < 0.76676399)) {
              sum += (float)-0.27893156;
            } else {
              sum += (float)-0.53550786;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.68655705)) {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.50907421;
            } else {
              sum += (float)-0.6540221;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.67451417;
            } else {
              sum += (float)-0.7748853;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.68470126)) {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[19].fvalue < 0.33010787)) {
              sum += (float)-0.48467091;
            } else {
              sum += (float)-0.11220678;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.34891659;
            } else {
              sum += (float)-0.49554506;
            }
          }
        } else {
          if (  (data[13].fvalue < 11.999999)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.030011415;
            } else {
              sum += (float)0.19809325;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.40396771;
            } else {
              sum += (float)-0.082458161;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 5.999999)) {
      if (  (data[19].fvalue < 0.8868835)) {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.29759991)) {
              sum += (float)-0.53070521;
            } else {
              sum += (float)-0.65922374;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.42198294;
            } else {
              sum += (float)-0.18347675;
            }
          }
        } else {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.19801733;
            } else {
              sum += (float)0.1623325;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.55597073;
            } else {
              sum += (float)-0.29651216;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.6085726;
            } else {
              sum += (float)-0.45620668;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.63395458;
            } else {
              sum += (float)-0.70724201;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)-0.55511159;
            } else {
              sum += (float)-0.15304615;
            }
          } else {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)-0.11020683;
            } else {
              sum += (float)-0.62856323;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.5526281)) {
        if (  (data[18].fvalue < 0.4199031)) {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[18].fvalue < 0.29480588)) {
              sum += (float)-0.52254438;
            } else {
              sum += (float)-0.30214477;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.48989701;
            } else {
              sum += (float)-0.55623472;
            }
          }
        } else {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[19].fvalue < 1.7920583)) {
              sum += (float)-0.10007887;
            } else {
              sum += (float)-0.2642386;
            }
          } else {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)-0.39947879;
            } else {
              sum += (float)-0.60171157;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 7.999999)) {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[19].fvalue < 1.1124383)) {
              sum += (float)0.15776759;
            } else {
              sum += (float)-0.14275353;
            }
          } else {
            if (  (data[19].fvalue < 1.0016315)) {
              sum += (float)-0.28281981;
            } else {
              sum += (float)-0.56673378;
            }
          }
        } else {
          if (  (data[12].fvalue < 13.999999)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.19075751;
            } else {
              sum += (float)0.40774167;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.25986484;
            } else {
              sum += (float)0.13700242;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 3.3073964)) {
    if ( (data[9].fvalue < 3.4279311)) {
      if (  (data[22].fvalue < 1.0434505)) {
        if (  (data[22].fvalue < 0.021448886)) {
          if (  (data[19].fvalue < 0.026972316)) {
            if (  (data[9].fvalue < 1.1559246)) {
              sum += (float)-0.1871464;
            } else {
              sum += (float)-0.013543051;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.039505169;
            } else {
              sum += (float)-0.069407381;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.10629611)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.14352705;
            } else {
              sum += (float)0.31564361;
            }
          } else {
            if ( (data[9].fvalue < 1.7040201)) {
              sum += (float)-0.071776949;
            } else {
              sum += (float)0.016185556;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.6508081)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.10082442)) {
              sum += (float)0.024346868;
            } else {
              sum += (float)-0.20941074;
            }
          } else {
            if (  (data[10].fvalue < 0.50424457)) {
              sum += (float)-0.10729964;
            } else {
              sum += (float)0.020187143;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.0012151)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.0013915829;
            } else {
              sum += (float)0.25571439;
            }
          } else {
            if (  (data[10].fvalue < 0.36895514)) {
              sum += (float)-0.16584155;
            } else {
              sum += (float)0.018375097;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.10629611)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.01086738)) {
              sum += (float)0.13517511;
            } else {
              sum += (float)0.20350863;
            }
          } else {
            if (  (data[22].fvalue < 0.021448886)) {
              sum += (float)0.0010713036;
            } else {
              sum += (float)0.32511386;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.021448886)) {
            if (  (data[10].fvalue < 0.36895514)) {
              sum += (float)-0.04930377;
            } else {
              sum += (float)0.02993976;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.10196725;
            } else {
              sum += (float)0.38090232;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.14709345)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.01086738)) {
              sum += (float)-0.49629763;
            } else {
              sum += (float)0.038908347;
            }
          } else {
            if (  (data[19].fvalue < 0.01086738)) {
              sum += (float)0.028424382;
            } else {
              sum += (float)-0.076284438;
            }
          }
        } else {
          if (  (data[22].fvalue < 3.2555392)) {
            if (  (data[22].fvalue < 0.51568878)) {
              sum += (float)0.0051149395;
            } else {
              sum += (float)0.078764781;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.027491599;
            } else {
              sum += (float)-0.068564408;
            }
          }
        }
      }
    }
  } else {
    if (  (data[9].fvalue < 2.8964496)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[22].fvalue < 1.0434505)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[10].fvalue < 0.6371752)) {
              sum += (float)0.41419688;
            } else {
              sum += (float)0.17302857;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.015555114;
            } else {
              sum += (float)-0.28560898;
            }
          }
        } else {
          if (  (data[22].fvalue < 2.5090766)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.068736389;
            } else {
              sum += (float)0.13118762;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.4190138;
            } else {
              sum += (float)-0.09075202;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.96464121)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.32230079;
            } else {
              sum += (float)-0.10605302;
            }
          } else {
            if (  (data[6].fvalue < 6.8579502)) {
              sum += (float)0.050017737;
            } else {
              sum += (float)-0.1529419;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.021448886)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.50332868;
            } else {
              sum += (float)-0.070180476;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.38386354;
            } else {
              sum += (float)0.010998891;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 4.7411566)) {
        if (  (data[10].fvalue < 0.39326289)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.03841576;
            } else {
              sum += (float)-0.22705904;
            }
          } else {
            if (  (data[22].fvalue < 0.10629611)) {
              sum += (float)-0.23555057;
            } else {
              sum += (float)-0.37127727;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.6371752)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.088116013;
            } else {
              sum += (float)-0.27855787;
            }
          } else {
            if (  (data[6].fvalue < 4.7222991)) {
              sum += (float)0.246075;
            } else {
              sum += (float)0.028182782;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 5.5302238)) {
          if (  (data[10].fvalue < 0.28463882)) {
            if (  (data[9].fvalue < 5.6632872)) {
              sum += (float)-0.12319312;
            } else {
              sum += (float)-0.0039646933;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.14288057;
            } else {
              sum += (float)0.015887333;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.30378988)) {
            if (  (data[9].fvalue < 6.2711058)) {
              sum += (float)-0.24913277;
            } else {
              sum += (float)-0.11565782;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.01009881;
            } else {
              sum += (float)-0.19919047;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[20].fvalue < 1.2993915)) {
      if (  (data[20].fvalue < 0.016002264)) {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[22].fvalue < 4.8259254)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.0034436299;
            } else {
              sum += (float)-0.17619611;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.28520387;
            } else {
              sum += (float)0.00040053175;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.9661341)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.003900192;
            } else {
              sum += (float)-0.13384515;
            }
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.1623206;
            } else {
              sum += (float)0.05098173;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.10035667)) {
          if (  (data[22].fvalue < 1.3985972)) {
            if (  (data[22].fvalue < 0.019585257)) {
              sum += (float)0.20817156;
            } else {
              sum += (float)0.27118936;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.12963779;
            } else {
              sum += (float)0.38243723;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.019585257)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.039574716;
            } else {
              sum += (float)0.015115125;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.021470809;
            } else {
              sum += (float)0.24668165;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 3.999999)) {
        if (  (data[5].fvalue < 11.179298)) {
          if (  (data[20].fvalue < 3.2616584)) {
            if (  (data[22].fvalue < 0.76954299)) {
              sum += (float)-0.19774154;
            } else {
              sum += (float)-0.060757369;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.06275516;
            } else {
              sum += (float)-0.10450406;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.025225334;
            } else {
              sum += (float)-0.054729696;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.073715925;
            } else {
              sum += (float)-0.013892709;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 5.4949031)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)0.12384416;
            } else {
              sum += (float)0.38373375;
            }
          } else {
            if (  (data[20].fvalue < 3.2616584)) {
              sum += (float)0.44745448;
            } else {
              sum += (float)0.28177208;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 1.3985972)) {
              sum += (float)-0.020878177;
            } else {
              sum += (float)-0.1265533;
            }
          } else {
            if (  (data[22].fvalue < 2.0574329)) {
              sum += (float)0.22896709;
            } else {
              sum += (float)0.04393125;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 2.999999)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[5].fvalue < 4.9616079)) {
            if (  (data[5].fvalue < 3.5066817)) {
              sum += (float)-0.48022595;
            } else {
              sum += (float)-0.37176767;
            }
          } else {
            if (  (data[5].fvalue < 20.485041)) {
              sum += (float)-0.27273029;
            } else {
              sum += (float)-0.21789974;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.019585257)) {
            if (  (data[5].fvalue < 22.037039)) {
              sum += (float)-0.40200308;
            } else {
              sum += (float)-0.32708609;
            }
          } else {
            if (  (data[5].fvalue < 27.492182)) {
              sum += (float)-0.31475976;
            } else {
              sum += (float)-0.25497231;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[20].fvalue < 0.10035667)) {
            if (  (data[5].fvalue < 26.714724)) {
              sum += (float)-0.21231395;
            } else {
              sum += (float)-0.12916797;
            }
          } else {
            if (  (data[22].fvalue < 1.6975216)) {
              sum += (float)-0.1791753;
            } else {
              sum += (float)-0.23548378;
            }
          }
        } else {
          if (  (data[5].fvalue < 28.292627)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.22811526;
            } else {
              sum += (float)-0.036998894;
            }
          } else {
            if (  (data[7].fvalue < 0.99401981)) {
              sum += (float)-0.082004242;
            } else {
              sum += (float)-0.17002749;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 3.999999)) {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[5].fvalue < 31.451584)) {
            if (  (data[7].fvalue < 0.99490547)) {
              sum += (float)-0.43591818;
            } else {
              sum += (float)-0.48643258;
            }
          } else {
            if (  (data[1].fvalue < 0.99999899)) {
              sum += (float)-0.39014304;
            } else {
              sum += (float)-0.42918089;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 25.204527)) {
              sum += (float)-0.47011301;
            } else {
              sum += (float)-0.39924061;
            }
          } else {
            if (  (data[20].fvalue < 5.4949031)) {
              sum += (float)-0.33448216;
            } else {
              sum += (float)-0.39628169;
            }
          }
        }
      } else {
        sum += (float)-0.52021813;
      }
    }
  }
  if (  (data[21].fvalue < 0.11025158)) {
    if (  (data[16].fvalue < 2.999999)) {
      if ( (data[9].fvalue < 2.1829782)) {
        if (  (data[22].fvalue < 0.88732576)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0036499235;
            } else {
              sum += (float)0.083104119;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.12407956;
            } else {
              sum += (float)0.23635776;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.17170683;
            } else {
              sum += (float)-0.017669559;
            }
          } else {
            if (  (data[22].fvalue < 1.4867182)) {
              sum += (float)0.049808338;
            } else {
              sum += (float)-0.17165115;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.10711422)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 2.9687428)) {
              sum += (float)-0.041667137;
            } else {
              sum += (float)0.1505315;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.23240195;
            } else {
              sum += (float)0.071087644;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.092963181;
            } else {
              sum += (float)-0.0096237343;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.13263053;
            } else {
              sum += (float)0.16742548;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.99999899)) {
        if (  (data[16].fvalue < 3.999999)) {
          if ( (data[9].fvalue < 3.80691)) {
            if (  (data[22].fvalue < 1.5936862)) {
              sum += (float)-0.028606232;
            } else {
              sum += (float)-0.20807406;
            }
          } else {
            if (  (data[22].fvalue < 4.8177161)) {
              sum += (float)0.05431325;
            } else {
              sum += (float)-0.084795728;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)-0.23210128;
            } else {
              sum += (float)-0.44980299;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.18193269;
            } else {
              sum += (float)-0.063623905;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if ( (data[9].fvalue < 2.0344613)) {
              sum += (float)0.18208639;
            } else {
              sum += (float)-0.058604989;
            }
          } else {
            if (  (data[7].fvalue < 0.99526358)) {
              sum += (float)0.43546006;
            } else {
              sum += (float)0.3186796;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.23490524;
            } else {
              sum += (float)0.42160323;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.52794117;
            } else {
              sum += (float)0.77808493;
            }
          }
        }
      }
    }
  } else {
    if (  (data[15].fvalue < 2.999999)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.020007676)) {
              sum += (float)-0.017978227;
            } else {
              sum += (float)0.03084879;
            }
          } else {
            if ( (data[9].fvalue < 2.2542019)) {
              sum += (float)-0.098927818;
            } else {
              sum += (float)-0.020984041;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.14684193;
            } else {
              sum += (float)0.055662598;
            }
          } else {
            if (  (data[21].fvalue < 0.34180355)) {
              sum += (float)-0.04611953;
            } else {
              sum += (float)0.057235263;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.11681368;
            } else {
              sum += (float)-0.20587888;
            }
          } else {
            if (  (data[21].fvalue < 1.7682172)) {
              sum += (float)0.080897056;
            } else {
              sum += (float)-0.12601444;
            }
          }
        } else {
          if (  (data[22].fvalue < 4.5768447)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.083273254;
            } else {
              sum += (float)0.23366763;
            }
          } else {
            if (  (data[21].fvalue < 2.7441571)) {
              sum += (float)0.0120191;
            } else {
              sum += (float)-0.10888875;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 3.529562)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.11344018;
            } else {
              sum += (float)-0.026356973;
            }
          } else {
            if (  (data[21].fvalue < 0.37082654)) {
              sum += (float)0.53575808;
            } else {
              sum += (float)0.17616037;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 1.8856404)) {
              sum += (float)0.29525593;
            } else {
              sum += (float)0.16032976;
            }
          } else {
            if (  (data[21].fvalue < 0.41009283)) {
              sum += (float)0.63732368;
            } else {
              sum += (float)0.35504907;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[21].fvalue < 8.1122828)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)-0.018307747;
            } else {
              sum += (float)0.31387141;
            }
          } else {
            if ( (data[9].fvalue < 5.0048323)) {
              sum += (float)-0.0040592877;
            } else {
              sum += (float)-0.093282595;
            }
          }
        } else {
          if (  (data[15].fvalue < 4.999999)) {
            if (  (data[22].fvalue < 6.451067)) {
              sum += (float)0.13058031;
            } else {
              sum += (float)0.0016752181;
            }
          } else {
            if (  (data[21].fvalue < 8.1122828)) {
              sum += (float)0.46709329;
            } else {
              sum += (float)0.25105572;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 15.059252)) {
    if (  (data[19].fvalue < 0.93959296)) {
      if (  (data[13].fvalue < 9.999999)) {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[10].fvalue < 0.087198123)) {
            if (  (data[9].fvalue < 0.96115094)) {
              sum += (float)0.57682639;
            } else {
              sum += (float)0.055355143;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.0024631941;
            } else {
              sum += (float)0.074464686;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.33111882)) {
            if (  (data[19].fvalue < 0.13402781)) {
              sum += (float)-0.56498092;
            } else {
              sum += (float)-0.17836773;
            }
          } else {
            if ( (data[9].fvalue < 1.2568398)) {
              sum += (float)0.25327808;
            } else {
              sum += (float)0.012839169;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 12.999999)) {
          if (  (data[19].fvalue < 0.55612862)) {
            if (  (data[19].fvalue < 0.34948796)) {
              sum += (float)-0.50855505;
            } else {
              sum += (float)-0.3780576;
            }
          } else {
            if (  (data[19].fvalue < 0.79678315)) {
              sum += (float)-0.2021549;
            } else {
              sum += (float)-0.045464229;
            }
          }
        } else {
          if (  (data[13].fvalue < 14.999999)) {
            if (  (data[19].fvalue < 0.7539525)) {
              sum += (float)-0.53403902;
            } else {
              sum += (float)-0.32963109;
            }
          } else {
            if (  (data[19].fvalue < 0.45494705)) {
              sum += (float)-0.5236885;
            } else {
              sum += (float)-0.61360544;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 7.2214646)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0090281153;
            } else {
              sum += (float)-0.065606803;
            }
          } else {
            if (  (data[19].fvalue < 2.2789378)) {
              sum += (float)0.054291297;
            } else {
              sum += (float)-0.026343768;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 1.8947196)) {
              sum += (float)-0.17536244;
            } else {
              sum += (float)0.0096413912;
            }
          } else {
            if ( (data[9].fvalue < 4.4713402)) {
              sum += (float)-0.20996638;
            } else {
              sum += (float)-0.088531278;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 2.8340898)) {
          if (  (data[13].fvalue < 3.999999)) {
            if ( (data[9].fvalue < 2.4784677)) {
              sum += (float)0.24524826;
            } else {
              sum += (float)0.0083911484;
            }
          } else {
            if (  (data[8].fvalue < 3.3628821)) {
              sum += (float)-0.034257863;
            } else {
              sum += (float)0.045222141;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.7155173)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.37707791;
            } else {
              sum += (float)0.0080623906;
            }
          } else {
            if (  (data[9].fvalue < 4.2345405)) {
              sum += (float)-0.1731468;
            } else {
              sum += (float)-0.069191068;
            }
          }
        }
      }
    }
  } else {
    if (  (data[10].fvalue < 0.60613227)) {
      if (  (data[19].fvalue < 0.21663837)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 0.98371321)) {
              sum += (float)0.12301961;
            } else {
              sum += (float)0.055972263;
            }
          } else {
            if (  (data[7].fvalue < 0.97466093)) {
              sum += (float)0.039694075;
            } else {
              sum += (float)0.003200792;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.070294641)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.17060047;
            } else {
              sum += (float)0.10198414;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.015741536;
            } else {
              sum += (float)-0.25179762;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 4.7814298)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.17658666;
            } else {
              sum += (float)0.075480297;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)0.04711207;
            } else {
              sum += (float)0.27291569;
            }
          }
        } else {
          if (  (data[9].fvalue < 5.2870965)) {
            if (  (data[19].fvalue < 6.1824293)) {
              sum += (float)-0.0025874383;
            } else {
              sum += (float)-0.11465996;
            }
          } else {
            if (  (data[9].fvalue < 6.0760713)) {
              sum += (float)0.0053479583;
            } else {
              sum += (float)0.073285051;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.89525813)) {
        if (  (data[19].fvalue < 4.7814298)) {
          if (  (data[7].fvalue < 0.9668867)) {
            if (  (data[19].fvalue < 0.26519012)) {
              sum += (float)0.11421164;
            } else {
              sum += (float)0.21290474;
            }
          } else {
            if (  (data[19].fvalue < 0.24842957)) {
              sum += (float)0.015256848;
            } else {
              sum += (float)0.10971049;
            }
          }
        } else {
          if (  (data[8].fvalue < 28.248041)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.050918553;
            } else {
              sum += (float)-0.14094113;
            }
          } else {
            if (  (data[19].fvalue < 6.1824293)) {
              sum += (float)0.1373433;
            } else {
              sum += (float)-0.016218793;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.7399953)) {
          if (  (data[11].fvalue < 0.95988065)) {
            if (  (data[19].fvalue < 6.1824293)) {
              sum += (float)0.22161303;
            } else {
              sum += (float)0.088727154;
            }
          } else {
            if (  (data[10].fvalue < 0.68659353)) {
              sum += (float)0.29074249;
            } else {
              sum += (float)0.36511117;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.95988065)) {
            if (  (data[19].fvalue < 6.1824293)) {
              sum += (float)0.34068623;
            } else {
              sum += (float)0.13960394;
            }
          } else {
            if (  (data[8].fvalue < 28.248041)) {
              sum += (float)0.31083244;
            } else {
              sum += (float)0.47846362;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[8].fvalue < 3.1682999)) {
      if (  (data[8].fvalue < 0.17115122)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[6].fvalue < 16.339657)) {
            if (  (data[19].fvalue < 0.12132981)) {
              sum += (float)0.16331169;
            } else {
              sum += (float)0.038785756;
            }
          } else {
            if (  (data[19].fvalue < 0.027011476)) {
              sum += (float)-0.25191656;
            } else {
              sum += (float)0.082795158;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.010189221)) {
            if (  (data[8].fvalue < 0.028248927)) {
              sum += (float)-0.50261629;
            } else {
              sum += (float)-0.63086647;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.099530041;
            } else {
              sum += (float)-0.0019174048;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.96542197)) {
          if (  (data[21].fvalue < 0.84700853)) {
            if (  (data[20].fvalue < 0.016857307)) {
              sum += (float)-0.0084216287;
            } else {
              sum += (float)0.035975814;
            }
          } else {
            if (  (data[18].fvalue < 1.0274459)) {
              sum += (float)-0.031303849;
            } else {
              sum += (float)-0.1724924;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.61629462)) {
            if (  (data[18].fvalue < 0.97315419)) {
              sum += (float)0.013733483;
            } else {
              sum += (float)-0.12079447;
            }
          } else {
            if (  (data[18].fvalue < 0.10629837)) {
              sum += (float)-0.1849528;
            } else {
              sum += (float)-0.047953527;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 2.09284)) {
        if (  (data[21].fvalue < 1.837194)) {
          if (  (data[18].fvalue < 0.1558129)) {
            if (  (data[19].fvalue < 0.12132981)) {
              sum += (float)-0.34732455;
            } else {
              sum += (float)0.074288063;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.080182023;
            } else {
              sum += (float)0.19506644;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.6535496)) {
            if (  (data[18].fvalue < 0.10629837)) {
              sum += (float)0.00023241834;
            } else {
              sum += (float)0.068111226;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.12803961;
            } else {
              sum += (float)0.027763445;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 1.5338848)) {
          if (  (data[18].fvalue < 1.6535496)) {
            if (  (data[19].fvalue < 0.79455686)) {
              sum += (float)0.032430612;
            } else {
              sum += (float)-0.021833621;
            }
          } else {
            if (  (data[8].fvalue < 13.423141)) {
              sum += (float)-0.088094108;
            } else {
              sum += (float)-0.0047092517;
            }
          }
        } else {
          if (  (data[8].fvalue < 9.2148666)) {
            if (  (data[18].fvalue < 1.236661)) {
              sum += (float)-0.12770756;
            } else {
              sum += (float)0.039305057;
            }
          } else {
            if (  (data[18].fvalue < 1.434141)) {
              sum += (float)-0.028016241;
            } else {
              sum += (float)0.1962202;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if (  (data[8].fvalue < 6.8110166)) {
        if (  (data[18].fvalue < 0.10629837)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.13973121)) {
              sum += (float)-0.14099781;
            } else {
              sum += (float)-0.26857609;
            }
          } else {
            if (  (data[19].fvalue < 1.1052003)) {
              sum += (float)-0.26678187;
            } else {
              sum += (float)-0.31535155;
            }
          }
        } else {
          if (  (data[19].fvalue < 6.1958895)) {
            if (  (data[18].fvalue < 4.780828)) {
              sum += (float)-0.20457664;
            } else {
              sum += (float)-0.058470011;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.076442726;
            } else {
              sum += (float)0.45214766;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.11855702)) {
          if (  (data[19].fvalue < 0.071558617)) {
            if (  (data[18].fvalue < 0.041798878)) {
              sum += (float)-0.48694319;
            } else {
              sum += (float)-0.26054862;
            }
          } else {
            if (  (data[20].fvalue < 0.12676211)) {
              sum += (float)-0.086534537;
            } else {
              sum += (float)-0.21668985;
            }
          }
        } else {
          if (  (data[21].fvalue < 3.9860361)) {
            if (  (data[19].fvalue < 6.1958895)) {
              sum += (float)-0.056739099;
            } else {
              sum += (float)0.26418623;
            }
          } else {
            if (  (data[18].fvalue < 1.1698612)) {
              sum += (float)-0.10102794;
            } else {
              sum += (float)-0.18374714;
            }
          }
        }
      }
    } else {
      sum += (float)-0.51032174;
    }
  }
  if (  (data[13].fvalue < 4.999999)) {
    if (  (data[12].fvalue < 5.999999)) {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.045832876;
            } else {
              sum += (float)-0.1036716;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.015363545;
            } else {
              sum += (float)-0.063362934;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.02225722)) {
              sum += (float)0.024918521;
            } else {
              sum += (float)-0.16345449;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.1644537;
            } else {
              sum += (float)0.042728681;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.06603045;
            } else {
              sum += (float)0.1931736;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.2878634;
            } else {
              sum += (float)-0.069820449;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.052767817;
            } else {
              sum += (float)0.021756861;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.076358385;
            } else {
              sum += (float)0.18966505;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.02225722)) {
              sum += (float)-0.58483726;
            } else {
              sum += (float)-0.47644976;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.27560553;
            } else {
              sum += (float)-0.40701646;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.02225722)) {
              sum += (float)-0.4649975;
            } else {
              sum += (float)-0.31872702;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.12813818;
            } else {
              sum += (float)-0.40192255;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 8.999999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.22485012;
            } else {
              sum += (float)-0.06250599;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.036900219;
            } else {
              sum += (float)0.081329435;
            }
          }
        } else {
          if (  (data[12].fvalue < 10.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.30633342;
            } else {
              sum += (float)-0.13440683;
            }
          } else {
            if (  (data[12].fvalue < 12.999999)) {
              sum += (float)-0.37929919;
            } else {
              sum += (float)-0.53164762;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[13].fvalue < 6.999999)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.10362118;
            } else {
              sum += (float)-0.28744572;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.39345106;
            } else {
              sum += (float)-0.11874637;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.04397396;
            } else {
              sum += (float)0.11635461;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.16168687;
            } else {
              sum += (float)0.047252297;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.52070034;
            } else {
              sum += (float)-0.57115698;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.39145231;
            } else {
              sum += (float)-0.50091147;
            }
          }
        } else {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.19516921;
            } else {
              sum += (float)-0.31553441;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.36668208;
            } else {
              sum += (float)-0.51790988;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 12.999999)) {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.013613675;
            } else {
              sum += (float)0.1345771;
            }
          } else {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.076719835;
            } else {
              sum += (float)-0.35039154;
            }
          }
        } else {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)0.014187831;
            } else {
              sum += (float)-0.19353479;
            }
          } else {
            if (  (data[10].fvalue < 0.01467388)) {
              sum += (float)0.32868743;
            } else {
              sum += (float)0.097296946;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 9.999999)) {
          if (  (data[12].fvalue < 14.999999)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.41759512;
            } else {
              sum += (float)-0.17861766;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.54714835;
            } else {
              sum += (float)-0.37521961;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.02225722)) {
              sum += (float)-0.25678474;
            } else {
              sum += (float)0.012740147;
            }
          } else {
            if (  (data[12].fvalue < 19.999998)) {
              sum += (float)0.14644948;
            } else {
              sum += (float)0.8258785;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 3.8758163)) {
    if (  (data[8].fvalue < 2.4779751)) {
      if (  (data[8].fvalue < 0.070809297)) {
        if (  (data[11].fvalue < 0.43351436)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.13149032;
            } else {
              sum += (float)-0.048774742;
            }
          } else {
            if (  (data[19].fvalue < 0.038389828)) {
              sum += (float)-0.44310609;
            } else {
              sum += (float)0.013815614;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.027636193)) {
            if (  (data[18].fvalue < 0.032920588)) {
              sum += (float)-0.3901028;
            } else {
              sum += (float)-0.081258111;
            }
          } else {
            if (  (data[22].fvalue < 0.70873356)) {
              sum += (float)-0.0053683715;
            } else {
              sum += (float)0.37772393;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.95974636)) {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[18].fvalue < 0.023662373)) {
              sum += (float)-0.026507115;
            } else {
              sum += (float)-0.0037966771;
            }
          } else {
            if (  (data[18].fvalue < 0.73106527)) {
              sum += (float)-0.38491467;
            } else {
              sum += (float)-0.060638729;
            }
          }
        } else {
          if (  (data[2].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.40425542)) {
              sum += (float)0.2133116;
            } else {
              sum += (float)0.0054128338;
            }
          } else {
            if (  (data[8].fvalue < 1.5918941)) {
              sum += (float)0.33315897;
            } else {
              sum += (float)0.21253027;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 2.2130654)) {
        if (  (data[20].fvalue < 1.4441444)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.27608645)) {
              sum += (float)0.022679657;
            } else {
              sum += (float)0.066283412;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.11350005;
            } else {
              sum += (float)0.29753879;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.2503109)) {
            if (  (data[19].fvalue < 1.3433899)) {
              sum += (float)-0.053118844;
            } else {
              sum += (float)-0.18971097;
            }
          } else {
            if (  (data[18].fvalue < 0.11981253)) {
              sum += (float)-0.016769491;
            } else {
              sum += (float)0.035376344;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.06600076;
            } else {
              sum += (float)-0.18847978;
            }
          } else {
            if (  (data[19].fvalue < 1.4245657)) {
              sum += (float)-0.07754121;
            } else {
              sum += (float)-0.26820153;
            }
          }
        } else {
          if (  (data[22].fvalue < 13.738794)) {
            if (  (data[11].fvalue < 0.27608645)) {
              sum += (float)-0.093383953;
            } else {
              sum += (float)-0.0099118268;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.065697528;
            } else {
              sum += (float)-0.15690297;
            }
          }
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 0.54166788)) {
      if (  (data[22].fvalue < 2.2130654)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 1.9307505)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.065881975;
            } else {
              sum += (float)-0.27488127;
            }
          } else {
            if (  (data[20].fvalue < 1.5425868)) {
              sum += (float)-0.011556556;
            } else {
              sum += (float)-0.13120815;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.42268279;
            } else {
              sum += (float)-0.56058055;
            }
          } else {
            if (  (data[11].fvalue < 0.32853925)) {
              sum += (float)-0.26093233;
            } else {
              sum += (float)-0.13526472;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.40058339)) {
          if (  (data[18].fvalue < 4.8479366)) {
            if (  (data[11].fvalue < 0.30191845)) {
              sum += (float)-0.32522446;
            } else {
              sum += (float)-0.21005848;
            }
          } else {
            if (  (data[22].fvalue < 4.8032317)) {
              sum += (float)-0.30888614;
            } else {
              sum += (float)-0.42863059;
            }
          }
        } else {
          if (  (data[22].fvalue < 5.5221324)) {
            if (  (data[19].fvalue < 1.3433899)) {
              sum += (float)-0.16199583;
            } else {
              sum += (float)0.061762523;
            }
          } else {
            if (  (data[18].fvalue < 4.8479366)) {
              sum += (float)-0.17285208;
            } else {
              sum += (float)-0.32356384;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 2.5031595)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.28145218)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.046886664;
            } else {
              sum += (float)0.12781322;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.47724506;
            } else {
              sum += (float)-0.11436243;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.38391992;
            } else {
              sum += (float)-0.15078171;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.0060990416;
            } else {
              sum += (float)0.16206372;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 1.1035306)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.37130165)) {
              sum += (float)-0.19317357;
            } else {
              sum += (float)0.14580134;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.069817841;
            } else {
              sum += (float)-0.11215815;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.46312726;
            } else {
              sum += (float)-0.076748721;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.076643638;
            } else {
              sum += (float)0.17211545;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 3.898782)) {
    if (  (data[21].fvalue < 1.2195203)) {
      if (  (data[15].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.1580162)) {
              sum += (float)0.016888142;
            } else {
              sum += (float)-0.022149708;
            }
          } else {
            if (  (data[9].fvalue < 1.152395)) {
              sum += (float)0.12153333;
            } else {
              sum += (float)-0.0031005456;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.13513975;
            } else {
              sum += (float)0.074491955;
            }
          } else {
            if (  (data[21].fvalue < 0.076137669)) {
              sum += (float)0.20508742;
            } else {
              sum += (float)-0.092637919;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 2.79058)) {
              sum += (float)0.70832515;
            } else {
              sum += (float)0.20853353;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.17975914;
            } else {
              sum += (float)0.51708645;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.53112119)) {
              sum += (float)0.26297006;
            } else {
              sum += (float)0.10988027;
            }
          } else {
            if (  (data[21].fvalue < 0.16881444)) {
              sum += (float)0.21506104;
            } else {
              sum += (float)-0.069043517;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 2.7059109)) {
        if (  (data[8].fvalue < 0.49920079)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 1.8406329)) {
              sum += (float)0.010223476;
            } else {
              sum += (float)0.30375716;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.046237726;
            } else {
              sum += (float)-0.29823279;
            }
          }
        } else {
          if (  (data[8].fvalue < 7.7744751)) {
            if (  (data[21].fvalue < 3.0695167)) {
              sum += (float)-0.15726121;
            } else {
              sum += (float)-0.012362455;
            }
          } else {
            if (  (data[6].fvalue < 8.6646318)) {
              sum += (float)0.12693971;
            } else {
              sum += (float)-0.043895535;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.022299813;
            } else {
              sum += (float)-0.074046753;
            }
          } else {
            if (  (data[21].fvalue < 2.3837013)) {
              sum += (float)0.0048605683;
            } else {
              sum += (float)-0.061231188;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.67438418;
            } else {
              sum += (float)0.34532136;
            }
          } else {
            if (  (data[8].fvalue < 6.3862138)) {
              sum += (float)0.036538769;
            } else {
              sum += (float)0.24448387;
            }
          }
        }
      }
    }
  } else {
    if (  (data[21].fvalue < 4.2363038)) {
      if (  (data[15].fvalue < 2.999999)) {
        if (  (data[14].fvalue < 3.999999)) {
          if (  (data[21].fvalue < 0.094212756)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.10208979;
            } else {
              sum += (float)0.034765828;
            }
          } else {
            if (  (data[9].fvalue < 5.451808)) {
              sum += (float)0.0069377609;
            } else {
              sum += (float)0.04216513;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.57255787;
            } else {
              sum += (float)0.31153792;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)0.084869251;
            } else {
              sum += (float)0.34694374;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 1.9542072)) {
              sum += (float)0.56153417;
            } else {
              sum += (float)0.40558368;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.18515697;
            } else {
              sum += (float)0.46352372;
            }
          }
        } else {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 5.5243278)) {
              sum += (float)0.019152183;
            } else {
              sum += (float)0.10620502;
            }
          } else {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.19461523;
            } else {
              sum += (float)0.43325037;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.15805091;
            } else {
              sum += (float)-0.10656498;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)0.038772631;
            } else {
              sum += (float)0.46158576;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.961374)) {
            if (  (data[8].fvalue < 0.1357661)) {
              sum += (float)0.37514171;
            } else {
              sum += (float)-0.0082507664;
            }
          } else {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.097087108;
            } else {
              sum += (float)0.2955285;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.022507627;
            } else {
              sum += (float)-0.041541997;
            }
          } else {
            if (  (data[10].fvalue < 0.2337316)) {
              sum += (float)-0.068417393;
            } else {
              sum += (float)-0.023444368;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)0.12678367;
            } else {
              sum += (float)0.40387914;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.078401707;
            } else {
              sum += (float)0.10849474;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[4].fvalue < 0.99999899)) {
      if (  (data[9].fvalue < 0.99173719)) {
        if (  (data[10].fvalue < 0.21811043)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 0.91089994)) {
              sum += (float)0.52527887;
            } else {
              sum += (float)0.22670384;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.089996897;
            } else {
              sum += (float)-0.45634782;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.38120449)) {
              sum += (float)0.17631519;
            } else {
              sum += (float)0.080267146;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.028203201;
            } else {
              sum += (float)0.10654876;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.4697638)) {
          if (  (data[10].fvalue < 0.60488278)) {
            if (  (data[9].fvalue < 1.0858085)) {
              sum += (float)-0.25295648;
            } else {
              sum += (float)-0.074092507;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.042829573;
            } else {
              sum += (float)-0.044652317;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.011268723;
            } else {
              sum += (float)-0.033471823;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.28922078;
            } else {
              sum += (float)-0.012060942;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 1.1027924)) {
        if ( (data[9].fvalue < 0.96227366)) {
          if (  (data[10].fvalue < 0.60488278)) {
            if (  (data[9].fvalue < 0.91089994)) {
              sum += (float)0.52534771;
            } else {
              sum += (float)1.2123532;
            }
          } else {
            if (  (data[10].fvalue < 0.74278444)) {
              sum += (float)0.29187295;
            } else {
              sum += (float)0.018424874;
            }
          }
        } else {
          if (  (data[20].fvalue < 9.2776976)) {
            if (  (data[9].fvalue < 5.1329088)) {
              sum += (float)0.0040556076;
            } else {
              sum += (float)0.04566782;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.032511655;
            } else {
              sum += (float)-0.051226255;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if ( (data[9].fvalue < 0.91089994)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.17972969;
            } else {
              sum += (float)-0.26749265;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.04058928;
            } else {
              sum += (float)0.096690007;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.010111392;
            } else {
              sum += (float)0.34196076;
            }
          } else {
            if (  (data[20].fvalue < 0.19568066)) {
              sum += (float)0.6296522;
            } else {
              sum += (float)0.15098114;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if ( (data[9].fvalue < 1.8874415)) {
        if (  (data[10].fvalue < 0.68591785)) {
          if (  (data[9].fvalue < 1.8141021)) {
            sum += (float)-0.51134676;
          } else {
            sum += (float)-0.39199212;
          }
        } else {
          sum += (float)-0.31674251;
        }
      } else {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[9].fvalue < 2.4680777)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.53727001;
            } else {
              sum += (float)-0.3279776;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.22993137;
            } else {
              sum += (float)-0.13521978;
            }
          }
        } else {
          if (  (data[9].fvalue < 4.7222857)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.12734039;
            } else {
              sum += (float)0.19663374;
            }
          } else {
            if (  (data[20].fvalue < 2.8563051)) {
              sum += (float)0.015239846;
            } else {
              sum += (float)-0.084417738;
            }
          }
        }
      }
    } else {
      sum += (float)-0.50429183;
    }
  }
  if (  (data[18].fvalue < 1.3258766)) {
    if (  (data[18].fvalue < 0.09579055)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.1297521)) {
            if (  (data[7].fvalue < 0.99995244)) {
              sum += (float)0.054729886;
            } else {
              sum += (float)-0.074248753;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.022232436;
            } else {
              sum += (float)0.014519168;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.010650025)) {
            if (  (data[10].fvalue < 0.12234388)) {
              sum += (float)-0.44524232;
            } else {
              sum += (float)-0.53971541;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.02047799;
            } else {
              sum += (float)-0.13758209;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.017401021)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[10].fvalue < 0.45352063)) {
              sum += (float)-0.61469913;
            } else {
              sum += (float)-0.67063791;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.54941583;
            } else {
              sum += (float)-0.50942808;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.07260593)) {
            if (  (data[10].fvalue < 0.033124357)) {
              sum += (float)-0.67500204;
            } else {
              sum += (float)-0.56958467;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.47055307;
            } else {
              sum += (float)-0.57205939;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 13.415844)) {
        if (  (data[10].fvalue < 0.3252351)) {
          if (  (data[21].fvalue < 0.075784966)) {
            if (  (data[20].fvalue < 0.10298426)) {
              sum += (float)0.013274018;
            } else {
              sum += (float)-0.060141813;
            }
          } else {
            if (  (data[5].fvalue < 8.0655489)) {
              sum += (float)-0.090468235;
            } else {
              sum += (float)-0.038564537;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 0.017401021)) {
              sum += (float)-0.041377366;
            } else {
              sum += (float)0.05589905;
            }
          } else {
            if (  (data[10].fvalue < 0.63980949)) {
              sum += (float)0.03431372;
            } else {
              sum += (float)0.11283811;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 0.017401021)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.005219114;
            } else {
              sum += (float)0.52583361;
            }
          } else {
            if (  (data[20].fvalue < 0.10298426)) {
              sum += (float)0.13003759;
            } else {
              sum += (float)0.056428425;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.12714897)) {
            if (  (data[20].fvalue < 0.017401021)) {
              sum += (float)0.0335465;
            } else {
              sum += (float)0.14487767;
            }
          } else {
            if (  (data[10].fvalue < 0.53020501)) {
              sum += (float)-0.019625815;
            } else {
              sum += (float)0.064753532;
            }
          }
        }
      }
    }
  } else {
    if (  (data[21].fvalue < 2.0727091)) {
      if (  (data[15].fvalue < 2.999999)) {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[18].fvalue < 4.7617826)) {
            if (  (data[21].fvalue < 1.4000496)) {
              sum += (float)0.0089682825;
            } else {
              sum += (float)-0.049971841;
            }
          } else {
            if (  (data[10].fvalue < 0.48685279)) {
              sum += (float)-0.073790066;
            } else {
              sum += (float)0.033611342;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 3.453722)) {
              sum += (float)0.48027599;
            } else {
              sum += (float)0.22893712;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.15475556;
            } else {
              sum += (float)0.047430437;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.13660014)) {
              sum += (float)0.58493704;
            } else {
              sum += (float)0.69990534;
            }
          } else {
            if (  (data[10].fvalue < 0.43833831)) {
              sum += (float)0.2562978;
            } else {
              sum += (float)0.47369081;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.17622046;
            } else {
              sum += (float)-0.032951992;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.14018972;
            } else {
              sum += (float)0.3145884;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 28.188545)) {
        if (  (data[7].fvalue < 0.98225147)) {
          if (  (data[10].fvalue < 0.38219717)) {
            if (  (data[5].fvalue < 14.992079)) {
              sum += (float)-0.19243023;
            } else {
              sum += (float)-0.09416648;
            }
          } else {
            if (  (data[5].fvalue < 16.571684)) {
              sum += (float)-0.085923187;
            } else {
              sum += (float)0.029064646;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 0.042790946)) {
              sum += (float)-0.3029055;
            } else {
              sum += (float)-0.15087248;
            }
          } else {
            if (  (data[18].fvalue < 3.1996009)) {
              sum += (float)-0.068295479;
            } else {
              sum += (float)-0.20769271;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99493682)) {
          if (  (data[21].fvalue < 9.6857395)) {
            if (  (data[20].fvalue < 7.7948813)) {
              sum += (float)0.046187371;
            } else {
              sum += (float)-0.044996608;
            }
          } else {
            if (  (data[10].fvalue < 0.53020501)) {
              sum += (float)-0.099249668;
            } else {
              sum += (float)0.037884112;
            }
          }
        } else {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.10583414;
            } else {
              sum += (float)-0.043467246;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.0099359723;
            } else {
              sum += (float)0.16292405;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 10.530207)) {
    if (  (data[19].fvalue < 0.028147899)) {
      if (  (data[18].fvalue < 0.012362585)) {
        if (  (data[5].fvalue < 24.381311)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.095417812;
            } else {
              sum += (float)-0.26559857;
            }
          } else {
            if (  (data[22].fvalue < 1.108264)) {
              sum += (float)-0.31106406;
            } else {
              sum += (float)0.52465492;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.11431606)) {
            if (  (data[11].fvalue < 0.015900148)) {
              sum += (float)-0.26810038;
            } else {
              sum += (float)-0.096385047;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.49219716;
            } else {
              sum += (float)0.031277783;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.023463693;
            } else {
              sum += (float)0.40960026;
            }
          } else {
            if (  (data[20].fvalue < 0.90281779)) {
              sum += (float)0.092705525;
            } else {
              sum += (float)0.00045493265;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.015310489)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.29486573;
            } else {
              sum += (float)-0.64801103;
            }
          } else {
            if (  (data[18].fvalue < 0.086290047)) {
              sum += (float)0.26357788;
            } else {
              sum += (float)-0.078708336;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 0.93167126)) {
        if (  (data[11].fvalue < 0.52065015)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.012362585)) {
              sum += (float)0.19851825;
            } else {
              sum += (float)0.44654489;
            }
          } else {
            if (  (data[19].fvalue < 0.10876331)) {
              sum += (float)-0.39466345;
            } else {
              sum += (float)0.1959351;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.2176062)) {
            if (  (data[11].fvalue < 0.80465794)) {
              sum += (float)-0.068020307;
            } else {
              sum += (float)-0.34951729;
            }
          } else {
            if (  (data[11].fvalue < 0.71959788)) {
              sum += (float)0.17777221;
            } else {
              sum += (float)0.014698268;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 4.7637463)) {
          if (  (data[5].fvalue < 20.441746)) {
            if (  (data[11].fvalue < 0.034220353)) {
              sum += (float)0.095831208;
            } else {
              sum += (float)-0.01137496;
            }
          } else {
            if (  (data[11].fvalue < 0.43261039)) {
              sum += (float)0.0095125278;
            } else {
              sum += (float)0.041305039;
            }
          }
        } else {
          if (  (data[5].fvalue < 32.866325)) {
            if (  (data[18].fvalue < 0.012362585)) {
              sum += (float)-0.066165417;
            } else {
              sum += (float)-0.15464257;
            }
          } else {
            if (  (data[11].fvalue < 0.52065015)) {
              sum += (float)-0.10015372;
            } else {
              sum += (float)0.029777205;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.97849363)) {
      if (  (data[19].fvalue < 0.75542355)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.012362585)) {
            if (  (data[5].fvalue < 26.686167)) {
              sum += (float)0.13259023;
            } else {
              sum += (float)-0.40872106;
            }
          } else {
            if (  (data[18].fvalue < 0.025780544)) {
              sum += (float)0.709167;
            } else {
              sum += (float)-0.048534356;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.11012988)) {
              sum += (float)0.23047024;
            } else {
              sum += (float)0.011008596;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.011190986;
            } else {
              sum += (float)-0.29205066;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 19.697397)) {
          if (  (data[11].fvalue < 0.67303526)) {
            if (  (data[19].fvalue < 6.249445)) {
              sum += (float)-0.093356468;
            } else {
              sum += (float)-0.312352;
            }
          } else {
            if (  (data[5].fvalue < 35.248585)) {
              sum += (float)-0.082034282;
            } else {
              sum += (float)-0.011700267;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.092460006;
            } else {
              sum += (float)0.30744624;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.18538243;
            } else {
              sum += (float)-0.1086608;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 7.6975894)) {
        if (  (data[11].fvalue < 0.70195508)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 19.697397)) {
              sum += (float)-0.090572171;
            } else {
              sum += (float)-0.20179771;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.21802463;
            } else {
              sum += (float)-0.096279964;
            }
          }
        } else {
          if (  (data[22].fvalue < 19.697397)) {
            if (  (data[5].fvalue < 34.476624)) {
              sum += (float)-0.085150935;
            } else {
              sum += (float)0.0067713433;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.13693713;
            } else {
              sum += (float)-0.042002264;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.75273973)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.42349452;
            } else {
              sum += (float)-0.25848973;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.10893723;
            } else {
              sum += (float)-0.32280794;
            }
          }
        } else {
          if (  (data[5].fvalue < 39.166168)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.33928764;
            } else {
              sum += (float)-0.16460639;
            }
          } else {
            if (  (data[22].fvalue < 14.955909)) {
              sum += (float)-0.0409813;
            } else {
              sum += (float)-0.16708307;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 0.093829274)) {
    if (  (data[19].fvalue < 0.016535424)) {
      if (  (data[9].fvalue < 1.0852607)) {
        if (  (data[9].fvalue < 0.96296078)) {
          if (  (data[10].fvalue < 0.50683337)) {
            if (  (data[10].fvalue < 0.014860749)) {
              sum += (float)0.47830364;
            } else {
              sum += (float)0.88136595;
            }
          } else {
            if (  (data[6].fvalue < 0.30966997)) {
              sum += (float)0.14942606;
            } else {
              sum += (float)-0.10852531;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.74032134)) {
            if (  (data[9].fvalue < 0.99266052)) {
              sum += (float)-0.14928573;
            } else {
              sum += (float)-0.28213012;
            }
          } else {
            if (  (data[10].fvalue < 0.90645486)) {
              sum += (float)-0.079129316;
            } else {
              sum += (float)0.067067377;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.3053794)) {
              sum += (float)-0.05708909;
            } else {
              sum += (float)0.23033465;
            }
          } else {
            if (  (data[10].fvalue < 0.14365821)) {
              sum += (float)0.06343165;
            } else {
              sum += (float)-0.056961413;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.022952598)) {
            if ( (data[9].fvalue < 1.216719)) {
              sum += (float)0.076865561;
            } else {
              sum += (float)-0.075573914;
            }
          } else {
            if (  (data[9].fvalue < 1.8843355)) {
              sum += (float)0.17603138;
            } else {
              sum += (float)-0.005240296;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 0.99266052)) {
        if (  (data[10].fvalue < 0.35835221)) {
          if (  (data[10].fvalue < 0.16597666)) {
            if (  (data[10].fvalue < 0.032663673)) {
              sum += (float)1.0201535;
            } else {
              sum += (float)0.61753666;
            }
          } else {
            if (  (data[19].fvalue < 0.029946826)) {
              sum += (float)0.17622706;
            } else {
              sum += (float)0.51298356;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.30966997)) {
            if (  (data[11].fvalue < 0.31460589)) {
              sum += (float)0.39289287;
            } else {
              sum += (float)0.10305393;
            }
          } else {
            if (  (data[10].fvalue < 0.68403119)) {
              sum += (float)0.1181262;
            } else {
              sum += (float)0.037511267;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[16].fvalue < 2.999999)) {
            if ( (data[9].fvalue < 2.0274401)) {
              sum += (float)-0.0065740761;
            } else {
              sum += (float)0.040616408;
            }
          } else {
            if (  (data[19].fvalue < 0.25157484)) {
              sum += (float)-0.18118742;
            } else {
              sum += (float)-0.034341283;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.022952598)) {
            if (  (data[10].fvalue < 0.35835221)) {
              sum += (float)0.21864572;
            } else {
              sum += (float)0.39098209;
            }
          } else {
            if (  (data[9].fvalue < 2.7076521)) {
              sum += (float)-0.132733;
            } else {
              sum += (float)0.12407435;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.6953859)) {
      if (  (data[14].fvalue < 0.99999899)) {
        if (  (data[19].fvalue < 0.096650101)) {
          if (  (data[10].fvalue < 0.3254512)) {
            if (  (data[9].fvalue < 1.216719)) {
              sum += (float)0.1440765;
            } else {
              sum += (float)0.0023264857;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.1195228;
            } else {
              sum += (float)0.089392118;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.15858462)) {
            if (  (data[9].fvalue < 1.2615458)) {
              sum += (float)0.067100331;
            } else {
              sum += (float)-0.26640016;
            }
          } else {
            if ( (data[9].fvalue < 1.3053794)) {
              sum += (float)-0.035400562;
            } else {
              sum += (float)-0.13013262;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.016535424)) {
            if (  (data[21].fvalue < 1.9481657)) {
              sum += (float)0.2546187;
            } else {
              sum += (float)-0.11928901;
            }
          } else {
            if (  (data[10].fvalue < 0.39505243)) {
              sum += (float)-0.025424831;
            } else {
              sum += (float)0.11439493;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.80293)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-1.1564883;
            } else {
              sum += (float)-0.50475311;
            }
          } else {
            sum += (float)-0.064469159;
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.55016679)) {
        if (  (data[19].fvalue < 2.4980805)) {
          if (  (data[9].fvalue < 2.8700497)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.0090679834;
            } else {
              sum += (float)-0.15322466;
            }
          } else {
            if (  (data[21].fvalue < 2.6903605)) {
              sum += (float)0.018562099;
            } else {
              sum += (float)-0.0084312586;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.7076521)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.31931353;
            } else {
              sum += (float)-0.16283649;
            }
          } else {
            if (  (data[9].fvalue < 4.3393397)) {
              sum += (float)-0.12554261;
            } else {
              sum += (float)-0.026004756;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 5.5466638)) {
          if (  (data[7].fvalue < 0.67898434)) {
            if (  (data[6].fvalue < 0.30966997)) {
              sum += (float)-0.58073598;
            } else {
              sum += (float)0.19396636;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.13303983;
            } else {
              sum += (float)0.068554699;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 1.6036022)) {
              sum += (float)0.030814428;
            } else {
              sum += (float)-0.078859791;
            }
          } else {
            if (  (data[9].fvalue < 2.1720607)) {
              sum += (float)0.097455919;
            } else {
              sum += (float)0.018825838;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 3.2516549)) {
    if (  (data[18].fvalue < 0.02546607)) {
      if (  (data[13].fvalue < 5.999999)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if ( (data[9].fvalue < 1.8860878)) {
            if (  (data[9].fvalue < 1.032643)) {
              sum += (float)-0.27613246;
            } else {
              sum += (float)0.20512721;
            }
          } else {
            if (  (data[10].fvalue < 0.1093198)) {
              sum += (float)-0.005620603;
            } else {
              sum += (float)-0.45032263;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.0897902)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.10627463;
            } else {
              sum += (float)0.044097871;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.030078402;
            } else {
              sum += (float)-0.056393184;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[18].fvalue < 0.013359056)) {
            if (  (data[21].fvalue < 0.11338384)) {
              sum += (float)-0.24630806;
            } else {
              sum += (float)-0.33016759;
            }
          } else {
            if (  (data[9].fvalue < 1.6971695)) {
              sum += (float)-0.18254176;
            } else {
              sum += (float)-0.044834238;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.013359056)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.39533642;
            } else {
              sum += (float)-0.47666582;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.16814664;
            } else {
              sum += (float)-0.31146049;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 1.1614224)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 0.99308854)) {
            if (  (data[10].fvalue < 0.30502826)) {
              sum += (float)0.7593829;
            } else {
              sum += (float)0.011091409;
            }
          } else {
            if (  (data[18].fvalue < 0.053972472)) {
              sum += (float)-0.46218607;
            } else {
              sum += (float)-0.022971164;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.63903248)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.26212507;
            } else {
              sum += (float)0.087602109;
            }
          } else {
            if (  (data[21].fvalue < 0.075440414)) {
              sum += (float)0.1014166;
            } else {
              sum += (float)-0.16901056;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.6437528)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.16200365)) {
              sum += (float)0.11786396;
            } else {
              sum += (float)0.0010456954;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.11849586;
            } else {
              sum += (float)-0.020524167;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.014623805)) {
            if (  (data[9].fvalue < 1.8860878)) {
              sum += (float)-0.13804418;
            } else {
              sum += (float)0.098557688;
            }
          } else {
            if (  (data[10].fvalue < 0.42345923)) {
              sum += (float)0.0018877457;
            } else {
              sum += (float)0.030220218;
            }
          }
        }
      }
    }
  } else {
    if (  (data[9].fvalue < 2.9522853)) {
      if (  (data[21].fvalue < 1.2145872)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[9].fvalue < 1.9594831)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.035446804;
            } else {
              sum += (float)0.36222243;
            }
          } else {
            if (  (data[5].fvalue < 12.620612)) {
              sum += (float)0.30964881;
            } else {
              sum += (float)0.1461522;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[21].fvalue < 0.094768524)) {
              sum += (float)-0.012397319;
            } else {
              sum += (float)-0.13070913;
            }
          } else {
            if (  (data[10].fvalue < 0.60344356)) {
              sum += (float)-0.28351974;
            } else {
              sum += (float)-0.11280149;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.68371695)) {
              sum += (float)-0.36614949;
            } else {
              sum += (float)-0.14016426;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.090566911;
            } else {
              sum += (float)-0.28308931;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.4707112)) {
            if (  (data[21].fvalue < 2.8624232)) {
              sum += (float)0.23715614;
            } else {
              sum += (float)0.040322691;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.034359761;
            } else {
              sum += (float)-0.082886159;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 4.3395052)) {
        if (  (data[10].fvalue < 0.35810146)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.026561813;
            } else {
              sum += (float)-0.21953905;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.18211606;
            } else {
              sum += (float)-0.2884095;
            }
          }
        } else {
          if (  (data[18].fvalue < 7.7778478)) {
            if (  (data[10].fvalue < 0.63903248)) {
              sum += (float)-0.064985357;
            } else {
              sum += (float)0.050250195;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.020569189;
            } else {
              sum += (float)-0.27263743;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 5.5796962)) {
          if ( (data[9].fvalue < 6.0852599)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.011961;
            } else {
              sum += (float)-0.076296575;
            }
          } else {
            if (  (data[21].fvalue < 0.5298155)) {
              sum += (float)-0.0015217522;
            } else {
              sum += (float)0.11073311;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[5].fvalue < 35.247505)) {
              sum += (float)-0.12403104;
            } else {
              sum += (float)-0.025820522;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)-0.16247961;
            } else {
              sum += (float)0.39351603;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 1.5909761)) {
    if (  (data[20].fvalue < 1.0478909)) {
      if (  (data[9].fvalue < 1.9557272)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.67501462)) {
              sum += (float)-0.00025205803;
            } else {
              sum += (float)-0.087540433;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.19091558;
            } else {
              sum += (float)0.06168396;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.013627515)) {
              sum += (float)0.46210766;
            } else {
              sum += (float)0.19380128;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.063754544;
            } else {
              sum += (float)0.35172597;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.67501462)) {
          if (  (data[22].fvalue < 0.66121483)) {
            if (  (data[9].fvalue < 3.12166)) {
              sum += (float)0.1288434;
            } else {
              sum += (float)0.011873052;
            }
          } else {
            if (  (data[9].fvalue < 2.3864009)) {
              sum += (float)-0.06178673;
            } else {
              sum += (float)0.046870034;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.7829962)) {
            if (  (data[18].fvalue < 0.47266117)) {
              sum += (float)0.016520081;
            } else {
              sum += (float)-0.12187722;
            }
          } else {
            if (  (data[21].fvalue < 1.9618595)) {
              sum += (float)0.069995329;
            } else {
              sum += (float)0.0085707121;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 2.7012839)) {
        if (  (data[21].fvalue < 0.19953398)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 2.5155456)) {
              sum += (float)-0.25632367;
            } else {
              sum += (float)-0.11393791;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.041879173;
            } else {
              sum += (float)-0.18675092;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.037025489;
            } else {
              sum += (float)-0.12427092;
            }
          } else {
            if (  (data[22].fvalue < 0.021012044)) {
              sum += (float)-0.28996739;
            } else {
              sum += (float)-0.030116046;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 2.3958778)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.003444215;
            } else {
              sum += (float)0.056006581;
            }
          } else {
            if (  (data[9].fvalue < 3.12166)) {
              sum += (float)-0.0015373798;
            } else {
              sum += (float)0.20262797;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.10639017)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.052670237;
            } else {
              sum += (float)-0.027714912;
            }
          } else {
            if (  (data[22].fvalue < 0.7034418)) {
              sum += (float)-0.054920111;
            } else {
              sum += (float)0.015150773;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 2.8671644)) {
      if (  (data[18].fvalue < 0.55828154)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.16172449)) {
            if (  (data[7].fvalue < 0.99985003)) {
              sum += (float)0.4009738;
            } else {
              sum += (float)0.048408322;
            }
          } else {
            if (  (data[22].fvalue < 3.4378548)) {
              sum += (float)-0.045661744;
            } else {
              sum += (float)0.16511385;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.013627515)) {
              sum += (float)-0.13629724;
            } else {
              sum += (float)-0.055794127;
            }
          } else {
            if (  (data[9].fvalue < 2.6227367)) {
              sum += (float)0.50917864;
            } else {
              sum += (float)0.070277832;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.72857267)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.28591943;
            } else {
              sum += (float)-0.18686689;
            }
          } else {
            if ( (data[9].fvalue < 2.5444319)) {
              sum += (float)0.40357238;
            } else {
              sum += (float)0.096136488;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.98921192)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.0017931756;
            } else {
              sum += (float)0.23752828;
            }
          } else {
            if (  (data[21].fvalue < 0.98102671)) {
              sum += (float)-0.13944495;
            } else {
              sum += (float)-0.065338202;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 5.2379813)) {
        if (  (data[18].fvalue < 1.5174565)) {
          if (  (data[22].fvalue < 7.5664034)) {
            if (  (data[9].fvalue < 4.8692255)) {
              sum += (float)0.0055321045;
            } else {
              sum += (float)0.059372086;
            }
          } else {
            if (  (data[20].fvalue < 0.10279994)) {
              sum += (float)0.025002483;
            } else {
              sum += (float)-0.027784441;
            }
          }
        } else {
          if (  (data[9].fvalue < 4.0955219)) {
            if (  (data[21].fvalue < 0.8438741)) {
              sum += (float)-0.16252576;
            } else {
              sum += (float)-0.063656539;
            }
          } else {
            if (  (data[22].fvalue < 5.5234871)) {
              sum += (float)0.048136812;
            } else {
              sum += (float)-0.043696146;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.47266117)) {
              sum += (float)0.050324846;
            } else {
              sum += (float)-0.004589566;
            }
          } else {
            if (  (data[14].fvalue < 4.999999)) {
              sum += (float)-0.049802646;
            } else {
              sum += (float)0.096546635;
            }
          }
        } else {
          if (  (data[14].fvalue < 5.999999)) {
            if (  (data[21].fvalue < 7.1426382)) {
              sum += (float)-0.1140394;
            } else {
              sum += (float)-0.20153773;
            }
          } else {
            if (  (data[14].fvalue < 6.999999)) {
              sum += (float)0.30842903;
            } else {
              sum += (float)0.53035098;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 0.72258425)) {
    if (  (data[19].fvalue < 0.05132046)) {
      if (  (data[10].fvalue < 1.1018126)) {
        if (  (data[17].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.68517274)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.0055573578;
            } else {
              sum += (float)0.036000289;
            }
          } else {
            if (  (data[8].fvalue < 0.076650068)) {
              sum += (float)0.82197654;
            } else {
              sum += (float)0.0096447943;
            }
          }
        } else {
          if (  (data[17].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 2.9573207)) {
              sum += (float)-0.18349271;
            } else {
              sum += (float)-0.076876432;
            }
          } else {
            sum += (float)-0.50152695;
          }
        }
      } else {
        if (  (data[8].fvalue < 0.14038496)) {
          if (  (data[8].fvalue < 0.10638121)) {
            if (  (data[8].fvalue < 0.076650068)) {
              sum += (float)1.5403067;
            } else {
              sum += (float)0.85817665;
            }
          } else {
            if (  (data[19].fvalue < 0.017060421)) {
              sum += (float)0.079808228;
            } else {
              sum += (float)0.65781081;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.17528212)) {
            if (  (data[19].fvalue < 0.030727001)) {
              sum += (float)-0.060805656;
            } else {
              sum += (float)0.45264107;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.070246533;
            } else {
              sum += (float)-0.16282265;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.10795361)) {
        if (  (data[17].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.04606805)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.03432174;
            } else {
              sum += (float)0.0059574856;
            }
          } else {
            if (  (data[19].fvalue < 0.20627861)) {
              sum += (float)0.0079984646;
            } else {
              sum += (float)0.11001951;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.16444412)) {
            if (  (data[8].fvalue < 2.3169582)) {
              sum += (float)-0.12959673;
            } else {
              sum += (float)-0.25368822;
            }
          } else {
            if (  (data[22].fvalue < 0.022512412)) {
              sum += (float)-0.072623186;
            } else {
              sum += (float)0.16286989;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.35985315)) {
          if (  (data[8].fvalue < 0.17528212)) {
            if (  (data[10].fvalue < 0.15129878)) {
              sum += (float)-0.177323;
            } else {
              sum += (float)0.059919145;
            }
          } else {
            if (  (data[19].fvalue < 0.16444412)) {
              sum += (float)-0.091697231;
            } else {
              sum += (float)-0.026023315;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 1.6950502)) {
              sum += (float)-0.042066708;
            } else {
              sum += (float)-0.0037078487;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.026610712;
            } else {
              sum += (float)0.00020991343;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 5.5162616)) {
      if (  (data[22].fvalue < 1.4802512)) {
        if (  (data[22].fvalue < 0.022512412)) {
          if (  (data[6].fvalue < 3.2477753)) {
            if (  (data[19].fvalue < 1.0564092)) {
              sum += (float)-0.074571438;
            } else {
              sum += (float)-0.13736558;
            }
          } else {
            if (  (data[10].fvalue < 0.1023284)) {
              sum += (float)-0.011033629;
            } else {
              sum += (float)-0.04559768;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.10795361)) {
            if (  (data[10].fvalue < 0.27626151)) {
              sum += (float)0.10452291;
            } else {
              sum += (float)0.17919713;
            }
          } else {
            if (  (data[19].fvalue < 3.7113881)) {
              sum += (float)0.0035669275;
            } else {
              sum += (float)-0.098668925;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.37043047)) {
          if (  (data[19].fvalue < 1.9338377)) {
            if (  (data[8].fvalue < 2.3169582)) {
              sum += (float)-0.071528561;
            } else {
              sum += (float)-0.019811895;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.18351617;
            } else {
              sum += (float)-0.081996769;
            }
          }
        } else {
          if (  (data[22].fvalue < 2.0561643)) {
            if (  (data[8].fvalue < 3.5580425)) {
              sum += (float)-0.23798671;
            } else {
              sum += (float)-0.064079836;
            }
          } else {
            if (  (data[8].fvalue < 4.0315456)) {
              sum += (float)-0.32392928;
            } else {
              sum += (float)-0.19897704;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 3.9689538)) {
        if (  (data[22].fvalue < 0.022512412)) {
          if (  (data[19].fvalue < 1.4195884)) {
            if (  (data[10].fvalue < 0.24268906)) {
              sum += (float)-0.025881667;
            } else {
              sum += (float)0.040510125;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.038630124;
            } else {
              sum += (float)-0.039669868;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.10795361)) {
            if (  (data[19].fvalue < 6.2540388)) {
              sum += (float)0.17682455;
            } else {
              sum += (float)0.044983357;
            }
          } else {
            if (  (data[19].fvalue < 6.2540388)) {
              sum += (float)0.047442976;
            } else {
              sum += (float)-0.038944907;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 17.346523)) {
          if (  (data[22].fvalue < 15.037466)) {
            if (  (data[10].fvalue < 0.47077352)) {
              sum += (float)-0.016730405;
            } else {
              sum += (float)-0.082816295;
            }
          } else {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.096145198;
            } else {
              sum += (float)-0.039492764;
            }
          }
        } else {
          if (  (data[8].fvalue < 27.902536)) {
            if (  (data[22].fvalue < 15.037466)) {
              sum += (float)0.028373927;
            } else {
              sum += (float)-0.041064899;
            }
          } else {
            if (  (data[10].fvalue < 0.68517274)) {
              sum += (float)0.055515513;
            } else {
              sum += (float)0.13904822;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 3.999999)) {
    if (  (data[12].fvalue < 4.999999)) {
      if (  (data[16].fvalue < 4.999999)) {
        if (  (data[18].fvalue < 0.08666148)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.062397745)) {
              sum += (float)-0.080974832;
            } else {
              sum += (float)0.019478083;
            }
          } else {
            if (  (data[18].fvalue < 0.012709902)) {
              sum += (float)-0.3530196;
            } else {
              sum += (float)-0.018609127;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.42525861)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.053288706;
            } else {
              sum += (float)-0.15370178;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.014375729;
            } else {
              sum += (float)0.073290773;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.33502123)) {
          if (  (data[18].fvalue < 0.25155622)) {
            if (  (data[16].fvalue < 5.999999)) {
              sum += (float)-0.44637102;
            } else {
              sum += (float)-0.6775707;
            }
          } else {
            if (  (data[16].fvalue < 5.999999)) {
              sum += (float)-0.13946906;
            } else {
              sum += (float)-0.34482369;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.30165026)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.30799735;
            } else {
              sum += (float)-0.062872984;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.49126971;
            } else {
              sum += (float)0.084954701;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.5874579)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.23572579)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.096539974;
            } else {
              sum += (float)-0.37250781;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.43932515;
            } else {
              sum += (float)-0.14837438;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.2692844)) {
            if (  (data[18].fvalue < 0.1342677)) {
              sum += (float)-0.60331428;
            } else {
              sum += (float)-0.2198866;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.076003626;
            } else {
              sum += (float)-0.076061234;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 2.0758858)) {
              sum += (float)-0.60091871;
            } else {
              sum += (float)-0.36322471;
            }
          } else {
            if (  (data[18].fvalue < 0.45054281)) {
              sum += (float)-0.5565421;
            } else {
              sum += (float)-0.37060255;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.45054281)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.42354333;
            } else {
              sum += (float)-0.58708638;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.26428729;
            } else {
              sum += (float)-0.014800077;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.19352022)) {
      if (  (data[19].fvalue < 0.097633071)) {
        if (  (data[18].fvalue < 0.35993138)) {
          if (  (data[10].fvalue < 0.47043496)) {
            if (  (data[19].fvalue < 0.074056648)) {
              sum += (float)-0.57811636;
            } else {
              sum += (float)-0.51717508;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.65194649;
            } else {
              sum += (float)-0.56209618;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.074056648)) {
              sum += (float)-0.40325502;
            } else {
              sum += (float)-0.055031337;
            }
          } else {
            if (  (data[19].fvalue < 0.086999081)) {
              sum += (float)-0.55728024;
            } else {
              sum += (float)-0.47429246;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.2692844)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[19].fvalue < 0.15012154)) {
              sum += (float)-0.33758333;
            } else {
              sum += (float)-0.20974754;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.39982641;
            } else {
              sum += (float)-0.51871735;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.91494399)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.024187181;
            } else {
              sum += (float)-0.48761609;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.34081122;
            } else {
              sum += (float)-0.050188124;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[18].fvalue < 0.5889861)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[19].fvalue < 0.43531048)) {
              sum += (float)-0.053255729;
            } else {
              sum += (float)0.070526004;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.20367371;
            } else {
              sum += (float)-0.054075509;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 1.4239223)) {
              sum += (float)-0.36330897;
            } else {
              sum += (float)-0.13736607;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.12194417;
            } else {
              sum += (float)-0.35095748;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.28430787)) {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[19].fvalue < 1.5300487)) {
              sum += (float)-0.12048618;
            } else {
              sum += (float)-0.3070333;
            }
          } else {
            if (  (data[18].fvalue < 0.21886568)) {
              sum += (float)-0.51352304;
            } else {
              sum += (float)-0.37075633;
            }
          }
        } else {
          if (  (data[20].fvalue < 2.8648555)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.043348048;
            } else {
              sum += (float)0.11940196;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.060076717;
            } else {
              sum += (float)0.032354292;
            }
          }
        }
      }
    }
  }
  if (  (data[20].fvalue < 0.12788679)) {
    if (  (data[20].fvalue < 0.01701441)) {
      if (  (data[9].fvalue < 2.3241079)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 0.32986733)) {
            if (  (data[9].fvalue < 0.96269035)) {
              sum += (float)0.11640391;
            } else {
              sum += (float)-0.0034088145;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.038345043;
            } else {
              sum += (float)7.852733e-05;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.0227656)) {
            if (  (data[10].fvalue < 0.30566356)) {
              sum += (float)0.023515813;
            } else {
              sum += (float)0.17740224;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.026668424;
            } else {
              sum += (float)-0.17043187;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)0.063574262;
            } else {
              sum += (float)-0.11574701;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.21056302;
            } else {
              sum += (float)0.056458052;
            }
          }
        } else {
          if (  (data[9].fvalue < 3.5874057)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.052986141;
            } else {
              sum += (float)-0.20432074;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.036030665;
            } else {
              sum += (float)-0.024850287;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 1.9537551)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.11283443;
            } else {
              sum += (float)0.31141973;
            }
          } else {
            if (  (data[20].fvalue < 0.043735038)) {
              sum += (float)0.024725799;
            } else {
              sum += (float)0.11543946;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.27723926)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.025809757;
            } else {
              sum += (float)-0.068297282;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.046773955;
            } else {
              sum += (float)0.27480567;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 0.081908196)) {
            if (  (data[10].fvalue < 0.48757145)) {
              sum += (float)0.23959355;
            } else {
              sum += (float)0.43158644;
            }
          } else {
            if (  (data[20].fvalue < 0.10214849)) {
              sum += (float)0.11798417;
            } else {
              sum += (float)-0.045005295;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.2967434)) {
            if (  (data[9].fvalue < 2.7085767)) {
              sum += (float)0.29538622;
            } else {
              sum += (float)0.5184024;
            }
          } else {
            if (  (data[20].fvalue < 0.10214849)) {
              sum += (float)0.55616635;
            } else {
              sum += (float)0.68584222;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 1.999999)) {
      if (  (data[16].fvalue < 0.99999899)) {
        if (  (data[20].fvalue < 3.0866423)) {
          if ( (data[9].fvalue < 2.7872293)) {
            if (  (data[20].fvalue < 0.77605277)) {
              sum += (float)-0.018433109;
            } else {
              sum += (float)-0.10102132;
            }
          } else {
            if (  (data[20].fvalue < 0.71518564)) {
              sum += (float)-0.040367428;
            } else {
              sum += (float)0.015411268;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.46931547)) {
              sum += (float)0.15090308;
            } else {
              sum += (float)0.013847106;
            }
          } else {
            if ( (data[9].fvalue < 3.119832)) {
              sum += (float)-0.088400111;
            } else {
              sum += (float)0.014746035;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 2.3241079)) {
          if (  (data[20].fvalue < 0.71518564)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.19420801;
            } else {
              sum += (float)0.09548907;
            }
          } else {
            if (  (data[20].fvalue < 1.6111946)) {
              sum += (float)0.023047809;
            } else {
              sum += (float)-0.093204729;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.077077918)) {
            if (  (data[8].fvalue < 0.031080989)) {
              sum += (float)0.41849819;
            } else {
              sum += (float)0.16122232;
            }
          } else {
            if (  (data[20].fvalue < 15.780417)) {
              sum += (float)0.0029823759;
            } else {
              sum += (float)-0.054008599;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.71518564)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 0.19475465)) {
            if ( (data[9].fvalue < 2.3241079)) {
              sum += (float)-0.16420843;
            } else {
              sum += (float)-0.21531852;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.068287037;
            } else {
              sum += (float)-0.18348551;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[20].fvalue < 0.19475465)) {
              sum += (float)0.10680395;
            } else {
              sum += (float)-0.11330813;
            }
          } else {
            if (  (data[20].fvalue < 0.19475465)) {
              sum += (float)0.42459553;
            } else {
              sum += (float)0.070243374;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 4.999999)) {
          if (  (data[20].fvalue < 3.4509599)) {
            if (  (data[9].fvalue < 3.0350637)) {
              sum += (float)-0.049280077;
            } else {
              sum += (float)0.025983378;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.020795938;
            } else {
              sum += (float)-0.10899271;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.042537164;
            } else {
              sum += (float)0.15022431;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.099754527;
            } else {
              sum += (float)0.23775969;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 11.504512)) {
    if (  (data[11].fvalue < 0.70087224)) {
      if (  (data[21].fvalue < 1.4035193)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[18].fvalue < 0.027090892)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0015283102;
            } else {
              sum += (float)-0.55171567;
            }
          } else {
            if (  (data[21].fvalue < 0.021626811)) {
              sum += (float)0.02867021;
            } else {
              sum += (float)-0.0040956447;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.15038221)) {
            if (  (data[18].fvalue < 0.11288051)) {
              sum += (float)-0.46707511;
            } else {
              sum += (float)-0.24404782;
            }
          } else {
            if (  (data[21].fvalue < 0.021626811)) {
              sum += (float)-0.041550674;
            } else {
              sum += (float)0.02220667;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 37.567917)) {
          if (  (data[18].fvalue < 0.66017413)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.0060801045;
            } else {
              sum += (float)-0.2070169;
            }
          } else {
            if (  (data[11].fvalue < 0.21509196)) {
              sum += (float)-0.13978297;
            } else {
              sum += (float)-0.042476267;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.50258106)) {
            if (  (data[11].fvalue < 0.12402065)) {
              sum += (float)-0.04900144;
            } else {
              sum += (float)-0.0035264681;
            }
          } else {
            if (  (data[18].fvalue < 0.23701076)) {
              sum += (float)0.0056301444;
            } else {
              sum += (float)0.044083048;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.3952646)) {
        if (  (data[10].fvalue < 0.049043622)) {
          if (  (data[5].fvalue < 7.3407331)) {
            if (  (data[5].fvalue < 0.98821455)) {
              sum += (float)-0.2416694;
            } else {
              sum += (float)-0.041519232;
            }
          } else {
            if (  (data[6].fvalue < 0.35772809)) {
              sum += (float)0.26856226;
            } else {
              sum += (float)0.086406238;
            }
          }
        } else {
          if (  (data[5].fvalue < 29.812984)) {
            if (  (data[18].fvalue < 0.17752747)) {
              sum += (float)-0.038843405;
            } else {
              sum += (float)-0.0051795696;
            }
          } else {
            if (  (data[21].fvalue < 0.73242182)) {
              sum += (float)-0.0095251678;
            } else {
              sum += (float)0.036620084;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.20701231)) {
          if (  (data[10].fvalue < 0.68551761)) {
            if (  (data[6].fvalue < 0.35772809)) {
              sum += (float)-0.093173243;
            } else {
              sum += (float)0.01010516;
            }
          } else {
            if (  (data[21].fvalue < 0.094406128)) {
              sum += (float)0.11065957;
            } else {
              sum += (float)0.041021343;
            }
          }
        } else {
          if (  (data[5].fvalue < 4.2527213)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.11629916;
            } else {
              sum += (float)0.0033499894;
            }
          } else {
            if (  (data[18].fvalue < 4.8653998)) {
              sum += (float)0.099490397;
            } else {
              sum += (float)0.039220415;
            }
          }
        }
      }
    }
  } else {
    if (  (data[10].fvalue < 0.049043622)) {
      if (  (data[18].fvalue < 0.66017413)) {
        if (  (data[5].fvalue < 29.036417)) {
          if (  (data[5].fvalue < 19.639782)) {
            if (  (data[10].fvalue < 0.015104277)) {
              sum += (float)0.12834927;
            } else {
              sum += (float)0.34362993;
            }
          } else {
            if (  (data[18].fvalue < 0.088410757)) {
              sum += (float)0.16176997;
            } else {
              sum += (float)0.086481005;
            }
          }
        } else {
          if (  (data[6].fvalue < 10.628187)) {
            if (  (data[6].fvalue < 0.35772809)) {
              sum += (float)0.26477924;
            } else {
              sum += (float)0.049158961;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.0053445045;
            } else {
              sum += (float)-0.21886326;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[6].fvalue < 16.387421)) {
              sum += (float)-0.048006903;
            } else {
              sum += (float)-0.14512263;
            }
          } else {
            if (  (data[6].fvalue < 0.35772809)) {
              sum += (float)0.13961244;
            } else {
              sum += (float)-0.18454356;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[10].fvalue < 0.033301741)) {
              sum += (float)-0.0096583972;
            } else {
              sum += (float)-0.092957459;
            }
          } else {
            if (  (data[10].fvalue < 0.015104277)) {
              sum += (float)0.14868574;
            } else {
              sum += (float)0.026275601;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.68551761)) {
        if (  (data[18].fvalue < 0.53309995)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[5].fvalue < 20.41061)) {
              sum += (float)0.1249025;
            } else {
              sum += (float)-0.032503389;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.20796391;
            } else {
              sum += (float)-0.39080274;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.25073749)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.11864297;
            } else {
              sum += (float)-0.046519067;
            }
          } else {
            if (  (data[11].fvalue < 0.71900493)) {
              sum += (float)-0.065497898;
            } else {
              sum += (float)-0.010111667;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.80667871)) {
          if (  (data[6].fvalue < 2.2076316)) {
            if (  (data[6].fvalue < 0.87977999)) {
              sum += (float)0.23939352;
            } else {
              sum += (float)0.13605565;
            }
          } else {
            if (  (data[18].fvalue < 4.8653998)) {
              sum += (float)0.05210536;
            } else {
              sum += (float)-0.090238906;
            }
          }
        } else {
          if (  (data[6].fvalue < 1.5083996)) {
            if (  (data[5].fvalue < 39.968513)) {
              sum += (float)0.31103191;
            } else {
              sum += (float)0.43790412;
            }
          } else {
            if (  (data[18].fvalue < 4.8653998)) {
              sum += (float)0.19237241;
            } else {
              sum += (float)0.027832385;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 2.999999)) {
    if (  (data[22].fvalue < 0.13706802)) {
      if (  (data[22].fvalue < 0.019256612)) {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 2.7109392)) {
              sum += (float)0.0034435051;
            } else {
              sum += (float)0.13346243;
            }
          } else {
            if (  (data[20].fvalue < 0.020764496)) {
              sum += (float)0.061952617;
            } else {
              sum += (float)-0.053748518;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.13674626;
            } else {
              sum += (float)-0.2700178;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.0042440286;
            } else {
              sum += (float)0.17212881;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.82510239)) {
              sum += (float)0.03469;
            } else {
              sum += (float)0.20324124;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.08488483;
            } else {
              sum += (float)0.04835964;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.084645636)) {
              sum += (float)0.20835401;
            } else {
              sum += (float)0.012440449;
            }
          } else {
            if (  (data[22].fvalue < 0.1087746)) {
              sum += (float)0.39912304;
            } else {
              sum += (float)0.62312657;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 2.999999)) {
        if (  (data[22].fvalue < 0.20611514)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.049690962;
            } else {
              sum += (float)-0.18792835;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.11863976;
            } else {
              sum += (float)0.36947429;
            }
          }
        } else {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.0091845281;
            } else {
              sum += (float)0.031111946;
            }
          } else {
            if (  (data[18].fvalue < 0.77925378)) {
              sum += (float)-0.30775213;
            } else {
              sum += (float)-0.052493416;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 0.020764496)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.35434654;
            } else {
              sum += (float)0.13115497;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.070142202;
            } else {
              sum += (float)-0.046537802;
            }
          }
        } else {
          if (  (data[20].fvalue < 3.9409356)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.11071587;
            } else {
              sum += (float)0.26537797;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.025404397;
            } else {
              sum += (float)0.12047794;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if (  (data[16].fvalue < 0.99999899)) {
        if (  (data[18].fvalue < 0.099195525)) {
          if (  (data[5].fvalue < 32.898724)) {
            sum += (float)-0.44094422;
          } else {
            sum += (float)-0.30671379;
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[5].fvalue < 39.204472)) {
              sum += (float)-0.28739315;
            } else {
              sum += (float)-0.20375006;
            }
          } else {
            sum += (float)-0.38859454;
          }
        }
      } else {
        if (  (data[18].fvalue < 0.16486089)) {
          if (  (data[5].fvalue < 40.701939)) {
            if (  (data[21].fvalue < 0.14152344)) {
              sum += (float)-0.24750958;
            } else {
              sum += (float)-0.32539833;
            }
          } else {
            if (  (data[5].fvalue < 43.863583)) {
              sum += (float)-0.12747115;
            } else {
              sum += (float)-0.28446162;
            }
          }
        } else {
          if (  (data[21].fvalue < 1.6097062)) {
            if (  (data[5].fvalue < 34.489712)) {
              sum += (float)-0.19386527;
            } else {
              sum += (float)-0.077279702;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.20415409;
            } else {
              sum += (float)-0.33871481;
            }
          }
        }
      }
    } else {
      sum += (float)-0.49786195;
    }
  }
  if (  (data[19].fvalue < 1.5306826)) {
    if (  (data[12].fvalue < 14.999999)) {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.016983353)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0085201263;
            } else {
              sum += (float)0.049146641;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.0053341957;
            } else {
              sum += (float)-0.02078159;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.061826255)) {
            if (  (data[11].fvalue < 0.46308684)) {
              sum += (float)-0.12028097;
            } else {
              sum += (float)-0.059271708;
            }
          } else {
            if (  (data[22].fvalue < 0.084727012)) {
              sum += (float)0.052950475;
            } else {
              sum += (float)-0.0036297794;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.016983353)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.08921013;
            } else {
              sum += (float)-0.27019638;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.11964999;
            } else {
              sum += (float)-0.21978757;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.13697842)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.12173025;
            } else {
              sum += (float)-0.020109167;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.010544217;
            } else {
              sum += (float)0.024176141;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.99669552)) {
        if (  (data[12].fvalue < 15.999999)) {
          if (  (data[19].fvalue < 0.43576363)) {
            if (  (data[19].fvalue < 0.26841637)) {
              sum += (float)-0.47399175;
            } else {
              sum += (float)-0.33700556;
            }
          } else {
            if (  (data[19].fvalue < 0.75271142)) {
              sum += (float)-0.25416112;
            } else {
              sum += (float)-0.14359142;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.75271142)) {
            if (  (data[12].fvalue < 17.999998)) {
              sum += (float)-0.41959366;
            } else {
              sum += (float)-0.49958271;
            }
          } else {
            if (  (data[12].fvalue < 16.999998)) {
              sum += (float)-0.24703261;
            } else {
              sum += (float)-0.36669421;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 20.999998)) {
          if (  (data[12].fvalue < 15.999999)) {
            if (  (data[22].fvalue < 2.509017)) {
              sum += (float)0.066379219;
            } else {
              sum += (float)-0.08389318;
            }
          } else {
            if (  (data[19].fvalue < 1.1784581)) {
              sum += (float)-0.26482576;
            } else {
              sum += (float)-0.10916604;
            }
          }
        } else {
          sum += (float)0.67423481;
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.021509623)) {
            if (  (data[5].fvalue < 18.025993)) {
              sum += (float)-0.088503234;
            } else {
              sum += (float)-0.0050943675;
            }
          } else {
            if (  (data[22].fvalue < 0.60861468)) {
              sum += (float)0.099891357;
            } else {
              sum += (float)-0.013026073;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.021509623)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.054865316;
            } else {
              sum += (float)-0.056984279;
            }
          } else {
            if (  (data[22].fvalue < 0.36120686)) {
              sum += (float)-0.16717711;
            } else {
              sum += (float)-0.053471323;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 6.6448331)) {
              sum += (float)0.18489581;
            } else {
              sum += (float)-0.14049113;
            }
          } else {
            if (  (data[22].fvalue < 0.021509623)) {
              sum += (float)0.10888506;
            } else {
              sum += (float)0.022155544;
            }
          }
        } else {
          if (  (data[19].fvalue < 6.6448331)) {
            if (  (data[5].fvalue < 27.480171)) {
              sum += (float)-0.042754274;
            } else {
              sum += (float)-0.00038422376;
            }
          } else {
            if (  (data[22].fvalue < 0.43809411)) {
              sum += (float)-0.17427082;
            } else {
              sum += (float)-0.066346139;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 2.999999)) {
        if (  (data[11].fvalue < 0.38405263)) {
          if (  (data[5].fvalue < 31.33777)) {
            if (  (data[19].fvalue < 1.9360998)) {
              sum += (float)-0.13337874;
            } else {
              sum += (float)-0.19403557;
            }
          } else {
            if (  (data[11].fvalue < 0.20428123)) {
              sum += (float)-0.15834615;
            } else {
              sum += (float)-0.080389373;
            }
          }
        } else {
          if (  (data[5].fvalue < 16.598923)) {
            if (  (data[19].fvalue < 1.9360998)) {
              sum += (float)-0.10702435;
            } else {
              sum += (float)-0.18092538;
            }
          } else {
            if (  (data[19].fvalue < 6.6448331)) {
              sum += (float)-0.044525675;
            } else {
              sum += (float)-0.14097062;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 5.999999)) {
          if (  (data[22].fvalue < 4.2165089)) {
            if (  (data[22].fvalue < 0.021509623)) {
              sum += (float)-0.055439495;
            } else {
              sum += (float)0.039475825;
            }
          } else {
            if (  (data[12].fvalue < 18.999998)) {
              sum += (float)-0.061743185;
            } else {
              sum += (float)0.37862545;
            }
          }
        } else {
          if (  (data[14].fvalue < 6.999999)) {
            if (  (data[22].fvalue < 1.5985739)) {
              sum += (float)0.38734633;
            } else {
              sum += (float)0.26240128;
            }
          } else {
            sum += (float)0.47856247;
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 1.999999)) {
    if (  (data[8].fvalue < 1.6900737)) {
      if (  (data[19].fvalue < 0.45858723)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.48707777)) {
            if (  (data[19].fvalue < 0.13826112)) {
              sum += (float)0.035049085;
            } else {
              sum += (float)-0.0087585803;
            }
          } else {
            if (  (data[8].fvalue < 0.0751113)) {
              sum += (float)0.32521361;
            } else {
              sum += (float)-0.028989773;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.016392075)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.0082225902;
            } else {
              sum += (float)-0.14313261;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.010535147;
            } else {
              sum += (float)-0.087008364;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.803599)) {
          if (  (data[10].fvalue < 0.014695535)) {
            if (  (data[8].fvalue < 0.10340384)) {
              sum += (float)0.108363;
            } else {
              sum += (float)0.0036146268;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.040665749;
            } else {
              sum += (float)-0.017108021;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.0485009)) {
            if (  (data[8].fvalue < 0.77177823)) {
              sum += (float)0.76603955;
            } else {
              sum += (float)0.18818678;
            }
          } else {
            if (  (data[19].fvalue < 0.68692952)) {
              sum += (float)-0.015251333;
            } else {
              sum += (float)0.14018276;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 5.999999)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.011142975;
            } else {
              sum += (float)-0.027325552;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.2131318;
            } else {
              sum += (float)0.072631568;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.050558683;
            } else {
              sum += (float)-0.014567907;
            }
          } else {
            if (  (data[19].fvalue < 0.22198705)) {
              sum += (float)0.0078051775;
            } else {
              sum += (float)0.06001737;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[6].fvalue < 9.6538591)) {
              sum += (float)0.31583989;
            } else {
              sum += (float)0.48707816;
            }
          } else {
            if (  (data[19].fvalue < 0.3540529)) {
              sum += (float)0.75955498;
            } else {
              sum += (float)0.50754768;
            }
          }
        } else {
          if (  (data[15].fvalue < 6.999999)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.10704832;
            } else {
              sum += (float)0.37010136;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.33632967;
            } else {
              sum += (float)0.55072218;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 3.999999)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 1.2632809)) {
            if (  (data[19].fvalue < 0.041202638)) {
              sum += (float)-0.40329525;
            } else {
              sum += (float)-0.13237272;
            }
          } else {
            if (  (data[10].fvalue < 0.34584925)) {
              sum += (float)-0.26036429;
            } else {
              sum += (float)-0.17521076;
            }
          }
        } else {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[8].fvalue < 11.862876)) {
              sum += (float)-0.10097837;
            } else {
              sum += (float)-0.001586639;
            }
          } else {
            if (  (data[19].fvalue < 0.39610276)) {
              sum += (float)-0.29952055;
            } else {
              sum += (float)-0.1862036;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 4.7649622)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.014695535)) {
              sum += (float)0.21796784;
            } else {
              sum += (float)-0.087211728;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.035420429;
            } else {
              sum += (float)0.12133787;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.34607863;
            } else {
              sum += (float)0.0029063991;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.02205232;
            } else {
              sum += (float)0.16202568;
            }
          }
        }
      }
    } else {
      sum += (float)-0.49155575;
    }
  }
  if (  (data[20].fvalue < 12.178804)) {
    if (  (data[8].fvalue < 8.4523764)) {
      if (  (data[18].fvalue < 1.241549)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.014846549)) {
              sum += (float)-0.010537622;
            } else {
              sum += (float)0.019295242;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.05996703;
            } else {
              sum += (float)0.006591327;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.014846549)) {
              sum += (float)-0.11688385;
            } else {
              sum += (float)-0.011960543;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.047839366;
            } else {
              sum += (float)0.0096984515;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.01837362)) {
              sum += (float)-0.034023128;
            } else {
              sum += (float)-0.00081956299;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.21058522;
            } else {
              sum += (float)0.076861136;
            }
          }
        } else {
          if (  (data[13].fvalue < 13.999999)) {
            if (  (data[8].fvalue < 2.0314729)) {
              sum += (float)-0.10199157;
            } else {
              sum += (float)-0.060050912;
            }
          } else {
            if (  (data[13].fvalue < 21.999998)) {
              sum += (float)0.16892144;
            } else {
              sum += (float)0.84152681;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.23367657)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.45901331;
            } else {
              sum += (float)-0.42136768;
            }
          } else {
            if (  (data[18].fvalue < 0.11240052)) {
              sum += (float)-0.60863268;
            } else {
              sum += (float)-0.19208916;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.014846549)) {
              sum += (float)-0.014596895;
            } else {
              sum += (float)-0.16113237;
            }
          } else {
            if (  (data[20].fvalue < 0.55612075)) {
              sum += (float)0.0010252318;
            } else {
              sum += (float)0.079993278;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 4.8837724)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.042719115;
            } else {
              sum += (float)-0.0078051919;
            }
          } else {
            if (  (data[20].fvalue < 2.7085366)) {
              sum += (float)0.089060262;
            } else {
              sum += (float)0.042558219;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[6].fvalue < 13.340805)) {
              sum += (float)0.052827138;
            } else {
              sum += (float)-0.0087444428;
            }
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.06653212;
            } else {
              sum += (float)0.098149195;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 2.999999)) {
      if (  (data[8].fvalue < 5.51229)) {
        if (  (data[18].fvalue < 0.099336855)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.12114551;
            } else {
              sum += (float)0.047039259;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.53740525;
            } else {
              sum += (float)-0.059546489;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.35761675)) {
            if (  (data[7].fvalue < 0.98041785)) {
              sum += (float)0.049938157;
            } else {
              sum += (float)0.83403617;
            }
          } else {
            if (  (data[8].fvalue < 0.17497072)) {
              sum += (float)0.28111005;
            } else {
              sum += (float)-0.0091708628;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 0.8682391)) {
              sum += (float)0.020012854;
            } else {
              sum += (float)-0.03801474;
            }
          } else {
            if (  (data[18].fvalue < 1.3336056)) {
              sum += (float)-0.061806653;
            } else {
              sum += (float)-0.15508276;
            }
          }
        } else {
          if (  (data[8].fvalue < 15.011057)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.01664703;
            } else {
              sum += (float)0.11072668;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.10838635;
            } else {
              sum += (float)0.27017781;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.99999899)) {
        if (  (data[18].fvalue < 0.087281868)) {
          if (  (data[8].fvalue < 9.1587553)) {
            if (  (data[18].fvalue < 0.014846549)) {
              sum += (float)-0.50019312;
            } else {
              sum += (float)-0.15683921;
            }
          } else {
            if (  (data[18].fvalue < 0.027823552)) {
              sum += (float)-0.5264287;
            } else {
              sum += (float)-0.84125239;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.34224692)) {
              sum += (float)-0.014393483;
            } else {
              sum += (float)0.22552978;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.020658813;
            } else {
              sum += (float)-0.044869803;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.29450548;
            } else {
              sum += (float)0.08745534;
            }
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.062912725;
            } else {
              sum += (float)-0.22256759;
            }
          }
        } else {
          if (  (data[8].fvalue < 27.699776)) {
            if (  (data[6].fvalue < 0.35761675)) {
              sum += (float)0.027247803;
            } else {
              sum += (float)-0.10825235;
            }
          } else {
            if (  (data[18].fvalue < 0.87474275)) {
              sum += (float)-0.06216618;
            } else {
              sum += (float)0.022896832;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 1.709039)) {
    if ( (data[9].fvalue < 2.0880399)) {
      if (  (data[10].fvalue < 0.057895616)) {
        if (  (data[9].fvalue < 0.99209535)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.014638946)) {
              sum += (float)0.30438283;
            } else {
              sum += (float)0.82874709;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.15754484;
            } else {
              sum += (float)-0.59432483;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.8775593)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.042476427;
            } else {
              sum += (float)-0.12814447;
            }
          } else {
            if (  (data[21].fvalue < 0.094047189)) {
              sum += (float)0.11810283;
            } else {
              sum += (float)-0.014514286;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.7892378)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.075915217)) {
              sum += (float)0.023585385;
            } else {
              sum += (float)-0.019968703;
            }
          } else {
            if (  (data[21].fvalue < 0.022685643)) {
              sum += (float)-0.035449848;
            } else {
              sum += (float)0.11295339;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.50451154)) {
            if (  (data[10].fvalue < 0.14386642)) {
              sum += (float)0.0046620839;
            } else {
              sum += (float)-0.10350719;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0077265259;
            } else {
              sum += (float)0.12109047;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[21].fvalue < 0.075915217)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.12514672)) {
              sum += (float)-0.25111637;
            } else {
              sum += (float)0.048432555;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.1010792;
            } else {
              sum += (float)0.017817391;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.60945338;
            } else {
              sum += (float)0.051956799;
            }
          } else {
            if (  (data[18].fvalue < 0.09930484)) {
              sum += (float)-0.074980818;
            } else {
              sum += (float)0.00027978327;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.22187619)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.21806742)) {
              sum += (float)-0.027758241;
            } else {
              sum += (float)-0.34582391;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.027141849;
            } else {
              sum += (float)-0.03638494;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.022685643)) {
              sum += (float)0.0047774599;
            } else {
              sum += (float)0.092393778;
            }
          } else {
            if (  (data[21].fvalue < 0.72864121)) {
              sum += (float)-0.01639122;
            } else {
              sum += (float)0.062256917;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 2.9456027)) {
      if (  (data[18].fvalue < 0.38193923)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.1371129)) {
            if (  (data[10].fvalue < 0.024562372)) {
              sum += (float)0.095039025;
            } else {
              sum += (float)0.51796556;
            }
          } else {
            if (  (data[21].fvalue < 2.5112517)) {
              sum += (float)-0.0018371844;
            } else {
              sum += (float)0.154402;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.014638946)) {
            if ( (data[9].fvalue < 2.6231761)) {
              sum += (float)-0.18278718;
            } else {
              sum += (float)-0.076675393;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.06193538;
            } else {
              sum += (float)-0.11579991;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[10].fvalue < 0.25960189)) {
            if (  (data[10].fvalue < 0.08832445)) {
              sum += (float)-0.34041187;
            } else {
              sum += (float)-0.20296606;
            }
          } else {
            if ( (data[9].fvalue < 2.3817747)) {
              sum += (float)-0.15053535;
            } else {
              sum += (float)-0.062773488;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.038377427;
            } else {
              sum += (float)0.17469294;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.11367405;
            } else {
              sum += (float)0.0045068273;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 6.8488874)) {
        if (  (data[9].fvalue < 6.2581601)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 2.5055923)) {
              sum += (float)0.00027905931;
            } else {
              sum += (float)-0.045296848;
            }
          } else {
            if (  (data[18].fvalue < 0.2509636)) {
              sum += (float)-0.14055003;
            } else {
              sum += (float)-0.010224097;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.082187451;
            } else {
              sum += (float)0.07510376;
            }
          } else {
            if (  (data[18].fvalue < 0.17581145)) {
              sum += (float)-0.024229947;
            } else {
              sum += (float)0.033208977;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.62396592)) {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.0082706874;
            } else {
              sum += (float)-0.096696913;
            }
          } else {
            if (  (data[18].fvalue < 0.42848459)) {
              sum += (float)-0.33577287;
            } else {
              sum += (float)-0.17010465;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[6].fvalue < 3.9913359)) {
              sum += (float)-0.17363755;
            } else {
              sum += (float)-0.067010351;
            }
          } else {
            if (  (data[10].fvalue < 0.066059835)) {
              sum += (float)0.069588661;
            } else {
              sum += (float)-0.026405256;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 8.4991188)) {
    if (  (data[8].fvalue < 4.0111341)) {
      if (  (data[7].fvalue < 0.99999899)) {
        if (  (data[8].fvalue < 0.033297703)) {
          if (  (data[20].fvalue < 0.56124651)) {
            if (  (data[7].fvalue < 0.98813981)) {
              sum += (float)0.095051296;
            } else {
              sum += (float)0.016064631;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.27080864;
            } else {
              sum += (float)0.41660756;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.89377534)) {
            if (  (data[20].fvalue < 1.2224163)) {
              sum += (float)0.0038436933;
            } else {
              sum += (float)-0.024323827;
            }
          } else {
            if (  (data[5].fvalue < 25.06813)) {
              sum += (float)-0.036108859;
            } else {
              sum += (float)0.0026948003;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.61018521)) {
          if (  (data[21].fvalue < 0.58145171)) {
            if (  (data[20].fvalue < 0.56124651)) {
              sum += (float)-0.56952846;
            } else {
              sum += (float)0.073258117;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.12392354;
            } else {
              sum += (float)0.34363684;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            sum += (float)0.023337387;
          } else {
            sum += (float)0.52450025;
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 3.2492208)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.018467661)) {
              sum += (float)-0.018321635;
            } else {
              sum += (float)0.071943186;
            }
          } else {
            if (  (data[20].fvalue < 0.10175651)) {
              sum += (float)0.027324937;
            } else {
              sum += (float)-0.01676012;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.20008473)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.29542288;
            } else {
              sum += (float)0.60175753;
            }
          } else {
            if (  (data[20].fvalue < 4.3064456)) {
              sum += (float)0.09366931;
            } else {
              sum += (float)0.016769337;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.034012537;
            } else {
              sum += (float)-0.0014723083;
            }
          } else {
            if (  (data[20].fvalue < 1.6183281)) {
              sum += (float)-0.014376791;
            } else {
              sum += (float)-0.07986705;
            }
          }
        } else {
          if (  (data[21].fvalue < 6.1597409)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.038088854;
            } else {
              sum += (float)0.12805623;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.032510452;
            } else {
              sum += (float)0.026340706;
            }
          }
        }
      }
    }
  } else {
    if (  (data[21].fvalue < 0.094600037)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 1.2224163)) {
              sum += (float)0.14415139;
            } else {
              sum += (float)0.3023459;
            }
          } else {
            if (  (data[20].fvalue < 1.2224163)) {
              sum += (float)-0.049117308;
            } else {
              sum += (float)0.097614177;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.018467661)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.08666414;
            } else {
              sum += (float)0.03060912;
            }
          } else {
            if (  (data[20].fvalue < 0.10175651)) {
              sum += (float)0.2032419;
            } else {
              sum += (float)0.041222032;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[20].fvalue < 0.10175651)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.057023786;
            } else {
              sum += (float)0.16696121;
            }
          } else {
            if (  (data[20].fvalue < 0.56124651)) {
              sum += (float)-0.076364569;
            } else {
              sum += (float)0.018996051;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.16387512)) {
              sum += (float)-0.030880593;
            } else {
              sum += (float)-0.15428482;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.1066687;
            } else {
              sum += (float)0.50055218;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 0.49189639)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 14.976016)) {
              sum += (float)-0.075616457;
            } else {
              sum += (float)-0.12917721;
            }
          } else {
            if (  (data[21].fvalue < 0.22784664)) {
              sum += (float)-0.24795292;
            } else {
              sum += (float)-0.11428443;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.20008473)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.25896662;
            } else {
              sum += (float)0.52547204;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.15270524;
            } else {
              sum += (float)-0.015022325;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 9.999999)) {
          if (  (data[22].fvalue < 19.922884)) {
            if (  (data[5].fvalue < 41.451492)) {
              sum += (float)-0.040523082;
            } else {
              sum += (float)-0.009918307;
            }
          } else {
            if (  (data[8].fvalue < 27.482985)) {
              sum += (float)-0.076582566;
            } else {
              sum += (float)-0.022826243;
            }
          }
        } else {
          if (  (data[20].fvalue < 1.8492104)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.15839075;
            } else {
              sum += (float)-0.025238207;
            }
          } else {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)-0.20081316;
            } else {
              sum += (float)0.06681034;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 1.1006106)) {
    if (  (data[12].fvalue < 12.999999)) {
      if (  (data[11].fvalue < 0.034215942)) {
        if (  (data[6].fvalue < 1.5327506)) {
          if (  (data[7].fvalue < 0.99789441)) {
            if (  (data[5].fvalue < 1.0993913)) {
              sum += (float)0.64711529;
            } else {
              sum += (float)0.51398164;
            }
          } else {
            sum += (float)0.24151032;
          }
        } else {
          if (  (data[19].fvalue < 0.03083571)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.18939246;
            } else {
              sum += (float)-0.035796572;
            }
          } else {
            if (  (data[5].fvalue < 29.794361)) {
              sum += (float)0.022509923;
            } else {
              sum += (float)-0.031650659;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.076831385)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 3.4575803)) {
              sum += (float)-0.0052867462;
            } else {
              sum += (float)0.04335203;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0072221491;
            } else {
              sum += (float)0.037829164;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.2268974)) {
            if (  (data[19].fvalue < 0.017316006)) {
              sum += (float)0.030776329;
            } else {
              sum += (float)-0.059308067;
            }
          } else {
            if (  (data[19].fvalue < 0.12365451)) {
              sum += (float)-0.0056491331;
            } else {
              sum += (float)0.011233296;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.39535642)) {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[19].fvalue < 0.22274479)) {
            if (  (data[19].fvalue < 0.16543016)) {
              sum += (float)-0.48272604;
            } else {
              sum += (float)-0.42209244;
            }
          } else {
            if (  (data[12].fvalue < 15.999999)) {
              sum += (float)-0.30981991;
            } else {
              sum += (float)-0.44676581;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.22274479)) {
            sum += (float)-0.36493942;
          } else {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)0.074477971;
            } else {
              sum += (float)-0.16830969;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 16.999998)) {
          if (  (data[19].fvalue < 0.83782637)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.18108147;
            } else {
              sum += (float)-0.08687038;
            }
          } else {
            if (  (data[21].fvalue < 0.021978762)) {
              sum += (float)-0.12496111;
            } else {
              sum += (float)-0.019172655;
            }
          }
        } else {
          if (  (data[16].fvalue < 4.999999)) {
            if (  (data[19].fvalue < 0.83782637)) {
              sum += (float)-0.3572849;
            } else {
              sum += (float)-0.2580981;
            }
          } else {
            sum += (float)0.0097051701;
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 1.999999)) {
      if (  (data[21].fvalue < 0.021978762)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 0.66065007)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.063042909;
            } else {
              sum += (float)-0.15395266;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.29679555;
            } else {
              sum += (float)-0.013222563;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 0.91201448)) {
              sum += (float)0.054965798;
            } else {
              sum += (float)-0.25273579;
            }
          } else {
            if (  (data[20].fvalue < 1.7037979)) {
              sum += (float)-0.083953857;
            } else {
              sum += (float)-0.18402889;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 1.313168)) {
          if (  (data[20].fvalue < 1.6158564)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.12613328;
            } else {
              sum += (float)0.0063937171;
            }
          } else {
            if (  (data[11].fvalue < 0.56144416)) {
              sum += (float)-0.076478057;
            } else {
              sum += (float)-0.005879275;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.016892474)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.32054377;
            } else {
              sum += (float)-0.060100209;
            }
          } else {
            if (  (data[10].fvalue < 0.34713054)) {
              sum += (float)-0.047140196;
            } else {
              sum += (float)0.0069464031;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 0.99999899)) {
        if (  (data[21].fvalue < 0.63390642)) {
          if (  (data[11].fvalue < 0.12393548)) {
            if (  (data[20].fvalue < 0.39399588)) {
              sum += (float)0.2754356;
            } else {
              sum += (float)0.098952495;
            }
          } else {
            if (  (data[5].fvalue < 8.0536432)) {
              sum += (float)0.23653404;
            } else {
              sum += (float)0.078060657;
            }
          }
        } else {
          if (  (data[20].fvalue < 3.4350202)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.020916268;
            } else {
              sum += (float)0.068156898;
            }
          } else {
            if (  (data[21].fvalue < 6.1760287)) {
              sum += (float)-0.012489616;
            } else {
              sum += (float)-0.077627584;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 3.999999)) {
          if (  (data[19].fvalue < 1.9305007)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.010022462;
            } else {
              sum += (float)0.074712172;
            }
          } else {
            if (  (data[11].fvalue < 0.46551555)) {
              sum += (float)-0.094701923;
            } else {
              sum += (float)-0.019429717;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[11].fvalue < 0.5011254)) {
              sum += (float)-0.045867339;
            } else {
              sum += (float)0.055356145;
            }
          } else {
            if (  (data[16].fvalue < 5.999999)) {
              sum += (float)0.072645076;
            } else {
              sum += (float)0.22794473;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 2.6916094)) {
    if (  (data[8].fvalue < 1.5711209)) {
      if (  (data[18].fvalue < 0.72928935)) {
        if (  (data[20].fvalue < 2.101815)) {
          if (  (data[21].fvalue < 3.4818013)) {
            if (  (data[22].fvalue < 0.13880605)) {
              sum += (float)-0.0014648871;
            } else {
              sum += (float)-0.020409262;
            }
          } else {
            if (  (data[19].fvalue < 0.7219646)) {
              sum += (float)0.088014774;
            } else {
              sum += (float)-0.050654639;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.31885374)) {
            if (  (data[18].fvalue < 0.099815294)) {
              sum += (float)0.23544246;
            } else {
              sum += (float)0.020358518;
            }
          } else {
            if (  (data[19].fvalue < 1.3433214)) {
              sum += (float)-0.00020560647;
            } else {
              sum += (float)-0.097656593;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.021419307)) {
            if (  (data[20].fvalue < 0.018445697)) {
              sum += (float)-0.15959355;
            } else {
              sum += (float)-0.035894044;
            }
          } else {
            if (  (data[18].fvalue < 2.5060799)) {
              sum += (float)-0.022469282;
            } else {
              sum += (float)-0.14248542;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.65988106)) {
            if (  (data[18].fvalue < 1.5308628)) {
              sum += (float)0.01351245;
            } else {
              sum += (float)0.13735315;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.073850386;
            } else {
              sum += (float)0.042885207;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.1764528)) {
        if (  (data[19].fvalue < 0.073638342)) {
          if (  (data[10].fvalue < 0.64044648)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.16493528;
            } else {
              sum += (float)-0.64228374;
            }
          } else {
            if (  (data[22].fvalue < 0.021419307)) {
              sum += (float)0.63741803;
            } else {
              sum += (float)-0.77668262;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.0530217)) {
            if (  (data[20].fvalue < 2.8724165)) {
              sum += (float)0.013987693;
            } else {
              sum += (float)0.0560477;
            }
          } else {
            if (  (data[18].fvalue < 0.01488565)) {
              sum += (float)0.011848448;
            } else {
              sum += (float)-0.043692503;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.021419307)) {
          if (  (data[10].fvalue < 0.39538005)) {
            if (  (data[20].fvalue < 0.10445996)) {
              sum += (float)0.042960532;
            } else {
              sum += (float)0.0042097112;
            }
          } else {
            if (  (data[19].fvalue < 0.43554375)) {
              sum += (float)0.0016303913;
            } else {
              sum += (float)-0.071676351;
            }
          }
        } else {
          if (  (data[21].fvalue < 2.8585467)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.017141314;
            } else {
              sum += (float)0.066739358;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.0015621613;
            } else {
              sum += (float)0.09089648;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.91779411)) {
      if (  (data[19].fvalue < 0.5563221)) {
        if (  (data[18].fvalue < 0.14957289)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.018445697)) {
              sum += (float)0.18868028;
            } else {
              sum += (float)0.059673786;
            }
          } else {
            if (  (data[19].fvalue < 0.098333262)) {
              sum += (float)-0.39979658;
            } else {
              sum += (float)-0.015359387;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.060798179)) {
            if (  (data[19].fvalue < 0.098333262)) {
              sum += (float)0.081980124;
            } else {
              sum += (float)0.023093836;
            }
          } else {
            if (  (data[18].fvalue < 0.32205585)) {
              sum += (float)0.020584712;
            } else {
              sum += (float)-0.015017981;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 8.4008121)) {
          if (  (data[10].fvalue < 0.4092533)) {
            if (  (data[19].fvalue < 1.626359)) {
              sum += (float)-0.026706019;
            } else {
              sum += (float)-0.066853084;
            }
          } else {
            if (  (data[19].fvalue < 1.1044329)) {
              sum += (float)-0.039462086;
            } else {
              sum += (float)-0.21246159;
            }
          }
        } else {
          if (  (data[20].fvalue < 8.3920488)) {
            if (  (data[19].fvalue < 4.7671366)) {
              sum += (float)0.016483122;
            } else {
              sum += (float)-0.057889476;
            }
          } else {
            if (  (data[8].fvalue < 27.402815)) {
              sum += (float)-0.071345858;
            } else {
              sum += (float)0.0032335871;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 17.496271)) {
        if (  (data[19].fvalue < 1.2573264)) {
          if (  (data[18].fvalue < 2.3446243)) {
            if (  (data[8].fvalue < 7.2041402)) {
              sum += (float)-0.058259215;
            } else {
              sum += (float)-0.017756321;
            }
          } else {
            if (  (data[21].fvalue < 1.7228475)) {
              sum += (float)-0.10825612;
            } else {
              sum += (float)-0.052147362;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.73615307)) {
            if (  (data[22].fvalue < 4.795423)) {
              sum += (float)0.020462951;
            } else {
              sum += (float)-0.063572049;
            }
          } else {
            if (  (data[18].fvalue < 1.3275143)) {
              sum += (float)0.013904293;
            } else {
              sum += (float)0.087964386;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 1.0530217)) {
          if (  (data[18].fvalue < 7.8568525)) {
            if (  (data[21].fvalue < 15.469204)) {
              sum += (float)0.031390082;
            } else {
              sum += (float)-0.084246077;
            }
          } else {
            if (  (data[10].fvalue < 0.42432532)) {
              sum += (float)-0.023054641;
            } else {
              sum += (float)-0.097976871;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 11.127339)) {
              sum += (float)0.093663856;
            } else {
              sum += (float)0.014998064;
            }
          } else {
            if (  (data[10].fvalue < 0.5769189)) {
              sum += (float)0.12586008;
            } else {
              sum += (float)0.25484106;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 4.8163004)) {
    if (  (data[4].fvalue < 0.99999899)) {
      if (  (data[11].fvalue < 0.008949114)) {
        if (  (data[18].fvalue < 0.014950917)) {
          if (  (data[7].fvalue < 0.99852133)) {
            if (  (data[6].fvalue < 6.4214554)) {
              sum += (float)0.51000816;
            } else {
              sum += (float)0.21321246;
            }
          } else {
            if (  (data[22].fvalue < 0.020259386)) {
              sum += (float)-0.0058578104;
            } else {
              sum += (float)0.18419479;
            }
          }
        } else {
          if (  (data[6].fvalue < 11.180006)) {
            if (  (data[18].fvalue < 0.044592399)) {
              sum += (float)0.32316193;
            } else {
              sum += (float)0.66936398;
            }
          } else {
            if (  (data[18].fvalue < 0.18989873)) {
              sum += (float)0.12491023;
            } else {
              sum += (float)0.25492114;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.34517428)) {
          if (  (data[18].fvalue < 0.026703998)) {
            if (  (data[22].fvalue < 0.020259386)) {
              sum += (float)-0.17040002;
            } else {
              sum += (float)-0.017014429;
            }
          } else {
            if (  (data[11].fvalue < 0.46631098)) {
              sum += (float)0.12751314;
            } else {
              sum += (float)-0.075276099;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 23.55928)) {
              sum += (float)-0.01555417;
            } else {
              sum += (float)0.0041674129;
            }
          } else {
            if (  (data[11].fvalue < 0.16137291)) {
              sum += (float)0.13130352;
            } else {
              sum += (float)-0.069650978;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.34517428)) {
        if (  (data[7].fvalue < 0.96689928)) {
          if (  (data[11].fvalue < 0.2149374)) {
            if (  (data[11].fvalue < 0.15109177)) {
              sum += (float)0.58695954;
            } else {
              sum += (float)0.27340189;
            }
          } else {
            if (  (data[11].fvalue < 0.36814493)) {
              sum += (float)0.10116327;
            } else {
              sum += (float)0.0014118913;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.9583016)) {
            if (  (data[11].fvalue < 0.14090843)) {
              sum += (float)0.79655701;
            } else {
              sum += (float)0.12989606;
            }
          } else {
            if (  (data[7].fvalue < 0.9840275)) {
              sum += (float)0.61486042;
            } else {
              sum += (float)0.8158496;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 2.9175229)) {
            if (  (data[20].fvalue < 0.018007141)) {
              sum += (float)-0.0043467814;
            } else {
              sum += (float)0.014295835;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.0099176262;
            } else {
              sum += (float)-0.01874036;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.42857185)) {
            if (  (data[22].fvalue < 0.020259386)) {
              sum += (float)-0.11888646;
            } else {
              sum += (float)0.027633509;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.098352239;
            } else {
              sum += (float)-0.044915393;
            }
          }
        }
      }
    }
  } else {
    if (  (data[5].fvalue < 36.00824)) {
      if (  (data[22].fvalue < 0.020259386)) {
        if (  (data[21].fvalue < 0.023436096)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 3.3064349)) {
              sum += (float)0.20870458;
            } else {
              sum += (float)-0.19710261;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.70392627;
            } else {
              sum += (float)-0.062209267;
            }
          }
        } else {
          if (  (data[21].fvalue < 3.4530623)) {
            if (  (data[11].fvalue < 0.41438609)) {
              sum += (float)-0.13783985;
            } else {
              sum += (float)0.0014269134;
            }
          } else {
            if (  (data[20].fvalue < 0.74906862)) {
              sum += (float)-0.25638348;
            } else {
              sum += (float)-0.074283451;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.77257562)) {
          if (  (data[18].fvalue < 7.8558583)) {
            if (  (data[7].fvalue < 0.99098676)) {
              sum += (float)-0.079208322;
            } else {
              sum += (float)-0.14737512;
            }
          } else {
            if (  (data[11].fvalue < 0.50129086)) {
              sum += (float)-0.28662261;
            } else {
              sum += (float)-0.17511301;
            }
          }
        } else {
          if (  (data[5].fvalue < 23.55928)) {
            if (  (data[22].fvalue < 4.8099103)) {
              sum += (float)-0.10444863;
            } else {
              sum += (float)-0.22664817;
            }
          } else {
            if (  (data[20].fvalue < 5.3303456)) {
              sum += (float)0.013372951;
            } else {
              sum += (float)-0.070592269;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.38499647)) {
        if (  (data[21].fvalue < 0.023436096)) {
          if (  (data[22].fvalue < 0.020259386)) {
            if (  (data[20].fvalue < 3.3064349)) {
              sum += (float)0.20773202;
            } else {
              sum += (float)-0.33929223;
            }
          } else {
            if (  (data[18].fvalue < 7.8558583)) {
              sum += (float)-0.02791355;
            } else {
              sum += (float)-0.15811759;
            }
          }
        } else {
          if (  (data[18].fvalue < 7.8558583)) {
            if (  (data[11].fvalue < 0.23633142)) {
              sum += (float)-0.20190774;
            } else {
              sum += (float)-0.060331829;
            }
          } else {
            if (  (data[11].fvalue < 0.27366841)) {
              sum += (float)-0.41084743;
            } else {
              sum += (float)-0.21277237;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 7.0712624)) {
          if (  (data[21].fvalue < 8.0690756)) {
            if (  (data[22].fvalue < 8.4898567)) {
              sum += (float)0.077078693;
            } else {
              sum += (float)-0.0203762;
            }
          } else {
            if (  (data[11].fvalue < 0.64556313)) {
              sum += (float)-0.13837157;
            } else {
              sum += (float)0.033873301;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.75034648)) {
            if (  (data[18].fvalue < 7.8558583)) {
              sum += (float)-0.068062335;
            } else {
              sum += (float)-0.21786891;
            }
          } else {
            if (  (data[20].fvalue < 15.80138)) {
              sum += (float)0.049038328;
            } else {
              sum += (float)-0.036613464;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 15.004057)) {
    if (  (data[8].fvalue < 17.519335)) {
      if (  (data[10].fvalue < 0.073406637)) {
        if (  (data[22].fvalue < 0.70923936)) {
          if (  (data[8].fvalue < 0.075655736)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.027288316;
            } else {
              sum += (float)-0.28226355;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.027296254;
            } else {
              sum += (float)-0.012420981;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.032565117)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.040610772;
            } else {
              sum += (float)0.36126921;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.043744858;
            } else {
              sum += (float)0.012608898;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.032565117)) {
          if (  (data[10].fvalue < 0.26872593)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.050424658;
            } else {
              sum += (float)-0.41031635;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.57618374;
            } else {
              sum += (float)-0.42453146;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.3924711)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.003348968;
            } else {
              sum += (float)0.040269975;
            }
          } else {
            if (  (data[8].fvalue < 5.1521025)) {
              sum += (float)-0.023681654;
            } else {
              sum += (float)-0.00030212218;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.74241906)) {
        if (  (data[16].fvalue < 4.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.0092895497;
            } else {
              sum += (float)0.08012639;
            }
          } else {
            if (  (data[6].fvalue < 3.9491904)) {
              sum += (float)0.0322401;
            } else {
              sum += (float)-0.0012488568;
            }
          }
        } else {
          if (  (data[22].fvalue < 7.5623293)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.19982934;
            } else {
              sum += (float)0.39670637;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.015172599;
            } else {
              sum += (float)0.11329698;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.13970889)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.68729532;
            } else {
              sum += (float)0.17657188;
            }
          } else {
            if (  (data[7].fvalue < 0.98814279)) {
              sum += (float)0.14585371;
            } else {
              sum += (float)0.037343912;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.32373795)) {
              sum += (float)0.20798178;
            } else {
              sum += (float)0.022892825;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.085113466;
            } else {
              sum += (float)0.28913221;
            }
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 5.999999)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.99962848)) {
              sum += (float)0.13767415;
            } else {
              sum += (float)-0.096488982;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.032574732;
            } else {
              sum += (float)-0.08639469;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.1585478)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.063031837;
            } else {
              sum += (float)0.0070074252;
            }
          } else {
            if (  (data[7].fvalue < 0.92080808)) {
              sum += (float)-0.032888249;
            } else {
              sum += (float)-0.10928868;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[10].fvalue < 0.68714464)) {
            if (  (data[10].fvalue < 0.12995046)) {
              sum += (float)0.010129659;
            } else {
              sum += (float)-0.032696232;
            }
          } else {
            if (  (data[7].fvalue < 0.8938731)) {
              sum += (float)0.20475452;
            } else {
              sum += (float)0.068620965;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[6].fvalue < 19.662289)) {
              sum += (float)-0.067698464;
            } else {
              sum += (float)-0.143031;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0034402902;
            } else {
              sum += (float)-0.052860655;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 6.999999)) {
              sum += (float)-0.36855176;
            } else {
              sum += (float)-0.51239491;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.4001492;
            } else {
              sum += (float)-0.2561923;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[16].fvalue < 6.999999)) {
              sum += (float)-0.14487863;
            } else {
              sum += (float)-0.29510659;
            }
          } else {
            if (  (data[22].fvalue < 19.786095)) {
              sum += (float)0.12111543;
            } else {
              sum += (float)0.0081823347;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 19.786095)) {
          if (  (data[8].fvalue < 14.851807)) {
            if (  (data[10].fvalue < 0.12277719)) {
              sum += (float)0.058693904;
            } else {
              sum += (float)-0.066747203;
            }
          } else {
            if (  (data[16].fvalue < 6.999999)) {
              sum += (float)0.1182595;
            } else {
              sum += (float)0.24628253;
            }
          }
        } else {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[7].fvalue < 0.96695542)) {
              sum += (float)-0.10523672;
            } else {
              sum += (float)-0.24787337;
            }
          } else {
            if (  (data[8].fvalue < 11.864409)) {
              sum += (float)-0.11662716;
            } else {
              sum += (float)-0.015343885;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 5.8316402)) {
    if (  (data[20].fvalue < 0.1036934)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[20].fvalue < 0.017906677)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.0041633691;
            } else {
              sum += (float)-0.15648027;
            }
          } else {
            if (  (data[21].fvalue < 0.14323069)) {
              sum += (float)-0.019210441;
            } else {
              sum += (float)-0.078867719;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.021121349)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.20164481;
            } else {
              sum += (float)0.37722501;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.076186202;
            } else {
              sum += (float)-0.039449792;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.021121349)) {
          if ( (data[9].fvalue < 1.4099828)) {
            if (  (data[9].fvalue < 0.99499476)) {
              sum += (float)0.016116975;
            } else {
              sum += (float)-0.040022533;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.063855007;
            } else {
              sum += (float)0.010371821;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.095236003)) {
              sum += (float)0.24493715;
            } else {
              sum += (float)0.12309492;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.0077109342;
            } else {
              sum += (float)0.16311249;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if ( (data[9].fvalue < 2.0818031)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.035210401;
            } else {
              sum += (float)0.140736;
            }
          } else {
            if (  (data[21].fvalue < 0.021121349)) {
              sum += (float)0.60291648;
            } else {
              sum += (float)0.20113248;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.01496252;
            } else {
              sum += (float)-0.065988481;
            }
          } else {
            if (  (data[21].fvalue < 0.021121349)) {
              sum += (float)0.27947271;
            } else {
              sum += (float)0.020802509;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.453228)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.095236003)) {
              sum += (float)-0.052150559;
            } else {
              sum += (float)0.068056397;
            }
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.069594525;
            } else {
              sum += (float)0.36444572;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.030295175;
            } else {
              sum += (float)-0.03363876;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.011246013;
            } else {
              sum += (float)-0.03259407;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 13.176904)) {
      if (  (data[21].fvalue < 14.719499)) {
        if (  (data[20].fvalue < 0.55364925)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.056236271;
            } else {
              sum += (float)0.12196383;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.04236196;
            } else {
              sum += (float)0.23555434;
            }
          }
        } else {
          if (  (data[20].fvalue < 4.3091869)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.056321014;
            } else {
              sum += (float)0.12374435;
            }
          } else {
            if (  (data[9].fvalue < 7.1559548)) {
              sum += (float)0.014567588;
            } else {
              sum += (float)0.064078465;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 1.9380734)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.13813232;
            } else {
              sum += (float)0.022140175;
            }
          } else {
            if (  (data[10].fvalue < 0.12987596)) {
              sum += (float)0.12667817;
            } else {
              sum += (float)0.021735981;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.14971238;
            } else {
              sum += (float)-0.051552396;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.017448885;
            } else {
              sum += (float)-0.041076452;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 1.5064619)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[21].fvalue < 0.095236003)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.24284822;
            } else {
              sum += (float)0.1471547;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.023113074;
            } else {
              sum += (float)0.098370351;
            }
          }
        } else {
          if (  (data[20].fvalue < 20.203766)) {
            if (  (data[9].fvalue < 6.8901792)) {
              sum += (float)0.0035801399;
            } else {
              sum += (float)0.047610283;
            }
          } else {
            if (  (data[10].fvalue < 0.16638578)) {
              sum += (float)0.002654006;
            } else {
              sum += (float)-0.047649607;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 20.203766)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.032497056;
            } else {
              sum += (float)-0.035218302;
            }
          } else {
            if (  (data[9].fvalue < 7.1559548)) {
              sum += (float)-0.056954;
            } else {
              sum += (float)-0.019823197;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.13711689)) {
            if (  (data[8].fvalue < 0.54673171)) {
              sum += (float)0.093413226;
            } else {
              sum += (float)-0.041668843;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.071810357;
            } else {
              sum += (float)-0.11897736;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 2.486809)) {
    if (  (data[13].fvalue < 2.999999)) {
      if (  (data[8].fvalue < 0.45401162)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 0.11279466)) {
            if (  (data[20].fvalue < 0.017950263)) {
              sum += (float)0.0064003565;
            } else {
              sum += (float)0.089231864;
            }
          } else {
            if (  (data[21].fvalue < 0.021263849)) {
              sum += (float)0.024880864;
            } else {
              sum += (float)-0.03863094;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.014855788)) {
            if (  (data[21].fvalue < 0.021263849)) {
              sum += (float)-0.026516577;
            } else {
              sum += (float)-0.20593263;
            }
          } else {
            if (  (data[21].fvalue < 0.021263849)) {
              sum += (float)0.046251222;
            } else {
              sum += (float)-0.021581532;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.13744144)) {
          if (  (data[18].fvalue < 0.014855788)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.11707257;
            } else {
              sum += (float)0.0053362106;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.12752031;
            } else {
              sum += (float)0.0492444;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.3428531)) {
            if (  (data[21].fvalue < 0.45722944)) {
              sum += (float)0.0094219018;
            } else {
              sum += (float)0.040756322;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0095159076;
            } else {
              sum += (float)-0.021427246;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.014855788)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[21].fvalue < 0.021263849)) {
            if (  (data[8].fvalue < 0.17595346)) {
              sum += (float)-0.037607934;
            } else {
              sum += (float)0.075312629;
            }
          } else {
            if (  (data[20].fvalue < 0.71843052)) {
              sum += (float)-0.16602132;
            } else {
              sum += (float)-0.0015590441;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.91200858)) {
            if (  (data[8].fvalue < 0.14008017)) {
              sum += (float)-0.3382853;
            } else {
              sum += (float)-0.20417435;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.083426669;
            } else {
              sum += (float)-0.34644923;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.078000449)) {
          if (  (data[8].fvalue < 0.65108603)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.059354167;
            } else {
              sum += (float)-0.073530294;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.22121388;
            } else {
              sum += (float)0.042668261;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.2943203)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.013131262;
            } else {
              sum += (float)-0.02569099;
            }
          } else {
            if (  (data[21].fvalue < 2.532732)) {
              sum += (float)0.026946917;
            } else {
              sum += (float)-0.016850038;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 4.999999)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[8].fvalue < 7.7101097)) {
          if (  (data[5].fvalue < 24.318504)) {
            if (  (data[21].fvalue < 0.45722944)) {
              sum += (float)-0.038846973;
            } else {
              sum += (float)-0.17829321;
            }
          } else {
            if (  (data[21].fvalue < 0.021263849)) {
              sum += (float)0.059970919;
            } else {
              sum += (float)-0.051885717;
            }
          }
        } else {
          if (  (data[18].fvalue < 7.7039409)) {
            if (  (data[21].fvalue < 3.9935181)) {
              sum += (float)0.069831431;
            } else {
              sum += (float)-0.0028756314;
            }
          } else {
            if (  (data[21].fvalue < 0.095233187)) {
              sum += (float)0.070703387;
            } else {
              sum += (float)-0.099169299;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 1.6113371)) {
            if (  (data[8].fvalue < 3.1320167)) {
              sum += (float)0.10805996;
            } else {
              sum += (float)0.051467281;
            }
          } else {
            if (  (data[5].fvalue < 18.821386)) {
              sum += (float)-0.090570204;
            } else {
              sum += (float)0.0064292643;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.017950263)) {
            if (  (data[8].fvalue < 4.5232387)) {
              sum += (float)0.10531726;
            } else {
              sum += (float)-0.027316526;
            }
          } else {
            if (  (data[20].fvalue < 0.43851769)) {
              sum += (float)-0.09559983;
            } else {
              sum += (float)-0.018574398;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 23.53631)) {
        if (  (data[21].fvalue < 0.11688934)) {
          if (  (data[8].fvalue < 10.024326)) {
            if (  (data[8].fvalue < 2.4364061)) {
              sum += (float)-0.15556186;
            } else {
              sum += (float)-0.079385459;
            }
          } else {
            if (  (data[20].fvalue < 0.017950263)) {
              sum += (float)-0.099963605;
            } else {
              sum += (float)0.095257215;
            }
          }
        } else {
          if (  (data[8].fvalue < 10.705483)) {
            if (  (data[18].fvalue < 4.01302)) {
              sum += (float)-0.16309746;
            } else {
              sum += (float)-0.24193685;
            }
          } else {
            if (  (data[21].fvalue < 4.9079409)) {
              sum += (float)-0.053759392;
            } else {
              sum += (float)-0.19883351;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 2.532732)) {
          if (  (data[13].fvalue < 17.999998)) {
            if (  (data[6].fvalue < 14.499701)) {
              sum += (float)0.0032228464;
            } else {
              sum += (float)-0.052620582;
            }
          } else {
            if (  (data[21].fvalue < 0.021263849)) {
              sum += (float)0.010272007;
            } else {
              sum += (float)0.38731083;
            }
          }
        } else {
          if (  (data[13].fvalue < 16.999998)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.073477246;
            } else {
              sum += (float)-0.14903925;
            }
          } else {
            if (  (data[13].fvalue < 20.999998)) {
              sum += (float)0.17483737;
            } else {
              sum += (float)0.56532657;
            }
          }
        }
      }
    }
  }
  if (  (data[4].fvalue < 0.99999899)) {
    if (  (data[10].fvalue < 1.0993985)) {
      if (  (data[9].fvalue < 3.5740345)) {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[20].fvalue < 0.018694844)) {
            if (  (data[18].fvalue < 0.34358633)) {
              sum += (float)0.014313053;
            } else {
              sum += (float)-0.029031517;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0234189;
            } else {
              sum += (float)-0.047368828;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.25395104)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.15523462;
            } else {
              sum += (float)-0.015416326;
            }
          } else {
            if (  (data[18].fvalue < 1.4267887)) {
              sum += (float)-0.015966289;
            } else {
              sum += (float)-0.11521149;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.32351992)) {
          if (  (data[20].fvalue < 0.77969074)) {
            if (  (data[20].fvalue < 0.018694844)) {
              sum += (float)-0.018119507;
            } else {
              sum += (float)-0.065244377;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.00034256669;
            } else {
              sum += (float)-0.1386379;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.069134779;
            } else {
              sum += (float)0.012640719;
            }
          } else {
            if (  (data[20].fvalue < 7.0178776)) {
              sum += (float)-0.008771712;
            } else {
              sum += (float)-0.071467549;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.99999899)) {
        if (  (data[18].fvalue < 0.02668421)) {
          if (  (data[18].fvalue < 0.014898227)) {
            if (  (data[5].fvalue < 17.998394)) {
              sum += (float)-0.5781219;
            } else {
              sum += (float)-0.33860233;
            }
          } else {
            sum += (float)-0.64093101;
          }
        } else {
          if (  (data[18].fvalue < 0.36078808)) {
            if (  (data[11].fvalue < 0.20246513)) {
              sum += (float)0.010253101;
            } else {
              sum += (float)-0.31175822;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.19317079;
            } else {
              sum += (float)0.012362652;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.014898227)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[6].fvalue < 0.87657654)) {
              sum += (float)-0.11001219;
            } else {
              sum += (float)0.036975071;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.073679812;
            } else {
              sum += (float)0.16430287;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[11].fvalue < 0.0088580977)) {
              sum += (float)0.37314773;
            } else {
              sum += (float)0.15188517;
            }
          } else {
            if (  (data[18].fvalue < 0.23580651)) {
              sum += (float)-0.040589459;
            } else {
              sum += (float)0.17072028;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.0849034)) {
      if (  (data[18].fvalue < 0.014898227)) {
        if (  (data[10].fvalue < 0.2686269)) {
          if (  (data[9].fvalue < 0.96169698)) {
            if (  (data[10].fvalue < 0.0147936)) {
              sum += (float)0.50102949;
            } else {
              sum += (float)0.73802888;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.072731346;
            } else {
              sum += (float)-0.40306538;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.73773712)) {
              sum += (float)-0.16449855;
            } else {
              sum += (float)-0.091233931;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.04153087;
            } else {
              sum += (float)-0.14137599;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.73773712)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.22508596;
            } else {
              sum += (float)0.066518635;
            }
          } else {
            if (  (data[18].fvalue < 0.55780315)) {
              sum += (float)0.047461107;
            } else {
              sum += (float)-0.13888402;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.1927153)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.089348391;
            } else {
              sum += (float)0.030874275;
            }
          } else {
            if (  (data[5].fvalue < 1.0096473)) {
              sum += (float)0.1512723;
            } else {
              sum += (float)0.048026271;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 1.8508135)) {
        if (  (data[9].fvalue < 3.9832964)) {
          if (  (data[18].fvalue < 0.91254741)) {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)0.011646589;
            } else {
              sum += (float)-0.21901681;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.055541329;
            } else {
              sum += (float)0.031369787;
            }
          }
        } else {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[20].fvalue < 0.56174064)) {
              sum += (float)0.0014047851;
            } else {
              sum += (float)0.034419928;
            }
          } else {
            if (  (data[18].fvalue < 0.38180092)) {
              sum += (float)-0.010626103;
            } else {
              sum += (float)0.10398499;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.62210214)) {
          if (  (data[9].fvalue < 3.2012889)) {
            if (  (data[18].fvalue < 0.45026219)) {
              sum += (float)-0.011257065;
            } else {
              sum += (float)-0.12773146;
            }
          } else {
            if (  (data[20].fvalue < 6.5082035)) {
              sum += (float)-0.0021756669;
            } else {
              sum += (float)-0.027233141;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.16258147;
            } else {
              sum += (float)0.011944874;
            }
          } else {
            if (  (data[10].fvalue < 0.50556409)) {
              sum += (float)0.012263654;
            } else {
              sum += (float)0.056442872;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 6.270658)) {
    if (  (data[15].fvalue < 4.999999)) {
      if (  (data[14].fvalue < 4.999999)) {
        if (  (data[19].fvalue < 0.030404709)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.00031557112;
            } else {
              sum += (float)-0.049020119;
            }
          } else {
            if (  (data[8].fvalue < 0.76947695)) {
              sum += (float)-0.51973462;
            } else {
              sum += (float)-0.16122052;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.10396824)) {
            if (  (data[10].fvalue < 0.39534581)) {
              sum += (float)0.021372281;
            } else {
              sum += (float)0.24220991;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0058486457;
            } else {
              sum += (float)-0.0048999088;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.69528013;
            } else {
              sum += (float)0.41416666;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.29963461;
            } else {
              sum += (float)0.095009856;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 5.999999)) {
              sum += (float)0.11442033;
            } else {
              sum += (float)0.45378914;
            }
          } else {
            if (  (data[14].fvalue < 5.999999)) {
              sum += (float)-0.14763434;
            } else {
              sum += (float)0.16223465;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 2.999999)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 4.5264268)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)0.61685753;
            } else {
              sum += (float)0.46885115;
            }
          } else {
            if (  (data[10].fvalue < 0.16649806)) {
              sum += (float)0.2670823;
            } else {
              sum += (float)0.44231755;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.45538616;
            } else {
              sum += (float)0.1194347;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.43741554;
            } else {
              sum += (float)0.60897589;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 5.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.091330692;
            } else {
              sum += (float)-0.079537034;
            }
          } else {
            if (  (data[15].fvalue < 6.999999)) {
              sum += (float)0.10324466;
            } else {
              sum += (float)0.38189822;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.28261286;
            } else {
              sum += (float)0.45165268;
            }
          } else {
            if (  (data[15].fvalue < 5.999999)) {
              sum += (float)0.0084650759;
            } else {
              sum += (float)0.29870152;
            }
          }
        }
      }
    }
  } else {
    if (  (data[0].fvalue < 0.99999899)) {
      if (  (data[16].fvalue < 0.99999899)) {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.28015473;
            } else {
              sum += (float)-0.84076244;
            }
          } else {
            if (  (data[10].fvalue < 0.36995685)) {
              sum += (float)-0.052106392;
            } else {
              sum += (float)0.11465876;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.05644713;
            } else {
              sum += (float)-0.13257317;
            }
          } else {
            if (  (data[10].fvalue < 0.38324821)) {
              sum += (float)-0.19368552;
            } else {
              sum += (float)-0.068602085;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.39534581)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.15685439;
            } else {
              sum += (float)-0.30308583;
            }
          } else {
            if (  (data[10].fvalue < 0.55098933)) {
              sum += (float)-0.14383779;
            } else {
              sum += (float)-0.025338648;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.1336956;
            } else {
              sum += (float)0.040300265;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.22229557;
            } else {
              sum += (float)-0.099892154;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[10].fvalue < 0.2858049)) {
          if (  (data[10].fvalue < 0.10915969)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.1468972;
            } else {
              sum += (float)0.2338015;
            }
          } else {
            if (  (data[8].fvalue < 8.4282417)) {
              sum += (float)0.064105332;
            } else {
              sum += (float)0.15886106;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.22204939;
            } else {
              sum += (float)-0.019474788;
            }
          } else {
            if (  (data[8].fvalue < 13.201619)) {
              sum += (float)-0.1047974;
            } else {
              sum += (float)0.027064301;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.21836406)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.037113387;
            } else {
              sum += (float)0.11126417;
            }
          } else {
            if (  (data[8].fvalue < 9.9226265)) {
              sum += (float)-0.24009678;
            } else {
              sum += (float)-0.098318867;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.13692264)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.47045153;
            } else {
              sum += (float)0.13663179;
            }
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.020134071;
            } else {
              sum += (float)0.18802819;
            }
          }
        }
      }
    }
  }
  if (  (data[17].fvalue < 3.999999)) {
    if (  (data[18].fvalue < 7.846643)) {
      if (  (data[12].fvalue < 20.999998)) {
        if (  (data[12].fvalue < 15.999999)) {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 1.9153957)) {
              sum += (float)-0.0028551798;
            } else {
              sum += (float)0.0041841459;
            }
          } else {
            if (  (data[8].fvalue < 1.8009652)) {
              sum += (float)-0.10677846;
            } else {
              sum += (float)-0.043124486;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.0895957)) {
            if (  (data[18].fvalue < 0.91369915)) {
              sum += (float)-0.56905061;
            } else {
              sum += (float)-0.40802827;
            }
          } else {
            if (  (data[18].fvalue < 2.882338)) {
              sum += (float)-0.0088380417;
            } else {
              sum += (float)-0.25877047;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[15].fvalue < 1.999999)) {
            sum += (float)1.2604309;
          } else {
            sum += (float)0.74985337;
          }
        } else {
          if (  (data[12].fvalue < 21.999998)) {
            sum += (float)0.0075792894;
          } else {
            sum += (float)0.60971528;
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[10].fvalue < 0.60727268)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.13168818;
            } else {
              sum += (float)-0.21462256;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.03809616;
            } else {
              sum += (float)-0.10970823;
            }
          }
        } else {
          if (  (data[8].fvalue < 20.466526)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.0027455846;
            } else {
              sum += (float)0.20344359;
            }
          } else {
            if (  (data[7].fvalue < 0.32590127)) {
              sum += (float)0.15797034;
            } else {
              sum += (float)-0.06978716;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.1075763;
            } else {
              sum += (float)0.026245512;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.0068885842;
            } else {
              sum += (float)0.26637465;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.29632416;
            } else {
              sum += (float)-0.17514856;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.068441771;
            } else {
              sum += (float)0.028743364;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 11.875119)) {
      sum += (float)-0.48739642;
    } else {
      sum += (float)-0.43993679;
    }
  }
  if (  (data[19].fvalue < 3.7356534)) {
    if ( (data[9].fvalue < 1.4676628)) {
      if (  (data[9].fvalue < 1.2616894)) {
        if (  (data[19].fvalue < 0.017278634)) {
          if (  (data[9].fvalue < 1.086344)) {
            if (  (data[10].fvalue < 0.50583827)) {
              sum += (float)-0.12328295;
            } else {
              sum += (float)-0.041114233;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.0038918604;
            } else {
              sum += (float)0.086066984;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.39464822)) {
            if (  (data[9].fvalue < 0.99414134)) {
              sum += (float)0.29105502;
            } else {
              sum += (float)0.03333861;
            }
          } else {
            if (  (data[19].fvalue < 0.15179184)) {
              sum += (float)-0.016684964;
            } else {
              sum += (float)0.020962033;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.086703561)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 0.083884284)) {
              sum += (float)0.026114853;
            } else {
              sum += (float)-0.035489984;
            }
          } else {
            if (  (data[19].fvalue < 0.017278634)) {
              sum += (float)0.3431012;
            } else {
              sum += (float)0.095071845;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.18087393)) {
            if (  (data[19].fvalue < 0.45944092)) {
              sum += (float)-0.1332169;
            } else {
              sum += (float)0.10101099;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.037116162;
            } else {
              sum += (float)0.088765599;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 1.3180552)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.095533714)) {
              sum += (float)0.009901803;
            } else {
              sum += (float)-0.005229455;
            }
          } else {
            if (  (data[19].fvalue < 0.1802893)) {
              sum += (float)-0.16381437;
            } else {
              sum += (float)-0.018020853;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.20192693)) {
            if (  (data[20].fvalue < 0.018601727)) {
              sum += (float)0.3407295;
            } else {
              sum += (float)0.15876472;
            }
          } else {
            if (  (data[19].fvalue < 0.017278634)) {
              sum += (float)0.084736466;
            } else {
              sum += (float)0.0048107877;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.43718648)) {
              sum += (float)0.064374134;
            } else {
              sum += (float)-0.016283356;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.025533479;
            } else {
              sum += (float)-0.11040402;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.6143544)) {
            if (  (data[10].fvalue < 0.46986622)) {
              sum += (float)-0.16026698;
            } else {
              sum += (float)-0.0091006821;
            }
          } else {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)-0.01335765;
            } else {
              sum += (float)0.037580311;
            }
          }
        }
      }
    }
  } else {
    if (  (data[9].fvalue < 4.2155871)) {
      if (  (data[9].fvalue < 3.0329633)) {
        if (  (data[20].fvalue < 1.2302547)) {
          if (  (data[6].fvalue < 13.345156)) {
            if (  (data[9].fvalue < 1.8100936)) {
              sum += (float)-0.06019127;
            } else {
              sum += (float)0.088238619;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.032536514;
            } else {
              sum += (float)0.04934236;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.32030436;
            } else {
              sum += (float)-0.073280863;
            }
          } else {
            if (  (data[9].fvalue < 2.6143544)) {
              sum += (float)0.025499333;
            } else {
              sum += (float)-0.11772147;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.2955181)) {
          if (  (data[9].fvalue < 3.6819918)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.1250262;
            } else {
              sum += (float)-0.26264784;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.010756132;
            } else {
              sum += (float)-0.13189432;
            }
          }
        } else {
          if (  (data[9].fvalue < 3.7866349)) {
            if (  (data[20].fvalue < 0.083884284)) {
              sum += (float)-0.040642995;
            } else {
              sum += (float)-0.11157417;
            }
          } else {
            if (  (data[10].fvalue < 0.57702017)) {
              sum += (float)-0.055763718;
            } else {
              sum += (float)0.01274375;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 5.3532991)) {
        if (  (data[9].fvalue < 5.4438586)) {
          if (  (data[20].fvalue < 4.778091)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.086106308;
            } else {
              sum += (float)-0.0015875268;
            }
          } else {
            if (  (data[10].fvalue < 0.35782033)) {
              sum += (float)-0.12739322;
            } else {
              sum += (float)-0.010131523;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.018601727)) {
            if (  (data[19].fvalue < 6.3749156)) {
              sum += (float)0.037701853;
            } else {
              sum += (float)-0.10277918;
            }
          } else {
            if (  (data[20].fvalue < 7.0468006)) {
              sum += (float)0.093785122;
            } else {
              sum += (float)0.0075747208;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 6.8878326)) {
          if (  (data[10].fvalue < 0.21034288)) {
            if (  (data[9].fvalue < 5.2696877)) {
              sum += (float)-0.24083409;
            } else {
              sum += (float)-0.12665634;
            }
          } else {
            if (  (data[21].fvalue < 8.0607061)) {
              sum += (float)-0.029047035;
            } else {
              sum += (float)-0.083242483;
            }
          }
        } else {
          if (  (data[21].fvalue < 15.43568)) {
            if (  (data[20].fvalue < 10.511596)) {
              sum += (float)0.059736747;
            } else {
              sum += (float)-0.057404108;
            }
          } else {
            if (  (data[20].fvalue < 3.9601238)) {
              sum += (float)-0.06291873;
            } else {
              sum += (float)-0.15416926;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 42.239788)) {
    if (  (data[18].fvalue < 1.4302742)) {
      if (  (data[19].fvalue < 6.2069926)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.52729201)) {
            if (  (data[5].fvalue < 5.7572398)) {
              sum += (float)-0.044850022;
            } else {
              sum += (float)-0.0063260412;
            }
          } else {
            if (  (data[19].fvalue < 0.017097794)) {
              sum += (float)-0.019924261;
            } else {
              sum += (float)0.029382765;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.83858079)) {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)0.0075301048;
            } else {
              sum += (float)-0.29937494;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.025732208;
            } else {
              sum += (float)0.023673242;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.34787038)) {
              sum += (float)-0.23953827;
            } else {
              sum += (float)-0.035367716;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.26003742;
            } else {
              sum += (float)-0.1576335;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.23774433)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.29848164;
            } else {
              sum += (float)-0.071563445;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.21107791;
            } else {
              sum += (float)-0.095394902;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[19].fvalue < 0.11302843)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0037959393;
            } else {
              sum += (float)-0.059005614;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.33894369;
            } else {
              sum += (float)0.018803019;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.18818103;
            } else {
              sum += (float)0.015175552;
            }
          } else {
            if (  (data[19].fvalue < 0.50829518)) {
              sum += (float)0.058045633;
            } else {
              sum += (float)-0.26734748;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[10].fvalue < 0.52729201)) {
              sum += (float)-0.2421198;
            } else {
              sum += (float)-0.080631733;
            }
          } else {
            if (  (data[19].fvalue < 0.65070254)) {
              sum += (float)-0.48882619;
            } else {
              sum += (float)-0.35137209;
            }
          }
        } else {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.032981571;
            } else {
              sum += (float)-0.21076873;
            }
          } else {
            if (  (data[13].fvalue < 16.999998)) {
              sum += (float)-2.1749491e-05;
            } else {
              sum += (float)0.24384472;
            }
          }
        }
      }
    }
  } else {
    if (  (data[10].fvalue < 0.2680794)) {
      if (  (data[11].fvalue < 0.02247913)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 0.089351937)) {
            if (  (data[19].fvalue < 0.017097794)) {
              sum += (float)-0.013447319;
            } else {
              sum += (float)0.11384776;
            }
          } else {
            if (  (data[19].fvalue < 0.25320017)) {
              sum += (float)-0.070019782;
            } else {
              sum += (float)-0.41052011;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.55847013)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.078120552;
            } else {
              sum += (float)-0.19400641;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.48328137;
            } else {
              sum += (float)-0.12457396;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.015079663)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.47417632;
            } else {
              sum += (float)1.4573537;
            }
          } else {
            if (  (data[19].fvalue < 0.03157815)) {
              sum += (float)0.30198601;
            } else {
              sum += (float)0.018143276;
            }
          }
        } else {
          if (  (data[19].fvalue < 6.2069926)) {
            if (  (data[10].fvalue < 0.15142094)) {
              sum += (float)0.029392462;
            } else {
              sum += (float)0.018199705;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.1686625;
            } else {
              sum += (float)-0.075560227;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.17988554)) {
        if (  (data[18].fvalue < 0.015079663)) {
          if (  (data[11].fvalue < 0.12277348)) {
            if (  (data[11].fvalue < 0.0090915095)) {
              sum += (float)-0.092915244;
            } else {
              sum += (float)0.11668191;
            }
          } else {
            if (  (data[10].fvalue < 0.74282366)) {
              sum += (float)-0.5775432;
            } else {
              sum += (float)0.26503599;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.011756953;
            } else {
              sum += (float)0.054414898;
            }
          } else {
            if (  (data[18].fvalue < 0.25399533)) {
              sum += (float)-0.28111327;
            } else {
              sum += (float)-0.017663972;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 10.674948)) {
          if (  (data[10].fvalue < 0.64275092)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.051461145;
            } else {
              sum += (float)0.014392632;
            }
          } else {
            if (  (data[10].fvalue < 0.80874306)) {
              sum += (float)0.11859643;
            } else {
              sum += (float)0.26916265;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.15081783)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.024602851;
            } else {
              sum += (float)0.016499642;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.016546378;
            } else {
              sum += (float)-0.014012935;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 0.99999899)) {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[13].fvalue < 0.99999899)) {
        if (  (data[5].fvalue < 13.414466)) {
          if (  (data[7].fvalue < 0.99934697)) {
            if (  (data[8].fvalue < 0.70816416)) {
              sum += (float)0.21095385;
            } else {
              sum += (float)0.045262415;
            }
          } else {
            if (  (data[11].fvalue < 0.079174891)) {
              sum += (float)0.1203668;
            } else {
              sum += (float)-0.073443621;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.16041745)) {
            if (  (data[8].fvalue < 3.1223574)) {
              sum += (float)-0.0556271;
            } else {
              sum += (float)0.29747266;
            }
          } else {
            if (  (data[5].fvalue < 32.163727)) {
              sum += (float)-0.17525698;
            } else {
              sum += (float)-0.37724474;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99995255)) {
          if (  (data[11].fvalue < 0.016283406)) {
            if (  (data[5].fvalue < 4.2086234)) {
              sum += (float)0.45597637;
            } else {
              sum += (float)-0.039895013;
            }
          } else {
            if (  (data[7].fvalue < 0.9994176)) {
              sum += (float)0.021653038;
            } else {
              sum += (float)0.056135576;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.029407991)) {
            if (  (data[8].fvalue < 0.03384991)) {
              sum += (float)0.13045862;
            } else {
              sum += (float)0.078986958;
            }
          } else {
            if (  (data[8].fvalue < 0.03384991)) {
              sum += (float)0.19458708;
            } else {
              sum += (float)0.15297838;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.54007959)) {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 3.7415931)) {
              sum += (float)-0.084388629;
            } else {
              sum += (float)-0.038055841;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.0137138;
            } else {
              sum += (float)-0.12308932;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[11].fvalue < 0.041852012)) {
              sum += (float)-0.15465073;
            } else {
              sum += (float)-0.26753852;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.35894784;
            } else {
              sum += (float)-0.44795403;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 10.381166)) {
          if (  (data[8].fvalue < 3.5421526)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.16878679;
            } else {
              sum += (float)-0.091012068;
            }
          } else {
            if (  (data[8].fvalue < 5.8964844)) {
              sum += (float)-0.026952768;
            } else {
              sum += (float)0.066186197;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.088718392;
            } else {
              sum += (float)0.0095451195;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.026546188;
            } else {
              sum += (float)-0.16507682;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.014592455)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.24803865)) {
            if (  (data[5].fvalue < 8.8500271)) {
              sum += (float)-0.32919019;
            } else {
              sum += (float)0.0102667;
            }
          } else {
            if (  (data[5].fvalue < 42.270008)) {
              sum += (float)0.93481278;
            } else {
              sum += (float)0.14166568;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.14372109)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.2145917;
            } else {
              sum += (float)-0.68583864;
            }
          } else {
            if (  (data[11].fvalue < 0.041852012)) {
              sum += (float)-0.48705208;
            } else {
              sum += (float)-0.42048144;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[11].fvalue < 0.008966459)) {
            if (  (data[5].fvalue < 32.867775)) {
              sum += (float)0.48860702;
            } else {
              sum += (float)0.23812918;
            }
          } else {
            if (  (data[5].fvalue < 11.154983)) {
              sum += (float)0.030281506;
            } else {
              sum += (float)-0.14212057;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.45552346)) {
              sum += (float)0.14607853;
            } else {
              sum += (float)0.28853706;
            }
          } else {
            if (  (data[8].fvalue < 0.97429514)) {
              sum += (float)-0.56921798;
            } else {
              sum += (float)-0.37324384;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.008966459)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.037419308)) {
              sum += (float)-0.20823041;
            } else {
              sum += (float)0.042673782;
            }
          } else {
            if (  (data[5].fvalue < 27.368496)) {
              sum += (float)0.25803065;
            } else {
              sum += (float)0.095505416;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.089395903)) {
            if (  (data[18].fvalue < 0.068505093)) {
              sum += (float)-0.63830173;
            } else {
              sum += (float)-0.29272357;
            }
          } else {
            if (  (data[18].fvalue < 0.15012102)) {
              sum += (float)-0.069239475;
            } else {
              sum += (float)0.24713519;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 5.8964844)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0012893662;
            } else {
              sum += (float)-0.040610991;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.022951055;
            } else {
              sum += (float)0.011594691;
            }
          }
        } else {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.014122275;
            } else {
              sum += (float)-0.095828727;
            }
          } else {
            if (  (data[18].fvalue < 0.50314903)) {
              sum += (float)-0.22360207;
            } else {
              sum += (float)0.0050084298;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 6.085547)) {
    if ( (data[9].fvalue < 3.6788762)) {
      if (  (data[22].fvalue < 0.61161268)) {
        if (  (data[21].fvalue < 1.072688)) {
          if ( (data[9].fvalue < 1.746057)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.00091068912;
            } else {
              sum += (float)-0.43273938;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.045888092;
            } else {
              sum += (float)0.005746522;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.084015228;
            } else {
              sum += (float)-0.0065002609;
            }
          } else {
            if (  (data[22].fvalue < 0.020651182)) {
              sum += (float)0.0361908;
            } else {
              sum += (float)-0.047557779;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.7709672)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 1.2263412)) {
              sum += (float)-0.049727596;
            } else {
              sum += (float)0.033977028;
            }
          } else {
            if (  (data[9].fvalue < 2.3758278)) {
              sum += (float)0.60089827;
            } else {
              sum += (float)0.13137446;
            }
          }
        } else {
          if (  (data[22].fvalue < 2.8605723)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)0.0026286517;
            } else {
              sum += (float)0.063851349;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.011522071;
            } else {
              sum += (float)-0.037635274;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 5.999999)) {
        if (  (data[12].fvalue < 9.999999)) {
          if (  (data[22].fvalue < 0.77669913)) {
            if (  (data[21].fvalue < 0.41998225)) {
              sum += (float)-0.038641483;
            } else {
              sum += (float)0.0086866356;
            }
          } else {
            if (  (data[21].fvalue < 5.6132669)) {
              sum += (float)0.031259708;
            } else {
              sum += (float)-0.013146905;
            }
          }
        } else {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.388044;
            } else {
              sum += (float)-0.12781471;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.46999705;
            } else {
              sum += (float)-0.30408242;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[22].fvalue < 0.020651182)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.21979183;
            } else {
              sum += (float)-0.084971704;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.025613716;
            } else {
              sum += (float)-0.10820424;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.9508106)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.05311447;
            } else {
              sum += (float)0.13748474;
            }
          } else {
            if (  (data[21].fvalue < 0.14101054)) {
              sum += (float)0.092362568;
            } else {
              sum += (float)0.0014201601;
            }
          }
        }
      }
    }
  } else {
    if (  (data[21].fvalue < 0.094546348)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 3.2903533)) {
              sum += (float)-0.045891907;
            } else {
              sum += (float)0.11075194;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.033496607;
            } else {
              sum += (float)-0.05240481;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.16441044;
            } else {
              sum += (float)-0.35144553;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.39735261;
            } else {
              sum += (float)-0.23376098;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.02047753;
            } else {
              sum += (float)-0.16704445;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.050010476;
            } else {
              sum += (float)-0.015065935;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.1286203;
            } else {
              sum += (float)-0.30695605;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.049093574;
            } else {
              sum += (float)0.2183893;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 4.8515711)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.28676146)) {
              sum += (float)0.027925033;
            } else {
              sum += (float)-0.033434294;
            }
          } else {
            if (  (data[21].fvalue < 0.22916569)) {
              sum += (float)-0.2520133;
            } else {
              sum += (float)-0.10095874;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.091755949;
            } else {
              sum += (float)-0.20534316;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.014426859;
            } else {
              sum += (float)-0.099610858;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.022656612;
            } else {
              sum += (float)0.030303961;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.12781705;
            } else {
              sum += (float)-0.039030086;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.016723048;
            } else {
              sum += (float)0.22581762;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.030757092;
            } else {
              sum += (float)-0.07096266;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.65754449)) {
    if (  (data[13].fvalue < 10.999999)) {
      if (  (data[16].fvalue < 3.999999)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[21].fvalue < 2.2514999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.0067286454;
            } else {
              sum += (float)-0.013699824;
            }
          } else {
            if (  (data[19].fvalue < 0.16642419)) {
              sum += (float)0.10429355;
            } else {
              sum += (float)0.0069522285;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.42367619)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.043765225;
            } else {
              sum += (float)0.21424034;
            }
          } else {
            if ( (data[9].fvalue < 2.4508433)) {
              sum += (float)-0.082850724;
            } else {
              sum += (float)-0.0098162126;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.55836147)) {
          if (  (data[18].fvalue < 0.1771685)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.29047874;
            } else {
              sum += (float)-0.11894793;
            }
          } else {
            if (  (data[19].fvalue < 0.13909128)) {
              sum += (float)-0.13162288;
            } else {
              sum += (float)0.0170648;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.049228355;
            } else {
              sum += (float)0.12175724;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.02164457;
            } else {
              sum += (float)0.075764306;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.72556949)) {
        if (  (data[13].fvalue < 11.999999)) {
          if (  (data[19].fvalue < 0.48392212)) {
            if (  (data[19].fvalue < 0.33574703)) {
              sum += (float)-0.4931567;
            } else {
              sum += (float)-0.36332345;
            }
          } else {
            if (  (data[9].fvalue < 1.3500546)) {
              sum += (float)0.3385936;
            } else {
              sum += (float)-0.21348862;
            }
          }
        } else {
          if (  (data[13].fvalue < 13.999999)) {
            if (  (data[19].fvalue < 0.61883831)) {
              sum += (float)-0.45433983;
            } else {
              sum += (float)-0.30055586;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.49727067;
            } else {
              sum += (float)-0.55193615;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 13.999999)) {
          if ( (data[9].fvalue < 1.4100935)) {
            if (  (data[18].fvalue < 0.32481721)) {
              sum += (float)0.45520294;
            } else {
              sum += (float)0.19834407;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.11249218;
            } else {
              sum += (float)-0.00082725327;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.2586026)) {
            if (  (data[13].fvalue < 15.999999)) {
              sum += (float)-0.34239483;
            } else {
              sum += (float)-0.5433147;
            }
          } else {
            if (  (data[18].fvalue < 0.45165041)) {
              sum += (float)-0.26710579;
            } else {
              sum += (float)-0.093127355;
            }
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 2.999999)) {
      if (  (data[19].fvalue < 0.58835608)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[21].fvalue < 0.63704902)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.0091058342;
            } else {
              sum += (float)0.10080369;
            }
          } else {
            if ( (data[9].fvalue < 2.4508433)) {
              sum += (float)-0.05114546;
            } else {
              sum += (float)-0.008734799;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.0891932)) {
            if ( (data[9].fvalue < 1.2163178)) {
              sum += (float)0.29628226;
            } else {
              sum += (float)0.0013669564;
            }
          } else {
            if (  (data[19].fvalue < 0.39403871)) {
              sum += (float)-0.20294541;
            } else {
              sum += (float)-0.061388299;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.6949568)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.67901748)) {
              sum += (float)-0.54582798;
            } else {
              sum += (float)-0.091092937;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.21282566;
            } else {
              sum += (float)-0.055120967;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 4.0906405)) {
              sum += (float)-0.023379453;
            } else {
              sum += (float)0.17595032;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.21611267;
            } else {
              sum += (float)0.020228354;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.2376285)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.46080399)) {
              sum += (float)0.043728396;
            } else {
              sum += (float)-0.059183281;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.050271537;
            } else {
              sum += (float)0.19318594;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.10032048)) {
            if (  (data[21].fvalue < 0.07731086)) {
              sum += (float)0.0063780006;
            } else {
              sum += (float)-0.11892904;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.046371073;
            } else {
              sum += (float)-0.054502513;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.68800193)) {
            if ( (data[9].fvalue < 3.5752063)) {
              sum += (float)0.3622607;
            } else {
              sum += (float)0.22532287;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.27166101;
            } else {
              sum += (float)0.43440065;
            }
          }
        } else {
          if (  (data[16].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 2.379643)) {
              sum += (float)0.053991009;
            } else {
              sum += (float)-0.016984802;
            }
          } else {
            if (  (data[19].fvalue < 0.55836147)) {
              sum += (float)0.037705477;
            } else {
              sum += (float)0.11191349;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 13.903909)) {
    if (  (data[12].fvalue < 7.999999)) {
      if (  (data[17].fvalue < 3.999999)) {
        if (  (data[15].fvalue < 3.999999)) {
          if (  (data[21].fvalue < 2.5321341)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.0019053925;
            } else {
              sum += (float)0.050113257;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.0063227657;
            } else {
              sum += (float)-0.010542146;
            }
          }
        } else {
          if (  (data[21].fvalue < 2.8678091)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.28103352;
            } else {
              sum += (float)0.13251665;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.013596816;
            } else {
              sum += (float)0.11818834;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 4.999999)) {
          if (  (data[18].fvalue < 1.2414492)) {
            sum += (float)-0.46317652;
          } else {
            sum += (float)-0.39989778;
          }
        } else {
          sum += (float)-0.49937862;
        }
      }
    } else {
      if (  (data[18].fvalue < 0.62268364)) {
        if (  (data[18].fvalue < 0.38385612)) {
          if (  (data[18].fvalue < 0.25368825)) {
            if (  (data[18].fvalue < 0.19311862)) {
              sum += (float)-0.50241613;
            } else {
              sum += (float)-0.42434913;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.23232087;
            } else {
              sum += (float)-0.44319454;
            }
          }
        } else {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[22].fvalue < 1.2871432)) {
              sum += (float)-0.084255397;
            } else {
              sum += (float)-0.18192333;
            }
          } else {
            if (  (data[12].fvalue < 12.999999)) {
              sum += (float)-0.34337455;
            } else {
              sum += (float)-0.4905386;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 1.5301081)) {
            if (  (data[12].fvalue < 14.999999)) {
              sum += (float)-0.028084904;
            } else {
              sum += (float)-0.18876036;
            }
          } else {
            if (  (data[22].fvalue < 0.020977562)) {
              sum += (float)-0.22092727;
            } else {
              sum += (float)-0.10737938;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.11628957)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.11953282;
            } else {
              sum += (float)0.47448504;
            }
          } else {
            if (  (data[22].fvalue < 2.6877143)) {
              sum += (float)0.03001632;
            } else {
              sum += (float)-0.059318084;
            }
          }
        }
      }
    }
  } else {
    if (  (data[22].fvalue < 19.871052)) {
      if (  (data[18].fvalue < 0.62268364)) {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[7].fvalue < 0.9821977)) {
              sum += (float)0.011743186;
            } else {
              sum += (float)-0.012971808;
            }
          } else {
            if (  (data[18].fvalue < 0.42894232)) {
              sum += (float)-0.14940605;
            } else {
              sum += (float)0.011037923;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.45129329)) {
            if (  (data[21].fvalue < 6.8864417)) {
              sum += (float)-0.33458877;
            } else {
              sum += (float)-0.17001842;
            }
          } else {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)-0.098939814;
            } else {
              sum += (float)-0.23718186;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.017400458;
            } else {
              sum += (float)-0.071658596;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.0520401;
            } else {
              sum += (float)-0.14385411;
            }
          }
        } else {
          if (  (data[21].fvalue < 3.076633)) {
            if (  (data[7].fvalue < 0.86772889)) {
              sum += (float)0.045909286;
            } else {
              sum += (float)-0.014136219;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.071160838;
            } else {
              sum += (float)-0.011199385;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.30457154)) {
        if (  (data[21].fvalue < 0.68194127)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.048143275;
            } else {
              sum += (float)-0.091651857;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.085389376;
            } else {
              sum += (float)0.10688201;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.016330704)) {
              sum += (float)-0.028925007;
            } else {
              sum += (float)0.022243496;
            }
          } else {
            if (  (data[18].fvalue < 0.12712897)) {
              sum += (float)-0.3553842;
            } else {
              sum += (float)-0.13622269;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 1.8534503)) {
              sum += (float)-0.021500984;
            } else {
              sum += (float)-0.089746952;
            }
          } else {
            if (  (data[18].fvalue < 0.65659094)) {
              sum += (float)-0.092469633;
            } else {
              sum += (float)-0.20782843;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.69829309)) {
              sum += (float)0.021281134;
            } else {
              sum += (float)-0.080601744;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.049634885;
            } else {
              sum += (float)0.036345441;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 9.2040634)) {
    if (  (data[8].fvalue < 7.7157936)) {
      if ( (data[9].fvalue < 4.5784502)) {
        if (  (data[22].fvalue < 0.89481485)) {
          if (  (data[8].fvalue < 1.3765148)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)3.0428444e-05;
            } else {
              sum += (float)-0.042361904;
            }
          } else {
            if ( (data[9].fvalue < 1.2610841)) {
              sum += (float)-0.039343707;
            } else {
              sum += (float)0.0092028528;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.9439096)) {
            if (  (data[8].fvalue < 0.40957788)) {
              sum += (float)0.015689008;
            } else {
              sum += (float)-0.034473952;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.0307818;
            } else {
              sum += (float)-0.016077749;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.35566819)) {
          if (  (data[7].fvalue < 0.97158802)) {
            if (  (data[10].fvalue < 0.073961176)) {
              sum += (float)-0.015718535;
            } else {
              sum += (float)0.13064708;
            }
          } else {
            if (  (data[10].fvalue < 0.03378313)) {
              sum += (float)0.34972516;
            } else {
              sum += (float)0.71658754;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.96282148)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.072594106;
            } else {
              sum += (float)-0.0013079429;
            }
          } else {
            if (  (data[6].fvalue < 2.524497)) {
              sum += (float)0.062795453;
            } else {
              sum += (float)0.010073114;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 1.999999)) {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[22].fvalue < 0.023712417)) {
              sum += (float)0.04527178;
            } else {
              sum += (float)-0.013105995;
            }
          } else {
            if (  (data[22].fvalue < 0.023712417)) {
              sum += (float)-0.13112019;
            } else {
              sum += (float)-0.030336443;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if ( (data[9].fvalue < 4.4560065)) {
              sum += (float)0.43970606;
            } else {
              sum += (float)0.29634017;
            }
          } else {
            if ( (data[9].fvalue < 4.4560065)) {
              sum += (float)0.14048196;
            } else {
              sum += (float)0.021952787;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 1.8166775)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.012364267;
            } else {
              sum += (float)0.1050657;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.11509294;
            } else {
              sum += (float)0.045204204;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.063423507;
            } else {
              sum += (float)0.25981274;
            }
          } else {
            if ( (data[9].fvalue < 5.6443543)) {
              sum += (float)-0.0076495707;
            } else {
              sum += (float)0.02629886;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 6.5122142)) {
      if (  (data[8].fvalue < 27.466154)) {
        if (  (data[6].fvalue < 19.666065)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 1.3765148)) {
              sum += (float)0.0986555;
            } else {
              sum += (float)-0.0068940846;
            }
          } else {
            if (  (data[22].fvalue < 15.03815)) {
              sum += (float)-0.021911183;
            } else {
              sum += (float)-0.050743077;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.21111003)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.010659441;
            } else {
              sum += (float)-0.068367012;
            }
          } else {
            if (  (data[6].fvalue < 27.830486)) {
              sum += (float)-0.054286726;
            } else {
              sum += (float)-0.092315882;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.69598633)) {
          if (  (data[10].fvalue < 0.80833131)) {
            if (  (data[6].fvalue < 0.35566819)) {
              sum += (float)-0.1052089;
            } else {
              sum += (float)0.091668062;
            }
          } else {
            if (  (data[5].fvalue < 28.960693)) {
              sum += (float)0.12597458;
            } else {
              sum += (float)0.32580534;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.57799429)) {
            if (  (data[22].fvalue < 19.814625)) {
              sum += (float)0.0059431186;
            } else {
              sum += (float)-0.054646879;
            }
          } else {
            if (  (data[10].fvalue < 0.80833131)) {
              sum += (float)0.030600389;
            } else {
              sum += (float)0.097656026;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 19.814625)) {
        if (  (data[14].fvalue < 2.999999)) {
          if (  (data[5].fvalue < 38.35918)) {
            if (  (data[8].fvalue < 0.40957788)) {
              sum += (float)0.15507105;
            } else {
              sum += (float)0.0015065598;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.058218136;
            } else {
              sum += (float)0.029350517;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.013792993;
            } else {
              sum += (float)0.031938944;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.011220637;
            } else {
              sum += (float)-0.072597705;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.50401437)) {
          if (  (data[9].fvalue < 7.5971475)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.083826825;
            } else {
              sum += (float)-0.081848323;
            }
          } else {
            if (  (data[6].fvalue < 5.5598783)) {
              sum += (float)0.227547;
            } else {
              sum += (float)0.12295984;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.13057782)) {
              sum += (float)0.013615357;
            } else {
              sum += (float)-0.030531419;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.089197196;
            } else {
              sum += (float)-0.004065495;
            }
          }
        }
      }
    }
  }
  if (  (data[20].fvalue < 3.0654898)) {
    if (  (data[22].fvalue < 0.021626418)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[19].fvalue < 0.39619026)) {
          if (  (data[19].fvalue < 0.015735649)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.0053407126;
            } else {
              sum += (float)-0.16525291;
            }
          } else {
            if (  (data[19].fvalue < 0.063271903)) {
              sum += (float)0.057754494;
            } else {
              sum += (float)0.007352781;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.60973769)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.026990626;
            } else {
              sum += (float)-0.044679534;
            }
          } else {
            if (  (data[9].fvalue < 2.5310853)) {
              sum += (float)-0.18328695;
            } else {
              sum += (float)-0.026972493;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.051803138)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[9].fvalue < 1.3014238)) {
              sum += (float)-0.2841315;
            } else {
              sum += (float)0.14187744;
            }
          } else {
            if ( (data[9].fvalue < 1.0888616)) {
              sum += (float)-0.69112837;
            } else {
              sum += (float)-0.5035125;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.2166708)) {
            if (  (data[19].fvalue < 0.13861611)) {
              sum += (float)-0.03744727;
            } else {
              sum += (float)0.10799969;
            }
          } else {
            if ( (data[9].fvalue < 1.7461469)) {
              sum += (float)-0.033325735;
            } else {
              sum += (float)0.028829115;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.084961317)) {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 3.0214362)) {
              sum += (float)0.096425362;
            } else {
              sum += (float)-0.0013583151;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.10292741;
            } else {
              sum += (float)0.031651855;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.1522043)) {
            if (  (data[19].fvalue < 0.11261668)) {
              sum += (float)-0.47056285;
            } else {
              sum += (float)-0.21872482;
            }
          } else {
            if ( (data[9].fvalue < 2.0097826)) {
              sum += (float)0.2335854;
            } else {
              sum += (float)0.13315453;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 0.017891254)) {
            if (  (data[19].fvalue < 0.41513094)) {
              sum += (float)0.0080042351;
            } else {
              sum += (float)-0.034315113;
            }
          } else {
            if (  (data[9].fvalue < 2.5310853)) {
              sum += (float)0.086730078;
            } else {
              sum += (float)0.0062302533;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.40099147)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.078131028;
            } else {
              sum += (float)-0.0022439784;
            }
          } else {
            if (  (data[20].fvalue < 0.77653867)) {
              sum += (float)-0.0077542635;
            } else {
              sum += (float)0.031228006;
            }
          }
        }
      }
    }
  } else {
    if (  (data[22].fvalue < 0.021626418)) {
      if (  (data[19].fvalue < 2.1204107)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if ( (data[9].fvalue < 2.5310853)) {
            if (  (data[11].fvalue < 0.27325225)) {
              sum += (float)-0.36783761;
            } else {
              sum += (float)-0.22140926;
            }
          } else {
            if (  (data[9].fvalue < 3.0214362)) {
              sum += (float)-0.07324779;
            } else {
              sum += (float)-0.0059800185;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.51003182)) {
              sum += (float)0.076572217;
            } else {
              sum += (float)0.012752153;
            }
          } else {
            if (  (data[19].fvalue < 0.39619026)) {
              sum += (float)-0.097018875;
            } else {
              sum += (float)0.0035951119;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 3.1070452)) {
          if (  (data[20].fvalue < 9.2536659)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.3561247;
            } else {
              sum += (float)-0.14282359;
            }
          } else {
            if (  (data[20].fvalue < 13.068463)) {
              sum += (float)-0.030143032;
            } else {
              sum += (float)0.20559594;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.033186048;
            } else {
              sum += (float)0.067884356;
            }
          } else {
            if (  (data[9].fvalue < 4.212585)) {
              sum += (float)-0.12766019;
            } else {
              sum += (float)-0.063063033;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.64467454)) {
        if (  (data[20].fvalue < 7.8173699)) {
          if ( (data[9].fvalue < 4.0858526)) {
            if (  (data[19].fvalue < 1.4344596)) {
              sum += (float)-0.024224266;
            } else {
              sum += (float)-0.088844813;
            }
          } else {
            if (  (data[22].fvalue < 7.5501509)) {
              sum += (float)-0.0072411573;
            } else {
              sum += (float)-0.037848532;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.65250093)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.032654941;
            } else {
              sum += (float)-0.20429395;
            }
          } else {
            if (  (data[22].fvalue < 1.0357893)) {
              sum += (float)-0.03442625;
            } else {
              sum += (float)-0.074077554;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 20.20223)) {
          if ( (data[9].fvalue < 6.5279894)) {
            if (  (data[19].fvalue < 0.11261668)) {
              sum += (float)-0.028160963;
            } else {
              sum += (float)-0.0025388431;
            }
          } else {
            if (  (data[22].fvalue < 11.120678)) {
              sum += (float)0.024788273;
            } else {
              sum += (float)-0.006879067;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.72583151)) {
            if (  (data[22].fvalue < 0.70970565)) {
              sum += (float)-0.091963165;
            } else {
              sum += (float)-0.025509488;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.10221382;
            } else {
              sum += (float)-0.036958031;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 9.3020859)) {
    if (  (data[18].fvalue < 3.376313)) {
      if (  (data[5].fvalue < 35.218945)) {
        if (  (data[21].fvalue < 1.5150776)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.021965574;
            } else {
              sum += (float)-0.028714737;
            }
          } else {
            if (  (data[18].fvalue < 0.016056046)) {
              sum += (float)-0.13266163;
            } else {
              sum += (float)0.0012119586;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 14.944869)) {
              sum += (float)-0.054330215;
            } else {
              sum += (float)-0.0086500114;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0075163818;
            } else {
              sum += (float)0.028316231;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.015639758;
            } else {
              sum += (float)-0.052010164;
            }
          } else {
            if (  (data[11].fvalue < 0.029074563)) {
              sum += (float)-0.035091292;
            } else {
              sum += (float)0.029206788;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.022602048)) {
            if (  (data[10].fvalue < 0.058770534)) {
              sum += (float)-0.077657349;
            } else {
              sum += (float)-0.01980656;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.0010556499;
            } else {
              sum += (float)0.02414351;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 6.999999)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 7.7997127)) {
              sum += (float)-0.013465907;
            } else {
              sum += (float)-0.068858512;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.23185946;
            } else {
              sum += (float)-0.10186886;
            }
          }
        } else {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.056052491;
            } else {
              sum += (float)-0.0018459064;
            }
          } else {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)-0.16158123;
            } else {
              sum += (float)-0.44096676;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[21].fvalue < 0.11631767)) {
              sum += (float)-0.089427955;
            } else {
              sum += (float)-0.21113062;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.35654366;
            } else {
              sum += (float)-0.50211853;
            }
          }
        } else {
          if (  (data[5].fvalue < 40.710365)) {
            if (  (data[21].fvalue < 0.11631767)) {
              sum += (float)-0.043862805;
            } else {
              sum += (float)-0.11519213;
            }
          } else {
            if (  (data[11].fvalue < 0.31305307)) {
              sum += (float)-0.10754684;
            } else {
              sum += (float)0.0043119807;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 3.999999)) {
      if (  (data[12].fvalue < 6.999999)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[18].fvalue < 0.36122099)) {
            if (  (data[18].fvalue < 0.016056046)) {
              sum += (float)-0.025980277;
            } else {
              sum += (float)0.024536347;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.022826524;
            } else {
              sum += (float)-0.12907369;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.065207571;
            } else {
              sum += (float)-0.049568042;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.028746352;
            } else {
              sum += (float)0.082033716;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.027132658;
            } else {
              sum += (float)-0.29834813;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.22586747;
            } else {
              sum += (float)-0.35916302;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.11594234)) {
              sum += (float)-0.096086554;
            } else {
              sum += (float)-0.22671685;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)0.051792845;
            } else {
              sum += (float)-0.29290915;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[18].fvalue < 0.50510138)) {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[11].fvalue < 0.60321993)) {
              sum += (float)-0.089090005;
            } else {
              sum += (float)-0.025473204;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.32145542;
            } else {
              sum += (float)-0.15050913;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 1.3206033)) {
              sum += (float)-0.39343616;
            } else {
              sum += (float)-0.25340235;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.11868469;
            } else {
              sum += (float)-0.20539193;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 1.1743326)) {
          if (  (data[18].fvalue < 0.30427778)) {
            if (  (data[18].fvalue < 0.15220587)) {
              sum += (float)-0.26554927;
            } else {
              sum += (float)-0.099716701;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)0.030338489;
            } else {
              sum += (float)-0.14170389;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.024786092)) {
            if (  (data[11].fvalue < 0.80321646)) {
              sum += (float)0.024657963;
            } else {
              sum += (float)0.15717462;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.038105823;
            } else {
              sum += (float)-0.099616639;
            }
          }
        }
      }
    }
  }
  if (  (data[20].fvalue < 15.828658)) {
    if (  (data[8].fvalue < 13.398405)) {
      if (  (data[20].fvalue < 1.5694689)) {
        if (  (data[8].fvalue < 1.3790143)) {
          if (  (data[18].fvalue < 0.69581419)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)0.00070587237;
            } else {
              sum += (float)-0.050376888;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.042847987;
            } else {
              sum += (float)0.0081780339;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.8202219)) {
            if (  (data[21].fvalue < 1.9828908)) {
              sum += (float)0.023534605;
            } else {
              sum += (float)-0.011792875;
            }
          } else {
            if (  (data[19].fvalue < 2.1248312)) {
              sum += (float)0.0010353405;
            } else {
              sum += (float)-0.050562046;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[18].fvalue < 0.5306533)) {
            if (  (data[19].fvalue < 0.43902311)) {
              sum += (float)0.044351976;
            } else {
              sum += (float)-0.012369525;
            }
          } else {
            if (  (data[19].fvalue < 0.99996734)) {
              sum += (float)-0.032393873;
            } else {
              sum += (float)0.061903413;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.406315)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.21112216;
            } else {
              sum += (float)-0.081336945;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.066510454;
            } else {
              sum += (float)0.0019103262;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.35683826)) {
        if (  (data[18].fvalue < 0.34312621)) {
          if (  (data[21].fvalue < 5.5989184)) {
            if (  (data[22].fvalue < 11.104362)) {
              sum += (float)-0.32566234;
            } else {
              sum += (float)-0.077222005;
            }
          } else {
            if (  (data[22].fvalue < 1.7067815)) {
              sum += (float)-0.16398333;
            } else {
              sum += (float)-0.0022915746;
            }
          }
        } else {
          if (  (data[18].fvalue < 7.9332852)) {
            if (  (data[18].fvalue < 0.82117617)) {
              sum += (float)-0.013617608;
            } else {
              sum += (float)0.030203369;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.11292074;
            } else {
              sum += (float)0.021872343;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.20740542)) {
              sum += (float)0.0071920073;
            } else {
              sum += (float)0.084053613;
            }
          } else {
            if (  (data[18].fvalue < 1.6438779)) {
              sum += (float)-0.3013998;
            } else {
              sum += (float)-0.029263033;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.017692547)) {
            if (  (data[22].fvalue < 0.021080961)) {
              sum += (float)-0.12220221;
            } else {
              sum += (float)0.015075793;
            }
          } else {
            if (  (data[21].fvalue < 6.1681428)) {
              sum += (float)0.08672455;
            } else {
              sum += (float)0.029760402;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.13931836)) {
      if (  (data[19].fvalue < 0.22418198)) {
        if (  (data[8].fvalue < 27.568579)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.30525255;
            } else {
              sum += (float)0.092324428;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.22627294;
            } else {
              sum += (float)-0.19233631;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.087174557)) {
            if (  (data[21].fvalue < 3.7399368)) {
              sum += (float)-0.77445519;
            } else {
              sum += (float)-0.51261914;
            }
          } else {
            if (  (data[21].fvalue < 4.0080957)) {
              sum += (float)-0.28537977;
            } else {
              sum += (float)0.087445483;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 2.1248312)) {
              sum += (float)-0.04581647;
            } else {
              sum += (float)0.038365643;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.26616338;
            } else {
              sum += (float)-0.045751218;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.010273101;
            } else {
              sum += (float)-0.28004077;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.16527094;
            } else {
              sum += (float)0.001910136;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.40658274)) {
        if (  (data[21].fvalue < 0.11440663)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 6.2558265)) {
              sum += (float)0.081217833;
            } else {
              sum += (float)-0.1799203;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.290916;
            } else {
              sum += (float)-0.0017215661;
            }
          }
        } else {
          if (  (data[8].fvalue < 27.568579)) {
            if (  (data[8].fvalue < 3.3309422)) {
              sum += (float)-0.036254115;
            } else {
              sum += (float)-0.098219216;
            }
          } else {
            if (  (data[20].fvalue < 20.280462)) {
              sum += (float)0.035690147;
            } else {
              sum += (float)-0.063967802;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[13].fvalue < 9.999999)) {
            if (  (data[21].fvalue < 0.095811293)) {
              sum += (float)0.032554284;
            } else {
              sum += (float)-0.028878624;
            }
          } else {
            if (  (data[18].fvalue < 1.0344983)) {
              sum += (float)-0.12179574;
            } else {
              sum += (float)-0.42345929;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.6326669)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.15097283;
            } else {
              sum += (float)-0.010304809;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.25665241;
            } else {
              sum += (float)-0.0018860176;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 15.513805)) {
    if (  (data[15].fvalue < 3.999999)) {
      if (  (data[21].fvalue < 0.20382531)) {
        if (  (data[21].fvalue < 0.14242126)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.0067488886;
            } else {
              sum += (float)-0.0094391042;
            }
          } else {
            if (  (data[21].fvalue < 0.078223348)) {
              sum += (float)-0.070686176;
            } else {
              sum += (float)0.16219524;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.016225053)) {
              sum += (float)0.036403488;
            } else {
              sum += (float)-0.078629315;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.039486986;
            } else {
              sum += (float)0.31691131;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 7.0600615)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.012649316;
            } else {
              sum += (float)-0.016468586;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.0094788773;
            } else {
              sum += (float)0.05147168;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.75044429)) {
            if (  (data[21].fvalue < 1.2240374)) {
              sum += (float)0.0026021483;
            } else {
              sum += (float)-0.03209725;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.024115896;
            } else {
              sum += (float)-0.0039126039;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.017191138)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.80285448)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.42405236;
            } else {
              sum += (float)0.56281424;
            }
          } else {
            if (  (data[22].fvalue < 3.0321062)) {
              sum += (float)0.055156548;
            } else {
              sum += (float)0.35841149;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.26708508)) {
            sum += (float)0.63553697;
          } else {
            sum += (float)0.56032604;
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 3.999999)) {
            if (  (data[20].fvalue < 3.9556923)) {
              sum += (float)0.010783056;
            } else {
              sum += (float)-0.06528157;
            }
          } else {
            if (  (data[22].fvalue < 4.8108063)) {
              sum += (float)0.20225202;
            } else {
              sum += (float)0.061684329;
            }
          }
        } else {
          if (  (data[20].fvalue < 6.5097232)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.16100875;
            } else {
              sum += (float)0.0021897564;
            }
          } else {
            if (  (data[22].fvalue < 0.89908862)) {
              sum += (float)0.14360727;
            } else {
              sum += (float)0.013701987;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 0.10414448)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.13930328)) {
              sum += (float)0.22870852;
            } else {
              sum += (float)0.14988443;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.31073883;
            } else {
              sum += (float)0.075336508;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.58300906)) {
              sum += (float)0.075667098;
            } else {
              sum += (float)-0.071329311;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.10610846;
            } else {
              sum += (float)0.27090082;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.58300906)) {
              sum += (float)-0.17591681;
            } else {
              sum += (float)-0.079311222;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.025911501;
            } else {
              sum += (float)0.15109167;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.85959172)) {
              sum += (float)0.15635745;
            } else {
              sum += (float)-0.026171597;
            }
          } else {
            if (  (data[22].fvalue < 3.4416077)) {
              sum += (float)0.3862659;
            } else {
              sum += (float)0.32487583;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.99999899)) {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[20].fvalue < 8.3793783)) {
            if (  (data[20].fvalue < 0.91801339)) {
              sum += (float)0.048892424;
            } else {
              sum += (float)-0.018869136;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.15570848;
            } else {
              sum += (float)0.070430346;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[6].fvalue < 2.5438454)) {
              sum += (float)-0.18133347;
            } else {
              sum += (float)-0.076385975;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.023586346;
            } else {
              sum += (float)0.13176681;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[22].fvalue < 0.61240923)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.2201141;
            } else {
              sum += (float)-0.077815801;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.0095877051;
            } else {
              sum += (float)-0.097075038;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)-0.043369882;
            } else {
              sum += (float)-0.12469166;
            }
          } else {
            if (  (data[20].fvalue < 4.8397965)) {
              sum += (float)0.015359669;
            } else {
              sum += (float)-0.030606478;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 7.999999)) {
    if (  (data[19].fvalue < 6.2377567)) {
      if (  (data[11].fvalue < 0.078961387)) {
        if (  (data[17].fvalue < 0.99999899)) {
          if (  (data[22].fvalue < 0.66665268)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.004975074;
            } else {
              sum += (float)0.059326649;
            }
          } else {
            if (  (data[19].fvalue < 0.23896793)) {
              sum += (float)0.0014884676;
            } else {
              sum += (float)-0.092231125;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.049255028)) {
              sum += (float)-0.18275432;
            } else {
              sum += (float)0.20002469;
            }
          } else {
            if (  (data[11].fvalue < 0.072060838)) {
              sum += (float)0.57975274;
            } else {
              sum += (float)0.43729612;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 5.999999)) {
          if (  (data[17].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.58232993)) {
              sum += (float)0.0014533416;
            } else {
              sum += (float)0.0063549667;
            }
          } else {
            if (  (data[17].fvalue < 3.999999)) {
              sum += (float)-0.035043228;
            } else {
              sum += (float)-0.45176765;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.1655097;
            } else {
              sum += (float)0.024177328;
            }
          } else {
            if (  (data[22].fvalue < 7.5408473)) {
              sum += (float)0.28523102;
            } else {
              sum += (float)0.077479459;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[11].fvalue < 0.83702797)) {
          if (  (data[22].fvalue < 3.9623058)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.02922667;
            } else {
              sum += (float)-0.049241051;
            }
          } else {
            if (  (data[11].fvalue < 0.41487592)) {
              sum += (float)-0.22193219;
            } else {
              sum += (float)-0.10982741;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.33655128)) {
            if (  (data[22].fvalue < 0.4772774)) {
              sum += (float)-0.31083885;
            } else {
              sum += (float)-0.18180321;
            }
          } else {
            if (  (data[11].fvalue < 0.92419112)) {
              sum += (float)0.0054820557;
            } else {
              sum += (float)-0.095690824;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.71821374)) {
          if (  (data[11].fvalue < 0.38427651)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.071890026;
            } else {
              sum += (float)-0.15773185;
            }
          } else {
            if (  (data[21].fvalue < 0.061090957)) {
              sum += (float)0.012965851;
            } else {
              sum += (float)-0.048973925;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.2776393)) {
            if (  (data[22].fvalue < 0.71176857)) {
              sum += (float)0.016447049;
            } else {
              sum += (float)0.10476403;
            }
          } else {
            if (  (data[22].fvalue < 8.4900093)) {
              sum += (float)0.010288719;
            } else {
              sum += (float)-0.055598766;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 1.7720279)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[19].fvalue < 0.017178141)) {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.37347746;
            } else {
              sum += (float)-0.26691657;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.44648874;
            } else {
              sum += (float)-0.50846624;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.10982164)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.022038527;
            } else {
              sum += (float)0.096663125;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.078848161;
            } else {
              sum += (float)-0.035981163;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 3.6845372)) {
          if (  (data[22].fvalue < 0.43696001)) {
            if (  (data[11].fvalue < 0.17167951)) {
              sum += (float)0.11832573;
            } else {
              sum += (float)0.27507544;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.041232459;
            } else {
              sum += (float)0.11487576;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.43869275)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.016784141;
            } else {
              sum += (float)0.229762;
            }
          } else {
            if (  (data[16].fvalue < 5.999999)) {
              sum += (float)-0.057056926;
            } else {
              sum += (float)0.14901496;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[22].fvalue < 0.10982164)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.15405886;
            } else {
              sum += (float)-0.051173072;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.062684052;
            } else {
              sum += (float)0.063193694;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.71176857)) {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.16614251;
            } else {
              sum += (float)-0.24496327;
            }
          } else {
            if (  (data[22].fvalue < 1.218294)) {
              sum += (float)-0.054727301;
            } else {
              sum += (float)-0.17852002;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 3.6845372)) {
          if (  (data[22].fvalue < 0.36513337)) {
            if (  (data[11].fvalue < 0.21561857)) {
              sum += (float)-0.072947912;
            } else {
              sum += (float)0.19968353;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.047943249;
            } else {
              sum += (float)0.019297156;
            }
          }
        } else {
          if (  (data[16].fvalue < 5.999999)) {
            if (  (data[19].fvalue < 3.6799817)) {
              sum += (float)-0.12435319;
            } else {
              sum += (float)-0.23114809;
            }
          } else {
            if (  (data[22].fvalue < 14.979065)) {
              sum += (float)0.18690969;
            } else {
              sum += (float)-0.081557781;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 5.1377459)) {
    if (  (data[19].fvalue < 0.83826917)) {
      if (  (data[13].fvalue < 8.999999)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[21].fvalue < 0.023372751)) {
              sum += (float)0.0063903588;
            } else {
              sum += (float)-0.0071472563;
            }
          } else {
            if (  (data[19].fvalue < 0.48257133)) {
              sum += (float)-0.23111147;
            } else {
              sum += (float)-0.06652195;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.17497817;
            } else {
              sum += (float)0.0084148422;
            }
          } else {
            if (  (data[20].fvalue < 0.08329165)) {
              sum += (float)0.071968824;
            } else {
              sum += (float)-0.0010094666;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.5098052)) {
          if (  (data[19].fvalue < 0.28703627)) {
            if (  (data[19].fvalue < 0.27105573)) {
              sum += (float)-0.4881835;
            } else {
              sum += (float)-0.34489226;
            }
          } else {
            if (  (data[9].fvalue < 1.3047446)) {
              sum += (float)-0.01784257;
            } else {
              sum += (float)-0.23902065;
            }
          }
        } else {
          if (  (data[13].fvalue < 13.999999)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.15779422;
            } else {
              sum += (float)-0.0051484387;
            }
          } else {
            if (  (data[13].fvalue < 14.999999)) {
              sum += (float)-0.35108039;
            } else {
              sum += (float)-0.4881953;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 4.092731)) {
        if (  (data[20].fvalue < 1.5031043)) {
          if (  (data[21].fvalue < 1.6121477)) {
            if (  (data[22].fvalue < 1.2890991)) {
              sum += (float)0.01848151;
            } else {
              sum += (float)-0.070437826;
            }
          } else {
            if (  (data[9].fvalue < 2.8632185)) {
              sum += (float)-0.1953323;
            } else {
              sum += (float)-0.061635911;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.944309)) {
            if (  (data[21].fvalue < 0.023372751)) {
              sum += (float)-0.23400123;
            } else {
              sum += (float)-0.092220783;
            }
          } else {
            if (  (data[19].fvalue < 1.6291293)) {
              sum += (float)-0.0048477734;
            } else {
              sum += (float)-0.11441991;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 3.0783074)) {
          if (  (data[22].fvalue < 4.5493379)) {
            if (  (data[20].fvalue < 4.3225322)) {
              sum += (float)0.082764044;
            } else {
              sum += (float)0.015406045;
            }
          } else {
            if (  (data[9].fvalue < 5.4452338)) {
              sum += (float)-0.044823527;
            } else {
              sum += (float)0.03464992;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.13101955)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.15745679;
            } else {
              sum += (float)0.024778977;
            }
          } else {
            if (  (data[9].fvalue < 6.8769403)) {
              sum += (float)-0.047606252;
            } else {
              sum += (float)0.021231592;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 0.22443895)) {
      if (  (data[12].fvalue < 0.99999899)) {
        if ( (data[9].fvalue < 2.3085754)) {
          if (  (data[21].fvalue < 0.023372751)) {
            if (  (data[22].fvalue < 0.020588284)) {
              sum += (float)0.49699137;
            } else {
              sum += (float)-0.63074154;
            }
          } else {
            if (  (data[19].fvalue < 0.031336632)) {
              sum += (float)-0.72767514;
            } else {
              sum += (float)-1.197753;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.10120604)) {
            if (  (data[9].fvalue < 5.4452338)) {
              sum += (float)-0.59330988;
            } else {
              sum += (float)-0.22837758;
            }
          } else {
            if (  (data[9].fvalue < 4.4510207)) {
              sum += (float)-0.35421991;
            } else {
              sum += (float)-0.01915857;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 17.219706)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.015341973;
            } else {
              sum += (float)-0.04040575;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.042544909;
            } else {
              sum += (float)0.091012754;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.0013529017;
            } else {
              sum += (float)-0.10726624;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.080651939;
            } else {
              sum += (float)0.052282814;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 8.0716629)) {
        if (  (data[22].fvalue < 7.1013651)) {
          if (  (data[20].fvalue < 0.019161489)) {
            if ( (data[9].fvalue < 4.8543034)) {
              sum += (float)0.01657214;
            } else {
              sum += (float)-0.052722529;
            }
          } else {
            if (  (data[20].fvalue < 7.0100498)) {
              sum += (float)0.041023061;
            } else {
              sum += (float)0.009194972;
            }
          }
        } else {
          if (  (data[22].fvalue < 19.915264)) {
            if ( (data[9].fvalue < 5.4452338)) {
              sum += (float)-0.019947503;
            } else {
              sum += (float)0.019623911;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.03469358;
            } else {
              sum += (float)-0.071847841;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.1043862)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.20372412;
            } else {
              sum += (float)0.014630893;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.018066479;
            } else {
              sum += (float)0.049921367;
            }
          }
        } else {
          if ( (data[9].fvalue < 6.0684485)) {
            if (  (data[8].fvalue < 17.219706)) {
              sum += (float)-0.055531953;
            } else {
              sum += (float)-0.01302897;
            }
          } else {
            if (  (data[21].fvalue < 15.461214)) {
              sum += (float)0.012418272;
            } else {
              sum += (float)-0.035859723;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 1.9497787)) {
    if (  (data[8].fvalue < 2.0220401)) {
      if (  (data[18].fvalue < 0.86708838)) {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[22].fvalue < 2.0625226)) {
            if (  (data[22].fvalue < 0.11120069)) {
              sum += (float)0.0012478144;
            } else {
              sum += (float)-0.010133929;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.091233209;
            } else {
              sum += (float)-0.028282322;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.23774065)) {
              sum += (float)-0.090968274;
            } else {
              sum += (float)-0.0082751978;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.0028610057;
            } else {
              sum += (float)0.075757064;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)-0.074367113;
            } else {
              sum += (float)0.33395854;
            }
          } else {
            if (  (data[18].fvalue < 1.4271498)) {
              sum += (float)-0.0073159207;
            } else {
              sum += (float)0.02985888;
            }
          }
        } else {
          if (  (data[22].fvalue < 1.3938942)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.084603317;
            } else {
              sum += (float)-0.018315708;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.11400565;
            } else {
              sum += (float)-0.0087498398;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 8.999999)) {
        if (  (data[14].fvalue < 5.999999)) {
          if (  (data[16].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.0057811718;
            } else {
              sum += (float)-0.0028063848;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.30952746;
            } else {
              sum += (float)-0.00054270221;
            }
          }
        } else {
          if (  (data[14].fvalue < 6.999999)) {
            if (  (data[8].fvalue < 6.3203316)) {
              sum += (float)0.28078344;
            } else {
              sum += (float)0.14663452;
            }
          } else {
            if (  (data[18].fvalue < 0.47807136)) {
              sum += (float)0.46275467;
            } else {
              sum += (float)0.35228631;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.47807136)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.01650624)) {
              sum += (float)-0.37985572;
            } else {
              sum += (float)-0.052189052;
            }
          } else {
            if (  (data[22].fvalue < 3.2426474)) {
              sum += (float)0.11107461;
            } else {
              sum += (float)-0.033824015;
            }
          }
        } else {
          if (  (data[22].fvalue < 4.1810069)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.039906595;
            } else {
              sum += (float)0.13005398;
            }
          } else {
            if (  (data[8].fvalue < 11.822043)) {
              sum += (float)-0.012206537;
            } else {
              sum += (float)0.087814294;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 5.999999)) {
      if (  (data[16].fvalue < 2.999999)) {
        if (  (data[22].fvalue < 1.3938942)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.047810249)) {
              sum += (float)-0.020739131;
            } else {
              sum += (float)0.01078743;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.080590464;
            } else {
              sum += (float)-0.023951594;
            }
          }
        } else {
          if (  (data[8].fvalue < 9.130024)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.073217385;
            } else {
              sum += (float)-0.040159404;
            }
          } else {
            if (  (data[8].fvalue < 27.32021)) {
              sum += (float)-0.01882581;
            } else {
              sum += (float)0.034833334;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 7.5550961)) {
          if (  (data[22].fvalue < 0.89947903)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.10749521;
            } else {
              sum += (float)0.067564949;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.046114169;
            } else {
              sum += (float)0.12495463;
            }
          }
        } else {
          if (  (data[8].fvalue < 27.32021)) {
            if (  (data[22].fvalue < 15.023174)) {
              sum += (float)-0.010258544;
            } else {
              sum += (float)-0.048207469;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.0088083614;
            } else {
              sum += (float)0.074446879;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 4.1810069)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[22].fvalue < 1.2882382)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.076628447;
            } else {
              sum += (float)-0.027425842;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.11321297;
            } else {
              sum += (float)-0.16042054;
            }
          }
        } else {
          if (  (data[8].fvalue < 9.8328791)) {
            if (  (data[22].fvalue < 0.33627152)) {
              sum += (float)0.078084491;
            } else {
              sum += (float)-0.046572078;
            }
          } else {
            if (  (data[22].fvalue < 0.6667769)) {
              sum += (float)0.21108612;
            } else {
              sum += (float)0.067171693;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 6.999999)) {
          if (  (data[13].fvalue < 18.999998)) {
            if (  (data[8].fvalue < 11.822043)) {
              sum += (float)-0.12815517;
            } else {
              sum += (float)-0.078447543;
            }
          } else {
            if (  (data[18].fvalue < 2.5076749)) {
              sum += (float)0.39584914;
            } else {
              sum += (float)0.062156696;
            }
          }
        } else {
          if (  (data[22].fvalue < 13.831827)) {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)0.20690507;
            } else {
              sum += (float)0.35673299;
            }
          } else {
            if (  (data[8].fvalue < 13.290144)) {
              sum += (float)-0.039916608;
            } else {
              sum += (float)0.024939364;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 3.771579)) {
    if (  (data[21].fvalue < 0.202913)) {
      if (  (data[13].fvalue < 5.999999)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.010886392;
            } else {
              sum += (float)0.032349348;
            }
          } else {
            if (  (data[21].fvalue < 0.021771576)) {
              sum += (float)0.029216753;
            } else {
              sum += (float)-0.076154739;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.79244304)) {
            if (  (data[18].fvalue < 0.12593929)) {
              sum += (float)-0.35938939;
            } else {
              sum += (float)-0.021111449;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.37263092;
            } else {
              sum += (float)-0.1371814;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.2859762)) {
          if (  (data[19].fvalue < 0.16718927)) {
            if (  (data[19].fvalue < 0.12661198)) {
              sum += (float)-0.49961334;
            } else {
              sum += (float)-0.35420907;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.072118618;
            } else {
              sum += (float)-0.29351768;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.021771576)) {
              sum += (float)-0.062438421;
            } else {
              sum += (float)0.098004095;
            }
          } else {
            if (  (data[18].fvalue < 1.171139)) {
              sum += (float)0.091450699;
            } else {
              sum += (float)0.0041094115;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 4.8118267)) {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.0082901437;
            } else {
              sum += (float)-0.11376657;
            }
          } else {
            if (  (data[19].fvalue < 0.3949382)) {
              sum += (float)-0.084643677;
            } else {
              sum += (float)0.0022891432;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.26975071)) {
            if (  (data[18].fvalue < 0.23690514)) {
              sum += (float)-0.20120494;
            } else {
              sum += (float)0.010801498;
            }
          } else {
            if (  (data[18].fvalue < 0.23690514)) {
              sum += (float)0.027371567;
            } else {
              sum += (float)0.084484845;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.50503325)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.020790126;
            } else {
              sum += (float)-0.014684602;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.23400073;
            } else {
              sum += (float)-0.060556371;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0142149;
            } else {
              sum += (float)-0.11455725;
            }
          } else {
            if (  (data[19].fvalue < 0.19586399)) {
              sum += (float)0.061655786;
            } else {
              sum += (float)-0.01808626;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[18].fvalue < 0.2528621)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 6.3140106)) {
              sum += (float)-0.0057866406;
            } else {
              sum += (float)-0.11249698;
            }
          } else {
            if (  (data[18].fvalue < 0.15166987)) {
              sum += (float)-0.3967768;
            } else {
              sum += (float)-0.024759157;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.15170832;
            } else {
              sum += (float)-0.052057933;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.10154483;
            } else {
              sum += (float)0.006030139;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 4.0189586)) {
              sum += (float)0.27752963;
            } else {
              sum += (float)-0.034225617;
            }
          } else {
            if (  (data[18].fvalue < 0.47827694)) {
              sum += (float)0.13074388;
            } else {
              sum += (float)-0.36598614;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.36268273)) {
              sum += (float)-0.060537182;
            } else {
              sum += (float)-0.23440899;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.073015757;
            } else {
              sum += (float)0.053106714;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[18].fvalue < 1.7707553)) {
          if (  (data[18].fvalue < 0.5285663)) {
            if (  (data[19].fvalue < 6.3140106)) {
              sum += (float)-0.36582184;
            } else {
              sum += (float)-0.46546102;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.37559974;
            } else {
              sum += (float)-0.19054557;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.13728094;
            } else {
              sum += (float)-0.45493692;
            }
          } else {
            if (  (data[21].fvalue < 0.28880328)) {
              sum += (float)0.020208972;
            } else {
              sum += (float)0.19415748;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.42829826)) {
          if (  (data[13].fvalue < 5.999999)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.16705853;
            } else {
              sum += (float)-0.37311998;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.018293189;
            } else {
              sum += (float)-0.14773427;
            }
          }
        } else {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[18].fvalue < 1.9485327)) {
              sum += (float)0.0032783216;
            } else {
              sum += (float)0.18498544;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.26797792;
            } else {
              sum += (float)-0.0095285391;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 4.781291)) {
    if (  (data[13].fvalue < 23.999998)) {
      if (  (data[12].fvalue < 11.999999)) {
        if (  (data[13].fvalue < 13.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0046803611;
            } else {
              sum += (float)-0.029197924;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.014089754;
            } else {
              sum += (float)0.014460335;
            }
          }
        } else {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[18].fvalue < 1.1749182)) {
              sum += (float)-0.18484505;
            } else {
              sum += (float)-0.49198622;
            }
          } else {
            if (  (data[18].fvalue < 0.45133224)) {
              sum += (float)-0.26114324;
            } else {
              sum += (float)0.099173859;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 8.999999)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)-0.24653682;
            } else {
              sum += (float)-0.41069227;
            }
          } else {
            if (  (data[12].fvalue < 15.999999)) {
              sum += (float)-0.12909888;
            } else {
              sum += (float)-0.33074382;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.97402263)) {
            if (  (data[12].fvalue < 13.999999)) {
              sum += (float)-0.1588816;
            } else {
              sum += (float)-0.40569893;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)3.6983579e-06;
            } else {
              sum += (float)0.14957882;
            }
          }
        }
      }
    } else {
      sum += (float)0.73558038;
    }
  } else {
    if (  (data[10].fvalue < 0.033490464)) {
      if (  (data[10].fvalue < 0.015205976)) {
        if (  (data[6].fvalue < 7.7598052)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.2547754;
            } else {
              sum += (float)0.36593643;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.37253898;
            } else {
              sum += (float)0.18508641;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.20051219;
            } else {
              sum += (float)0.20550691;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.28733754;
            } else {
              sum += (float)0.10971577;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 1.5146872)) {
          if (  (data[2].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.024776652)) {
              sum += (float)0.13457172;
            } else {
              sum += (float)0.019835752;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.19040035;
            } else {
              sum += (float)0.39376155;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.44214481)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.034715809;
            } else {
              sum += (float)0.057485066;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0081550516;
            } else {
              sum += (float)0.10341434;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.99999899)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.37121174)) {
              sum += (float)-0.10910646;
            } else {
              sum += (float)0.0089377863;
            }
          } else {
            if (  (data[10].fvalue < 0.23506463)) {
              sum += (float)0.25724229;
            } else {
              sum += (float)0.08659339;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[20].fvalue < 1.7111967)) {
              sum += (float)-0.31276351;
            } else {
              sum += (float)-0.19378591;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.12363978;
            } else {
              sum += (float)-0.38201821;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.038661495;
            } else {
              sum += (float)-0.13250366;
            }
          } else {
            if (  (data[18].fvalue < 7.8815904)) {
              sum += (float)-0.056020718;
            } else {
              sum += (float)-0.27750754;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.17034079;
            } else {
              sum += (float)0.049264014;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.033561956;
            } else {
              sum += (float)-0.041952271;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 14.999999)) {
    if (  (data[21].fvalue < 0.021972563)) {
      if (  (data[14].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.011325369;
            } else {
              sum += (float)0.011006338;
            }
          } else {
            if (  (data[18].fvalue < 0.69639891)) {
              sum += (float)-0.064501822;
            } else {
              sum += (float)-0.22672525;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if ( (data[9].fvalue < 1.3041869)) {
              sum += (float)0.033037595;
            } else {
              sum += (float)-0.14625834;
            }
          } else {
            if (  (data[18].fvalue < 0.22182851)) {
              sum += (float)-0.19420148;
            } else {
              sum += (float)0.0081795119;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.0805123)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.12710251)) {
              sum += (float)0.23306872;
            } else {
              sum += (float)0.66587329;
            }
          } else {
            if (  (data[22].fvalue < 0.10956848)) {
              sum += (float)0.16225286;
            } else {
              sum += (float)0.028337352;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.6402091)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.0071035917;
            } else {
              sum += (float)0.10054792;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.227283;
            } else {
              sum += (float)0.03663604;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 0.095501915)) {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.20027237;
            } else {
              sum += (float)-0.11115197;
            }
          } else {
            if (  (data[18].fvalue < 0.028161451)) {
              sum += (float)-0.19327682;
            } else {
              sum += (float)-0.007796071;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.060640197)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.17535894;
            } else {
              sum += (float)0.26499769;
            }
          } else {
            if (  (data[9].fvalue < 3.0266793)) {
              sum += (float)0.079120725;
            } else {
              sum += (float)0.16905764;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.2170366)) {
          if (  (data[18].fvalue < 0.1399529)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.13495429;
            } else {
              sum += (float)-0.034711838;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.030351192;
            } else {
              sum += (float)-0.11272707;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)0.0016780096;
            } else {
              sum += (float)-0.12511711;
            }
          } else {
            if (  (data[9].fvalue < 2.8526254)) {
              sum += (float)-0.076226115;
            } else {
              sum += (float)-0.0011173303;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 8.999999)) {
      if (  (data[13].fvalue < 6.999999)) {
        if (  (data[12].fvalue < 16.999998)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.4673242;
            } else {
              sum += (float)-0.40029967;
            }
          } else {
            if (  (data[21].fvalue < 0.99010319)) {
              sum += (float)-0.35294658;
            } else {
              sum += (float)-0.25033391;
            }
          }
        } else {
          if (  (data[12].fvalue < 18.999998)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)-0.486853;
            } else {
              sum += (float)-0.401977;
            }
          } else {
            sum += (float)-0.55441493;
          }
        }
      } else {
        if (  (data[12].fvalue < 16.999998)) {
          if (  (data[18].fvalue < 4.0654635)) {
            if (  (data[21].fvalue < 0.79632241)) {
              sum += (float)-0.17449327;
            } else {
              sum += (float)-0.065635592;
            }
          } else {
            sum += (float)-0.29285187;
          }
        } else {
          if (  (data[12].fvalue < 17.999998)) {
            if (  (data[6].fvalue < 11.1928)) {
              sum += (float)-0.21012382;
            } else {
              sum += (float)-0.31136924;
            }
          } else {
            if (  (data[21].fvalue < 0.92045039)) {
              sum += (float)-0.32250759;
            } else {
              sum += (float)-0.48316082;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 1.1737452)) {
        if (  (data[18].fvalue < 0.86415273)) {
          if (  (data[13].fvalue < 13.999999)) {
            sum += (float)-0.34471956;
          } else {
            if (  (data[9].fvalue < 4.2036562)) {
              sum += (float)-0.47656479;
            } else {
              sum += (float)-0.56181586;
            }
          }
        } else {
          if (  (data[12].fvalue < 16.999998)) {
            if (  (data[18].fvalue < 1.0387793)) {
              sum += (float)-0.24366261;
            } else {
              sum += (float)-0.13064323;
            }
          } else {
            if (  (data[13].fvalue < 14.999999)) {
              sum += (float)-0.35739073;
            } else {
              sum += (float)-0.47582513;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 14.999999)) {
          if (  (data[12].fvalue < 16.999998)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.025940325;
            } else {
              sum += (float)-0.056645829;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.24624217;
            } else {
              sum += (float)-0.062211871;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.608789)) {
            if ( (data[9].fvalue < 1.9404869)) {
              sum += (float)-0.45234191;
            } else {
              sum += (float)-0.20064083;
            }
          } else {
            if (  (data[12].fvalue < 19.999998)) {
              sum += (float)0.12338039;
            } else {
              sum += (float)0.48726076;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 2.4906638)) {
    if (  (data[18].fvalue < 0.016717479)) {
      if (  (data[19].fvalue < 0.062975667)) {
        if (  (data[17].fvalue < 0.99999899)) {
          if (  (data[21].fvalue < 1.1511648)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.052952752;
            } else {
              sum += (float)-0.21292181;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.0093951756;
            } else {
              sum += (float)0.6177538;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.56055444;
            } else {
              sum += (float)-0.076671116;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.32494289;
            } else {
              sum += (float)-0.03045078;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[21].fvalue < 0.3832655)) {
            if (  (data[21].fvalue < 0.02296352)) {
              sum += (float)0.0022786756;
            } else {
              sum += (float)0.058064736;
            }
          } else {
            if (  (data[8].fvalue < 1.1999625)) {
              sum += (float)-0.044536468;
            } else {
              sum += (float)-0.0053202161;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.19570598)) {
              sum += (float)-0.21765308;
            } else {
              sum += (float)-0.36143404;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.11332073;
            } else {
              sum += (float)-0.5310595;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 0.10628708)) {
        if (  (data[6].fvalue < 0.34405723)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.12554874;
            } else {
              sum += (float)0.23236187;
            }
          } else {
            if (  (data[18].fvalue < 0.068590209)) {
              sum += (float)-0.57973963;
            } else {
              sum += (float)0.18824419;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.58632326)) {
            if (  (data[19].fvalue < 0.086887501)) {
              sum += (float)0.040151495;
            } else {
              sum += (float)-0.0049550151;
            }
          } else {
            if (  (data[8].fvalue < 0.034522142)) {
              sum += (float)0.28225175;
            } else {
              sum += (float)0.078094125;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[6].fvalue < 1.2434355)) {
            if (  (data[7].fvalue < 0.98696262)) {
              sum += (float)-0.086711422;
            } else {
              sum += (float)0.25613779;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0013720207;
            } else {
              sum += (float)0.019064967;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0043498981;
            } else {
              sum += (float)0.023161789;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.085938752;
            } else {
              sum += (float)0.0081634279;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 2.999999)) {
      if (  (data[18].fvalue < 0.11483054)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 0.02296352)) {
              sum += (float)-0.047629163;
            } else {
              sum += (float)0.012819236;
            }
          } else {
            if (  (data[5].fvalue < 27.363026)) {
              sum += (float)-0.055381067;
            } else {
              sum += (float)0.06512332;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.068590209)) {
              sum += (float)-0.41449952;
            } else {
              sum += (float)-0.29020417;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.13041368;
            } else {
              sum += (float)0.05913046;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[18].fvalue < 0.45090082)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.022288024;
            } else {
              sum += (float)0.1416485;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.11553546;
            } else {
              sum += (float)-0.15440394;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[5].fvalue < 25.885439)) {
              sum += (float)-0.099442922;
            } else {
              sum += (float)-0.0054354756;
            }
          } else {
            if (  (data[13].fvalue < 9.999999)) {
              sum += (float)-0.15901355;
            } else {
              sum += (float)-0.33925906;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.32448304)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.02296352)) {
              sum += (float)0.22768186;
            } else {
              sum += (float)0.01268169;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.059519999;
            } else {
              sum += (float)-0.38976395;
            }
          }
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[18].fvalue < 0.2543112)) {
              sum += (float)-0.30120212;
            } else {
              sum += (float)-0.18104523;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.061306201;
            } else {
              sum += (float)-0.15306719;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.59080619)) {
              sum += (float)0.04108236;
            } else {
              sum += (float)-0.208242;
            }
          } else {
            if (  (data[18].fvalue < 2.0599506)) {
              sum += (float)-0.19807485;
            } else {
              sum += (float)0.059541404;
            }
          }
        } else {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[18].fvalue < 2.0599506)) {
              sum += (float)-0.005682773;
            } else {
              sum += (float)0.20708284;
            }
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.2042941;
            } else {
              sum += (float)0.013131796;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 46.131725)) {
    if (  (data[18].fvalue < 2.5348294)) {
      if (  (data[11].fvalue < 0.071915105)) {
        if (  (data[6].fvalue < 0.9258098)) {
          if (  (data[11].fvalue < 0.055678561)) {
            if (  (data[10].fvalue < 0.015647652)) {
              sum += (float)0.42344794;
            } else {
              sum += (float)0.68964624;
            }
          } else {
            if (  (data[6].fvalue < 0.57156795)) {
              sum += (float)0.79469788;
            } else {
              sum += (float)0.21686743;
            }
          }
        } else {
          if (  (data[5].fvalue < 10.303511)) {
            if (  (data[18].fvalue < 0.016873106)) {
              sum += (float)-0.033073001;
            } else {
              sum += (float)0.068165012;
            }
          } else {
            if (  (data[22].fvalue < 2.0555892)) {
              sum += (float)-0.012202851;
            } else {
              sum += (float)0.23130462;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 8.8500986)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.85922414)) {
              sum += (float)-0.013865978;
            } else {
              sum += (float)-0.26632652;
            }
          } else {
            if (  (data[21].fvalue < 0.022003066)) {
              sum += (float)-0.0010994398;
            } else {
              sum += (float)0.035928801;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.1094136)) {
            if (  (data[6].fvalue < 0.34285679)) {
              sum += (float)0.13109364;
            } else {
              sum += (float)0.01017613;
            }
          } else {
            if (  (data[10].fvalue < 0.57745075)) {
              sum += (float)-0.0012875589;
            } else {
              sum += (float)0.016378293;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.35213071)) {
        if (  (data[21].fvalue < 3.7463109)) {
          if (  (data[22].fvalue < 3.4345427)) {
            if (  (data[21].fvalue < 0.29146013)) {
              sum += (float)0.016199235;
            } else {
              sum += (float)-0.03880737;
            }
          } else {
            if (  (data[5].fvalue < 36.775471)) {
              sum += (float)-0.21145114;
            } else {
              sum += (float)-0.11270229;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.52869695)) {
            if (  (data[5].fvalue < 33.611019)) {
              sum += (float)-0.21181086;
            } else {
              sum += (float)-0.10720953;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.31642032;
            } else {
              sum += (float)-0.16176964;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 26.633879)) {
          if (  (data[22].fvalue < 0.61218917)) {
            if (  (data[21].fvalue < 2.7205579)) {
              sum += (float)0.031017702;
            } else {
              sum += (float)-0.072111197;
            }
          } else {
            if (  (data[5].fvalue < 17.210262)) {
              sum += (float)-0.10554916;
            } else {
              sum += (float)-0.038722992;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.1094136)) {
            if (  (data[6].fvalue < 0.34285679)) {
              sum += (float)0.2608895;
            } else {
              sum += (float)0.043961011;
            }
          } else {
            if (  (data[18].fvalue < 7.9343848)) {
              sum += (float)0.012612365;
            } else {
              sum += (float)-0.02467203;
            }
          }
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 0.42969841)) {
      if (  (data[21].fvalue < 3.7463109)) {
        if (  (data[22].fvalue < 4.7999797)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 0.32619119)) {
              sum += (float)0.012704478;
            } else {
              sum += (float)-0.0026492763;
            }
          } else {
            if (  (data[11].fvalue < 0.13171358)) {
              sum += (float)-0.035185724;
            } else {
              sum += (float)0.082815543;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.32564223)) {
            if (  (data[18].fvalue < 2.5348294)) {
              sum += (float)-0.024534974;
            } else {
              sum += (float)-0.1235313;
            }
          } else {
            if (  (data[22].fvalue < 11.089332)) {
              sum += (float)0.036739755;
            } else {
              sum += (float)-0.039386399;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.015647652)) {
          if (  (data[18].fvalue < 0.069850676)) {
            if (  (data[11].fvalue < 0.32564223)) {
              sum += (float)-0.22653295;
            } else {
              sum += (float)-0.091536216;
            }
          } else {
            if (  (data[22].fvalue < 1.4866738)) {
              sum += (float)-0.012281233;
            } else {
              sum += (float)-0.12645367;
            }
          }
        } else {
          if (  (data[18].fvalue < 4.8032465)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.0094245439;
            } else {
              sum += (float)-0.048351806;
            }
          } else {
            if (  (data[11].fvalue < 0.33872133)) {
              sum += (float)-0.16914983;
            } else {
              sum += (float)-0.049868319;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.77647406)) {
        if (  (data[21].fvalue < 1.1463882)) {
          if (  (data[10].fvalue < 0.80735153)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.093644127;
            } else {
              sum += (float)0.015173851;
            }
          } else {
            sum += (float)0.37766311;
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.11078272)) {
              sum += (float)0.021984268;
            } else {
              sum += (float)-0.029194793;
            }
          } else {
            if (  (data[10].fvalue < 0.27739719)) {
              sum += (float)0.059173767;
            } else {
              sum += (float)0.15977129;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 4.0442381)) {
          if (  (data[16].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.016873106)) {
              sum += (float)0.00084246584;
            } else {
              sum += (float)0.027512835;
            }
          } else {
            if (  (data[22].fvalue < 11.089332)) {
              sum += (float)0.093499698;
            } else {
              sum += (float)0.023436964;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.15154457)) {
            if (  (data[21].fvalue < 0.54318041)) {
              sum += (float)0.06038335;
            } else {
              sum += (float)0.13636005;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.089284018;
            } else {
              sum += (float)0.041817792;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 0.99461144)) {
    if (  (data[10].fvalue < 0.48875952)) {
      if (  (data[18].fvalue < 0.038583536)) {
        if (  (data[9].fvalue < 0.96287221)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.015093146)) {
              sum += (float)0.17070957;
            } else {
              sum += (float)0.67406458;
            }
          } else {
            sum += (float)-0.78498954;
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.017284796)) {
              sum += (float)-0.52196938;
            } else {
              sum += (float)-0.060649488;
            }
          } else {
            if (  (data[18].fvalue < 0.017284796)) {
              sum += (float)0.048865024;
            } else {
              sum += (float)0.35104296;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 0.96287221)) {
          sum += (float)-2.0479395;
        } else {
          if (  (data[18].fvalue < 0.058183659)) {
            if (  (data[10].fvalue < 0.34742713)) {
              sum += (float)0.51998943;
            } else {
              sum += (float)0.099218525;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.70602727;
            } else {
              sum += (float)0.27266723;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.058183659)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 0.96287221)) {
            if (  (data[10].fvalue < 0.68621773)) {
              sum += (float)0.18703918;
            } else {
              sum += (float)-0.14651978;
            }
          } else {
            if (  (data[18].fvalue < 0.048129309)) {
              sum += (float)-0.65317416;
            } else {
              sum += (float)-0.30707896;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.017284796)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.039063971;
            } else {
              sum += (float)0.026122948;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.12700757;
            } else {
              sum += (float)-0.079565801;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.74309129)) {
          if (  (data[18].fvalue < 0.10340934)) {
            if (  (data[9].fvalue < 0.96287221)) {
              sum += (float)0.56303996;
            } else {
              sum += (float)0.098890804;
            }
          } else {
            if (  (data[10].fvalue < 0.64261943)) {
              sum += (float)0.67078215;
            } else {
              sum += (float)0.26502225;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.04731128;
            } else {
              sum += (float)-0.13026422;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.21842061;
            } else {
              sum += (float)-0.0017341496;
            }
          }
        }
      }
    }
  } else {
    if (  (data[5].fvalue < 1.1134739)) {
      if ( (data[9].fvalue < 1.1590157)) {
        if (  (data[18].fvalue < 0.058183659)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.26770988)) {
              sum += (float)-0.35658205;
            } else {
              sum += (float)-0.17377736;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.085292354;
            } else {
              sum += (float)-0.059386577;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0313096)) {
            if (  (data[10].fvalue < 0.29564565)) {
              sum += (float)0.38510677;
            } else {
              sum += (float)0.04693063;
            }
          } else {
            if (  (data[18].fvalue < 0.23955326)) {
              sum += (float)-0.16406284;
            } else {
              sum += (float)0.026805775;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.017284796)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[6].fvalue < 0.33867657)) {
              sum += (float)0.45589912;
            } else {
              sum += (float)0.21916397;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.018506795;
            } else {
              sum += (float)0.16956128;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.38383034)) {
            if (  (data[9].fvalue < 1.2635131)) {
              sum += (float)0.0016399338;
            } else {
              sum += (float)-0.081924684;
            }
          } else {
            if (  (data[9].fvalue < 1.3035337)) {
              sum += (float)0.23666197;
            } else {
              sum += (float)-0.060922183;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 0.99995053)) {
              sum += (float)0.045420852;
            } else {
              sum += (float)-0.099300198;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.099370293;
            } else {
              sum += (float)0.011972062;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.2217987)) {
              sum += (float)-0.042334493;
            } else {
              sum += (float)0.0005293297;
            }
          } else {
            if (  (data[18].fvalue < 0.4067837)) {
              sum += (float)0.030626776;
            } else {
              sum += (float)-0.033689529;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.4092761)) {
          if (  (data[10].fvalue < 0.45515108)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.0061548087;
            } else {
              sum += (float)0.064665414;
            }
          } else {
            if ( (data[9].fvalue < 1.2635131)) {
              sum += (float)-0.03606984;
            } else {
              sum += (float)0.003540627;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.28867123)) {
            if (  (data[9].fvalue < 1.6866981)) {
              sum += (float)0.044751849;
            } else {
              sum += (float)0.0045731906;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.01037914;
            } else {
              sum += (float)0.0061415709;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 0.64950144)) {
    if (  (data[13].fvalue < 6.999999)) {
      if (  (data[13].fvalue < 2.999999)) {
        if (  (data[19].fvalue < 0.45937309)) {
          if (  (data[19].fvalue < 0.018454872)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0023188046;
            } else {
              sum += (float)-0.077481352;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.026603043;
            } else {
              sum += (float)-0.013723616;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.016708478)) {
            if (  (data[22].fvalue < 0.77661884)) {
              sum += (float)0.012318823;
            } else {
              sum += (float)-0.035350215;
            }
          } else {
            if (  (data[22].fvalue < 0.71012396)) {
              sum += (float)-0.097181566;
            } else {
              sum += (float)-0.0087484457;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.11249604)) {
          if (  (data[18].fvalue < 0.22402303)) {
            if (  (data[19].fvalue < 0.07555566)) {
              sum += (float)-0.43928164;
            } else {
              sum += (float)-0.24549429;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.032586224;
            } else {
              sum += (float)-0.39767006;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.37461594)) {
            if (  (data[8].fvalue < 0.25057903)) {
              sum += (float)0.01762406;
            } else {
              sum += (float)-0.042614311;
            }
          } else {
            if (  (data[18].fvalue < 0.14126234)) {
              sum += (float)0.078352213;
            } else {
              sum += (float)0.020713516;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.23766604)) {
        if (  (data[19].fvalue < 0.1799697)) {
          if (  (data[19].fvalue < 0.1520761)) {
            sum += (float)-0.50339943;
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.37656081;
            } else {
              sum += (float)-0.49393904;
            }
          }
        } else {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[18].fvalue < 0.28901508)) {
              sum += (float)-0.31371596;
            } else {
              sum += (float)-0.18221019;
            }
          } else {
            if (  (data[11].fvalue < 0.05543907)) {
              sum += (float)-0.34953266;
            } else {
              sum += (float)-0.43963525;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.049285129)) {
          if (  (data[10].fvalue < 0.30542943)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)0.076863609;
            } else {
              sum += (float)-0.07298813;
            }
          } else {
            if (  (data[8].fvalue < 0.60477871)) {
              sum += (float)0.37249941;
            } else {
              sum += (float)0.10837752;
            }
          }
        } else {
          if (  (data[13].fvalue < 11.999999)) {
            if (  (data[18].fvalue < 1.526832)) {
              sum += (float)-0.066092245;
            } else {
              sum += (float)-0.14525899;
            }
          } else {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)-0.34462371;
            } else {
              sum += (float)-0.5029406;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 2.999999)) {
      if (  (data[18].fvalue < 0.12793426)) {
        if (  (data[18].fvalue < 0.016708478)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 2.0348499)) {
              sum += (float)-0.046859823;
            } else {
              sum += (float)-0.0096917087;
            }
          } else {
            if (  (data[22].fvalue < 0.71012396)) {
              sum += (float)0.077964932;
            } else {
              sum += (float)-0.0007260688;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.35223606;
            } else {
              sum += (float)-0.18448645;
            }
          } else {
            if (  (data[18].fvalue < 0.069367573)) {
              sum += (float)-0.10720038;
            } else {
              sum += (float)0.061069973;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.45351803)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.035251461;
            } else {
              sum += (float)0.11552275;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.051601533;
            } else {
              sum += (float)-0.060387511;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.045101583;
            } else {
              sum += (float)0.12389326;
            }
          } else {
            if (  (data[22].fvalue < 0.71012396)) {
              sum += (float)-0.23807755;
            } else {
              sum += (float)-0.074836582;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.16474852)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[18].fvalue < 0.016708478)) {
              sum += (float)0.082709849;
            } else {
              sum += (float)0.16267483;
            }
          } else {
            if (  (data[19].fvalue < 1.1741431)) {
              sum += (float)0.040771864;
            } else {
              sum += (float)-0.046762519;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.016708478)) {
            if (  (data[11].fvalue < 0.32562423)) {
              sum += (float)-0.18831113;
            } else {
              sum += (float)-0.08036723;
            }
          } else {
            if (  (data[8].fvalue < 4.5424972)) {
              sum += (float)-0.03386661;
            } else {
              sum += (float)0.074784316;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[18].fvalue < 0.42972261)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.028574355;
            } else {
              sum += (float)-0.062533006;
            }
          } else {
            if (  (data[22].fvalue < 0.71012396)) {
              sum += (float)-0.11089282;
            } else {
              sum += (float)-0.016352953;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.4226012)) {
            if (  (data[13].fvalue < 14.999999)) {
              sum += (float)0.031233147;
            } else {
              sum += (float)-0.2393045;
            }
          } else {
            if (  (data[13].fvalue < 12.999999)) {
              sum += (float)-0.018755833;
            } else {
              sum += (float)0.060084343;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 2.0846567)) {
    if (  (data[19].fvalue < 1.2552702)) {
      if (  (data[18].fvalue < 1.530295)) {
        if (  (data[19].fvalue < 0.0875329)) {
          if (  (data[9].fvalue < 1.0968161)) {
            if (  (data[18].fvalue < 0.081457995)) {
              sum += (float)-0.079624005;
            } else {
              sum += (float)0.045797776;
            }
          } else {
            if (  (data[18].fvalue < 0.091503188)) {
              sum += (float)0.12402254;
            } else {
              sum += (float)-0.0047711283;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.0365944)) {
            if (  (data[8].fvalue < 0.21766496)) {
              sum += (float)0.26224741;
            } else {
              sum += (float)-0.0089705214;
            }
          } else {
            if (  (data[8].fvalue < 0.97663802)) {
              sum += (float)-0.024231086;
            } else {
              sum += (float)0.0066587604;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.020957461)) {
          if (  (data[8].fvalue < 3.7572958)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.24097729;
            } else {
              sum += (float)-0.091594689;
            }
          } else {
            if (  (data[18].fvalue < 2.3423121)) {
              sum += (float)0.025153816;
            } else {
              sum += (float)-0.067185283;
            }
          }
        } else {
          if (  (data[22].fvalue < 2.0607524)) {
            if (  (data[8].fvalue < 3.525367)) {
              sum += (float)-0.0024811914;
            } else {
              sum += (float)0.13085581;
            }
          } else {
            if (  (data[22].fvalue < 3.2419302)) {
              sum += (float)-0.095650084;
            } else {
              sum += (float)-0.24505517;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 2.6103051)) {
        if (  (data[9].fvalue < 1.8737143)) {
          if (  (data[19].fvalue < 1.5159698)) {
            if (  (data[8].fvalue < 1.5649158)) {
              sum += (float)-0.23133211;
            } else {
              sum += (float)-0.059866719;
            }
          } else {
            if (  (data[18].fvalue < 0.081457995)) {
              sum += (float)-0.33423531;
            } else {
              sum += (float)-0.19206002;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.757882)) {
            if (  (data[18].fvalue < 0.53312695)) {
              sum += (float)0.010876692;
            } else {
              sum += (float)-0.18976329;
            }
          } else {
            if (  (data[9].fvalue < 2.0133865)) {
              sum += (float)-0.18238465;
            } else {
              sum += (float)-0.054699536;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.87009948)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.32698587)) {
              sum += (float)-0.013546143;
            } else {
              sum += (float)-0.38619402;
            }
          } else {
            if (  (data[18].fvalue < 0.48114559)) {
              sum += (float)0.13663198;
            } else {
              sum += (float)-0.016555337;
            }
          }
        } else {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[8].fvalue < 8.4184275)) {
              sum += (float)-0.56532604;
            } else {
              sum += (float)-0.10783768;
            }
          } else {
            if (  (data[18].fvalue < 1.0957733)) {
              sum += (float)-0.050985958;
            } else {
              sum += (float)-0.24754293;
            }
          }
        }
      }
    }
  } else {
    if (  (data[10].fvalue < 0.68784612)) {
      if (  (data[8].fvalue < 0.10918356)) {
        if (  (data[19].fvalue < 0.017316721)) {
          if (  (data[22].fvalue < 0.046837471)) {
            if (  (data[7].fvalue < 0.99995214)) {
              sum += (float)-0.015052603;
            } else {
              sum += (float)-0.32229114;
            }
          } else {
            if (  (data[8].fvalue < 0.035224494)) {
              sum += (float)0.13313705;
            } else {
              sum += (float)0.032430254;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.035224494)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.27239439;
            } else {
              sum += (float)0.42039275;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.06089535;
            } else {
              sum += (float)0.20841803;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99661219)) {
          if (  (data[10].fvalue < 0.38235721)) {
            if (  (data[9].fvalue < 3.6730163)) {
              sum += (float)-0.014929407;
            } else {
              sum += (float)-0.00074800843;
            }
          } else {
            if (  (data[19].fvalue < 0.1664528)) {
              sum += (float)-0.012793201;
            } else {
              sum += (float)0.017924378;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.9566592)) {
            if (  (data[22].fvalue < 2.0607524)) {
              sum += (float)0.011590491;
            } else {
              sum += (float)-0.0067639607;
            }
          } else {
            if (  (data[10].fvalue < 0.015264708)) {
              sum += (float)0.15019061;
            } else {
              sum += (float)0.61949992;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 2.2246144)) {
        if (  (data[10].fvalue < 0.81070858)) {
          if (  (data[19].fvalue < 2.4673181)) {
            if (  (data[19].fvalue < 0.3754009)) {
              sum += (float)0.064318113;
            } else {
              sum += (float)0.12912287;
            }
          } else {
            if (  (data[9].fvalue < 3.1979933)) {
              sum += (float)0.0040161498;
            } else {
              sum += (float)0.084655009;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.067251392)) {
            if (  (data[18].fvalue < 3.3619063)) {
              sum += (float)0.24565996;
            } else {
              sum += (float)0.10862593;
            }
          } else {
            if (  (data[19].fvalue < 0.12652269)) {
              sum += (float)0.04211228;
            } else {
              sum += (float)0.1517138;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.46656686)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 2.1099913)) {
              sum += (float)-0.069146022;
            } else {
              sum += (float)-0.16780442;
            }
          } else {
            if (  (data[18].fvalue < 1.6531165)) {
              sum += (float)0.056534145;
            } else {
              sum += (float)-0.049674533;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.92408979)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.0081763491;
            } else {
              sum += (float)0.048865587;
            }
          } else {
            if (  (data[10].fvalue < 0.81070858)) {
              sum += (float)0.092377372;
            } else {
              sum += (float)0.20867506;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 0.96286577)) {
    if (  (data[7].fvalue < 0.99995124)) {
      if (  (data[9].fvalue < 0.91281688)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.008822931)) {
            if (  (data[7].fvalue < 0.99898463)) {
              sum += (float)0.39893284;
            } else {
              sum += (float)0.082271658;
            }
          } else {
            if (  (data[19].fvalue < 0.28538722)) {
              sum += (float)0.0016455554;
            } else {
              sum += (float)0.060081676;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.016861154)) {
            if (  (data[19].fvalue < 0.19611101)) {
              sum += (float)-0.017640857;
            } else {
              sum += (float)0.084163964;
            }
          } else {
            if (  (data[18].fvalue < 1.0901946)) {
              sum += (float)-0.042997435;
            } else {
              sum += (float)0.058070306;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.89570123)) {
          if (  (data[11].fvalue < 0.26111436)) {
            if (  (data[7].fvalue < 0.35562524)) {
              sum += (float)0.3122125;
            } else {
              sum += (float)0.09649162;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.098434903;
            } else {
              sum += (float)0.025468165;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.01769577)) {
            if (  (data[19].fvalue < 0.042687032)) {
              sum += (float)-0.12233966;
            } else {
              sum += (float)0.026319517;
            }
          } else {
            if (  (data[19].fvalue < 0.017281793)) {
              sum += (float)0.013353919;
            } else {
              sum += (float)0.099537686;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.01769577)) {
        if (  (data[19].fvalue < 0.017281793)) {
          if (  (data[4].fvalue < 0.99999899)) {
            sum += (float)-0.65902913;
          } else {
            if (  (data[1].fvalue < 0.99999899)) {
              sum += (float)0.27783984;
            } else {
              sum += (float)0.32922065;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.05390105)) {
            if (  (data[9].fvalue < 0.91281688)) {
              sum += (float)0.55386877;
            } else {
              sum += (float)0.29031223;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.5764693;
            } else {
              sum += (float)0.38681662;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 0.91281688)) {
          sum += (float)0.67956918;
        } else {
          if (  (data[19].fvalue < 0.017281793)) {
            if (  (data[18].fvalue < 0.049486745)) {
              sum += (float)0.30165166;
            } else {
              sum += (float)0.43432355;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.56423569;
            } else {
              sum += (float)0.41538212;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.4147252)) {
      if (  (data[19].fvalue < 0.41400766)) {
        if (  (data[18].fvalue < 0.34696618)) {
          if (  (data[22].fvalue < 0.24056171)) {
            if ( (data[9].fvalue < 0.99577051)) {
              sum += (float)0.0056200307;
            } else {
              sum += (float)-0.037911516;
            }
          } else {
            if (  (data[18].fvalue < 0.058684636)) {
              sum += (float)0.098975144;
            } else {
              sum += (float)-0.015279168;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.3044779)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.077637263;
            } else {
              sum += (float)-0.037806544;
            }
          } else {
            if (  (data[18].fvalue < 0.50943065)) {
              sum += (float)-0.065707996;
            } else {
              sum += (float)-0.017727315;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.36640289)) {
          if (  (data[9].fvalue < 1.350341)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.054657467;
            } else {
              sum += (float)-0.050167765;
            }
          } else {
            if (  (data[19].fvalue < 0.5872227)) {
              sum += (float)-0.084005021;
            } else {
              sum += (float)0.015849801;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.029422833)) {
            if (  (data[11].fvalue < 0.022331847)) {
              sum += (float)0.29219553;
            } else {
              sum += (float)0.054808062;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.12404138;
            } else {
              sum += (float)0.16218248;
            }
          }
        }
      }
    } else {
      if (  (data[22].fvalue < 0.138474)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.30749199)) {
              sum += (float)0.045544513;
            } else {
              sum += (float)-0.020399218;
            }
          } else {
            if (  (data[18].fvalue < 0.01769577)) {
              sum += (float)-0.021131933;
            } else {
              sum += (float)0.0071362397;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.085693561)) {
            if (  (data[22].fvalue < 0.047635399)) {
              sum += (float)-0.21700124;
            } else {
              sum += (float)0.012730797;
            }
          } else {
            if (  (data[9].fvalue < 1.9387448)) {
              sum += (float)0.29331145;
            } else {
              sum += (float)0.16174117;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.8722458)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.061007533;
            } else {
              sum += (float)-0.0027630706;
            }
          } else {
            if (  (data[19].fvalue < 0.017281793)) {
              sum += (float)0.2038338;
            } else {
              sum += (float)0.010089668;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.0075993063;
            } else {
              sum += (float)0.18914281;
            }
          } else {
            if (  (data[9].fvalue < 4.4522815)) {
              sum += (float)-0.009957524;
            } else {
              sum += (float)0.0032252977;
            }
          }
        }
      }
    }
  }
  if (  (data[21].fvalue < 3.0811)) {
    if (  (data[15].fvalue < 1.999999)) {
      if (  (data[16].fvalue < 4.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[21].fvalue < 0.45922858)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.0029764178;
            } else {
              sum += (float)0.019430352;
            }
          } else {
            if (  (data[22].fvalue < 1.0375947)) {
              sum += (float)-0.029520798;
            } else {
              sum += (float)0.0054454911;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.42735261)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.0048283557;
            } else {
              sum += (float)-0.039298277;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.05904182;
            } else {
              sum += (float)-0.0028214224;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if ( (data[9].fvalue < 4.7040577)) {
            if (  (data[22].fvalue < 5.5190496)) {
              sum += (float)-0.098408438;
            } else {
              sum += (float)-0.26032028;
            }
          } else {
            if (  (data[22].fvalue < 8.4875517)) {
              sum += (float)0.071072176;
            } else {
              sum += (float)-0.087222405;
            }
          }
        } else {
          if (  (data[22].fvalue < 7.5445142)) {
            if (  (data[9].fvalue < 4.4498496)) {
              sum += (float)0.028703591;
            } else {
              sum += (float)0.17179896;
            }
          } else {
            if (  (data[10].fvalue < 0.24281979)) {
              sum += (float)-0.075158268;
            } else {
              sum += (float)-0.00047172391;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[21].fvalue < 0.58654475)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.13108651;
            } else {
              sum += (float)0.041853789;
            }
          } else {
            if (  (data[15].fvalue < 3.999999)) {
              sum += (float)0.010071728;
            } else {
              sum += (float)0.23634528;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.92548615)) {
              sum += (float)-0.090682693;
            } else {
              sum += (float)-0.033023227;
            }
          } else {
            if (  (data[21].fvalue < 0.17499499)) {
              sum += (float)0.22220521;
            } else {
              sum += (float)0.019538729;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 3.0317953)) {
          if (  (data[9].fvalue < 2.694371)) {
            if (  (data[10].fvalue < 0.27751377)) {
              sum += (float)-0.23165545;
            } else {
              sum += (float)0.0093041947;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.18889958;
            } else {
              sum += (float)0.10168405;
            }
          }
        } else {
          if (  (data[21].fvalue < 1.1448247)) {
            if (  (data[16].fvalue < 3.999999)) {
              sum += (float)0.062381975;
            } else {
              sum += (float)0.12585112;
            }
          } else {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)0.024273708;
            } else {
              sum += (float)-0.062955372;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 7.999999)) {
      if ( (data[9].fvalue < 5.439476)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[10].fvalue < 0.3155731)) {
              sum += (float)0.0070752003;
            } else {
              sum += (float)-0.01687249;
            }
          } else {
            if (  (data[21].fvalue < 5.367763)) {
              sum += (float)0.16879246;
            } else {
              sum += (float)0.070756115;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.018285921;
            } else {
              sum += (float)-0.057048611;
            }
          } else {
            if (  (data[10].fvalue < 0.57544899)) {
              sum += (float)-0.042661563;
            } else {
              sum += (float)0.037704084;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.015526282)) {
              sum += (float)-0.24462871;
            } else {
              sum += (float)-0.065152332;
            }
          } else {
            if (  (data[10].fvalue < 0.08138556)) {
              sum += (float)-0.043720424;
            } else {
              sum += (float)0.013773253;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.043307502;
            } else {
              sum += (float)-0.017833639;
            }
          } else {
            if (  (data[22].fvalue < 11.076076)) {
              sum += (float)0.0064126705;
            } else {
              sum += (float)-0.021979552;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 5.2691236)) {
        if (  (data[22].fvalue < 1.4873092)) {
          if (  (data[10].fvalue < 0.60570729)) {
            if (  (data[21].fvalue < 5.367763)) {
              sum += (float)-0.024211032;
            } else {
              sum += (float)-0.079122104;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.047591209;
            } else {
              sum += (float)0.3375344;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.52759469)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.07927528;
            } else {
              sum += (float)-0.15375185;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.11089782;
            } else {
              sum += (float)0.0565373;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 8.4875517)) {
          if (  (data[21].fvalue < 5.367763)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.05189104;
            } else {
              sum += (float)0.044650283;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.027151067;
            } else {
              sum += (float)-0.042661961;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 6.8725548)) {
              sum += (float)-0.037097625;
            } else {
              sum += (float)0.037894294;
            }
          } else {
            if (  (data[16].fvalue < 6.999999)) {
              sum += (float)-0.095743984;
            } else {
              sum += (float)0.13409993;
            }
          }
        }
      }
    }
  }
  if (  (data[20].fvalue < 2.5187626)) {
    if (  (data[8].fvalue < 0.034759093)) {
      if (  (data[9].fvalue < 0.99468428)) {
        if (  (data[9].fvalue < 0.96412212)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.015748231)) {
              sum += (float)0.17085263;
            } else {
              sum += (float)0.73716456;
            }
          } else {
            if (  (data[6].fvalue < 0.35971668)) {
              sum += (float)-1.3078482;
            } else {
              sum += (float)-0.55413425;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 0.33670452)) {
              sum += (float)0.1689761;
            } else {
              sum += (float)0.57340145;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.21653959;
            } else {
              sum += (float)-0.43475306;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.6948849)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.15157714)) {
              sum += (float)-0.5015586;
            } else {
              sum += (float)-0.10855063;
            }
          } else {
            if (  (data[11].fvalue < 0.71715683)) {
              sum += (float)-0.0061300276;
            } else {
              sum += (float)-0.2905212;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.2202141)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.057243001;
            } else {
              sum += (float)0.09939003;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.07286495;
            } else {
              sum += (float)0.0022958152;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.4113342)) {
        if (  (data[8].fvalue < 0.28983071)) {
          if (  (data[10].fvalue < 1.0992911)) {
            if (  (data[9].fvalue < 0.96412212)) {
              sum += (float)0.066429578;
            } else {
              sum += (float)-0.006245723;
            }
          } else {
            if (  (data[8].fvalue < 0.21535243)) {
              sum += (float)0.29326698;
            } else {
              sum += (float)0.086941645;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99468428)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.055796135;
            } else {
              sum += (float)-0.0004032492;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.022122409;
            } else {
              sum += (float)-0.015793754;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.13125007)) {
          if (  (data[20].fvalue < 0.043000799)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.0098077673;
            } else {
              sum += (float)-0.017710788;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.026061894;
            } else {
              sum += (float)0.11124615;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.6871234)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.054594632;
            } else {
              sum += (float)0.051356994;
            }
          } else {
            if (  (data[9].fvalue < 3.195317)) {
              sum += (float)-0.0062675145;
            } else {
              sum += (float)0.0068167187;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 7.6735706)) {
      if (  (data[10].fvalue < 0.55133963)) {
        if (  (data[9].fvalue < 2.375453)) {
          if (  (data[8].fvalue < 2.1654401)) {
            if (  (data[8].fvalue < 0.97422791)) {
              sum += (float)0.91046035;
            } else {
              sum += (float)0.37650061;
            }
          } else {
            if (  (data[20].fvalue < 3.2609699)) {
              sum += (float)-0.061648138;
            } else {
              sum += (float)0.1503191;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 3.195317)) {
              sum += (float)-0.039537262;
            } else {
              sum += (float)-0.01019458;
            }
          } else {
            if (  (data[8].fvalue < 1.5705632)) {
              sum += (float)-0.10809018;
            } else {
              sum += (float)-0.046107817;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 4.5221562)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 4.5309792)) {
              sum += (float)0.0014570642;
            } else {
              sum += (float)-0.22942196;
            }
          } else {
            if (  (data[6].fvalue < 8.1989965)) {
              sum += (float)0.043455798;
            } else {
              sum += (float)-0.1055496;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.3006036)) {
            if (  (data[6].fvalue < 9.66506)) {
              sum += (float)0.39530122;
            } else {
              sum += (float)0.18980573;
            }
          } else {
            if (  (data[20].fvalue < 5.2461905)) {
              sum += (float)-0.088193536;
            } else {
              sum += (float)0.23469412;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 10.533896)) {
        if ( (data[9].fvalue < 2.1521928)) {
          if (  (data[11].fvalue < 0.32559073)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.24835809;
            } else {
              sum += (float)-0.069318697;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.087450527;
            } else {
              sum += (float)0.064273387;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.39788795)) {
            if (  (data[20].fvalue < 4.3211341)) {
              sum += (float)0.012535435;
            } else {
              sum += (float)-0.032752652;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.0018495137;
            } else {
              sum += (float)0.015734604;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.83621323)) {
          if (  (data[20].fvalue < 15.83392)) {
            if ( (data[9].fvalue < 6.0531483)) {
              sum += (float)-0.03423157;
            } else {
              sum += (float)0.0061416118;
            }
          } else {
            if (  (data[9].fvalue < 3.6746867)) {
              sum += (float)0.10515519;
            } else {
              sum += (float)-0.05161459;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.8580401)) {
            if (  (data[20].fvalue < 15.83392)) {
              sum += (float)0.17451824;
            } else {
              sum += (float)0.31336093;
            }
          } else {
            if (  (data[8].fvalue < 14.739176)) {
              sum += (float)-0.019406987;
            } else {
              sum += (float)0.0025051578;
            }
          }
        }
      }
    }
  }
  if (  (data[19].fvalue < 4.7958355)) {
    if (  (data[19].fvalue < 0.017784081)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0065179868;
            } else {
              sum += (float)-0.17136392;
            }
          } else {
            if (  (data[9].fvalue < 1.0904318)) {
              sum += (float)0.20530513;
            } else {
              sum += (float)-0.059864413;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.061599132;
            } else {
              sum += (float)-0.22539139;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.5981639;
            } else {
              sum += (float)0.37389058;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.0089369547)) {
              sum += (float)-0.46120679;
            } else {
              sum += (float)-0.51224488;
            }
          } else {
            if (  (data[9].fvalue < 1.0904318)) {
              sum += (float)-0.63509321;
            } else {
              sum += (float)-0.55911613;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.4710262)) {
            if (  (data[9].fvalue < 1.1646996)) {
              sum += (float)-0.61191976;
            } else {
              sum += (float)-0.42466098;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.042230394;
            } else {
              sum += (float)-0.37596649;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.35053119)) {
        if (  (data[9].fvalue < 0.99661279)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.076219715)) {
              sum += (float)0.25264579;
            } else {
              sum += (float)0.034492984;
            }
          } else {
            if (  (data[8].fvalue < 0.037039872)) {
              sum += (float)-0.54658651;
            } else {
              sum += (float)-0.15981483;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.053975191)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.36773068;
            } else {
              sum += (float)0.02284283;
            }
          } else {
            if (  (data[9].fvalue < 1.0370495)) {
              sum += (float)0.18379532;
            } else {
              sum += (float)0.011325159;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.12719277)) {
              sum += (float)-0.018979231;
            } else {
              sum += (float)0.013260213;
            }
          } else {
            if (  (data[9].fvalue < 1.1646996)) {
              sum += (float)0.078050762;
            } else {
              sum += (float)-0.029223656;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.15280566)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.054189663;
            } else {
              sum += (float)-0.092105538;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.080274537;
            } else {
              sum += (float)0.0099365367;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 3.0290103)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[21].fvalue < 0.024262514)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 6.1904931)) {
              sum += (float)-0.035842709;
            } else {
              sum += (float)0.058581386;
            }
          } else {
            if ( (data[9].fvalue < 2.5321503)) {
              sum += (float)0.53710014;
            } else {
              sum += (float)0.23865372;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[21].fvalue < 3.7472119)) {
              sum += (float)0.22174406;
            } else {
              sum += (float)-0.11956726;
            }
          } else {
            if (  (data[9].fvalue < 2.6928563)) {
              sum += (float)0.054325663;
            } else {
              sum += (float)-0.080551066;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.43186659;
            } else {
              sum += (float)-0.085483707;
            }
          } else {
            if (  (data[21].fvalue < 0.07881245)) {
              sum += (float)0.20210095;
            } else {
              sum += (float)-0.21305078;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.14967786;
            } else {
              sum += (float)-0.14285713;
            }
          } else {
            if (  (data[8].fvalue < 9.1004877)) {
              sum += (float)-0.11207345;
            } else {
              sum += (float)0.0081277788;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 3.9785197)) {
        if (  (data[8].fvalue < 2.9391084)) {
          if (  (data[21].fvalue < 0.024262514)) {
            if (  (data[9].fvalue < 3.5727723)) {
              sum += (float)-0.20765294;
            } else {
              sum += (float)-0.0027216214;
            }
          } else {
            if (  (data[9].fvalue < 3.7794311)) {
              sum += (float)-0.29662305;
            } else {
              sum += (float)-0.17282447;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.02704894;
            } else {
              sum += (float)-0.14828852;
            }
          } else {
            if (  (data[21].fvalue < 0.043543648)) {
              sum += (float)-0.026999613;
            } else {
              sum += (float)-0.14022572;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.082584143;
            } else {
              sum += (float)0.051669102;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.10452233;
            } else {
              sum += (float)-0.3090128;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.13058306;
            } else {
              sum += (float)-0.097075358;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.088527821;
            } else {
              sum += (float)-0.003539141;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 5.999999)) {
    if (  (data[22].fvalue < 2.0583901)) {
      if (  (data[21].fvalue < 0.023402359)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[20].fvalue < 0.019705754)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.00099751609;
            } else {
              sum += (float)-0.04292674;
            }
          } else {
            if (  (data[18].fvalue < 0.038266826)) {
              sum += (float)0.017535664;
            } else {
              sum += (float)0.10612848;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.55143714)) {
            if (  (data[20].fvalue < 1.6162512)) {
              sum += (float)-0.0035370723;
            } else {
              sum += (float)0.13434075;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.032399341;
            } else {
              sum += (float)-0.0047992966;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.33237553)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 0.021601548)) {
              sum += (float)0.0012879778;
            } else {
              sum += (float)-0.033048131;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.078733727;
            } else {
              sum += (float)0.040161785;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.11752667)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.082127981;
            } else {
              sum += (float)0.017369725;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.00043693391;
            } else {
              sum += (float)0.013693968;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.66159469)) {
        if (  (data[12].fvalue < 5.999999)) {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.90553027)) {
              sum += (float)0.082152307;
            } else {
              sum += (float)0.01983431;
            }
          } else {
            if (  (data[21].fvalue < 0.53859866)) {
              sum += (float)-0.024632774;
            } else {
              sum += (float)0.0018288682;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.43238193)) {
            if (  (data[8].fvalue < 5.8852711)) {
              sum += (float)-0.21590066;
            } else {
              sum += (float)-0.096138731;
            }
          } else {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)-0.01501062;
            } else {
              sum += (float)-0.085925527;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.6127014)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[21].fvalue < 0.043096106)) {
              sum += (float)-0.14046195;
            } else {
              sum += (float)-0.058309633;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.043208182;
            } else {
              sum += (float)0.00916111;
            }
          }
        } else {
          if (  (data[21].fvalue < 7.1449366)) {
            if (  (data[8].fvalue < 7.7362409)) {
              sum += (float)-0.015785582;
            } else {
              sum += (float)0.010516019;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.004797176;
            } else {
              sum += (float)-0.040751431;
            }
          }
        }
      }
    }
  } else {
    if (  (data[21].fvalue < 9.3025656)) {
      if (  (data[21].fvalue < 3.7551765)) {
        if (  (data[12].fvalue < 5.999999)) {
          sum += (float)0.38586685;
        } else {
          sum += (float)0.48828968;
        }
      } else {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.53412974)) {
              sum += (float)0.13974072;
            } else {
              sum += (float)0.30981499;
            }
          } else {
            if (  (data[8].fvalue < 4.8152118)) {
              sum += (float)0.13875955;
            } else {
              sum += (float)0.042607483;
            }
          }
        } else {
          if (  (data[15].fvalue < 6.999999)) {
            if (  (data[8].fvalue < 3.1214986)) {
              sum += (float)0.3864648;
            } else {
              sum += (float)0.24196288;
            }
          } else {
            sum += (float)0.53322792;
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[8].fvalue < 4.5147338)) {
          if (  (data[18].fvalue < 0.59528488)) {
            sum += (float)0.0070137763;
          } else {
            sum += (float)0.21041785;
          }
        } else {
          if (  (data[18].fvalue < 1.330073)) {
            if (  (data[20].fvalue < 8.3620663)) {
              sum += (float)-0.074687608;
            } else {
              sum += (float)-0.33942261;
            }
          } else {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.11257072;
            } else {
              sum += (float)-0.082736313;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 6.999999)) {
          if (  (data[20].fvalue < 8.3620663)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.14797841;
            } else {
              sum += (float)0.36350331;
            }
          } else {
            sum += (float)-0.0096323835;
          }
        } else {
          sum += (float)0.45792657;
        }
      }
    }
  }
  if (  (data[13].fvalue < 9.999999)) {
    if (  (data[12].fvalue < 9.999999)) {
      if (  (data[12].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.4868325)) {
            if (  (data[5].fvalue < 20.398617)) {
              sum += (float)-0.014132604;
            } else {
              sum += (float)-0.19268477;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.29587463;
            } else {
              sum += (float)-0.62743497;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.46965697)) {
              sum += (float)0.010558258;
            } else {
              sum += (float)-0.010486302;
            }
          } else {
            if (  (data[10].fvalue < 0.042482112)) {
              sum += (float)0.047245316;
            } else {
              sum += (float)-0.027613971;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 4.999999)) {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.0014706207;
            } else {
              sum += (float)0.071895443;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.12684202;
            } else {
              sum += (float)0.13775332;
            }
          }
        } else {
          if (  (data[15].fvalue < 3.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.01717893;
            } else {
              sum += (float)-0.0034204444;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.20082094;
            } else {
              sum += (float)0.028190317;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 6.999999)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.35723037;
            } else {
              sum += (float)-0.47381777;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.26586604;
            } else {
              sum += (float)-0.16024119;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.37330085;
            } else {
              sum += (float)-0.13362977;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.21706922;
            } else {
              sum += (float)0.025704931;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 8.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.57676315)) {
              sum += (float)-0.088742055;
            } else {
              sum += (float)0.10298583;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.055868477;
            } else {
              sum += (float)0.042625468;
            }
          }
        } else {
          if (  (data[12].fvalue < 11.999999)) {
            if (  (data[10].fvalue < 0.05896762)) {
              sum += (float)0.13134584;
            } else {
              sum += (float)0.034439877;
            }
          } else {
            if (  (data[12].fvalue < 17.999998)) {
              sum += (float)-0.020066941;
            } else {
              sum += (float)-0.24736509;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 6.999999)) {
      if (  (data[13].fvalue < 12.999999)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.30852273;
            } else {
              sum += (float)-0.42206171;
            }
          } else {
            if (  (data[10].fvalue < 0.68731475)) {
              sum += (float)-0.22915421;
            } else {
              sum += (float)0.080417134;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.063715734;
            } else {
              sum += (float)-0.12417774;
            }
          } else {
            if (  (data[10].fvalue < 0.55227232)) {
              sum += (float)-0.17051055;
            } else {
              sum += (float)0.030030891;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 16.999998)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.4667947;
            } else {
              sum += (float)-0.3485288;
            }
          } else {
            if (  (data[10].fvalue < 0.60690707)) {
              sum += (float)-0.23041882;
            } else {
              sum += (float)0.062065862;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.51578057;
            } else {
              sum += (float)-0.38671356;
            }
          } else {
            if (  (data[13].fvalue < 17.999998)) {
              sum += (float)-0.5262447;
            } else {
              sum += (float)-0.61360806;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 8.999999)) {
        if (  (data[13].fvalue < 15.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.033568498)) {
              sum += (float)0.10589794;
            } else {
              sum += (float)0.0089927251;
            }
          } else {
            if (  (data[10].fvalue < 0.60690707)) {
              sum += (float)-0.043234009;
            } else {
              sum += (float)0.12212406;
            }
          }
        } else {
          if (  (data[13].fvalue < 18.999998)) {
            if (  (data[10].fvalue < 0.4868325)) {
              sum += (float)-0.13100956;
            } else {
              sum += (float)0.094714962;
            }
          } else {
            if (  (data[7].fvalue < 0.99568236)) {
              sum += (float)-0.28443268;
            } else {
              sum += (float)-0.39000309;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 14.999999)) {
          if (  (data[12].fvalue < 9.999999)) {
            if (  (data[13].fvalue < 13.999999)) {
              sum += (float)0.059935588;
            } else {
              sum += (float)-0.062361125;
            }
          } else {
            if (  (data[10].fvalue < 0.0245544)) {
              sum += (float)0.19719411;
            } else {
              sum += (float)0.083992146;
            }
          }
        } else {
          if (  (data[12].fvalue < 21.999998)) {
            if (  (data[13].fvalue < 22.999998)) {
              sum += (float)-0.039809145;
            } else {
              sum += (float)0.46522209;
            }
          } else {
            sum += (float)0.63114238;
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 4.999999)) {
    if (  (data[19].fvalue < 6.331244)) {
      if (  (data[21].fvalue < 2.2545199)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.018209718)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.0050478065;
            } else {
              sum += (float)-0.45110866;
            }
          } else {
            if (  (data[19].fvalue < 0.088564493)) {
              sum += (float)0.015564143;
            } else {
              sum += (float)-0.00010907518;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 1.0457116)) {
              sum += (float)-0.037387524;
            } else {
              sum += (float)0.01044818;
            }
          } else {
            if (  (data[21].fvalue < 0.38597369)) {
              sum += (float)0.10454559;
            } else {
              sum += (float)0.026904628;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.65006936)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.022639008;
            } else {
              sum += (float)0.00073758687;
            }
          } else {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.032863542;
            } else {
              sum += (float)-0.12428232;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.18145521)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.16474707;
            } else {
              sum += (float)-0.071670711;
            }
          } else {
            if (  (data[5].fvalue < 24.348347)) {
              sum += (float)-0.041751567;
            } else {
              sum += (float)-0.0070349136;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[11].fvalue < 0.67161757)) {
          if (  (data[8].fvalue < 7.1927052)) {
            if (  (data[11].fvalue < 0.55883485)) {
              sum += (float)-0.030957954;
            } else {
              sum += (float)-0.10273616;
            }
          } else {
            if (  (data[5].fvalue < 29.812677)) {
              sum += (float)0.064435549;
            } else {
              sum += (float)-0.0080195237;
            }
          }
        } else {
          if (  (data[8].fvalue < 10.750333)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.30391699;
            } else {
              sum += (float)-0.1031912;
            }
          } else {
            if (  (data[11].fvalue < 0.92414558)) {
              sum += (float)-0.0057999073;
            } else {
              sum += (float)-0.13745409;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 2.7145009)) {
              sum += (float)0.0018976281;
            } else {
              sum += (float)-0.037061609;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.11154011;
            } else {
              sum += (float)0.10394165;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.6359784)) {
            sum += (float)0.33515802;
          } else {
            if (  (data[17].fvalue < 0.99999899)) {
              sum += (float)0.097286478;
            } else {
              sum += (float)-0.023985531;
            }
          }
        }
      }
    }
  } else {
    if (  (data[21].fvalue < 3.4868217)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.3681255)) {
            sum += (float)0.61140126;
          } else {
            sum += (float)0.44510311;
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 2.2545199)) {
              sum += (float)0.43883312;
            } else {
              sum += (float)0.31309769;
            }
          } else {
            sum += (float)0.19519401;
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 4.5423837)) {
              sum += (float)0.11177083;
            } else {
              sum += (float)-0.012288442;
            }
          } else {
            if (  (data[21].fvalue < 2.7145009)) {
              sum += (float)0.23865889;
            } else {
              sum += (float)0.076128006;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.61820936)) {
            if (  (data[5].fvalue < 29.812677)) {
              sum += (float)0.29165491;
            } else {
              sum += (float)0.39933586;
            }
          } else {
            if (  (data[21].fvalue < 1.1490582)) {
              sum += (float)0.3345513;
            } else {
              sum += (float)0.16943423;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.022306835;
            } else {
              sum += (float)0.088525258;
            }
          } else {
            if (  (data[8].fvalue < 4.5423837)) {
              sum += (float)0.21901278;
            } else {
              sum += (float)0.12903954;
            }
          }
        } else {
          if (  (data[8].fvalue < 20.203608)) {
            if (  (data[21].fvalue < 15.426888)) {
              sum += (float)-0.039017387;
            } else {
              sum += (float)-0.16585191;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.30501637;
            } else {
              sum += (float)0.15996046;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 11.808577)) {
            if (  (data[8].fvalue < 4.5423837)) {
              sum += (float)0.090805292;
            } else {
              sum += (float)-0.078705639;
            }
          } else {
            sum += (float)0.13752773;
          }
        } else {
          if (  (data[16].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.12798932)) {
              sum += (float)-0.3385911;
            } else {
              sum += (float)-0.14849785;
            }
          } else {
            if (  (data[7].fvalue < 0.98932296)) {
              sum += (float)0.10140843;
            } else {
              sum += (float)-0.049874146;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 1.1615946)) {
    if (  (data[18].fvalue < 0.038903255)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[19].fvalue < 0.042609002)) {
          if (  (data[9].fvalue < 1.0369114)) {
            if (  (data[9].fvalue < 0.99631774)) {
              sum += (float)-0.12461539;
            } else {
              sum += (float)-0.55522478;
            }
          } else {
            if (  (data[10].fvalue < 0.14481349)) {
              sum += (float)0.022963867;
            } else {
              sum += (float)0.28236014;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.016281471)) {
            if (  (data[19].fvalue < 0.16799387)) {
              sum += (float)-0.044788584;
            } else {
              sum += (float)0.015257659;
            }
          } else {
            if ( (data[9].fvalue < 1.0369114)) {
              sum += (float)0.14303425;
            } else {
              sum += (float)0.017378658;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.19657525)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.032364104)) {
              sum += (float)-0.42619255;
            } else {
              sum += (float)-0.54924655;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.7783488;
            } else {
              sum += (float)-0.57830375;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.50802165)) {
              sum += (float)-0.13099113;
            } else {
              sum += (float)0.28621808;
            }
          } else {
            sum += (float)-0.61712956;
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 0.68303591)) {
        if (  (data[9].fvalue < 0.99631774)) {
          if (  (data[10].fvalue < 0.41018176)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)0.36019072;
            } else {
              sum += (float)-0.21643093;
            }
          } else {
            if (  (data[19].fvalue < 0.018432997)) {
              sum += (float)0.075467043;
            } else {
              sum += (float)0.3005999;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.19657525)) {
            if (  (data[9].fvalue < 1.0369114)) {
              sum += (float)0.070043989;
            } else {
              sum += (float)-0.035689071;
            }
          } else {
            if (  (data[9].fvalue < 1.0950193)) {
              sum += (float)0.37835419;
            } else {
              sum += (float)0.094117872;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.33704871)) {
          if (  (data[18].fvalue < 0.23938002)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0077941418;
            } else {
              sum += (float)-0.19017597;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.038110591;
            } else {
              sum += (float)0.33785728;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.80929261)) {
            if (  (data[9].fvalue < 1.0950193)) {
              sum += (float)0.1737639;
            } else {
              sum += (float)-0.06328252;
            }
          } else {
            if (  (data[18].fvalue < 0.25735804)) {
              sum += (float)-0.057790805;
            } else {
              sum += (float)-0.15675999;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 0.084387004)) {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[20].fvalue < 0.018458318)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[19].fvalue < 0.61775768)) {
              sum += (float)-0.0013124164;
            } else {
              sum += (float)-0.018260133;
            }
          } else {
            if (  (data[9].fvalue < 2.6074083)) {
              sum += (float)0.37480882;
            } else {
              sum += (float)-0.022636617;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.0115252)) {
            if (  (data[19].fvalue < 0.032364104)) {
              sum += (float)0.28339618;
            } else {
              sum += (float)0.10836931;
            }
          } else {
            if (  (data[19].fvalue < 0.46072254)) {
              sum += (float)-0.039391238;
            } else {
              sum += (float)0.038609322;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[18].fvalue < 0.047849525)) {
            if (  (data[9].fvalue < 1.7441784)) {
              sum += (float)0.13016224;
            } else {
              sum += (float)0.027611284;
            }
          } else {
            if (  (data[19].fvalue < 0.018432997)) {
              sum += (float)0.046365045;
            } else {
              sum += (float)-0.021892045;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.43361986)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.012157171;
            } else {
              sum += (float)-0.13039584;
            }
          } else {
            if (  (data[9].fvalue < 1.3514162)) {
              sum += (float)0.21139295;
            } else {
              sum += (float)0.098458342;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 2.0115252)) {
            if (  (data[19].fvalue < 0.018432997)) {
              sum += (float)0.18715647;
            } else {
              sum += (float)0.026141029;
            }
          } else {
            if (  (data[19].fvalue < 0.018432997)) {
              sum += (float)-0.016888108;
            } else {
              sum += (float)0.013658413;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.8770646)) {
            if (  (data[18].fvalue < 0.047849525)) {
              sum += (float)-0.001045706;
            } else {
              sum += (float)-0.06636095;
            }
          } else {
            if (  (data[7].fvalue < 0.97170192)) {
              sum += (float)0.0085575366;
            } else {
              sum += (float)-0.0080978051;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 2.936173)) {
          if (  (data[9].fvalue < 1.9399837)) {
            if (  (data[18].fvalue < 0.18234436)) {
              sum += (float)-0.14613375;
            } else {
              sum += (float)0.031329948;
            }
          } else {
            if (  (data[10].fvalue < 0.48836178)) {
              sum += (float)-0.11420467;
            } else {
              sum += (float)-0.018332845;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.38628879)) {
            if (  (data[19].fvalue < 0.99077684)) {
              sum += (float)-0.039820313;
            } else {
              sum += (float)-0.11095398;
            }
          } else {
            if (  (data[20].fvalue < 1.708603)) {
              sum += (float)0.030804314;
            } else {
              sum += (float)-0.018524861;
            }
          }
        }
      }
    }
  }
  if (  (data[4].fvalue < 0.99999899)) {
    if (  (data[10].fvalue < 0.041918553)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[11].fvalue < 0.55859417)) {
          if (  (data[5].fvalue < 9.6632538)) {
            if (  (data[21].fvalue < 0.022120249)) {
              sum += (float)-0.085124522;
            } else {
              sum += (float)0.0192369;
            }
          } else {
            if (  (data[10].fvalue < 0.014985877)) {
              sum += (float)-0.048253942;
            } else {
              sum += (float)0.011735606;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 8.0724955)) {
              sum += (float)0.02132792;
            } else {
              sum += (float)-0.1078283;
            }
          } else {
            if (  (data[18].fvalue < 0.038697857)) {
              sum += (float)0.13591307;
            } else {
              sum += (float)0.042162098;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.74972445)) {
          if (  (data[18].fvalue < 0.038697857)) {
            if (  (data[18].fvalue < 0.028748915)) {
              sum += (float)-0.29217988;
            } else {
              sum += (float)-0.1483527;
            }
          } else {
            if (  (data[20].fvalue < 0.018826228)) {
              sum += (float)0.071436435;
            } else {
              sum += (float)0.026030164;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.014985877)) {
            if (  (data[18].fvalue < 0.36627933)) {
              sum += (float)0.10903518;
            } else {
              sum += (float)0.21838337;
            }
          } else {
            if (  (data[21].fvalue < 15.525005)) {
              sum += (float)0.10837654;
            } else {
              sum += (float)0.027070867;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.018826228)) {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[19].fvalue < 0.032811668)) {
            if (  (data[5].fvalue < 1.1979592)) {
              sum += (float)-0.12308215;
            } else {
              sum += (float)-0.0064811022;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.025159376;
            } else {
              sum += (float)-0.036845401;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.43240559)) {
              sum += (float)-0.099047132;
            } else {
              sum += (float)-0.21426776;
            }
          } else {
            if (  (data[18].fvalue < 0.38673833)) {
              sum += (float)-0.02404994;
            } else {
              sum += (float)0.04878293;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[21].fvalue < 0.097283363)) {
              sum += (float)-0.040342424;
            } else {
              sum += (float)-0.012290254;
            }
          } else {
            if (  (data[19].fvalue < 0.35440683)) {
              sum += (float)-0.03914123;
            } else {
              sum += (float)-0.19978733;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.91747051)) {
            if (  (data[11].fvalue < 0.53783673)) {
              sum += (float)0.0038913395;
            } else {
              sum += (float)0.041439265;
            }
          } else {
            if (  (data[10].fvalue < 0.57732612)) {
              sum += (float)-0.035264302;
            } else {
              sum += (float)0.063225985;
            }
          }
        }
      }
    }
  } else {
    if (  (data[19].fvalue < 2.4717243)) {
      if (  (data[18].fvalue < 1.1753943)) {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[10].fvalue < 0.57732612)) {
              sum += (float)0.0057965219;
            } else {
              sum += (float)-0.0077482932;
            }
          } else {
            if (  (data[19].fvalue < 0.68468118)) {
              sum += (float)0.027129935;
            } else {
              sum += (float)-0.16857897;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.48216459)) {
              sum += (float)-0.0041627456;
            } else {
              sum += (float)-0.14811563;
            }
          } else {
            if (  (data[18].fvalue < 0.12899579)) {
              sum += (float)-0.17100994;
            } else {
              sum += (float)0.051174257;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 4.999999)) {
          if (  (data[19].fvalue < 0.15303293)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0053969491;
            } else {
              sum += (float)-0.043729469;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.055154286;
            } else {
              sum += (float)0.0079069287;
            }
          }
        } else {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[13].fvalue < 7.999999)) {
              sum += (float)-0.067168258;
            } else {
              sum += (float)-0.24873661;
            }
          } else {
            if (  (data[19].fvalue < 0.64949208)) {
              sum += (float)0.061127137;
            } else {
              sum += (float)-0.01875508;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 1.0010307)) {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 11.592218)) {
              sum += (float)0.0035818231;
            } else {
              sum += (float)0.060049042;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)0.072078057;
            } else {
              sum += (float)-0.021416476;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[18].fvalue < 0.59682816)) {
              sum += (float)-0.36515012;
            } else {
              sum += (float)-0.14719996;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)0.036109097;
            } else {
              sum += (float)-0.11094242;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.59682816)) {
          if (  (data[5].fvalue < 26.592216)) {
            if (  (data[10].fvalue < 0.28653869)) {
              sum += (float)-0.13606693;
            } else {
              sum += (float)-0.029237527;
            }
          } else {
            if (  (data[11].fvalue < 0.3259207)) {
              sum += (float)-0.07183101;
            } else {
              sum += (float)-0.011959669;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 3.7269406)) {
              sum += (float)0.26183251;
            } else {
              sum += (float)0.087936126;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.15442252;
            } else {
              sum += (float)0.020820726;
            }
          }
        }
      }
    }
  }
  if (  (data[6].fvalue < 0.3687017)) {
    if (  (data[7].fvalue < 0.92220944)) {
      if (  (data[21].fvalue < 0.32273912)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.029246733)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.39965701;
            } else {
              sum += (float)0.014844077;
            }
          } else {
            if (  (data[18].fvalue < 0.038433183)) {
              sum += (float)0.20736325;
            } else {
              sum += (float)0.014592256;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.4322485)) {
              sum += (float)0.048202414;
            } else {
              sum += (float)0.0053865113;
            }
          } else {
            if (  (data[20].fvalue < 0.018453669)) {
              sum += (float)-0.022071101;
            } else {
              sum += (float)0.0076265959;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.79892844)) {
          if (  (data[18].fvalue < 0.016306445)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.24757826;
            } else {
              sum += (float)-0.057598397;
            }
          } else {
            if (  (data[20].fvalue < 0.83998334)) {
              sum += (float)-0.046240002;
            } else {
              sum += (float)-0.0064479806;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.89603812)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.04334699;
            } else {
              sum += (float)0.035122506;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.031774528;
            } else {
              sum += (float)0.12245185;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 0.36689317)) {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.99806261)) {
            if (  (data[18].fvalue < 0.016306445)) {
              sum += (float)-0.013787859;
            } else {
              sum += (float)0.14208893;
            }
          } else {
            sum += (float)-0.98453307;
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 0.022212591)) {
              sum += (float)0.28916281;
            } else {
              sum += (float)0.11720613;
            }
          } else {
            if (  (data[18].fvalue < 0.82314932)) {
              sum += (float)0.050549395;
            } else {
              sum += (float)0.35423031;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.98418868)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.070438907)) {
              sum += (float)-0.10224836;
            } else {
              sum += (float)0.037316311;
            }
          } else {
            if (  (data[21].fvalue < 3.7488036)) {
              sum += (float)0.30708292;
            } else {
              sum += (float)0.22823171;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99305779)) {
            if (  (data[20].fvalue < 7.8258786)) {
              sum += (float)0.44142878;
            } else {
              sum += (float)0.28750396;
            }
          } else {
            if (  (data[7].fvalue < 0.99886799)) {
              sum += (float)0.49495873;
            } else {
              sum += (float)0.578201;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 4.8318086)) {
      if (  (data[12].fvalue < 2.999999)) {
        if (  (data[18].fvalue < 0.34977028)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.00012050108;
            } else {
              sum += (float)-0.021606488;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.012064344;
            } else {
              sum += (float)0.044261198;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.015491166;
            } else {
              sum += (float)0.032277063;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.16095108;
            } else {
              sum += (float)0.0043849503;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.25719482)) {
          if (  (data[18].fvalue < 0.070438907)) {
            if (  (data[18].fvalue < 0.048793007)) {
              sum += (float)-0.5051136;
            } else {
              sum += (float)-0.23987503;
            }
          } else {
            if (  (data[13].fvalue < 6.999999)) {
              sum += (float)-0.075199053;
            } else {
              sum += (float)0.029409753;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.084276304)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.075994894;
            } else {
              sum += (float)0.025026755;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.049389504;
            } else {
              sum += (float)0.012742157;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.99999899)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 7.9811182)) {
            if (  (data[20].fvalue < 0.018453669)) {
              sum += (float)-0.070136838;
            } else {
              sum += (float)0.017723998;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.14840378;
            } else {
              sum += (float)-0.0402744;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 7.9811182)) {
              sum += (float)0.045594033;
            } else {
              sum += (float)0.16326658;
            }
          } else {
            if (  (data[21].fvalue < 0.74276161)) {
              sum += (float)-0.21759781;
            } else {
              sum += (float)-0.094223492;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0034550789;
            } else {
              sum += (float)-0.10473993;
            }
          } else {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)-0.017521322;
            } else {
              sum += (float)-0.096854255;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.35385659)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.14455879;
            } else {
              sum += (float)0.065764368;
            }
          } else {
            if (  (data[21].fvalue < 3.7488036)) {
              sum += (float)0.14299099;
            } else {
              sum += (float)-0.016455252;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.036607206)) {
    if (  (data[7].fvalue < 0.95656484)) {
      if (  (data[11].fvalue < 0.35373706)) {
        if (  (data[2].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.70849288)) {
            if (  (data[7].fvalue < 0.37007788)) {
              sum += (float)0.52547467;
            } else {
              sum += (float)0.39474997;
            }
          } else {
            if (  (data[11].fvalue < 0.11294933)) {
              sum += (float)0.4599328;
            } else {
              sum += (float)0.20078357;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.12233378)) {
            if (  (data[11].fvalue < 0.096460477)) {
              sum += (float)0.47946101;
            } else {
              sum += (float)0.3115772;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.21374577;
            } else {
              sum += (float)-0.0032295857;
            }
          }
        }
      } else {
        if (  (data[2].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.64599138)) {
            if (  (data[11].fvalue < 0.39807737)) {
              sum += (float)0.19601429;
            } else {
              sum += (float)0.084002681;
            }
          } else {
            if (  (data[7].fvalue < 0.37007788)) {
              sum += (float)-0.15960084;
            } else {
              sum += (float)0.093970224;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.6190061)) {
              sum += (float)0.010345086;
            } else {
              sum += (float)-0.13896857;
            }
          } else {
            if (  (data[7].fvalue < 0.37007788)) {
              sum += (float)-0.40539008;
            } else {
              sum += (float)-0.18493968;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.0089123379)) {
        if (  (data[5].fvalue < 6.5914807)) {
          if (  (data[4].fvalue < 0.99999899)) {
            sum += (float)0.38219231;
          } else {
            if (  (data[5].fvalue < 5.8259091)) {
              sum += (float)0.80839604;
            } else {
              sum += (float)0.61322451;
            }
          }
        } else {
          if (  (data[5].fvalue < 39.06284)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.042831518;
            } else {
              sum += (float)0.19757736;
            }
          } else {
            if (  (data[5].fvalue < 46.936485)) {
              sum += (float)0.044414304;
            } else {
              sum += (float)-0.016365202;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.060038045;
            } else {
              sum += (float)-0.0019356885;
            }
          } else {
            if (  (data[7].fvalue < 0.99994987)) {
              sum += (float)-0.071997896;
            } else {
              sum += (float)0.048593294;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.043545067;
            } else {
              sum += (float)-0.33214802;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.010732173;
            } else {
              sum += (float)0.18240002;
            }
          }
        }
      }
    }
  } else {
    if (  (data[22].fvalue < 15.019717)) {
      if (  (data[8].fvalue < 3.7580404)) {
        if (  (data[22].fvalue < 0.66629583)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.00097930245;
            } else {
              sum += (float)-0.089098215;
            }
          } else {
            if (  (data[22].fvalue < 0.066848226)) {
              sum += (float)-0.10490206;
            } else {
              sum += (float)0.02600011;
            }
          }
        } else {
          if (  (data[5].fvalue < 10.318744)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.068380088;
            } else {
              sum += (float)-0.0079498291;
            }
          } else {
            if (  (data[11].fvalue < 0.15023209)) {
              sum += (float)-0.023847884;
            } else {
              sum += (float)0.0010397055;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 11.09648)) {
          if (  (data[22].fvalue < 1.9378873)) {
            if (  (data[11].fvalue < 0.95888811)) {
              sum += (float)0.052012421;
            } else {
              sum += (float)-0.076896705;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.018028028;
            } else {
              sum += (float)-0.031961516;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 0.71417809)) {
              sum += (float)-0.0062435884;
            } else {
              sum += (float)0.0042196168;
            }
          } else {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)0.017553873;
            } else {
              sum += (float)0.144052;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.77171451)) {
        if (  (data[16].fvalue < 5.999999)) {
          if (  (data[8].fvalue < 10.759933)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)0.030981334;
            } else {
              sum += (float)-0.030743474;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.050314076;
            } else {
              sum += (float)0.0053614588;
            }
          }
        } else {
          if (  (data[5].fvalue < 36.007935)) {
            if (  (data[8].fvalue < 5.1406703)) {
              sum += (float)-0.37374273;
            } else {
              sum += (float)-0.20283519;
            }
          } else {
            if (  (data[11].fvalue < 0.64599138)) {
              sum += (float)-0.17928435;
            } else {
              sum += (float)-0.073728882;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 38.32542)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.020828946;
            } else {
              sum += (float)-0.064885624;
            }
          } else {
            if (  (data[5].fvalue < 25.808546)) {
              sum += (float)-0.099241167;
            } else {
              sum += (float)-0.03629978;
            }
          }
        } else {
          if (  (data[22].fvalue < 19.879824)) {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)0.022014463;
            } else {
              sum += (float)0.097250961;
            }
          } else {
            if (  (data[7].fvalue < 0.37007788)) {
              sum += (float)0.082622007;
            } else {
              sum += (float)-0.0079254769;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 6.999999)) {
    if (  (data[13].fvalue < 10.999999)) {
      if (  (data[13].fvalue < 7.999999)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.00081107067;
            } else {
              sum += (float)0.013782531;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.05094941;
            } else {
              sum += (float)0.11620709;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.045418374;
            } else {
              sum += (float)-0.062010776;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.022401817;
            } else {
              sum += (float)0.0016785961;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.18735108;
            } else {
              sum += (float)-0.10320619;
            }
          } else {
            if (  (data[22].fvalue < 3.9708512)) {
              sum += (float)0.1418183;
            } else {
              sum += (float)-0.059527211;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.0081325043;
            } else {
              sum += (float)0.064765289;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.075411938;
            } else {
              sum += (float)0.0012926433;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 14.999999)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 11.999999)) {
              sum += (float)-0.31013152;
            } else {
              sum += (float)-0.42458546;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.23622137;
            } else {
              sum += (float)0.0089964522;
            }
          }
        } else {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.034427598)) {
              sum += (float)0.15294831;
            } else {
              sum += (float)-0.12534994;
            }
          } else {
            if (  (data[22].fvalue < 3.0277331)) {
              sum += (float)-0.00044032407;
            } else {
              sum += (float)-0.089956351;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 17.999998)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.34848496;
            } else {
              sum += (float)-0.23590043;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.18853921;
            } else {
              sum += (float)-0.067692548;
            }
          }
        } else {
          if (  (data[6].fvalue < 17.036854)) {
            sum += (float)-0.48683608;
          } else {
            sum += (float)-0.36261714;
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 6.999999)) {
      if (  (data[15].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.33108324;
            } else {
              sum += (float)-0.16202769;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.40390027;
            } else {
              sum += (float)-0.26244858;
            }
          }
        } else {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[11].fvalue < 0.016164137)) {
              sum += (float)0.27154857;
            } else {
              sum += (float)-0.050156973;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.10030146;
            } else {
              sum += (float)-0.1852446;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 1.6955767)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 4.999999)) {
              sum += (float)-0.0035089764;
            } else {
              sum += (float)0.16648623;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.16029403;
            } else {
              sum += (float)0.035469111;
            }
          }
        } else {
          if (  (data[16].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.047967099;
            } else {
              sum += (float)-0.14052857;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.14553264;
            } else {
              sum += (float)-0.0518841;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 7.999999)) {
          if (  (data[11].fvalue < 0.049019948)) {
            if (  (data[5].fvalue < 22.799927)) {
              sum += (float)0.34100446;
            } else {
              sum += (float)0.078649752;
            }
          } else {
            if (  (data[12].fvalue < 9.999999)) {
              sum += (float)-0.044260923;
            } else {
              sum += (float)-0.14871345;
            }
          }
        } else {
          if (  (data[16].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 8.999999)) {
              sum += (float)0.0361524;
            } else {
              sum += (float)-0.026507528;
            }
          } else {
            if (  (data[22].fvalue < 5.2269311)) {
              sum += (float)0.10651769;
            } else {
              sum += (float)-0.0084111476;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 0.11089116)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)0.00061487686;
            } else {
              sum += (float)0.06494347;
            }
          } else {
            if (  (data[22].fvalue < 0.020475319)) {
              sum += (float)0.20513085;
            } else {
              sum += (float)0.10347849;
            }
          }
        } else {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)0.032101855;
            } else {
              sum += (float)-0.024000319;
            }
          } else {
            if (  (data[11].fvalue < 0.55932355)) {
              sum += (float)-0.091588818;
            } else {
              sum += (float)-0.0013428663;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 7.999999)) {
    if (  (data[13].fvalue < 9.999999)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.010349405;
            } else {
              sum += (float)-0.029153155;
            }
          } else {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.059727266;
            } else {
              sum += (float)0.017224928;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.43470266)) {
              sum += (float)-0.031409293;
            } else {
              sum += (float)-0.0047949334;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.037992623;
            } else {
              sum += (float)-0.0061017401;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.0099080037;
            } else {
              sum += (float)-0.098465726;
            }
          } else {
            if (  (data[11].fvalue < 0.21444537)) {
              sum += (float)-0.10630966;
            } else {
              sum += (float)-0.025304848;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.98020869)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.0011756085;
            } else {
              sum += (float)0.045817774;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.018468583;
            } else {
              sum += (float)0.041137539;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 1.0340142)) {
        if (  (data[18].fvalue < 0.20965533)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.32437238;
            } else {
              sum += (float)-0.054078724;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)0.33850271;
            } else {
              sum += (float)-0.11084056;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.239069;
            } else {
              sum += (float)-0.11063614;
            }
          } else {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.033789042;
            } else {
              sum += (float)0.035455063;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 6.999999)) {
          if (  (data[13].fvalue < 10.999999)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.14411749;
            } else {
              sum += (float)-0.23649077;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.25082213;
            } else {
              sum += (float)-0.34020036;
            }
          }
        } else {
          if (  (data[18].fvalue < 1.4221593)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.13123666;
            } else {
              sum += (float)-0.036796946;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.14692315;
            } else {
              sum += (float)-0.26480442;
            }
          }
        }
      }
    }
  } else {
    if (  (data[18].fvalue < 0.73640198)) {
      if (  (data[18].fvalue < 0.25767815)) {
        if (  (data[12].fvalue < 8.999999)) {
          if (  (data[18].fvalue < 0.20965533)) {
            if (  (data[18].fvalue < 0.18201105)) {
              sum += (float)-0.46580267;
            } else {
              sum += (float)-0.33564135;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)-0.10344244;
            } else {
              sum += (float)-0.22530194;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.20965533)) {
            sum += (float)-0.49848816;
          } else {
            if (  (data[13].fvalue < 10.999999)) {
              sum += (float)-0.39224675;
            } else {
              sum += (float)-0.49190298;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 11.999999)) {
          if (  (data[13].fvalue < 7.999999)) {
            if (  (data[11].fvalue < 0.063700274)) {
              sum += (float)0.049559753;
            } else {
              sum += (float)-0.11183447;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)0.012780862;
            } else {
              sum += (float)-0.068321347;
            }
          }
        } else {
          if (  (data[13].fvalue < 10.999999)) {
            if (  (data[8].fvalue < 2.4403687)) {
              sum += (float)-0.073895417;
            } else {
              sum += (float)-0.23388723;
            }
          } else {
            if (  (data[12].fvalue < 12.999999)) {
              sum += (float)-0.25022358;
            } else {
              sum += (float)-0.41663638;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 1.7736353)) {
        if (  (data[13].fvalue < 7.999999)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.052406736;
            } else {
              sum += (float)-0.093754113;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.09297882;
            } else {
              sum += (float)0.053568013;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.86821967)) {
            if (  (data[12].fvalue < 11.999999)) {
              sum += (float)0.012850595;
            } else {
              sum += (float)-0.18171383;
            }
          } else {
            if (  (data[12].fvalue < 15.999999)) {
              sum += (float)0.050396953;
            } else {
              sum += (float)-0.063883781;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.20159163)) {
          if (  (data[18].fvalue < 2.5401316)) {
            if (  (data[11].fvalue < 0.12346061)) {
              sum += (float)-0.20905745;
            } else {
              sum += (float)-0.0753216;
            }
          } else {
            if (  (data[8].fvalue < 3.5403483)) {
              sum += (float)-0.2427033;
            } else {
              sum += (float)-0.16281757;
            }
          }
        } else {
          if (  (data[5].fvalue < 13.41279)) {
            if (  (data[11].fvalue < 0.53830189)) {
              sum += (float)-0.19857129;
            } else {
              sum += (float)-0.094613038;
            }
          } else {
            if (  (data[11].fvalue < 0.74816686)) {
              sum += (float)-0.024354022;
            } else {
              sum += (float)0.02602165;
            }
          }
        }
      }
    }
  }
  if (  (data[18].fvalue < 0.016706392)) {
    if (  (data[19].fvalue < 0.054980662)) {
      if (  (data[10].fvalue < 0.15196475)) {
        if (  (data[20].fvalue < 1.3009425)) {
          if (  (data[17].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 1.4188898)) {
              sum += (float)-0.0049781138;
            } else {
              sum += (float)0.11952387;
            }
          } else {
            if (  (data[9].fvalue < 2.4602096)) {
              sum += (float)0.20223412;
            } else {
              sum += (float)-0.43788114;
            }
          }
        } else {
          if (  (data[9].fvalue < 2.768841)) {
            if (  (data[8].fvalue < 0.10938028)) {
              sum += (float)0.37670022;
            } else {
              sum += (float)0.84764582;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.1456769;
            } else {
              sum += (float)-0.13136233;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 2.2993462)) {
          if (  (data[9].fvalue < 1.0384835)) {
            if (  (data[9].fvalue < 0.99591762)) {
              sum += (float)-0.15444219;
            } else {
              sum += (float)-0.61561584;
            }
          } else {
            if ( (data[9].fvalue < 1.7475569)) {
              sum += (float)0.19632867;
            } else {
              sum += (float)-0.090512976;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.23462288)) {
            if (  (data[20].fvalue < 2.523237)) {
              sum += (float)-0.35362357;
            } else {
              sum += (float)-0.025371501;
            }
          } else {
            if (  (data[10].fvalue < 0.68353128)) {
              sum += (float)-0.52361041;
            } else {
              sum += (float)0.33216929;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 0.64345032)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 0.5593574)) {
            if (  (data[9].fvalue < 3.9774759)) {
              sum += (float)0.011634951;
            } else {
              sum += (float)-0.055141397;
            }
          } else {
            if (  (data[19].fvalue < 0.22360449)) {
              sum += (float)0.10783509;
            } else {
              sum += (float)-0.038287591;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.33608565)) {
            if (  (data[19].fvalue < 0.12769023)) {
              sum += (float)-0.36005068;
            } else {
              sum += (float)-0.092564151;
            }
          } else {
            if (  (data[20].fvalue < 0.5246563)) {
              sum += (float)0.22929065;
            } else {
              sum += (float)0.049163219;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 2.2993462)) {
          if (  (data[14].fvalue < 0.99999899)) {
            if (  (data[9].fvalue < 1.5833336)) {
              sum += (float)0.0018053342;
            } else {
              sum += (float)-0.24104312;
            }
          } else {
            if (  (data[19].fvalue < 0.9346903)) {
              sum += (float)-0.023306407;
            } else {
              sum += (float)0.11291904;
            }
          }
        } else {
          if (  (data[20].fvalue < 2.7278464)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.0026423724;
            } else {
              sum += (float)0.095947549;
            }
          } else {
            if (  (data[19].fvalue < 0.25484976)) {
              sum += (float)0.024002112;
            } else {
              sum += (float)-0.026782438;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.6872429)) {
      if (  (data[9].fvalue < 1.2206562)) {
        if (  (data[8].fvalue < 0.25087595)) {
          if (  (data[19].fvalue < 0.018893562)) {
            if (  (data[18].fvalue < 0.30816695)) {
              sum += (float)-0.0052765277;
            } else {
              sum += (float)0.45737141;
            }
          } else {
            if (  (data[9].fvalue < 1.0384835)) {
              sum += (float)0.23199101;
            } else {
              sum += (float)0.048660744;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.16756205)) {
            if (  (data[19].fvalue < 0.22360449)) {
              sum += (float)-0.10823421;
            } else {
              sum += (float)0.011557993;
            }
          } else {
            if (  (data[8].fvalue < 0.6059159)) {
              sum += (float)0.060830794;
            } else {
              sum += (float)-0.026955497;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.064429782)) {
          if (  (data[8].fvalue < 0.10938028)) {
            if (  (data[20].fvalue < 0.083767489)) {
              sum += (float)-0.035075806;
            } else {
              sum += (float)-0.19141644;
            }
          } else {
            if (  (data[18].fvalue < 0.14339542)) {
              sum += (float)0.11219364;
            } else {
              sum += (float)0.0048901597;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.7730265)) {
            if (  (data[10].fvalue < 0.12340834)) {
              sum += (float)-0.070685603;
            } else {
              sum += (float)-0.031904127;
            }
          } else {
            if (  (data[21].fvalue < 0.022552978)) {
              sum += (float)0.015248043;
            } else {
              sum += (float)0.12415473;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.029055402)) {
        if (  (data[19].fvalue < 0.16724572)) {
          if (  (data[21].fvalue < 1.2322513)) {
            if (  (data[10].fvalue < 0.3058764)) {
              sum += (float)0.25835547;
            } else {
              sum += (float)-0.14316656;
            }
          } else {
            if (  (data[19].fvalue < 0.018893562)) {
              sum += (float)0.78647667;
            } else {
              sum += (float)0.3404842;
            }
          }
        } else {
          if (  (data[19].fvalue < 1.2495638)) {
            if (  (data[10].fvalue < 0.33713153)) {
              sum += (float)-0.0020505644;
            } else {
              sum += (float)0.14436154;
            }
          } else {
            if (  (data[9].fvalue < 2.6865909)) {
              sum += (float)-0.0084899021;
            } else {
              sum += (float)-0.12718005;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.18114164)) {
          if (  (data[18].fvalue < 0.14339542)) {
            if (  (data[19].fvalue < 0.018893562)) {
              sum += (float)-0.024987223;
            } else {
              sum += (float)0.1559308;
            }
          } else {
            if (  (data[20].fvalue < 1.1356831)) {
              sum += (float)0.00046379858;
            } else {
              sum += (float)-0.018760609;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.48849645)) {
            if (  (data[18].fvalue < 0.16756205)) {
              sum += (float)-0.013063572;
            } else {
              sum += (float)0.01084741;
            }
          } else {
            if (  (data[19].fvalue < 3.2084117)) {
              sum += (float)0.044591829;
            } else {
              sum += (float)-0.0081767328;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 0.96435791)) {
    if (  (data[10].fvalue < 0.68450403)) {
      if (  (data[18].fvalue < 0.072157189)) {
        if (  (data[10].fvalue < 0.37104216)) {
          if (  (data[18].fvalue < 0.028438672)) {
            if (  (data[10].fvalue < 0.015100639)) {
              sum += (float)0.12062366;
            } else {
              sum += (float)0.73818541;
            }
          } else {
            sum += (float)-0.5736177;
          }
        } else {
          if (  (data[18].fvalue < 0.039395262)) {
            if (  (data[11].fvalue < 0.010723008)) {
              sum += (float)0.35487005;
            } else {
              sum += (float)0.20804554;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.65999866;
            } else {
              sum += (float)0.37633094;
            }
          }
        }
      } else {
        sum += (float)-1.0635521;
      }
    } else {
      if (  (data[18].fvalue < 0.049117256)) {
        if (  (data[7].fvalue < 0.99989551)) {
          if (  (data[5].fvalue < 5.088562)) {
            if (  (data[11].fvalue < 0.016544593)) {
              sum += (float)0.4442113;
            } else {
              sum += (float)-0.0025957192;
            }
          } else {
            if (  (data[11].fvalue < 0.21363224)) {
              sum += (float)-0.028793095;
            } else {
              sum += (float)0.15128021;
            }
          }
        } else {
          if (  (data[5].fvalue < 30.585463)) {
            if (  (data[5].fvalue < 21.987492)) {
              sum += (float)0.78342229;
            } else {
              sum += (float)0.31877095;
            }
          } else {
            if (  (data[10].fvalue < 1.0994481)) {
              sum += (float)-0.013978527;
            } else {
              sum += (float)0.103045;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.80717093)) {
          if (  (data[18].fvalue < 0.072157189)) {
            if (  (data[10].fvalue < 0.74292201)) {
              sum += (float)0.38022947;
            } else {
              sum += (float)0.080697052;
            }
          } else {
            if (  (data[18].fvalue < 0.09366636)) {
              sum += (float)0.39953384;
            } else {
              sum += (float)0.67483592;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.029638065)) {
            if (  (data[5].fvalue < 5.088562)) {
              sum += (float)0.34661466;
            } else {
              sum += (float)0.058078669;
            }
          } else {
            if (  (data[5].fvalue < 2.8521614)) {
              sum += (float)0.058260739;
            } else {
              sum += (float)-0.026908845;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.1635885)) {
      if (  (data[18].fvalue < 0.11741082)) {
        if (  (data[20].fvalue < 0.043060165)) {
          if (  (data[10].fvalue < 0.2265785)) {
            if (  (data[9].fvalue < 0.99724364)) {
              sum += (float)0.10612776;
            } else {
              sum += (float)-0.040096201;
            }
          } else {
            if (  (data[10].fvalue < 0.90889537)) {
              sum += (float)-0.05979367;
            } else {
              sum += (float)0.012738129;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.64191163)) {
            if (  (data[9].fvalue < 1.1013314)) {
              sum += (float)0.48413438;
            } else {
              sum += (float)0.19901344;
            }
          } else {
            if (  (data[9].fvalue < 1.1013314)) {
              sum += (float)0.0078299567;
            } else {
              sum += (float)-0.06999509;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.74292201)) {
          if (  (data[9].fvalue < 1.0405456)) {
            if (  (data[10].fvalue < 0.50535476)) {
              sum += (float)0.42282385;
            } else {
              sum += (float)0.098329775;
            }
          } else {
            if (  (data[18].fvalue < 0.43272451)) {
              sum += (float)-0.021903681;
            } else {
              sum += (float)0.11975814;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.43272451)) {
            if (  (data[10].fvalue < 0.90889537)) {
              sum += (float)-0.033593711;
            } else {
              sum += (float)-0.12516901;
            }
          } else {
            if (  (data[11].fvalue < 0.029638065)) {
              sum += (float)0.1040468;
            } else {
              sum += (float)0.013591722;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.99999899)) {
        if (  (data[9].fvalue < 2.4541643)) {
          if (  (data[20].fvalue < 0.91889149)) {
            if (  (data[18].fvalue < 0.016547546)) {
              sum += (float)0.015170273;
            } else {
              sum += (float)-0.016281741;
            }
          } else {
            if (  (data[10].fvalue < 0.041855667)) {
              sum += (float)0.12157971;
            } else {
              sum += (float)-0.077192515;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99686342)) {
            if (  (data[20].fvalue < 0.72489744)) {
              sum += (float)-0.013467513;
            } else {
              sum += (float)0.0023578545;
            }
          } else {
            if (  (data[20].fvalue < 0.02165642)) {
              sum += (float)0.026439041;
            } else {
              sum += (float)0.002958498;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.022525741)) {
          if (  (data[5].fvalue < 29.792603)) {
            if (  (data[18].fvalue < 0.38653862)) {
              sum += (float)0.0066027264;
            } else {
              sum += (float)0.29190841;
            }
          } else {
            if (  (data[18].fvalue < 0.25678316)) {
              sum += (float)-0.020977948;
            } else {
              sum += (float)-0.084882721;
            }
          }
        } else {
          if (  (data[20].fvalue < 3.7631497)) {
            if (  (data[10].fvalue < 0.058875255)) {
              sum += (float)-0.0040533398;
            } else {
              sum += (float)0.005112581;
            }
          } else {
            if (  (data[5].fvalue < 36.798824)) {
              sum += (float)-0.012482713;
            } else {
              sum += (float)0.0054957056;
            }
          }
        }
      }
    }
  }
  if (  (data[20].fvalue < 13.309873)) {
    if (  (data[14].fvalue < 3.999999)) {
      if (  (data[21].fvalue < 9.6862888)) {
        if (  (data[8].fvalue < 9.146081)) {
          if (  (data[8].fvalue < 0.111774)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.021278881;
            } else {
              sum += (float)-0.02615411;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.00071758474;
            } else {
              sum += (float)-0.020681417;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.75720793)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.016154312;
            } else {
              sum += (float)0.10579789;
            }
          } else {
            if (  (data[18].fvalue < 0.017790124)) {
              sum += (float)0.0023284187;
            } else {
              sum += (float)0.021092312;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.12990724)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 4.852879)) {
              sum += (float)0.071683593;
            } else {
              sum += (float)-0.14767137;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.094584152;
            } else {
              sum += (float)-0.042335004;
            }
          }
        } else {
          if (  (data[6].fvalue < 19.654387)) {
            if (  (data[8].fvalue < 0.25277138)) {
              sum += (float)0.13203897;
            } else {
              sum += (float)-0.013096539;
            }
          } else {
            if (  (data[20].fvalue < 1.1052734)) {
              sum += (float)-0.019417882;
            } else {
              sum += (float)-0.059497565;
            }
          }
        }
      }
    } else {
      if (  (data[0].fvalue < 0.99999899)) {
        if (  (data[20].fvalue < 4.016624)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[21].fvalue < 3.7515819)) {
              sum += (float)0.4723224;
            } else {
              sum += (float)0.28587013;
            }
          } else {
            if (  (data[20].fvalue < 0.75720793)) {
              sum += (float)0.42768961;
            } else {
              sum += (float)0.16647214;
            }
          }
        } else {
          if (  (data[21].fvalue < 7.166079)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.15979345;
            } else {
              sum += (float)0.012421178;
            }
          } else {
            if (  (data[20].fvalue < 7.8885512)) {
              sum += (float)0.089731708;
            } else {
              sum += (float)0.003779273;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[20].fvalue < 3.5425069)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.037518959;
            } else {
              sum += (float)0.17150792;
            }
          } else {
            if (  (data[21].fvalue < 5.3628874)) {
              sum += (float)0.060609769;
            } else {
              sum += (float)-0.012163528;
            }
          }
        } else {
          if (  (data[8].fvalue < 17.324043)) {
            if (  (data[21].fvalue < 0.021426585)) {
              sum += (float)0.18235794;
            } else {
              sum += (float)-0.056688473;
            }
          } else {
            if (  (data[20].fvalue < 7.8885512)) {
              sum += (float)0.14394842;
            } else {
              sum += (float)0.018854374;
            }
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[16].fvalue < 1.999999)) {
        if (  (data[21].fvalue < 0.062141281)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.048578758)) {
              sum += (float)0.26586542;
            } else {
              sum += (float)0.065594427;
            }
          } else {
            if (  (data[6].fvalue < 21.946344)) {
              sum += (float)0.016203851;
            } else {
              sum += (float)0.10363612;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.16755216)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)0.033889126;
            } else {
              sum += (float)-0.031504385;
            }
          } else {
            if (  (data[6].fvalue < 25.155666)) {
              sum += (float)-0.018125819;
            } else {
              sum += (float)-0.12332545;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 0.081180856)) {
            if (  (data[21].fvalue < 0.021426585)) {
              sum += (float)-0.23661819;
            } else {
              sum += (float)-0.053059835;
            }
          } else {
            if (  (data[21].fvalue < 0.096605726)) {
              sum += (float)0.0065177432;
            } else {
              sum += (float)-0.075554505;
            }
          }
        } else {
          if (  (data[21].fvalue < 5.6008639)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.021831643;
            } else {
              sum += (float)0.022013791;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.025659777;
            } else {
              sum += (float)-0.055093478;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 2.999999)) {
        if (  (data[0].fvalue < 0.99999899)) {
          if (  (data[18].fvalue < 0.87079614)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.01469136;
            } else {
              sum += (float)-0.10675867;
            }
          } else {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.18246526;
            } else {
              sum += (float)-0.073570579;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.36708072)) {
            if (  (data[18].fvalue < 0.14330928)) {
              sum += (float)-0.20280467;
            } else {
              sum += (float)-0.085088834;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.044160273;
            } else {
              sum += (float)-0.01807777;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 3.999999)) {
              sum += (float)-0.0804663;
            } else {
              sum += (float)0.041621897;
            }
          } else {
            if (  (data[21].fvalue < 2.2531927)) {
              sum += (float)-0.2094797;
            } else {
              sum += (float)-0.14110728;
            }
          }
        } else {
          if (  (data[14].fvalue < 3.999999)) {
            if (  (data[8].fvalue < 0.90810233)) {
              sum += (float)0.093815908;
            } else {
              sum += (float)-0.06891498;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.035639122;
            } else {
              sum += (float)-0.13840811;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 5.999999)) {
    if (  (data[13].fvalue < 1.999999)) {
      if (  (data[21].fvalue < 0.022046771)) {
        if (  (data[19].fvalue < 0.50835609)) {
          if (  (data[19].fvalue < 0.020104311)) {
            if ( (data[9].fvalue < 1.101844)) {
              sum += (float)-0.051920142;
            } else {
              sum += (float)-0.010089937;
            }
          } else {
            if ( (data[9].fvalue < 1.3511282)) {
              sum += (float)-0.00043644526;
            } else {
              sum += (float)0.049629621;
            }
          }
        } else {
          if ( (data[9].fvalue < 3.027976)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.073784821;
            } else {
              sum += (float)0.11959471;
            }
          } else {
            if (  (data[9].fvalue < 5.443686)) {
              sum += (float)0.031612806;
            } else {
              sum += (float)-0.042142846;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 1.1370251)) {
          if (  (data[19].fvalue < 0.8364771)) {
            if (  (data[19].fvalue < 0.020104311)) {
              sum += (float)0.015070484;
            } else {
              sum += (float)-0.013448203;
            }
          } else {
            if (  (data[21].fvalue < 0.50275981)) {
              sum += (float)0.11683403;
            } else {
              sum += (float)0.017383678;
            }
          }
        } else {
          if ( (data[9].fvalue < 2.4580235)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.048650756;
            } else {
              sum += (float)-0.32983524;
            }
          } else {
            if (  (data[19].fvalue < 0.6491614)) {
              sum += (float)-0.0013227917;
            } else {
              sum += (float)-0.020016735;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.043268178)) {
        if (  (data[13].fvalue < 2.999999)) {
          if (  (data[9].fvalue < 1.101844)) {
            if (  (data[8].fvalue < 0.077777445)) {
              sum += (float)-0.091359071;
            } else {
              sum += (float)-0.33267742;
            }
          } else {
            if (  (data[19].fvalue < 0.020104311)) {
              sum += (float)-0.25984061;
            } else {
              sum += (float)0.022233991;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.032272194)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.59444469;
            } else {
              sum += (float)-0.53040159;
            }
          } else {
            if (  (data[9].fvalue < 1.3511282)) {
              sum += (float)-0.52616501;
            } else {
              sum += (float)-0.33010864;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.101844)) {
          if (  (data[19].fvalue < 0.11540537)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.00229362;
            } else {
              sum += (float)-0.21453153;
            }
          } else {
            if (  (data[8].fvalue < 0.45582426)) {
              sum += (float)0.24569854;
            } else {
              sum += (float)0.0011838143;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.088859938)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)0.11033317;
            } else {
              sum += (float)-0.30082014;
            }
          } else {
            if (  (data[19].fvalue < 0.2251451)) {
              sum += (float)-0.02541404;
            } else {
              sum += (float)0.0030137999;
            }
          }
        }
      }
    }
  } else {
    if (  (data[20].fvalue < 10.541907)) {
      if (  (data[20].fvalue < 6.5216646)) {
        if (  (data[13].fvalue < 2.999999)) {
          sum += (float)0.45298201;
        } else {
          if (  (data[13].fvalue < 4.999999)) {
            sum += (float)0.25673574;
          } else {
            if (  (data[19].fvalue < 1.0967565)) {
              sum += (float)0.50006109;
            } else {
              sum += (float)0.36441842;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 6.999999)) {
          if (  (data[13].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.20659319;
            } else {
              sum += (float)0.083090976;
            }
          } else {
            if (  (data[19].fvalue < 1.9216294)) {
              sum += (float)0.35938293;
            } else {
              sum += (float)0.13230339;
            }
          }
        } else {
          sum += (float)0.42597345;
        }
      }
    } else {
      if (  (data[19].fvalue < 0.88313997)) {
        if (  (data[21].fvalue < 6.1917901)) {
          sum += (float)0.044604305;
        } else {
          if (  (data[19].fvalue < 0.39668006)) {
            sum += (float)-0.17162791;
          } else {
            sum += (float)-0.071909085;
          }
        }
      } else {
        if (  (data[14].fvalue < 6.999999)) {
          if (  (data[20].fvalue < 20.224874)) {
            if (  (data[8].fvalue < 20.244755)) {
              sum += (float)0.076593377;
            } else {
              sum += (float)0.2391583;
            }
          } else {
            sum += (float)-0.072726838;
          }
        } else {
          sum += (float)0.31614959;
        }
      }
    }
  }
  if (  (data[22].fvalue < 4.7997904)) {
    if (  (data[16].fvalue < 3.999999)) {
      if (  (data[19].fvalue < 0.16886845)) {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.071261331)) {
              sum += (float)-0.059533466;
            } else {
              sum += (float)0.0070192562;
            }
          } else {
            if (  (data[18].fvalue < 0.15583389)) {
              sum += (float)0.01761278;
            } else {
              sum += (float)-0.073152475;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.11830171)) {
            if (  (data[18].fvalue < 0.059012283)) {
              sum += (float)-0.48470154;
            } else {
              sum += (float)-0.20834856;
            }
          } else {
            if (  (data[18].fvalue < 0.32839385)) {
              sum += (float)-0.028195346;
            } else {
              sum += (float)0.072262421;
            }
          }
        }
      } else {
        if (  (data[22].fvalue < 2.0571845)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[22].fvalue < 1.1260172)) {
              sum += (float)0.0034451149;
            } else {
              sum += (float)-0.023220476;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.092569545;
            } else {
              sum += (float)-0.001105419;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.52615494)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.049985606;
            } else {
              sum += (float)0.057053089;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.0094284248;
            } else {
              sum += (float)-0.040532533;
            }
          }
        }
      }
    } else {
      if (  (data[18].fvalue < 0.3674185)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[19].fvalue < 0.1150075)) {
            if (  (data[18].fvalue < 0.094072483)) {
              sum += (float)-0.52782959;
            } else {
              sum += (float)-0.18342458;
            }
          } else {
            if (  (data[18].fvalue < 0.19764818)) {
              sum += (float)0.040917888;
            } else {
              sum += (float)0.28859642;
            }
          }
        } else {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.19764818)) {
              sum += (float)-0.32537183;
            } else {
              sum += (float)-0.15107378;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.040292792;
            } else {
              sum += (float)-0.047894146;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 5.999999)) {
          if (  (data[19].fvalue < 0.35616878)) {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)-0.0018405919;
            } else {
              sum += (float)0.30685458;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.410676;
            } else {
              sum += (float)0.095843509;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.75425643)) {
            if (  (data[12].fvalue < 5.999999)) {
              sum += (float)-0.24753977;
            } else {
              sum += (float)-0.019599961;
            }
          } else {
            if (  (data[20].fvalue < 2.2258019)) {
              sum += (float)0.048949949;
            } else {
              sum += (float)-0.057878409;
            }
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 1.999999)) {
      if (  (data[17].fvalue < 0.99999899)) {
        if (  (data[18].fvalue < 0.13016486)) {
          if (  (data[19].fvalue < 0.12920356)) {
            if (  (data[20].fvalue < 0.018594008)) {
              sum += (float)0.28035295;
            } else {
              sum += (float)0.17879255;
            }
          } else {
            if (  (data[19].fvalue < 0.98908532)) {
              sum += (float)0.064232685;
            } else {
              sum += (float)-0.0084790578;
            }
          }
        } else {
          if (  (data[20].fvalue < 3.0799243)) {
            if (  (data[19].fvalue < 0.33702299)) {
              sum += (float)0.012217905;
            } else {
              sum += (float)-0.033150151;
            }
          } else {
            if (  (data[22].fvalue < 15.028989)) {
              sum += (float)0.045044683;
            } else {
              sum += (float)-0.0076577533;
            }
          }
        }
      } else {
        if (  (data[18].fvalue < 0.86886305)) {
          if (  (data[19].fvalue < 0.5319134)) {
            if (  (data[19].fvalue < 0.06634941)) {
              sum += (float)-0.09745723;
            } else {
              sum += (float)-0.23678598;
            }
          } else {
            if (  (data[18].fvalue < 0.16926789)) {
              sum += (float)-0.070573181;
            } else {
              sum += (float)-0.16042224;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[19].fvalue < 0.55776525)) {
              sum += (float)0.018962888;
            } else {
              sum += (float)-0.13328472;
            }
          } else {
            if (  (data[17].fvalue < 1.999999)) {
              sum += (float)-0.14165375;
            } else {
              sum += (float)0.019664114;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 4.999999)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[12].fvalue < 0.99999899)) {
            sum += (float)-0.52692795;
          } else {
            if (  (data[20].fvalue < 0.018594008)) {
              sum += (float)-0.056295581;
            } else {
              sum += (float)-0.01612222;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.028602585)) {
            if (  (data[19].fvalue < 0.01989343)) {
              sum += (float)0.49296749;
            } else {
              sum += (float)-0.019136205;
            }
          } else {
            if (  (data[19].fvalue < 1.0992413)) {
              sum += (float)0.0080044875;
            } else {
              sum += (float)-0.022307979;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.50897908)) {
          if (  (data[12].fvalue < 5.999999)) {
            if (  (data[18].fvalue < 0.77543128)) {
              sum += (float)-0.11261961;
            } else {
              sum += (float)-0.22755402;
            }
          } else {
            if (  (data[12].fvalue < 8.999999)) {
              sum += (float)-0.0086666923;
            } else {
              sum += (float)0.1428085;
            }
          }
        } else {
          if (  (data[20].fvalue < 1.6216491)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.026741952;
            } else {
              sum += (float)0.021091318;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.037995014;
            } else {
              sum += (float)-0.11005634;
            }
          }
        }
      }
    }
  }
  if (  (data[9].fvalue < 0.96303463)) {
    if (  (data[18].fvalue < 0.34837839)) {
      if (  (data[19].fvalue < 0.02005025)) {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[18].fvalue < 0.016600475)) {
              sum += (float)-0.47721678;
            } else {
              sum += (float)-0.071558505;
            }
          } else {
            if (  (data[18].fvalue < 0.029177472)) {
              sum += (float)0.077475041;
            } else {
              sum += (float)-0.025808914;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.048667002)) {
            if (  (data[18].fvalue < 0.016600475)) {
              sum += (float)-0.53282398;
            } else {
              sum += (float)-0.27214825;
            }
          } else {
            if (  (data[18].fvalue < 0.094246313)) {
              sum += (float)0.058661986;
            } else {
              sum += (float)0.21940312;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.21034005)) {
            if (  (data[7].fvalue < 0.99989468)) {
              sum += (float)-0.034805033;
            } else {
              sum += (float)0.07652504;
            }
          } else {
            if (  (data[7].fvalue < 0.99984473)) {
              sum += (float)0.046868429;
            } else {
              sum += (float)0.19759099;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.22555813)) {
              sum += (float)0.14716557;
            } else {
              sum += (float)-0.0097728958;
            }
          } else {
            if (  (data[18].fvalue < 0.059009332)) {
              sum += (float)-0.23756818;
            } else {
              sum += (float)0.044268977;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.055788483)) {
        if (  (data[18].fvalue < 1.0918875)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.70087242)) {
              sum += (float)0.065779217;
            } else {
              sum += (float)0.14208393;
            }
          } else {
            if (  (data[21].fvalue < 0.022364985)) {
              sum += (float)0.27152082;
            } else {
              sum += (float)-0.16222483;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.0323002)) {
              sum += (float)0.24823146;
            } else {
              sum += (float)-0.14719471;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.13854334;
            } else {
              sum += (float)-0.22950226;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 4.999999)) {
          if (  (data[18].fvalue < 0.70087242)) {
            if (  (data[19].fvalue < 0.65018678)) {
              sum += (float)-0.069472328;
            } else {
              sum += (float)-0.2597439;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.34825942;
            } else {
              sum += (float)-0.13280852;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.30347037)) {
            if (  (data[12].fvalue < 7.999999)) {
              sum += (float)0.25332889;
            } else {
              sum += (float)0.82949376;
            }
          } else {
            if (  (data[19].fvalue < 0.8354395)) {
              sum += (float)0.10532311;
            } else {
              sum += (float)-0.10271706;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.0980483)) {
      if (  (data[19].fvalue < 0.16898331)) {
        if (  (data[18].fvalue < 0.1437376)) {
          if ( (data[9].fvalue < 0.99649566)) {
            if (  (data[19].fvalue < 0.043799128)) {
              sum += (float)-0.066597;
            } else {
              sum += (float)0.036051743;
            }
          } else {
            if (  (data[21].fvalue < 0.022364985)) {
              sum += (float)-0.15456916;
            } else {
              sum += (float)0.31817254;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[19].fvalue < 0.02005025)) {
              sum += (float)-0.030141324;
            } else {
              sum += (float)0.13137154;
            }
          } else {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.18915573;
            } else {
              sum += (float)-0.098727047;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.24109381)) {
            if (  (data[9].fvalue < 1.0388603)) {
              sum += (float)0.12418465;
            } else {
              sum += (float)-0.13245095;
            }
          } else {
            if (  (data[7].fvalue < 0.99979448)) {
              sum += (float)0.10352085;
            } else {
              sum += (float)0.25143889;
            }
          }
        } else {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[18].fvalue < 0.4328163)) {
              sum += (float)0.020079516;
            } else {
              sum += (float)-0.15580887;
            }
          } else {
            if (  (data[7].fvalue < 0.99886048)) {
              sum += (float)0.055991419;
            } else {
              sum += (float)0.1902522;
            }
          }
        }
      }
    } else {
      if (  (data[9].fvalue < 6.8847795)) {
        if (  (data[18].fvalue < 4.0799303)) {
          if (  (data[21].fvalue < 1.4200363)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.00012376122;
            } else {
              sum += (float)0.015729593;
            }
          } else {
            if (  (data[9].fvalue < 1.9455937)) {
              sum += (float)-0.080605648;
            } else {
              sum += (float)-0.0030584896;
            }
          }
        } else {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[9].fvalue < 3.0320756)) {
              sum += (float)0.06407278;
            } else {
              sum += (float)-0.0073608733;
            }
          } else {
            if (  (data[9].fvalue < 4.7149205)) {
              sum += (float)-0.09467411;
            } else {
              sum += (float)-0.020647608;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.74798554)) {
          if (  (data[19].fvalue < 2.814862)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.044404481;
            } else {
              sum += (float)0.055039991;
            }
          } else {
            if (  (data[19].fvalue < 6.4081788)) {
              sum += (float)0.084195189;
            } else {
              sum += (float)-0.0089019975;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.98927432)) {
            if (  (data[21].fvalue < 15.476667)) {
              sum += (float)0.012498537;
            } else {
              sum += (float)-0.01398257;
            }
          } else {
            if (  (data[18].fvalue < 1.4293518)) {
              sum += (float)0.02206366;
            } else {
              sum += (float)0.069483787;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.11230718)) {
    if (  (data[9].fvalue < 0.96369779)) {
      if (  (data[18].fvalue < 0.082504556)) {
        if (  (data[18].fvalue < 0.039324809)) {
          if (  (data[8].fvalue < 0.069928214)) {
            if (  (data[18].fvalue < 0.017649338)) {
              sum += (float)0.32649717;
            } else {
              sum += (float)0.52424657;
            }
          } else {
            if (  (data[7].fvalue < 0.70225954)) {
              sum += (float)0.01945376;
            } else {
              sum += (float)0.25151631;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.037395548)) {
            sum += (float)-1.6114038;
          } else {
            if (  (data[8].fvalue < 0.069928214)) {
              sum += (float)0.27209869;
            } else {
              sum += (float)0.61142117;
            }
          }
        }
      } else {
        sum += (float)-0.78600943;
      }
    } else {
      if (  (data[9].fvalue < 1.0400038)) {
        if (  (data[18].fvalue < 0.071355388)) {
          if (  (data[8].fvalue < 0.069928214)) {
            if (  (data[18].fvalue < 0.049286891)) {
              sum += (float)0.066659562;
            } else {
              sum += (float)0.21536724;
            }
          } else {
            if (  (data[6].fvalue < 0.39408478)) {
              sum += (float)-0.14737955;
            } else {
              sum += (float)-0.049046315;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99765539)) {
            if (  (data[8].fvalue < 0.037395548)) {
              sum += (float)-0.073316686;
            } else {
              sum += (float)0.67515761;
            }
          } else {
            if (  (data[18].fvalue < 0.10667758)) {
              sum += (float)0.1531712;
            } else {
              sum += (float)0.62291783;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 1.0991012)) {
          if (  (data[18].fvalue < 0.15626054)) {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)-0.071475014;
            } else {
              sum += (float)-0.37231487;
            }
          } else {
            if (  (data[18].fvalue < 0.18268673)) {
              sum += (float)0.34891436;
            } else {
              sum += (float)0.74228108;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.2213483)) {
            if (  (data[18].fvalue < 0.24150757)) {
              sum += (float)0.017850086;
            } else {
              sum += (float)0.34754819;
            }
          } else {
            if (  (data[9].fvalue < 1.9476184)) {
              sum += (float)-0.028436979;
            } else {
              sum += (float)0.030249916;
            }
          }
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 1.0991012)) {
      if (  (data[18].fvalue < 0.13007323)) {
        if ( (data[9].fvalue < 0.91251773)) {
          if (  (data[8].fvalue < 0.25193653)) {
            if (  (data[18].fvalue < 0.049286891)) {
              sum += (float)0.23842694;
            } else {
              sum += (float)0.59540409;
            }
          } else {
            if (  (data[8].fvalue < 0.37379488)) {
              sum += (float)0.07494805;
            } else {
              sum += (float)-0.054129209;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.020931562)) {
            if (  (data[8].fvalue < 0.14926055)) {
              sum += (float)0.0032847428;
            } else {
              sum += (float)-0.075128905;
            }
          } else {
            if (  (data[8].fvalue < 0.66143817)) {
              sum += (float)0.43735749;
            } else {
              sum += (float)0.02431008;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.50204939)) {
          if (  (data[9].fvalue < 0.91251773)) {
            if (  (data[8].fvalue < 0.4192878)) {
              sum += (float)0.65745986;
            } else {
              sum += (float)0.28524467;
            }
          } else {
            if (  (data[18].fvalue < 0.22629684)) {
              sum += (float)0.030123789;
            } else {
              sum += (float)0.18125737;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.30864459)) {
            if (  (data[8].fvalue < 0.71232235)) {
              sum += (float)-0.049802724;
            } else {
              sum += (float)-0.13727248;
            }
          } else {
            if (  (data[8].fvalue < 0.90851462)) {
              sum += (float)0.10009309;
            } else {
              sum += (float)-0.015159882;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 44.626953)) {
        if (  (data[22].fvalue < 4.829803)) {
          if (  (data[8].fvalue < 1.9110547)) {
            if (  (data[18].fvalue < 0.27434647)) {
              sum += (float)0.0041984557;
            } else {
              sum += (float)-0.011797888;
            }
          } else {
            if (  (data[9].fvalue < 6.521883)) {
              sum += (float)0.0048468225;
            } else {
              sum += (float)-0.01583143;
            }
          }
        } else {
          if (  (data[9].fvalue < 4.5806665)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.0031398444;
            } else {
              sum += (float)-0.041766871;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.013832773;
            } else {
              sum += (float)-0.012629248;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[22].fvalue < 1.2138237)) {
            if (  (data[6].fvalue < 22.708704)) {
              sum += (float)-0.034211762;
            } else {
              sum += (float)0.0027207052;
            }
          } else {
            if (  (data[9].fvalue < 2.5314074)) {
              sum += (float)-0.057048734;
            } else {
              sum += (float)0.012744956;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99916726)) {
            if (  (data[22].fvalue < 2.5102794)) {
              sum += (float)0.043869492;
            } else {
              sum += (float)0.018707516;
            }
          } else {
            if (  (data[9].fvalue < 2.6143942)) {
              sum += (float)-0.176153;
            } else {
              sum += (float)-0.0078996681;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.036985833)) {
    if (  (data[10].fvalue < 0.30565915)) {
      if (  (data[19].fvalue < 0.032639597)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.074019559)) {
            if (  (data[11].fvalue < 0.03467609)) {
              sum += (float)0.13891484;
            } else {
              sum += (float)-0.071496397;
            }
          } else {
            if (  (data[11].fvalue < 0.41301256)) {
              sum += (float)-0.20577864;
            } else {
              sum += (float)-0.43695858;
            }
          }
        } else {
          if (  (data[0].fvalue < 0.99999899)) {
            if (  (data[22].fvalue < 0.085243054)) {
              sum += (float)0.13578863;
            } else {
              sum += (float)-0.029925896;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.0094097713;
            } else {
              sum += (float)0.36125451;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.11676579)) {
          if (  (data[19].fvalue < 0.05606648)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.20076875;
            } else {
              sum += (float)0.012200279;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.030594381;
            } else {
              sum += (float)0.070196442;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.05606648)) {
            if (  (data[10].fvalue < 0.24286962)) {
              sum += (float)0.096734345;
            } else {
              sum += (float)0.38013083;
            }
          } else {
            if (  (data[10].fvalue < 0.18824665)) {
              sum += (float)0.3383311;
            } else {
              sum += (float)0.52524751;
            }
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.05606648)) {
        if (  (data[19].fvalue < 0.019449733)) {
          if (  (data[10].fvalue < 0.48988557)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.17695235;
            } else {
              sum += (float)0.23156361;
            }
          } else {
            if (  (data[11].fvalue < 0.02257712)) {
              sum += (float)0.74789387;
            } else {
              sum += (float)0.54876256;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.48988557)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.67446679;
            } else {
              sum += (float)0.36311805;
            }
          } else {
            sum += (float)-0.25705868;
          }
        }
      } else {
        sum += (float)-1.2152487;
      }
    }
  } else {
    if (  (data[19].fvalue < 0.076618336)) {
      if (  (data[12].fvalue < 0.99999899)) {
        if (  (data[16].fvalue < 1.999999)) {
          if (  (data[22].fvalue < 1.2119629)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.038231485;
            } else {
              sum += (float)-0.16357929;
            }
          } else {
            if (  (data[10].fvalue < 0.024808537)) {
              sum += (float)0.0061585964;
            } else {
              sum += (float)0.29078919;
            }
          }
        } else {
          if (  (data[22].fvalue < 4.1835213)) {
            if (  (data[8].fvalue < 0.84470683)) {
              sum += (float)-0.2300512;
            } else {
              sum += (float)-0.54495484;
            }
          } else {
            if (  (data[0].fvalue < 0.99999899)) {
              sum += (float)0.038886111;
            } else {
              sum += (float)-0.29285222;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 3.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0015372507;
            } else {
              sum += (float)-0.020368889;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.0048856162;
            } else {
              sum += (float)0.086603023;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.05606648)) {
            if (  (data[8].fvalue < 2.16449)) {
              sum += (float)-0.51049;
            } else {
              sum += (float)-0.42868933;
            }
          } else {
            if (  (data[8].fvalue < 0.84470683)) {
              sum += (float)-0.36095515;
            } else {
              sum += (float)-0.0819479;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 1.0940343)) {
        if (  (data[10].fvalue < 0.25982419)) {
          if (  (data[8].fvalue < 1.1250685)) {
            if (  (data[11].fvalue < 0.80249143)) {
              sum += (float)-0.010889563;
            } else {
              sum += (float)0.053877208;
            }
          } else {
            if (  (data[19].fvalue < 1.4235381)) {
              sum += (float)0.0034108411;
            } else {
              sum += (float)-0.0088342568;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.1108576)) {
            if (  (data[10].fvalue < 0.45415834)) {
              sum += (float)0.24437195;
            } else {
              sum += (float)0.55208719;
            }
          } else {
            if (  (data[8].fvalue < 0.18347144)) {
              sum += (float)0.10126013;
            } else {
              sum += (float)0.0036889457;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.37541971)) {
          if (  (data[19].fvalue < 0.12934837)) {
            if (  (data[8].fvalue < 0.29247537)) {
              sum += (float)0.5054937;
            } else {
              sum += (float)0.093810126;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.73961043;
            } else {
              sum += (float)0.45350689;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.15596609)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.30054098;
            } else {
              sum += (float)-0.0059863622;
            }
          } else {
            if (  (data[8].fvalue < 0.5585652)) {
              sum += (float)0.35272038;
            } else {
              sum += (float)0.011146658;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 11.121825)) {
    if ( (data[9].fvalue < 4.9906425)) {
      if (  (data[16].fvalue < 2.999999)) {
        if (  (data[19].fvalue < 2.8216894)) {
          if (  (data[18].fvalue < 3.3626215)) {
            if ( (data[9].fvalue < 1.1652663)) {
              sum += (float)-0.0074689006;
            } else {
              sum += (float)0.0037276451;
            }
          } else {
            if (  (data[10].fvalue < 0.2512362)) {
              sum += (float)-0.068655647;
            } else {
              sum += (float)-0.0038701918;
            }
          }
        } else {
          if (  (data[22].fvalue < 3.2519076)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.00073142204;
            } else {
              sum += (float)-0.042960409;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)-0.18166244;
            } else {
              sum += (float)-0.058197223;
            }
          }
        }
      } else {
        if (  (data[9].fvalue < 2.7748237)) {
          if (  (data[10].fvalue < 0.36952394)) {
            if (  (data[9].fvalue < 2.0828302)) {
              sum += (float)-0.028108811;
            } else {
              sum += (float)-0.17066614;
            }
          } else {
            if (  (data[22].fvalue < 2.5130589)) {
              sum += (float)0.046584353;
            } else {
              sum += (float)-0.13114855;
            }
          }
        } else {
          if (  (data[22].fvalue < 3.4351971)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0608631;
            } else {
              sum += (float)0.0013623021;
            }
          } else {
            if ( (data[9].fvalue < 4.2106938)) {
              sum += (float)-0.065866008;
            } else {
              sum += (float)-0.014412777;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 0.21147279)) {
            if (  (data[19].fvalue < 0.6860798)) {
              sum += (float)-0.0079540713;
            } else {
              sum += (float)-0.061692048;
            }
          } else {
            if (  (data[19].fvalue < 0.24120966)) {
              sum += (float)-0.014346386;
            } else {
              sum += (float)0.072695658;
            }
          }
        } else {
          if (  (data[22].fvalue < 0.43862912)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.040695757;
            } else {
              sum += (float)0.016347306;
            }
          } else {
            if (  (data[19].fvalue < 2.8216894)) {
              sum += (float)0.022426449;
            } else {
              sum += (float)0.069609597;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.18351135)) {
          if (  (data[18].fvalue < 0.11739449)) {
            if (  (data[19].fvalue < 0.056218054)) {
              sum += (float)-0.2835159;
            } else {
              sum += (float)-0.08232449;
            }
          } else {
            if (  (data[12].fvalue < 4.999999)) {
              sum += (float)0.017832877;
            } else {
              sum += (float)0.17185053;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.25817707)) {
              sum += (float)0.042643912;
            } else {
              sum += (float)0.16699904;
            }
          } else {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)-0.040474094;
            } else {
              sum += (float)0.041753881;
            }
          }
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 0.60299152)) {
      if (  (data[16].fvalue < 2.999999)) {
        if (  (data[14].fvalue < 1.999999)) {
          if (  (data[18].fvalue < 0.14308099)) {
            if (  (data[19].fvalue < 0.11720695)) {
              sum += (float)0.15154135;
            } else {
              sum += (float)0.0047975765;
            }
          } else {
            if (  (data[22].fvalue < 15.00602)) {
              sum += (float)-0.0066303248;
            } else {
              sum += (float)-0.048997764;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.3386327)) {
              sum += (float)-0.031564549;
            } else {
              sum += (float)-0.1170522;
            }
          } else {
            if (  (data[19].fvalue < 1.4259019)) {
              sum += (float)0.011964279;
            } else {
              sum += (float)-0.066333577;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 5.2689457)) {
          if (  (data[14].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 4.999999)) {
              sum += (float)-0.10625555;
            } else {
              sum += (float)-0.23433344;
            }
          } else {
            if (  (data[19].fvalue < 1.7558844)) {
              sum += (float)-0.011313796;
            } else {
              sum += (float)-0.071457453;
            }
          }
        } else {
          if (  (data[16].fvalue < 4.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.010266189;
            } else {
              sum += (float)-0.047745638;
            }
          } else {
            if (  (data[18].fvalue < 0.91814893)) {
              sum += (float)-0.13042143;
            } else {
              sum += (float)-0.021953437;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 1.999999)) {
        if (  (data[16].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 3.999999)) {
            if (  (data[10].fvalue < 0.60562027)) {
              sum += (float)0.035926495;
            } else {
              sum += (float)0.10453675;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)-0.09431424;
            } else {
              sum += (float)0.0067488654;
            }
          }
        } else {
          if ( (data[9].fvalue < 5.6380477)) {
            if (  (data[19].fvalue < 1.2515496)) {
              sum += (float)-0.081426248;
            } else {
              sum += (float)0.0054489817;
            }
          } else {
            if (  (data[22].fvalue < 19.819202)) {
              sum += (float)0.020072171;
            } else {
              sum += (float)-0.025585175;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 7.6128101)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.069817811;
            } else {
              sum += (float)0.078928582;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0091481255;
            } else {
              sum += (float)-0.039397877;
            }
          }
        } else {
          if (  (data[22].fvalue < 19.819202)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.014506578;
            } else {
              sum += (float)0.051161282;
            }
          } else {
            if (  (data[16].fvalue < 5.999999)) {
              sum += (float)-0.0045107533;
            } else {
              sum += (float)-0.075416699;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 0.082603045)) {
    if (  (data[15].fvalue < 0.99999899)) {
      if (  (data[19].fvalue < 0.032149341)) {
        if (  (data[18].fvalue < 0.017214999)) {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[20].fvalue < 0.13380326)) {
              sum += (float)-0.1801782;
            } else {
              sum += (float)0.12901217;
            }
          } else {
            if (  (data[20].fvalue < 0.020509969)) {
              sum += (float)0.069728605;
            } else {
              sum += (float)0.54747313;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.11808076)) {
              sum += (float)0.064324126;
            } else {
              sum += (float)-0.036899593;
            }
          } else {
            if (  (data[18].fvalue < 0.22585897)) {
              sum += (float)-0.32601696;
            } else {
              sum += (float)0.062429018;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.99999899)) {
          if (  (data[19].fvalue < 0.55848223)) {
            if (  (data[20].fvalue < 0.020509969)) {
              sum += (float)0.085835136;
            } else {
              sum += (float)0.012178405;
            }
          } else {
            if (  (data[19].fvalue < 1.1015935)) {
              sum += (float)-0.050752956;
            } else {
              sum += (float)0.087278374;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.69956893)) {
            if (  (data[19].fvalue < 0.064153217)) {
              sum += (float)0.042640656;
            } else {
              sum += (float)-0.061648671;
            }
          } else {
            if (  (data[8].fvalue < 0.03724568)) {
              sum += (float)0.27896708;
            } else {
              sum += (float)0.1525155;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 0.26242006)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[21].fvalue < 0.079048768)) {
            if (  (data[18].fvalue < 0.11808076)) {
              sum += (float)0.2421297;
            } else {
              sum += (float)0.062615477;
            }
          } else {
            if (  (data[18].fvalue < 0.86925536)) {
              sum += (float)-0.020654375;
            } else {
              sum += (float)0.19531508;
            }
          }
        } else {
          if (  (data[20].fvalue < 0.78245646)) {
            if (  (data[16].fvalue < 1.999999)) {
              sum += (float)-0.099330254;
            } else {
              sum += (float)0.070715792;
            }
          } else {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)0.063111655;
            } else {
              sum += (float)0.1767392;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.12985614)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[20].fvalue < 0.44336766)) {
              sum += (float)0.057360169;
            } else {
              sum += (float)0.26318729;
            }
          } else {
            if (  (data[18].fvalue < 0.028816566)) {
              sum += (float)0.2471808;
            } else {
              sum += (float)-0.090557411;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.19771886)) {
              sum += (float)-0.15100482;
            } else {
              sum += (float)0.056562569;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.072528057;
            } else {
              sum += (float)0.0049562627;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 9.1298141)) {
      if (  (data[21].fvalue < 2.3919246)) {
        if (  (data[19].fvalue < 0.10301187)) {
          if (  (data[20].fvalue < 0.044084173)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.019084712;
            } else {
              sum += (float)0.064679116;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.025911637;
            } else {
              sum += (float)-0.031900067;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.3055444)) {
            if (  (data[20].fvalue < 0.44336766)) {
              sum += (float)0.0048180465;
            } else {
              sum += (float)-0.017978061;
            }
          } else {
            if (  (data[21].fvalue < 0.505714)) {
              sum += (float)0.0010531819;
            } else {
              sum += (float)0.018664695;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.37684968)) {
          if (  (data[18].fvalue < 0.18239813)) {
            if (  (data[20].fvalue < 0.020509969)) {
              sum += (float)0.13843416;
            } else {
              sum += (float)0.035948265;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.032623861;
            } else {
              sum += (float)-0.010293777;
            }
          }
        } else {
          if (  (data[20].fvalue < 8.3779697)) {
            if (  (data[18].fvalue < 0.18239813)) {
              sum += (float)-0.026573585;
            } else {
              sum += (float)-0.0053989994;
            }
          } else {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)-0.041789226;
            } else {
              sum += (float)-0.082211658;
            }
          }
        }
      }
    } else {
      if (  (data[21].fvalue < 14.716711)) {
        if (  (data[19].fvalue < 0.53532428)) {
          if (  (data[18].fvalue < 0.43324998)) {
            if (  (data[20].fvalue < 2.8861616)) {
              sum += (float)-0.1241905;
            } else {
              sum += (float)-0.0077980952;
            }
          } else {
            if (  (data[20].fvalue < 6.093286)) {
              sum += (float)0.017939867;
            } else {
              sum += (float)-0.0024205162;
            }
          }
        } else {
          if (  (data[20].fvalue < 4.8529811)) {
            if (  (data[16].fvalue < 2.999999)) {
              sum += (float)0.013880758;
            } else {
              sum += (float)0.064605452;
            }
          } else {
            if (  (data[18].fvalue < 0.13040952)) {
              sum += (float)0.014873171;
            } else {
              sum += (float)-0.018410601;
            }
          }
        }
      } else {
        if (  (data[19].fvalue < 0.6513254)) {
          if (  (data[18].fvalue < 0.24119391)) {
            if (  (data[19].fvalue < 0.22560391)) {
              sum += (float)0.12732111;
            } else {
              sum += (float)0.012608772;
            }
          } else {
            if (  (data[8].fvalue < 20.409441)) {
              sum += (float)-0.039298166;
            } else {
              sum += (float)-0.014780996;
            }
          }
        } else {
          if (  (data[8].fvalue < 27.682461)) {
            if (  (data[18].fvalue < 0.19771886)) {
              sum += (float)-0.077487297;
            } else {
              sum += (float)-0.046205323;
            }
          } else {
            if (  (data[19].fvalue < 1.2550366)) {
              sum += (float)-0.038727205;
            } else {
              sum += (float)0.014396258;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 0.99812824)) {
    if (  (data[8].fvalue < 0.14626788)) {
      if (  (data[9].fvalue < 0.96435589)) {
        if (  (data[13].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 0.91295797)) {
            if (  (data[7].fvalue < 0.34566662)) {
              sum += (float)0.51384312;
            } else {
              sum += (float)0.77455175;
            }
          } else {
            if (  (data[8].fvalue < 0.036329895)) {
              sum += (float)0.24417424;
            } else {
              sum += (float)0.075123094;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.26702148;
            } else {
              sum += (float)-0.025978684;
            }
          } else {
            if (  (data[8].fvalue < 0.07874921)) {
              sum += (float)-0.56012762;
            } else {
              sum += (float)-0.034098268;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.036329895)) {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.12220101;
            } else {
              sum += (float)-0.042838294;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.10430327;
            } else {
              sum += (float)-0.37893847;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.062804945;
            } else {
              sum += (float)0.08101847;
            }
          } else {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.021902787;
            } else {
              sum += (float)0.19914508;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 4.999999)) {
        if (  (data[8].fvalue < 7.7526321)) {
          if (  (data[9].fvalue < 0.91295797)) {
            if (  (data[8].fvalue < 0.21893947)) {
              sum += (float)0.23405133;
            } else {
              sum += (float)-0.0086208954;
            }
          } else {
            if (  (data[7].fvalue < 0.9997924)) {
              sum += (float)-0.026515488;
            } else {
              sum += (float)-0.078925714;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)1.0290385;
            } else {
              sum += (float)0.083092324;
            }
          } else {
            if (  (data[21].fvalue < 3.3163998)) {
              sum += (float)0.080244899;
            } else {
              sum += (float)-0.095383845;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.77705854)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)-0.085522436;
            } else {
              sum += (float)-0.316744;
            }
          } else {
            if (  (data[12].fvalue < 3.999999)) {
              sum += (float)0.28251681;
            } else {
              sum += (float)0.51402903;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.22028434;
            } else {
              sum += (float)0.0296214;
            }
          } else {
            if (  (data[8].fvalue < 2.617805)) {
              sum += (float)0.45177746;
            } else {
              sum += (float)0.25377047;
            }
          }
        }
      }
    }
  } else {
    if (  (data[9].fvalue < 1.0979089)) {
      if (  (data[21].fvalue < 0.043387871)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.99999899)) {
              sum += (float)-0.073281541;
            } else {
              sum += (float)0.16285902;
            }
          } else {
            if (  (data[8].fvalue < 0.29089507)) {
              sum += (float)0.14717631;
            } else {
              sum += (float)-0.015003851;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.074037373;
            } else {
              sum += (float)-0.013107802;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)-0.075908922;
            } else {
              sum += (float)0.078086555;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.41861516)) {
          if (  (data[8].fvalue < 0.29089507)) {
            if (  (data[21].fvalue < 0.078923374)) {
              sum += (float)0.42500725;
            } else {
              sum += (float)0.68316245;
            }
          } else {
            if (  (data[21].fvalue < 0.11764823)) {
              sum += (float)0.23250887;
            } else {
              sum += (float)0.56130809;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.22530237;
            } else {
              sum += (float)-0.25857431;
            }
          } else {
            if (  (data[8].fvalue < 4.8507805)) {
              sum += (float)0.0014044069;
            } else {
              sum += (float)0.1690616;
            }
          }
        }
      }
    } else {
      if (  (data[12].fvalue < 6.999999)) {
        if (  (data[13].fvalue < 6.999999)) {
          if (  (data[16].fvalue < 1.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.00027185929;
            } else {
              sum += (float)0.0093264626;
            }
          } else {
            if (  (data[9].fvalue < 3.2936792)) {
              sum += (float)-0.02106598;
            } else {
              sum += (float)0.0031981743;
            }
          }
        } else {
          if (  (data[21].fvalue < 0.11764823)) {
            if (  (data[21].fvalue < 0.021809604)) {
              sum += (float)0.00078017527;
            } else {
              sum += (float)0.08545287;
            }
          } else {
            if (  (data[9].fvalue < 3.1141274)) {
              sum += (float)-0.11186167;
            } else {
              sum += (float)-0.018302886;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[12].fvalue < 8.999999)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)-0.23833916;
            } else {
              sum += (float)-0.082892209;
            }
          } else {
            if (  (data[12].fvalue < 10.999999)) {
              sum += (float)-0.25274041;
            } else {
              sum += (float)-0.42928159;
            }
          }
        } else {
          if (  (data[13].fvalue < 6.999999)) {
            if (  (data[9].fvalue < 1.415059)) {
              sum += (float)0.13251485;
            } else {
              sum += (float)-0.030048249;
            }
          } else {
            if (  (data[9].fvalue < 2.6136653)) {
              sum += (float)-0.036047056;
            } else {
              sum += (float)0.015754726;
            }
          }
        }
      }
    }
  }
  if (  (data[10].fvalue < 0.74335289)) {
    if (  (data[19].fvalue < 6.212594)) {
      if (  (data[8].fvalue < 0.18810655)) {
        if (  (data[19].fvalue < 0.044088934)) {
          if (  (data[10].fvalue < 0.24292049)) {
            if (  (data[21].fvalue < 4.5597167)) {
              sum += (float)0.0018951618;
            } else {
              sum += (float)-0.16315857;
            }
          } else {
            if (  (data[8].fvalue < 0.036184948)) {
              sum += (float)0.091758914;
            } else {
              sum += (float)-0.10343324;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.15893225)) {
            if (  (data[15].fvalue < 0.99999899)) {
              sum += (float)0.0039431881;
            } else {
              sum += (float)-0.04128905;
            }
          } else {
            if (  (data[8].fvalue < 0.083093137)) {
              sum += (float)0.15155639;
            } else {
              sum += (float)0.01509425;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.11623911)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.014808596)) {
              sum += (float)-0.060427725;
            } else {
              sum += (float)0.00091874413;
            }
          } else {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)0.016261207;
            } else {
              sum += (float)-0.024910634;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.33877993)) {
            if (  (data[13].fvalue < 2.999999)) {
              sum += (float)0.0049827904;
            } else {
              sum += (float)-0.028948965;
            }
          } else {
            if (  (data[8].fvalue < 2.7815173)) {
              sum += (float)-0.012156007;
            } else {
              sum += (float)0.00248022;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 13.344064)) {
            if (  (data[10].fvalue < 0.5512383)) {
              sum += (float)-0.018271526;
            } else {
              sum += (float)0.060524337;
            }
          } else {
            if (  (data[10].fvalue < 0.5512383)) {
              sum += (float)-0.083790459;
            } else {
              sum += (float)-0.027696388;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 5.1343513)) {
              sum += (float)0.040367588;
            } else {
              sum += (float)0.094974346;
            }
          } else {
            if (  (data[11].fvalue < 0.24767764)) {
              sum += (float)-0.10244628;
            } else {
              sum += (float)-0.0044594845;
            }
          }
        }
      } else {
        if (  (data[21].fvalue < 0.54742557)) {
          if (  (data[15].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.029476319;
            } else {
              sum += (float)-0.11346991;
            }
          } else {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.023654416;
            } else {
              sum += (float)0.15024674;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99999899)) {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.01398004;
            } else {
              sum += (float)0.030987693;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.047242586;
            } else {
              sum += (float)-0.017018268;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 0.11188364)) {
      if (  (data[8].fvalue < 0.083093137)) {
        if (  (data[10].fvalue < 0.81252688)) {
          if (  (data[19].fvalue < 0.019673727)) {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.0051639816;
            } else {
              sum += (float)0.21543922;
            }
          } else {
            if (  (data[19].fvalue < 0.032709744)) {
              sum += (float)0.71731681;
            } else {
              sum += (float)0.40967;
            }
          }
        } else {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.91076618)) {
              sum += (float)0.5244602;
            } else {
              sum += (float)0.72565252;
            }
          } else {
            sum += (float)-0.12354203;
          }
        }
      } else {
        if (  (data[19].fvalue < 0.032709744)) {
          if (  (data[10].fvalue < 0.91076618)) {
            if (  (data[13].fvalue < 0.99999899)) {
              sum += (float)0.0032142147;
            } else {
              sum += (float)-0.31906292;
            }
          } else {
            if (  (data[19].fvalue < 0.019673727)) {
              sum += (float)0.17621955;
            } else {
              sum += (float)0.73545939;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[19].fvalue < 0.066580795)) {
              sum += (float)0.58693331;
            } else {
              sum += (float)0.16196501;
            }
          } else {
            sum += (float)0.036078781;
          }
        }
      }
    } else {
      if (  (data[19].fvalue < 0.22697529)) {
        if (  (data[8].fvalue < 0.18810655)) {
          if (  (data[19].fvalue < 0.056342456)) {
            if (  (data[10].fvalue < 1.0958668)) {
              sum += (float)-0.11264392;
            } else {
              sum += (float)0.15389703;
            }
          } else {
            if (  (data[10].fvalue < 0.91076618)) {
              sum += (float)0.25080821;
            } else {
              sum += (float)0.5842616;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.049001)) {
            if (  (data[19].fvalue < 0.10406639)) {
              sum += (float)-0.06362427;
            } else {
              sum += (float)0.0038588971;
            }
          } else {
            if (  (data[19].fvalue < 0.066580795)) {
              sum += (float)0.035094306;
            } else {
              sum += (float)-0.041301955;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 0.65935206)) {
          if (  (data[8].fvalue < 0.41720122)) {
            if (  (data[8].fvalue < 0.37820703)) {
              sum += (float)0.76998448;
            } else {
              sum += (float)0.45261231;
            }
          } else {
            if (  (data[19].fvalue < 0.28893328)) {
              sum += (float)0.083771244;
            } else {
              sum += (float)0.2695387;
            }
          }
        } else {
          if (  (data[19].fvalue < 0.35691661)) {
            if (  (data[8].fvalue < 4.2454581)) {
              sum += (float)-0.080567271;
            } else {
              sum += (float)0.097349033;
            }
          } else {
            if (  (data[8].fvalue < 0.84343696)) {
              sum += (float)0.15850492;
            } else {
              sum += (float)0.014795048;
            }
          }
        }
      }
    }
  }
  if (  (data[22].fvalue < 9.2414408)) {
    if (  (data[18].fvalue < 0.039112259)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)0.0052653668;
            } else {
              sum += (float)-0.023017887;
            }
          } else {
            if (  (data[13].fvalue < 1.999999)) {
              sum += (float)-0.09123987;
            } else {
              sum += (float)0.04093368;
            }
          }
        } else {
          if (  (data[18].fvalue < 0.016284689)) {
            if (  (data[13].fvalue < 4.999999)) {
              sum += (float)0.043600306;
            } else {
              sum += (float)-0.027720405;
            }
          } else {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)0.1805224;
            } else {
              sum += (float)0.2481285;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 1.999999)) {
            if (  (data[18].fvalue < 0.029939279)) {
              sum += (float)-0.39273468;
            } else {
              sum += (float)-0.18236156;
            }
          } else {
            if (  (data[18].fvalue < 0.016284689)) {
              sum += (float)-0.37414414;
            } else {
              sum += (float)-0.084786847;
            }
          }
        } else {
          if (  (data[13].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 0.01850469)) {
              sum += (float)-0.58277428;
            } else {
              sum += (float)-0.45284021;
            }
          } else {
            if (  (data[18].fvalue < 0.029939279)) {
              sum += (float)-0.53659207;
            } else {
              sum += (float)-0.37939104;
            }
          }
        }
      }
    } else {
      if (  (data[11].fvalue < 0.016880585)) {
        if (  (data[6].fvalue < 16.429703)) {
          if (  (data[18].fvalue < 0.093005583)) {
            if (  (data[6].fvalue < 6.4576125)) {
              sum += (float)0.3584446;
            } else {
              sum += (float)0.032278858;
            }
          } else {
            if (  (data[6].fvalue < 9.198082)) {
              sum += (float)0.5536229;
            } else {
              sum += (float)0.21020931;
            }
          }
        } else {
          if (  (data[6].fvalue < 29.871113)) {
            if (  (data[18].fvalue < 0.25787988)) {
              sum += (float)0.023832938;
            } else {
              sum += (float)0.20564179;
            }
          } else {
            if (  (data[18].fvalue < 0.50921983)) {
              sum += (float)-0.011856515;
            } else {
              sum += (float)0.11153066;
            }
          }
        }
      } else {
        if (  (data[20].fvalue < 0.083893687)) {
          if (  (data[15].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 6.999999)) {
              sum += (float)0.0029092547;
            } else {
              sum += (float)0.033788834;
            }
          } else {
            if (  (data[14].fvalue < 0.99999899)) {
              sum += (float)0.13675661;
            } else {
              sum += (float)-0.03064971;
            }
          }
        } else {
          if (  (data[12].fvalue < 6.999999)) {
            if (  (data[20].fvalue < 0.20375557)) {
              sum += (float)-0.023913547;
            } else {
              sum += (float)0.0015044959;
            }
          } else {
            if (  (data[18].fvalue < 0.48254329)) {
              sum += (float)-0.14606036;
            } else {
              sum += (float)-0.016582137;
            }
          }
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 0.5195291)) {
      if (  (data[13].fvalue < 1.999999)) {
        if (  (data[20].fvalue < 2.1252365)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[15].fvalue < 2.999999)) {
              sum += (float)-0.018378751;
            } else {
              sum += (float)0.089147769;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.29261637;
            } else {
              sum += (float)0.023166431;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 1.999999)) {
              sum += (float)-0.036105283;
            } else {
              sum += (float)-0.066789478;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.030549536;
            } else {
              sum += (float)-0.17798564;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 3.999999)) {
              sum += (float)-0.019433046;
            } else {
              sum += (float)-0.052772097;
            }
          } else {
            if (  (data[4].fvalue < 0.99999899)) {
              sum += (float)0.28484714;
            } else {
              sum += (float)0.034546707;
            }
          }
        } else {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[18].fvalue < 0.32841638)) {
              sum += (float)-0.075783223;
            } else {
              sum += (float)-0.20422336;
            }
          } else {
            if (  (data[18].fvalue < 0.97848505)) {
              sum += (float)-0.00050847972;
            } else {
              sum += (float)-0.08503776;
            }
          }
        }
      }
    } else {
      if (  (data[20].fvalue < 5.5535479)) {
        if (  (data[15].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[22].fvalue < 19.760508)) {
              sum += (float)-0.0017093609;
            } else {
              sum += (float)-0.020060081;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.11372498;
            } else {
              sum += (float)0.0080783507;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 5.999999)) {
              sum += (float)0.064493954;
            } else {
              sum += (float)-0.059431389;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.071562834;
            } else {
              sum += (float)-0.039011322;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 0.85862756)) {
          if (  (data[18].fvalue < 1.0300453)) {
            if (  (data[14].fvalue < 2.999999)) {
              sum += (float)-0.012574336;
            } else {
              sum += (float)-0.039167084;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)-0.0061978605;
            } else {
              sum += (float)-0.081876308;
            }
          }
        } else {
          if (  (data[14].fvalue < 2.999999)) {
            if (  (data[20].fvalue < 20.31521)) {
              sum += (float)0.012504054;
            } else {
              sum += (float)-0.044804119;
            }
          } else {
            if (  (data[15].fvalue < 1.999999)) {
              sum += (float)0.025021385;
            } else {
              sum += (float)-0.025333291;
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
