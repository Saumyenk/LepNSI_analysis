/****************************************************************************/
/***  This macro PLOTs variables from Delphes output   ***/
/****************************************************************************/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootFilter.h"
#else
class ExRootTreeReader;
class ExRootResult;
class ExRootTreeBranch;
class ExRootTreeWriter;
class ExRootFilter;
#endif
#include "fastjet/Selector.hh"
#include "TObjArray.h"
#include "modules/FastJetFinder.h"
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <TArrayC.h>
#include <string>
#include "TH1.h"                                 // for histrograming
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TBranch.h"
#include "TBranchElement.h"
#include "TFile.h"                     // ROOT, for saving file.
#include  <map>
#include "TTree.h"                     //for Tree file 
#include "TROOT.h"

//#define Lumi 1000
//------------------------------------------------------------------------------
using namespace std;

//------------- User-defined Functions ----------------------------------
//-----------------------------------------------------------------------
void Normalize(TH1F* hist)
{
// Normalizes a 1-d histogram to its bin width and number of events
  if (hist->GetSumw2N() == 0) hist->Sumw2(kTRUE);//hist->Sumw2();
  hist->Scale(1.0 / hist->Integral(),"width");
//  hist->Scale(1.0,"width");
//  hist->Scale(1.0 / hist->Integral());
}

double theta(double eta)
{
double th = 2*TMath::ATan(TMath::Exp(-eta));
return th;
}

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
	return TMath::Abs(eta1-eta2);
}

double sqr(double a)
{
return (a*a);
}

double SigSys(int B1, int B2, int S, double x, double y)
{
//int B = B1 + B2;
double Sig = (Double_t)S/TMath::Sqrt(S+B1+B2+sqr(0.01*(x*B1 + y*B2)));
return Sig;
}

//------------------------------------------------------------------------

void MonoPh(int Pol)
{//int t, 
  gSystem->Load("libDelphes");
  gSystem->cd("/home/saumyen/Research/NSI_Coll/ILC/Histos");
//  gSystem->cd("/home/saumyen/Dropbox/NSI_Col/3Numerical/Codes/ILC");
    
    TFile outfile("/home/saumyen/Research/NSI_Dashboard/output/histograms.root", "RECREATE");
    
  gStyle->SetCanvasPreferGL(1); // For transparent FillColor
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.02);
//gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadTopMargin(0.016);
  gStyle->SetLegendFillColor(-1);
  gStyle->SetLegendBorderSize(1);
  gStyle->SetTextFont(132);
  gStyle->SetLegendFont(132);
  
cout<<"Polarisation Choice-"<<Pol<<endl; 

/******************* Files and Cross-sections **********************/

// For different operator plots 
TString bkgbh = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Bhabha/Pol%d/Bhabha%d.root",Pol,Pol);
TString bkgvv = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Neutrino/Pol%d/Neutrino%d.root",Pol,Pol);
TString sig0 = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Signal/W10M/Pol%d/Signal%d_500.root",Pol,Pol);
TString sig1 = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Signal/W10M/Pol%d/Signal%d_1000.root",Pol,Pol);
TString sig2 = TString::Format("/home/saumyen/Research/NSI_Coll/ILC/Signal/W10M/Pol%d/Signal%d_5000.root",Pol,Pol);
TString sig0I = TString::Format("/home/saumyen/Research/NSI_Coll/WithInterference/Signal%d_500.root",Pol);
TString sig1I = TString::Format("/home/saumyen/Research/NSI_Coll/WithInterference/Signal%d_1000.root",Pol);
TString sig2I = TString::Format("/home/saumyen/Research/NSI_Coll/WithInterference/Signal%d_5000.root",Pol);
TString signal[6] = {sig0, sig1, sig2, sig0I, sig1I, sig2I};
std::vector<TString > inputFiles = {sig0I, sig0, bkgvv, bkgbh};//, signal[2]}; //, signal[3], signal[4], signal[5]};
Int_t ii = inputFiles.size();//      
//const char *legendvalue[8] = {"#nu#bar{#nu} + N#gamma","e^{-}e^{+} + N#gamma","m_{Z^{'}} = 0.5 TeV","m_{Z^{'}} = 1.0 TeV","m_{Z^{'}} = 5.0 TeV","","",""};
const char *legendvalue[8] = {"Full", "Pure", "#nu#bar{#nu} + N#gamma","e^{-}e^{+} + N#gamma"};

   //Book Legends
 TLegend *legend1 = new TLegend(0.16,0.14,0.38,0.38);
  legend1->SetFillColorAlpha(0,0.3);
  legend1->SetTextSize(0.041);
  legend1->SetTextAlign(12);
