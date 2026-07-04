/****************************************************************************/
/****************  This macro cut-efficiencies for events  ******************/
/****************************************************************************/
#include <iostream>
#include <TMath.h>
#include <TClonesArray.h>
#include <TTree.h>
#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif
 #define PI 3.1415927
#define Lda 1.0
//#define factor 0.026519596 //fraction of events of Bhaba according to arXiv:2001.03011v1 [hep-ex] 
//--------------------------------------------------------------------------------------

using namespace std;

/*******************************************************************************************/
double DR(double eta1,double phi1,double eta2,double phi2)
{
        double DPhi = TMath::Abs(phi1-phi2);
        double DEta = eta1-eta2;
        if(DPhi > TMath::Pi())
        DPhi = TMath::TwoPi() - DPhi;
        return TMath::Sqrt(DEta*DEta + DPhi*DPhi);
}

double R(double eta1,double phi1)
{
        return TMath::Sqrt(eta1*eta1 + phi1*phi1);
}

/* Delta_Phi */

double DPhi(double phi1, double phi2)
{
        double delPhi = TMath::Abs(phi1-phi2);
        if(delPhi > TMath::Pi())
        delPhi = TMath::TwoPi() - delPhi;
        return delPhi;
}

double DEta(double eta1, double eta2)
{	
	double delEta = TMath::Abs(eta1-eta2);
	return delEta;
}

double sqr(double a)
{
return (a*a);
}

double sqrsubt( double a, double b, double c)
{
return (a*a - b*b - c*c);
}

double sqrsum( double a, double b)
{
return (a*a + b*b);
}

double SigSys(int S, int B1, int B2, double x, double y)
{
double Sig = (Double_t)S/TMath::Sqrt(S+B1+B2+sqr(0.01*(x*B1 + y*B2)));
return Sig;
}

double SigSys1(int S, int B1, int B2, double x, double y)
{
double Sig = (Double_t)S/TMath::Sqrt(B1+B2 + sqrsum(0.01*x*B1, 0.01*y*B2));
return Sig;
}
// For interference effects  (THIS IS TOTALLY WRONG)
double SigSys2(int S0, int N, int Bh, int Si, int Ni, double x, double y) 
{
double S = Si - Ni - S0; // only interference
double Sig = (double)S/TMath::Sqrt(N+Bh + sqrsum(0.01*x*N, 0.01*y*Bh));
return Sig;
}
// For interference effects  (CORRECTED)
double SigSys3(int Spure, int N, int Bh, int Stot, int Ni, double x, double y) 
{
double Si = Stot - Ni - Spure; // only interference
double deno = TMath::Sqrt(N+Bh + sqrsum(0.01*x*N, 0.01*y*Bh));
double Limit = (double)(-Si+TMath::Sqrt(Si*Si + 4*Spure*3*deno))/(2*Spure);
return Limit;
}

// Limit calculation (g_{Z^\prime}^2)
double CouplingLimitH20(
double Stot1,double Pure1,double BnuInt1,double Bnu1,double Bbh1,
double Stot2,double Pure2,double BnuInt2,double Bnu2,double Bbh2,
double Stot3,double Pure3,double BnuInt3,double Bnu3,double Bbh3,
double Stot4,double Pure4,double BnuInt4,double Bnu4,double Bbh4,
double x,double y)
{
    const double gmin = 1e-5;
    const double gmax = 5.0;
    const double step = 1e-4;

    for(double g=gmin; g<=gmax; g+=step)
    {
        double D1=sqrt(Bnu1+Bbh1+sqrsum(0.01*x*Bnu1,0.01*y*Bbh1));
        double D2=sqrt(Bnu2+Bbh2+sqrsum(0.01*x*Bnu2,0.01*y*Bbh2));
        double D3=sqrt(Bnu3+Bbh3+sqrsum(0.01*x*Bnu3,0.01*y*Bbh3));
        double D4=sqrt(Bnu4+Bbh4+sqrsum(0.01*x*Bnu4,0.01*y*Bbh4));
        
        double Int1 = Stot1 - Pure1 - BnuInt1; 
        double Int2 = Stot2 - Pure2 - BnuInt2; 
        double Int3 = Stot3 - Pure3 - BnuInt3; 
        double Int4 = Stot4 - Pure4 - BnuInt4; 

        double Sig1=(g*g*Int1+g*g*g*g*Pure1)/D1;
        double Sig2=(g*g*Int2+g*g*g*g*Pure2)/D2;
        double Sig3=(g*g*Int3+g*g*g*g*Pure3)/D3;
        double Sig4=(g*g*Int4+g*g*g*g*Pure4)/D4;

        double SigH20 =
        sqrt(Sig1*Sig1 +
             Sig2*Sig2 +
             Sig3*Sig3 +
             Sig4*Sig4);

        if(SigH20 > 2.99 && SigH20 < 3.01)
            return g;
    }

    return -1.0;
}

