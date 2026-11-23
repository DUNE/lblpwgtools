#include "geoEff.h"
// C++ includes
#include <iostream>
#include <vector>
#include <random>
#include <math.h>

// Eigen Library
#include <Eigen/Dense>

geoEff::geoEff(int seed, bool verbose){

  verbosity = verbose;

  if (verbosity) {
    std::cout << "geoEff constructor called" << std::endl;
  }

  if (verbosity) {
    std::cout << "geoEff set seed to " << seed << std::endl;
  }

  N_THROWS = 64*64;

  if (verbosity){
    std::cout << "Number of throws set to " << N_THROWS << std::endl;
  }

  prnGenerator = std::mt19937_64(seed);
  if (verbosity) {
    std::cout << "geoEff set random number generator to mt19937_64: seed= "<< seed << std::endl;
  }

  uniform = std::uniform_real_distribution<>(0., 1.);
  if (verbosity){
    std::cout << "geoEff set uniform distribution in [0, 1]" << std::endl;
  }

  translations[0].reserve(N_THROWS);
  translations[1].reserve(N_THROWS);
  translations[2].reserve(N_THROWS);
  rotations.reserve(N_THROWS);

  if (verbosity){
    std::cout << "geoEff allocated memory for transformation vectors" << std::endl;
  }

  vetoSize = std::vector<float>(1,30.); // {30} cm
  vetoEnergy = std::vector<float>(1,30.); // 30 {MeV}

  if (verbosity){
    std::cout << "geoEff set veto size to 30 cm and energy threshold to 30 MeV" << std::endl;
  }

  useFixedBeamDir = false;

  // Initialize to all dimensions randomized
  for (int dim = 0; dim < 3; dim++) randomizeVertex[dim] = true;
}

void geoEff::setNthrows(unsigned long n){
  N_THROWS = n;
  if (verbosity){
    std::cout << "geoEff set number of throws to " << N_THROWS << std::endl;
  }

  if (N_THROWS%64) std::cout << "geoEff warning: number of throws should be multiple of 64 for optimal use of output format."  << std::endl;
}

void geoEff::setVertex(float x, float y, float z){
  vertex[0] = x;
  vertex[1] = y;
  vertex[2] = z;
  if(verbosity){
    std::cout << "geoEff set vertex to " << vertex[0] << " "<< vertex[1] << " "<< vertex[2] << std::endl;
  }
}

void geoEff::setHitSegEdeps(std::vector<float> thishitSegEdeps){
  hitSegEdeps = thishitSegEdeps;
  if (verbosity) {
    std::cout << "geoEff setting hit segment energy deposits to ";
    for (unsigned int i = 0; i < hitSegEdeps.size(); i++) std::cout << hitSegEdeps[i] << " ";
    std::cout << std::endl;
  }
}

void geoEff::setHitSegPoss(std::vector<float> thishitSegPoss){

  // Set the vector
  hitSegPoss = thishitSegPoss;
  if (verbosity) {
    std::cout << "geoEff setting hit segment positions to ";
    for (unsigned int i = 0; i < hitSegPoss.size(); i++) std::cout << hitSegPoss[i] << " ";
    std::cout << std::endl;
  }

}

void geoEff::setRangeX(float xmin, float xmax){
  range[0][0] = xmin;
  range[0][1] = xmax;
}
void geoEff::setRangeY(float ymin, float ymax){
  range[1][0] = ymin;
  range[1][1] = ymax;
}
void geoEff::setRangeZ(float zmin, float zmax){
  range[2][0] = zmin;
  range[2][1] = zmax;
}

void geoEff::setRandomizeX(bool r){
  randomizeVertex[0] = r;
}

void geoEff::setRandomizeY(bool r){
  randomizeVertex[1] = r;
}

void geoEff::setRandomizeZ(bool r){
  randomizeVertex[2] = r;
}

void geoEff::setActiveX(float xmin, float xmax){
  active[0][0] = xmin;
  active[0][1] = xmax;
}
void geoEff::setActiveY(float ymin, float ymax){
  active[1][0] = ymin;
  active[1][1] = ymax;
}
void geoEff::setActiveZ(float zmin, float zmax){
  active[2][0] = zmin;
  active[2][1] = zmax;
}

void geoEff::setOffsetX(float x){
  offset[0] = x;
}

void geoEff::setOffsetY(float y){
  offset[1] = y;
}

void geoEff::setOffsetZ(float z){
  offset[2] = z;
}



void geoEff::setBeamDir(float xdir, float ydir, float zdir){
  beamdir[0] = xdir;
  beamdir[1] = ydir;
  beamdir[2] = zdir;
}

void geoEff::setDecayPos(float x, float y, float z){
  decaypos[0] = x;
  decaypos[1] = y;
  decaypos[2] = z;
}

float geoEff::getDecayPos(int dim)
{
  return decaypos[dim];
}

void geoEff::setUseFixedBeamDir(bool use){
  useFixedBeamDir = use;
}

