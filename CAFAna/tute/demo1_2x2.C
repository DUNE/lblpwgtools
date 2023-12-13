// Make a simple spectrum plot
// cafe demo0.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/MultiVar.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include <vector>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TFile.h"
using namespace ana;

typedef _HistAxis<MultiVar> MultiVarHistAxis;

 // Lets try something with similar structure to the neutron prong cuts in nova
 // but instead we're gonna plot the energy of tracks with a certain pdg...
///// 
 const std::vector<const caf::SRNDLArIntProxy*>& GetRecoInts(const caf::SRProxy* sr)
 {
    static std::vector<const caf::SRNDLArIntProxy*> ret;
    for (const auto & dlp : sr->nd.lar.dlp)
      ret.push_back(&dlp);
    return ret;

 };

  std::vector<const caf::SRTrackProxy*> GetTracks(const caf::SRProxy* sr)
 {
    std::vector<const caf::SRTrackProxy*> ret;
    std::vector<const caf::SRNDLArIntProxy*> dlp = GetRecoInts(sr);
    for (const auto & d : dlp )
      for(const auto & trk : d->tracks)
        ret.push_back(&trk);
    return ret;

 };
// energy of tracks
const MultiVar kTrackEnergyVisE( double MinEvis ){
 
  const MultiVar kTrackEnergy ([MinEvis] (const caf::SRProxy * sr){
      
      std::vector<double> ret;
      std::vector<const caf::SRTrackProxy*> tracks = GetTracks(sr);
      std::cout<< "got "<< tracks.size()<<"tracks\n";
      double energy;
      
      for ( const auto & trk : tracks){
        std::cout<<"Saw vise "<< trk->Evis<<std::endl;
        // here's the cut 
        if( trk->Evis> MinEvis){          
          energy = trk->E;
          std::cout<< "Get E "<<energy<<std::endl;
          ret.push_back(energy);
        }
      }
      return ret;
    });
  return kTrackEnergy;

}

const MultiVar kTrackEnergyVisE2( double MinEvis ){
 
  const MultiVar kTrackEnergy2 ([MinEvis] (const caf::SRProxy * sr){
      
      std::vector<double> ret;
      double energy;
      
      for ( const auto & dlp : sr->nd.lar.dlp){
        for (const auto & trk : dlp.tracks){
          std::cout<<"Saw vise "<< trk.Evis<<std::endl;
          if( trk.Evis> MinEvis){          
          energy = trk.E;
          std::cout<< "Get E "<<energy<<std::endl;
          ret.push_back(energy);
          }
        }
      }
      return ret;
    });
  return kTrackEnergy2;

}

const MultiVar kPartEnergyPDG( int pdg ){
 
  const MultiVar kPartEnergy ([pdg] (const caf::SRProxy * sr){
      
      std::vector<double> ret;
      double energy;
      
      for ( const auto & ixn : sr->common.ixn.dlp){
        for (const auto & part : ixn.part.dlp ){
          std::cout<<"Saw pdg "<< part.pdg <<std::endl;
          if( abs(part.pdg) == pdg){          
          energy = part.E;
          std::cout<< "Get E "<<energy<<std::endl;
          ret.push_back(energy);
          }
        }
      }
      return ret;
    });
  return kPartEnergy;

}


// need to create a class that has templates like this?? i.e, interaction level cut, 
// return the sr with embedded cut
//  const std::vector<std::pair< const caf::SRInteractionProxy*, bool >> kXContainmentCut(double lowerX, double upperX, const caf::SRProxy* sr){    
 // const std::set<std::pair< const caf::SRInteractionProxy*&, bool >> kXContainmentCut(double lowerX, double upperX, const caf::SRProxy* sr){    
  const std::set< const caf::SRInteractionProxy* >&kXContainmentCut(double lowerX, double upperX, const caf::SRProxy* sr){    

//       std::vector<std::pair< const caf::SRInteractionProxy*, bool >> ret;       
        static std::set< const caf::SRInteractionProxy*  > ret;

        double x;
        if (sr->common.ixn.ndlp>0){
          for( const auto & dlp : sr->common.ixn.dlp ){
             x=dlp.vtx.x;
             if ( x>lowerX && x<upperX ){
              //ret.push_back(std::make_pair(&dlp,true));
              ret.insert(&dlp);
            } else {
              ret.insert(&dlp);
              //ret.push_back(std::make_pair(&dlp,false));
            }
          }
          
          for( auto & r : ret) std::cout<<" vtx "<< r->vtx.x <<" passed " ;
          std::cout<<"\n";
          
      }
      return ret;

  }

