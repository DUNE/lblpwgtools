#include "TFile.h"

void ValidateRootFile(char* fIn) {
  TFile file(fIn);
  
  if(file.IsZombie() || file.TestBit(TFile::kRecovered)) {
    std::cout << fIn << " FILE TEST: File is corrupted in some manner. Do not use!" << std::endl;
    exit(0);
  }
  else {
    std::cout << fIn << " FILE TEST: Appears to be fine.  More testing may be required though." << std::endl;
    exit(1);
  }

}