void geoEff::setVetoSizes(std::vector< float > vSizes){
  vetoSize = vSizes;
  if(verbosity){
    std::cout << "geoEff set veto sizes to ";
    for (unsigned int i = 0; i < vetoSize.size(); i++) std::cout << vetoSize[i] << " ";
  }
  std::cout << std::endl;
}
void geoEff::setVetoEnergyThresholds(std::vector< float > vThresholds){
  vetoEnergy = vThresholds;
  if(verbosity){
    std::cout << "geoEff set veto energy thresholds to ";
    for (unsigned int i = 0; i < vetoEnergy.size(); i++) std::cout << vetoEnergy[i] << " ";
  }
  std::cout << std::endl;
}

void geoEff::throwTransforms(){

  // Clear vectors
  translations[0].clear();
  translations[1].clear();
  translations[2].clear();
  rotations.clear();


  // dim from 0 to 2, corresponding to x, y and z
  for (int dim = 0; dim < 3; dim++){
    if (not randomizeVertex[dim]){
      translations[dim].resize(0,0);
    } else {
      translations[dim].clear();
      for (unsigned int i = 0; i < N_THROWS; i++){
        translations[dim].emplace_back(uniform(prnGenerator)*(range[dim][1]-range[dim][0])+range[dim][0]+offset[dim]);
      }
    }
  }

  rotations.clear();
  for (unsigned int i = 0; i < N_THROWS; i++){
    rotations.emplace_back((uniform(prnGenerator)-0.5)*2*M_PI);
  }

}

std::vector< Eigen::Transform<float,3,Eigen::Affine> > geoEff::getTransforms(unsigned int iStart, int iEnd){

  unsigned int thisEnd;
  if (iEnd < 0) thisEnd = N_THROWS;
  else if (iEnd >= 0){
    thisEnd = iEnd;
    if (thisEnd > N_THROWS) thisEnd = N_THROWS;
  }

  std::vector< Eigen::Transform<float,3,Eigen::Affine> > transforms;

  // Tranformations that do not depend on the throws:
  // Move vertex to coordinate system origin to apply rotation
  Eigen::Affine3f tThere(Eigen::Translation3f(Eigen::Vector3f(-vertex[0], -vertex[1], -vertex[2])));
  // Move vertex back
  Eigen::Affine3f tBack(Eigen::Translation3f(Eigen::Vector3f(vertex[0], vertex[1], vertex[2])));

  for (unsigned int iThrow = iStart; iThrow < thisEnd; iThrow++){

    // Vertex displacement:
    Eigen::Affine3f tThrow(Eigen::Translation3f(Eigen::Vector3f(randomizeVertex[0] ? translations[0][iThrow]-vertex[0] : 0.,
								randomizeVertex[1] ? translations[1][iThrow]-vertex[1] : 0.,
								randomizeVertex[2] ? translations[2][iThrow]-vertex[2] : 0.)));

    // Rotation
    Eigen::Affine3f rThrow;
    if (useFixedBeamDir){
      rThrow = Eigen::Affine3f(Eigen::AngleAxisf(rotations[iThrow], Eigen::Vector3f(beamdir[0], beamdir[1], beamdir[2])));
    } else {
      // Calculate rotation due to translation
      // Calculate rotation angle
      double decayToVertex[3] = {0};
      double decayToTranslated[3] = {0};
      double translationAngle = 0, magDecayToVertex = 0, magDecayToTranslated = 0;
      for (int dim = 0; dim < 3; dim++) {
        decayToVertex[dim] = vertex[dim]-decaypos[dim];
        decayToTranslated[dim] = randomizeVertex[dim] ? translations[dim][iThrow]-decaypos[dim] : vertex[dim]-decaypos[dim];

        translationAngle += (decayToVertex[dim])*(decayToTranslated[dim]);
        magDecayToVertex += pow(decayToVertex[dim], 2);
        magDecayToTranslated += pow(decayToTranslated[dim], 2);
      }
      magDecayToVertex = sqrt(magDecayToVertex);
      magDecayToTranslated = sqrt(magDecayToTranslated);
      translationAngle /= (magDecayToVertex*magDecayToTranslated);
      translationAngle = acos(translationAngle);

      // Calculate rotation axis
      // Cross-product
      float translationAxis[3] = {0};
      translationAxis[0] = decayToVertex[1]*decayToTranslated[2] - decayToVertex[2]*decayToTranslated[1];
      translationAxis[1] = decayToVertex[2]*decayToTranslated[0] - decayToVertex[0]*decayToTranslated[2];
      translationAxis[2] = decayToVertex[0]*decayToTranslated[1] - decayToVertex[1]*decayToTranslated[0];
      float magTranslationAxis = 0.;
      for (int dim = 0; dim < 3; dim++) magTranslationAxis += pow(translationAxis[dim], 2);
      magTranslationAxis = sqrt(magTranslationAxis);

      if(magTranslationAxis!=0)  {for (int dim = 0; dim < 3; dim++) translationAxis[dim] /= magTranslationAxis;}
      else{for (int dim = 0; dim < 3; dim++) {translationAxis[dim] = 0.;translationAngle =0.;}}
      Eigen::Affine3f rTranslation(Eigen::Affine3f(Eigen::AngleAxisf(translationAngle, Eigen::Vector3f(translationAxis[0], translationAxis[1], translationAxis[2]))));

      // Calculate rotation due to thrown angle
      Eigen::Affine3f rPhiThrow(Eigen::Affine3f(Eigen::AngleAxisf(rotations[iThrow], Eigen::Vector3f(decayToTranslated[0]/magDecayToTranslated, decayToTranslated[1]/magDecayToTranslated, decayToTranslated[2]/magDecayToTranslated))));

      // Combine
      rThrow = rPhiThrow * rTranslation;
    }

    // Put everything together in single transform and store.
    transforms.emplace_back(tThrow * tBack * rThrow * tThere);
  }

  return transforms;
}

