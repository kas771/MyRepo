/**
 * \file michelfilter.h
 *
 * \ingroup MichelElectron
 * 
 * \brief Class def header for a class michelfilter
 *
 * @author kathrynsutton
 */

/** \addtogroup MichelElectron

    @{*/

#ifndef LARLITE_MICHELFILTER_H
#define LARLITE_MICHELFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class michelfilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class michelfilter : public ana_base{
  
  public:

    /// Default constructor
    michelfilter(){ _name="michelfilter"; _fout=0;}

    /// Default destructor
    virtual ~michelfilter(){}

    /** IMPLEMENT in michelfilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in michelfilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in michelfilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    size_t total_evts;
    size_t kept_evts;

    
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
