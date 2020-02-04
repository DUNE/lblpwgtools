#include <cassert>
#include <map>
#include <set>
#include <iostream>

#include "TClass.h"
#include "TFile.h"
#include "TH1.h"
#include "THnSparse.h"
#include "TObject.h"
#include "TObjString.h"
#include "TParameter.h"
#include "TVectorD.h"
#include "TVector3.h"

std::string ConcatPath(const std::string& a, const std::string& b)
{
  if(a.empty()) return b;
  return a+"/"+b;
}

std::string dirname(const std::string& x)
{
  const size_t slashLoc = x.rfind("/");
  if(slashLoc == std::string::npos) return "";
  return x.substr(0, slashLoc);
}

std::string basename(const std::string& x)
{
  return x.substr(x.rfind("/")+1);
}

std::map<std::string, TObject*> GetObjectMap(TDirectory* x,
					     std::string path = "")
{
  std::map<std::string, TObject*> ret;

  TList* keys = x->GetListOfKeys();

  // TODO is this bit from the python version necessary?
  //     # Use a set to collapse all the cycle numbers down into the same name
  //     for key in {k.GetName() for k in x.GetListOfKeys()}:

  TIter next(keys);
  while(TObject* key = next()){
    TObject* kid = x->Get(key->GetName());
    if(kid->InheritsFrom(TDirectoryFile::Class())){
      std::map<std::string, TObject*> kids = GetObjectMap((TDirectoryFile*)kid,
							  ConcatPath(path, kid->GetName()));
      delete kid;
      ret.insert(kids.begin(), kids.end());
    }
    else{
      if(kid->InheritsFrom(TH1::Class())){
	((TH1*)kid)->SetDirectory(0);
      }
      ret[ConcatPath(path, key->GetName())] = kid;
    }
  } // end while

  return ret;
}

// Consumes its arguments
TObject* Sum(TObject* a, TObject* b)
{
  if(a->ClassName() != std::string(b->ClassName())){
    std::cout << "Unable to add unlike types "
	      << a->ClassName() << " and "
	      << b->ClassName() << std::endl;
    exit(1);
  }

  if(a->ClassName() == std::string("TObjString")){
    const TObjString* as = (TObjString*)a;
    const TObjString* bs = (TObjString*)b;

    if(as->GetString() != bs->GetString()){
      std::cout << "Unable to add differing strings "
		<< as->GetString() << " and "
		<< bs->GetString() << std::endl;
      exit(1);
    }
    delete b;
    return a;
  }

  if(a->ClassName() == std::string("TVectorT<double>")){
    const TVectorD* av = (TVectorD*)a;
    const TVectorD* bv = (TVectorD*)b;

    bool diff = false;
    if(av->GetNrows() != bv->GetNrows()){
      diff = true;
    }
    else{
      for(int i = 0; i < av->GetNrows(); ++i){
        if(fabs((*av)[i]-(*bv)[i]) > 1e-6 * std::max(fabs((*av)[i]), fabs((*bv)[i]))){
          diff = true;
        }
      }
    }

    if(diff){
      std::cout << "Unable to add differing vectors " << std::endl;
      av->Print();
      std::cout << " and " << std::endl;
      bv->Print();
      exit(1);
    }

    delete b;
    return a;
  }

  if(a->ClassName() == std::string("TParameter<int>")){
    auto aint = dynamic_cast<TParameter<int>*>(a);
    auto bint = dynamic_cast<TParameter<int>*>(b);

    if(aint->GetVal() != bint->GetVal()){
      std::cout << "Unable to add differing integer TParameters "
          << aint->GetName() << " = " << aint->GetVal() << " and "
          << bint->GetName() << " = " << bint->GetVal() << std::endl;
      exit(1);
    }
    delete b;
    return a;
  }

  if(a->ClassName() == std::string("TVector3")){
    const TVector3* av = (TVector3*)a;
    const TVector3* bv = (TVector3*)b;

    bool diff = false;
    for(int i = 0; i < 3; ++i){
      if(fabs((*av)[i]-(*bv)[i]) > 1e-6 * std::max(fabs((*av)[i]), fabs((*bv)[i]))){
        diff = true;
      }
    }

    if(diff){
      std::cout << "Unable to add differing TVector3's " << std::endl;
      av->Print();
      std::cout << " and " << std::endl;
      bv->Print();
      exit(1);
    }

    delete b;
    return a;
  }

  if(a->ClassName() == std::string("THnSparseT<TArrayD>")){
    THnSparseD* as = (THnSparseD*)a;
    THnSparseD* bs = (THnSparseD*)b;

    as->Add(bs);
    delete bs;
    return as;
  }

  if(!a->InheritsFrom(TH1::Class()) || !b->InheritsFrom(TH1::Class())){
    std::cout << "Don't know how to add types "
	      << a->ClassName() << " and "
	      << b->ClassName() << std::endl;
    exit(1);
  }

  TH1* ah = (TH1*)a;
  TH1* bh = (TH1*)b;

  ah->Add(bh);
  delete bh;
  return ah;
}


