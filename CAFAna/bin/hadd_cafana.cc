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
#include "TTree.h"
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

const std::vector<std::string> kSpecialTypes
{
  "SystShifts",
};
// don't sum a folder that corresponds to a special class we shouldn't add
bool ShouldSumFolder(TDirectory* dir)
{
  auto type = dynamic_cast<const TObjString*>(dir->Get("type"));
  if (!type)
    return true;

  for (const auto & spType : kSpecialTypes)
  {
    if (type->GetString().BeginsWith(spType.c_str()))
      return false;
  }
  return true;
}

// for objects that weren't supposed to be summed,
// we just compare them to make sure they're the same
// (and that we retain all of them across all the files)
void CheckAndMergeNoSumObjects(std::map<std::string, TObject*>& a,
                               const std::map<std::string, TObject*>& b)
{
  // the 'no sum' objects were once TDirectorys;
  // to make the comparisons here we'll partially rebuild them.
  // todo: rebuilding 'a' every time inside a loop may be too slow,
  //       should be revisited if it is problematic
  auto RebuildDirs = [](const std::map<std::string, TObject*>& map)
  {
    std::map<std::string, std::map<std::string, TObject*>> ret;
    for (const auto & objPair : map)
    {
      auto dirIdx = objPair.first.find_last_of("/");
      ret[objPair.first.substr(0, dirIdx)][objPair.first.substr(dirIdx+1)] = objPair.second;
    }
    return ret;
  };
  auto aObjs = RebuildDirs(a);
  auto bObjs = RebuildDirs(b);

  std::set<std::string> keys;
  for (const auto & collection : {aObjs, bObjs})
    std::transform(collection.begin(), collection.end(), std::inserter(keys, keys.end()),
                   [](const decltype(aObjs)::value_type & nameMapPair){ return nameMapPair.first; });

  for (const auto & key : keys)
  {
    // first two cases are easy -- obj is only in one of the two
    if (aObjs.find(key) == aObjs.end())
    {
      for (auto & objPair : bObjs.find(key)->second)
        a[ConcatPath(key, objPair.first)] = objPair.second;
      continue;
    }
    else if (bObjs.find(key) == bObjs.end())
      continue;

    // now if shared...
    auto aObj = aObjs.at(key);
    auto bObj = bObjs.at(key);
    auto typeA = dynamic_cast<TObjString *>(aObj.at("type"));
    auto typeB = dynamic_cast<TObjString *>(bObj.at("type"));
    if (typeA && typeB)
    {
      if (typeA->GetString() != typeB->GetString() || strlen(typeA->GetString()) == 0)
      {
        std::cout << "Unable to add incompatible CAFAna custom types "
                  << typeA->GetString() << " and " << typeB->GetString()
                  << std::endl;
        exit(1);
      }

      if (typeA->GetString().BeginsWith("SystShifts"))
      {
        auto valsA = dynamic_cast<TH1D*>(aObj.at("vals"));
        auto valsB = dynamic_cast<TH1D*>(bObj.at("vals"));

        // if one of them doesn't have stored values, neither of them should
        bool match = (!valsA) == (!valsB);
        if (match && valsA)
        {
          match = (valsA->GetNbinsX() == valsB->GetNbinsX());
          if (match)
          {
            std::map<std::string, double> pairsA, pairsB;
            for (int binIdx = 1; binIdx <= valsA->GetNbinsX(); binIdx++)
            {
              pairsA[valsA->GetXaxis()->GetBinLabel(binIdx)] = valsA->GetBinContent(binIdx);
              pairsB[valsB->GetXaxis()->GetBinLabel(binIdx)] = valsB->GetBinContent(binIdx);
            }
            match = match && pairsA == pairsB;
          } // if (... GetNbinsX() matches)
        } // if (... valsA is non-null)

        if (!match)
        {
          std::cout << "Unable to add differing SystShifts objects at path: "
                    << key
                    << std::endl;
          for (const auto & obj : {aObj, bObj})
            for (const auto & stuffPair : obj)
              stuffPair.second->Print("all");
          exit(1);
        }
      } // if (typeA ... is SystShifts)
      else
      {
        std::cout << "Don't know what to do with custom CAFAna type: " << typeA << std::endl;
        std::cout << "How did we get here??" << std::endl;
        exit(1);
      }
    } // if (typeA && typeB)

  } // for (key)

}