//legend1->AddEntry((TObject*)0, "", "");
 TLegend *legend2 = new TLegend(0.378,0.13,0.65,0.22);
  legend2->SetFillColor(0);
  legend2->SetTextSize(0.038);
  legend2->SetTextAlign(12);
//  legend2->AddEntry((TObject*)0, "--", "-");

    // Create dummy graphs to represent solid and dashed lines
    TGraph *solidLine = new TGraph();  
    solidLine->SetLineWidth(2); solidLine->SetLineStyle(1);  // Solid line
    TGraph *dashedLine = new TGraph();
    dashedLine->SetLineWidth(2); dashedLine->SetLineStyle(7);  // Dashed line

   // Add entries to legend
   legend2->AddEntry(solidLine, "NSI", "l");
   legend2->AddEntry(dashedLine, "NSI+Interference+BG", "l");


// CSs are for Lda = 3TeV		  		     *********/

/******************* Cut are defined here **************************/
// Baseline-selection			  **************************/
Double_t phot_E = 5.; //		  **************************/
Double_t phot_eta = 2.8; //		  **************************/
Double_t emj_pt = 5;
Double_t emj_eta = 2.8;
// Analysis				  **************************/


 //Booking the Canvases
   TCanvas *c1 = new TCanvas("photE","phot E",900,600);
   TCanvas *c2 = new TCanvas("costhetaph","cosine theta_ph",900,600);
//   TCanvas *c3 = new TCanvas("photETA","phot ETA",900,600);
//   TCanvas *c4 = new TCanvas("photPHI","phot PHI",900,600);
//   TCanvas *c5 = new TCanvas("Missing_mass","Missing mass",900,600);
//   TCanvas *c6 = new TCanvas("Evis","Visible Energy",900,600);
//   TCanvas *c7 = new TCanvas("Eeta","Photon Energy Pseudorapidity",2400,400);
//   TCanvas *c[6] = {c1,c2,c3,c4,c5,c6};
   TCanvas *c[2] = {c1,c2};
//  c[7]->Divide(3,1);
  //Booking the Stacks
   THStack *z1 = new THStack("h1","");
   THStack *z2 = new THStack("h2","");/*
   THStack *z3 = new THStack("h3","");
   THStack *z4 = new THStack("h4","");
   THStack *z5 = new THStack("h5","");
   THStack *z6 = new THStack("h6","");
   THStack *z7 = new THStack("h7","");
   THStack *z8 = new THStack("h8","");
   THStack *z9 = new THStack("h9","");*/
   THStack *Stacks[2] = {z1,z2};//,z3,z4,z5,z6}; //,z7,z8,z9};

//Booking the Histograms
TH1F *histphotE[ii], *histETAphot[ii], *histPHIphot[ii], *histdPHI[ii], *histdETA[ii], *histdR[ii], *histMET[ii], *histCostheta[ii], *histMissMass[ii], *histvisE[ii];
TH2F *histphotEeta[ii];

