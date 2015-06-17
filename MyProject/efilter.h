/**
 * \file efilter.h
 *
 * \ingroup MyProject
 * 
 * \brief Class def header for a class efilter
 *
 * @author kathrynsutton
 */

/** \addtogroup MyProject

    @{*/

#ifndef LARLITE_EFILTER_H
#define LARLITE_EFILTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class efilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class efilter : public ana_base{
  
  public:

    /// Default constructor
    efilter(){ _name="efilter"; _fout=0;}

    /// Default destructor
    virtual ~efilter(){}

    /** IMPLEMENT in efilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in efilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in efilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

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
