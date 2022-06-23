#ifndef HOUGHCALC_H
#define HOUGHCALC_H

//////////////////////////////////////////////////////////////////////////
// $Id: HoughCalc.h,v 1.6 2011-02-11 20:10:15 vahle Exp $                                                           //
//                                                                      //
// HoughCalc                                                            //
//                                                                      //
// Calculate linear Hough transformation for provided points and return //
// Hough peaks parameterized by r and theta                             //
// Adapted from RecoPrimo                                               //
// Author: Alexandre Sousa <asousa@physics.harvard.edu>	        	//
// 2010/02/08                                                           //
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// //Example usage:                                                     // 
// #include "Utilities/HoughCalc.h"                                     //
//                                                                      //
// bool GetHoughCandidates()                                            //
// {                                                                    //
//   util::HoughCalc hough;                                             //
//   hough.SetThresh(fHoughThreshold);                                  // 
//   for(loops over pairs of clusters/hits i,j in e.g. XZ View){        //
//     hough.AddPoint(clust[i]->Z()-Zave, clust[i]->dZ(),               //
//	 	      clust[j]->Z()-Zave, clust[j]->dZ(),               // 
//		      clust[i]->X()-Tave, clust[i]->dX(),               //
//		      clust[j]->X()-Tave, clust[i]->dX(),               //
//		      1., i);                                           //
//   }//for(clust.size())                                               //
//   hough.MakeTree(fHoughThreshold);                                   //
//   if(!hough.UpdateTree()) return true; //Done finding candidates     // 
//                                                                      //
//   //2. Pick up maximum point in the map                              //
//   double theta, rho, score;                                          //
//   hough.FindLines(&theta, &rho, &score);                             //
//                                                                      //
//   //Test if hits belong to line candidate                            //
//   for(unsigned int i=0; i<clust.size(); i++){                        //
//   bool testHit =  hough.TestHit(clust[i]->Z() - Zave,                //
//				   clust[i]->X() - Tave,                //
//				   *theta, *rho,                        //
//				   clust[i]->dX()*2.)                   //
//   }//for(clust.size())                                               //
//                                                                      //
//   return false; //keep going while there are lines to be found       //
//                                                                      //
// }//GetHoughCandidates()                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceTable.h"

///utility services
namespace util {

  class HoughCalc{

  public:
    struct Params
    {
      // There are actually no configuration parameters
    };

    // Allows 'nova --print-description' to work
    using Parameters = art::ServiceTable<Params>;

    HoughCalc(const Parameters& params, art::ActivityRegistry& reg);
    virtual ~HoughCalc();
    void Reset();
    //Add XY point to Hough r,theta map
    void AddPoint(double x1, double sigx1, 
		  double x2, double sigx2, 
		  double y1, double sigy1, 
		  double y2, double sigy2, 
		  double w, int ih1);

    //Get polar coordinates of line candidates 
    void FindLines(double* theta, double* rho, double* score);
    void FindLineErrs(double* etheta, double* erho);

    //Test if hit XY coordinates are consistent with line candidate
    bool TestHit(double x,    double y,
		 double thmx, double rhomx,
		 double sigth, double sigr);
    bool TestHit(double x,    double y,
		 double thmx, double rhomx,
		 double sigr);
    
    //Calculate distance from hit to line candidate
    double DistHit(double x,     double y,
		   double thmx,  double rhomx);
    
    int GetHit();
    
    //Create Hough map as linked list of all Hough bins
    //occupied above threshold
    static void MakeTree(double thresh);
    
    //Update HoughMap
    static int UpdateTree();
    
    //User defined Hough threshold
    static void SetThresh(double thresh);
    
    //Auxiliary internal types (need to be public to be used in definitions) 
    struct hitnode
    {
      int hit;
      float weight;
      hitnode *c0;
    };
    
    struct node
    {
      int rho;
      int theta;
      int hitcount;
      float weight;
      hitnode *hitlist;
      node *c0;
      node *c1;  
      int treeready;
      int goodsort;
    };
    
  private:
    
    static void InsertSort(node * head, node *item);
    static void DeleteChain(node *head);
    static void DeleteChain(hitnode * head);
    
    static node *newNode(int t, int r);
    static void nodeUpdate(node * head, double ww, int ih1);
    static void AddPoint(double theta, double rho, double ww, int ih1);
    static void RemovePoint(double theta, double rho, double ww, int ih1);
    
  };
  
}

DECLARE_ART_SERVICE(util::HoughCalc, LEGACY)
#endif
