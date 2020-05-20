void TriggerTime(){

  TString outfile = "/home/compton/hanjie/compana1/Rootfiles/eDet_1073.root";
  TFile *f1 = new TFile(outfile,"READ");
  TTree *T = (TTree *)f1->Get("T");

  /***  VETROC planes rising time ***/
/* 
  TH1F *hEA = new TH1F("hEA","plane A rising time",200,0,2100);
  TH1F *hEB = new TH1F("hEB","plane B rising time",200,0,2100);
  TH1F *hEC = new TH1F("hEC","plane C rising time",200,0,2100);
  TH1F *hED = new TH1F("hED","plane D rising time",200,0,2100);

  TCanvas *c1 = new TCanvas("c1","c1",1500,1500); 

  T->Draw("eplaneA_rt>>hEA","");
  T->Draw("eplaneB_rt>>hEB","");
  T->Draw("eplaneC_rt>>hEC","");
  T->Draw("eplaneD_rt>>hED","");

  Double_t n1 = hEA->Integral();
  Double_t n2 = hEB->Integral();
  Double_t n3 = hEC->Integral();
  Double_t n4 = hED->Integral();

  hEB->Scale(n1/n2);
  hEC->Scale(n1/n3);
  hED->Scale(n1/n4);

  hEA->SetLineColor(2);
  hEA->SetLineWidth(4);
  hEB->SetLineColor(4);
  hEB->SetLineWidth(4);
  hEC->SetLineColor(1);
  hEC->SetLineWidth(4);
  hED->SetLineColor(8);
  hED->SetLineWidth(4);

  hEA->Draw();
  hEB->Draw("same HIST");
  hEC->Draw("same HIST");
  hED->Draw("same HIST");

  gStyle->SetOptStat(0);
  auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
  leg1->AddEntry(hEA,"plane A","L");
  leg1->AddEntry(hEB,"plane B","L");
  leg1->AddEntry(hEC,"plane C","L");
  leg1->AddEntry(hED,"plane D","L");
  leg1->Draw(); 

  //gPad->SetLogy();
//  c1->Update();
*/
  /***   FADC photon signal time ***/

  TH1F *hP = new TH1F("hP","photon time",200,0,800);

  TCanvas *c2 = new TCanvas("c2","c2",1500,1500); 

  T->Draw("fadc_t[0]*0.0625>>hP","fadc_t[0]>0");

  hP->SetLineWidth(4);
  hP->GetXaxis()->SetTitle("fadc_t[0] (ns)");

  hP->Draw();

  gStyle->SetOptStat(0);
  /*
  auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
  leg1->AddEntry(hEA,"plane A","L");
  leg1->Draw(); 
*/


}