std::vector<float> geoEff::getCurrentThrowTranslationsX(){
  return translations[0];
}
std::vector<float> geoEff::getCurrentThrowTranslationsY(){
  return translations[1];
}
std::vector<float> geoEff::getCurrentThrowTranslationsZ(){
  return translations[2];
}
std::vector<float> geoEff::getCurrentThrowRotations(){
  return rotations;
}


// Get the coordinates of hadron hits after eigen transformation, i is the # of throw
std::vector< float > geoEff::getCurrentThrowDeps(int i, int dim){

  // Set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > hitSegPosOrig(hitSegPoss.data(),3,hitSegPoss.size()/3,Eigen::OuterStride<>(3));

  Eigen::Matrix3Xf transformedEdeps = getTransforms(i, i+1)[0] * hitSegPosOrig;

  int nEdeps = hitSegEdeps.size();

  std::vector< float > ret(nEdeps);

  for (int iDep = 0; iDep < nEdeps; iDep++){
    ret[iDep] = transformedEdeps(dim, iDep);
  }

  return ret;
}

std::vector< float > geoEff::getCurrentThrowDepsX(int i){
  return getCurrentThrowDeps(i, 0);
}
std::vector< float > geoEff::getCurrentThrowDepsY(int i){
  return getCurrentThrowDeps(i, 1);
}
std::vector< float > geoEff::getCurrentThrowDepsZ(int i){
  return getCurrentThrowDeps(i, 2);
}


std::vector< std::vector< std::vector< uint64_t > > > geoEff::getHadronContainmentThrows(bool ignore_uncontained){

  // Figure out how many multiples of 64 bits needed to store output
  int n_longs = N_THROWS / 64;
  if (N_THROWS % 64) n_longs++;

  // Pass/fail for each set of vetoSize and vetoEnergy
  std::vector< std::vector< std::vector< uint64_t > > > hadronContainment(vetoSize.size(), std::vector< std::vector< uint64_t > >(vetoEnergy.size(), std::vector < uint64_t >(n_longs, 0)));

  // Set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > hitSegPosOrig(hitSegPoss.data(),3,hitSegPoss.size()/3,Eigen::OuterStride<>(3));

  // Check if event is contained by any of the existing conditions
  if (ignore_uncontained) {
    int origContained = 0;
    std::vector< std::vector< bool > > vecOrigContained = getHadronContainmentOrigin();
    for (unsigned int i = 0; i < vetoSize.size(); i++){
      for (unsigned int j = 0; j < vetoEnergy.size(); j++){
	if (vecOrigContained[i][j]) origContained++;
      }
    }

    // If not, then return
    if (origContained == 0) return hadronContainment;
  }

  std::vector< Eigen::Transform<float,3,Eigen::Affine> > transforms = getTransforms();
  // Else, loop through set of rotation translations
  for (unsigned int t = 0; t < N_THROWS; t++){
    // Apply transformation to energy deposit positions
    Eigen::Matrix3Xf transformedEdeps = transforms[t] * hitSegPosOrig;
    // Loop through conditions
    for (unsigned int i = 0; i < vetoSize.size(); i++){
      for (unsigned int j = 0; j < vetoEnergy.size(); j++){
        // Check containment and set bit
        if (isContained(transformedEdeps, hitSegEdeps, vetoSize[i], vetoEnergy[j]))
        {
	         hadronContainment[i][j][t/64] |= ((uint64_t)1)<<(t%64);
	      }
      }
    }
  }

  return hadronContainment;
}
// getHadronContainmentThrows for FD GEC
std::vector< std::vector< std::vector< uint64_t > > > geoEff::getHadronContainmentThrows_FD_GEC(bool ignore_uncontained){

  // Figure out how many multiples of 64 bits needed to store output
  int n_longs = N_THROWS / 64;
  if (N_THROWS % 64) n_longs++;

  // Pass/fail for each set of vetoSize and vetoEnergy
  std::vector< std::vector< std::vector< uint64_t > > > hadronContainment(vetoSize.size(), std::vector< std::vector< uint64_t > >(vetoEnergy.size(), std::vector < uint64_t >(n_longs, 0)));

  // Set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > hitSegPosOrig(hitSegPoss.data(),3,hitSegPoss.size()/3,Eigen::OuterStride<>(3));

  // Check if event is contained by any of the existing conditions
  if (ignore_uncontained) {
    int origContained = 0;
    std::vector< std::vector< bool > > vecOrigContained = getHadronContainmentOrigin_FD_GEC();
    for (unsigned int i = 0; i < vetoSize.size(); i++){
      for (unsigned int j = 0; j < vetoEnergy.size(); j++){
	        if (vecOrigContained[i][j]) origContained++;
      }
    }

    // If not, then return
    if (origContained == 0) return hadronContainment;
  }

  std::vector< Eigen::Transform<float,3,Eigen::Affine> > transforms = getTransforms();
  // Else, loop through set of rotation translations
  for (unsigned int t = 0; t < N_THROWS; t++){
    // Apply transformation to energy deposit positions
    Eigen::Matrix3Xf transformedEdeps = transforms[t] * hitSegPosOrig;
    // Loop through conditions
    for (unsigned int i = 0; i < vetoSize.size(); i++){
      for (unsigned int j = 0; j < vetoEnergy.size(); j++){
        // Check containment and set bit
        if (isContained_FD_GEC(transformedEdeps, hitSegEdeps, vetoSize[i], vetoEnergy[j]))
        {
	         hadronContainment[i][j][t/64] |= ((uint64_t)1)<<(t%64);
                 //std::cout<<" contained"<<std::endl; 
        }
        //std::cout<<"Throw: "<<t/64<<" i: "<<i<<" j: "<<j<<" hadron containment: "<<hadronContainment[i][j][t/64] <<"veto Energy hadron thrrow fct: "<<getVetoE(transformedEdeps, hitSegEdeps, vetoSize[i])<<std::endl;
      }
    }
  }

  return hadronContainment;
}

