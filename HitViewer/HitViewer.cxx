#ifndef LARLITE_HITVIEWER_CXX
#define LARLITE_HITVIEWER_CXX

#include "HitViewer.h"

namespace larlite{

  //Constructor (called when you create a HitViewer object)
  HitViewer::HitViewer()
  {
    //Initializing pointers to zero is generally good practice.
    hHitHisto = 0;
    endMuon = 0;
    energyBox = 0;
    smHisto = 0;
    r1 = 0;
    r2 = 0;

    //arbitrary - gives size of ellipse
    radius_factor = 100;

    //how close to be in the smaller histogram
    zoom = 10;
    bin_factor = 5;
    
    //Geometry utilities
    myprop = larutil::DetectorProperties::GetME();
    mygeoutil = larutil::GeometryUtilities::GetME();

    //constants
    fTimetoCm = mygeoutil->TimeToCm();
    fWiretoCm = mygeoutil->WireToCm();
    triggerOffset = myprop -> TriggerOffset();

  }

  //Destructor (called at the very end, to save output etc)
  HitViewer::~HitViewer()
  {
    //Delete histograms when you're done with them!
    if(hHitHisto) delete hHitHisto;
    if (endMuon) delete endMuon;
    if (energyBox) delete energyBox;
    if (smHisto) delete smHisto;
  }
  
  void HitViewer::GenerateHisto(const larlite::event_hit *ev_hit, const larlite::event_mctrack *ev_mctrack, const larlite::event_mcshower *ev_mcshower, int plane){
    
    double Emu = 0;
    double Ee = 0;
    //find electron energy
    mcshower shower = ev_mcshower-> at(0);
    Ee  = shower.DetProfile().E();
    
   
    //find muon end point and energy
       ::larlite::mctrack track = ev_mctrack->at(0);
       mcstep endpoint = track.End();
       double endTime = endpoint.X();
       double endWire = endpoint.Z();

       for (auto const &step: track){
	 Emu += step.E();
       }

    energyBox = new TPaveText();

    std::string Emu1 = std::to_string(Emu);
    std::string Ee1 = std::to_string(Ee);
    std::string text ("E muon in Mev: " + Emu1 + ",  E electron in Mev: "+ Ee1);
    std::cout << text << std::endl;
    
    energyBox -> AddText(text.c_str());
    energyBox-> SetTextSize(.03);
    
 

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

    double wiremin = _min_wire*fWiretoCm*0.9;
    double wiremax = _max_wire*fWiretoCm*1.1;
    double timemin = _min_time*fTimetoCm*0.9;
    double timemax = _max_time*fTimetoCm*1.1;
    hHitHisto = new TH2D("hHitHisto",
			 Form("Event %d, Plane %d;Wire [cm]; Time [cm]",ev_hit->event_id(),plane),
			 n_bins_wire,wiremin ,wiremax,
			 n_bins_time, timemin,timemax);

    //give center and radius to ellipse for muon end point
    r2 = (timemax-timemin)/radius_factor;
    r1 = r2 * (wiremax-wiremin)/(timemax-timemin);

    endMuon = new TEllipse (endWire, endTime, r2, r2);

    double smWmin = endWire-zoom;
    double smWmax =  endWire + zoom;
    double smTmin = endTime -zoom;
    double smTmax = endTime +zoom;
    smHisto = new TH2D("smHisto", "End of the Muon Track", n_bins_wire/bin_factor, smWmin, smWmax, n_bins_time/bin_factor, smTmin, smTmax);


    //Now we loop again over the hits and fill this histo
    for (auto const &ihit : *ev_hit){
      
      //Skip hits that aren't in the plane we're trying to draw
      if( (int)ihit.View() != plane )
	continue;

      double hitWire =  ihit.WireID().Wire*fWiretoCm;
      double hitTime = (ihit.PeakTime()-triggerOffset) *fTimetoCm;
      hHitHisto->Fill(hitWire, hitTime, ihit.SummedADC() );

      //fill zoomed in histogram
      if (hitWire > smWmin && hitWire < smWmax
	  && hitTime > smTmin && hitTime < smTmax){
	smHisto->Fill(hitWire, hitTime, ihit.SummedADC() );
      }

      smHisto -> GetXaxis () -> SetTitle ("Wire[cm]");
      smHisto -> GetYaxis () -> SetTitle ("Time[cm]");

    }//End second loop over hits to fill histogram


  }//end GenerateHisto function

}//end namespace larlite
#endif
