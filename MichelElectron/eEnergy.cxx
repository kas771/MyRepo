#ifndef LARLITE_EENERGY_CXX
#define LARLITE_EENERGY_CXX

#include "eEnergy.h"

namespace larlite {

  bool eEnergy::initialize() {


    mygeoutil = larutil::GeometryUtilities::GetME();

    myhist = new TH2D("myhist", "hit positions", 100, 0, 1000, 100, 0, 1000);
    
    return true;
  }
  
  bool eEnergy::analyze(storage_manager* storage) {
    int nInCircle = 0;
    double r = 5.0;
    
    double fTimetoCm = mygeoutil->TimeToCm();
    double fWiretoCm = mygeoutil->WireToCm();
    
    ::larlite::event_mctrack *ev_mctrack = storage->get_data< ::larlite::event_mctrack>("mcreco");

    
    //Let's check to make sure that the hits were successfully read in
    if(!ev_mctrack){
      print(msg::kERROR,__FUNCTION__,"Uh oh, I didn't find any mctracks made by mcrecco in this event. I'm skipping this event.");
      return false;
    }

    //also check that there's only one
    if(ev_mctrack->size() != 1 ) {
      std::cout << "event that doesn't have one track" << std::endl;
      return false;
    }

    //for each track in event track
      ::larlite::mctrack track = ev_mctrack->at(0);
       mcstep endpoint = track.End();
       double endX = endpoint.X();
       double endZ = endpoint.Z();

       double Px = endpoint.Px();
       double Pz = endpoint.Pz();

       double centerX = endX + Px*r;
       double centerZ = endZ + Pz*r;

       ::larlite::event_hit *ev_hit = storage->get_data< ::larlite::event_hit>("gaushit");
       for(size_t m = 0; m < ev_hit->size(); m++){
      ::larlite::hit myhit = ev_hit->at(m);

    double hitZ = myhit.WireID().Wire *fWiretoCm ;
    double hitX = myhit.PeakTime() * fTimetoCm;
    
    double dx = hitX-centerX;
    double dz = hitZ-centerZ;

    myhist ->Fill (dx, dz);
  
    if (inCircle(dx, dz, r) == true){
	   nInCircle += 1;
	 }
    }
       
     	 std::cout << ev_hit->size() << std::endl;
	 std::cout << nInCircle << std::endl;
	 //std::cout << centerX<< std::endl;
	 // std::cout << centerZ << std::endl;
  

    return true;
  }

  bool eEnergy::finalize() {

    if(_fout){
      _fout->cd();
      myhist->Write();
      //It's always good to delete things when you're done with them
      delete myhist;
    }
    
    return true;
  }

  
  //returns true if a hit is within the radius of the circle
  bool eEnergy::inCircle(double dx, double dz, double r){
   
    double r2 = r*r;
    double dX2 =  dx*dx + dz*dz;
    
    if (r2> dX2){
      std::cout << "hit in the circle" << std::endl;
      return true;
      
    }

    return false;
}
 
  
}
#endif