//try to make function that returns vetoE of each throw in getHadronContainmentThrows_FD_GEC

std::vector< std::vector< std::vector< float > > > geoEff::getVetoEPerThrow_FD_GEC(){//   getVetoEPerThrow_FD_GEC(std::vector< std::vector< std::vector< uint64_t > > > hadronThrowResult){

 // Figure out how many multiples of 64 bits needed to store output
  int n_longs = N_THROWS / 64;
  if (N_THROWS % 64) n_longs++;
  
  // Pass/fail for each set of vetoSize and vetoEnergy
  std::vector< std::vector< std::vector< uint64_t > > > hadronContainment(vetoSize.size(), std::vector< std::vector< uint64_t > >(vetoEnergy.size(), std::vector < uint64_t >(n_longs, 0)));
  
  //vector with veto E
  std::vector< std::vector< std::vector< float > > > vetoEnergyVector(vetoSize.size(), std::vector< std::vector< float > >(vetoEnergy.size(), std::vector < float >(4096, 0)));
  //Set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > hitSegPosOrig(hitSegPoss.data(),3,hitSegPoss.size()/3,Eigen::OuterStride<>(3));

  std::vector< Eigen::Transform<float,3,Eigen::Affine> > transforms = getTransforms();
  //loop through set of rotation translations
    for (unsigned int t = 0; t < N_THROWS; t++){
    // Apply transformation to energy deposit positions
    Eigen::Matrix3Xf transformedEdeps = transforms[t] * hitSegPosOrig;
    // Loop through conditions
    for (unsigned int i = 0; i < vetoSize.size(); i++){
      for (unsigned int j = 0; j < vetoEnergy.size(); j++){
          vetoEnergyVector[i][j][t] = getVetoE(transformedEdeps, hitSegEdeps, vetoSize[i]);
        // Check containment and set bit
        //std::cout<<" veto E before contained cut: "<<getVetoE(transformedEdeps, hitSegEdeps, vetoSize[i])<<std::endl;
        /*if (isContained_FD_GEC(transformedEdeps, hitSegEdeps, vetoSize[i], vetoEnergy[j]))
        {
	         hadronContainment[i][j][t/64] |= ((uint64_t)1)<<(t%64);
		 //std::cout<<" had containment: "<< hadronContainment[i][j][t/64]<<" hadthrow: "<<hadronThrowResult[i][j][t/64]<<" vetoE without equal throw:"<<getVetoE(transformedEdeps, hitSegEdeps, vetoSize[i])<< std::endl;
                 //ensure we have 1 to 1 correspondence in vetoE for throws
                 //if(hadronContainment[i][j][t/64] == hadronThrowResult[i][j][t/64]){
                    //std::cout<<" same throws can now save the veto E"<<std::endl;
                    //vetoEnergyVector[i][j][t/64] = 5;
                    vetoEnergyVector[i][j][t/64] = getVetoE(transformedEdeps, hitSegEdeps, vetoSize[i]);
                    //std::cout<<" veto E: "<<getVetoE(transformedEdeps, hitSegEdeps, vetoSize[i])<<std::endl;              
                 //}
	}*/
      }
    }
  }
  return vetoEnergyVector;
}