double ReqSig(double B)
{
double a = 9991;
double b = 90000 + (18*B);
double c = 9*B*B + 90000*B;
double signal = (b + sqrt((b*b) + (4*a*c)))/(2*a);
return signal;
}

double ReqSig2(double B1, double B2, double x, double y)// with systematic uncertainty
{
double a = x*0.01;
double b = y*0.01;
double signal = (9. + sqrt(81. + 36*(B1+B2 + sqr(a*B1+b*B2))))/2.;
return signal;
}

/*******************************************************************************************/

//void ContourPhU(int t, double CS0, double CS1, double CS2, double CS3, double CS4, int m)
void ContourPhIntf(double CS1I, double CS2I, double CS3I, double CS4I, double CS1, double CS2, double CS3, double CS4, int m)
{
ROOT::EnableImplicitMT();
   gSystem->Load("libDelphes");
   gSystem->cd("/home/saumyen/Research/NSI_Coll/Interference/ILC");

//const char *mass[22] = {"1", "10", "100", "200", "400", "500", "700", "800", "950", "1000", "1050", "1150", "1250", "1350", "1500", "1750", "2000", "3000", "4000", "5000", "7000", "10000"}; // W10M
const char *mass[24] = {"1", "10", "100", "200", "400", "500", "700", "800", "900", "950", "1000", "1050", "1150", "1250", "1350", "1500", "1650", "1750", "2000", "3000", "4000", "5000", "7000", "10000"}; // W30M
//  TString outfileU = TString::Format("ContourData.dat");
  TString outfile = TString::Format("ContourIntW30M_Jun26.dat");
//  TString CSfile = TString::Format("CS_M.txt");

/******************* Files and Cross-sections **********************/
//const char *BGvv0 = "/media/saumyen/SeagateExpansionDrive/Research/LeptonNSI/Numerical/ILC/Neutrino/Pol0/Neutrino0.root";
const char *BGvv1 = "/home/saumyen/Research/NSI_Coll/ILC/Neutrino/Pol1/Neutrino1.root";
const char *BGvv2 = "/home/saumyen/Research/NSI_Coll/ILC/Neutrino/Pol2/Neutrino2.root";
const char *BGvv3 = "/home/saumyen/Research/NSI_Coll/ILC/Neutrino/Pol3/Neutrino3.root";
const char *BGvv4 = "/home/saumyen/Research/NSI_Coll/ILC/Neutrino/Pol4/Neutrino4.root";

//
//const char *BGbh0 = "/media/saumyen/SeagateExpansionDrive/Research/LeptonNSI/Numerical/ILC/Bhabha/Pol0/Bhabha0.root";
const char *BGbh1 = "/home/saumyen/Research/NSI_Coll/ILC/Bhabha/Pol1/Bhabha1.root";
const char *BGbh2 = "/home/saumyen/Research/NSI_Coll/ILC/Bhabha/Pol2/Bhabha2.root";
const char *BGbh3 = "/home/saumyen/Research/NSI_Coll/ILC/Bhabha/Pol3/Bhabha3.root";
const char *BGbh4 = "/home/saumyen/Research/NSI_Coll/ILC/Bhabha/Pol4/Bhabha4.root";

//const char *BGvv0I = "/home/saumyen/Research/NSI_Coll/Interference/ILC/Neutrino/Pol1/Neutrino0.root";
const char *BGvv1I = "/home/saumyen/Research/NSI_Coll/Interference/ILC/Neutrino/Pol1/Neutrino1.root";
const char *BGvv2I = "/home/saumyen/Research/NSI_Coll/Interference/ILC/Neutrino/Pol2/Neutrino2.root";
const char *BGvv3I = "/home/saumyen/Research/NSI_Coll/Interference/ILC/Neutrino/Pol3/Neutrino3.root";
const char *BGvv4I = "/home/saumyen/Research/NSI_Coll/Interference/ILC/Neutrino/Pol4/Neutrino4.root";

Double_t CSvv0 = 6646.3295, CSvv1 = 1848.5895, CSvv2 = 1188.0690, CSvv3 = 15245.153, CSvv4 = 8247.2998;
Double_t CSbh0 = 46922.227, CSbh1 = 39566.067, CSbh2 = 47278.765, CSbh3 = 48645.510, CSbh4 = 46371.292;
Double_t CSvv0I = 6646.3295, CSvv1I = 1.4942042E+03, CSvv2I = 9.4657917E+02, CSvv3I = 1.2533085E+04, CSvv4I = 6.7596076E+03;
/******************************************************************************/
//TString sig0 = TString::Format("/media/saumyen/SeagateExpansionDrive/Research/LeptonNSI/Numerical/ILC/Signal/W30M/Pol0/Signal0_")+mass[m]+TString::Format(".root");
TString sig1 = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Signal/W30M/Pol1/Signal1_")+mass[m]+TString::Format(".root");
TString sig2 = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Signal/W30M/Pol2/Signal2_")+mass[m]+TString::Format(".root");
TString sig3 = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Signal/W30M/Pol3/Signal3_")+mass[m]+TString::Format(".root");
TString sig4 = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Signal/W30M/Pol4/Signal4_")+mass[m]+TString::Format(".root");

// (SM + Pure + Interference)
//TString sig0I = TString::Format("/media/saumyen/SeagateExpansionDrive/Research/LeptonNSI/Numerical/ILC/SignalI/W30M/Pol0/Signal0_")+mass[m]+TString::Format(".root");
TString sig1I = TString::Format("/home/saumyen/Research/NSI_Coll/Interference/ILC/Signal30/Pol1/Signal1_")+mass[m]+TString::Format(".root");
TString sig2I = TString::Format("/home/saumyen/Research/NSI_Coll/Interference/ILC/Signal30/Pol2/Signal2_")+mass[m]+TString::Format(".root");
TString sig3I = TString::Format("/home/saumyen/Research/NSI_Coll/Interference/ILC/Signal30/Pol3/Signal3_")+mass[m]+TString::Format(".root");
TString sig4I = TString::Format("/home/saumyen/Research/NSI_Coll/Interference/ILC/Signal30/Pol4/Signal4_")+mass[m]+TString::Format(".root");

//TString sig[5] = {sig0, sig1, sig2, sig3, sig4};
cout<<"Z' mass = "<<mass[m]<<" and the CS are\t"<<CS1<<"\t"<<CS2<<"\t"<<CS3<<"\t"<<CS4<<"\t"<<endl;


std::vector<const char *> inputFiles = {sig1, sig2, sig3, sig4, BGvv1, BGvv2, BGvv3, BGvv4, BGbh1, BGbh2, BGbh3, BGbh4, BGvv1I, BGvv2I, BGvv3I, BGvv4I, sig1I, sig2I, sig3I, sig4I};
std::vector<Double_t> Xsection = {CS1, CS2, CS3, CS4, CSvv1, CSvv2, CSvv3, CSvv4, CSbh1, CSbh2, CSbh3, CSbh4, CSvv1I, CSvv2I, CSvv3I, CSvv4I, CS1I, CS2I, CS3I, CS4I}; 
std::vector<Double_t> GenEvnts = {50000,50000,50000,50000,200000,200000,200000,200000,300000,300000,300000,300000,50000,50000,50000,50000,50000,50000,50000,50000}; 
std::vector<Double_t> Lumi = {800,3200,3200,800,800,3200,3200,800,800,3200,3200,800,800,3200,3200,800,800,3200,3200,800}; //,8000,8000,8000};

//std::vector<const char *> inputFiles = {sig0, BGvv0, BGbh0};
//std::vector<Double_t> Xsection = {CS0, CSvv0, CSbh0};
//std::vector<Double_t> GenEvnts = {50000,200000,300000};
//std::vector<Double_t> Lumi = {8000,8000,8000};

Int_t ii = inputFiles.size();
/******************* Cut are defined here *************************/
Double_t phot_E = 5.; //		  **************************/
Double_t phot_eta = 2.8; //		  **************************/
Double_t emj_pt = 5; //		          **************************/
Double_t emj_eta = 2.8; //		  **************************/
Double_t photE_ana = 470.; //		  **************************/
Double_t cos_theta = 0.95; //		  **************************/
/******************************************************************/

Double_t Eff0[ii], Eff1[ii], Eff2[ii], Eff3[ii], Eff4[ii], Eff5[ii];

Int_t TotEvnt[ii], Events[ii], EveAC[ii], EveAC1[ii], EveAC2[ii], EveAC3[ii], EveAC4[ii], EveAC5[ii], BeamCal[ii]; 

for(int nfile=0; nfile < ii; nfile++)
{
  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFiles[nfile]);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchBCalPhoton = treeReader->UseBranch("BCalPhoton");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");

   Photon *photon[20], *bcalphoton[20];
   MissingET *misset;
   Electron *elec[10];
   Muon *muon[10];
   Jet *jet[20];

Int_t multiphot(0), bgevent(0), monoelec(0), bielec(0), evntnum(0);
Int_t AC_NEV(0), AC1_NEV(0), AC2_NEV(0), AC3_NEV(0), AC4_NEV(0), AC5_NEV(0);

  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
   Int_t phot_no(0), bphot_no(0), elec_no(0), muon_no(0), emj_no(0), jet_no(0), charg_no(0);
    Double_t MET(0), PTph(0), Eph(0), ETAph(0), PHIph(0), dPHI(0), dETA(0), dR(0), missCtheta(0), Mmiss(0), Evis(0);

    TLorentzVector visible(0.0,0.0,0.0,0.0);
    TLorentzVector visible1(0.0,0.0,0.0,0.0);

     for(Int_t j=0;j<branchElectron->GetEntries();j++)   //Electron
    {
      elec[j] = (Electron *) branchElectron->At(j);
    if((elec[j]->PT) > 2. && abs(elec[j]->Eta) < 2.8){elec_no++; charg_no++;
    Evis += elec[j]->P4().E(); visible += elec[j]->P4();
    }
    }
    for(Int_t j=0;j<branchMuon->GetEntries();j++)   //Muon
    {
      muon[j] = (Muon *) branchMuon->At(j);
    if((muon[j]->PT) > 5. && abs(muon[j]->Eta) < 2.8){muon_no++; charg_no++;
        Evis += muon[j]->P4().E(); visible += muon[j]->P4();
    }
    }

    for(Int_t j=0;j<branchJet->GetEntries();j++)   //Jet branch
    {
     jet[j] = (Jet *) branchJet->At(j);
     if((jet[j]->PT) > 5 && abs(jet[j]->Eta) < 2.8)
     {
     jet_no++; Evis += jet[j]->P4().E(); visible += jet[j]->P4();
     if(jet[j]->Charge != 0){charg_no++;}     //visible += jet[j]->P4();
     }
    }

    for(Int_t k = 0; k < branchPhoton->GetEntriesFast(); k++)  // Photon
    {
      photon[k] = (Photon*) branchPhoton->At(k);  
      phot_no++; 
     }

    for(Int_t j = 0; j < branchBCalPhoton->GetEntriesFast(); j++) // BCal Photon
    {
      // Take first photon
      bcalphoton[j] = (Photon*) branchBCalPhoton->At(j);
    //  if((bcalphoton[j]->PT > 1.5 && abs(bcalphoton[j]->Phi) > (PI/180)*35) || (bcalphoton[j]->PT > 5.0 && abs(bcalphoton[j]->Phi) < (PI/180)*35)){bphot_no++; }
      bphot_no++;
    }

emj_no = jet_no + elec_no + muon_no;
//  CM energy
TLorentzVector Ecm;
Ecm.SetPxPyPzE(0.0,0.0,0.0,500.0);
//  Missing energy
//     TLorentzVector missE = Ecm - visible - photon[0]->P4();
Double_t sqrtS = 500.0;

// Variables definition
if(phot_no > 0){
Eph = photon[0]->E;
ETAph = photon[0]->Eta;
PHIph = photon[0]->Phi;
//Mmiss = missE.E();
missCtheta = TMath::Abs(TMath::Cos(photon[0]->P4().Theta()));
//Evis = visible.E();

/*************** For cut implimantation ******************/
//if(nfile != 1){
if(Eph > phot_E && abs(ETAph) < phot_eta){  //Baseline cut
AC_NEV++ ; //Baseline
if(Eph < photE_ana){AC1_NEV++; 
if(charg_no==0){ AC2_NEV++; 
if(missCtheta < cos_theta){ AC3_NEV++; 
if(bphot_no ==0){ AC4_NEV++; 

}}}}}
}
} //Event loop

