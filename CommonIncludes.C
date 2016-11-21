#include <iostream>
#include <vector>
#include <utility>
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include <TFile.h>
#include <TVector3.h>
#include <TStyle.h>

const double MUON_MASS = 105.659;
const double PROTON_MASS = 938.272;
const double PION_MASS = 139.570;
const double bindE = 25.;

double CalcQ2GeV(double Ev, double truth_mu_px, double truth_mu_py, double truth_mu_pz, double thetamu) {
  double Q2=-9999.0;
  double pmu = ( sqrt( pow(truth_mu_px,2.0) + pow(truth_mu_py,2.0) + pow(truth_mu_pz,2.0) ) ); //MeV
  double Emu = sqrt( pow(pmu,2) + pow(MUON_MASS,2.0) ); //MeV
    
  Q2 = 2.0*Ev*(Emu - pmu*cos(thetamu)) - pow(MUON_MASS,2.0);
  if (Q2 < 0.0) Q2 = 0.0;
  
  return (1.0e-6)*Q2; //GeV
}

double CalcWexpGeV(double E_hadronic_system_MeV, double Q2_MeV ){
  double Wexp = pow(PROTON_MASS,2.0) - Q2_MeV + 2.0*(PROTON_MASS)*E_hadronic_system_MeV;
  if (Wexp>0) Wexp = sqrt(Wexp);
  else Wexp = 0.0;
  return (1.0e-3)*Wexp;
}

/*
  double CalcDotProduct(double v1[3], double v2[3]){
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] + v1[3]* v2[3];
  }

  double Calc3Magnitude(double v[3]){
    return pow(v[1],2) + pow(v[2],2) + pow(v[3],2);
  }
    
  double CalcMBEnu(double[3] pmu, double[3] ppi, double theta_munu, double theta_mupi){
    double MB_Enu;
    double Emu = pmu[0];
    double Epi = ppi[0];
    double mag_pmu = Calc3Magnitude(pmu);
    double mag_ppi = Calc3Magnitude(ppi);
    return_Enu = pow(MUON_MASS,2) + pow(PION_MASS,2) - 2*PROTON_MASS*(Emu+Epi)+ 2*CalcDotProduct(pmu, ppi);
    return_Enu /= 2*(Emu + Epi - mag_pmu*cos(theta_munu) - mag_ppi*cos(theta_pinu) - PROTON_MASS);
    return MB_Enu;
  }
*/

// take original vector and calculate the parallel projection along another vector
TVector3 ParallelComponent(TVector3 original, TVector3 newframe) {
  TVector3 part1;
  TVector3 part2;

  for(int i=0; i<3;i++) part1[i] = original[i]*newframe[i];
   
  double norm_newframe = sqrt(pow(newframe[0],2) + pow(newframe[1],2)+ pow(newframe[2],2));

  for(int i=0; i<3;i++) part1[i] = part1[i]/norm_newframe;

  for(int i=0; i<3;i++) part2[i] = newframe[i]/norm_newframe;

  TVector3 parallel;
  for(int i=0; i<3;i++) parallel[i] = part1[i]*part2[i];

  // double norm_parallel = sqrt(pow(parallel[0],2) + pow(parallel[1],2)+ pow(parallel[2],2));
  //for(int i=0; i<3;i++) parallel[i] = parallel[i]/norm_parallel;

  return  parallel;
}

// take original vector vector and the parallel projection over another direction and perform the perpendicular projection in the same direction
TVector3 PerpendicularComponent(TVector3 parallel, TVector3 total) {
  TVector3 perpendicular;

  for(int i=0; i<3;i++) perpendicular[i] = total[i]-parallel[i];

  //double norm_perpendicular = sqrt(pow(perpendicular[0],2) + pow(perpendicular[1],2)+ pow(perpendicular[2],2));
  //for(int i=0; i<3;i++) perpendicular[i] = perpendicular[i]/norm_perpendicular;

  return  perpendicular;

}

TVector3 BoostMomentum(TVector3 beta, double energy, TVector3 inMomPar) {
  double gamma= 0.;

  double  norm_beta2= pow(beta[0],2) + pow(beta[1],2)+ pow(beta[2],2);

  gamma = 1./(sqrt(1. - norm_beta2));

  //  std::cout<<" gamma   "<<gamma<<std::endl;
  //std::cout<<" norm_beta2   "<<norm_beta2<<std::endl;


  TVector3 parBoost;

  for(int i=0; i<3;i++) parBoost[i] = - (gamma*(sqrt(norm_beta2))*energy) +  (gamma*inMomPar[i]);

  //std::cout<<"parBoost    "<<parBoost[0]<<"   "<< parBoost[1]<<"  "<<parBoost[2]<<std::endl;


  //double norm_parBoost = sqrt(pow(parBoost[0],2) + pow(parBoost[1],2)+ pow(parBoost[2],2));
  //  for(int i=0; i<3;i++) parBoost[i] = parBoost[i]/norm_parBoost;

  return  parBoost;

}

