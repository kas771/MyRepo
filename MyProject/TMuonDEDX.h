/**
 * \file TMuonDEDX.h
 *
 * \ingroup MyProject
 * 
 * \brief Class def header for a class TMuonDEDX
 *
 * @author kathrynsutton
 */

/** \addtogroup MyProject

    @{*/

#ifndef LARLITE_TMUONDEDX_H
#define LARLITE_TMUONDEDX_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctrajectory.h"
#include "TH2.h"
#include "TH1.h"

namespace larlite {
  /**
     \class TMuonDEDX
     User custom analysis class made by SHELL_USER_NAME
   */
  class TMuonDEDX : public ana_base{
  
  public:

    /// Default constructor
    TMuonDEDX(){ _name="TMuonDEDX"; _fout=0;}

    /// Default destructor
    virtual ~TMuonDEDX(){}

    /** IMPLEMENT in TMuonDEDX.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TMuonDEDX.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TMuonDEDX.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    double distance(mctrack mytrack, size_t myblah);

    double tracklength(mctrack mytrack);
    double dE(mctrack track, size_t  m);
    
  protected:
    TH2D *myhist;

    TH1D * histdE;
    TH1D * histdX;


    int nentries;
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
