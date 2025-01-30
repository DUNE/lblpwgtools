#include "BDTReweighter.h"
class GeneratorReweight_numubar_FD_RHC : public BDTReweighter {
 public :
 GeneratorReweight_numubar_FD_RHC() : BDTReweighter(1.) {;}
  ~GeneratorReweight_numubar_FD_RHC() {;}


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
    if ( (data[13].fvalue < 0.04510776)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[15].fvalue < 0.021128722)) {
            if ( (data[14].fvalue < 0.23676243)) {
              sum += (float)-0.17305464;
            } else {
              sum += (float)-0.094717264;
            }
          } else {
            if ( (data[16].fvalue < 0.80104601)) {
              sum += (float)-0.39839289;
            } else {
              sum += (float)-0.23668559;
            }
          }
        } else {
          if ( (data[13].fvalue < 3.2730364e-07)) {
            sum += (float)0.5888688;
          } else {
            if ( (data[14].fvalue < 0.082032964)) {
              sum += (float)-0.19924231;
            } else {
              sum += (float)0.13790444;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[4].fvalue < 1.2976971)) {
            if ( (data[14].fvalue < 0.0072341142)) {
              sum += (float)0.32209915;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[14].fvalue < 0.075232193)) {
              sum += (float)-0.15206951;
            } else {
              sum += (float)0.13329218;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0085427966)) {
            if ( (data[3].fvalue < 0.11994936)) {
              sum += (float)-0.24724893;
            } else {
              sum += (float)-0.39483267;
            }
          } else {
            if ( (data[3].fvalue < 0.2334677)) {
              sum += (float)-0.066392735;
            } else {
              sum += (float)-0.22127104;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 5.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.024037769)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0047488245;
            } else {
              sum += (float)-0.375406;
            }
          } else {
            if (  (data[4].fvalue < 1.0680332)) {
              sum += (float)-0.077881403;
            } else {
              sum += (float)0.16892488;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[13].fvalue < 0.21505252)) {
              sum += (float)-0.12342795;
            } else {
              sum += (float)-0.0029971064;
            }
          } else {
            if (  (data[4].fvalue < 1.0861359)) {
              sum += (float)0.25900337;
            } else {
              sum += (float)0.038394786;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.40555528)) {
          if ( (data[3].fvalue < 0.44768828)) {
            sum += (float)0.35670739;
          } else {
            sum += (float)0.15554702;
          }
        } else {
          sum += (float)0.43330905;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 7.5)) {
      if ( (data[14].fvalue < 0.16170634)) {
        if ( (data[13].fvalue < 0.16367996)) {
          sum += (float)0.51219034;
        } else {
          sum += (float)0.24994206;
        }
      } else {
        if ( (data[15].fvalue < 0.0023372443)) {
          if ( (data[13].fvalue < 0.11364451)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.37464857;
            } else {
              sum += (float)0.11428095;
            }
          } else {
            if ( (data[3].fvalue < 0.99192721)) {
              sum += (float)0.3284688;
            } else {
              sum += (float)0.15780684;
            }
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.31763205)) {
        sum += (float)0.57326972;
      } else {
        if ( (data[15].fvalue < 0.0059484206)) {
          if ( (data[8].fvalue < 9.5)) {
            if ( (data[3].fvalue < 0.9989475)) {
              sum += (float)0.44449469;
            } else {
              sum += (float)0.25171635;
            }
          } else {
            sum += (float)0.54225546;
          }
        } else {
          sum += (float)0.23678884;
        }
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[13].fvalue < 0.15925381)) {
      if ( (data[7].fvalue < 4.5)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.018363386)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.098311618;
            } else {
              sum += (float)-0.0035983766;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.21789271;
            } else {
              sum += (float)0.071464941;
            }
          }
        } else {
          if ( (data[8].fvalue < 4.5)) {
            if ( (data[16].fvalue < 0.12406179)) {
              sum += (float)-0.23710988;
            } else {
              sum += (float)-0.015705327;
            }
          } else {
            if ( (data[14].fvalue < 0.17214885)) {
              sum += (float)0.28775772;
            } else {
              sum += (float)-0.11018448;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.20148697)) {
          if ( (data[14].fvalue < 0.10261257)) {
            sum += (float)0.42374462;
          } else {
            sum += (float)0.25694066;
          }
        } else {
          sum += (float)-0.0041697021;
        }
      }
    } else {
      if ( (data[7].fvalue < 4.5)) {
        if ( (data[13].fvalue < 1.0790532)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.012191522)) {
              sum += (float)0.038323134;
            } else {
              sum += (float)0.19358908;
            }
          } else {
            if ( (data[14].fvalue < 0.11103694)) {
              sum += (float)-0.18179436;
            } else {
              sum += (float)0.052832671;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.0071178135)) {
            if ( (data[16].fvalue < 0.42875957)) {
              sum += (float)-0.10886297;
            } else {
              sum += (float)0.13007796;
            }
          } else {
            if ( (data[14].fvalue < 0.0032189053)) {
              sum += (float)-0.39483765;
            } else {
              sum += (float)-0.18121558;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 6.5)) {
          if ( (data[17].fvalue < 0.0074516535)) {
            if ( (data[14].fvalue < 0.21652848)) {
              sum += (float)0.14941062;
            } else {
              sum += (float)0.26801994;
            }
          } else {
            sum += (float)0.062426057;
          }
        } else {
          sum += (float)0.3448306;
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 5.5)) {
      if ( (data[3].fvalue < 0.65893382)) {
        if (  (data[4].fvalue < 1.1395597)) {
          sum += (float)0.45407262;
        } else {
          if ( (data[8].fvalue < 8.5)) {
            if ( (data[3].fvalue < 0.27719009)) {
              sum += (float)0.28249681;
            } else {
              sum += (float)0.18087959;
            }
          } else {
            sum += (float)0.37633079;
          }
        }
      } else {
        if ( (data[8].fvalue < 8.5)) {
          if ( (data[14].fvalue < 0.67755866)) {
            sum += (float)0.06149739;
          } else {
            sum += (float)0.16753995;
          }
        } else {
          sum += (float)0.26091239;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.8822212)) {
        if ( (data[3].fvalue < 1.276421)) {
          sum += (float)0.45229569;
        } else {
          sum += (float)0.26474401;
        }
      } else {
        sum += (float)0.27574751;
      }
    }
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[13].fvalue < 0.075452752)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.15985247)) {
          if ( (data[5].fvalue < 0.98554981)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.091808245;
            } else {
              sum += (float)0.11968049;
            }
          } else {
            if ( (data[14].fvalue < 9.1378666e-07)) {
              sum += (float)0.31944034;
            } else {
              sum += (float)0.0081293667;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.018490355)) {
            if (  (data[4].fvalue < 1.0461793)) {
              sum += (float)-0.066114217;
            } else {
              sum += (float)0.018521225;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.21810837;
            } else {
              sum += (float)-0.023265854;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[14].fvalue < 0.18649679)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.20848842;
            } else {
              sum += (float)0.16387866;
            }
          } else {
            if ( (data[13].fvalue < 0.040246435)) {
              sum += (float)-0.34346411;
            } else {
              sum += (float)-0.23562361;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.14279866)) {
            sum += (float)0.30276662;
          } else {
            sum += (float)-0.1003686;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.38122165)) {
        if ( (data[7].fvalue < 4.5)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.0081280628)) {
              sum += (float)0.023004819;
            } else {
              sum += (float)0.12152608;
            }
          } else {
            if ( (data[14].fvalue < 0.28554863)) {
              sum += (float)-0.10168409;
            } else {
              sum += (float)0.058903303;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.31090194)) {
            if ( (data[16].fvalue < 0.087023579)) {
              sum += (float)0.028661665;
            } else {
              sum += (float)0.18246351;
            }
          } else {
            if ( (data[16].fvalue < 0.00094904343)) {
              sum += (float)0.31130958;
            } else {
              sum += (float)0.15592206;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[17].fvalue < 0.038670469)) {
              sum += (float)-0.12744361;
            } else {
              sum += (float)-0.29980817;
            }
          } else {
            if ( (data[14].fvalue < 7.6307642e-06)) {
              sum += (float)0.55129135;
            } else {
              sum += (float)0.041652549;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[13].fvalue < 0.34235597)) {
              sum += (float)-0;
            } else {
              sum += (float)0.26049405;
            }
          } else {
            if ( (data[14].fvalue < 0.03498324)) {
              sum += (float)-0.26838988;
            } else {
              sum += (float)-0.055002958;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 7.5)) {
      if ( (data[6].fvalue < 0.11007822)) {
        if (  (data[4].fvalue < 1.0866117)) {
          sum += (float)0.40618986;
        } else {
          sum += (float)0.2146223;
        }
      } else {
        if ( (data[16].fvalue < 0.14277756)) {
          if ( (data[15].fvalue < 0.0024563135)) {
            if ( (data[7].fvalue < 6.5)) {
              sum += (float)0.098494701;
            } else {
              sum += (float)0.27964368;
            }
          } else {
            sum += (float)-0.14541739;
          }
        } else {
          if (  (data[4].fvalue < 1.876953)) {
            if ( (data[3].fvalue < 0.63984358)) {
              sum += (float)0.36774614;
            } else {
              sum += (float)0.17599382;
            }
          } else {
            sum += (float)0.1133546;
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 6.5)) {
        if ( (data[3].fvalue < 0.85284805)) {
          if (  (data[4].fvalue < 1.2122667)) {
            sum += (float)0.37903112;
          } else {
            if ( (data[8].fvalue < 8.5)) {
              sum += (float)0.21403812;
            } else {
              sum += (float)0.29953086;
            }
          }
        } else {
          sum += (float)0.12879789;
        }
      } else {
        sum += (float)0.3843832;
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[13].fvalue < 0.026414402)) {
      if ( (data[13].fvalue < 1.8528158e-07)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.015486522)) {
              sum += (float)-0.031914853;
            } else {
              sum += (float)-0.11662693;
            }
          } else {
            if ( (data[16].fvalue < 0.14686877)) {
              sum += (float)0.026487604;
            } else {
              sum += (float)0.12881698;
            }
          }
        } else {
          sum += (float)0.48124102;
        }
      } else {
        if ( (data[13].fvalue < 0.0042362125)) {
          if ( (data[13].fvalue < 0.0016248655)) {
            sum += (float)-0.31300813;
          } else {
            if ( (data[3].fvalue < 0.20442075)) {
              sum += (float)-0.16082075;
            } else {
              sum += (float)-0.26786342;
            }
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[14].fvalue < 7.8943566e-07)) {
              sum += (float)0.1200963;
            } else {
              sum += (float)-0.14051604;
            }
          } else {
            if ( (data[14].fvalue < 0.0066841803)) {
              sum += (float)-0.23423126;
            } else {
              sum += (float)0.055028796;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.27182117)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.013891053)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.030929007;
            } else {
              sum += (float)0.22126028;
            }
          } else {
            if (  (data[4].fvalue < 1.8692989)) {
              sum += (float)0.19905303;
            } else {
              sum += (float)0.10666107;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.13578473;
            } else {
              sum += (float)0.023174627;
            }
          } else {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.080185995;
            } else {
              sum += (float)0.088127285;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 4.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.1212306;
            } else {
              sum += (float)0.046847388;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.12635632;
            } else {
              sum += (float)-0.069504194;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.13640472)) {
            sum += (float)0.28911397;
          } else {
            if ( (data[13].fvalue < 0.80487239)) {
              sum += (float)0.04601226;
            } else {
              sum += (float)0.21519691;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[14].fvalue < 0.1420472)) {
        sum += (float)0.3331573;
      } else {
        if ( (data[15].fvalue < 0.0022920952)) {
          if ( (data[3].fvalue < 1.0356241)) {
            if (  (data[4].fvalue < 1.1549144)) {
              sum += (float)0.29526848;
            } else {
              sum += (float)0.1488577;
            }
          } else {
            sum += (float)0.037690263;
          }
        } else {
          sum += (float)-0.028983206;
        }
      }
    } else {
      if ( (data[3].fvalue < 1.0660784)) {
        if (  (data[4].fvalue < 1.8769845)) {
          sum += (float)0.34630686;
        } else {
          sum += (float)0.187288;
        }
      } else {
        sum += (float)0.16623257;
      }
    }
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[13].fvalue < 0.26290119)) {
      if ( (data[15].fvalue < 0.0091423756)) {
        if (  (data[4].fvalue < 1.0342207)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.43639642;
            } else {
              sum += (float)-0.055832703;
            }
          } else {
            if ( (data[3].fvalue < 0.44176617)) {
              sum += (float)0.38192734;
            } else {
              sum += (float)-0.0028214029;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.011656278)) {
              sum += (float)-0.0091270013;
            } else {
              sum += (float)0.054734468;
            }
          } else {
            if ( (data[16].fvalue < 0.11210945)) {
              sum += (float)-0.093853645;
            } else {
              sum += (float)0.053066082;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.4066468)) {
          if ( (data[15].fvalue < 0.11452564)) {
            sum += (float)-0.3404136;
          } else {
            sum += (float)-0.16111115;
          }
        } else {
          if ( (data[14].fvalue < 1.0254278)) {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)-0.086207978;
            } else {
              sum += (float)0.039043874;
            }
          } else {
            if ( (data[14].fvalue < 1.2858102)) {
              sum += (float)-0.15276431;
            } else {
              sum += (float)-0.2792325;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.018172584)) {
        if ( (data[14].fvalue < 0.31625313)) {
          if ( (data[16].fvalue < 0.36258471)) {
            if (  (data[4].fvalue < 1.7119997)) {
              sum += (float)0.043253094;
            } else {
              sum += (float)-0.126862;
            }
          } else {
            if ( (data[5].fvalue < 0.49550954)) {
              sum += (float)0.17180707;
            } else {
              sum += (float)-0.0094605451;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.38718504)) {
            if (  (data[4].fvalue < 1.6945468)) {
              sum += (float)0.13162851;
            } else {
              sum += (float)0.0082199415;
            }
          } else {
            if ( (data[14].fvalue < 0.4329313)) {
              sum += (float)0.159106;
            } else {
              sum += (float)0.28359622;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0045578862)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.73252285)) {
              sum += (float)-0.29341727;
            } else {
              sum += (float)-0.043409128;
            }
          } else {
            sum += (float)0.10728473;
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.73115879)) {
              sum += (float)0.12979901;
            } else {
              sum += (float)-0.048981622;
            }
          } else {
            if ( (data[17].fvalue < 0.11429875)) {
              sum += (float)-0.13624541;
            } else {
              sum += (float)-0.0086020343;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 6.5)) {
      if ( (data[6].fvalue < 0.13710839)) {
        if (  (data[4].fvalue < 1.154022)) {
          sum += (float)0.33638203;
        } else {
          sum += (float)0.12789686;
        }
      } else {
        if ( (data[3].fvalue < 0.42138422)) {
          if ( (data[8].fvalue < 7.5)) {
            if ( (data[16].fvalue < 0.074775741)) {
              sum += (float)0.074422337;
            } else {
              sum += (float)0.21369089;
            }
          } else {
            if (  (data[4].fvalue < 1.4364119)) {
              sum += (float)0.24686171;
            } else {
              sum += (float)0.15158665;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.60761386)) {
              sum += (float)0.049705151;
            } else {
              sum += (float)0.20660762;
            }
          } else {
            if ( (data[13].fvalue < 0.064581916)) {
              sum += (float)-0.17300014;
            } else {
              sum += (float)0.041447606;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 1.1476955)) {
        if (  (data[4].fvalue < 1.841152)) {
          sum += (float)0.3291719;
        } else {
          sum += (float)0.17643298;
        }
      } else {
        sum += (float)0.13431795;
      }
    }
  }
  if ( (data[7].fvalue < 5.5)) {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[5].fvalue < 0.12345019)) {
        if (  (data[4].fvalue < 1.245153)) {
          if ( (data[16].fvalue < 0.14931549)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.093411803;
            } else {
              sum += (float)0.0014995668;
            }
          } else {
            sum += (float)0.075608961;
          }
        } else {
          if (  (data[4].fvalue < 1.8513949)) {
            if (  (data[4].fvalue < 1.4464438)) {
              sum += (float)0.043911621;
            } else {
              sum += (float)0.10661639;
            }
          } else {
            if ( (data[16].fvalue < 0.7189849)) {
              sum += (float)-0.090035491;
            } else {
              sum += (float)0.035291493;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.3646122)) {
          if ( (data[4].fvalue < 1.0340135)) {
            if ( (data[14].fvalue < 5.7198115e-08)) {
              sum += (float)0.20880949;
            } else {
              sum += (float)-0.061811339;
            }
          } else {
            if ( (data[17].fvalue < 0.032957632)) {
              sum += (float)0.00066385895;
            } else {
              sum += (float)-0.055771448;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.00087761524)) {
            if ( (data[14].fvalue < 0.031981636)) {
              sum += (float)0.26618955;
            } else {
              sum += (float)0.087135114;
            }
          } else {
            if ( (data[14].fvalue < 0.046762675)) {
              sum += (float)-0.059457809;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.083062679)) {
        if ( (data[0].fvalue < 3.9839916)) {
          sum += (float)0.37306502;
        } else {
          sum += (float)0.20124097;
        }
      } else {
        if ( (data[16].fvalue < 0.14795598)) {
          if ( (data[15].fvalue < 0.005471291)) {
            if ( (data[14].fvalue < 0.53023326)) {
              sum += (float)-0;
            } else {
              sum += (float)0.11544953;
            }
          } else {
            if ( (data[15].fvalue < 0.17005149)) {
              sum += (float)-0.22344856;
            } else {
              sum += (float)-0.05713151;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7528443)) {
            if ( (data[3].fvalue < 0.80217558)) {
              sum += (float)0.29206368;
            } else {
              sum += (float)0.079540439;
            }
          } else {
            if ( (data[16].fvalue < 0.59060764)) {
              sum += (float)-0.014855156;
            } else {
              sum += (float)0.16036287;
            }
          }
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.12964693)) {
      sum += (float)0.31312886;
    } else {
      if ( (data[7].fvalue < 8.5)) {
        if ( (data[3].fvalue < 1.1453674)) {
          if (  (data[4].fvalue < 1.9170171)) {
            if (  (data[4].fvalue < 1.2760684)) {
              sum += (float)0.24523975;
            } else {
              sum += (float)0.14443654;
            }
          } else {
            sum += (float)0.00037975007;
          }
        } else {
          sum += (float)-0;
        }
      } else {
        sum += (float)0.29056165;
      }
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[7].fvalue < 4.5)) {
      if ( (data[14].fvalue < 1.3972008)) {
        if ( (data[13].fvalue < 0.18446559)) {
          if ( (data[5].fvalue < 1.1832836)) {
            if ( (data[14].fvalue < 0.184127)) {
              sum += (float)-0.033120539;
            } else {
              sum += (float)0.00059561402;
            }
          } else {
            if ( (data[13].fvalue < 1.8775232e-06)) {
              sum += (float)0.094520755;
            } else {
              sum += (float)-0.011212365;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.38953048)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.040274635;
            } else {
              sum += (float)-0.045830123;
            }
          } else {
            if ( (data[13].fvalue < 0.43777168)) {
              sum += (float)0.067058027;
            } else {
              sum += (float)0.20413433;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.44388282)) {
          if ( (data[16].fvalue < 0.0014120035)) {
            if (  (data[4].fvalue < 1.9218532)) {
              sum += (float)-0.017433599;
            } else {
              sum += (float)-0.12223981;
            }
          } else {
            if (  (data[4].fvalue < 2.0343752)) {
              sum += (float)-0.14913552;
            } else {
              sum += (float)-0.33300769;
            }
          }
        } else {
          sum += (float)0.001671562;
        }
      }
    } else {
      if ( (data[6].fvalue < 0.16576958)) {
        if ( (data[8].fvalue < 4.5)) {
          sum += (float)0.11141282;
        } else {
          sum += (float)0.273747;
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)0.25563782;
        } else {
          if ( (data[7].fvalue < 6.5)) {
            if ( (data[3].fvalue < 0.35445133)) {
              sum += (float)0.075121805;
            } else {
              sum += (float)-0.018262088;
            }
          } else {
            if ( (data[7].fvalue < 7.5)) {
              sum += (float)0.075762883;
            } else {
              sum += (float)0.17402874;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.20689017)) {
      sum += (float)0.29945627;
    } else {
      if ( (data[8].fvalue < 10.5)) {
        if ( (data[17].fvalue < 0.016590912)) {
          if ( (data[6].fvalue < 0.19384195)) {
            sum += (float)0.20347922;
          } else {
            if ( (data[14].fvalue < 0.60815877)) {
              sum += (float)0.036815897;
            } else {
              sum += (float)0.13748173;
            }
          }
        } else {
          sum += (float)-0.022082781;
        }
      } else {
        sum += (float)0.26426739;
      }
    }
  }
  if ( (data[7].fvalue < 6.5)) {
    if ( (data[15].fvalue < 0.019041901)) {
      if ( (data[12].fvalue < 0.5)) {
        if ( (data[8].fvalue < 5.5)) {
          if (  (data[4].fvalue < 1.3185351)) {
            if ( (data[5].fvalue < 1.1657438)) {
              sum += (float)-0.017488398;
            } else {
              sum += (float)0.042119779;
            }
          } else {
            if ( (data[17].fvalue < 0.022235624)) {
              sum += (float)0.043584038;
            } else {
              sum += (float)-0.016238315;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1198847)) {
            if ( (data[3].fvalue < 0.8708716)) {
              sum += (float)0.28790009;
            } else {
              sum += (float)-0.05914183;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.18216506;
            } else {
              sum += (float)0.028771784;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7538347)) {
          if ( (data[3].fvalue < 0.39125592)) {
            sum += (float)0.17511497;
          } else {
            sum += (float)-0.13425824;
          }
        } else {
          sum += (float)-0.40876517;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.000740733)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.35173067)) {
            sum += (float)-0.1357287;
          } else {
            sum += (float)-0.0067445189;
          }
        } else {
          if (  (data[4].fvalue < 1.6248671)) {
            sum += (float)-0.32042927;
          } else {
            sum += (float)-0.12353965;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.0056711202)) {
            if ( (data[16].fvalue < 0.94099319)) {
              sum += (float)-0.16680555;
            } else {
              sum += (float)-0.0073743016;
            }
          } else {
            if ( (data[14].fvalue < 0.018448044)) {
              sum += (float)-0.14690088;
            } else {
              sum += (float)0.072793856;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.15045416)) {
              sum += (float)-0.044445869;
            } else {
              sum += (float)0.083004147;
            }
          } else {
            if ( (data[15].fvalue < 0.098775566)) {
              sum += (float)-0.15216497;
            } else {
              sum += (float)-0.028996075;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 1.032253)) {
      if (  (data[4].fvalue < 1.7661433)) {
        if ( (data[8].fvalue < 8.5)) {
          sum += (float)0.196073;
        } else {
          sum += (float)0.30111814;
        }
      } else {
        sum += (float)0.10620641;
      }
    } else {
      sum += (float)0.040643502;
    }
  }
  if ( (data[8].fvalue < 8.5)) {
    if ( (data[5].fvalue < 0.079763368)) {
      if (  (data[4].fvalue < 1.8977656)) {
        if (  (data[4].fvalue < 1.4662845)) {
          if ( (data[16].fvalue < 0.0044501917)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.063946716;
            } else {
              sum += (float)0.12162208;
            }
          } else {
            if ( (data[14].fvalue < 0.026204396)) {
              sum += (float)-0.020583043;
            } else {
              sum += (float)0.14007731;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.082118668)) {
            if ( (data[14].fvalue < 0.014127521)) {
              sum += (float)0.07103987;
            } else {
              sum += (float)0.17565116;
            }
          } else {
            if ( (data[17].fvalue < 0.023181539)) {
              sum += (float)0.078135744;
            } else {
              sum += (float)0.0043433565;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.82286346)) {
          if ( (data[16].fvalue < 0.056165002)) {
            sum += (float)0.13938761;
          } else {
            if ( (data[14].fvalue < 0.12278426)) {
              sum += (float)-0.089497194;
            } else {
              sum += (float)0.018260561;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.0021324155)) {
            sum += (float)-0.078379817;
          } else {
            sum += (float)-0.22030155;
          }
        }
      }
    } else {
      if ( (data[15].fvalue < 0.0064577097)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.18142855)) {
              sum += (float)-0.019348856;
            } else {
              sum += (float)0.0086509408;
            }
          } else {
            if (  (data[4].fvalue < 1.6969653)) {
              sum += (float)-0.014413252;
            } else {
              sum += (float)-0.31921822;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1114328)) {
            if ( (data[3].fvalue < 0.60744768)) {
              sum += (float)0.31252474;
            } else {
              sum += (float)-0.0046247938;
            }
          } else {
            if ( (data[16].fvalue < 0.11197765)) {
              sum += (float)-0.023007985;
            } else {
              sum += (float)0.11715779;
            }
          }
        }
      } else {
        if ( (data[4].fvalue < 1.4331208)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.18067068)) {
              sum += (float)-0.21606062;
            } else {
              sum += (float)-0.044174202;
            }
          } else {
            sum += (float)-0.25628382;
          }
        } else {
          if ( (data[17].fvalue < 0.055300694)) {
            if ( (data[5].fvalue < 0.3438471)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.072192751;
            }
          } else {
            if (  (data[4].fvalue < 1.6941357)) {
              sum += (float)0.0007585428;
            } else {
              sum += (float)-0.11640353;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.29057217)) {
      sum += (float)0.27807832;
    } else {
      if ( (data[8].fvalue < 10.5)) {
        if ( (data[3].fvalue < 0.48978662)) {
          sum += (float)0.10729356;
        } else {
          sum += (float)0.0013786909;
        }
      } else {
        sum += (float)0.20299014;
      }
    }
  }
  if ( (data[7].fvalue < 7.5)) {
    if ( (data[14].fvalue < 1.5917432)) {
      if ( (data[5].fvalue < 0.2189565)) {
        if (  (data[4].fvalue < 1.2889607)) {
          if ( (data[16].fvalue < 0.11178151)) {
            if (  (data[4].fvalue < 1.1476018)) {
              sum += (float)0.026502151;
            } else {
              sum += (float)-0.05447584;
            }
          } else {
            if ( (data[13].fvalue < 0.0064576394)) {
              sum += (float)0.026367867;
            } else {
              sum += (float)0.1700145;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.05108577)) {
            if ( (data[16].fvalue < 0.018208142)) {
              sum += (float)0.12987368;
            } else {
              sum += (float)-0.023360904;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.065153934;
            } else {
              sum += (float)0.0034329155;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.039519183)) {
          if ( (data[14].fvalue < 2.1389603e-09)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.10327899;
            } else {
              sum += (float)0.50103056;
            }
          } else {
            if ( (data[13].fvalue < 1.4744163e-07)) {
              sum += (float)0.017538959;
            } else {
              sum += (float)-0.035817556;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.010249738)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.10076021;
            } else {
              sum += (float)0.0014438562;
            }
          } else {
            if ( (data[3].fvalue < 0.88858563)) {
              sum += (float)0.033923287;
            } else {
              sum += (float)-0.037861187;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.010109294)) {
        if ( (data[4].fvalue < 1.9414035)) {
          sum += (float)-0;
        } else {
          sum += (float)-0.12512477;
        }
      } else {
        if ( (data[17].fvalue < 0.28568164)) {
          sum += (float)-0.28693631;
        } else {
          sum += (float)-0.062690251;
        }
      }
    }
  } else {
    if ( (data[13].fvalue < 0.25277522)) {
      sum += (float)0.26850796;
    } else {
      if ( (data[6].fvalue < 0.46199828)) {
        sum += (float)0.167575;
      } else {
        sum += (float)0.059547983;
      }
    }
  }
  if ( (data[7].fvalue < 4.5)) {
    if ( (data[6].fvalue < 0.88731885)) {
      if ( (data[13].fvalue < 1.6132991)) {
        if ( (data[13].fvalue < 0.24884224)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.13702881)) {
              sum += (float)-0.021912444;
            } else {
              sum += (float)0.012171176;
            }
          } else {
            if ( (data[3].fvalue < 0.40311456)) {
              sum += (float)-0.010192794;
            } else {
              sum += (float)-0.10584148;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.28202537)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.028037373;
            } else {
              sum += (float)-0.12132479;
            }
          } else {
            if ( (data[13].fvalue < 0.34735531)) {
              sum += (float)0.035557393;
            } else {
              sum += (float)0.11652827;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.65550804)) {
          sum += (float)-0.092672288;
        } else {
          sum += (float)-0.24430074;
        }
      }
    } else {
      if ( (data[3].fvalue < 0.67176843)) {
        sum += (float)0.04750466;
      } else {
        sum += (float)0.24633414;
      }
    }
  } else {
    if ( (data[6].fvalue < 0.17385107)) {
      if ( (data[0].fvalue < 3.4186444)) {
        sum += (float)0.26148281;
      } else {
        sum += (float)0.11953764;
      }
    } else {
      if ( (data[10].fvalue < 1.5)) {
        if ( (data[7].fvalue < 8.5)) {
          if ( (data[13].fvalue < 0.47571424)) {
            if ( (data[16].fvalue < 0.19054654)) {
              sum += (float)-0.015274874;
            } else {
              sum += (float)0.13224679;
            }
          } else {
            if ( (data[16].fvalue < 0.0035739553)) {
              sum += (float)0.18687344;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          sum += (float)0.20532195;
        }
      } else {
        sum += (float)-0.13677004;
      }
    }
  }
  if ( (data[8].fvalue < 6.5)) {
    if ( (data[15].fvalue < 0.040537626)) {
      if ( (data[6].fvalue < 0.011888636)) {
        sum += (float)-0.24230359;
      } else {
        if ( (data[16].fvalue < 0.42509496)) {
          if ( (data[14].fvalue < 1.1614692)) {
            if ( (data[13].fvalue < 1.3997946)) {
              sum += (float)0.0010031159;
            } else {
              sum += (float)-0.13302521;
            }
          } else {
            if ( (data[16].fvalue < 0.038241692)) {
              sum += (float)-0.019349616;
            } else {
              sum += (float)-0.19328605;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.033615023)) {
            if ( (data[13].fvalue < 0.28832346)) {
              sum += (float)-0.070371859;
            } else {
              sum += (float)0.075458288;
            }
          } else {
            if ( (data[17].fvalue < 0.026320128)) {
              sum += (float)0.10772863;
            } else {
              sum += (float)-0.0044256281;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.4673033)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)-0.13846277;
          } else {
            sum += (float)-0.035624877;
          }
        } else {
          sum += (float)-0.1897832;
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.91994154)) {
            if ( (data[17].fvalue < 0.046236798)) {
              sum += (float)-0.030243251;
            } else {
              sum += (float)-0.12858872;
            }
          } else {
            sum += (float)-0.20589961;
          }
        } else {
          if ( (data[16].fvalue < 0.002152778)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.0092142178;
            } else {
              sum += (float)-0.10829996;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.050364811;
            } else {
              sum += (float)-0.033433713;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.23055586)) {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[14].fvalue < 0.37071532)) {
          sum += (float)0.032225195;
        } else {
          sum += (float)0.16577391;
        }
      } else {
        if (  (data[4].fvalue < 1.2337558)) {
          sum += (float)0.26189148;
        } else {
          sum += (float)0.14804226;
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        sum += (float)0.14812213;
      } else {
        if ( (data[14].fvalue < 0.18687466)) {
          sum += (float)0.16127296;
        } else {
          if ( (data[13].fvalue < 0.068931475)) {
            sum += (float)-0.13825677;
          } else {
            if ( (data[17].fvalue < 0.0017507609)) {
              sum += (float)0.047054064;
            } else {
              sum += (float)-0.067888878;
            }
          }
        }
      }
    }
  }
  if ( (data[8].fvalue < 9.5)) {
    if ( (data[13].fvalue < 0.035894506)) {
      if ( (data[13].fvalue < 1.4744163e-07)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.01666048)) {
              sum += (float)-0.0077652447;
            } else {
              sum += (float)-0.094586022;
            }
          } else {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.01692329;
            } else {
              sum += (float)0.10767236;
            }
          }
        } else {
          sum += (float)0.41550317;
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.035322428)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.25553554;
            } else {
              sum += (float)-0.060675517;
            }
          } else {
            if (  (data[4].fvalue < 1.0653619)) {
              sum += (float)-0.036367152;
            } else {
              sum += (float)0.1842694;
            }
          }
        } else {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[3].fvalue < 0.37518865)) {
              sum += (float)-0.070536412;
            } else {
              sum += (float)-0.19723406;
            }
          } else {
            sum += (float)0.21855016;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.30717549)) {
        if ( (data[16].fvalue < 0.00016288321)) {
          if (  (data[4].fvalue < 1.1397085)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.057367615;
            } else {
              sum += (float)0.24609928;
            }
          } else {
            if ( (data[17].fvalue < 0.14847055)) {
              sum += (float)-0.053657562;
            } else {
              sum += (float)0.14862721;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.6876564)) {
            if ( (data[16].fvalue < 0.25233629)) {
              sum += (float)0.100742;
            } else {
              sum += (float)0.17972292;
            }
          } else {
            if ( (data[16].fvalue < 0.45932442)) {
              sum += (float)-0.063616849;
            } else {
              sum += (float)0.072336063;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.016114369)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.054993544;
            } else {
              sum += (float)0.076128185;
            }
          } else {
            if ( (data[14].fvalue < 0.17054501)) {
              sum += (float)0.057360481;
            } else {
              sum += (float)0.12421018;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.062409777)) {
            if ( (data[14].fvalue < 0.043912418)) {
              sum += (float)-0.2509861;
            } else {
              sum += (float)-0.14064637;
            }
          } else {
            if ( (data[16].fvalue < 0.7565378)) {
              sum += (float)-0.021380162;
            } else {
              sum += (float)0.11809971;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.48034975)) {
      sum += (float)0.23844506;
    } else {
      sum += (float)0.066012219;
    }
  }
  if ( (data[8].fvalue < 7.5)) {
    if ( (data[13].fvalue < 0.14304064)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.28046116)) {
          if ( (data[16].fvalue < 0.043258928)) {
            if ( (data[14].fvalue < 5.7198115e-08)) {
              sum += (float)0.28375188;
            } else {
              sum += (float)-0.010851541;
            }
          } else {
            if ( (data[13].fvalue < 0.0063333064)) {
              sum += (float)-0.06192435;
            } else {
              sum += (float)0.013510469;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.80452138)) {
            if ( (data[17].fvalue < 0.97345734)) {
              sum += (float)0.0044109696;
            } else {
              sum += (float)0.16985542;
            }
          } else {
            if ( (data[5].fvalue < 0.37533745)) {
              sum += (float)0.23128185;
            } else {
              sum += (float)0.0049398961;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.20373827)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.032028824)) {
              sum += (float)-0.23171079;
            } else {
              sum += (float)-0.021079857;
            }
          } else {
            if ( (data[6].fvalue < 0.065170974)) {
              sum += (float)0.36374873;
            } else {
              sum += (float)0.03156051;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.050610185)) {
            if (  (data[4].fvalue < 1.1980083)) {
              sum += (float)-0.082188465;
            } else {
              sum += (float)-0.23434287;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.0040400089;
            } else {
              sum += (float)-0.11936707;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.019020859)) {
        if ( (data[14].fvalue < 0.42629215)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.027160948;
            } else {
              sum += (float)0.085867278;
            }
          } else {
            if ( (data[14].fvalue < 0.070327841)) {
              sum += (float)-0.15093268;
            } else {
              sum += (float)0.0050583654;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.0050807032)) {
            if ( (data[13].fvalue < 0.29622221)) {
              sum += (float)0.057609249;
            } else {
              sum += (float)0.20569028;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.11144688;
            } else {
              sum += (float)-0.047743082;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.0046945717)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.63532591)) {
              sum += (float)-0.20301552;
            } else {
              sum += (float)-0.046223693;
            }
          } else {
            sum += (float)0.045798846;
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[13].fvalue < 0.88544005)) {
              sum += (float)0.057478871;
            } else {
              sum += (float)-0.078744963;
            }
          } else {
            if ( (data[17].fvalue < 0.15428299)) {
              sum += (float)-0.17126018;
            } else {
              sum += (float)-0.00070002768;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.16906865)) {
      sum += (float)0.23900016;
    } else {
      if ( (data[6].fvalue < 0.4318867)) {
        if ( (data[8].fvalue < 9.5)) {
          if ( (data[14].fvalue < 0.3567431)) {
            sum += (float)-0;
          } else {
            if ( (data[3].fvalue < 0.48985708)) {
              sum += (float)0.14552531;
            } else {
              sum += (float)0.032911029;
            }
          }
        } else {
          sum += (float)0.16815071;
        }
      } else {
        if ( (data[7].fvalue < 4.5)) {
          sum += (float)0.059911918;
        } else {
          sum += (float)-0.057822913;
        }
      }
    }
  }
  if ( (data[8].fvalue < 4.5)) {
    if (  (data[4].fvalue < 1.0405089)) {
      if ( (data[8].fvalue < 0.5)) {
        if ( (data[13].fvalue < 0.06810496)) {
          sum += (float)-0.41507164;
        } else {
          sum += (float)-0.21664414;
        }
      } else {
        if ( (data[14].fvalue < 2.1389603e-09)) {
          sum += (float)0.40972397;
        } else {
          if ( (data[13].fvalue < 3.3036076e-06)) {
            if ( (data[14].fvalue < 0.038824268)) {
              sum += (float)0.17943141;
            } else {
              sum += (float)-0.028886443;
            }
          } else {
            if ( (data[14].fvalue < 0.023559913)) {
              sum += (float)-0.19271821;
            } else {
              sum += (float)-0.044627309;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[16].fvalue < 0.039978556)) {
          if ( (data[14].fvalue < 7.8588123e-08)) {
            if ( (data[17].fvalue < 0.040222786)) {
              sum += (float)0.29859725;
            } else {
              sum += (float)-0.1785771;
            }
          } else {
            if ( (data[14].fvalue < 0.010458143)) {
              sum += (float)-0.12377543;
            } else {
              sum += (float)0.037160061;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.034969196)) {
            if ( (data[14].fvalue < 0.12213093)) {
              sum += (float)-0.010756047;
            } else {
              sum += (float)0.040313553;
            }
          } else {
            if ( (data[13].fvalue < 0.0098987948)) {
              sum += (float)-0.14333351;
            } else {
              sum += (float)-0.03009147;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[14].fvalue < 0.10520738)) {
              sum += (float)-0.074358784;
            } else {
              sum += (float)0.069768384;
            }
          } else {
            if (  (data[4].fvalue < 1.1592126)) {
              sum += (float)0.021215195;
            } else {
              sum += (float)-0.05537178;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.034093335)) {
            if ( (data[14].fvalue < 0.065648347)) {
              sum += (float)0.019417472;
            } else {
              sum += (float)-0.18247916;
            }
          } else {
            if (  (data[4].fvalue < 1.1064129)) {
              sum += (float)0.098098867;
            } else {
              sum += (float)-0.043856137;
            }
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.1174854)) {
      if ( (data[3].fvalue < 0.66904294)) {
        if ( (data[3].fvalue < 0.27518368)) {
          sum += (float)0.31457928;
        } else {
          if (  (data[4].fvalue < 1.0345843)) {
            sum += (float)0.24069819;
          } else {
            sum += (float)0.093705669;
          }
        }
      } else {
        if ( (data[3].fvalue < 1.2553226)) {
          sum += (float)0.069868609;
        } else {
          sum += (float)-0.15404882;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.088585719)) {
        sum += (float)0.17158222;
      } else {
        if ( (data[13].fvalue < 0.065327689)) {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.52344352)) {
              sum += (float)-0.019113477;
            } else {
              sum += (float)0.14016815;
            }
          } else {
            if ( (data[16].fvalue < 0.015984803)) {
              sum += (float)-0.13036823;
            } else {
              sum += (float)0.042261012;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.18948326)) {
            if ( (data[16].fvalue < 0.026678622)) {
              sum += (float)-0.16104615;
            } else {
              sum += (float)0.052979778;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.10982793;
            } else {
              sum += (float)0.01657898;
            }
          }
        }
      }
    }
  }
  if ( (data[12].fvalue < 0.5)) {
    if ( (data[16].fvalue < 0.15809914)) {
      if ( (data[16].fvalue < 0.048599429)) {
        if ( (data[17].fvalue < 0.14141133)) {
          if ( (data[14].fvalue < 5.7499676e-07)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.0065038134;
            } else {
              sum += (float)0.37301457;
            }
          } else {
            if ( (data[13].fvalue < 0.3647005)) {
              sum += (float)-0.013748762;
            } else {
              sum += (float)0.069293737;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.42746878)) {
            if ( (data[14].fvalue < 0.03086349)) {
              sum += (float)-0.080141306;
            } else {
              sum += (float)0.096438356;
            }
          } else {
            if ( (data[17].fvalue < 0.4892714)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.12504923;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.16216013)) {
          if ( (data[13].fvalue < 0.020292768)) {
            if ( (data[14].fvalue < 0.085093506)) {
              sum += (float)-0.10226994;
            } else {
              sum += (float)0.028454712;
            }
          } else {
            if ( (data[14].fvalue < 0.036911871)) {
              sum += (float)0.0087467385;
            } else {
              sum += (float)0.10473281;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.4317822)) {
            if ( (data[16].fvalue < 0.12340529)) {
              sum += (float)-0.061821472;
            } else {
              sum += (float)0.0059730695;
            }
          } else {
            if ( (data[15].fvalue < 0.00046328822)) {
              sum += (float)-0.1114711;
            } else {
              sum += (float)-0.042096712;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.038976401)) {
        if ( (data[5].fvalue < 0.4352901)) {
          if ( (data[14].fvalue < 0.023980999)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.00089795835;
            } else {
              sum += (float)0.12461547;
            }
          } else {
            if ( (data[3].fvalue < 0.18520698)) {
              sum += (float)0.13738568;
            } else {
              sum += (float)0.047263935;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.27950412)) {
            if ( (data[13].fvalue < 0.0045380262)) {
              sum += (float)-0.028683482;
            } else {
              sum += (float)0.053620059;
            }
          } else {
            if ( (data[13].fvalue < 0.37498119)) {
              sum += (float)-0.074304193;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 1.0566509)) {
          if ( (data[14].fvalue < 0.74735051)) {
            if ( (data[17].fvalue < 0.16669343)) {
              sum += (float)-0.11088062;
            } else {
              sum += (float)-0.01647873;
            }
          } else {
            if ( (data[17].fvalue < 0.36395672)) {
              sum += (float)-0.22518052;
            } else {
              sum += (float)-0.067117229;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.051325105)) {
            sum += (float)0.0038152644;
          } else {
            sum += (float)0.11132211;
          }
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.7844391)) {
      if ( (data[3].fvalue < 0.48801839)) {
        sum += (float)0.10631377;
      } else {
        sum += (float)-0.084916182;
      }
    } else {
      sum += (float)-0.3087683;
    }
  }
  if ( (data[6].fvalue < 0.013532786)) {
    sum += (float)-0.17432863;
  } else {
    if ( (data[15].fvalue < 0.016230844)) {
      if ( (data[5].fvalue < 0.052310042)) {
        if ( (data[14].fvalue < 0.8725512)) {
          if (  (data[4].fvalue < 1.2077186)) {
            if ( (data[13].fvalue < 0.010034484)) {
              sum += (float)-0.035417609;
            } else {
              sum += (float)0.0502831;
            }
          } else {
            if (  (data[4].fvalue < 1.7940905)) {
              sum += (float)0.057124317;
            } else {
              sum += (float)0.0016780265;
            }
          }
        } else {
          sum += (float)-0.080118455;
        }
      } else {
        if (  (data[4].fvalue < 0.8819679)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.00015442219)) {
              sum += (float)0.10516564;
            } else {
              sum += (float)0.021476567;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.040917184;
            } else {
              sum += (float)0.10734744;
            }
          }
        } else {
          if ( (data[8].fvalue < 10.5)) {
            if ( (data[12].fvalue < 0.5)) {
              sum += (float)-0.00098517607;
            } else {
              sum += (float)-0.10491195;
            }
          } else {
            sum += (float)0.17962661;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.00064290874)) {
        if ( (data[15].fvalue < 0.14594841)) {
          if ( (data[7].fvalue < 0.5)) {
            sum += (float)-0.075237215;
          } else {
            sum += (float)-0.20296603;
          }
        } else {
          if ( (data[14].fvalue < 0.51519918)) {
            sum += (float)-0.084677815;
          } else {
            sum += (float)-0.0055710496;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.75591838)) {
            if ( (data[13].fvalue < 0.011071885)) {
              sum += (float)-0.057965349;
            } else {
              sum += (float)0.0026142311;
            }
          } else {
            sum += (float)-0.15098314;
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.22929858)) {
              sum += (float)-0.0024618926;
            } else {
              sum += (float)0.054891359;
            }
          } else {
            if ( (data[15].fvalue < 0.11609127)) {
              sum += (float)-0.092112765;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  }
  if ( (data[1].fvalue < 0.28534305)) {
    if ( (data[3].fvalue < 0.71785438)) {
      if ( (data[5].fvalue < 0.17355615)) {
        if ( (data[0].fvalue < 1.4750652)) {
          sum += (float)0.14778449;
        } else {
          sum += (float)0.065758839;
        }
      } else {
        sum += (float)-0.0053084828;
      }
    } else {
      sum += (float)0.1946052;
    }
  } else {
    if ( (data[8].fvalue < 4.5)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[13].fvalue < 0.011636779)) {
          if ( (data[13].fvalue < 3.2367284e-06)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0.0066565382;
            } else {
              sum += (float)0.36771044;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.013092651;
            } else {
              sum += (float)-0.13918808;
            }
          }
        } else {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[16].fvalue < 0.00065984146)) {
              sum += (float)-0.36081308;
            } else {
              sum += (float)-0.034532152;
            }
          } else {
            if ( (data[16].fvalue < 0.13033323)) {
              sum += (float)0.020750048;
            } else {
              sum += (float)0.10805626;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[10].fvalue < 0.5)) {
            sum += (float)-0.19930589;
          } else {
            if ( (data[5].fvalue < 0.38403428)) {
              sum += (float)0.19478062;
            } else {
              sum += (float)0.075740814;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.07013417)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.0044209659;
            } else {
              sum += (float)0.27856728;
            }
          } else {
            if ( (data[14].fvalue < 0.012160091)) {
              sum += (float)-0.20902118;
            } else {
              sum += (float)-0.044126816;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.13958248)) {
        if (  (data[4].fvalue < 1.1492968)) {
          if ( (data[3].fvalue < 0.18413222)) {
            sum += (float)0.30528411;
          } else {
            if ( (data[0].fvalue < 2.9620748)) {
              sum += (float)0.24553005;
            } else {
              sum += (float)0.080630861;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.29435596)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.1065773;
            } else {
              sum += (float)0.070209578;
            }
          } else {
            sum += (float)0.12266843;
          }
        }
      } else {
        if ( (data[3].fvalue < 1.4197757)) {
          if ( (data[3].fvalue < 0.16407731)) {
            if ( (data[16].fvalue < 0.069345124)) {
              sum += (float)0.053581543;
            } else {
              sum += (float)0.17237002;
            }
          } else {
            if ( (data[14].fvalue < 0.48348305)) {
              sum += (float)-0.023912782;
            } else {
              sum += (float)0.044244498;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.51181149)) {
            sum += (float)-0;
          } else {
            if ( (data[14].fvalue < 0.66552329)) {
              sum += (float)-0.22484523;
            } else {
              sum += (float)-0.046335064;
            }
          }
        }
      }
    }
  }
  if ( (data[7].fvalue < 7.5)) {
    if ( (data[6].fvalue < 0.039117947)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[14].fvalue < 0.056994844)) {
          if ( (data[13].fvalue < 8.5766951e-05)) {
            if ( (data[6].fvalue < 0.020260207)) {
              sum += (float)-0.061270121;
            } else {
              sum += (float)0.13360605;
            }
          } else {
            if ( (data[14].fvalue < 1.381578e-06)) {
              sum += (float)0.030058593;
            } else {
              sum += (float)-0.1755662;
            }
          }
        } else {
          if ( (data[0].fvalue < 3.0727172)) {
            sum += (float)-0.32242781;
          } else {
            if ( (data[14].fvalue < 0.088221394)) {
              sum += (float)-0.056339495;
            } else {
              sum += (float)-0.21592474;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.004484144)) {
          sum += (float)0.10799348;
        } else {
          sum += (float)0.36353436;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1436584)) {
        if (  (data[4].fvalue < 1.0586183)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[13].fvalue < 3.4483544e-06)) {
              sum += (float)0.0088863485;
            } else {
              sum += (float)-0.04489316;
            }
          } else {
            if ( (data[3].fvalue < 0.74282253)) {
              sum += (float)0.16396594;
            } else {
              sum += (float)-0.020426422;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.35200414)) {
            if ( (data[13].fvalue < 0.0075479299)) {
              sum += (float)0.065032907;
            } else {
              sum += (float)0.18023431;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.037626684;
            } else {
              sum += (float)-0.052101389;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.47360414)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[16].fvalue < 0.05554688)) {
              sum += (float)0.020279605;
            } else {
              sum += (float)-0.016600145;
            }
          } else {
            if ( (data[17].fvalue < 0.16894105)) {
              sum += (float)-0.037215102;
            } else {
              sum += (float)0.051242679;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.31612515)) {
            if ( (data[14].fvalue < 0.17245331)) {
              sum += (float)0.022033541;
            } else {
              sum += (float)0.082968898;
            }
          } else {
            if ( (data[14].fvalue < 0.43690467)) {
              sum += (float)-0.049959753;
            } else {
              sum += (float)0.054162212;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.45624226)) {
      sum += (float)0.17607433;
    } else {
      sum += (float)0.040250901;
    }
  }
  if ( (data[8].fvalue < 5.5)) {
    if ( (data[15].fvalue < 0.038964145)) {
      if ( (data[13].fvalue < 0.20386738)) {
        if ( (data[4].fvalue < 0.8515799)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[4].fvalue < 0.76567197)) {
              sum += (float)0.16681197;
            } else {
              sum += (float)0.0849168;
            }
          } else {
            if ( (data[4].fvalue < 0.79110408)) {
              sum += (float)-0.070521109;
            } else {
              sum += (float)0.0060708905;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0272685)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.31473354;
            } else {
              sum += (float)-0.013528821;
            }
          } else {
            if (  (data[4].fvalue < 1.1230872)) {
              sum += (float)0.040520236;
            } else {
              sum += (float)-0.0073881969;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 1.039973)) {
          if (  (data[4].fvalue < 1.7905705)) {
            if ( (data[14].fvalue < 0.21978357)) {
              sum += (float)0.011399763;
            } else {
              sum += (float)0.064323999;
            }
          } else {
            if ( (data[17].fvalue < 0.89453834)) {
              sum += (float)-0.077087626;
            } else {
              sum += (float)0.09798108;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7715342)) {
            sum += (float)-0.066997573;
          } else {
            if ( (data[13].fvalue < 0.43087456)) {
              sum += (float)0.07517904;
            } else {
              sum += (float)0.22311774;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.044930011)) {
        if (  (data[4].fvalue < 1.8253834)) {
          sum += (float)-0.17182469;
        } else {
          sum += (float)-0.047209017;
        }
      } else {
        if ( (data[14].fvalue < 0.85993207)) {
          if ( (data[16].fvalue < 0.00073680427)) {
            if ( (data[14].fvalue < 0.49236593)) {
              sum += (float)-0.10670338;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[17].fvalue < 0.33987552)) {
              sum += (float)0.0062150774;
            } else {
              sum += (float)-0.074725784;
            }
          }
        } else {
          if ( (data[0].fvalue < 4.4099822)) {
            if ( (data[5].fvalue < 0.22118708)) {
              sum += (float)-0.17199871;
            } else {
              sum += (float)-0.073426969;
            }
          } else {
            sum += (float)-0.0070921029;
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.14578247)) {
      if (  (data[4].fvalue < 1.3367143)) {
        if ( (data[14].fvalue < 0.26393223)) {
          sum += (float)0.077012092;
        } else {
          sum += (float)0.25482145;
        }
      } else {
        if ( (data[14].fvalue < 0.46105498)) {
          sum += (float)0.015816784;
        } else {
          sum += (float)0.10994346;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.0026876377)) {
        if ( (data[14].fvalue < 0.66458935)) {
          if ( (data[16].fvalue < 0.099819116)) {
            if (  (data[4].fvalue < 1.1027198)) {
              sum += (float)0.10414228;
            } else {
              sum += (float)-0.05753237;
            }
          } else {
            if (  (data[4].fvalue < 1.5627301)) {
              sum += (float)0.17699732;
            } else {
              sum += (float)0.021562172;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            sum += (float)0.1662441;
          } else {
            if ( (data[13].fvalue < 0.27733916)) {
              sum += (float)-0.038872447;
            } else {
              sum += (float)0.1417989;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.17421031)) {
          sum += (float)-0.14304779;
        } else {
          if (  (data[4].fvalue < 1.7351127)) {
            sum += (float)0.13280892;
          } else {
            sum += (float)-0.036983941;
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.014639625)) {
    if ( (data[0].fvalue < 3.9076505)) {
      sum += (float)-0.24954358;
    } else {
      sum += (float)-0;
    }
  } else {
    if ( (data[5].fvalue < 1.0626249)) {
      if ( (data[5].fvalue < 0.40571272)) {
        if (  (data[4].fvalue < 1.036298)) {
          if ( (data[14].fvalue < 0.047401577)) {
            if ( (data[13].fvalue < 5.8088412e-05)) {
              sum += (float)0.075828232;
            } else {
              sum += (float)-0.082430407;
            }
          } else {
            if ( (data[3].fvalue < 0.050744131)) {
              sum += (float)-0.23650785;
            } else {
              sum += (float)-0.053875726;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.1249732)) {
            if ( (data[13].fvalue < 0.0078410497)) {
              sum += (float)0.01675616;
            } else {
              sum += (float)0.1936665;
            }
          } else {
            if ( (data[14].fvalue < 0.23775354)) {
              sum += (float)-0.0049577141;
            } else {
              sum += (float)0.017348904;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.021320025)) {
          if ( (data[16].fvalue < 0.054387368)) {
            if ( (data[13].fvalue < 1.4744163e-07)) {
              sum += (float)0.013136956;
            } else {
              sum += (float)-0.021308148;
            }
          } else {
            if ( (data[13].fvalue < 0.013235949)) {
              sum += (float)-0.068147019;
            } else {
              sum += (float)-0.0023769352;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.1276367)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.072937146;
            } else {
              sum += (float)-0.1894803;
            }
          } else {
            if ( (data[2].fvalue < 0.82148594)) {
              sum += (float)-0.013771905;
            } else {
              sum += (float)-0.11367429;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[13].fvalue < 1.9271474e-06)) {
          if ( (data[14].fvalue < 0.0497659)) {
            if ( (data[3].fvalue < 0.15363425)) {
              sum += (float)-0;
            } else {
              sum += (float)0.21413937;
            }
          } else {
            if ( (data[3].fvalue < 0.23754196)) {
              sum += (float)-0.2300036;
            } else {
              sum += (float)0.036095984;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.013461774)) {
            sum += (float)-0.091873243;
          } else {
            if ( (data[5].fvalue < 1.5281026)) {
              sum += (float)-0.011562139;
            } else {
              sum += (float)0.053576156;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.8488251)) {
          sum += (float)0.21405581;
        } else {
          sum += (float)0.0011544181;
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.90736061)) {
    if ( (data[7].fvalue < 3.5)) {
      if ( (data[7].fvalue < 2.5)) {
        if ( (data[13].fvalue < 0.12948684)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.1200463)) {
              sum += (float)-0.012781178;
            } else {
              sum += (float)0.0054255496;
            }
          } else {
            if ( (data[13].fvalue < 0.022474121)) {
              sum += (float)-0.16740084;
            } else {
              sum += (float)-0.010025052;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.84063554)) {
            if ( (data[16].fvalue < 0.11966146)) {
              sum += (float)0.0078191739;
            } else {
              sum += (float)0.042839069;
            }
          } else {
            if ( (data[16].fvalue < 0.66553152)) {
              sum += (float)-0.068492681;
            } else {
              sum += (float)0.12953185;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[6].fvalue < 0.11459514)) {
            sum += (float)0.026796715;
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.061900433;
            } else {
              sum += (float)-0.1378314;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.13626929)) {
            sum += (float)0.13346539;
          } else {
            if ( (data[3].fvalue < 0.65715528)) {
              sum += (float)0.021462359;
            } else {
              sum += (float)-0.056245044;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.99620169)) {
        if (  (data[4].fvalue < 1.0587444)) {
          if ( (data[3].fvalue < 0.40439194)) {
            sum += (float)0.28367379;
          } else {
            sum += (float)0.14002781;
          }
        } else {
          if (  (data[4].fvalue < 1.9198699)) {
            if ( (data[13].fvalue < 0.42194194)) {
              sum += (float)0.027023537;
            } else {
              sum += (float)0.12476677;
            }
          } else {
            if ( (data[16].fvalue < 0.077399895)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.11618411;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.20184898)) {
          sum += (float)-0.21024732;
        } else {
          if ( (data[16].fvalue < 0.0066620493)) {
            if ( (data[14].fvalue < 0.57773483)) {
              sum += (float)-0.0067849192;
            } else {
              sum += (float)0.12350673;
            }
          } else {
            if ( (data[16].fvalue < 0.25502324)) {
              sum += (float)-0.17541146;
            } else {
              sum += (float)-0.018075297;
            }
          }
        }
      }
    }
  } else {
    sum += (float)0.12580538;
  }
  if ( (data[11].fvalue < 1.5)) {
    if ( (data[16].fvalue < 0.16008525)) {
      if ( (data[17].fvalue < 0.17097816)) {
        if ( (data[14].fvalue < 5.7198115e-08)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.23874862;
            } else {
              sum += (float)-0.061228298;
            }
          } else {
            sum += (float)0.35044247;
          }
        } else {
          if ( (data[14].fvalue < 0.0057520499)) {
            sum += (float)-0.14607123;
          } else {
            if ( (data[13].fvalue < 0.58443666)) {
              sum += (float)-0.009695705;
            } else {
              sum += (float)0.066011444;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.07947737)) {
          if ( (data[13].fvalue < 0.0046126763)) {
            if ( (data[17].fvalue < 0.63609874)) {
              sum += (float)-0.046403296;
            } else {
              sum += (float)-0.27334139;
            }
          } else {
            if ( (data[14].fvalue < 0.0013255606)) {
              sum += (float)-0.078119136;
            } else {
              sum += (float)0.069266915;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.014810598)) {
            if ( (data[5].fvalue < 0.39862621)) {
              sum += (float)0.094972804;
            } else {
              sum += (float)-0.0013197394;
            }
          } else {
            if ( (data[13].fvalue < 0.0012782051)) {
              sum += (float)-0.091462336;
            } else {
              sum += (float)0.029450404;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.12935011)) {
        if ( (data[13].fvalue < 0.0079018306)) {
          if ( (data[14].fvalue < 0.041274503)) {
            if (  (data[4].fvalue < 1.26753)) {
              sum += (float)0.049429528;
            } else {
              sum += (float)-0.036666177;
            }
          } else {
            if (  (data[4].fvalue < 1.6760761)) {
              sum += (float)0.10686321;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7516637)) {
            if ( (data[3].fvalue < 0.062278017)) {
              sum += (float)0.20241173;
            } else {
              sum += (float)0.11790747;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.016885944)) {
          if ( (data[14].fvalue < 0.062243883)) {
            if ( (data[5].fvalue < 0.17386959)) {
              sum += (float)-0.057369091;
            } else {
              sum += (float)-0.135886;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.022558574;
            } else {
              sum += (float)0.056131519;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.030267961)) {
            if ( (data[13].fvalue < 0.13800681)) {
              sum += (float)-0.079193816;
            } else {
              sum += (float)0.01101574;
            }
          } else {
            if (  (data[4].fvalue < 1.7544901)) {
              sum += (float)0.074780561;
            } else {
              sum += (float)-0.0017322905;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 0.47093928)) {
      if (  (data[4].fvalue < 1.7707088)) {
        if ( (data[17].fvalue < 0.26845753)) {
          sum += (float)-0.046021264;
        } else {
          if ( (data[13].fvalue < 0.006460255)) {
            sum += (float)0.011782111;
          } else {
            sum += (float)0.13225806;
          }
        }
      } else {
        if ( (data[17].fvalue < 1.1486304)) {
          if ( (data[17].fvalue < 0.51197684)) {
            sum += (float)-0.17254908;
          } else {
            if ( (data[6].fvalue < 0.65279174)) {
              sum += (float)-0.011090204;
            } else {
              sum += (float)-0.13481602;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.24950036)) {
            sum += (float)0.0010059499;
          } else {
            sum += (float)0.09089724;
          }
        }
      }
    } else {
      sum += (float)-0.1721092;
    }
  }
  if ( (data[6].fvalue < 0.86721814)) {
    if ( (data[17].fvalue < 0.040148579)) {
      if ( (data[17].fvalue < 7.1983442e-05)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[6].fvalue < 0.086483434)) {
            if ( (data[14].fvalue < 0.099075899)) {
              sum += (float)0.014360456;
            } else {
              sum += (float)-0.077972814;
            }
          } else {
            if ( (data[16].fvalue < 0.036934257)) {
              sum += (float)0.046107851;
            } else {
              sum += (float)0.0039178096;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.038064569)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.016540648;
            } else {
              sum += (float)0.21665435;
            }
          } else {
            if ( (data[14].fvalue < 0.054360121)) {
              sum += (float)-0.093223833;
            } else {
              sum += (float)-0.0087963566;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.011732526)) {
          if ( (data[16].fvalue < 0.009036934)) {
            sum += (float)0.26158005;
          } else {
            sum += (float)0.12752879;
          }
        } else {
          if ( (data[3].fvalue < 0.63215661)) {
            if ( (data[16].fvalue < 0.013434237)) {
              sum += (float)0.10958546;
            } else {
              sum += (float)0.0062190588;
            }
          } else {
            if ( (data[3].fvalue < 0.96063256)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.065606803;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.12173596)) {
        if ( (data[17].fvalue < 0.058332339)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[16].fvalue < 0.057581209)) {
              sum += (float)0.0391092;
            } else {
              sum += (float)-0.056526195;
            }
          } else {
            sum += (float)-0.09035217;
          }
        } else {
          if ( (data[2].fvalue < 0.089764617)) {
            sum += (float)-0.0021298828;
          } else {
            if ( (data[14].fvalue < 0.096996024)) {
              sum += (float)-0.14331882;
            } else {
              sum += (float)-0.088918127;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.0066189812)) {
            if ( (data[16].fvalue < 0.0097121336)) {
              sum += (float)-0.029056298;
            } else {
              sum += (float)-0.12002433;
            }
          } else {
            if ( (data[14].fvalue < 0.0034873295)) {
              sum += (float)-0.061454095;
            } else {
              sum += (float)0.079199284;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7799165)) {
            if ( (data[3].fvalue < 0.51175749)) {
              sum += (float)0.11742576;
            } else {
              sum += (float)0.026907565;
            }
          } else {
            if ( (data[17].fvalue < 0.72198081)) {
              sum += (float)-0.072448574;
            } else {
              sum += (float)0.058163196;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.80091619)) {
      if ( (data[16].fvalue < 0.37675932)) {
        sum += (float)-0.025410548;
      } else {
        sum += (float)0.053103637;
      }
    } else {
      sum += (float)0.14750966;
    }
  }
  if ( (data[14].fvalue < 0.35156024)) {
    if ( (data[16].fvalue < 0.12072064)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[16].fvalue < 0.055648439)) {
          if ( (data[4].fvalue < 1.0220792)) {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)-0.26035395;
            } else {
              sum += (float)-0.0083242934;
            }
          } else {
            if ( (data[17].fvalue < 0.83010876)) {
              sum += (float)0.028167646;
            } else {
              sum += (float)-0.068603888;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0023295828)) {
            if ( (data[14].fvalue < 0.10106159)) {
              sum += (float)-0.12334038;
            } else {
              sum += (float)-0.047687721;
            }
          } else {
            if (  (data[4].fvalue < 1.3678318)) {
              sum += (float)0.014263599;
            } else {
              sum += (float)-0.065398932;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.1167963)) {
          if ( (data[3].fvalue < 0.23150095)) {
            if ( (data[14].fvalue < 0.092228964)) {
              sum += (float)0.054286622;
            } else {
              sum += (float)0.28170776;
            }
          } else {
            if ( (data[13].fvalue < 0.058564037)) {
              sum += (float)-0.095450141;
            } else {
              sum += (float)0.042865209;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.10519306)) {
            if ( (data[14].fvalue < 0.21061556)) {
              sum += (float)-0.14289039;
            } else {
              sum += (float)-0.058715586;
            }
          } else {
            if ( (data[13].fvalue < 0.20680022)) {
              sum += (float)0.083946705;
            } else {
              sum += (float)-0.090996124;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[3].fvalue < 0.24046487)) {
          if ( (data[13].fvalue < 0.0076184678)) {
            if ( (data[14].fvalue < 0.042402264)) {
              sum += (float)-0.018295135;
            } else {
              sum += (float)0.042779814;
            }
          } else {
            if (  (data[4].fvalue < 1.6632862)) {
              sum += (float)0.079575777;
            } else {
              sum += (float)0.0049498333;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.055291973)) {
            if ( (data[14].fvalue < 0.10756126)) {
              sum += (float)-0.092516154;
            } else {
              sum += (float)-0.0078252275;
            }
          } else {
            if ( (data[6].fvalue < 0.52753109)) {
              sum += (float)0.018391162;
            } else {
              sum += (float)-0.027731461;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.2094852)) {
          if ( (data[14].fvalue < 0.2523641)) {
            if (  (data[4].fvalue < 1.3182517)) {
              sum += (float)0.27636632;
            } else {
              sum += (float)0.16548054;
            }
          } else {
            sum += (float)0.044562433;
          }
        } else {
          if ( (data[14].fvalue < 0.224742)) {
            sum += (float)-0.10735237;
          } else {
            sum += (float)-0;
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.73749262)) {
      if ( (data[13].fvalue < 0.46462673)) {
        if ( (data[4].fvalue < 1.8066504)) {
          if ( (data[0].fvalue < 2.0833991)) {
            if ( (data[16].fvalue < 0.0015211572)) {
              sum += (float)0.024813823;
            } else {
              sum += (float)0.089573376;
            }
          } else {
            if ( (data[2].fvalue < 0.77896619)) {
              sum += (float)0.0072333538;
            } else {
              sum += (float)-0.070217483;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.99489385)) {
            if ( (data[15].fvalue < 0.55661213)) {
              sum += (float)-0.10182645;
            } else {
              sum += (float)0.039830603;
            }
          } else {
            sum += (float)0.15891421;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.52901459)) {
          if (  (data[4].fvalue < 1.7644738)) {
            sum += (float)0.068803594;
          } else {
            sum += (float)-0.010555604;
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            sum += (float)0.19912404;
          } else {
            sum += (float)0.086483084;
          }
        }
      }
    } else {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[5].fvalue < 0.39089346)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)0.28680357;
          } else {
            sum += (float)0.080552757;
          }
        } else {
          sum += (float)-0;
        }
      } else {
        sum += (float)0.0025672212;
      }
    }
  }
  if ( (data[16].fvalue < 0.40831459)) {
    if (  (data[4].fvalue < 1.4189758)) {
      if ( (data[16].fvalue < 0.11305606)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[16].fvalue < 0.00090601761)) {
              sum += (float)-0.35050657;
            } else {
              sum += (float)0.031254984;
            }
          } else {
            if ( (data[14].fvalue < 7.8588123e-08)) {
              sum += (float)0.32607788;
            } else {
              sum += (float)-0.003830452;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.82583046)) {
            if (  (data[4].fvalue < 1.0593691)) {
              sum += (float)0.21910848;
            } else {
              sum += (float)0.048524521;
            }
          } else {
            if ( (data[3].fvalue < 1.2564493)) {
              sum += (float)-0.027554661;
            } else {
              sum += (float)-0.14100417;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[3].fvalue < 0.44439393)) {
            if ( (data[14].fvalue < 0.027456891)) {
              sum += (float)-0.0053245435;
            } else {
              sum += (float)0.064568244;
            }
          } else {
            if ( (data[13].fvalue < 0.1968236)) {
              sum += (float)-0.020635894;
            } else {
              sum += (float)0.064479418;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.52037078)) {
            sum += (float)0.23581012;
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if ( (data[16].fvalue < 0.018995114)) {
        if ( (data[14].fvalue < 3.7342488e-06)) {
          if (  (data[4].fvalue < 1.7678862)) {
            sum += (float)0.13230255;
          } else {
            sum += (float)0.29272753;
          }
        } else {
          if ( (data[17].fvalue < 0.35152209)) {
            if ( (data[13].fvalue < 0.27924836)) {
              sum += (float)-0.04939758;
            } else {
              sum += (float)0.031408403;
            }
          } else {
            if ( (data[14].fvalue < 0.13693032)) {
              sum += (float)-0.047180004;
            } else {
              sum += (float)0.062387343;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.6095414)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.22096434)) {
              sum += (float)-0.032712352;
            } else {
              sum += (float)0.03303612;
            }
          } else {
            if ( (data[16].fvalue < 0.19378509)) {
              sum += (float)-0.13732475;
            } else {
              sum += (float)-0.0032490466;
            }
          }
        } else {
          if ( (data[17].fvalue < 1.3160512)) {
            if ( (data[15].fvalue < 0.00023274875)) {
              sum += (float)-0.081888109;
            } else {
              sum += (float)-0.012668251;
            }
          } else {
            sum += (float)0.063368477;
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 0.60847628)) {
      if ( (data[14].fvalue < 0.23005545)) {
        if ( (data[13].fvalue < 0.1419989)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.18338436)) {
              sum += (float)0.00073316292;
            } else {
              sum += (float)-0.09907753;
            }
          } else {
            sum += (float)0.19657813;
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.8951449)) {
              sum += (float)0.031208135;
            } else {
              sum += (float)0.13250278;
            }
          } else {
            if ( (data[13].fvalue < 0.83922017)) {
              sum += (float)0.016433097;
            } else {
              sum += (float)-0.12422635;
            }
          }
        }
      } else {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[15].fvalue < 0.037869304)) {
              sum += (float)0.1114396;
            } else {
              sum += (float)0.0023937435;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.059815343;
            } else {
              sum += (float)0.055349208;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.0391517)) {
            sum += (float)0.012868748;
          } else {
            sum += (float)-0.10490999;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.45729548)) {
        if ( (data[16].fvalue < 0.56938004)) {
          sum += (float)-0.036536988;
        } else {
          sum += (float)-0.13808942;
        }
      } else {
        sum += (float)-0.027468709;
      }
    }
  }
  if ( (data[3].fvalue < 1.435503)) {
    if ( (data[13].fvalue < 0.10425188)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[6].fvalue < 0.023972485)) {
            if ( (data[14].fvalue < 0.031818181)) {
              sum += (float)0.043022711;
            } else {
              sum += (float)-0.18421397;
            }
          } else {
            if ( (data[13].fvalue < 0.007340108)) {
              sum += (float)-0.00076724583;
            } else {
              sum += (float)0.02104266;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.0916173)) {
            if ( (data[3].fvalue < 0.37030867)) {
              sum += (float)0.12948027;
            } else {
              sum += (float)-0.019697404;
            }
          } else {
            if ( (data[14].fvalue < 0.4343614)) {
              sum += (float)-0.11426977;
            } else {
              sum += (float)-0.013438422;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.14283398)) {
          if (  (data[4].fvalue < 1.2329675)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.069756173;
            } else {
              sum += (float)0.19733705;
            }
          } else {
            if ( (data[16].fvalue < 0.0069468357)) {
              sum += (float)-0.099509336;
            } else {
              sum += (float)0.03354479;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.03204855)) {
            if ( (data[16].fvalue < 0.1893211)) {
              sum += (float)-0.17257163;
            } else {
              sum += (float)-0.027960857;
            }
          } else {
            if ( (data[16].fvalue < 0.094749995)) {
              sum += (float)-0.055918824;
            } else {
              sum += (float)0.0150118;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.23654427)) {
        if ( (data[16].fvalue < 0.30857989)) {
          if (  (data[4].fvalue < 1.3599176)) {
            if ( (data[16].fvalue < 0.00049975113)) {
              sum += (float)-0.016444486;
            } else {
              sum += (float)0.064091139;
            }
          } else {
            if ( (data[17].fvalue < 0.71580356)) {
              sum += (float)-0.055485096;
            } else {
              sum += (float)0.072533906;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[5].fvalue < 0.30176821)) {
              sum += (float)0.066394329;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.51746374)) {
              sum += (float)0.027451698;
            } else {
              sum += (float)-0.10650217;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[13].fvalue < 0.44135112)) {
            if ( (data[14].fvalue < 0.73285222)) {
              sum += (float)-0.017125471;
            } else {
              sum += (float)-0.12933259;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.10203697;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3005561)) {
            if ( (data[3].fvalue < 0.44081223)) {
              sum += (float)0.22930273;
            } else {
              sum += (float)0.082984984;
            }
          } else {
            if ( (data[14].fvalue < 0.50949955)) {
              sum += (float)0.0043315361;
            } else {
              sum += (float)0.075515538;
            }
          }
        }
      }
    }
  } else {
    if ( (data[7].fvalue < 1.5)) {
      if (  (data[4].fvalue < 1.0611109)) {
        if ( (data[14].fvalue < 0.80782843)) {
          if ( (data[13].fvalue < 2.3688055e-05)) {
            sum += (float)0.11422243;
          } else {
            sum += (float)0.032664109;
          }
        } else {
          sum += (float)-0;
        }
      } else {
        if ( (data[4].fvalue < 1.9837638)) {
          if ( (data[13].fvalue < 0.70568371)) {
            if ( (data[14].fvalue < 0.82412815)) {
              sum += (float)-0.080879711;
            } else {
              sum += (float)-0.012391645;
            }
          } else {
            if ( (data[13].fvalue < 1.0773444)) {
              sum += (float)0.044040632;
            } else {
              sum += (float)-0.020366365;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.47538966)) {
            sum += (float)-0;
          } else {
            sum += (float)0.069308318;
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 1.3714312)) {
        if ( (data[14].fvalue < 0.55916238)) {
          sum += (float)-0.19443573;
        } else {
          sum += (float)-0.066950522;
        }
      } else {
        if ( (data[13].fvalue < 0.23191622)) {
          sum += (float)-0.08972165;
        } else {
          if ( (data[14].fvalue < 0.62191629)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.0080998968;
            } else {
              sum += (float)-0.061405458;
            }
          } else {
            sum += (float)0.070974886;
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 1.0221934)) {
    if ( (data[14].fvalue < 0.4306832)) {
      if ( (data[3].fvalue < 0.45876455)) {
        if ( (data[7].fvalue < 3.5)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[17].fvalue < 0.13034208)) {
              sum += (float)-0.015049391;
            } else {
              sum += (float)0.043969125;
            }
          } else {
            if ( (data[16].fvalue < 0.033442337)) {
              sum += (float)0.10679857;
            } else {
              sum += (float)0.0045381864;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.2442553)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.038839679;
            } else {
              sum += (float)0.1974896;
            }
          } else {
            if ( (data[3].fvalue < 0.071149543)) {
              sum += (float)0.087603435;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[16].fvalue < 0.056587413)) {
            if ( (data[17].fvalue < 0.48031509)) {
              sum += (float)0.04293938;
            } else {
              sum += (float)-0.069743909;
            }
          } else {
            if ( (data[13].fvalue < 0.28434038)) {
              sum += (float)-0.042492684;
            } else {
              sum += (float)0.014619507;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.37015614)) {
            if ( (data[14].fvalue < 0.14475022)) {
              sum += (float)-0.14674817;
            } else {
              sum += (float)-0.052117191;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.0066481885;
            } else {
              sum += (float)0.083625056;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.5365864)) {
        if ( (data[16].fvalue < 0.64957696)) {
          if (  (data[4].fvalue < 1.6073232)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.0098049901;
            } else {
              sum += (float)0.06976071;
            }
          } else {
            if ( (data[17].fvalue < 0.62865531)) {
              sum += (float)-0.043755829;
            } else {
              sum += (float)0.080085374;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.062597506)) {
              sum += (float)0.1403639;
            } else {
              sum += (float)0.019670559;
            }
          } else {
            sum += (float)-0.050807558;
          }
        }
      } else {
        if (  (data[4].fvalue < 1.9762021)) {
          sum += (float)0.14584827;
        } else {
          sum += (float)0.036101371;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 3.5)) {
      if ( (data[14].fvalue < 0.18740639)) {
        if ( (data[13].fvalue < 1.9271474e-06)) {
          if ( (data[3].fvalue < 0.41673031)) {
            if ( (data[14].fvalue < 0.10060771)) {
              sum += (float)0.078468114;
            } else {
              sum += (float)-0.10134237;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.24316569;
            } else {
              sum += (float)0.026147028;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.013307001)) {
            sum += (float)-0.0488622;
          } else {
            if ( (data[2].fvalue < 0.4114725)) {
              sum += (float)-0.010414073;
            } else {
              sum += (float)0.024495345;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.56340444)) {
          sum += (float)-0.15880412;
        } else {
          if ( (data[14].fvalue < 0.25455484)) {
            if (  (data[4].fvalue < 0.85589135)) {
              sum += (float)-0;
            } else {
              sum += (float)0.16655591;
            }
          } else {
            if ( (data[3].fvalue < 0.80389345)) {
              sum += (float)-0.10129265;
            } else {
              sum += (float)0.015477946;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.81969821)) {
        sum += (float)0.19191596;
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[5].fvalue < 1.0130684)) {
    if ( (data[14].fvalue < 0.1983217)) {
      if (  (data[4].fvalue < 1.1070206)) {
        if (  (data[4].fvalue < 1.0438676)) {
          if ( (data[3].fvalue < 0.077817917)) {
            if ( (data[14].fvalue < 0.031263601)) {
              sum += (float)0.021437213;
            } else {
              sum += (float)-0.10320941;
            }
          } else {
            if ( (data[14].fvalue < 0.11622719)) {
              sum += (float)0.02758983;
            } else {
              sum += (float)-0.030666418;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.32739866)) {
            if ( (data[14].fvalue < 0.022891769)) {
              sum += (float)-0.062388282;
            } else {
              sum += (float)0.10181899;
            }
          } else {
            if ( (data[13].fvalue < 0.14465579)) {
              sum += (float)-0.10050703;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.00067456008)) {
          if (  (data[4].fvalue < 1.5043614)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.10460118;
            } else {
              sum += (float)-0.015745491;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.21984093;
            } else {
              sum += (float)-0.0041054725;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[3].fvalue < 0.07415209)) {
              sum += (float)0.018775169;
            } else {
              sum += (float)-0.024769207;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.14197335;
            } else {
              sum += (float)-0.00034753664;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.29748827)) {
        if ( (data[17].fvalue < 1.162879)) {
          if ( (data[3].fvalue < 0.17405286)) {
            if ( (data[16].fvalue < 0.0040835505)) {
              sum += (float)0.0070938999;
            } else {
              sum += (float)0.070072055;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.00078712002;
            } else {
              sum += (float)-0.052857127;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.4608092)) {
            sum += (float)0.046757221;
          } else {
            sum += (float)0.13088962;
          }
        }
      } else {
        if ( (data[6].fvalue < 0.33627358)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[6].fvalue < 0.27782619)) {
              sum += (float)-0;
            } else {
              sum += (float)0.074293211;
            }
          } else {
            if ( (data[6].fvalue < 0.27794158)) {
              sum += (float)0.15267624;
            } else {
              sum += (float)0.043384388;
            }
          }
        } else {
          if ( (data[0].fvalue < 3.0288308)) {
            if ( (data[13].fvalue < 0.62503886)) {
              sum += (float)0.029461285;
            } else {
              sum += (float)0.10695896;
            }
          } else {
            if ( (data[14].fvalue < 0.68703574)) {
              sum += (float)-0.02479157;
            } else {
              sum += (float)0.048934195;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.13089696)) {
      sum += (float)-0.096834794;
    } else {
      if ( (data[16].fvalue < 0.0010291236)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[13].fvalue < 1.8775232e-06)) {
            if ( (data[14].fvalue < 0.070670187)) {
              sum += (float)0.11547441;
            } else {
              sum += (float)0.015000928;
            }
          } else {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.011990707;
            } else {
              sum += (float)0.05949422;
            }
          }
        } else {
          sum += (float)0.11549035;
        }
      } else {
        sum += (float)-0.097610153;
      }
    }
  }
  if ( (data[13].fvalue < 0.12477742)) {
    if ( (data[6].fvalue < 0.86965507)) {
      if ( (data[14].fvalue < 0.0058027497)) {
        if ( (data[14].fvalue < 1.5621883e-05)) {
          if ( (data[16].fvalue < 0.014088145)) {
            if (  (data[4].fvalue < 1.574053)) {
              sum += (float)-0.0092501035;
            } else {
              sum += (float)0.3130458;
            }
          } else {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)0.013237382;
            } else {
              sum += (float)-0.08489535;
            }
          }
        } else {
          if (  (data[4].fvalue < 0.99551499)) {
            sum += (float)-0.085450247;
          } else {
            if ( (data[13].fvalue < 0.0015019774)) {
              sum += (float)-0.29227921;
            } else {
              sum += (float)-0.11460267;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.38623294)) {
          if (  (data[4].fvalue < 1.23941)) {
            if ( (data[16].fvalue < 0.14777473)) {
              sum += (float)0.0039369827;
            } else {
              sum += (float)0.053264517;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.0070676426;
            } else {
              sum += (float)-0.059676006;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0032622402)) {
            if ( (data[14].fvalue < 0.44592321)) {
              sum += (float)-0.027546171;
            } else {
              sum += (float)0.059629943;
            }
          } else {
            if ( (data[3].fvalue < 0.6975131)) {
              sum += (float)0.10924952;
            } else {
              sum += (float)0.0054910784;
            }
          }
        }
      }
    } else {
      sum += (float)0.092598163;
    }
  } else {
    if (  (data[4].fvalue < 1.3205402)) {
      if ( (data[3].fvalue < 0.7609657)) {
        if ( (data[8].fvalue < 4.5)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.02112747;
            } else {
              sum += (float)0.042888965;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.067026921;
            } else {
              sum += (float)0.12964281;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.28919542)) {
            if ( (data[13].fvalue < 0.18054992)) {
              sum += (float)0.0098318476;
            } else {
              sum += (float)0.18348646;
            }
          } else {
            sum += (float)0.25614163;
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.44818887)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.04599908;
            } else {
              sum += (float)-0.053908203;
            }
          } else {
            if ( (data[14].fvalue < 0.00068686507)) {
              sum += (float)0.038221098;
            } else {
              sum += (float)0.1649242;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.29560798)) {
            if ( (data[16].fvalue < 0.096041419)) {
              sum += (float)-0.14988317;
            } else {
              sum += (float)-0.026981456;
            }
          } else {
            if ( (data[3].fvalue < 1.6000221)) {
              sum += (float)0.049961299;
            } else {
              sum += (float)-0.079636507;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.63541013)) {
        if ( (data[16].fvalue < 0.40552977)) {
          if ( (data[17].fvalue < 0.14762694)) {
            if ( (data[15].fvalue < 0.10940211)) {
              sum += (float)-0.036557097;
            } else {
              sum += (float)0.064047404;
            }
          } else {
            if (  (data[4].fvalue < 1.7030113)) {
              sum += (float)0.064443089;
            } else {
              sum += (float)-0.028991053;
            }
          }
        } else {
          if ( (data[10].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.39905322)) {
              sum += (float)0.004953485;
            } else {
              sum += (float)0.067790754;
            }
          } else {
            if ( (data[16].fvalue < 1.1634703)) {
              sum += (float)-0.047500193;
            } else {
              sum += (float)0.072079986;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.8833394)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.23208888)) {
              sum += (float)-0.001411947;
            } else {
              sum += (float)0.039684825;
            }
          } else {
            if ( (data[3].fvalue < 1.1648185)) {
              sum += (float)0.17119704;
            } else {
              sum += (float)0.042288467;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.31749302)) {
            sum += (float)-0.089658111;
          } else {
            sum += (float)0.021481782;
          }
        }
      }
    }
  }
  if ( (data[3].fvalue < 1.6517253)) {
    if ( (data[6].fvalue < 0.012674622)) {
      sum += (float)-0.12807457;
    } else {
      if ( (data[7].fvalue < 7.5)) {
        if ( (data[3].fvalue < 0.11561034)) {
          if (  (data[4].fvalue < 0.95850587)) {
            if ( (data[14].fvalue < 0.027466279)) {
              sum += (float)0.039383408;
            } else {
              sum += (float)-0.26594442;
            }
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.01143159;
            } else {
              sum += (float)0.070824102;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.033919059)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0029550898;
            } else {
              sum += (float)-0.13243704;
            }
          } else {
            if ( (data[17].fvalue < 0.41422796)) {
              sum += (float)-0.00057357369;
            } else {
              sum += (float)0.030991051;
            }
          }
        }
      } else {
        sum += (float)0.097999148;
      }
    }
  } else {
    if ( (data[13].fvalue < 0.76630074)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[5].fvalue < 0.49051678)) {
            if ( (data[2].fvalue < 0.54693496)) {
              sum += (float)-0.021266682;
            } else {
              sum += (float)0.063999549;
            }
          } else {
            if ( (data[4].fvalue < 1.1528442)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.093659505;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.3141205)) {
              sum += (float)0.12822677;
            } else {
              sum += (float)0.017280802;
            }
          } else {
            sum += (float)-0.073443204;
          }
        }
      } else {
        if ( (data[4].fvalue < 1.7136018)) {
          if ( (data[14].fvalue < 0.63761687)) {
            sum += (float)-0.17006339;
          } else {
            sum += (float)-0.073671721;
          }
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.3607323)) {
        sum += (float)0.12044166;
      } else {
        if ( (data[0].fvalue < 4.3943806)) {
          sum += (float)-0.050244872;
        } else {
          sum += (float)0.0060851644;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.20372382)) {
    if ( (data[8].fvalue < 8.5)) {
      if ( (data[13].fvalue < 1.4744163e-07)) {
        if ( (data[7].fvalue < 0.5)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.045113139)) {
              sum += (float)0.031636193;
            } else {
              sum += (float)-0.022635229;
            }
          } else {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.013476962;
            } else {
              sum += (float)0.06623169;
            }
          }
        } else {
          sum += (float)0.3401387;
        }
      } else {
        if ( (data[13].fvalue < 0.0056579616)) {
          if ( (data[13].fvalue < 0.00098098244)) {
            sum += (float)-0.19494972;
          } else {
            if ( (data[0].fvalue < 1.5890695)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.096839197;
            }
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[17].fvalue < 0.00038781454)) {
              sum += (float)-0.031947244;
            } else {
              sum += (float)0.032969419;
            }
          } else {
            if ( (data[14].fvalue < 0.004005542)) {
              sum += (float)-0.040187247;
            } else {
              sum += (float)0.037973549;
            }
          }
        }
      }
    } else {
      sum += (float)0.080642745;
    }
  } else {
    if ( (data[6].fvalue < 0.61077946)) {
      if ( (data[4].fvalue < 1.4435499)) {
        if ( (data[3].fvalue < 1.0910009)) {
          if ( (data[14].fvalue < 0.14087746)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.045507029;
            } else {
              sum += (float)-0.0045242002;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.044053145;
            } else {
              sum += (float)0.13018592;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.55370414)) {
            if ( (data[14].fvalue < 0.52412307)) {
              sum += (float)-0.075817555;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[16].fvalue < 0.062283628)) {
              sum += (float)0.098044559;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.65387517)) {
          if ( (data[14].fvalue < 0.57608783)) {
            if ( (data[16].fvalue < 0.2789613)) {
              sum += (float)-0.031730413;
            } else {
              sum += (float)0.02066439;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.034203928;
            } else {
              sum += (float)0.084170014;
            }
          }
        } else {
          sum += (float)0.092587277;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.85336566)) {
        if ( (data[3].fvalue < 0.32847247)) {
          if ( (data[0].fvalue < 2.0372291)) {
            sum += (float)0.068105035;
          } else {
            sum += (float)-0.046045225;
          }
        } else {
          if ( (data[17].fvalue < 0.65185535)) {
            if ( (data[6].fvalue < 0.7280277)) {
              sum += (float)-0.044974111;
            } else {
              sum += (float)-0.12052176;
            }
          } else {
            sum += (float)0.0054297224;
          }
        }
      } else {
        if ( (data[13].fvalue < 0.685763)) {
          if ( (data[14].fvalue < 0.0027619153)) {
            sum += (float)-0.038111515;
          } else {
            sum += (float)0.022586625;
          }
        } else {
          sum += (float)0.10301708;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.3965193)) {
    if (  (data[4].fvalue < 1.2612002)) {
      if ( (data[8].fvalue < 6.5)) {
        if ( (data[16].fvalue < 0.15101159)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[16].fvalue < 0.0003311662)) {
              sum += (float)-0.30762601;
            } else {
              sum += (float)0.009887591;
            }
          } else {
            if ( (data[14].fvalue < 5.7198115e-08)) {
              sum += (float)0.30952674;
            } else {
              sum += (float)0.0011741527;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.15435308)) {
            if ( (data[0].fvalue < 4.0471239)) {
              sum += (float)0.11103272;
            } else {
              sum += (float)0.01923532;
            }
          } else {
            if ( (data[13].fvalue < 0.0061304071)) {
              sum += (float)0.00046856853;
            } else {
              sum += (float)0.046942398;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.51380384)) {
          sum += (float)0.16857903;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.12843972)) {
          if ( (data[17].fvalue < 0.47282392)) {
            if ( (data[16].fvalue < 0.0049513662)) {
              sum += (float)0.083777837;
            } else {
              sum += (float)-0.018097723;
            }
          } else {
            if ( (data[13].fvalue < 0.00022232276)) {
              sum += (float)-0.14963654;
            } else {
              sum += (float)0.00079638668;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.0029467556)) {
              sum += (float)0.0056544472;
            } else {
              sum += (float)0.038399205;
            }
          } else {
            if ( (data[14].fvalue < 0.7271148)) {
              sum += (float)-0.065840811;
            } else {
              sum += (float)0.043963585;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.22122394)) {
          if ( (data[8].fvalue < 0.5)) {
            if ( (data[5].fvalue < 0.1878532)) {
              sum += (float)0.087997302;
            } else {
              sum += (float)0.018154776;
            }
          } else {
            if ( (data[15].fvalue < 0.12001764)) {
              sum += (float)-0.061815608;
            } else {
              sum += (float)0.035870899;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.39754409)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)0.030858329;
            } else {
              sum += (float)0.13682985;
            }
          } else {
            if (  (data[4].fvalue < 1.9491537)) {
              sum += (float)0.00055603427;
            } else {
              sum += (float)-0.085621372;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.39906675)) {
      if (  (data[4].fvalue < 1.6737843)) {
        if ( (data[14].fvalue < 0.0077240448)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.24402118)) {
              sum += (float)-0.027676243;
            } else {
              sum += (float)0.05834477;
            }
          } else {
            sum += (float)0.10066115;
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.22265413)) {
              sum += (float)0.12640232;
            } else {
              sum += (float)0.027793204;
            }
          } else {
            if ( (data[3].fvalue < 0.63445687)) {
              sum += (float)0.088138163;
            } else {
              sum += (float)-0.0037095987;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.34365302)) {
          sum += (float)-0.024662159;
        } else {
          sum += (float)0.013402102;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.92195475)) {
        if ( (data[10].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.49410003)) {
            if (  (data[4].fvalue < 2.0604973)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.076058753;
            }
          } else {
            if ( (data[13].fvalue < 0.6467849)) {
              sum += (float)0.035087083;
            } else {
              sum += (float)0.12021623;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.005761683)) {
            if ( (data[0].fvalue < 3.2386353)) {
              sum += (float)-0.061459597;
            } else {
              sum += (float)-0.18499896;
            }
          } else {
            sum += (float)0.0025465689;
          }
        }
      } else {
        if ( (data[5].fvalue < 0.38308814)) {
          if ( (data[10].fvalue < 1.5)) {
            sum += (float)0.16070256;
          } else {
            sum += (float)0.010214886;
          }
        } else {
          sum += (float)-0;
        }
      }
    }
  }
  if ( (data[11].fvalue < 1.5)) {
    if ( (data[14].fvalue < 0.34151179)) {
      if (  (data[4].fvalue < 1.1505206)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[3].fvalue < 0.032799471)) {
            if (  (data[4].fvalue < 0.97644234)) {
              sum += (float)-0.24277589;
            } else {
              sum += (float)-0.02164996;
            }
          } else {
            if ( (data[14].fvalue < 0.072035432)) {
              sum += (float)0.024875501;
            } else {
              sum += (float)-0.0015981722;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.49885911)) {
            if ( (data[3].fvalue < 0.17812574)) {
              sum += (float)0.23284304;
            } else {
              sum += (float)0.090898111;
            }
          } else {
            if ( (data[14].fvalue < 0.11690155)) {
              sum += (float)-0.15496488;
            } else {
              sum += (float)0.006380504;
            }
          }
        }
      } else {
        if ( (data[10].fvalue < 0.5)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.0026901171;
            } else {
              sum += (float)-0.059576526;
            }
          } else {
            if ( (data[17].fvalue < 0.052488379)) {
              sum += (float)-0.1366241;
            } else {
              sum += (float)0.056142118;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[5].fvalue < 0.33497459)) {
              sum += (float)0.0048287325;
            } else {
              sum += (float)-0.026662396;
            }
          } else {
            if ( (data[13].fvalue < 0.30855453)) {
              sum += (float)0.099903636;
            } else {
              sum += (float)-0.10063545;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.42140535)) {
        if ( (data[4].fvalue < 1.472322)) {
          if ( (data[13].fvalue < 0.025355536)) {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)0.0011134802;
            } else {
              sum += (float)0.049204361;
            }
          } else {
            if ( (data[3].fvalue < 0.3791008)) {
              sum += (float)0.10914213;
            } else {
              sum += (float)0.021970542;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.015241897)) {
            if ( (data[16].fvalue < 0.33683258)) {
              sum += (float)-0.015219606;
            } else {
              sum += (float)0.061271429;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.096676208;
            } else {
              sum += (float)-0.012601138;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.50575036)) {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[16].fvalue < 0.0038082874)) {
              sum += (float)0.16474846;
            } else {
              sum += (float)0.0069650714;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)-0.022173785;
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 1.5)) {
      if ( (data[14].fvalue < 0.49418306)) {
        if ( (data[14].fvalue < 0.014025886)) {
          sum += (float)-0.11484635;
        } else {
          if ( (data[11].fvalue < 2.5)) {
            if ( (data[17].fvalue < 0.58557945)) {
              sum += (float)-0.069609359;
            } else {
              sum += (float)0.018870858;
            }
          } else {
            sum += (float)0.028389039;
          }
        }
      } else {
        sum += (float)-0.17628415;
      }
    } else {
      if ( (data[7].fvalue < 3.5)) {
        if ( (data[17].fvalue < 0.28695917)) {
          sum += (float)-0.032814696;
        } else {
          if (  (data[4].fvalue < 2.006959)) {
            if ( (data[5].fvalue < 0.24263769)) {
              sum += (float)0.11265946;
            } else {
              sum += (float)0.015574806;
            }
          } else {
            sum += (float)0.0031562704;
          }
        }
      } else {
        sum += (float)-0.11109188;
      }
    }
  }
  if ( (data[14].fvalue < 2.1269708)) {
    if ( (data[2].fvalue < 0.076614432)) {
      if ( (data[13].fvalue < 0.13707738)) {
        if (  (data[4].fvalue < 0.96987152)) {
          sum += (float)-0.061180059;
        } else {
          if ( (data[5].fvalue < 0.19569159)) {
            if (  (data[4].fvalue < 1.0201933)) {
              sum += (float)-0.05016287;
            } else {
              sum += (float)0.015572385;
            }
          } else {
            if ( (data[14].fvalue < 0.045394052)) {
              sum += (float)0.10572747;
            } else {
              sum += (float)0.0050694551;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.044099599)) {
          sum += (float)0.11182301;
        } else {
          sum += (float)0.0042463802;
        }
      }
    } else {
      if ( (data[17].fvalue < 0.048444822)) {
        if (  (data[4].fvalue < 1.8417841)) {
          if ( (data[16].fvalue < 1.0410302)) {
            if ( (data[16].fvalue < 0.18352373)) {
              sum += (float)-0.0025578549;
            } else {
              sum += (float)0.015880242;
            }
          } else {
            if (  (data[4].fvalue < 1.6959935)) {
              sum += (float)-0.14472967;
            } else {
              sum += (float)-0.017621154;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[15].fvalue < 0.064477287)) {
              sum += (float)0.10567111;
            } else {
              sum += (float)-0.0024401026;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.038854994;
            } else {
              sum += (float)-0.035220634;
            }
          }
        }
      } else {
        if ( (data[17].fvalue < 0.13102762)) {
          if ( (data[14].fvalue < 0.052401278)) {
            if ( (data[3].fvalue < 0.46919394)) {
              sum += (float)-0.14719629;
            } else {
              sum += (float)-0.036095224;
            }
          } else {
            if ( (data[3].fvalue < 0.084645107)) {
              sum += (float)0.060271993;
            } else {
              sum += (float)-0.045858145;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.0041110916)) {
            if ( (data[14].fvalue < 0.075735264)) {
              sum += (float)-0.11687075;
            } else {
              sum += (float)-0.0072915386;
            }
          } else {
            if (  (data[4].fvalue < 1.7040296)) {
              sum += (float)0.04855293;
            } else {
              sum += (float)-0.01143162;
            }
          }
        }
      }
    }
  } else {
    sum += (float)-0.10996404;
  }
  if ( (data[13].fvalue < 0.30009782)) {
    if ( (data[4].fvalue < 1.1690197)) {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[3].fvalue < 0.060348988)) {
          if (  (data[4].fvalue < 0.96403855)) {
            sum += (float)-0.27374342;
          } else {
            if ( (data[14].fvalue < 0.034682915)) {
              sum += (float)0.046524592;
            } else {
              sum += (float)-0.072112404;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.31787679)) {
            if ( (data[14].fvalue < 0.018035868)) {
              sum += (float)-0.085016392;
            } else {
              sum += (float)0.072286062;
            }
          } else {
            if ( (data[3].fvalue < 0.17739311)) {
              sum += (float)-0.022829842;
            } else {
              sum += (float)0.0067592873;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.15861529)) {
          if (  (data[4].fvalue < 1.0967969)) {
            sum += (float)0.24232118;
          } else {
            sum += (float)0.12384417;
          }
        } else {
          if ( (data[14].fvalue < 0.14957371)) {
            if (  (data[4].fvalue < 0.96135724)) {
              sum += (float)0.020212842;
            } else {
              sum += (float)-0.14605132;
            }
          } else {
            if ( (data[3].fvalue < 0.65833277)) {
              sum += (float)0.11610851;
            } else {
              sum += (float)-0.011761052;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.25833726)) {
        if ( (data[5].fvalue < 0.24454144)) {
          if ( (data[17].fvalue < 0.00043541746)) {
            if ( (data[16].fvalue < 0.0010968228)) {
              sum += (float)-0.087189607;
            } else {
              sum += (float)-0.0028529279;
            }
          } else {
            if ( (data[14].fvalue < 0.021021752)) {
              sum += (float)-0.050723068;
            } else {
              sum += (float)0.034976128;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.0013273838)) {
            if (  (data[4].fvalue < 1.6425312)) {
              sum += (float)-0.10529572;
            } else {
              sum += (float)0.051389821;
            }
          } else {
            if ( (data[17].fvalue < 0.10132585)) {
              sum += (float)-0.040505905;
            } else {
              sum += (float)0.044916525;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.050122619)) {
          if ( (data[8].fvalue < 2.5)) {
            if (  (data[4].fvalue < 1.4181691)) {
              sum += (float)-0.04221455;
            } else {
              sum += (float)0.062826134;
            }
          } else {
            if (  (data[4].fvalue < 1.3166451)) {
              sum += (float)0.16747385;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.0098408451;
            } else {
              sum += (float)0.014841416;
            }
          } else {
            if (  (data[4].fvalue < 1.3466773)) {
              sum += (float)0.052867044;
            } else {
              sum += (float)-0.088840887;
            }
          }
        }
      }
    }
  } else {
    if ( (data[14].fvalue < 0.16846618)) {
      if ( (data[8].fvalue < 2.5)) {
        if ( (data[14].fvalue < 0.030148905)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[16].fvalue < 0.28065446)) {
              sum += (float)-0.061370406;
            } else {
              sum += (float)0.017842695;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.11838183;
            } else {
              sum += (float)-0.12701134;
            }
          }
        } else {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.047361393;
            } else {
              sum += (float)0.13850203;
            }
          } else {
            if ( (data[7].fvalue < 3.5)) {
              sum += (float)-0.028822362;
            } else {
              sum += (float)0.074692361;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.3780067)) {
          sum += (float)0.0061695077;
        } else {
          sum += (float)-0.1830436;
        }
      }
    } else {
      if (  (data[4].fvalue < 2.0457249)) {
        if ( (data[3].fvalue < 1.9492366)) {
          if (  (data[4].fvalue < 1.3990119)) {
            if ( (data[3].fvalue < 0.86525059)) {
              sum += (float)0.13798778;
            } else {
              sum += (float)0.038156066;
            }
          } else {
            if ( (data[13].fvalue < 0.44507882)) {
              sum += (float)0.0090945577;
            } else {
              sum += (float)0.071771495;
            }
          }
        } else {
          sum += (float)-0.039354172;
        }
      } else {
        if ( (data[8].fvalue < 4.5)) {
          sum += (float)0.0059445496;
        } else {
          sum += (float)-0.076984391;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.2260243)) {
    if ( (data[4].fvalue < 1.1910017)) {
      if ( (data[14].fvalue < 0.0058096889)) {
        if ( (data[14].fvalue < 1.5394744e-05)) {
          if (  (data[4].fvalue < 1.0352166)) {
            sum += (float)0.07288187;
          } else {
            if ( (data[3].fvalue < 0.046946686)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.06429489;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.70302415)) {
            sum += (float)-0.11845871;
          } else {
            sum += (float)-0.041349933;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.035064161)) {
          if ( (data[13].fvalue < 1.2067298e-06)) {
            if (  (data[4].fvalue < 1.0030153)) {
              sum += (float)0.17011559;
            } else {
              sum += (float)-0.18026069;
            }
          } else {
            if (  (data[4].fvalue < 1.0357366)) {
              sum += (float)-0.072141729;
            } else {
              sum += (float)0.025407447;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.03184031)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.12724607;
            } else {
              sum += (float)0.11796081;
            }
          } else {
            if ( (data[5].fvalue < 0.32298595)) {
              sum += (float)0.041457161;
            } else {
              sum += (float)0.0041398499;
            }
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.13721994)) {
        if ( (data[17].fvalue < 0.15176718)) {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.26479512)) {
              sum += (float)-0.063455045;
            } else {
              sum += (float)0.0075413557;
            }
          } else {
            if ( (data[14].fvalue < 0.16827522)) {
              sum += (float)0.0013740906;
            } else {
              sum += (float)0.075039081;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.7340274)) {
            if ( (data[14].fvalue < 0.039759919)) {
              sum += (float)-0.021632932;
            } else {
              sum += (float)0.12576006;
            }
          } else {
            sum += (float)-0.044350162;
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[14].fvalue < 0.2220439)) {
              sum += (float)-0.02192918;
            } else {
              sum += (float)0.0087036639;
            }
          } else {
            if ( (data[16].fvalue < 0.19991368)) {
              sum += (float)-0.21103892;
            } else {
              sum += (float)-0.01864324;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.5)) {
            if ( (data[14].fvalue < 0.42874628)) {
              sum += (float)-0.03054033;
            } else {
              sum += (float)0.034595679;
            }
          } else {
            if ( (data[13].fvalue < 0.01670137)) {
              sum += (float)-0.16216841;
            } else {
              sum += (float)-0.034012221;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.19923648)) {
      if (  (data[4].fvalue < 1.4447148)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.073358685)) {
            sum += (float)0.057847094;
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)0.2151933;
        }
      } else {
        if ( (data[0].fvalue < 2.6571085)) {
          if ( (data[5].fvalue < 0.057300709)) {
            sum += (float)0.095701993;
          } else {
            sum += (float)0.011584641;
          }
        } else {
          if ( (data[6].fvalue < 0.2696681)) {
            sum += (float)0.046380103;
          } else {
            if ( (data[14].fvalue < 0.19836263)) {
              sum += (float)-0.039224863;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.16010749)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[14].fvalue < 0.004804357)) {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.023335632;
            } else {
              sum += (float)0.050831001;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.046729665;
            } else {
              sum += (float)-0.042421162;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.25093386)) {
            sum += (float)-0.0080160107;
          } else {
            sum += (float)-0.15783413;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.15696993)) {
          if ( (data[0].fvalue < 3.2413926)) {
            if ( (data[6].fvalue < 0.24675134)) {
              sum += (float)0.1127499;
            } else {
              sum += (float)0.031247843;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.0043866029;
            } else {
              sum += (float)-0.074520968;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.39409375)) {
            sum += (float)0.053510033;
          } else {
            sum += (float)0.1371527;
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.016511364)) {
    if ( (data[13].fvalue < 2.8406495e-07)) {
      if ( (data[7].fvalue < 0.5)) {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[3].fvalue < 0.28025687)) {
            if ( (data[14].fvalue < 0.071521983)) {
              sum += (float)0.0069473009;
            } else {
              sum += (float)-0.037575778;
            }
          } else {
            if ( (data[14].fvalue < 2.1921078e-06)) {
              sum += (float)0.15577087;
            } else {
              sum += (float)-0.0010072193;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.093850017)) {
            if ( (data[5].fvalue < 0.28967083)) {
              sum += (float)-0.030824387;
            } else {
              sum += (float)-0.19922377;
            }
          } else {
            if ( (data[17].fvalue < 0.048282839)) {
              sum += (float)0.044132233;
            } else {
              sum += (float)-0.022105759;
            }
          }
        }
      } else {
        sum += (float)0.31788853;
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.015160691)) {
          if (  (data[4].fvalue < 1.0153017)) {
            sum += (float)-0.0056919963;
          } else {
            sum += (float)-0.14396358;
          }
        } else {
          if ( (data[6].fvalue < 0.047143161)) {
            sum += (float)-0.078598365;
          } else {
            if ( (data[13].fvalue < 0.0061405399)) {
              sum += (float)0.031029699;
            } else {
              sum += (float)0.1210219;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.20579201)) {
          if (  (data[4].fvalue < 1.233735)) {
            if ( (data[14].fvalue < 0.078093618)) {
              sum += (float)-0.04878189;
            } else {
              sum += (float)0.095344543;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.018817762;
            } else {
              sum += (float)-0.11916228;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.34130824)) {
            if ( (data[14].fvalue < 0.26467276)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.090452306;
            }
          } else {
            if ( (data[13].fvalue < 0.0081735384)) {
              sum += (float)-0.1868998;
            } else {
              sum += (float)-0.11262005;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.39595354)) {
      if (  (data[4].fvalue < 1.2799866)) {
        if ( (data[14].fvalue < 0.048389744)) {
          if (  (data[4].fvalue < 1.0500832)) {
            if ( (data[5].fvalue < 0.90808976)) {
              sum += (float)-0.065145835;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[13].fvalue < 0.063051715)) {
              sum += (float)-0.032848708;
            } else {
              sum += (float)0.050628401;
            }
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            if (  (data[4].fvalue < 1.1113083)) {
              sum += (float)0.10384162;
            } else {
              sum += (float)0.0084564267;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.11687437;
            } else {
              sum += (float)0.28099605;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.28354871)) {
            if ( (data[14].fvalue < 0.12387244)) {
              sum += (float)0.022604154;
            } else {
              sum += (float)0.079611853;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.021779712;
            } else {
              sum += (float)0.043423459;
            }
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.38624504)) {
              sum += (float)-0.050046988;
            } else {
              sum += (float)0.024757231;
            }
          } else {
            if ( (data[14].fvalue < 0.44871423)) {
              sum += (float)-0.018936653;
            } else {
              sum += (float)0.06610579;
            }
          }
        }
      }
    } else {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[14].fvalue < 0.0077182958)) {
          if (  (data[4].fvalue < 1.4314296)) {
            if ( (data[16].fvalue < 0.55277991)) {
              sum += (float)0.04073352;
            } else {
              sum += (float)-0.079519875;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)-0.041332003;
            } else {
              sum += (float)0.037057221;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.076383993)) {
            if ( (data[14].fvalue < 0.11997112)) {
              sum += (float)0.024345702;
            } else {
              sum += (float)0.10447547;
            }
          } else {
            if ( (data[13].fvalue < 0.53973693)) {
              sum += (float)0.0014142406;
            } else {
              sum += (float)0.068349816;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.084054738)) {
          if ( (data[5].fvalue < 0.52275074)) {
            if ( (data[13].fvalue < 0.29394481)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.11161903;
            }
          } else {
            if ( (data[13].fvalue < 0.16068973)) {
              sum += (float)-0.23576839;
            } else {
              sum += (float)-0.10311038;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.16311969)) {
            if ( (data[3].fvalue < 0.65166426)) {
              sum += (float)-0.0065699234;
            } else {
              sum += (float)-0.069921337;
            }
          } else {
            if ( (data[6].fvalue < 0.21879928)) {
              sum += (float)0.04983183;
            } else {
              sum += (float)-0.0036509219;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.090937488)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[16].fvalue < 0.042031143)) {
          if ( (data[14].fvalue < 5.7198115e-08)) {
            if (  (data[4].fvalue < 1.5574999)) {
              sum += (float)-0.0049063219;
            } else {
              sum += (float)0.23269922;
            }
          } else {
            if (  (data[4].fvalue < 0.97951305)) {
              sum += (float)-0.010825021;
            } else {
              sum += (float)0.012666031;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.10291242)) {
            if ( (data[16].fvalue < 0.095892474)) {
              sum += (float)-0.12482137;
            } else {
              sum += (float)-0.021657301;
            }
          } else {
            if ( (data[13].fvalue < 0.0025039157)) {
              sum += (float)-0.0046815369;
            } else {
              sum += (float)0.050687123;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.099265136)) {
          if (  (data[4].fvalue < 1.008682)) {
            if ( (data[6].fvalue < 0.09188655)) {
              sum += (float)0.11066768;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[8].fvalue < 5.5)) {
              sum += (float)-0.067352787;
            } else {
              sum += (float)0.023601156;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.3384397)) {
            sum += (float)0.11050179;
          } else {
            sum += (float)0.012128375;
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.039963119)) {
        sum += (float)0.15796883;
      } else {
        if ( (data[0].fvalue < 2.4559174)) {
          if ( (data[6].fvalue < 0.096114337)) {
            sum += (float)0.12218086;
          } else {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)-0.036937814;
            } else {
              sum += (float)0.058018748;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.0020224615)) {
            if ( (data[14].fvalue < 0.19130021)) {
              sum += (float)-0.14364275;
            } else {
              sum += (float)-0.051568482;
            }
          } else {
            if ( (data[14].fvalue < 0.26132131)) {
              sum += (float)0.022432407;
            } else {
              sum += (float)-0.081533499;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.18249446)) {
      if ( (data[8].fvalue < 3.5)) {
        if ( (data[0].fvalue < 2.9639211)) {
          if ( (data[7].fvalue < 1.5)) {
            if (  (data[4].fvalue < 1.0446005)) {
              sum += (float)-0.050147567;
            } else {
              sum += (float)0.053180795;
            }
          } else {
            if (  (data[4].fvalue < 1.1260393)) {
              sum += (float)0.16871925;
            } else {
              sum += (float)0.042999089;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.10772087)) {
            if ( (data[6].fvalue < 0.10186813)) {
              sum += (float)0.024327476;
            } else {
              sum += (float)-0.031152332;
            }
          } else {
            if ( (data[3].fvalue < 0.40586889)) {
              sum += (float)0.091325909;
            } else {
              sum += (float)0.010777712;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 2.8747396)) {
          sum += (float)0.21999507;
        } else {
          if ( (data[14].fvalue < 0.171065)) {
            sum += (float)-0.045527045;
          } else {
            if ( (data[0].fvalue < 4.1618791)) {
              sum += (float)0.15895337;
            } else {
              sum += (float)0.016824735;
            }
          }
        }
      }
    } else {
      if ( (data[0].fvalue < 2.3326654)) {
        if ( (data[8].fvalue < 3.5)) {
          if ( (data[16].fvalue < 0.17777118)) {
            if ( (data[17].fvalue < 0.05003532)) {
              sum += (float)-0.0078000496;
            } else {
              sum += (float)0.052710641;
            }
          } else {
            if ( (data[3].fvalue < 0.7044723)) {
              sum += (float)0.053948153;
            } else {
              sum += (float)-0.0029800925;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.31880012)) {
            if ( (data[14].fvalue < 0.22771317)) {
              sum += (float)0.056462187;
            } else {
              sum += (float)0.20187461;
            }
          } else {
            if ( (data[14].fvalue < 0.50150949)) {
              sum += (float)-0.0046118991;
            } else {
              sum += (float)0.12159472;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.5328455)) {
          if ( (data[16].fvalue < 0.7726326)) {
            if (  (data[4].fvalue < 1.9088488)) {
              sum += (float)0.0031637526;
            } else {
              sum += (float)-0.046044029;
            }
          } else {
            if ( (data[13].fvalue < 0.52703071)) {
              sum += (float)0.0042053242;
            } else {
              sum += (float)0.096440308;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.61131549)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.01400639;
            } else {
              sum += (float)-0.076068513;
            }
          } else {
            if ( (data[16].fvalue < 0.014243782)) {
              sum += (float)0.070295565;
            } else {
              sum += (float)-0.017720466;
            }
          }
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.11414315)) {
    if (  (data[4].fvalue < 1.2318933)) {
      if ( (data[16].fvalue < 0.14893113)) {
        if ( (data[8].fvalue < 0.5)) {
          if ( (data[16].fvalue < 0.072188146)) {
            sum += (float)-0.18982792;
          } else {
            sum += (float)-0.0039464878;
          }
        } else {
          if ( (data[14].fvalue < 0.0007825786)) {
            sum += (float)0.14891574;
          } else {
            if ( (data[11].fvalue < 0.5)) {
              sum += (float)-0.0011456143;
            } else {
              sum += (float)0.038545407;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.62826693)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.0091847107)) {
              sum += (float)0.0174419;
            } else {
              sum += (float)0.099513724;
            }
          } else {
            sum += (float)0.17758417;
          }
        } else {
          if (  (data[4].fvalue < 1.1074306)) {
            sum += (float)0.021332288;
          } else {
            if ( (data[3].fvalue < 0.96551985)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.072536997;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 0.29979113)) {
        if ( (data[14].fvalue < 0.67570364)) {
          if ( (data[14].fvalue < 0.14726104)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.019269319;
            } else {
              sum += (float)0.1240382;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.02990344;
            } else {
              sum += (float)-0.027032647;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.60971749)) {
            if (  (data[4].fvalue < 1.9251664)) {
              sum += (float)-0.018281292;
            } else {
              sum += (float)-0.097413234;
            }
          } else {
            sum += (float)0.059256293;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.48264021)) {
          if ( (data[15].fvalue < 0.10386682)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.041630473;
            } else {
              sum += (float)-0.096226454;
            }
          } else {
            sum += (float)0.028243255;
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.011601919;
            } else {
              sum += (float)0.038703736;
            }
          } else {
            if ( (data[14].fvalue < 0.852175)) {
              sum += (float)-0.066130348;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    }
  } else {
    if ( (data[17].fvalue < 0.78862685)) {
      if ( (data[3].fvalue < 1.2778332)) {
        if (  (data[4].fvalue < 1.3791052)) {
          if ( (data[8].fvalue < 3.5)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.035201702;
            }
          } else {
            if ( (data[14].fvalue < 0.36370805)) {
              sum += (float)0.040036123;
            } else {
              sum += (float)0.15267847;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.3317194)) {
            if ( (data[14].fvalue < 0.62170869)) {
              sum += (float)-0.024393542;
            } else {
              sum += (float)0.050111663;
            }
          } else {
            if (  (data[4].fvalue < 1.5869055)) {
              sum += (float)0.065049224;
            } else {
              sum += (float)0.0072851316;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.50469649)) {
          if ( (data[14].fvalue < 0.69738728)) {
            if ( (data[16].fvalue < 0.28425521)) {
              sum += (float)-0.11275751;
            } else {
              sum += (float)-0.033026632;
            }
          } else {
            if ( (data[0].fvalue < 3.6781354)) {
              sum += (float)0.030846493;
            } else {
              sum += (float)-0.014519297;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.0044288794)) {
            if ( (data[6].fvalue < 0.43587458)) {
              sum += (float)0.041939303;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.042622801;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 2.3135364)) {
        if ( (data[13].fvalue < 0.26387841)) {
          sum += (float)0.043836165;
        } else {
          sum += (float)0.12272343;
        }
      } else {
        sum += (float)-0;
      }
    }
  }
  if ( (data[13].fvalue < 0.012345755)) {
    if ( (data[13].fvalue < 1.8528158e-07)) {
      if ( (data[17].fvalue < 0.041007634)) {
        if ( (data[14].fvalue < 0.071857467)) {
          if ( (data[16].fvalue < 0.0070065744)) {
            if ( (data[3].fvalue < 0.0615394)) {
              sum += (float)-0.025294838;
            } else {
              sum += (float)0.10623275;
            }
          } else {
            if ( (data[8].fvalue < 0.5)) {
              sum += (float)0.0047424426;
            } else {
              sum += (float)-0.063226283;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.051107831)) {
            if ( (data[0].fvalue < 2.8532538)) {
              sum += (float)-0.24887487;
            } else {
              sum += (float)-0.035550877;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.00024309503;
            } else {
              sum += (float)0.025622131;
            }
          }
        }
      } else {
        if ( (data[14].fvalue < 0.044497222)) {
          if (  (data[4].fvalue < 1.5304)) {
            sum += (float)-0.21245386;
          } else {
            sum += (float)-0.066389456;
          }
        } else {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.25100026)) {
              sum += (float)-0.083834186;
            } else {
              sum += (float)-0.0031188261;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.055775478;
            } else {
              sum += (float)-0.012109435;
            }
          }
        }
      }
    } else {
      if ( (data[13].fvalue < 0.0025703679)) {
        if ( (data[16].fvalue < 0.00023695033)) {
          if ( (data[4].fvalue < 0.99541533)) {
            sum += (float)-0.057703193;
          } else {
            sum += (float)-0.1805234;
          }
        } else {
          sum += (float)-0.02732357;
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.0088036545)) {
            sum += (float)-0.088162184;
          } else {
            if ( (data[16].fvalue < 0.0034716865)) {
              sum += (float)-0;
            } else {
              sum += (float)0.077226639;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.044169825)) {
            sum += (float)0.068632163;
          } else {
            if ( (data[14].fvalue < 0.1075583)) {
              sum += (float)-0.10343582;
            } else {
              sum += (float)-0.025904536;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.074760951)) {
      if ( (data[0].fvalue < 1.9155881)) {
        if ( (data[16].fvalue < 0.0078599928)) {
          if ( (data[8].fvalue < 1.5)) {
            sum += (float)-0.067733303;
          } else {
            if (  (data[4].fvalue < 1.2215649)) {
              sum += (float)0.14540733;
            } else {
              sum += (float)0.055068608;
            }
          }
        } else {
          sum += (float)0.1541947;
        }
      } else {
        if ( (data[3].fvalue < 0.015543282)) {
          if ( (data[14].fvalue < 0.1098354)) {
            sum += (float)0.11204163;
          } else {
            sum += (float)0.03526672;
          }
        } else {
          if ( (data[5].fvalue < 0.061023436)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.034049459;
            } else {
              sum += (float)-0.018717369;
            }
          } else {
            if ( (data[14].fvalue < 0.035461351)) {
              sum += (float)-0.02738381;
            } else {
              sum += (float)0.08125826;
            }
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.085617125)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.007719567)) {
            if ( (data[14].fvalue < 1.5247995e-05)) {
              sum += (float)-0.0040904297;
            } else {
              sum += (float)-0.073284402;
            }
          } else {
            if (  (data[4].fvalue < 1.007906)) {
              sum += (float)-0.03707983;
            } else {
              sum += (float)0.033563014;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.044370264)) {
            sum += (float)0.075581051;
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)-0.09510947;
            } else {
              sum += (float)-0.012469862;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.11934917)) {
          if ( (data[0].fvalue < 4.2120209)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0;
            } else {
              sum += (float)0.12359509;
            }
          } else {
            if ( (data[3].fvalue < 0.19822139)) {
              sum += (float)0.043184407;
            } else {
              sum += (float)-0.027300401;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.00033017521)) {
            if ( (data[14].fvalue < 0.57062149)) {
              sum += (float)-0.013104373;
            } else {
              sum += (float)0.034856595;
            }
          } else {
            if ( (data[6].fvalue < 0.48563361)) {
              sum += (float)0.054862645;
            } else {
              sum += (float)-0.000898497;
            }
          }
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.13530859)) {
    if ( (data[16].fvalue < 0.053978939)) {
      if ( (data[13].fvalue < 0.67967832)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[6].fvalue < 0.026798232)) {
            if ( (data[14].fvalue < 0.024363555)) {
              sum += (float)0.052704662;
            } else {
              sum += (float)-0.12700996;
            }
          } else {
            if ( (data[14].fvalue < 0.082527705)) {
              sum += (float)0.03501571;
            } else {
              sum += (float)-9.9372519e-05;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.042787664)) {
            if (  (data[4].fvalue < 0.99446011)) {
              sum += (float)-0.010945128;
            } else {
              sum += (float)-0.14572194;
            }
          } else {
            if ( (data[6].fvalue < 0.076292545)) {
              sum += (float)0.038857777;
            } else {
              sum += (float)-0.0098146517;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 3.5)) {
          if (  (data[4].fvalue < 1.7576498)) {
            if ( (data[6].fvalue < 0.39353564)) {
              sum += (float)0.052336078;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)-0.067583531;
          }
        } else {
          if ( (data[8].fvalue < 3.5)) {
            sum += (float)0.20696326;
          } else {
            sum += (float)0.062035825;
          }
        }
      }
    } else {
      if ( (data[3].fvalue < 0.051486008)) {
        if ( (data[13].fvalue < 0.00099424459)) {
          if ( (data[14].fvalue < 0.10659528)) {
            if (  (data[4].fvalue < 1.1824738)) {
              sum += (float)0.023214359;
            } else {
              sum += (float)-0.085293911;
            }
          } else {
            sum += (float)0.046345145;
          }
        } else {
          if (  (data[4].fvalue < 1.2503976)) {
            sum += (float)0.12100584;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[14].fvalue < 0.12802909)) {
          if (  (data[4].fvalue < 1.8085184)) {
            if (  (data[4].fvalue < 1.207324)) {
              sum += (float)-0.02848994;
            } else {
              sum += (float)-0.085447922;
            }
          } else {
            sum += (float)0.058266215;
          }
        } else {
          if ( (data[3].fvalue < 0.29725432)) {
            if (  (data[4].fvalue < 1.4549301)) {
              sum += (float)0.042327281;
            } else {
              sum += (float)-0.051353227;
            }
          } else {
            if ( (data[13].fvalue < 0.11472102)) {
              sum += (float)-0.044640359;
            } else {
              sum += (float)0.0068850676;
            }
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[7].fvalue < 2.5)) {
        if ( (data[13].fvalue < 0.01100545)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[17].fvalue < 0.0011623437)) {
              sum += (float)-0.0054621203;
            } else {
              sum += (float)-0.047116745;
            }
          } else {
            if ( (data[14].fvalue < 0.062261038)) {
              sum += (float)-0.080480017;
            } else {
              sum += (float)0.051948216;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.0070010764)) {
            if ( (data[6].fvalue < 0.28855452)) {
              sum += (float)0.040872525;
            } else {
              sum += (float)-0.010098413;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.047620002;
            } else {
              sum += (float)-0.013738063;
            }
          }
        }
      } else {
        if ( (data[11].fvalue < 0.5)) {
          if (  (data[4].fvalue < 1.4500117)) {
            sum += (float)-0.020758469;
          } else {
            if ( (data[13].fvalue < 0.38530558)) {
              sum += (float)-0.16916505;
            } else {
              sum += (float)-0.063000485;
            }
          }
        } else {
          sum += (float)0.019017095;
        }
      }
    } else {
      if (  (data[4].fvalue < 1.6902201)) {
        if ( (data[7].fvalue < 2.5)) {
          if ( (data[3].fvalue < 1.0370646)) {
            if ( (data[16].fvalue < 0.25414798)) {
              sum += (float)0.010717906;
            } else {
              sum += (float)0.1050617;
            }
          } else {
            sum += (float)-0.045648392;
          }
        } else {
          if ( (data[6].fvalue < 0.34865278)) {
            if (  (data[4].fvalue < 1.4017799)) {
              sum += (float)0.27089366;
            } else {
              sum += (float)0.14111748;
            }
          } else {
            sum += (float)0.064119317;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.95919853)) {
          if ( (data[15].fvalue < 0.00046437467)) {
            if ( (data[17].fvalue < 0.27990961)) {
              sum += (float)-0.10455393;
            } else {
              sum += (float)0.014258024;
            }
          } else {
            if ( (data[6].fvalue < 0.59298003)) {
              sum += (float)0.079106972;
            } else {
              sum += (float)-0.014206039;
            }
          }
        } else {
          sum += (float)0.053314526;
        }
      }
    }
  }
  if ( (data[16].fvalue < 0.6495595)) {
    if ( (data[4].fvalue < 1.2973731)) {
      if ( (data[16].fvalue < 0.14253119)) {
        if ( (data[13].fvalue < 0.14361539)) {
          if ( (data[14].fvalue < 0.45910206)) {
            if (  (data[4].fvalue < 1.1452107)) {
              sum += (float)0.0028257894;
            } else {
              sum += (float)-0.025540914;
            }
          } else {
            if (  (data[4].fvalue < 1.1100539)) {
              sum += (float)-0.011142401;
            } else {
              sum += (float)0.057600219;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.63353527)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)-0.0025098671;
            } else {
              sum += (float)0.091446243;
            }
          } else {
            if ( (data[13].fvalue < 0.42646179)) {
              sum += (float)-0.022458535;
            } else {
              sum += (float)0.04088138;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.51012903)) {
          if ( (data[14].fvalue < 0.01190581)) {
            if ( (data[16].fvalue < 0.17393345)) {
              sum += (float)-0.065332331;
            } else {
              sum += (float)0.02867445;
            }
          } else {
            if ( (data[16].fvalue < 0.21694076)) {
              sum += (float)0.054067325;
            } else {
              sum += (float)0.1122033;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.027581699)) {
            if (  (data[4].fvalue < 1.1316307)) {
              sum += (float)0.026217265;
            } else {
              sum += (float)-0.048418447;
            }
          } else {
            if ( (data[14].fvalue < 0.094294414)) {
              sum += (float)0.00063460943;
            } else {
              sum += (float)0.060475584;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.39796045)) {
        if (  (data[4].fvalue < 1.6496561)) {
          if ( (data[12].fvalue < 0.5)) {
            if ( (data[10].fvalue < 0.5)) {
              sum += (float)-0.023865523;
            } else {
              sum += (float)0.0045108455;
            }
          } else {
            sum += (float)0.10903449;
          }
        } else {
          if ( (data[17].fvalue < 0.055036999)) {
            if ( (data[16].fvalue < 0.016901426)) {
              sum += (float)0.038433302;
            } else {
              sum += (float)-0.024459178;
            }
          } else {
            if ( (data[15].fvalue < 0.00072224753)) {
              sum += (float)-0.074207097;
            } else {
              sum += (float)-0.0049395841;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[14].fvalue < 0.092989765)) {
            if ( (data[13].fvalue < 0.0046656271)) {
              sum += (float)-0.092475489;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[3].fvalue < 0.9603489)) {
              sum += (float)0.034727056;
            } else {
              sum += (float)-0.031006195;
            }
          }
        } else {
          if (  (data[4].fvalue < 2.0224824)) {
            if ( (data[13].fvalue < 0.26368672)) {
              sum += (float)0.12390783;
            } else {
              sum += (float)0.019664973;
            }
          } else {
            sum += (float)-0.0017166802;
          }
        }
      }
    }
  } else {
    if ( (data[4].fvalue < 1.503787)) {
      if ( (data[16].fvalue < 0.79316968)) {
        sum += (float)-0;
      } else {
        sum += (float)-0.10354026;
      }
    } else {
      if ( (data[14].fvalue < 0.15964447)) {
        if ( (data[16].fvalue < 1.2880878)) {
          if (  (data[4].fvalue < 2.1475344)) {
            if ( (data[3].fvalue < 0.2212508)) {
              sum += (float)0.066434458;
            } else {
              sum += (float)0.020720791;
            }
          } else {
            sum += (float)-0.032625429;
          }
        } else {
          if ( (data[13].fvalue < 0.35791546)) {
            if ( (data[14].fvalue < 0.015407601)) {
              sum += (float)-0.057765961;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.065992683;
          }
        }
      } else {
        if ( (data[16].fvalue < 1.4617056)) {
          if (  (data[4].fvalue < 2.2936034)) {
            if ( (data[1].fvalue < 0.87209177)) {
              sum += (float)0.090530381;
            } else {
              sum += (float)0.035748892;
            }
          } else {
            sum += (float)-0.048503131;
          }
        } else {
          sum += (float)0.10976039;
        }
      }
    }
  }
  if ( (data[12].fvalue < 0.5)) {
    if ( (data[13].fvalue < 0.29726177)) {
      if ( (data[8].fvalue < 4.5)) {
        if ( (data[17].fvalue < 0.00010586449)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[3].fvalue < 0.20980346)) {
              sum += (float)-0.01223792;
            } else {
              sum += (float)0.01093562;
            }
          } else {
            if ( (data[6].fvalue < 0.023526002)) {
              sum += (float)0.093629986;
            } else {
              sum += (float)-0.017970899;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.021350605)) {
            if ( (data[16].fvalue < 0.026228152)) {
              sum += (float)0.18948527;
            } else {
              sum += (float)0.052780244;
            }
          } else {
            if ( (data[3].fvalue < 0.19958201)) {
              sum += (float)0.031999599;
            } else {
              sum += (float)-0.0075228591;
            }
          }
        }
      } else {
        if ( (data[3].fvalue < 0.23108593)) {
          if (  (data[4].fvalue < 1.3496169)) {
            if ( (data[14].fvalue < 0.29230773)) {
              sum += (float)0.024739508;
            } else {
              sum += (float)0.21175309;
            }
          } else {
            if ( (data[14].fvalue < 0.50374329)) {
              sum += (float)-0.058152903;
            } else {
              sum += (float)0.06488733;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.77770507)) {
            if ( (data[17].fvalue < 0.088865243)) {
              sum += (float)-0.033081066;
            } else {
              sum += (float)0.075800985;
            }
          } else {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.11511803;
            } else {
              sum += (float)-0;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.71013832)) {
        if ( (data[0].fvalue < 2.4806204)) {
          if ( (data[6].fvalue < 0.33403003)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.024043068;
            } else {
              sum += (float)0.15919319;
            }
          } else {
            if ( (data[13].fvalue < 0.46086138)) {
              sum += (float)-0;
            } else {
              sum += (float)0.050711643;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.57412434)) {
            if ( (data[6].fvalue < 0.23471522)) {
              sum += (float)0.035574414;
            } else {
              sum += (float)-0.0020674958;
            }
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.060848426;
            } else {
              sum += (float)-0.0097615281;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 1.0979631)) {
          if ( (data[14].fvalue < 0.21854177)) {
            if ( (data[13].fvalue < 0.95681268)) {
              sum += (float)-0.022784309;
            } else {
              sum += (float)-0.096356936;
            }
          } else {
            sum += (float)-0;
          }
        } else {
          sum += (float)0.032835796;
        }
      }
    }
  } else {
    if (  (data[4].fvalue < 1.7849216)) {
      if ( (data[3].fvalue < 0.54492706)) {
        sum += (float)0.10989995;
      } else {
        sum += (float)-0.041822471;
      }
    } else {
      sum += (float)-0.25806871;
    }
  }
  if ( (data[3].fvalue < 1.0900095)) {
    if ( (data[7].fvalue < 3.5)) {
      if ( (data[14].fvalue < 0.35362619)) {
        if ( (data[8].fvalue < 3.5)) {
          if (  (data[4].fvalue < 0.81359267)) {
            if ( (data[5].fvalue < 1.6022322)) {
              sum += (float)0.10077687;
            } else {
              sum += (float)-0;
            }
          } else {
            if ( (data[5].fvalue < 1.7819636)) {
              sum += (float)-0.0011354859;
            } else {
              sum += (float)-0.085376568;
            }
          }
        } else {
          if ( (data[16].fvalue < 0.0105012)) {
            if (  (data[4].fvalue < 1.0775324)) {
              sum += (float)0.033161841;
            } else {
              sum += (float)-0.086855851;
            }
          } else {
            if (  (data[4].fvalue < 1.2727369)) {
              sum += (float)0.1462584;
            } else {
              sum += (float)-0.00023415407;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 4.5)) {
          if (  (data[4].fvalue < 1.0790228)) {
            if ( (data[3].fvalue < 0.79131579)) {
              sum += (float)-0.090747967;
            } else {
              sum += (float)0.0049698078;
            }
          } else {
            if (  (data[4].fvalue < 1.550571)) {
              sum += (float)0.019096533;
            } else {
              sum += (float)-0.010060301;
            }
          }
        } else {
          if ( (data[13].fvalue < 0.028628454)) {
            if (  (data[4].fvalue < 1.2910798)) {
              sum += (float)0.057615358;
            } else {
              sum += (float)-0.054849003;
            }
          } else {
            if (  (data[4].fvalue < 1.3412971)) {
              sum += (float)0.1590388;
            } else {
              sum += (float)0.044626478;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.0595548)) {
        sum += (float)0.13602108;
      } else {
        if ( (data[13].fvalue < 0.62922418)) {
          if (  (data[4].fvalue < 1.3344424)) {
            if ( (data[16].fvalue < 0.025656674)) {
              sum += (float)-0;
            } else {
              sum += (float)0.20013119;
            }
          } else {
            if ( (data[16].fvalue < 0.26060292)) {
              sum += (float)-0.047775231;
            } else {
              sum += (float)0.031430673;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.8026365)) {
            sum += (float)0.15817329;
          } else {
            sum += (float)0.0056020161;
          }
        }
      }
    }
  } else {
    if ( (data[8].fvalue < 2.5)) {
      if ( (data[4].fvalue < 1.2190503)) {
        if ( (data[7].fvalue < 1.5)) {
          if ( (data[2].fvalue < 0.3001563)) {
            sum += (float)-0.035407685;
          } else {
            if ( (data[14].fvalue < 0.74212438)) {
              sum += (float)0.05336323;
            } else {
              sum += (float)0.00018249592;
            }
          }
        } else {
          sum += (float)-0.10467931;
        }
      } else {
        if (  (data[4].fvalue < 1.9017702)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[13].fvalue < 0.635674)) {
              sum += (float)-0.05836473;
            } else {
              sum += (float)0.0054188496;
            }
          } else {
            if ( (data[14].fvalue < 0.58816433)) {
              sum += (float)-0.020316791;
            } else {
              sum += (float)0.066613004;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.82963693)) {
            if ( (data[13].fvalue < 0.067775145)) {
              sum += (float)0.021023734;
            } else {
              sum += (float)-0.014095888;
            }
          } else {
            sum += (float)0.11224608;
          }
        }
      }
    } else {
      if ( (data[14].fvalue < 0.23460065)) {
        sum += (float)-0.1297963;
      } else {
        if ( (data[3].fvalue < 2.428453)) {
          if ( (data[13].fvalue < 0.15703152)) {
            if ( (data[7].fvalue < 0.5)) {
              sum += (float)-0;
            } else {
              sum += (float)-0.095761307;
            }
          } else {
            if ( (data[13].fvalue < 0.68427372)) {
              sum += (float)-0;
            } else {
              sum += (float)0.079471432;
            }
          }
        } else {
          sum += (float)-0.13237149;
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.0058785444)) {
    if ( (data[14].fvalue < 2.1389603e-09)) {
      if ( (data[7].fvalue < 1.5)) {
        if ( (data[17].fvalue < 0.00095734594)) {
          if ( (data[16].fvalue < 0.022282999)) {
            if ( (data[13].fvalue < 0.046943009)) {
              sum += (float)0.15368904;
            } else {
              sum += (float)-0.033298947;
            }
          } else {
            if (  (data[4].fvalue < 1.7861197)) {
              sum += (float)-0.016734531;
            } else {
              sum += (float)0.027472163;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.21013376)) {
            sum += (float)-0.19850308;
          } else {
            if (  (data[4].fvalue < 1.7191744)) {
              sum += (float)-0.080399692;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        if ( (data[13].fvalue < 0.66701829)) {
          if ( (data[5].fvalue < 0.34963691)) {
            if ( (data[3].fvalue < 0.41581863)) {
              sum += (float)0.073709421;
            } else {
              sum += (float)0.014562231;
            }
          } else {
            sum += (float)-0.039846819;
          }
        } else {
          sum += (float)0.13525565;
        }
      }
    } else {
      if ( (data[14].fvalue < 0.00095848937)) {
        sum += (float)-0.20167616;
      } else {
        if ( (data[7].fvalue < 0.5)) {
          sum += (float)-0.11884833;
        } else {
          if ( (data[14].fvalue < 0.0034793885)) {
            if ( (data[5].fvalue < 0.33594036)) {
              sum += (float)-0.030183958;
            } else {
              sum += (float)-0.099172451;
            }
          } else {
            if ( (data[1].fvalue < 2.1435869)) {
              sum += (float)-0.019815166;
            } else {
              sum += (float)0.028165376;
            }
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.49207851)) {
      if ( (data[17].fvalue < 0.11638336)) {
        if ( (data[4].fvalue < 1.2119584)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[0].fvalue < 3.5680099)) {
              sum += (float)-0.0019577036;
            } else {
              sum += (float)0.023576764;
            }
          } else {
            if ( (data[3].fvalue < 0.74403381)) {
              sum += (float)0.055444174;
            } else {
              sum += (float)-0.071087539;
            }
          }
        } else {
          if ( (data[10].fvalue < 0.5)) {
            if ( (data[14].fvalue < 1.0364573)) {
              sum += (float)-0.029467814;
            } else {
              sum += (float)0.090694308;
            }
          } else {
            if ( (data[14].fvalue < 0.1227362)) {
              sum += (float)-0.023822827;
            } else {
              sum += (float)0.011240231;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.7313833)) {
          if ( (data[3].fvalue < 0.79318392)) {
            if ( (data[8].fvalue < 4.5)) {
              sum += (float)0.042278651;
            } else {
              sum += (float)0.16847438;
            }
          } else {
            if ( (data[16].fvalue < 0.08718057)) {
              sum += (float)-0.024804397;
            } else {
              sum += (float)0.051559467;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.5786978)) {
            if ( (data[5].fvalue < 0.20630959)) {
              sum += (float)-0.078751259;
            } else {
              sum += (float)-0.0087000998;
            }
          } else {
            if ( (data[16].fvalue < 0.072902888)) {
              sum += (float)0.032140192;
            } else {
              sum += (float)-0.029820969;
            }
          }
        }
      }
    } else {
      if ( (data[17].fvalue < 0.59213454)) {
        if ( (data[0].fvalue < 4.806529)) {
          if ( (data[8].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.53170693)) {
              sum += (float)-0;
            } else {
              sum += (float)0.054923464;
            }
          } else {
            if ( (data[6].fvalue < 0.5559833)) {
              sum += (float)0.090671644;
            } else {
              sum += (float)-0;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.18098433)) {
            sum += (float)-0.063668914;
          } else {
            sum += (float)-0;
          }
        }
      } else {
        sum += (float)0.10436691;
      }
    }
  }
  if ( (data[14].fvalue < 0.26558441)) {
    if (  (data[4].fvalue < 0.86571348)) {
      if ( (data[8].fvalue < 1.5)) {
        if ( (data[3].fvalue < 0.61343896)) {
          if ( (data[14].fvalue < 0.11437185)) {
            sum += (float)0.084476918;
          } else {
            sum += (float)-0.094162136;
          }
        } else {
          if ( (data[13].fvalue < 0.0052652266)) {
            sum += (float)0.16196069;
          } else {
            sum += (float)0.062396243;
          }
        }
      } else {
        if ( (data[5].fvalue < 1.4356678)) {
          sum += (float)-0.077580646;
        } else {
          sum += (float)0.054596826;
        }
      }
    } else {
      if ( (data[3].fvalue < 0.55237979)) {
        if ( (data[16].fvalue < 0.17722338)) {
          if (  (data[4].fvalue < 1.1861572)) {
            if ( (data[3].fvalue < 0.010205043)) {
              sum += (float)-0.090611383;
            } else {
              sum += (float)0.0040469742;
            }
          } else {
            if ( (data[17].fvalue < 7.2787523e-05)) {
              sum += (float)-0.042377759;
            } else {
              sum += (float)0.014639976;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.30478)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.027251406;
            } else {
              sum += (float)0.12448898;
            }
          } else {
            if ( (data[13].fvalue < 0.018150877)) {
              sum += (float)-0.013622784;
            } else {
              sum += (float)0.01623622;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 2.5)) {
          if ( (data[4].fvalue < 1.7733669)) {
            if ( (data[1].fvalue < 0.73699456)) {
              sum += (float)-0.10661904;
            } else {
              sum += (float)-0.018610412;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)0.02454816;
            } else {
              sum += (float)-0.018524732;
            }
          }
        } else {
          if ( (data[4].fvalue < 1.2775229)) {
            if ( (data[13].fvalue < 0.069540694)) {
              sum += (float)-0.26534986;
            } else {
              sum += (float)-0.086154372;
            }
          } else {
            if ( (data[13].fvalue < 0.1970246)) {
              sum += (float)0.064747497;
            } else {
              sum += (float)-0.079053357;
            }
          }
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.34236312)) {
      if ( (data[8].fvalue < 2.5)) {
        if (  (data[4].fvalue < 1.1040704)) {
          sum += (float)-0.16923586;
        } else {
          if ( (data[14].fvalue < 0.55113721)) {
            if ( (data[13].fvalue < 0.073433571)) {
              sum += (float)0.028413827;
            } else {
              sum += (float)-0.035531078;
            }
          } else {
            if (  (data[4].fvalue < 1.4606332)) {
              sum += (float)-0.14522928;
            } else {
              sum += (float)-0.022420423;
            }
          }
        }
      } else {
        if (  (data[4].fvalue < 1.279556)) {
          if ( (data[7].fvalue < 1.5)) {
            if ( (data[14].fvalue < 0.32918513)) {
              sum += (float)0.019380979;
            } else {
              sum += (float)0.11887158;
            }
          } else {
            sum += (float)0.21996762;
          }
        } else {
          if ( (data[13].fvalue < 0.11901298)) {
            if ( (data[14].fvalue < 0.44953662)) {
              sum += (float)-0.088578291;
            } else {
              sum += (float)0.018020939;
            }
          } else {
            if (  (data[4].fvalue < 1.427664)) {
              sum += (float)0.14095353;
            } else {
              sum += (float)0.030756496;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.1167627)) {
        if ( (data[8].fvalue < 2.5)) {
          if (  (data[4].fvalue < 0.96757269)) {
            if ( (data[3].fvalue < 0.69275033)) {
              sum += (float)-0.19399779;
            } else {
              sum += (float)-0.066599004;
            }
          } else {
            if ( (data[3].fvalue < 0.50586998)) {
              sum += (float)-0.099065654;
            } else {
              sum += (float)0.031271309;
            }
          }
        } else {
          sum += (float)0.091238931;
        }
      } else {
        if ( (data[13].fvalue < 0.29964554)) {
          if ( (data[7].fvalue < 2.5)) {
            if ( (data[17].fvalue < 1.4663002)) {
              sum += (float)-0;
            } else {
              sum += (float)0.074480079;
            }
          } else {
            if (  (data[4].fvalue < 1.1923759)) {
              sum += (float)0.043673556;
            } else {
              sum += (float)-0.059048448;
            }
          }
        } else {
          if ( (data[8].fvalue < 7.5)) {
            if ( (data[3].fvalue < 0.93103427)) {
              sum += (float)0.065256536;
            } else {
              sum += (float)0.0073747216;
            }
          } else {
            sum += (float)-0.063963346;
          }
        }
      }
    }
  }
  if ( (data[14].fvalue < 0.48242611)) {
    if ( (data[5].fvalue < 0.25021115)) {
      if ( (data[3].fvalue < 0.11637266)) {
        if ( (data[14].fvalue < 0.10248508)) {
          if ( (data[13].fvalue < 0.043398075)) {
            if ( (data[16].fvalue < 0.098654337)) {
              sum += (float)-0.03494079;
            } else {
              sum += (float)0.0094864788;
            }
          } else {
            if (  (data[4].fvalue < 1.1087533)) {
              sum += (float)0.12938923;
            } else {
              sum += (float)0.019508161;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.3024697)) {
            if ( (data[16].fvalue < 0.0039480045)) {
              sum += (float)0.035812255;
            } else {
              sum += (float)0.1292661;
            }
          } else {
            if ( (data[13].fvalue < 0.27052963)) {
              sum += (float)-0.0026766737;
            } else {
              sum += (float)0.068456277;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.43571621)) {
          if (  (data[4].fvalue < 1.2161715)) {
            if ( (data[14].fvalue < 0.092014298)) {
              sum += (float)-0.0082880072;
            } else {
              sum += (float)0.041183855;
            }
          } else {
            if ( (data[14].fvalue < 0.24906008)) {
              sum += (float)-0.021115698;
            } else {
              sum += (float)-0.0010743147;
            }
          }
        } else {
          if ( (data[0].fvalue < 2.9036582)) {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.024235751;
            } else {
              sum += (float)0.087457515;
            }
          } else {
            if ( (data[17].fvalue < 0.23473264)) {
              sum += (float)-0.0032212858;
            } else {
              sum += (float)0.039452717;
            }
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.1816211)) {
        if ( (data[3].fvalue < 0.13076493)) {
          if ( (data[14].fvalue < 0.063045293)) {
            if (  (data[4].fvalue < 0.96095228)) {
              sum += (float)-0.071097821;
            } else {
              sum += (float)0.042905249;
            }
          } else {
            if (  (data[4].fvalue < 1.0036253)) {
              sum += (float)-0.20339337;
            } else {
              sum += (float)-0.021675041;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.36250269)) {
            if ( (data[14].fvalue < 0.11602162)) {
              sum += (float)-0;
            } else {
              sum += (float)0.082193635;
            }
          } else {
            if ( (data[13].fvalue < 1.4744163e-07)) {
              sum += (float)0.010373088;
            } else {
              sum += (float)-0.015354441;
            }
          }
        }
      } else {
        if ( (data[16].fvalue < 0.00012653753)) {
          if ( (data[17].fvalue < 0.00088638917)) {
            if ( (data[8].fvalue < 3.5)) {
              sum += (float)-0.034302551;
            } else {
              sum += (float)-0.13349815;
            }
          } else {
            if ( (data[13].fvalue < 0.0069175339)) {
              sum += (float)-0.046184842;
            } else {
              sum += (float)0.015763229;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.086894795)) {
            if ( (data[13].fvalue < 0.16899678)) {
              sum += (float)-0.021375457;
            } else {
              sum += (float)0.0038450302;
            }
          } else {
            if ( (data[8].fvalue < 1.5)) {
              sum += (float)0.0049018781;
            } else {
              sum += (float)0.086527817;
            }
          }
        }
      }
    }
  } else {
    if ( (data[16].fvalue < 0.076718092)) {
      if ( (data[11].fvalue < 1.5)) {
        if ( (data[17].fvalue < 0.29869446)) {
          if ( (data[4].fvalue < 1.4068444)) {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)0.010856408;
            } else {
              sum += (float)0.067806236;
            }
          } else {
            if ( (data[13].fvalue < 0.14470407)) {
              sum += (float)-0.027709661;
            } else {
              sum += (float)0.037242655;
            }
          }
        } else {
          if ( (data[8].fvalue < 2.5)) {
            if ( (data[5].fvalue < 0.47078133)) {
              sum += (float)0.13040054;
            } else {
              sum += (float)0.0028051888;
            }
          } else {
            sum += (float)-0;
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          sum += (float)-0.099317886;
        } else {
          sum += (float)-0;
        }
      }
    } else {
      if ( (data[16].fvalue < 0.82799101)) {
        if (  (data[4].fvalue < 1.9456053)) {
          if ( (data[5].fvalue < 0.44531775)) {
            if ( (data[16].fvalue < 0.25485396)) {
              sum += (float)-0.0076953149;
            } else {
              sum += (float)0.04251904;
            }
          } else {
            if ( (data[16].fvalue < 0.14000005)) {
              sum += (float)-0.089912891;
            } else {
              sum += (float)-0.013248156;
            }
          }
        } else {
          if ( (data[15].fvalue < 0.41108531)) {
            if ( (data[17].fvalue < 0.4917942)) {
              sum += (float)-0.11166084;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.042245168;
          }
        }
      } else {
        if ( (data[16].fvalue < 1.1509522)) {
          sum += (float)0.013351006;
        } else {
          sum += (float)0.081298269;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.37425268)) {
    if ( (data[8].fvalue < 9.5)) {
      if ( (data[17].fvalue < 0.00039277354)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[15].fvalue < 0.025556553)) {
            if ( (data[7].fvalue < 2.5)) {
              sum += (float)0.005488676;
            } else {
              sum += (float)-0.078226984;
            }
          } else {
            if ( (data[10].fvalue < 1.5)) {
              sum += (float)-0.069023035;
            } else {
              sum += (float)0.01908439;
            }
          }
        } else {
          if ( (data[0].fvalue < 1.9535849)) {
            if ( (data[6].fvalue < 0.31481224)) {
              sum += (float)0.037600443;
            } else {
              sum += (float)-0.016312435;
            }
          } else {
            if ( (data[15].fvalue < 0.13096249)) {
              sum += (float)-0.021245103;
            } else {
              sum += (float)0.055135012;
            }
          }
        }
      } else {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[13].fvalue < 0.0040394897)) {
            if ( (data[17].fvalue < 0.1688595)) {
              sum += (float)-0.063543715;
            } else {
              sum += (float)-0.014603634;
            }
          } else {
            if ( (data[17].fvalue < 0.032977134)) {
              sum += (float)0.13103084;
            } else {
              sum += (float)0.019395029;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.2919846)) {
            if ( (data[5].fvalue < 0.66486394)) {
              sum += (float)0.10887626;
            } else {
              sum += (float)-0.032593645;
            }
          } else {
            if (  (data[4].fvalue < 1.7098186)) {
              sum += (float)0.026505817;
            } else {
              sum += (float)-0.0078797881;
            }
          }
        }
      }
    } else {
      sum += (float)0.085062049;
    }
  } else {
    if (  (data[4].fvalue < 1.5196998)) {
      if ( (data[6].fvalue < 0.50780427)) {
        if ( (data[0].fvalue < 4.1957273)) {
          if ( (data[7].fvalue < 3.5)) {
            if ( (data[13].fvalue < 0.42765868)) {
              sum += (float)0.00989269;
            } else {
              sum += (float)0.055337157;
            }
          } else {
            if ( (data[3].fvalue < 0.66008574)) {
              sum += (float)0.15968674;
            } else {
              sum += (float)0.040788505;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.21727978)) {
            sum += (float)0.016423661;
          } else {
            sum += (float)-0.02222262;
          }
        }
      } else {
        if ( (data[0].fvalue < 1.9684486)) {
          sum += (float)0.0011939765;
        } else {
          sum += (float)-0.046318606;
        }
      }
    } else {
      if ( (data[10].fvalue < 1.5)) {
        if ( (data[16].fvalue < 0.63662022)) {
          if ( (data[14].fvalue < 0.49052027)) {
            if ( (data[17].fvalue < 0.49168041)) {
              sum += (float)-0.020293884;
            } else {
              sum += (float)0.037209224;
            }
          } else {
            if ( (data[13].fvalue < 0.48720193)) {
              sum += (float)-0;
            } else {
              sum += (float)0.091815755;
            }
          }
        } else {
          if (  (data[4].fvalue < 1.9268603)) {
            if ( (data[16].fvalue < 0.87166202)) {
              sum += (float)0.028065162;
            } else {
              sum += (float)-0;
            }
          } else {
            sum += (float)0.11252748;
          }
        }
      } else {
        if ( (data[15].fvalue < 0.0015633686)) {
          if (  (data[4].fvalue < 1.8958902)) {
            sum += (float)-0.0076533207;
          } else {
            sum += (float)-0.13039516;
          }
        } else {
          sum += (float)0.0083965603;
        }
      }
    }
  }
  if ( (data[13].fvalue < 0.090996958)) {
    if ( (data[7].fvalue < 1.5)) {
      if ( (data[6].fvalue < 0.85129941)) {
        if ( (data[4].fvalue < 0.71877456)) {
          sum += (float)0.07455714;
        } else {
          if ( (data[5].fvalue < 1.7122197)) {
            if ( (data[5].fvalue < 0.59024358)) {
              sum += (float)-0.0040762904;
            } else {
              sum += (float)0.005557653;
            }
          } else {
            if ( (data[3].fvalue < 0.31916264)) {
              sum += (float)-0.1100402;
            } else {
              sum += (float)-0;
            }
          }
        }
      } else {
        sum += (float)0.059343375;
      }
    } else {
      if ( (data[16].fvalue < 0.0005833779)) {
        if (  (data[4].fvalue < 0.99257469)) {
          if ( (data[3].fvalue < 0.21094878)) {
            sum += (float)0.15002114;
          } else {
            if ( (data[14].fvalue < 0.075286724)) {
              sum += (float)-0.13103741;
            } else {
              sum += (float)0.028912129;
            }
          }
        } else {
          if ( (data[17].fvalue < 0.055520043)) {
            if ( (data[14].fvalue < 0.20984286)) {
              sum += (float)-0.15046999;
            } else {
              sum += (float)-0.030246742;
            }
          } else {
            if ( (data[14].fvalue < 0.2208766)) {
              sum += (float)0.11600758;
            } else {
              sum += (float)-0.095349558;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 0.38263366)) {
          if (  (data[4].fvalue < 1.2293415)) {
            sum += (float)0.1739666;
          } else {
            if ( (data[17].fvalue < 0.0011452558)) {
              sum += (float)-0.01554352;
            } else {
              sum += (float)0.069165647;
            }
          }
        } else {
          sum += (float)-0.067394622;
        }
      }
    }
  } else {
    if ( (data[3].fvalue < 0.20001596)) {
      if (  (data[4].fvalue < 1.6304581)) {
        if ( (data[8].fvalue < 2.5)) {
          if (  (data[4].fvalue < 1.1200671)) {
            sum += (float)0.089138985;
          } else {
            if ( (data[16].fvalue < 0.0061249034)) {
              sum += (float)-0.025729766;
            } else {
              sum += (float)0.040790439;
            }
          }
        } else {
          if ( (data[14].fvalue < 0.14131901)) {
            if ( (data[6].fvalue < 0.16031316)) {
              sum += (float)0.025115231;
            } else {
              sum += (float)-0.047949411;
            }
          } else {
            if (  (data[4].fvalue < 1.2851281)) {
              sum += (float)0.22545624;
            } else {
              sum += (float)0.073930576;
            }
          }
        }
      } else {
        if ( (data[0].fvalue < 2.4702194)) {
          sum += (float)0.028754799;
        } else {
          if ( (data[3].fvalue < 0.14712261)) {
            if (  (data[4].fvalue < 1.822583)) {
              sum += (float)-0.04106769;
            } else {
              sum += (float)-0.10512855;
            }
          } else {
            sum += (float)-0;
          }
        }
      }
    } else {
      if (  (data[4].fvalue < 1.052201)) {
        if ( (data[8].fvalue < 1.5)) {
          if ( (data[6].fvalue < 0.08541172)) {
            sum += (float)-0.076237693;
          } else {
            if ( (data[7].fvalue < 1.5)) {
              sum += (float)0.037953407;
            } else {
              sum += (float)-0.05374277;
            }
          }
        } else {
          if ( (data[3].fvalue < 1.0697415)) {
            if ( (data[14].fvalue < 0.090539336)) {
              sum += (float)-0;
            } else {
              sum += (float)0.20073743;
            }
          } else {
            sum += (float)-0.0084496597;
          }
        }
      } else {
        if ( (data[16].fvalue < 0.00065991399)) {
          if ( (data[14].fvalue < 0.57716078)) {
            if ( (data[13].fvalue < 0.4846026)) {
              sum += (float)-0.047739763;
            } else {
              sum += (float)0.016900612;
            }
          } else {
            if ( (data[8].fvalue < 2.5)) {
              sum += (float)-0.013700084;
            } else {
              sum += (float)0.057867121;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.33563945)) {
            if ( (data[14].fvalue < 0.24578762)) {
              sum += (float)0.018765688;
            } else {
              sum += (float)0.081123978;
            }
          } else {
            if ( (data[15].fvalue < 0.11472067)) {
              sum += (float)-0.0099959886;
            } else {
              sum += (float)0.030696403;
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