//function that returns trim E for each throws
std::vector< std::vector< std::vector< float > > > geoEff::getTrimEPerThrow_FD_GEC(){
  
  //vector with Trim E
  std::vector< std::vector< std::vector< float > > > TrimEnergyVector(vetoSize.size(), std::vector< std::vector< float > >(vetoEnergy.size(), std::vector < float >(4096, 0)));
  //set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > hitSegPosOrig(hitSegPoss.data(),3,hitSegPoss.size()/3,Eigen::OuterStride<>(3));
  
  std::vector< Eigen::Transform<float,3,Eigen::Affine> > transforms = getTransforms();
  //loop through set of rotation translations
  for (unsigned int t = 0; t < N_THROWS; t++){
    // Apply transformation to energy deposit positions
    Eigen::Matrix3Xf transformedEdeps = transforms[t] * hitSegPosOrig;
    for (unsigned int i = 0; i < vetoSize.size(); i++){
       for (unsigned int j = 0; j < vetoEnergy.size(); j++){
           TrimEnergyVector[i][j][t] = getTrimE(transformedEdeps, hitSegEdeps);
       }
    }
  }
 
  return TrimEnergyVector;
}
  

void geoEff::setSeed(int seed){
  prnGenerator = std::mt19937_64(seed);
}

std::vector< std::vector< bool > > geoEff::getHadronContainmentOrigin(){
  // Initialize return vector
  std::vector< std::vector< bool > > hadronContainment(vetoSize.size(), std::vector< bool >(vetoEnergy.size(), false));

  // Set Eigen Map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > hitSegPosOrig(hitSegPoss.data(),3,hitSegPoss.size()/3,Eigen::OuterStride<>(3));

  for (unsigned int i = 0; i < vetoSize.size(); i++){
    for (unsigned int j = 0; j < vetoEnergy.size(); j++){
      if (isContained(hitSegPosOrig, hitSegEdeps, vetoSize[i], vetoEnergy[j])) hadronContainment[i][j] = true;
    }
  }



  return hadronContainment;
}
// getHadronContainmentOrigin for FD GEC
std::vector< std::vector< bool > > geoEff::getHadronContainmentOrigin_FD_GEC(){
  // Initialize return vector
  std::vector< std::vector< bool > > hadronContainment(vetoSize.size(), std::vector< bool >(vetoEnergy.size(), false));

  // Set Eigen Map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > hitSegPosOrig(hitSegPoss.data(),3,hitSegPoss.size()/3,Eigen::OuterStride<>(3));

  for (unsigned int i = 0; i < vetoSize.size(); i++){
    for (unsigned int j = 0; j < vetoEnergy.size(); j++){
      if (isContained_FD_GEC(hitSegPosOrig, hitSegEdeps, vetoSize[i], vetoEnergy[j])) hadronContainment[i][j] = true;
    }
  }

  return hadronContainment;
}

void geoEff::setOffAxisOffsetX(float x){
  OffAxisOffset[0] = x;
}

void geoEff::setOffAxisOffsetY(float y){
  OffAxisOffset[1] = y;
}

void geoEff::setOffAxisOffsetZ(float z){
  OffAxisOffset[2] = z;
}

bool geoEff::isContained( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits, float vSize, float vetoEnergyThreshold ){

  float vetoEnergy = 0.;

  for (unsigned int i = 0; i < energyDeposits.size(); i++){
    for (int dim = 0; dim < 3; dim++){
      // low
      if ( (hitSegments(dim, i)-offset[dim] < active[dim][0]+vSize) and
           (hitSegments(dim, i)-offset[dim] > active[dim][0]) ) {
        vetoEnergy += energyDeposits[i];
        break; // Only count each energy deposit once
      }
      // high
      if ( (hitSegments(dim, i)-offset[dim] > active[dim][1]-vSize) and
           (hitSegments(dim, i)-offset[dim] < active[dim][1]) ) {
        vetoEnergy += energyDeposits[i];
        break; // Only count each energy deposit once
      }
    }
  }

  return vetoEnergy < vetoEnergyThreshold;
}

// isContainde for FD GEC
bool geoEff::isContained_FD_GEC( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits, float vSize, float vetoEnergyThreshold ){

  float vetoEnergy = 0.;

  for (unsigned int i = 0; i < energyDeposits.size(); i++){
    for (int dim = 0; dim < 3; dim++){
      // low
      if ( (hitSegments(dim, i)-OffAxisOffset[dim] < active[dim][0]+vSize) and
           (hitSegments(dim, i)-OffAxisOffset[dim] > active[dim][0]) ) {
        vetoEnergy += energyDeposits[i];
        break; // Only count each energy deposit once
      }
      // high
      if ( (hitSegments(dim, i)-OffAxisOffset[dim] > active[dim][1]-vSize) and
           (hitSegments(dim, i)-OffAxisOffset[dim] < active[dim][1]) ) {
        vetoEnergy += energyDeposits[i];
        break; // Only count each energy deposit once
      }
    }
  }

  return vetoEnergy < vetoEnergyThreshold;
}

