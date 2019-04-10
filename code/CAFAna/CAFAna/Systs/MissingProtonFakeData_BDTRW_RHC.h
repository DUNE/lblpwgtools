#include "BDTReweighter.h"
class MissingProtonFakeData_BDTRW_RHC : public BDTReweighter {
 public :
 MissingProtonFakeData_BDTRW_RHC() : BDTReweighter(1.0064907) {;}
  ~MissingProtonFakeData_BDTRW_RHC() {;}

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
  if ( (data[6].fvalue < 0.45269591)) {
    if ( (data[6].fvalue < 0.16682376)) {
      if ( (data[6].fvalue < 0.00011694336)) {
        if ( (data[7].fvalue < 0.30885273)) {
          if ( (data[5].fvalue < 2.0038989)) {
            if ( (data[5].fvalue < 1.3885491)) {
              sum += (float)0.15175185;
            } else {
              sum += (float)0.15987001;
            }
          } else {
            if ( (data[5].fvalue < 9.1452484)) {
              sum += (float)0.14793456;
            } else {
              sum += (float)0.15908314;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.4784958)) {
            if ( (data[7].fvalue < 0.61300755)) {
              sum += (float)0.13599153;
            } else {
              sum += (float)0.10891013;
            }
          } else {
            if ( (data[7].fvalue < 0.70348138)) {
              sum += (float)0.14335038;
            } else {
              sum += (float)0.12728065;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.035989441)) {
          if ( (data[6].fvalue < 0.018289184)) {
            if ( (data[5].fvalue < 2.7520628)) {
              sum += (float)0.07132411;
            } else {
              sum += (float)0.086108357;
            }
          } else {
            if ( (data[7].fvalue < 0.10573661)) {
              sum += (float)0.10551964;
            } else {
              sum += (float)0.094913177;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.080001622)) {
            if ( (data[7].fvalue < 0.10115251)) {
              sum += (float)0.12976412;
            } else {
              sum += (float)0.11465821;
            }
          } else {
            if ( (data[7].fvalue < 0.13143726)) {
              sum += (float)0.15410252;
            } else {
              sum += (float)0.131194;
            }
          }
        }
      }
    } else {
      if ( (data[1].fvalue < 0.5)) {
        if ( (data[6].fvalue < 0.33411315)) {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.20701352)) {
              sum += (float)0.18483108;
            } else {
              sum += (float)0.19475742;
            }
          } else {
            if ( (data[7].fvalue < 0.30043644)) {
              sum += (float)0.17221265;
            } else {
              sum += (float)0.15489927;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.8820357)) {
            if ( (data[7].fvalue < 0.28163651)) {
              sum += (float)0.22114484;
            } else {
              sum += (float)0.1983027;
            }
          } else {
            sum += (float)0.18699759;
          }
        }
      } else {
        if ( (data[6].fvalue < 0.3506698)) {
          if ( (data[7].fvalue < 0.29464805)) {
            if ( (data[5].fvalue < 4.6272931)) {
              sum += (float)0.17208168;
            } else {
              sum += (float)0.15079024;
            }
          } else {
            if ( (data[5].fvalue < 1.7574575)) {
              sum += (float)0.16342141;
            } else {
              sum += (float)0.13336521;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.1362457)) {
            if ( (data[7].fvalue < 0.29781091)) {
              sum += (float)0.19789101;
            } else {
              sum += (float)0.17416099;
            }
          } else {
            sum += (float)0.15280339;
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.92937684)) {
      if ( (data[5].fvalue < 3.9087193)) {
        if ( (data[6].fvalue < 0.57406211)) {
          if ( (data[7].fvalue < 0.30004179)) {
            sum += (float)0.26490891;
          } else {
            if ( (data[6].fvalue < 0.57310236)) {
              sum += (float)0.24384823;
            } else {
              sum += (float)0.10699137;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.8688946)) {
            if ( (data[5].fvalue < 1.2488282)) {
              sum += (float)0.061976142;
            } else {
              sum += (float)0.29598075;
            }
          } else {
            if ( (data[6].fvalue < 0.66481888)) {
              sum += (float)0.30588889;
            } else {
              sum += (float)0.34052691;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.62583101)) {
          if ( (data[5].fvalue < 7.1203237)) {
            sum += (float)0.22143644;
          } else {
            if ( (data[6].fvalue < 0.55567515)) {
              sum += (float)0.18505763;
            } else {
              sum += (float)0.21236174;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.4657793)) {
            if ( (data[7].fvalue < 0.385961)) {
              sum += (float)0.28523821;
            } else {
              sum += (float)0.24591346;
            }
          } else {
            sum += (float)0.23212808;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 8.1007137)) {
        if ( (data[6].fvalue < 1.0601412)) {
          sum += (float)0.35058108;
        } else {
          sum += (float)0.41936255;
        }
      } else {
        if ( (data[6].fvalue < 1.1615186)) {
          sum += (float)0.27203685;
        } else {
          if ( (data[7].fvalue < 0.080448546)) {
            sum += (float)0.094356395;
          } else {
            sum += (float)0.35918128;
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.43421352)) {
    if ( (data[6].fvalue < 0.16251728)) {
      if ( (data[6].fvalue < 0.00043951417)) {
        if ( (data[7].fvalue < 0.35557044)) {
          if ( (data[5].fvalue < 1.9209991)) {
            if ( (data[7].fvalue < 0.24821115)) {
              sum += (float)0.11218868;
            } else {
              sum += (float)0.10179696;
            }
          } else {
            if ( (data[5].fvalue < 2.5957694)) {
              sum += (float)0.10052413;
            } else {
              sum += (float)0.10548731;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.90427923)) {
            sum += (float)0.079436727;
          } else {
            if ( (data[7].fvalue < 0.68928075)) {
              sum += (float)0.098545857;
            } else {
              sum += (float)0.089096606;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.035989441)) {
          if ( (data[6].fvalue < 0.023115233)) {
            if ( (data[7].fvalue < 0.10978826)) {
              sum += (float)0.061281618;
            } else {
              sum += (float)0.050538082;
            }
          } else {
            if ( (data[7].fvalue < 0.087620631)) {
              sum += (float)0.078024648;
            } else {
              sum += (float)0.069879442;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.079043433)) {
            if ( (data[7].fvalue < 0.11481707)) {
              sum += (float)0.090001419;
            } else {
              sum += (float)0.079644702;
            }
          } else {
            if ( (data[7].fvalue < 0.10775109)) {
              sum += (float)0.11025768;
            } else {
              sum += (float)0.092200965;
            }
          }
        }
      }
    } else {
      if ( (data[1].fvalue < 0.5)) {
        if ( (data[6].fvalue < 0.2832219)) {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[5].fvalue < 2.7324791)) {
              sum += (float)0.12623709;
            } else {
              sum += (float)0.13578327;
            }
          } else {
            if ( (data[7].fvalue < 0.27446121)) {
              sum += (float)0.11704215;
            } else {
              sum += (float)0.10590285;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.9973936)) {
            if ( (data[7].fvalue < 0.31048623)) {
              sum += (float)0.14368142;
            } else {
              sum += (float)0.12839612;
            }
          } else {
            if ( (data[7].fvalue < 0.14468831)) {
              sum += (float)0.11949033;
            } else {
              sum += (float)0.094946727;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.31048608)) {
          if ( (data[5].fvalue < 4.335485)) {
            if ( (data[6].fvalue < 0.30537444)) {
              sum += (float)0.11656415;
            } else {
              sum += (float)0.140081;
            }
          } else {
            if ( (data[6].fvalue < 0.28766605)) {
              sum += (float)0.10330527;
            } else {
              sum += (float)0.11346316;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.26931682)) {
            if ( (data[5].fvalue < 2.5622749)) {
              sum += (float)0.10077615;
            } else {
              sum += (float)0.084306672;
            }
          } else {
            if ( (data[5].fvalue < 3.9239178)) {
              sum += (float)0.11639775;
            } else {
              sum += (float)0.09152095;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.69954109)) {
      if ( (data[5].fvalue < 3.7900877)) {
        if ( (data[6].fvalue < 0.54229653)) {
          if ( (data[7].fvalue < 0.33413237)) {
            if ( (data[6].fvalue < 0.47438198)) {
              sum += (float)0.16574858;
            } else {
              sum += (float)0.1882937;
            }
          } else {
            if ( (data[5].fvalue < 2.0660951)) {
              sum += (float)0.17387089;
            } else {
              sum += (float)0.1508507;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.30299485)) {
            sum += (float)0.22958292;
          } else {
            if ( (data[5].fvalue < 1.2093853)) {
              sum += (float)0.080001526;
            } else {
              sum += (float)0.20048477;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.56214035)) {
          if ( (data[5].fvalue < 7.0939255)) {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.15579869;
            } else {
              sum += (float)0.13495535;
            }
          } else {
            sum += (float)0.1294758;
          }
        } else {
          if ( (data[5].fvalue < 7.2554312)) {
            sum += (float)0.17642036;
          } else {
            if ( (data[6].fvalue < 0.69630623)) {
              sum += (float)0.15008484;
            } else {
              sum += (float)0.065569639;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.0603541)) {
        if ( (data[5].fvalue < 6.8184986)) {
          if ( (data[7].fvalue < 0.46789396)) {
            if ( (data[5].fvalue < 4.0032253)) {
              sum += (float)0.25062954;
            } else {
              sum += (float)0.22037013;
            }
          } else {
            if ( (data[5].fvalue < 4.3194256)) {
              sum += (float)0.21652149;
            } else {
              sum += (float)0.15279986;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.86113209)) {
            sum += (float)0.16464595;
          } else {
            if ( (data[5].fvalue < 32.749332)) {
              sum += (float)0.19087195;
            } else {
              sum += (float)0.044717461;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.2059898)) {
          sum += (float)0.31237191;
        } else {
          if ( (data[6].fvalue < 1.4172075)) {
            if ( (data[7].fvalue < 0.38843712)) {
              sum += (float)0.25114208;
            } else {
              sum += (float)0.19473448;
            }
          } else {
            if ( (data[7].fvalue < 0.080448546)) {
              sum += (float)0.080202937;
            } else {
              sum += (float)0.2781294;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.41110086)) {
    if ( (data[6].fvalue < 0.16825406)) {
      if ( (data[6].fvalue < 0.0005318909)) {
        if ( (data[7].fvalue < 0.29601482)) {
          if ( (data[5].fvalue < 2.0236998)) {
            if ( (data[5].fvalue < 0.60062164)) {
              sum += (float)0.055573631;
            } else {
              sum += (float)0.077888653;
            }
          } else {
            if ( (data[5].fvalue < 2.5432384)) {
              sum += (float)0.069053799;
            } else {
              sum += (float)0.074033625;
            }
          }
        } else {
          if ( (data[5].fvalue < 4.3998775)) {
            if ( (data[7].fvalue < 0.83631027)) {
              sum += (float)0.068144731;
            } else {
              sum += (float)0.042082943;
            }
          } else {
            sum += (float)0.075053558;
          }
        }
      } else {
        if ( (data[6].fvalue < 0.035989441)) {
          if ( (data[6].fvalue < 0.017962128)) {
            if ( (data[5].fvalue < 3.0942428)) {
              sum += (float)0.033275463;
            } else {
              sum += (float)0.044446971;
            }
          } else {
            if ( (data[5].fvalue < 1.9090483)) {
              sum += (float)0.057547715;
            } else {
              sum += (float)0.047876298;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.083906218)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.057148322;
            } else {
              sum += (float)0.06615372;
            }
          } else {
            if ( (data[7].fvalue < 0.15206927)) {
              sum += (float)0.076571301;
            } else {
              sum += (float)0.062827282;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.30990469)) {
        if ( (data[5].fvalue < 4.0543022)) {
          if ( (data[6].fvalue < 0.3113426)) {
            if ( (data[7].fvalue < 0.1445843)) {
              sum += (float)0.098409273;
            } else {
              sum += (float)0.085917965;
            }
          } else {
            if ( (data[6].fvalue < 0.35943156)) {
              sum += (float)0.098382026;
            } else {
              sum += (float)0.1085147;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.091474801)) {
            if ( (data[5].fvalue < 19.773733)) {
              sum += (float)0.089756072;
            } else {
              sum += (float)0.06847582;
            }
          } else {
            if ( (data[5].fvalue < 6.8173103)) {
              sum += (float)0.078774497;
            } else {
              sum += (float)0.067279972;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.829656)) {
          if ( (data[7].fvalue < 0.67927843)) {
            if ( (data[6].fvalue < 0.35093868)) {
              sum += (float)0.09391211;
            } else {
              sum += (float)0.11643351;
            }
          } else {
            if ( (data[7].fvalue < 0.7305721)) {
              sum += (float)0.061973661;
            } else {
              sum += (float)0.026107425;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.31694067)) {
            if ( (data[7].fvalue < 0.36606699)) {
              sum += (float)0.07034938;
            } else {
              sum += (float)0.060560912;
            }
          } else {
            if ( (data[7].fvalue < 0.41200691)) {
              sum += (float)0.083159313;
            } else {
              sum += (float)0.069367178;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.83659899)) {
      if ( (data[6].fvalue < 0.52616686)) {
        if ( (data[5].fvalue < 3.7287583)) {
          if ( (data[7].fvalue < 0.81121814)) {
            if ( (data[7].fvalue < 0.33374047)) {
              sum += (float)0.12439779;
            } else {
              sum += (float)0.11038852;
            }
          } else {
            if ( (data[7].fvalue < 0.8532905)) {
              sum += (float)0.01245778;
            } else {
              sum += (float)0.055190392;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.30356082)) {
            if ( (data[5].fvalue < 7.3264751)) {
              sum += (float)0.10344137;
            } else {
              sum += (float)0.08966624;
            }
          } else {
            if ( (data[6].fvalue < 0.50041449)) {
              sum += (float)0.073909104;
            } else {
              sum += (float)0.10574472;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 3.8500404)) {
          if ( (data[6].fvalue < 0.60697246)) {
            if ( (data[7].fvalue < 0.43864048)) {
              sum += (float)0.14171277;
            } else {
              sum += (float)0.11917281;
            }
          } else {
            if ( (data[7].fvalue < 0.32267374)) {
              sum += (float)0.17754604;
            } else {
              sum += (float)0.14937793;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.5067434)) {
            if ( (data[6].fvalue < 0.64608407)) {
              sum += (float)0.11429555;
            } else {
              sum += (float)0.13668375;
            }
          } else {
            if ( (data[5].fvalue < 45.631248)) {
              sum += (float)0.10690173;
            } else {
              sum += (float)0.024765968;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.9948833)) {
        if ( (data[5].fvalue < 6.5106206)) {
          if ( (data[7].fvalue < 0.71649104)) {
            if ( (data[6].fvalue < 0.98804522)) {
              sum += (float)0.17011623;
            } else {
              sum += (float)0.098020405;
            }
          } else {
            if ( (data[6].fvalue < 0.86861831)) {
              sum += (float)0.023760028;
            } else {
              sum += (float)0.12187437;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.6188745)) {
            sum += (float)-0.029167378;
          } else {
            sum += (float)0.13413578;
          }
        }
      } else {
        if ( (data[5].fvalue < 6.3471036)) {
          if ( (data[7].fvalue < 0.70040435)) {
            sum += (float)0.21709734;
          } else {
            if ( (data[5].fvalue < 2.3997786)) {
              sum += (float)0.21827757;
            } else {
              sum += (float)0.14267063;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.1189141)) {
            if ( (data[5].fvalue < 21.693436)) {
              sum += (float)0.13097732;
            } else {
              sum += (float)0.069766499;
            }
          } else {
            if ( (data[6].fvalue < 1.798722)) {
              sum += (float)0.16620144;
            } else {
              sum += (float)0.19821422;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.46849304)) {
    if ( (data[6].fvalue < 0.19003601)) {
      if ( (data[6].fvalue < 0.0007984009)) {
        if ( (data[5].fvalue < 9.1452484)) {
          if ( (data[5].fvalue < 3.7446671)) {
            if ( (data[7].fvalue < 0.11639829)) {
              sum += (float)0.053423587;
            } else {
              sum += (float)0.049778879;
            }
          } else {
            if ( (data[7].fvalue < 0.18351004)) {
              sum += (float)0.046376754;
            } else {
              sum += (float)0.051236313;
            }
          }
        } else {
          if ( (data[5].fvalue < 20.223152)) {
            if ( (data[7].fvalue < 0.062237933)) {
              sum += (float)0.063101098;
            } else {
              sum += (float)0.05457329;
            }
          } else {
            if ( (data[7].fvalue < 0.17975828)) {
              sum += (float)0.045185879;
            } else {
              sum += (float)0.064116023;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.040186372)) {
          if ( (data[6].fvalue < 0.01528775)) {
            if ( (data[6].fvalue < 0.0034661256)) {
              sum += (float)0.036390692;
            } else {
              sum += (float)0.02266695;
            }
          } else {
            if ( (data[7].fvalue < 0.31760389)) {
              sum += (float)0.035741847;
            } else {
              sum += (float)0.024158999;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.28917989)) {
            if ( (data[6].fvalue < 0.083906218)) {
              sum += (float)0.04326668;
            } else {
              sum += (float)0.051556468;
            }
          } else {
            if ( (data[5].fvalue < 1.7757614)) {
              sum += (float)0.046240322;
            } else {
              sum += (float)0.03262803;
            }
          }
        }
      }
    } else {
      if ( (data[1].fvalue < 0.5)) {
        if ( (data[6].fvalue < 0.3756659)) {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[5].fvalue < 1.9978195)) {
              sum += (float)0.080432847;
            } else {
              sum += (float)0.066905558;
            }
          } else {
            if ( (data[5].fvalue < 2.2278633)) {
              sum += (float)0.067600869;
            } else {
              sum += (float)0.057271361;
            }
          }
        } else {
          if ( (data[4].fvalue < 0.5)) {
            if ( (data[5].fvalue < 7.3065701)) {
              sum += (float)0.075173236;
            } else {
              sum += (float)0.055701252;
            }
          } else {
            if ( (data[5].fvalue < 1.2390904)) {
              sum += (float)0.0043712854;
            } else {
              sum += (float)0.091940194;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.26517868)) {
          if ( (data[5].fvalue < 3.7335281)) {
            if ( (data[6].fvalue < 0.29325765)) {
              sum += (float)0.060766704;
            } else {
              sum += (float)0.076834448;
            }
          } else {
            if ( (data[6].fvalue < 0.31060508)) {
              sum += (float)0.052345324;
            } else {
              sum += (float)0.059835695;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.5533514)) {
            if ( (data[6].fvalue < 0.34589401)) {
              sum += (float)0.050077945;
            } else {
              sum += (float)0.062987126;
            }
          } else {
            if ( (data[6].fvalue < 0.40289414)) {
              sum += (float)0.040567167;
            } else {
              sum += (float)0.053596433;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.95209062)) {
      if ( (data[5].fvalue < 3.9982815)) {
        if ( (data[6].fvalue < 0.60704535)) {
          if ( (data[6].fvalue < 0.60677469)) {
            if ( (data[5].fvalue < 1.1453488)) {
              sum += (float)0.030381832;
            } else {
              sum += (float)0.091294788;
            }
          } else {
            sum += (float)-0.00065442501;
          }
        } else {
          if ( (data[7].fvalue < 0.70114577)) {
            if ( (data[7].fvalue < 0.32267374)) {
              sum += (float)0.12372702;
            } else {
              sum += (float)0.10840233;
            }
          } else {
            if ( (data[6].fvalue < 0.92357606)) {
              sum += (float)0.084716573;
            } else {
              sum += (float)0.011339379;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.64526546)) {
          if ( (data[6].fvalue < 0.64409745)) {
            if ( (data[6].fvalue < 0.50031459)) {
              sum += (float)0.064371154;
            } else {
              sum += (float)0.07604558;
            }
          } else {
            if ( (data[7].fvalue < 0.36391577)) {
              sum += (float)0.030988073;
            } else {
              sum += (float)-0.0099563068;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.30866838)) {
            if ( (data[7].fvalue < 0.19477767)) {
              sum += (float)0.08335223;
            } else {
              sum += (float)0.1060923;
            }
          } else {
            if ( (data[5].fvalue < 11.644032)) {
              sum += (float)0.078829423;
            } else {
              sum += (float)0.047581587;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 8.184432)) {
        if ( (data[6].fvalue < 1.3734715)) {
          if ( (data[6].fvalue < 1.3494527)) {
            if ( (data[7].fvalue < 0.4552131)) {
              sum += (float)0.14912705;
            } else {
              sum += (float)0.12419375;
            }
          } else {
            if ( (data[5].fvalue < 4.6351295)) {
              sum += (float)0.079351448;
            } else {
              sum += (float)0.02681797;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.27957565)) {
            sum += (float)0.0010000528;
          } else {
            if ( (data[7].fvalue < 0.85598028)) {
              sum += (float)0.17334175;
            } else {
              sum += (float)0.06771113;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 1.4169521)) {
          if ( (data[5].fvalue < 10.38205)) {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.10335145;
            } else {
              sum += (float)0.052349024;
            }
          } else {
            if ( (data[5].fvalue < 14.567866)) {
              sum += (float)0.12465961;
            } else {
              sum += (float)0.084689453;
            }
          }
        } else {
          sum += (float)0.13010313;
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.47180402)) {
    if ( (data[6].fvalue < 0.16171929)) {
      if ( (data[6].fvalue < 0.00061935425)) {
        if ( (data[7].fvalue < 0.3632361)) {
          if ( (data[5].fvalue < 2.0992043)) {
            if ( (data[7].fvalue < 0.050200783)) {
              sum += (float)0.034164838;
            } else {
              sum += (float)0.03939056;
            }
          } else {
            if ( (data[5].fvalue < 2.5432384)) {
              sum += (float)0.032382473;
            } else {
              sum += (float)0.036398076;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.5249684)) {
            if ( (data[7].fvalue < 0.49741268)) {
              sum += (float)0.032258503;
            } else {
              sum += (float)0.02009479;
            }
          } else {
            if ( (data[7].fvalue < 0.74528229)) {
              sum += (float)0.034451827;
            } else {
              sum += (float)0.026489118;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.023602143)) {
          if ( (data[7].fvalue < 0.34258771)) {
            if ( (data[5].fvalue < 2.9678063)) {
              sum += (float)0.016879499;
            } else {
              sum += (float)0.023549266;
            }
          } else {
            if ( (data[6].fvalue < 0.015259064)) {
              sum += (float)0.0014202838;
            } else {
              sum += (float)0.012529382;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.086513102)) {
            if ( (data[6].fvalue < 0.097593077)) {
              sum += (float)0.032662995;
            } else {
              sum += (float)0.046300858;
            }
          } else {
            if ( (data[5].fvalue < 1.963466)) {
              sum += (float)0.036286347;
            } else {
              sum += (float)0.026487174;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.27152538)) {
        if ( (data[7].fvalue < 0.12717624)) {
          if ( (data[5].fvalue < 3.584507)) {
            if ( (data[5].fvalue < 2.6627116)) {
              sum += (float)0.038425073;
            } else {
              sum += (float)0.061872333;
            }
          } else {
            if ( (data[7].fvalue < 0.063024394)) {
              sum += (float)0.046614733;
            } else {
              sum += (float)0.038574833;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.0858846)) {
            if ( (data[7].fvalue < 0.42809507)) {
              sum += (float)0.053677611;
            } else {
              sum += (float)0.03609065;
            }
          } else {
            if ( (data[7].fvalue < 0.31617129)) {
              sum += (float)0.036927201;
            } else {
              sum += (float)0.027923632;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 7.2611938)) {
          if ( (data[7].fvalue < 0.27003968)) {
            if ( (data[5].fvalue < 3.685904)) {
              sum += (float)0.055488255;
            } else {
              sum += (float)0.047050532;
            }
          } else {
            if ( (data[5].fvalue < 2.4647422)) {
              sum += (float)0.049874902;
            } else {
              sum += (float)0.03884501;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.11622153)) {
            if ( (data[5].fvalue < 19.827187)) {
              sum += (float)0.042463426;
            } else {
              sum += (float)0.02551977;
            }
          } else {
            if ( (data[5].fvalue < 9.2405357)) {
              sum += (float)0.019675205;
            } else {
              sum += (float)0.032780334;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.91804779)) {
      if ( (data[5].fvalue < 4.068718)) {
        if ( (data[6].fvalue < 0.66508192)) {
          if ( (data[6].fvalue < 0.52977145)) {
            if ( (data[7].fvalue < 0.81121814)) {
              sum += (float)0.060224418;
            } else {
              sum += (float)0.0070431642;
            }
          } else {
            if ( (data[5].fvalue < 2.9796879)) {
              sum += (float)0.063019149;
            } else {
              sum += (float)0.075396925;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.41282076)) {
            if ( (data[5].fvalue < 3.6561875)) {
              sum += (float)0.093197644;
            } else {
              sum += (float)0.066147648;
            }
          } else {
            if ( (data[5].fvalue < 2.3010805)) {
              sum += (float)0.078040786;
            } else {
              sum += (float)0.062525593;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.55694246)) {
          if ( (data[6].fvalue < 0.54728508)) {
            if ( (data[7].fvalue < 0.54113662)) {
              sum += (float)0.050057065;
            } else {
              sum += (float)0.024261907;
            }
          } else {
            if ( (data[6].fvalue < 0.54916358)) {
              sum += (float)-0.0045295674;
            } else {
              sum += (float)0.033885248;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.3237868)) {
            if ( (data[5].fvalue < 5.8221931)) {
              sum += (float)0.07514175;
            } else {
              sum += (float)0.057765353;
            }
          } else {
            if ( (data[5].fvalue < 4.6704073)) {
              sum += (float)0.031369515;
            } else {
              sum += (float)0.056705687;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 4.4606552)) {
        if ( (data[7].fvalue < 0.90228617)) {
          if ( (data[6].fvalue < 1.0648844)) {
            if ( (data[6].fvalue < 1.0466213)) {
              sum += (float)0.097439311;
            } else {
              sum += (float)0.04429315;
            }
          } else {
            if ( (data[7].fvalue < 0.57221061)) {
              sum += (float)0.13257894;
            } else {
              sum += (float)0.098848127;
            }
          }
        } else {
          sum += (float)-0.0064054071;
        }
      } else {
        if ( (data[6].fvalue < 1.2015269)) {
          if ( (data[7].fvalue < 0.32980031)) {
            if ( (data[7].fvalue < 0.19777063)) {
              sum += (float)0.056164507;
            } else {
              sum += (float)0.099955097;
            }
          } else {
            if ( (data[5].fvalue < 4.6158662)) {
              sum += (float)-0.0084694903;
            } else {
              sum += (float)0.042349145;
            }
          }
        } else {
          if ( (data[5].fvalue < 7.395381)) {
            if ( (data[7].fvalue < 0.39427337)) {
              sum += (float)0.13232642;
            } else {
              sum += (float)0.074997887;
            }
          } else {
            if ( (data[5].fvalue < 11.036093)) {
              sum += (float)0.073555298;
            } else {
              sum += (float)0.095568679;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.38545972)) {
    if ( (data[6].fvalue < 0.16171929)) {
      if ( (data[6].fvalue < 0.0011330261)) {
        if ( (data[5].fvalue < 8.6930475)) {
          if ( (data[5].fvalue < 3.7135873)) {
            if ( (data[7].fvalue < 0.1312722)) {
              sum += (float)0.026654107;
            } else {
              sum += (float)0.024070583;
            }
          } else {
            if ( (data[7].fvalue < 0.18766028)) {
              sum += (float)0.021409608;
            } else {
              sum += (float)0.025491433;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.089837663)) {
            if ( (data[5].fvalue < 18.954718)) {
              sum += (float)0.033585981;
            } else {
              sum += (float)0.021750094;
            }
          } else {
            if ( (data[3].fvalue < 0.5)) {
              sum += (float)0.024781987;
            } else {
              sum += (float)0.050493646;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.054473907)) {
          if ( (data[7].fvalue < 0.39191428)) {
            if ( (data[5].fvalue < 1.8679861)) {
              sum += (float)0.022321846;
            } else {
              sum += (float)0.014986993;
            }
          } else {
            if ( (data[7].fvalue < 0.68224084)) {
              sum += (float)0.0061437944;
            } else {
              sum += (float)-0.010162241;
            }
          }
        } else {
          if ( (data[4].fvalue < 0.5)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.018605215;
            } else {
              sum += (float)0.029655669;
            }
          } else {
            if ( (data[5].fvalue < 2.602354)) {
              sum += (float)0.02372797;
            } else {
              sum += (float)0.03379935;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.27436441)) {
        if ( (data[5].fvalue < 3.9367933)) {
          if ( (data[5].fvalue < 3.0965953)) {
            if ( (data[5].fvalue < 2.0855277)) {
              sum += (float)0.043015078;
            } else {
              sum += (float)0.028171791;
            }
          } else {
            if ( (data[7].fvalue < 0.1891156)) {
              sum += (float)0.041612793;
            } else {
              sum += (float)0.031476796;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.12701319)) {
            if ( (data[5].fvalue < 19.690155)) {
              sum += (float)0.030158931;
            } else {
              sum += (float)0.020071663;
            }
          } else {
            if ( (data[6].fvalue < 0.27207804)) {
              sum += (float)0.018750284;
            } else {
              sum += (float)0.02632286;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.4586649)) {
          if ( (data[7].fvalue < 0.76988435)) {
            if ( (data[7].fvalue < 0.39462119)) {
              sum += (float)0.036703069;
            } else {
              sum += (float)0.027213812;
            }
          } else {
            if ( (data[5].fvalue < 1.9544141)) {
              sum += (float)-0.017663104;
            } else {
              sum += (float)-0.0027834708;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.36406338)) {
            if ( (data[6].fvalue < 0.24893916)) {
              sum += (float)0.020481467;
            } else {
              sum += (float)0.025593709;
            }
          } else {
            if ( (data[5].fvalue < 9.0434341)) {
              sum += (float)0.018401086;
            } else {
              sum += (float)0.0066580833;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.74375463)) {
      if ( (data[5].fvalue < 3.7288117)) {
        if ( (data[5].fvalue < 3.542901)) {
          if ( (data[5].fvalue < 2.0416458)) {
            if ( (data[5].fvalue < 1.2402856)) {
              sum += (float)0.020230129;
            } else {
              sum += (float)0.055240232;
            }
          } else {
            if ( (data[7].fvalue < 0.3343707)) {
              sum += (float)0.04718798;
            } else {
              sum += (float)0.033769287;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.49556839)) {
            if ( (data[7].fvalue < 0.30979383)) {
              sum += (float)0.055605941;
            } else {
              sum += (float)0.025546202;
            }
          } else {
            if ( (data[7].fvalue < 0.15421957)) {
              sum += (float)0.006162751;
            } else {
              sum += (float)0.077239096;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 7.3264751)) {
          if ( (data[5].fvalue < 4.4985409)) {
            if ( (data[7].fvalue < 0.68705869)) {
              sum += (float)0.032229099;
            } else {
              sum += (float)0.090801433;
            }
          } else {
            if ( (data[6].fvalue < 0.56510711)) {
              sum += (float)0.038136337;
            } else {
              sum += (float)0.055030741;
            }
          }
        } else {
          if ( (data[5].fvalue < 10.46546)) {
            if ( (data[7].fvalue < 0.069537625)) {
              sum += (float)0.045299329;
            } else {
              sum += (float)0.0194536;
            }
          } else {
            if ( (data[7].fvalue < 0.396653)) {
              sum += (float)0.036836617;
            } else {
              sum += (float)0.0048381034;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.0802714)) {
        if ( (data[5].fvalue < 4.1137362)) {
          if ( (data[7].fvalue < 0.35027713)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.081661239;
            } else {
              sum += (float)0.029842576;
            }
          } else {
            if ( (data[6].fvalue < 1.0707085)) {
              sum += (float)0.057446975;
            } else {
              sum += (float)-0.012794066;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.39020365)) {
            if ( (data[5].fvalue < 6.8184986)) {
              sum += (float)0.05912631;
            } else {
              sum += (float)0.04416769;
            }
          } else {
            if ( (data[7].fvalue < 0.50063163)) {
              sum += (float)0.01515434;
            } else {
              sum += (float)0.052590329;
            }
          }
        }
      } else {
        if ( (data[1].fvalue < 0.5)) {
          if ( (data[7].fvalue < 0.78382134)) {
            if ( (data[5].fvalue < 10.147168)) {
              sum += (float)0.11040707;
            } else {
              sum += (float)0.046577424;
            }
          } else {
            sum += (float)-0.025939161;
          }
        } else {
          if ( (data[5].fvalue < 2.2995262)) {
            sum += (float)0.12521498;
          } else {
            if ( (data[5].fvalue < 3.0081851)) {
              sum += (float)0.014343128;
            } else {
              sum += (float)0.064192861;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.49214569)) {
    if ( (data[6].fvalue < 0.21497706)) {
      if ( (data[6].fvalue < 0.0011330261)) {
        if ( (data[2].fvalue < 0.5)) {
          if ( (data[1].fvalue < 0.5)) {
            if ( (data[5].fvalue < 5.9776053)) {
              sum += (float)0.018199202;
            } else {
              sum += (float)0.023473561;
            }
          } else {
            if ( (data[5].fvalue < 11.556337)) {
              sum += (float)0.014681578;
            } else {
              sum += (float)0.020847179;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.0306915)) {
            if ( (data[7].fvalue < 0.44188806)) {
              sum += (float)0.013184495;
            } else {
              sum += (float)-0.00033851093;
            }
          } else {
            if ( (data[5].fvalue < 2.2976246)) {
              sum += (float)0.01893536;
            } else {
              sum += (float)0.015572645;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.02623117)) {
          if ( (data[7].fvalue < 0.085650995)) {
            if ( (data[5].fvalue < 2.4423435)) {
              sum += (float)0.0054438752;
            } else {
              sum += (float)0.014951021;
            }
          } else {
            if ( (data[6].fvalue < 0.0033538206)) {
              sum += (float)0.012978671;
            } else {
              sum += (float)0.0055416785;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.3505891)) {
            if ( (data[5].fvalue < 1.917073)) {
              sum += (float)0.02469817;
            } else {
              sum += (float)0.015540368;
            }
          } else {
            if ( (data[4].fvalue < 0.5)) {
              sum += (float)0.007841385;
            } else {
              sum += (float)0.025708066;
            }
          }
        }
      }
    } else {
      if ( (data[1].fvalue < 0.5)) {
        if ( (data[6].fvalue < 0.35602885)) {
          if ( (data[7].fvalue < 0.32885814)) {
            if ( (data[5].fvalue < 3.8264649)) {
              sum += (float)0.025920659;
            } else {
              sum += (float)0.018361416;
            }
          } else {
            if ( (data[2].fvalue < 0.5)) {
              sum += (float)0.031189522;
            } else {
              sum += (float)0.015097773;
            }
          }
        } else {
          if ( (data[4].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.2271592)) {
              sum += (float)0.029343978;
            } else {
              sum += (float)0.023810178;
            }
          } else {
            if ( (data[7].fvalue < 0.23285827)) {
              sum += (float)0.027326202;
            } else {
              sum += (float)0.046342835;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.22421387)) {
          if ( (data[5].fvalue < 6.4611292)) {
            if ( (data[5].fvalue < 3.1573462)) {
              sum += (float)0.018991681;
            } else {
              sum += (float)0.027530955;
            }
          } else {
            if ( (data[5].fvalue < 9.6317596)) {
              sum += (float)0.01119217;
            } else {
              sum += (float)0.021121027;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.1984725)) {
            if ( (data[7].fvalue < 0.83185208)) {
              sum += (float)0.01850082;
            } else {
              sum += (float)-0.0078130169;
            }
          } else {
            if ( (data[5].fvalue < 13.92857)) {
              sum += (float)0.0080013638;
            } else {
              sum += (float)0.018204916;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.95209062)) {
      if ( (data[7].fvalue < 0.19747257)) {
        if ( (data[5].fvalue < 20.010904)) {
          if ( (data[6].fvalue < 0.58883643)) {
            if ( (data[7].fvalue < 0.1952129)) {
              sum += (float)0.021829749;
            } else {
              sum += (float)-0.03264986;
            }
          } else {
            if ( (data[5].fvalue < 5.8040943)) {
              sum += (float)0.048604816;
            } else {
              sum += (float)0.027045634;
            }
          }
        } else {
          if ( (data[5].fvalue < 24.617977)) {
            if ( (data[5].fvalue < 21.916618)) {
              sum += (float)0.016215287;
            } else {
              sum += (float)-0.012314375;
            }
          } else {
            if ( (data[5].fvalue < 41.308289)) {
              sum += (float)0.025117952;
            } else {
              sum += (float)0.0012121038;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.30031186)) {
          if ( (data[6].fvalue < 0.66451418)) {
            if ( (data[5].fvalue < 3.1489699)) {
              sum += (float)0.048994247;
            } else {
              sum += (float)0.03116001;
            }
          } else {
            if ( (data[5].fvalue < 3.7562633)) {
              sum += (float)0.060749762;
            } else {
              sum += (float)0.041937027;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.0379143)) {
            if ( (data[7].fvalue < 0.69528532)) {
              sum += (float)0.048346769;
            } else {
              sum += (float)0.01919909;
            }
          } else {
            if ( (data[5].fvalue < 2.7665677)) {
              sum += (float)0.02269238;
            } else {
              sum += (float)0.032969475;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 4.2168088)) {
        if ( (data[5].fvalue < 3.6056323)) {
          if ( (data[5].fvalue < 3.5588894)) {
            if ( (data[7].fvalue < 0.4500573)) {
              sum += (float)0.073720813;
            } else {
              sum += (float)0.048829183;
            }
          } else {
            if ( (data[6].fvalue < 1.1531014)) {
              sum += (float)-0.030710161;
            } else {
              sum += (float)0.013009788;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.43728748)) {
            if ( (data[7].fvalue < 0.43447572)) {
              sum += (float)0.063626967;
            } else {
              sum += (float)-0.013557994;
            }
          } else {
            if ( (data[6].fvalue < 0.96028256)) {
              sum += (float)0.0058030132;
            } else {
              sum += (float)0.10045224;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.39085805)) {
          if ( (data[5].fvalue < 6.1683378)) {
            if ( (data[6].fvalue < 1.2011509)) {
              sum += (float)0.048572917;
            } else {
              sum += (float)0.091488317;
            }
          } else {
            if ( (data[5].fvalue < 29.285845)) {
              sum += (float)0.042516921;
            } else {
              sum += (float)-0.0023276377;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.823384)) {
            if ( (data[5].fvalue < 4.2724466)) {
              sum += (float)-0.020965021;
            } else {
              sum += (float)0.017009484;
            }
          } else {
            if ( (data[6].fvalue < 2.5269947)) {
              sum += (float)0.063170202;
            } else {
              sum += (float)0.037281852;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.40551984)) {
    if ( (data[6].fvalue < 0.082985967)) {
      if ( (data[6].fvalue < 0.0030590515)) {
        if ( (data[5].fvalue < 1.3681591)) {
          if ( (data[7].fvalue < 0.50915098)) {
            if ( (data[5].fvalue < 0.6703738)) {
              sum += (float)0.0012800525;
            } else {
              sum += (float)0.011163421;
            }
          } else {
            if ( (data[7].fvalue < 0.61394596)) {
              sum += (float)0.0025917613;
            } else {
              sum += (float)-0.0066078254;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.8956805)) {
            if ( (data[7].fvalue < 0.18886256)) {
              sum += (float)0.017782597;
            } else {
              sum += (float)0.011814537;
            }
          } else {
            if ( (data[5].fvalue < 2.6256614)) {
              sum += (float)0.010619884;
            } else {
              sum += (float)0.012789411;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.8276381)) {
          if ( (data[5].fvalue < 1.9369936)) {
            if ( (data[6].fvalue < 0.015287598)) {
              sum += (float)0.0030456628;
            } else {
              sum += (float)0.01254792;
            }
          } else {
            if ( (data[5].fvalue < 2.5487945)) {
              sum += (float)0.0011587236;
            } else {
              sum += (float)0.0067120702;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.28660268)) {
            if ( (data[5].fvalue < 3.8263822)) {
              sum += (float)0.013443618;
            } else {
              sum += (float)0.0078950198;
            }
          } else {
            if ( (data[5].fvalue < 4.1243219)) {
              sum += (float)-3.491154e-05;
            } else {
              sum += (float)0.0073660184;
            }
          }
        }
      }
    } else {
      if ( (data[4].fvalue < 0.5)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[6].fvalue < 0.19102784)) {
            if ( (data[6].fvalue < 0.10975379)) {
              sum += (float)0.016132196;
            } else {
              sum += (float)0.0061123739;
            }
          } else {
            if ( (data[5].fvalue < 2.4220262)) {
              sum += (float)0.018114392;
            } else {
              sum += (float)0.01246733;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.6108055)) {
            if ( (data[5].fvalue < 1.9424305)) {
              sum += (float)0.023218527;
            } else {
              sum += (float)0.0044582211;
            }
          } else {
            if ( (data[5].fvalue < 3.8605967)) {
              sum += (float)0.026912762;
            } else {
              sum += (float)0.016627429;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.6237826)) {
          if ( (data[5].fvalue < 1.973269)) {
            if ( (data[6].fvalue < 0.10981171)) {
              sum += (float)0.034161672;
            } else {
              sum += (float)0.021670595;
            }
          } else {
            if ( (data[7].fvalue < 0.22211543)) {
              sum += (float)0.0048226784;
            } else {
              sum += (float)0.023765912;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.9371524)) {
            if ( (data[7].fvalue < 0.14851332)) {
              sum += (float)0.028192529;
            } else {
              sum += (float)0.01609616;
            }
          } else {
            if ( (data[6].fvalue < 0.097544588)) {
              sum += (float)0.0012660872;
            } else {
              sum += (float)0.015409485;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.77460194)) {
      if ( (data[5].fvalue < 3.7288117)) {
        if ( (data[5].fvalue < 3.5353246)) {
          if ( (data[7].fvalue < 0.81121814)) {
            if ( (data[6].fvalue < 0.56921971)) {
              sum += (float)0.019843511;
            } else {
              sum += (float)0.026451664;
            }
          } else {
            if ( (data[7].fvalue < 0.83797812)) {
              sum += (float)-0.028978813;
            } else {
              sum += (float)0.0022321411;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.49556839)) {
            if ( (data[6].fvalue < 0.48508924)) {
              sum += (float)0.03357197;
            } else {
              sum += (float)-0.013975206;
            }
          } else {
            if ( (data[6].fvalue < 0.70955825)) {
              sum += (float)0.045609687;
            } else {
              sum += (float)0.025894579;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.67708218)) {
          if ( (data[7].fvalue < 0.33863592)) {
            if ( (data[7].fvalue < 0.19705859)) {
              sum += (float)0.015358126;
            } else {
              sum += (float)0.021474073;
            }
          } else {
            if ( (data[7].fvalue < 0.44571358)) {
              sum += (float)0.0074073137;
            } else {
              sum += (float)0.017454682;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.71230364)) {
            if ( (data[5].fvalue < 12.910159)) {
              sum += (float)0.065726787;
            } else {
              sum += (float)0.013799394;
            }
          } else {
            if ( (data[5].fvalue < 7.0034366)) {
              sum += (float)-0.0006954968;
            } else {
              sum += (float)0.0092055919;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.4543004)) {
        if ( (data[1].fvalue < 0.5)) {
          if ( (data[7].fvalue < 0.76764834)) {
            if ( (data[6].fvalue < 1.0243925)) {
              sum += (float)0.032202434;
            } else {
              sum += (float)0.051029399;
            }
          } else {
            if ( (data[5].fvalue < 2.0448437)) {
              sum += (float)-0.046385519;
            } else {
              sum += (float)-0.01113157;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.8591259)) {
            if ( (data[7].fvalue < 0.47010159)) {
              sum += (float)0.048700284;
            } else {
              sum += (float)0.023815269;
            }
          } else {
            if ( (data[7].fvalue < 0.30341607)) {
              sum += (float)0.024980269;
            } else {
              sum += (float)0.013620578;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.8787394)) {
          if ( (data[7].fvalue < 0.62508488)) {
            if ( (data[6].fvalue < 1.4567196)) {
              sum += (float)0.024858715;
            } else {
              sum += (float)0.09327241;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.082117125;
            } else {
              sum += (float)0.035768401;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.52777135)) {
            if ( (data[6].fvalue < 1.8194745)) {
              sum += (float)0.030592931;
            } else {
              sum += (float)0.045954097;
            }
          } else {
            if ( (data[7].fvalue < 0.62638497)) {
              sum += (float)-0.010533637;
            } else {
              sum += (float)0.033443891;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.35121286)) {
    if ( (data[7].fvalue < 0.36387247)) {
      if ( (data[6].fvalue < 0.097605199)) {
        if ( (data[6].fvalue < 0.0030590515)) {
          if ( (data[5].fvalue < 3.5946317)) {
            if ( (data[5].fvalue < 3.0599413)) {
              sum += (float)0.0085688233;
            } else {
              sum += (float)0.011600411;
            }
          } else {
            if ( (data[5].fvalue < 8.2701254)) {
              sum += (float)0.0065346928;
            } else {
              sum += (float)0.010724169;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.015256347)) {
            if ( (data[5].fvalue < 4.161582)) {
              sum += (float)0.00078596332;
            } else {
              sum += (float)0.009721878;
            }
          } else {
            if ( (data[5].fvalue < 1.8065147)) {
              sum += (float)0.012733942;
            } else {
              sum += (float)0.0062040454;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.10994022)) {
          if ( (data[5].fvalue < 4.5340595)) {
            if ( (data[5].fvalue < 2.8388286)) {
              sum += (float)0.019050319;
            } else {
              sum += (float)0.026105743;
            }
          } else {
            if ( (data[7].fvalue < 0.22878617)) {
              sum += (float)0.0056253755;
            } else {
              sum += (float)0.026216909;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.14037994)) {
            if ( (data[5].fvalue < 2.4838634)) {
              sum += (float)0.00017555167;
            } else {
              sum += (float)0.013272189;
            }
          } else {
            if ( (data[5].fvalue < 2.2505193)) {
              sum += (float)0.013845202;
            } else {
              sum += (float)0.0066812388;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.73218596)) {
        if ( (data[4].fvalue < 0.5)) {
          if ( (data[5].fvalue < 0.83956379)) {
            if ( (data[6].fvalue < 0.0033118285)) {
              sum += (float)-0.003334204;
            } else {
              sum += (float)-0.01495385;
            }
          } else {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.0050670942;
            } else {
              sum += (float)0.017817223;
            }
          }
        } else {
          if ( (data[5].fvalue < 0.96083552)) {
            if ( (data[7].fvalue < 0.44654751)) {
              sum += (float)0.0053858017;
            } else {
              sum += (float)-0.0079615414;
            }
          } else {
            if ( (data[7].fvalue < 0.53167999)) {
              sum += (float)0.019290032;
            } else {
              sum += (float)0.01076305;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.8884887)) {
          if ( (data[6].fvalue < 0.037965942)) {
            if ( (data[5].fvalue < 1.7404845)) {
              sum += (float)-0.013998251;
            } else {
              sum += (float)0.0024205663;
            }
          } else {
            if ( (data[4].fvalue < 0.5)) {
              sum += (float)-0.025236282;
            } else {
              sum += (float)0.0045129866;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.0018348389)) {
            if ( (data[7].fvalue < 0.83218169)) {
              sum += (float)0.0070227389;
            } else {
              sum += (float)-0.0034564331;
            }
          } else {
            if ( (data[6].fvalue < 0.22633679)) {
              sum += (float)-0.0084398547;
            } else {
              sum += (float)0.0038769126;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.66436511)) {
      if ( (data[5].fvalue < 2.0705161)) {
        if ( (data[7].fvalue < 0.66028988)) {
          if ( (data[5].fvalue < 1.6468267)) {
            if ( (data[5].fvalue < 1.1902604)) {
              sum += (float)0.0090963216;
            } else {
              sum += (float)0.032304823;
            }
          } else {
            if ( (data[5].fvalue < 1.648693)) {
              sum += (float)-0.041301034;
            } else {
              sum += (float)0.019580254;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.3580314)) {
            if ( (data[5].fvalue < 1.1987872)) {
              sum += (float)0.014068929;
            } else {
              sum += (float)-0.024570918;
            }
          } else {
            if ( (data[7].fvalue < 0.70071781)) {
              sum += (float)0.028984925;
            } else {
              sum += (float)0.0029767312;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.37220687)) {
          if ( (data[5].fvalue < 3.6859503)) {
            if ( (data[5].fvalue < 3.2459753)) {
              sum += (float)0.013641041;
            } else {
              sum += (float)0.023105808;
            }
          } else {
            if ( (data[6].fvalue < 0.46561855)) {
              sum += (float)0.0090805218;
            } else {
              sum += (float)0.013295577;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.37889588)) {
            if ( (data[5].fvalue < 3.9290295)) {
              sum += (float)-0.0086773057;
            } else {
              sum += (float)-0.0356149;
            }
          } else {
            if ( (data[6].fvalue < 0.63297737)) {
              sum += (float)0.007135245;
            } else {
              sum += (float)0.026051564;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.4543004)) {
        if ( (data[5].fvalue < 8.2221756)) {
          if ( (data[7].fvalue < 0.30219519)) {
            if ( (data[6].fvalue < 1.0746975)) {
              sum += (float)0.025059614;
            } else {
              sum += (float)0.061440431;
            }
          } else {
            if ( (data[5].fvalue < 4.4690709)) {
              sum += (float)0.020612735;
            } else {
              sum += (float)0.0055178781;
            }
          }
        } else {
          if ( (data[5].fvalue < 8.7937222)) {
            if ( (data[6].fvalue < 0.71736407)) {
              sum += (float)0.03889842;
            } else {
              sum += (float)-0.025901848;
            }
          } else {
            if ( (data[5].fvalue < 28.535995)) {
              sum += (float)0.016707156;
            } else {
              sum += (float)-0.012170059;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 5.8753843)) {
          if ( (data[6].fvalue < 1.4826105)) {
            if ( (data[6].fvalue < 1.4567196)) {
              sum += (float)0.021129906;
            } else {
              sum += (float)0.07800442;
            }
          } else {
            if ( (data[6].fvalue < 1.4969869)) {
              sum += (float)-0.0094478317;
            } else {
              sum += (float)0.040438041;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.8228552)) {
            if ( (data[7].fvalue < 0.24012375)) {
              sum += (float)0.032080062;
            } else {
              sum += (float)0.0031085049;
            }
          } else {
            if ( (data[7].fvalue < 0.10291445)) {
              sum += (float)-0.010075129;
            } else {
              sum += (float)0.031688049;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.24887466)) {
    if ( (data[2].fvalue < 0.5)) {
      if ( (data[1].fvalue < 0.5)) {
        if ( (data[5].fvalue < 2.5593712)) {
          if ( (data[5].fvalue < 1.9361897)) {
            if ( (data[5].fvalue < 0.69470233)) {
              sum += (float)-0.0019957426;
            } else {
              sum += (float)0.0089364769;
            }
          } else {
            if ( (data[7].fvalue < 0.17672615)) {
              sum += (float)0.00049874192;
            } else {
              sum += (float)0.01076122;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.6487889)) {
            if ( (data[6].fvalue < 0.028592132)) {
              sum += (float)0.0084276088;
            } else {
              sum += (float)0.01451223;
            }
          } else {
            if ( (data[5].fvalue < 7.594254)) {
              sum += (float)0.0045883027;
            } else {
              sum += (float)0.010302829;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.00094171148)) {
          if ( (data[5].fvalue < 1.4364591)) {
            if ( (data[7].fvalue < 0.63728106)) {
              sum += (float)-0.0016442232;
            } else {
              sum += (float)-0.013985745;
            }
          } else {
            if ( (data[7].fvalue < 0.15267524)) {
              sum += (float)0.0029075905;
            } else {
              sum += (float)0.0057244468;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.3379097)) {
            if ( (data[6].fvalue < 0.17974024)) {
              sum += (float)0.0027848347;
            } else {
              sum += (float)0.0065438366;
            }
          } else {
            if ( (data[6].fvalue < 0.164379)) {
              sum += (float)-0.0027302385;
            } else {
              sum += (float)0.0035487486;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 3.0814519)) {
        if ( (data[5].fvalue < 2.297894)) {
          if ( (data[5].fvalue < 0.94350386)) {
            if ( (data[7].fvalue < 0.46801662)) {
              sum += (float)2.9334418e-05;
            } else {
              sum += (float)-0.0099242209;
            }
          } else {
            if ( (data[7].fvalue < 0.16008998)) {
              sum += (float)-0.0010277078;
            } else {
              sum += (float)0.0067349519;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.0033194274)) {
            if ( (data[7].fvalue < 0.23246574)) {
              sum += (float)0.0012695293;
            } else {
              sum += (float)0.004795515;
            }
          } else {
            if ( (data[6].fvalue < 0.054469116)) {
              sum += (float)-0.0038431687;
            } else {
              sum += (float)0.0014990005;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 4.2515411)) {
          if ( (data[7].fvalue < 0.26626307)) {
            if ( (data[6].fvalue < 0.070712984)) {
              sum += (float)0.0067656143;
            } else {
              sum += (float)0.010671221;
            }
          } else {
            if ( (data[5].fvalue < 3.9915957)) {
              sum += (float)0.0023296871;
            } else {
              sum += (float)0.011126542;
            }
          }
        } else {
          if ( (data[5].fvalue < 8.5349579)) {
            if ( (data[5].fvalue < 5.3506999)) {
              sum += (float)0.0028587407;
            } else {
              sum += (float)-0.0022730178;
            }
          } else {
            if ( (data[7].fvalue < 0.092650399)) {
              sum += (float)0.0081049018;
            } else {
              sum += (float)8.9246139e-05;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 0.82850248)) {
      if ( (data[5].fvalue < 7.1751823)) {
        if ( (data[7].fvalue < 0.40135074)) {
          if ( (data[5].fvalue < 2.2256494)) {
            if ( (data[2].fvalue < 0.5)) {
              sum += (float)0.014719211;
            } else {
              sum += (float)0.021819308;
            }
          } else {
            if ( (data[5].fvalue < 2.7855949)) {
              sum += (float)0.0062699504;
            } else {
              sum += (float)0.011210597;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.2877846)) {
            if ( (data[5].fvalue < 2.0444355)) {
              sum += (float)0.010638924;
            } else {
              sum += (float)0.0030092534;
            }
          } else {
            if ( (data[7].fvalue < 0.42786995)) {
              sum += (float)-0.0054970551;
            } else {
              sum += (float)0.034319792;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 9.6382046)) {
          if ( (data[7].fvalue < 0.069227345)) {
            if ( (data[4].fvalue < 0.5)) {
              sum += (float)0.017585121;
            } else {
              sum += (float)-0.01286682;
            }
          } else {
            if ( (data[5].fvalue < 8.0303631)) {
              sum += (float)0.0040662312;
            } else {
              sum += (float)-0.0045979484;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.32507843)) {
            if ( (data[7].fvalue < 0.16676793)) {
              sum += (float)0.0066025853;
            } else {
              sum += (float)0.017706757;
            }
          } else {
            if ( (data[5].fvalue < 13.351034)) {
              sum += (float)-0.014471564;
            } else {
              sum += (float)0.0061841165;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.20398647)) {
        if ( (data[5].fvalue < 23.478415)) {
          if ( (data[7].fvalue < 0.20357497)) {
            if ( (data[5].fvalue < 12.376128)) {
              sum += (float)0.0050697718;
            } else {
              sum += (float)0.017812476;
            }
          } else {
            sum += (float)-0.041821614;
          }
        } else {
          if ( (data[7].fvalue < 0.10903795)) {
            if ( (data[5].fvalue < 25.941288)) {
              sum += (float)0.036498044;
            } else {
              sum += (float)-0.0017659152;
            }
          } else {
            if ( (data[6].fvalue < 1.1196747)) {
              sum += (float)-0.016072262;
            } else {
              sum += (float)-0.061240681;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.2382831)) {
          if ( (data[7].fvalue < 0.22550529)) {
            if ( (data[7].fvalue < 0.22529578)) {
              sum += (float)0.024580386;
            } else {
              sum += (float)-0.049173761;
            }
          } else {
            if ( (data[6].fvalue < 1.1580399)) {
              sum += (float)0.038789902;
            } else {
              sum += (float)0.083455592;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.26353759)) {
            if ( (data[7].fvalue < 0.2450166)) {
              sum += (float)0.013507552;
            } else {
              sum += (float)-0.01545883;
            }
          } else {
            if ( (data[7].fvalue < 0.29315442)) {
              sum += (float)0.040124938;
            } else {
              sum += (float)0.016044347;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.4972136)) {
    if ( (data[5].fvalue < 2.9362702)) {
      if ( (data[5].fvalue < 2.2294559)) {
        if ( (data[7].fvalue < 0.67786503)) {
          if ( (data[7].fvalue < 0.1003218)) {
            if ( (data[5].fvalue < 1.8365065)) {
              sum += (float)0.0061092777;
            } else {
              sum += (float)5.0025978e-05;
            }
          } else {
            if ( (data[7].fvalue < 0.27087227)) {
              sum += (float)0.0070338105;
            } else {
              sum += (float)0.0039664069;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.9987652)) {
            if ( (data[5].fvalue < 1.7096338)) {
              sum += (float)-0.012709069;
            } else {
              sum += (float)-0.0036213081;
            }
          } else {
            if ( (data[7].fvalue < 0.77261198)) {
              sum += (float)0.015234094;
            } else {
              sum += (float)-0.007581044;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.10284543)) {
          if ( (data[5].fvalue < 2.4731302)) {
            if ( (data[6].fvalue < 0.082463965)) {
              sum += (float)-0.0019046018;
            } else {
              sum += (float)-0.010326779;
            }
          } else {
            if ( (data[5].fvalue < 2.7833376)) {
              sum += (float)0.011153925;
            } else {
              sum += (float)0.0062324735;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.16258588)) {
            if ( (data[6].fvalue < 0.0033194274)) {
              sum += (float)0.0013943616;
            } else {
              sum += (float)-0.002335991;
            }
          } else {
            if ( (data[6].fvalue < 0.48773023)) {
              sum += (float)0.0037538609;
            } else {
              sum += (float)-0.020255461;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 3.7609155)) {
        if ( (data[7].fvalue < 0.23103462)) {
          if ( (data[6].fvalue < 0.029673766)) {
            if ( (data[6].fvalue < 0.003506317)) {
              sum += (float)0.0058644214;
            } else {
              sum += (float)0.0018568403;
            }
          } else {
            if ( (data[5].fvalue < 3.1437473)) {
              sum += (float)0.0082543418;
            } else {
              sum += (float)0.012640008;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.48664057)) {
            if ( (data[6].fvalue < 0.48493963)) {
              sum += (float)0.0019963451;
            } else {
              sum += (float)-0.045556977;
            }
          } else {
            if ( (data[7].fvalue < 0.4488827)) {
              sum += (float)0.023745265;
            } else {
              sum += (float)-0.012725621;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 9.6408062)) {
          if ( (data[7].fvalue < 0.21401268)) {
            if ( (data[5].fvalue < 5.2541466)) {
              sum += (float)0.0028641191;
            } else {
              sum += (float)0.00065129466;
            }
          } else {
            if ( (data[5].fvalue < 7.200911)) {
              sum += (float)0.0052249832;
            } else {
              sum += (float)-0.0010850932;
            }
          }
        } else {
          if ( (data[5].fvalue < 19.020031)) {
            if ( (data[7].fvalue < 0.26503897)) {
              sum += (float)0.0072043962;
            } else {
              sum += (float)0.0010113276;
            }
          } else {
            if ( (data[7].fvalue < 0.19933477)) {
              sum += (float)-0.0021574351;
            } else {
              sum += (float)0.010256294;
            }
          }
        }
      }
    }
  } else {
    if ( (data[2].fvalue < 0.5)) {
      if ( (data[7].fvalue < 0.77311432)) {
        if ( (data[7].fvalue < 0.64952874)) {
          if ( (data[5].fvalue < 3.3617535)) {
            if ( (data[5].fvalue < 3.0111156)) {
              sum += (float)0.007584102;
            } else {
              sum += (float)0.021010533;
            }
          } else {
            if ( (data[5].fvalue < 3.4451241)) {
              sum += (float)-0.02833643;
            } else {
              sum += (float)0.0071357526;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.6223514)) {
            if ( (data[5].fvalue < 1.7368257)) {
              sum += (float)-0.018148623;
            } else {
              sum += (float)0.016202549;
            }
          } else {
            if ( (data[5].fvalue < 3.0957284)) {
              sum += (float)-0.036213186;
            } else {
              sum += (float)0.0103363;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.0964847)) {
          if ( (data[6].fvalue < 0.61616659)) {
            sum += (float)0.0074171666;
          } else {
            sum += (float)-0.03425771;
          }
        } else {
          if ( (data[7].fvalue < 0.83519661)) {
            if ( (data[5].fvalue < 3.2932677)) {
              sum += (float)0.06392663;
            } else {
              sum += (float)0.00039228558;
            }
          } else {
            if ( (data[5].fvalue < 3.1822982)) {
              sum += (float)-0.020861747;
            } else {
              sum += (float)0.029827915;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.60194534)) {
        if ( (data[5].fvalue < 2.7303562)) {
          if ( (data[6].fvalue < 0.97205001)) {
            if ( (data[5].fvalue < 1.7354925)) {
              sum += (float)0.028082887;
            } else {
              sum += (float)0.00203968;
            }
          } else {
            if ( (data[5].fvalue < 2.4042869)) {
              sum += (float)-0.020883214;
            } else {
              sum += (float)-0.085359707;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.1654859)) {
            if ( (data[6].fvalue < 1.0232829)) {
              sum += (float)0.019347856;
            } else {
              sum += (float)0.084731288;
            }
          } else {
            if ( (data[5].fvalue < 3.2275302)) {
              sum += (float)-0.010821503;
            } else {
              sum += (float)0.012171998;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.5455787)) {
          if ( (data[7].fvalue < 0.76764834)) {
            if ( (data[6].fvalue < 0.93173194)) {
              sum += (float)0.027534723;
            } else {
              sum += (float)0.0706404;
            }
          } else {
            if ( (data[6].fvalue < 1.2960113)) {
              sum += (float)-0.026001172;
            } else {
              sum += (float)0.028633257;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.56277657)) {
            sum += (float)-0.043817449;
          } else {
            if ( (data[6].fvalue < 0.89472306)) {
              sum += (float)-0.0010179847;
            } else {
              sum += (float)-0.03242125;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.31126267)) {
    if ( (data[6].fvalue < 0.00038430787)) {
      if ( (data[5].fvalue < 1.3541512)) {
        if ( (data[3].fvalue < 0.5)) {
          if ( (data[5].fvalue < 0.56780553)) {
            if ( (data[7].fvalue < 0.092278108)) {
              sum += (float)0.0027367151;
            } else {
              sum += (float)-0.0083127785;
            }
          } else {
            if ( (data[7].fvalue < 0.50968593)) {
              sum += (float)0.0015049212;
            } else {
              sum += (float)-0.0020980553;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.46687675)) {
            if ( (data[7].fvalue < 0.31792462)) {
              sum += (float)-0.0069362964;
            } else {
              sum += (float)0.002692098;
            }
          } else {
            if ( (data[5].fvalue < 1.3303134)) {
              sum += (float)-0.01796581;
            } else {
              sum += (float)0.0035375387;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.2540259)) {
          if ( (data[7].fvalue < 0.067873389)) {
            if ( (data[5].fvalue < 1.8253547)) {
              sum += (float)0.0048568668;
            } else {
              sum += (float)0.00052685902;
            }
          } else {
            if ( (data[7].fvalue < 0.32648954)) {
              sum += (float)0.006921174;
            } else {
              sum += (float)0.003299396;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.5152397)) {
            if ( (data[7].fvalue < 0.20024318)) {
              sum += (float)-0.00061541382;
            } else {
              sum += (float)0.0060752234;
            }
          } else {
            if ( (data[7].fvalue < 0.11639829)) {
              sum += (float)0.0039439602;
            } else {
              sum += (float)0.0025490043;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.054469116)) {
        if ( (data[5].fvalue < 8.4696503)) {
          if ( (data[5].fvalue < 2.5882673)) {
            if ( (data[5].fvalue < 2.0605173)) {
              sum += (float)0.00064492784;
            } else {
              sum += (float)-0.0026525503;
            }
          } else {
            if ( (data[7].fvalue < 0.13980889)) {
              sum += (float)0.0027583116;
            } else {
              sum += (float)-0.00069729873;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.060108721)) {
            if ( (data[5].fvalue < 19.032829)) {
              sum += (float)0.012239404;
            } else {
              sum += (float)0.0022249927;
            }
          } else {
            if ( (data[5].fvalue < 10.269232)) {
              sum += (float)-0.001049553;
            } else {
              sum += (float)0.0054858685;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 5.0557098)) {
          if ( (data[7].fvalue < 0.074440971)) {
            if ( (data[5].fvalue < 2.4280994)) {
              sum += (float)0.00066054624;
            } else {
              sum += (float)0.010150857;
            }
          } else {
            if ( (data[5].fvalue < 1.8645027)) {
              sum += (float)0.0059715309;
            } else {
              sum += (float)0.0026602189;
            }
          }
        } else {
          if ( (data[5].fvalue < 8.6760387)) {
            if ( (data[6].fvalue < 0.28919691)) {
              sum += (float)-0.0043108324;
            } else {
              sum += (float)0.0082997857;
            }
          } else {
            if ( (data[5].fvalue < 20.009869)) {
              sum += (float)0.0025678149;
            } else {
              sum += (float)-0.0049963137;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.3734715)) {
      if ( (data[5].fvalue < 6.2213078)) {
        if ( (data[7].fvalue < 0.16420063)) {
          if ( (data[5].fvalue < 2.9336491)) {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)0.0050584665;
            } else {
              sum += (float)-0.006010788;
            }
          } else {
            if ( (data[5].fvalue < 3.4810321)) {
              sum += (float)0.024241732;
            } else {
              sum += (float)0.0097329235;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.53599912)) {
            if ( (data[6].fvalue < 0.53558475)) {
              sum += (float)0.0042749769;
            } else {
              sum += (float)-0.057881102;
            }
          } else {
            if ( (data[6].fvalue < 1.3506445)) {
              sum += (float)0.0076111276;
            } else {
              sum += (float)-0.036050495;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 6.7017026)) {
          if ( (data[7].fvalue < 0.2306712)) {
            if ( (data[6].fvalue < 1.0202894)) {
              sum += (float)0.0038984625;
            } else {
              sum += (float)-0.041189138;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.017810741;
            } else {
              sum += (float)-0.028710866;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.8762455)) {
            if ( (data[6].fvalue < 0.9906618)) {
              sum += (float)0.030413425;
            } else {
              sum += (float)-0.030130101;
            }
          } else {
            if ( (data[5].fvalue < 10.790912)) {
              sum += (float)-0.00047533715;
            } else {
              sum += (float)0.0057453923;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 8.3507509)) {
        if ( (data[6].fvalue < 1.3832057)) {
          sum += (float)0.077314541;
        } else {
          if ( (data[7].fvalue < 0.62981117)) {
            if ( (data[5].fvalue < 4.5159092)) {
              sum += (float)0.052534543;
            } else {
              sum += (float)0.019588236;
            }
          } else {
            if ( (data[1].fvalue < 0.5)) {
              sum += (float)0.037037436;
            } else {
              sum += (float)0.00044982258;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 1.4169521)) {
          if ( (data[5].fvalue < 14.708443)) {
            if ( (data[7].fvalue < 0.34241578)) {
              sum += (float)-0.050032668;
            } else {
              sum += (float)0.019661039;
            }
          } else {
            if ( (data[5].fvalue < 18.435747)) {
              sum += (float)0.00064746739;
            } else {
              sum += (float)0.022188127;
            }
          }
        } else {
          if ( (data[5].fvalue < 8.6782522)) {
            sum += (float)-0.049607888;
          } else {
            if ( (data[7].fvalue < 0.36468434)) {
              sum += (float)0.014865242;
            } else {
              sum += (float)-0.003740181;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.22192019)) {
    if ( (data[2].fvalue < 0.5)) {
      if ( (data[1].fvalue < 0.5)) {
        if ( (data[5].fvalue < 2.6097164)) {
          if ( (data[5].fvalue < 1.9766195)) {
            if ( (data[5].fvalue < 0.73882723)) {
              sum += (float)-0.0025514958;
            } else {
              sum += (float)0.0036122948;
            }
          } else {
            if ( (data[6].fvalue < 0.0034771424)) {
              sum += (float)0.0019860882;
            } else {
              sum += (float)-0.0016680193;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.097696349)) {
            if ( (data[5].fvalue < 3.5114207)) {
              sum += (float)0.0040917066;
            } else {
              sum += (float)0.0015419848;
            }
          } else {
            if ( (data[6].fvalue < 0.10994799)) {
              sum += (float)0.013334152;
            } else {
              sum += (float)0.0056827441;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.71792138)) {
          if ( (data[5].fvalue < 11.556337)) {
            if ( (data[5].fvalue < 5.9933605)) {
              sum += (float)0.00085340603;
            } else {
              sum += (float)-0.0021319599;
            }
          } else {
            if ( (data[7].fvalue < 0.18017751)) {
              sum += (float)0.00021153668;
            } else {
              sum += (float)0.0096077723;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.10609116)) {
            if ( (data[5].fvalue < 1.7901897)) {
              sum += (float)-0.014860307;
            } else {
              sum += (float)-0.0012839042;
            }
          } else {
            if ( (data[6].fvalue < 0.19220942)) {
              sum += (float)-0.018571902;
            } else {
              sum += (float)-0.0016786676;
            }
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 0.0010263367)) {
        if ( (data[7].fvalue < 0.18790963)) {
          if ( (data[5].fvalue < 2.7869544)) {
            if ( (data[5].fvalue < 2.0434523)) {
              sum += (float)0.0054298318;
            } else {
              sum += (float)-0.0037213725;
            }
          } else {
            if ( (data[5].fvalue < 3.9821932)) {
              sum += (float)0.0032664409;
            } else {
              sum += (float)0.0011776527;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.8777273)) {
            if ( (data[7].fvalue < 0.30201676)) {
              sum += (float)-0.0030495003;
            } else {
              sum += (float)0.0015469247;
            }
          } else {
            if ( (data[5].fvalue < 2.4404409)) {
              sum += (float)0.007718314;
            } else {
              sum += (float)0.0018477273;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.05573184)) {
          if ( (data[5].fvalue < 3.2537599)) {
            if ( (data[7].fvalue < 0.4657813)) {
              sum += (float)-0.0016688384;
            } else {
              sum += (float)-0.0065330155;
            }
          } else {
            if ( (data[7].fvalue < 0.11389044)) {
              sum += (float)-0.0014813007;
            } else {
              sum += (float)0.0026607537;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.1098656)) {
            if ( (data[6].fvalue < 0.097239047)) {
              sum += (float)0.001479952;
            } else {
              sum += (float)0.0084870681;
            }
          } else {
            if ( (data[6].fvalue < 0.15860742)) {
              sum += (float)-0.0017970813;
            } else {
              sum += (float)0.0015352353;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 2.2516525)) {
      if ( (data[6].fvalue < 0.98054862)) {
        if ( (data[7].fvalue < 0.69563603)) {
          if ( (data[5].fvalue < 1.1399264)) {
            if ( (data[7].fvalue < 0.51043916)) {
              sum += (float)0.021882987;
            } else {
              sum += (float)-0.02260736;
            }
          } else {
            if ( (data[7].fvalue < 0.59344798)) {
              sum += (float)0.0064493981;
            } else {
              sum += (float)0.013514476;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.93815738)) {
            if ( (data[5].fvalue < 1.5996072)) {
              sum += (float)-0.01224037;
            } else {
              sum += (float)0.0003076221;
            }
          } else {
            sum += (float)-0.07351172;
          }
        }
      } else {
        if ( (data[7].fvalue < 0.6568163)) {
          if ( (data[5].fvalue < 2.1897502)) {
            if ( (data[6].fvalue < 1.0598156)) {
              sum += (float)0.001704208;
            } else {
              sum += (float)-0.040914785;
            }
          } else {
            sum += (float)0.043559417;
          }
        } else {
          if ( (data[7].fvalue < 0.78382134)) {
            if ( (data[6].fvalue < 1.0923861)) {
              sum += (float)0.092241473;
            } else {
              sum += (float)0.04423748;
            }
          } else {
            sum += (float)-0.022054182;
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.41711426)) {
        if ( (data[6].fvalue < 0.6641162)) {
          if ( (data[5].fvalue < 3.6859503)) {
            if ( (data[6].fvalue < 0.65310645)) {
              sum += (float)0.004148656;
            } else {
              sum += (float)-0.021216311;
            }
          } else {
            if ( (data[7].fvalue < 0.068705127)) {
              sum += (float)0.0062111611;
            } else {
              sum += (float)0.0011437638;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.3675282)) {
            if ( (data[6].fvalue < 1.0139163)) {
              sum += (float)0.012062929;
            } else {
              sum += (float)0.053692743;
            }
          } else {
            if ( (data[5].fvalue < 3.3768435)) {
              sum += (float)-0.052300498;
            } else {
              sum += (float)0.0059378142;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 3.0111156)) {
          if ( (data[5].fvalue < 2.9462678)) {
            if ( (data[7].fvalue < 0.73520112)) {
              sum += (float)-0.00466287;
            } else {
              sum += (float)0.0067652506;
            }
          } else {
            if ( (data[6].fvalue < 1.0255703)) {
              sum += (float)-0.018405547;
            } else {
              sum += (float)-0.086208358;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.42250592)) {
            if ( (data[6].fvalue < 0.36357075)) {
              sum += (float)0.0056460779;
            } else {
              sum += (float)-0.035175346;
            }
          } else {
            if ( (data[5].fvalue < 3.1343265)) {
              sum += (float)0.020718878;
            } else {
              sum += (float)0.0026825513;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.40845487)) {
    if ( (data[7].fvalue < 0.64955986)) {
      if ( (data[4].fvalue < 0.5)) {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[5].fvalue < 3.1459584)) {
            if ( (data[5].fvalue < 2.3836255)) {
              sum += (float)0.001688219;
            } else {
              sum += (float)-0.0009473316;
            }
          } else {
            if ( (data[5].fvalue < 3.7456455)) {
              sum += (float)0.0027186447;
            } else {
              sum += (float)0.00080154667;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.5362196)) {
            if ( (data[7].fvalue < 0.11690579)) {
              sum += (float)0.0001254839;
            } else {
              sum += (float)0.0027863777;
            }
          } else {
            if ( (data[5].fvalue < 3.5052772)) {
              sum += (float)0.0033368042;
            } else {
              sum += (float)0.00099742017;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.700825)) {
          if ( (data[5].fvalue < 1.9753609)) {
            if ( (data[5].fvalue < 0.82855606)) {
              sum += (float)-0.0017003666;
            } else {
              sum += (float)0.0042698174;
            }
          } else {
            if ( (data[7].fvalue < 0.24253544)) {
              sum += (float)-0.00047680695;
            } else {
              sum += (float)0.0097150654;
            }
          }
        } else {
          if ( (data[5].fvalue < 3.9044178)) {
            if ( (data[7].fvalue < 0.16050786)) {
              sum += (float)0.0050117765;
            } else {
              sum += (float)-0.00032717275;
            }
          } else {
            if ( (data[5].fvalue < 4.2766843)) {
              sum += (float)-0.0017915455;
            } else {
              sum += (float)0.0022557944;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 1.5916084)) {
        if ( (data[5].fvalue < 1.3296776)) {
          if ( (data[6].fvalue < 0.12654489)) {
            if ( (data[7].fvalue < 0.70718479)) {
              sum += (float)-0.013334361;
            } else {
              sum += (float)-0.0041276305;
            }
          } else {
            if ( (data[7].fvalue < 0.69086653)) {
              sum += (float)-0.034129508;
            } else {
              sum += (float)-0.013660723;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.3582924)) {
            if ( (data[6].fvalue < 0.28569633)) {
              sum += (float)-0.0068862657;
            } else {
              sum += (float)-0.028513316;
            }
          } else {
            if ( (data[5].fvalue < 1.4564354)) {
              sum += (float)0.028842747;
            } else {
              sum += (float)-0.0079421885;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.7472842)) {
          if ( (data[7].fvalue < 0.7799325)) {
            if ( (data[5].fvalue < 2.0003138)) {
              sum += (float)-0.00072921591;
            } else {
              sum += (float)0.010304905;
            }
          } else {
            if ( (data[5].fvalue < 2.4613118)) {
              sum += (float)-0.013003628;
            } else {
              sum += (float)0.0028216697;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.80153608)) {
            if ( (data[7].fvalue < 0.66501987)) {
              sum += (float)0.0059662741;
            } else {
              sum += (float)-0.0087163299;
            }
          } else {
            if ( (data[7].fvalue < 0.83631027)) {
              sum += (float)0.0091441246;
            } else {
              sum += (float)-0.0055675111;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.2015269)) {
      if ( (data[6].fvalue < 1.19426)) {
        if ( (data[5].fvalue < 20.004335)) {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[5].fvalue < 11.150696)) {
              sum += (float)0.0012699051;
            } else {
              sum += (float)0.0080225114;
            }
          } else {
            if ( (data[7].fvalue < 0.59463096)) {
              sum += (float)0.0039665494;
            } else {
              sum += (float)0.012004133;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.16332197)) {
            if ( (data[5].fvalue < 20.087881)) {
              sum += (float)-0.044599451;
            } else {
              sum += (float)-0.010211894;
            }
          } else {
            if ( (data[6].fvalue < 0.73683929)) {
              sum += (float)0.018817691;
            } else {
              sum += (float)-0.0079467222;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.14906439)) {
          sum += (float)0.0089822896;
        } else {
          if ( (data[5].fvalue < 22.210419)) {
            if ( (data[5].fvalue < 6.4144735)) {
              sum += (float)-0.013644641;
            } else {
              sum += (float)-0.057625841;
            }
          } else {
            sum += (float)-0.0017766595;
          }
        }
      }
    } else {
      if ( (data[6].fvalue < 1.2060061)) {
        sum += (float)0.068973005;
      } else {
        if ( (data[6].fvalue < 1.2152082)) {
          if ( (data[6].fvalue < 1.2076558)) {
            if ( (data[5].fvalue < 3.6789083)) {
              sum += (float)0.033775058;
            } else {
              sum += (float)-0.025340296;
            }
          } else {
            if ( (data[6].fvalue < 1.212278)) {
              sum += (float)-0.066327915;
            } else {
              sum += (float)-0.015847225;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.221585)) {
            if ( (data[5].fvalue < 8.6474485)) {
              sum += (float)0.061592806;
            } else {
              sum += (float)0.020940147;
            }
          } else {
            if ( (data[6].fvalue < 1.2328484)) {
              sum += (float)-0.016096625;
            } else {
              sum += (float)0.0080208872;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 0.16251728)) {
    if ( (data[6].fvalue < 0.10994022)) {
      if ( (data[6].fvalue < 0.097605199)) {
        if ( (data[6].fvalue < 0.0034833374)) {
          if ( (data[5].fvalue < 1.4785845)) {
            if ( (data[3].fvalue < 0.5)) {
              sum += (float)-0.00032233054;
            } else {
              sum += (float)-0.0055946182;
            }
          } else {
            if ( (data[5].fvalue < 2.2984028)) {
              sum += (float)0.0022874679;
            } else {
              sum += (float)0.0010350773;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.015256347)) {
            if ( (data[5].fvalue < 3.9985464)) {
              sum += (float)-0.0029913161;
            } else {
              sum += (float)0.00246449;
            }
          } else {
            if ( (data[7].fvalue < 0.072434083)) {
              sum += (float)0.0019417756;
            } else {
              sum += (float)7.3056573e-05;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.1029005)) {
          if ( (data[7].fvalue < 0.54981011)) {
            if ( (data[7].fvalue < 0.12130357)) {
              sum += (float)0.0043450836;
            } else {
              sum += (float)0.018307304;
            }
          } else {
            if ( (data[5].fvalue < 2.0365987)) {
              sum += (float)-0.0010586273;
            } else {
              sum += (float)0.020365899;
            }
          }
        } else {
          if ( (data[5].fvalue < 4.4884233)) {
            if ( (data[5].fvalue < 2.6834772)) {
              sum += (float)0.0037890738;
            } else {
              sum += (float)0.0087102158;
            }
          } else {
            if ( (data[7].fvalue < 0.22878617)) {
              sum += (float)-0.0008955099;
            } else {
              sum += (float)0.013183014;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.3853305)) {
        if ( (data[7].fvalue < 0.15520392)) {
          if ( (data[5].fvalue < 4.6835022)) {
            if ( (data[5].fvalue < 2.5568619)) {
              sum += (float)-0.0020956013;
            } else {
              sum += (float)0.0050453856;
            }
          } else {
            if ( (data[5].fvalue < 19.233744)) {
              sum += (float)-0.0017300652;
            } else {
              sum += (float)-0.011143631;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.3718364)) {
            if ( (data[5].fvalue < 0.7267012)) {
              sum += (float)-0.011886288;
            } else {
              sum += (float)0.0015817506;
            }
          } else {
            if ( (data[5].fvalue < 3.0110583)) {
              sum += (float)-0.0058263447;
            } else {
              sum += (float)-0.00078473444;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.51152641)) {
          if ( (data[5].fvalue < 1.6325257)) {
            if ( (data[6].fvalue < 0.12951624)) {
              sum += (float)-0.008025147;
            } else {
              sum += (float)0.004605107;
            }
          } else {
            if ( (data[6].fvalue < 0.13595907)) {
              sum += (float)-0.0031176773;
            } else {
              sum += (float)-0.013110063;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.0227958)) {
            if ( (data[7].fvalue < 0.55527329)) {
              sum += (float)-0.025972303;
            } else {
              sum += (float)-0.012406682;
            }
          } else {
            if ( (data[7].fvalue < 0.51997983)) {
              sum += (float)-0.022874201;
            } else {
              sum += (float)-0.0084219957;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 1.6429973)) {
      if ( (data[7].fvalue < 0.62927568)) {
        if ( (data[6].fvalue < 0.5238539)) {
          if ( (data[7].fvalue < 0.31582063)) {
            if ( (data[5].fvalue < 1.3742)) {
              sum += (float)-0.01393922;
            } else {
              sum += (float)0.010516292;
            }
          } else {
            if ( (data[7].fvalue < 0.56992424)) {
              sum += (float)0.010261832;
            } else {
              sum += (float)-0.00060397945;
            }
          }
        } else {
          if ( (data[4].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.58978999)) {
              sum += (float)0.022440162;
            } else {
              sum += (float)0.052155603;
            }
          } else {
            if ( (data[6].fvalue < 0.58489847)) {
              sum += (float)0.020674789;
            } else {
              sum += (float)-0.0081058508;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 1.3438737)) {
          if ( (data[7].fvalue < 0.68833816)) {
            if ( (data[5].fvalue < 1.1998959)) {
              sum += (float)-0.015908727;
            } else {
              sum += (float)-0.03706339;
            }
          } else {
            if ( (data[5].fvalue < 1.2945687)) {
              sum += (float)-0.0099951588;
            } else {
              sum += (float)0.0045768446;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.6234143)) {
            if ( (data[7].fvalue < 0.64594996)) {
              sum += (float)0.027261974;
            } else {
              sum += (float)-0.004172239;
            }
          } else {
            if ( (data[6].fvalue < 0.63280022)) {
              sum += (float)0.055611946;
            } else {
              sum += (float)0.0028202471;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 1.6531932)) {
        if ( (data[7].fvalue < 0.58524179)) {
          if ( (data[6].fvalue < 0.52813196)) {
            if ( (data[5].fvalue < 1.6522813)) {
              sum += (float)-0.0067953584;
            } else {
              sum += (float)-0.021863474;
            }
          } else {
            sum += (float)-0.061307468;
          }
        } else {
          if ( (data[7].fvalue < 0.67486542)) {
            sum += (float)0.020649128;
          } else {
            if ( (data[6].fvalue < 0.71906149)) {
              sum += (float)-0.027841154;
            } else {
              sum += (float)-0.0027210834;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.10019757)) {
          if ( (data[5].fvalue < 18.21874)) {
            if ( (data[6].fvalue < 1.1037)) {
              sum += (float)0.0050831586;
            } else {
              sum += (float)-0.040928755;
            }
          } else {
            if ( (data[5].fvalue < 18.228262)) {
              sum += (float)-0.04835045;
            } else {
              sum += (float)-0.001750824;
            }
          }
        } else {
          if ( (data[5].fvalue < 6.4192863)) {
            if ( (data[6].fvalue < 0.96050453)) {
              sum += (float)0.0015352787;
            } else {
              sum += (float)0.0082225082;
            }
          } else {
            if ( (data[5].fvalue < 6.6442652)) {
              sum += (float)-0.016260633;
            } else {
              sum += (float)-0.00051648583;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 3.1542645)) {
    if ( (data[6].fvalue < 2.5496688)) {
      if ( (data[6].fvalue < 2.204586)) {
        if ( (data[7].fvalue < 0.70024967)) {
          if ( (data[6].fvalue < 0.72150534)) {
            if ( (data[6].fvalue < 0.71651936)) {
              sum += (float)0.00069384766;
            } else {
              sum += (float)-0.028391367;
            }
          } else {
            if ( (data[6].fvalue < 0.72886795)) {
              sum += (float)0.022050047;
            } else {
              sum += (float)0.0024878285;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.70300829)) {
            if ( (data[7].fvalue < 0.70130461)) {
              sum += (float)-0.0013468543;
            } else {
              sum += (float)-0.029711572;
            }
          } else {
            if ( (data[5].fvalue < 1.8707004)) {
              sum += (float)-0.0064458451;
            } else {
              sum += (float)-1.2555314e-05;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 2.5256376)) {
          if ( (data[7].fvalue < 0.17282729)) {
            if ( (data[5].fvalue < 24.619411)) {
              sum += (float)0.0095334472;
            } else {
              sum += (float)-0.00015785695;
            }
          } else {
            sum += (float)0.037007444;
          }
        } else {
          if ( (data[5].fvalue < 11.435677)) {
            sum += (float)0.0038037479;
          } else {
            sum += (float)-0.0066291513;
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 12.288451)) {
        if ( (data[5].fvalue < 8.190218)) {
          sum += (float)-0.0092766173;
        } else {
          sum += (float)-0.046152763;
        }
      } else {
        if ( (data[7].fvalue < 0.31920364)) {
          if ( (data[5].fvalue < 17.479145)) {
            if ( (data[5].fvalue < 14.416603)) {
              sum += (float)-0.010974116;
            } else {
              sum += (float)0.0095334472;
            }
          } else {
            sum += (float)-0.02959254;
          }
        } else {
          sum += (float)0.030742623;
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 19.97455)) {
      sum += (float)0.046043996;
    } else {
      if ( (data[5].fvalue < 39.064953)) {
        sum += (float)0.0028477192;
      } else {
        sum += (float)-0.012651497;
      }
    }
  }
  if ( (data[5].fvalue < 2.9584582)) {
    if ( (data[5].fvalue < 2.2981477)) {
      if ( (data[7].fvalue < 0.0385653)) {
        if ( (data[5].fvalue < 1.801944)) {
          if ( (data[6].fvalue < 0.015350159)) {
            if ( (data[6].fvalue < 0.0029106447)) {
              sum += (float)0.00053859834;
            } else {
              sum += (float)-0.0019661644;
            }
          } else {
            if ( (data[5].fvalue < 1.6389527)) {
              sum += (float)0.0067485091;
            } else {
              sum += (float)-0.00064162869;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.0087576)) {
            if ( (data[6].fvalue < 0.01539029)) {
              sum += (float)-0.007397708;
            } else {
              sum += (float)-0.0029905559;
            }
          } else {
            if ( (data[5].fvalue < 2.2640843)) {
              sum += (float)-0.00089729507;
            } else {
              sum += (float)-0.0061424477;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.7323572)) {
          if ( (data[6].fvalue < 0.83659899)) {
            if ( (data[6].fvalue < 0.8234666)) {
              sum += (float)0.0010761025;
            } else {
              sum += (float)-0.054074131;
            }
          } else {
            if ( (data[7].fvalue < 0.62422794)) {
              sum += (float)-0.0087832073;
            } else {
              sum += (float)0.030848753;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.2149832)) {
            if ( (data[6].fvalue < 0.73245442)) {
              sum += (float)-0.0039722938;
            } else {
              sum += (float)-0.025082808;
            }
          } else {
            sum += (float)0.029472882;
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.11699054)) {
        if ( (data[5].fvalue < 2.4519076)) {
          if ( (data[7].fvalue < 0.0050555645)) {
            if ( (data[5].fvalue < 2.4150338)) {
              sum += (float)0.001362089;
            } else {
              sum += (float)0.026253896;
            }
          } else {
            if ( (data[6].fvalue < 0.082705475)) {
              sum += (float)-0.0038278606;
            } else {
              sum += (float)-0.010176036;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.7991338)) {
            if ( (data[7].fvalue < 0.074859224)) {
              sum += (float)0.0066559319;
            } else {
              sum += (float)-7.3661824e-05;
            }
          } else {
            if ( (data[7].fvalue < 0.084993526)) {
              sum += (float)-0.0022090513;
            } else {
              sum += (float)0.0093120756;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.20970327)) {
          if ( (data[5].fvalue < 2.8003104)) {
            if ( (data[5].fvalue < 2.4076521)) {
              sum += (float)-0.0011800292;
            } else {
              sum += (float)-0.004374905;
            }
          } else {
            if ( (data[7].fvalue < 0.14353627)) {
              sum += (float)0.005079654;
            } else {
              sum += (float)-0.0020800943;
            }
          }
        } else {
          if ( (data[3].fvalue < 0.5)) {
            if ( (data[5].fvalue < 2.3875642)) {
              sum += (float)0.0040439367;
            } else {
              sum += (float)-0.00071874936;
            }
          } else {
            if ( (data[7].fvalue < 0.38918298)) {
              sum += (float)-0.013150268;
            } else {
              sum += (float)-0.001173994;
            }
          }
        }
      }
    }
  } else {
    if ( (data[5].fvalue < 3.739717)) {
      if ( (data[6].fvalue < 0.023751862)) {
        if ( (data[7].fvalue < 0.02599171)) {
          if ( (data[5].fvalue < 3.44766)) {
            if ( (data[6].fvalue < 0.0046709292)) {
              sum += (float)-0.0014484135;
            } else {
              sum += (float)0.00057314074;
            }
          } else {
            if ( (data[7].fvalue < 0.002111861)) {
              sum += (float)-0.012770794;
            } else {
              sum += (float)-0.0060664001;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.0035686647)) {
            if ( (data[7].fvalue < 0.12683579)) {
              sum += (float)0.00059683394;
            } else {
              sum += (float)0.0020741613;
            }
          } else {
            if ( (data[5].fvalue < 3.4118531)) {
              sum += (float)-0.0032849715;
            } else {
              sum += (float)0.00040916211;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.211532)) {
          if ( (data[6].fvalue < 0.37475082)) {
            if ( (data[7].fvalue < 0.17786905)) {
              sum += (float)0.0047894511;
            } else {
              sum += (float)0.0016576386;
            }
          } else {
            if ( (data[6].fvalue < 0.49409527)) {
              sum += (float)0.016103871;
            } else {
              sum += (float)-0.0036325853;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.47440737)) {
            if ( (data[5].fvalue < 3.3010254)) {
              sum += (float)-0.00317325;
            } else {
              sum += (float)0.00066436757;
            }
          } else {
            if ( (data[5].fvalue < 3.3703973)) {
              sum += (float)0.008240344;
            } else {
              sum += (float)0.00091252296;
            }
          }
        }
      }
    } else {
      if ( (data[7].fvalue < 0.18006699)) {
        if ( (data[5].fvalue < 8.6855507)) {
          if ( (data[6].fvalue < 0.31126732)) {
            if ( (data[6].fvalue < 0.018698517)) {
              sum += (float)-0.00043540538;
            } else {
              sum += (float)-0.002046044;
            }
          } else {
            if ( (data[7].fvalue < 0.16417037)) {
              sum += (float)0.0036146757;
            } else {
              sum += (float)-0.0057028271;
            }
          }
        } else {
          if ( (data[5].fvalue < 19.998093)) {
            if ( (data[7].fvalue < 0.096816346)) {
              sum += (float)0.0031005156;
            } else {
              sum += (float)-0.0014995676;
            }
          } else {
            if ( (data[6].fvalue < 1.519383)) {
              sum += (float)-0.0030681072;
            } else {
              sum += (float)-0.022723502;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.10983536)) {
          if ( (data[3].fvalue < 0.5)) {
            if ( (data[6].fvalue < 0.077371247)) {
              sum += (float)0.0023288659;
            } else {
              sum += (float)0.0070062419;
            }
          } else {
            if ( (data[7].fvalue < 0.23160326)) {
              sum += (float)0.0045843548;
            } else {
              sum += (float)0.012183783;
            }
          }
        } else {
          if ( (data[5].fvalue < 10.786023)) {
            if ( (data[5].fvalue < 8.2756596)) {
              sum += (float)0.00031337552;
            } else {
              sum += (float)-0.0070232903;
            }
          } else {
            if ( (data[7].fvalue < 0.32339394)) {
              sum += (float)0.0097841388;
            } else {
              sum += (float)-0.0036466313;
            }
          }
        }
      }
    }
  }
  if ( (data[5].fvalue < 0.57486796)) {
    if ( (data[6].fvalue < 0.026317231)) {
      if ( (data[5].fvalue < 0.51016331)) {
        sum += (float)-0.008123016;
      } else {
        if ( (data[7].fvalue < 0.19301793)) {
          if ( (data[7].fvalue < 0.17208883)) {
            if ( (data[6].fvalue < 0.0029114685)) {
              sum += (float)0.00082281261;
            } else {
              sum += (float)-0.002911747;
            }
          } else {
            sum += (float)0.0069053741;
          }
        } else {
          sum += (float)-0.0064458591;
        }
      }
    } else {
      if ( (data[7].fvalue < 0.19688357)) {
        if ( (data[5].fvalue < 0.52973878)) {
          sum += (float)-0.013564008;
        } else {
          sum += (float)-0.002174424;
        }
      } else {
        sum += (float)-0.019564655;
      }
    }
  } else {
    if ( (data[7].fvalue < 0.41807228)) {
      if ( (data[6].fvalue < 0.20600986)) {
        if ( (data[4].fvalue < 0.5)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.17482987)) {
              sum += (float)-0.00074647617;
            } else {
              sum += (float)0.00045631736;
            }
          } else {
            if ( (data[5].fvalue < 5.786921)) {
              sum += (float)0.0003186437;
            } else {
              sum += (float)0.0026922135;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.0035892029)) {
            if ( (data[5].fvalue < 3.7459874)) {
              sum += (float)0.0021690854;
            } else {
              sum += (float)0.00022283642;
            }
          } else {
            if ( (data[5].fvalue < 1.9143976)) {
              sum += (float)0.0024840524;
            } else {
              sum += (float)0.00017997334;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 2.1509509)) {
          if ( (data[7].fvalue < 0.26999736)) {
            if ( (data[5].fvalue < 2.0173838)) {
              sum += (float)0.0079830615;
            } else {
              sum += (float)0.014020221;
            }
          } else {
            if ( (data[5].fvalue < 1.1764963)) {
              sum += (float)0.019836742;
            } else {
              sum += (float)0.0010579431;
            }
          }
        } else {
          if ( (data[5].fvalue < 2.1696024)) {
            if ( (data[7].fvalue < 0.26861721)) {
              sum += (float)-0.0071295151;
            } else {
              sum += (float)-0.028943492;
            }
          } else {
            if ( (data[5].fvalue < 6.1380582)) {
              sum += (float)0.0015293318;
            } else {
              sum += (float)-0.00047693445;
            }
          }
        }
      }
    } else {
      if ( (data[5].fvalue < 1.0469013)) {
        if ( (data[7].fvalue < 0.47100222)) {
          if ( (data[6].fvalue < 0.12613998)) {
            if ( (data[6].fvalue < 0.10013275)) {
              sum += (float)-0.0023601847;
            } else {
              sum += (float)-0.01561525;
            }
          } else {
            if ( (data[5].fvalue < 0.8398059)) {
              sum += (float)-0.021465091;
            } else {
              sum += (float)0.023093317;
            }
          }
        } else {
          if ( (data[5].fvalue < 1.0297279)) {
            if ( (data[6].fvalue < 0.37874958)) {
              sum += (float)-0.0084036281;
            } else {
              sum += (float)0.02520751;
            }
          } else {
            if ( (data[6].fvalue < 0.024190186)) {
              sum += (float)-0.0087570744;
            } else {
              sum += (float)-0.029890791;
            }
          }
        }
      } else {
        if ( (data[5].fvalue < 5.2680664)) {
          if ( (data[5].fvalue < 4.2490597)) {
            if ( (data[5].fvalue < 4.2257032)) {
              sum += (float)-0.00029082669;
            } else {
              sum += (float)0.026183186;
            }
          } else {
            if ( (data[6].fvalue < 0.26824546)) {
              sum += (float)0.00015953268;
            } else {
              sum += (float)-0.014530024;
            }
          }
        } else {
          if ( (data[5].fvalue < 10.094309)) {
            if ( (data[7].fvalue < 0.44576249)) {
              sum += (float)-0.0026118578;
            } else {
              sum += (float)0.01000908;
            }
          } else {
            if ( (data[6].fvalue < 1.0479228)) {
              sum += (float)-0.00092342618;
            } else {
              sum += (float)-0.016244192;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 1.8228552)) {
    if ( (data[7].fvalue < 0.33449918)) {
      if ( (data[7].fvalue < 0.33420524)) {
        if ( (data[6].fvalue < 0.75867343)) {
          if ( (data[6].fvalue < 0.75302202)) {
            if ( (data[6].fvalue < 0.097581789)) {
              sum += (float)0.00015500037;
            } else {
              sum += (float)0.00073757098;
            }
          } else {
            if ( (data[0].fvalue < 0.5)) {
              sum += (float)-0.024365226;
            } else {
              sum += (float)0.0049546333;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.76454878)) {
            if ( (data[6].fvalue < 0.76125288)) {
              sum += (float)0.014938111;
            } else {
              sum += (float)0.041858461;
            }
          } else {
            if ( (data[5].fvalue < 6.9558744)) {
              sum += (float)0.0073177274;
            } else {
              sum += (float)-0.0011137743;
            }
          }
        }
      } else {
        if ( (data[6].fvalue < 0.39025778)) {
          if ( (data[0].fvalue < 0.5)) {
            if ( (data[7].fvalue < 0.33428001)) {
              sum += (float)0.0025170881;
            } else {
              sum += (float)0.0078102811;
            }
          } else {
            sum += (float)8.8390894e-05;
          }
        } else {
          sum += (float)0.043712012;
        }
      }
    } else {
      if ( (data[5].fvalue < 0.8386783)) {
        if ( (data[6].fvalue < 0.079644993)) {
          if ( (data[5].fvalue < 0.74135131)) {
            if ( (data[6].fvalue < 0.021031007)) {
              sum += (float)-0.0075059808;
            } else {
              sum += (float)-0.013071728;
            }
          } else {
            if ( (data[5].fvalue < 0.79736906)) {
              sum += (float)-0.0018046751;
            } else {
              sum += (float)-0.0077219503;
            }
          }
        } else {
          if ( (data[6].fvalue < 0.2383666)) {
            if ( (data[6].fvalue < 0.14084488)) {
              sum += (float)-0.013046042;
            } else {
              sum += (float)-0.023778731;
            }
          } else {
            sum += (float)0.00022839454;
          }
        }
      } else {
        if ( (data[0].fvalue < 0.5)) {
          if ( (data[7].fvalue < 0.33739454)) {
            if ( (data[6].fvalue < 0.40254492)) {
              sum += (float)-0.0031086218;
            } else {
              sum += (float)-0.017231725;
            }
          } else {
            if ( (data[4].fvalue < 0.5)) {
              sum += (float)-0.00034477678;
            } else {
              sum += (float)0.0033364154;
            }
          }
        } else {
          if ( (data[7].fvalue < 0.48917842)) {
            if ( (data[6].fvalue < 0.13801633)) {
              sum += (float)0.0046438519;
            } else {
              sum += (float)0.017361848;
            }
          } else {
            if ( (data[6].fvalue < 0.17670569)) {
              sum += (float)0.0064214305;
            } else {
              sum += (float)-0.016914112;
            }
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.9686902)) {
      if ( (data[7].fvalue < 0.47431517)) {
        if ( (data[7].fvalue < 0.23484099)) {
          if ( (data[5].fvalue < 16.778133)) {
            sum += (float)0.014814294;
          } else {
            sum += (float)-0.0058976593;
          }
        } else {
          if ( (data[7].fvalue < 0.27880561)) {
            sum += (float)0.060550477;
          } else {
            if ( (data[5].fvalue < 8.8582306)) {
              sum += (float)0.027252035;
            } else {
              sum += (float)5.0532821e-05;
            }
          }
        }
      } else {
        if ( (data[7].fvalue < 0.68321937)) {
          if ( (data[6].fvalue < 1.9403253)) {
            if ( (data[5].fvalue < 6.403656)) {
              sum += (float)-0.0032704712;
            } else {
              sum += (float)-0.014524783;
            }
          } else {
            sum += (float)0.010078901;
          }
        } else {
          sum += (float)0.020170331;
        }
      }
    } else {
      if ( (data[6].fvalue < 2.1287365)) {
        if ( (data[5].fvalue < 7.9421511)) {
          if ( (data[5].fvalue < 5.4246254)) {
            sum += (float)-0.008530248;
          } else {
            sum += (float)0.012026847;
          }
        } else {
          sum += (float)-0.035315573;
        }
      } else {
        if ( (data[6].fvalue < 2.5256376)) {
          if ( (data[6].fvalue < 2.2172909)) {
            if ( (data[6].fvalue < 2.1967778)) {
              sum += (float)0.017408323;
            } else {
              sum += (float)-0.022455247;
            }
          } else {
            if ( (data[5].fvalue < 26.495249)) {
              sum += (float)0.027373623;
            } else {
              sum += (float)0.0033458949;
            }
          }
        } else {
          if ( (data[6].fvalue < 3.1542645)) {
            if ( (data[5].fvalue < 12.288451)) {
              sum += (float)-0.023103302;
            } else {
              sum += (float)-0.0059706657;
            }
          } else {
            if ( (data[5].fvalue < 19.97455)) {
              sum += (float)0.037069317;
            } else {
              sum += (float)-0.0028230667;
            }
          }
        }
      }
    }
  }
  if ( (data[6].fvalue < 1.2015269)) {
    if ( (data[6].fvalue < 1.1901114)) {
      if ( (data[6].fvalue < 1.1896853)) {
        if ( (data[6].fvalue < 1.1461949)) {
          if ( (data[6].fvalue < 1.0978411)) {
            if ( (data[6].fvalue < 1.0933394)) {
              sum += (float)0.00015808156;
            } else {
              sum += (float)-0.037422743;
            }
          } else {
            if ( (data[5].fvalue < 4.8097682)) {
              sum += (float)0.030265737;
            } else {
              sum += (float)-0.005047048;
            }
          }
        } else {
          if ( (data[5].fvalue < 5.2946815)) {
            if ( (data[5].fvalue < 4.3433914)) {
              sum += (float)-0.0056505632;
            } else {
              sum += (float)-0.058428477;
            }
          } else {
            if ( (data[7].fvalue < 0.19643801)) {
              sum += (float)-0.026976405;
            } else {
              sum += (float)0.025564732;
            }
          }
        }
      } else {
        sum += (float)0.057180639;
      }
    } else {
      if ( (data[5].fvalue < 3.7921014)) {
        if ( (data[5].fvalue < 3.3182778)) {
          sum += (float)-0.016987724;
        } else {
          sum += (float)0.031573035;
        }
      } else {
        if ( (data[7].fvalue < 0.14906439)) {
          sum += (float)0.0078027966;
        } else {
          if ( (data[5].fvalue < 22.210419)) {
            if ( (data[6].fvalue < 1.1965703)) {
              sum += (float)-0.058712699;
            } else {
              sum += (float)-0.027761281;
            }
          } else {
            sum += (float)-0.0011070193;
          }
        }
      }
    }
  } else {
    if ( (data[6].fvalue < 1.2060061)) {
      if ( (data[5].fvalue < 8.5478439)) {
        sum += (float)0.059202507;
      } else {
        sum += (float)0.021066759;
      }
    } else {
      if ( (data[6].fvalue < 1.2152082)) {
        if ( (data[6].fvalue < 1.2076558)) {
          if ( (data[5].fvalue < 3.6789083)) {
            sum += (float)0.027060719;
          } else {
            sum += (float)-0.02309745;
          }
        } else {
          if ( (data[6].fvalue < 1.212278)) {
            sum += (float)-0.053930547;
          } else {
            sum += (float)-0.014161975;
          }
        }
      } else {
        if ( (data[6].fvalue < 1.2290506)) {
          if ( (data[2].fvalue < 0.5)) {
            if ( (data[5].fvalue < 8.6474485)) {
              sum += (float)0.058541063;
            } else {
              sum += (float)0.015838305;
            }
          } else {
            if ( (data[6].fvalue < 1.2261066)) {
              sum += (float)0.01402254;
            } else {
              sum += (float)-0.0080410857;
            }
          }
        } else {
          if ( (data[6].fvalue < 1.2328484)) {
            if ( (data[6].fvalue < 1.2309155)) {
              sum += (float)0.0059884316;
            } else {
              sum += (float)-0.053916942;
            }
          } else {
            if ( (data[6].fvalue < 1.2379389)) {
              sum += (float)0.066507779;
            } else {
              sum += (float)0.0023610704;
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
