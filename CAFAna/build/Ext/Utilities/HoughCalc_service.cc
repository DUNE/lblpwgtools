////////////////////////////////////////////////////////////////////////
/// \brief   Hough Calculation service
/// \author  Alexandre Sousa - asousa@physics.harvard.edu
/// \date
////////////////////////////////////////////////////////////////////////
#include "Utilities/HoughCalc.h"

#include <iostream>
#include <cmath>


#include "TMath.h"

namespace util{

  //This hough map uses a unique data structure written by Steve Cavanaugh, 
  //May 2007
  //  Instead of using a TH2F which would result in many empty bins
  //  which would be search over many times when looking for a peak,
  //  a linked list of bins over the required threshold is instead created -
  //  the search for a peak then only looks at bins which are above the threshold

  // Don't pollute the util namespace with these
  namespace{
    const static int gsNtheta = 360;
    const static int gsNrho   = 500;

    const static double minrho = -500;
    const static double maxrho = 500;
    const static double mintheta = 0.0;
    const static double maxtheta = M_PI;

    double myThresh=0;  //will hold tree threshold when tree is first made


    HoughCalc::node *nodearray[gsNtheta][gsNrho];
    HoughCalc::node *thetaarray[gsNtheta];

    int thetacount[gsNtheta];

    HoughCalc::node *treehead =NULL;
    HoughCalc::hitnode *peaklist=NULL;
  }

  HoughCalc::HoughCalc(const Parameters& /*params*/, art::ActivityRegistry& reg)
  { 
    for(int i=0;i<gsNtheta;i++){
      for(int j=0;j<gsNrho;j++){
	nodearray[i][j]=NULL;
      }			
      thetaarray[i]=NULL;
      thetacount[i]=0;
    }
  }

  HoughCalc::~HoughCalc()
  {
    //Free up all linked list nodes....
    for(int i=0;i<gsNtheta;i++){
      if(thetacount[i]>0)DeleteChain(thetaarray[i]);
    }
  
    delete treehead;treehead=NULL;
  }


  //......................................................................
  void HoughCalc::Reset(){
    if(treehead!=NULL){
      for(int i=0;i<gsNtheta;i++){
	if(thetacount[i]>0)DeleteChain(thetaarray[i]);
	thetaarray[i]=NULL;
	thetacount[i]=0;
      	for(int j=0;j<gsNrho;j++){
	  nodearray[i][j]=NULL;
	}	
      }
      treehead=NULL;
    }        
  }


  //......................................................................

  void HoughCalc::SetThresh(double thresh){myThresh=thresh;}

  //......................................................................

  void HoughCalc::MakeTree(double thresh){
  
    peaklist=NULL;
  
    myThresh=thresh;
  
    //need a infinite weight node for insertion sort
    if (treehead==NULL)
      {
	treehead=newNode(-1,-1);
	treehead->weight=INFINITY;
      }
    else treehead->c1=NULL;
  
    node *toadd=NULL;
  
    for(int i=0;i<gsNtheta;i++){
      toadd=thetaarray[i];
      while(toadd!=NULL){
	toadd->c1=NULL;
	if(toadd->weight>=thresh)
	  {
	    if(treehead->c1==NULL)treehead->c1=toadd;
	    else InsertSort(treehead,toadd);
	    toadd->goodsort=1;
	  }
	toadd=toadd->c0;
      }
    }
  }

  //......................................................................

  //UpdateTree remakes the tree by resorting and only keeping bins above myThresh

  int HoughCalc::UpdateTree()
  {
  
    //MsgDebug("Util","Updating Tree\n");
  
    node * toadd=NULL;
    node * temp = treehead->c1;
    node * pretemp = treehead;
  
    while(temp!=NULL){
    
      if(temp->weight<myThresh){pretemp->c1=temp->c1;temp->c1=NULL;temp=pretemp->c1;continue;}
    
      if (temp->goodsort==0){
	pretemp->c1=temp->c1;

	if(toadd==NULL)
	  {
	    toadd=temp;
	    toadd->c1=NULL;
	  }
	else
	  {
	    temp->c1=toadd;
	    toadd=temp;
	  }
      
	//leave pretemp in place
	temp=pretemp->c1;
      
      }else{
	pretemp=pretemp->c1;
	temp=pretemp->c1;
      }
    }
  
    while(toadd!=NULL){
      node * toaddtemp = toadd;
      toadd=toadd->c1;
      toaddtemp->c1=NULL;	
    
      toaddtemp->goodsort=1;
      if(toaddtemp->weight>=myThresh) InsertSort(treehead,toaddtemp);
    }

    if(treehead->c1!=NULL)return 1;
    return 0;  
  }

  //......................................................................