//Color_t Lcolor[8] = {kTeal-1, kOrange, kRed+1, kGreen+3, kBlue+1, kRed+1, kGreen+3, kBlue+1};
Color_t Lcolor[8] = {kRed+1, kGreen+3, kTeal-1, kOrange, kBlue+1, kRed+1, kGreen+3, kBlue+1};


for(int nfile=0; nfile<ii;nfile++){

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFiles[nfile]);
  
  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  //cout << "** Chain contains " << numberOfEntries << " events" << endl; //To see the total number of events

 // ExRootResult *result = new ExRootResult();
double PI = TMath::Pi();
   histphotE[nfile] = new TH1F(TString::Format("E_{photon}_%d", nfile), "E_{#gamma}", 50, 0.0, 520.0);
//   histETAphot[nfile] = new TH1F(TString::Format("#eta_{photon}_%d", nfile), "#eta_{#gamma}", 42, -3.50, 3.50);
//   histPHIphot[nfile] = new TH1F(TString::Format("#phi_{photon}_%d", nfile), "#phi_{#gamma}", 50, -PI, PI);
//   histMissMass[nfile] = new TH1F(TString::Format("MissingM_%d", nfile), "M_{miss})", 40, 0.0,550.0);
   histCostheta[nfile] = new TH1F(TString::Format("Cos_thetaph_%d", nfile), "cos(#theta_{#gamma})", 50, 0.5, 1.);
//   histvisE[nfile] = new TH1F(TString::Format("E_{visible}_%d", nfile), "E_{vis}", 50, 0.0, 550.0);
//   histphotEeta[nfile] = new TH2F(TString::Format("E-#eta_{photon}_%d", nfile), "E-#eta_{#gamma}", 20, 0.0, 20.0, 40,-PI,PI);

std::vector<TH1F *> Histos = { histphotE[nfile], histCostheta[nfile]}; //, histETAphot[nfile], histPHIphot[nfile], histMissMass[nfile], histvisE[nfile]};
Int_t hh = Histos.size();

for(int k=0; k<hh; k++)
{
Histos[k]->SetDirectory(nullptr);
}

 //Setting the Legends
//if(nfile<5){
if(nfile>1)
{
  legend1->AddEntry(histphotE[nfile], legendvalue[nfile],"f");
}else{
  legend1->AddEntry(histphotE[nfile], legendvalue[nfile],"l");
}
//}
  // Get pointers to branches used in this analysis
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

   Double_t MET(0), PTph(0), Eph(0), ETAph(0), PHIph(0), dPHI(0), dETA(0), dR(0);


  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
   Int_t phot_no(0), bphot_no(0), elec_no(0), muon_no(0), emj_no(0), jet_no(0), charg_no(0);
    Double_t missCtheta(0), Mmiss(0), Evis(0);
    TLorentzVector visible(0.0,0.0,0.0,0.0);
    TLorentzVector visible1(0.0,0.0,0.0,0.0);
        
    for(Int_t j=0;j<branchMissingET->GetEntries();j++)   //Missing energy branch
    {
     misset  = (MissingET *) branchMissingET->At(j);
    }

    for(Int_t j=0;j<branchElectron->GetEntries();j++)   //Electron
    {
      elec[j] = (Electron *) branchElectron->At(j);
    if((elec[j]->PT) > 2. && abs(elec[j]->Eta) < 2.8){elec_no++; }
    charg_no++;}
    for(Int_t j=0;j<branchMuon->GetEntries();j++)   //Muon
    {
      muon[j] = (Muon *) branchMuon->At(j);
    if((muon[j]->PT) > 5. && abs(muon[j]->Eta) < 2.8){muon_no++; }
    charg_no++;}

    for(Int_t j=0;j<branchJet->GetEntries();j++)   //Jet branch
    {
     jet[j] = (Jet *) branchJet->At(j);
     if((jet[j]->PT) > 5 && abs(jet[j]->Eta) < 2.8)
     {
     jet_no++; visible += jet[j]->P4();

     }
     if(jet[j]->Charge != 0){charg_no++;}     //visible += jet[j]->P4();
    }
      
    for(Int_t k = 0; k < branchPhoton->GetEntriesFast(); k++)  // Photon
    {
      // Take first photon
      photon[k] = (Photon*) branchPhoton->At(k);  
     if((photon[k]->E) > 5.0 && photon[k]->Eta < 2.8 && photon[k]->Eta > -2.8){phot_no++; 
     if(k > 0){ visible += photon[k]->P4(); }
     }
    }

    for(Int_t j = 0; j < branchBCalPhoton->GetEntriesFast(); j++) // BCal Photon
    {
      // Take first photon
      bcalphoton[j] = (Photon*) branchBCalPhoton->At(j);
//      if((bcalphoton[j]->PT) > 10.0 && abs(bcalphoton[j]->Eta) > 2.5 ){bphot_no++; }
      bphot_no++; visible1 += bcalphoton[j]->P4();
    }