const MultiVar kPartEnergyVtxCut( double lowerX, double upperX ){
 
  const MultiVar kPartE ([lowerX,upperX] (const caf::SRProxy * sr){
      std::vector<double> ret = {-5000.};
      //std::vector<std::pair< const caf::SRInteractionProxy*, bool >> dlp_containment = kXContainmentCut(lowerX,upperX,sr);
      auto  dlp_containment = kXContainmentCut(lowerX,upperX,sr);
      std::cout <<  "got "<< dlp_containment.size()<< " ixns \n";
      if (dlp_containment.size()>0 && sr->common.ixn.ndlp>0) {       
        for ( const auto & dlp : dlp_containment ){ //corresponds to sr->common.ixn.dlp
          //if(dlp.second){ // check if vertex from ix was contained
            // now you can loop through particles in that ixn
            //if (dlp.first->part.ndlp>0)
            if (dlp->part.ndlp>0)
              for(const auto & partdlp : dlp->part.dlp ) ret.push_back(partdlp.E);
          }
        }
      //} 
      return ret;
    });
  return kPartE;
}


// the same multivar but everything is embedded here (not adaptable )
const MultiVar kPartEnergyVtxCutSimple( double lowerX, double upperX ){
 
  const MultiVar kPartES ([lowerX,upperX] (const caf::SRProxy * sr){
      std::vector<double> ret = {-5000.};
      // loop over ixns 
      double x;
      if (sr->common.ixn.ndlp>0){
          for( const auto & dlp : sr->common.ixn.dlp ){
             x=dlp.vtx.x;
             if ( x>lowerX && x<upperX ){
              for (const auto & partdlp : dlp.part.dlp )
                ret.push_back(partdlp.E);
          }
        }
      } 
      return ret;
    });
  return kPartES;
}



void demo1_2x2()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/minirun4/notruth/outputCAF_notruth_*";
  //"/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_FHC_nonswap.root";

  // Source of events
  SpectrumLoader loader(fname);

  // A Var is a little snippet of code that takes a record representing the
  // event record and returns a single number to plot.
  const Var kTrueEnergy([](const caf::SRProxy* sr)
                        {
                          return sr->mc.nu[0].E;
                        });


// number interactions found by DeepLearnPhysics machine learning stack
  const Var kNumberNDLP = SIMPLEVAR(common.ixn.ndlp);

// particles in each interaction
  const MultiVar kPartNdlp([](const caf::SRProxy* sr){

      std::vector<double> dlps = {-5000.};
       if (sr->common.ixn.ndlp>0){
        for (int i=0; i<sr->common.ixn.ndlp; i++){
          //for (int j=0; j<sr->common.ixn.dlp[i].part.ndlp; j++){

            dlps.push_back(sr->common.ixn.dlp[i].part.ndlp);
          //}
        }
       }    
      return dlps; 
  });



 const MultiVar kRecoE([](const caf::SRProxy* sr){
      std::vector<double> energy = {-5000.};
       if (sr->common.ixn.ndlp>0){
        for (int i=0; i<sr->common.ixn.ndlp; i++){
          for (int j=0; j<sr->common.ixn.dlp[i].part.ndlp; j++){
            energy.push_back(sr->common.ixn.dlp[i].part.dlp[j].E);
          }
        }
       }      
      return energy; 

  });

  const MultiVar kAllIxnVtxX([](const caf::SRProxy* sr){
      std::vector<double> vtxs = {-5000.};
       if (sr->common.ixn.ndlp>0){
        for (int i=0; i<sr->common.ixn.ndlp; i++){
          //std::cout<< "From int vtx var, ndlp="<<i<<", x="<<sr->common.ixn.dlp[i].vtx.x<<std::endl;
          vtxs.push_back(sr->common.ixn.dlp[i].vtx.x);
        }
       }    
      return vtxs; 
  });


  double lowerX= -50.;
  double upperX=50.;
 // const MultiVar kXContainmentCut(double lowerX, double upperX){
    const MultiVar kXContainmentCut([lowerX,upperX](const caf::SRProxy* sr){
        // Always need to put a garbage value at first
        std::vector<double> cut = {-5000.};
        double x;
        if (sr->common.ixn.ndlp>0){
          for (int i=0; i<sr->common.ixn.ndlp; i++){
            // get x position
            x = sr->common.ixn.dlp[i].vtx.x;
            //double y = sr->common.ixn.dlp[i].vtx.y;           
            if ( x>lowerX && x<upperX ) {
              //std::cout<< "yes cut, at ndlp="<<i<<", x="<<x <<std::endl;
              cut.emplace_back(1.);
            }else{
                cut.emplace_back(0.);
              //std::cout<< "no x="<<x<<", y= "<<y <<std::endl;
              }
            }           
          }
          //for( auto & cont : cut) std::cout<< cont<<",";
          //std::cout<<"\n";
          return cut;
      });
 //    return kXContainment;
 // }