  void HoughCalc::AddPoint(double theta, double rho, double ww, int ih1)
  {
    int t = (int)TMath::Floor((theta-mintheta)/(maxtheta-mintheta)*(gsNtheta));
    int r = (int)TMath::Floor((rho-minrho)/(maxrho-minrho)*(gsNrho));
  
    if(r<0||t<0||r>=gsNrho||t>=gsNtheta) return;

    if (nodearray[t][r]==NULL){
      nodearray[t][r]=newNode(t,r);
      thetacount[t]++;
      nodearray[t][r]->c0=thetaarray[t];
      thetaarray[t]=nodearray[t][r];
    }
  
    nodeUpdate(nodearray[t][r],ww,ih1);
  }

  //......................................................................

  void HoughCalc::RemovePoint(double theta, double rho, double ww, int /*ih1*/)
  {
    int t = (int)TMath::Floor((theta-mintheta)/(maxtheta-mintheta)*(gsNtheta));
    int r = (int)TMath::Floor((rho-minrho)/(maxrho-minrho)*(gsNrho));
  
    if(r<0||t<0||r>=gsNrho||t>=gsNtheta) return;
    if(nodearray[t][r]==NULL)return;

    //use this code if keeping list of hits which made up bin
    /*
      int foundhit=0;
    
      hitnode * hit = nodearray[t][r]->hitlist;
      while(hit!=NULL&&hit->hit==ih1){nodearray[t][r]->hitlist=hit->c0;delete hit;hit=nodearray[t][r]->hitlist;foundhit=1;}
      while(hit!=NULL){if(hit->c0==NULL)break;if(hit->c0->hit==ih1){hitnode* t=hit->c0;hit->c0=hit->c0->c0;delete t;foundhit=1;}else hit=hit->c0;}
    
      if(foundhit==1){
      nodearray[t][r]->hitcount--;
      }
    */
  
    nodearray[t][r]->goodsort=0;
    nodearray[t][r]->weight+=ww;  //ww is <0 here... 
  }

  //......................................................................

  int HoughCalc::GetHit()
  {
    int hit=-1;
  
    if(peaklist!=NULL)
      {
	hit=peaklist->hit;
	peaklist=peaklist->c0;
      }
    return hit;
  }

  //......................................................................

  void HoughCalc::nodeUpdate(node * head, double ww, int ih1)
  {
  
    head->weight+=ww;	
  
    //use code below to see which hits made up this bin
    return;
    hitnode *temp = head->hitlist;
    while(temp!=NULL){if(temp->hit==ih1){temp->weight+=ww;return;}temp=temp->c0;}

    hitnode *n = new hitnode;
    n->hit=ih1;
    n->weight=ww;
    n->c0=head->hitlist;
    head->hitlist=n;
    head->hitcount++;
  }

  //......................................................................

  HoughCalc::node* HoughCalc::newNode(int t, int r)
  {
    node *temp=new node;
    temp->rho=r;
    temp->theta=t;
    temp->hitcount=0;
    temp->weight=0.0;
    temp->hitlist=NULL;
    temp->c0=NULL;
    temp->c1=NULL;
    temp->goodsort=0;
    temp->treeready=0;
    return temp;
  }

  //......................................................................

  void HoughCalc::InsertSort(node * head, node *item)
  {
    while(head->c1 != NULL && 
	  item->weight < head->c1->weight )
      {head=head->c1;}
    item->c1=head->c1;
    head->c1=item;
  }

  //......................................................................

  void HoughCalc::DeleteChain(node *head)
  {
    while(head != NULL){
      node *temp = head->c0;
      DeleteChain(head->hitlist);
      delete head;
      head=temp;
    }

  }

  //......................................................................

  void HoughCalc::DeleteChain(hitnode * head)
  {
    while(head!=NULL){
      hitnode *temp=head;
      head=head->c0;
      delete temp;
    } 
  }

  //......................................................................

  void HoughCalc::AddPoint(double x1, double /*sigx1*/,
			   double x2, double /*sigx2*/,
			   double y1, double sigy1, double y2, double sigy2, 
			   double w, int ih1) 
  {
    int itheta;
    int ir;
    double theta;
    double r;
    double yy1, yy2;
    double ww;

    double theta_segment = (y2!=y1)?
      -1.*atan((x2-x1)/(y2-y1)):
      M_PI/2.;
    if (theta_segment<0.) theta_segment += M_PI;
    double r_segment = cos(theta_segment)*x1 + sin(theta_segment)*y1;

    for (itheta=-3; itheta<=3; ++itheta) {
      theta = theta_segment + M_PI*(double)(itheta)/(double)(gsNtheta);
      if(theta<0.) theta = M_PI - theta;
      if(theta>M_PI) theta = theta - M_PI;
      for (ir=-3; ir<=3; ++ir) {
	r = r_segment + (maxrho-minrho)*(double)(ir)/(double)(gsNrho);  //mod by steve, must multiply by (maxrho-minrho)....
	yy1 = (sin(theta)!=0.)?
	  (r - cos(theta)*x1)/sin(theta):
	  y1;
	yy2 = (sin(theta)!=0.)?
	  (r - cos(theta)*x2)/sin(theta):
	  y2;
	ww = TMath::Gaus(yy1,y1,sigy1)*TMath::Gaus(yy2,y2,sigy2)*w;

	if(ww > 0.01) AddPoint(theta,r,ww,ih1);	
	if(ww < 0) RemovePoint(theta,r,ww,ih1);
      }
    }
  }