// Get veto E
float geoEff::getVetoE( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits, float vSize ){

  float vetoEnergy = 0.;

  for (unsigned int i = 0; i < energyDeposits.size(); i++){
    for (int dim = 0; dim < 3; dim++){
      // low
      if ( (hitSegments(dim, i)-OffAxisOffset[dim] < active[dim][0]+vSize) and
           (hitSegments(dim, i)-OffAxisOffset[dim] > active[dim][0]) ) {
        vetoEnergy += energyDeposits[i];
        break; // Only count each energy deposit once
      }
      // high
      if ( (hitSegments(dim, i)-OffAxisOffset[dim] > active[dim][1]-vSize) and
           (hitSegments(dim, i)-OffAxisOffset[dim] < active[dim][1]) ) {
        vetoEnergy += energyDeposits[i];
        break; // Only count each energy deposit once
      }
    }
  }
  return vetoEnergy ;
}

//Get Trim E
float geoEff::getTrimE( Eigen::Matrix3Xf hitSegments, std::vector<float> energyDeposits ){
   
  //Trim E = totEnergy - outEnergy
  float outEnergy = 0.;
  float totEnergy = 0.;
  for (unsigned int i = 0; i < energyDeposits.size(); i++){
    totEnergy += energyDeposits[i];
    for (int dim = 0; dim < 3; dim++){
      //low
      if( (hitSegments(dim, i)-OffAxisOffset[dim] < active[dim][0])  ){
        outEnergy += energyDeposits[i];
        break;
      }
      //high
      if( (hitSegments(dim, i)-OffAxisOffset[dim] > active[dim][1])  ){
        outEnergy += energyDeposits[i];
        break;
      }

    }
  }
  float trimEnergy = totEnergy - outEnergy;
  //std::cout<<" trimEnergy = "<<trimEnergy<<" totEnergy: "<<totEnergy<<" out E: "<<outEnergy<<std::endl;
  return trimEnergy;
}

// Get TOTAL E
float geoEff::getTotE(std::vector<float> energyDeposits){

  float totEnergy = 0.;

  for (unsigned int i = 0; i < energyDeposits.size(); i++){
    totEnergy += energyDeposits[i];
  }
  return totEnergy ;
}
// Get current throw veto E, t is the # of throw
float geoEff::getCurrentThrowsTotE(){
  float totEnergy = 0.;
  totEnergy = getTotE(hitSegEdeps);
  return totEnergy;
}


//
// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################
// #########################################################################
// Translation from On-Axis position to Off-Axis positions
void geoEff::setOnAxisVertex(float x, float y, float z){
  OnAxisVertex[0] = x;
  OnAxisVertex[1] = y;
  OnAxisVertex[2] = z;
  if(verbosity){
    std::cout << "geoEff set On-Axis vertex to " << OnAxisVertex[0] << " "<< OnAxisVertex[1] << " "<< OnAxisVertex[2] << std::endl;
  }
}
// Vertex before rotations
void geoEff::setOffAxisVertex(float x, float y, float z){
  OffAxisVertex[0] = x;
  OffAxisVertex[1] = y;
  OffAxisVertex[2] = z;
}


