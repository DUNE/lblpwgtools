
/* This code was adapted from globes-discovery.c in GLoBES version 3.1.11.
 */
#include "globes-REdiscovery.h"
/* This function is need to ensure the proper result if either the
   upper and lower range are the same or the number of steps is 1.   
*/
double safe_range(double range[],int steps)
{
  double erg;
  /* trying to catch the case where upper and lower range are the same
     requires 4*DBL_EPSILON to make sure that when this value is
     divided by 2 is still is larger than DBL_EPSILON, and we take
     twice that, just to be sure.
  */
  if(steps==0||range[0]>=range[1]) erg=range[1]-range[0]+16*DBL_EPSILON;
  else erg=(range[1]-range[0])/steps;
  return erg;
}


int rescale_lumi(int exp, double factor)
{
  int s;
  double l0;
  l0=glbGetTargetMass(exp);
  s=glbSetTargetMass(exp,l0*factor);
  return s;
}


/****************************************************************************/
void globesREdiscovery(double osc[], double *inxrange)
{
  int ynoct,ynhier;
  projection Xaxis,Yaxis;
  int i,resume=0;
  char oct='+';
  char hie='+';
  //set the xrange based on the input value of t12 (osc[1])
  double xrangevarup, xrangevardown;
  
  if(inxrange[0]==1e-2 && inxrange[1]==10.0){
    xrangevarup=xrangevardown=log10(pow(sin(2*osc[1]),2));
  }else{
    xrangevardown=inxrange[0];
    xrangevarup=inxrange[1];
  }
  printf("Setting xrange to %f, %f.\n", xrangevardown, xrangevarup);

  double xrange[]={xrangevardown-1,xrangevarup+1};
  double yrange[]={-180,180};
  double Yrange[]={-180,180};
  double Xrange[]={xrangevardown,xrangevarup};
  double posx,posy;
  int resolution[]={70,40};
  //used arguments.tSteps as resolution (10 cannot be set, shrug)
  int Resolution[]={1,arguments.tSteps==10?73:arguments.tSteps};
  int Plane[]={GLB_THETA_13,GLB_DELTA_CP};

  /* Initialize parameter vector(s) */
  /*glb_params true_values = glbAllocParams(); //these are defined globally in mgt
  glb_params input_errors = glbAllocParams();
  glb_params test_values = glbAllocParams();*/
  glb_params starting_values = glbAllocParams();
  glb_params minimum = glbAllocParams();
  glb_params buffer = glbAllocParams();
   

  double X,Y;
  double thetheta13,delta;    
  glb_params *listb=NULL;
  double *fchi;
  int ct;

  setup_axis(&Xaxis,Plane[0]);
  setup_axis(&Yaxis,Plane[1]);

  Resolution[0]=Resolution[0]-1;
  Resolution[1]=Resolution[1]-1;

  /* just not to leave them undefined */
  if(resume!=1)
    {
      posx=Xrange[0];
      posy=Yrange[0];  
    }
#ifdef DEBUG
  fprintf(stderr,"X-start %f %f difference %g safe %g\n",Yrange[0],Yrange[1],Yrange[1]-Yrange[0],safe_range(Yrange,Resolution[1]));
  int count=0;
#endif
  
  int xbreak=NO;
  int ybreak=NO;

  int d1,d2;

  for(X=Xrange[0];X<=Xrange[1]+safe_range(Xrange,Resolution[0])/2&&xbreak==NO;
      X=X+safe_range(Xrange,Resolution[0]))
    {
      ybreak=NO;
      for(Y=Yrange[0];Y<=Yrange[1]+safe_range(Yrange,Resolution[1])/2&&ybreak==NO;
	  Y=Y+safe_range(Yrange,Resolution[1]))
	{
	  AddToOutputprintf("C   %6.6e\t%6.6e\n",X,Y);
	  for(d1=NO;d1<NO+2;d1++)
	    {
	      for(d2=NO;d2<NO+2;d2++)
		{
		  ynhier=d1;
		  ynoct=d2;
		  if(fabs(osc[2]-M_PI/4.0)<1E-3) d2++;
		  if(ynhier==NO )hie='+';
		  if(ynhier==YES )hie='-';
		  if(ynoct==NO )oct='+';
		  if(ynoct==YES )oct='-';
#ifdef DEBUG
	fprintf(stderr,"loop %f %f\n",X,Y);
	count++;
	if(count>50) exit(1);
#else /* !DEBUG */
	thetheta13=(Xaxis.f)(X);
	delta=(Yaxis.f)(Y);
	glbDefineParams(true_values,osc[0],osc[1],osc[2],osc[3],osc[4],osc[5]);
	glbSetOscParams(true_values,thetheta13,Xaxis.which);
	glbSetOscParams(true_values,delta,Yaxis.which);
	
	/* this allows us to produce output, test the parsing etc,
	   without needing much time at all.
	*/
	if(arguments.test==YES){
	  //don't scan with eightfold, just use glbchisys everywhere with the input osc values
	  ct=1;
	  listb=(glb_params *) realloc(listb,sizeof(glb_params)*1);
	  //glb_params listb[1];
	  listb[0]=glbAllocParams();
	  glbCopyParams(true_values,listb[0]);
  }else{
	
	  ct=eightfold(true_values,input_errors,ynhier,ynoct,xrange,yrange,resolution,&listb,&fchi);
	  double rdeg=DBL_MAX;
	  for(i=0;i<ct;i++){
	 
	    AddToOutputprintf("G%c%c %6.6e\t",hie,oct,fchi[i]);
	    mgt_print_params(listb[i]);
	    AddToOutputprintf("\n");
	    if(fchi[i]<=rdeg) rdeg=fchi[i];
	  }
  }
	glbCopyParams(true_values,test_values);
	
	if(ynhier==YES)
	  {
	    double nv;
	    /* following hep-ph/0509359 eq. 3 */
	    nv=-osc[5]+2*osc[4]*cos(osc[0])*cos(osc[0]);
	    glbSetOscParams(test_values,nv,GLB_DM_ATM);
	    
	  }
	if(ynoct==YES) {
	  glbSetOscParams(test_values,M_PI/2-osc[2],GLB_THETA_23);
	}
	glbCopyParams(test_values,starting_values);

	glbSetStartingValues(starting_values);
	
	/* The simulated data are computed */
	glbSetOscillationParameters(true_values);
	glbSetRates();



 
	double res=DBL_MAX,rest,resa,resb,rd,rt,rat,resat;
	
	rat=DBL_MAX;
	rd=DBL_MAX;
	rt=DBL_MAX;

	   
	    if(YES==YES)
	      {
		for(i=0;i<ct;i++)
		  {
		    glbCopyParams(listb[i],buffer);
		    
		    glbSetOscParams(buffer,0,GLB_THETA_13);
		    
		    if(arguments.test==YES){
		      rest=glbChiSys(buffer,GLB_ALL,GLB_ALL);
		      glbCopyParams(buffer,minimum);
	      }else{
	        rest=glbChiTheta(buffer,minimum,GLB_ALL);
	      }
		    AddToOutputprintf("T%c%c %6.6e\t",hie,oct,rest);
		    mgt_print_params(minimum);
		    AddToOutputprintf("\n");
		    if(rest<=rt) rt=rest;
		    
		  }
	      }
	    
	    if(YES==YES)
	      {
		for(i=0;i<ct;i++)
		  {
		     glbCopyParams(listb[i],buffer);
		
		    
		    double resc=0;
		    double resd=0;
		    /* checking the case th13=th13^0 */
		    glbCopyParams(true_values,buffer);
		    glbSetOscParams(buffer,0,GLB_DELTA_CP);
		    if(arguments.test==YES){
		      resc=glbChiSys(buffer,GLB_ALL,GLB_ALL);
		      glbCopyParams(buffer,minimum);
	      }else{
	        resc=glbChiDelta(buffer,minimum,GLB_ALL); 
	      }
		    AddToOutputprintf("D%c%c %6.6e\t",hie,oct,resc);
		    mgt_print_params(minimum);
		    AddToOutputprintf("\n");
		    
		    glbCopyParams(true_values,buffer);
		    glbSetOscParams(buffer,M_PI,GLB_DELTA_CP);
		    if(arguments.test==YES){
		      resd=glbChiSys(buffer,GLB_ALL,GLB_ALL);
		      glbCopyParams(buffer,minimum);
	      }else{
	        resd=glbChiDelta(buffer,minimum,GLB_ALL); 
	      }
		    AddToOutputprintf("D%c%c %6.6e\t",hie,oct,resd);
		    mgt_print_params(minimum);
		    AddToOutputprintf("\n");

		    glbCopyParams(listb[i],buffer);
		    /*testing the other cases found previously as global
		      minima */ 	
		    glbSetOscParams(buffer,0,GLB_DELTA_CP);
		    if(arguments.test==YES){
		      resa=glbChiSys(buffer,GLB_ALL,GLB_ALL);
		      glbCopyParams(buffer,minimum);
	      }else{
	        resa=glbChiDelta(buffer,minimum,GLB_ALL); 
	      }
		    AddToOutputprintf("D%c%c %6.6e\t",hie,oct,resa);
		    mgt_print_params(minimum);
		    AddToOutputprintf("\n");
		    glbSetOscParams(buffer,M_PI,GLB_DELTA_CP);
		    if(arguments.test==YES){
		      resb=glbChiSys(buffer,GLB_ALL,GLB_ALL);
		      glbCopyParams(buffer,minimum);
	      }else{
	        resb=glbChiDelta(buffer,minimum,GLB_ALL); 
	      }
		    AddToOutputprintf("D%c%c %6.6e\t",hie,oct,resb);
		    mgt_print_params(minimum);
		    AddToOutputprintf("\n");
		    if(resa<resb) res=resa;
		    else res =resb;
		    if(res<=rd) rd=res;
		    if(resc<=rd) rd=resc;	
		    if(resd<=rd) rd=resd;
		  }
	      }

	    if(YES==YES)
	      {
		for(i=0;i<ct;i++)
		  {
		    
		    glbCopyParams(listb[i],buffer);
		    glbSetOscParams(buffer,M_PI/4,GLB_THETA_23);
		    if(arguments.test==YES){
		      resat=glbChiSys(buffer,GLB_ALL,GLB_ALL);
		      glbCopyParams(buffer,minimum);
	      }else{
	        resat=glbChiTheta23(buffer,minimum,GLB_ALL);
	      }
		    AddToOutputprintf("A%c%c %6.6e\t",hie,oct,resat);
		    mgt_print_params(minimum);
		    AddToOutputprintf("\n");
		    if(resat<=rat) rat=resat;
		  }
	      }
	    
	  
	//free_list(listb,ct);
	//if(arguments.test==NO) free(fchi);
	
	if(Yrange[0]==Yrange[1]) {ybreak=YES;}
#endif /* !DEBUG */
      }
      if(Xrange[0]==Xrange[1]) {xbreak=YES;}
    }
	}
    }
  /* Clean up*/
  
}
