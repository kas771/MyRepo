#ifndef LARLITE_EFILTER_CXX
#define LARLITE_EFILTER_CXX

#include "efilter.h"
#include "Dataformat/mcshower.h"

namespace larlite {

  bool efilter::initialize() {


    return true;
  }
  
  bool efilter::analyze(storage_manager* storage) {
  
        //get shower data
        ::larlite::event_mcshower *ev_mcshower = storage->get_data< ::larlite::event_mcshower>("mcreco");

    if (ev_mcshower->size() != 1){
      std::cout << "wrong number showers per ev_mcshower" << std::endl;
      return false;
    }

    ::larlite::mcshower shower = ev_mcshower->at(0); 

    if ((shower.DetProfile().E()/shower.Start().E()) < 0.95){
      return false;
    }
  
    return true;
  }

  bool efilter::finalize() {

  
    return true;
  }

}
#endif