double lowerE=0.;
double upperE=0.2;
//const MultiVar kEnergyCut(double lowerE, double upperE){
    
    const MultiVar kEnergyCut([lowerE,upperE](const caf::SRProxy* sr){
        std::vector<double> cut = {-5000.};
        double E;
        if (sr->common.ixn.ndlp>0){
        for (int i=0; i<sr->common.ixn.ndlp; i++){
          for (int j=0; j<sr->common.ixn.dlp[i].part.ndlp; j++){
            E= sr->common.ixn.dlp[i].part.dlp[j].E;
            if( E > lowerE && E<upperE ){
               std::cout<< "yes cut, at ndlp="<<i<<", dlp,"<<j<<", E="<<E <<std::endl;
              cut.emplace_back(1.);
              } else {
              cut.emplace_back(0.);
              }
            }
          }
        }
        for( auto & cont : cut) std::cout<< cont<<",";
        return cut;
      });
//     return kEnergy;
//}


// std::vector<std::pair< int , bool >> MultiCutVtxX ( const caf::SRProxy* sr){
// // static std::set<const caf::SRProngProxy*> ret;
//      std::vector<std::pair< int , bool >> CutIdx;
//      if (sr->common.ixn.ndlp>0){
//          for (int i=0; i<sr->common.ixn.ndlp; i++){
//            // get x position
//            x = sr->common.ixn.dlp[i].vtx.x;
//            //double y = sr->common.ixn.dlp[i].vtx.y;           
//            if ( x>lowerX && x<upperX ) {
//              std::cout<< "yes cut, at ndlp="<<i<<", x="<<x <<std::endl;
//              CutIdx.emplace_back(std::make_pair{i, true});
//            }else{
//              CutIdx.emplace_back(std::make_pair{i, false});
//              //std::cout<< "no x="<<x<<", y= "<<y <<std::endl;
//              }
//            }           
//          }
//
// };




  // Define a spectrum, ie a histogram with associated POT information
  const Binning binsNDLP = Binning::Simple(10, 0, 10);
  const Binning binsvtx = Binning::Simple(100, -100., 100);
  const Binning binsE = Binning::Simple(50, 0., 1.);
  const Binning binCut = Binning::Simple(2, -0.01,  1.01);

  const HistAxis axNDLP("common.ixn.ndlp", binsNDLP, kNumberNDLP);
  const MultiVarHistAxis axPartDlp("particles per interaction (common.ixn.dlp.part.ndlp)", binsNDLP,kPartNdlp);
  const MultiVarHistAxis axEnergy("Reco energy (common.ixn.dlp.part.dlp.E)", binsE ,kRecoE);

  const MultiVarHistAxis axContCut( "Pass containment cut", binCut, kXContainmentCut);
  const MultiVarHistAxis axECut( "Pass Energy cut", binCut, kEnergyCut);

  const MultiVarHistAxis axAllVtx("Vertex X Position for all ixn (MULTIVAR)", binsvtx, kAllIxnVtxX);
 
  const MultiVarHistAxis axXVtxCutVtx( "Vertex X Position ", binsvtx, kAllIxnVtxX,
                                    "Passes X containment cut", binCut, kXContainmentCut) ;

  const MultiVarHistAxis axECutVtx( "Reco energy", binsE, kRecoE,
                                 "Passes X containment cut", binCut, kXContainmentCut) ;

  const MultiVarHistAxis axECutE( "Reco energy", binsE, kRecoE,
                                "Passes Energy cut", binCut, kEnergyCut) ;

  const MultiVarHistAxis axEvisE("Energy with visible energy cut (0.05)", binsE, kTrackEnergyVisE2(0.05));

  const MultiVarHistAxis axEproton("Energy of protons", binsE,kPartEnergyPDG(2212));
  const MultiVarHistAxis axEpion("Energy of charged pions", binsE,kPartEnergyPDG(211));
  const MultiVarHistAxis axEmuon("Energy of muons", binsE,kPartEnergyPDG(13));
  const MultiVarHistAxis axEelectron("Energy of electrons", binsE,kPartEnergyPDG(11));

  //const MultiVarHistAxis axEnergyVtxCut("Energy of particles contained" , binsE, kPartEnergyVtxCut(-50,50));
  const MultiVarHistAxis axEnergyVtxCutSimple("Energy of particles contained (no ext.cut)" , binsE,kPartEnergyVtxCutSimple(-50,50));

  std::vector< std::pair<Spectrum*,std::string>> specs1D, specs2D;
  specs1D.emplace_back( new Spectrum(loader, axNDLP, kNoCut) ,   "ndlp");
  specs1D.emplace_back( new Spectrum(loader, axPartDlp, kNoCut), "dlp.part.ndlp");
  specs1D.emplace_back( new Spectrum(loader, axEnergy, kNoCut) , "Energy");
  specs1D.emplace_back( new Spectrum(loader, axAllVtx, kNoCut) , "vtxX");
  specs1D.emplace_back( new Spectrum(loader, axEvisE, kNoCut), "EvisEcut");
  specs1D.emplace_back( new Spectrum(loader, axEproton, kNoCut), "Eproton");
  specs1D.emplace_back( new Spectrum(loader, axEmuon, kNoCut), "Emuon");
  specs1D.emplace_back( new Spectrum(loader, axEpion, kNoCut), "Epion");
  specs1D.emplace_back( new Spectrum(loader, axEelectron, kNoCut), "Eelectron");
  //specs1D.emplace_back( new Spectrum(loader, axEnergyVtxCut, kNoCut), "axEnergyVtxCut");
  specs1D.emplace_back( new Spectrum(loader, axEnergyVtxCutSimple, kNoCut), "axEnergyVtxCutSimple");

  specs2D.emplace_back( new Spectrum(loader, axXVtxCutVtx, kNoCut),"vtxCutvtx");
  specs2D.emplace_back( new Spectrum(loader,axECutVtx,kNoCut ),    "ECutvtx");
  specs2D.emplace_back( new Spectrum(loader,axECutE,kNoCut ),      "ECutE");

