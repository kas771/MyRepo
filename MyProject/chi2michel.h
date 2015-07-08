/**
 * \file chi2michel.h
 *
 * \ingroup MyProject
 * 
 * \brief Class def header for a class chi2michel
 *
 * @author kathrynsutton
 */

/** \addtogroup MyProject

    @{*/

#ifndef LARLITE_CHI2MICHEL_H
#define LARLITE_CHI2MICHEL_H

#include "Analysis/ana_base.h"
#include "LArUtil/LArUtilManager.h"
#include "DataFormat/hit.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mctrajectory.h"
#include "DataFormat/mcshower.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TH2.h"



namespace larlite {
  /**
     \class chi2michel
     User custom analysis class made by SHELL_USER_NAME
   */
  class chi2michel : public ana_base{
  
  public:

    /// Default constructor
    chi2michel(){ _name="chi2michel"; _fout=0;}

    /// Default destructor
    virtual ~chi2michel(){}

    /** IMPLEMENT in chi2michel.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in chi2michel.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in chi2michel.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    double distance(hit myhit, double xstart, double zstart);
     double distance(hit hit0, hit hit1);
     void printvec(std::vector<int> v);
      void printvec(std::vector<double> v);
     
  protected:
    const larutil::GeometryUtilities* mygeoutil;
    const larutil::DetectorProperties *myprop;

    double fTimetoCm;
    double fWiretoCm;
    double triggerOffset;

    int n_matches;

    double cutoff;

    int range;

    TGraphErrors*graph;
    TGraph*chigraph;
    TGraph*total;
    TH2D*chiweighted;

    
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
