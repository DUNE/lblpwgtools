////////////////////////////////////////////////////////////////////////
/// \brief   This is a simple tree class
/// \author  jpaley@anl.gov
/// \date
////////////////////////////////////////////////////////////////////////
#ifndef NTREE_H
#define NTREE_H

#include <vector>
#include <iostream>

namespace util {

  template<class T> class NTree {

  public:
    NTree*             fParent;
    T                  fTree;
    std::vector<NTree> fChild;

    void Print();
  };

  template<class T> void NTree<T>::Print() {
    if (fChild.empty()) {
      std::cout << &fTree << " <- ";      
    }
    else {
      for (unsigned int i=0; i<fChild.size(); ++i)
	fChild[i].Print();
      std::cout << std::endl;
    }
  }
}


#endif