emj_no = jet_no + elec_no + muon_no;
//if(jet){}
//  CM energy
//TLorentzVector Ecm;
//Ecm.SetPxPyPzE(0.0,0.0,0.0,500.0);
//  Missing energy
//visible = sorted_by_pt(visible);
//     TLorentzVector missE = Ecm - visible - photon[0]->P4();
     //TVector3 V = misset->P4().Vect();
//Double_t sqrtS = 500.0;

// Variables definition
if(phot_no > 0){
//MET = misset->MET;
//MET = missE.E();
//ETAmiss = missE.Eta();
//PHImiss = missE.Phi();
//PTph = photon[0]->P4().E();
Eph = photon[0]->E;
//ETAph = photon[0]->Eta;
//PHIph = photon[0]->Phi;
//dPHI = DPhi(missE->Phi, photon[0]->Phi);
//dETA = DEta(missE.Eta(), photon[0]->Eta);
//dR = DR(missE.Eta(), missE.Phi(), photon[0]->Eta, photon[0]->Phi);
//Mmiss = missE.E();
missCtheta = TMath::Abs(TMath::Cos(photon[0]->P4().Theta()));
//Evis = visible.E();

if(Eph > 5.0 && Eph < 36000.0 && abs(ETAph) < 2.8 && missCtheta < 0.95 && emj_no==0 && bphot_no<1){
   histphotE[nfile] -> Fill(Eph);
//   histETAphot[nfile] -> Fill(ETAph);
//   histPHIphot[nfile] -> Fill(PHIph);
//   histMissMass[nfile] -> Fill(Mmiss);
   histCostheta[nfile] -> Fill(missCtheta);
//   histvisE[nfile] -> Fill(MET);
//   histphotEeta[nfile] -> Fill(photon[0]->PT, photon[0]->Phi);
}
}

} //Event-loop ends

//=========== For Plotting ===================
for(int h = 0; h < hh; ++h){
     Histos[h] ->SetLineColor(Lcolor[nfile]);
if(nfile<2){Histos[h] ->SetLineWidth(3);}
if(nfile>4){Histos[h] ->SetLineStyle(7);}
if(nfile>=2){ Histos[h] ->SetFillColorAlpha(Lcolor[nfile], 0.3);} //Histos[h] ->SetFillColorAlpha(Lcolor[nfile], 0.3); }
//if(nfile<2){ Histos[h] ->SetFillStyle(3744);}
//Histos[h] ->SetLineWidth(2);
     Normalize(Histos[h]);
     Stacks[h] -> Add(Histos[h]);
}