/**************** For Cut-flow *************************/
Int_t TotEvnt = numberOfEntries;
//cout<< "Total Events = " << TotEvnt <<endl;
Double_t MergEff = TotEvnt/GenEvnts[nfile]; //cout<<"Merging Efficiency = "<<MergEff<<endl;
Events[nfile] = Xsection[nfile]*MergEff*Lumi[nfile];  //Actual number of events as par Xsection*Lumi

/*
ofstream funit2;
funit2.open(mergedCSfile,ios::app);
if(m==1){funit2<< "Merged cross-section for File-"<<nfile<<" = "<<Xsection[nfile]*MergEff<<endl;}
*/
//ofstream funit2;
//funit2.open(CSfile,ios::app);
//funit2<< mass[m]<<"\t"<<Xsection[nfile]*MergEff<<endl;

// Base-line cut
Eff0[nfile] = (Double_t)AC_NEV/TotEvnt;
EveAC[nfile] = Events[nfile]*Eff0[nfile];
// Cut-1
Eff1[nfile] = (Double_t)AC1_NEV/TotEvnt;
EveAC1[nfile] = Events[nfile]*Eff1[nfile];

// Cut-2
Eff2[nfile] = (Double_t)AC2_NEV/TotEvnt;
EveAC2[nfile] = Events[nfile]*Eff2[nfile];
// Cut-3
Eff3[nfile] = (Double_t)AC3_NEV/TotEvnt;
EveAC3[nfile] = Events[nfile]*Eff3[nfile];
// Cut-4
Eff4[nfile] = (Double_t)AC4_NEV/TotEvnt;
EveAC4[nfile] = Events[nfile]*Eff4[nfile];

}//File-loop ends here

