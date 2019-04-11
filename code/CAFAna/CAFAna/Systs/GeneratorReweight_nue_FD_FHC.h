#include "BDTReweighter.h"
class GeneratorReweight_nue_FD_FHC : public BDTReweighter {
 public :
 GeneratorReweight_nue_FD_FHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_nue_FD_FHC() {;}


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
    if ( (data[14].fvalue < 0.053305663)) {
      if ( (data[5].fvalue < 1.2666825)) {
        if ( (data[13].fvalue < 1.2132158)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.11285476;
            } else {
              sum += (float)-0.0018306082;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.34644496;
            } else {
              sum += (float)0.20992574;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.85565889)) {
            if ( (data[4].fvalue < 1.5868783)) {
              sum += (float)-0.13317919;
            } else {
              sum += (float)-0.35263103;
            }
          } else {
            if ( (data[17].fvalue < 1.2645574)) {
              sum += (float)-0.04425928;
            } else {
              sum += (float)0.10028003;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 3.02956e-05)) {
          if ( (data[13].fvalue < 0.034936294)) {
            if ( (data[3].fvalue < 0.17615442)) {
              sum += (float)0.1766302;
            } else {
              sum += (float)0.43335614;
            }
          } else {
            if ( (data[3].fvalue < 0.24438697)) {
              sum += (float)-0.16466364;
            } else {
              sum += (float)0.15329392;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.7125993)) {
            if ( (data[14].fvalue < 0.021079689)) {
              sum += (float)-0.20461896;
            } else {
              sum += (float)-0.018695403;
            }
          } else {
            sum += (float)0.071066871;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.01775714)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.20870882)) {
              sum += (float)-0.31140882;
            } else {
              sum += (float)-0.12686394;
            }
          } else {
            if ( (data[13].fvalue < 6.0285329e-06)) {
              sum += (float)0.57972348;
            } else {
              sum += (float)0.023419255;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.20884028)) {
              sum += (float)-0.0017755803;
            } else {
              sum += (float)0.2933833;
            }
          } else {
            if ( (data[3].fvalue < 0.24601066)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.28879717;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.0874714)) {
            if ( (data[3].fvalue < 1.0879803)) {
              sum += (float)-0.022769481;
            } else {
              sum += (float)0.14767253;
            }
          } else {
            if ( (data[13].fvalue < 0.9728359)) {
              sum += (float)0.25535515;
            } else {
              sum += (float)0.05233172;
            }
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.11775539;
            } else {
              sum += (float)-0.054782193;
            }
          } else {
            if ( (data[3].fvalue < 1.0417783)) {
              sum += (float)0.35593456;
            } else {
              sum += (float)0.15334395;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 7.5)) {
      if ( (data[13].fvalue < 0.16259438)) {
        if ( (data[14].fvalue < 0.22163875)) {
          sum += (float)0.52859896;
        } else {
          sum += (float)0.3114;
        }
      } else {
        if ( (data[16].fvalue < 0.0022433149)) {
          if ( (data[14].fvalue < 0.14797446)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.33041811;
            } else {
              sum += (float)0.10995413;
            }
          } else {
            if ( (data[3].fvalue < 1.2003355)) {
              sum += (float)0.38477278;
            } else {
              sum += (float)0.23439917;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.33129752)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.061492287;
            } else {
              sum += (float)0.24005176;
            }
          } else {
            sum += (float)-0.076323316;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.0025444259)) {
        if ( (data[3].fvalue < 1.6939917)) {
          sum += (float)0.56007081;
        } else {
          if ( (data[7].fvalue < 9.5)) {
            sum += (float)0.32021424;
          } else {
            sum += (float)0.48922801;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.37419742)) {
          if ( (data[7].fvalue < 9.5)) {
            sum += (float)0.33282208;
          } else {
            sum += (float)0.49667531;
          }
        } else {
          sum += (float)0.13452229;
        }
      }
    }
  }
  if ( (data[8].fvalue < 4.5)) {
    if ( (data[14].fvalue < 0.15242557)) {
      if ( (data[16].fvalue < 0.0092592752)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.84171355)) {
            if ( (data[14].fvalue < 0.023767546)) {
              sum += (float)-0.042911585;
            } else {
              sum += (float)0.069037706;
            }
          } else {
            if ( (data[13].fvalue < 0.20714393)) {
              sum += (float)-0.30695477;
            } else {
              sum += (float)0.012035283;
            }
          }
        } else {
          if ( (data[7].fvalue < 4.5)) {
            if ( (data[3].fvalue < 0.31506222)) {
              sum += (float)-0.093511671;
            } else {
              sum += (float)-0.25566635;
            }
          } else {
            if ( (data[13].fvalue < 0.16160005)) {
              sum += (float)0.35085467;
            } else {
              sum += (float)-0.037976477;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0098282788)) {
              sum += (float)-0.29786021;
            } else {
              sum += (float)-0.0079667242;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.057187721;
            } else {
              sum += (float)-0.18853144;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.0030410262)) {
              sum += (float)-0.079504311;
            } else {
              sum += (float)0.15597597;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.24199554;
            } else {
              sum += (float)0.09022373;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.35397339)) {
        if ( (data[13].fvalue < 0.0093604177)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.38002837)) {
              sum += (float)-0.13017856;
            } else {
              sum += (float)0.06610693;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.24839614;
            } else {
              sum += (float)-0.13156812;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.2410195;
            } else {
              sum += (float)0.14384581;
            }
          } else {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)0.029305719;
            } else {
              sum += (float)0.21893878;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.1892357;
            } else {
              sum += (float)0.062650166;
            }
          } else {
            if ( (data[14].fvalue < 0.31747347)) {
              sum += (float)-0.073848501;
            } else {
              sum += (float)0.17450182;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 1.0660397e-05)) {
              sum += (float)0.43248355;
            } else {
              sum += (float)0.1232615;
            }
          } else {
            if ( (data[7].fvalue < 5.5)) {
              sum += (float)-0.055280212;
            } else {
              sum += (float)0.19812413;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 6.5)) {
      if ( (data[3].fvalue < 0.72805774)) {
        if (  (data[4].fvalue < 1.1582224)) {
          if ( (data[3].fvalue < 0.36301363)) {
            sum += (float)0.48311657;
          } else {
            sum += (float)0.32645792;
          }
        } else {
          if ( (data[15].fvalue < 0.13643461)) {
            if ( (data[14].fvalue < 0.30684173)) {
              sum += (float)0.096295483;
            } else {
              sum += (float)0.25997961;
            }
          } else {
            if (  (data[4].fvalue < 1.702765)) {
              sum += (float)0.39215741;
            } else {
              sum += (float)0.17883043;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.2819241)) {
          if ( (data[13].fvalue < 0.30045277)) {
            if ( (data[5].fvalue < 0.56581479)) {
              sum += (float)0.2146918;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[14].fvalue < 0.19393054)) {
              sum += (float)-0.27877823;
            } else {
              sum += (float)-0.038299516;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.19895002)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.15266646;
            } else {
              sum += (float)-0.10357798;
            }
          } else {
            if ( (data[17].fvalue < 0.0031310245)) {
              sum += (float)0.19730236;
            } else {
              sum += (float)0.052687261;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.1131957)) {
        if ( (data[14].fvalue < 0.20226447)) {
          sum += (float)0.46095487;
        } else {
          if ( (data[8].fvalue < 8.5)) {
            sum += (float)0.31002772;
          } else {
            if (  (data[4].fvalue < 2.0225902)) {
              sum += (float)0.44092223;
            } else {
              sum += (float)0.2973074;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 9.5)) {
          if ( (data[17].fvalue < 0.0032794203)) {
            if ( (data[3].fvalue < 1.7862667)) {
              sum += (float)0.26719266;
            } else {
              sum += (float)0.13540678;
            }
          } else {
            if ( (data[17].fvalue < 0.25527185)) {
              sum += (float)-0.074341737;
            } else {
              sum += (float)0.16758692;
            }
          }
        } else {
          if ( (data[3].fvalue < 2.1909776)) {
            sum += (float)0.39191827;
          } else {
            sum += (float)0.23745088;
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[13].fvalue < 1.4113907)) {
        if ( (data[5].fvalue < 0.2510367)) {
          if ( (data[14].fvalue < 0.030229103)) {
            if ( (data[13].fvalue < 0.1114545)) {
              sum += (float)-0.086247623;
            } else {
              sum += (float)0.0104067;
            }
          } else {
            if ( (data[13].fvalue < 0.011857865)) {
              sum += (float)0.010595757;
            } else {
              sum += (float)0.10565516;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.4397323)) {
            if ( (data[14].fvalue < 0.22627506)) {
              sum += (float)-0.060315643;
            } else {
              sum += (float)0.0025203906;
            }
          } else {
            if ( (data[14].fvalue < 3.726286e-05)) {
              sum += (float)0.17907664;
            } else {
              sum += (float)0.035731133;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.54488081)) {
          if ( (data[4].fvalue < 1.7986592)) {
            if ( (data[17].fvalue < 0.016656695)) {
              sum += (float)-0.061764847;
            } else {
              sum += (float)-0.23101903;
            }
          } else {
            if ( (data[16].fvalue < 0.5027597)) {
              sum += (float)-0.29899067;
            } else {
              sum += (float)-0.035730474;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.4049876)) {
            sum += (float)0.042701587;
          } else {
            sum += (float)-0.09042725;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.38533717)) {
        if (  (data[4].fvalue < 1.1330702)) {
          sum += (float)0.40930763;
        } else {
          if ( (data[8].fvalue < 6.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.13517983;
            } else {
              sum += (float)0.24487911;
            }
          } else {
            sum += (float)0.31102651;
          }
        }
      } else {
        if ( (data[8].fvalue < 6.5)) {
          if ( (data[3].fvalue < 1.4701071)) {
            if ( (data[15].fvalue < 0.35802662)) {
              sum += (float)0.057384335;
            } else {
              sum += (float)0.19183755;
            }
          } else {
            if ( (data[14].fvalue < 0.45056304)) {
              sum += (float)-0.13038786;
            } else {
              sum += (float)0.024810249;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.38373247;
          } else {
            if ( (data[3].fvalue < 2.111269)) {
              sum += (float)0.17302006;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.0042629479)) {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[3].fvalue < 0.81679344)) {
          if (  (data[4].fvalue < 1.1786227)) {
            sum += (float)0.40758219;
          } else {
            sum += (float)0.29717368;
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)0.32666847;
          } else {
            if ( (data[14].fvalue < 0.23347765)) {
              sum += (float)0.020721372;
            } else {
              sum += (float)0.1964599;
            }
          }
        }
      } else {
        sum += (float)0.38622603;
      }
    } else {
      if ( (data[16].fvalue < 0.1245904)) {
        sum += (float)0.014316668;
      } else {
        if ( (data[5].fvalue < 0.39106584)) {
          sum += (float)0.27363351;
        } else {
          sum += (float)0.10125331;
        }
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.23250936)) {
      if ( (data[13].fvalue < 1.0477202)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.057233226;
            } else {
              sum += (float)0.023639364;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.12447897;
            } else {
              sum += (float)-0.024841981;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.091932222)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.047119033;
            } else {
              sum += (float)0.37709889;
            }
          } else {
            if ( (data[15].fvalue < 0.13428727)) {
              sum += (float)-0.14733313;
            } else {
              sum += (float)0.060224477;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 1.0244265)) {
          if ( (data[4].fvalue < 1.4690154)) {
            if ( (data[3].fvalue < 1.9351285)) {
              sum += (float)0.051214673;
            } else {
              sum += (float)-0.073569432;
            }
          } else {
            if ( (data[15].fvalue < 1.220927)) {
              sum += (float)-0.18900897;
            } else {
              sum += (float)0.13823891;
            }
          }
        } else {
          if ( (data[17].fvalue < 1.4120133)) {
            sum += (float)0.021280652;
          } else {
            sum += (float)0.18394674;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.03523352)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.06074094;
            } else {
              sum += (float)0.14049755;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.15563361;
            } else {
              sum += (float)-0.054285824;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.63882756)) {
            sum += (float)-0.24511293;
          } else {
            sum += (float)-0.43666127;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.33718809)) {
            if ( (data[5].fvalue < 0.77406585)) {
              sum += (float)0.12690809;
            } else {
              sum += (float)-0.0047885613;
            }
          } else {
            if ( (data[14].fvalue < 1.3497462)) {
              sum += (float)0.19690867;
            } else {
              sum += (float)0.034983113;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.31592208)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.045186967;
            } else {
              sum += (float)-0.075333014;
            }
          } else {
            if ( (data[14].fvalue < 0.44656992)) {
              sum += (float)0.060613621;
            } else {
              sum += (float)0.17611563;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 8.5)) {
      if ( (data[3].fvalue < 0.60031497)) {
        if (  (data[4].fvalue < 1.1614432)) {
          sum += (float)0.36025596;
        } else {
          if ( (data[8].fvalue < 6.5)) {
            if ( (data[13].fvalue < 0.53767729)) {
              sum += (float)0.11323027;
            } else {
              sum += (float)0.22816081;
            }
          } else {
            if ( (data[13].fvalue < 0.30419129)) {
              sum += (float)0.32534418;
            } else {
              sum += (float)0.18901946;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)0.2918418;
        } else {
          if ( (data[14].fvalue < 0.068616197)) {
            sum += (float)-0.19644333;
          } else {
            if ( (data[15].fvalue < 0.26969177)) {
              sum += (float)0.052381009;
            } else {
              sum += (float)0.19008566;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.7592368)) {
        if ( (data[13].fvalue < 0.32252571)) {
          sum += (float)0.36569214;
        } else {
          if ( (data[7].fvalue < 9.5)) {
            sum += (float)0.22523062;
          } else {
            sum += (float)0.32758924;
          }
        }
      } else {
        if ( (data[7].fvalue < 10.5)) {
          sum += (float)0.083533138;
        } else {
          sum += (float)0.25210357;
        }
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[16].fvalue < 0.0035318271)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.11480591;
            } else {
              sum += (float)-0.09508232;
            }
          } else {
            if ( (data[13].fvalue < 1.0870286e-07)) {
              sum += (float)0.5271824;
            } else {
              sum += (float)-0.0037683791;
            }
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.37182853;
            } else {
              sum += (float)-0.1450624;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.11330502;
            } else {
              sum += (float)0.017952347;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.87520921)) {
          if ( (data[14].fvalue < 1.2924924)) {
            if (  (data[4].fvalue < 2.6866922)) {
              sum += (float)0.12313809;
            } else {
              sum += (float)-0.022047445;
            }
          } else {
            if (  (data[4].fvalue < 2.3773174)) {
              sum += (float)-0.12392267;
            } else {
              sum += (float)0.038642019;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.34441477)) {
            if ( (data[14].fvalue < 0.06699793)) {
              sum += (float)-0.14654878;
            } else {
              sum += (float)-0.018992905;
            }
          } else {
            sum += (float)0.047780976;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.90381062)) {
        if (  (data[4].fvalue < 1.0994931)) {
          sum += (float)0.31801215;
        } else {
          if ( (data[8].fvalue < 7.5)) {
            if ( (data[3].fvalue < 0.28163242)) {
              sum += (float)0.1601391;
            } else {
              sum += (float)0.065155342;
            }
          } else {
            sum += (float)0.26378578;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.410712)) {
          if ( (data[14].fvalue < 0.66932571)) {
            if ( (data[17].fvalue < 0.59572661)) {
              sum += (float)-0.1092828;
            } else {
              sum += (float)0.12317113;
            }
          } else {
            if ( (data[13].fvalue < 0.44586611)) {
              sum += (float)0.0093377912;
            } else {
              sum += (float)0.18666811;
            }
          }
        } else {
          if ( (data[8].fvalue < 5.5)) {
            sum += (float)0.046086576;
          } else {
            if ( (data[3].fvalue < 1.9951372)) {
              sum += (float)0.20762762;
            } else {
              sum += (float)0.069569387;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.0845189)) {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[13].fvalue < 0.18911165)) {
          sum += (float)0.30145255;
        } else {
          if ( (data[16].fvalue < 0.0091301724)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.25351673;
            } else {
              sum += (float)0.1646098;
            }
          } else {
            sum += (float)0.061961491;
          }
        }
      } else {
        if (  (data[4].fvalue < 2.0210309)) {
          sum += (float)0.32847434;
        } else {
          sum += (float)0.19023697;
        }
      }
    } else {
      if ( (data[7].fvalue < 9.5)) {
        if ( (data[8].fvalue < 2.5)) {
          sum += (float)0.24680674;
        } else {
          if ( (data[16].fvalue < 0.002208845)) {
            if ( (data[5].fvalue < 0.55667496)) {
              sum += (float)0.15065537;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.088186242;
          }
        }
      } else {
        sum += (float)0.23904693;
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[12].fvalue < 0.5)) {
      if ( (data[5].fvalue < 0.29207528)) {
        if ( (data[14].fvalue < 0.032467417)) {
          if ( (data[13].fvalue < 0.081448242)) {
            if ( (data[17].fvalue < 0.75616223)) {
              sum += (float)-0.038618173;
            } else {
              sum += (float)-0.35240862;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.020300832;
            } else {
              sum += (float)-0.26072115;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.031511083)) {
              sum += (float)0.023981882;
            } else {
              sum += (float)0.13171144;
            }
          } else {
            if ( (data[7].fvalue < 4.5)) {
              sum += (float)-0.040378913;
            } else {
              sum += (float)0.085543007;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.020223383)) {
          if ( (data[5].fvalue < 1.1484454)) {
            if ( (data[4].fvalue < 1.0350516)) {
              sum += (float)-0.048816577;
            } else {
              sum += (float)0.006146952;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.26679567;
            } else {
              sum += (float)0.076594859;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.15105969)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.1045833;
            } else {
              sum += (float)-0.23671655;
            }
          } else {
            if ( (data[14].fvalue < 0.014965814)) {
              sum += (float)-0.11014077;
            } else {
              sum += (float)-0.0058286763;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.7286056)) {
        if ( (data[3].fvalue < 0.84160149)) {
          if ( (data[3].fvalue < 0.39045644)) {
            sum += (float)0.25100443;
          } else {
            sum += (float)0.037179776;
          }
        } else {
          if (  (data[4].fvalue < 1.5606332)) {
            sum += (float)-0.13194261;
          } else {
            sum += (float)-0.26959106;
          }
        }
      } else {
        if (  (data[4].fvalue < 2.4360559)) {
          sum += (float)-0.37671468;
        } else {
          sum += (float)-0.028421091;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.22541705)) {
      if ( (data[8].fvalue < 5.5)) {
        sum += (float)0.14121449;
      } else {
        sum += (float)0.3204968;
      }
    } else {
      if ( (data[7].fvalue < 9.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.048114508)) {
            sum += (float)0.10631109;
          } else {
            sum += (float)0.26690096;
          }
        } else {
          if ( (data[17].fvalue < 0.0048366301)) {
            if ( (data[16].fvalue < 0.00019331164)) {
              sum += (float)0.13677138;
            } else {
              sum += (float)0.0022034324;
            }
          } else {
            if ( (data[17].fvalue < 0.17602178)) {
              sum += (float)-0.18911542;
            } else {
              sum += (float)0.023807235;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.0056608967)) {
          sum += (float)0.28016433;
        } else {
          sum += (float)0.1435734;
        }
      }
    }
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[14].fvalue < 0.27235639)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.028253067)) {
              sum += (float)0.43559197;
            } else {
              sum += (float)-0.061781187;
            }
          } else {
            if ( (data[14].fvalue < 0.14906701)) {
              sum += (float)-0.27680987;
            } else {
              sum += (float)-0.096099459;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.01565478)) {
            if ( (data[14].fvalue < 7.0669245e-08)) {
              sum += (float)-0.0043283864;
            } else {
              sum += (float)-0.16039483;
            }
          } else {
            if (  (data[4].fvalue < 1.0485202)) {
              sum += (float)-0.049757309;
            } else {
              sum += (float)0.11975314;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.067562371)) {
          if ( (data[13].fvalue < 0.24037902)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.16572475;
            } else {
              sum += (float)0.19065489;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.097616173;
            } else {
              sum += (float)-0.29053363;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.12895399)) {
            if ( (data[6].fvalue < 0.080018193)) {
              sum += (float)0.12616289;
            } else {
              sum += (float)-0.081242763;
            }
          } else {
            if ( (data[3].fvalue < 1.029315)) {
              sum += (float)0.098961599;
            } else {
              sum += (float)-0.058982462;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 1.276402)) {
        if ( (data[13].fvalue < 0.4509083)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.0087294076)) {
              sum += (float)0.027572844;
            } else {
              sum += (float)0.11504292;
            }
          } else {
            if ( (data[13].fvalue < 0.068847984)) {
              sum += (float)-0.23291084;
            } else {
              sum += (float)0.0033475396;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.48241687)) {
            if ( (data[15].fvalue < 0.023006566)) {
              sum += (float)0.12589805;
            } else {
              sum += (float)0.008958173;
            }
          } else {
            if ( (data[17].fvalue < 0.04135491)) {
              sum += (float)0.25415775;
            } else {
              sum += (float)0.11928254;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.68178713)) {
          if ( (data[13].fvalue < 0.42896533)) {
            if ( (data[15].fvalue < 0.42149794)) {
              sum += (float)-0.1886296;
            } else {
              sum += (float)-0.045472644;
            }
          } else {
            sum += (float)0.16181219;
          }
        } else {
          if ( (data[17].fvalue < 0.0011325225)) {
            if ( (data[5].fvalue < 0.43860418)) {
              sum += (float)0.28286725;
            } else {
              sum += (float)0.08326456;
            }
          } else {
            sum += (float)-0.0044988869;
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.2718871)) {
      if (  (data[4].fvalue < 1.2281839)) {
        if ( (data[7].fvalue < 4.5)) {
          sum += (float)0.18046689;
        } else {
          sum += (float)0.31149831;
        }
      } else {
        if ( (data[8].fvalue < 7.5)) {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.24776697;
          } else {
            if ( (data[15].fvalue < 0.1606532)) {
              sum += (float)0.025054282;
            } else {
              sum += (float)0.12600039;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8353897)) {
            sum += (float)0.25917834;
          } else {
            if ( (data[13].fvalue < 0.34349659)) {
              sum += (float)0.18914478;
            } else {
              sum += (float)0.035439834;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.63811707)) {
        if ( (data[14].fvalue < 0.91052759)) {
          if ( (data[3].fvalue < 2.3074546)) {
            if ( (data[8].fvalue < 7.5)) {
              sum += (float)-0.047348313;
            } else {
              sum += (float)0.037075829;
            }
          } else {
            sum += (float)-0.17815638;
          }
        } else {
          if ( (data[13].fvalue < 0.41234627)) {
            sum += (float)0.00017710052;
          } else {
            sum += (float)0.15030436;
          }
        }
      } else {
        sum += (float)0.15094462;
      }
    }
  }
  if ( (data[7].fvalue < 7.5)) {
    if ( (data[13].fvalue < 1.8521565)) {
      if ( (data[5].fvalue < 0.13016021)) {
        if (  (data[4].fvalue < 1.8145633)) {
          if (  (data[4].fvalue < 1.4213343)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.014561882;
            } else {
              sum += (float)0.065851718;
            }
          } else {
            if ( (data[3].fvalue < 0.062395476)) {
              sum += (float)0.20445877;
            } else {
              sum += (float)0.090126783;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.66704607)) {
            if ( (data[13].fvalue < 0.82465446)) {
              sum += (float)-0.010920649;
            } else {
              sum += (float)-0.15526788;
            }
          } else {
            if ( (data[13].fvalue < 1.2447948)) {
              sum += (float)0.051956844;
            } else {
              sum += (float)-0.052292444;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.028125068)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.006382715;
            } else {
              sum += (float)-0.059088923;
            }
          } else {
            if ( (data[6].fvalue < 0.12424779)) {
              sum += (float)0.26714432;
            } else {
              sum += (float)0.033809625;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.00279231)) {
              sum += (float)-0.13089775;
            } else {
              sum += (float)-0.001385842;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.14810279;
            } else {
              sum += (float)-0.041873142;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.72508)) {
        if ( (data[0].fvalue < 3.6766136)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.074166603;
        }
      } else {
        if ( (data[17].fvalue < 0.52320313)) {
          if ( (data[16].fvalue < 0.31235012)) {
            if (  (data[4].fvalue < 2.1521282)) {
              sum += (float)-0.17394106;
            } else {
              sum += (float)-0.28854981;
            }
          } else {
            sum += (float)-0.092418239;
          }
        } else {
          sum += (float)0.0078496076;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.24631982)) {
      sum += (float)0.28193957;
    } else {
      if ( (data[16].fvalue < 0.0080134049)) {
        if ( (data[7].fvalue < 10.5)) {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)0.24064364;
          } else {
            if ( (data[17].fvalue < 0.011468505)) {
              sum += (float)0.12880959;
            } else {
              sum += (float)0.028368123;
            }
          }
        } else {
          sum += (float)0.26800838;
        }
      } else {
        if ( (data[8].fvalue < 4.5)) {
          sum += (float)0.12952699;
        } else {
          sum += (float)-0.043044768;
        }
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[7].fvalue < 2.5)) {
      if ( (data[13].fvalue < 0.14987716)) {
        if ( (data[14].fvalue < 0.25263661)) {
          if ( (data[15].fvalue < 1.2034844)) {
            if ( (data[17].fvalue < 0.83874893)) {
              sum += (float)-0.015894948;
            } else {
              sum += (float)-0.20464818;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.23663238;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.39962116)) {
            if ( (data[15].fvalue < 0.46946156)) {
              sum += (float)0.0082203588;
            } else {
              sum += (float)0.11318134;
            }
          } else {
            if ( (data[13].fvalue < 0.021695452)) {
              sum += (float)-0.049309921;
            } else {
              sum += (float)0.035121046;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.007726924)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.011153136)) {
              sum += (float)0.011409428;
            } else {
              sum += (float)-0.084130064;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.10072722;
            } else {
              sum += (float)-0.21961172;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[5].fvalue < 0.65968466)) {
              sum += (float)0.15375955;
            } else {
              sum += (float)0.02688699;
            }
          } else {
            if ( (data[14].fvalue < 0.28596938)) {
              sum += (float)-0.047797911;
            } else {
              sum += (float)0.071722753;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 0.5)) {
        if ( (data[15].fvalue < 0.14595711)) {
          if ( (data[13].fvalue < 0.10801689)) {
            sum += (float)-0.075741768;
          } else {
            if ( (data[3].fvalue < 0.4000591)) {
              sum += (float)0.07057789;
            } else {
              sum += (float)0.0066906973;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.48620319)) {
            sum += (float)0.090401188;
          } else {
            sum += (float)0.20253849;
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.031072047)) {
            if ( (data[6].fvalue < 0.082563877)) {
              sum += (float)-0.0032988845;
            } else {
              sum += (float)-0.2863906;
            }
          } else {
            if ( (data[4].fvalue < 1.3787663)) {
              sum += (float)-0.14363568;
            } else {
              sum += (float)-0.052888673;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.0786286)) {
            sum += (float)0.3202264;
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.076349303;
            } else {
              sum += (float)0.0083843227;
            }
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.0010144565)) {
      if ( (data[6].fvalue < 0.163541)) {
        if (  (data[4].fvalue < 1.139385)) {
          if ( (data[3].fvalue < 0.49269164)) {
            sum += (float)0.35219863;
          } else {
            sum += (float)0.21338256;
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[13].fvalue < 0.29160357)) {
              sum += (float)-0.11059181;
            } else {
              sum += (float)0.067504518;
            }
          } else {
            sum += (float)0.23279819;
          }
        }
      } else {
        if ( (data[7].fvalue < 10.5)) {
          if ( (data[8].fvalue < 0.5)) {
            sum += (float)0.25534144;
          } else {
            if ( (data[14].fvalue < 0.040957391)) {
              sum += (float)-0.12030089;
            } else {
              sum += (float)0.076028503;
            }
          }
        } else {
          sum += (float)0.26019275;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.14000918)) {
        if ( (data[15].fvalue < 0.10438249)) {
          if ( (data[13].fvalue < 0.76119566)) {
            sum += (float)-0.33795816;
          } else {
            sum += (float)-0.10907397;
          }
        } else {
          sum += (float)-0.0367731;
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.2117103)) {
            if ( (data[16].fvalue < 0.51137233)) {
              sum += (float)-0.0044775526;
            } else {
              sum += (float)0.12301109;
            }
          } else {
            sum += (float)0.19049869;
          }
        } else {
          sum += (float)-0.13288422;
        }
      }
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[12].fvalue < 0.5)) {
      if ( (data[5].fvalue < 0.36346513)) {
        if ( (data[13].fvalue < 1.647956)) {
          if ( (data[13].fvalue < 0.18967074)) {
            if ( (data[14].fvalue < 0.012117069)) {
              sum += (float)-0.034686383;
            } else {
              sum += (float)0.022760337;
            }
          } else {
            if ( (data[17].fvalue < 0.90230262)) {
              sum += (float)0.031140788;
            } else {
              sum += (float)0.15498401;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.38592684)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.20559353;
            } else {
              sum += (float)-0.034368627;
            }
          } else {
            sum += (float)0.017225899;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.0023664781)) {
          if ( (data[17].fvalue < 0.017543264)) {
            if ( (data[14].fvalue < 4.347784e-08)) {
              sum += (float)0.031136774;
            } else {
              sum += (float)-0.025346644;
            }
          } else {
            if ( (data[17].fvalue < 0.13407668)) {
              sum += (float)-0.11955769;
            } else {
              sum += (float)-0.035002418;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.12592751)) {
            if ( (data[4].fvalue < 1.2920878)) {
              sum += (float)-0.30249691;
            } else {
              sum += (float)-0.14516811;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.075835302;
            } else {
              sum += (float)0.063028544;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.6345061)) {
        if ( (data[3].fvalue < 0.70807099)) {
          sum += (float)0.14269994;
        } else {
          sum += (float)-0.12472577;
        }
      } else {
        if (  (data[4].fvalue < 2.5454383)) {
          if ( (data[3].fvalue < 0.37082413)) {
            sum += (float)-0.061609831;
          } else {
            if (  (data[4].fvalue < 1.7993453)) {
              sum += (float)-0.17009439;
            } else {
              sum += (float)-0.31398436;
            }
          }
        } else {
          sum += (float)0.01281736;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.39459443)) {
      if ( (data[14].fvalue < 0.29933438)) {
        sum += (float)0.27333748;
      } else {
        if ( (data[17].fvalue < 0.0019326594)) {
          sum += (float)0.16512847;
        } else {
          sum += (float)0.039682511;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.011714804)) {
        if ( (data[14].fvalue < 0.69495225)) {
          sum += (float)0.042257093;
        } else {
          sum += (float)0.1633686;
        }
      } else {
        sum += (float)-0.074640438;
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[5].fvalue < 0.16849051)) {
      if ( (data[13].fvalue < 0.82585573)) {
        if (  (data[4].fvalue < 1.275512)) {
          if (  (data[4].fvalue < 0.99309862)) {
            sum += (float)0.19306734;
          } else {
            if ( (data[15].fvalue < 0.12082806)) {
              sum += (float)-0.045452237;
            } else {
              sum += (float)0.069311135;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.92788762)) {
            if ( (data[16].fvalue < 0.026769303)) {
              sum += (float)0.060635541;
            } else {
              sum += (float)0.0063592759;
            }
          } else {
            if ( (data[15].fvalue < 0.67594612)) {
              sum += (float)-0.10968434;
            } else {
              sum += (float)0.051784486;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.3918247)) {
          if ( (data[17].fvalue < 0.38693911)) {
            if (  (data[4].fvalue < 1.878171)) {
              sum += (float)-0.071536005;
            } else {
              sum += (float)-0.21505402;
            }
          } else {
            sum += (float)0.062523536;
          }
        } else {
          if ( (data[13].fvalue < 1.6829537)) {
            sum += (float)0.15435226;
          } else {
            sum += (float)-0.077444933;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 1.3412135)) {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.28829592)) {
            if ( (data[17].fvalue < 0.044018622)) {
              sum += (float)-0.015592925;
            } else {
              sum += (float)-0.059400667;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.017681243;
            } else {
              sum += (float)-0.039712939;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.51107848)) {
            sum += (float)-0.025461726;
          } else {
            if ( (data[0].fvalue < 2.4978573)) {
              sum += (float)-0.082110167;
            } else {
              sum += (float)-0.21688598;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 0.78837299)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[4].fvalue < 0.68300617)) {
              sum += (float)0.25899041;
            } else {
              sum += (float)0.14590308;
            }
          } else {
            sum += (float)0.0023487788;
          }
        } else {
          if ( (data[13].fvalue < 0.088445358)) {
            if ( (data[14].fvalue < 1.1724632e-05)) {
              sum += (float)0.14683382;
            } else {
              sum += (float)-0.014603556;
            }
          } else {
            if ( (data[3].fvalue < 0.41831458)) {
              sum += (float)-0.24124721;
            } else {
              sum += (float)0.03584934;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.093935259)) {
      if ( (data[14].fvalue < 0.12232274)) {
        sum += (float)0.2947979;
      } else {
        sum += (float)0.013748106;
      }
    } else {
      if ( (data[7].fvalue < 9.5)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.61170542)) {
            sum += (float)0.052517522;
          } else {
            sum += (float)0.26209107;
          }
        } else {
          if ( (data[14].fvalue < 0.041344348)) {
            if (  (data[4].fvalue < 1.1357611)) {
              sum += (float)0.08951889;
            } else {
              sum += (float)-0.16047686;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.12693648;
            } else {
              sum += (float)0.014664548;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 1.1336019)) {
          sum += (float)0.23720203;
        } else {
          sum += (float)0.094872892;
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.18146059)) {
    if ( (data[8].fvalue < 6.5)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[15].fvalue < 0.049136724)) {
          if ( (data[13].fvalue < 4.6981097e-07)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.08753179;
            } else {
              sum += (float)0.46476927;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.0015230486;
            } else {
              sum += (float)0.093928911;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.021334834)) {
            if ( (data[5].fvalue < 0.099628538)) {
              sum += (float)-0.055278491;
            } else {
              sum += (float)-0.16941193;
            }
          } else {
            if ( (data[14].fvalue < 0.0040454902)) {
              sum += (float)-0.04777373;
            } else {
              sum += (float)0.097374007;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.11420161)) {
          if (  (data[4].fvalue < 1.1142607)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.11743832;
            } else {
              sum += (float)0.13297349;
            }
          } else {
            if ( (data[17].fvalue < 0.1385475)) {
              sum += (float)-0.15303904;
            } else {
              sum += (float)-0.0093980171;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.36279368)) {
            if ( (data[3].fvalue < 0.65553117)) {
              sum += (float)0.13512442;
            } else {
              sum += (float)-0.011154221;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.031790961;
            } else {
              sum += (float)-0.22748227;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.17604339)) {
        sum += (float)0.26763752;
      } else {
        sum += (float)0.051133648;
      }
    }
  } else {
    if ( (data[13].fvalue < 0.30230051)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.34831408;
            } else {
              sum += (float)-0.014442415;
            }
          } else {
            if ( (data[16].fvalue < 0.0237699)) {
              sum += (float)0.08380568;
            } else {
              sum += (float)-0.003086864;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.66425109)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.12601629;
            } else {
              sum += (float)0.059260573;
            }
          } else {
            if ( (data[14].fvalue < 0.56121302)) {
              sum += (float)-0.12965304;
            } else {
              sum += (float)0.082975939;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.08101815)) {
          if ( (data[3].fvalue < 0.39335722)) {
            if (  (data[4].fvalue < 1.2320065)) {
              sum += (float)0.068233348;
            } else {
              sum += (float)-0.13550553;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.088613428;
            } else {
              sum += (float)-0.24054684;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.86135125)) {
            if ( (data[6].fvalue < 0.17434633)) {
              sum += (float)0.12272678;
            } else {
              sum += (float)-0.0021927424;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.17389509;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.7992499)) {
        if ( (data[14].fvalue < 0.36490214)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.061197408;
            } else {
              sum += (float)0.16586563;
            }
          } else {
            if ( (data[6].fvalue < 0.33225116)) {
              sum += (float)0.095721491;
            } else {
              sum += (float)-0.025221976;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.48065192)) {
            if (  (data[4].fvalue < 1.8048418)) {
              sum += (float)0.10249809;
            } else {
              sum += (float)-0.0034813832;
            }
          } else {
            if ( (data[15].fvalue < 0.04309424)) {
              sum += (float)0.20039092;
            } else {
              sum += (float)0.073488921;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.56111431)) {
          if (  (data[4].fvalue < 1.3696368)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.046305709;
            } else {
              sum += (float)-0.20500183;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.05277513;
            } else {
              sum += (float)-0.050252888;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.72167134)) {
            if ( (data[15].fvalue < 0.41454527)) {
              sum += (float)-0.040328957;
            } else {
              sum += (float)0.089660786;
            }
          } else {
            if ( (data[17].fvalue < 0.020347822)) {
              sum += (float)0.21971606;
            } else {
              sum += (float)0.052551653;
            }
          }
        }
      }
    }
  }
  if ( (data[17].fvalue < 0.037119836)) {
    if ( (data[5].fvalue < 0.47551608)) {
      if ( (data[15].fvalue < 0.18033817)) {
        if ( (data[13].fvalue < 3.031407e-06)) {
          if ( (data[14].fvalue < 7.2789448e-06)) {
            if ( (data[6].fvalue < 0.27981681)) {
              sum += (float)0.00021131021;
            } else {
              sum += (float)0.47311065;
            }
          } else {
            if (  (data[4].fvalue < 1.1235199)) {
              sum += (float)-0.1575187;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1515124)) {
            if (  (data[4].fvalue < 1.0493703)) {
              sum += (float)-0.025604287;
            } else {
              sum += (float)0.14107955;
            }
          } else {
            if ( (data[13].fvalue < 0.3361389)) {
              sum += (float)-0.042677283;
            } else {
              sum += (float)0.013181211;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.052331045)) {
          if ( (data[14].fvalue < 0.15391394)) {
            if ( (data[3].fvalue < 0.4185254)) {
              sum += (float)-0.019726712;
            } else {
              sum += (float)-0.151628;
            }
          } else {
            if ( (data[15].fvalue < 0.30246538)) {
              sum += (float)-0.013342066;
            } else {
              sum += (float)0.084716655;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6292069)) {
            if ( (data[5].fvalue < 0.19004166)) {
              sum += (float)0.19912772;
            } else {
              sum += (float)0.089709476;
            }
          } else {
            if ( (data[15].fvalue < 0.45389727)) {
              sum += (float)-0.026936393;
            } else {
              sum += (float)0.070137627;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 1.4579578e-08)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.038758025)) {
            if ( (data[3].fvalue < 0.063850746)) {
              sum += (float)-0.045736134;
            } else {
              sum += (float)0.22898497;
            }
          } else {
            if ( (data[3].fvalue < 0.2753484)) {
              sum += (float)-0.082803763;
            } else {
              sum += (float)0.0072217337;
            }
          }
        } else {
          sum += (float)0.58988762;
        }
      } else {
        if ( (data[14].fvalue < 0.020650178)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[3].fvalue < 0.3898136)) {
              sum += (float)-0.1299278;
            } else {
              sum += (float)0.056104105;
            }
          } else {
            if ( (data[6].fvalue < 0.04277515)) {
              sum += (float)-0.017947407;
            } else {
              sum += (float)-0.25060138;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.039135568)) {
            if ( (data[14].fvalue < 0.41249681)) {
              sum += (float)-0.10150107;
            } else {
              sum += (float)-0.00725471;
            }
          } else {
            if ( (data[3].fvalue < 0.34357849)) {
              sum += (float)0.13238068;
            } else {
              sum += (float)-0.0064948509;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.16394852)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[13].fvalue < 0.013198554)) {
          if ( (data[15].fvalue < 1.1930262)) {
            if ( (data[0].fvalue < 2.4263735)) {
              sum += (float)-0.27141201;
            } else {
              sum += (float)-0.1561311;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[14].fvalue < 0.019769873)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.12449361;
            } else {
              sum += (float)-0.012574987;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.026187332;
            } else {
              sum += (float)-0.1095495;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.59578741)) {
          if (  (data[4].fvalue < 1.3753605)) {
            sum += (float)0.017766578;
          } else {
            if ( (data[14].fvalue < 0.33236945)) {
              sum += (float)-0.20105287;
            } else {
              sum += (float)-0.092602313;
            }
          }
        } else {
          sum += (float)-0.30427355;
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.0040727528)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.18942595)) {
              sum += (float)-0.16836092;
            } else {
              sum += (float)-0.058286425;
            }
          } else {
            if ( (data[13].fvalue < 0.8899498)) {
              sum += (float)0.06531705;
            } else {
              sum += (float)-0.10949254;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.028121661)) {
            if ( (data[17].fvalue < 0.63268715)) {
              sum += (float)-0.075496525;
            } else {
              sum += (float)0.029678116;
            }
          } else {
            if ( (data[15].fvalue < 0.055194248)) {
              sum += (float)0.14378251;
            } else {
              sum += (float)0.02953925;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[16].fvalue < 0.007380601)) {
            if ( (data[13].fvalue < 0.13954206)) {
              sum += (float)0.017169682;
            } else {
              sum += (float)0.15343338;
            }
          } else {
            if ( (data[13].fvalue < 0.49749398)) {
              sum += (float)-0.055737887;
            } else {
              sum += (float)0.046574052;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.58448833)) {
            if ( (data[14].fvalue < 0.011350145)) {
              sum += (float)-0.15782571;
            } else {
              sum += (float)0.040989392;
            }
          } else {
            if ( (data[5].fvalue < 0.7830615)) {
              sum += (float)-0.08964394;
            } else {
              sum += (float)-0.211666;
            }
          }
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.0065890364)) {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[5].fvalue < 0.51384294)) {
        if ( (data[13].fvalue < 1.7701623)) {
          if ( (data[14].fvalue < 1.0672705)) {
            if ( (data[12].fvalue < 0.5)) {
              sum += (float)0.018980114;
            } else {
              sum += (float)-0.09591388;
            }
          } else {
            if ( (data[15].fvalue < 0.98771864)) {
              sum += (float)-0.087520212;
            } else {
              sum += (float)0.21571709;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.43058378)) {
            if ( (data[13].fvalue < 2.5534644)) {
              sum += (float)-0.12026135;
            } else {
              sum += (float)-0.21430419;
            }
          } else {
            sum += (float)0.002188304;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.037097894)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.27602792;
            } else {
              sum += (float)0.21064427;
            }
          } else {
            if ( (data[13].fvalue < 1.0870286e-07)) {
              sum += (float)0.38589662;
            } else {
              sum += (float)-0.0039593196;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.1302672)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.093400806;
            } else {
              sum += (float)-0.29652402;
            }
          } else {
            if ( (data[15].fvalue < 1.0495536)) {
              sum += (float)-0.016595505;
            } else {
              sum += (float)-0.10804359;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.096827917)) {
        sum += (float)0.23250194;
      } else {
        if ( (data[8].fvalue < 10.5)) {
          if ( (data[15].fvalue < 0.16100681)) {
            if ( (data[13].fvalue < 0.61363399)) {
              sum += (float)0.001283985;
            } else {
              sum += (float)0.07988435;
            }
          } else {
            if ( (data[17].fvalue < 0.013271981)) {
              sum += (float)0.14823076;
            } else {
              sum += (float)0.021852838;
            }
          }
        } else {
          sum += (float)0.20207955;
        }
      }
    }
  } else {
    if ( (data[9].fvalue < 0.5)) {
      if ( (data[16].fvalue < 0.54765028)) {
        if ( (data[7].fvalue < 1.5)) {
          if (  (data[4].fvalue < 1.9079754)) {
            sum += (float)-0.38882944;
          } else {
            sum += (float)-0.15173694;
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.39457673)) {
              sum += (float)-0.10248139;
            } else {
              sum += (float)0.019756801;
            }
          } else {
            if ( (data[4].fvalue < 1.4043119)) {
              sum += (float)-0.2741507;
            } else {
              sum += (float)-0.10582049;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.063509978)) {
          sum += (float)-0.045020089;
        } else {
          sum += (float)0.15418208;
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.0020147027)) {
          if (  (data[4].fvalue < 2.1606698)) {
            if ( (data[13].fvalue < 0.59101886)) {
              sum += (float)-0.074305035;
            } else {
              sum += (float)-0.17291991;
            }
          } else {
            if ( (data[13].fvalue < 0.3031325)) {
              sum += (float)-0.079276599;
            } else {
              sum += (float)0.069346562;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.025309935)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.19679727;
            } else {
              sum += (float)0.0041164127;
            }
          } else {
            if ( (data[16].fvalue < 0.81542033)) {
              sum += (float)0.031525239;
            } else {
              sum += (float)0.18868575;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[17].fvalue < 0.037040267)) {
            if ( (data[13].fvalue < 0.17640516)) {
              sum += (float)-0;
            } else {
              sum += (float)0.11839348;
            }
          } else {
            if (  (data[4].fvalue < 2.0244918)) {
              sum += (float)-0.037229415;
            } else {
              sum += (float)0.045315851;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.11927529)) {
            if ( (data[2].fvalue < 0.29290116)) {
              sum += (float)-0.0088980626;
            } else {
              sum += (float)-0.13019052;
            }
          } else {
            if ( (data[5].fvalue < 0.44385976)) {
              sum += (float)0.047362376;
            } else {
              sum += (float)-0.07500577;
            }
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.09984687)) {
    if ( (data[8].fvalue < 1.5)) {
      if ( (data[15].fvalue < 0.04901693)) {
        if ( (data[13].fvalue < 4.6981097e-07)) {
          if (  (data[4].fvalue < 1.4898009)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.36802763;
            } else {
              sum += (float)0.38830063;
            }
          } else {
            if ( (data[5].fvalue < 0.21625185)) {
              sum += (float)0.25239074;
            } else {
              sum += (float)0.40659416;
            }
          }
        } else {
          if ( (data[14].fvalue < 1.4579578e-08)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.007191767;
            } else {
              sum += (float)0.50928479;
            }
          } else {
            if ( (data[14].fvalue < 0.0075593479)) {
              sum += (float)-0.18018995;
            } else {
              sum += (float)-0.0070405463;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.10451071)) {
          if ( (data[5].fvalue < 0.19801629)) {
            if ( (data[15].fvalue < 1.7723012)) {
              sum += (float)-0.034319788;
            } else {
              sum += (float)-0.15191947;
            }
          } else {
            if ( (data[15].fvalue < 0.49683517)) {
              sum += (float)-0.10066754;
            } else {
              sum += (float)-0.21908869;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0056875134)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.052476164;
            } else {
              sum += (float)0.071906894;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.17050128;
            } else {
              sum += (float)-0.025598451;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.41477674)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[13].fvalue < 0.017527595)) {
            if ( (data[15].fvalue < 0.13164133)) {
              sum += (float)-0.23621579;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[6].fvalue < 0.054306414)) {
              sum += (float)0.23061748;
            } else {
              sum += (float)-0.051699881;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.12147404)) {
            if ( (data[3].fvalue < 0.17122233)) {
              sum += (float)0.29375389;
            } else {
              sum += (float)0.14859454;
            }
          } else {
            if (  (data[4].fvalue < 1.2241114)) {
              sum += (float)0.15805432;
            } else {
              sum += (float)-0.084358603;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.1745038)) {
          if ( (data[14].fvalue < 0.074352205)) {
            if ( (data[3].fvalue < 0.57869232)) {
              sum += (float)-0.15861134;
            } else {
              sum += (float)-0.24949116;
            }
          } else {
            if ( (data[3].fvalue < 0.59529889)) {
              sum += (float)-0.0035426051;
            } else {
              sum += (float)-0.14072576;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.029678378)) {
            sum += (float)-0.16117249;
          } else {
            if ( (data[13].fvalue < 0.59204775)) {
              sum += (float)0.018553952;
            } else {
              sum += (float)-0.10932776;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.99955952)) {
      if ( (data[17].fvalue < 0.011034578)) {
        if ( (data[16].fvalue < 0.3917008)) {
          if ( (data[3].fvalue < 1.1178083)) {
            if ( (data[15].fvalue < 0.19123606)) {
              sum += (float)0.015455837;
            } else {
              sum += (float)0.062893257;
            }
          } else {
            if ( (data[14].fvalue < 0.43085238)) {
              sum += (float)-0.067692161;
            } else {
              sum += (float)0.021734608;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.66797352)) {
            if ( (data[0].fvalue < 3.731602)) {
              sum += (float)0.11289536;
            } else {
              sum += (float)0.0047678216;
            }
          } else {
            sum += (float)0.14295997;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.0051546572)) {
          if ( (data[9].fvalue < 0.5)) {
            sum += (float)-0.30166072;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.083424903;
            } else {
              sum += (float)0.067646161;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[4].fvalue < 1.7452289)) {
              sum += (float)0.046648428;
            } else {
              sum += (float)-0.0093053179;
            }
          } else {
            if ( (data[5].fvalue < 0.71192646)) {
              sum += (float)-0.030019615;
            } else {
              sum += (float)-0.2641854;
            }
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.20970866)) {
        if ( (data[11].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.32122207)) {
            if ( (data[13].fvalue < 0.19448125)) {
              sum += (float)0.052566852;
            } else {
              sum += (float)0.13863415;
            }
          } else {
            sum += (float)0.24095351;
          }
        } else {
          if (  (data[4].fvalue < 2.1261394)) {
            sum += (float)0.15506795;
          } else {
            sum += (float)0.043828867;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.51241505)) {
          if ( (data[15].fvalue < 0.67052698)) {
            if (  (data[4].fvalue < 2.1911378)) {
              sum += (float)0.071250238;
            } else {
              sum += (float)-0.0015813981;
            }
          } else {
            sum += (float)-0.082684755;
          }
        } else {
          sum += (float)0.098514833;
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.029547976)) {
    if ( (data[6].fvalue < 0.14316431)) {
      if (  (data[4].fvalue < 1.2132604)) {
        sum += (float)-0.02459841;
      } else {
        sum += (float)0.04382138;
      }
    } else {
      if (  (data[4].fvalue < 1.8224897)) {
        if ( (data[17].fvalue < 0.10472617)) {
          if ( (data[6].fvalue < 0.24817988)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.08155524;
            }
          } else {
            if ( (data[5].fvalue < 0.015425744)) {
              sum += (float)0.17478696;
            } else {
              sum += (float)0.069646105;
            }
          }
        } else {
          sum += (float)0.24320078;
        }
      } else {
        if (  (data[4].fvalue < 2.1954947)) {
          sum += (float)-0.042582177;
        } else {
          sum += (float)0.08793094;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[13].fvalue < 2.2345338)) {
        if ( (data[13].fvalue < 0.21616919)) {
          if ( (data[15].fvalue < 2.6336126)) {
            if ( (data[17].fvalue < 2.3641171)) {
              sum += (float)-0.0088246828;
            } else {
              sum += (float)-0.1683006;
            }
          } else {
            if ( (data[14].fvalue < 0.42111039)) {
              sum += (float)-0.24047107;
            } else {
              sum += (float)0.019711072;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.95353675)) {
            if ( (data[16].fvalue < 0.97249365)) {
              sum += (float)0.0016626859;
            } else {
              sum += (float)0.11442213;
            }
          } else {
            if (  (data[4].fvalue < 1.8126085)) {
              sum += (float)-0.013251607;
            } else {
              sum += (float)0.096628852;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.060019162)) {
          if ( (data[4].fvalue < 1.9213996)) {
            sum += (float)-0.017744573;
          } else {
            sum += (float)-0.080404736;
          }
        } else {
          if ( (data[15].fvalue < 0.35815951)) {
            sum += (float)-0.2197741;
          } else {
            sum += (float)-0.082545415;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.28198555)) {
        sum += (float)0.25016567;
      } else {
        if ( (data[7].fvalue < 4.5)) {
          sum += (float)0.17788699;
        } else {
          if (  (data[4].fvalue < 1.7498256)) {
            sum += (float)0.075712085;
          } else {
            sum += (float)-0.048560303;
          }
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.01007759)) {
    if ( (data[4].fvalue < 0.81694788)) {
      if ( (data[4].fvalue < 0.52916634)) {
        sum += (float)0.22507183;
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.53180695)) {
            if (  (data[4].fvalue < 0.74398196)) {
              sum += (float)0.15764396;
            } else {
              sum += (float)0.075380698;
            }
          } else {
            if (  (data[4].fvalue < 0.74750018)) {
              sum += (float)0.051434517;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.4406996)) {
            if ( (data[14].fvalue < 0.1655958)) {
              sum += (float)-0.096821547;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.10504714;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.0353701)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.17516142)) {
              sum += (float)-0.30830237;
            } else {
              sum += (float)0.026625926;
            }
          } else {
            if ( (data[13].fvalue < 2.7249141e-07)) {
              sum += (float)0.35203859;
            } else {
              sum += (float)-0.024792733;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.70356667)) {
            if ( (data[3].fvalue < 0.43036187)) {
              sum += (float)0.33463052;
            } else {
              sum += (float)0.17458141;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.095492654;
            } else {
              sum += (float)0.065053821;
            }
          }
        }
      } else {
        if ( (data[12].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.088269979)) {
            if ( (data[14].fvalue < 0.04563234)) {
              sum += (float)-0.042184785;
            } else {
              sum += (float)0.0018234;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.031429879;
            } else {
              sum += (float)-0.011675022;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7685177)) {
            if ( (data[3].fvalue < 0.99740046)) {
              sum += (float)0.089437075;
            } else {
              sum += (float)-0.13277626;
            }
          } else {
            if (  (data[4].fvalue < 2.3428769)) {
              sum += (float)-0.26089746;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.14059488)) {
      if ( (data[15].fvalue < 0.00038524921)) {
        if ( (data[13].fvalue < 0.48767489)) {
          if ( (data[8].fvalue < 2.5)) {
            sum += (float)-0.18668826;
          } else {
            sum += (float)-0.30341059;
          }
        } else {
          if ( (data[16].fvalue < 0.093964107)) {
            if (  (data[4].fvalue < 1.6063178)) {
              sum += (float)-0.056203302;
            } else {
              sum += (float)-0.13656256;
            }
          } else {
            sum += (float)-0.026530238;
          }
        }
      } else {
        if ( (data[4].fvalue < 1.4201429)) {
          if ( (data[13].fvalue < 0.32048315)) {
            sum += (float)-0.20156653;
          } else {
            sum += (float)-0.049452119;
          }
        } else {
          if ( (data[17].fvalue < 0.18216425)) {
            if ( (data[16].fvalue < 0.050575633)) {
              sum += (float)0.056382034;
            } else {
              sum += (float)-0.030345254;
            }
          } else {
            if (  (data[4].fvalue < 2.2188182)) {
              sum += (float)-0.1210635;
            } else {
              sum += (float)-0.033487793;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.039618496)) {
        if (  (data[4].fvalue < 2.1799016)) {
          if ( (data[17].fvalue < 0.022217896)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.058018055;
            } else {
              sum += (float)0.035283603;
            }
          } else {
            if (  (data[4].fvalue < 1.6999991)) {
              sum += (float)-0.030856637;
            } else {
              sum += (float)-0.1518364;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.95423532)) {
            if ( (data[13].fvalue < 1.5462267)) {
              sum += (float)0.075624168;
            } else {
              sum += (float)-0.037964806;
            }
          } else {
            if ( (data[13].fvalue < 0.25677076)) {
              sum += (float)-0.10776903;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.01753396)) {
          if ( (data[4].fvalue < 1.414768)) {
            sum += (float)-0.062721603;
          } else {
            if (  (data[4].fvalue < 2.3554263)) {
              sum += (float)0.073677443;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.69482672)) {
              sum += (float)-0.01934626;
            } else {
              sum += (float)0.043995559;
            }
          } else {
            sum += (float)-0.10128935;
          }
        }
      }
    }
  }
  if ( (data[3].fvalue < 0.16855915)) {
    if ( (data[15].fvalue < 0.12098745)) {
      if ( (data[17].fvalue < 0.12939884)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[5].fvalue < 0.083180711)) {
              sum += (float)0.050125863;
            } else {
              sum += (float)-0.24769177;
            }
          } else {
            if ( (data[13].fvalue < 5.008427e-07)) {
              sum += (float)0.32882002;
            } else {
              sum += (float)-0.0094747124;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1716285)) {
            sum += (float)0.28573203;
          } else {
            if ( (data[13].fvalue < 0.24216488)) {
              sum += (float)-0.0052579944;
            } else {
              sum += (float)0.11577819;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.02191136)) {
          sum += (float)-0.049065575;
        } else {
          if (  (data[4].fvalue < 1.6350815)) {
            if ( (data[3].fvalue < 0.12718678)) {
              sum += (float)0.25123525;
            } else {
              sum += (float)0.13409163;
            }
          } else {
            sum += (float)0.032746755;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.005888585)) {
        if ( (data[13].fvalue < 0.031793468)) {
          if ( (data[15].fvalue < 0.84223115)) {
            if (  (data[4].fvalue < 1.2576625)) {
              sum += (float)0.027940245;
            } else {
              sum += (float)-0.10955632;
            }
          } else {
            sum += (float)0.064673774;
          }
        } else {
          if (  (data[4].fvalue < 1.6932764)) {
            if ( (data[13].fvalue < 0.1379602)) {
              sum += (float)0.060726147;
            } else {
              sum += (float)0.17264104;
            }
          } else {
            sum += (float)0.00096303009;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7462925)) {
          if ( (data[15].fvalue < 0.17757058)) {
            sum += (float)0.083974928;
          } else {
            if ( (data[3].fvalue < 0.14204983)) {
              sum += (float)0.20945877;
            } else {
              sum += (float)0.10907296;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.24649256)) {
            sum += (float)-0.079256698;
          } else {
            sum += (float)0.010278074;
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 1.2481232)) {
      if ( (data[14].fvalue < 0.3585909)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.04901775)) {
            if ( (data[13].fvalue < 1.0637999e-05)) {
              sum += (float)0.16972066;
            } else {
              sum += (float)0.0037665765;
            }
          } else {
            if ( (data[15].fvalue < 0.1240731)) {
              sum += (float)-0.081323124;
            } else {
              sum += (float)-0.01220897;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.090391569)) {
            if ( (data[15].fvalue < 0.15236969)) {
              sum += (float)-0.1312356;
            } else {
              sum += (float)-0.0044878605;
            }
          } else {
            if ( (data[17].fvalue < 0.44290763)) {
              sum += (float)-0.027788581;
            } else {
              sum += (float)0.0722716;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.62395716)) {
          if ( (data[4].fvalue < 1.4568111)) {
            if ( (data[3].fvalue < 1.4156749)) {
              sum += (float)0.072618365;
            } else {
              sum += (float)-0.0034889614;
            }
          } else {
            if ( (data[13].fvalue < 0.58738154)) {
              sum += (float)-0.041829877;
            } else {
              sum += (float)0.089324065;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.5729208)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.1051453;
            } else {
              sum += (float)0.007394081;
            }
          } else {
            if ( (data[14].fvalue < 0.6646415)) {
              sum += (float)-0.070935287;
            } else {
              sum += (float)0.0080790864;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[14].fvalue < 1.1724632e-05)) {
          if ( (data[13].fvalue < 0.06141936)) {
            if ( (data[3].fvalue < 0.22915444)) {
              sum += (float)0.084671773;
            } else {
              sum += (float)0.2230038;
            }
          } else {
            if ( (data[3].fvalue < 0.30916309)) {
              sum += (float)-0.27773833;
            } else {
              sum += (float)0.050811041;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.037815563)) {
            if ( (data[4].fvalue < 0.84978104)) {
              sum += (float)-0.018878112;
            } else {
              sum += (float)-0.11411442;
            }
          } else {
            if ( (data[5].fvalue < 1.5952101)) {
              sum += (float)-0.0096587967;
            } else {
              sum += (float)0.071465187;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.7678622)) {
          sum += (float)0.30560553;
        } else {
          sum += (float)0.10598348;
        }
      }
    }
  }
  if ( (data[14].fvalue < 1.6204721)) {
    if ( (data[5].fvalue < 0.59997064)) {
      if ( (data[7].fvalue < 5.5)) {
        if ( (data[13].fvalue < 0.94670588)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.16894156)) {
              sum += (float)-0.0014396319;
            } else {
              sum += (float)0.032173429;
            }
          } else {
            if (  (data[4].fvalue < 1.1126685)) {
              sum += (float)0.17770787;
            } else {
              sum += (float)-0.028783094;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.52686268)) {
            if ( (data[16].fvalue < 0.7500366)) {
              sum += (float)-0.06828244;
            } else {
              sum += (float)0.15684468;
            }
          } else {
            if ( (data[15].fvalue < 0.021987017)) {
              sum += (float)0.11207823;
            } else {
              sum += (float)-0.0014805037;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.37087101)) {
            sum += (float)-0.038194872;
          } else {
            if ( (data[14].fvalue < 0.029940009)) {
              sum += (float)0.086017169;
            } else {
              sum += (float)0.20697261;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3002653)) {
            if ( (data[3].fvalue < 0.3256048)) {
              sum += (float)0.22381222;
            } else {
              sum += (float)0.052194756;
            }
          } else {
            if ( (data[7].fvalue < 10.5)) {
              sum += (float)-0.010206027;
            } else {
              sum += (float)0.14381912;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.006604759)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[14].fvalue < 1.4579578e-08)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.0061944677;
            } else {
              sum += (float)0.39029989;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0056867241;
            } else {
              sum += (float)-0.078909151;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.7618832)) {
            if (  (data[4].fvalue < 1.0187548)) {
              sum += (float)0.26271316;
            } else {
              sum += (float)0.10360629;
            }
          } else {
            if ( (data[14].fvalue < 0.16666436)) {
              sum += (float)-0.16222623;
            } else {
              sum += (float)-0.0010092908;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.011216968)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.12002311;
            } else {
              sum += (float)0.056011137;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.044006422;
            } else {
              sum += (float)-0.090801351;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.67465973)) {
            sum += (float)-0.061614741;
          } else {
            sum += (float)-0.21207722;
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.6040318)) {
      if ( (data[13].fvalue < 0.31733248)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.0582214)) {
            sum += (float)0.026600525;
          } else {
            if ( (data[15].fvalue < 0.32525861)) {
              sum += (float)-0.16857478;
            } else {
              sum += (float)-0.056256074;
            }
          }
        } else {
          sum += (float)-0.19850506;
        }
      } else {
        sum += (float)-0;
      }
    } else {
      if ( (data[4].fvalue < 2.3004572)) {
        sum += (float)0.060577385;
      } else {
        sum += (float)-0.013654893;
      }
    }
  }
  if ( (data[17].fvalue < 0.035209775)) {
    if ( (data[14].fvalue < 0.2500914)) {
      if ( (data[15].fvalue < 2.0967164)) {
        if ( (data[7].fvalue < 7.5)) {
          if ( (data[14].fvalue < 7.0669245e-08)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.0061534909;
            } else {
              sum += (float)0.40440574;
            }
          } else {
            if ( (data[14].fvalue < 0.010241725)) {
              sum += (float)-0.11973684;
            } else {
              sum += (float)-0.001357829;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.76961714)) {
            sum += (float)0.16665445;
          } else {
            sum += (float)0.055695135;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.027968636)) {
          if ( (data[5].fvalue < 0.19066933)) {
            sum += (float)-0.069917247;
          } else {
            sum += (float)-0.26846102;
          }
        } else {
          if ( (data[13].fvalue < 0.34846574)) {
            sum += (float)-0.062121153;
          } else {
            sum += (float)0.022894127;
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.3995676)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.011395596)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0038369021;
            } else {
              sum += (float)0.068813905;
            }
          } else {
            if ( (data[14].fvalue < 0.5420351)) {
              sum += (float)0.033034217;
            } else {
              sum += (float)0.099676237;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.11851266)) {
            if ( (data[6].fvalue < 0.21305442)) {
              sum += (float)0.016069742;
            } else {
              sum += (float)-0.15541388;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.048857938;
            } else {
              sum += (float)0.053334247;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.0076897871)) {
          if ( (data[14].fvalue < 0.38550812)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.085711956;
            }
          } else {
            if ( (data[13].fvalue < 0.5528152)) {
              sum += (float)0.079065368;
            } else {
              sum += (float)0.19644681;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.10545056)) {
            if ( (data[3].fvalue < 1.274003)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.10132796;
            }
          } else {
            if ( (data[14].fvalue < 0.35920918)) {
              sum += (float)-0;
            } else {
              sum += (float)0.08116053;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.13684574)) {
      if ( (data[15].fvalue < 1.0504427)) {
        if ( (data[3].fvalue < 0.084663779)) {
          sum += (float)0.045436013;
        } else {
          if ( (data[13].fvalue < 0.049745306)) {
            if ( (data[15].fvalue < 0.0013194871)) {
              sum += (float)-0.21649788;
            } else {
              sum += (float)-0.080828443;
            }
          } else {
            if ( (data[13].fvalue < 0.60360301)) {
              sum += (float)-0.04827464;
            } else {
              sum += (float)-0.11526307;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.075827092)) {
          sum += (float)0.089099318;
        } else {
          sum += (float)-0.0011307088;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.0090451259)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.11220863)) {
            if ( (data[17].fvalue < 0.65142131)) {
              sum += (float)-0.024706971;
            } else {
              sum += (float)-0.23840554;
            }
          } else {
            if ( (data[3].fvalue < 0.69925189)) {
              sum += (float)0.0052233059;
            } else {
              sum += (float)-0.060886174;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.057200581)) {
              sum += (float)0.10044699;
            } else {
              sum += (float)-0.002180923;
            }
          } else {
            sum += (float)-0.12490737;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.8435694)) {
          if ( (data[16].fvalue < 0.0039521586)) {
            if ( (data[17].fvalue < 0.49676892)) {
              sum += (float)0.020691942;
            } else {
              sum += (float)0.064848408;
            }
          } else {
            if ( (data[15].fvalue < 0.00034565511)) {
              sum += (float)-0.090687081;
            } else {
              sum += (float)-0.013044716;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.84331203)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.13508196;
            } else {
              sum += (float)-0.017049164;
            }
          } else {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.12271412;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  }
  if ( (data[1].fvalue < 0.39052182)) {
    if ( (data[3].fvalue < 0.30406401)) {
      if (  (data[4].fvalue < 1.2322239)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[13].fvalue < 0.11060265)) {
            sum += (float)0.0407793;
          } else {
            sum += (float)-0.041268863;
          }
        } else {
          sum += (float)0.052097339;
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[3].fvalue < 0.14323559)) {
            sum += (float)0.1598075;
          } else {
            if ( (data[13].fvalue < 0.12016381)) {
              sum += (float)0.025509637;
            } else {
              sum += (float)0.097327955;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.19314504)) {
            sum += (float)0.066376761;
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.2058992)) {
        if ( (data[17].fvalue < 0.049904164)) {
          if ( (data[5].fvalue < 0.41605002)) {
            if ( (data[16].fvalue < 0.029000692)) {
              sum += (float)0.046262294;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.12683991)) {
              sum += (float)-0.055698454;
            } else {
              sum += (float)0.01727757;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.38009098)) {
            if (  (data[4].fvalue < 1.4588132)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.064575888;
            }
          } else {
            if ( (data[13].fvalue < 0.38822937)) {
              sum += (float)-0.013261466;
            } else {
              sum += (float)0.046976123;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 1.8594103)) {
          if ( (data[15].fvalue < 0.58589172)) {
            if ( (data[17].fvalue < 0.29322171)) {
              sum += (float)0.13433328;
            } else {
              sum += (float)0.058066819;
            }
          } else {
            if ( (data[16].fvalue < 0.031945944)) {
              sum += (float)0.080036104;
            } else {
              sum += (float)-0.035566863;
            }
          }
        } else {
          sum += (float)-0.030146372;
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.053855535)) {
      if (  (data[4].fvalue < 1.0369875)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[14].fvalue < 3.4297068e-07)) {
            if ( (data[13].fvalue < 0.055467218)) {
              sum += (float)0.12870838;
            } else {
              sum += (float)-0.025312269;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.071357556;
            } else {
              sum += (float)0.049154334;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.4164174)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.083675034;
            } else {
              sum += (float)0.23791201;
            }
          } else {
            sum += (float)-0.094652981;
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[15].fvalue < 0.046373345)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.032004125;
            } else {
              sum += (float)0.1215737;
            }
          } else {
            if ( (data[5].fvalue < 0.37468684)) {
              sum += (float)0.017212233;
            } else {
              sum += (float)-0.026338974;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.337512)) {
              sum += (float)0.028543904;
            } else {
              sum += (float)0.11018632;
            }
          } else {
            if ( (data[14].fvalue < 0.10435367)) {
              sum += (float)-0.093127221;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.1179754)) {
        if ( (data[15].fvalue < 1.1176293)) {
          if ( (data[13].fvalue < 0.046684753)) {
            if ( (data[6].fvalue < 0.5417015)) {
              sum += (float)-0.15491782;
            } else {
              sum += (float)-0.060411859;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.040782206;
            } else {
              sum += (float)-0.10617083;
            }
          }
        } else {
          sum += (float)0.024795171;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.0019262617)) {
            if (  (data[4].fvalue < 2.0756454)) {
              sum += (float)-0.060211584;
            } else {
              sum += (float)0.00889224;
            }
          } else {
            if ( (data[13].fvalue < 0.0061751856)) {
              sum += (float)-0.029950213;
            } else {
              sum += (float)0.036538899;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.43740189)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.040514484;
            } else {
              sum += (float)0.15926489;
            }
          } else {
            if ( (data[17].fvalue < 0.89801532)) {
              sum += (float)-0.009963233;
            } else {
              sum += (float)0.046306103;
            }
          }
        }
      }
    }
  }
  if ( (data[8].fvalue < 11.5)) {
    if ( (data[15].fvalue < 2.4109726)) {
      if ( (data[9].fvalue < 2.5)) {
        if ( (data[16].fvalue < 0.004037777)) {
          if ( (data[12].fvalue < 0.5)) {
            if (  (data[4].fvalue < 2.0626426)) {
              sum += (float)0.0014416168;
            } else {
              sum += (float)0.04101846;
            }
          } else {
            if (  (data[4].fvalue < 1.7684019)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.1668274;
            }
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.22657159;
            } else {
              sum += (float)-0.040508017;
            }
          } else {
            if ( (data[15].fvalue < 0.019083697)) {
              sum += (float)0.13898739;
            } else {
              sum += (float)-0.012230563;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.77820379)) {
          sum += (float)0.18554747;
        } else {
          sum += (float)0.035042096;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.60822892)) {
        if ( (data[13].fvalue < 0.36460936)) {
          if ( (data[11].fvalue < 0.5)) {
            if ( (data[5].fvalue < 0.25332531)) {
              sum += (float)-0.11081053;
            } else {
              sum += (float)-0.22678123;
            }
          } else {
            sum += (float)-0.074264422;
          }
        } else {
          sum += (float)0.032727953;
        }
      } else {
        sum += (float)0.090842612;
      }
    }
  } else {
    sum += (float)0.17980477;
  }
  if ( (data[4].fvalue < 0.78630865)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[4].fvalue < 0.62880588)) {
        sum += (float)0.18012522;
      } else {
        if ( (data[14].fvalue < 0.081751995)) {
          if ( (data[13].fvalue < 0.60595095)) {
            if ( (data[13].fvalue < 0.25865358)) {
              sum += (float)0.15038535;
            } else {
              sum += (float)0.071155019;
            }
          } else {
            sum += (float)0.0077619543;
          }
        } else {
          sum += (float)0.012204872;
        }
      }
    } else {
      if ( (data[5].fvalue < 1.3983688)) {
        sum += (float)-0.063481025;
      } else {
        sum += (float)0.035734925;
      }
    }
  } else {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[8].fvalue < 6.5)) {
        if ( (data[13].fvalue < 1.9479284)) {
          if (  (data[4].fvalue < 2.0279577)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.0069554434;
            } else {
              sum += (float)0.04298706;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.029878281;
            } else {
              sum += (float)-0.038021609;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.48983157)) {
            if ( (data[15].fvalue < 0.060073264)) {
              sum += (float)-0.048038572;
            } else {
              sum += (float)-0.17198785;
            }
          } else {
            sum += (float)0.039638162;
          }
        }
      } else {
        if ( (data[7].fvalue < 2.5)) {
          sum += (float)0.1821923;
        } else {
          if ( (data[14].fvalue < 0.55666852)) {
            sum += (float)-0;
          } else {
            sum += (float)0.094795473;
          }
        }
      }
    } else {
      if ( (data[10].fvalue < 1.5)) {
        if ( (data[3].fvalue < 0.36931181)) {
          if (  (data[4].fvalue < 1.1563579)) {
            sum += (float)0.24194458;
          } else {
            if (  (data[4].fvalue < 1.8850641)) {
              sum += (float)0.067615561;
            } else {
              sum += (float)-0.088813655;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.68864965)) {
              sum += (float)-0.0041110213;
            } else {
              sum += (float)0.12730643;
            }
          } else {
            if ( (data[6].fvalue < 0.2574257)) {
              sum += (float)0.092373304;
            } else {
              sum += (float)-0.031991363;
            }
          }
        }
      } else {
        sum += (float)-0.15180264;
      }
    }
  }
  if ( (data[14].fvalue < 0.028650016)) {
    if ( (data[8].fvalue < 1.5)) {
      if ( (data[15].fvalue < 0.027171947)) {
        if ( (data[13].fvalue < 4.6981097e-07)) {
          if ( (data[6].fvalue < 0.40692043)) {
            sum += (float)-0.031249262;
          } else {
            if ( (data[3].fvalue < 0.97718126)) {
              sum += (float)0.18517832;
            } else {
              sum += (float)0.32605803;
            }
          }
        } else {
          if ( (data[14].fvalue < 3.4297068e-07)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.004600727;
            } else {
              sum += (float)0.36552551;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.026208648;
            } else {
              sum += (float)-0.10133342;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.0019214298)) {
            if (  (data[4].fvalue < 2.0751152)) {
              sum += (float)-0.063907817;
            } else {
              sum += (float)0.030659713;
            }
          } else {
            if ( (data[13].fvalue < 0.022472396)) {
              sum += (float)-0.1856802;
            } else {
              sum += (float)0.17908058;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[17].fvalue < 0.02359754)) {
              sum += (float)0.083391786;
            } else {
              sum += (float)-0.008589494;
            }
          } else {
            if ( (data[14].fvalue < 0.0067330077)) {
              sum += (float)-0.2030281;
            } else {
              sum += (float)-0.039250493;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.019890368)) {
            sum += (float)-0.25148934;
          } else {
            sum += (float)-0.14309624;
          }
        } else {
          if ( (data[13].fvalue < 0.20239756)) {
            sum += (float)0.096016809;
          } else {
            sum += (float)-0.18470052;
          }
        }
      } else {
        if ( (data[3].fvalue < 0.18990543)) {
          sum += (float)0.20860982;
        } else {
          sum += (float)-0.061991557;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.04256136)) {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.6318612)) {
              sum += (float)-0.3986986;
            } else {
              sum += (float)-0.11503716;
            }
          } else {
            if ( (data[13].fvalue < 0.00019908903)) {
              sum += (float)0.27880776;
            } else {
              sum += (float)-0.055534057;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)0.16168194;
          } else {
            if ( (data[6].fvalue < 0.073860988)) {
              sum += (float)0.12485179;
            } else {
              sum += (float)-0.12300822;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.16242681)) {
          if ( (data[14].fvalue < 0.05457449)) {
            sum += (float)-0;
          } else {
            if (  (data[4].fvalue < 1.6453435)) {
              sum += (float)0.15500244;
            } else {
              sum += (float)0.037569024;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[4].fvalue < 1.3987615)) {
              sum += (float)0.028396964;
            } else {
              sum += (float)-0.029188043;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.10583519;
            } else {
              sum += (float)-0.091567501;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.053091254)) {
          if (  (data[4].fvalue < 1.050154)) {
            if ( (data[3].fvalue < 0.37196225)) {
              sum += (float)-0.058919702;
            } else {
              sum += (float)0.097873278;
            }
          } else {
            if ( (data[16].fvalue < 0.030534519)) {
              sum += (float)0.17916436;
            } else {
              sum += (float)0.0014801724;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.7943754)) {
            if ( (data[15].fvalue < 0.00065132848)) {
              sum += (float)0.021802485;
            } else {
              sum += (float)0.077030197;
            }
          } else {
            if ( (data[15].fvalue < 0.022415604)) {
              sum += (float)0.056608405;
            } else {
              sum += (float)-0.024789741;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.08945974)) {
          if ( (data[0].fvalue < 3.9113958)) {
            if ( (data[0].fvalue < 3.1815131)) {
              sum += (float)0.30872935;
            } else {
              sum += (float)0.16095513;
            }
          } else {
            if ( (data[0].fvalue < 4.6778984)) {
              sum += (float)0.048180729;
            } else {
              sum += (float)-0.033533737;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.6313163)) {
            if (  (data[4].fvalue < 1.1147444)) {
              sum += (float)0.16826417;
            } else {
              sum += (float)0.0022506437;
            }
          } else {
            if ( (data[3].fvalue < 0.74093181)) {
              sum += (float)0.066839524;
            } else {
              sum += (float)-0.062136896;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.011457656)) {
    sum += (float)-0.1695094;
  } else {
    if ( (data[5].fvalue < 0.0090336977)) {
      if ( (data[6].fvalue < 0.16399422)) {
        sum += (float)-0;
      } else {
        sum += (float)0.12882572;
      }
    } else {
      if ( (data[13].fvalue < 0.2537598)) {
        if ( (data[17].fvalue < 1.7058003)) {
          if ( (data[8].fvalue < 6.5)) {
            if ( (data[15].fvalue < 2.0380652)) {
              sum += (float)-0.0044012964;
            } else {
              sum += (float)-0.070641451;
            }
          } else {
            if ( (data[14].fvalue < 0.42037845)) {
              sum += (float)0.13021284;
            } else {
              sum += (float)0.018766612;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.3475008)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.24899182;
            } else {
              sum += (float)-0.031774402;
            }
          } else {
            sum += (float)0.12578957;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.47634947)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 5.5)) {
              sum += (float)0.0071709151;
            } else {
              sum += (float)0.10234708;
            }
          } else {
            if ( (data[14].fvalue < 0.19409341)) {
              sum += (float)-0.1036612;
            } else {
              sum += (float)0.006008036;
            }
          }
        } else {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.68195057)) {
              sum += (float)0.037598889;
            } else {
              sum += (float)0.13519968;
            }
          } else {
            if ( (data[3].fvalue < 1.7475181)) {
              sum += (float)-0.0039281775;
            } else {
              sum += (float)-0.15268455;
            }
          }
        }
      }
    }
  }
  if ( (data[15].fvalue < 0.21184772)) {
    if ( (data[15].fvalue < 0.048228491)) {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[16].fvalue < 0.002712111)) {
          if ( (data[13].fvalue < 0.53257501)) {
            if ( (data[17].fvalue < 0.89753282)) {
              sum += (float)-0.0023511767;
            } else {
              sum += (float)-0.072547831;
            }
          } else {
            if ( (data[17].fvalue < 0.90004754)) {
              sum += (float)0.010581984;
            } else {
              sum += (float)0.12935434;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.976136)) {
              sum += (float)-0.29065621;
            } else {
              sum += (float)-0.023349399;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.047002979;
            } else {
              sum += (float)-0.074652903;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.015210178)) {
          if ( (data[5].fvalue < 0.59909481)) {
            if ( (data[15].fvalue < 0.0071808705)) {
              sum += (float)0.28295794;
            } else {
              sum += (float)0.16246773;
            }
          } else {
            sum += (float)0.052955538;
          }
        } else {
          if ( (data[5].fvalue < 0.42981815)) {
            if ( (data[14].fvalue < 0.0025994463)) {
              sum += (float)-0;
            } else {
              sum += (float)0.090781271;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.0011429701;
            } else {
              sum += (float)-0.14249571;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.87778127)) {
        if ( (data[15].fvalue < 0.12424856)) {
          if ( (data[5].fvalue < 0.2797156)) {
            if ( (data[16].fvalue < 0.016918799)) {
              sum += (float)-0.031814929;
            } else {
              sum += (float)0.03727334;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.056943484;
            } else {
              sum += (float)-0.12030648;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.4221492)) {
            if ( (data[5].fvalue < 0.39379394)) {
              sum += (float)0.066557728;
            } else {
              sum += (float)0.0018525209;
            }
          } else {
            if ( (data[16].fvalue < 0.0077891033)) {
              sum += (float)-0.06221633;
            } else {
              sum += (float)0.012945367;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.5505139)) {
          sum += (float)-0.034703679;
        } else {
          if ( (data[4].fvalue < 2.3315139)) {
            if ( (data[16].fvalue < 0.008935241)) {
              sum += (float)-0.20058081;
            } else {
              sum += (float)-0.1142045;
            }
          } else {
            sum += (float)-0.020946182;
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.44601661)) {
      if ( (data[14].fvalue < 0.004138276)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.13716868)) {
            if ( (data[5].fvalue < 0.13930669)) {
              sum += (float)0.0014330014;
            } else {
              sum += (float)-0.13847549;
            }
          } else {
            if ( (data[3].fvalue < 0.59669161)) {
              sum += (float)0.052591205;
            } else {
              sum += (float)-0.032312546;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.23444486)) {
            sum += (float)0.053096738;
          } else {
            sum += (float)0.1156889;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.008037556)) {
          if ( (data[14].fvalue < 0.075268157)) {
            sum += (float)-0.074752569;
          } else {
            if (  (data[4].fvalue < 1.5126824)) {
              sum += (float)0.065553635;
            } else {
              sum += (float)-0.00348022;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7491808)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.091134846;
            } else {
              sum += (float)0.17432176;
            }
          } else {
            if ( (data[15].fvalue < 0.47914061)) {
              sum += (float)-0.043858737;
            } else {
              sum += (float)0.037244279;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.34004313)) {
        if ( (data[13].fvalue < 0.055637378)) {
          if ( (data[15].fvalue < 1.7295465)) {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.028972019;
            } else {
              sum += (float)0.091959625;
            }
          } else {
            if ( (data[3].fvalue < 1.1134338)) {
              sum += (float)-0.060391307;
            } else {
              sum += (float)-0.17644337;
            }
          }
        } else {
          if ( (data[0].fvalue < 1.5702125)) {
            if ( (data[5].fvalue < 0.27720726)) {
              sum += (float)0.13382602;
            } else {
              sum += (float)0.017890511;
            }
          } else {
            if ( (data[15].fvalue < 0.5448209)) {
              sum += (float)-0.027290151;
            } else {
              sum += (float)0.020536205;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 1.092521)) {
          if ( (data[0].fvalue < 2.1313715)) {
            if ( (data[14].fvalue < 0.62143213)) {
              sum += (float)0.089565128;
            } else {
              sum += (float)0.010935368;
            }
          } else {
            if ( (data[17].fvalue < 0.56367475)) {
              sum += (float)-0.013161066;
            } else {
              sum += (float)0.045440789;
            }
          }
        } else {
          if ( (data[4].fvalue < 2.4925146)) {
            if (  (data[4].fvalue < 1.6571543)) {
              sum += (float)-0.041818429;
            } else {
              sum += (float)0.11185846;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.03211439;
            } else {
              sum += (float)-0.058907244;
            }
          }
        }
      }
    }
  }
  if ( (data[0].fvalue < 2.579432)) {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[5].fvalue < 0.059723958)) {
        if (  (data[4].fvalue < 1.8162481)) {
          if ( (data[6].fvalue < 0.64803952)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.051889118;
            } else {
              sum += (float)-0.002072444;
            }
          } else {
            sum += (float)0.15455271;
          }
        } else {
          sum += (float)-0.017054096;
        }
      } else {
        if ( (data[13].fvalue < 0.013666493)) {
          if ( (data[13].fvalue < 1.0870286e-07)) {
            if ( (data[17].fvalue < 0.025939085)) {
              sum += (float)0.0036974263;
            } else {
              sum += (float)-0.054969426;
            }
          } else {
            if ( (data[14].fvalue < 1.5838509e-06)) {
              sum += (float)0.056569505;
            } else {
              sum += (float)-0.10493854;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.026919466)) {
            sum += (float)-0.15539163;
          } else {
            if ( (data[14].fvalue < 0.018597666)) {
              sum += (float)0.0016435751;
            } else {
              sum += (float)0.020723762;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.29232615)) {
        if (  (data[4].fvalue < 1.0982263)) {
          if ( (data[13].fvalue < 0.064773485)) {
            sum += (float)0.080092475;
          } else {
            sum += (float)0.26174462;
          }
        } else {
          if ( (data[13].fvalue < 0.2153962)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.084128007;
            }
          } else {
            sum += (float)0.17409529;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.20068684)) {
          if ( (data[15].fvalue < 0.22968352)) {
            if ( (data[14].fvalue < 0.46221155)) {
              sum += (float)-0.057970423;
            } else {
              sum += (float)0.056920495;
            }
          } else {
            if ( (data[6].fvalue < 0.59512943)) {
              sum += (float)0.20144458;
            } else {
              sum += (float)0.047147032;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.69314361)) {
            sum += (float)0.20436311;
          } else {
            sum += (float)0.075353414;
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if (  (data[4].fvalue < 1.987231)) {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[15].fvalue < 1.4838555)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.0028799586;
            } else {
              sum += (float)-0.030499676;
            }
          } else {
            if ( (data[14].fvalue < 0.51596951)) {
              sum += (float)-0.13988489;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.52571464)) {
            if ( (data[16].fvalue < 0.0049622292)) {
              sum += (float)-0.014366926;
            } else {
              sum += (float)0.043694984;
            }
          } else {
            if ( (data[5].fvalue < 0.25295925)) {
              sum += (float)-0;
            } else {
              sum += (float)0.11927991;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 1.488614)) {
          if ( (data[13].fvalue < 0.41266054)) {
            if ( (data[17].fvalue < 2.5328646)) {
              sum += (float)0.014096497;
            } else {
              sum += (float)-0.13221806;
            }
          } else {
            if ( (data[17].fvalue < 1.4095225)) {
              sum += (float)0.042243198;
            } else {
              sum += (float)0.14356714;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.41936278)) {
            if ( (data[17].fvalue < 0.50106382)) {
              sum += (float)-0.10059716;
            } else {
              sum += (float)0.029436771;
            }
          } else {
            sum += (float)0.085553609;
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.053307056)) {
        sum += (float)0.20884329;
      } else {
        if ( (data[14].fvalue < 0.24858603)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[15].fvalue < 0.14348765)) {
              sum += (float)-0.1525725;
            } else {
              sum += (float)-0.040037133;
            }
          } else {
            if ( (data[13].fvalue < 0.6172123)) {
              sum += (float)0.032602042;
            } else {
              sum += (float)-0.096807189;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.1693055)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.023956914;
            } else {
              sum += (float)-0.13315217;
            }
          } else {
            if ( (data[6].fvalue < 0.26620528)) {
              sum += (float)0.10808863;
            } else {
              sum += (float)-0.00092837558;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 10.5)) {
    if ( (data[0].fvalue < 1.8067851)) {
      if ( (data[8].fvalue < 4.5)) {
        if ( (data[14].fvalue < 0.11590461)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[5].fvalue < 0.24146423)) {
              sum += (float)-0.050987773;
            } else {
              sum += (float)-0.20564486;
            }
          } else {
            if (  (data[4].fvalue < 1.3000716)) {
              sum += (float)0.0020964355;
            } else {
              sum += (float)0.028473517;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.52376688)) {
            if (  (data[4].fvalue < 1.1566215)) {
              sum += (float)0.099463694;
            } else {
              sum += (float)0.034523036;
            }
          } else {
            if ( (data[13].fvalue < 0.39338237)) {
              sum += (float)-0.016004384;
            } else {
              sum += (float)0.099519156;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.53456998)) {
          if ( (data[13].fvalue < 0.18996859)) {
            sum += (float)0.083732478;
          } else {
            sum += (float)0.19508302;
          }
        } else {
          if ( (data[0].fvalue < 1.2754171)) {
            sum += (float)0.076340891;
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.39225191)) {
          if ( (data[5].fvalue < 0.27042168)) {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)0.0042601093;
            } else {
              sum += (float)0.066750497;
            }
          } else {
            if ( (data[4].fvalue < 1.1743996)) {
              sum += (float)-0.002734185;
            } else {
              sum += (float)-0.024616906;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.62457931)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.010688573;
            } else {
              sum += (float)-0.053235177;
            }
          } else {
            if ( (data[16].fvalue < 0.010880094)) {
              sum += (float)0.10615408;
            } else {
              sum += (float)0.00041606865;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.6874707)) {
          if ( (data[13].fvalue < 0.42022637)) {
            sum += (float)0.094766036;
          } else {
            sum += (float)-0.080791757;
          }
        } else {
          if (  (data[4].fvalue < 2.1983714)) {
            if (  (data[4].fvalue < 1.8654323)) {
              sum += (float)-0.080761887;
            } else {
              sum += (float)-0.28974357;
            }
          } else {
            if ( (data[17].fvalue < 0.040831961)) {
              sum += (float)-0.053788248;
            } else {
              sum += (float)0.054824423;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.1297463;
  }
  if ( (data[5].fvalue < 1.0772794)) {
    if (  (data[4].fvalue < 1.0266764)) {
      if ( (data[7].fvalue < 2.5)) {
        if ( (data[14].fvalue < 1.9908774e-05)) {
          if ( (data[13].fvalue < 0.035144877)) {
            if ( (data[6].fvalue < 0.01439989)) {
              sum += (float)-0.035684332;
            } else {
              sum += (float)0.21170656;
            }
          } else {
            if ( (data[3].fvalue < 0.11893064)) {
              sum += (float)-0.14209525;
            } else {
              sum += (float)-0.020012051;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.053468518)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.11947078;
            } else {
              sum += (float)0.021447908;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.014028446;
            } else {
              sum += (float)-0.056821365;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.29436547)) {
          if ( (data[14].fvalue < 0.0070114126)) {
            sum += (float)0.037500579;
          } else {
            sum += (float)0.25171036;
          }
        } else {
          if ( (data[13].fvalue < 0.14078036)) {
            sum += (float)-0.16323312;
          } else {
            if ( (data[3].fvalue < 1.0850377)) {
              sum += (float)0.10442871;
            } else {
              sum += (float)-0.07422363;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.33630389)) {
        if ( (data[13].fvalue < 0.052499779)) {
          if ( (data[14].fvalue < 0.045023195)) {
            if ( (data[5].fvalue < 0.1316905)) {
              sum += (float)-0.025187548;
            } else {
              sum += (float)-0.13550584;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.058762211;
            } else {
              sum += (float)0.04495145;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.14436188)) {
            if (  (data[4].fvalue < 1.1620173)) {
              sum += (float)0.036284849;
            } else {
              sum += (float)0.0010446209;
            }
          } else {
            if ( (data[0].fvalue < 3.4046602)) {
              sum += (float)0.11516508;
            } else {
              sum += (float)0.025524439;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.37240794)) {
          if ( (data[15].fvalue < 0.034069486)) {
            if ( (data[13].fvalue < 1.1268922e-05)) {
              sum += (float)0.10871384;
            } else {
              sum += (float)-0.0046979855;
            }
          } else {
            if ( (data[0].fvalue < 1.8529389)) {
              sum += (float)0.0192833;
            } else {
              sum += (float)-0.038442533;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.42702079)) {
            if ( (data[13].fvalue < 0.30490139)) {
              sum += (float)-0.020154515;
            } else {
              sum += (float)0.023134105;
            }
          } else {
            if ( (data[17].fvalue < 0.013091799)) {
              sum += (float)0.047700863;
            } else {
              sum += (float)-0.0032574588;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 3.5)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[3].fvalue < 0.34217733)) {
          if ( (data[13].fvalue < 0.08605817)) {
            if ( (data[3].fvalue < 0.21623656)) {
              sum += (float)-0.052473787;
            } else {
              sum += (float)0.086037681;
            }
          } else {
            sum += (float)-0.21547824;
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.15408607)) {
              sum += (float)0.15047297;
            } else {
              sum += (float)0.0186846;
            }
          } else {
            sum += (float)-0.10710102;
          }
        }
      } else {
        if ( (data[3].fvalue < 0.2478683)) {
          sum += (float)0.14279251;
        } else {
          if ( (data[13].fvalue < 0.055207327)) {
            sum += (float)-0.1307172;
          } else {
            if ( (data[3].fvalue < 0.5344485)) {
              sum += (float)0.056917425;
            } else {
              sum += (float)-0.035440989;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.0478539)) {
        sum += (float)0.26241902;
      } else {
        sum += (float)-0.020401774;
      }
    }
  }
  if ( (data[15].fvalue < 0.17393233)) {
    if ( (data[15].fvalue < 0.036246955)) {
      if ( (data[9].fvalue < 0.5)) {
        if ( (data[14].fvalue < 3.4297068e-07)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.026265875)) {
              sum += (float)0.10508365;
            } else {
              sum += (float)-0.0016518451;
            }
          } else {
            sum += (float)0.33672702;
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.25322995;
            } else {
              sum += (float)0.11786942;
            }
          } else {
            if ( (data[13].fvalue < 5.008427e-07)) {
              sum += (float)0.31876636;
            } else {
              sum += (float)-0.010960296;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.0050654113)) {
          sum += (float)0.23383643;
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.008097766)) {
              sum += (float)-0.0044149747;
            } else {
              sum += (float)0.12380068;
            }
          } else {
            if ( (data[5].fvalue < 0.30325133)) {
              sum += (float)0.0084047988;
            } else {
              sum += (float)-0.086454682;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.15198523)) {
        if ( (data[16].fvalue < 0.0071868664)) {
          if (  (data[4].fvalue < 1.4261246)) {
            if ( (data[13].fvalue < 0.047740042)) {
              sum += (float)-0.039361522;
            } else {
              sum += (float)0.077010378;
            }
          } else {
            if (  (data[4].fvalue < 2.1781683)) {
              sum += (float)-0.079689093;
            } else {
              sum += (float)0.053592898;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7175496)) {
            sum += (float)0.15163043;
          } else {
            if ( (data[1].fvalue < 1.0870346)) {
              sum += (float)0.049208272;
            } else {
              sum += (float)-0.029116863;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 1.1772079)) {
          if ( (data[6].fvalue < 0.38214678)) {
            if ( (data[13].fvalue < 0.17997102)) {
              sum += (float)-0.047807854;
            } else {
              sum += (float)0.012167228;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.040883455;
            } else {
              sum += (float)-0.10446493;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.18815251)) {
            sum += (float)0.10962701;
          } else {
            sum += (float)-0;
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.31141233)) {
      if (  (data[4].fvalue < 1.7484603)) {
        if ( (data[14].fvalue < 0.0082139354)) {
          if ( (data[13].fvalue < 0.024462834)) {
            if ( (data[15].fvalue < 0.71865189)) {
              sum += (float)-0.059530921;
            } else {
              sum += (float)0.077399783;
            }
          } else {
            if ( (data[6].fvalue < 0.31842411)) {
              sum += (float)0.076073609;
            } else {
              sum += (float)0.027915893;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.2836235)) {
            sum += (float)0.19508657;
          } else {
            if ( (data[15].fvalue < 0.23765728)) {
              sum += (float)0.011097374;
            } else {
              sum += (float)0.10015791;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.58718443)) {
          if ( (data[16].fvalue < 0.22258021)) {
            if ( (data[17].fvalue < 0.37538445)) {
              sum += (float)-0.11076901;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.030274114;
          }
        } else {
          if (  (data[4].fvalue < 1.9499116)) {
            sum += (float)0.071315408;
          } else {
            if ( (data[14].fvalue < 0.0031643175)) {
              sum += (float)0.039510045;
            } else {
              sum += (float)-0.034180366;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.12007469)) {
        if ( (data[13].fvalue < 1.1003256)) {
          if ( (data[14].fvalue < 0.050428987)) {
            if ( (data[13].fvalue < 0.31977212)) {
              sum += (float)-0.020165931;
            } else {
              sum += (float)0.051218621;
            }
          } else {
            if ( (data[13].fvalue < 0.21330187)) {
              sum += (float)-0.018505819;
            } else {
              sum += (float)-0.10551938;
            }
          }
        } else {
          sum += (float)-0.1085467;
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0503726)) {
              sum += (float)0.086310841;
            } else {
              sum += (float)-0.0087606339;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.061839253;
            } else {
              sum += (float)-0.017540744;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.038894985)) {
            sum += (float)-0.11793963;
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.057207279;
            } else {
              sum += (float)0.011428685;
            }
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.11431034)) {
    if ( (data[8].fvalue < 5.5)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[6].fvalue < 0.037112217)) {
            if ( (data[13].fvalue < 0.073527128)) {
              sum += (float)-0.0025743295;
            } else {
              sum += (float)-0.18286747;
            }
          } else {
            if ( (data[15].fvalue < 0.027160207)) {
              sum += (float)0.012507201;
            } else {
              sum += (float)-0.0088049239;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.0014579028;
            } else {
              sum += (float)0.082423374;
            }
          } else {
            if ( (data[14].fvalue < 0.0091214869)) {
              sum += (float)-0.175124;
            } else {
              sum += (float)-0.061658148;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.031543665)) {
          sum += (float)0.23620965;
        } else {
          if ( (data[14].fvalue < 0.046255421)) {
            if ( (data[3].fvalue < 0.14043212)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.12364515;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.042257857;
            } else {
              sum += (float)0.024113258;
            }
          }
        }
      }
    } else {
      sum += (float)0.14131513;
    }
  } else {
    if ( (data[6].fvalue < 0.23321331)) {
      if ( (data[13].fvalue < 0.24514596)) {
        if ( (data[9].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.2208173)) {
            if ( (data[13].fvalue < 0.0069750678)) {
              sum += (float)-0.071930356;
            } else {
              sum += (float)0.040869907;
            }
          } else {
            if ( (data[14].fvalue < 0.3067776)) {
              sum += (float)-0.13497397;
            } else {
              sum += (float)0.020532634;
            }
          }
        } else {
          if ( (data[0].fvalue < 4.653451)) {
            if ( (data[3].fvalue < 0.45015013)) {
              sum += (float)0.12730686;
            } else {
              sum += (float)0.046844423;
            }
          } else {
            sum += (float)-0.025621921;
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[1].fvalue < 3.4961395)) {
            if ( (data[6].fvalue < 0.17133108)) {
              sum += (float)0.13770103;
            } else {
              sum += (float)0.027280729;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[4].fvalue < 1.3018219)) {
            sum += (float)0.23491485;
          } else {
            sum += (float)0.062599577;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.66289073)) {
        if ( (data[13].fvalue < 0.37561882)) {
          if ( (data[15].fvalue < 0.29038316)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.016155144;
            } else {
              sum += (float)-0.071116701;
            }
          } else {
            if ( (data[3].fvalue < 1.324572)) {
              sum += (float)0.022966297;
            } else {
              sum += (float)-0.0064305612;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.8753246)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)0.047313463;
            } else {
              sum += (float)-0.10246357;
            }
          } else {
            if ( (data[16].fvalue < 0.42699748)) {
              sum += (float)-0.058913667;
            } else {
              sum += (float)0.046967905;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 3.4854736)) {
          if ( (data[5].fvalue < 0.50014961)) {
            if ( (data[17].fvalue < 1.185221)) {
              sum += (float)0.068739697;
            } else {
              sum += (float)0.13416202;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          if ( (data[11].fvalue < 2.5)) {
            if ( (data[15].fvalue < 0.58698285)) {
              sum += (float)0.051657304;
            } else {
              sum += (float)-0.010581004;
            }
          } else {
            sum += (float)-0.070927404;
          }
        }
      }
    }
  }
  if ( (data[8].fvalue < 10.5)) {
    if ( (data[6].fvalue < 0.012506254)) {
      sum += (float)-0.11729842;
    } else {
      if ( (data[5].fvalue < 0.46696252)) {
        if (  (data[4].fvalue < 1.1304213)) {
          if (  (data[4].fvalue < 1.0623636)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.025460878;
            } else {
              sum += (float)0.1554576;
            }
          } else {
            if ( (data[13].fvalue < 0.11318624)) {
              sum += (float)-0;
            } else {
              sum += (float)0.17133084;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.29080623)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.021994229;
            } else {
              sum += (float)0.0083382167;
            }
          } else {
            if ( (data[16].fvalue < 0.026595514)) {
              sum += (float)0.027330058;
            } else {
              sum += (float)-0.03552971;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.38013589)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.46946615)) {
              sum += (float)-0.0076700468;
            } else {
              sum += (float)0.012257817;
            }
          } else {
            sum += (float)-0.19065496;
          }
        } else {
          if ( (data[14].fvalue < 0.033463288)) {
            if ( (data[13].fvalue < 0.73658013)) {
              sum += (float)-0.1107715;
            } else {
              sum += (float)-0.01110727;
            }
          } else {
            if ( (data[17].fvalue < 1.1336496)) {
              sum += (float)-0.026727546;
            } else {
              sum += (float)0.051610846;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.11148656;
  }
  if ( (data[17].fvalue < 2.1727576)) {
    if ( (data[13].fvalue < 2.4600871)) {
      if ( (data[14].fvalue < 1.6087475)) {
        if ( (data[14].fvalue < 0.090562984)) {
          if (  (data[4].fvalue < 2.2104216)) {
            if ( (data[15].fvalue < 1.8388658)) {
              sum += (float)-0.0033104275;
            } else {
              sum += (float)-0.1171189;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.16825438;
            } else {
              sum += (float)0.031193625;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.21689077)) {
            if (  (data[4].fvalue < 1.1334054)) {
              sum += (float)0.14842743;
            } else {
              sum += (float)0.030492721;
            }
          } else {
            if ( (data[12].fvalue < 1.5)) {
              sum += (float)0.0030486186;
            } else {
              sum += (float)0.13349833;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.64130759)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.066912882;
            } else {
              sum += (float)0.039232105;
            }
          } else {
            sum += (float)0.10433077;
          }
        } else {
          sum += (float)-0.13567294;
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        sum += (float)-0.11458405;
      } else {
        sum += (float)-0.014282404;
      }
    }
  } else {
    if ( (data[13].fvalue < 0.6790992)) {
      if ( (data[14].fvalue < 0.079413608)) {
        if ( (data[11].fvalue < 1.5)) {
          sum += (float)-0.24234171;
        } else {
          sum += (float)-0.0060711368;
        }
      } else {
        if ( (data[0].fvalue < 5.0982351)) {
          sum += (float)0.068718657;
        } else {
          sum += (float)-0.0498381;
        }
      }
    } else {
      sum += (float)0.09298043;
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[14].fvalue < 0.38037032)) {
      if ( (data[15].fvalue < 3.0383897)) {
        if ( (data[13].fvalue < 0.87770522)) {
          if ( (data[11].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.89810932)) {
              sum += (float)0.00013255207;
            } else {
              sum += (float)-0.072234407;
            }
          } else {
            if ( (data[4].fvalue < 1.4049581)) {
              sum += (float)-0.14254923;
            } else {
              sum += (float)0.048739243;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.58636594)) {
              sum += (float)-0.058794554;
            } else {
              sum += (float)0.044215932;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.084878765;
            } else {
              sum += (float)-0.031102879;
            }
          }
        }
      } else {
        sum += (float)-0.12109815;
      }
    } else {
      if ( (data[4].fvalue < 1.3954692)) {
        if ( (data[3].fvalue < 0.93904686)) {
          if ( (data[14].fvalue < 0.53725642)) {
            if (  (data[4].fvalue < 1.3282176)) {
              sum += (float)0.087603599;
            } else {
              sum += (float)0.002267035;
            }
          } else {
            sum += (float)0.16663347;
          }
        } else {
          if ( (data[14].fvalue < 0.62042379)) {
            if ( (data[3].fvalue < 1.733857)) {
              sum += (float)0.023291852;
            } else {
              sum += (float)-0.061281897;
            }
          } else {
            if ( (data[15].fvalue < 0.0077993884)) {
              sum += (float)0.11665633;
            } else {
              sum += (float)0.023920078;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.52340555)) {
          if ( (data[15].fvalue < 0.99298155)) {
            if ( (data[17].fvalue < 0.82876432)) {
              sum += (float)-0.026132667;
            } else {
              sum += (float)0.05984427;
            }
          } else {
            if ( (data[14].fvalue < 0.57083356)) {
              sum += (float)-0.020281553;
            } else {
              sum += (float)0.074407354;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.017412163)) {
            if ( (data[17].fvalue < 0.0096513787)) {
              sum += (float)0.14589308;
            } else {
              sum += (float)0.055345535;
            }
          } else {
            if ( (data[14].fvalue < 0.60144758)) {
              sum += (float)-0.026965993;
            } else {
              sum += (float)0.062070176;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[13].fvalue < 0.44505668)) {
        sum += (float)-0.0043990947;
      } else {
        if ( (data[14].fvalue < 0.056348018)) {
          sum += (float)0.082013123;
        } else {
          sum += (float)0.17435494;
        }
      }
    } else {
      if ( (data[3].fvalue < 0.50746036)) {
        if (  (data[4].fvalue < 1.3910844)) {
          sum += (float)0.17342126;
        } else {
          sum += (float)0.011566215;
        }
      } else {
        if ( (data[15].fvalue < 0.20725462)) {
          if (  (data[4].fvalue < 1.1864268)) {
            sum += (float)0.017493516;
          } else {
            if ( (data[13].fvalue < 0.88807213)) {
              sum += (float)-0.11133061;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)0.023960767;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.37281528)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[17].fvalue < 2.1610675)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.066703066)) {
              sum += (float)0.031916786;
            } else {
              sum += (float)-0.0011106554;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.066577703;
            } else {
              sum += (float)-0.061213132;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0970305)) {
            if ( (data[3].fvalue < 0.42036483)) {
              sum += (float)0.11993938;
            } else {
              sum += (float)-0.025043787;
            }
          } else {
            if ( (data[15].fvalue < 0.0008231735)) {
              sum += (float)-0.090759151;
            } else {
              sum += (float)0.01842436;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 0.5)) {
          sum += (float)-0.14206418;
        } else {
          sum += (float)-0.021828251;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.10958548)) {
        if ( (data[15].fvalue < 0.15541135)) {
          if ( (data[14].fvalue < 0.73365295)) {
            if ( (data[3].fvalue < 0.17187244)) {
              sum += (float)0.082265072;
            } else {
              sum += (float)-0.059797745;
            }
          } else {
            if ( (data[6].fvalue < 0.52741051)) {
              sum += (float)0.15162539;
            } else {
              sum += (float)0.026781769;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.41650766)) {
            if ( (data[13].fvalue < 0.18618385)) {
              sum += (float)0.17494024;
            } else {
              sum += (float)0.04739286;
            }
          } else {
            if (  (data[4].fvalue < 1.9168556)) {
              sum += (float)0.038462978;
            } else {
              sum += (float)-0.079247274;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.39242846)) {
          if ( (data[14].fvalue < 0.23174343)) {
            sum += (float)0.21989055;
          } else {
            sum += (float)0.09521959;
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            sum += (float)0.065257721;
          } else {
            sum += (float)-0.079618499;
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.0943556)) {
      if ( (data[3].fvalue < 0.61442339)) {
        sum += (float)-0.14865987;
      } else {
        if ( (data[3].fvalue < 1.2979634)) {
          if ( (data[13].fvalue < 0.46454152)) {
            if (  (data[4].fvalue < 1.0154676)) {
              sum += (float)-0.019345751;
            } else {
              sum += (float)0.087540545;
            }
          } else {
            if (  (data[4].fvalue < 0.98143977)) {
              sum += (float)-0.098442025;
            } else {
              sum += (float)-0.02645457;
            }
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.69883531)) {
              sum += (float)0.085734911;
            } else {
              sum += (float)-0.0083831623;
            }
          } else {
            if ( (data[14].fvalue < 0.25815913)) {
              sum += (float)-0.15370286;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.4884508)) {
        if ( (data[3].fvalue < 1.324667)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.050903082)) {
              sum += (float)0.10661132;
            } else {
              sum += (float)0.026965553;
            }
          } else {
            if ( (data[14].fvalue < 0.052975643)) {
              sum += (float)-0.0086034108;
            } else {
              sum += (float)0.071576662;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.79665732)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.0020069361;
            } else {
              sum += (float)-0.070402727;
            }
          } else {
            if ( (data[14].fvalue < 0.012868193)) {
              sum += (float)0.0027207958;
            } else {
              sum += (float)0.065110557;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.54730296)) {
          if ( (data[15].fvalue < 0.87855089)) {
            if ( (data[16].fvalue < 1.0285112)) {
              sum += (float)-0.028690726;
            } else {
              sum += (float)0.11650438;
            }
          } else {
            if ( (data[16].fvalue < 0.054985277)) {
              sum += (float)0.10027196;
            } else {
              sum += (float)0.010468158;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.48106134)) {
            if ( (data[15].fvalue < 0.022097744)) {
              sum += (float)0.093113482;
            } else {
              sum += (float)0.0089468788;
            }
          } else {
            if ( (data[17].fvalue < 1.6142871)) {
              sum += (float)-0.042540714;
            } else {
              sum += (float)0.01766523;
            }
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 0.87254483)) {
    if ( (data[15].fvalue < 0.002407331)) {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[5].fvalue < 1.1098945)) {
          sum += (float)0.10218418;
        } else {
          if ( (data[13].fvalue < 0.59779966)) {
            if ( (data[4].fvalue < 0.78838348)) {
              sum += (float)0.055779796;
            } else {
              sum += (float)0.010950178;
            }
          } else {
            if ( (data[3].fvalue < 1.7876103)) {
              sum += (float)-0.10006444;
            } else {
              sum += (float)0.011124375;
            }
          }
        }
      } else {
        sum += (float)0.084216617;
      }
    } else {
      sum += (float)-0.073771;
    }
  } else {
    if ( (data[5].fvalue < 0.6274007)) {
      if ( (data[13].fvalue < 0.21736196)) {
        if ( (data[8].fvalue < 5.5)) {
          if ( (data[14].fvalue < 0.013560846)) {
            if ( (data[17].fvalue < 0.80454332)) {
              sum += (float)-0.011208256;
            } else {
              sum += (float)-0.12501843;
            }
          } else {
            if ( (data[13].fvalue < 0.0073768254)) {
              sum += (float)-0.016333491;
            } else {
              sum += (float)0.010330676;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.15242431;
          } else {
            if ( (data[15].fvalue < 0.0087059978)) {
              sum += (float)-0.027708353;
            } else {
              sum += (float)0.071377359;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.6926246)) {
          if ( (data[14].fvalue < 0.31103212)) {
            if (  (data[4].fvalue < 1.053466)) {
              sum += (float)-0.132759;
            } else {
              sum += (float)0.014635759;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.02816546;
            } else {
              sum += (float)0.085417733;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.74880254)) {
            if ( (data[15].fvalue < 0.6359756)) {
              sum += (float)-0.034365229;
            } else {
              sum += (float)0.02148119;
            }
          } else {
            if (  (data[4].fvalue < 2.1861815)) {
              sum += (float)0.064699024;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.75309944)) {
        if ( (data[3].fvalue < 2.3374262)) {
          if ( (data[16].fvalue < 0.0036894409)) {
            if ( (data[3].fvalue < 0.078364506)) {
              sum += (float)-0.10558017;
            } else {
              sum += (float)-0.0082915956;
            }
          } else {
            if ( (data[13].fvalue < 0.37671798)) {
              sum += (float)-0.17246209;
            } else {
              sum += (float)-0.025541021;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.054206289;
            } else {
              sum += (float)0.026749948;
            }
          } else {
            sum += (float)-0.16731092;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.028971195)) {
          if ( (data[7].fvalue < 1.5)) {
            sum += (float)0.18271935;
          } else {
            sum += (float)0.043432716;
          }
        } else {
          if ( (data[15].fvalue < 0.13851163)) {
            sum += (float)-0.063416973;
          } else {
            if ( (data[3].fvalue < 3.7021914)) {
              sum += (float)0.035006735;
            } else {
              sum += (float)-0.037037481;
            }
          }
        }
      }
    }
  }
  if ( (data[1].fvalue < 0.11683212)) {
    if ( (data[3].fvalue < 0.37389448)) {
      sum += (float)0.095820278;
    } else {
      sum += (float)0.023144629;
    }
  } else {
    if ( (data[14].fvalue < 0.4030171)) {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.48499113)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.00086082559;
            } else {
              sum += (float)-0.034890842;
            }
          } else {
            if ( (data[14].fvalue < 0.022616535)) {
              sum += (float)-0.00030703135;
            } else {
              sum += (float)0.037748706;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.20002335)) {
            if ( (data[0].fvalue < 2.0468419)) {
              sum += (float)0.202242;
            } else {
              sum += (float)0.0068843272;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.086975269;
            } else {
              sum += (float)-0.012715206;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.20394292)) {
          sum += (float)0.13745806;
        } else {
          sum += (float)0.015358484;
        }
      }
    } else {
      if ( (data[4].fvalue < 1.3808531)) {
        if ( (data[3].fvalue < 1.0823976)) {
          if ( (data[13].fvalue < 0.040293381)) {
            if ( (data[3].fvalue < 0.82093495)) {
              sum += (float)0.07944122;
            } else {
              sum += (float)0.0060513257;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.03971808;
            } else {
              sum += (float)0.14731252;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.0071991854)) {
              sum += (float)-0.0044931122;
            } else {
              sum += (float)0.067242846;
            }
          } else {
            if ( (data[3].fvalue < 2.5654299)) {
              sum += (float)-0.004378322;
            } else {
              sum += (float)-0.15366124;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.67451608)) {
          if ( (data[15].fvalue < 0.46727544)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.054539964;
            } else {
              sum += (float)0.012803844;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.036253437;
            } else {
              sum += (float)-0.026918931;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.017009513;
            } else {
              sum += (float)0.15590963;
            }
          } else {
            if (  (data[4].fvalue < 1.914994)) {
              sum += (float)0.052499849;
            } else {
              sum += (float)-0.036651205;
            }
          }
        }
      }
    }
  }
  if ( (data[9].fvalue < 2.5)) {
    if ( (data[10].fvalue < 1.5)) {
      if ( (data[14].fvalue < 0.23228273)) {
        if ( (data[7].fvalue < 5.5)) {
          if ( (data[4].fvalue < 0.71728462)) {
            if ( (data[3].fvalue < 1.5566969)) {
              sum += (float)0.11092437;
            } else {
              sum += (float)0.017358804;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.0024164282;
            } else {
              sum += (float)-0.022633061;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.15017679)) {
            if ( (data[13].fvalue < 0.76720393)) {
              sum += (float)-0;
            } else {
              sum += (float)0.073222309;
            }
          } else {
            sum += (float)0.10114795;
          }
        }
      } else {
        if ( (data[0].fvalue < 3.3184721)) {
          if ( (data[6].fvalue < 0.45455524)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.027367292;
            } else {
              sum += (float)0.082087696;
            }
          } else {
            if ( (data[17].fvalue < 0.58944678)) {
              sum += (float)0.0014623665;
            } else {
              sum += (float)0.073636316;
            }
          }
        } else {
          if ( (data[17].fvalue < 1.4980109)) {
            if ( (data[15].fvalue < 1.3504211)) {
              sum += (float)-0.018931761;
            } else {
              sum += (float)0.033458967;
            }
          } else {
            if ( (data[15].fvalue < 0.21770701)) {
              sum += (float)0.10889753;
            } else {
              sum += (float)0.0048861811;
            }
          }
        }
      }
    } else {
      if ( (data[9].fvalue < 1.5)) {
        if ( (data[4].fvalue < 2.1199005)) {
          sum += (float)-0.18326944;
        } else {
          sum += (float)-0.082580872;
        }
      } else {
        if ( (data[0].fvalue < 4.0154929)) {
          sum += (float)0.10076959;
        } else {
          sum += (float)-0.044305924;
        }
      }
    }
  } else {
    if ( (data[0].fvalue < 4.1171546)) {
      sum += (float)0.15723634;
    } else {
      sum += (float)-0;
    }
  }
  if ( (data[5].fvalue < 0.43691868)) {
    if ( (data[16].fvalue < 0.39441481)) {
      if ( (data[13].fvalue < 1.238147)) {
        if ( (data[17].fvalue < 0.19328547)) {
          if (  (data[4].fvalue < 2.0179551)) {
            if ( (data[15].fvalue < 0.10601185)) {
              sum += (float)-0.0084266076;
            } else {
              sum += (float)0.010606304;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.10792395;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7350503)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.030426148;
            } else {
              sum += (float)0.15206948;
            }
          } else {
            if ( (data[17].fvalue < 0.88774562)) {
              sum += (float)-0.021239989;
            } else {
              sum += (float)0.029675951;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 2.0791359)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[17].fvalue < 0.28146178)) {
              sum += (float)-0.029340822;
            } else {
              sum += (float)0.045222845;
            }
          } else {
            sum += (float)0.07925076;
          }
        } else {
          if ( (data[15].fvalue < 0.81478155)) {
            if ( (data[15].fvalue < 0.020336993)) {
              sum += (float)-0.0048563029;
            } else {
              sum += (float)-0.14510053;
            }
          } else {
            sum += (float)0.043526728;
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.018328581)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.25116277)) {
            if ( (data[16].fvalue < 0.79565847)) {
              sum += (float)0.049809568;
            } else {
              sum += (float)-0.070195459;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.086083546;
            } else {
              sum += (float)0.0011297687;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.1996238)) {
            sum += (float)0.13620274;
          } else {
            sum += (float)0.034786664;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.52146876)) {
          if (  (data[4].fvalue < 2.4114017)) {
            if ( (data[14].fvalue < 0.010561993)) {
              sum += (float)-0.043313485;
            } else {
              sum += (float)0.038358755;
            }
          } else {
            sum += (float)0.091916934;
          }
        } else {
          if ( (data[13].fvalue < 0.50861943)) {
            if ( (data[14].fvalue < 0.14099653)) {
              sum += (float)-0.15336013;
            } else {
              sum += (float)-0.04494926;
            }
          } else {
            sum += (float)0.0023226289;
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.53404516)) {
      if ( (data[4].fvalue < 1.2081444)) {
        if ( (data[16].fvalue < 0.0007513467)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.0060908641;
            } else {
              sum += (float)-0.017188506;
            }
          } else {
            if ( (data[3].fvalue < 0.62143993)) {
              sum += (float)0.13844414;
            } else {
              sum += (float)-0.015487959;
            }
          }
        } else {
          sum += (float)-0.13407083;
        }
      } else {
        if ( (data[14].fvalue < 0.59275138)) {
          if ( (data[15].fvalue < 1.0742888)) {
            if ( (data[15].fvalue < 0.12992989)) {
              sum += (float)-0.074694708;
            } else {
              sum += (float)-0.018379582;
            }
          } else {
            if (  (data[4].fvalue < 1.607069)) {
              sum += (float)-0.15875696;
            } else {
              sum += (float)-0.076330975;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0023024911;
            } else {
              sum += (float)0.075098127;
            }
          } else {
            if ( (data[13].fvalue < 0.21540222)) {
              sum += (float)-0.089625388;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.81320131)) {
        if ( (data[3].fvalue < 1.2359512)) {
          if (  (data[4].fvalue < 1.1206455)) {
            if ( (data[3].fvalue < 0.8931452)) {
              sum += (float)-0.087785684;
            } else {
              sum += (float)0.036937665;
            }
          } else {
            if ( (data[15].fvalue < 0.00519979)) {
              sum += (float)0.082148671;
            } else {
              sum += (float)0.026907565;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.9358353)) {
            if ( (data[14].fvalue < 0.0029005809)) {
              sum += (float)-0.037352446;
            } else {
              sum += (float)0.0029560707;
            }
          } else {
            if ( (data[17].fvalue < 0.011150044)) {
              sum += (float)0.054404344;
            } else {
              sum += (float)-0.011439429;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.85482943)) {
          if ( (data[3].fvalue < 1.907095)) {
            sum += (float)-0.081472114;
          } else {
            sum += (float)-0.013412824;
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.015569366;
            } else {
              sum += (float)0.056387581;
            }
          } else {
            sum += (float)-0.06295456;
          }
        }
      }
    }
  }
  if ( (data[0].fvalue < 1.6489971)) {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[17].fvalue < 0.10951018)) {
        if ( (data[15].fvalue < 0.1322526)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.07886371;
          } else {
            if ( (data[4].fvalue < 1.1523535)) {
              sum += (float)0.011484375;
            } else {
              sum += (float)-0.014755548;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.35290968)) {
            if ( (data[13].fvalue < 0.22599807)) {
              sum += (float)0.025158614;
            } else {
              sum += (float)0.12254531;
            }
          } else {
            if ( (data[14].fvalue < 0.14571942)) {
              sum += (float)-0.024867717;
            } else {
              sum += (float)0.040530585;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.044754788)) {
          sum += (float)-0.074236862;
        } else {
          if ( (data[3].fvalue < 0.24993154)) {
            sum += (float)0.16510639;
          } else {
            if ( (data[14].fvalue < 0.0075943675)) {
              sum += (float)-0;
            } else {
              sum += (float)0.082709059;
            }
          }
        }
      }
    } else {
      if ( (data[0].fvalue < 1.2669361)) {
        if (  (data[4].fvalue < 1.2639313)) {
          if ( (data[13].fvalue < 0.11540096)) {
            sum += (float)0.08578708;
          } else {
            sum += (float)0.190577;
          }
        } else {
          sum += (float)0.040631946;
        }
      } else {
        if ( (data[6].fvalue < 0.3812539)) {
          sum += (float)0.094065525;
        } else {
          if ( (data[14].fvalue < 0.3719967)) {
            sum += (float)-0.064562812;
          } else {
            sum += (float)0.022503234;
          }
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 3.0944376)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[7].fvalue < 5.5)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.003270908;
            } else {
              sum += (float)-0.020685501;
            }
          } else {
            if (  (data[4].fvalue < 1.4351239)) {
              sum += (float)0.013852813;
            } else {
              sum += (float)0.138301;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.058608908)) {
            if ( (data[13].fvalue < 0.011664244)) {
              sum += (float)-0.091680788;
            } else {
              sum += (float)0.15769309;
            }
          } else {
            if ( (data[14].fvalue < 0.074049309)) {
              sum += (float)-0.05662518;
            } else {
              sum += (float)-0.0043520341;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.28581375)) {
          sum += (float)0.11674499;
        } else {
          if (  (data[4].fvalue < 1.5145965)) {
            sum += (float)0.040556446;
          } else {
            if (  (data[4].fvalue < 2.4559689)) {
              sum += (float)-0.10867899;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      sum += (float)-0.083497159;
    }
  }
  if ( (data[13].fvalue < 0.21865496)) {
    if ( (data[4].fvalue < 1.8662635)) {
      if ( (data[3].fvalue < 0.54904318)) {
        if ( (data[17].fvalue < 0.14025189)) {
          if (  (data[4].fvalue < 0.99935895)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.0025321238;
            } else {
              sum += (float)0.17178601;
            }
          } else {
            if ( (data[15].fvalue < 0.12198237)) {
              sum += (float)-0.026265604;
            } else {
              sum += (float)0.011843801;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0045745857)) {
            if ( (data[14].fvalue < 0.14907387)) {
              sum += (float)-0.17633128;
            } else {
              sum += (float)-0.024808582;
            }
          } else {
            if ( (data[14].fvalue < 0.0017269377)) {
              sum += (float)0.023570379;
            } else {
              sum += (float)0.11912724;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[4].fvalue < 1.0484782)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.096341535;
            } else {
              sum += (float)-0.097951852;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)-0.024114989;
            } else {
              sum += (float)0.053811278;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.24346741)) {
            if ( (data[17].fvalue < 0.18674438)) {
              sum += (float)-0.11306941;
            } else {
              sum += (float)0.0028980759;
            }
          } else {
            if ( (data[13].fvalue < 0.10726088)) {
              sum += (float)-0.049937408;
            } else {
              sum += (float)0.049842946;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[15].fvalue < 1.4383374)) {
            if ( (data[3].fvalue < 0.83147091)) {
              sum += (float)0.16858466;
            } else {
              sum += (float)0.37826791;
            }
          } else {
            sum += (float)0.020656593;
          }
        } else {
          if ( (data[15].fvalue < 1.9773073)) {
            if ( (data[17].fvalue < 0.011402729)) {
              sum += (float)0.061182868;
            } else {
              sum += (float)-0.0019261293;
            }
          } else {
            if ( (data[14].fvalue < 0.34596336)) {
              sum += (float)-0.12698586;
            } else {
              sum += (float)0.038989726;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0024577291)) {
          if ( (data[16].fvalue < 0.30354843)) {
            if ( (data[17].fvalue < 1.5791079)) {
              sum += (float)0.02446752;
            } else {
              sum += (float)-0.1112362;
            }
          } else {
            if ( (data[3].fvalue < 0.89730442)) {
              sum += (float)-0.064840317;
            } else {
              sum += (float)-0.15167221;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.32607231)) {
            if ( (data[17].fvalue < 0.31311458)) {
              sum += (float)0.099553481;
            } else {
              sum += (float)0.011172535;
            }
          } else {
            if ( (data[16].fvalue < 0.15376416)) {
              sum += (float)-0.061510906;
            } else {
              sum += (float)0.048426788;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.083323881)) {
      if (  (data[4].fvalue < 1.074219)) {
        if ( (data[0].fvalue < 4.6482553)) {
          if ( (data[6].fvalue < 0.073401242)) {
            sum += (float)-0.1983107;
          } else {
            sum += (float)-0.07882531;
          }
        } else {
          sum += (float)-0.017523061;
        }
      } else {
        sum += (float)0.028835036;
      }
    } else {
      if ( (data[4].fvalue < 1.4614233)) {
        if ( (data[5].fvalue < 0.67237383)) {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0852752)) {
              sum += (float)-0.042228103;
            } else {
              sum += (float)0.05290404;
            }
          } else {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.0011111072;
            } else {
              sum += (float)0.07786423;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.40143007)) {
            sum += (float)-0.11155789;
          } else {
            if ( (data[13].fvalue < 0.30117792)) {
              sum += (float)0.032100767;
            } else {
              sum += (float)-0.0019294251;
            }
          }
        }
      } else {
        if ( (data[15].fvalue < 0.37920785)) {
          if ( (data[14].fvalue < 0.0032508145)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.04901902;
            } else {
              sum += (float)0.024594244;
            }
          } else {
            if ( (data[16].fvalue < 0.098221026)) {
              sum += (float)-0.0087970635;
            } else {
              sum += (float)0.047920138;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.060799643)) {
              sum += (float)0.045670059;
            } else {
              sum += (float)-0.0058976514;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)-0.042015411;
            } else {
              sum += (float)0.031840358;
            }
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.21632226)) {
    if ( (data[17].fvalue < 0.24306783)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[15].fvalue < 0.67762518)) {
          if (  (data[4].fvalue < 2.1170449)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.00094129011;
            } else {
              sum += (float)-0.037575904;
            }
          } else {
            if ( (data[13].fvalue < 1.2855418)) {
              sum += (float)0.11689412;
            } else {
              sum += (float)-0.048344277;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.649246)) {
            if ( (data[5].fvalue < 0.15246418)) {
              sum += (float)-0.0076754787;
            } else {
              sum += (float)-0.12535618;
            }
          } else {
            if ( (data[13].fvalue < 0.52803892)) {
              sum += (float)-0.034050643;
            } else {
              sum += (float)0.042546574;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.51278055)) {
          if (  (data[4].fvalue < 1.2295566)) {
            if ( (data[13].fvalue < 0.24919073)) {
              sum += (float)0.014634287;
            } else {
              sum += (float)0.26002041;
            }
          } else {
            if ( (data[15].fvalue < 0.0051588193)) {
              sum += (float)-0.088502362;
            } else {
              sum += (float)0.013158606;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.18815997)) {
            if ( (data[13].fvalue < 0.11177406)) {
              sum += (float)-0.22377448;
            } else {
              sum += (float)-0.069677375;
            }
          } else {
            if ( (data[13].fvalue < 0.261886)) {
              sum += (float)0.034467626;
            } else {
              sum += (float)-0.039125372;
            }
          }
        }
      }
    } else {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[17].fvalue < 0.97587806)) {
          if ( (data[16].fvalue < 0.039815564)) {
            if ( (data[15].fvalue < 0.44733313)) {
              sum += (float)0.011654831;
            } else {
              sum += (float)0.072470993;
            }
          } else {
            if ( (data[13].fvalue < 0.30743176)) {
              sum += (float)-0.07777074;
            } else {
              sum += (float)-0.0056762877;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.38822943)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.1207016;
            } else {
              sum += (float)0.059649706;
            }
          } else {
            if ( (data[5].fvalue < 0.48105252)) {
              sum += (float)0.091515936;
            } else {
              sum += (float)-0.05989318;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.68712473)) {
          if (  (data[4].fvalue < 2.5235701)) {
            if ( (data[17].fvalue < 1.155792)) {
              sum += (float)0.045419782;
            } else {
              sum += (float)0.14133511;
            }
          } else {
            sum += (float)-0.0414639;
          }
        } else {
          if ( (data[16].fvalue < 0.034307167)) {
            if ( (data[17].fvalue < 1.2807852)) {
              sum += (float)-0.1184295;
            } else {
              sum += (float)0.042529061;
            }
          } else {
            sum += (float)0.061792456;
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.3798356)) {
      if ( (data[3].fvalue < 0.53302085)) {
        if ( (data[13].fvalue < 0.13122572)) {
          if ( (data[15].fvalue < 0.00050673535)) {
            if (  (data[4].fvalue < 1.2402192)) {
              sum += (float)0.06019352;
            } else {
              sum += (float)-0.074166276;
            }
          } else {
            sum += (float)0.095114879;
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if (  (data[4].fvalue < 1.3091645)) {
              sum += (float)0.12262945;
            } else {
              sum += (float)0.012482937;
            }
          } else {
            sum += (float)0.22395603;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.66109478)) {
          if ( (data[3].fvalue < 1.898807)) {
            if ( (data[13].fvalue < 0.14066681)) {
              sum += (float)-0.01860692;
            } else {
              sum += (float)0.045445155;
            }
          } else {
            if ( (data[13].fvalue < 0.73911119)) {
              sum += (float)-0.08996217;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[3].fvalue < 2.9760585)) {
            if (  (data[4].fvalue < 1.3099937)) {
              sum += (float)0.093297422;
            } else {
              sum += (float)0.016535459;
            }
          } else {
            sum += (float)-0.0045483764;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.98177576)) {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.11329274)) {
            if ( (data[15].fvalue < 0.42978847)) {
              sum += (float)-0.013877635;
            } else {
              sum += (float)0.0094972821;
            }
          } else {
            if (  (data[4].fvalue < 1.9329016)) {
              sum += (float)0.087837316;
            } else {
              sum += (float)-0.0043061059;
            }
          }
        } else {
          sum += (float)-0.081715979;
        }
      } else {
        if ( (data[15].fvalue < 0.41334748)) {
          sum += (float)0.1526376;
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[1].fvalue < 3.4522476)) {
    if ( (data[6].fvalue < 0.015801299)) {
      sum += (float)-0.14675331;
    } else {
      if ( (data[14].fvalue < 0.019784188)) {
        if ( (data[15].fvalue < 0.02538988)) {
          if ( (data[13].fvalue < 0.084404126)) {
            if ( (data[17].fvalue < 0.28765154)) {
              sum += (float)0.054632824;
            } else {
              sum += (float)-0.13095537;
            }
          } else {
            if ( (data[6].fvalue < 0.050809316)) {
              sum += (float)-0.14744541;
            } else {
              sum += (float)0.00055107014;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.2476995)) {
            if ( (data[15].fvalue < 0.08914832)) {
              sum += (float)-0.1138318;
            } else {
              sum += (float)-0.038714711;
            }
          } else {
            if ( (data[17].fvalue < 0.017374959)) {
              sum += (float)0.0022432795;
            } else {
              sum += (float)-0.030891646;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.058578283)) {
          if ( (data[15].fvalue < 0.010740751)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.03310293;
            } else {
              sum += (float)0.048781041;
            }
          } else {
            if (  (data[4].fvalue < 1.3013289)) {
              sum += (float)0.16354595;
            } else {
              sum += (float)0.055748265;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.11969082)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.032442026;
            } else {
              sum += (float)0.0028562953;
            }
          } else {
            if ( (data[6].fvalue < 0.17436847)) {
              sum += (float)0.073726609;
            } else {
              sum += (float)0.0051853606;
            }
          }
        }
      }
    }
  } else {
    if ( (data[2].fvalue < 0.06105959)) {
      if ( (data[13].fvalue < 0.065065883)) {
        if ( (data[14].fvalue < 4.6418068e-06)) {
          sum += (float)0.083070956;
        } else {
          sum += (float)-0;
        }
      } else {
        if ( (data[14].fvalue < 0.0043958593)) {
          sum += (float)-0.036093343;
        } else {
          sum += (float)0.02814983;
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[13].fvalue < 0.042501837)) {
          if ( (data[14].fvalue < 7.0669245e-08)) {
            sum += (float)0.015105115;
          } else {
            if ( (data[14].fvalue < 0.16713418)) {
              sum += (float)-0.088758349;
            } else {
              sum += (float)-0.017777937;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.023920532)) {
            sum += (float)-0.08936134;
          } else {
            if ( (data[13].fvalue < 0.10784329)) {
              sum += (float)0.043164957;
            } else {
              sum += (float)-0.016303487;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.15739229)) {
          sum += (float)-0.10568295;
        } else {
          if ( (data[0].fvalue < 4.7459421)) {
            sum += (float)0.027433727;
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.030933294;
            } else {
              sum += (float)-0.10182172;
            }
          }
        }
      }
    }
  }
  if ( (data[0].fvalue < 1.0823897)) {
    if ( (data[8].fvalue < 1.5)) {
      if (  (data[4].fvalue < 0.98402578)) {
        if ( (data[13].fvalue < 0.092344284)) {
          if ( (data[3].fvalue < 0.13188459)) {
            sum += (float)-0.065612033;
          } else {
            if ( (data[14].fvalue < 0.011545196)) {
              sum += (float)0.099965073;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.34815362)) {
            sum += (float)-0.15013011;
          } else {
            if ( (data[13].fvalue < 0.20396145)) {
              sum += (float)0.031680811;
            } else {
              sum += (float)-0.050719909;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.016696919)) {
          sum += (float)-0.054977316;
        } else {
          if ( (data[6].fvalue < 0.21519363)) {
            sum += (float)-0.017738366;
          } else {
            if ( (data[6].fvalue < 0.54683256)) {
              sum += (float)0.051730525;
            } else {
              sum += (float)0.0092377579;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.28100204)) {
        sum += (float)0.19138512;
      } else {
        if ( (data[13].fvalue < 0.2760846)) {
          if ( (data[0].fvalue < 0.80559796)) {
            sum += (float)0.073809229;
          } else {
            if ( (data[14].fvalue < 0.22183031)) {
              sum += (float)-0.054543186;
            } else {
              sum += (float)0.038124513;
            }
          }
        } else {
          sum += (float)0.15055817;
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.26047319)) {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.56092751)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.70760548)) {
              sum += (float)0.0021420342;
            } else {
              sum += (float)-0.02474732;
            }
          } else {
            if ( (data[6].fvalue < 0.18584178)) {
              sum += (float)0.019796569;
            } else {
              sum += (float)-0.025162097;
            }
          }
        } else {
          if ( (data[9].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.74259996)) {
              sum += (float)0.014438161;
            } else {
              sum += (float)0.12843403;
            }
          } else {
            if ( (data[14].fvalue < 0.74355233)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.08133667;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.31574976)) {
          sum += (float)-0.0083460454;
        } else {
          if ( (data[7].fvalue < 2.5)) {
            sum += (float)-0.14882135;
          } else {
            sum += (float)-0.057722934;
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[14].fvalue < 1.0755093)) {
          if ( (data[13].fvalue < 0.30745435)) {
            if ( (data[11].fvalue < 1.5)) {
              sum += (float)-0.018815571;
            } else {
              sum += (float)0.046091631;
            }
          } else {
            if ( (data[15].fvalue < 0.010369007)) {
              sum += (float)0.030733859;
            } else {
              sum += (float)-0.012483574;
            }
          }
        } else {
          if ( (data[0].fvalue < 4.3941998)) {
            sum += (float)-0.11125935;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.9860992)) {
          if ( (data[3].fvalue < 1.1065123)) {
            if (  (data[4].fvalue < 1.6324244)) {
              sum += (float)0.1457575;
            } else {
              sum += (float)0.049372505;
            }
          } else {
            if ( (data[17].fvalue < 0.64701581)) {
              sum += (float)-0.0079804529;
            } else {
              sum += (float)0.066102125;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.10129809)) {
            sum += (float)-0.08815226;
          } else {
            if ( (data[11].fvalue < 2.5)) {
              sum += (float)0.017149933;
            } else {
              sum += (float)-0.087085575;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.45112753)) {
    if ( (data[13].fvalue < 0.17957026)) {
      if (  (data[4].fvalue < 1.7931774)) {
        if ( (data[16].fvalue < 0.13932875)) {
          if ( (data[16].fvalue < 0.0024312721)) {
            if ( (data[3].fvalue < 0.28413975)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.019998746;
            }
          } else {
            if ( (data[13].fvalue < 0.07699351)) {
              sum += (float)-0.14740051;
            } else {
              sum += (float)-0.049286343;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.068423569)) {
            sum += (float)-0;
          } else {
            sum += (float)0.097761661;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.098518521)) {
          if ( (data[17].fvalue < 0.23352675)) {
            if ( (data[14].fvalue < 0.60474283)) {
              sum += (float)0.18272643;
            } else {
              sum += (float)-0.0057345531;
            }
          } else {
            if ( (data[15].fvalue < 0.00039965121)) {
              sum += (float)-0.037945442;
            } else {
              sum += (float)0.05734976;
            }
          }
        } else {
          if ( (data[16].fvalue < 1.3872206)) {
            if ( (data[15].fvalue < 1.8714182)) {
              sum += (float)0.013112915;
            } else {
              sum += (float)-0.026137078;
            }
          } else {
            sum += (float)-0.10888105;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1726636)) {
        if (  (data[4].fvalue < 1.0784447)) {
          sum += (float)-0.12455315;
        } else {
          if (  (data[4].fvalue < 1.1462295)) {
            if ( (data[5].fvalue < 0.38833451)) {
              sum += (float)0.18621102;
            } else {
              sum += (float)0.080794126;
            }
          } else {
            if ( (data[13].fvalue < 0.27377516)) {
              sum += (float)-0.0041995789;
            } else {
              sum += (float)0.12051579;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7121763)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.020688787;
            } else {
              sum += (float)0.044283237;
            }
          } else {
            if (  (data[4].fvalue < 1.4479733)) {
              sum += (float)0.071995251;
            } else {
              sum += (float)0.01901675;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.55221856)) {
            if ( (data[17].fvalue < 0.68003094)) {
              sum += (float)-0.023434728;
            } else {
              sum += (float)0.022850875;
            }
          } else {
            if (  (data[4].fvalue < 2.6970329)) {
              sum += (float)0.058123618;
            } else {
              sum += (float)-0.028200639;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 1.0081919)) {
      if (  (data[4].fvalue < 0.93873143)) {
        sum += (float)-0.10922991;
      } else {
        if ( (data[5].fvalue < 0.99177861)) {
          if (  (data[4].fvalue < 0.95125186)) {
            if ( (data[13].fvalue < 0.13824588)) {
              sum += (float)-0.025262367;
            } else {
              sum += (float)-0.10938685;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.0026674268;
            } else {
              sum += (float)-0.020978348;
            }
          }
        } else {
          sum += (float)0.11081512;
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[3].fvalue < 0.26015505)) {
          if ( (data[13].fvalue < 0.062200852)) {
            if ( (data[5].fvalue < 1.4675652)) {
              sum += (float)0.049231093;
            } else {
              sum += (float)-0.088063486;
            }
          } else {
            sum += (float)-0.17770545;
          }
        } else {
          if ( (data[13].fvalue < 0.11826929)) {
            if ( (data[14].fvalue < 0.00022479409)) {
              sum += (float)0.16050592;
            } else {
              sum += (float)-0.014530282;
            }
          } else {
            if ( (data[3].fvalue < 0.38496155)) {
              sum += (float)-0.19600977;
            } else {
              sum += (float)0.0051222974;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 1.2802556)) {
          if ( (data[14].fvalue < 0.092729241)) {
            sum += (float)-0;
          } else {
            sum += (float)0.2233424;
          }
        } else {
          sum += (float)-0.06428989;
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.11814509)) {
    if ( (data[7].fvalue < 6.5)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[6].fvalue < 0.04343985)) {
            if ( (data[13].fvalue < 0.075733826)) {
              sum += (float)-0.00030711529;
            } else {
              sum += (float)-0.10316999;
            }
          } else {
            if ( (data[15].fvalue < 0.030572258)) {
              sum += (float)0.0068558524;
            } else {
              sum += (float)-0.0087951636;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0594808)) {
            sum += (float)0.093640342;
          } else {
            if ( (data[13].fvalue < 0.6062541)) {
              sum += (float)-0.11529796;
            } else {
              sum += (float)0.010029161;
            }
          }
        }
      } else {
        if ( (data[9].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.032807246)) {
            sum += (float)0.1258695;
          } else {
            if ( (data[13].fvalue < 0.024615046)) {
              sum += (float)-0.17250223;
            } else {
              sum += (float)-0.044950895;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.47153312)) {
            if (  (data[4].fvalue < 1.2292781)) {
              sum += (float)0.19270611;
            } else {
              sum += (float)0.036585551;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.032465313;
            } else {
              sum += (float)-0.054745633;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.64456427)) {
        sum += (float)0.09375561;
      } else {
        sum += (float)0.0073130634;
      }
    }
  } else {
    if (  (data[4].fvalue < 1.2468355)) {
      if ( (data[3].fvalue < 0.71519423)) {
        if ( (data[13].fvalue < 0.08690428)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.1704631;
            } else {
              sum += (float)-0;
            }
          } else {
            if (  (data[4].fvalue < 1.2106081)) {
              sum += (float)0.14127414;
            } else {
              sum += (float)0.025812816;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0518899)) {
            if (  (data[4].fvalue < 1.0039496)) {
              sum += (float)0.26506171;
            } else {
              sum += (float)0.13817987;
            }
          } else {
            if ( (data[14].fvalue < 0.25753325)) {
              sum += (float)0.036852386;
            } else {
              sum += (float)0.15607551;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.33207226)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.023972755;
            } else {
              sum += (float)-0.02370465;
            }
          } else {
            if ( (data[13].fvalue < 0.093618877)) {
              sum += (float)-0.14147539;
            } else {
              sum += (float)-0.024708942;
            }
          }
        } else {
          if ( (data[3].fvalue < 2.3977292)) {
            if ( (data[13].fvalue < 0.28871381)) {
              sum += (float)0.02215125;
            } else {
              sum += (float)0.11371474;
            }
          } else {
            if ( (data[14].fvalue < 0.95963192)) {
              sum += (float)-0.080741443;
            } else {
              sum += (float)0.049192142;
            }
          }
        }
      }
    } else {
      if ( (data[12].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.59088379)) {
          if ( (data[15].fvalue < 0.15799373)) {
            if ( (data[17].fvalue < 0.7159785)) {
              sum += (float)-0.029741725;
            } else {
              sum += (float)0.034486637;
            }
          } else {
            if ( (data[4].fvalue < 1.5119255)) {
              sum += (float)0.043999881;
            } else {
              sum += (float)-0.00048354367;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8572248)) {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)0.015533979;
            } else {
              sum += (float)0.091970198;
            }
          } else {
            if ( (data[14].fvalue < 0.6636219)) {
              sum += (float)-0.039378859;
            } else {
              sum += (float)0.057800226;
            }
          }
        }
      } else {
        sum += (float)0.12384137;
      }
    }
  }
  if ( (data[14].fvalue < 0.11304241)) {
    if ( (data[5].fvalue < 0.34523946)) {
      if ( (data[13].fvalue < 0.68046546)) {
        if ( (data[13].fvalue < 0.034269128)) {
          if ( (data[17].fvalue < 0.87437755)) {
            if ( (data[3].fvalue < 0.56972742)) {
              sum += (float)-0.02146266;
            } else {
              sum += (float)0.12129259;
            }
          } else {
            sum += (float)-0.14889282;
          }
        } else {
          if (  (data[4].fvalue < 1.0437007)) {
            if ( (data[13].fvalue < 0.073557094)) {
              sum += (float)0.033865791;
            } else {
              sum += (float)-0.1567672;
            }
          } else {
            if (  (data[4].fvalue < 1.1240608)) {
              sum += (float)0.11013202;
            } else {
              sum += (float)0.010853367;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.78251576)) {
          if ( (data[7].fvalue < 4.5)) {
            if (  (data[4].fvalue < 2.3506036)) {
              sum += (float)-0.069457181;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.029520599;
          }
        } else {
          if ( (data[16].fvalue < 0.73061806)) {
            if ( (data[16].fvalue < 0.04907193)) {
              sum += (float)0.035562389;
            } else {
              sum += (float)-0.056774449;
            }
          } else {
            sum += (float)0.11347594;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 7.0669245e-08)) {
        if ( (data[13].fvalue < 0.11235952)) {
          if ( (data[6].fvalue < 0.030473255)) {
            if ( (data[13].fvalue < 0.031020176)) {
              sum += (float)0.094177537;
            } else {
              sum += (float)-0.10024749;
            }
          } else {
            if ( (data[0].fvalue < 2.6338305)) {
              sum += (float)0.040252186;
            } else {
              sum += (float)0.15312561;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.23421448)) {
            if (  (data[4].fvalue < 1.0190475)) {
              sum += (float)-0.19334653;
            } else {
              sum += (float)-0.01755522;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.013972245;
            } else {
              sum += (float)0.01050025;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.08310505)) {
          if ( (data[5].fvalue < 1.3546619)) {
            if ( (data[3].fvalue < 0.17469499)) {
              sum += (float)-0.04534797;
            } else {
              sum += (float)-0.10766954;
            }
          } else {
            if ( (data[3].fvalue < 0.36422294)) {
              sum += (float)0.046902169;
            } else {
              sum += (float)-0.024167735;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)0.0048050582;
            } else {
              sum += (float)0.097764805;
            }
          } else {
            if ( (data[3].fvalue < 0.37979507)) {
              sum += (float)0.094857179;
            } else {
              sum += (float)-0.061529703;
            }
          }
        }
      }
    }
  } else {
    if ( (data[0].fvalue < 2.8542762)) {
      if ( (data[6].fvalue < 0.13638791)) {
        if ( (data[13].fvalue < 0.019823549)) {
          sum += (float)-0;
        } else {
          sum += (float)0.15992725;
        }
      } else {
        if ( (data[13].fvalue < 0.31102014)) {
          if ( (data[9].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.19427064)) {
              sum += (float)-0.052777477;
            } else {
              sum += (float)0.002470884;
            }
          } else {
            if ( (data[6].fvalue < 0.30852407)) {
              sum += (float)0.071145356;
            } else {
              sum += (float)0.012404094;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.37545645)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.00079262094;
            } else {
              sum += (float)0.16961062;
            }
          } else {
            if ( (data[16].fvalue < 0.14325812)) {
              sum += (float)0.016534436;
            } else {
              sum += (float)0.10509866;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.55073392)) {
        if ( (data[5].fvalue < 0.35914171)) {
          if (  (data[4].fvalue < 1.199861)) {
            sum += (float)0.085166551;
          } else {
            if (  (data[4].fvalue < 2.7510033)) {
              sum += (float)-0.0049648024;
            } else {
              sum += (float)-0.051875785;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.60777462)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.025132369;
            } else {
              sum += (float)-0.013030133;
            }
          } else {
            if ( (data[5].fvalue < 0.45515281)) {
              sum += (float)0.096905202;
            } else {
              sum += (float)0.0039094249;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.80949664)) {
          if ( (data[6].fvalue < 0.091148868)) {
            if ( (data[13].fvalue < 0.038489684)) {
              sum += (float)-0.0079823537;
            } else {
              sum += (float)0.082907073;
            }
          } else {
            if ( (data[14].fvalue < 0.47915864)) {
              sum += (float)-0.051892091;
            } else {
              sum += (float)-0.0098393178;
            }
          }
        } else {
          if ( (data[2].fvalue < 0.67546022)) {
            sum += (float)0.087440267;
          } else {
            sum += (float)0.001494565;
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.05903)) {
    if (  (data[4].fvalue < 1.8945727)) {
      if (  (data[4].fvalue < 1.2257292)) {
        if (  (data[4].fvalue < 1.1034803)) {
          sum += (float)0.037451863;
        } else {
          if ( (data[1].fvalue < 2.3225832)) {
            sum += (float)-0.061134055;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.0095730368)) {
          if ( (data[15].fvalue < 0.49686944)) {
            if ( (data[14].fvalue < 0.086372718)) {
              sum += (float)-0.077248052;
            } else {
              sum += (float)0.0028540979;
            }
          } else {
            sum += (float)0.079961762;
          }
        } else {
          if ( (data[17].fvalue < 0.12944731)) {
            if ( (data[15].fvalue < 0.096406147)) {
              sum += (float)0.010785254;
            } else {
              sum += (float)0.077644572;
            }
          } else {
            if (  (data[4].fvalue < 1.6440353)) {
              sum += (float)0.13698304;
            } else {
              sum += (float)0.032038134;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.018271284)) {
        if (  (data[4].fvalue < 2.2501631)) {
          sum += (float)-0.0025926742;
        } else {
          sum += (float)0.069877863;
        }
      } else {
        if ( (data[2].fvalue < 0.36301714)) {
          if ( (data[3].fvalue < 0.20137656)) {
            sum += (float)-0.088624969;
          } else {
            sum += (float)-0.010112232;
          }
        } else {
          sum += (float)0.010289221;
        }
      }
    }
  } else {
    if ( (data[11].fvalue < 2.5)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.51602352)) {
          if ( (data[8].fvalue < 8.5)) {
            if ( (data[17].fvalue < 0.12567565)) {
              sum += (float)-0.0066785766;
            } else {
              sum += (float)0.0079087028;
            }
          } else {
            if ( (data[14].fvalue < 0.51159459)) {
              sum += (float)0.14697951;
            } else {
              sum += (float)0.017946707;
            }
          }
        } else {
          if ( (data[15].fvalue < 1.0815103)) {
            if ( (data[8].fvalue < 6.5)) {
              sum += (float)0.0067001292;
            } else {
              sum += (float)-0.084922358;
            }
          } else {
            if ( (data[9].fvalue < 1.5)) {
              sum += (float)0.1010551;
            } else {
              sum += (float)0.010625036;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8272128)) {
          if ( (data[3].fvalue < 1.1964734)) {
            if ( (data[2].fvalue < 0.28864348)) {
              sum += (float)-0;
            } else {
              sum += (float)0.087023176;
            }
          } else {
            sum += (float)-0.094798498;
          }
        } else {
          if ( (data[15].fvalue < 0.14739238)) {
            sum += (float)-0.19169436;
          } else {
            sum += (float)-0.012270948;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.63149095)) {
        sum += (float)0.035230789;
      } else {
        if ( (data[17].fvalue < 1.7086128)) {
          if ( (data[3].fvalue < 1.6314912)) {
            if ( (data[14].fvalue < 0.0052731168)) {
              sum += (float)-0.088493615;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.15556759;
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.011681243)) {
    sum += (float)-0.122044;
  } else {
    if ( (data[4].fvalue < 0.62233317)) {
      sum += (float)0.06955374;
    } else {
      if ( (data[14].fvalue < 0.48781005)) {
        if ( (data[15].fvalue < 0.14903972)) {
          if ( (data[15].fvalue < 0.05700551)) {
            if (  (data[4].fvalue < 1.7049466)) {
              sum += (float)-0.0042424453;
            } else {
              sum += (float)0.02058615;
            }
          } else {
            if ( (data[15].fvalue < 0.099942744)) {
              sum += (float)-0.04477147;
            } else {
              sum += (float)-0.0036729926;
            }
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[15].fvalue < 1.9355977)) {
              sum += (float)0.0040089092;
            } else {
              sum += (float)-0.038184304;
            }
          } else {
            if (  (data[4].fvalue < 1.4262199)) {
              sum += (float)0.19342935;
            } else {
              sum += (float)0.022907672;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.33977014)) {
          if ( (data[0].fvalue < 5.1390142)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.045939744;
            } else {
              sum += (float)0.12585939;
            }
          } else {
            sum += (float)-0.00079714338;
          }
        } else {
          if ( (data[16].fvalue < 0.75125355)) {
            if ( (data[0].fvalue < 1.755475)) {
              sum += (float)0.064442478;
            } else {
              sum += (float)-0.0017400073;
            }
          } else {
            sum += (float)0.10826159;
          }
        }
      }
    }
  }
  if ( (data[4].fvalue < 1.3048172)) {
    if ( (data[17].fvalue < 0.11546318)) {
      if ( (data[14].fvalue < 0.070878237)) {
        if ( (data[14].fvalue < 4.347784e-08)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.016833793)) {
              sum += (float)0.010559739;
            } else {
              sum += (float)-0.027905734;
            }
          } else {
            sum += (float)0.31717402;
          }
        } else {
          if ( (data[9].fvalue < 0.5)) {
            if (  (data[4].fvalue < 0.98506445)) {
              sum += (float)-0.0057209432;
            } else {
              sum += (float)-0.059915084;
            }
          } else {
            if ( (data[13].fvalue < 0.010042918)) {
              sum += (float)-0.087963343;
            } else {
              sum += (float)0.064064793;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.120431)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.043570664;
            } else {
              sum += (float)0.082856111;
            }
          } else {
            if (  (data[4].fvalue < 1.1558962)) {
              sum += (float)0.21737011;
            } else {
              sum += (float)0.078858621;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.11171673)) {
            if ( (data[13].fvalue < 0.31557572)) {
              sum += (float)-0.014733882;
            } else {
              sum += (float)0.048961509;
            }
          } else {
            if ( (data[3].fvalue < 0.80311483)) {
              sum += (float)0.11116523;
            } else {
              sum += (float)0.032526184;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.2788811)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[4].fvalue < 1.2129923)) {
            if ( (data[14].fvalue < 0.00080618245)) {
              sum += (float)0.036065541;
            } else {
              sum += (float)0.11348204;
            }
          } else {
            if ( (data[3].fvalue < 0.38496798)) {
              sum += (float)0.047767382;
            } else {
              sum += (float)-0.026021762;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.69939256)) {
            sum += (float)0.19947349;
          } else {
            sum += (float)0.039739963;
          }
        }
      } else {
        if ( (data[17].fvalue < 0.54098344)) {
          if ( (data[3].fvalue < 1.4527164)) {
            if ( (data[5].fvalue < 0.56309831)) {
              sum += (float)-0.009511224;
            } else {
              sum += (float)0.085665502;
            }
          } else {
            sum += (float)-0.028378157;
          }
        } else {
          sum += (float)-0.050370805;
        }
      }
    }
  } else {
    if ( (data[15].fvalue < 0.27412418)) {
      if ( (data[15].fvalue < 0.055089585)) {
        if ( (data[13].fvalue < 1.6413054e-05)) {
          if ( (data[17].fvalue < 0.0059656631)) {
            if ( (data[14].fvalue < 0.28281444)) {
              sum += (float)0.30067894;
            } else {
              sum += (float)0.074889079;
            }
          } else {
            if ( (data[9].fvalue < 0.5)) {
              sum += (float)-0.14398937;
            } else {
              sum += (float)0.058504406;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.55664086)) {
            if ( (data[17].fvalue < 0.2249493)) {
              sum += (float)-0.022189507;
            } else {
              sum += (float)0.0087018479;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.00021138829;
            } else {
              sum += (float)0.076330021;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 1.2652612)) {
          sum += (float)0.076787971;
        } else {
          if ( (data[16].fvalue < 0.0058846436)) {
            if ( (data[17].fvalue < 1.307857)) {
              sum += (float)-0.042093225;
            } else {
              sum += (float)0.035944488;
            }
          } else {
            if ( (data[16].fvalue < 0.25306827)) {
              sum += (float)0.030423831;
            } else {
              sum += (float)-0.018331802;
            }
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 1.6397231)) {
        if ( (data[13].fvalue < 0.0085434504)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0067090071;
            } else {
              sum += (float)0.051783055;
            }
          } else {
            if ( (data[13].fvalue < 0.0041579213)) {
              sum += (float)-0.183771;
            } else {
              sum += (float)-0.043810867;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6174369)) {
            if ( (data[15].fvalue < 0.78943443)) {
              sum += (float)0.040388677;
            } else {
              sum += (float)-0.030300751;
            }
          } else {
            if ( (data[15].fvalue < 0.66563851)) {
              sum += (float)-0.0087049389;
            } else {
              sum += (float)0.015819769;
            }
          }
        }
      } else {
        sum += (float)-0.077279598;
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