  //......................................................................

  void HoughCalc::FindLines(double* thetamx, double* rmx, double *score)
  {
    if (treehead->c1==NULL){*score=0;peaklist=NULL;return;}

    if(treehead->c1->goodsort==0)UpdateTree();  
    //update the tree if we failed to do so
    //so long as treehead->c1 has not been touched
    //it is still the max, so don't bother resorting rest of tree

    if (treehead->c1==NULL){*score=0;peaklist=NULL;return;}


    *thetamx=(double)(treehead->c1->theta+0.5)/(double)(gsNtheta)
      *(maxtheta-mintheta)+mintheta;
    *rmx=(double)(treehead->c1->rho+0.5)/(double)(gsNrho)
      *(maxrho-minrho)+minrho;
    *score=treehead->c1->weight;

    peaklist=treehead->c1->hitlist;
	
    return;
  }

  //......................................................................

  void HoughCalc::FindLineErrs(double* ethetamx, double* ermx)
  {
    //this was a trial function, but it doesn't work very well.  I wouldn't use it.--TV

    if (treehead->c1==NULL){peaklist=NULL;return;}

    if(treehead->c1->goodsort==0)UpdateTree();  
    //update the tree if we failed to do so
    //so long as treehead->c1 has not been touched
    //it is still the max, so don't bother resorting rest of tree

    if (treehead->c1==NULL){peaklist=NULL;return;}

    double sigr=0.;
    double sigt=0.;
    double totw=0.;

    double thetamx=(double)(treehead->c1->theta);
    double rmx=(double)(treehead->c1->rho);
    //	std::cout<<"thetamx "<<thetamx<<" rho "<<rmx<<std::endl;
    node *temp = treehead->c1;
    while(temp!=NULL){
      //	  std::cout<<"temp weight is "<<temp->weight<<" my thres "<<myThresh<<std::endl;
      if(temp->weight>=myThresh){
	sigr+=(temp->rho-rmx)*(temp->rho-rmx)*fabs(temp->weight);
	sigt+=(temp->theta-thetamx)*(temp->theta-thetamx)*fabs(temp->weight);
	totw+=fabs(temp->weight);
	//	    std::cout<<" rho "<<temp->rho<<" theta "<<temp->theta<<std::endl;
	//	    std::cout<<"sigr "<<sigr<<" sigt "<<sigt<<" totw "<<totw<<std::endl;
      }
      temp = temp->c1;
    }
    //	std::cout<<"after loop sigr "<<sigr<<" sigt "<<sigt<<" totw "<<totw<<std::endl;
    *ethetamx=0.;
    *ermx=0.;
    if(fabs(totw)>0.001){
      *ethetamx = sqrt(sigt/totw)/(double)(gsNtheta)*(maxtheta-mintheta);
      *ermx = sqrt(sigr/totw)/(double)(gsNrho)*(maxrho-minrho);
    }
    //	std::cout<<"theta "<<thetamx<<" rmx "<<rmx<<std::endl;
    //	std::cout<<"all done err theta "<<*ethetamx<<" ermx "<<*ermx<<std::endl;
    return;
  }

  //......................................................................

  bool HoughCalc::TestHit(double x,     double y,
			  double thmx,  double rhomx,
			  double sigth, double sigr)
  {
    int    itheta;
    double theta;
    double r;
    for (itheta=0; itheta<5*gsNtheta; ++itheta) {
      theta = M_PI*(double)itheta/(double)(5*gsNtheta);
      if (fabs(theta-thmx)>sigth) continue;
      r     = cos(theta)*x + sin(theta)*y;
      if (fabs(r-rhomx)<sigr) return true;
    }
    return false;
  }

  //......................................................................

  bool HoughCalc::TestHit(double x,     double y,
			  double thmx,  double rhomx,
			  double sigr)
  {
    double r;
    r = cos(thmx)*x + sin(thmx)*y;
    if (fabs(r-rhomx)<sigr) return true;
    return false;
  }

  //......................................................................

  double HoughCalc::DistHit(double x,     double y,
			    double thmx,  double rhomx)
  {
    double r;
    r = cos(thmx)*x + sin(thmx)*y;
    return fabs(r-rhomx);
  }
}//namespace


////////////////////////////////////////////////////////////////////////
namespace util 
{
  DEFINE_ART_SERVICE(HoughCalc)
} // namespace
