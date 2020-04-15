#include "../SetParams.h"

void StripRate()
{
     int runnumber=0;
     cout<<"Which run ?  ";
	 cin>>runnumber;

     Double_t gain = 0.00033;
	 Double_t offset = -1.1678;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *VTP = (TTree*) f0->Get("VTP");

     int nstrip[43];
     for(int ii=0;ii<12;ii++) nstrip[ii]=ii;
     for(int ii=16;ii<28;ii++) nstrip[ii-4]=ii;
     for(int ii=36;ii<44;ii++) nstrip[ii-12]=ii;
     for(int ii=49;ii<60;ii++) nstrip[ii-17]=ii;

     Int_t vtpA_scal[128]={0},vtpB_scal[128]={0},vtpC_scal[128]={0},vtpD_scal[128]={0};
	 Int_t vtp_scaldat[16]={0};
	 Int_t current_hel=0;

     Int_t nentries=0;
     nentries = VTP->GetEntries();
	 cout<<"Total entries:  "<<nentries<<endl;
	 Int_t maxevent=0;
	 cout<<"How many events ? (0 for total) ";
	 cin>>maxevent;

	 if(maxevent==0)maxevent=nentries;

     VTP->SetBranchAddress("eplaneA_scalcnt",vtpA_scal);
     VTP->SetBranchAddress("eplaneB_scalcnt",vtpB_scal);
     VTP->SetBranchAddress("eplaneC_scalcnt",vtpC_scal);
     VTP->SetBranchAddress("eplaneD_scalcnt",vtpD_scal);
     VTP->SetBranchAddress("scaldat",vtp_scaldat);

	 TH1F *hA_laseroff[43];
	 TH1F *hA_laseron[43];
	 TH1F *hB_laseroff[43];
	 TH1F *hB_laseron[43];
	 TH1F *hC_laseroff[43];
	 TH1F *hC_laseron[43];
	 TH1F *hD_laseroff[43];
	 TH1F *hD_laseron[43];

     for(int ii=0;ii<43;ii++){
	    int strip_num = nstrip[ii];
		hA_laseroff[ii] = new TH1F(Form("hA_laseroff_%d",strip_num),Form("plane A chan %d strip rate (laser off)",strip_num),20,0,20);
		hA_laseron[ii] = new TH1F(Form("hA_laseron_%d",strip_num),Form("plane A chan %d strip rate (laser on)",strip_num),20,0,20);
		hB_laseroff[ii] = new TH1F(Form("hB_laseroff_%d",strip_num),Form("plane A chan %d strip rate (laser off)",strip_num),40,0,40);
		hB_laseron[ii] = new TH1F(Form("hB_laseron_%d",strip_num),Form("plane A chan %d strip rate (laser on)",strip_num),40,0,40);
		hC_laseroff[ii] = new TH1F(Form("hC_laseroff_%d",strip_num),Form("plane A chan %d strip rate (laser off)",strip_num),150,0,150);
		hC_laseron[ii] = new TH1F(Form("hC_laseron_%d",strip_num),Form("plane A chan %d strip rate (laser on)",strip_num),150,0,150);
		hD_laseroff[ii] = new TH1F(Form("hD_laseroff_%d",strip_num),Form("plane A chan %d strip rate (laser off)",strip_num),400,0,400);
		hD_laseron[ii] = new TH1F(Form("hD_laseron_%d",strip_num),Form("plane A chan %d strip rate (laser on)",strip_num),400,0,400);
	 }

	 Double_t maxhit_A=0,maxhit_B=0,maxhit_C=0,maxhit_D=0;
     Int_t pre_hel_win_cnt=0;
	 bool update=false;   // mark if the vtp scaler is read out;
     for(int nn=0;nn<maxevent;nn++){
	   VTP->GetEntry(nn);
	   if(vtp_scaldat[1]==0) continue;  // clock is 0; the scaler is not read out

	   Double_t BCM = vtp_scaldat[15];
	   Double_t CavPower = vtp_scaldat[12];   
	   Double_t helQ = (BCM*gain/(1./120.)+offset)*(1./120.);
	   for(int jj=0;jj<43;jj++){
	    int strip_num=nstrip[jj]; 

		if(BCM>3350 && CavPower<40){    // beam on, laser off
	 	  if(vtpA_scal[strip_num]>0) hA_laseroff[jj]->Fill(vtpA_scal[strip_num]*1.0/helQ);
	 	  if(vtpB_scal[strip_num]>0) hB_laseroff[jj]->Fill(vtpB_scal[strip_num]*1.0/helQ);
	 	  if(vtpC_scal[strip_num]>0) hC_laseroff[jj]->Fill(vtpC_scal[strip_num]*1.0/helQ);
	 	  if(vtpD_scal[strip_num]>0) hD_laseroff[jj]->Fill(vtpD_scal[strip_num]*1.0/helQ);
		 }

		if(BCM>3350 && CavPower>100){
	 	  if(vtpA_scal[strip_num]>0) hA_laseron[jj]->Fill(vtpA_scal[strip_num]*1.0/helQ);
	 	  if(vtpB_scal[strip_num]>0) hB_laseron[jj]->Fill(vtpB_scal[strip_num]*1.0/helQ);
	 	  if(vtpC_scal[strip_num]>0) hC_laseron[jj]->Fill(vtpC_scal[strip_num]*1.0/helQ);
	 	  if(vtpD_scal[strip_num]>0) hD_laseron[jj]->Fill(vtpD_scal[strip_num]*1.0/helQ);

		  if(vtpA_scal[strip_num]*1.0/helQ > maxhit_A) maxhit_A=vtpA_scal[strip_num]*1.0/helQ;
		  if(vtpB_scal[strip_num]*1.0/helQ > maxhit_B) maxhit_B=vtpB_scal[strip_num]*1.0/helQ;
		  if(vtpC_scal[strip_num]*1.0/helQ > maxhit_C) maxhit_C=vtpB_scal[strip_num]*1.0/helQ;
		  if(vtpD_scal[strip_num]*1.0/helQ > maxhit_D) maxhit_D=vtpD_scal[strip_num]*1.0/helQ;
		 }
 	   }
	 }	
	 delete VTP;

     cout<<"maxhit A:   "<<maxhit_A<<endl;
     cout<<"maxhit B:   "<<maxhit_B<<endl;
     cout<<"maxhit C:   "<<maxhit_C<<endl;
     cout<<"maxhit D:   "<<maxhit_D<<endl;
/*
    TCanvas *cA1=new TCanvas("cA1","cA1",1500,1500);
    cA1->Divide(2,4);
    for(int ii=0;ii<8;ii++){
       cA1->cd(ii+1);
       Double_t n1 = hA_laseron[ii]->GetEntries();
       Double_t n2 = hA_laseroff[ii]->GetEntries();
	   Double_t mean_on = hA_laseron[ii]->GetMean();
	   Double_t mean_off = hA_laseroff[ii]->GetMean();
       hA_laseron[ii]->Draw();
       //hA_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hA_laseron[ii]->SetTitle(Form("plane A chan %d; cnts/Q;",nstrip[ii]));

       hA_laseroff[ii]->Draw("same");
       hA_laseroff[ii]->Scale(n1/n2);
       hA_laseroff[ii]->SetLineColor(2); 
       hA_laseroff[ii]->SetLineWidth(2);
       hA_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hA_laseron[ii],"laser on","L");
       leg->AddEntry(hA_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cA2=new TCanvas("cA2","cA2",1500,1500);
    cA2->Divide(2,4);
    for(int ii=8;ii<16;ii++){
       cA2->cd(ii-7);
       Double_t n1 = hA_laseron[ii]->GetEntries();
       Double_t n2 = hA_laseroff[ii]->GetEntries();
	   Double_t mean_on = hA_laseron[ii]->GetMean();
	   Double_t mean_off = hA_laseroff[ii]->GetMean();
       hA_laseron[ii]->Draw();
       //hA_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hA_laseron[ii]->SetTitle(Form("plane A chan %d; cnts/Q;",nstrip[ii]));

       hA_laseroff[ii]->Draw("same");
       hA_laseroff[ii]->Scale(n1/n2);
       hA_laseroff[ii]->SetLineColor(2); 
       hA_laseroff[ii]->SetLineWidth(2);
       hA_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hA_laseron[ii],"laser on","L");
       leg->AddEntry(hA_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cA3=new TCanvas("cA3","cA3",1500,1500);
    cA3->Divide(2,4);
    for(int ii=16;ii<24;ii++){
       cA3->cd(ii-15);
       Double_t n1 = hA_laseron[ii]->GetEntries();
       Double_t n2 = hA_laseroff[ii]->GetEntries();
	   Double_t mean_on = hA_laseron[ii]->GetMean();
	   Double_t mean_off = hA_laseroff[ii]->GetMean();
       hA_laseron[ii]->Draw();
       //hA_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hA_laseron[ii]->SetTitle(Form("plane A chan %d; cnts/Q;",nstrip[ii]));

       hA_laseroff[ii]->Draw("same");
       hA_laseroff[ii]->Scale(n1/n2);
       hA_laseroff[ii]->SetLineColor(2); 
       hA_laseroff[ii]->SetLineWidth(2);
       hA_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hA_laseron[ii],"laser on","L");
       leg->AddEntry(hA_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cA4=new TCanvas("cA4","cA4",1500,1500);
    cA4->Divide(2,4);
    for(int ii=24;ii<32;ii++){
       cA4->cd(ii-23);
       Double_t n1 = hA_laseron[ii]->GetEntries();
       Double_t n2 = hA_laseroff[ii]->GetEntries();
	   Double_t mean_on = hA_laseron[ii]->GetMean();
	   Double_t mean_off = hA_laseroff[ii]->GetMean();
       hA_laseron[ii]->Draw();
       //hA_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hA_laseron[ii]->SetTitle(Form("plane A chan %d; cnts/Q;",nstrip[ii]));

       hA_laseroff[ii]->Draw("same");
       hA_laseroff[ii]->Scale(n1/n2);
       hA_laseroff[ii]->SetLineColor(2); 
       hA_laseroff[ii]->SetLineWidth(2);
       hA_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hA_laseron[ii],"laser on","L");
       leg->AddEntry(hA_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cA5=new TCanvas("cA5","cA5",1500,1500);
    cA5->Divide(2,4);
    for(int ii=32;ii<40;ii++){
       cA5->cd(ii-31);
       Double_t n1 = hA_laseron[ii]->GetEntries();
       Double_t n2 = hA_laseroff[ii]->GetEntries();
	   Double_t mean_on = hA_laseron[ii]->GetMean();
	   Double_t mean_off = hA_laseroff[ii]->GetMean();
       hA_laseron[ii]->Draw();
       //hA_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hA_laseron[ii]->SetTitle(Form("plane A chan %d; cnts/Q;",nstrip[ii]));

       hA_laseroff[ii]->Draw("same");
       hA_laseroff[ii]->Scale(n1/n2);
       hA_laseroff[ii]->SetLineColor(2); 
       hA_laseroff[ii]->SetLineWidth(2);
       hA_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hA_laseron[ii],"laser on","L");
       leg->AddEntry(hA_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cA6=new TCanvas("cA6","cA6",1500,1500);
    cA6->Divide(2,4);
    for(int ii=40;ii<43;ii++){
       cA6->cd(ii-39);
       Double_t n1 = hA_laseron[ii]->GetEntries();
       Double_t n2 = hA_laseroff[ii]->GetEntries();
	   Double_t mean_on = hA_laseron[ii]->GetMean();
	   Double_t mean_off = hA_laseroff[ii]->GetMean();
       hA_laseron[ii]->Draw();
       //hA_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hA_laseron[ii]->SetTitle(Form("plane A chan %d; cnts/Q;",nstrip[ii]));

       hA_laseroff[ii]->Draw("same");
       hA_laseroff[ii]->Scale(n1/n2);
       hA_laseroff[ii]->SetLineColor(2); 
       hA_laseroff[ii]->SetLineWidth(2);
       hA_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hA_laseron[ii],"laser on","L");
       leg->AddEntry(hA_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

   cA1->Print("A_striprate.pdf[");
   cA1->Print("A_striprate.pdf");
   cA2->Print("A_striprate.pdf");
   cA3->Print("A_striprate.pdf");
   cA4->Print("A_striprate.pdf");
   cA5->Print("A_striprate.pdf");
   cA6->Print("A_striprate.pdf");
   cA6->Print("A_striprate.pdf]");


    TCanvas *cB1=new TCanvas("cB1","cB1",1500,1500);
    cB1->Divide(2,4);
    for(int ii=0;ii<8;ii++){
       cB1->cd(ii+1);
       Double_t n1 = hB_laseron[ii]->GetEntries();
       Double_t n2 = hB_laseroff[ii]->GetEntries();
	   Double_t mean_on = hB_laseron[ii]->GetMean();
	   Double_t mean_off = hB_laseroff[ii]->GetMean();
       hB_laseron[ii]->Draw();
       //hB_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hB_laseron[ii]->SetTitle(Form("plane B chan %d; cnts/Q;",nstrip[ii]));

       hB_laseroff[ii]->Draw("same");
       hB_laseroff[ii]->Scale(n1/n2);
       hB_laseroff[ii]->SetLineColor(2); 
       hB_laseroff[ii]->SetLineWidth(2);
       hB_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hB_laseron[ii],"laser on","L");
       leg->AddEntry(hB_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cB2=new TCanvas("cB2","cB2",1500,1500);
    cB2->Divide(2,4);
    for(int ii=8;ii<16;ii++){
       cB2->cd(ii-7);
       Double_t n1 = hB_laseron[ii]->GetEntries();
       Double_t n2 = hB_laseroff[ii]->GetEntries();
	   Double_t mean_on = hB_laseron[ii]->GetMean();
	   Double_t mean_off = hB_laseroff[ii]->GetMean();
       hB_laseron[ii]->Draw();
       //hB_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hB_laseron[ii]->SetTitle(Form("plane B chan %d; cnts/Q;",nstrip[ii]));

       hB_laseroff[ii]->Draw("same");
       hB_laseroff[ii]->Scale(n1/n2);
       hB_laseroff[ii]->SetLineColor(2); 
       hB_laseroff[ii]->SetLineWidth(2);
       hB_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hB_laseron[ii],"laser on","L");
       leg->AddEntry(hB_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cB3=new TCanvas("cB3","cB3",1500,1500);
    cB3->Divide(2,4);
    for(int ii=16;ii<24;ii++){
       cB3->cd(ii-15);
       Double_t n1 = hB_laseron[ii]->GetEntries();
       Double_t n2 = hB_laseroff[ii]->GetEntries();
	   Double_t mean_on = hB_laseron[ii]->GetMean();
	   Double_t mean_off = hB_laseroff[ii]->GetMean();
       hB_laseron[ii]->Draw();
       //hB_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hB_laseron[ii]->SetTitle(Form("plane B chan %d; cnts/Q;",nstrip[ii]));

       hB_laseroff[ii]->Draw("same");
       hB_laseroff[ii]->Scale(n1/n2);
       hB_laseroff[ii]->SetLineColor(2); 
       hB_laseroff[ii]->SetLineWidth(2);
       hB_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hB_laseron[ii],"laser on","L");
       leg->AddEntry(hB_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cB4=new TCanvas("cB4","cB4",1500,1500);
    cB4->Divide(2,4);
    for(int ii=24;ii<32;ii++){
       cB4->cd(ii-23);
       Double_t n1 = hB_laseron[ii]->GetEntries();
       Double_t n2 = hB_laseroff[ii]->GetEntries();
	   Double_t mean_on = hB_laseron[ii]->GetMean();
	   Double_t mean_off = hB_laseroff[ii]->GetMean();
       hB_laseron[ii]->Draw();
       //hB_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hB_laseron[ii]->SetTitle(Form("plane B chan %d; cnts/Q;",nstrip[ii]));

       hB_laseroff[ii]->Draw("same");
       hB_laseroff[ii]->Scale(n1/n2);
       hB_laseroff[ii]->SetLineColor(2); 
       hB_laseroff[ii]->SetLineWidth(2);
       hB_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hB_laseron[ii],"laser on","L");
       leg->AddEntry(hB_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cB5=new TCanvas("cB5","cB5",1500,1500);
    cB5->Divide(2,4);
    for(int ii=32;ii<40;ii++){
       cB5->cd(ii-31);
       Double_t n1 = hB_laseron[ii]->GetEntries();
       Double_t n2 = hB_laseroff[ii]->GetEntries();
	   Double_t mean_on = hB_laseron[ii]->GetMean();
	   Double_t mean_off = hB_laseroff[ii]->GetMean();
       hB_laseron[ii]->Draw();
       //hB_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hB_laseron[ii]->SetTitle(Form("plane B chan %d; cnts/Q;",nstrip[ii]));

       hB_laseroff[ii]->Draw("same");
       hB_laseroff[ii]->Scale(n1/n2);
       hB_laseroff[ii]->SetLineColor(2); 
       hB_laseroff[ii]->SetLineWidth(2);
       hB_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hB_laseron[ii],"laser on","L");
       leg->AddEntry(hB_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cB6=new TCanvas("cB6","cB6",1500,1500);
    cB6->Divide(2,4);
    for(int ii=40;ii<43;ii++){
       cB6->cd(ii-39);
       Double_t n1 = hB_laseron[ii]->GetEntries();
       Double_t n2 = hB_laseroff[ii]->GetEntries();
	   Double_t mean_on = hB_laseron[ii]->GetMean();
	   Double_t mean_off = hB_laseroff[ii]->GetMean();
       hB_laseron[ii]->Draw();
       //hB_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hB_laseron[ii]->SetTitle(Form("plane B chan %d; cnts/Q;",nstrip[ii]));

       hB_laseroff[ii]->Draw("same");
       hB_laseroff[ii]->Scale(n1/n2);
       hB_laseroff[ii]->SetLineColor(2); 
       hB_laseroff[ii]->SetLineWidth(2);
       hB_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hB_laseron[ii],"laser on","L");
       leg->AddEntry(hB_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

   cB1->Print("B_striprate.pdf[");
   cB1->Print("B_striprate.pdf");
   cB2->Print("B_striprate.pdf");
   cB3->Print("B_striprate.pdf");
   cB4->Print("B_striprate.pdf");
   cB5->Print("B_striprate.pdf");
   cB6->Print("B_striprate.pdf");
   cB6->Print("B_striprate.pdf]");


    TCanvas *cC1=new TCanvas("cC1","cC1",1500,1500);
    cC1->Divide(2,4);
    for(int ii=0;ii<8;ii++){
       cC1->cd(ii+1);
       Double_t n1 = hC_laseron[ii]->GetEntries();
       Double_t n2 = hC_laseroff[ii]->GetEntries();
	   Double_t mean_on = hC_laseron[ii]->GetMean();
	   Double_t mean_off = hC_laseroff[ii]->GetMean();
       hC_laseron[ii]->Draw();
       //hC_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hC_laseron[ii]->SetTitle(Form("plane C chan %d; cnts/Q;",nstrip[ii]));

       hC_laseroff[ii]->Draw("same");
       hC_laseroff[ii]->Scale(n1/n2);
       hC_laseroff[ii]->SetLineColor(2); 
       hC_laseroff[ii]->SetLineWidth(2);
       hC_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hC_laseron[ii],"laser on","L");
       leg->AddEntry(hC_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cC2=new TCanvas("cC2","cC2",1500,1500);
    cC2->Divide(2,4);
    for(int ii=8;ii<16;ii++){
       cC2->cd(ii-7);
       Double_t n1 = hC_laseron[ii]->GetEntries();
       Double_t n2 = hC_laseroff[ii]->GetEntries();
	   Double_t mean_on = hC_laseron[ii]->GetMean();
	   Double_t mean_off = hC_laseroff[ii]->GetMean();
       hC_laseron[ii]->Draw();
       //hC_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hC_laseron[ii]->SetTitle(Form("plane C chan %d; cnts/Q;",nstrip[ii]));

       hC_laseroff[ii]->Draw("same");
       hC_laseroff[ii]->Scale(n1/n2);
       hC_laseroff[ii]->SetLineColor(2); 
       hC_laseroff[ii]->SetLineWidth(2);
       hC_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hC_laseron[ii],"laser on","L");
       leg->AddEntry(hC_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cC3=new TCanvas("cC3","cC3",1500,1500);
    cC3->Divide(2,4);
    for(int ii=16;ii<24;ii++){
       cC3->cd(ii-15);
       Double_t n1 = hC_laseron[ii]->GetEntries();
       Double_t n2 = hC_laseroff[ii]->GetEntries();
	   Double_t mean_on = hC_laseron[ii]->GetMean();
	   Double_t mean_off = hC_laseroff[ii]->GetMean();
       hC_laseron[ii]->Draw();
       //hC_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hC_laseron[ii]->SetTitle(Form("plane C chan %d; cnts/Q;",nstrip[ii]));

       hC_laseroff[ii]->Draw("same");
       hC_laseroff[ii]->Scale(n1/n2);
       hC_laseroff[ii]->SetLineColor(2); 
       hC_laseroff[ii]->SetLineWidth(2);
       hC_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hC_laseron[ii],"laser on","L");
       leg->AddEntry(hC_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cC4=new TCanvas("cC4","cC4",1500,1500);
    cC4->Divide(2,4);
    for(int ii=24;ii<32;ii++){
       cC4->cd(ii-23);
       Double_t n1 = hC_laseron[ii]->GetEntries();
       Double_t n2 = hC_laseroff[ii]->GetEntries();
	   Double_t mean_on = hC_laseron[ii]->GetMean();
	   Double_t mean_off = hC_laseroff[ii]->GetMean();
       hC_laseron[ii]->Draw();
       //hC_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hC_laseron[ii]->SetTitle(Form("plane C chan %d; cnts/Q;",nstrip[ii]));

       hC_laseroff[ii]->Draw("same");
       hC_laseroff[ii]->Scale(n1/n2);
       hC_laseroff[ii]->SetLineColor(2); 
       hC_laseroff[ii]->SetLineWidth(2);
       hC_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hC_laseron[ii],"laser on","L");
       leg->AddEntry(hC_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cC5=new TCanvas("cC5","cC5",1500,1500);
    cC5->Divide(2,4);
    for(int ii=32;ii<40;ii++){
       cC5->cd(ii-31);
       Double_t n1 = hC_laseron[ii]->GetEntries();
       Double_t n2 = hC_laseroff[ii]->GetEntries();
	   Double_t mean_on = hC_laseron[ii]->GetMean();
	   Double_t mean_off = hC_laseroff[ii]->GetMean();
       hC_laseron[ii]->Draw();
       //hC_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hC_laseron[ii]->SetTitle(Form("plane C chan %d; cnts/Q;",nstrip[ii]));

       hC_laseroff[ii]->Draw("same");
       hC_laseroff[ii]->Scale(n1/n2);
       hC_laseroff[ii]->SetLineColor(2); 
       hC_laseroff[ii]->SetLineWidth(2);
       hC_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hC_laseron[ii],"laser on","L");
       leg->AddEntry(hC_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cC6=new TCanvas("cC6","cC6",1500,1500);
    cC6->Divide(2,4);
    for(int ii=40;ii<43;ii++){
       cC6->cd(ii-39);
       Double_t n1 = hC_laseron[ii]->GetEntries();
       Double_t n2 = hC_laseroff[ii]->GetEntries();
	   Double_t mean_on = hC_laseron[ii]->GetMean();
	   Double_t mean_off = hC_laseroff[ii]->GetMean();
       hC_laseron[ii]->Draw();
       //hC_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hC_laseron[ii]->SetTitle(Form("plane C chan %d; cnts/Q;",nstrip[ii]));

       hC_laseroff[ii]->Draw("same");
       hC_laseroff[ii]->Scale(n1/n2);
       hC_laseroff[ii]->SetLineColor(2); 
       hC_laseroff[ii]->SetLineWidth(2);
       hC_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hC_laseron[ii],"laser on","L");
       leg->AddEntry(hC_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

   cC1->Print("C_striprate.pdf[");
   cC1->Print("C_striprate.pdf");
   cC2->Print("C_striprate.pdf");
   cC3->Print("C_striprate.pdf");
   cC4->Print("C_striprate.pdf");
   cC5->Print("C_striprate.pdf");
   cC6->Print("C_striprate.pdf");
   cC6->Print("C_striprate.pdf]");


    TCanvas *cD1=new TCanvas("cD1","cD1",1500,1500);
    cD1->Divide(2,4);
    for(int ii=0;ii<8;ii++){
       cD1->cd(ii+1);
       Double_t n1 = hD_laseron[ii]->GetEntries();
       Double_t n2 = hD_laseroff[ii]->GetEntries();
	   Double_t mean_on = hD_laseron[ii]->GetMean();
	   Double_t mean_off = hD_laseroff[ii]->GetMean();
       hD_laseron[ii]->Draw();
       //hD_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hD_laseron[ii]->SetTitle(Form("plane D chan %d; cnts/Q;",nstrip[ii]));

       hD_laseroff[ii]->Draw("same");
       hD_laseroff[ii]->Scale(n1/n2);
       hD_laseroff[ii]->SetLineColor(2); 
       hD_laseroff[ii]->SetLineWidth(2);
       hD_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hD_laseron[ii],"laser on","L");
       leg->AddEntry(hD_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cD2=new TCanvas("cD2","cD2",1500,1500);
    cD2->Divide(2,4);
    for(int ii=8;ii<16;ii++){
       cD2->cd(ii-7);
       Double_t n1 = hD_laseron[ii]->GetEntries();
       Double_t n2 = hD_laseroff[ii]->GetEntries();
	   Double_t mean_on = hD_laseron[ii]->GetMean();
	   Double_t mean_off = hD_laseroff[ii]->GetMean();
       hD_laseron[ii]->Draw();
       //hD_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hD_laseron[ii]->SetTitle(Form("plane D chan %d; cnts/Q;",nstrip[ii]));

       hD_laseroff[ii]->Draw("same");
       hD_laseroff[ii]->Scale(n1/n2);
       hD_laseroff[ii]->SetLineColor(2); 
       hD_laseroff[ii]->SetLineWidth(2);
       hD_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hD_laseron[ii],"laser on","L");
       leg->AddEntry(hD_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cD3=new TCanvas("cD3","cD3",1500,1500);
    cD3->Divide(2,4);
    for(int ii=16;ii<24;ii++){
       cD3->cd(ii-15);
       Double_t n1 = hD_laseron[ii]->GetEntries();
       Double_t n2 = hD_laseroff[ii]->GetEntries();
	   Double_t mean_on = hD_laseron[ii]->GetMean();
	   Double_t mean_off = hD_laseroff[ii]->GetMean();
       hD_laseron[ii]->Draw();
       //hD_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hD_laseron[ii]->SetTitle(Form("plane D chan %d; cnts/Q;",nstrip[ii]));

       hD_laseroff[ii]->Draw("same");
       hD_laseroff[ii]->Scale(n1/n2);
       hD_laseroff[ii]->SetLineColor(2); 
       hD_laseroff[ii]->SetLineWidth(2);
       hD_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hD_laseron[ii],"laser on","L");
       leg->AddEntry(hD_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cD4=new TCanvas("cD4","cD4",1500,1500);
    cD4->Divide(2,4);
    for(int ii=24;ii<32;ii++){
       cD4->cd(ii-23);
       Double_t n1 = hD_laseron[ii]->GetEntries();
       Double_t n2 = hD_laseroff[ii]->GetEntries();
	   Double_t mean_on = hD_laseron[ii]->GetMean();
	   Double_t mean_off = hD_laseroff[ii]->GetMean();
       hD_laseron[ii]->Draw();
       //hD_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hD_laseron[ii]->SetTitle(Form("plane D chan %d; cnts/Q;",nstrip[ii]));

       hD_laseroff[ii]->Draw("same");
       hD_laseroff[ii]->Scale(n1/n2);
       hD_laseroff[ii]->SetLineColor(2); 
       hD_laseroff[ii]->SetLineWidth(2);
       hD_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hD_laseron[ii],"laser on","L");
       leg->AddEntry(hD_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cD5=new TCanvas("cD5","cD5",1500,1500);
    cD5->Divide(2,4);
    for(int ii=32;ii<40;ii++){
       cD5->cd(ii-31);
       Double_t n1 = hD_laseron[ii]->GetEntries();
       Double_t n2 = hD_laseroff[ii]->GetEntries();
	   Double_t mean_on = hD_laseron[ii]->GetMean();
	   Double_t mean_off = hD_laseroff[ii]->GetMean();
       hD_laseron[ii]->Draw();
       //hD_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hD_laseron[ii]->SetTitle(Form("plane D chan %d; cnts/Q;",nstrip[ii]));

       hD_laseroff[ii]->Draw("same");
       hD_laseroff[ii]->Scale(n1/n2);
       hD_laseroff[ii]->SetLineColor(2); 
       hD_laseroff[ii]->SetLineWidth(2);
       hD_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hD_laseron[ii],"laser on","L");
       leg->AddEntry(hD_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

    TCanvas *cD6=new TCanvas("cD6","cD6",1500,1500);
    cD6->Divide(2,4);
    for(int ii=40;ii<43;ii++){
       cD6->cd(ii-39);
       Double_t n1 = hD_laseron[ii]->GetEntries();
       Double_t n2 = hD_laseroff[ii]->GetEntries();
	   Double_t mean_on = hD_laseron[ii]->GetMean();
	   Double_t mean_off = hD_laseroff[ii]->GetMean();
       hD_laseron[ii]->Draw();
       //hD_laseroff[ii]->GetXaxis()->SetRangeUser(0,400);
       hD_laseron[ii]->SetTitle(Form("plane D chan %d; cnts/Q;",nstrip[ii]));

       hD_laseroff[ii]->Draw("same");
       hD_laseroff[ii]->Scale(n1/n2);
       hD_laseroff[ii]->SetLineColor(2); 
       hD_laseroff[ii]->SetLineWidth(2);
       hD_laseroff[ii]->SetLineWidth(2);
    
       auto leg = new TLegend(0.6,0.6,0.75,0.75);
       leg->AddEntry(hD_laseron[ii],"laser on","L");
       leg->AddEntry(hD_laseroff[ii],"laser off","L");
       leg->SetMargin(0.5);
       leg->Draw();


       TLatex latex;
       latex.SetNDC();
       latex.SetTextSize(0.05);
       //latex.SetTextAlign(12);
       latex.DrawLatex(0.6,0.5,Form("Mean(laser on)=%.2lf",mean_on));
       latex.DrawLatex(0.6,0.45,Form("Mean (laser off)=%.2lf",mean_off));
    }   

   cD1->Print("D_striprate.pdf[");
   cD1->Print("D_striprate.pdf");
   cD2->Print("D_striprate.pdf");
   cD3->Print("D_striprate.pdf");
   cD4->Print("D_striprate.pdf");
   cD5->Print("D_striprate.pdf");
   cD6->Print("D_striprate.pdf");
   cD6->Print("D_striprate.pdf]");




*/


}
