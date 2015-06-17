/**
 * \file LeslieSampleAna.h
 *
 * \ingroup testpkg
 * 
 * \brief Class def header for a class LeslieSampleAna
 *
 * @author kaleko
 */

/** \addtogroup testpkg

    @{*/

#ifndef LARLITE_LESLIESAMPLEANA_H
#define LARLITE_LESLIESAMPLEANA_H

#include "Analysis/ana_base.h"
#include "TH1.h"

namespace larlite {
  /**
     \class LeslieSampleAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class LeslieSampleAna : public ana_base{
  
  public:

    /// Default constructor
    /// This is called immediately after you create an instance of this class
    /// It's good practice to initialize pointer histograms to zero here
    /// Also you can set default parameter values here if you want.
    LeslieSampleAna(){ 
      _name="LeslieSampleAna"; 
      _fout=0;
      myhist=0;
      _debug=false;
    }

    /// Default destructor
    virtual ~LeslieSampleAna(){}

    /** IMPLEMENT in LeslieSampleAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in LeslieSampleAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in LeslieSampleAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    //This is a (public) function to set the debug flag.
    //Since it is public, I can call it from my python run script.
    void SetDebugFlag(bool input) { _debug = input; }

  protected:

    //This is declaring the histogram that I want to fill
    TH1F *myhist;

    //This is a flag that I can set from my python run script... by default it is false,
    //but if I set it to true it will cout things to the screen.
    bool _debug;

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
