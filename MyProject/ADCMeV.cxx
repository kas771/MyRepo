#ifndef LARLITE_ADCMEV_CXX
#define LARLITE_ADCMEV_CXX

#include "ADCMeV.h"
#include "DataFormat/hit.h"
#include "DataFormat/mcshower.h"

// fit for ADC to MeV conversion: y =  0.00695746 x + 44.6731 where x is ADC and y is MeV
//doesn't look great, to improve later

namespace larlite {

  bool ADCMeV::initialize() {

    //myhist = new TH2D("myhist", "ADC hits vs. MeV showers", 100, 0, 250000, 100, 0, 1200);
    myhist = new TH2D("myhist", "ADC hits vs. MeV showers", 100, 10000, 75000, 100, 100, 600);
    myhist->GetYaxis()->SetTitle("Deposited energy MeV");
    myhist->GetXaxis()->SetTitle("Summed ADC");
    
    return true;
  }
  
  bool ADCMeV::analyze(storage_manager* storage) {

    //get hit data
        ::larlite::event_hit *ev_hit = storage->get_data< ::larlite::event_hit>("gaushit");

        //get shower data
        ::larlite::event_mcshower *ev_mcshower = storage->get_data< ::larlite::event_mcshower>("mcreco");

    //Let's check to make sure that the hits were successfully read in
    if(!ev_hit){
      print(msg::kERROR,__FUNCTION__,"Uh oh, I didn't find hits made by gaushit in this event. I'm skipping this event.");
      return false;
    }

    float sumADC = 0.0;
    for(size_t n = 0; n < ev_hit->size(); n++){
      ::larlite::hit ihit = ev_hit->at(n);
      int plane = int(ihit.View());
      if (plane == 2){
       float ADC = ihit.SummedADC();
       sumADC += ADC;
      }
    }

       ::larlite::mcshower shower = ev_mcshower->at(0);
       double E = shower.DetProfile().E();

       // if (E < 300 ){
	 myhist->Fill(sumADC, E);
	 //}
     

    return true;
  }

  bool ADCMeV::finalize() {

    
     //linear fit
    myhist->Fit("pol1");

    /*TF1 *myfit = new TF1("myfit","[0]x");
    myfit->SetParName(0,"m");
    myfit->SetParameter(0, 0.005);
     myhist->ProfileX() -> Fit("myfit");
    */

   if(_fout){
      _fout->cd();
      myhist->Write();
      //It's always good to delete things when you're done with them
      delete myhist;
    }
 
    return true;
  }

}
#endif
