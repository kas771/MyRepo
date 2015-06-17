#ifndef LARLITE_EENERGY_CXX
#define LARLITE_EENERGY_CXX

#include "eEnergy.h"

namespace larlite {

  bool eEnergy::initialize() {
    double r = 5;
    mygeoutil = larutil::GeometryUtilities::GetME();
    
    return true;
  }
  
  bool eEnergy::analyze(storage_manager* storage) {
    int nInCircle = 0;
    
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
	 if (inCircle(myhit, centerX, centerZ, fTimetoCm, fWiretoCm) == true){
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

  
    return true;
  }

  
  //returns true if a hit is within the radius of the circle
  bool eEnergy::inCircle(hit myhit, double centerX, double centerZ,  double fTimetoCm, double fWiretoCm){
    double hitX = myhit.PeakAmplitude() *fWiretoCm  ;
    double hitZ = myhit.PeakTime() * fTimetoCm;
    
    double dx = hitX-centerX;
    double dz = hitZ-centerZ;

    
    //  std::cout << dx << std::endl;
    //   std::cout << dz << std::endl;
    
    if (r*r > dx*dx + dz*dz){
      return true;
    }

    return false;
}
 
  
}
#endif