// first element contains usual objects, second one contains CAFAna types that shouldn't be summed
std::pair<std::map<std::string, TObject*>, std::map<std::string, TObject*>>
GetObjectMap(TDirectory* x,
             std::string path = "",
             bool forceExtractObjs = false)
{
  std::map<std::string, TObject*> ret;
  std::map<std::string, TObject*> retNoSum;

  TList* keys = x->GetListOfKeys();

  // TODO is this bit from the python version necessary?
  //     # Use a set to collapse all the cycle numbers down into the same name
  //     for key in {k.GetName() for k in x.GetListOfKeys()}:

  TIter next(keys);
  while(TObject* key = next()){
    TObject* kid = x->Get(key->GetName());
    if(auto dir = dynamic_cast<TDirectoryFile*>(kid)){
      if(ShouldSumFolder(dir) || forceExtractObjs)
      {
        auto kids = GetObjectMap(dir, ConcatPath(path, dir->GetName()));
        delete kid;
        ret.insert(kids.first.begin(), kids.first.end());
      }
      else
      {
        // since we know this folder isn't meant to be summed, recurse one more time
        // using the 'forceExtractObjs' mode to pull all the objects out
        // so we can stuff them into 'retNoSum' rather than 'ret'
        auto kids = GetObjectMap(dir, ConcatPath(path, dir->GetName()), true);
        delete kid;
        retNoSum.insert(kids.first.begin(), kids.first.end());
      }
    }
    else{
      if(auto th1 = dynamic_cast<TH1*>(kid)){
        th1->SetDirectory(0);
      }
      else if (auto ttree = dynamic_cast<TTree*>(kid))
      {
        // load the entire tree into memory so it can be relocated to a new file
        auto newtree = ttree->CloneTree();
        // disconnect the copy from the original that came out of the file.
        // the arguments are right, despite how they appear to be backwards...
        ttree->CopyAddresses(newtree, true);
        newtree->SetBranchStatus("*", true);
        newtree->SetDirectory(nullptr);
        delete kid;
        kid = newtree;
      }
      ret[ConcatPath(path, key->GetName())] = kid;
    }
  } // end while

  return std::make_pair(ret, retNoSum);
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

  if(auto listA = dynamic_cast<TList*>(a))
  {
    bool match = true;
    auto listB = dynamic_cast<TList*>(b);
    TIter itA(listA), itB(listB);
    TObject * objA = itA.Next();
    TObject * objB = itB.Next();
    while ( objA && objB )
    {
      if (strcmp(objA->ClassName(), objB->ClassName()) != 0)
      {
        std::cout << "Unable to add differing elements inside TList: " << std::endl;
        objA->Print();
        objB->Print();
        match = false;
        break;
      }

      if (auto valA = dynamic_cast<TObjString*>(objA))
        match = match && strcmp(valA->GetString(), dynamic_cast<TObjString*>(objB)->GetString()) == 0;
      else
      {
        std::cout << "Unable to compare differing " << objA->ClassName() << "s inside TLists!" << std::endl;
        match = false;
      }
      objA = itA.Next();
      objB = itB.Next();
    }
    // if we didn't get to the end of both lists at the same time, they don't match
    match = match && (!objA == !objB);
    if (!match)
    {
      std::cout << "Unable to add differing TLists:" << std::endl;
      listA->Print();
      listB->Print();
      exit(1);
    }
    delete b;
    return a;
  }

  if (auto at = dynamic_cast<TTree*>(a))
  {
    auto bt = dynamic_cast<TTree*>(b);
    bt->SetBranchStatus("*", true);
    TList otherTreeList;
    otherTreeList.SetOwner(false);
    otherTreeList.Add(bt);
    at->Merge(&otherTreeList);
    otherTreeList.Clear();
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
  std::map<std::string, TObject*> noSumObjs;

  for(const std::string& fname: innames){
    TFile* fin = TFile::Open(fname.c_str());
    if(!fin || fin->IsZombie()){
      if(skip)
        continue;
      else
        exit(1);
    }

    std::cout << "Finding objects in " << fname << std::endl;

    // first element contains usual objects, second one contains CAFAna types that shouldn't be summed
    std::pair<std::map<std::string, TObject*>, std::map<std::string, TObject*>> objs = GetObjectMap(fin);
    std::cout << "Found " << objs.first.size() + objs.second.size() << " objects." << std::endl;
    fin->Close();
    delete fin;

    if(   (!sumObjs.empty() && !SameKeys(objs.first, sumObjs))
       || (!noSumObjs.empty() && !SameKeys(objs.second, noSumObjs)) ){
      std::cout << "Warning: input files have different keys." << std::endl;
    }

    std::cout << "Summing objects into total." << std::endl;
    sumObjs = SumObjects(sumObjs, objs.first);
    CheckAndMergeNoSumObjects(noSumObjs, objs.second);
  }

  std::cout << "Writing " << sumObjs.size() << " objects to " << outname << std::endl;

  TFile* fout = TFile::Open(outname.c_str(), force ? "RECREATE" : "CREATE");
  if(!fout || fout->IsZombie()) exit(1);

  // Duplicate the directory structure in the output
  for (auto & collection : {sumObjs, noSumObjs})
  {
    for (auto &it: collection)
    {
      const std::string &key = it.first;
      const std::string d = dirname(key);
      if (!fout->Get(d.c_str())) fout->mkdir(d.c_str());

      fout->cd(d.c_str());
      Int_t op = 0;
      if (dynamic_cast<TList*>(it.second))
        op = TObject::kSingleKey;  // otherwise the key elements get stored individually rather than as a TList
      it.second->Write(basename(key).c_str(), op);
    }
  }

  fout->Close();
}