std::map<std::string, TObject*>
SumObjects(const std::map<std::string, TObject*>& a,
	   const std::map<std::string, TObject*>& b)
{
  std::map<std::string, TObject*> ret;

  for(auto it: a){
    const std::string& key = it.first;
    if(b.count(key) == 0)
      ret[key] = it.second;
  }

  for(auto it: b){
    const std::string& key = it.first;
    if(a.count(key) == 0)
      ret[key] = it.second;
  }

  for(auto it: a){
    const std::string& key = it.first;
    if(b.count(key) > 0)
      ret[key] = Sum(it.second, b.find(key)->second);
  }

  return ret;
}

template<class A, class B> bool SameKeys(const std::map<A, B>& a,
					 const std::map<A, B>& b)
{
  if(a.size() != b.size()) return false;

  for(auto it: a) if(b.count(it.first) == 0) return false;

  return true;
}

void CheckInputNames(const std::vector<std::string>& innames)
{
  int M = -1;
  for(const std::string& f: innames){
    if(f.rfind(".root") != f.size()-5) return;
    const int ofloc = f.find("_of_");
    if(ofloc == -1) return;
    const int m = atoi(f.substr(ofloc+4, f.size()-ofloc-8).c_str());
    if(M != -1 && m != M) return;
    M = m;
  }

  if(int(innames.size()) != M){
    std::cout << "WARNING!!! Input files are all of the form *_of_"
	      << M << ".root, but there are " << innames.size()
	      << " files. Are you sure you have all the files you intended?"
	      << std::endl;
  }
}

void usage()
{
  std::cout << "Usage: hadd_cafana [-f] [-k] output.root input1.root input2.root ..." << std::endl;
  std::cout << "  -f Allow overwriting a preexisting output.root" << std::endl;
  std::cout << "  -k Skip over unreadable input files rather than aborting" << std::endl;

  exit(1);
}

int main(int argc, char** argv)
{
  if(argc < 3 ||
     argv[1] == std::string("-h") ||
     argv[1] == std::string("--help")) usage();

  int argIdx = 1;
  bool force = false;
  bool skip = false;

  while(argIdx < argc){
    if(argv[argIdx] == std::string("-f")){
      force = true;
      ++argIdx;
    }
    else if(argv[argIdx] == std::string("-k")){
      skip = true;
      ++argIdx;
    }
    else{
      break;
    }
  } // end while

  if(argc - argIdx < 2) usage();

  const std::string outname = argv[argIdx];
  ++argIdx;

  std::vector<std::string> innames;
  for(int i = argIdx; i < argc; ++i) innames.push_back(argv[i]);

  std::cout << "Adding " << innames.size() << " input files." << std::endl;

  CheckInputNames(innames);

  std::map<std::string, TObject*> sumObjs;

  for(const std::string& fname: innames){
    TFile* fin = TFile::Open(fname.c_str());
    if(!fin || fin->IsZombie()){
      if(skip)
	continue;
      else
	exit(1);
    }

    std::cout << "Finding objects in " << fname << std::endl;

    std::map<std::string, TObject*> objs = GetObjectMap(fin);
    std::cout << "Found " << objs.size() << " objects." << std::endl;
    fin->Close();
    delete fin;

    if(!sumObjs.empty() && !SameKeys(objs, sumObjs)){
      std::cout << "Warning: input files have different keys." << std::endl;
    }

    std::cout << "Summing objects into total." << std::endl;
    sumObjs = SumObjects(sumObjs, objs);
  }

  std::cout << "Writing " << sumObjs.size() << " objects to " << outname << std::endl;

  TFile* fout = TFile::Open(outname.c_str(), force ? "RECREATE" : "CREATE");
  if(!fout || fout->IsZombie()) exit(1);

  // Duplicate the directory structure in the output
  for(auto it: sumObjs){
    const std::string& key = it.first;
    const std::string d = dirname(key);
    if(!fout->Get(d.c_str())) fout->mkdir(d.c_str());

    fout->cd(d.c_str());
    it.second->Write(basename(key).c_str());
  }

  fout->Close();
}