//Unique function to calculate the polar and azimutal angles in the Delta++ rest frame for a CC1Pion interaction
// using different X,Y,Z axis according to:
//  RefSystemDef== 0 (expected to be closer to the Nuclear angle defintion ?)
//  RefSystemDef== 1 (Radcky et al./// ANL experiment)
//  RefSystemDef== 2 (// P.Allen et al.)
TVector3 AdlerAngle(int RefSystemDef, double dmumom, double dpimom, TVector3 NeuDir, TVector3 MuDir, TVector3 PiDir, double Enu) {

  if( Enu  < 0  || dmumom < 0 || dpimom < 0 ) return TVector3(-9,-9,-9);    

  //double MUON_MASS = 105.658;
  //double PION_MASS = 139.570;
  //double PROTON_MASS = 938.272;
  //double bindE = 25.;
  
  double Epion = sqrt(PION_MASS*PION_MASS+dpimom*dpimom);
  double Emuon = sqrt(MUON_MASS*MUON_MASS+dmumom*dmumom); 
  double Eproton = PROTON_MASS-bindE;
  
  double Edelta = (PROTON_MASS-bindE) + Enu - Emuon;  // Assume target nucleon at rest. 
  
  double delta[3];/// this is just Q_3 
  for( int i = 0; i < 3; i++ ) delta[i] = Enu*NeuDir[i] - dmumom*MuDir[i];////vector directors must be normalized

  double beta[3];/// proper of the boost to the Delta rest frame
  
  double b2 = 0.;
  
  for(int i = 0; i < 3; i++ ) {
    beta[i] = delta[i]/Edelta;
    b2 += beta[i]*beta[i]; 
  }

  double gamma = 1./sqrt(1.-b2); 
  
  double b = sqrt(b2);

  //std::cout<<" módulo de beta debe ser menor que 1 ----->    "<<b<<std::endl;
  TVector3 AdlerSyst;

  if( b > 1. ) {
    AdlerSyst[0] = -1000;
    AdlerSyst[1] = -1000;
    AdlerSyst[2] = -1000;
    std::cout<<" |beta| should be less than one ----->    "<<b<<std::endl; /// this canot happen by definition
 
    return AdlerSyst;
  }

 
  double piparallel[3];
  double piperpend[3];
  double piboost[3];
  double piboostabs = 0.;
  
  double nuparallel[3];
  double nuperpend[3];
  double nuboost[3];
  double nuboostabs = 0.;

  double muparallel[3];
  double muperpend[3];
  double muboost[3];
  double muboostabs = 0.;

  double prparallel[3];
  double prperpend[3];
  double prboost[3];
  double prboostabs = 0.;
  
  for(int i = 0; i < 3; i++ ) {   
    piparallel[i] = beta[i]/b*(beta[0]/b*dpimom*PiDir[0]+beta[1]/b*dpimom*PiDir[1]+beta[2]/b*dpimom*PiDir[2]); // Pre boost 
    piperpend[i] = dpimom*PiDir[i]-piparallel[i];
    piparallel[i] = gamma*(piparallel[i]-beta[i]*Epion); // After boost
    piboost[i] =  piparallel[i]+piperpend[i];
    piboostabs += piboost[i]*piboost[i];
    
    prparallel[i] = 0.;      // Pre boost 
    prperpend[i] =  0.;
    prparallel[i] = gamma*(prparallel[i]-beta[i]*Eproton); // After boost
    prboost[i] =  prparallel[i]+prperpend[i];
    prboostabs += prboost[i]*prboost[i];

    //nuparallel[i] = beta[i]/b*Enu/Abspv[ineut]*(beta[0]/b*Pmomv[ineut][0]+beta[1]/b*Pmomv[ineut][1]+beta[2]/b*Pmomv[ineut][2]); // Pre boost
    nuparallel[i] = beta[i]/b*(beta[0]/b*Enu*NeuDir[0]+beta[1]/b*Enu*NeuDir[1]+beta[2]/b*Enu*NeuDir[2]); // Pre boost
    nuperpend[i] = Enu*NeuDir[i]-nuparallel[i];
    nuparallel[i] = gamma*(nuparallel[i]-beta[i]*Enu); // After boost
    nuboost[i] =  nuparallel[i]+nuperpend[i];
    nuboostabs += nuboost[i]*nuboost[i];

    muparallel[i] = beta[i]/b*(beta[0]/b*dmumom*MuDir[0]+beta[1]/b*dmumom*MuDir[1]+beta[2]/b*dmumom*MuDir[2]); // Pre boost 
    muperpend[i] = dmumom*MuDir[i]-muparallel[i];
    muparallel[i] = gamma*(muparallel[i]-beta[i]*Emuon); // After boost
    muboost[i] =  muparallel[i]+muperpend[i];
    muboostabs += muboost[i]*muboost[i];
  }

  //std::cout<<" resultado del boost debe ser 0....................................................." << std::endl; 

  // for(int i = 0 ; i < 3; i++ ) 
  //  std::cout << nuboost[i]-muboost[i]+prboost[i] << "   " ;
  // std::cout << std::endl; 
  // std::cout<<" ....................................................." << std::endl; 

  piboostabs = sqrt(piboostabs); 
  nuboostabs = sqrt(nuboostabs);
  muboostabs = sqrt(muboostabs);

  double angtest = 0.;
  
  for(int i=0;i < 3; i++)  angtest +=  piboost[i]* beta[i]/b/piboostabs;// Z = beta
  
  angtest = acos(angtest); 

  double x[3];
  double y[3];
  double z[3];

  if( RefSystemDef == 0 ) { // Closer to Nuclear angle defintion ? --->> Z = beta, Y = Z x mu
  
    z[0] = beta[0]/b;
    z[1] = beta[1]/b;
    z[2] = beta[2]/b;
    
    y[0] = muboost[1]*z[2]-muboost[2]*z[1];
    y[1] = muboost[2]*z[0]-muboost[0]*z[2];
    y[2] = muboost[0]*z[1]-muboost[1]*z[0];

   
  } else if( RefSystemDef == 1 ) { // Radecky et al. (Wrong?) //// ---> Z = nu-mu (after boost), y = nu xmu (after boost)
    
    z[0] = nuboost[0]/nuboostabs-muboost[0]/muboostabs;
    z[1] = nuboost[1]/nuboostabs-muboost[1]/muboostabs;
    z[2] = nuboost[2]/nuboostabs-muboost[2]/muboostabs;

    double norm = sqrt(z[0]*z[0]+z[1]*z[1]+z[2]*z[2]);

    z[0] /= norm;
    z[1] /= norm;
    z[2] /= norm;

    y[0] = nuboost[1]*muboost[2]-nuboost[2]*muboost[1];
    y[1] = nuboost[2]*muboost[0]-nuboost[0]*muboost[2];
    y[2] = nuboost[0]*muboost[1]-nuboost[1]*muboost[0];
    
  } else {  // P.Allen et al. //// Z = nu -mu (after boost), Y = Z x mu (after boost)

    z[0] = nuboost[0]-muboost[0];
    z[1] = nuboost[1]-muboost[1];
    z[2] = nuboost[2]-muboost[2];
   
    double norm = sqrt(z[0]*z[0]+z[1]*z[1]+z[2]*z[2]);
    
    z[0] /= norm;
    z[1] /= norm;
    z[2] /= norm;

    // std::cout << z[0] << "  " << z[1] << "   " << z[2] << std::endl; 

    y[0] = z[1]*muboost[2]-z[2]*muboost[1];
    y[1] = z[2]*muboost[0]-z[0]*muboost[2];
    y[2] = z[0]*muboost[1]-z[1]*muboost[0];

  }

  double normy = sqrt(y[0]*y[0]+y[1]*y[1]+y[2]*y[2]); 

  for(int i=0;i < 3; i++) y[i] = y[i] /normy; 
 
  x[0] = y[1]*z[2]-y[2]*z[1];
  x[1] = y[2]*z[0]-y[0]*z[2];
  x[2] = y[0]*z[1]-y[1]*z[0]; 

  double ang = 0.; 
  
  for(int i=0;i < 3; i++)  ang +=  piboost[i]*z[i]/piboostabs;
  
  ang = acos(ang); 
  
  double px = piboost[0]*x[0]+piboost[1]*x[1]+piboost[2]*x[2];
  double py = piboost[0]*y[0]+piboost[1]*y[1]+piboost[2]*y[2];

  double  phi = atan2(py/piboostabs,px/piboostabs);
  
  
  AdlerSyst[0] = angtest;
  AdlerSyst[1] = ang;
  AdlerSyst[2] = phi;

  return AdlerSyst; 
}

//#include "TTree.h"
//#include <TFile.h>
//#include <TChain.h>
