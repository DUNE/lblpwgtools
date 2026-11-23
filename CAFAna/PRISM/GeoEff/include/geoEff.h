#ifndef GEOEFF_H
#define GEOEFF_H

#include <Python.h> // Need this before including std to avoid _POSIX_C_SOURCE redefinition warning

#include <vector>
#include <random>

#include <Eigen/Dense>

class geoEff
{

 private:

  // Event vertex
  float vertex[3];
  // New vector
  float OnAxisVertex[3];
  float OffAxisVertex[3];
  std::vector<float>  OffAxisMuEndV_BF;
  std::vector<float>  OffAxisMuStartP_BF; // Can only be float instead of double
  std::vector<float>  OffAxisHadronHitV_BF;

  // Vector to store energy deposits corresponding to hit segments
  std::vector<float> hitSegEdeps;
  // Vector to store positions of hit segments
  std::vector<float> hitSegPoss;

  // Range of coordinates to randomize over. Set negative for no randomization.
  float range[3][2];

  // If false, use vertex for this dimension in every throw: do not randomize
  bool randomizeVertex[3];

  // Active volume:
  float active[3][2];

  // Detector coordinates offset:
  float offset[3];
  float OffAxisOffset[3];

  // Beam direction:
  float beamdir[3];

  // Decay position used to calculate neutrino direction.
  float decaypos[3];

  // Flag to determine whether to use fixed beam direction (appropriate for FD) or calculate direction from vertex position and decay point.
  bool useFixedBeamDir;

  // Number of throws. Should be a multiple of 64 to optimize output efficiency
  unsigned long N_THROWS;

  // Veto size. Each element defines a veto region in cm from the end of the active volume in all directions.
  std::vector < float > vetoSize;
  // Veto energy. Each element defines a veto energy threshold.
  std::vector < float > vetoEnergy;

  // Current throws
  std::vector< float > translations[3];
  std::vector< float > rotations;

  bool verbosity;

  std::mt19937_64 prnGenerator;
  std::uniform_real_distribution<> uniform;

  bool isContained( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits, float vSize, float vetoEnergyThreshold );
  bool isContained_FD_GEC( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits, float vSize, float vetoEnergyThreshold );
  float getVetoE( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits, float vSize);
  float getTrimE( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits);
  float getTotE( std::vector<float> energyDeposits );

  // Calculate transforms for current vertex
  std::vector< Eigen::Transform<float,3,Eigen::Affine> > getTransforms(unsigned int iStart = 0, int iEnd = -1);
  // Position transforms from On-Axis to Off-Axis
  std::vector< Eigen::Transform<float,3,Eigen::Affine> > getTransforms_NDtoND();
  // Momentum transforms from On-Axis to Off-Axis
  std::vector< Eigen::Transform<float,3,Eigen::Affine> > getTransforms_NDtoND_P();

 public:
  geoEff(int seed, bool verbose = false);
  ~geoEff(){;}

  void setNthrows(unsigned long n);

  void setVertex(float x, float y, float z);
  void setHitSegEdeps(std::vector<float> thishitSegEdeps);
  void setHitSegPoss(std::vector<float> thishitSegPoss);
  // Transforms from On-Axis to Off-Axis
  void setOnAxisVertex(float x, float y, float z);
  void setOffAxisVertex(float x, float y, float z);
  void setMuEndV(float x, float y, float z);
  void setMuStartP(float x, float y, float z);
  void setHadronHitV(float x, float y, float z);

  void setRangeX(float xmin, float xmax);
  void setRangeY(float ymin, float ymax);
  void setRangeZ(float zmin, float zmax);

  void setRandomizeX(bool r);
  void setRandomizeY(bool r);
  void setRandomizeZ(bool r);

  void setActiveX(float xmin, float xmax);
  void setActiveY(float ymin, float ymax);
  void setActiveZ(float zmin, float zmax);

  void setOffsetX(float x);
  void setOffsetY(float y);
  void setOffsetZ(float z);
  void setOffAxisOffsetX(float x);
  void setOffAxisOffsetY(float y);
  void setOffAxisOffsetZ(float z);

  void setBeamDir(float xdir, float ydir, float zdir);
  void setDecayPos(float x, float y, float z);
  float getDecayPos(int dim);
  void setUseFixedBeamDir(bool use);

  void setVetoSizes(std::vector< float > vSizes);
  void setVetoEnergyThresholds(std::vector< float > vThresholds);

  void throwTransforms();

  std::vector< float > getCurrentThrowTranslationsX();
  std::vector< float > getCurrentThrowTranslationsY();
  std::vector< float > getCurrentThrowTranslationsZ();
  std::vector< float > getCurrentThrowRotations();

  std::vector< float > getCurrentThrowDeps(int i, int dim);
  std::vector< float > getCurrentThrowDepsX(int i);
  std::vector< float > getCurrentThrowDepsY(int i);
  std::vector< float > getCurrentThrowDepsZ(int i);

  // Transforms from On-Axis to Off-Axis
  float getOffAxisMuEndV(int dim);
  float getOffAxisMuStartP(int dim);
  float getOffAxisHadronHitV(int dim);

  // Unchanged case
  double RemainUnchanged(double element);
  // Total momentum calculations
  float getTotalMomentum(double momentum[3]);
  // Distance calculation
  double getDistance(double v1[3],double v2[3]);
  // Get earth curvature
  double getEarthCurvature(double v[3], double BeamAngle, int dim);
  // Get translations
  double getTranslations(double v_bf[3], double vtx_bf[3], double vtx_af[3], int dim);

  // Pass/fail for each set of vetoSize and vetoEnergy. Storing in TTree as uint64_t seems to take ~half the space of the equivalent vector< bool >.
  std::vector< std::vector< std::vector< uint64_t > > > getHadronContainmentThrows(bool ignore_uncontained);
  std::vector< std::vector< std::vector< uint64_t > > > getHadronContainmentThrows_FD_GEC(bool ignore_uncontained);

  //veto E for each HadronContainmentThrows_FD_GEC
  std::vector< std::vector< std::vector< float > > > getVetoEPerThrow_FD_GEC();//(std::vector< std::vector< std::vector< uint64_t > > > hadronThrowResult);
  //trim E for each HadronContainmentThrows_FD_GEC
  std::vector< std::vector< std::vector< float > > > getTrimEPerThrow_FD_GEC();

  // Get pass/fail containment criterion for original event
  std::vector< std::vector< bool > > getHadronContainmentOrigin();
  std::vector< std::vector< bool > > getHadronContainmentOrigin_FD_GEC();
  
  void setSeed(int seed);

  // get throwEnergy
  float getCurrentThrowsTotE();


};

#endif
