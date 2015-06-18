/**
 * \file eEnergy.h
 *
 * \ingroup MichelElectron
 * 
 * \brief Class def header for a class eEnergy
 *
 * @author kathrynsutton
 */

/** \addtogroup MichelElectron

    @{*/

#ifndef LARLITE_EENERGY_H
#define LARLITE_EENERGY_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctrajectory.h"
#include "LArUtil/LArUtilManager.h"
#include "DataFormat/hit.h"
#include "TH2.h"

namespace larlite {
  /**
     \class eEnergy
     User custom analysis class made by SHELL_USER_NAME
   */
  class eEnergy : public ana_base{
  
  public:

    /// Default constructor
    eEnergy(){ _name="eEnergy"; _fout=0;}

    /// Default destructor
    virtual ~eEnergy(){}

    /** IMPLEMENT in eEnergy.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in eEnergy.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in eEnergy.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    double r;
    bool inCircle(double dx, double dz, double r);
    const larutil::GeometryUtilities *mygeoutil;
    TH2D*myhist;
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
