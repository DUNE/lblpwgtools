#include "BDTReweighter.h"
class GeneratorReweight_numu_FD_RHC : public BDTReweighter {
 public :
 GeneratorReweight_numu_FD_RHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_numu_FD_RHC() {;}


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
    if ( (data[14].fvalue < 0.039013341)) {
      if ( (data[13].fvalue < 1.5245912)) {
        if ( (data[16].fvalue < 0.021752056)) {
          if ( (data[14].fvalue < 4.559254e-07)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.082502373;
            } else {
              sum += (float)0.094715163;
            }
          } else {
            if ( (data[14].fvalue < 0.012795744)) {
              sum += (float)-0.25571963;
            } else {
              sum += (float)-0.10324101;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.29930377;
            } else {
              sum += (float)-0.075570881;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.056587588;
            } else {
              sum += (float)-0.1670181;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.62061608)) {
          if ( (data[4].fvalue < 1.8126199)) {
            sum += (float)-0.21196851;
          } else {
            if ( (data[15].fvalue < 0.47124845)) {
              sum += (float)-0.44594792;
            } else {
              sum += (float)-0.27660695;
            }
          }
        } else {
          sum += (float)-0.069338672;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.016085204)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.23234107)) {
              sum += (float)-0.29600638;
            } else {
              sum += (float)-0.11993074;
            }
          } else {
            sum += (float)0.13803953;
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.256641)) {
              sum += (float)-0;
            } else {
              sum += (float)0.28999373;
            }
          } else {
            sum += (float)-0.22482084;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[4].fvalue < 1.0734817)) {
            if ( (data[3].fvalue < 0.80253857)) {
              sum += (float)-0.044259042;
            } else {
              sum += (float)0.10100927;
            }
          } else {
            if ( (data[13].fvalue < 1.0014367)) {
              sum += (float)0.26035386;
            } else {
              sum += (float)0.043691695;
            }
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.11462604;
            } else {
              sum += (float)-0.058870811;
            }
          } else {
            if ( (data[3].fvalue < 0.96680325)) {
              sum += (float)0.34120032;
            } else {
              sum += (float)0.15340543;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[10].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.20434371)) {
          sum += (float)0.4402985;
        } else {
          if ( (data[14].fvalue < 0.15332668)) {
            sum += (float)0.1635212;
          } else {
            sum += (float)0.32385463;
          }
        }
      } else {
        sum += (float)0.13043848;
      }
    } else {
      if ( (data[16].fvalue < 0.0040325872)) {
        sum += (float)0.53260422;
      } else {
        sum += (float)0.34536847;
      }
    }
  }
  if ( (data[8].fvalue < 4.5)) {
    if ( (data[14].fvalue < 0.13679065)) {
      if ( (data[13].fvalue < 1.9544083)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.1009685;
            } else {
              sum += (float)0.042125221;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.076655559;
            } else {
              sum += (float)-0.041456796;
            }
          }
        } else {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[3].fvalue < 0.2361415)) {
              sum += (float)-0.093375593;
            } else {
              sum += (float)-0.26582918;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.39337006)) {
          sum += (float)-0.32413322;
        } else {
          sum += (float)-0.10263981;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.012622658)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.31720251)) {
            if ( (data[15].fvalue < 0.50214547)) {
              sum += (float)-0.10193522;
            } else {
              sum += (float)0.053746592;
            }
          } else {
            if (  (data[4].fvalue < 1.2486293)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.1445585;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.10019989)) {
            sum += (float)-0.016328532;
          } else {
            if ( (data[17].fvalue < 0.032343805)) {
              sum += (float)0.18186697;
            } else {
              sum += (float)0.095201962;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 5.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.18623637;
            } else {
              sum += (float)0.032842759;
            }
          } else {
            if ( (data[5].fvalue < 0.30982572)) {
              sum += (float)0.085463539;
            } else {
              sum += (float)-0.038422372;
            }
          }
        } else {
          sum += (float)0.27920187;
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.0171925)) {
      if ( (data[8].fvalue < 6.5)) {
        if (  (data[4].fvalue < 1.207495)) {
          sum += (float)0.34401995;
        } else {
          sum += (float)0.18210527;
        }
      } else {
        sum += (float)0.41072801;
      }
    } else {
      if ( (data[8].fvalue < 7.5)) {
        if ( (data[14].fvalue < 0.33830491)) {
          sum += (float)-0.079397537;
        } else {
          if ( (data[13].fvalue < 0.47562438)) {
            sum += (float)0.046883777;
          } else {
            sum += (float)0.2120939;
          }
        }
      } else {
        sum += (float)0.25686672;
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[14].fvalue < 0.30784672)) {
      if ( (data[3].fvalue < 1.1212647)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[5].fvalue < 1.2339978)) {
            if ( (data[14].fvalue < 0.067156814)) {
              sum += (float)-0.048401654;
            } else {
              sum += (float)0.019920629;
            }
          } else {
            if ( (data[14].fvalue < 1.3580733e-05)) {
              sum += (float)0.18016548;
            } else {
              sum += (float)0.00034047355;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.17345297)) {
            sum += (float)0.27390948;
          } else {
            sum += (float)0.00092102983;
          }
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[13].fvalue < 2.2674489)) {
            if ( (data[13].fvalue < 0.34714693)) {
              sum += (float)-0.18768278;
            } else {
              sum += (float)-0.072569475;
            }
          } else {
            sum += (float)-0.26536745;
          }
        } else {
          if ( (data[17].fvalue < 1.1376147)) {
            sum += (float)-0.075224876;
          } else {
            if ( (data[15].fvalue < 0.024397975)) {
              sum += (float)0.1443096;
            } else {
              sum += (float)0.0050972495;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.29108047)) {
        if ( (data[13].fvalue < 0.30607229)) {
          if ( (data[15].fvalue < 0.35716462)) {
            if ( (data[14].fvalue < 0.93374372)) {
              sum += (float)0.096736565;
            } else {
              sum += (float)-0.086021528;
            }
          } else {
            if ( (data[17].fvalue < 0.026679294)) {
              sum += (float)0.1933417;
            } else {
              sum += (float)0.07695628;
            }
          }
        } else {
          sum += (float)0.2068207;
        }
      } else {
        if ( (data[13].fvalue < 0.30546713)) {
          if ( (data[14].fvalue < 1.4761393)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.036185816;
            } else {
              sum += (float)-0.066464871;
            }
          } else {
            if ( (data[15].fvalue < 0.60940659)) {
              sum += (float)-0.22809707;
            } else {
              sum += (float)0.084091537;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.016189737)) {
            if ( (data[14].fvalue < 0.50061822)) {
              sum += (float)0.089837655;
            } else {
              sum += (float)0.21976539;
            }
          } else {
            sum += (float)0.010447447;
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.00066899293)) {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[3].fvalue < 0.53070873)) {
          sum += (float)0.32759565;
        } else {
          sum += (float)0.19376506;
        }
      } else {
        sum += (float)0.37633917;
      }
    } else {
      sum += (float)0.17091641;
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.27224278)) {
      if ( (data[13].fvalue < 1.3268361)) {
        if ( (data[15].fvalue < 2.8416166)) {
          if ( (data[5].fvalue < 1.4944426)) {
            if ( (data[5].fvalue < 0.22295508)) {
              sum += (float)0.016715253;
            } else {
              sum += (float)-0.039024383;
            }
          } else {
            sum += (float)0.16506815;
          }
        } else {
          sum += (float)-0.24595225;
        }
      } else {
        if ( (data[17].fvalue < 0.91341603)) {
          if ( (data[15].fvalue < 0.89194608)) {
            if ( (data[4].fvalue < 1.5607095)) {
              sum += (float)-0.057558712;
            } else {
              sum += (float)-0.24175766;
            }
          } else {
            sum += (float)-0.006373276;
          }
        } else {
          sum += (float)0.068222381;
        }
      }
    } else {
      if ( (data[13].fvalue < 0.032392543)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.87890065)) {
            if ( (data[14].fvalue < 1.2477164)) {
              sum += (float)-0.035291284;
            } else {
              sum += (float)-0.19234911;
            }
          } else {
            if ( (data[14].fvalue < 0.74482715)) {
              sum += (float)-0.046379503;
            } else {
              sum += (float)0.11687572;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)0.16894671;
          } else {
            sum += (float)-0.14703637;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.53302139)) {
            sum += (float)0.17140767;
          } else {
            sum += (float)0.076304093;
          }
        } else {
          if ( (data[13].fvalue < 0.3949157)) {
            if ( (data[13].fvalue < 0.097258791)) {
              sum += (float)-0.10845954;
            } else {
              sum += (float)0.02591609;
            }
          } else {
            if ( (data[17].fvalue < 0.025784744)) {
              sum += (float)0.16916178;
            } else {
              sum += (float)0.049475025;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[16].fvalue < 0.00042187335)) {
        if ( (data[13].fvalue < 0.18000825)) {
          sum += (float)0.2914784;
        } else {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)0.2351459;
          } else {
            if ( (data[3].fvalue < 1.1812332)) {
              sum += (float)0.14441563;
            } else {
              sum += (float)0.024384473;
            }
          }
        }
      } else {
        sum += (float)0.02063274;
      }
    } else {
      if ( (data[3].fvalue < 1.1974618)) {
        sum += (float)0.33329916;
      } else {
        sum += (float)0.20597593;
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[14].fvalue < 0.10734265)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[17].fvalue < 0.041770436)) {
          if ( (data[15].fvalue < 0.83123541)) {
            if ( (data[13].fvalue < 1.4974796e-06)) {
              sum += (float)0.16280945;
            } else {
              sum += (float)0.0029538779;
            }
          } else {
            if ( (data[13].fvalue < 0.27318716)) {
              sum += (float)-0.24435733;
            } else {
              sum += (float)0.070335574;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.012865487)) {
              sum += (float)-0.13551757;
            } else {
              sum += (float)0.072264284;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.10633607;
            } else {
              sum += (float)-0.044829745;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.22428507)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.022281764)) {
              sum += (float)-0.24088611;
            } else {
              sum += (float)-0.033385132;
            }
          } else {
            sum += (float)0.2638869;
          }
        } else {
          if ( (data[14].fvalue < 0.049719989)) {
            sum += (float)-0.33753112;
          } else {
            sum += (float)-0.17150481;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.38520217)) {
        if ( (data[13].fvalue < 0.010856017)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.20899397)) {
              sum += (float)-0.13788451;
            } else {
              sum += (float)0.0067047742;
            }
          } else {
            if ( (data[5].fvalue < 0.24059696)) {
              sum += (float)0.13266836;
            } else {
              sum += (float)0.052795894;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.15231493;
            } else {
              sum += (float)0.061770406;
            }
          } else {
            if ( (data[15].fvalue < 0.30610195)) {
              sum += (float)-0;
            } else {
              sum += (float)0.093840472;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.030317035)) {
          if ( (data[14].fvalue < 0.47084346)) {
            if ( (data[15].fvalue < 0.78807992)) {
              sum += (float)-0.090218492;
            } else {
              sum += (float)-0.22803563;
            }
          } else {
            if ( (data[17].fvalue < 0.032670602)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.099815801;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.1311839)) {
              sum += (float)0.0024063485;
            } else {
              sum += (float)0.10515764;
            }
          } else {
            if ( (data[17].fvalue < 0.0045801909)) {
              sum += (float)0.005765398;
            } else {
              sum += (float)-0.10792947;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.0026966711)) {
      if ( (data[7].fvalue < 8.5)) {
        sum += (float)0.19111766;
      } else {
        sum += (float)0.29056871;
      }
    } else {
      if ( (data[3].fvalue < 1.4224118)) {
        sum += (float)0.13911518;
      } else {
        sum += (float)0.022707531;
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[13].fvalue < 2.0746155)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.21778342)) {
              sum += (float)-0.012357991;
            } else {
              sum += (float)0.033181909;
            }
          } else {
            if ( (data[5].fvalue < 0.21828629)) {
              sum += (float)-0.037906393;
            } else {
              sum += (float)-0.20479764;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.12206624)) {
            if (  (data[4].fvalue < 1.1024729)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.12749562;
            }
          } else {
            if ( (data[5].fvalue < 0.42424688)) {
              sum += (float)0.047103051;
            } else {
              sum += (float)-0.063830934;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.34142566)) {
          sum += (float)-0.21619421;
        } else {
          sum += (float)-0.027521653;
        }
      }
    } else {
      if ( (data[6].fvalue < 0.11663434)) {
        sum += (float)0.23468915;
      } else {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)0.17072777;
        } else {
          if ( (data[14].fvalue < 0.31060639)) {
            if ( (data[15].fvalue < 0.16286531)) {
              sum += (float)-0.053615149;
            } else {
              sum += (float)0.090075508;
            }
          } else {
            if ( (data[13].fvalue < 0.55443406)) {
              sum += (float)-0;
            } else {
              sum += (float)0.16823396;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.35736188)) {
      if ( (data[14].fvalue < 0.42237779)) {
        sum += (float)0.30107355;
      } else {
        sum += (float)0.16396458;
      }
    } else {
      if ( (data[17].fvalue < 0.0019889413)) {
        sum += (float)0.148984;
      } else {
        sum += (float)-0.014058461;
      }
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[9].fvalue < 1.5)) {
      if ( (data[16].fvalue < 0.00037187469)) {
        if ( (data[7].fvalue < 4.5)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[15].fvalue < 2.9221425)) {
              sum += (float)-0.00047254335;
            } else {
              sum += (float)-0.18501757;
            }
          } else {
            sum += (float)-0.15881452;
          }
        } else {
          if ( (data[13].fvalue < 0.15899283)) {
            sum += (float)0.19608688;
          } else {
            if ( (data[14].fvalue < 0.11524896)) {
              sum += (float)-0;
            } else {
              sum += (float)0.096509308;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 2.4032488)) {
          if ( (data[15].fvalue < 0.00037594885)) {
            if ( (data[16].fvalue < 0.15783417)) {
              sum += (float)-0.2695387;
            } else {
              sum += (float)-0.099496216;
            }
          } else {
            if ( (data[14].fvalue < 0.012762855)) {
              sum += (float)-0.12450248;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.48661864)) {
            if ( (data[14].fvalue < 0.022952834)) {
              sum += (float)-0.10230508;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[16].fvalue < 0.60771018)) {
              sum += (float)0.001180646;
            } else {
              sum += (float)0.18786623;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.026318546)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.01431103)) {
            sum += (float)0.064223886;
          } else {
            if ( (data[13].fvalue < 0.0045504589)) {
              sum += (float)0.081118077;
            } else {
              sum += (float)0.18596846;
            }
          }
        } else {
          sum += (float)0.029390657;
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.022167068)) {
            sum += (float)-0.041289173;
          } else {
            if ( (data[13].fvalue < 0.0054477956)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12328546;
            }
          }
        } else {
          sum += (float)-0.04269553;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.51992583)) {
      if ( (data[14].fvalue < 0.41011637)) {
        sum += (float)0.28092223;
      } else {
        sum += (float)0.13934244;
      }
    } else {
      sum += (float)0.01917173;
    }
  }
  if ( (data[7].fvalue < 7.5)) {
    if ( (data[5].fvalue < 0.18388456)) {
      if ( (data[13].fvalue < 1.1158147)) {
        if ( (data[13].fvalue < 0.08852794)) {
          if ( (data[14].fvalue < 0.052496515)) {
            if ( (data[15].fvalue < 0.041076411)) {
              sum += (float)0.0060142637;
            } else {
              sum += (float)-0.088238187;
            }
          } else {
            if ( (data[3].fvalue < 0.10230286)) {
              sum += (float)0.15466072;
            } else {
              sum += (float)0.021597892;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.0061293878)) {
              sum += (float)0.056233287;
            } else {
              sum += (float)0.15157931;
            }
          } else {
            if (  (data[4].fvalue < 1.385994)) {
              sum += (float)-0.080162302;
            } else {
              sum += (float)0.048284311;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.7046599)) {
          sum += (float)-0.19169106;
        } else {
          sum += (float)0.06865634;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.032578416)) {
        if ( (data[5].fvalue < 1.3200328)) {
          if ( (data[14].fvalue < 0.41644692)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.00055509346;
            } else {
              sum += (float)-0.057398792;
            }
          } else {
            if ( (data[13].fvalue < 0.40572166)) {
              sum += (float)0.026661687;
            } else {
              sum += (float)0.15772082;
            }
          }
        } else {
          if ( (data[14].fvalue < 1.6789885e-05)) {
            sum += (float)0.14280954;
          } else {
            sum += (float)0.024535716;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.1938023)) {
          if ( (data[15].fvalue < 0.76303101)) {
            if ( (data[3].fvalue < 0.61077666)) {
              sum += (float)-0.058669727;
            } else {
              sum += (float)-0.17060833;
            }
          } else {
            sum += (float)-0.0063061742;
          }
        } else {
          if ( (data[14].fvalue < 0.017878234)) {
            if ( (data[13].fvalue < 0.26048702)) {
              sum += (float)-0.18103418;
            } else {
              sum += (float)-0.040770009;
            }
          } else {
            if ( (data[13].fvalue < 0.022421941)) {
              sum += (float)-0.057024572;
            } else {
              sum += (float)0.05013933;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.2044156)) {
      sum += (float)0.20620835;
    } else {
      sum += (float)0.066833563;
    }
  }
  if ( (data[7].fvalue < 8.5)) {
    if ( (data[5].fvalue < 0.33944523)) {
      if ( (data[14].fvalue < 0.022645909)) {
        if ( (data[14].fvalue < 6.35253e-07)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 5.2137926e-07)) {
              sum += (float)0.10380348;
            } else {
              sum += (float)-0.046660095;
            }
          } else {
            if ( (data[6].fvalue < 0.41157579)) {
              sum += (float)0.043092184;
            } else {
              sum += (float)0.1617443;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.030159665)) {
            sum += (float)-0.19859535;
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.11234233;
            } else {
              sum += (float)-0.13306889;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.008027547)) {
          if ( (data[14].fvalue < 0.1759927)) {
            if ( (data[15].fvalue < 0.92772031)) {
              sum += (float)-0.089759871;
            } else {
              sum += (float)-0.23280303;
            }
          } else {
            if ( (data[17].fvalue < 0.031764537)) {
              sum += (float)0.057372332;
            } else {
              sum += (float)-0.022501862;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.15220354;
            } else {
              sum += (float)0.057648163;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.10156755;
            } else {
              sum += (float)-0.0044435062;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 1.1659882)) {
        if ( (data[17].fvalue < 0.010810868)) {
          if ( (data[4].fvalue < 1.031352)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.049223576;
            } else {
              sum += (float)0.055069897;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.015347128;
            } else {
              sum += (float)-0.055748656;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.73451173)) {
            if ( (data[3].fvalue < 1.4870464)) {
              sum += (float)-0.03421304;
            } else {
              sum += (float)-0.11686049;
            }
          } else {
            if ( (data[14].fvalue < 0.023764171)) {
              sum += (float)-0.061751571;
            } else {
              sum += (float)0.046716347;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 2.2169727e-06)) {
          if ( (data[13].fvalue < 0.079715505)) {
            sum += (float)0.15231541;
          } else {
            if ( (data[4].fvalue < 0.85137033)) {
              sum += (float)0.10413028;
            } else {
              sum += (float)-0.010581946;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.069708779)) {
            sum += (float)-0.048844527;
          } else {
            sum += (float)0.055606116;
          }
        }
      }
    }
  } else {
    sum += (float)0.17005722;
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[13].fvalue < 2.4638383)) {
      if ( (data[5].fvalue < 0.25068256)) {
        if ( (data[13].fvalue < 0.13457835)) {
          if ( (data[14].fvalue < 0.058890335)) {
            if ( (data[17].fvalue < 0.68085033)) {
              sum += (float)-0.014216845;
            } else {
              sum += (float)-0.19528849;
            }
          } else {
            if ( (data[15].fvalue < 0.0012469152)) {
              sum += (float)-0.056554522;
            } else {
              sum += (float)0.03746317;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.73044235)) {
            if ( (data[13].fvalue < 0.80941904)) {
              sum += (float)0.042263202;
            } else {
              sum += (float)-0.050044194;
            }
          } else {
            if ( (data[16].fvalue < 0.017219499)) {
              sum += (float)0.16295168;
            } else {
              sum += (float)0.012013613;
            }
          }
        }
      } else {
        if ( (data[12].fvalue < 0.5)) {
          if (  (data[4].fvalue < 2.1163754)) {
            if ( (data[16].fvalue < 0.010297913)) {
              sum += (float)-0.0088904323;
            } else {
              sum += (float)-0.086584158;
            }
          } else {
            if ( (data[13].fvalue < 0.59575188)) {
              sum += (float)-0.0069858679;
            } else {
              sum += (float)0.11434989;
            }
          }
        } else {
          sum += (float)-0.15041864;
        }
      }
    } else {
      sum += (float)-0.12453786;
    }
  } else {
    if ( (data[3].fvalue < 0.61393917)) {
      if (  (data[4].fvalue < 1.3299141)) {
        sum += (float)0.26924238;
      } else {
        sum += (float)0.1261255;
      }
    } else {
      if ( (data[7].fvalue < 3.5)) {
        sum += (float)0.12663922;
      } else {
        if ( (data[16].fvalue < 0.002232282)) {
          if ( (data[3].fvalue < 1.4325414)) {
            sum += (float)0.052043185;
          } else {
            sum += (float)-0.00015956073;
          }
        } else {
          sum += (float)-0.10414849;
        }
      }
    }
  }
  if ( (data[8].fvalue < 8.5)) {
    if ( (data[13].fvalue < 1.6791143)) {
      if ( (data[13].fvalue < 0.44007292)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[17].fvalue < 2.2672458)) {
            if ( (data[15].fvalue < 2.1175632)) {
              sum += (float)-0.00257915;
            } else {
              sum += (float)-0.085065484;
            }
          } else {
            if ( (data[14].fvalue < 0.18560129)) {
              sum += (float)-0.20313017;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.53726852)) {
            if (  (data[4].fvalue < 1.3272513)) {
              sum += (float)0.19598192;
            } else {
              sum += (float)0.073070347;
            }
          } else {
            if ( (data[15].fvalue < 0.25444442)) {
              sum += (float)-0.011668741;
            } else {
              sum += (float)0.064900674;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.2330532)) {
          if ( (data[16].fvalue < 0.032932505)) {
            if ( (data[14].fvalue < 0.43639374)) {
              sum += (float)0.011399721;
            } else {
              sum += (float)0.13585085;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.12907144;
            } else {
              sum += (float)-0.0075066132;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[16].fvalue < 0.021820128)) {
              sum += (float)0.19176964;
            } else {
              sum += (float)0.083566897;
            }
          } else {
            sum += (float)0.00079978909;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.40660477)) {
        if ( (data[5].fvalue < 0.54696)) {
          if (  (data[4].fvalue < 2.5653348)) {
            sum += (float)-0.178959;
          } else {
            sum += (float)-0.042653427;
          }
        } else {
          sum += (float)-0;
        }
      } else {
        sum += (float)0.028838249;
      }
    }
  } else {
    sum += (float)0.14249502;
  }
  if ( (data[7].fvalue < 9.5)) {
    if ( (data[13].fvalue < 2.7711334)) {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[16].fvalue < 0.0022505075)) {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0053543919;
            } else {
              sum += (float)-0.031664707;
            }
          } else {
            if ( (data[3].fvalue < 0.32062602)) {
              sum += (float)0.1642274;
            } else {
              sum += (float)0.031763397;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.00027476216)) {
            if ( (data[16].fvalue < 0.15706429)) {
              sum += (float)-0.20630775;
            } else {
              sum += (float)-0.050139144;
            }
          } else {
            if ( (data[14].fvalue < 0.0056494437)) {
              sum += (float)-0.061897464;
            } else {
              sum += (float)0.015245116;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.04370594)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.69396299)) {
              sum += (float)0.09862598;
            } else {
              sum += (float)0.01435001;
            }
          } else {
            sum += (float)0.0010321958;
          }
        } else {
          if (  (data[4].fvalue < 2.6366258)) {
            if ( (data[13].fvalue < 0.041150317)) {
              sum += (float)0.084117867;
            } else {
              sum += (float)-0.011998981;
            }
          } else {
            if ( (data[15].fvalue < 1.6189432)) {
              sum += (float)-0.066350818;
            } else {
              sum += (float)-0.0019356185;
            }
          }
        }
      }
    } else {
      sum += (float)-0.11218709;
    }
  } else {
    sum += (float)0.148719;
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[15].fvalue < 0.037411012)) {
        if ( (data[13].fvalue < 3.853806e-06)) {
          if ( (data[14].fvalue < 1.8329269e-05)) {
            sum += (float)0.45885065;
          } else {
            if ( (data[5].fvalue < 0.60925186)) {
              sum += (float)-0.069447078;
            } else {
              sum += (float)0.070516542;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.037084)) {
            if ( (data[14].fvalue < 4.559254e-07)) {
              sum += (float)0.020360049;
            } else {
              sum += (float)-0.11514018;
            }
          } else {
            if ( (data[13].fvalue < 0.042153172)) {
              sum += (float)-0.08293587;
            } else {
              sum += (float)0.046849664;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0054025706)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[4].fvalue < 2.2965322)) {
              sum += (float)-0.097868226;
            } else {
              sum += (float)-0.0027132523;
            }
          } else {
            if ( (data[15].fvalue < 0.20480096)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12716685;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.046419978)) {
            if ( (data[14].fvalue < 0.15375973)) {
              sum += (float)-0.13043086;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.11695003;
            } else {
              sum += (float)0.019012475;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.063432746)) {
        sum += (float)0.12404787;
      } else {
        if ( (data[14].fvalue < 0.197905)) {
          if ( (data[15].fvalue < 0.12095866)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.029794203;
            } else {
              sum += (float)-0.13069239;
            }
          } else {
            if (  (data[4].fvalue < 1.9913157)) {
              sum += (float)0.074335322;
            } else {
              sum += (float)-0.051105656;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.15313333)) {
            sum += (float)-0.16647208;
          } else {
            if ( (data[3].fvalue < 0.8831526)) {
              sum += (float)0.061139852;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      sum += (float)0.15667599;
    } else {
      if ( (data[13].fvalue < 0.26283664)) {
        sum += (float)0.14514385;
      } else {
        if ( (data[14].fvalue < 0.22838506)) {
          sum += (float)-0.1095717;
        } else {
          if ( (data[17].fvalue < 0.0028876071)) {
            sum += (float)0.062829003;
          } else {
            sum += (float)-0.039931253;
          }
        }
      }
    }
  }
  if ( (data[12].fvalue < 0.5)) {
    if ( (data[5].fvalue < 0.41234544)) {
      if ( (data[13].fvalue < 1.9584444)) {
        if ( (data[13].fvalue < 0.13576156)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.031758036)) {
              sum += (float)-0.0021296251;
            } else {
              sum += (float)-0.057256438;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.14367779;
            } else {
              sum += (float)0.0031377519;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.010164831)) {
              sum += (float)0.02003186;
            } else {
              sum += (float)0.094420649;
            }
          } else {
            if ( (data[14].fvalue < 0.098579526)) {
              sum += (float)-0.11390985;
            } else {
              sum += (float)0.028076675;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.6642895)) {
          sum += (float)-0.11902634;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[3].fvalue < 0.84198344)) {
          sum += (float)-0.23769866;
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.7427872)) {
              sum += (float)-0.11421096;
            } else {
              sum += (float)-0.0052582528;
            }
          } else {
            sum += (float)0.086887948;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.0029503168)) {
          sum += (float)0.25181201;
        } else {
          if ( (data[17].fvalue < 0.0019009417)) {
            if ( (data[4].fvalue < 0.79907525)) {
              sum += (float)0.095755689;
            } else {
              sum += (float)-0.0017996011;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.026306646;
            } else {
              sum += (float)-0.127487;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 2.4950714)) {
      sum += (float)-0.12805413;
    } else {
      sum += (float)0.0024049799;
    }
  }
  if ( (data[15].fvalue < 3.4081903)) {
    if ( (data[14].fvalue < 2.1513865)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[16].fvalue < 0.0090847909)) {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.012351087;
            } else {
              sum += (float)-0.029354656;
            }
          } else {
            if (  (data[4].fvalue < 1.1170006)) {
              sum += (float)0.17458244;
            } else {
              sum += (float)0.039843552;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.11366666)) {
            if ( (data[15].fvalue < 0.15154848)) {
              sum += (float)-0.1542194;
            } else {
              sum += (float)-0.05551286;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.022730345;
            } else {
              sum += (float)0.038577598;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 2.4451442)) {
          sum += (float)-0.11968524;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[15].fvalue < 0.33872235)) {
        sum += (float)-0.17758314;
      } else {
        sum += (float)-0;
      }
    }
  } else {
    sum += (float)-0.10792356;
  }
  if ( (data[17].fvalue < 0.034508795)) {
    if ( (data[16].fvalue < 0.68514585)) {
      if ( (data[8].fvalue < 3.5)) {
        if (  (data[4].fvalue < 1.0544598)) {
          if ( (data[14].fvalue < 4.559254e-07)) {
            if ( (data[13].fvalue < 0.039319266)) {
              sum += (float)0.2073836;
            } else {
              sum += (float)-0.020258494;
            }
          } else {
            if ( (data[14].fvalue < 0.019572927)) {
              sum += (float)-0.16607553;
            } else {
              sum += (float)-0.025027266;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[15].fvalue < 0.045268461)) {
              sum += (float)0.045974847;
            } else {
              sum += (float)-0.00020501077;
            }
          } else {
            if ( (data[14].fvalue < 0.43868586)) {
              sum += (float)-0.080488265;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.62220669)) {
          if (  (data[4].fvalue < 1.1301545)) {
            sum += (float)0.2891821;
          } else {
            if ( (data[15].fvalue < 0.10671783)) {
              sum += (float)-0;
            } else {
              sum += (float)0.14120501;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.50064707)) {
            if ( (data[13].fvalue < 0.41130555)) {
              sum += (float)-0.0010922395;
            } else {
              sum += (float)-0.13236053;
            }
          } else {
            if ( (data[13].fvalue < 0.26893511)) {
              sum += (float)0.001412746;
            } else {
              sum += (float)0.087911204;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.8053416)) {
        if ( (data[14].fvalue < 0.015112497)) {
          sum += (float)-0;
        } else {
          sum += (float)0.094937444;
        }
      } else {
        sum += (float)0.15433376;
      }
    }
  } else {
    if ( (data[17].fvalue < 0.12868673)) {
      if ( (data[15].fvalue < 0.90333074)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[17].fvalue < 0.064905718)) {
            sum += (float)-0.017451467;
          } else {
            sum += (float)-0.10896053;
          }
        } else {
          sum += (float)-0.19225615;
        }
      } else {
        sum += (float)-0;
      }
    } else {
      if ( (data[14].fvalue < 0.01098073)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.5301609)) {
              sum += (float)-0.087209463;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[15].fvalue < 0.019643812)) {
              sum += (float)0.07380151;
            } else {
              sum += (float)-0.041982997;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.056718282)) {
            sum += (float)0.083478235;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[17].fvalue < 1.1970234)) {
          if (  (data[4].fvalue < 1.715785)) {
            if ( (data[5].fvalue < 0.56908131)) {
              sum += (float)0.091137834;
            } else {
              sum += (float)-0.044663597;
            }
          } else {
            if ( (data[15].fvalue < 0.42641607)) {
              sum += (float)-0.055568341;
            } else {
              sum += (float)0.0017218562;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.66650784)) {
            if ( (data[13].fvalue < 0.021282235)) {
              sum += (float)-0.07917241;
            } else {
              sum += (float)0.075047404;
            }
          } else {
            sum += (float)0.12375317;
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.025061108)) {
    if ( (data[14].fvalue < 6.8384963e-08)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.028729498)) {
          if ( (data[3].fvalue < 0.74395323)) {
            if ( (data[15].fvalue < 0.029259909)) {
              sum += (float)0.16884647;
            } else {
              sum += (float)-0.10860065;
            }
          } else {
            sum += (float)0.35023266;
          }
        } else {
          if ( (data[4].fvalue < 1.5844083)) {
            if ( (data[5].fvalue < 0.1262114)) {
              sum += (float)0.12916681;
            } else {
              sum += (float)-0.014028465;
            }
          } else {
            if ( (data[17].fvalue < 0.71787423)) {
              sum += (float)-0.10347356;
            } else {
              sum += (float)-0.01521836;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.76337099)) {
          if ( (data[15].fvalue < 0.0014480341)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.025601141;
            } else {
              sum += (float)0.073126152;
            }
          } else {
            sum += (float)0.083796382;
          }
        } else {
          if (  (data[4].fvalue < 1.1776508)) {
            sum += (float)0.012404034;
          } else {
            if ( (data[17].fvalue < 0.16765392)) {
              sum += (float)0.1807863;
            } else {
              sum += (float)0.089098543;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.0058522979)) {
        if ( (data[7].fvalue < 1.5)) {
          sum += (float)-0.082704663;
        } else {
          sum += (float)-0.23772898;
        }
      } else {
        if ( (data[13].fvalue < 0.019001374)) {
          sum += (float)-0.15247115;
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[4].fvalue < 1.0910991)) {
              sum += (float)-0.089716449;
            } else {
              sum += (float)0.067775369;
            }
          } else {
            sum += (float)-0.12256668;
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.0252278)) {
      if ( (data[14].fvalue < 0.21216522)) {
        if ( (data[15].fvalue < 1.068119)) {
          if ( (data[5].fvalue < 0.19335735)) {
            sum += (float)0.042508736;
          } else {
            if ( (data[13].fvalue < 2.63e-05)) {
              sum += (float)-0.028769951;
            } else {
              sum += (float)-0.11511051;
            }
          }
        } else {
          sum += (float)-0.17124635;
        }
      } else {
        if ( (data[15].fvalue < 0.2690506)) {
          if ( (data[4].fvalue < 1.3864307)) {
            sum += (float)0.042310797;
          } else {
            if ( (data[17].fvalue < 0.53573084)) {
              sum += (float)-0.13506995;
            } else {
              sum += (float)0.0031998185;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.46011394)) {
            if ( (data[16].fvalue < 0.02399772)) {
              sum += (float)0.052694421;
            } else {
              sum += (float)-0.019579317;
            }
          } else {
            if ( (data[14].fvalue < 0.87702799)) {
              sum += (float)-0.07048858;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        if (  (data[4].fvalue < 1.0568509)) {
          if ( (data[3].fvalue < 0.78853405)) {
            sum += (float)-0.072375759;
          } else {
            sum += (float)0.082794555;
          }
        } else {
          if ( (data[14].fvalue < 0.10204439)) {
            if ( (data[4].fvalue < 1.8577106)) {
              sum += (float)0.16742258;
            } else {
              sum += (float)0.067939073;
            }
          } else {
            if ( (data[14].fvalue < 0.49155575)) {
              sum += (float)0.0261888;
            } else {
              sum += (float)0.084440708;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.078003004)) {
          sum += (float)0.11808302;
        } else {
          if ( (data[5].fvalue < 0.66120696)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.057063691;
            } else {
              sum += (float)-0.011825173;
            }
          } else {
            if ( (data[3].fvalue < 1.1267514)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.13681468;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.3572242)) {
    if ( (data[5].fvalue < 0.025944743)) {
      sum += (float)0.075130925;
    } else {
      if ( (data[16].fvalue < 0.62536013)) {
        if ( (data[16].fvalue < 0.015526693)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.37932229)) {
              sum += (float)-0.014011879;
            } else {
              sum += (float)0.016289176;
            }
          } else {
            if ( (data[3].fvalue < 0.24465235)) {
              sum += (float)0.13522965;
            } else {
              sum += (float)0.021494396;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.4408168)) {
            sum += (float)-0.16410673;
          } else {
            if ( (data[15].fvalue < 0.00050337164)) {
              sum += (float)-0.089415185;
            } else {
              sum += (float)-0.0095927967;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.48547843)) {
          if ( (data[14].fvalue < 0.12682211)) {
            if ( (data[13].fvalue < 0.23943907)) {
              sum += (float)-0.095517233;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.03539763;
          }
        } else {
          if ( (data[17].fvalue < 0.011640128)) {
            sum += (float)0.10946953;
          } else {
            sum += (float)0.036126405;
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.35602975)) {
      sum += (float)-0;
    } else {
      sum += (float)0.097822905;
    }
  }
  if ( (data[10].fvalue < 1.5)) {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[5].fvalue < 1.1169291)) {
        if ( (data[14].fvalue < 0.33389655)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.042534456)) {
              sum += (float)0.015025822;
            } else {
              sum += (float)-0.021366438;
            }
          } else {
            if ( (data[13].fvalue < 0.24232048)) {
              sum += (float)0.0065929261;
            } else {
              sum += (float)-0.080738634;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.99201888)) {
            if ( (data[14].fvalue < 1.1186954)) {
              sum += (float)0.022563148;
            } else {
              sum += (float)-0.059181321;
            }
          } else {
            if ( (data[14].fvalue < 0.62158328)) {
              sum += (float)-0;
            } else {
              sum += (float)0.107474;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.11735222)) {
          if ( (data[3].fvalue < 0.2517584)) {
            sum += (float)-0;
          } else {
            if ( (data[6].fvalue < 0.058903955)) {
              sum += (float)0.12185388;
            } else {
              sum += (float)0.037875038;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.63851392)) {
            sum += (float)-0.037962791;
          } else {
            if ( (data[13].fvalue < 0.29028198)) {
              sum += (float)0.096847691;
            } else {
              sum += (float)0.0039953399;
            }
          }
        }
      }
    } else {
      sum += (float)0.12213732;
    }
  } else {
    if ( (data[14].fvalue < 0.094400018)) {
      sum += (float)-0;
    } else {
      sum += (float)-0.16264167;
    }
  }
  if ( (data[17].fvalue < 0.025425535)) {
    if ( (data[15].fvalue < 0.16693865)) {
      if ( (data[15].fvalue < 0.033992782)) {
        if ( (data[13].fvalue < 3.0827387e-08)) {
          if ( (data[6].fvalue < 0.21277818)) {
            sum += (float)0.025444647;
          } else {
            sum += (float)0.26197976;
          }
        } else {
          if ( (data[15].fvalue < 0.0005682453)) {
            if ( (data[5].fvalue < 0.26860052)) {
              sum += (float)-0.041542117;
            } else {
              sum += (float)0.0076534306;
            }
          } else {
            sum += (float)0.10885663;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.2401067)) {
          if ( (data[1].fvalue < 2.7647374)) {
            if ( (data[13].fvalue < 0.14182279)) {
              sum += (float)-0;
            } else {
              sum += (float)0.071866304;
            }
          } else {
            sum += (float)-0.020788232;
          }
        } else {
          if ( (data[3].fvalue < 0.90930259)) {
            if ( (data[13].fvalue < 0.22370294)) {
              sum += (float)-0.07495787;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[0].fvalue < 4.526638)) {
              sum += (float)-0.14168794;
            } else {
              sum += (float)-0.062131308;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.025886737)) {
        if ( (data[15].fvalue < 2.6370327)) {
          if ( (data[4].fvalue < 1.8021841)) {
            if ( (data[3].fvalue < 0.38278663)) {
              sum += (float)0.038503408;
            } else {
              sum += (float)-0.058924854;
            }
          } else {
            if ( (data[15].fvalue < 1.7681026)) {
              sum += (float)0.078004695;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)-0.12055933;
        }
      } else {
        if ( (data[3].fvalue < 0.14406395)) {
          sum += (float)0.20023672;
        } else {
          if ( (data[14].fvalue < 0.008311782)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0096019842;
            } else {
              sum += (float)0.092635542;
            }
          } else {
            if (  (data[4].fvalue < 1.7325761)) {
              sum += (float)0.11311781;
            } else {
              sum += (float)0.033768684;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.1691792)) {
      if ( (data[3].fvalue < 0.25859031)) {
        sum += (float)-0;
      } else {
        if ( (data[15].fvalue < 0.86930549)) {
          if ( (data[16].fvalue < 0.099211961)) {
            if ( (data[17].fvalue < 0.053891495)) {
              sum += (float)-0.036529765;
            } else {
              sum += (float)-0.10460203;
            }
          } else {
            sum += (float)-0.0118238;
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[5].fvalue < 0.46474379)) {
        if ( (data[13].fvalue < 0.28854212)) {
          if ( (data[14].fvalue < 0.0077764988)) {
            if ( (data[17].fvalue < 0.76287127)) {
              sum += (float)0.026303582;
            } else {
              sum += (float)-0.14637706;
            }
          } else {
            if (  (data[4].fvalue < 1.6065316)) {
              sum += (float)0.07596302;
            } else {
              sum += (float)-5.5201592e-05;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.74371469)) {
            if (  (data[4].fvalue < 1.711484)) {
              sum += (float)0.073438764;
            } else {
              sum += (float)-0.038306314;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.10149784;
            } else {
              sum += (float)0.0047602919;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.51493418)) {
          if ( (data[14].fvalue < 0.10163657)) {
            sum += (float)-0.11027159;
          } else {
            if ( (data[3].fvalue < 2.8960567)) {
              sum += (float)0.00077953422;
            } else {
              sum += (float)-0.051635079;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.67558229)) {
            sum += (float)-0.035315201;
          } else {
            sum += (float)0.018024504;
          }
        }
      }
    }
  }
  if ( (data[17].fvalue < 3.3552194)) {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[8].fvalue < 5.5)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.1563541)) {
            if ( (data[14].fvalue < 0.11787119)) {
              sum += (float)-0.025748327;
            } else {
              sum += (float)0.0035056842;
            }
          } else {
            if ( (data[13].fvalue < 0.91401684)) {
              sum += (float)0.024058243;
            } else {
              sum += (float)-0.028070716;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.074499637)) {
            sum += (float)0.090559088;
          } else {
            if ( (data[14].fvalue < 0.092475921)) {
              sum += (float)-0.07713154;
            } else {
              sum += (float)-0.0091099516;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.59289169)) {
          sum += (float)0.027641719;
        } else {
          sum += (float)0.099243127;
        }
      }
    } else {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[14].fvalue < 0.076334402)) {
            sum += (float)0.013727359;
          } else {
            if ( (data[13].fvalue < 0.5123297)) {
              sum += (float)0.03453457;
            } else {
              sum += (float)0.18837519;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.5486685)) {
            sum += (float)0.08401835;
          } else {
            if ( (data[15].fvalue < 0.44395143)) {
              sum += (float)-0.044939246;
            } else {
              sum += (float)0.046232641;
            }
          }
        }
      } else {
        sum += (float)-0.093842611;
      }
    }
  } else {
    sum += (float)-0.10544057;
  }
  if ( (data[3].fvalue < 0.14688352)) {
    if ( (data[7].fvalue < 0.5)) {
      sum += (float)-0.048607379;
    } else {
      if ( (data[8].fvalue < 0.5)) {
        if (  (data[4].fvalue < 1.2373339)) {
          if ( (data[13].fvalue < 0.035551064)) {
            sum += (float)0.064955719;
          } else {
            if ( (data[6].fvalue < 0.06642969)) {
              sum += (float)-0.12495449;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[0].fvalue < 3.4837599)) {
            sum += (float)0.10315282;
          } else {
            sum += (float)0.03133494;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1517956)) {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.070665844;
          } else {
            sum += (float)0.23734595;
          }
        } else {
          if ( (data[15].fvalue < 0.0035074973)) {
            if (  (data[4].fvalue < 1.3269024)) {
              sum += (float)-0.074025013;
            } else {
              sum += (float)0.023566108;
            }
          } else {
            sum += (float)0.093652628;
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.26778698)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.46245804)) {
          if ( (data[15].fvalue < 0.85080582)) {
            if ( (data[17].fvalue < 1.7222924)) {
              sum += (float)0.016003145;
            } else {
              sum += (float)-0.16500339;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.12850972;
            } else {
              sum += (float)0.032328732;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.80307198)) {
            if ( (data[4].fvalue < 1.4405284)) {
              sum += (float)0.086952329;
            } else {
              sum += (float)-0.025283361;
            }
          } else {
            if ( (data[5].fvalue < 0.45385158)) {
              sum += (float)0.10175578;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.036869109)) {
          sum += (float)-0.20197858;
        } else {
          if ( (data[3].fvalue < 0.38412476)) {
            if (  (data[4].fvalue < 1.1255283)) {
              sum += (float)0.075281203;
            } else {
              sum += (float)-0.035554085;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.072798483;
            } else {
              sum += (float)0.00083182805;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.99124658)) {
        if ( (data[17].fvalue < 1.3707123)) {
          if ( (data[4].fvalue < 1.8500218)) {
            if ( (data[14].fvalue < 0.16898575)) {
              sum += (float)-0.0032695592;
            } else {
              sum += (float)0.049986336;
            }
          } else {
            if ( (data[16].fvalue < 0.46332809)) {
              sum += (float)-0.067123659;
            } else {
              sum += (float)0.042810243;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.67262793)) {
            sum += (float)-0;
          } else {
            sum += (float)0.12366428;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.84260762)) {
          if ( (data[3].fvalue < 2.582262)) {
            if (  (data[4].fvalue < 2.5244291)) {
              sum += (float)0.12890342;
            } else {
              sum += (float)0.0015558596;
            }
          } else {
            sum += (float)-0.043691564;
          }
        } else {
          if ( (data[15].fvalue < 1.5249776)) {
            sum += (float)0.015532504;
          } else {
            sum += (float)0.1734603;
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.3212533)) {
    if ( (data[13].fvalue < 1.0140228)) {
      if (  (data[4].fvalue < 2.7837625)) {
        if ( (data[14].fvalue < 0.89129519)) {
          if ( (data[15].fvalue < 0.13293396)) {
            if ( (data[6].fvalue < 0.59942359)) {
              sum += (float)-0;
            } else {
              sum += (float)0.054246921;
            }
          } else {
            if ( (data[17].fvalue < 0.050233223)) {
              sum += (float)0.056947358;
            } else {
              sum += (float)0.011666278;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.48338425)) {
            sum += (float)-0.10351533;
          } else {
            sum += (float)0.040318042;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.027261596)) {
          if ( (data[0].fvalue < 6.6385441)) {
            sum += (float)0.030845841;
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            sum += (float)-0.018544322;
          } else {
            sum += (float)-0.10930134;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.5384378)) {
        if ( (data[15].fvalue < 0.011958242)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.14122893;
        }
      } else {
        if ( (data[2].fvalue < 0.40416133)) {
          sum += (float)-0.0029630619;
        } else {
          sum += (float)0.09602198;
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.15722013)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[13].fvalue < 3.0827387e-08)) {
          sum += (float)0.069125809;
        } else {
          if ( (data[14].fvalue < 0.023168713)) {
            if ( (data[14].fvalue < 4.559254e-07)) {
              sum += (float)-0.008491585;
            } else {
              sum += (float)-0.11204758;
            }
          } else {
            if ( (data[13].fvalue < 0.12276864)) {
              sum += (float)-0.077808097;
            } else {
              sum += (float)0.081741832;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.26032996)) {
          sum += (float)0.054137379;
        } else {
          if ( (data[14].fvalue < 0.048501268)) {
            sum += (float)-0.24228235;
          } else {
            if ( (data[15].fvalue < 0.074802473)) {
              sum += (float)-0.10062458;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.07366275)) {
        if ( (data[1].fvalue < 0.91351366)) {
          if ( (data[14].fvalue < 0.60530013)) {
            sum += (float)-0.10519125;
          } else {
            sum += (float)-0.014669237;
          }
        } else {
          if ( (data[15].fvalue < 0.26942036)) {
            if ( (data[4].fvalue < 1.4011405)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.07271003;
            }
          } else {
            if ( (data[14].fvalue < 0.37460199)) {
              sum += (float)-0.012202696;
            } else {
              sum += (float)0.032319468;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 1.3816109)) {
          if ( (data[13].fvalue < 0.26108438)) {
            if ( (data[6].fvalue < 0.21214814)) {
              sum += (float)0.06773933;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 1.3055009)) {
              sum += (float)0.13383999;
            } else {
              sum += (float)0.03457978;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.81208694)) {
            if ( (data[15].fvalue < 0.52842498)) {
              sum += (float)-0.0018884036;
            } else {
              sum += (float)0.050776932;
            }
          } else {
            sum += (float)-0.074873865;
          }
        }
      }
    }
  }
  if (  (data[4].fvalue < 1.9775739)) {
    if ( (data[3].fvalue < 2.2744465)) {
      if ( (data[8].fvalue < 6.5)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[15].fvalue < 0.00089698553)) {
            sum += (float)-0.23183668;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.044493571;
            } else {
              sum += (float)0.12291472;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0018306191)) {
            sum += (float)0.18554637;
          } else {
            if ( (data[15].fvalue < 0.26677066)) {
              sum += (float)-0.010286961;
            } else {
              sum += (float)0.030901063;
            }
          }
        }
      } else {
        sum += (float)0.092807889;
      }
    } else {
      if ( (data[15].fvalue < 0.022277605)) {
        if ( (data[17].fvalue < 0.00093829597)) {
          sum += (float)0.032409418;
        } else {
          sum += (float)-0.044756871;
        }
      } else {
        if ( (data[15].fvalue < 0.25436524)) {
          sum += (float)-0.15916628;
        } else {
          if ( (data[17].fvalue < 0.13966361)) {
            if ( (data[0].fvalue < 5.1420889)) {
              sum += (float)-0.10434505;
            } else {
              sum += (float)-0.026274281;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    }
  } else {
    if ( (data[12].fvalue < 0.5)) {
      if ( (data[15].fvalue < 0.057595305)) {
        if ( (data[17].fvalue < 2.8684838)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.00066803407)) {
              sum += (float)0.14872801;
            } else {
              sum += (float)0.033762716;
            }
          } else {
            sum += (float)0.006443765;
          }
        } else {
          sum += (float)-0.017239965;
        }
      } else {
        if ( (data[14].fvalue < 0.67191255)) {
          if ( (data[15].fvalue < 3.0398278)) {
            if ( (data[13].fvalue < 1.0681643)) {
              sum += (float)0.012028961;
            } else {
              sum += (float)-0.046042852;
            }
          } else {
            sum += (float)-0.088684775;
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[14].fvalue < 2.2252824)) {
              sum += (float)0.072358429;
            } else {
              sum += (float)-0.027769998;
            }
          } else {
            sum += (float)-0.041885175;
          }
        }
      }
    } else {
      sum += (float)-0.08048746;
    }
  }
  if ( (data[13].fvalue < 0.49727291)) {
    if ( (data[3].fvalue < 0.4749766)) {
      if ( (data[7].fvalue < 0.5)) {
        if (  (data[4].fvalue < 1.2107692)) {
          sum += (float)-0.20714931;
        } else {
          if ( (data[14].fvalue < 0.095609114)) {
            sum += (float)-0.043457717;
          } else {
            if (  (data[4].fvalue < 1.7500827)) {
              sum += (float)0.067854278;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.13361457)) {
          if (  (data[4].fvalue < 1.1549783)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.012856249;
            } else {
              sum += (float)0.15881008;
            }
          } else {
            if ( (data[17].fvalue < 0.12891117)) {
              sum += (float)-0.039833408;
            } else {
              sum += (float)0.063650973;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.704849)) {
            if ( (data[14].fvalue < 0.0047095362)) {
              sum += (float)0.027230849;
            } else {
              sum += (float)0.14349516;
            }
          } else {
            sum += (float)-0.01873355;
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[15].fvalue < 2.1185579)) {
          if ( (data[17].fvalue < 0.073428914)) {
            if ( (data[15].fvalue < 0.0351266)) {
              sum += (float)0.055959426;
            } else {
              sum += (float)0.0005328313;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.031402458;
            } else {
              sum += (float)0.031080468;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.42678678)) {
            sum += (float)-0.13353373;
          } else {
            sum += (float)0.041629884;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.057949699)) {
          sum += (float)-0.20141663;
        } else {
          if ( (data[15].fvalue < 0.18005598)) {
            if ( (data[17].fvalue < 0.21857892)) {
              sum += (float)-0.078819558;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[3].fvalue < 1.6008182)) {
              sum += (float)0.035805546;
            } else {
              sum += (float)-0.013428095;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.67375135)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[14].fvalue < 0.027799295)) {
          if ( (data[15].fvalue < 0.65505564)) {
            if ( (data[15].fvalue < 0.045986861)) {
              sum += (float)0.010030618;
            } else {
              sum += (float)-0.042098619;
            }
          } else {
            if ( (data[16].fvalue < 0.01709931)) {
              sum += (float)0.12744756;
            } else {
              sum += (float)0.054715395;
            }
          }
        } else {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[13].fvalue < 1.4687501)) {
              sum += (float)0.065350875;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.12719752;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.16664979)) {
          sum += (float)-0.18780456;
        } else {
          if ( (data[3].fvalue < 1.7856903)) {
            if (  (data[4].fvalue < 1.8071625)) {
              sum += (float)0.086417049;
            } else {
              sum += (float)-0.015708247;
            }
          } else {
            sum += (float)-0.079661138;
          }
        }
      }
    } else {
      sum += (float)0.10482853;
    }
  }
  if ( (data[5].fvalue < 0.037691176)) {
    if (  (data[4].fvalue < 1.8014832)) {
      sum += (float)0.073640719;
    } else {
      sum += (float)-0.0027075866;
    }
  } else {
    if ( (data[16].fvalue < 0.65228856)) {
      if ( (data[16].fvalue < 0.021832462)) {
        if (  (data[4].fvalue < 2.0336061)) {
          if ( (data[0].fvalue < 3.0402198)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.00069181324;
            } else {
              sum += (float)0.046589486;
            }
          } else {
            if ( (data[6].fvalue < 0.23412544)) {
              sum += (float)-0.0031588541;
            } else {
              sum += (float)-0.026188402;
            }
          }
        } else {
          if ( (data[14].fvalue < 1.6251321)) {
            if ( (data[13].fvalue < 1.7159996)) {
              sum += (float)0.044676416;
            } else {
              sum += (float)-0.038594678;
            }
          } else {
            sum += (float)-0.049422346;
          }
        }
      } else {
        if ( (data[9].fvalue < 0.5)) {
          if ( (data[14].fvalue < 0.10590357)) {
            sum += (float)-0.1261003;
          } else {
            sum += (float)-0.024238391;
          }
        } else {
          if ( (data[6].fvalue < 0.36558658)) {
            if ( (data[0].fvalue < 5.7391291)) {
              sum += (float)0.073209725;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[16].fvalue < 0.28944629)) {
              sum += (float)-0.053606432;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.79249489)) {
        if ( (data[16].fvalue < 1.1705487)) {
          if ( (data[17].fvalue < 0.020938527)) {
            sum += (float)0.079470322;
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[15].fvalue < 0.44430211)) {
            sum += (float)0.026072137;
          } else {
            sum += (float)-0.086404353;
          }
        }
      } else {
        if ( (data[16].fvalue < 1.1921958)) {
          sum += (float)0.015963176;
        } else {
          sum += (float)0.12539683;
        }
      }
    }
  }
  if ( (data[13].fvalue < 2.1941452e-07)) {
    if ( (data[14].fvalue < 2.0004743e-05)) {
      if (  (data[4].fvalue < 1.6420174)) {
        sum += (float)-0.033771798;
      } else {
        sum += (float)0.24247082;
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[4].fvalue < 1.14186)) {
          sum += (float)0.083696224;
        } else {
          if ( (data[14].fvalue < 0.10476625)) {
            sum += (float)-0.13784441;
          } else {
            if ( (data[15].fvalue < 0.16201136)) {
              sum += (float)-0.082112074;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.4177663)) {
          sum += (float)0.012972283;
        } else {
          sum += (float)0.094879545;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.00421821)) {
      sum += (float)-0.20634669;
    } else {
      if ( (data[14].fvalue < 0.031468458)) {
        if ( (data[14].fvalue < 5.3582937e-07)) {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.143)) {
              sum += (float)0.009105647;
            } else {
              sum += (float)-0.031724397;
            }
          } else {
            if ( (data[3].fvalue < 1.0409751)) {
              sum += (float)0.0089312475;
            } else {
              sum += (float)0.10300012;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.061519869)) {
              sum += (float)-0.11313704;
            } else {
              sum += (float)0.078841805;
            }
          } else {
            if ( (data[3].fvalue < 0.27472097)) {
              sum += (float)-0.015206269;
            } else {
              sum += (float)-0.1358059;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 2.5)) {
            if (  (data[4].fvalue < 1.021168)) {
              sum += (float)-0.025414923;
            } else {
              sum += (float)0.0457635;
            }
          } else {
            if ( (data[13].fvalue < 0.46090519)) {
              sum += (float)-0.067638665;
            } else {
              sum += (float)0.038907327;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.098734722)) {
            sum += (float)0.096744277;
          } else {
            if ( (data[14].fvalue < 0.36937976)) {
              sum += (float)-0.04581346;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.08986)) {
    if (  (data[4].fvalue < 1.0319245)) {
      if ( (data[13].fvalue < 0.075592026)) {
        if ( (data[14].fvalue < 8.9116298e-07)) {
          if ( (data[3].fvalue < 0.069483966)) {
            sum += (float)0.0022150702;
          } else {
            sum += (float)0.15673798;
          }
        } else {
          if ( (data[14].fvalue < 0.065751746)) {
            sum += (float)-0.070920177;
          } else {
            sum += (float)0.00061896682;
          }
        }
      } else {
        if ( (data[3].fvalue < 0.20620239)) {
          sum += (float)-0.14826229;
        } else {
          if ( (data[13].fvalue < 0.17420797)) {
            if ( (data[14].fvalue < 0.00018036096)) {
              sum += (float)0.058293324;
            } else {
              sum += (float)-0.00053826073;
            }
          } else {
            if ( (data[3].fvalue < 0.40855148)) {
              sum += (float)-0.13290246;
            } else {
              sum += (float)-0.013997506;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1313795)) {
        if ( (data[5].fvalue < 0.35598904)) {
          sum += (float)0.12411916;
        } else {
          if ( (data[13].fvalue < 0.10916384)) {
            sum += (float)-0.0833719;
          } else {
            if ( (data[3].fvalue < 0.41825145)) {
              sum += (float)0.066701539;
            } else {
              sum += (float)0.013101355;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.59798157)) {
          if ( (data[2].fvalue < 0.3768777)) {
            if ( (data[13].fvalue < 1.0242137)) {
              sum += (float)-0.00027669876;
            } else {
              sum += (float)-0.040194511;
            }
          } else {
            if ( (data[13].fvalue < 0.54029191)) {
              sum += (float)0.0034771413;
            } else {
              sum += (float)0.030261835;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.021877721;
            } else {
              sum += (float)0.035104424;
            }
          } else {
            sum += (float)-0.070097767;
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 0.5)) {
      if ( (data[0].fvalue < 2.6912928)) {
        if ( (data[1].fvalue < 1.2103969)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.038666263;
        }
      } else {
        if ( (data[2].fvalue < 0.18554673)) {
          sum += (float)-0;
        } else {
          sum += (float)0.068050787;
        }
      }
    } else {
      if ( (data[3].fvalue < 0.85251194)) {
        sum += (float)0.10349012;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[17].fvalue < 3.504127)) {
    if ( (data[17].fvalue < 1.1351504)) {
      if ( (data[16].fvalue < 0.46413732)) {
        if ( (data[0].fvalue < 4.2607756)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.11401349)) {
              sum += (float)-0.085552312;
            } else {
              sum += (float)0.00043982302;
            }
          } else {
            if ( (data[3].fvalue < 1.6035739)) {
              sum += (float)0.013545035;
            } else {
              sum += (float)-0.023545953;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.46391663)) {
            if ( (data[15].fvalue < 0.035557345)) {
              sum += (float)-0.0076473444;
            } else {
              sum += (float)-0.036136605;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.012074572;
            } else {
              sum += (float)-0.082235366;
            }
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.026435856)) {
            if ( (data[15].fvalue < 0.45050263)) {
              sum += (float)-0.044684161;
            } else {
              sum += (float)0.050847389;
            }
          } else {
            if ( (data[15].fvalue < 0.20033658)) {
              sum += (float)0.11393224;
            } else {
              sum += (float)0.042529732;
            }
          }
        } else {
          sum += (float)-0.020950843;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.32699221)) {
        if ( (data[13].fvalue < 0.61345011)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.16069305;
            } else {
              sum += (float)-0.013658438;
            }
          } else {
            sum += (float)0.062520854;
          }
        } else {
          if ( (data[3].fvalue < 2.6253219)) {
            sum += (float)0.10014065;
          } else {
            sum += (float)0.022350758;
          }
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          sum += (float)0.12638989;
        } else {
          sum += (float)0.0035559773;
        }
      }
    }
  } else {
    sum += (float)-0.081061512;
  }
  if ( (data[6].fvalue < 0.23368925)) {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[14].fvalue < 0.12507372)) {
          sum += (float)-0.14422815;
        } else {
          sum += (float)0.022210773;
        }
      } else {
        if ( (data[13].fvalue < 0.0034796495)) {
          sum += (float)0.13630667;
        } else {
          if ( (data[6].fvalue < 0.084843412)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.027514908;
            } else {
              sum += (float)0.0038184435;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.021842008;
            } else {
              sum += (float)-0.019459572;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1311936)) {
        sum += (float)0.12756523;
      } else {
        if ( (data[13].fvalue < 0.19577947)) {
          sum += (float)-0;
        } else {
          sum += (float)0.033681016;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[6].fvalue < 0.6333974)) {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.028483542)) {
            if ( (data[13].fvalue < 0.3327471)) {
              sum += (float)-0.019479495;
            } else {
              sum += (float)0.010028421;
            }
          } else {
            if (  (data[4].fvalue < 1.6157658)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.063819699;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.50770527)) {
            sum += (float)0.065395042;
          } else {
            sum += (float)-0.011853932;
          }
        }
      } else {
        if ( (data[16].fvalue < 1.7905307)) {
          if ( (data[16].fvalue < 0.37977695)) {
            if ( (data[13].fvalue < 2.2769313)) {
              sum += (float)0.0092090927;
            } else {
              sum += (float)-0.055125196;
            }
          } else {
            if ( (data[14].fvalue < 0.25912333)) {
              sum += (float)0.070895016;
            } else {
              sum += (float)-0.017700851;
            }
          }
        } else {
          sum += (float)-0.048513368;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.19073331)) {
        if ( (data[13].fvalue < 0.3110401)) {
          sum += (float)0.017153425;
        } else {
          sum += (float)-0.1582209;
        }
      } else {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.1197813)) {
            if ( (data[13].fvalue < 0.033328325)) {
              sum += (float)-0.011597087;
            } else {
              sum += (float)-0.080977224;
            }
          } else {
            if ( (data[14].fvalue < 0.93864214)) {
              sum += (float)0.0040237354;
            } else {
              sum += (float)0.075717367;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.5715265)) {
            sum += (float)-0.019256771;
          } else {
            sum += (float)-0.10454629;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[8].fvalue < 6.5)) {
      if ( (data[7].fvalue < 2.5)) {
        if ( (data[5].fvalue < 0.022693824)) {
          sum += (float)0.076977238;
        } else {
          if ( (data[4].fvalue < 0.76865709)) {
            sum += (float)0.075552136;
          } else {
            if (  (data[4].fvalue < 1.0278212)) {
              sum += (float)-0.016518371;
            } else {
              sum += (float)0.0034550477;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[4].fvalue < 1.5389901)) {
            if ( (data[6].fvalue < 0.12005086)) {
              sum += (float)-0.0091063306;
            } else {
              sum += (float)-0.082198597;
            }
          } else {
            if ( (data[14].fvalue < 0.0029943259)) {
              sum += (float)0.050077632;
            } else {
              sum += (float)-0.020764532;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.1542311)) {
            sum += (float)0.08541619;
          } else {
            if ( (data[13].fvalue < 0.49454278)) {
              sum += (float)-0.030944526;
            } else {
              sum += (float)0.032487083;
            }
          }
        }
      }
    } else {
      sum += (float)0.071232744;
    }
  } else {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[13].fvalue < 0.64119673)) {
        if (  (data[4].fvalue < 1.4160329)) {
          sum += (float)-0;
        } else {
          sum += (float)0.045376848;
        }
      } else {
        sum += (float)0.10746152;
      }
    } else {
      if ( (data[3].fvalue < 0.83145285)) {
        if (  (data[4].fvalue < 1.4167947)) {
          sum += (float)0.11475738;
        } else {
          sum += (float)-0;
        }
      } else {
        if ( (data[5].fvalue < 0.53587127)) {
          if ( (data[8].fvalue < 6.5)) {
            sum += (float)0.0090253428;
          } else {
            sum += (float)-0.047417417;
          }
        } else {
          sum += (float)-0.090492949;
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.17549303)) {
    if ( (data[15].fvalue < 0.13438788)) {
      if ( (data[7].fvalue < 0.5)) {
        sum += (float)-0.09278205;
      } else {
        if ( (data[0].fvalue < 3.5855994)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.057780676)) {
              sum += (float)0.066548847;
            } else {
              sum += (float)-0.021255707;
            }
          } else {
            if ( (data[13].fvalue < 0.040513411)) {
              sum += (float)0.0075534084;
            } else {
              sum += (float)0.10185409;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.13108554)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.00015814065;
            } else {
              sum += (float)-0.048017044;
            }
          } else {
            sum += (float)0.067234628;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.0037926226)) {
        sum += (float)0.0028581871;
      } else {
        sum += (float)0.10129717;
      }
    }
  } else {
    if ( (data[15].fvalue < 0.044245362)) {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[16].fvalue < 0.00042187335)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.010544204)) {
              sum += (float)0.049195666;
            } else {
              sum += (float)-0.013975915;
            }
          } else {
            if ( (data[17].fvalue < 0.10633937)) {
              sum += (float)-0.031574968;
            } else {
              sum += (float)0.031866707;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.19637302)) {
            if ( (data[1].fvalue < 1.5561157)) {
              sum += (float)-0.045749545;
            } else {
              sum += (float)-0.12275602;
            }
          } else {
            sum += (float)0.017930606;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.02248507)) {
          sum += (float)0.13351551;
        } else {
          sum += (float)0.02051104;
        }
      }
    } else {
      if ( (data[15].fvalue < 0.3379865)) {
        if ( (data[13].fvalue < 0.77739942)) {
          if ( (data[14].fvalue < 1.3112071)) {
            if ( (data[4].fvalue < 2.0053303)) {
              sum += (float)-0.022866713;
            } else {
              sum += (float)0.029522978;
            }
          } else {
            sum += (float)-0.0856148;
          }
        } else {
          if ( (data[5].fvalue < 0.44688177)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.15864433;
            } else {
              sum += (float)-0.03967467;
            }
          } else {
            sum += (float)-0.022153269;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.070292667)) {
          if ( (data[14].fvalue < 0.15380269)) {
            if ( (data[5].fvalue < 0.13655998)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.10029233;
            }
          } else {
            if ( (data[17].fvalue < 0.031777374)) {
              sum += (float)0.012983442;
            } else {
              sum += (float)-0.026389387;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.1044693)) {
            if ( (data[5].fvalue < 0.37517232)) {
              sum += (float)0.058198668;
            } else {
              sum += (float)-7.1578936e-05;
            }
          } else {
            if ( (data[15].fvalue < 2.282089)) {
              sum += (float)-0.018084571;
            } else {
              sum += (float)0.036373902;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.016797137)) {
    sum += (float)0.070313901;
  } else {
    if ( (data[0].fvalue < 1.3744272)) {
      if ( (data[5].fvalue < 0.18121865)) {
        sum += (float)0.080383822;
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[3].fvalue < 0.3535375)) {
            if ( (data[3].fvalue < 0.18281183)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.045465596;
            }
          } else {
            if ( (data[2].fvalue < 1.1553848)) {
              sum += (float)0.05074852;
            } else {
              sum += (float)-0.00011345597;
            }
          }
        } else {
          sum += (float)0.05478825;
        }
      }
    } else {
      if ( (data[0].fvalue < 1.5971584)) {
        if ( (data[5].fvalue < 0.35757965)) {
          sum += (float)-0;
        } else {
          if ( (data[6].fvalue < 0.2425774)) {
            sum += (float)-0.014294974;
          } else {
            sum += (float)-0.072809927;
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[17].fvalue < 0.72281897)) {
              sum += (float)-0.0033711097;
            } else {
              sum += (float)0.015369498;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.094627298;
            } else {
              sum += (float)0.01320116;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.053959861)) {
            sum += (float)0.0025668151;
          } else {
            sum += (float)-0.096856236;
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.73868322)) {
    if ( (data[13].fvalue < 0.32340139)) {
      if ( (data[13].fvalue < 2.1941452e-07)) {
        if ( (data[14].fvalue < 6.8384963e-08)) {
          if ( (data[3].fvalue < 0.37265182)) {
            sum += (float)0.015047287;
          } else {
            sum += (float)0.23586109;
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[0].fvalue < 5.4476013)) {
              sum += (float)-0.00043521292;
            } else {
              sum += (float)-0.046597145;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.032375935;
            } else {
              sum += (float)0.10672439;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.0091763809)) {
          sum += (float)-0.14375281;
        } else {
          if ( (data[3].fvalue < 0.35503685)) {
            if ( (data[15].fvalue < 0.13422284)) {
              sum += (float)-0.0029803945;
            } else {
              sum += (float)0.057346907;
            }
          } else {
            if ( (data[14].fvalue < 0.0048681493)) {
              sum += (float)-0.06031533;
            } else {
              sum += (float)-0.0023660907;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.5661197)) {
        if ( (data[14].fvalue < 0.098542802)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.22560553)) {
              sum += (float)-0.019121027;
            } else {
              sum += (float)0.047819491;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.041767575;
            } else {
              sum += (float)-0.089338474;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.59244347)) {
            if (  (data[4].fvalue < 1.3616573)) {
              sum += (float)0.075603895;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.11733035;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.50624216)) {
          if ( (data[15].fvalue < 1.069294)) {
            if ( (data[14].fvalue < 0.47888833)) {
              sum += (float)-0.039998744;
            } else {
              sum += (float)0.046462204;
            }
          } else {
            if ( (data[16].fvalue < 0.056846157)) {
              sum += (float)0.081705853;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.8586657)) {
            if ( (data[3].fvalue < 2.0194507)) {
              sum += (float)0.030957652;
            } else {
              sum += (float)-0.054821543;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.1053862;
            } else {
              sum += (float)0.00050646107;
            }
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.0020939726)) {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[3].fvalue < 1.3185635)) {
          if ( (data[13].fvalue < 0.27261347)) {
            if ( (data[14].fvalue < 4.559254e-07)) {
              sum += (float)0.019516505;
            } else {
              sum += (float)-0.045159981;
            }
          } else {
            if ( (data[3].fvalue < 0.70938969)) {
              sum += (float)-0.11308555;
            } else {
              sum += (float)-0.030901847;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.85361063)) {
            sum += (float)0.057374645;
          } else {
            sum += (float)-0.0051449547;
          }
        }
      } else {
        sum += (float)0.06428761;
      }
    } else {
      if ( (data[14].fvalue < 0.46557152)) {
        sum += (float)-0.088718563;
      } else {
        sum += (float)-0.016513975;
      }
    }
  }
  if ( (data[8].fvalue < 1.5)) {
    if (  (data[4].fvalue < 2.0907683)) {
      if ( (data[7].fvalue < 0.5)) {
        if (  (data[4].fvalue < 1.2186468)) {
          sum += (float)-0.13695319;
        } else {
          if ( (data[15].fvalue < 0.086278923)) {
            sum += (float)0.046578243;
          } else {
            if ( (data[14].fvalue < 0.15086189)) {
              sum += (float)-0.081029259;
            } else {
              sum += (float)-0.0079319328;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.058639113)) {
              sum += (float)-0.0076860716;
            } else {
              sum += (float)-0.097490899;
            }
          } else {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)0.017425025;
            } else {
              sum += (float)0.11080073;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0040063486)) {
              sum += (float)0.22081739;
            } else {
              sum += (float)0.059850853;
            }
          } else {
            if ( (data[14].fvalue < 0.012783048)) {
              sum += (float)-0.12302256;
            } else {
              sum += (float)0.003961815;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 2.7499757)) {
        if ( (data[16].fvalue < 0.0068931719)) {
          if ( (data[14].fvalue < 9.8961391e-05)) {
            if ( (data[13].fvalue < 1.2120888)) {
              sum += (float)0.11696346;
            } else {
              sum += (float)0.0051833703;
            }
          } else {
            if ( (data[14].fvalue < 0.3938944)) {
              sum += (float)-0.016932171;
            } else {
              sum += (float)0.056102924;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.58039749)) {
            if ( (data[17].fvalue < 0.56243026)) {
              sum += (float)0.078327924;
            } else {
              sum += (float)-0.001674632;
            }
          } else {
            if ( (data[13].fvalue < 0.26045871)) {
              sum += (float)-0.040725131;
            } else {
              sum += (float)0.007613678;
            }
          }
        }
      } else {
        sum += (float)-0.030343739;
      }
    }
  } else {
    if ( (data[6].fvalue < 0.050178707)) {
      sum += (float)0.10509966;
    } else {
      if ( (data[14].fvalue < 0.038981594)) {
        sum += (float)-0.11461988;
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[2].fvalue < 0.50099081)) {
            if ( (data[14].fvalue < 0.40197894)) {
              sum += (float)0.096634999;
            } else {
              sum += (float)0.029895976;
            }
          } else {
            sum += (float)0.010956704;
          }
        } else {
          if ( (data[13].fvalue < 0.051461466)) {
            if ( (data[5].fvalue < 0.45136571)) {
              sum += (float)-0.033247408;
            } else {
              sum += (float)-0.15761754;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)-0.00081352092;
            } else {
              sum += (float)-0.098456226;
            }
          }
        }
      }
    }
  }
  if ( (data[15].fvalue < 0.18101016)) {
    if ( (data[15].fvalue < 0.052306939)) {
      if ( (data[15].fvalue < 2.142087e-05)) {
        if ( (data[13].fvalue < 1.1401206)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.063596226;
          } else {
            if ( (data[13].fvalue < 0.0035175118)) {
              sum += (float)0.090594739;
            } else {
              sum += (float)-0.0037862253;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.76307988)) {
            if ( (data[17].fvalue < 0.039572589)) {
              sum += (float)0.043597978;
            } else {
              sum += (float)-0.0020908588;
            }
          } else {
            sum += (float)0.096249335;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.51653177)) {
          if ( (data[15].fvalue < 0.028632479)) {
            sum += (float)0.12848276;
          } else {
            sum += (float)0.030969055;
          }
        } else {
          sum += (float)-0.028127031;
        }
      }
    } else {
      if ( (data[15].fvalue < 0.10318483)) {
        if ( (data[13].fvalue < 0.016059659)) {
          sum += (float)-0;
        } else {
          if ( (data[0].fvalue < 2.8639925)) {
            sum += (float)-0.0072729625;
          } else {
            if ( (data[6].fvalue < 0.18347977)) {
              sum += (float)-0.030009232;
            } else {
              sum += (float)-0.099491194;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.3918071)) {
          if (  (data[4].fvalue < 1.255549)) {
            sum += (float)-0;
          } else {
            sum += (float)0.059953336;
          }
        } else {
          if ( (data[16].fvalue < 0.0696242)) {
            if ( (data[6].fvalue < 0.45177141)) {
              sum += (float)-0.083663754;
            } else {
              sum += (float)-0.01467491;
            }
          } else {
            sum += (float)0.0057726218;
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 2.9784491)) {
      if ( (data[17].fvalue < 0.052643217)) {
        if ( (data[13].fvalue < 0.1687737)) {
          if ( (data[14].fvalue < 0.045869783)) {
            if ( (data[5].fvalue < 0.14643583)) {
              sum += (float)0.0081332382;
            } else {
              sum += (float)-0.063003078;
            }
          } else {
            if (  (data[4].fvalue < 1.4404207)) {
              sum += (float)0.065668032;
            } else {
              sum += (float)0.012534059;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7230628)) {
            if ( (data[5].fvalue < 0.43425041)) {
              sum += (float)0.10437969;
            } else {
              sum += (float)0.015937321;
            }
          } else {
            if ( (data[15].fvalue < 0.52612722)) {
              sum += (float)-0.033223268;
            } else {
              sum += (float)0.04142978;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.25778502)) {
          if (  (data[4].fvalue < 1.7960492)) {
            sum += (float)-0;
          } else {
            if ( (data[16].fvalue < 0.13592087)) {
              sum += (float)-0.10181712;
            } else {
              sum += (float)-0.01557612;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.68574595)) {
            if ( (data[17].fvalue < 0.39218783)) {
              sum += (float)-0.00086241623;
            } else {
              sum += (float)0.027278582;
            }
          } else {
            sum += (float)-0.019742735;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.0048178313)) {
        sum += (float)-0.077132724;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[13].fvalue < 3.0827387e-08)) {
    if ( (data[14].fvalue < 1.3580733e-05)) {
      if ( (data[3].fvalue < 0.31982407)) {
        sum += (float)0.0055698538;
      } else {
        sum += (float)0.17522098;
      }
    } else {
      if ( (data[4].fvalue < 1.2119737)) {
        sum += (float)0.080591381;
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.14319754)) {
            sum += (float)-0.074841872;
          } else {
            if ( (data[15].fvalue < 0.26835656)) {
              sum += (float)-0.057970725;
            } else {
              sum += (float)0.0040030414;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.67783701)) {
            sum += (float)0.053948727;
          } else {
            sum += (float)2.0992691e-05;
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.0039274497)) {
      sum += (float)-0.16393131;
    } else {
      if ( (data[14].fvalue < 0.4741275)) {
        if ( (data[7].fvalue < 7.5)) {
          if ( (data[15].fvalue < 0.12330543)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0031360309;
            } else {
              sum += (float)-0.032824557;
            }
          } else {
            if (  (data[4].fvalue < 1.7325125)) {
              sum += (float)0.023549547;
            } else {
              sum += (float)-0.012427147;
            }
          }
        } else {
          sum += (float)0.067246832;
        }
      } else {
        if (  (data[4].fvalue < 1.5758791)) {
          if ( (data[3].fvalue < 1.5698464)) {
            sum += (float)0.093076043;
          } else {
            sum += (float)0.0015205381;
          }
        } else {
          if ( (data[13].fvalue < 0.45096382)) {
            if ( (data[16].fvalue < 0.12537313)) {
              sum += (float)-0.050584514;
            } else {
              sum += (float)0.032408394;
            }
          } else {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)0.089159057;
            } else {
              sum += (float)-0.0042818016;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 3.5)) {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[8].fvalue < 2.5)) {
        if (  (data[4].fvalue < 0.97105026)) {
          if ( (data[5].fvalue < 1.0212559)) {
            if ( (data[13].fvalue < 0.063284174)) {
              sum += (float)-0.0030287409;
            } else {
              sum += (float)-0.065112241;
            }
          } else {
            if ( (data[14].fvalue < 2.9965415e-06)) {
              sum += (float)0.01476259;
            } else {
              sum += (float)-0.033949453;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.086935312)) {
            if ( (data[15].fvalue < 0.085932545)) {
              sum += (float)-0;
            } else {
              sum += (float)0.075442329;
            }
          } else {
            if ( (data[14].fvalue < 0.013845802)) {
              sum += (float)-0.0077362549;
            } else {
              sum += (float)0.0063112197;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.3630442)) {
          if (  (data[4].fvalue < 1.0954537)) {
            sum += (float)0.017660076;
          } else {
            if ( (data[13].fvalue < 0.06604939)) {
              sum += (float)-0.01166454;
            } else {
              sum += (float)-0.096596651;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.17464396)) {
            if ( (data[5].fvalue < 0.34942126)) {
              sum += (float)-0.047789227;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.043350518;
          }
        }
      }
    } else {
      sum += (float)0.083629064;
    }
  } else {
    if ( (data[13].fvalue < 0.09997873)) {
      sum += (float)0.12913935;
    } else {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.092235237)) {
          if ( (data[7].fvalue < 4.5)) {
            sum += (float)-0.067504443;
          } else {
            sum += (float)0.00079596112;
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[13].fvalue < 0.28119972)) {
              sum += (float)-0.0029564542;
            } else {
              sum += (float)0.077205054;
            }
          } else {
            if ( (data[3].fvalue < 1.2029026)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.051573686;
            }
          }
        }
      } else {
        sum += (float)-0.060067464;
      }
    }
  }
  if ( (data[16].fvalue < 0.64712769)) {
    if ( (data[11].fvalue < 1.5)) {
      if ( (data[17].fvalue < 2.2668185)) {
        if ( (data[3].fvalue < 0.31156552)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.089867972)) {
              sum += (float)-0.0090208827;
            } else {
              sum += (float)0.040979516;
            }
          } else {
            if (  (data[4].fvalue < 1.1705501)) {
              sum += (float)0.21858846;
            } else {
              sum += (float)0.0026879515;
            }
          }
        } else {
          if ( (data[14].fvalue < 4.559254e-07)) {
            if ( (data[15].fvalue < 0.0061051822)) {
              sum += (float)0.019957677;
            } else {
              sum += (float)-0.016541999;
            }
          } else {
            if ( (data[17].fvalue < 0.39028001)) {
              sum += (float)-0.015989305;
            } else {
              sum += (float)0.021180442;
            }
          }
        }
      } else {
        sum += (float)-0.074569829;
      }
    } else {
      if ( (data[17].fvalue < 1.7006536)) {
        if ( (data[3].fvalue < 1.2702227)) {
          if ( (data[13].fvalue < 0.5200358)) {
            if ( (data[14].fvalue < 0.19720051)) {
              sum += (float)0.088759206;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[14].fvalue < 0.45239782)) {
            sum += (float)-0.0056382231;
          } else {
            sum += (float)-0.087458432;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.01548738)) {
          sum += (float)0.11839518;
        } else {
          sum += (float)0.014646411;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.94408703)) {
      if (  (data[4].fvalue < 2.7764852)) {
        if ( (data[17].fvalue < 0.024185926)) {
          if ( (data[14].fvalue < 0.026447281)) {
            sum += (float)0.014722871;
          } else {
            sum += (float)0.084116146;
          }
        } else {
          sum += (float)-0.013513711;
        }
      } else {
        sum += (float)-0.049535725;
      }
    } else {
      sum += (float)0.069046766;
    }
  }
  if ( (data[7].fvalue < 3.5)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[17].fvalue < 1.6222897)) {
          if ( (data[14].fvalue < 1.3841884)) {
            if ( (data[6].fvalue < 0.38358203)) {
              sum += (float)-0.0042642988;
            } else {
              sum += (float)0.011803415;
            }
          } else {
            if ( (data[15].fvalue < 0.4973138)) {
              sum += (float)-0.11248365;
            } else {
              sum += (float)0.015754396;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.2560277)) {
            if ( (data[13].fvalue < 0.68769205)) {
              sum += (float)-0.098356187;
            } else {
              sum += (float)0.025730981;
            }
          } else {
            sum += (float)0.04377263;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.046013087)) {
          sum += (float)-0.14151812;
        } else {
          if ( (data[16].fvalue < 0.10629875)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.013757714;
            } else {
              sum += (float)-0.02870102;
            }
          } else {
            if ( (data[15].fvalue < 0.66596603)) {
              sum += (float)0.084064558;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.47882265)) {
        sum += (float)-0;
      } else {
        sum += (float)0.072170354;
      }
    }
  } else {
    if (  (data[4].fvalue < 1.1169991)) {
      sum += (float)0.097638816;
    } else {
      if ( (data[13].fvalue < 0.63088399)) {
        if ( (data[15].fvalue < 0.041512527)) {
          if ( (data[17].fvalue < 0.15623499)) {
            if ( (data[13].fvalue < 0.31616622)) {
              sum += (float)-0.14277332;
            } else {
              sum += (float)-0.022107011;
            }
          } else {
            sum += (float)0.025336109;
          }
        } else {
          if ( (data[14].fvalue < 0.31436527)) {
            sum += (float)0.099594884;
          } else {
            sum += (float)-0.035789136;
          }
        }
      } else {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[14].fvalue < 0.078655779)) {
            sum += (float)-0;
          } else {
            if ( (data[17].fvalue < 0.0060264478)) {
              sum += (float)0.12663129;
            } else {
              sum += (float)0.050580643;
            }
          }
        } else {
          sum += (float)-0.024545332;
        }
      }
    }
  }
  if ( (data[8].fvalue < 8.5)) {
    if ( (data[2].fvalue < 2.0377741)) {
      if ( (data[1].fvalue < 0.234685)) {
        sum += (float)0.044567175;
      } else {
        if ( (data[17].fvalue < 0.20858642)) {
          if ( (data[17].fvalue < 0.061278827)) {
            if ( (data[16].fvalue < 1.0797026)) {
              sum += (float)-0.00099043397;
            } else {
              sum += (float)0.03966523;
            }
          } else {
            if ( (data[13].fvalue < 0.60198021)) {
              sum += (float)-0.01091034;
            } else {
              sum += (float)-0.06578134;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0065072421)) {
            if ( (data[13].fvalue < 0.11667012)) {
              sum += (float)-0.11904461;
            } else {
              sum += (float)0.006119092;
            }
          } else {
            if (  (data[4].fvalue < 2.0601926)) {
              sum += (float)0.042914469;
            } else {
              sum += (float)0.0014538564;
            }
          }
        }
      }
    } else {
      sum += (float)-0.0377037;
    }
  } else {
    sum += (float)0.049734976;
  }
  if ( (data[2].fvalue < 1.2675521)) {
    if ( (data[13].fvalue < 6.0995615e-07)) {
      if ( (data[14].fvalue < 0.013500364)) {
        if ( (data[15].fvalue < 0.28417891)) {
          sum += (float)0.13580781;
        } else {
          sum += (float)-0;
        }
      } else {
        if ( (data[5].fvalue < 0.25952747)) {
          if ( (data[3].fvalue < 0.26727879)) {
            sum += (float)0.014606755;
          } else {
            if ( (data[15].fvalue < 0.54680967)) {
              sum += (float)-0.052193254;
            } else {
              sum += (float)-0.0042522131;
            }
          }
        } else {
          if ( (data[1].fvalue < 0.93499988)) {
            sum += (float)-0.019220605;
          } else {
            if ( (data[14].fvalue < 0.20903508)) {
              sum += (float)-0;
            } else {
              sum += (float)0.033439938;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.0044140862)) {
        sum += (float)-0.10943262;
      } else {
        if ( (data[15].fvalue < 0.34327957)) {
          if ( (data[3].fvalue < 1.110764)) {
            if (  (data[4].fvalue < 1.7481251)) {
              sum += (float)0.0069249016;
            } else {
              sum += (float)-0.031083845;
            }
          } else {
            if ( (data[13].fvalue < 0.90259635)) {
              sum += (float)-0.034158606;
            } else {
              sum += (float)0.0058270372;
            }
          }
        } else {
          if ( (data[0].fvalue < 2.2633023)) {
            sum += (float)0.060487363;
          } else {
            if ( (data[5].fvalue < 0.10256523)) {
              sum += (float)-0.016963873;
            } else {
              sum += (float)0.013801227;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[2].fvalue < 1.9595581)) {
        if ( (data[2].fvalue < 1.3628391)) {
          sum += (float)-0.037016612;
        } else {
          if ( (data[15].fvalue < 0.047241852)) {
            sum += (float)0.029086715;
          } else {
            sum += (float)-0.0036930561;
          }
        }
      } else {
        sum += (float)-0.046011485;
      }
    } else {
      sum += (float)-0.071155518;
    }
  }
  if ( (data[14].fvalue < 0.55232668)) {
    if ( (data[9].fvalue < 1.5)) {
      if ( (data[15].fvalue < 1.7033315)) {
        if ( (data[13].fvalue < 3.0827387e-08)) {
          if ( (data[15].fvalue < 0.31611359)) {
            if ( (data[17].fvalue < 0.0038061694)) {
              sum += (float)0.075584486;
            } else {
              sum += (float)-0.0074038198;
            }
          } else {
            if (  (data[4].fvalue < 1.5015838)) {
              sum += (float)-0.060472455;
            } else {
              sum += (float)0.0080520771;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.012769761)) {
            if (  (data[4].fvalue < 0.99642342)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.088085257;
            }
          } else {
            if ( (data[16].fvalue < 1.493643)) {
              sum += (float)-0.0054171449;
            } else {
              sum += (float)0.044052318;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.20242612)) {
          if ( (data[14].fvalue < 0.16142346)) {
            sum += (float)-0.16178645;
          } else {
            sum += (float)-0.047987614;
          }
        } else {
          sum += (float)0.037625153;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.80315506)) {
        if ( (data[13].fvalue < 0.058755189)) {
          sum += (float)0.086238153;
        } else {
          if ( (data[16].fvalue < 0.042524148)) {
            sum += (float)-0.031352561;
          } else {
            if (  (data[4].fvalue < 2.193697)) {
              sum += (float)0.084487908;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        sum += (float)-0.064746752;
      }
    }
  } else {
    if ( (data[17].fvalue < 1.4572775)) {
      if ( (data[13].fvalue < 0.85191214)) {
        if ( (data[15].fvalue < 1.3183324)) {
          if ( (data[4].fvalue < 1.5085269)) {
            if ( (data[3].fvalue < 1.9308767)) {
              sum += (float)0.065278515;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[16].fvalue < 0.15004656)) {
              sum += (float)-0.044812445;
            } else {
              sum += (float)0.017594511;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.025584629)) {
            if ( (data[14].fvalue < 1.0375221)) {
              sum += (float)0.039740048;
            } else {
              sum += (float)0.13448021;
            }
          } else {
            sum += (float)-0.033662222;
          }
        }
      } else {
        sum += (float)0.085904881;
      }
    } else {
      sum += (float)0.087671198;
    }
  }
  if ( (data[10].fvalue < 1.5)) {
    if ( (data[6].fvalue < 0.87318039)) {
      if ( (data[6].fvalue < 0.27190334)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[15].fvalue < 0.06657286)) {
            sum += (float)-0.16229491;
          } else {
            if ( (data[14].fvalue < 0.091883451)) {
              sum += (float)-0;
            } else {
              sum += (float)0.033633109;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0042386223)) {
            sum += (float)0.089854509;
          } else {
            if ( (data[17].fvalue < 0.13829903)) {
              sum += (float)0.0051909164;
            } else {
              sum += (float)0.044978634;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.53768206)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.055124618)) {
              sum += (float)0.014050466;
            } else {
              sum += (float)-0.0050276848;
            }
          } else {
            sum += (float)-0.051377062;
          }
        } else {
          if ( (data[13].fvalue < 1.09389)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.018211996;
            } else {
              sum += (float)-0.067771152;
            }
          } else {
            if ( (data[17].fvalue < 0.013746477)) {
              sum += (float)0.045747489;
            } else {
              sum += (float)-0.031337239;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.91722101)) {
        if ( (data[3].fvalue < 1.2781816)) {
          sum += (float)0.0037152616;
        } else {
          if ( (data[17].fvalue < 0.74170578)) {
            sum += (float)0.076903917;
          } else {
            sum += (float)0.016823512;
          }
        }
      } else {
        if (  (data[4].fvalue < 2.9830904)) {
          if ( (data[14].fvalue < 0.22360796)) {
            sum += (float)-0.019034699;
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)0.035742987;
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 2.6252532)) {
      sum += (float)-0;
    } else {
      sum += (float)-0.093158953;
    }
  }
  if ( (data[0].fvalue < 0.83057201)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[6].fvalue < 0.34418306)) {
        sum += (float)-0.0057277982;
      } else {
        sum += (float)0.053301364;
      }
    } else {
      sum += (float)0.072341479;
    }
  } else {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[5].fvalue < 0.73289382)) {
        if (  (data[4].fvalue < 1.2453012)) {
          if ( (data[13].fvalue < 0.46505046)) {
            if ( (data[14].fvalue < 0.063564062)) {
              sum += (float)-0.0022689027;
            } else {
              sum += (float)0.035838645;
            }
          } else {
            sum += (float)0.067863144;
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[15].fvalue < 0.00089927006)) {
              sum += (float)-0.010121018;
            } else {
              sum += (float)0.0054138959;
            }
          } else {
            if ( (data[14].fvalue < 0.33592373)) {
              sum += (float)-0.046054587;
            } else {
              sum += (float)0.0048149168;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.29513401)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.059082802;
          } else {
            if ( (data[3].fvalue < 0.35090631)) {
              sum += (float)-0.016299929;
            } else {
              sum += (float)0.0217128;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.73811114)) {
            sum += (float)-0.090656199;
          } else {
            if ( (data[3].fvalue < 1.3674384)) {
              sum += (float)0.0040116534;
            } else {
              sum += (float)-0.04006457;
            }
          }
        }
      }
    } else {
      sum += (float)0.054918379;
    }
  }
  if ( (data[13].fvalue < 0.26862282)) {
    if ( (data[8].fvalue < 6.5)) {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[13].fvalue < 6.0995615e-07)) {
          if ( (data[15].fvalue < 0.094250448)) {
            if ( (data[0].fvalue < 2.8424251)) {
              sum += (float)-0;
            } else {
              sum += (float)0.065806136;
            }
          } else {
            if ( (data[14].fvalue < 1.0571601)) {
              sum += (float)-0.013149404;
            } else {
              sum += (float)0.032575265;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.51624948)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.016878817;
            } else {
              sum += (float)0.023863608;
            }
          } else {
            if ( (data[14].fvalue < 0.047789197)) {
              sum += (float)-0.11434456;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.77913886)) {
          if ( (data[14].fvalue < 0.27776098)) {
            sum += (float)0.10438993;
          } else {
            sum += (float)0.015862141;
          }
        } else {
          sum += (float)-0.033555325;
        }
      }
    } else {
      sum += (float)0.069439143;
    }
  } else {
    if ( (data[11].fvalue < 2.5)) {
      if ( (data[3].fvalue < 2.1077876)) {
        if (  (data[4].fvalue < 1.6232214)) {
          if ( (data[14].fvalue < 0.1264407)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.01867754;
            } else {
              sum += (float)-0.01674764;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.078574367;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.38182309)) {
            if ( (data[16].fvalue < 0.37295973)) {
              sum += (float)-0.034643132;
            } else {
              sum += (float)0.034452327;
            }
          } else {
            if ( (data[15].fvalue < 0.029787261)) {
              sum += (float)0.062183429;
            } else {
              sum += (float)0.0011324283;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 1.4182276)) {
          if ( (data[5].fvalue < 0.54792285)) {
            if ( (data[16].fvalue < 0.094484746)) {
              sum += (float)0.031728741;
            } else {
              sum += (float)-0.044147454;
            }
          } else {
            if ( (data[6].fvalue < 0.64248562)) {
              sum += (float)-0.084875055;
            } else {
              sum += (float)-0.014870014;
            }
          }
        } else {
          if ( (data[0].fvalue < 6.2959342)) {
            if ( (data[15].fvalue < 0.06198971)) {
              sum += (float)0.034703724;
            } else {
              sum += (float)-0.042481627;
            }
          } else {
            sum += (float)0.059920698;
          }
        }
      }
    } else {
      sum += (float)-0.055854909;
    }
  }
  if ( (data[3].fvalue < 0.44022387)) {
    if ( (data[6].fvalue < 0.68438917)) {
      if (  (data[4].fvalue < 1.7695793)) {
        if ( (data[17].fvalue < 0.14112461)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[17].fvalue < 0.033161443)) {
              sum += (float)0.00021073299;
            } else {
              sum += (float)-0.056001186;
            }
          } else {
            if (  (data[4].fvalue < 1.1353247)) {
              sum += (float)0.15767983;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.22926015)) {
            sum += (float)0.11232012;
          } else {
            sum += (float)0.01869585;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.79103452)) {
          if (  (data[4].fvalue < 2.1055202)) {
            sum += (float)-0.086611375;
          } else {
            sum += (float)-0.012467162;
          }
        } else {
          sum += (float)0.026869824;
        }
      }
    } else {
      if ( (data[1].fvalue < 0.41417336)) {
        sum += (float)0.089980841;
      } else {
        sum += (float)0.0058109076;
      }
    }
  } else {
    if (  (data[4].fvalue < 1.8315372)) {
      if ( (data[6].fvalue < 0.56488836)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.39459929)) {
              sum += (float)-0.033175524;
            } else {
              sum += (float)0.011699301;
            }
          } else {
            if (  (data[4].fvalue < 1.2743847)) {
              sum += (float)-0.13137899;
            } else {
              sum += (float)0.01518587;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.078237481)) {
            sum += (float)-0.14940163;
          } else {
            if ( (data[3].fvalue < 0.80793297)) {
              sum += (float)0.030030543;
            } else {
              sum += (float)-0.035889018;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.69933122)) {
          if ( (data[1].fvalue < 0.47350201)) {
            if ( (data[13].fvalue < 0.41040769)) {
              sum += (float)-0.046737768;
            } else {
              sum += (float)0.01351587;
            }
          } else {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.037712507;
            } else {
              sum += (float)-0.089418806;
            }
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[17].fvalue < 0.039152548)) {
          if ( (data[4].fvalue < 2.5244937)) {
            if ( (data[15].fvalue < 2.228404)) {
              sum += (float)0.045720082;
            } else {
              sum += (float)-0.020535056;
            }
          } else {
            if ( (data[3].fvalue < 1.6155488)) {
              sum += (float)3.2479085e-05;
            } else {
              sum += (float)-0.037994795;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.036020175)) {
            if ( (data[13].fvalue < 0.58085018)) {
              sum += (float)-0;
            } else {
              sum += (float)0.034764864;
            }
          } else {
            if ( (data[4].fvalue < 2.2312937)) {
              sum += (float)-0.0374064;
            } else {
              sum += (float)0.0019390886;
            }
          }
        }
      } else {
        sum += (float)-0.068995237;
      }
    }
  }
  if ( (data[17].fvalue < 0.6135543)) {
    if (  (data[4].fvalue < 1.1549557)) {
      if ( (data[7].fvalue < 0.5)) {
        sum += (float)-0.090341039;
      } else {
        if ( (data[13].fvalue < 0.004955071)) {
          sum += (float)0.10020482;
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if (  (data[4].fvalue < 1.0274818)) {
              sum += (float)-0.0010174407;
            } else {
              sum += (float)0.020467464;
            }
          } else {
            if ( (data[3].fvalue < 0.70024151)) {
              sum += (float)0.15646476;
            } else {
              sum += (float)-0.027077066;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.607741)) {
        if ( (data[13].fvalue < 0.34189576)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[3].fvalue < 1.7984779)) {
              sum += (float)-0.0027868776;
            } else {
              sum += (float)0.029992498;
            }
          } else {
            if ( (data[15].fvalue < 5.7513556e-05)) {
              sum += (float)-0.066626489;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.5138922)) {
            if ( (data[14].fvalue < 0.010595664)) {
              sum += (float)0.0074059963;
            } else {
              sum += (float)0.057021741;
            }
          } else {
            if ( (data[15].fvalue < 0.3426016)) {
              sum += (float)-0.021275511;
            } else {
              sum += (float)0.014491854;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.010835266)) {
          if ( (data[13].fvalue < 0.91521108)) {
            sum += (float)-0.0022435216;
          } else {
            sum += (float)0.018141132;
          }
        } else {
          if ( (data[15].fvalue < 0.5764538)) {
            sum += (float)-0.07240063;
          } else {
            sum += (float)-0.0078076324;
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.4124774)) {
      if ( (data[5].fvalue < 0.089931034)) {
        sum += (float)-0.0031143865;
      } else {
        if (  (data[4].fvalue < 1.7984165)) {
          sum += (float)-0;
        } else {
          if ( (data[6].fvalue < 0.47526419)) {
            sum += (float)0.10958635;
          } else {
            if ( (data[3].fvalue < 1.0052681)) {
              sum += (float)-0;
            } else {
              sum += (float)0.057537552;
            }
          }
        }
      }
    } else {
      if ( (data[11].fvalue < 2.5)) {
        if ( (data[14].fvalue < 0.22925159)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.5023905)) {
              sum += (float)-0.11101963;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.045458049;
          }
        } else {
          if ( (data[15].fvalue < 0.45591396)) {
            sum += (float)0.056040999;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        sum += (float)-0.043075915;
      }
    }
  }
  if ( (data[16].fvalue < 0.042258963)) {
    if ( (data[15].fvalue < 0.45077735)) {
      if ( (data[15].fvalue < 0.056342054)) {
        if ( (data[7].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.5680842)) {
            sum += (float)-0.12322496;
          } else {
            sum += (float)0.028222758;
          }
        } else {
          if ( (data[14].fvalue < 0.23519883)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0068291971;
            } else {
              sum += (float)-0.036410261;
            }
          } else {
            if ( (data[4].fvalue < 1.337971)) {
              sum += (float)0.054544054;
            } else {
              sum += (float)0.0075698318;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.5726006)) {
          if ( (data[14].fvalue < 0.0042531826)) {
            if ( (data[13].fvalue < 0.16320352)) {
              sum += (float)-0.079943947;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[15].fvalue < 0.14537638)) {
              sum += (float)-0.013025609;
            } else {
              sum += (float)0.057548665;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.71788859)) {
            if ( (data[4].fvalue < 2.1619062)) {
              sum += (float)-0.075435489;
            } else {
              sum += (float)-0.0046221996;
            }
          } else {
            if ( (data[13].fvalue < 0.15962137)) {
              sum += (float)0.039963853;
            } else {
              sum += (float)-0.015724638;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.68910998)) {
        if ( (data[13].fvalue < 0.91957772)) {
          if ( (data[14].fvalue < 0.0055056885)) {
            if ( (data[0].fvalue < 4.3689213)) {
              sum += (float)-0.065428883;
            } else {
              sum += (float)0.026642887;
            }
          } else {
            if (  (data[4].fvalue < 2.1359975)) {
              sum += (float)0.040442411;
            } else {
              sum += (float)-0.0041816458;
            }
          }
        } else {
          sum += (float)0.0779415;
        }
      } else {
        sum += (float)-0.052522384;
      }
    }
  } else {
    if ( (data[16].fvalue < 0.12374299)) {
      if ( (data[3].fvalue < 0.61764014)) {
        sum += (float)-0;
      } else {
        sum += (float)-0.075985283;
      }
    } else {
      if ( (data[17].fvalue < 1.0748665)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.10339122)) {
            sum += (float)-0.073475651;
          } else {
            if ( (data[13].fvalue < 0.011466088)) {
              sum += (float)-0.03231721;
            } else {
              sum += (float)0.011268507;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7850952)) {
            sum += (float)0.073749207;
          } else {
            if ( (data[13].fvalue < 1.3300581)) {
              sum += (float)-0.009082838;
            } else {
              sum += (float)0.059887215;
            }
          }
        }
      } else {
        sum += (float)-0.055936527;
      }
    }
  }
  if ( (data[0].fvalue < 7.8949518)) {
    if ( (data[17].fvalue < 0.11785745)) {
      if ( (data[17].fvalue < 0.041537665)) {
        if ( (data[4].fvalue < 0.85458636)) {
          if ( (data[13].fvalue < 0.22603449)) {
            sum += (float)0.052478734;
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[11].fvalue < 0.5)) {
            if ( (data[15].fvalue < 2.6367755)) {
              sum += (float)-0.0031400113;
            } else {
              sum += (float)-0.049693651;
            }
          } else {
            if ( (data[14].fvalue < 0.11787571)) {
              sum += (float)0.058748037;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.5555716)) {
              sum += (float)-0.084412262;
            } else {
              sum += (float)-0.0092916107;
            }
          } else {
            sum += (float)0.0079851132;
          }
        } else {
          sum += (float)-0.082436033;
        }
      }
    } else {
      if ( (data[3].fvalue < 0.18931854)) {
        sum += (float)0.073008612;
      } else {
        if ( (data[2].fvalue < 0.13565448)) {
          sum += (float)-0.047044322;
        } else {
          if ( (data[13].fvalue < 0.0025092838)) {
            if ( (data[3].fvalue < 0.97818029)) {
              sum += (float)-0.062950402;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[3].fvalue < 0.51388049)) {
              sum += (float)0.046157621;
            } else {
              sum += (float)0.0067554964;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.057489023;
  }
  sum = sum + (float)(-0);
  if (!pred_margin) {
    return pred_transform(sum);
  } else {
    return sum;
  }
}
};
