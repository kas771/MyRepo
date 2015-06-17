/**
 * \file ADCMeV.h
 *
 * \ingroup MyProject
 * 
 * \brief Class def header for a class ADCMeV
 *
 * @author kathrynsutton
 */

/** \addtogroup MyProject

    @{*/

#ifndef LARLITE_ADCMEV_H
#define LARLITE_ADCMEV_H

#include "Analysis/ana_base.h"
#include "TH2.H"
#include "TProfile.h"
#include "TF1.h"


namespace larlite {
  /**
     \class ADCMeV
     User custom analysis class made by SHELL_USER_NAME
   */
  class ADCMeV : public ana_base{
  
  public:

    /// Default constructor
    ADCMeV(){ _name="ADCMeV"; _fout=0;}

    /// Default destructor
    virtual ~ADCMeV(){}

    /** IMPLEMENT in ADCMeV.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ADCMeV.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ADCMeV.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    TH2D *myhist;

  protected:
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
