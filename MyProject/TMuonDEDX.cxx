#ifndef LARLITE_TMUONDEDX_CXX
#define LARLITE_TMUONDEDX_CXX

//#include "Math.h"
#include "TMuonDEDX.h"


namespace larlite {

  bool TMuonDEDX::initialize() {

    //    myhist = new TH2D("myhist", "Residual Range vs. dE/dX for mctracks", 6670, 0, 250, 6670, 0, 100);
    myhist = new TH2D("myhist", "Residual Range vs. dE/dX for Muon Truth Events in 3D", 400, 0, 50, 400, 0, 10);
    myhist->GetXaxis()->SetTitle("Residual Range (cm)");
    myhist->GetYaxis()->SetTitle("dE/dX (MeV/cm)");

    histdE = new TH1D("histdE", "dE", 100, 0, 50);
    histdX = new TH1D("histdX", "dX", 100, 0, 50);
    
    nentries = 0;

    return true;
  }
  
  bool TMuonDEDX::analyze(storage_manager* storage) {
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //

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
      for (size_t m = 1; m < track.size()-1; m++){
	//calculate residual range
	currentdist += distance(track, m);
	rrange = mylength- currentdist;

	//calculate absolute value dEdx
	double mydE = dE(track, m);
	double dx = distance(track, m);
	double dEdX;
	if (dx > 0 ) {
	  dEdX = mydE/dx;
	  nentries += 1;
	}
	else {dEdX = 0;}
	//std::cout<< dEdX <<std::endl;
	
	
	if (dEdX > 0) {  
	 myhist->Fill(rrange, dEdX);
	 histdE->Fill(mydE);
	 histdX->Fill(dx);
	}


      }
       }

	}
  
    return true;
  }

  bool TMuonDEDX::finalize() {

         std::cout<<"i'm couting here"<<std::endl;
        std::cout << nentries  << std::endl;

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //

      if(_fout){
      _fout->cd();
      myhist->Write();
      histdE -> Write();
      histdX -> Write();
      //It's always good to delete things when you're done with them
      delete myhist;
      delete histdE;
      delete histdX;
    }
  
    return true;
  }

  //calculates distance between a given step and the step before it in 3D
  double TMuonDEDX::distance(mctrack track, size_t m){
    mcstep stepn  = track.at(m-1);
    mcstep stepm = track.at(m);
	
	double xn = stepn.X();
	double xm = stepm.X();

	double yn = stepn.Y();
	double ym = stepm.Y();

	double zn = stepn.Z();
	double zm = stepm.Z();

	double dx = xm-xn;
	double dy = ym-yn;
	double dz = zm-zn;

	//compute distance between steps
	double distance = sqrt((dx*dx) +(dy*dy) + (dz*dz));
	return distance;
  }

  double TMuonDEDX::tracklength(mctrack track){
     double totaldist = 0.0;
    //for each step in each track
     for (size_t m = 1; m < track.size(); m++){
	
	//distance between points
	double mydistance = distance(track, m);

	//add to total track length
	totaldist += mydistance;
	//std::cout << distance  << std::endl;
      }
     return totaldist;
    }

  //calculates the absolute value of change in energy per step
  double TMuonDEDX:: dE(mctrack track, size_t  m){
    mcstep stepn  = track.at(m-1);
    mcstep stepm = track.at(m);
    
    double Em = stepm.E();
    double En = stepn.E();

    double dE= Em-En;
    return std::abs(dE);
  }

  
}
#endif
