#ifndef LARLITE_HITVIEWER_CXX
#define LARLITE_HITVIEWER_CXX

#include "HitViewer.h"

namespace larlite{

  //Constructor (called when you create a HitViewer object)
  HitViewer::HitViewer()
  {
    //Initializing pointers to zero is generally good practice.
    hHitHisto = 0;
    //Geometry utilities
     myprop = larutil::DetectorProperties::GetME();
    mygeoutil = larutil::GeometryUtilities::GetME();
    fTimetoCm = mygeoutil->TimeToCm();
    fWiretoCm = mygeoutil->WireToCm();
    triggerOffset = myprop -> TriggerOffset();


  }

  //Destructor (called at the very end, to save output etc)
  HitViewer::~HitViewer()
  {
    //Delete histograms when you're done with them!
    if(hHitHisto) delete hHitHisto;
  }
  /*
     //get end of track position
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

       ::larlite::mctrack track = ev_mctrack->at(0);
       mcstep endpoint = track.End();
       double endTime = endpoint.X();
       double endWire = endpoint.Z();
  */

  void HitViewer::GenerateHisto(const larlite::event_hit *ev_hit, int plane){
    // std::cout <<triggerOffset  << std::endl;

    //First we loop over the hits to determine the histogram max and min
    //(so the histogram is automatically zoomed in to the right region for
    //this event)

    int    _min_wire = 1e5;
    int    _max_wire = -1e5;
    double _min_time = 1e5;
    double _max_time = -1e5;

    //std::cout<< "flag1" << std::endl;

    //This is another way to write a for-loop
    //I'm still just looping over the "ev_hit" vector
    //This might not compile if you don't have C++11
    //in which case, change it to a for-loop you're used to
    for (auto const &ihit : *ev_hit){

      //Skip hits that aren't in the plane we're trying to draw
      if( (int)ihit.View() != plane )
	continue;

      double mytime  = ihit.PeakTime()- triggerOffset;

      if (mytime < _min_time)    _min_time = mytime;
      if (mytime > _max_time)    _max_time = mytime;
      if (ihit.WireID().Wire < _min_wire) _min_wire = ihit.WireID().Wire;
      if (ihit.WireID().Wire > _max_wire) _max_wire = ihit.WireID().Wire;

					 
    }//End first loop over hits

    // std::cout << "flag2" <<std::endl;
    
    //Now we have the bounds, let's define the histogram
    //If the histogram already exists (from a previous event), let's delete it
    if(hHitHisto) delete hHitHisto;
    int n_bins_wire = std::abs(_max_wire - _min_wire);
    int n_bins_time = std::abs((_max_time - _min_time)*fTimetoCm);

    // std::cout << "flag3" <<std::endl;

    hHitHisto = new TH2D("hHitHisto",
			 Form("Event %d, Plane %d;Wire [cm]; Time [cm]",ev_hit->event_id(),plane),
			 n_bins_wire,_min_wire*fWiretoCm*0.9,_max_wire*fWiretoCm*1.1,
			 n_bins_time,_min_time*fTimetoCm*0.9,_max_time*fTimetoCm*1.1);

    //Now we loop again over the hits and fill this histo
    for (auto const &ihit : *ev_hit){
      
      //Skip hits that aren't in the plane we're trying to draw
      if( (int)ihit.View() != plane )
	continue;

      //Fill the histogram, weighted by the hits summed ADC
      //that way "bigger" hits are drawn more pronounced
      //when you draw the histo with the "COLZ" option

      // std::cout << "flag4" << std::endl;

      hHitHisto->Fill(ihit.WireID().Wire*fWiretoCm,
		      ( ihit.PeakTime()-triggerOffset) *fTimetoCm,
		      ihit.SummedADC() );
      

    }//End second loop over hits to fill histogram

  }//end GenerateHisto function

}//end namespace larlite
#endif
