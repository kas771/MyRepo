#ifndef LARLITE_EENERGY_CXX
#define LARLITE_EENERGY_CXX

#include "eEnergy.h"

namespace larlite {

  bool eEnergy::initialize() {
    double r = 5;

    return true;
  }
  
  bool eEnergy::analyze(storage_manager* storage) {

    
    ::larlite::event_mctrack *ev_mctrack = storage->get_data< ::larlite::event_mctrack>("mcreco");

    
    //Let's check to make sure that the hits were successfully read in
    if(!ev_mctrack){
      print(msg::kERROR,__FUNCTION__,"Uh oh, I didn't find any mctracks made by mcrecco in this event. I'm skipping this event.");
      return false;
    }

    //also check that there's only one
    if(ev_track->size() != 1 ) {
      std::cout << "event that doesn't have one track" << std::endl;
      return false;
    }

    //for each track in event track
     for(size_t n = 0; n < ev_mctrack->size(); n++){
      ::larlite::mctrack track = ev_mctrack->at(n);
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
	 if (inCircle(myhit, centerX, centerZ) == true){
	 }
     }

    return true;
  }

  bool eEnergy::finalize() {

  
    return true;
  }

  //returns true if a hit is within the radius of the circle
  bool eEnergy::inCircle(hit myhit, double centerX, double centerZ){
    double hitX = ;
    double hitZ = ;

    double dx = hitX-centerX;
    double dz = hitz-centerz;
    
    if (r*r > dx*dx + dz*dz){
      return true;
    }

    return false;
}
  
}
#endif

