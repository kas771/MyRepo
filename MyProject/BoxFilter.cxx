#ifndef LARLITE_BOXFILTER_CXX
#define LARLITE_BOXFILTER_CXX

#include "BoxFilter.h"

namespace larlite {

  bool BoxFilter::initialize() {

  //Set DistToBoxWall's "box" to be TPC 
    
    _myGeoAABox.Min( 0,
		     -(::larutil::Geometry::GetME()->DetHalfHeight()),
		     0);
    
    _myGeoAABox.Max ( 2*(::larutil::Geometry::GetME()->DetHalfWidth()),
		     ::larutil::Geometry::GetME()->DetHalfHeight(),
		     ::larutil::Geometry::GetME()->DetLength());
   
    return true;
  }
  
  bool BoxFilter::analyze(storage_manager* storage) {

        ::larlite::event_mctrack *ev_mctrack = storage->get_data< ::larlite::event_mctrack>("mcreco");

       //Let's check to make sure that the hits were successfully read in
    if(!ev_mctrack){
      print(msg::kERROR,__FUNCTION__,"Uh oh, I didn't find any mctracks made by mcrecco in this event. I'm skipping this event.");
      return false;
    }


    //for each track in the event
    for(size_t n = 0; n < ev_mctrack->size(); n++){
     ::larlite::mctrack track = ev_mctrack->at(n);
    
       //Make sure track MC start point and MC end point are in active volume
     return isFullyContained(track);
      
   
  }
  }

  bool BoxFilter::finalize() {

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
  
    return true;
  }

    
     bool BoxFilter::isFullyContained(mctrack mytrack){

      if(_myGeoAABox.Contain(mytrack.Start().Position()) > 0 &&
       _myGeoAABox.Contain(mytrack.End().Position()) > 0) {

      return true;
    }
 
    return false;
     }

}
#endif
