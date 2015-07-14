//
//  This example has been taken from the ROOT tutorials website
//  and has been modified by P.Castro 14th July, 2003
//
//   Example of a program to fit non-equidistant data points
//   =======================================================
//
//   The fitting function fcn is a simple chisquare function
//   The data consists of 5 data points (arrays x,y,z) + the errors in errorsz
//   More details on the various functions or parameters for these functions
//   can be obtained in an interactive ROOT session with:
//    Root > TMinuit *minuit = new TMinuit(10);
//    Root > minuit->mnhelp("*")  to see the list of possible keywords
//    Root > minuit->mnhelp("SET") explains most parameters
//
#include <iostream>

#include <TROOT.h>
#include <TMinuit.h>

using namespace std;

const int iNum = 5;
Float_t x[5],y[5],errory[5];

// this is the function used for the fit
//par: vector with the fit parameters

Double_t fit_function(float x,Double_t *par)
{
  // double value=( (par[0]*par[0])/(x*x)-1)/ ( par[1]+par[2]*y-par[3]*y*y);
  double value=  par[0]+par[1]*x;
  return value;
}

void calc_chi_square(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  //calculate chisquare
  double chisq = 0;
  for (int i=0;i<iNum; i++) {
    double delta  = (y[i]-fit_function(x[i],par))/errory[i];
    chisq += delta*delta;
  }
  f = chisq;
  return;
}

int main()
{

  // The errors on z values
  Float_t error = 0.01;
  errory[0]=error;
  errory[1]=error;
  errory[2]=error;
  errory[3]=error;
  errory[4]=error;

  // the x values
  x[0]=0.0;
  x[1]=1.0;
  x[2]=2.0;
  x[3]=3.0;
  x[4]=4.0;

  // the y values
  y[0]=1.5;
  y[1]=0.5;
  y[2]=3.5;
  y[3]=4.5;
  y[4]=10.5;

  TMinuit *ptMinuit = new TMinuit(2);  //initialize TMinuit with a maximum of 2 params
  //
  //  select verbose level:
  //    default :     (58 lines in this test)
  //    -1 : minimum  ( 4 lines in this test)
  //     0 : low      (31 lines)
  //     1 : medium   (61 lines)
  //     2 : high     (89 lines)
  //     3 : maximum (199 lines in this test)
  //
  ptMinuit->SetPrintLevel();

  // set the user function that calculates chi_square (the value to minimize)
  ptMinuit->SetFCN(calc_chi_square);

  Double_t arglist[10];
  Int_t ierflg = 0;
  
  arglist[0] = 1;
  ptMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
  
  // Set starting values and step sizes for parameters
  static Double_t vstart[2] = {-5, 1};
  static Double_t step[2]   = {0.0001 , 0.0001};

  ptMinuit->mnparm(0, "a1", vstart[0], step[0],0,0,ierflg);
  ptMinuit->mnparm(1, "a2", vstart[1], step[1],0,0,ierflg);

  // Now ready for minimization step
  arglist[0] = 500;
  arglist[1] = 1.;
  ptMinuit->mnexcm("MINUIT", arglist,2,ierflg);

  // Print results
  cout << "\nPrint results from minuit\n";
  double fParamVal;
  double fParamErr;
  ptMinuit->GetParameter(0,fParamVal,fParamErr);
  cout << "a1=" << fParamVal << "\n";
  ptMinuit->GetParameter(1,fParamVal,fParamErr);
  cout << "a2=" << fParamVal << "\n";

  // if you want to access to these parameters, use:
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  ptMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  //void mnstat(Double_t &fmin, Double_t &fedm, Double_t &errdef, Int_t &npari, Int_t &nparx, Int_t &istat) 
  //*-*-*-*-*Returns concerning the current status of the minimization*-*-*-*-*
  //*-*      =========================================================
  //*-*       User-called
  //*-*          Namely, it returns:
  //*-*        FMIN: the best function value found so far
  //*-*        FEDM: the estimated vertical distance remaining to minimum
  //*-*        ERRDEF: the value of UP defining parameter uncertainties
  //*-*        NPARI: the number of currently variable parameters
  //*-*        NPARX: the highest (external) parameter number defined by user
  //*-*        ISTAT: a status integer indicating how good is the covariance
  //*-*           matrix:  0= not calculated at all
  //*-*                    1= approximation only, not accurate
  //*-*                    2= full matrix, but forced positive-definite
  //*-*                    3= full accurate covariance matrix
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  cout << "\n";
  cout << " Minimum chi square = " << amin << "\n";
  cout << " Estimated vert. distance to min. = " << edm << "\n";
  cout << " Number of variable parameters = " << nvpar << "\n";
  cout << " Highest number of parameters defined by user = " << nparx << "\n";
  cout << " Status of covariance matrix = " << icstat << "\n";

  cout << "\n";
  ptMinuit->mnprin(3,amin);
  //*-*-*-*Prints the values of the parameters at the time of the call*-*-*-*-*
  //*-*    ===========================================================
  //*-*        also prints other relevant information such as function value,
  //*-*        estimated distance to minimum, parameter errors, step sizes.
  //*-*
  //*-*         According to the value of IKODE, the printout is:
  //*-*    IKODE=INKODE= 0    only info about function value
  //*-*                  1    parameter values, errors, limits
  //*-*                  2    values, errors, step sizes, internal values
  //*-*                  3    values, errors, step sizes, first derivs.
  //*-*                  4    values, parabolic errors, MINOS errors
  //*-*    when INKODE=5, MNPRIN chooses IKODE=1,2, or 3, according to ISW(2)
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

  return EXIT_SUCCESS;
}

