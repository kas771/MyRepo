/**
 * \file HitViewer.h
 *
 * \ingroup scratch_ana
 * 
 * \brief Class to read in reconstructed hits and make a TH2 histogram of them
 *
 * @author davidkaleko
 */

/** \addtogroup scratch_ana

    @{*/
#ifndef LARLITE_HITVIEWER_H
#define LARLITE_HITVIEWER_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "Analysis/ana_base.h"
#include "LArUtil/LArUtilManager.h"
#include "DataFormat/hit.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctrajectory.h"
#include "DataFormat/mcshower.h"
#include "TH2D.h"
#include "TEllipse.h"
#include "TPaveText.h"



/* NOTE: this is _not_ an ana_processor module like you are used to.
This started as an empty class (llgen_class_empty).
The way this one works is that the python run script will get the data
from the data file, and hand it to this class for each event.
That's why there are no "initialize", "analyze", "finalize" functions here.
Instead, you should initialize histograms and things in the constructor.
*/

namespace larlite{				
  /**
     \class HitViewer
     User defined class HitViewer ... these comments are used to generate
     doxygen documentation!
  */
  class HitViewer{
    
  public:
    
    /// Default constructor
    HitViewer();
    
    /// Default destructor
    ~HitViewer();

    void GenerateHisto(const larlite::event_hit *ev_hit, const larlite::event_mctrack *ev_mctrack, const larlite::event_mcshower *ev_mcshower, int plane);

    TH2D* GetHitHisto() { return hHitHisto; }
    TEllipse * GetendMuon() {return endMuon; }
    TPaveText * GetBox() {return energyBox;}
    TH2D * GetSmHisto() {return smHisto;}

  protected:

    TH2D* hHitHisto;
    TEllipse * endMuon;
    TPaveText * energyBox;
    TH2D * smHisto;
    
    const larutil::GeometryUtilities* mygeoutil;
    const larutil::DetectorProperties *myprop;
    double fTimetoCm;
    double fWiretoCm;
    double triggerOffset;
    double r1;
    double r2;
    double radius_factor;
    double zoom;
    double bin_factor;
 
  };

}//end namespace larlite  
#endif
  /** @} */ // end of doxygen group 
  
