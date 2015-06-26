#ifndef LARLITE_CHI2MICHEL_CXX
#define LARLITE_CHI2MICHEL_CXX

#include "chi2michel.h"

namespace larlite {

  bool chi2michel::initialize() {
        
    //Geometry utilities
    myprop = larutil::DetectorProperties::GetME();
    mygeoutil = larutil::GeometryUtilities::GetME();

    //constants
    fTimetoCm = mygeoutil->TimeToCm();
    fWiretoCm = mygeoutil->WireToCm();
    triggerOffset = myprop -> TriggerOffset();

    position_error = 1;
    n_matches = 0;
    

    return true;
  }
  
  bool chi2michel::analyze(storage_manager* storage) {

    //to find the start of the track
    //firt read in track info
    ::larlite::event_mctrack *ev_mctrack = storage->get_data< ::larlite::event_mctrack>("mcreco");

    //check to make sure that the hits were successfully read in
    if(!ev_mctrack){
      print(msg::kERROR,__FUNCTION__,"Uh oh, I didn't find any mctracks made by mcrecco in this event. I'm skipping this event.");
      return false;
    }

    //check for one track per event
    if (ev_mctrack->size() != 1 ){
      print(msg::kERROR,__FUNCTION__,"Uh oh, there was more either more than one or no tracks in this event");
      return false;
    }

    //find location of start point from track
    larlite::mctrack track = ev_mctrack->at(0);
    //catch tracks with no steps
    if (track.size() <= 0 ){
       print(msg::kERROR,__FUNCTION__,"Uh oh, there were no steps in this track.");
       return false;
    }

    larlite::mcstep start = track.Start();
    double xstart = start.X();
    double zstart = start.Z();

    //now read in the hits from the event
   ::larlite::event_hit *ev_hit = storage->get_data< ::larlite::event_hit>("gaushit");

   //Let's check to make sure that the hits were successfully read in
   if(!ev_hit){
      print(msg::kERROR,__FUNCTION__,"Uh oh, I didn't find hits made by gaushit in this event. I'm skipping this event.");
      return false;
    }

   /*
   //now lets find the hit corresponding to the start of the track
   int n = 0; //index of hit that matches the start of the track
   bool match = false;
 
   
   while (match == false && n < ev_hit->size()-1){
     ::larlite::hit myhit = ev_hit->at(n);
     double xhit = (myhit.PeakTime() - triggerOffset) * fTimetoCm;
     double zhit = myhit.WireID().Wire * fWiretoCm;

     
      if (xhit <= xstart + position_error  && xhit >= xstart - position_error
	  && zhit <= zstart + position_error && zhit >=  zstart - position_error ){
       match = true;
     } 

     n += 1;
   }
  */
   int m= 0;
   ::larlite::hit firsthit = ev_hit->at(m);
   double mydistance = distance(firsthit, xstart, zstart);;
   for (int n = 1; n < ev_hit->size(); n++){
     ::larlite::hit myhit = ev_hit->at(n);
     double newdistance = distance(myhit, xstart, zstart);
     if (newdistance < mydistance) {
       m=n;
     }
   }

   /*
   //if no matches found
   if (match == false) {
     std::cout << "there was no correspond hit to the initial mcstep" << std::endl;
     std::cout << xstart << std::endl;
     std::cout << zstart << std::endl;
   }

   else {
   */
     n_matches += 1;
      std::cout << "success!" << std::endl;
     std::cout << n_matches << std::endl;

      
     std::cout << m << std::endl;
     std::cout << ev_hit->size() << std::endl;
     
     std::cout << xstart << std::endl;
     std::cout << zstart << std::endl;

     ::larlite::hit myhit = ev_hit->at(m);
     double xhit = (myhit.PeakTime() - triggerOffset) * fTimetoCm;
     double zhit = myhit.WireID().Wire * fWiretoCm;
     std::cout << xhit << std::endl;
     std::cout <<zhit << std ::endl;
     //  }

   
   
   
    return true;
  }

  bool chi2michel::finalize() {

    return true;
  }

  double chi2michel:: distance(hit myhit, double xstart, double zstart){
     double xhit = (myhit.PeakTime() - triggerOffset) * fTimetoCm;
     double zhit = myhit.WireID().Wire * fWiretoCm;
     double dx = xstart-xhit;
     double dz = zstart- zhit;

     double newdistance = dx*dx + dz*dz;
     return newdistance;
  }

}
#endif