Stacks[0] -> Add(Histos[0]);
if(nfile == 0){
//--------------------------------
// Signal (FULL - Pure + Interference + SM)
//--------------------------------
outfile.cd();

outfile.mkdir("Full");
outfile.cd("Full");

histphotE[nfile]->Write("PhotonE");
histCostheta[nfile]->Write("CosTheta");
}else if(nfile == 1){
//--------------------------------
// Signal (PURE)
//--------------------------------
outfile.cd();

outfile.mkdir("Signal");
outfile.cd("Signal");

histphotE[nfile]->Write("PhotonE");
histCostheta[nfile]->Write("CosTheta");
}else if(nfile == 2){
//--------------------------------
// Neutrino
//--------------------------------
outfile.cd();

outfile.mkdir("NuNu");
outfile.cd("NuNu");

histphotE[nfile]->Write("PhotonE");
histCostheta[nfile]->Write("CosTheta");
}else{
//--------------------------------
// Bhabha
//--------------------------------
outfile.cd();

outfile.mkdir("Bhabha");
outfile.cd("Bhabha");

histphotE[nfile]->Write("PhotonE");
histCostheta[nfile]->Write("CosTheta");

}


} // file-loop
outfile.Write();
outfile.Close();

const char* xTitles[6] = {"#bf{E_{#gamma} [GeV]}", "#bf{cos(#theta_{#gamma})}", "#bf{#eta_{#gamma}}", "#bf{#phi_{#gamma} [rad]}}", "#bf{M_{miss} [GeV]}", "#bf{E_{T}^{miss} [GeV]}"};

const char* yTitles[6] = {"#bf{#frac{1}{N} #frac{dN}{dE_{#gamma}} [GeV^{-1}]}", "#bf{#frac{1}{N} #frac{dN}{dcos(#theta_{#gamma})}}", "#bf{#frac{1}{N} #frac{dN}{d#eta}}", "#bf{#frac{1}{N} #frac{dN}{d#phi} [rad^{-1}]}", "#bf{#frac{1}{N} #frac{dN}{dM_{miss}} [GeV^{-1}]}", "#bf{#frac{1}{N} #frac{dN}{dE_{T}^{miss} [GeV^{-1}])}}" };

for(Int_t i=0; i< 2; i++){ //Loop over canvases to draw and save
//continue;
c[i]->cd();
c[i]->SetTickx();
c[i]->SetTicky();
if(i==0 || i==4 || i==5){c[i]->SetLogy();}
c[i]->SetGrid();
Stacks[i]->Draw("histo nostack");
Stacks[i]->GetXaxis()->SetTitle(xTitles[i]);
Stacks[i]->GetYaxis()->SetTitle(yTitles[i]);
 Stacks[i]->GetXaxis()->SetTitleSize(0.044);
 Stacks[i]->GetYaxis()->SetTitleSize(0.035);
 Stacks[i]->GetXaxis()->SetTitleFont(132);
 Stacks[i]->GetYaxis()->SetTitleFont(132);
//if(i==13){ Stacks[i]->GetYaxis()->SetTitleOffset(1.5); Stacks[i]->GetYaxis()->SetLabelOffset(0.002);  Stacks[i]->GetYaxis()->SetTitleSize(0.037);}

  TLatex latex1;
  latex1.SetTextSize(0.045);
  latex1.SetTextAlign(13);  //default bottom alignment
  TLatex latex2;
  latex2.SetTextSize(0.05);
  latex2.SetTextAlign(13);  //default bottom alignment

//if(i==0){
legend1->Draw(); //legend2->Draw();
//}

//  latex1.DrawLatex(75.,0.02,"#bf{C_{M} = 0, C_{E} = 1}");

//c[i]->Modified(); c[i]->Update(); 
/*
 TString fname(TString::Format("%d_", Pol)+=c[i]->GetName()); fname += TString::Format(".png");
 c[i]->Print(fname,"png");
 TString fname1(TString::Format("%d_", Pol)+=c[i]->GetName()); fname1 += TString::Format(".pdf");
 c[i]->Print(fname1,"pdf");
*/
 TString fname(TString::Format("l0_")+=c[i]->GetName()); fname += TString::Format(".pdf");
 c[i]->Print(fname,"pdf");

} //for-loop (Canvass)


}