ofstream funit, funitU;
funit.open(outfile,ios::app);
//funitU.open(outfileU,ios::app);

// Optimal Polarised Case
Double_t TotBGvvop;
Double_t TotBGbhop;
Double_t TotSigop;
Double_t TotSigopI;
Double_t TotBGvvopI;

// For Optimal Polarisation case

 TotBGvvop = EveAC4[5]*2.5; // The factor 2.5 to make the Luminosity 8/ab
 TotBGbhop = EveAC4[9]*2.5;
 TotSigop = EveAC4[1]*2.5;
 TotSigopI = EveAC4[17]*2.5;
 TotBGvvopI = EveAC4[13]*2.5; 
 
Double_t Lda30op = SigSys3(TotSigop,TotBGvvop,TotBGbhop, TotSigopI, TotBGvvopI, 0.0,0.0);
Double_t Lda31op = SigSys3(TotSigop,TotBGvvop,TotBGbhop, TotSigopI, TotBGvvopI, 0.2,1.0);

// H20 scenario 2nd Method
// WRONG ALGORITHM
//Double_t x0 = std::sqrt(sqr(SigSys2(EveAC4[0], EveAC4[4], EveAC4[8], EveAC4[16],EveAC4[12], 0.,0.)) + sqr(SigSys2(EveAC4[1],EveAC4[5], EveAC4[9], EveAC4[17], EveAC4[13],0,0)) + sqr(SigSys2(EveAC4[2],EveAC4[6], EveAC4[10], EveAC4[18], EveAC4[14],0,0)) + sqr(SigSys2(EveAC4[3],EveAC4[7], EveAC4[11], EveAC4[19], EveAC4[15],0,0)));
//Double_t x1 = std::sqrt(sqr(SigSys2(EveAC4[0], EveAC4[4], EveAC4[8], EveAC4[16], EveAC4[12],0.2,1.0)) + sqr(SigSys2(EveAC4[1],EveAC4[5],EveAC4[9], EveAC4[17], EveAC4[13], 0.2,1.0)) + sqr(SigSys2(EveAC4[2],EveAC4[6], EveAC4[10], EveAC4[18], EveAC4[14], 0.2,1.0)) + sqr(SigSys2(EveAC4[3],EveAC4[7], EveAC4[11], EveAC4[19], EveAC4[15], 0.2,1.0)));
//Double_t Lda30N = Lda*(3/x0);
//Double_t Lda31N = Lda*(3/x1);

