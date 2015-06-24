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
#include "DataFormat/mcshower.h"
#include "LArUtil/LArUtilManager.h"
#include "DataFormat/hit.h"
#include "TH2.h"
#include "TTree.h"


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
    bool inCircle(double dx, double dz, double r);
    void ResetTree(TTree _tree);
    double normalizeVX(double Px, double Pz);
    double normalizeVZ(double Px, double Pz);
    int indexP(mctrack track);

  protected:
    double r;
    const larutil::GeometryUtilities *mygeoutil;
    const larutil::DetectorProperties *myprop;
   
    TTree *_tree;

    //energy deposited (mcshowers)
    double _dep_energy;
    //summed ADC from hits
    double _hits_energy;

    int  _n_hits_InCircle;
    int  _n_hits_total;

    double _r2_shower;

    //normalized direction vectors
    double _Px;
    double _Pz;


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
