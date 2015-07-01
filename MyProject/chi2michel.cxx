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

    cutoff = 100000;

    // myhist = new TH2D("myhist", "Position of Hits", 50, 0, 1000, 50, 200, 600);
    

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

    //catch tracks with no steps
     larlite::mctrack track = ev_mctrack->at(0);
    if (track.size() <= 0 ){
       print(msg::kERROR,__FUNCTION__,"Uh oh, there were no steps in this track.");
       return false;
    }

    //find location of start point from track
  

    larlite::mcstep start = track.Start();
    double xstart = start.X();
    double zstart = start.Z();

     std::cout <<zstart << std::endl;
    std::cout <<xstart << std::endl;
    

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

   //if no matches found
   if (match == false) {
     std::cout << "there was no correspond hit to the initial mcstep" << std::endl;
     std::cout << xstart << std::endl;
     std::cout << zstart << std::endl;
   }

   else {
}
   */
   //find the start hit using the closest to the start mcstep
   int m= 0;
   ::larlite::hit firsthit;
   while (1){
   firsthit = ev_hit->at(m);
    int plane = int(firsthit.View());
     if (plane == 2){
       break;
     }
     m++;
   }
   
   double mydistance = distance(firsthit, xstart, zstart);
     
   for (int n = m+1; n < ev_hit->size(); n++){
     ::larlite::hit myhit = ev_hit->at(n);
     int plane = int(myhit.View());
     if (plane == 2){
       double newdistance = distance(myhit, xstart, zstart);
       if (newdistance < mydistance) {
	 m=n;
     }
     }
   }
   


   //create empty array for ordered indices of hits
   std::vector<int> ind;
   //starting at first point
   ind.push_back(m);

   //create array of original indices
   int nhits = ev_hit->size();
   std::vector<int> original;
   //fill array excluding the start point
   for (int i = 0; i < nhits; i++){
     int plane = int(ev_hit->at(i).View());
     if (plane == 2 && i!=m ){
       original.push_back(i);
     }
   }

   /*
   printvec(ind);
   printvec(original);
   */
   
   ::larlite::hit hit0 = ev_hit->at(m);
   //find  second point
   ::larlite::hit hit1 = ev_hit->at(original.at(0));
   
   //calculate distance between points
   double distance1 = distance(hit0, hit1);

 
   //while vector of original order still exists
   while( original.size() > 0){

     // std::cout <<"flag1" <<std::endl;

     int j = 0;
     bool close = false;
     if (original.size() >= 2){
     //for each remaining hit
         printvec(original);
     for (int i = 1; i < original.size(); i ++){

       // std::cout <<"flag2" <<std::endl;
 
      //if the index of this hit isn't in the index array
 
       ::larlite::hit myhit = ev_hit->at(original.at(i));
        int plane = int(myhit.View());
	if (plane == 2 ){
       
      //calculate the distance from this one to the previous hit
      double mydistance = distance(myhit, hit0);
      
      //if it's the shortest distance, this is the next hit
      // close = false;
      if (mydistance < distance1){
	j = i;
      
	if (mydistance < cutoff*cutoff){
	  close = true;
	}
      }
	}
     }
     }
     
     // std::cout <<"flag3" <<std::endl;
   
     larlite::hit thishit = ev_hit->at(original.at(j));
     mydistance = distance(hit0, thishit);
     std::cout <<mydistance <<std::endl;
      std::cout << j  <<std::endl;


     
    if (close == true){
   //add this to the ordered array
     ind.push_back(original.at(j));
    }

    close = false;

    //set new last point
    hit0 = ev_hit->at(ind.back());
    //remove that from original array
   original.erase(original.begin() + j);

   //         std::cout <<"flag4" <<std::endl;
   if (original.size()>0){
    //set new second point
    hit1 = ev_hit->at(original.at(0));
    //calculate new distance between last and second points
    distance1 = distance(hit0, hit1);
   
    std::string h0 = std::to_string(ind.back());
     std::string h1 = std::to_string(original.at(0));
     std::cout << "hit0 = " + h0 +  ", hit1 = "+ h1 <<std::endl;
	 }


	 // std::cout <<"flag5" <<std::endl;

    }
   
   // ind.push_back(original.at(0));

  
   printvec(ind);
  // printvec(original);

   //graph the hits
  Int_t n = ind.size();
  Double_t xpos[n];
  Double_t zpos[n];
  
   for(Int_t i = 0; i < n; i ++){
     int indexhit = ind.at(i);
     larlite::hit myhit = ev_hit->at(indexhit);
     xpos[i] = (myhit.PeakTime()- triggerOffset)*fTimetoCm;
     zpos[i] = myhit.WireID().Wire*fWiretoCm;
     //myhist ->Fill (zpos, xpos);
   }

   graph = new TGraph(n, zpos, xpos);
   std::cout << n << std::endl;
   /*
    std::cout << distance(ev_hit->at(344), ev_hit->at(343)) << std::endl;
    std::cout << distance(ev_hit->at(343), ev_hit->at(342)) << std::endl;
    std::cout << distance(ev_hit->at(342), ev_hit->at(341)) << std::endl;
    std::cout << distance(ev_hit->at(341), ev_hit->at(340)) << std::endl;
    std::cout << distance(ev_hit->at(342), ev_hit->at(340)) << std::endl;

    larlite::hit problem = ev_hit -> at(342);
    double x = (problem.PeakTime()- triggerOffset)*fTimetoCm;
    double z = problem.WireID().Wire*fWiretoCm;

    std::cout <<z <<std::endl;
       std::cout <<x <<std::endl;
       */


   /*
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
   */
   
    return true;
  }

  bool chi2michel::finalize() {
     if(_fout){
      _fout->cd();
      graph->Write();
      delete graph;

 
      // myhist->Write();
      // delete myhist;
     }

    return true;
  }

  void chi2michel::printvec(std::vector<int> v){
    std::string mystring("{ ");
    for (int i = 0; i <v.size(); i++){
      mystring.append(std::to_string(v.at(i)));
      mystring.append(", ");
    }
    mystring.append("}");
    std::cout<< mystring << std::endl;
  }

  double chi2michel:: distance(hit myhit, double xstart, double zstart){
     double xhit = (myhit.PeakTime() - triggerOffset) * fTimetoCm;
     double zhit = myhit.WireID().Wire * fWiretoCm;
     double dx = xstart-xhit;
     double dz = zstart- zhit;

     double newdistance = dx*dx + dz*dz;
     return newdistance;
  }

   double chi2michel:: distance(hit hit0, hit hit1 ){
     double xhit0 = (hit0.PeakTime()- triggerOffset) * fTimetoCm;
     double zhit0 = hit0.WireID().Wire* fWiretoCm;
     double xhit1 = (hit1.PeakTime()- triggerOffset) * fTimetoCm;
     double zhit1 = hit1.WireID().Wire* fWiretoCm;
       
     double dx = xhit1-xhit0;
     double dz = zhit1 - zhit0;

     double newdistance = dx*dx + dz*dz;
     return newdistance;
  }

}
#endif
