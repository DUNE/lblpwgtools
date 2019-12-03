#ifndef BDTREWEIGHTER_H
#define BDTREWEIGHTER_H

class BDTReweighter {

 protected :
  float multiplier;

 public :
  union BDTReweighterFeature {
    int missing;
    float fvalue;
    int qvalue;
  };
  

  
  virtual size_t get_num_output_group(void) = 0;
  virtual size_t get_num_feature(void) = 0;
  
  virtual float predict(union BDTReweighter::BDTReweighterFeature* data, int pred_margin) = 0;

  float GetWeight(union BDTReweighter::BDTReweighterFeature* data, int pred_margin){ return multiplier*predict(data, pred_margin); }

 BDTReweighter() : multiplier (1.) {};
 BDTReweighter(float mult) : multiplier (mult) {};
  ~BDTReweighter(){};
  
};

#endif
