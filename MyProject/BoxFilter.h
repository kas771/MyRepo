/**
 * \file BoxFilter.h
 *
 * \ingroup MyProject
 * 
 * \brief Class def header for a class BoxFilter
 *
 * @author kathrynsutton
 */

/** \addtogroup MyProject

    @{*/

#ifndef LARLITE_BOXFILTER_H
#define LARLITE_BOXFILTER_H

#include "Analysis/ana_base.h"
#include "GeoAlgo/GeoAABox.h"
#include "LArUtil/Geometry.h"
#include "DataFormat/mctrack.h"

namespace larlite {
  /**
     \class BoxFilter
     User custom analysis class made by SHELL_USER_NAME
   */
  class BoxFilter : public ana_base{
  
  public:

    /// Default constructor
    BoxFilter(){ _name="BoxFilter"; _fout=0;}

    /// Default destructor
    virtual ~BoxFilter(){}

    /** IMPLEMENT in BoxFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in BoxFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in BoxFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    bool isFullyContained(mctrack mytrack);

  protected:
        geoalgo::AABox _myGeoAABox;

    
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
