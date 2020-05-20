void PhotonAsym(){
     Double_t ped = 21024.5;
     Double_t gain = 0.00033;
     Double_t offset = -1.1678;

     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber);

     TFile *f0 = new TFile(filename);
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTP = (TTree*) f0->Get("VTP");

     Int_t nentries=0;
     nentries = VTP->GetEntries();
     cout<<"Total entries:  "<<nentries<<endl;

	 T->AddFriend(VTP);

     TH1F *h11_p = new TH1F("h11_p","photon distribution beam on laser on hel=1",100,0,100000);
     TH1F *h11_m = new TH1F("h11_m","photon distribution beam on laser on hel=0",100,0,100000);

     TH1F *h10_p = new TH1F("h10_p","photon distribution beam on laser off hel=1",100,0,100000);
     TH1F *h10_m = new TH1F("h10_m","photon distribution beam on laser off hel=0",100,0,100000);

     TH1F *h01_p = new TH1F("h01_p","photon distribution beam off laser on hel=1",100,0,100000);
     TH1F *h01_m = new TH1F("h01_m","photon distribution beam off laser on hel=0",100,0,100000);

     TH1F *h00_p = new TH1F("h00_p","photon distribution beam off laser on hel=1",100,0,100000);
     TH1F *h00_m = new TH1F("h00_m","photon distribution beam off laser on hel=0",100,0,100000);

  T->Draw(Form("fadc_a[0]-%f>>h11_p",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM>3500 && curCavPower>100 && current_helicity==1");// && Entry$<10000");
  T->Draw(Form("fadc_a[0]-%f>>h11_m",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM>3500 && curCavPower>100 && current_helicity==0");// && Entry$<10000");

  T->Draw(Form("fadc_a[0]-%f>>h10_p",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM>3500 && curCavPower<40 && current_helicity==1");// && Entry$<10000");
  T->Draw(Form("fadc_a[0]-%f>>h10_m",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM>3500 && curCavPower<40 && current_helicity==0");// && Entry$<10000");

  T->Draw(Form("fadc_a[0]-%f>>h01_p",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM==0 && curCavPower>100 && current_helicity==1");// && Entry$<10000");
  T->Draw(Form("fadc_a[0]-%f>>h01_m",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM==0 && curCavPower>100 && current_helicity==0");// && Entry$<10000");

  T->Draw(Form("(fadc_a[0]-%f)>>h00_p",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM==0 && curCavPower<40 && current_helicity==1");// && Entry$<10000");
  T->Draw(Form("(fadc_a[0]-%f)>>h00_m",ped),"fadc_t[0]*0.0625>20 && fadc_t[0]*0.0625<448 && curBCM==0 && curCavPower<40 && current_helicity==0");// && Entry$<10000");

  TGraphErrors *g11 = new TGraphErrors();
  TGraphErrors *g10 = new TGraphErrors();
  TGraphErrors *g01 = new TGraphErrors();
  TGraphErrors *g00 = new TGraphErrors();

  int nbins = h11_p->GetNbinsX();
  cout<<"nbins:  "<<nbins<<endl;

  for(int ii=0; ii<nbins; ii++){
	Double_t n11p = h11_p->GetBinContent(ii+1);
	Double_t n11m = h11_m->GetBinContent(ii+1);
    Double_t n11p_C = h11_p->Integral();
    Double_t n11m_C = h11_m->Integral();

	Double_t asym11=0,asym11_err=0,N11p=0,N11p_err=0,N11m=0,N11m_err=0;
	if(n11p_C!=0){
	  N11p = n11p/n11p_C;
	  N11p_err = sqrt(n11p)/n11p_C;
	}
	if(n11m_C!=0){
	 N11m = n11m/n11m_C;
	 N11m_err = sqrt(n11m)/n11m_C;
	}
	if((n11p+n11m) != 0){
      asym11 = (N11p-N11m)/(N11p+N11m);
	  asym11_err = 2./pow(N11p+N11m,2)*sqrt(pow(N11m_err*N11p,2)+pow(N11m*N11p_err,2));
    }
	Double_t binc = h11_p->GetBinCenter(ii+1);
	g11->SetPoint(ii,binc,asym11);
	g11->SetPointError(ii,0,asym11_err);

	Double_t n10p = h10_p->GetBinContent(ii+1);
	Double_t n10m = h10_m->GetBinContent(ii+1);
    Double_t n10p_C = h10_p->Integral();
    Double_t n10m_C = h10_m->Integral();
	Double_t asym10=0,asym10_err=0,N10p=0,N10p_err=0,N10m=0,N10m_err=0;
	if(n10p_C!=0){
	  N10p = n10p/n10p_C;
	  N10p_err = sqrt(n10p)/n10p_C;
	}
	if(n10m_C!=0){
	 N10m = n10m/n10m_C;
	 N10m_err = sqrt(n10m)/n10m_C;
	}
	if((n10p+n10m) != 0){
      asym10 = (N10p-N10m)/(N10p+N10m);
	  asym10_err = 2./pow(N10p+N10m,2)*sqrt(pow(N10m_err*N10p,2)+pow(N10m*N10p_err,2));
    }
	binc = h10_p->GetBinCenter(ii+1);
	g10->SetPoint(ii,binc,asym10);
	g10->SetPointError(ii,0,asym10_err);
   }

  int nbins1 = h01_p->GetNbinsX();
  cout<<"nbins:  "<<nbins1<<endl;

  for(int ii=0; ii<nbins1; ii++){
	Double_t n01p = h01_p->GetBinContent(ii+1);
	Double_t n01m = h01_m->GetBinContent(ii+1);
    Double_t n01p_C = h01_p->Integral();
    Double_t n01m_C = h01_m->Integral();

	Double_t asym01=0,asym01_err=0,N01p=0,N01p_err=0,N01m=0,N01m_err=0;
	if(n01p_C!=0){
	  N01p = n01p/n01p_C;
	  N01p_err = sqrt(n01p)/n01p_C;
	}
	if(n01m_C!=0){
	 N01m = n01m/n01m_C;
	 N01m_err = sqrt(n01m)/n01m_C;
	}
	if((n01p+n01m) != 0){
      asym01 = (N01p-N01m)/(N01p+N01m);
	  asym01_err = 2./pow(N01p+N01m,2)*sqrt(pow(N01m_err*N01p,2)+pow(N01m*N01p_err,2));
    }
	Double_t binc = h01_p->GetBinCenter(ii+1);
	g01->SetPoint(ii,binc,asym01);
	g01->SetPointError(ii,0,asym01_err);

	Double_t n00p = h00_p->GetBinContent(ii+1);
	Double_t n00m = h00_m->GetBinContent(ii+1);
    Double_t n00p_C = h00_p->Integral();
    Double_t n00m_C = h00_m->Integral();

	Double_t asym00=0,asym00_err=0,N00p=0,N00p_err=0,N00m=0,N00m_err=0;
	if(n00p_C!=0){
	  N00p = n00p/n00p_C;
	  N00p_err = sqrt(n00p)/n00p_C;
	}
	if(n00m_C!=0){
	 N00m = n00m/n00m_C;
	 N00m_err = sqrt(n00m)/n00m_C;
	}
	if((n00p+n00m) != 0){
      asym00 = (N00p-N00m)/(N00p+N00m);
	  asym00_err = 2./pow(N00p+N00m,2)*sqrt(pow(N00m_err*N00p,2)+pow(N00m*N00p_err,2));
    }
	binc = h00_p->GetBinCenter(ii+1);
	g00->SetPoint(ii,binc,asym00);
	g00->SetPointError(ii,0,asym00_err);

  }

  TCanvas *c1 = new TCanvas("c1","c1",1500,1200);
  c1->Divide(2,2);
  c1->cd(1);
  g11->SetMarkerStyle(8);
  g11->SetMarkerColor(4);
  g11->Draw("AP");
  g11->SetTitle("beam on laser on;fadc_ac[0];asym");

  c1->cd(2);
  g10->SetMarkerStyle(8);
  g10->SetMarkerColor(4);
  g10->Draw("AP");
  g10->SetTitle("beam on laser off;fadc_ac[0];asym");

  c1->cd(3);
  g01->SetMarkerStyle(8);
  g01->SetMarkerColor(4);
  g01->Draw("AP");
  g01->SetTitle("beam off laser on;fadc_ac[0];asym");

  c1->cd(4);
  g00->SetMarkerStyle(8);
  g00->SetMarkerColor(4);
  g00->Draw("AP");
  g00->SetTitle("beam off laser off;fadc_ac[0];asym");

  TCanvas *c2 = new TCanvas("c2","c2",1500,1000);
  c2->Divide(2,1);
  c2->cd(1);
  h11_p->SetLineColor(2);
  h11_p->SetLineWidth(2);

  h10_p->SetLineColor(4);
  h10_p->SetLineWidth(2);

  h01_p->SetLineColor(1);
  h01_p->SetLineWidth(2);

  h00_p->SetLineColor(8);
  h00_p->SetLineWidth(2);

  h11_p->Draw();
  h10_p->Draw("same");
  h01_p->Draw("same");
  h00_p->Draw("same");
  h11_p->SetTitle("hel=1;fadc_a[0]");

  TLegend *leg = new TLegend(0.7,0.7,0.85,0.85);
  leg->AddEntry(h11_p,"beam on laser on","L");
  leg->AddEntry(h10_p,"beam off laser off","L");
  leg->AddEntry(h01_p,"beam on laser on","L");
  leg->AddEntry(h00_p,"beam off laser off","L");
  leg->Draw();

  c2->cd(2);
  h11_m->SetLineColor(2);
  h11_m->SetLineWidth(2);

  h10_m->SetLineColor(4);
  h10_m->SetLineWidth(2);

  h01_m->SetLineColor(1);
  h01_m->SetLineWidth(2);

  h00_m->SetLineColor(8);
  h00_m->SetLineWidth(2);

  h11_m->Draw();
  h10_m->Draw("same");
  h01_m->Draw("same");
  h00_m->Draw("same");
  h11_m->SetTitle("hel=0;fadc_a[0]");

  TLegend *leg1 = new TLegend(0.7,0.7,0.85,0.85);
  leg1->AddEntry(h11_m,"beam on laser on","L");
  leg1->AddEntry(h10_m,"beam off laser off","L");
  leg1->AddEntry(h01_m,"beam on laser on","L");
  leg1->AddEntry(h00_m,"beam off laser off","L");
  leg1->Draw();

}
