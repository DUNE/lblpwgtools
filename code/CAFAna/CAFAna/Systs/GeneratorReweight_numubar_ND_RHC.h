#include "BDTReweighter.h"
class GeneratorReweight_numubar_ND_RHC : public BDTReweighter {
 public :
 GeneratorReweight_numubar_ND_RHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_numubar_ND_RHC() {;}


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
  if (  (data[8].fvalue < 5.999999)) {
    if (  (data[13].fvalue < 0.061551429)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.2091137)) {
              sum += (float)-0.17402463;
            } else {
              sum += (float)-0.090890922;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.39017114;
            } else {
              sum += (float)0.40684456;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.0312178)) {
              sum += (float)-0.58271205;
            } else {
              sum += (float)-0.31187287;
            }
          } else {
            if (  (data[13].fvalue < 0.0012544931)) {
              sum += (float)0.40137574;
            } else {
              sum += (float)0.010108077;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[4].fvalue < 1.2864982)) {
            if (  (data[14].fvalue < 0.0062027965)) {
              sum += (float)0.39001673;
            } else {
              sum += (float)-0.001674123;
            }
          } else {
            if (  (data[14].fvalue < 0.045504496)) {
              sum += (float)-0.23404376;
            } else {
              sum += (float)0.13099098;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.011743468)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.32823831;
            } else {
              sum += (float)0.18489449;
            }
          } else {
            if (  (data[3].fvalue < 0.30774716)) {
              sum += (float)-0.055963997;
            } else {
              sum += (float)-0.20717961;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 5.999999)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 0.02258683)) {
            if (  (data[17].fvalue < 0.026695976)) {
              sum += (float)0.034820661;
            } else {
              sum += (float)-0.15154126;
            }
          } else {
            if (  (data[4].fvalue < 1.0419295)) {
              sum += (float)-0.080530085;
            } else {
              sum += (float)0.16464193;
            }
          }
        } else {
          if (  (data[8].fvalue < 4.999999)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.10982867;
            } else {
              sum += (float)0.027915394;
            }
          } else {
            if (  (data[6].fvalue < 0.1068396)) {
              sum += (float)0.31666917;
            } else {
              sum += (float)0.0525239;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 7.999999)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.33919618)) {
              sum += (float)0.32998157;
            } else {
              sum += (float)0.53281415;
            }
          } else {
            if (  (data[3].fvalue < 0.75131869)) {
              sum += (float)0.34666917;
            } else {
              sum += (float)0.19279285;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.60299766)) {
            sum += (float)0.5249669;
          } else {
            sum += (float)0.33831984;
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 7.999999)) {
      if (  (data[14].fvalue < 0.15568486)) {
        if (  (data[13].fvalue < 0.19749948)) {
          if (  (data[4].fvalue < 1.084179)) {
            if (  (data[3].fvalue < 0.56681013)) {
              sum += (float)0.58110726;
            } else {
              sum += (float)0.21053858;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.28038862;
            } else {
              sum += (float)0.54033917;
            }
          }
        } else {
          if (  (data[7].fvalue < 5.999999)) {
            if (  (data[6].fvalue < 0.27567282)) {
              sum += (float)0.2363513;
            } else {
              sum += (float)-0.11777426;
            }
          } else {
            if (  (data[7].fvalue < 6.999999)) {
              sum += (float)0.24878673;
            } else {
              sum += (float)0.47634631;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.00033797399)) {
          if (  (data[8].fvalue < 6.999999)) {
            if (  (data[13].fvalue < 0.11989373)) {
              sum += (float)0.11102278;
            } else {
              sum += (float)0.26155692;
            }
          } else {
            if (  (data[7].fvalue < 5.999999)) {
              sum += (float)0.29205742;
            } else {
              sum += (float)0.42617777;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.5563322)) {
              sum += (float)-0.31509304;
            } else {
              sum += (float)-0.064059645;
            }
          } else {
            if (  (data[5].fvalue < 0.46077093)) {
              sum += (float)0.13979413;
            } else {
              sum += (float)-0.19741558;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.28443125)) {
        if (  (data[7].fvalue < 5.999999)) {
          if (  (data[4].fvalue < 1.1814461)) {
            if (  (data[3].fvalue < 0.79212958)) {
              sum += (float)0.58524644;
            } else {
              sum += (float)0.32235947;
            }
          } else {
            if (  (data[16].fvalue < 0.028372657)) {
              sum += (float)0.37786365;
            } else {
              sum += (float)0.54660672;
            }
          }
        } else {
          sum += (float)0.59404951;
        }
      } else {
        if (  (data[8].fvalue < 9.999999)) {
          if (  (data[15].fvalue < 0.00033797399)) {
            if (  (data[3].fvalue < 1.1900239)) {
              sum += (float)0.44476846;
            } else {
              sum += (float)0.2743265;
            }
          } else {
            if (  (data[5].fvalue < 0.42689064)) {
              sum += (float)0.21422665;
            } else {
              sum += (float)-0.058358599;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.00033797399)) {
            if (  (data[3].fvalue < 1.3125869)) {
              sum += (float)0.56534559;
            } else {
              sum += (float)0.45830134;
            }
          } else {
            if (  (data[3].fvalue < 1.1900239)) {
              sum += (float)0.42785528;
            } else {
              sum += (float)0.19449736;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 5.999999)) {
    if (  (data[13].fvalue < 0.16189627)) {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.019665381)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.028938269)) {
              sum += (float)-0.062865548;
            } else {
              sum += (float)-0.14217231;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.033545747;
            } else {
              sum += (float)-0.19242142;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.47356424;
            } else {
              sum += (float)-0.15672253;
            }
          } else {
            if (  (data[4].fvalue < 1.0421709)) {
              sum += (float)-0.059104033;
            } else {
              sum += (float)0.11640724;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 3.999999)) {
          if (  (data[16].fvalue < 0.12452018)) {
            if (  (data[13].fvalue < 0.034860574)) {
              sum += (float)-0.31741267;
            } else {
              sum += (float)-0.17382695;
            }
          } else {
            if (  (data[3].fvalue < 0.46044865)) {
              sum += (float)0.065896571;
            } else {
              sum += (float)-0.13381004;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.14814024)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.083347812;
            } else {
              sum += (float)0.39486343;
            }
          } else {
            if (  (data[4].fvalue < 1.1541923)) {
              sum += (float)0.067603223;
            } else {
              sum += (float)-0.20785208;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 4.999999)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.011480323)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.029216683;
            } else {
              sum += (float)0.21612689;
            }
          } else {
            if (  (data[4].fvalue < 1.1021502)) {
              sum += (float)0.067909814;
            } else {
              sum += (float)0.21039532;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.12476605)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.26595494;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.16021353;
            } else {
              sum += (float)-0.011838866;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.44114336)) {
          if (  (data[3].fvalue < 0.88955063)) {
            if (  (data[14].fvalue < 0.11007047)) {
              sum += (float)0.081190087;
            } else {
              sum += (float)0.21213742;
            }
          } else {
            if (  (data[5].fvalue < 0.4789598)) {
              sum += (float)0.09792605;
            } else {
              sum += (float)-0.094275549;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 0.65305805)) {
              sum += (float)0.39580151;
            } else {
              sum += (float)0.1211747;
            }
          } else {
            if (  (data[7].fvalue < 6.999999)) {
              sum += (float)0.10439029;
            } else {
              sum += (float)0.29691479;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 7.999999)) {
      if (  (data[14].fvalue < 0.13239881)) {
        if (  (data[13].fvalue < 0.14657077)) {
          if (  (data[14].fvalue < 0.089199208)) {
            if (  (data[3].fvalue < 0.42453077)) {
              sum += (float)0.44991967;
            } else {
              sum += (float)0.33308256;
            }
          } else {
            if (  (data[4].fvalue < 1.1021502)) {
              sum += (float)0.41487226;
            } else {
              sum += (float)0.28886458;
            }
          }
        } else {
          if (  (data[7].fvalue < 5.999999)) {
            if (  (data[4].fvalue < 1.2038109)) {
              sum += (float)0.29072863;
            } else {
              sum += (float)-0.017455833;
            }
          } else {
            if (  (data[3].fvalue < 0.52103263)) {
              sum += (float)0.39390269;
            } else {
              sum += (float)0.22747642;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 5.999999)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 6.999999)) {
              sum += (float)0.27599105;
            } else {
              sum += (float)0.37884519;
            }
          } else {
            if (  (data[13].fvalue < 0.046935342)) {
              sum += (float)-0.023319334;
            } else {
              sum += (float)0.15159653;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.94738388)) {
            if (  (data[4].fvalue < 1.2644241)) {
              sum += (float)0.45354244;
            } else {
              sum += (float)0.30557698;
            }
          } else {
            if (  (data[7].fvalue < 7.999999)) {
              sum += (float)0.06971252;
            } else {
              sum += (float)0.26894426;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.25053123)) {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[14].fvalue < 0.16453271)) {
            if (  (data[13].fvalue < 0.11970957)) {
              sum += (float)0.45080873;
            } else {
              sum += (float)0.30848312;
            }
          } else {
            if (  (data[4].fvalue < 1.1813976)) {
              sum += (float)0.40772632;
            } else {
              sum += (float)0.22362082;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.3141559)) {
            sum += (float)0.46000955;
          } else {
            sum += (float)0.30399269;
          }
        }
      } else {
        if (  (data[8].fvalue < 9.999999)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.94738388)) {
              sum += (float)0.33633122;
            } else {
              sum += (float)0.19846766;
            }
          } else {
            if (  (data[15].fvalue < 0.17391147)) {
              sum += (float)-0;
            } else {
              sum += (float)0.192812;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.9431472)) {
            if (  (data[3].fvalue < 2.1572862)) {
              sum += (float)0.43572393;
            } else {
              sum += (float)0.19798149;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.31442776;
            } else {
              sum += (float)0.12809937;
            }
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 6.999999)) {
    if (  (data[13].fvalue < 0.03383892)) {
      if (  (data[14].fvalue < 0.172968)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[5].fvalue < 1.1727071)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.094802111;
            } else {
              sum += (float)-0.26747438;
            }
          } else {
            if (  (data[13].fvalue < 0.0031637342)) {
              sum += (float)0.071833454;
            } else {
              sum += (float)-0.1257775;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.058560442)) {
            if (  (data[0].fvalue < 3.2701068)) {
              sum += (float)0.52468026;
            } else {
              sum += (float)0.35775033;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.010741788;
            } else {
              sum += (float)0.29976511;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[5].fvalue < 0.34044144)) {
            if (  (data[15].fvalue < 0.06008229)) {
              sum += (float)0.042319063;
            } else {
              sum += (float)-0.11683311;
            }
          } else {
            if (  (data[4].fvalue < 1.0310684)) {
              sum += (float)-0.077955164;
            } else {
              sum += (float)-0.023599563;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1374457)) {
            if (  (data[3].fvalue < 0.39286035)) {
              sum += (float)0.19613513;
            } else {
              sum += (float)-0.31581041;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.12975982;
            } else {
              sum += (float)-0.36568227;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 4.999999)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.01655711)) {
            if (  (data[17].fvalue < 0.027940655)) {
              sum += (float)0.026939843;
            } else {
              sum += (float)-0.11397143;
            }
          } else {
            if (  (data[4].fvalue < 1.0678465)) {
              sum += (float)-0.041954555;
            } else {
              sum += (float)0.16907419;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.033485852)) {
              sum += (float)-0.25136223;
            } else {
              sum += (float)0.056357719;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)-0.10659527;
            } else {
              sum += (float)0.038673829;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 6.999999)) {
          if (  (data[3].fvalue < 0.48054895)) {
            if (  (data[4].fvalue < 1.1197846)) {
              sum += (float)0.40820694;
            } else {
              sum += (float)0.15702376;
            }
          } else {
            if (  (data[13].fvalue < 0.51361281)) {
              sum += (float)0.012831016;
            } else {
              sum += (float)0.14097433;
            }
          }
        } else {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.59383512)) {
              sum += (float)0.34299082;
            } else {
              sum += (float)0.2609863;
            }
          } else {
            if (  (data[5].fvalue < 0.42803094)) {
              sum += (float)0.22287472;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if (  (data[7].fvalue < 6.999999)) {
      if (  (data[6].fvalue < 0.1753515)) {
        if (  (data[4].fvalue < 1.1374457)) {
          if (  (data[3].fvalue < 0.6820147)) {
            sum += (float)0.40930936;
          } else {
            if (  (data[4].fvalue < 0.96904141)) {
              sum += (float)0.35787022;
            } else {
              sum += (float)0.15352611;
            }
          }
        } else {
          if (  (data[8].fvalue < 7.999999)) {
            if (  (data[16].fvalue < 0.029196728)) {
              sum += (float)0.16698381;
            } else {
              sum += (float)0.38499114;
            }
          } else {
            if (  (data[3].fvalue < 0.3627215)) {
              sum += (float)0.35931575;
            } else {
              sum += (float)0.23697546;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.83957988)) {
          if (  (data[8].fvalue < 8.999999)) {
            if (  (data[14].fvalue < 0.156152)) {
              sum += (float)0.31610456;
            } else {
              sum += (float)0.17482419;
            }
          } else {
            if (  (data[4].fvalue < 1.875312)) {
              sum += (float)0.32914367;
            } else {
              sum += (float)0.18293488;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.69577891)) {
              sum += (float)0.17186266;
            } else {
              sum += (float)0.32877606;
            }
          } else {
            if (  (data[13].fvalue < 0.095965758)) {
              sum += (float)-0.15848181;
            } else {
              sum += (float)0.084818542;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.8193265)) {
        if (  (data[3].fvalue < 1.4797291)) {
          if (  (data[3].fvalue < 1.0164866)) {
            sum += (float)0.39826033;
          } else {
            if (  (data[7].fvalue < 7.999999)) {
              sum += (float)0.2276942;
            } else {
              sum += (float)0.35991263;
            }
          }
        } else {
          if (  (data[7].fvalue < 9.999999)) {
            if (  (data[8].fvalue < 8.999999)) {
              sum += (float)0.034295917;
            } else {
              sum += (float)0.16080388;
            }
          } else {
            sum += (float)0.37526703;
          }
        }
      } else {
        if (  (data[7].fvalue < 9.999999)) {
          if (  (data[13].fvalue < 0.21803959)) {
            sum += (float)0.33377069;
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.19138575;
            } else {
              sum += (float)-0.033895548;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            sum += (float)0.38052264;
          } else {
            sum += (float)0.25661793;
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 6.999999)) {
    if (  (data[13].fvalue < 0.23995799)) {
      if (  (data[12].fvalue < 3.999999)) {
        if (  (data[15].fvalue < 0.00033797399)) {
          if (  (data[8].fvalue < 4.999999)) {
            if (  (data[4].fvalue < 1.2749774)) {
              sum += (float)-0.045669958;
            } else {
              sum += (float)0.0070158741;
            }
          } else {
            if ( (data[4].fvalue < 1.1016054)) {
              sum += (float)0.30209053;
            } else {
              sum += (float)0.0023521499;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 1.4435922)) {
              sum += (float)-0.27668458;
            } else {
              sum += (float)-0.097266883;
            }
          } else {
            if (  (data[4].fvalue < 1.8753901)) {
              sum += (float)0.51821715;
            } else {
              sum += (float)-0.32135844;
            }
          }
        }
      } else {
        sum += (float)0.60424399;
      }
    } else {
      if (  (data[14].fvalue < 0.33910105)) {
        if (  (data[13].fvalue < 1.0404383)) {
          if (  (data[5].fvalue < 0.26899016)) {
            if (  (data[16].fvalue < 0.43269345)) {
              sum += (float)0.067968994;
            } else {
              sum += (float)0.17479873;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.034370687;
            } else {
              sum += (float)-0.095623694;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.66036081)) {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)-0.1862424;
            } else {
              sum += (float)0.15005735;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.25432873;
            } else {
              sum += (float)-0.091461129;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.3823002)) {
          if (  (data[3].fvalue < 1.0999016)) {
            if (  (data[4].fvalue < 1.8753901)) {
              sum += (float)0.16976151;
            } else {
              sum += (float)-0.036679521;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.072187558;
            } else {
              sum += (float)-0.040906087;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.0001807627)) {
            if (  (data[14].fvalue < 0.49720937)) {
              sum += (float)0.21769173;
            } else {
              sum += (float)0.33522618;
            }
          } else {
            if (  (data[16].fvalue < 0.0064961775)) {
              sum += (float)0.18799698;
            } else {
              sum += (float)0.033368491;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 8.999999)) {
      if (  (data[14].fvalue < 0.18281825)) {
        if (  (data[13].fvalue < 0.216194)) {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[4].fvalue < 1.1016054)) {
              sum += (float)0.34517837;
            } else {
              sum += (float)0.19396612;
            }
          } else {
            sum += (float)0.35737357;
          }
        } else {
          if (  (data[7].fvalue < 6.999999)) {
            if (  (data[6].fvalue < 0.2949096)) {
              sum += (float)0.1766053;
            } else {
              sum += (float)-0.12472186;
            }
          } else {
            sum += (float)0.27737561;
          }
        }
      } else {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[3].fvalue < 0.37922654)) {
            if (  (data[4].fvalue < 1.1928625)) {
              sum += (float)0.35433024;
            } else {
              sum += (float)0.17660911;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.20479199;
            } else {
              sum += (float)0.089806698;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.49720937)) {
              sum += (float)-0.27982235;
            } else {
              sum += (float)-0.035882719;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.12790348;
            } else {
              sum += (float)-0.043509938;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.3248972)) {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[14].fvalue < 0.21074265)) {
            sum += (float)0.3431946;
          } else {
            if (  (data[4].fvalue < 1.2538984)) {
              sum += (float)0.29524094;
            } else {
              sum += (float)0.092273735;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.3197365)) {
            sum += (float)0.36476824;
          } else {
            sum += (float)0.24305448;
          }
        }
      } else {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 10.999999)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.25914389;
            } else {
              sum += (float)0.13571678;
            }
          } else {
            if (  (data[8].fvalue < 12.999999)) {
              sum += (float)0.30493334;
            } else {
              sum += (float)0.36667296;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.029331116)) {
            if (  (data[16].fvalue < 0.11056483)) {
              sum += (float)0.09519235;
            } else {
              sum += (float)0.23677528;
            }
          } else {
            if (  (data[15].fvalue < 0.17234001)) {
              sum += (float)-0.096052743;
            } else {
              sum += (float)0.07393907;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 5.999999)) {
    if (  (data[8].fvalue < 5.999999)) {
      if (  (data[5].fvalue < 0.21607494)) {
        if (  (data[4].fvalue < 1.2542306)) {
          if (  (data[4].fvalue < 1.0310427)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.15699908;
            } else {
              sum += (float)0.2694563;
            }
          } else {
            if (  (data[4].fvalue < 1.1205412)) {
              sum += (float)0.14940277;
            } else {
              sum += (float)-0.050720721;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.2774826)) {
            if (  (data[17].fvalue < 0.03046485)) {
              sum += (float)0.055605017;
            } else {
              sum += (float)-0.0041085249;
            }
          } else {
            if (  (data[16].fvalue < 0.66268909)) {
              sum += (float)-0.24348836;
            } else {
              sum += (float)0.087311029;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 1.2849931)) {
          if (  (data[13].fvalue < 0.29741579)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.027769744;
            } else {
              sum += (float)-0.09700346;
            }
          } else {
            if (  (data[14].fvalue < 0.2408383)) {
              sum += (float)-0.0031876483;
            } else {
              sum += (float)0.10371833;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.46529552;
            } else {
              sum += (float)-0.2829009;
            }
          } else {
            if (  (data[14].fvalue < 0.0049465098)) {
              sum += (float)0.27330151;
            } else {
              sum += (float)0.057277977;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.13772312)) {
        if (  (data[4].fvalue < 1.1381648)) {
          if (  (data[3].fvalue < 0.52491093)) {
            if (  (data[4].fvalue < 1.1205412)) {
              sum += (float)0.34933257;
            } else {
              sum += (float)0.24561213;
            }
          } else {
            if (  (data[4].fvalue < 0.99777114)) {
              sum += (float)0.30462682;
            } else {
              sum += (float)0.029787175;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.009231532)) {
            if (  (data[14].fvalue < 0.27381125)) {
              sum += (float)-0.066409573;
            } else {
              sum += (float)0.18434526;
            }
          } else {
            if (  (data[0].fvalue < 4.7521996)) {
              sum += (float)0.36294639;
            } else {
              sum += (float)0.20383641;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 7.999999)) {
          if (  (data[16].fvalue < 0.14412072)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.16641529;
            } else {
              sum += (float)-0.0033865105;
            }
          } else {
            if (  (data[4].fvalue < 1.7719367)) {
              sum += (float)0.24439381;
            } else {
              sum += (float)0.050087344;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.39516935)) {
            if (  (data[4].fvalue < 1.8753804)) {
              sum += (float)0.22989234;
            } else {
              sum += (float)0.052232642;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.24015288;
            } else {
              sum += (float)0.064565055;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.15834685)) {
      if (  (data[14].fvalue < 0.52638984)) {
        if (  (data[8].fvalue < 4.999999)) {
          if (  (data[14].fvalue < 0.11019223)) {
            if (  (data[13].fvalue < 0.065240577)) {
              sum += (float)0.45588195;
            } else {
              sum += (float)0.22508797;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if (  (data[7].fvalue < 6.999999)) {
            if (  (data[14].fvalue < 0.17376703)) {
              sum += (float)0.3371892;
            } else {
              sum += (float)0.25200078;
            }
          } else {
            sum += (float)0.34561512;
          }
        }
      } else {
        if (  (data[8].fvalue < 9.999999)) {
          sum += (float)-0.089637294;
        } else {
          sum += (float)0.2915054;
        }
      }
    } else {
      if (  (data[7].fvalue < 7.999999)) {
        if (  (data[10].fvalue < 1.999999)) {
          if (  (data[3].fvalue < 1.3226497)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.26999703;
            } else {
              sum += (float)0.14485715;
            }
          } else {
            if (  (data[4].fvalue < 1.4436066)) {
              sum += (float)-0.1004309;
            } else {
              sum += (float)0.085695975;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.79607505)) {
            if (  (data[3].fvalue < 1.021511)) {
              sum += (float)-0.095961452;
            } else {
              sum += (float)-0.2947481;
            }
          } else {
            sum += (float)0.047208909;
          }
        }
      } else {
        if (  (data[7].fvalue < 9.999999)) {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 1.4877131)) {
              sum += (float)0.2694616;
            } else {
              sum += (float)0.10256202;
            }
          } else {
            sum += (float)0.018132616;
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            sum += (float)0.33853596;
          } else {
            sum += (float)0.18763399;
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 5.999999)) {
    if (  (data[13].fvalue < 0.025208106)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[17].fvalue < 0.032387462)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if ( (data[4].fvalue < 0.87856245)) {
              sum += (float)0.10012738;
            } else {
              sum += (float)-0.041715525;
            }
          } else {
            if (  (data[13].fvalue < 0.00056019303)) {
              sum += (float)0.44165945;
            } else {
              sum += (float)-0.039121002;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.011478476)) {
            if (  (data[14].fvalue < 0.11008508)) {
              sum += (float)-0.16331866;
            } else {
              sum += (float)-0.016387535;
            }
          } else {
            if (  (data[13].fvalue < 0.0038888885)) {
              sum += (float)-0.2555083;
            } else {
              sum += (float)0.023468867;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 0.14527869)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.013120353;
            } else {
              sum += (float)0.1211549;
            }
          } else {
            if (  (data[14].fvalue < 0.032272317)) {
              sum += (float)-0.1790165;
            } else {
              sum += (float)0.14264716;
            }
          }
        } else {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[3].fvalue < 0.25210318)) {
              sum += (float)-0.056688026;
            } else {
              sum += (float)-0.1854734;
            }
          } else {
            if (  (data[3].fvalue < 0.27451959)) {
              sum += (float)0.35164315;
            } else {
              sum += (float)0.12669879;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.46464601)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.46729988;
            } else {
              sum += (float)-0.0049243276;
            }
          } else {
            if (  (data[4].fvalue < 1.1391262)) {
              sum += (float)0.071048297;
            } else {
              sum += (float)-0.020645844;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.094818614)) {
            if (  (data[4].fvalue < 1.8203593)) {
              sum += (float)0.20860352;
            } else {
              sum += (float)0.0066985269;
            }
          } else {
            if (  (data[4].fvalue < 1.8203593)) {
              sum += (float)0.088310078;
            } else {
              sum += (float)-0.018979967;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.10465897;
            } else {
              sum += (float)0.20858397;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.13809085;
            } else {
              sum += (float)-0.065162428;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.068751015)) {
            if (  (data[16].fvalue < 0.49430215)) {
              sum += (float)-0.29590076;
            } else {
              sum += (float)-0.10254129;
            }
          } else {
            if (  (data[16].fvalue < 0.16473114)) {
              sum += (float)-0.059874494;
            } else {
              sum += (float)0.051586796;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 8.999999)) {
      if (  (data[13].fvalue < 0.092660137)) {
        if (  (data[14].fvalue < 0.1739464)) {
          sum += (float)0.32932174;
        } else {
          if (  (data[13].fvalue < 0.05661637)) {
            sum += (float)0.28313833;
          } else {
            if (  (data[3].fvalue < 0.2984997)) {
              sum += (float)0.1793109;
            } else {
              sum += (float)-0.023403697;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 7.999999)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.28055671;
            } else {
              sum += (float)0.11051503;
            }
          } else {
            if (  (data[17].fvalue < 0.1841408)) {
              sum += (float)-0.093053818;
            } else {
              sum += (float)0.081616022;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 8.999999)) {
              sum += (float)0.19317293;
            } else {
              sum += (float)0.27843785;
            }
          } else {
            if (  (data[16].fvalue < 0.56712812)) {
              sum += (float)-0.054498594;
            } else {
              sum += (float)0.14837554;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.7727958)) {
        if (  (data[3].fvalue < 1.7297394)) {
          if (  (data[3].fvalue < 0.9550153)) {
            if (  (data[4].fvalue < 1.3866239)) {
              sum += (float)0.3326084;
            } else {
              sum += (float)0.29683751;
            }
          } else {
            if (  (data[7].fvalue < 7.999999)) {
              sum += (float)0.13078974;
            } else {
              sum += (float)0.29558045;
            }
          }
        } else {
          sum += (float)0.083997503;
        }
      } else {
        if (  (data[14].fvalue < 0.42580602)) {
          if (  (data[13].fvalue < 0.23818351)) {
            sum += (float)0.31859875;
          } else {
            sum += (float)0.14308922;
          }
        } else {
          if (  (data[8].fvalue < 12.999999)) {
            if (  (data[17].fvalue < 0.032387462)) {
              sum += (float)0.074743047;
            } else {
              sum += (float)-0.10849912;
            }
          } else {
            sum += (float)0.20502315;
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 6.999999)) {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[13].fvalue < 0.14234346)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.13332993)) {
            if (  (data[16].fvalue < 0.032238457)) {
              sum += (float)-0.016584521;
            } else {
              sum += (float)-0.075219013;
            }
          } else {
            if (  (data[13].fvalue < 0.0071254577)) {
              sum += (float)-0.0036118587;
            } else {
              sum += (float)0.069273092;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.19248435)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.087978899;
            } else {
              sum += (float)0.18028413;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.072622009;
            } else {
              sum += (float)-0.15854692;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 1.1025354)) {
          if (  (data[14].fvalue < 0.26311323)) {
            if (  (data[16].fvalue < 0.34676108)) {
              sum += (float)-0.0017076421;
            } else {
              sum += (float)0.088406876;
            }
          } else {
            if (  (data[13].fvalue < 0.33375183)) {
              sum += (float)0.053017601;
            } else {
              sum += (float)0.15855785;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.43486917)) {
            if (  (data[5].fvalue < 0.42988762)) {
              sum += (float)-0.0078635877;
            } else {
              sum += (float)-0.10384075;
            }
          } else {
            if (  (data[14].fvalue < 0.49726573)) {
              sum += (float)-0.026401414;
            } else {
              sum += (float)0.16817495;
            }
          }
        }
      }
    } else {
      sum += (float)0.46865228;
    }
  } else {
    if (  (data[14].fvalue < 0.16604288)) {
      if (  (data[8].fvalue < 8.999999)) {
        if (  (data[3].fvalue < 0.59849787)) {
          if (  (data[4].fvalue < 1.102859)) {
            sum += (float)0.31991374;
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.10174256;
            } else {
              sum += (float)0.2937201;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.18439649)) {
            sum += (float)-0.018747238;
          } else {
            sum += (float)0.21917951;
          }
        }
      } else {
        sum += (float)0.32210851;
      }
    } else {
      if (  (data[8].fvalue < 9.999999)) {
        if (  (data[3].fvalue < 1.1025354)) {
          if (  (data[4].fvalue < 1.1388925)) {
            if (  (data[3].fvalue < 0.57272869)) {
              sum += (float)0.32307273;
            } else {
              sum += (float)0.21641301;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.16959295;
            } else {
              sum += (float)0.057879787;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.37883231)) {
            if (  (data[16].fvalue < 0.49430797)) {
              sum += (float)-0.11511452;
            } else {
              sum += (float)0.19692494;
            }
          } else {
            if (  (data[14].fvalue < 0.49726573)) {
              sum += (float)-0.10422522;
            } else {
              sum += (float)0.16832305;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.00032991735)) {
          if (  (data[3].fvalue < 1.2005209)) {
            if (  (data[4].fvalue < 1.69482)) {
              sum += (float)0.29489145;
            } else {
              sum += (float)0.19281992;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.19118994;
            } else {
              sum += (float)-0.0017129885;
            }
          }
        } else {
          if (  (data[8].fvalue < 12.999999)) {
            if (  (data[16].fvalue < 0.085553266)) {
              sum += (float)-0.070805393;
            } else {
              sum += (float)0.083669424;
            }
          } else {
            sum += (float)0.20251162;
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 7.999999)) {
    if (  (data[15].fvalue < 0.00028493445)) {
      if (  (data[4].fvalue < 1.0308539)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.32906017;
            } else {
              sum += (float)-0.037975736;
            }
          } else {
            sum += (float)0.68536353;
          }
        } else {
          if (  (data[3].fvalue < 0.656578)) {
            if (  (data[3].fvalue < 0.28733009)) {
              sum += (float)0.38200015;
            } else {
              sum += (float)0.22113131;
            }
          } else {
            if (  (data[3].fvalue < 1.2045503)) {
              sum += (float)0.053024974;
            } else {
              sum += (float)-0.15563168;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.038230874)) {
          if (  (data[17].fvalue < 0.031453002)) {
            if (  (data[16].fvalue < 0.032447468)) {
              sum += (float)0.1190784;
            } else {
              sum += (float)-0.019753635;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.13250424;
            } else {
              sum += (float)0.034283277;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.0028249701)) {
              sum += (float)0.0076029692;
            } else {
              sum += (float)0.093584947;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.055274066;
            } else {
              sum += (float)-0.040268976;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.42273)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.10345513)) {
            if (  (data[14].fvalue < 0.032298017)) {
              sum += (float)-0.39960912;
            } else {
              sum += (float)-0.2676914;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.0715537;
            } else {
              sum += (float)-0.23182641;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            sum += (float)-0.38141665;
          } else {
            if (  (data[5].fvalue < 0.60810047)) {
              sum += (float)-0.17564915;
            } else {
              sum += (float)-0.3597998;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.031453002)) {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.79875147)) {
              sum += (float)-0.043046355;
            } else {
              sum += (float)0.096592613;
            }
          } else {
            if (  (data[4].fvalue < 1.8758579)) {
              sum += (float)0.42074135;
            } else {
              sum += (float)-0.17405175;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 2.3536713)) {
              sum += (float)-0.24700165;
            } else {
              sum += (float)-0.076592043;
            }
          } else {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.043505669;
            } else {
              sum += (float)-0.16068514;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 0.21161668)) {
      if (  (data[8].fvalue < 8.999999)) {
        if (  (data[14].fvalue < 0.10345513)) {
          sum += (float)0.30176011;
        } else {
          if (  (data[4].fvalue < 1.1816852)) {
            if (  (data[6].fvalue < 0.16319355)) {
              sum += (float)0.27421039;
            } else {
              sum += (float)0.14753909;
            }
          } else {
            if (  (data[16].fvalue < 0.010066199)) {
              sum += (float)-0.041640818;
            } else {
              sum += (float)0.22226317;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.29550284)) {
          sum += (float)0.30942297;
        } else {
          sum += (float)0.20827068;
        }
      }
    } else {
      if (  (data[8].fvalue < 10.999999)) {
        if (  (data[6].fvalue < 0.22582662)) {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[0].fvalue < 2.9737401)) {
              sum += (float)0.23358549;
            } else {
              sum += (float)0.085950717;
            }
          } else {
            if ( (data[4].fvalue < 1.3852679)) {
              sum += (float)0.29001024;
            } else {
              sum += (float)0.14386873;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.42612824)) {
              sum += (float)0.024013523;
            } else {
              sum += (float)0.21770737;
            }
          } else {
            if (  (data[13].fvalue < 0.063341044)) {
              sum += (float)-0.12716649;
            } else {
              sum += (float)0.055244148;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8758579)) {
          if (  (data[3].fvalue < 1.4926516)) {
            if (  (data[8].fvalue < 11.999999)) {
              sum += (float)0.24297619;
            } else {
              sum += (float)0.30432162;
            }
          } else {
            sum += (float)0.073355414;
          }
        } else {
          if (  (data[8].fvalue < 12.999999)) {
            if (  (data[7].fvalue < 5.999999)) {
              sum += (float)0.10277806;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.17656057;
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 6.999999)) {
    if (  (data[8].fvalue < 4.999999)) {
      if (  (data[5].fvalue < 0.12397124)) {
        if (  (data[4].fvalue < 1.2243329)) {
          if (  (data[16].fvalue < 0.098249078)) {
            if (  (data[14].fvalue < 0.069331706)) {
              sum += (float)-0.097903162;
            } else {
              sum += (float)-0.046888057;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.020176139;
            } else {
              sum += (float)0.22548778;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.8589375)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.057732258;
            } else {
              sum += (float)-0.018569486;
            }
          } else {
            if (  (data[16].fvalue < 0.56634533)) {
              sum += (float)-0.14350297;
            } else {
              sum += (float)0.094828799;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.0132322)) {
              sum += (float)-0.3345075;
            } else {
              sum += (float)-0.046307512;
            }
          } else {
            if (  (data[14].fvalue < 0.0044863182)) {
              sum += (float)0.20728678;
            } else {
              sum += (float)-0.0079991259;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.20567524;
            } else {
              sum += (float)0.19689205;
            }
          } else {
            if (  (data[14].fvalue < 0.015210853)) {
              sum += (float)-0.26281273;
            } else {
              sum += (float)-0.046460114;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.089660838)) {
        if (  (data[4].fvalue < 1.1209131)) {
          if (  (data[3].fvalue < 0.35243708)) {
            sum += (float)0.3483806;
          } else {
            if (  (data[4].fvalue < 0.98360503)) {
              sum += (float)0.27967441;
            } else {
              sum += (float)0.05861216;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.28714973)) {
              sum += (float)-0.072217345;
            } else {
              sum += (float)0.10069292;
            }
          } else {
            if (  (data[0].fvalue < 6.068192)) {
              sum += (float)0.21155715;
            } else {
              sum += (float)0.049961917;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.12692896)) {
          if (  (data[14].fvalue < 0.56062281)) {
            if (  (data[4].fvalue < 1.1384693)) {
              sum += (float)0.1126195;
            } else {
              sum += (float)-0.049892906;
            }
          } else {
            if (  (data[15].fvalue < 0.00033562217)) {
              sum += (float)0.10949391;
            } else {
              sum += (float)-0.078862712;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.7303281)) {
            if (  (data[3].fvalue < 1.0250336)) {
              sum += (float)0.27160984;
            } else {
              sum += (float)-3.869455e-05;
            }
          } else {
            if (  (data[16].fvalue < 0.66433853)) {
              sum += (float)-0.044642471;
            } else {
              sum += (float)0.17731358;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.15676644)) {
      if (  (data[3].fvalue < 0.76013994)) {
        if (  (data[7].fvalue < 7.999999)) {
          if (  (data[14].fvalue < 0.34029904)) {
            sum += (float)0.2757163;
          } else {
            sum += (float)0.11546019;
          }
        } else {
          sum += (float)0.31018335;
        }
      } else {
        sum += (float)0.13385187;
      }
    } else {
      if (  (data[7].fvalue < 9.999999)) {
        if (  (data[11].fvalue < 0.99999899)) {
          if (  (data[6].fvalue < 0.33702669)) {
            if (  (data[0].fvalue < 2.6356187)) {
              sum += (float)0.25104177;
            } else {
              sum += (float)0.16984305;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.24516948;
            } else {
              sum += (float)0.091505691;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.13181996)) {
              sum += (float)-0.091412902;
            } else {
              sum += (float)0.12376424;
            }
          } else {
            sum += (float)-0.18910532;
          }
        }
      } else {
        if (  (data[10].fvalue < 1.999999)) {
          sum += (float)0.28501698;
        } else {
          sum += (float)0.052530382;
        }
      }
    }
  }
  if (  (data[7].fvalue < 4.999999)) {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[13].fvalue < 0.11338273)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.15050346)) {
            if (  (data[5].fvalue < 0.92732471)) {
              sum += (float)-0.036045019;
            } else {
              sum += (float)0.035816293;
            }
          } else {
            if (  (data[16].fvalue < 0.66599613)) {
              sum += (float)0.0031769264;
            } else {
              sum += (float)0.14241236;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.36683694)) {
            if (  (data[4].fvalue < 1.1398501)) {
              sum += (float)0.059347086;
            } else {
              sum += (float)-0.072583109;
            }
          } else {
            if (  (data[16].fvalue < 0.18523356)) {
              sum += (float)-0.16767311;
            } else {
              sum += (float)-0.016914029;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.40738708)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.7573989)) {
              sum += (float)0.043786682;
            } else {
              sum += (float)-0.033718847;
            }
          } else {
            if (  (data[14].fvalue < 0.044560403)) {
              sum += (float)-0.1839361;
            } else {
              sum += (float)-0.011830898;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 1.0012238)) {
              sum += (float)0.039458498;
            } else {
              sum += (float)-0.10808323;
            }
          } else {
            if (  (data[17].fvalue < 0.0001772074)) {
              sum += (float)0.15008861;
            } else {
              sum += (float)0.026966354;
            }
          }
        }
      }
    } else {
      sum += (float)0.40137672;
    }
  } else {
    if (  (data[6].fvalue < 0.18332319)) {
      if (  (data[8].fvalue < 5.999999)) {
        if (  (data[4].fvalue < 1.0692531)) {
          if (  (data[3].fvalue < 0.3817704)) {
            sum += (float)0.359209;
          } else {
            if (  (data[14].fvalue < 0.08994706)) {
              sum += (float)0.0024097008;
            } else {
              sum += (float)0.24062523;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.033379957)) {
            if (  (data[14].fvalue < 0.15050346)) {
              sum += (float)-0.071269691;
            } else {
              sum += (float)0.13438395;
            }
          } else {
            if (  (data[3].fvalue < 0.27578017)) {
              sum += (float)0.36258107;
            } else {
              sum += (float)0.16925657;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.3106637)) {
          if (  (data[3].fvalue < 0.80192602)) {
            sum += (float)0.29924548;
          } else {
            sum += (float)0.199053;
          }
        } else {
          if (  (data[3].fvalue < 0.26438338)) {
            sum += (float)0.20767614;
          } else {
            sum += (float)0.047238048;
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 8.999999)) {
        if (  (data[3].fvalue < 0.25325266)) {
          if (  (data[4].fvalue < 1.8758289)) {
            if (  (data[16].fvalue < 0.12717244)) {
              sum += (float)0.12913722;
            } else {
              sum += (float)0.26100758;
            }
          } else {
            if (  (data[16].fvalue < 0.011564658)) {
              sum += (float)0.039843004;
            } else {
              sum += (float)-0.091464482;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.29429924;
            } else {
              sum += (float)0.11617132;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.018107224;
            } else {
              sum += (float)-0.15692253;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.9443544)) {
          if (  (data[3].fvalue < 1.7333956)) {
            if (  (data[3].fvalue < 0.95776206)) {
              sum += (float)0.27889362;
            } else {
              sum += (float)0.1891426;
            }
          } else {
            sum += (float)0.038102731;
          }
        } else {
          if (  (data[7].fvalue < 11.999999)) {
            if (  (data[8].fvalue < 6.999999)) {
              sum += (float)0.15131475;
            } else {
              sum += (float)-0.024074903;
            }
          } else {
            sum += (float)0.21185856;
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 8.999999)) {
    if (  (data[15].fvalue < 0.00034746496)) {
      if (  (data[5].fvalue < 0.080739915)) {
        if (  (data[4].fvalue < 1.8766279)) {
          if (  (data[4].fvalue < 1.4427414)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.033525079;
            } else {
              sum += (float)0.059480976;
            }
          } else {
            if (  (data[3].fvalue < 0.05478429)) {
              sum += (float)0.16117416;
            } else {
              sum += (float)0.080805153;
            }
          }
        } else {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.034074944)) {
              sum += (float)0.11018503;
            } else {
              sum += (float)-0.056542613;
            }
          } else {
            if (  (data[4].fvalue < 2.3552494)) {
              sum += (float)-0.41875395;
            } else {
              sum += (float)-0.15847115;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 0.37150362)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.012686531;
            } else {
              sum += (float)0.1380429;
            }
          } else {
            if (  (data[16].fvalue < 0.80183136)) {
              sum += (float)0.012410211;
            } else {
              sum += (float)0.20329449;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.10216274)) {
            if (  (data[0].fvalue < 3.8499038)) {
              sum += (float)0.30039543;
            } else {
              sum += (float)0.11218508;
            }
          } else {
            if (  (data[3].fvalue < 0.8997547)) {
              sum += (float)0.052745409;
            } else {
              sum += (float)-0.047630943;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[13].fvalue < 0.0050662952)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.56989998)) {
              sum += (float)-0.21170601;
            } else {
              sum += (float)-0.064905673;
            }
          } else {
            if (  (data[4].fvalue < 1.7729521)) {
              sum += (float)0.34854633;
            } else {
              sum += (float)-0.17604885;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.032608803)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.19434702;
            } else {
              sum += (float)-0.035158444;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.41584179;
            } else {
              sum += (float)0.071307175;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.042730503;
            } else {
              sum += (float)-0.070516668;
            }
          } else {
            if (  (data[13].fvalue < 0.12593424)) {
              sum += (float)-0.24884403;
            } else {
              sum += (float)-0.11072581;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.031737894)) {
              sum += (float)0.11727685;
            } else {
              sum += (float)-0.0064191832;
            }
          } else {
            if (  (data[5].fvalue < 0.4647153)) {
              sum += (float)-0.02414499;
            } else {
              sum += (float)-0.21521892;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.29545394)) {
      if ( (data[4].fvalue < 1.3373258)) {
        if (  (data[3].fvalue < 1.0262053)) {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[3].fvalue < 0.31267294)) {
              sum += (float)0.26114717;
            } else {
              sum += (float)0.11907037;
            }
          } else {
            sum += (float)0.29221946;
          }
        } else {
          sum += (float)0.094801456;
        }
      } else {
        if (  (data[8].fvalue < 10.999999)) {
          if (  (data[3].fvalue < 0.14630152)) {
            sum += (float)0.19606455;
          } else {
            if (  (data[14].fvalue < 0.52731913)) {
              sum += (float)-0.011753631;
            } else {
              sum += (float)0.10905205;
            }
          }
        } else {
          sum += (float)0.24901894;
        }
      }
    } else {
      if (  (data[8].fvalue < 11.999999)) {
        if (  (data[0].fvalue < 2.0801229)) {
          if (  (data[6].fvalue < 0.58987212)) {
            if (  (data[0].fvalue < 1.544277)) {
              sum += (float)0.25478715;
            } else {
              sum += (float)0.1564111;
            }
          } else {
            sum += (float)0.039199881;
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.19115737)) {
              sum += (float)0.12170825;
            } else {
              sum += (float)0.24505137;
            }
          } else {
            if (  (data[16].fvalue < 0.31415638)) {
              sum += (float)-0.082828268;
            } else {
              sum += (float)0.093578316;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 3.5948224)) {
          if (  (data[6].fvalue < 0.47921395)) {
            sum += (float)0.28224462;
          } else {
            if (  (data[7].fvalue < 10.999999)) {
              sum += (float)0.12867548;
            } else {
              sum += (float)0.27386576;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.1127199)) {
            sum += (float)0.012014752;
          } else {
            sum += (float)0.12893936;
          }
        }
      }
    }
  }
  if (  (data[8].fvalue < 5.999999)) {
    if (  (data[14].fvalue < 1.2755084)) {
      if (  (data[5].fvalue < 0.3051869)) {
        if (  (data[4].fvalue < 1.04246)) {
          if (  (data[14].fvalue < 0.044291202)) {
            if (  (data[13].fvalue < 0.0013204721)) {
              sum += (float)0.11525875;
            } else {
              sum += (float)-0.14030765;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.19727261;
            } else {
              sum += (float)0.26609319;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.121725)) {
            if (  (data[13].fvalue < 0.008573466)) {
              sum += (float)0.074452549;
            } else {
              sum += (float)0.25244439;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.021199759;
            } else {
              sum += (float)-0.024242802;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.032851677)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.029980658)) {
              sum += (float)-0.36240718;
            } else {
              sum += (float)-0.0061244261;
            }
          } else {
            if (  (data[4].fvalue < 1.351658)) {
              sum += (float)0.49799928;
            } else {
              sum += (float)-0.07042399;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.13335985)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.089670718;
            } else {
              sum += (float)-0.21860768;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.067000486;
            } else {
              sum += (float)0.023131894;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.032851677)) {
        if (  (data[15].fvalue < 0.00033797399)) {
          if (  (data[16].fvalue < 0.49585044)) {
            if (  (data[16].fvalue < 0.033969171)) {
              sum += (float)0.052077208;
            } else {
              sum += (float)-0.13812099;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.33286166;
            } else {
              sum += (float)-0.14848527;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[15].fvalue < 0.39108971)) {
              sum += (float)-0.32670996;
            } else {
              sum += (float)-0.12442653;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.017721361;
            } else {
              sum += (float)-0.15133151;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.49545869)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)-0.167969;
            } else {
              sum += (float)-0.36883944;
            }
          } else {
            if (  (data[15].fvalue < 0.15887523)) {
              sum += (float)-0.35851833;
            } else {
              sum += (float)-0.13876797;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.012366466)) {
              sum += (float)0.25105938;
            } else {
              sum += (float)-0.084732622;
            }
          } else {
            if (  (data[17].fvalue < 0.99473596)) {
              sum += (float)-0.24793664;
            } else {
              sum += (float)-0.00029895752;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.43094707)) {
      if (  (data[4].fvalue < 1.1393466)) {
        if (  (data[4].fvalue < 1.1034094)) {
          sum += (float)0.29898122;
        } else {
          if (  (data[3].fvalue < 0.2428499)) {
            sum += (float)0.28782821;
          } else {
            sum += (float)0.057969797;
          }
        }
      } else {
        if (  (data[8].fvalue < 10.999999)) {
          if (  (data[3].fvalue < 0.1204935)) {
            if (  (data[14].fvalue < 0.32653603)) {
              sum += (float)0.079855777;
            } else {
              sum += (float)0.1809426;
            }
          } else {
            if (  (data[14].fvalue < 0.42710921)) {
              sum += (float)-0.001975032;
            } else {
              sum += (float)0.093666516;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8742416)) {
            sum += (float)0.28078187;
          } else {
            sum += (float)0.050551627;
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.49585044)) {
        if (  (data[3].fvalue < 1.2054614)) {
          if ( (data[4].fvalue < 1.013466)) {
            if (  (data[14].fvalue < 0.14242363)) {
              sum += (float)0.028399413;
            } else {
              sum += (float)0.24147765;
            }
          } else {
            if (  (data[14].fvalue < 0.64345813)) {
              sum += (float)-0.041885607;
            } else {
              sum += (float)0.068823308;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.85952604)) {
            if (  (data[17].fvalue < 0.49545869)) {
              sum += (float)-0.17257407;
            } else {
              sum += (float)0.078111872;
            }
          } else {
            if (  (data[16].fvalue < 0.012366466)) {
              sum += (float)0.074526504;
            } else {
              sum += (float)-0.10973438;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.032851677)) {
          if (  (data[14].fvalue < 0.85952604)) {
            if (  (data[13].fvalue < 0.50571394)) {
              sum += (float)0.22150619;
            } else {
              sum += (float)0.089147612;
            }
          } else {
            sum += (float)0.034160018;
          }
        } else {
          if (  (data[14].fvalue < 0.34063718)) {
            sum += (float)0.13373055;
          } else {
            if (  (data[1].fvalue < 1.5489783)) {
              sum += (float)-0.0082401121;
            } else {
              sum += (float)-0.082260773;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 6.999999)) {
    if (  (data[11].fvalue < 2.999999)) {
      if (  (data[12].fvalue < 4.999999)) {
        if (  (data[13].fvalue < 0.26289722)) {
          if (  (data[5].fvalue < 1.1754377)) {
            if ( (data[4].fvalue < 1.0321853)) {
              sum += (float)-0.0252866;
            } else {
              sum += (float)-0.00022809161;
            }
          } else {
            if (  (data[3].fvalue < 0.11248399)) {
              sum += (float)-0.38927653;
            } else {
              sum += (float)0.046497054;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.29970029)) {
            if (  (data[16].fvalue < 1.0051491)) {
              sum += (float)-0.0019352742;
            } else {
              sum += (float)0.13432787;
            }
          } else {
            if (  (data[16].fvalue < 0.013815201)) {
              sum += (float)0.12587222;
            } else {
              sum += (float)0.020091644;
            }
          }
        }
      } else {
        sum += (float)0.3619681;
      }
    } else {
      if (  (data[17].fvalue < 0.66534549)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.0084522627)) {
            sum += (float)-0.4450385;
          } else {
            sum += (float)-0.22748731;
          }
        } else {
          if (  (data[5].fvalue < 0.28149256)) {
            if (  (data[4].fvalue < 1.8747684)) {
              sum += (float)-0.0073209149;
            } else {
              sum += (float)-0.14215381;
            }
          } else {
            sum += (float)-0.25274795;
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[4].fvalue < 2.1525517)) {
            if (  (data[13].fvalue < 0.0084522627)) {
              sum += (float)-0.28991675;
            } else {
              sum += (float)-0.036391251;
            }
          } else {
            if (  (data[14].fvalue < 0.37115657)) {
              sum += (float)-0.052162744;
            } else {
              sum += (float)0.052738946;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[17].fvalue < 0.99760056)) {
              sum += (float)0.01496837;
            } else {
              sum += (float)0.13154057;
            }
          } else {
            sum += (float)-0.070216335;
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.1557865)) {
      if (  (data[14].fvalue < 0.25321743)) {
        if (  (data[8].fvalue < 5.999999)) {
          sum += (float)0.1654871;
        } else {
          sum += (float)0.29206613;
        }
      } else {
        if (  (data[8].fvalue < 8.999999)) {
          sum += (float)-0;
        } else {
          sum += (float)0.22617529;
        }
      }
    } else {
      if (  (data[7].fvalue < 9.999999)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.11880284)) {
              sum += (float)0.12207476;
            } else {
              sum += (float)0.27681592;
            }
          } else {
            if (  (data[13].fvalue < 0.75881326)) {
              sum += (float)0.0079587763;
            } else {
              sum += (float)0.16021796;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.770746)) {
            if (  (data[16].fvalue < 0.12849523)) {
              sum += (float)0.04450126;
            } else {
              sum += (float)0.18614769;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.083471626;
            } else {
              sum += (float)-0.10272675;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.9433522)) {
          if (  (data[3].fvalue < 1.3294542)) {
            sum += (float)0.26330549;
          } else {
            sum += (float)0.14809628;
          }
        } else {
          sum += (float)0.06303519;
        }
      }
    }
  }
  if (  (data[7].fvalue < 7.999999)) {
    if (  (data[15].fvalue < 0.00030664771)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.25331631)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)-0.0076556257;
            } else {
              sum += (float)0.048756216;
            }
          } else {
            if (  (data[16].fvalue < 1.0046549)) {
              sum += (float)0.012174662;
            } else {
              sum += (float)0.18659668;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6314418)) {
            if (  (data[4].fvalue < 1.4209214)) {
              sum += (float)0.48562005;
            } else {
              sum += (float)0.29200214;
            }
          } else {
            if (  (data[16].fvalue < 0.39031485)) {
              sum += (float)-0.2211118;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.7294552)) {
          if (  (data[12].fvalue < 2.999999)) {
            if (  (data[3].fvalue < 0.253975)) {
              sum += (float)0.17784725;
            } else {
              sum += (float)-0.11860503;
            }
          } else {
            sum += (float)0.34050602;
          }
        } else {
          if (  (data[4].fvalue < 2.3524954)) {
            if (  (data[4].fvalue < 1.8181161)) {
              sum += (float)-0.21367423;
            } else {
              sum += (float)-0.48622379;
            }
          } else {
            if (  (data[6].fvalue < 0.74013221)) {
              sum += (float)-0.22106065;
            } else {
              sum += (float)-0.0047527086;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.4833586)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.21254049)) {
              sum += (float)-0.21170221;
            } else {
              sum += (float)-0.095056266;
            }
          } else {
            if (  (data[3].fvalue < 0.85046995)) {
              sum += (float)0.41777527;
            } else {
              sum += (float)-7.1254208e-06;
            }
          }
        } else {
          sum += (float)0.34051928;
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.69588393)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.0090313135;
            } else {
              sum += (float)-0.11900862;
            }
          } else {
            if (  (data[15].fvalue < 0.39341816)) {
              sum += (float)-0.23509766;
            } else {
              sum += (float)-0.048264395;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.033049598)) {
              sum += (float)0.08137092;
            } else {
              sum += (float)-0.0056402716;
            }
          } else {
            if (  (data[15].fvalue < 0.16150291)) {
              sum += (float)-0.096947879;
            } else {
              sum += (float)0.00016100479;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.21105003)) {
      if (  (data[8].fvalue < 7.999999)) {
        if (  (data[13].fvalue < 0.12518404)) {
          sum += (float)0.22340973;
        } else {
          sum += (float)0.036936227;
        }
      } else {
        if (  (data[14].fvalue < 0.38842514)) {
          sum += (float)0.29051346;
        } else {
          sum += (float)0.16749516;
        }
      }
    } else {
      if (  (data[17].fvalue < 0.00018569129)) {
        if (  (data[7].fvalue < 9.999999)) {
          if (  (data[8].fvalue < 3.999999)) {
            sum += (float)0.20105657;
          } else {
            if (  (data[6].fvalue < 0.30496362)) {
              sum += (float)0.15426245;
            } else {
              sum += (float)0.03406762;
            }
          }
        } else {
          sum += (float)0.21376367;
        }
      } else {
        if (  (data[17].fvalue < 0.18494892)) {
          if (  (data[3].fvalue < 0.76287687)) {
            sum += (float)-0.012992958;
          } else {
            sum += (float)-0.19102941;
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            sum += (float)0.15648852;
          } else {
            sum += (float)-0.0059258784;
          }
        }
      }
    }
  }
  if (  (data[16].fvalue < 0.14639504)) {
    if (  (data[16].fvalue < 0.050718147)) {
      if (  (data[14].fvalue < 0.0039705574)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.5526053)) {
              sum += (float)0.17564157;
            } else {
              sum += (float)0.46247312;
            }
          } else {
            if (  (data[4].fvalue < 1.7285736)) {
              sum += (float)-0.30183002;
            } else {
              sum += (float)0.15156928;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.35740504;
            } else {
              sum += (float)-0.0080030439;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.22968273;
            } else {
              sum += (float)0.016059613;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.1335191)) {
          if (  (data[13].fvalue < 0.0010104595)) {
            if (  (data[14].fvalue < 0.032046605)) {
              sum += (float)0.18538737;
            } else {
              sum += (float)-0.0022850786;
            }
          } else {
            if (  (data[13].fvalue < 0.29285648)) {
              sum += (float)-0.050016798;
            } else {
              sum += (float)0.045794148;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.19254246)) {
            if (  (data[4].fvalue < 1.7698128)) {
              sum += (float)0.1887351;
            } else {
              sum += (float)0.0098076956;
            }
          } else {
            if (  (data[5].fvalue < 0.4317154)) {
              sum += (float)0.048825543;
            } else {
              sum += (float)-0.040368073;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.0044629895)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.2747844)) {
              sum += (float)-0.13225164;
            } else {
              sum += (float)-0.031227715;
            }
          } else {
            if (  (data[4].fvalue < 1.3094707)) {
              sum += (float)-0.066291735;
            } else {
              sum += (float)-0.24171914;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.050071083)) {
            if (  (data[14].fvalue < 0.026091123)) {
              sum += (float)-0.31338251;
            } else {
              sum += (float)-0.17735717;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.0080684377;
            } else {
              sum += (float)-0.20166337;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.2121312)) {
          if (  (data[14].fvalue < 0.032046605)) {
            if (  (data[13].fvalue < 0.045473732)) {
              sum += (float)-0.14745007;
            } else {
              sum += (float)-0.0020704067;
            }
          } else {
            if (  (data[4].fvalue < 1.2642304)) {
              sum += (float)0.19025762;
            } else {
              sum += (float)0.02217822;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.17548038)) {
              sum += (float)-0.050333761;
            } else {
              sum += (float)0.13045491;
            }
          } else {
            if (  (data[5].fvalue < 0.68546551)) {
              sum += (float)-0.079629734;
            } else {
              sum += (float)-0.27019703;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 0.032684311)) {
      if (  (data[14].fvalue < 0.020320533)) {
        if (  (data[13].fvalue < 0.10021085)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[5].fvalue < 0.15975751)) {
              sum += (float)-0.056780357;
            } else {
              sum += (float)-0.17798021;
            }
          } else {
            if (  (data[13].fvalue < 0.016807837)) {
              sum += (float)0.42240381;
            } else {
              sum += (float)0.2370217;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.030720336;
            } else {
              sum += (float)-0.084094293;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.18045661;
            } else {
              sum += (float)-0.14731924;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.23252827)) {
          if (  (data[13].fvalue < 0.0044629895)) {
            if (  (data[3].fvalue < 0.11246309)) {
              sum += (float)0.13922997;
            } else {
              sum += (float)0.062360335;
            }
          } else {
            if (  (data[4].fvalue < 1.8171682)) {
              sum += (float)0.20503651;
            } else {
              sum += (float)0.016381636;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0044629895)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.036840018;
            } else {
              sum += (float)0.06235395;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.12567317;
            } else {
              sum += (float)0.032991394;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.008230445)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[17].fvalue < 0.66602695)) {
            if (  (data[14].fvalue < 0.69529313)) {
              sum += (float)-0.16258;
            } else {
              sum += (float)-0.28819865;
            }
          } else {
            if (  (data[16].fvalue < 0.34894034)) {
              sum += (float)-0.15945818;
            } else {
              sum += (float)0.062500253;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.49603263)) {
            if (  (data[4].fvalue < 1.7285736)) {
              sum += (float)-0.032870114;
            } else {
              sum += (float)-0.10491698;
            }
          } else {
            if (  (data[17].fvalue < 0.30545869)) {
              sum += (float)0.013722914;
            } else {
              sum += (float)0.12599759;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.75804508)) {
          if (  (data[14].fvalue < 0.0039705574)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.093047008;
            } else {
              sum += (float)0.11184748;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.048046462;
            } else {
              sum += (float)-0.066007726;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.80122226)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.26375142;
            } else {
              sum += (float)-0.089338772;
            }
          } else {
            if (  (data[16].fvalue < 1.3737748)) {
              sum += (float)-0.037250701;
            } else {
              sum += (float)0.17724365;
            }
          }
        }
      }
    }
  }
  if (  (data[15].fvalue < 0.046242304)) {
    if (  (data[8].fvalue < 9.999999)) {
      if (  (data[14].fvalue < 0.22255968)) {
        if (  (data[13].fvalue < 0.016635109)) {
          if (  (data[16].fvalue < 0.031800363)) {
            if (  (data[14].fvalue < 0.038366344)) {
              sum += (float)0.062986419;
            } else {
              sum += (float)-0.020691218;
            }
          } else {
            if (  (data[3].fvalue < 0.16469781)) {
              sum += (float)-0.018172938;
            } else {
              sum += (float)-0.078322329;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.33942884)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.00071466429;
            } else {
              sum += (float)0.061410934;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.0055635637;
            } else {
              sum += (float)-0.063100003;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.38652822)) {
          if (  (data[17].fvalue < 0.99506217)) {
            if ( (data[4].fvalue < 1.8712101)) {
              sum += (float)0.0094281267;
            } else {
              sum += (float)-0.12147315;
            }
          } else {
            if (  (data[4].fvalue < 1.6908257)) {
              sum += (float)-0.12284491;
            } else {
              sum += (float)0.17642872;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.44981468)) {
            if (  (data[17].fvalue < 0.030454444)) {
              sum += (float)0.14955153;
            } else {
              sum += (float)-0.016711766;
            }
          } else {
            if (  (data[13].fvalue < 0.012215393)) {
              sum += (float)-0.079190508;
            } else {
              sum += (float)0.017227493;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.32480469)) {
        if ( (data[4].fvalue < 1.381399)) {
          if (  (data[3].fvalue < 0.76126581)) {
            sum += (float)0.27773878;
          } else {
            sum += (float)0.14263777;
          }
        } else {
          if (  (data[7].fvalue < 5.999999)) {
            sum += (float)0.06017546;
          } else {
            sum += (float)0.16997327;
          }
        }
      } else {
        if (  (data[0].fvalue < 2.5156901)) {
          if (  (data[8].fvalue < 11.999999)) {
            if (  (data[0].fvalue < 1.7644159)) {
              sum += (float)0.1712776;
            } else {
              sum += (float)0.05156678;
            }
          } else {
            sum += (float)0.22820647;
          }
        } else {
          if (  (data[8].fvalue < 12.999999)) {
            if (  (data[16].fvalue < 0.25447807)) {
              sum += (float)-0.085850976;
            } else {
              sum += (float)0.066316038;
            }
          } else {
            sum += (float)0.11440018;
          }
        }
      }
    }
  } else {
    if (  (data[12].fvalue < 1.999999)) {
      if (  (data[15].fvalue < 0.15524274)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.7680559)) {
              sum += (float)-0.098703884;
            } else {
              sum += (float)0.11101416;
            }
          } else {
            if (  (data[14].fvalue < 0.55950618)) {
              sum += (float)-0.23030275;
            } else {
              sum += (float)-0.13313943;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.85581905)) {
              sum += (float)-0.09017048;
            } else {
              sum += (float)-0.24603719;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.11609672;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 0.49283329)) {
            if (  (data[13].fvalue < 0.0043615117)) {
              sum += (float)-0.12948798;
            } else {
              sum += (float)-0.019367408;
            }
          } else {
            if (  (data[2].fvalue < 0.89117968)) {
              sum += (float)-0.0084468154;
            } else {
              sum += (float)0.15251216;
            }
          }
        } else {
          if (  (data[9].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.038366344)) {
              sum += (float)-0.13617964;
            } else {
              sum += (float)0.02751;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.30486056;
            } else {
              sum += (float)-0.023341499;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.8154049)) {
        sum += (float)0.35156509;
      } else {
        sum += (float)-0.046120573;
      }
    }
  }
  if (  (data[11].fvalue < 2.999999)) {
    if (  (data[14].fvalue < 0.11171812)) {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[6].fvalue < 0.022107597)) {
            if (  (data[0].fvalue < 2.3981249)) {
              sum += (float)-0.40767583;
            } else {
              sum += (float)-0.063743785;
            }
          } else {
            if (  (data[16].fvalue < 0.030315498)) {
              sum += (float)0.021925215;
            } else {
              sum += (float)-0.018915618;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.044189494)) {
            if (  (data[6].fvalue < 0.047347192)) {
              sum += (float)-0.01343101;
            } else {
              sum += (float)-0.1523001;
            }
          } else {
            if (  (data[3].fvalue < 0.38253927)) {
              sum += (float)0.0020314856;
            } else {
              sum += (float)-0.087548994;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.11189147)) {
          if (  (data[3].fvalue < 0.24293755)) {
            if (  (data[4].fvalue < 0.99737287)) {
              sum += (float)0.35794985;
            } else {
              sum += (float)0.16310062;
            }
          } else {
            if (  (data[16].fvalue < 0.12524466)) {
              sum += (float)-0.047042228;
            } else {
              sum += (float)0.25555867;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1368772)) {
            if (  (data[3].fvalue < 0.52840602)) {
              sum += (float)0.30658093;
            } else {
              sum += (float)-0.167622;
            }
          } else {
            if (  (data[16].fvalue < 0.046408974)) {
              sum += (float)-0.19774011;
            } else {
              sum += (float)-0.0203407;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.4328185)) {
        if (  (data[6].fvalue < 0.053328954)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[0].fvalue < 3.5166879)) {
              sum += (float)-0.27509123;
            } else {
              sum += (float)-0.055838864;
            }
          } else {
            if ( (data[4].fvalue < 1.1368772)) {
              sum += (float)0.31875905;
            } else {
              sum += (float)-0.0069490685;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.017725891;
            } else {
              sum += (float)-0.03870555;
            }
          } else {
            if (  (data[13].fvalue < 0.06036457)) {
              sum += (float)-0.10580631;
            } else {
              sum += (float)0.00075634604;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.3872743)) {
          if (  (data[16].fvalue < 1.3633518)) {
            if (  (data[17].fvalue < 0.030439749)) {
              sum += (float)0.0903759;
            } else {
              sum += (float)-0.0049723755;
            }
          } else {
            if (  (data[14].fvalue < 0.44749051)) {
              sum += (float)0.12971322;
            } else {
              sum += (float)0.27001277;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.008074237)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.084949866;
            } else {
              sum += (float)0.041551873;
            }
          } else {
            if (  (data[3].fvalue < 1.7321954)) {
              sum += (float)0.078002468;
            } else {
              sum += (float)-0.042402603;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.0043312758)) {
      if (  (data[4].fvalue < 1.9375187)) {
        if (  (data[8].fvalue < 1.999999)) {
          sum += (float)-0.3396;
        } else {
          sum += (float)-0.12177893;
        }
      } else {
        if (  (data[11].fvalue < 3.999999)) {
          if (  (data[17].fvalue < 0.66341311)) {
            sum += (float)-0.17136298;
          } else {
            if (  (data[14].fvalue < 0.2227584)) {
              sum += (float)-0.12907222;
            } else {
              sum += (float)0.064271286;
            }
          }
        } else {
          sum += (float)-0.29092339;
        }
      }
    } else {
      if (  (data[17].fvalue < 0.49366561)) {
        if (  (data[3].fvalue < 0.9002912)) {
          if (  (data[10].fvalue < 0.99999899)) {
            sum += (float)-0.13509396;
          } else {
            sum += (float)-0.045199435;
          }
        } else {
          sum += (float)-0.2241147;
        }
      } else {
        if (  (data[11].fvalue < 3.999999)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 0.069556579)) {
              sum += (float)-0;
            } else {
              sum += (float)0.08629027;
            }
          } else {
            if (  (data[13].fvalue < 0.23376158)) {
              sum += (float)0.025652161;
            } else {
              sum += (float)-0.10605867;
            }
          }
        } else {
          sum += (float)-0.14314687;
        }
      }
    }
  }
  if (  (data[8].fvalue < 4.999999)) {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[12].fvalue < 1.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[4].fvalue < 1.0310391)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.24484056;
            } else {
              sum += (float)-0.013129256;
            }
          } else {
            if ( (data[4].fvalue < 1.1374563)) {
              sum += (float)0.042789556;
            } else {
              sum += (float)-0.005116526;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.5738615)) {
            if ( (data[4].fvalue < 1.3983052)) {
              sum += (float)0.41553557;
            } else {
              sum += (float)0.23408724;
            }
          } else {
            if (  (data[4].fvalue < 1.725136)) {
              sum += (float)0.079589084;
            } else {
              sum += (float)-0.18024103;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.725136)) {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.50714791)) {
              sum += (float)0.05698156;
            } else {
              sum += (float)-0.15628989;
            }
          } else {
            sum += (float)0.34301519;
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 2.3460734)) {
              sum += (float)-0.33359599;
            } else {
              sum += (float)-0.16874702;
            }
          } else {
            sum += (float)-0.042412564;
          }
        }
      }
    } else {
      sum += (float)0.32660627;
    }
  } else {
    if (  (data[6].fvalue < 0.1202234)) {
      if ( (data[4].fvalue < 1.1198572)) {
        if (  (data[0].fvalue < 3.5879951)) {
          if (  (data[14].fvalue < 0.17658119)) {
            if (  (data[3].fvalue < 0.20224114)) {
              sum += (float)0.30769292;
            } else {
              sum += (float)0.18513438;
            }
          } else {
            if (  (data[0].fvalue < 2.9244916)) {
              sum += (float)0.42750701;
            } else {
              sum += (float)0.30409148;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.059516169)) {
            sum += (float)0.27161154;
          } else {
            if (  (data[14].fvalue < 0.18536735)) {
              sum += (float)-0.17467937;
            } else {
              sum += (float)0.11850861;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.26493931)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.12862998;
            } else {
              sum += (float)0.28967589;
            }
          } else {
            if (  (data[0].fvalue < 3.8414001)) {
              sum += (float)0.27414754;
            } else {
              sum += (float)0.041709192;
            }
          }
        } else {
          if (  (data[0].fvalue < 5.0383301)) {
            sum += (float)0.36337781;
          } else {
            sum += (float)0.18940085;
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.76096892)) {
        if (  (data[4].fvalue < 1.0837156)) {
          if (  (data[3].fvalue < 0.50714791)) {
            if (  (data[14].fvalue < 0.15128025)) {
              sum += (float)0.16077077;
            } else {
              sum += (float)0.33391887;
            }
          } else {
            if (  (data[14].fvalue < 0.17658119)) {
              sum += (float)-0.080560699;
            } else {
              sum += (float)0.1820488;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.096765086)) {
            if (  (data[14].fvalue < 0.35580611)) {
              sum += (float)-0.060027655;
            } else {
              sum += (float)0.054265574;
            }
          } else {
            if (  (data[4].fvalue < 1.627301)) {
              sum += (float)0.21809241;
            } else {
              sum += (float)-0.0033613155;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.66284448)) {
          if (  (data[17].fvalue < 0.00019010108)) {
            if (  (data[14].fvalue < 0.76238382)) {
              sum += (float)-0.060402121;
            } else {
              sum += (float)0.055129021;
            }
          } else {
            if (  (data[17].fvalue < 0.18227009)) {
              sum += (float)-0.23624675;
            } else {
              sum += (float)-0.028340803;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[6].fvalue < 0.56069648)) {
              sum += (float)0.24617459;
            } else {
              sum += (float)0.13754971;
            }
          } else {
            if (  (data[16].fvalue < 0.0064854007)) {
              sum += (float)0.10034022;
            } else {
              sum += (float)-0.050362863;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 1.0964531)) {
    if (  (data[4].fvalue < 1.0132648)) {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[14].fvalue < 0.0045038355)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.30000612;
            } else {
              sum += (float)0.23785153;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.2059402;
            } else {
              sum += (float)-0.30787519;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.00073400844)) {
            if (  (data[14].fvalue < 0.044568397)) {
              sum += (float)0.12003797;
            } else {
              sum += (float)-0.045807499;
            }
          } else {
            if (  (data[14].fvalue < 0.032494728)) {
              sum += (float)-0.18659125;
            } else {
              sum += (float)-0.055104248;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.551678)) {
          if (  (data[3].fvalue < 0.15565081)) {
            if (  (data[13].fvalue < 0.0041854838)) {
              sum += (float)0.24221773;
            } else {
              sum += (float)0.37825555;
            }
          } else {
            if (  (data[14].fvalue < 0.07005851)) {
              sum += (float)-0.18200772;
            } else {
              sum += (float)0.23023956;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.17682217)) {
            if (  (data[13].fvalue < 0.15521677)) {
              sum += (float)-0.4626058;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[3].fvalue < 0.95930606)) {
              sum += (float)0.11710423;
            } else {
              sum += (float)-0.076897979;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1540904)) {
        if (  (data[14].fvalue < 0.038538381)) {
          if (  (data[13].fvalue < 0.038575295)) {
            if (  (data[16].fvalue < 0.072145432)) {
              sum += (float)-0.21437983;
            } else {
              sum += (float)-0.053868141;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.023409136;
            } else {
              sum += (float)-0.13427819;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.41479352)) {
            if (  (data[13].fvalue < 0.007934669)) {
              sum += (float)0.026393533;
            } else {
              sum += (float)0.12001523;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.043002199;
            } else {
              sum += (float)-0.056370776;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[13].fvalue < 0.0041854838)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.028649984;
            } else {
              sum += (float)0.021576524;
            }
          } else {
            if (  (data[14].fvalue < 0.0098249279)) {
              sum += (float)-0.021378407;
            } else {
              sum += (float)0.051078949;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.125063)) {
            if (  (data[14].fvalue < 0.32676038)) {
              sum += (float)-0.092140585;
            } else {
              sum += (float)0.0019512017;
            }
          } else {
            if (  (data[4].fvalue < 1.7658839)) {
              sum += (float)0.078862697;
            } else {
              sum += (float)-0.024696039;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 3.999999)) {
      if (  (data[3].fvalue < 0.10409278)) {
        sum += (float)-0.35249266;
      } else {
        if (  (data[13].fvalue < 0.00073400844)) {
          if (  (data[14].fvalue < 0.038538381)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.26317817;
            } else {
              sum += (float)0.072594523;
            }
          } else {
            if (  (data[3].fvalue < 0.22245207)) {
              sum += (float)-0.22620574;
            } else {
              sum += (float)0.04232531;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.0045038355)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.18260057;
            } else {
              sum += (float)-0.22622801;
            }
          } else {
            if (  (data[13].fvalue < 0.012028792)) {
              sum += (float)-0.178602;
            } else {
              sum += (float)-0.025433904;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.95930606)) {
        if (  (data[13].fvalue < 0.032409787)) {
          if (  (data[3].fvalue < 0.43159273)) {
            if (  (data[14].fvalue < 0.076767914)) {
              sum += (float)0.2096145;
            } else {
              sum += (float)0.397587;
            }
          } else {
            if (  (data[14].fvalue < 0.16817765)) {
              sum += (float)-0.25944728;
            } else {
              sum += (float)0.23058622;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.551678)) {
            sum += (float)0.41898295;
          } else {
            if (  (data[14].fvalue < 0.11216609)) {
              sum += (float)0.009363954;
            } else {
              sum += (float)0.37407371;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.19467258)) {
          sum += (float)-0.24289246;
        } else {
          if (  (data[3].fvalue < 1.730692)) {
            if (  (data[13].fvalue < 0.089038245)) {
              sum += (float)0.011813951;
            } else {
              sum += (float)0.23783965;
            }
          } else {
            if (  (data[14].fvalue < 0.55901217)) {
              sum += (float)-0.2519697;
            } else {
              sum += (float)-0.010502533;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 3.999999)) {
    if (  (data[7].fvalue < 1.999999)) {
      if (  (data[6].fvalue < 0.041965365)) {
        if (  (data[14].fvalue < 0.026823074)) {
          if (  (data[13].fvalue < 0.00039930563)) {
            if (  (data[0].fvalue < 1.2437104)) {
              sum += (float)-0.42675325;
            } else {
              sum += (float)0.13696313;
            }
          } else {
            if (  (data[14].fvalue < 0.0045703794)) {
              sum += (float)0.030797919;
            } else {
              sum += (float)-0.23574251;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[3].fvalue < 0.15600388)) {
              sum += (float)-0.13316855;
            } else {
              sum += (float)-0.012592998;
            }
          } else {
            if (  (data[4].fvalue < 0.99112844)) {
              sum += (float)0.2552264;
            } else {
              sum += (float)0.034825239;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[16].fvalue < 0.046732467)) {
            if (  (data[14].fvalue < 0.0045703794)) {
              sum += (float)0.10879011;
            } else {
              sum += (float)0.01252359;
            }
          } else {
            if (  (data[17].fvalue < 0.032387462)) {
              sum += (float)0.0050129183;
            } else {
              sum += (float)-0.072305627;
            }
          }
        } else {
          if (  (data[8].fvalue < 5.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)5.1565188e-05;
            } else {
              sum += (float)-0.094349593;
            }
          } else {
            if (  (data[13].fvalue < 0.045102883)) {
              sum += (float)0.0094250254;
            } else {
              sum += (float)0.18810917;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 0.99999899)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 2.999999)) {
            sum += (float)-0.35269788;
          } else {
            if (  (data[4].fvalue < 1.0060629)) {
              sum += (float)0.21358706;
            } else {
              sum += (float)-0.0099031879;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.026472267)) {
            sum += (float)-0.16213049;
          } else {
            if (  (data[3].fvalue < 0.45005316)) {
              sum += (float)0.20217806;
            } else {
              sum += (float)0.10539527;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 0.009966895)) {
            if (  (data[5].fvalue < 0.52306074)) {
              sum += (float)-0.13894865;
            } else {
              sum += (float)-0.29841486;
            }
          } else {
            if (  (data[13].fvalue < 0.021149192)) {
              sum += (float)-0.18308477;
            } else {
              sum += (float)-0.043519028;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.090726838)) {
            if ( (data[4].fvalue < 1.0857028)) {
              sum += (float)0.29095593;
            } else {
              sum += (float)0.026211636;
            }
          } else {
            if (  (data[13].fvalue < 0.078749187)) {
              sum += (float)-0.060474563;
            } else {
              sum += (float)0.024763532;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.21838817)) {
      if (  (data[6].fvalue < 0.060148716)) {
        if (  (data[0].fvalue < 4.5212889)) {
          sum += (float)0.36817852;
        } else {
          sum += (float)0.16514744;
        }
      } else {
        if (  (data[0].fvalue < 2.8809867)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.2932831)) {
              sum += (float)-0.049717207;
            } else {
              sum += (float)0.20917125;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.17214957;
            } else {
              sum += (float)0.36497289;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.11016177)) {
            if (  (data[14].fvalue < 0.18620872)) {
              sum += (float)-0.051197924;
            } else {
              sum += (float)0.07897301;
            }
          } else {
            if (  (data[4].fvalue < 1.4178529)) {
              sum += (float)0.26046053;
            } else {
              sum += (float)0.085276857;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.60722858)) {
        if (  (data[0].fvalue < 1.9051179)) {
          if (  (data[6].fvalue < 0.37158)) {
            if (  (data[14].fvalue < 0.21424478)) {
              sum += (float)0.050800543;
            } else {
              sum += (float)0.23952673;
            }
          } else {
            if (  (data[16].fvalue < 0.28003547)) {
              sum += (float)0.0015833388;
            } else {
              sum += (float)0.20931317;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.43313068)) {
            if (  (data[17].fvalue < 0.2409091)) {
              sum += (float)-0.07622613;
            } else {
              sum += (float)0.065367788;
            }
          } else {
            if (  (data[14].fvalue < 0.42747176)) {
              sum += (float)0.12161977;
            } else {
              sum += (float)-0.06889464;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.0073082503)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.23910782;
            } else {
              sum += (float)0.14868449;
            }
          } else {
            if (  (data[14].fvalue < 0.52700531)) {
              sum += (float)-0.0072741755;
            } else {
              sum += (float)0.15795317;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.1773597)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.12636726;
            } else {
              sum += (float)-0.17622519;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.10935312;
            } else {
              sum += (float)-0.045131415;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 0.036452636)) {
    if (  (data[4].fvalue < 1.8700299)) {
      if (  (data[17].fvalue < 0.13200782)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 0.19038743)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.046433896;
            } else {
              sum += (float)0.26294187;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.010621447;
            } else {
              sum += (float)0.140248;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.015783476)) {
            if (  (data[13].fvalue < 0.02614419)) {
              sum += (float)-0.029006636;
            } else {
              sum += (float)0.10668739;
            }
          } else {
            if (  (data[16].fvalue < 0.12571643)) {
              sum += (float)0.074221767;
            } else {
              sum += (float)0.15566429;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.0086678844)) {
          if (  (data[14].fvalue < 0.010296283)) {
            sum += (float)-0.032365911;
          } else {
            if (  (data[3].fvalue < 0.030169629)) {
              sum += (float)0.27966487;
            } else {
              sum += (float)0.17958145;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.003538856)) {
            sum += (float)-0.11303633;
          } else {
            if (  (data[14].fvalue < 0.010296283)) {
              sum += (float)-0.049102895;
            } else {
              sum += (float)0.13301976;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.99499434)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.021436438)) {
            sum += (float)0.27251643;
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.065660603;
            } else {
              sum += (float)-0.022576965;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.49227014)) {
            if (  (data[13].fvalue < 0.75497645)) {
              sum += (float)-0.068412565;
            } else {
              sum += (float)-0.21175697;
            }
          } else {
            if (  (data[14].fvalue < 0.12901908)) {
              sum += (float)-0.032547783;
            } else {
              sum += (float)0.041521251;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.30393767)) {
          if (  (data[16].fvalue < 0.34513146)) {
            sum += (float)-0.19371945;
          } else {
            sum += (float)-0.082916401;
          }
        } else {
          sum += (float)-0.04146117;
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 1.999999)) {
      if (  (data[17].fvalue < 0.18272075)) {
        if (  (data[17].fvalue < 0.032018807)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.12571643)) {
              sum += (float)-0.0069622584;
            } else {
              sum += (float)0.016714711;
            }
          } else {
            if (  (data[16].fvalue < 0.031565148)) {
              sum += (float)0.15092681;
            } else {
              sum += (float)0.015596949;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.38115)) {
            if (  (data[17].fvalue < 0.13200782)) {
              sum += (float)-0.036138706;
            } else {
              sum += (float)0.070255756;
            }
          } else {
            if (  (data[14].fvalue < 0.8555426)) {
              sum += (float)-0.069617495;
            } else {
              sum += (float)-0.15721731;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.0073300167)) {
            if (  (data[14].fvalue < 0.099003598)) {
              sum += (float)-0.15398853;
            } else {
              sum += (float)-0.0050194552;
            }
          } else {
            if (  (data[14].fvalue < 0.0048417775)) {
              sum += (float)-0.029282264;
            } else {
              sum += (float)0.10875974;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7673666)) {
            if (  (data[3].fvalue < 0.76400387)) {
              sum += (float)0.13772334;
            } else {
              sum += (float)0.016496453;
            }
          } else {
            if (  (data[17].fvalue < 0.66458601)) {
              sum += (float)-0.03473128;
            } else {
              sum += (float)0.10380621;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.49498379)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.003538856)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.2869882;
            } else {
              sum += (float)-0.15440296;
            }
          } else {
            if (  (data[4].fvalue < 1.3489037)) {
              sum += (float)-0.27341846;
            } else {
              sum += (float)-0.064455926;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[17].fvalue < 0.18272075)) {
              sum += (float)-0.11219343;
            } else {
              sum += (float)0.01853947;
            }
          } else {
            if (  (data[5].fvalue < 0.35824993)) {
              sum += (float)-0.16269805;
            } else {
              sum += (float)-0.31015858;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.003538856)) {
            if (  (data[17].fvalue < 0.9931432)) {
              sum += (float)-0.12619472;
            } else {
              sum += (float)-0.024720745;
            }
          } else {
            if (  (data[13].fvalue < 0.19038743)) {
              sum += (float)0.09213227;
            } else {
              sum += (float)-0.031037569;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[5].fvalue < 0.46683353)) {
              sum += (float)0.095518708;
            } else {
              sum += (float)-0.046631113;
            }
          } else {
            if (  (data[6].fvalue < 0.56143558)) {
              sum += (float)0.061787706;
            } else {
              sum += (float)-0.057599653;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.1907794)) {
    if (  (data[8].fvalue < 8.999999)) {
      if ( (data[4].fvalue < 0.83566976)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 0.41614434)) {
              sum += (float)-0.063339151;
            } else {
              sum += (float)0.10677142;
            }
          } else {
            sum += (float)-0.12774001;
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.028928837;
            } else {
              sum += (float)0.089405388;
            }
          } else {
            sum += (float)-0.30816555;
          }
        }
      } else {
        if (  (data[1].fvalue < 0.35343745)) {
          if (  (data[0].fvalue < 2.4754817)) {
            if (  (data[5].fvalue < 0.14275829)) {
              sum += (float)0.091953889;
            } else {
              sum += (float)0.0040743859;
            }
          } else {
            if (  (data[3].fvalue < 0.80470592)) {
              sum += (float)0.056281097;
            } else {
              sum += (float)0.30410296;
            }
          }
        } else {
          if (  (data[14].fvalue < 1.2603452)) {
            if (  (data[12].fvalue < 2.999999)) {
              sum += (float)-0.0044838386;
            } else {
              sum += (float)0.30623084;
            }
          } else {
            if (  (data[4].fvalue < 2.0245354)) {
              sum += (float)-0.022017196;
            } else {
              sum += (float)-0.12189244;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.3076867)) {
        if (  (data[3].fvalue < 0.69194031)) {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 0.30188268)) {
              sum += (float)0.082360908;
            } else {
              sum += (float)0.23486578;
            }
          } else {
            sum += (float)0.27660537;
          }
        } else {
          if (  (data[3].fvalue < 1.027877)) {
            sum += (float)0.078383341;
          } else {
            sum += (float)-0.07433787;
          }
        }
      } else {
        if (  (data[14].fvalue < 0.23469439)) {
          sum += (float)0.23895285;
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            sum += (float)0.16641435;
          } else {
            if (  (data[8].fvalue < 11.999999)) {
              sum += (float)-0.067472123;
            } else {
              sum += (float)0.16721153;
            }
          }
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 0.17820756)) {
      if (  (data[16].fvalue < 0.38482955)) {
        if ( (data[4].fvalue < 1.6565708)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.027097739;
            } else {
              sum += (float)-0.1200148;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.11065721;
            } else {
              sum += (float)-0.078856796;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.6592949)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.19882469;
            } else {
              sum += (float)-0.051229749;
            }
          } else {
            if (  (data[17].fvalue < 0.98766947)) {
              sum += (float)-0.024621634;
            } else {
              sum += (float)0.11365509;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.46730337)) {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.029693428)) {
              sum += (float)0.11925019;
            } else {
              sum += (float)0.001309376;
            }
          } else {
            if (  (data[16].fvalue < 1.365779)) {
              sum += (float)-0.04298766;
            } else {
              sum += (float)0.1160375;
            }
          }
        } else {
          if (  (data[2].fvalue < 0.89199072)) {
            if (  (data[13].fvalue < 0.37582076)) {
              sum += (float)-0.10883413;
            } else {
              sum += (float)0.015436592;
            }
          } else {
            if (  (data[6].fvalue < 0.64949411)) {
              sum += (float)-0.066963509;
            } else {
              sum += (float)-0.2216951;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.0086510507)) {
        if (  (data[14].fvalue < 0.59841287)) {
          if (  (data[7].fvalue < 3.999999)) {
            if (  (data[6].fvalue < 0.18340132)) {
              sum += (float)0.082050279;
            } else {
              sum += (float)0.0050071464;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.15072706;
            } else {
              sum += (float)0.036712181;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.50620842)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.013525411;
            } else {
              sum += (float)0.14015435;
            }
          } else {
            if (  (data[3].fvalue < 1.4928297)) {
              sum += (float)0.2784448;
            } else {
              sum += (float)0.13944805;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if ( (data[4].fvalue < 1.9374455)) {
            if (  (data[5].fvalue < 0.52325982)) {
              sum += (float)0.12529388;
            } else {
              sum += (float)0.018100675;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.035407193;
            } else {
              sum += (float)-0.06152866;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.7332722)) {
            if (  (data[4].fvalue < 1.4366535)) {
              sum += (float)0.089950666;
            } else {
              sum += (float)-0.027336305;
            }
          } else {
            if (  (data[5].fvalue < 0.54364979)) {
              sum += (float)-0.0189589;
            } else {
              sum += (float)-0.18738197;
            }
          }
        }
      }
    }
  }
  if (  (data[7].fvalue < 8.999999)) {
    if (  (data[17].fvalue < 0.23774788)) {
      if (  (data[17].fvalue < 0.062243931)) {
        if (  (data[13].fvalue < 0.32873407)) {
          if (  (data[16].fvalue < 0.12553193)) {
            if ( (data[4].fvalue < 1.1549091)) {
              sum += (float)0.0051884931;
            } else {
              sum += (float)-0.025784744;
            }
          } else {
            if (  (data[3].fvalue < 0.52984387)) {
              sum += (float)0.031345554;
            } else {
              sum += (float)-0.031812094;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.44899306)) {
            if (  (data[16].fvalue < 0.56528109)) {
              sum += (float)0.001399928;
            } else {
              sum += (float)0.073487625;
            }
          } else {
            if (  (data[13].fvalue < 0.50284076)) {
              sum += (float)0.061886672;
            } else {
              sum += (float)0.16826922;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.85337847)) {
          if (  (data[17].fvalue < 0.13017964)) {
            if (  (data[3].fvalue < 0.080083363)) {
              sum += (float)0.011763007;
            } else {
              sum += (float)-0.067982309;
            }
          } else {
            if ( (data[4].fvalue < 1.3973576)) {
              sum += (float)0.064249925;
            } else {
              sum += (float)-0.024203654;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.24176367;
            } else {
              sum += (float)-0.038281154;
            }
          } else {
            if (  (data[4].fvalue < 1.8120586)) {
              sum += (float)0.015501456;
            } else {
              sum += (float)-0.13193575;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 0.45051473)) {
        if (  (data[16].fvalue < 0.0084424503)) {
          if (  (data[14].fvalue < 0.027417762)) {
            if (  (data[13].fvalue < 0.011202978)) {
              sum += (float)-0.19546078;
            } else {
              sum += (float)0.03110243;
            }
          } else {
            if (  (data[3].fvalue < 0.41576827)) {
              sum += (float)0.10565367;
            } else {
              sum += (float)0.036738724;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.38482094)) {
            if (  (data[13].fvalue < 0.003355802)) {
              sum += (float)-0.11124087;
            } else {
              sum += (float)0.0061138286;
            }
          } else {
            if (  (data[2].fvalue < 0.89347452)) {
              sum += (float)0.048549239;
            } else {
              sum += (float)0.24522978;
            }
          }
        }
      } else {
        if (  (data[2].fvalue < 0.78671974)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.003355802)) {
              sum += (float)-0.087659612;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[3].fvalue < 1.7336738)) {
              sum += (float)0.038181797;
            } else {
              sum += (float)-0.05079861;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.61588424)) {
            if (  (data[17].fvalue < 0.30225745)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.076575756;
            }
          } else {
            if (  (data[5].fvalue < 0.52405316)) {
              sum += (float)-0.05819064;
            } else {
              sum += (float)-0.16693667;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.44194418)) {
      if (  (data[0].fvalue < 3.839957)) {
        sum += (float)0.24556109;
      } else {
        sum += (float)0.10727751;
      }
    } else {
      if (  (data[7].fvalue < 10.999999)) {
        if (  (data[8].fvalue < 5.999999)) {
          sum += (float)0.1300844;
        } else {
          if (  (data[0].fvalue < 2.8365977)) {
            sum += (float)0.074299015;
          } else {
            sum += (float)-0.10271187;
          }
        }
      } else {
        sum += (float)0.16251253;
      }
    }
  }
  if (  (data[14].fvalue < 0.15244514)) {
    if (  (data[5].fvalue < 0.92957735)) {
      if (  (data[4].fvalue < 0.95234543)) {
        if (  (data[3].fvalue < 0.07178548)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 0.94308203)) {
              sum += (float)-0.2824876;
            } else {
              sum += (float)-0.44913921;
            }
          } else {
            sum += (float)-0.24162452;
          }
        } else {
          if (  (data[14].fvalue < 0.039156236)) {
            if (  (data[13].fvalue < 0.0032732785)) {
              sum += (float)0.1869209;
            } else {
              sum += (float)-0.06592121;
            }
          } else {
            if (  (data[3].fvalue < 0.12977029)) {
              sum += (float)-0.31534904;
            } else {
              sum += (float)-0.051799908;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.30141523)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 0.044127055)) {
              sum += (float)-0.014064568;
            } else {
              sum += (float)0.016361561;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.0085469652;
            } else {
              sum += (float)0.17930222;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.030999672)) {
              sum += (float)0.027836416;
            } else {
              sum += (float)-0.024764571;
            }
          } else {
            if (  (data[16].fvalue < 0.16161795)) {
              sum += (float)-0.10433115;
            } else {
              sum += (float)-0.014231998;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.096381754)) {
        if (  (data[5].fvalue < 0.97743708)) {
          sum += (float)-0.04605205;
        } else {
          if (  (data[14].fvalue < 0.027171724)) {
            if (  (data[3].fvalue < 0.079871044)) {
              sum += (float)-0.42267588;
            } else {
              sum += (float)-0.2535179;
            }
          } else {
            sum += (float)-0.20136449;
          }
        }
      } else {
        if (  (data[3].fvalue < 0.31402618)) {
          if (  (data[14].fvalue < 0.077681437)) {
            if (  (data[13].fvalue < 0.0032732785)) {
              sum += (float)0.087321058;
            } else {
              sum += (float)-0.025690101;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.16626875;
            } else {
              sum += (float)0.38964063;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.14150995;
            } else {
              sum += (float)-0.17189845;
            }
          } else {
            if (  (data[3].fvalue < 0.7239722)) {
              sum += (float)-0.0084352307;
            } else {
              sum += (float)-0.18687624;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.12129562)) {
      if (  (data[10].fvalue < 0.99999899)) {
        if (  (data[11].fvalue < 0.99999899)) {
          if (  (data[4].fvalue < 1.1540649)) {
            if (  (data[13].fvalue < 0.0032732785)) {
              sum += (float)0.065184265;
            } else {
              sum += (float)0.27364174;
            }
          } else {
            if (  (data[14].fvalue < 0.25495139)) {
              sum += (float)-0.10563465;
            } else {
              sum += (float)0.0067450115;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3063164)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.070349582;
            } else {
              sum += (float)0.32728231;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.14351274;
            } else {
              sum += (float)0.037679121;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.3959056)) {
          if (  (data[16].fvalue < 0.046811152)) {
            if (  (data[14].fvalue < 0.24436043)) {
              sum += (float)0.25652626;
            } else {
              sum += (float)0.40392435;
            }
          } else {
            if (  (data[13].fvalue < 0.0032732785)) {
              sum += (float)0.11341237;
            } else {
              sum += (float)0.2499278;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.028227333)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.099489138;
            } else {
              sum += (float)-0.034971658;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.098656863;
            } else {
              sum += (float)0.01921151;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.9988901)) {
        if (  (data[17].fvalue < 0.65923268)) {
          if ( (data[4].fvalue < 1.8105905)) {
            if (  (data[13].fvalue < 0.37113565)) {
              sum += (float)0.00030392525;
            } else {
              sum += (float)0.054728191;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.05249799;
            } else {
              sum += (float)-0.24420242;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.48630732)) {
            if (  (data[14].fvalue < 0.42685196)) {
              sum += (float)0.031406194;
            } else {
              sum += (float)0.1213669;
            }
          } else {
            if (  (data[14].fvalue < 0.69263607)) {
              sum += (float)-0.11745941;
            } else {
              sum += (float)-0.013539069;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.7222968)) {
          if (  (data[13].fvalue < 0.011096075)) {
            if (  (data[4].fvalue < 1.6540197)) {
              sum += (float)-0.20872733;
            } else {
              sum += (float)-0.070707314;
            }
          } else {
            sum += (float)0.025039067;
          }
        } else {
          if (  (data[14].fvalue < 0.32732186)) {
            if (  (data[1].fvalue < 0.77324551)) {
              sum += (float)0.12429918;
            } else {
              sum += (float)0.035140503;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.18944536;
            } else {
              sum += (float)0.050276298;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.4270491)) {
    if (  (data[3].fvalue < 1.0256431)) {
      if (  (data[12].fvalue < 0.99999899)) {
        if (  (data[13].fvalue < 0.015548621)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.0089504318;
            } else {
              sum += (float)-0.12565656;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.0054204063;
            } else {
              sum += (float)0.22835195;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.1383067)) {
              sum += (float)0.023855584;
            } else {
              sum += (float)-0.035744224;
            }
          } else {
            if (  (data[14].fvalue < 0.0047879857)) {
              sum += (float)-0.0086525744;
            } else {
              sum += (float)0.046146765;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.6858966)) {
          if (  (data[4].fvalue < 1.3620894)) {
            sum += (float)0.37609199;
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.10271534;
            } else {
              sum += (float)0.33097047;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8102144)) {
            if (  (data[3].fvalue < 0.18401209)) {
              sum += (float)0.086106747;
            } else {
              sum += (float)-0.078806072;
            }
          } else {
            if (  (data[16].fvalue < 0.99618465)) {
              sum += (float)-0.23201159;
            } else {
              sum += (float)0.050815549;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 1.999999)) {
        if ( (data[4].fvalue < 1.0325972)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.008474662)) {
              sum += (float)0.23126978;
            } else {
              sum += (float)0.13453463;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.15334827;
            } else {
              sum += (float)-0.19081934;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7626289)) {
            if (  (data[13].fvalue < 0.37147811)) {
              sum += (float)-0.11711047;
            } else {
              sum += (float)-0.0089407628;
            }
          } else {
            if (  (data[1].fvalue < 0.35484189)) {
              sum += (float)0.25605974;
            } else {
              sum += (float)0.024944426;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.52377361)) {
          if (  (data[14].fvalue < 0.084580652)) {
            if (  (data[13].fvalue < 0.37147811)) {
              sum += (float)-0.069450915;
            } else {
              sum += (float)-0.21058518;
            }
          } else {
            if (  (data[16].fvalue < 0.79534382)) {
              sum += (float)-0.039831802;
            } else {
              sum += (float)0.060718987;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.3250309)) {
            if (  (data[13].fvalue < 0.20897746)) {
              sum += (float)-0.14073551;
            } else {
              sum += (float)-0.022056049;
            }
          } else {
            if (  (data[13].fvalue < 0.60030508)) {
              sum += (float)-0.19675924;
            } else {
              sum += (float)-0.088101171;
            }
          }
        }
      }
    }
  } else {
    if (  (data[11].fvalue < 1.999999)) {
      if (  (data[17].fvalue < 0.49091759)) {
        if (  (data[13].fvalue < 0.42755941)) {
          if ( (data[4].fvalue < 1.476293)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.01819944;
            } else {
              sum += (float)0.058044486;
            }
          } else {
            if (  (data[16].fvalue < 0.49116451)) {
              sum += (float)-0.030392073;
            } else {
              sum += (float)0.066615015;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 0.68633127)) {
              sum += (float)0.12910041;
            } else {
              sum += (float)-0.036757931;
            }
          } else {
            if (  (data[5].fvalue < 0.30736798)) {
              sum += (float)-0.017463671;
            } else {
              sum += (float)-0.18529889;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.37300509)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.2676163;
            } else {
              sum += (float)0.065908104;
            }
          } else {
            if (  (data[13].fvalue < 0.15343723)) {
              sum += (float)-0.003591062;
            } else {
              sum += (float)0.094542295;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.6344229)) {
            if (  (data[16].fvalue < 0.008474662)) {
              sum += (float)0.077077791;
            } else {
              sum += (float)-0.014879734;
            }
          } else {
            if (  (data[0].fvalue < 4.7346191)) {
              sum += (float)-0.085524641;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.98995572)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.031428102)) {
              sum += (float)-0.23566616;
            } else {
              sum += (float)-0.1104385;
            }
          } else {
            if (  (data[4].fvalue < 1.865602)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.15004504;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.98871851)) {
              sum += (float)0.00096811273;
            } else {
              sum += (float)-0.077671021;
            }
          } else {
            if (  (data[13].fvalue < 0.18837313)) {
              sum += (float)-0.25541976;
            } else {
              sum += (float)-0.1062411;
            }
          }
        }
      } else {
        if (  (data[11].fvalue < 2.999999)) {
          if (  (data[14].fvalue < 0.98871851)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.12112379;
            } else {
              sum += (float)0.012032516;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.080358207;
            } else {
              sum += (float)0.089426719;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            sum += (float)0.013154494;
          } else {
            sum += (float)-0.10434241;
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 4.999999)) {
    if (  (data[0].fvalue < 2.1172283)) {
      if (  (data[6].fvalue < 0.022649596)) {
        if (  (data[14].fvalue < 0.021292945)) {
          sum += (float)-0.36856428;
        } else {
          sum += (float)-0.15906473;
        }
      } else {
        if (  (data[8].fvalue < 4.999999)) {
          if (  (data[6].fvalue < 0.096889503)) {
            if (  (data[14].fvalue < 0.05786515)) {
              sum += (float)0.025781177;
            } else {
              sum += (float)-0.079148427;
            }
          } else {
            if (  (data[14].fvalue < 0.051483866)) {
              sum += (float)-0.020199463;
            } else {
              sum += (float)0.017871579;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.20325184)) {
            if ( (data[4].fvalue < 1.1020825)) {
              sum += (float)0.25604147;
            } else {
              sum += (float)0.13166603;
            }
          } else {
            if (  (data[17].fvalue < 0.17685533)) {
              sum += (float)0.02848603;
            } else {
              sum += (float)0.21301891;
            }
          }
        }
      }
    } else {
      if (  (data[1].fvalue < 0.35440299)) {
        if (  (data[3].fvalue < 0.69022763)) {
          if (  (data[16].fvalue < 0.99606299)) {
            if (  (data[13].fvalue < 1.0326662)) {
              sum += (float)-0.0162089;
            } else {
              sum += (float)-0.21269919;
            }
          } else {
            if (  (data[14].fvalue < 0.05786515)) {
              sum += (float)0.044256188;
            } else {
              sum += (float)0.15270549;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.45146751)) {
            if (  (data[16].fvalue < 0.79490674)) {
              sum += (float)0.14777613;
            } else {
              sum += (float)0.29340732;
            }
          } else {
            sum += (float)-0.12920459;
          }
        }
      } else {
        if (  (data[6].fvalue < 0.53504473)) {
          if (  (data[13].fvalue < 0.20899686)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.0027106188;
            } else {
              sum += (float)-0.02494785;
            }
          } else {
            if (  (data[6].fvalue < 0.30283958)) {
              sum += (float)0.035714727;
            } else {
              sum += (float)0.0018781844;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[5].fvalue < 0.41866726)) {
              sum += (float)-0.00034264871;
            } else {
              sum += (float)-0.063295543;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.031145491;
            } else {
              sum += (float)-0.10278694;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.31555653;
  }
  if (  (data[14].fvalue < 0.28961667)) {
    if (  (data[7].fvalue < 4.999999)) {
      if ( (data[4].fvalue < 1.1375335)) {
        if (  (data[8].fvalue < 0.99999899)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.38237557;
            } else {
              sum += (float)0.060966235;
            }
          } else {
            if (  (data[13].fvalue < 0.020275917)) {
              sum += (float)-0.081374504;
            } else {
              sum += (float)0.063864134;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.0047761202)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.10832157;
            } else {
              sum += (float)-0.22049357;
            }
          } else {
            if (  (data[4].fvalue < 1.0547976)) {
              sum += (float)-0.0073579461;
            } else {
              sum += (float)0.0424169;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.0068600844)) {
              sum += (float)-0.018954003;
            } else {
              sum += (float)0.017884601;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.091063157;
            } else {
              sum += (float)-0.12604941;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.12715225)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.12364936;
            } else {
              sum += (float)0.0024432919;
            }
          } else {
            if (  (data[13].fvalue < 0.18819243)) {
              sum += (float)0.12647697;
            } else {
              sum += (float)-0.039614342;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.14510991)) {
        if ( (data[4].fvalue < 1.0845327)) {
          if (  (data[3].fvalue < 0.27817303)) {
            sum += (float)0.29964262;
          } else {
            if (  (data[14].fvalue < 0.064408727)) {
              sum += (float)0.0090698507;
            } else {
              sum += (float)0.22364669;
            }
          }
        } else {
          if (  (data[8].fvalue < 5.999999)) {
            if (  (data[16].fvalue < 0.0075650867)) {
              sum += (float)-0;
            } else {
              sum += (float)0.19575806;
            }
          } else {
            if (  (data[4].fvalue < 1.232466)) {
              sum += (float)0.22841239;
            } else {
              sum += (float)0.084670208;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.059349678)) {
          if (  (data[14].fvalue < 0.077837199)) {
            sum += (float)0.29328898;
          } else {
            if (  (data[1].fvalue < 1.2839376)) {
              sum += (float)0.15241043;
            } else {
              sum += (float)0.014048476;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.28923988)) {
              sum += (float)-0.048322383;
            } else {
              sum += (float)0.1653555;
            }
          } else {
            if (  (data[16].fvalue < 0.16140793)) {
              sum += (float)-0.023279648;
            } else {
              sum += (float)0.066088393;
            }
          }
        }
      }
    }
  } else {
    if (  (data[16].fvalue < 0.99635983)) {
      if (  (data[0].fvalue < 2.5902791)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 0.13789231)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.028618196;
            } else {
              sum += (float)-0.013488659;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.03303659;
            } else {
              sum += (float)0.084478803;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.30231106)) {
            if (  (data[4].fvalue < 1.4332012)) {
              sum += (float)0.21534605;
            } else {
              sum += (float)0.046544883;
            }
          } else {
            if (  (data[13].fvalue < 0.0031280294)) {
              sum += (float)0.0019589462;
            } else {
              sum += (float)0.056963485;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.98235232)) {
          if (  (data[6].fvalue < 0.51356292)) {
            if (  (data[13].fvalue < 0.49956802)) {
              sum += (float)0.00086966873;
            } else {
              sum += (float)0.069024459;
            }
          } else {
            if (  (data[15].fvalue < 0.38205057)) {
              sum += (float)-0.060581412;
            } else {
              sum += (float)0.012575739;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8092835)) {
            if (  (data[13].fvalue < 0.010994731)) {
              sum += (float)-0.043276813;
            } else {
              sum += (float)0.10441552;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.15401289;
            } else {
              sum += (float)0.026363887;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.8644625)) {
        if (  (data[16].fvalue < 1.3614265)) {
          if (  (data[5].fvalue < 0.58825439)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0;
            } else {
              sum += (float)0.09079513;
            }
          } else {
            sum += (float)-0.086496741;
          }
        } else {
          sum += (float)-0.10316629;
        }
      } else {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 2.0199776)) {
              sum += (float)0.089448929;
            } else {
              sum += (float)0.20553826;
            }
          } else {
            sum += (float)-0.0080818534;
          }
        } else {
          if (  (data[1].fvalue < 0.47510889)) {
            sum += (float)0.15804234;
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.077433251;
            } else {
              sum += (float)-0.013105274;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 0.026932569)) {
    if (  (data[4].fvalue < 1.8648927)) {
      if (  (data[17].fvalue < 0.091647223)) {
        if (  (data[8].fvalue < 5.999999)) {
          if (  (data[4].fvalue < 1.4756808)) {
            if (  (data[16].fvalue < 0.31011838)) {
              sum += (float)0.01606629;
            } else {
              sum += (float)-0.062499769;
            }
          } else {
            if (  (data[14].fvalue < 0.5589658)) {
              sum += (float)0.08843933;
            } else {
              sum += (float)-0.02097363;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6238405)) {
            if (  (data[14].fvalue < 0.17853285)) {
              sum += (float)0.012190192;
            } else {
              sum += (float)0.18549643;
            }
          } else {
            sum += (float)0.012795069;
          }
        }
      } else {
        if (  (data[14].fvalue < 0.01044419)) {
          sum += (float)-0.046660408;
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[5].fvalue < 0.016123531)) {
              sum += (float)0.20873109;
            } else {
              sum += (float)0.12304999;
            }
          } else {
            if (  (data[13].fvalue < 0.0029507438)) {
              sum += (float)-0.043947209;
            } else {
              sum += (float)0.1028108;
            }
          }
        }
      }
    } else {
      if (  (data[10].fvalue < 1.999999)) {
        if (  (data[3].fvalue < 0.02161452)) {
          sum += (float)0.086140774;
        } else {
          if (  (data[6].fvalue < 0.58515733)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.020935165;
            } else {
              sum += (float)-0.098825909;
            }
          } else {
            if (  (data[16].fvalue < 1.3593947)) {
              sum += (float)0.026001319;
            } else {
              sum += (float)-0.043143183;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.28884864)) {
          if (  (data[14].fvalue < 0.20572856)) {
            sum += (float)-0;
          } else {
            sum += (float)-0.10096891;
          }
        } else {
          sum += (float)-0.17404243;
        }
      }
    }
  } else {
    if (  (data[14].fvalue < 0.092051551)) {
      if (  (data[4].fvalue < 0.9985863)) {
        if (  (data[13].fvalue < 0.0029507438)) {
          if (  (data[3].fvalue < 0.12204613)) {
            if (  (data[14].fvalue < 0.033548024)) {
              sum += (float)0.086254381;
            } else {
              sum += (float)-0.12437858;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.1227185;
            } else {
              sum += (float)-0.26614454;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[14].fvalue < 0.0050489879)) {
              sum += (float)0.040724926;
            } else {
              sum += (float)-0.090733714;
            }
          } else {
            if (  (data[3].fvalue < 0.21402897)) {
              sum += (float)0.40161827;
            } else {
              sum += (float)-0.029271429;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[16].fvalue < 0.031545091)) {
            if (  (data[4].fvalue < 1.6853577)) {
              sum += (float)-0.00079932704;
            } else {
              sum += (float)0.17301893;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.029294308;
            } else {
              sum += (float)0.046289563;
            }
          }
        } else {
          if (  (data[8].fvalue < 4.999999)) {
            if (  (data[14].fvalue < 0.02753965)) {
              sum += (float)-0.14917856;
            } else {
              sum += (float)-0.047564454;
            }
          } else {
            if (  (data[17].fvalue < 0.091647223)) {
              sum += (float)0.046442427;
            } else {
              sum += (float)0.26943225;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.060698189)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[0].fvalue < 2.9678767)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.23332298;
            } else {
              sum += (float)-0.034507129;
            }
          } else {
            if (  (data[6].fvalue < 0.042546637)) {
              sum += (float)-0.10088746;
            } else {
              sum += (float)0.013879948;
            }
          }
        } else {
          if (  (data[0].fvalue < 3.5127795)) {
            if (  (data[13].fvalue < 0.0029507438)) {
              sum += (float)0.27480492;
            } else {
              sum += (float)0.47804371;
            }
          } else {
            if (  (data[6].fvalue < 0.036376573)) {
              sum += (float)0.24957135;
            } else {
              sum += (float)0.080588803;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.28003263)) {
          if ( (data[4].fvalue < 1.2522322)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.009669709;
            } else {
              sum += (float)0.34952122;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.00075752911;
            } else {
              sum += (float)-0.033001777;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.46936867)) {
            if (  (data[4].fvalue < 1.6853577)) {
              sum += (float)0.096517831;
            } else {
              sum += (float)0.0064865085;
            }
          } else {
            if (  (data[5].fvalue < 0.50543517)) {
              sum += (float)0.020180648;
            } else {
              sum += (float)-0.024213828;
            }
          }
        }
      }
    }
  }
  if (  (data[6].fvalue < 0.022832397)) {
    if (  (data[8].fvalue < 2.999999)) {
      if (  (data[0].fvalue < 2.3567531)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[14].fvalue < 0.010564036)) {
            if (  (data[0].fvalue < 2.1582468)) {
              sum += (float)-0.33996174;
            } else {
              sum += (float)-0.037317712;
            }
          } else {
            sum += (float)-0.36917359;
          }
        } else {
          sum += (float)0.031458456;
        }
      } else {
        if (  (data[14].fvalue < 0.021799415)) {
          if (  (data[13].fvalue < 0.0028949338)) {
            if (  (data[3].fvalue < 0.047549322)) {
              sum += (float)0.013226925;
            } else {
              sum += (float)0.16801535;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.12396324;
            } else {
              sum += (float)0.075327404;
            }
          }
        } else {
          if (  (data[0].fvalue < 3.5122659)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.28630105;
            } else {
              sum += (float)-0.015622536;
            }
          } else {
            if (  (data[14].fvalue < 0.052261818)) {
              sum += (float)0.04064633;
            } else {
              sum += (float)-0.14047474;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 0.99999899)) {
        if (  (data[14].fvalue < 0.046000894)) {
          sum += (float)-0;
        } else {
          sum += (float)0.32149419;
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          sum += (float)0.55616087;
        } else {
          sum += (float)0.31473467;
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.17578654)) {
      if (  (data[8].fvalue < 4.999999)) {
        if (  (data[16].fvalue < 0.11039685)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.012120427;
            } else {
              sum += (float)0.17041746;
            }
          } else {
            if (  (data[14].fvalue < 0.010564036)) {
              sum += (float)-0.14060137;
            } else {
              sum += (float)0.06515193;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.2523322)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.08359734;
            } else {
              sum += (float)0.38949174;
            }
          } else {
            if (  (data[14].fvalue < 0.027757112)) {
              sum += (float)-0.016797632;
            } else {
              sum += (float)0.047548603;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1686239)) {
          if (  (data[14].fvalue < 0.19713409)) {
            if (  (data[4].fvalue < 1.1212795)) {
              sum += (float)0.24312863;
            } else {
              sum += (float)0.047243521;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.23645763;
            } else {
              sum += (float)0.5165199;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.077145182)) {
            if (  (data[16].fvalue < 0.0090760412)) {
              sum += (float)-0.01376081;
            } else {
              sum += (float)0.16669972;
            }
          } else {
            if (  (data[4].fvalue < 1.3781116)) {
              sum += (float)0.17587857;
            } else {
              sum += (float)0.035490185;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.52744657)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 0.047364585)) {
              sum += (float)0.010192284;
            } else {
              sum += (float)-0.010861509;
            }
          } else {
            if (  (data[13].fvalue < 0.49874875)) {
              sum += (float)-0.097443923;
            } else {
              sum += (float)0.032900061;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.043738037)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.01433167;
            } else {
              sum += (float)-0.11635162;
            }
          } else {
            if (  (data[6].fvalue < 0.1395327)) {
              sum += (float)0.11005531;
            } else {
              sum += (float)-0.024104914;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.02779083)) {
          if (  (data[13].fvalue < 0.49874875)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.026760155;
            } else {
              sum += (float)-0.030562211;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.14639714;
            } else {
              sum += (float)-0.01188558;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.37925583)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.10965879;
            } else {
              sum += (float)0.010461882;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.069575101;
            } else {
              sum += (float)-0.030771302;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.098589823)) {
    if (  (data[7].fvalue < 4.999999)) {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[4].fvalue < 1.0439687)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.32093954;
            } else {
              sum += (float)-0.010692533;
            }
          } else {
            if (  (data[14].fvalue < 0.0857706)) {
              sum += (float)-0.032925759;
            } else {
              sum += (float)0.018079659;
            }
          }
        } else {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.64176565)) {
              sum += (float)-0.0074061984;
            } else {
              sum += (float)0.086673833;
            }
          } else {
            if (  (data[13].fvalue < 0.0065071788)) {
              sum += (float)-0.20371106;
            } else {
              sum += (float)-0.047271058;
            }
          }
        }
      } else {
        if (  (data[6].fvalue < 0.043092288)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.025295088)) {
              sum += (float)-0.22325227;
            } else {
              sum += (float)-0.024183514;
            }
          } else {
            if (  (data[0].fvalue < 3.0114043)) {
              sum += (float)0.54982597;
            } else {
              sum += (float)0.22715183;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.60416466)) {
            if (  (data[4].fvalue < 1.2139254)) {
              sum += (float)0.016826829;
            } else {
              sum += (float)-0.053766426;
            }
          } else {
            if (  (data[13].fvalue < 0.037192728)) {
              sum += (float)-0.19955815;
            } else {
              sum += (float)-0.089419827;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.043794852)) {
        if (  (data[3].fvalue < 0.38619155)) {
          sum += (float)0.25030777;
        } else {
          sum += (float)0.082880504;
        }
      } else {
        if (  (data[14].fvalue < 0.11500112)) {
          if (  (data[16].fvalue < 0.084869601)) {
            if (  (data[6].fvalue < 0.085720465)) {
              sum += (float)0.14725697;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.26024756;
          }
        } else {
          if (  (data[3].fvalue < 0.57851964)) {
            if (  (data[8].fvalue < 6.999999)) {
              sum += (float)-0.05393409;
            } else {
              sum += (float)0.12114955;
            }
          } else {
            if (  (data[14].fvalue < 0.3160831)) {
              sum += (float)-0.023014378;
            } else {
              sum += (float)-0.26764134;
            }
          }
        }
      }
    }
  } else {
    if (  (data[6].fvalue < 0.3462562)) {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[16].fvalue < 0.18269403)) {
          if (  (data[13].fvalue < 0.42604452)) {
            if (  (data[3].fvalue < 0.40169424)) {
              sum += (float)0.011140675;
            } else {
              sum += (float)-0.021215845;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.026134027;
            } else {
              sum += (float)0.14280094;
            }
          }
        } else {
          if (  (data[0].fvalue < 2.7101514)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.075795136;
            } else {
              sum += (float)0.18076028;
            }
          } else {
            if (  (data[6].fvalue < 0.23353827)) {
              sum += (float)0.064393215;
            } else {
              sum += (float)0.02134992;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 2.5123689)) {
          if (  (data[14].fvalue < 0.21645601)) {
            if (  (data[6].fvalue < 0.164323)) {
              sum += (float)0.27691126;
            } else {
              sum += (float)0.019280799;
            }
          } else {
            if (  (data[6].fvalue < 0.24945503)) {
              sum += (float)0.32451275;
            } else {
              sum += (float)0.13325486;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.20688114)) {
            if (  (data[6].fvalue < 0.10990272)) {
              sum += (float)0.11982442;
            } else {
              sum += (float)-0.069061019;
            }
          } else {
            if (  (data[6].fvalue < 0.17064233)) {
              sum += (float)0.15526056;
            } else {
              sum += (float)0.031390369;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.49007684)) {
        if ( (data[4].fvalue < 1.7621644)) {
          if (  (data[3].fvalue < 0.85042161)) {
            if (  (data[14].fvalue < 0.69498134)) {
              sum += (float)0.0063731004;
            } else {
              sum += (float)0.1357038;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.011531323;
            } else {
              sum += (float)-0.079349197;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.0050997692)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.16470735;
            } else {
              sum += (float)0.062968038;
            }
          } else {
            if (  (data[15].fvalue < 0.15081076)) {
              sum += (float)-0.042355843;
            } else {
              sum += (float)0.040510714;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.5454219)) {
          if (  (data[13].fvalue < 0.42604452)) {
            if (  (data[14].fvalue < 0.76211852)) {
              sum += (float)0.015685223;
            } else {
              sum += (float)-0.14231491;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.13477294;
            } else {
              sum += (float)-0.04166371;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.010541356)) {
            if (  (data[13].fvalue < 0.59986991)) {
              sum += (float)-0.17943403;
            } else {
              sum += (float)-0.050547753;
            }
          } else {
            if (  (data[6].fvalue < 0.68790811)) {
              sum += (float)0.031324472;
            } else {
              sum += (float)-0.12355058;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.010566443)) {
    if (  (data[8].fvalue < 0.99999899)) {
      if (  (data[3].fvalue < 0.55413896)) {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 0.252673)) {
            if (  (data[4].fvalue < 1.5955185)) {
              sum += (float)0.061634831;
            } else {
              sum += (float)0.40168211;
            }
          } else {
            if (  (data[4].fvalue < 1.4331814)) {
              sum += (float)-0.087000214;
            } else {
              sum += (float)0.022998361;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0064444346)) {
            sum += (float)-0.3227697;
          } else {
            sum += (float)-0.22811212;
          }
        }
      } else {
        if (  (data[16].fvalue < 0.66071641)) {
          if (  (data[17].fvalue < 0.38050219)) {
            if (  (data[16].fvalue < 0.0095086889)) {
              sum += (float)0.52692306;
            } else {
              sum += (float)0.75058985;
            }
          } else {
            sum += (float)0.2470949;
          }
        } else {
          if (  (data[4].fvalue < 2.1397271)) {
            sum += (float)-0.20401531;
          } else {
            sum += (float)0.16945826;
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.0052865981)) {
        if (  (data[4].fvalue < 1.0067328)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.63500935;
            } else {
              sum += (float)0.23202808;
            }
          } else {
            if (  (data[13].fvalue < 0.0026680399)) {
              sum += (float)0.21138468;
            } else {
              sum += (float)-0.0059996592;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 0.14354506)) {
              sum += (float)-0.29649854;
            } else {
              sum += (float)-0.37971753;
            }
          } else {
            sum += (float)-0.0029340666;
          }
        }
      } else {
        if ( (data[4].fvalue < 1.4331814)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.0026680399)) {
              sum += (float)-0.00017405319;
            } else {
              sum += (float)-0.042780712;
            }
          } else {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.31742328;
            } else {
              sum += (float)-0.069559567;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.30968252)) {
              sum += (float)-0.067754395;
            } else {
              sum += (float)-0.0063081258;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.032831516;
            } else {
              sum += (float)-0.12956336;
            }
          }
        }
      }
    }
  } else {
    if (  (data[17].fvalue < 0.12874824)) {
      if (  (data[16].fvalue < 0.12550126)) {
        if (  (data[3].fvalue < 0.48967475)) {
          if (  (data[4].fvalue < 1.2134966)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.017760014;
            } else {
              sum += (float)0.055129558;
            }
          } else {
            if (  (data[14].fvalue < 0.21624088)) {
              sum += (float)-0.054140821;
            } else {
              sum += (float)0.0055663865;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.089648046;
            } else {
              sum += (float)0.030028017;
            }
          } else {
            if (  (data[13].fvalue < 0.18803422)) {
              sum += (float)-0.084888332;
            } else {
              sum += (float)-0.012495645;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.3615268)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[3].fvalue < 0.48967475)) {
              sum += (float)0.084104508;
            } else {
              sum += (float)0.012027943;
            }
          } else {
            if (  (data[3].fvalue < 1.489351)) {
              sum += (float)0.26023152;
            } else {
              sum += (float)-0.066396736;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.252673)) {
            if (  (data[14].fvalue < 0.1708885)) {
              sum += (float)-0.084501356;
            } else {
              sum += (float)0.01421225;
            }
          } else {
            if (  (data[3].fvalue < 0.072783276)) {
              sum += (float)0.11046041;
            } else {
              sum += (float)0.014516558;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.720933)) {
        if (  (data[3].fvalue < 0.6922316)) {
          if (  (data[14].fvalue < 0.0052865981)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.045563232;
            } else {
              sum += (float)0.08836095;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.10894711;
            } else {
              sum += (float)0.22034363;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.489351)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.029480621;
            } else {
              sum += (float)0.16264687;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.013515839;
            } else {
              sum += (float)-0.10645556;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.65968996)) {
          if (  (data[13].fvalue < 1.0267243)) {
            if (  (data[16].fvalue < 0.66071641)) {
              sum += (float)-0.03327667;
            } else {
              sum += (float)0.0410292;
            }
          } else {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)-0.20308527;
            } else {
              sum += (float)-0.012866734;
            }
          }
        } else {
          if (  (data[15].fvalue < 0.042165387)) {
            if (  (data[17].fvalue < 0.98837119)) {
              sum += (float)0.030717252;
            } else {
              sum += (float)0.085644618;
            }
          } else {
            if (  (data[13].fvalue < 0.74812871)) {
              sum += (float)-0.024037668;
            } else {
              sum += (float)0.082424417;
            }
          }
        }
      }
    }
  }
  if (  (data[3].fvalue < 0.14928211)) {
    if (  (data[4].fvalue < 0.96114421)) {
      if (  (data[3].fvalue < 0.056110378)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[4].fvalue < 0.94306511)) {
            sum += (float)-0.15467447;
          } else {
            sum += (float)-0.42266542;
          }
        } else {
          sum += (float)0.068253204;
        }
      } else {
        if (  (data[14].fvalue < 0.046398889)) {
          if (  (data[5].fvalue < 1.2838631)) {
            if (  (data[3].fvalue < 0.089440294)) {
              sum += (float)-0.051301911;
            } else {
              sum += (float)0.11155918;
            }
          } else {
            if (  (data[14].fvalue < 0.016464943)) {
              sum += (float)-0.081790268;
            } else {
              sum += (float)-0.36355081;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.05267125)) {
              sum += (float)-0.2043591;
            } else {
              sum += (float)-0.35517412;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.14310293;
            } else {
              sum += (float)0.50073051;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 3.999999)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.065722369)) {
              sum += (float)0.031690277;
            } else {
              sum += (float)-0.025354259;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.30428511;
            } else {
              sum += (float)0.066653468;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.031601969)) {
            if (  (data[4].fvalue < 1.3182553)) {
              sum += (float)0.21594231;
            } else {
              sum += (float)0.012877039;
            }
          } else {
            if (  (data[14].fvalue < 0.065722369)) {
              sum += (float)0.0013910577;
            } else {
              sum += (float)0.044160329;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.3457735)) {
          if (  (data[14].fvalue < 0.19748637)) {
            if (  (data[16].fvalue < 0.0090708882)) {
              sum += (float)0.029026762;
            } else {
              sum += (float)0.28430587;
            }
          } else {
            if (  (data[4].fvalue < 1.261898)) {
              sum += (float)0.40520191;
            } else {
              sum += (float)0.18461393;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.42643282)) {
            if (  (data[17].fvalue < 0.12681212)) {
              sum += (float)-0.028821919;
            } else {
              sum += (float)0.11244885;
            }
          } else {
            if (  (data[4].fvalue < 1.6842455)) {
              sum += (float)0.2219864;
            } else {
              sum += (float)-0.00354241;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.1812317)) {
      if (  (data[12].fvalue < 0.99999899)) {
        if (  (data[8].fvalue < 6.999999)) {
          if (  (data[13].fvalue < 0.74929088)) {
            if (  (data[14].fvalue < 0.022216767)) {
              sum += (float)-0.024994319;
            } else {
              sum += (float)0.0055979276;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.18718804;
            } else {
              sum += (float)-0.0355727;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.4881369)) {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)0.083982907;
            } else {
              sum += (float)0.22690681;
            }
          } else {
            if (  (data[13].fvalue < 0.23255235)) {
              sum += (float)-0.20468728;
            } else {
              sum += (float)-0.009712927;
            }
          }
        }
      } else {
        sum += (float)0.3150537;
      }
    } else {
      if (  (data[14].fvalue < 0.44993261)) {
        if (  (data[5].fvalue < 0.37432513)) {
          if (  (data[16].fvalue < 0.20440406)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.0048585385;
            } else {
              sum += (float)-0.054582499;
            }
          } else {
            if (  (data[13].fvalue < 0.0064751329)) {
              sum += (float)-0.012011918;
            } else {
              sum += (float)0.020772936;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.25936615)) {
            if (  (data[16].fvalue < 0.79339564)) {
              sum += (float)-0.058294881;
            } else {
              sum += (float)-0.1450855;
            }
          } else {
            if (  (data[6].fvalue < 0.68739337)) {
              sum += (float)0.0078137359;
            } else {
              sum += (float)-0.11321174;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.5983507)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.0068103932;
            } else {
              sum += (float)0.075484194;
            }
          } else {
            if (  (data[13].fvalue < 0.18892911)) {
              sum += (float)-0.11877013;
            } else {
              sum += (float)0.018814908;
            }
          }
        } else {
          if (  (data[8].fvalue < 9.999999)) {
            if (  (data[14].fvalue < 0.64149857)) {
              sum += (float)0.055828121;
            } else {
              sum += (float)0.15787977;
            }
          } else {
            sum += (float)-0.070518047;
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 4.999999)) {
    if (  (data[13].fvalue < 0.23232204)) {
      if (  (data[8].fvalue < 10.999999)) {
        if (  (data[16].fvalue < 0.16123389)) {
          if (  (data[16].fvalue < 0.060299695)) {
            if (  (data[4].fvalue < 1.7611703)) {
              sum += (float)-0.0036468408;
            } else {
              sum += (float)0.042938329;
            }
          } else {
            if (  (data[13].fvalue < 0.0025723206)) {
              sum += (float)-0.04823257;
            } else {
              sum += (float)-0.0020351952;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.010921714)) {
            if (  (data[3].fvalue < 0.26851469)) {
              sum += (float)0.0046181679;
            } else {
              sum += (float)-0.081218734;
            }
          } else {
            if (  (data[13].fvalue < 0.0025723206)) {
              sum += (float)-0.00079663796;
            } else {
              sum += (float)0.047653161;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.47266853)) {
          sum += (float)0.22556289;
        } else {
          if (  (data[3].fvalue < 0.72557342)) {
            sum += (float)0.15313078;
          } else {
            sum += (float)0.007438208;
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.3943419)) {
        if (  (data[3].fvalue < 0.50947648)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.0033714615;
            } else {
              sum += (float)0.071124874;
            }
          } else {
            if (  (data[4].fvalue < 1.2825392)) {
              sum += (float)0.26065278;
            } else {
              sum += (float)0.12255269;
            }
          }
        } else {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.66157895)) {
              sum += (float)0.048629839;
            } else {
              sum += (float)-0.095494471;
            }
          } else {
            if (  (data[3].fvalue < 1.725935)) {
              sum += (float)-0.003637735;
            } else {
              sum += (float)-0.10286824;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.79391384)) {
          if (  (data[4].fvalue < 1.9315406)) {
            if (  (data[14].fvalue < 0.59642422)) {
              sum += (float)-0.0042555463;
            } else {
              sum += (float)0.082204007;
            }
          } else {
            if (  (data[17].fvalue < 0.986974)) {
              sum += (float)-0.083657734;
            } else {
              sum += (float)0.065865301;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6840601)) {
            if (  (data[14].fvalue < 0.010921714)) {
              sum += (float)-0.08586245;
            } else {
              sum += (float)0.072688535;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.10709877;
            } else {
              sum += (float)-0.0052769859;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.30539656;
  }
  if ( (data[4].fvalue < 1.2717193)) {
    if (  (data[12].fvalue < 0.99999899)) {
      if (  (data[8].fvalue < 6.999999)) {
        if (  (data[16].fvalue < 0.10975628)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.26275858;
            } else {
              sum += (float)0.012972959;
            }
          } else {
            if (  (data[16].fvalue < 0.046544518)) {
              sum += (float)0.003770927;
            } else {
              sum += (float)-0.029221287;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[3].fvalue < 0.34283695)) {
              sum += (float)0.05149281;
            } else {
              sum += (float)-0.007765105;
            }
          } else {
            if (  (data[3].fvalue < 0.84934485)) {
              sum += (float)0.28095764;
            } else {
              sum += (float)0.028694313;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.65983284)) {
          if (  (data[14].fvalue < 0.31615388)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.018281246;
            } else {
              sum += (float)0.18758987;
            }
          } else {
            if (  (data[3].fvalue < 0.29184276)) {
              sum += (float)0.33997482;
            } else {
              sum += (float)0.16545857;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.13801414)) {
            if (  (data[14].fvalue < 0.52761829)) {
              sum += (float)-0.13129318;
            } else {
              sum += (float)0.019707043;
            }
          } else {
            if (  (data[3].fvalue < 1.7274985)) {
              sum += (float)0.10577514;
            } else {
              sum += (float)-0.096771367;
            }
          }
        }
      }
    } else {
      sum += (float)0.31852788;
    }
  } else {
    if (  (data[16].fvalue < 0.43021095)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[14].fvalue < 0.11533421)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.37845829)) {
              sum += (float)-0.00044344334;
            } else {
              sum += (float)-0.062378675;
            }
          } else {
            if (  (data[14].fvalue < 0.034312967)) {
              sum += (float)-0.17728387;
            } else {
              sum += (float)-0.039239876;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0026032967)) {
            if (  (data[4].fvalue < 1.4736451)) {
              sum += (float)0.0073534977;
            } else {
              sum += (float)-0.030901792;
            }
          } else {
            if (  (data[13].fvalue < 0.059005678)) {
              sum += (float)0.10510583;
            } else {
              sum += (float)0.0052481215;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.49903134)) {
          if (  (data[17].fvalue < 0.17598745)) {
            if (  (data[13].fvalue < 0.12380555)) {
              sum += (float)-0.10074046;
            } else {
              sum += (float)-0.032330502;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.021160528;
            } else {
              sum += (float)0.10457701;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.077182531)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.036430232;
            } else {
              sum += (float)-0.099611312;
            }
          } else {
            if (  (data[16].fvalue < 0.0074052913)) {
              sum += (float)0.068788536;
            } else {
              sum += (float)-0.010705514;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 0.40382183)) {
        if (  (data[10].fvalue < 2.999999)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[13].fvalue < 0.37067243)) {
              sum += (float)0.0067135743;
            } else {
              sum += (float)0.046782505;
            }
          } else {
            if (  (data[4].fvalue < 1.8627071)) {
              sum += (float)0.17526983;
            } else {
              sum += (float)-0.0017576274;
            }
          }
        } else {
          if (  (data[9].fvalue < 0.99999899)) {
            sum += (float)-0.36894748;
          } else {
            if (  (data[4].fvalue < 2.3403065)) {
              sum += (float)0.048310544;
            } else {
              sum += (float)-0.11037154;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.79059362)) {
          if (  (data[17].fvalue < 0.48806301)) {
            if (  (data[1].fvalue < 0.77517724)) {
              sum += (float)-0.055749007;
            } else {
              sum += (float)0.0005522297;
            }
          } else {
            if (  (data[6].fvalue < 0.58444327)) {
              sum += (float)0.16713277;
            } else {
              sum += (float)0.014433429;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 1.5429189)) {
              sum += (float)-0.14536083;
            } else {
              sum += (float)-0.035382189;
            }
          } else {
            if (  (data[14].fvalue < 0.24663779)) {
              sum += (float)0.093928218;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  }
  if (  (data[11].fvalue < 2.999999)) {
    if (  (data[17].fvalue < 0.17498308)) {
      if (  (data[17].fvalue < 0.05864808)) {
        if (  (data[11].fvalue < 0.99999899)) {
          if (  (data[13].fvalue < 0.36965248)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.0028481167;
            } else {
              sum += (float)-0.0089698024;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)0.02891987;
            } else {
              sum += (float)-0.063733995;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.026077807)) {
            if (  (data[4].fvalue < 1.281707)) {
              sum += (float)0.1754989;
            } else {
              sum += (float)0.051279332;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.020990711;
            } else {
              sum += (float)-0.086670622;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.0023197348)) {
            if (  (data[4].fvalue < 2.0177176)) {
              sum += (float)-0.10509781;
            } else {
              sum += (float)0.035807375;
            }
          } else {
            if (  (data[14].fvalue < 0.00565641)) {
              sum += (float)-0.084558241;
            } else {
              sum += (float)0.028637726;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.95733333)) {
            if (  (data[4].fvalue < 1.3445042)) {
              sum += (float)0.037934255;
            } else {
              sum += (float)-0.027339473;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.023229472;
            } else {
              sum += (float)-0.11855146;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.6826757)) {
        if (  (data[13].fvalue < 0.014689368)) {
          if (  (data[14].fvalue < 0.022283839)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.34107;
            } else {
              sum += (float)-0.14016818;
            }
          } else {
            if (  (data[3].fvalue < 0.41670391)) {
              sum += (float)0.054593142;
            } else {
              sum += (float)-0.014239733;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.55267841)) {
            if (  (data[4].fvalue < 1.33031)) {
              sum += (float)0.20157519;
            } else {
              sum += (float)0.080473714;
            }
          } else {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.019755581;
            } else {
              sum += (float)0.11008078;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.3770276)) {
          if (  (data[15].fvalue < 0.14830862)) {
            if (  (data[16].fvalue < 1.3529999)) {
              sum += (float)-0.071117565;
            } else {
              sum += (float)0.049966615;
            }
          } else {
            if (  (data[16].fvalue < 0.084018856)) {
              sum += (float)-0.022820709;
            } else {
              sum += (float)0.045554586;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.090933912)) {
            if (  (data[4].fvalue < 1.8069979)) {
              sum += (float)0.032203414;
            } else {
              sum += (float)-0.027406225;
            }
          } else {
            if (  (data[16].fvalue < 0.48892647)) {
              sum += (float)0.016433533;
            } else {
              sum += (float)0.066041887;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.0061014174)) {
      if (  (data[4].fvalue < 2.0177176)) {
        if (  (data[17].fvalue < 0.65457654)) {
          sum += (float)-0.22821634;
        } else {
          if (  (data[14].fvalue < 0.24625881)) {
            sum += (float)-0.16787834;
          } else {
            sum += (float)-0.076329499;
          }
        }
      } else {
        if (  (data[3].fvalue < 0.65885496)) {
          sum += (float)-0.10344242;
        } else {
          if (  (data[5].fvalue < 0.1998823)) {
            sum += (float)0.015054366;
          } else {
            sum += (float)-0.018674167;
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 4.999999)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.984914)) {
              sum += (float)-0.18145536;
            } else {
              sum += (float)-0.018254535;
            }
          } else {
            sum += (float)0.045796078;
          }
        } else {
          if (  (data[5].fvalue < 0.30894023)) {
            if (  (data[5].fvalue < 0.10835885)) {
              sum += (float)0.0042401631;
            } else {
              sum += (float)0.10254139;
            }
          } else {
            sum += (float)-0.0170977;
          }
        }
      } else {
        if (  (data[4].fvalue < 2.1392603)) {
          sum += (float)-0.014283452;
        } else {
          sum += (float)-0.18703033;
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.26781857)) {
    if ( (data[4].fvalue < 1.1194699)) {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[3].fvalue < 0.047849033)) {
          if (  (data[4].fvalue < 0.96905887)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.32811031;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[14].fvalue < 0.034314249)) {
              sum += (float)0.040422685;
            } else {
              sum += (float)-0.072453827;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.28483164)) {
            if (  (data[4].fvalue < 1.0546399)) {
              sum += (float)-0.026174609;
            } else {
              sum += (float)0.16138336;
            }
          } else {
            if (  (data[13].fvalue < 0.0022748634)) {
              sum += (float)0.014378677;
            } else {
              sum += (float)-0.016900439;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.25677103)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 0.99865466)) {
              sum += (float)0.25635764;
            } else {
              sum += (float)-0.011108121;
            }
          } else {
            if (  (data[4].fvalue < 1.0062175)) {
              sum += (float)0.42083171;
            } else {
              sum += (float)0.25160798;
            }
          }
        } else {
          if (  (data[5].fvalue < 1.1762183)) {
            if (  (data[3].fvalue < 0.76177716)) {
              sum += (float)0.027408427;
            } else {
              sum += (float)-0.15064326;
            }
          } else {
            if (  (data[14].fvalue < 0.034314249)) {
              sum += (float)-0.0028411869;
            } else {
              sum += (float)0.24758957;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[3].fvalue < 0.12300009)) {
          if (  (data[14].fvalue < 0.028314298)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.015202657;
            } else {
              sum += (float)-0.0488129;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.028333684;
            } else {
              sum += (float)-0.058606852;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.15264109)) {
            if (  (data[4].fvalue < 1.8069482)) {
              sum += (float)-0.03007064;
            } else {
              sum += (float)0.021905208;
            }
          } else {
            if (  (data[14].fvalue < 0.0057003554)) {
              sum += (float)-0.0095017552;
            } else {
              sum += (float)0.021442886;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.18756761)) {
              sum += (float)-0.18602595;
            } else {
              sum += (float)-0.0020092917;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)-0.010731649;
            } else {
              sum += (float)0.13513964;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.28844455)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.0059847427;
            } else {
              sum += (float)0.1321189;
            }
          } else {
            if (  (data[14].fvalue < 0.17097673)) {
              sum += (float)-0.18027562;
            } else {
              sum += (float)-0.021352025;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.0841755)) {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[3].fvalue < 0.65892065)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 0.96905887)) {
              sum += (float)-0.19314535;
            } else {
              sum += (float)-0.062998392;
            }
          } else {
            if (  (data[3].fvalue < 0.41634694)) {
              sum += (float)0.34950837;
            } else {
              sum += (float)0.037898112;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.373714)) {
              sum += (float)0.074166767;
            } else {
              sum += (float)-0.0011940156;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.0024792536;
            } else {
              sum += (float)-0.10651585;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.89925915)) {
          if (  (data[3].fvalue < 0.55266672)) {
            if (  (data[4].fvalue < 1.068287)) {
              sum += (float)0.40402618;
            } else {
              sum += (float)0.19785756;
            }
          } else {
            if (  (data[4].fvalue < 0.96104842)) {
              sum += (float)0.31762174;
            } else {
              sum += (float)0.10504865;
            }
          }
        } else {
          if (  (data[3].fvalue < 1.48695)) {
            if (  (data[4].fvalue < 0.96905887)) {
              sum += (float)0.14018305;
            } else {
              sum += (float)-0.041768599;
            }
          } else {
            if (  (data[14].fvalue < 0.6398977)) {
              sum += (float)-0.13886422;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.4306935)) {
        if (  (data[3].fvalue < 0.41634694)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.0033049667;
            } else {
              sum += (float)0.10092181;
            }
          } else {
            if (  (data[13].fvalue < 0.0022748634)) {
              sum += (float)0.099766366;
            } else {
              sum += (float)0.21354948;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 0.059916131)) {
              sum += (float)0.047075819;
            } else {
              sum += (float)-0.0079750586;
            }
          } else {
            if (  (data[14].fvalue < 0.55895615)) {
              sum += (float)-0.05794134;
            } else {
              sum += (float)0.026931329;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.37706801)) {
          if (  (data[16].fvalue < 0.30843991)) {
            if (  (data[13].fvalue < 0.42467886)) {
              sum += (float)-0.030596668;
            } else {
              sum += (float)0.04684858;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.036375165;
            } else {
              sum += (float)-0.032444354;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.0081155468)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.082940556;
            } else {
              sum += (float)-0.0034196358;
            }
          } else {
            if (  (data[16].fvalue < 0.30843991)) {
              sum += (float)-0.028043916;
            } else {
              sum += (float)0.023814725;
            }
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 1.2510227)) {
    if (  (data[13].fvalue < 0.086992413)) {
      if (  (data[12].fvalue < 0.99999899)) {
        if (  (data[4].fvalue < 1.0545889)) {
          if (  (data[14].fvalue < 0.13052948)) {
            if (  (data[13].fvalue < 0.002129365)) {
              sum += (float)0.031702455;
            } else {
              sum += (float)-0.042428736;
            }
          } else {
            if (  (data[3].fvalue < 0.35619488)) {
              sum += (float)-0.07327725;
            } else {
              sum += (float)0.00094173045;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.079318263)) {
            if (  (data[16].fvalue < 0.14190929)) {
              sum += (float)-0.075866573;
            } else {
              sum += (float)0.044850573;
            }
          } else {
            if (  (data[3].fvalue < 0.26825818)) {
              sum += (float)0.046874456;
            } else {
              sum += (float)-0.00035271308;
            }
          }
        }
      } else {
        sum += (float)0.29659122;
      }
    } else {
      if (  (data[3].fvalue < 0.60243005)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.17619812;
            } else {
              sum += (float)0.0061124624;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.042418364;
            } else {
              sum += (float)0.15087739;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0545889)) {
            if (  (data[14].fvalue < 0.072506145)) {
              sum += (float)0.15346518;
            } else {
              sum += (float)0.35751194;
            }
          } else {
            if (  (data[14].fvalue < 0.1795446)) {
              sum += (float)-0.017256446;
            } else {
              sum += (float)0.17191565;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.0428499)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.088604495;
            } else {
              sum += (float)-0.072193272;
            }
          } else {
            if (  (data[3].fvalue < 1.2005519)) {
              sum += (float)0.054333832;
            } else {
              sum += (float)-0.0823704;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.32507089)) {
            if (  (data[14].fvalue < 0.37398469)) {
              sum += (float)-0.087516963;
            } else {
              sum += (float)0.00040617588;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.049376782;
            } else {
              sum += (float)-0.018265955;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.056408379)) {
      if (  (data[13].fvalue < 0.11023408)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.011019776)) {
            if (  (data[16].fvalue < 0.0080347005)) {
              sum += (float)0.18594044;
            } else {
              sum += (float)-0.028556928;
            }
          } else {
            if (  (data[16].fvalue < 0.18201956)) {
              sum += (float)0.0009560585;
            } else {
              sum += (float)0.076777101;
            }
          }
        } else {
          sum += (float)0.22616307;
        }
      } else {
        if (  (data[0].fvalue < 2.0753267)) {
          if (  (data[16].fvalue < 0.031176323)) {
            if (  (data[14].fvalue < 0.23603663)) {
              sum += (float)0.03541597;
            } else {
              sum += (float)0.14614631;
            }
          } else {
            if (  (data[2].fvalue < 0.099036522)) {
              sum += (float)0.21271308;
            } else {
              sum += (float)0.11487258;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6829072)) {
            if (  (data[13].fvalue < 0.32507089)) {
              sum += (float)0.038666517;
            } else {
              sum += (float)0.11734631;
            }
          } else {
            if (  (data[3].fvalue < 0.021824608)) {
              sum += (float)0.1016077;
            } else {
              sum += (float)-0.051448442;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.27851364)) {
        if (  (data[14].fvalue < 0.19739702)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 0.059990816)) {
              sum += (float)0.0032807828;
            } else {
              sum += (float)-0.038390476;
            }
          } else {
            if (  (data[17].fvalue < 0.48740995)) {
              sum += (float)-0.10334589;
            } else {
              sum += (float)0.13417499;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.28845155)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.0024909661;
            } else {
              sum += (float)-0.068887621;
            }
          } else {
            if (  (data[3].fvalue < 0.7248686)) {
              sum += (float)0.045618892;
            } else {
              sum += (float)-0.0045519387;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[15].fvalue < 0.03994314)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)0.0055858814;
            } else {
              sum += (float)-0.26496807;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)-0.061289605;
            } else {
              sum += (float)0.0071875807;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.5672907)) {
            if (  (data[3].fvalue < 0.60243005)) {
              sum += (float)0.20858409;
            } else {
              sum += (float)0.094396845;
            }
          } else {
            if (  (data[16].fvalue < 0.79186642)) {
              sum += (float)-0.016702546;
            } else {
              sum += (float)0.080017999;
            }
          }
        }
      }
    }
  }
  if (  (data[12].fvalue < 4.999999)) {
    if (  (data[11].fvalue < 2.999999)) {
      if (  (data[14].fvalue < 0.37452969)) {
        if ( (data[4].fvalue < 1.1673877)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[4].fvalue < 1.0428418)) {
              sum += (float)-0.0045531425;
            } else {
              sum += (float)0.018531235;
            }
          } else {
            if (  (data[3].fvalue < 0.37081265)) {
              sum += (float)0.11966812;
            } else {
              sum += (float)-0.034332752;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.26291242)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.0041296771;
            } else {
              sum += (float)-0.026998414;
            }
          } else {
            if (  (data[13].fvalue < 0.23099278)) {
              sum += (float)-0.040993907;
            } else {
              sum += (float)0.0051937862;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 2.668972)) {
          if (  (data[13].fvalue < 0.042984169)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.0119763;
            } else {
              sum += (float)-0.099646501;
            }
          } else {
            if (  (data[6].fvalue < 0.2925559)) {
              sum += (float)0.13967368;
            } else {
              sum += (float)0.027820336;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.38159552)) {
            if (  (data[13].fvalue < 0.050332926)) {
              sum += (float)0.00033481419;
            } else {
              sum += (float)0.02882923;
            }
          } else {
            if (  (data[16].fvalue < 1.3560255)) {
              sum += (float)-0.018579623;
            } else {
              sum += (float)0.086682275;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.0057653575)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[17].fvalue < 0.65586507)) {
            sum += (float)-0.20587131;
          } else {
            if (  (data[14].fvalue < 0.16278972)) {
              sum += (float)-0.19446038;
            } else {
              sum += (float)-0.033267599;
            }
          }
        } else {
          sum += (float)-0;
        }
      } else {
        if (  (data[7].fvalue < 4.999999)) {
          if (  (data[4].fvalue < 1.7185614)) {
            sum += (float)-0.079147957;
          } else {
            if (  (data[14].fvalue < 0.17122759)) {
              sum += (float)0.042811863;
            } else {
              sum += (float)-0.020700382;
            }
          }
        } else {
          sum += (float)-0.12500934;
        }
      }
    }
  } else {
    sum += (float)0.30066702;
  }
  if (  (data[7].fvalue < 10.999999)) {
    if (  (data[12].fvalue < 3.999999)) {
      if (  (data[16].fvalue < 0.18298508)) {
        if (  (data[16].fvalue < 0.047360908)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.0055076862)) {
              sum += (float)0.046463393;
            } else {
              sum += (float)-0.0032557752;
            }
          } else {
            if (  (data[5].fvalue < 0.59139872)) {
              sum += (float)0.048031807;
            } else {
              sum += (float)-0.050440867;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.001990211)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.054987576;
            } else {
              sum += (float)0.012869098;
            }
          } else {
            if (  (data[14].fvalue < 0.093266711)) {
              sum += (float)-0.027577776;
            } else {
              sum += (float)0.026756391;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.6835678)) {
          if (  (data[16].fvalue < 0.99192613)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.014461428;
            } else {
              sum += (float)0.094311357;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.12419748;
            } else {
              sum += (float)0.19124012;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.5640924)) {
            if (  (data[17].fvalue < 0.48783973)) {
              sum += (float)-0.04886324;
            } else {
              sum += (float)0.016803194;
            }
          } else {
            if (  (data[13].fvalue < 0.59725738)) {
              sum += (float)0.0021858118;
            } else {
              sum += (float)0.06388925;
            }
          }
        }
      }
    } else {
      sum += (float)0.28383756;
    }
  } else {
    if (  (data[4].fvalue < 1.65127)) {
      sum += (float)0.24027009;
    } else {
      sum += (float)0.1176309;
    }
  }
  if ( (data[4].fvalue < 0.83511597)) {
    if (  (data[5].fvalue < 1.4600264)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[14].fvalue < 0.32930556)) {
          if (  (data[3].fvalue < 0.84898579)) {
            if (  (data[14].fvalue < 0.27912202)) {
              sum += (float)0.10675202;
            } else {
              sum += (float)-0.25978479;
            }
          } else {
            if (  (data[13].fvalue < 0.0020429422)) {
              sum += (float)0.30252469;
            } else {
              sum += (float)0.052107397;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.95760417)) {
            sum += (float)-0.2384225;
          } else {
            if (  (data[14].fvalue < 0.37387466)) {
              sum += (float)0.18188909;
            } else {
              sum += (float)0.0074417731;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.17963108)) {
            if (  (data[13].fvalue < 0.12336869)) {
              sum += (float)-0.27411577;
            } else {
              sum += (float)-0.016197696;
            }
          } else {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.012844771;
            } else {
              sum += (float)0.07189706;
            }
          }
        } else {
          sum += (float)-0.2557779;
        }
      }
    } else {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[3].fvalue < 0.53073418)) {
          if (  (data[14].fvalue < 0.13039848)) {
            if (  (data[3].fvalue < 0.45112166)) {
              sum += (float)-0.04249911;
            } else {
              sum += (float)0.10072855;
            }
          } else {
            sum += (float)-0.30982226;
          }
        } else {
          if (  (data[14].fvalue < 0.17963108)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.16361561;
            } else {
              sum += (float)0.015517204;
            }
          } else {
            if (  (data[3].fvalue < 0.65986896)) {
              sum += (float)-0.23715232;
            } else {
              sum += (float)0.061464403;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.046527565)) {
          sum += (float)-0;
        } else {
          if (  (data[13].fvalue < 0.076677933)) {
            sum += (float)0.18721882;
          } else {
            sum += (float)0.37363571;
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 1.3249679)) {
      if (  (data[14].fvalue < 0.40869156)) {
        if (  (data[16].fvalue < 0.11021996)) {
          if (  (data[4].fvalue < 1.1366996)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.00021254749;
            } else {
              sum += (float)0.050718352;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.025749533;
            } else {
              sum += (float)0.0098291812;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.292611)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.023888867;
            } else {
              sum += (float)0.20080541;
            }
          } else {
            if (  (data[14].fvalue < 0.16259252)) {
              sum += (float)-0.010954334;
            } else {
              sum += (float)0.016669635;
            }
          }
        }
      } else {
        if (  (data[8].fvalue < 4.999999)) {
          if (  (data[4].fvalue < 1.0838221)) {
            if (  (data[3].fvalue < 0.72525126)) {
              sum += (float)-0.11251502;
            } else {
              sum += (float)-0.014496767;
            }
          } else {
            if (  (data[5].fvalue < 0.21024075)) {
              sum += (float)-0.0066155246;
            } else {
              sum += (float)0.020378349;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3588763)) {
            if (  (data[3].fvalue < 0.31658509)) {
              sum += (float)0.23975831;
            } else {
              sum += (float)0.062403392;
            }
          } else {
            if (  (data[14].fvalue < 0.69275784)) {
              sum += (float)-0.045395959;
            } else {
              sum += (float)0.065626942;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 0.0082629444)) {
            if (  (data[17].fvalue < 0.37606937)) {
              sum += (float)0.042078357;
            } else {
              sum += (float)-0.026288064;
            }
          } else {
            if (  (data[4].fvalue < 1.9283344)) {
              sum += (float)-0.025474323;
            } else {
              sum += (float)0.022185005;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.3380468)) {
            if (  (data[4].fvalue < 1.541118)) {
              sum += (float)-0.049614694;
            } else {
              sum += (float)-0.19943649;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if (  (data[14].fvalue < 0.84966791)) {
          if (  (data[16].fvalue < 0.43072131)) {
            if (  (data[4].fvalue < 1.7177672)) {
              sum += (float)-0.12359416;
            } else {
              sum += (float)-0.01786558;
            }
          } else {
            if (  (data[13].fvalue < 0.42366138)) {
              sum += (float)0.025152864;
            } else {
              sum += (float)-0.049678966;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.23142895)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.056610063;
            } else {
              sum += (float)-0.082062341;
            }
          } else {
            if (  (data[3].fvalue < 2.1674671)) {
              sum += (float)0.09511108;
            } else {
              sum += (float)0.010683067;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.12331327)) {
    if (  (data[8].fvalue < 3.999999)) {
      if (  (data[7].fvalue < 1.999999)) {
        if (  (data[6].fvalue < 0.030524276)) {
          if (  (data[14].fvalue < 0.040212687)) {
            if (  (data[0].fvalue < 1.7602466)) {
              sum += (float)-0.30143395;
            } else {
              sum += (float)0.022439588;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.13709402;
            } else {
              sum += (float)0.10005336;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.046778679)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.16783631;
            } else {
              sum += (float)0.0054733362;
            }
          } else {
            if (  (data[14].fvalue < 0.05914329)) {
              sum += (float)-0.032257665;
            } else {
              sum += (float)0.005178717;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.050368484)) {
          if (  (data[6].fvalue < 0.030524276)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.088001557;
            } else {
              sum += (float)0.34903675;
            }
          } else {
            if (  (data[3].fvalue < 0.1319319)) {
              sum += (float)-0.03775407;
            } else {
              sum += (float)-0.11554336;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[4].fvalue < 1.0427856)) {
              sum += (float)-0.073396273;
            } else {
              sum += (float)0.091389991;
            }
          } else {
            if (  (data[6].fvalue < 0.085334957)) {
              sum += (float)0.15276571;
            } else {
              sum += (float)-0.052605178;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.0680254)) {
        if (  (data[3].fvalue < 0.45095962)) {
          if (  (data[14].fvalue < 0.12263024)) {
            if (  (data[3].fvalue < 0.1319319)) {
              sum += (float)0.19414248;
            } else {
              sum += (float)-0.080819868;
            }
          } else {
            if (  (data[3].fvalue < 0.24627775)) {
              sum += (float)0.39775416;
            } else {
              sum += (float)0.22481576;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.17098282)) {
            if (  (data[5].fvalue < 1.2844872)) {
              sum += (float)-0.36071077;
            } else {
              sum += (float)0.026247421;
            }
          } else {
            if (  (data[3].fvalue < 0.7247377)) {
              sum += (float)0.10936035;
            } else {
              sum += (float)-0.037504673;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[17].fvalue < 0.088333994)) {
            if (  (data[14].fvalue < 0.19753318)) {
              sum += (float)-0.24373876;
            } else {
              sum += (float)-0.049770899;
            }
          } else {
            if (  (data[14].fvalue < 0.22609366)) {
              sum += (float)0.19984636;
            } else {
              sum += (float)-0.025950281;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3042959)) {
            if (  (data[3].fvalue < 0.55284029)) {
              sum += (float)0.19508632;
            } else {
              sum += (float)-0.050391827;
            }
          } else {
            if (  (data[16].fvalue < 0.34269536)) {
              sum += (float)-0.025968436;
            } else {
              sum += (float)0.05745583;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.3925868)) {
      if (  (data[3].fvalue < 0.32930589)) {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.1364955)) {
              sum += (float)0.074513905;
            } else {
              sum += (float)-0.00087668718;
            }
          } else {
            if (  (data[14].fvalue < 0.0054803579)) {
              sum += (float)0.020143304;
            } else {
              sum += (float)0.11032931;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.24643637)) {
            if (  (data[16].fvalue < 0.0079727396)) {
              sum += (float)0.051458213;
            } else {
              sum += (float)0.30590966;
            }
          } else {
            if (  (data[4].fvalue < 1.2926093)) {
              sum += (float)0.39186248;
            } else {
              sum += (float)0.18022326;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.42370671)) {
          if (  (data[16].fvalue < 0.096754313)) {
            if ( (data[4].fvalue < 1.0427856)) {
              sum += (float)0.019091461;
            } else {
              sum += (float)-0.035437178;
            }
          } else {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.019673526;
            } else {
              sum += (float)0.11142968;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.58361447)) {
            if (  (data[3].fvalue < 0.46928161)) {
              sum += (float)0.15243384;
            } else {
              sum += (float)0.043266937;
            }
          } else {
            if (  (data[6].fvalue < 0.73540038)) {
              sum += (float)-0.030133316;
            } else {
              sum += (float)-0.16528626;
            }
          }
        }
      }
    } else {
      if (  (data[17].fvalue < 0.48775607)) {
        if (  (data[15].fvalue < 0.14769833)) {
          if (  (data[16].fvalue < 0.30859971)) {
            if (  (data[14].fvalue < 0.52687281)) {
              sum += (float)-0.036472153;
            } else {
              sum += (float)0.020634092;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.018249962;
            } else {
              sum += (float)-0.048284158;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.0054803579)) {
            if (  (data[16].fvalue < 0.34269536)) {
              sum += (float)-0.18352768;
            } else {
              sum += (float)-0.051674273;
            }
          } else {
            if (  (data[4].fvalue < 2.1378686)) {
              sum += (float)0.090658411;
            } else {
              sum += (float)0.014672301;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8064473)) {
          if (  (data[2].fvalue < 0.71431082)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.076726057;
            } else {
              sum += (float)0.14298984;
            }
          } else {
            if (  (data[16].fvalue < 0.031055747)) {
              sum += (float)0.066704899;
            } else {
              sum += (float)-0.033574313;
            }
          }
        } else {
          if (  (data[11].fvalue < 1.999999)) {
            if (  (data[17].fvalue < 0.98449641)) {
              sum += (float)-0;
            } else {
              sum += (float)0.084553666;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.058711749;
            } else {
              sum += (float)-0.002034022;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.32520097)) {
    if ( (data[4].fvalue < 1.241213)) {
      if (  (data[8].fvalue < 5.999999)) {
        if (  (data[12].fvalue < 0.99999899)) {
          if (  (data[16].fvalue < 0.096774086)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.074864469;
            } else {
              sum += (float)0.00019307151;
            }
          } else {
            if (  (data[3].fvalue < 0.098218344)) {
              sum += (float)0.081695035;
            } else {
              sum += (float)0.0075194282;
            }
          }
        } else {
          sum += (float)0.28939691;
        }
      } else {
        if (  (data[3].fvalue < 0.29214579)) {
          if (  (data[14].fvalue < 0.2469687)) {
            if (  (data[13].fvalue < 0.13763124)) {
              sum += (float)0.032105777;
            } else {
              sum += (float)0.28639638;
            }
          } else {
            if (  (data[13].fvalue < 0.019065833)) {
              sum += (float)0.15231608;
            } else {
              sum += (float)0.39578697;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.32994434)) {
            if (  (data[16].fvalue < 0.0074646012)) {
              sum += (float)-0.10348978;
            } else {
              sum += (float)0.17132095;
            }
          } else {
            if (  (data[3].fvalue < 0.76351482)) {
              sum += (float)0.15961495;
            } else {
              sum += (float)0.0026279401;
            }
          }
        }
      }
    } else {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[5].fvalue < 0.32234573)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.0056278454)) {
              sum += (float)-0.0010130921;
            } else {
              sum += (float)0.028283048;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.021413207;
            } else {
              sum += (float)0.023008822;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.29166427)) {
            if (  (data[17].fvalue < 0.98380339)) {
              sum += (float)-0.048496399;
            } else {
              sum += (float)-0.14957811;
            }
          } else {
            if (  (data[14].fvalue < 0.76021606)) {
              sum += (float)-0.012372809;
            } else {
              sum += (float)0.017893232;
            }
          }
        }
      } else {
        if (  (data[12].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 0.20465244)) {
              sum += (float)-0.13635868;
            } else {
              sum += (float)-0.037373062;
            }
          } else {
            if (  (data[4].fvalue < 1.3443352)) {
              sum += (float)0.028843623;
            } else {
              sum += (float)-0.043587424;
            }
          }
        } else {
          sum += (float)0.35616189;
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.6206224)) {
      if (  (data[7].fvalue < 3.999999)) {
        if (  (data[8].fvalue < 5.999999)) {
          if (  (data[2].fvalue < 1.0899731)) {
            if (  (data[16].fvalue < 0.20465244)) {
              sum += (float)0.0096269231;
            } else {
              sum += (float)0.039807174;
            }
          } else {
            if (  (data[6].fvalue < 0.73565954)) {
              sum += (float)-0.0082672862;
            } else {
              sum += (float)-0.10261328;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.90087128)) {
            if (  (data[4].fvalue < 1.451304)) {
              sum += (float)0.22905928;
            } else {
              sum += (float)0.10581772;
            }
          } else {
            if (  (data[13].fvalue < 0.49749842)) {
              sum += (float)-0.038729019;
            } else {
              sum += (float)0.087417051;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.57788944)) {
          if (  (data[4].fvalue < 1.3443352)) {
            if (  (data[14].fvalue < 0.072914369)) {
              sum += (float)0.10288443;
            } else {
              sum += (float)0.3067733;
            }
          } else {
            if (  (data[13].fvalue < 0.49749842)) {
              sum += (float)0.048224814;
            } else {
              sum += (float)0.19072405;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.74820119)) {
            if (  (data[4].fvalue < 1.2710874)) {
              sum += (float)0.052312803;
            } else {
              sum += (float)-0.05398174;
            }
          } else {
            if (  (data[3].fvalue < 2.1641088)) {
              sum += (float)0.16948935;
            } else {
              sum += (float)-0.0027562752;
            }
          }
        }
      }
    } else {
      if (  (data[16].fvalue < 0.79063082)) {
        if (  (data[14].fvalue < 0.64017528)) {
          if (  (data[4].fvalue < 2.0166011)) {
            if (  (data[13].fvalue < 1.0249782)) {
              sum += (float)-0.028698428;
            } else {
              sum += (float)0.035333235;
            }
          } else {
            if (  (data[17].fvalue < 0.98380339)) {
              sum += (float)-0.10037387;
            } else {
              sum += (float)0.062079467;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.74820119)) {
            if (  (data[7].fvalue < 7.999999)) {
              sum += (float)0.026339598;
            } else {
              sum += (float)-0.12855001;
            }
          } else {
            if (  (data[8].fvalue < 8.999999)) {
              sum += (float)0.15326318;
            } else {
              sum += (float)0.0049481075;
            }
          }
        }
      } else {
        if (  (data[15].fvalue < 0.038932215)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[13].fvalue < 0.59701687)) {
              sum += (float)0.035858594;
            } else {
              sum += (float)0.12269991;
            }
          } else {
            if (  (data[14].fvalue < 0.18018387)) {
              sum += (float)-0.099758215;
            } else {
              sum += (float)0.025546433;
            }
          }
        } else {
          if (  (data[1].fvalue < 0.93548983)) {
            if (  (data[3].fvalue < 0.90087128)) {
              sum += (float)-0.030440606;
            } else {
              sum += (float)0.048867885;
            }
          } else {
            if (  (data[6].fvalue < 0.58391696)) {
              sum += (float)-0.032131687;
            } else {
              sum += (float)-0.10711349;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.23630674)) {
    if (  (data[3].fvalue < 0.69075394)) {
      if (  (data[4].fvalue < 1.1911592)) {
        if (  (data[16].fvalue < 0.084418692)) {
          if (  (data[3].fvalue < 0.03087149)) {
            if (  (data[4].fvalue < 0.9765662)) {
              sum += (float)-0.26624006;
            } else {
              sum += (float)-0.010812968;
            }
          } else {
            if (  (data[4].fvalue < 1.1188092)) {
              sum += (float)0.0073847021;
            } else {
              sum += (float)-0.022074943;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.0096056284)) {
              sum += (float)-0.0037176909;
            } else {
              sum += (float)0.067225248;
            }
          } else {
            if (  (data[3].fvalue < 0.24634357)) {
              sum += (float)0.25880995;
            } else {
              sum += (float)0.06213928;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.16142546)) {
          if (  (data[4].fvalue < 1.4716276)) {
            if (  (data[13].fvalue < 0.28807247)) {
              sum += (float)-0.042959541;
            } else {
              sum += (float)0.027460678;
            }
          } else {
            if (  (data[16].fvalue < 0.0077427593)) {
              sum += (float)0.04062818;
            } else {
              sum += (float)-0.023542382;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7176782)) {
            if (  (data[7].fvalue < 3.999999)) {
              sum += (float)0.01142077;
            } else {
              sum += (float)0.12664175;
            }
          } else {
            if (  (data[16].fvalue < 0.65837747)) {
              sum += (float)-0.041103341;
            } else {
              sum += (float)-0.0052575478;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[13].fvalue < 0.49568015)) {
          if (  (data[4].fvalue < 1.7582372)) {
            if (  (data[5].fvalue < 1.2850929)) {
              sum += (float)-0.046975285;
            } else {
              sum += (float)0.069194905;
            }
          } else {
            if (  (data[16].fvalue < 0.78920799)) {
              sum += (float)0.066842102;
            } else {
              sum += (float)-0.023380367;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.0322807)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.15747887;
            } else {
              sum += (float)0.035532873;
            }
          } else {
            if (  (data[16].fvalue < 1.3524745)) {
              sum += (float)-0.001711647;
            } else {
              sum += (float)0.093351781;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.5616473)) {
          if (  (data[17].fvalue < 0.1743031)) {
            if (  (data[13].fvalue < 0.076427288)) {
              sum += (float)-0.22635858;
            } else {
              sum += (float)-0.10369065;
            }
          } else {
            if (  (data[13].fvalue < 0.32415253)) {
              sum += (float)0.054222289;
            } else {
              sum += (float)-0.097011782;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.28807247)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.020702716;
            } else {
              sum += (float)0.21750899;
            }
          } else {
            if (  (data[7].fvalue < 4.999999)) {
              sum += (float)-0.12407082;
            } else {
              sum += (float)0.0054433998;
            }
          }
        }
      }
    }
  } else {
    if (  (data[3].fvalue < 0.081422217)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.0017015757)) {
              sum += (float)-0.16852693;
            } else {
              sum += (float)-0.0083470447;
            }
          } else {
            if (  (data[4].fvalue < 1.3923779)) {
              sum += (float)0.23093615;
            } else {
              sum += (float)0.067922756;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3750831)) {
            if (  (data[16].fvalue < 0.059905294)) {
              sum += (float)0.31100458;
            } else {
              sum += (float)0.17156677;
            }
          } else {
            if (  (data[14].fvalue < 0.3436625)) {
              sum += (float)-0.0079468833;
            } else {
              sum += (float)0.081812993;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.3165036)) {
          if (  (data[13].fvalue < 0.066963173)) {
            if (  (data[14].fvalue < 0.3584716)) {
              sum += (float)0.06856031;
            } else {
              sum += (float)0.20788048;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.2340434;
            } else {
              sum += (float)0.42124286;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.44985935)) {
            if (  (data[13].fvalue < 0.076427288)) {
              sum += (float)-0.12309946;
            } else {
              sum += (float)0.062751971;
            }
          } else {
            if (  (data[4].fvalue < 1.4106427)) {
              sum += (float)0.17736825;
            } else {
              sum += (float)0.044136662;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.0678369)) {
        if (  (data[3].fvalue < 0.5530647)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 0.9765662)) {
              sum += (float)-0.21682028;
            } else {
              sum += (float)-0.069813386;
            }
          } else {
            if (  (data[3].fvalue < 0.41702572)) {
              sum += (float)0.32416463;
            } else {
              sum += (float)0.14457513;
            }
          }
        } else {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[14].fvalue < 0.3436625)) {
              sum += (float)0.028525313;
            } else {
              sum += (float)-0.010233412;
            }
          } else {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.058947157;
            } else {
              sum += (float)-0.19384666;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.2119094)) {
          if (  (data[3].fvalue < 0.37078953)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)0.044882499;
            } else {
              sum += (float)0.30067095;
            }
          } else {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.028088572;
            } else {
              sum += (float)-0.018535241;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.29630989)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.017280243;
            } else {
              sum += (float)0.011842606;
            }
          } else {
            if (  (data[16].fvalue < 0.0077427593)) {
              sum += (float)0.041505925;
            } else {
              sum += (float)-0.0063015684;
            }
          }
        }
      }
    }
  }
  if (  (data[11].fvalue < 1.999999)) {
    if (  (data[17].fvalue < 0.12447632)) {
      if (  (data[17].fvalue < 0.05746229)) {
        if (  (data[14].fvalue < 0.59777498)) {
          if (  (data[8].fvalue < 3.999999)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.0002806235;
            } else {
              sum += (float)0.043253049;
            }
          } else {
            if ( (data[4].fvalue < 1.1007569)) {
              sum += (float)0.046226434;
            } else {
              sum += (float)-0.033635765;
            }
          }
        } else {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[16].fvalue < 0.34231141)) {
              sum += (float)-0.011967628;
            } else {
              sum += (float)0.051973838;
            }
          } else {
            if (  (data[16].fvalue < 0.030613869)) {
              sum += (float)0.058175486;
            } else {
              sum += (float)-0.02638053;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.0096690441)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.69470155)) {
              sum += (float)-0.071891017;
            } else {
              sum += (float)-0.14630756;
            }
          } else {
            if (  (data[14].fvalue < 0.072953768)) {
              sum += (float)-0.1728165;
            } else {
              sum += (float)-0.003052454;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3167312)) {
            if (  (data[3].fvalue < 0.55358243)) {
              sum += (float)0.098994054;
            } else {
              sum += (float)-0.059813403;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.018194698;
            } else {
              sum += (float)-0.071064077;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.6496567)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.0017529198)) {
            if (  (data[14].fvalue < 0.011163017)) {
              sum += (float)-0.25624391;
            } else {
              sum += (float)-0.0085163433;
            }
          } else {
            if (  (data[14].fvalue < 0.0057245861)) {
              sum += (float)-0;
            } else {
              sum += (float)0.072324917;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 1.3043046)) {
              sum += (float)0.097424902;
            } else {
              sum += (float)0.02986026;
            }
          } else {
            if (  (data[6].fvalue < 0.31222239)) {
              sum += (float)0.22025864;
            } else {
              sum += (float)0.076210439;
            }
          }
        }
      } else {
        if (  (data[17].fvalue < 0.48508567)) {
          if (  (data[13].fvalue < 1.0219834)) {
            if (  (data[14].fvalue < 0.39234436)) {
              sum += (float)0.0034349018;
            } else {
              sum += (float)-0.041762993;
            }
          } else {
            if ( (data[4].fvalue < 1.8610408)) {
              sum += (float)0.042158846;
            } else {
              sum += (float)-0.13325907;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.26895058)) {
            if (  (data[13].fvalue < 0.0055347057)) {
              sum += (float)-0.091088921;
            } else {
              sum += (float)0.024381081;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)0.07516627;
            } else {
              sum += (float)-0.0013997882;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.2313352)) {
      sum += (float)-0.23998237;
    } else {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[17].fvalue < 0.37609512)) {
          if (  (data[13].fvalue < 0.0055347057)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.18969499;
            } else {
              sum += (float)-0.026357559;
            }
          } else {
            if (  (data[16].fvalue < 0.20415412)) {
              sum += (float)-0.037532676;
            } else {
              sum += (float)-0.1210594;
            }
          }
        } else {
          if (  (data[11].fvalue < 3.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.034310237;
            } else {
              sum += (float)0.065002531;
            }
          } else {
            sum += (float)-0.23818108;
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[13].fvalue < 0.20837773)) {
            if (  (data[14].fvalue < 0.85214251)) {
              sum += (float)0.0093125654;
            } else {
              sum += (float)-0.039683785;
            }
          } else {
            if (  (data[4].fvalue < 2.1358635)) {
              sum += (float)0.069732957;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[17].fvalue < 0.23100126)) {
            if (  (data[3].fvalue < 0.60323346)) {
              sum += (float)-0.071898617;
            } else {
              sum += (float)-0.17285483;
            }
          } else {
            if (  (data[10].fvalue < 1.999999)) {
              sum += (float)-0.0022625474;
            } else {
              sum += (float)-0.13043301;
            }
          }
        }
      }
    }
  }
  if (  (data[5].fvalue < 0.39077196)) {
    if (  (data[4].fvalue < 0.96902126)) {
      if (  (data[14].fvalue < 0.022569036)) {
        sum += (float)-0.36260062;
      } else {
        sum += (float)-0.12346859;
      }
    } else {
      if (  (data[14].fvalue < 0.1154318)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 2.999999)) {
            if (  (data[12].fvalue < 1.999999)) {
              sum += (float)-0.0080757253;
            } else {
              sum += (float)-0.15635106;
            }
          } else {
            if (  (data[14].fvalue < 0.066147625)) {
              sum += (float)-0.11984187;
            } else {
              sum += (float)-0.022683194;
            }
          }
        } else {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[13].fvalue < 0.04276133)) {
              sum += (float)-0.067158893;
            } else {
              sum += (float)0.11734029;
            }
          } else {
            if (  (data[4].fvalue < 1.2213932)) {
              sum += (float)0.052760746;
            } else {
              sum += (float)-0.036251999;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.0425268)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 0.10655581)) {
              sum += (float)-0.37837562;
            } else {
              sum += (float)-0.11243289;
            }
          } else {
            if (  (data[14].fvalue < 0.13066337)) {
              sum += (float)0.019281885;
            } else {
              sum += (float)0.23491633;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1357989)) {
            if (  (data[13].fvalue < 0.0017754961)) {
              sum += (float)0.060984444;
            } else {
              sum += (float)0.20517842;
            }
          } else {
            if (  (data[0].fvalue < 1.8081009)) {
              sum += (float)0.030748999;
            } else {
              sum += (float)0.0042159394;
            }
          }
        }
      }
    }
  } else {
    if (  (data[13].fvalue < 0.0017754961)) {
      if (  (data[14].fvalue < 0.066147625)) {
        if (  (data[8].fvalue < 2.999999)) {
          if (  (data[3].fvalue < 0.065032706)) {
            if (  (data[14].fvalue < 0.022569036)) {
              sum += (float)0.090476096;
            } else {
              sum += (float)-0.19596486;
            }
          } else {
            if (  (data[4].fvalue < 0.95228398)) {
              sum += (float)0.050464727;
            } else {
              sum += (float)0.15253399;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.03457395)) {
            sum += (float)-0.51812339;
          } else {
            if (  (data[6].fvalue < 0.03073846)) {
              sum += (float)0.20871298;
            } else {
              sum += (float)-0.27218595;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.1956163)) {
          if (  (data[4].fvalue < 0.98393732)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)-0.21840246;
            } else {
              sum += (float)0.39229438;
            }
          } else {
            if (  (data[14].fvalue < 0.1154318)) {
              sum += (float)0.090440035;
            } else {
              sum += (float)-0.097988494;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.12295187)) {
            if (  (data[8].fvalue < 2.999999)) {
              sum += (float)0.071931273;
            } else {
              sum += (float)-0.20674071;
            }
          } else {
            if (  (data[3].fvalue < 0.41662934)) {
              sum += (float)-0.027756335;
            } else {
              sum += (float)0.0034244054;
            }
          }
        }
      }
    } else {
      if (  (data[13].fvalue < 0.0096495664)) {
        if (  (data[8].fvalue < 1.999999)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.1956163)) {
              sum += (float)-0.11534785;
            } else {
              sum += (float)-0.0163812;
            }
          } else {
            if (  (data[14].fvalue < 0.059558913)) {
              sum += (float)-0.19610231;
            } else {
              sum += (float)0.1846056;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.35630542)) {
            if (  (data[14].fvalue < 0.1385781)) {
              sum += (float)-0.13447452;
            } else {
              sum += (float)0.12824522;
            }
          } else {
            if (  (data[5].fvalue < 1.462296)) {
              sum += (float)-0.18153396;
            } else {
              sum += (float)-0.006478257;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.072870582)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.0011456957;
            } else {
              sum += (float)-0.060947262;
            }
          } else {
            if (  (data[3].fvalue < 0.12360682)) {
              sum += (float)0.16943477;
            } else {
              sum += (float)-0.10512369;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.38526097)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.049033277;
            } else {
              sum += (float)0.098763339;
            }
          } else {
            if (  (data[16].fvalue < 0.1106194)) {
              sum += (float)-0.018433711;
            } else {
              sum += (float)0.030167403;
            }
          }
        }
      }
    }
  }
  if (  (data[13].fvalue < 0.15325092)) {
    if (  (data[0].fvalue < 4.3298903)) {
      if (  (data[5].fvalue < 0.046675134)) {
        if (  (data[4].fvalue < 1.7582088)) {
          if (  (data[12].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.0064787)) {
              sum += (float)-0.089094624;
            } else {
              sum += (float)0.019782737;
            }
          } else {
            if (  (data[3].fvalue < 0.048367225)) {
              sum += (float)0.25635996;
            } else {
              sum += (float)0.12649751;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.59682953)) {
            if (  (data[16].fvalue < 0.047439601)) {
              sum += (float)0.044005949;
            } else {
              sum += (float)-0.029956724;
            }
          } else {
            if (  (data[4].fvalue < 1.9281533)) {
              sum += (float)-0.0019884149;
            } else {
              sum += (float)-0.11411931;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.2124214)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.24872373;
            } else {
              sum += (float)-0.013517704;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)-0.0012311253;
            } else {
              sum += (float)0.26515388;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.12323459)) {
            if (  (data[4].fvalue < 1.5663124)) {
              sum += (float)-0.042387906;
            } else {
              sum += (float)0.017053593;
            }
          } else {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.0014177709;
            } else {
              sum += (float)-0.063198909;
            }
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[16].fvalue < 0.060536921)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[6].fvalue < 0.21036468)) {
              sum += (float)-0.046716493;
            } else {
              sum += (float)0.31086081;
            }
          } else {
            if (  (data[12].fvalue < 0.99999899)) {
              sum += (float)0.034969322;
            } else {
              sum += (float)-0.0983859;
            }
          }
        } else {
          if (  (data[1].fvalue < 0.64652306)) {
            if (  (data[5].fvalue < 0.1259785)) {
              sum += (float)0.084364489;
            } else {
              sum += (float)0.18605316;
            }
          } else {
            if (  (data[14].fvalue < 1.2435758)) {
              sum += (float)0.0061813002;
            } else {
              sum += (float)-0.055908315;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.086584754)) {
            sum += (float)-0.19088553;
          } else {
            if (  (data[3].fvalue < 0.18616168)) {
              sum += (float)0.070392646;
            } else {
              sum += (float)0.011359133;
            }
          }
        } else {
          if (  (data[5].fvalue < 0.37563598)) {
            if (  (data[14].fvalue < 0.07284423)) {
              sum += (float)0.11814177;
            } else {
              sum += (float)-0.039151326;
            }
          } else {
            if (  (data[6].fvalue < 0.030861743)) {
              sum += (float)0.088405132;
            } else {
              sum += (float)-0.10575354;
            }
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.4506569)) {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.40218657;
            } else {
              sum += (float)0.12741497;
            }
          } else {
            if (  (data[13].fvalue < 0.59527367)) {
              sum += (float)-0.011459318;
            } else {
              sum += (float)0.10252925;
            }
          }
        } else {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[2].fvalue < 1.0892992)) {
              sum += (float)0.03691265;
            } else {
              sum += (float)-0.070913777;
            }
          } else {
            if (  (data[3].fvalue < 0.8034969)) {
              sum += (float)-0.0091026975;
            } else {
              sum += (float)-0.14783898;
            }
          }
        }
      } else {
        if (  (data[3].fvalue < 0.95795894)) {
          if (  (data[14].fvalue < 0.15518738)) {
            if (  (data[4].fvalue < 1.2023324)) {
              sum += (float)0.086196437;
            } else {
              sum += (float)-0.04387838;
            }
          } else {
            if (  (data[4].fvalue < 1.3045132)) {
              sum += (float)0.15762112;
            } else {
              sum += (float)0.058355313;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.20789208)) {
            if (  (data[16].fvalue < 0.27907422)) {
              sum += (float)-0.14371778;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[3].fvalue < 1.7249995)) {
              sum += (float)0.023565013;
            } else {
              sum += (float)-0.062086169;
            }
          }
        }
      }
    } else {
      if (  (data[15].fvalue < 0.38107887)) {
        if (  (data[17].fvalue < 0.65200478)) {
          if (  (data[13].fvalue < 0.49652216)) {
            if (  (data[7].fvalue < 2.999999)) {
              sum += (float)-0.0050181556;
            } else {
              sum += (float)-0.049020149;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.020205142;
            } else {
              sum += (float)0.03038319;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.64590049)) {
            if (  (data[4].fvalue < 2.1365244)) {
              sum += (float)0.079174042;
            } else {
              sum += (float)0.00041813924;
            }
          } else {
            if (  (data[1].fvalue < 0.56946814)) {
              sum += (float)0.045673493;
            } else {
              sum += (float)-0.020302672;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.79049414)) {
              sum += (float)-0.10617441;
            } else {
              sum += (float)-0.011709305;
            }
          } else {
            if (  (data[13].fvalue < 0.59527367)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12873849;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.3381381)) {
            if (  (data[3].fvalue < 1.485456)) {
              sum += (float)0.13995007;
            } else {
              sum += (float)0.048360009;
            }
          } else {
            if (  (data[5].fvalue < 0.1259785)) {
              sum += (float)-0.029980181;
            } else {
              sum += (float)0.087874711;
            }
          }
        }
      }
    }
  }
  if (  (data[3].fvalue < 0.20610467)) {
    if (  (data[8].fvalue < 2.999999)) {
      if (  (data[4].fvalue < 0.95213038)) {
        if (  (data[14].fvalue < 0.053445537)) {
          if (  (data[3].fvalue < 0.081605077)) {
            if (  (data[14].fvalue < 0.011433387)) {
              sum += (float)-0.16398078;
            } else {
              sum += (float)-0.2979981;
            }
          } else {
            if (  (data[5].fvalue < 1.2852237)) {
              sum += (float)0.042831935;
            } else {
              sum += (float)-0.097160503;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[5].fvalue < 1.0339063)) {
              sum += (float)-0.1795439;
            } else {
              sum += (float)-0.38080785;
            }
          } else {
            if (  (data[3].fvalue < 0.16801102)) {
              sum += (float)0.25533941;
            } else {
              sum += (float)-0.079304084;
            }
          }
        }
      } else {
        if (  (data[10].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.080072917)) {
            if (  (data[13].fvalue < 0.0017183505)) {
              sum += (float)0.064178348;
            } else {
              sum += (float)-0.035884347;
            }
          } else {
            if (  (data[4].fvalue < 1.0143685)) {
              sum += (float)-0.13039312;
            } else {
              sum += (float)-0.0084512578;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.017036377)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.0091298455;
            } else {
              sum += (float)-0.060134795;
            }
          } else {
            if (  (data[17].fvalue < 0.024003647)) {
              sum += (float)0.037956607;
            } else {
              sum += (float)-0.04361897;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 0.97681296)) {
        if (  (data[14].fvalue < 0.034926388)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.31114584;
            } else {
              sum += (float)0.019938555;
            }
          } else {
            sum += (float)0.38141099;
          }
        } else {
          if (  (data[3].fvalue < 0.13232449)) {
            sum += (float)0.44804257;
          } else {
            if (  (data[5].fvalue < 0.9808327)) {
              sum += (float)0.17823651;
            } else {
              sum += (float)0.41183069;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.2816812)) {
          if (  (data[14].fvalue < 0.080072917)) {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)-0.1276795;
            } else {
              sum += (float)0.071815401;
            }
          } else {
            if (  (data[4].fvalue < 1.101516)) {
              sum += (float)0.20530893;
            } else {
              sum += (float)0.045338687;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.39251348)) {
            if (  (data[13].fvalue < 0.20828015)) {
              sum += (float)-0.053128645;
            } else {
              sum += (float)0.039830506;
            }
          } else {
            if (  (data[4].fvalue < 1.3754028)) {
              sum += (float)0.10276076;
            } else {
              sum += (float)0.012083461;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 2.999999)) {
      if (  (data[7].fvalue < 2.999999)) {
        if (  (data[16].fvalue < 0.060622822)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[14].fvalue < 0.13912824)) {
              sum += (float)0.029465377;
            } else {
              sum += (float)0.0034234705;
            }
          } else {
            if (  (data[13].fvalue < 0.036248453)) {
              sum += (float)-0.14379053;
            } else {
              sum += (float)-0.018235361;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0055082776)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.031438347;
            } else {
              sum += (float)0.021841384;
            }
          } else {
            if (  (data[14].fvalue < 0.005987837)) {
              sum += (float)-0.013956312;
            } else {
              sum += (float)0.025745723;
            }
          }
        }
      } else {
        if (  (data[7].fvalue < 3.999999)) {
          if (  (data[8].fvalue < 0.99999899)) {
            if (  (data[16].fvalue < 0.0096367933)) {
              sum += (float)0.11851918;
            } else {
              sum += (float)0.018601451;
            }
          } else {
            if (  (data[14].fvalue < 0.022873145)) {
              sum += (float)-0.17901658;
            } else {
              sum += (float)-0.078782812;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.59339565)) {
            if (  (data[14].fvalue < 0.034926388)) {
              sum += (float)-0.10188846;
            } else {
              sum += (float)0.0047950866;
            }
          } else {
            if (  (data[16].fvalue < 0.047517084)) {
              sum += (float)0.17839402;
            } else {
              sum += (float)0.02668702;
            }
          }
        }
      }
    } else {
      if (  (data[5].fvalue < 1.2852237)) {
        if (  (data[14].fvalue < 0.10115403)) {
          if (  (data[16].fvalue < 0.20527007)) {
            if (  (data[17].fvalue < 0.17375536)) {
              sum += (float)-0.16560763;
            } else {
              sum += (float)0.057941247;
            }
          } else {
            if (  (data[13].fvalue < 0.23144822)) {
              sum += (float)0.12103253;
            } else {
              sum += (float)-0.13833597;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.08687301)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)0.0070932661;
            } else {
              sum += (float)-0.067209944;
            }
          } else {
            if (  (data[4].fvalue < 1.0686984)) {
              sum += (float)0.073995344;
            } else {
              sum += (float)0.0028746231;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.0055082776)) {
          if (  (data[14].fvalue < 0.059944559)) {
            sum += (float)-0.18554178;
          } else {
            if (  (data[3].fvalue < 0.40178853)) {
              sum += (float)0.28418136;
            } else {
              sum += (float)-0.021478755;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.53109759)) {
            if (  (data[14].fvalue < 0.047162846)) {
              sum += (float)0.12766306;
            } else {
              sum += (float)0.42276117;
            }
          } else {
            if (  (data[14].fvalue < 0.094003282)) {
              sum += (float)-0.075782835;
            } else {
              sum += (float)0.18234953;
            }
          }
        }
      }
    }
  }
  if (  (data[3].fvalue < 1.7235035)) {
    if (  (data[13].fvalue < 0.28824082)) {
      if (  (data[8].fvalue < 1.999999)) {
        if (  (data[7].fvalue < 0.99999899)) {
          if (  (data[14].fvalue < 0.053364664)) {
            if (  (data[16].fvalue < 0.0095921308)) {
              sum += (float)0.076255731;
            } else {
              sum += (float)-0.010867234;
            }
          } else {
            if (  (data[6].fvalue < 0.043586139)) {
              sum += (float)-0.095019996;
            } else {
              sum += (float)-0.0059822304;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.001627479)) {
            if (  (data[4].fvalue < 1.1200794)) {
              sum += (float)0.24739186;
            } else {
              sum += (float)-0.18433261;
            }
          } else {
            if (  (data[14].fvalue < 0.13938332)) {
              sum += (float)-0.0067374301;
            } else {
              sum += (float)0.050840929;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.034954336)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.10492764;
            } else {
              sum += (float)0.049831826;
            }
          } else {
            if (  (data[13].fvalue < 0.036296543)) {
              sum += (float)0.22791089;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.018586194;
            } else {
              sum += (float)0.032994162;
            }
          } else {
            if (  (data[4].fvalue < 1.2814628)) {
              sum += (float)0.061919723;
            } else {
              sum += (float)0.0047267578;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.5409497)) {
        if (  (data[7].fvalue < 3.999999)) {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.16203706)) {
              sum += (float)-0.018338444;
            } else {
              sum += (float)0.039377753;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.086378828;
            } else {
              sum += (float)0.0098840604;
            }
          }
        } else {
          if (  (data[3].fvalue < 0.23629285)) {
            if (  (data[14].fvalue < 0.10853819)) {
              sum += (float)0.089433745;
            } else {
              sum += (float)0.22081457;
            }
          } else {
            if (  (data[14].fvalue < 0.079934701)) {
              sum += (float)-0.05532527;
            } else {
              sum += (float)0.07388518;
            }
          }
        }
      } else {
        if (  (data[16].fvalue < 0.98799652)) {
          if (  (data[4].fvalue < 1.8596654)) {
            if (  (data[13].fvalue < 0.74478662)) {
              sum += (float)-0.0054374188;
            } else {
              sum += (float)0.032288112;
            }
          } else {
            if (  (data[15].fvalue < 0.37983984)) {
              sum += (float)-0.034109253;
            } else {
              sum += (float)0.06318938;
            }
          }
        } else {
          if (  (data[10].fvalue < 1.999999)) {
            if (  (data[13].fvalue < 0.59377861)) {
              sum += (float)0.047347926;
            } else {
              sum += (float)0.18613875;
            }
          } else {
            if (  (data[5].fvalue < 0.10829045)) {
              sum += (float)-0.087448858;
            } else {
              sum += (float)0.053039424;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 2.999999)) {
      if ( (data[4].fvalue < 1.0330448)) {
        if (  (data[14].fvalue < 0.84975398)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[16].fvalue < 0.0095921308)) {
              sum += (float)0.14148438;
            } else {
              sum += (float)0.076339439;
            }
          } else {
            if (  (data[14].fvalue < 0.13938332)) {
              sum += (float)0.06597399;
            } else {
              sum += (float)-0.12828107;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 2.1602926)) {
              sum += (float)-0.066478387;
            } else {
              sum += (float)0.003585173;
            }
          } else {
            sum += (float)0.10314987;
          }
        }
      } else {
        if (  (data[5].fvalue < 0.40482816)) {
          if (  (data[1].fvalue < 0.56923896)) {
            sum += (float)0.19815081;
          } else {
            if (  (data[13].fvalue < 0.036296543)) {
              sum += (float)0.020916844;
            } else {
              sum += (float)-0.035113215;
            }
          }
        } else {
          if (  (data[8].fvalue < 1.999999)) {
            if (  (data[6].fvalue < 0.7351827)) {
              sum += (float)-0.037272833;
            } else {
              sum += (float)-0.099004515;
            }
          } else {
            if (  (data[7].fvalue < 1.999999)) {
              sum += (float)0.049422894;
            } else {
              sum += (float)-0.10850802;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.98498499)) {
        if (  (data[5].fvalue < 0.74811178)) {
          if (  (data[16].fvalue < 1.3544685)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)-0.0085452637;
            } else {
              sum += (float)-0.080607124;
            }
          } else {
            sum += (float)0.062892891;
          }
        } else {
          if (  (data[11].fvalue < 0.99999899)) {
            if (  (data[14].fvalue < 0.69302541)) {
              sum += (float)-0.14354324;
            } else {
              sum += (float)-0.038748108;
            }
          } else {
            if (  (data[17].fvalue < 0.17309807)) {
              sum += (float)-0.3103689;
            } else {
              sum += (float)-0.10787372;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4506514)) {
          if (  (data[14].fvalue < 1.2432872)) {
            if (  (data[3].fvalue < 2.1602926)) {
              sum += (float)0.047301259;
            } else {
              sum += (float)-0.05472761;
            }
          } else {
            if (  (data[8].fvalue < 4.999999)) {
              sum += (float)0.023633933;
            } else {
              sum += (float)0.11607843;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.098261163)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.027092872;
            } else {
              sum += (float)-0.13024466;
            }
          } else {
            if (  (data[1].fvalue < 1.5516747)) {
              sum += (float)0.041601188;
            } else {
              sum += (float)-0.028330404;
            }
          }
        }
      }
    }
  }
  if (  (data[14].fvalue < 0.35978603)) {
    if (  (data[16].fvalue < 0.18310761)) {
      if ( (data[4].fvalue < 1.1920545)) {
        if (  (data[9].fvalue < 0.99999899)) {
          if (  (data[7].fvalue < 2.999999)) {
            if (  (data[4].fvalue < 0.96928376)) {
              sum += (float)-0.0085269883;
            } else {
              sum += (float)0.0066060959;
            }
          } else {
            if ( (data[4].fvalue < 0.99902564)) {
              sum += (float)0.12035308;
            } else {
              sum += (float)-0.0044724056;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.1380988)) {
            if (  (data[7].fvalue < 0.99999899)) {
              sum += (float)-0.12824664;
            } else {
              sum += (float)-0.25299785;
            }
          } else {
            sum += (float)-0.033368744;
          }
        }
      } else {
        if (  (data[8].fvalue < 3.999999)) {
          if (  (data[14].fvalue < 0.13149557)) {
            if (  (data[4].fvalue < 1.6495608)) {
              sum += (float)-0.037759546;
            } else {
              sum += (float)0.017795183;
            }
          } else {
            if (  (data[5].fvalue < 0.24049439)) {
              sum += (float)0.012854848;
            } else {
              sum += (float)-0.017840981;
            }
          }
        } else {
          if (  (data[12].fvalue < 1.999999)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)-0.08140669;
            } else {
              sum += (float)0.033370022;
            }
          } else {
            sum += (float)0.46886915;
          }
        }
      }
    } else {
      if (  (data[8].fvalue < 2.999999)) {
        if (  (data[3].fvalue < 0.32994398)) {
          if (  (data[4].fvalue < 1.2817413)) {
            if (  (data[13].fvalue < 0.0055514751)) {
              sum += (float)0.046857398;
            } else {
              sum += (float)0.12036104;
            }
          } else {
            if (  (data[14].fvalue < 0.011356849)) {
              sum += (float)-0.00926496;
            } else {
              sum += (float)0.019129792;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.1883848)) {
            if (  (data[17].fvalue < 0.12348475)) {
              sum += (float)-0.032292943;
            } else {
              sum += (float)0.029438991;
            }
          } else {
            if (  (data[6].fvalue < 0.58320981)) {
              sum += (float)0.021869278;
            } else {
              sum += (float)-0.0093472265;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.4506822)) {
          if (  (data[7].fvalue < 1.999999)) {
            if (  (data[3].fvalue < 0.21582209)) {
              sum += (float)0.14550416;
            } else {
              sum += (float)-0.033293474;
            }
          } else {
            if (  (data[3].fvalue < 0.80268949)) {
              sum += (float)0.23482166;
            } else {
              sum += (float)0.060719561;
            }
          }
        } else {
          if (  (data[16].fvalue < 0.48850006)) {
            if (  (data[17].fvalue < 0.12348475)) {
              sum += (float)-0.069449447;
            } else {
              sum += (float)0.062252678;
            }
          } else {
            if (  (data[13].fvalue < 0.13798493)) {
              sum += (float)0.14822726;
            } else {
              sum += (float)-0.0053787446;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 4.999999)) {
      if (  (data[7].fvalue < 3.999999)) {
        if (  (data[17].fvalue < 0.97927123)) {
          if ( (data[4].fvalue < 1.8598982)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.0039904988;
            } else {
              sum += (float)0.019001704;
            }
          } else {
            if (  (data[16].fvalue < 0.78763354)) {
              sum += (float)-0.042197887;
            } else {
              sum += (float)0.051765412;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.9277579)) {
            if (  (data[1].fvalue < 0.77506614)) {
              sum += (float)-0.10760777;
            } else {
              sum += (float)0.011348205;
            }
          } else {
            if (  (data[11].fvalue < 1.999999)) {
              sum += (float)0.12923604;
            } else {
              sum += (float)0.01824796;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.1883848)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.72517061)) {
              sum += (float)0.028966602;
            } else {
              sum += (float)-0.1257344;
            }
          } else {
            sum += (float)-0.20420948;
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[11].fvalue < 0.99999899)) {
              sum += (float)0.16659132;
            } else {
              sum += (float)0.076980516;
            }
          } else {
            if (  (data[13].fvalue < 0.74344605)) {
              sum += (float)-0.0029360252;
            } else {
              sum += (float)0.1765652;
            }
          }
        }
      }
    } else {
      if (  (data[6].fvalue < 0.25673664)) {
        if (  (data[13].fvalue < 0.024442632)) {
          if (  (data[0].fvalue < 2.0741572)) {
            sum += (float)0.25317815;
          } else {
            if (  (data[14].fvalue < 0.49993831)) {
              sum += (float)-0.067472316;
            } else {
              sum += (float)0.05976272;
            }
          }
        } else {
          if (  (data[0].fvalue < 3.103399)) {
            if (  (data[6].fvalue < 0.19659081)) {
              sum += (float)0.39749005;
            } else {
              sum += (float)0.20811479;
            }
          } else {
            if (  (data[6].fvalue < 0.15792599)) {
              sum += (float)0.16889104;
            } else {
              sum += (float)0.033941999;
            }
          }
        }
      } else {
        if (  (data[0].fvalue < 1.7606282)) {
          if (  (data[6].fvalue < 0.35651404)) {
            if (  (data[13].fvalue < 0.014255814)) {
              sum += (float)0.070503443;
            } else {
              sum += (float)0.33658534;
            }
          } else {
            if (  (data[14].fvalue < 0.59766096)) {
              sum += (float)0.022097338;
            } else {
              sum += (float)0.14633924;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.76089698)) {
            if (  (data[16].fvalue < 0.11096836)) {
              sum += (float)-0.048013784;
            } else {
              sum += (float)0.037141658;
            }
          } else {
            if (  (data[4].fvalue < 1.8598982)) {
              sum += (float)0.083360672;
            } else {
              sum += (float)-0.037868742;
            }
          }
        }
      }
    }
  }
  if (  (data[3].fvalue < 0.84763104)) {
    if (  (data[13].fvalue < 0.04291581)) {
      if (  (data[8].fvalue < 3.999999)) {
        if (  (data[7].fvalue < 1.999999)) {
          if (  (data[13].fvalue < 0.0095823128)) {
            if (  (data[3].fvalue < 0.13204689)) {
              sum += (float)-0.012390825;
            } else {
              sum += (float)0.0029610978;
            }
          } else {
            if (  (data[14].fvalue < 0.059802756)) {
              sum += (float)-0.022064755;
            } else {
              sum += (float)0.050221164;
            }
          }
        } else {
          if (  (data[6].fvalue < 0.03748022)) {
            if (  (data[14].fvalue < 0.017008094)) {
              sum += (float)-0.092847377;
            } else {
              sum += (float)0.17959443;
            }
          } else {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)-0.10635862;
            } else {
              sum += (float)-0.016520215;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 0.96129596)) {
          if (  (data[3].fvalue < 0.32976043)) {
            if (  (data[14].fvalue < 0.041025735)) {
              sum += (float)0.021363638;
            } else {
              sum += (float)0.28678244;
            }
          } else {
            if (  (data[14].fvalue < 0.12376031)) {
              sum += (float)-0.19712634;
            } else {
              sum += (float)0.15942612;
            }
          }
        } else {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[3].fvalue < 0.09803424)) {
              sum += (float)-0.0064232438;
            } else {
              sum += (float)-0.078026958;
            }
          } else {
            if (  (data[4].fvalue < 1.2223424)) {
              sum += (float)0.14770703;
            } else {
              sum += (float)0.016981119;
            }
          }
        }
      }
    } else {
      if (  (data[3].fvalue < 0.2158411)) {
        if (  (data[8].fvalue < 4.999999)) {
          if (  (data[4].fvalue < 1.3045075)) {
            if (  (data[10].fvalue < 0.99999899)) {
              sum += (float)0.017492121;
            } else {
              sum += (float)0.091150694;
            }
          } else {
            if (  (data[0].fvalue < 1.6000854)) {
              sum += (float)0.058118615;
            } else {
              sum += (float)-0.0057756752;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3169323)) {
            if (  (data[14].fvalue < 0.2476836)) {
              sum += (float)0.064632364;
            } else {
              sum += (float)0.31702328;
            }
          } else {
            if (  (data[14].fvalue < 0.42977548)) {
              sum += (float)-0.014848151;
            } else {
              sum += (float)0.099978335;
            }
          }
        }
      } else {
        if (  (data[14].fvalue < 0.45086649)) {
          if (  (data[10].fvalue < 0.99999899)) {
            if (  (data[4].fvalue < 1.0438511)) {
              sum += (float)0.023747304;
            } else {
              sum += (float)-0.036450136;
            }
          } else {
            if (  (data[4].fvalue < 1.3169323)) {
              sum += (float)0.042744011;
            } else {
              sum += (float)-0.00090970809;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3753265)) {
            if (  (data[8].fvalue < 3.999999)) {
              sum += (float)0.035973728;
            } else {
              sum += (float)0.18754692;
            }
          } else {
            if (  (data[4].fvalue < 1.8595035)) {
              sum += (float)0.025620619;
            } else {
              sum += (float)-0.037516102;
            }
          }
        }
      }
    }
  } else {
    if (  (data[8].fvalue < 2.999999)) {
      if (  (data[16].fvalue < 0.010036197)) {
        if (  (data[17].fvalue < 0.057260443)) {
          if (  (data[13].fvalue < 0.0015766992)) {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)0.36638042;
            } else {
              sum += (float)0.032220181;
            }
          } else {
            if (  (data[8].fvalue < 0.99999899)) {
              sum += (float)-0.27866092;
            } else {
              sum += (float)-0.0025458774;
            }
          }
        } else {
          if (  (data[14].fvalue < 0.49979514)) {
            if (  (data[17].fvalue < 0.65422863)) {
              sum += (float)-0.014351401;
            } else {
              sum += (float)-0.093860254;
            }
          } else {
            if (  (data[17].fvalue < 0.98118544)) {
              sum += (float)-0.0054808464;
            } else {
              sum += (float)0.06286554;
            }
          }
        }
      } else {
        if (  (data[5].fvalue < 0.52638149)) {
          if (  (data[6].fvalue < 0.80416024)) {
            if (  (data[13].fvalue < 0.49530965)) {
              sum += (float)-0.016330745;
            } else {
              sum += (float)0.010716262;
            }
          } else {
            if (  (data[16].fvalue < 1.3499485)) {
              sum += (float)0.032771546;
            } else {
              sum += (float)0.17210846;
            }
          }
        } else {
          if (  (data[13].fvalue < 0.0015766992)) {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.079985686;
            } else {
              sum += (float)0.021251554;
            }
          } else {
            if (  (data[8].fvalue < 1.999999)) {
              sum += (float)-0.0032508036;
            } else {
              sum += (float)-0.072283901;
            }
          }
        }
      }
    } else {
      if (  (data[14].fvalue < 0.85073215)) {
        if (  (data[16].fvalue < 0.34299448)) {
          if (  (data[13].fvalue < 0.74407679)) {
            if (  (data[4].fvalue < 1.8595035)) {
              sum += (float)-0.081699036;
            } else {
              sum += (float)0.048850346;
            }
          } else {
            if (  (data[14].fvalue < 0.25839627)) {
              sum += (float)-0.036120385;
            } else {
              sum += (float)0.075274453;
            }
          }
        } else {
          if (  (data[7].fvalue < 4.999999)) {
            if (  (data[6].fvalue < 0.27409559)) {
              sum += (float)0.11904762;
            } else {
              sum += (float)-0.015381942;
            }
          } else {
            if (  (data[13].fvalue < 0.49530965)) {
              sum += (float)0.14527161;
            } else {
              sum += (float)0.015806038;
            }
          }
        }
      } else {
        if (  (data[13].fvalue < 0.12360074)) {
          if (  (data[7].fvalue < 0.99999899)) {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.020893881;
            } else {
              sum += (float)0.16148564;
            }
          } else {
            if (  (data[13].fvalue < 0.02995481)) {
              sum += (float)-0.1634459;
            } else {
              sum += (float)-0.0064393762;
            }
          }
        } else {
          if (  (data[0].fvalue < 3.5066435)) {
            if (  (data[3].fvalue < 2.1592727)) {
              sum += (float)0.11017668;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[8].fvalue < 5.999999)) {
              sum += (float)0.051993717;
            } else {
              sum += (float)-0.0043195407;
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