//  Spectrum sEnergyNC(loader, axEnergy, kIsNC);


  // This is the call that actually fills in those spectra
  loader.Go();
  std::cout<< "Overriding POT\n";



  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  for ( auto & s : specs1D) s.first->OverridePOT(pot);
  for ( auto & s : specs2D) s.first->OverridePOT(pot);


  std::cout<< specs1D[0].first->POT()<< " POT\n";

  std::string out_file= "demo1_2x2_output.root";
  std::cout<<" Save spectra in " <<out_file;

  TFile *file_o = new TFile(out_file.c_str(),"Recreate");
  // save Spectrum objs and norm hists for quick view

  for ( auto & s : specs1D) s.first->SaveTo(file_o,s.second);
  for ( auto & s : specs2D) s.first->SaveTo(file_o,s.second);

  file_o->Print();
  file_o->Close(); 


   for ( auto & s : specs1D){
    new TCanvas;
    s.first->ToTH1(pot, kBlue)->Draw("hist");
    std::cout<< "Integral of "<< s.second<< " = "<<s.first->ToTH1(pot)->Integral()<<std::endl;
   }

   for ( auto & s : specs2D){
    new TCanvas;
    s.first->ToTH2(pot)->Draw("colz");
    std::cout<< "Integral of "<< s.second<< " = "<<s.first->ToTH2(pot)->Integral()<<std::endl;
    // projection of 0 bin 
    new TCanvas;
    TH1 * spass = s.first->ToTH2(pot)->ProjectionX((s.second+"pass").c_str(),2,-1);
    spass->Draw("hist");
    std::cout<< "Integral of "<< s.second<< " passing cut = "<<spass->Integral()<<std::endl;

    //
    new TCanvas;
    TH1 * snopass = s.first->ToTH2(pot)->ProjectionX((s.second+"nopass").c_str(),0,1);//->Draw("hist");
    snopass->Draw("hist");
    std::cout<< "Integral of "<< s.second<< " not passing cut = "<<snopass->Integral()<<std::endl;
    //
   }
   




}
