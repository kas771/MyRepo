#ifndef LARLITE_TPROJMUONDEDX_CXX
#define LARLITE_TPROJMUONDEDX_CXX

#include "TProjMuonDEDX.h"

namespace larlite {

  bool TProjMuonDEDX::initialize() {

    projhist = new TH2D("projhist", "Residual Range vs. dE/dX for Muon Truth Events in 2D", 400, 0, 50, 400, 0, 10);
    projhist->GetXaxis()->SetTitle("Residual Range (cm)");
    projhist->GetYaxis()->SetTitle("dE/dX (MeV/cm)");

     nentries = 0;

    return true;
  }
  
  bool TProjMuonDEDX::analyze(storage_manager* storage) {
    	double dEdX_l = 0;
	double dEdX_m = 0;
	double dEdX_n = 0;

    ::larlite::event_mctrack *ev_mctrack = storage->get_data< ::larlite::event_mctrack>("mcreco");

    //Let's check to make sure that the hits were successfully read in
    if(!ev_mctrack){
      print(msg::kERROR,__FUNCTION__,"Uh oh, I didn't find any mctracks made by mcrecco in this event. I'm skipping this event.");
      return false;
    }

    //std::cout<<"flag1"<<std::endl;
        
    
    //for each track in event track
     for(size_t n = 0; n < ev_mctrack->size(); n++){
       // std::cout<<"flag2"<<std::endl;
	
       double currentdist = 0.0;
       double rrange = 0.0;
       //std::cout<<"before"<<std::endl;
      ::larlite::mctrack track = ev_mctrack->at(n);
       if (track.size()>0) {
       
       // std::cout<<"after"<<std::endl;
      double mylength = tracklength(track);

      //for each step in track
      double dx1 = projdistance(track, 1);
      double dx2 =  projdistance(track, 2);
       dEdX_l = dE(track, 1)/dx1;
       if (dx1 > 0) {
	 currentdist += projdistance(track, 1);
	 rrange = mylength- currentdist;
	 projhist->Fill(rrange, dEdX_l);
       }
       dEdX_m = dE(track, 2)/dx2;
       if (dx2 > 0) {
	 currentdist += projdistance(track, 2);
	 rrange = mylength- currentdist;
	 projhist->Fill(rrange, dEdX_m);
       }
       
      for (size_t n = 3; n < track.size()-1; n++){
	//calculate residual range
	currentdist += projdistance(track, n);
	rrange = mylength- currentdist;

	//calculate absolute value dEdx
	double mydE = dE(track, n);
	double dx = projdistance(track, n);

	dEdX_n = mydE/dx;

	if (dEdX_l >5 && dEdX_m > 5 && dEdX_n > 5 && rrange <20){
	  continue;
	}
	
	if (dx <= 0 ) {
	  continue;
	}

	 nentries += 1;
	 projhist->Fill(rrange, dEdX_n);

	 dEdX_l = dEdX_m;
	 dEdX_m = dEdX_n;	 
      }
       }

	}
  
    return true;
  }  
  

  bool TProjMuonDEDX::finalize() {

     if(_fout){
      _fout->cd();
      std::cout << "writing" << std::endl;
      projhist->Write();
      //It's always good to delete things when you're done with them
      delete projhist;
    }
  
    return true;
  }

    //calculates distance between a given step and the step before it in 3D
  double TProjMuonDEDX::projdistance(mctrack track, size_t m){
    mcstep stepn  = track.at(m-1);
    mcstep stepm = track.at(m);
	
	double xn = stepn.X();
	double xm = stepm.X();

	double zn = stepn.Z();
	double zm = stepm.Z();

	double dx = xm-xn;
	double dz = zm-zn;

	//compute distance between steps
	double distance = sqrt((dx*dx) + (dz*dz));
	return distance;
  }

  double TProjMuonDEDX::tracklength(mctrack track){
     double totaldist = 0.0;
    //for each step in each track
     for (size_t m = 1; m < track.size(); m++){
	
	//distance between points
	double mydistance = projdistance(track, m);

	//add to total track length
	totaldist += mydistance;
	//std::cout << distance  << std::endl;
      }
     return totaldist;
    }

  //calculates the absolute value of change in energy per step
  double TProjMuonDEDX:: dE(mctrack track, size_t  m){
    mcstep stepn  = track.at(m-1);
    mcstep stepm = track.at(m);
    
    double Em = stepm.E();
    double En = stepn.E();

    double dE= Em-En;
    return std::abs(dE);
  }

}
#endif