/* Helper snippet
double Stot1,double Pure1,double BnuInt1,double Bnu1,double Bbh1,
double Stot2,double Pure2,double BnuInt2,double Bnu2,double Bbh2,
double Stot3,double Pure3,double BnuInt3,double Bnu3,double Bbh3,
double Stot4,double Pure4,double BnuInt4,double Bnu4,double Bbh4,
  
{sig1, sig2, sig3, sig4, BGvv1, BGvv2, BGvv3, BGvv4, BGbh1, BGbh2, BGbh3, BGbh4, BGvv1I, BGvv2I, BGvv3I, BGvv4I, sig1I, sig2I, sig3I, sig4I}
  0     1      2     3     4      5      6      7      8      9      10     11     12      13      14      15      16     17     18     19
*/
Double_t Lda30N = CouplingLimitH20(EveAC4[16],EveAC4[0],EveAC4[12],EveAC4[4],EveAC4[8],
				   EveAC4[17],EveAC4[1],EveAC4[13],EveAC4[5],EveAC4[9],
				   EveAC4[18],EveAC4[2],EveAC4[14],EveAC4[6],EveAC4[10],
				   EveAC4[19],EveAC4[3],EveAC4[15],EveAC4[7],EveAC4[11],
				   0.0, 0.0);
Double_t Lda31N = CouplingLimitH20(EveAC4[16],EveAC4[0],EveAC4[12],EveAC4[4],EveAC4[8],
				   EveAC4[17],EveAC4[1],EveAC4[13],EveAC4[5],EveAC4[9],
				   EveAC4[18],EveAC4[2],EveAC4[14],EveAC4[6],EveAC4[10],
				   EveAC4[19],EveAC4[3],EveAC4[15],EveAC4[7],EveAC4[11],
				   0.2, 1.0);