// Position vector space Eigen transformation
std::vector< Eigen::Transform<float,3,Eigen::Affine> > geoEff::getTransforms_NDtoND(){

  std::vector< Eigen::Transform<float,3,Eigen::Affine> > transforms_NDtoND;

  // Tranformations that do not depend on the throws:
  // Move vertex to coordinate system origin to apply rotation
  Eigen::Affine3f tThere_NDtoND(Eigen::Translation3f(Eigen::Vector3f(-OffAxisVertex[0], -OffAxisVertex[1], -OffAxisVertex[2])));
  // Move vertex back
  Eigen::Affine3f tBack_NDtoND(Eigen::Translation3f(Eigen::Vector3f(OffAxisVertex[0], OffAxisVertex[1], OffAxisVertex[2])));
  // Eigen::Affine3f is a typedef of Eigen::Transform<float, 3, Eigen::Affine>

    // Vertex displacement:
    // Eigen::Affine3f tThrow_NDtoND(Eigen::Translation3f(Eigen::Vector3f(OffAxisVertex[0]-OnAxisVertex[0],OffAxisVertex[1]-OnAxisVertex[1],OffAxisVertex[2]-OnAxisVertex[2])));

    // Rotation
    Eigen::Affine3f rThrow_NDtoND;
    {
      // Calculate rotation due to translation
      // Calculate rotation angle
      double decayToVertex[3] = {0};
      double decayToTranslated[3] = {0};
      double translationAngle = 0., magDecayToVertex = 0., magDecayToTranslated = 0.; // Use double in case translationAngle is so tiny then gives nan results
      for (int dim = 0; dim < 3; dim++) {
        decayToVertex[dim] = OnAxisVertex[dim]-decaypos[dim];
        decayToTranslated[dim] = OffAxisVertex[dim]-decaypos[dim];

        translationAngle += (decayToVertex[dim])*(decayToTranslated[dim]);
        magDecayToVertex += pow(decayToVertex[dim], 2);
        magDecayToTranslated += pow(decayToTranslated[dim], 2);
      }
      magDecayToVertex = sqrt(magDecayToVertex);
      magDecayToTranslated = sqrt(magDecayToTranslated);
      translationAngle /= (magDecayToVertex*magDecayToTranslated);
      translationAngle = acos(translationAngle);

      // Calculate rotation axis
      // Cross-product
      float translationAxis[3] = {0};
      translationAxis[0] = decayToVertex[1]*decayToTranslated[2] - decayToVertex[2]*decayToTranslated[1];
      translationAxis[1] = decayToVertex[2]*decayToTranslated[0] - decayToVertex[0]*decayToTranslated[2];
      translationAxis[2] = decayToVertex[0]*decayToTranslated[1] - decayToVertex[1]*decayToTranslated[0];
      float magTranslationAxis = 0.;
      for (int dim = 0; dim < 3; dim++) magTranslationAxis += pow(translationAxis[dim], 2);
      magTranslationAxis = sqrt(magTranslationAxis);
      // Get rotation axis n_{hat}
      if(magTranslationAxis!=0)  {for (int dim = 0; dim < 3; dim++) translationAxis[dim] /= magTranslationAxis;}
      else{for (int dim = 0; dim < 3; dim++) {translationAxis[dim] = 0.;translationAngle =0.;}}
      Eigen::Affine3f rTranslation_NDtoND(Eigen::Affine3f(Eigen::AngleAxisf(translationAngle, Eigen::Vector3f(translationAxis[0], translationAxis[1], translationAxis[2]))));

      // Combine: we only have translation when moving ND onaxis to offaxis
      rThrow_NDtoND = rTranslation_NDtoND;

      if(verbosity)
      {
        std::cout << "translationAngle: " << translationAngle << std::endl;
        std::cout << "translationAxis: " << translationAxis[0] << ", " << translationAxis[1] << ", " << translationAxis[2] << "\n";
      }
    }

    // Put everything together in single transform and store.
    // transforms_NDtoND.emplace_back(tThrow_NDtoND * tBack_NDtoND * rThrow_NDtoND * tThere_NDtoND);
    transforms_NDtoND.emplace_back(tBack_NDtoND * rThrow_NDtoND * tThere_NDtoND );

    /*
    I want to apply a rotation to an event and then move it to a different place.
    First I move the event vertex to the origin of the coordinate system with tThere.
    Then I apply the rotation, rThrow. Since I moved the event vertex to the origin, I know the rotation will not move the vertex, as intended.
    Then, move the vertex back to its original position, tBack.
    And finally, move it to the new position with tThrow
    */

    return transforms_NDtoND;
    // returen value: 1D vector
}

// Set Sim_mu_end_vertex
void geoEff::setMuEndV(float x, float y, float z){
  OffAxisMuEndV_BF.resize(3);
  OffAxisMuEndV_BF.at(0)=x;
  OffAxisMuEndV_BF.at(1)=y;
  OffAxisMuEndV_BF.at(2)=z;
}

// Get Sim_mu_end_vertex after rotations
float geoEff::getOffAxisMuEndV(int dim){

  // Set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > VectorCoordinate(OffAxisMuEndV_BF.data(),3,OffAxisMuEndV_BF.size()/3,Eigen::OuterStride<>(3));
  // Get the rotated vector coordinate, a 3*1 matrix
  Eigen::Matrix3Xf RotMuEndV_AF = getTransforms_NDtoND()[0] * VectorCoordinate;
  // Return the results for (x,y,z)<->dim=(0,1,2)
  return RotMuEndV_AF(dim, 0);
}

// Set ND_Sim_mu_hadronic_hit
void geoEff::setHadronHitV(float x, float y, float z){
  OffAxisHadronHitV_BF.resize(3);
  OffAxisHadronHitV_BF.at(0)=x;
  OffAxisHadronHitV_BF.at(1)=y;
  OffAxisHadronHitV_BF.at(2)=z;
}

