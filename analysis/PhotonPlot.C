void PhotonPlot(){

  int nrun;
  cout<<"Which run?  ";
  cin>>nrun;

  TString outfile = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root",nrun);
  TFile *f1 = new TFile(outfile,"READ");
  TTree *T = (TTree *)f1->Get("T");

  TH1F *h1 = new TH1F("h1","beam on laser on",1000,0,400000);
  TH1F *h2 = new TH1F("h2","beam on laser off",1000,0,400000);
  TH1F *h3 = new TH1F("h3","beam off laser on",1000,0,400000);
  TH1F *h4 = new TH1F("h4","beam off laser off",1000,0,400000);

  TCanvas *c1 = new TCanvas("c1","c1",1500,1500); 

  T->Draw("fadc_a[0]>>h1","fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM>3500 && curCavPower>100");
  T->Draw("fadc_a[0]>>h2","fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM>3500 && curCavPower<40","same");
  T->Draw("fadc_a[0]>>h3","fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM==0 && curCavPower>100","same");
  T->Draw("fadc_a[0]>>h4","fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM==0 && curCavPower<40","same");
/*
  T->Draw("fadc_a[0]>>h1","fadc_t[0]*0.0625>4 && curBCM>3500 && curCavPower>100");
  T->Draw("fadc_a[0]>>h2","fadc_t[0]*0.0625>4 && curBCM>3500 && curCavPower<40","same");
  T->Draw("fadc_a[0]>>h3","fadc_t[0]*0.0625>4 && curBCM==0 && curCavPower>100","same");
  T->Draw("fadc_a[0]>>h4","fadc_t[0]*0.0625>4 && curBCM==0 && curCavPower<40","same");
*/

  Double_t n1 = h1->Integral();
  Double_t n2 = h2->Integral();
  Double_t n3 = h3->Integral();
  Double_t n4 = h4->Integral();
/*
  h2->Scale(n1/n2);
  h3->Scale(n1/n3);
  h4->Scale(n1/n4);

  h1->Draw();
  h2->Draw("same HIST");
  h3->Draw("same HIST");
  h4->Draw("same HIST");
*/
  gStyle->SetOptStat(0);

  h1->SetLineColor(2);
  h1->SetLineWidth(3);
  h2->SetLineColor(4);
  h2->SetLineWidth(3);
  h3->SetLineColor(1);
  h3->SetLineWidth(3);
  h4->SetLineColor(8);
  h4->SetLineWidth(3);

  h1->GetXaxis()->SetTitle("fadc_a[0]");

  gStyle->SetOptStat(0);
  auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
  leg1->AddEntry(h1,"beam on laser on","L");
  leg1->AddEntry(h2,"beam on laser off","L");
  leg1->AddEntry(h3,"beam off laser on","L");
  leg1->AddEntry(h4,"beam off laser off","L");
  leg1->Draw(); 

  TLatex tex;
  tex.SetNDC();
  tex.SetTextSize(0.025);
  tex.DrawLatex(0.7,0.5,"20<t<448 ns");
  tex.DrawLatex(0.55,0.5,Form("run %d",nrun));

//  gPad->SetLogy();
//  c1->Update();

}