// Unpolarised case
//Double_t Lda30u = Lda*(3./SigSys2(EveAC4[15],EveAC4[12],EveAC4[13],EveAC4[14],0.0,0.0));
//Double_t Lda31u = Lda*(3./SigSys2(EveAC4[15],EveAC4[12],EveAC4[13],EveAC4[14],0.2,1.0));


 if(funit.is_open()){
 funit<<mass[m]<<"\t"<<Lda30op<<"\t"<<Lda31op<<"\t"<<Lda30N<<"\t"<<Lda31N<<endl;
 }else{cout<<"Unable to open file"<<endl; }
 funit.close();
 
// if(funitU.is_open()){
// funitU<<mass[m] <<"\t"<<Lda30u<<"\t"<<Lda31u<<endl;
// }else{cout<<"Unable to open file"<<endl; }
// funitU.close();

//cout<<mass[m]<<"\t"<<Lda30u<<"\t"<<Lda31u<<endl; //<<"\t"<<Lda30op<<"\t"<<Lda31op<<"\t"<<Lda30N<<"\t"<<Lda31N<<endl;

/*
cout<<"::Signal::"<<endl;

cout<< EveAC4[12]<<"\t"<<TotSigop<<endl;

cout<<"::Neutrino::"<<endl;

cout<<EveAC4[13]<<"\t"<<TotBGvvop<<endl;

cout<<"::Bhabha::"<<endl;
cout<<EveAC4[14]<<"\t"<<TotBGbhop<<endl;

cout<<"::Lambda::"<<endl;
cout<<"Unpol \t Opt Pol \t H20 "<<endl;
cout<<Lda30u<<"\t"<<Lda30op<<"\t"<<Lda30N<<endl;
cout<<Lda31u<<"\t"<<Lda31op<<"\t"<<Lda31N<<endl;
*/
}
