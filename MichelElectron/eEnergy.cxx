#ifndef LARLITE_EENERGY_CXX
#define LARLITE_EENERGY_CXX

#include "eEnergy.h"
#include "TStyle.h"

namespace larlite {

  bool eEnergy::initialize() {


    mygeoutil = larutil::GeometryUtilities::GetME();
    myprop = larutil::DetectorProperties::GetME();
    _tree = new TTree("_tree", "_tree");
    _tree -> Branch("_dep_energy", & _dep_energy, " _dep_energy/D");
    _tree -> Branch("_hits_energy", & _hits_energy, " _hits_energy/D");
     _tree -> Branch("_n_hits_InCircle", & _n_hits_InCircle, "_n_hits_InCircle/I");
     _tree -> Branch("_n_hits_total", &_n_hits_total, " _n_hits_total/I");

    /*
    mydx = new TH1D("dx", "distance from center to hit in x", 100, 0, 100);
    mydz = new TH1D("dz", "distance from center to hit in z", 100, 0, 100);

    myhist = new TH2D("myhist", "location end of muon track", 100, 0, 250, 100, 0, 1000);

    Hitsx = new TH1D("hitsx", "distribution hits in x", 100, 0, 600);

    phist = new TH2D("phist", "total number of steps per track vs. the last step with non-zero momentum", 100, 0, 100, 100, 0, 100);

    phist -> GetXaxis() -> SetTitle("last index with non-zero p");
    phist -> GetYaxis() -> SetTitle("total number steps per track");
    
    histADC =  new TH1D("histADC", "Energy of Hits in Circle", 20, 0, 100);
    histADC -> GetXaxis() -> SetTitle("Energy (MeV)");
    histADC -> GetYaxis() -> SetTitle("counts");
    */

    return true;
  }
  
  bool eEnergy::analyze(storage_manager* storage) {
    int nInCircle = 0;
    double r = 5.0;
    
    double fTimetoCm = mygeoutil->TimeToCm();
    double fWiretoCm = mygeoutil->WireToCm();
    double triggerOffset = myprop -> TriggerOffset();
    
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

       //find last index for step with non-zero momentum
       if (track.size() > 1 ) {
       int n = 0;
       int l = track.size() -1 ;
       while ( n == 0 && l > 0) {
	 mcstep step  = track.at(l);
	 double Px = step.Px();
	 double Pz = step.Pz();

	 if (Px != 0 || Pz !=0) {
	   n = l;
	   // phist -> Fill (n,track.size() );
	 }
	 l -= 1;
	 
	 }

       double Px = track.at(n).Px();
       double Pz = track.at(n).Pz();

       //normalize momentum vector
       double Px1 = 0;
       double Pz1 = 0;
       if (Px == 0){
	 Pz1 = 1;
       }

       if (Pz == 0) {
	 Px1 = 1;
       }

       else {
	 double length = sqrt ( Px*Px + Pz*Pz);
	 Px1 = Px/length;
	   Pz1 = Pz/length;
       }

       //calculate location of center
       double centerX = endX + Px1*r;
       double centerZ = endZ + Pz1*r;


         double ADC = 0.0;
       ::larlite::event_hit *ev_hit = storage->get_data< ::larlite::event_hit>("gaushit");
	 if (ev_hit->size() > 0 ) {
       for(size_t m = 0; m < ev_hit->size(); m++){
	 ::larlite::hit myhit = ev_hit->at(m);
   
	 if (int(myhit.View()) == 2){
    double hitZ = myhit.WireID().Wire *fWiretoCm ;
    double hitX = (myhit.PeakTime()-triggerOffset) * fTimetoCm;
    
    double dx = hitX-centerX;
    double dz = hitZ-centerZ;

    /*
    Hitsx -> Fill(hitX);
    mydx -> Fill(dx);
   mydz ->Fill(dz);
   myhist-> Fill(centerX, centerZ);
    */
  
    if (inCircle(dx, dz, r) == true){
      ADC += myhit.SummedADC();
      nInCircle += 1;
	 }
	 }
    
       }

       //get energy from mcshower
     auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");
     for(auto const& shower : *ev_mcshower){
        //from ADCMeV.cxx
  
       _dep_energy = shower.DetProfile().E();
       _tree -> Fill();
      }

     _hits_energy  =   0.00804069*ADC;
     _n_hits_InCircle = nInCircle;
     _n_hits_total = ev_hit->size();
      _tree -> Fill();
     
    
    // histADC -> Fill(E);
      //std::cout << ev_hit->size() << std::endl;
      //std::cout << nInCircle << std::endl;
	 //std::cout << centerX<< std::endl;
	 // std::cout << centerZ << std::endl;
  
	 }
       }
    return true;
  }

  bool eEnergy::finalize() {

    
    if(_fout){
      _fout->cd();
      _tree-> Write();
      _tree -> Print();
           /*
      histADC ->Write();
      myhist->Write();
      mydx -> Write();
      mydz -> Write();
      Hitsx -> Write();
      phist-> Write();
      phist -> Fit("pol1");
      //It's always good to delete things when you're done with them
      delete myhist;
      delete mydx;
      delete mydz;
      delete Hitsx;
      delete phist;
    
      delete histADC;
  */
      //	delete _tree;
    }
    
    
    return true;
  }

  
  //returns true if a hit is within the radius of the circle
  bool eEnergy::inCircle(double dx, double dz, double r){
   
    double r2 = r*r;
    double dX2 =  dx*dx + dz*dz;
    
    if (r2> dX2){
      // std::cout << "hit in the circle" << std::endl;
      return true;
      
    }

    return false;
}

  void eEnergy:: ResetTree(TTree _tree){
  }
 
  
}
#endif

