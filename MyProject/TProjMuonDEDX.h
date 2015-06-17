
/**
 * \file TProjMuonDEDX.h
 *
 * \ingroup MyProject
 * 
 * \brief Class def header for a class TProjMuonDEDX
 *
 * @author kathrynsutton
 */

/** \addtogroup MyProject

    @{*/

#ifndef LARLITE_TPROJMUONDEDX_H
#define LARLITE_TPROJMUONDEDX_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctrajectory.h"
#include "TH2.h"
#include "TH1.h"

namespace larlite {
  /**
     \class TProjMuonDEDX
     User custom analysis class made by SHELL_USER_NAME
   */
  class TProjMuonDEDX : public ana_base{
  
  public:

    /// Default constructor
    TProjMuonDEDX(){ _name="TProjMuonDEDX"; _fout=0;}

    /// Default destructor
    virtual ~TProjMuonDEDX(){}

    /** IMPLEMENT in TProjMuonDEDX.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TProjMuonDEDX.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TProjMuonDEDX.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    double projdistance(mctrack mytrack, size_t myblah);
    double tracklength(mctrack mytrack);
    double dE(mctrack track, size_t  m);

  protected:
    TH2D *projhist;

    // TH1D * histdE;
    // TH1D * histdX;


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