// Get Sim_mu_end_vertex after rotations
float geoEff::getOffAxisHadronHitV(int dim){

  // Set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > VectorCoordinate(OffAxisHadronHitV_BF.data(),3,OffAxisHadronHitV_BF.size()/3,Eigen::OuterStride<>(3));
  // Get the rotated vector coordinate, a 3*1 matrix
  Eigen::Matrix3Xf OffAxisHadronHitV_AF = getTransforms_NDtoND()[0] * VectorCoordinate;
  // Return the results for (x,y,z)<->dim=(0,1,2)
  return OffAxisHadronHitV_AF(dim, 0);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Momentum vector space Eigen transformation
std::vector< Eigen::Transform<float,3,Eigen::Affine> > geoEff::getTransforms_NDtoND_P(){

  std::vector< Eigen::Transform<float,3,Eigen::Affine> > transforms_NDtoND_P;

    // Rotation
    Eigen::Affine3f rThrow_NDtoND_P;
    {
      // Calculate rotation due to translation
      // Calculate rotation angle
      double decayToVertex[3] = {0};
      double decayToTranslated[3] = {0};
      double translationAngle = 0, magDecayToVertex = 0, magDecayToTranslated = 0;
      for (int dim = 0; dim < 3; dim++) {
        decayToVertex[dim] = OnAxisVertex[dim]-decaypos[dim];
        decayToTranslated[dim] = OffAxisVertex[dim]-decaypos[dim];

        translationAngle += (decayToVertex[dim])*(decayToTranslated[dim]);
        magDecayToVertex += pow(decayToVertex[dim], 2);
        magDecayToTranslated += pow(decayToTranslated[dim], 2);
      }
      magDecayToVertex = sqrt(magDecayToVertex);
      magDecayToTranslated = sqrt(magDecayToTranslated);
      translationAngle /= (magDecayToVertex*magDecayToTranslated);
      translationAngle = acos(translationAngle);

      // Calculate rotation axis
      // Cross-product
      float translationAxis[3] = {0};
      translationAxis[0] = decayToVertex[1]*decayToTranslated[2] - decayToVertex[2]*decayToTranslated[1];
      translationAxis[1] = decayToVertex[2]*decayToTranslated[0] - decayToVertex[0]*decayToTranslated[2];
      translationAxis[2] = decayToVertex[0]*decayToTranslated[1] - decayToVertex[1]*decayToTranslated[0];
      float magTranslationAxis = 0.;
      for (int dim = 0; dim < 3; dim++) magTranslationAxis += pow(translationAxis[dim], 2);
      magTranslationAxis = sqrt(magTranslationAxis);
      // Get rotation axis n_{hat}
      if(magTranslationAxis!=0)  {for (int dim = 0; dim < 3; dim++) translationAxis[dim] /= magTranslationAxis;}
      else{for (int dim = 0; dim < 3; dim++) {translationAxis[dim] = 0.;translationAngle =0.;}}


      Eigen::Affine3f rTranslation_NDtoND_P(Eigen::Affine3f(Eigen::AngleAxisf(translationAngle, Eigen::Vector3f(translationAxis[0], translationAxis[1], translationAxis[2]))));

      rThrow_NDtoND_P = rTranslation_NDtoND_P;
    }

    // Put everything together in single transform and store.
    // Momentum space is different to the position space, it is not relevant to the position of a vector, so we just need rThrow.
    transforms_NDtoND_P.emplace_back(rThrow_NDtoND_P);

    return transforms_NDtoND_P;
    // returen value: 1D vector
}


// Set ND_Sim_mu_start_p
void geoEff::setMuStartP(float x, float y, float z){
  OffAxisMuStartP_BF.resize(3);
  OffAxisMuStartP_BF.at(0)=x;
  OffAxisMuStartP_BF.at(1)=y;
  OffAxisMuStartP_BF.at(2)=z;
}

// Get Sim_mu_end_vertex after rotations
float geoEff::getOffAxisMuStartP(int dim){

  // Set the Eigen map
  Eigen::Map<Eigen::Matrix3Xf,0,Eigen::OuterStride<> > VectorCoordinate(OffAxisMuStartP_BF.data(),3,OffAxisMuStartP_BF.size()/3,Eigen::OuterStride<>(3));
  // Get the rotated vector coordinate
  Eigen::Matrix3Xf RotMuStartP_AF = getTransforms_NDtoND_P()[0] * VectorCoordinate;

  return RotMuStartP_AF(dim, 0);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Vector doesn't change
double geoEff::RemainUnchanged(double element)
{
  return element;
}
// Momentum calculations
float geoEff::getTotalMomentum(double momentum[3])
{
  float TotalP = sqrt(pow(momentum[0],2)+pow(momentum[1],2)+pow(momentum[2],2));
  return TotalP;
}
double geoEff::getDistance(double v1[3],double v2[3])
{
  double distance = sqrt(pow(v1[0]-v2[0],2)+pow(v1[1]-v2[1],2)+pow(v1[2]-v2[2],2));
  return distance;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Earth curvature rotations
// Local y-z axes in FD and ND are rotated due to Earth curvature, x direction is not change
// FD event coordinates, if unchanged, would represent different event in ND coordinate sys.
// Apply an active transformation matrix R_x(theta): rotate each point counterclockwise by theta around x-axis in ND
// theta is 2*|beamLineRotation|
// Transform FD relative coordinate, x coordinate unchanged
//
//              [ 1          0             0
// R_x(theta) =   0      cos(theta)   -sin(theta)
//                0      sin(theta)    cos(theta) ]

double geoEff::getEarthCurvature(double v[3], double BeamAngle, int dim)
{
  double Vector_af[3];
  Vector_af[0]=v[0];
  Vector_af[1]=cos( 2*abs(BeamAngle) )*v[1] - sin( 2*abs(BeamAngle) )*v[2];
  Vector_af[2]=sin( 2*abs(BeamAngle) )*v[1] + cos( 2*abs(BeamAngle) )*v[2];
  return Vector_af[dim];
}
// Put events back to beam center
double geoEff::getTranslations(double v_bf[3], double vtx_bf[3], double vtx_af[3], int dim)
{
  double Vector_af[3];
  for(int i=0; i<3; i++)
  {
    Vector_af[i] = v_bf[i] + (vtx_af[i]-vtx_bf[i]);
  }
  return Vector_af[dim];
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Draw 2d graph for geoEff, x axis: ND_LAr_pos, y axis: ND_OffAxis_pos, z axis(colz): ND_OffAxis_eff
