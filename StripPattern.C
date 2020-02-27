#include "SetParams.h"

void StripPattern()
{
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *T = (TTree*) f0->Get("T");

	 Int_t eplaneA_nhits=0, eplaneB_nhits=0, eplaneC_nhits=0, eplaneD_nhits=0;
     Int_t eplaneA_chan[VETROC_MAXHIT]={0};
     Int_t eplaneB_chan[VETROC_MAXHIT]={0};
     Int_t eplaneC_chan[VETROC_MAXHIT]={0};
     Int_t eplaneD_chan[VETROC_MAXHIT]={0};
	 Int_t clock=0, CavPower=0, BPM2AX_p=0;

     Int_t nentries=0;
     nentries = T->GetEntries();
	 cout<<"Total entries:  "<<nentries<<endl;
	 Int_t maxevent=0;
	 cout<<"How many events ? (0 for total) ";
	 cin>>maxevent;

	 if(maxevent==0)maxevent=nentries;
	 

	 TH1F *hA = new TH1F("hA","plane A hits pattern",50,0,50);
	 TH1F *hA_on = new TH1F("hA_on","plane A hits pattern laser on",50,0,50);
	 TH1F *hB = new TH1F("hB","plane B hits pattern",50,0,50);
	 TH1F *hB_on = new TH1F("hB_on","plane B hits pattern laser on",50,0,50);
	 TH1F *hC = new TH1F("hC","plane C hits pattern",50,0,50);
	 TH1F *hC_on = new TH1F("hC_on","plane C hits pattern laser on",50,0,50);
	 TH1F *hD = new TH1F("hD","plane D hits pattern",50,0,50);
	 TH1F *hD_on = new TH1F("hD_on","plane D hits pattern laser on",50,0,50);

     T->SetBranchAddress("eplaneA_nhits",&eplaneA_nhits);
     T->SetBranchAddress("eplaneA_chan",eplaneA_chan);
     T->SetBranchAddress("eplaneB_nhits",&eplaneB_nhits);
     T->SetBranchAddress("eplaneB_chan",eplaneB_chan);
     T->SetBranchAddress("eplaneC_nhits",&eplaneC_nhits);
     T->SetBranchAddress("eplaneC_chan",eplaneC_chan);
     T->SetBranchAddress("eplaneD_nhits",&eplaneD_nhits);
     T->SetBranchAddress("eplaneD_chan",eplaneD_chan);

     for(int nn=0;nn<maxevent;nn++){
	   T->GetEntry(nn);
	   int jj=0;
	   for(jj=0;jj<eplaneA_nhits;jj++){
		 int nstrip = 0;
		 if(eplaneA_chan[jj]>=32 && eplaneA_chan[jj]<44){
			nstrip = (43-eplaneA_chan[jj])*2+26;
		 }
		 if(eplaneA_chan[jj]>=48 && eplaneA_chan[jj]<60){
			nstrip = (59-eplaneA_chan[jj])*2+2;
		 }
		 if(eplaneA_chan[jj]>=0 && eplaneA_chan[jj]<12){
			nstrip = (11-eplaneA_chan[jj])*2+25;
		 }
		 if(eplaneA_chan[jj]>=16 && eplaneA_chan[jj]<28){
			nstrip = (27-eplaneA_chan[jj])*2+1;
		 }
	     hA->Fill(nstrip);	
		 if(jj<6)hA_on->Fill(nstrip);
 	   }

	   for(jj=0;jj<eplaneB_nhits;jj++){
		 int nstrip = 0;
		 if(eplaneB_chan[jj]>=32 && eplaneB_chan[jj]<44){
			nstrip = (43-eplaneB_chan[jj])*2+26;
		 }
		 if(eplaneB_chan[jj]>=48 && eplaneB_chan[jj]<60){
			nstrip = (59-eplaneB_chan[jj])*2+2;
		 }
		 if(eplaneB_chan[jj]>=0 && eplaneB_chan[jj]<12){
			nstrip = (11-eplaneB_chan[jj])*2+25;
		 }
		 if(eplaneB_chan[jj]>=16 && eplaneB_chan[jj]<28){
			nstrip = (27-eplaneB_chan[jj])*2+1;
		 }
	     hB->Fill(nstrip);	
		 if(jj<6)hB_on->Fill(nstrip);
 	   }

	   for(jj=0;jj<eplaneC_nhits;jj++){
		 int nstrip = 0;
		 if(eplaneC_chan[jj]>=32 && eplaneC_chan[jj]<44){
			nstrip = (43-eplaneC_chan[jj])*2+26;
		 }
		 if(eplaneC_chan[jj]>=48 && eplaneC_chan[jj]<60){
			nstrip = (59-eplaneC_chan[jj])*2+2;
		 }
		 if(eplaneC_chan[jj]>=0 && eplaneC_chan[jj]<12){
			nstrip = (11-eplaneC_chan[jj])*2+25;
		 }
		 if(eplaneC_chan[jj]>=16 && eplaneC_chan[jj]<28){
			nstrip = (27-eplaneC_chan[jj])*2+1;
		 }
	     hC->Fill(nstrip);	
	 	 if(jj<6)hC_on->Fill(nstrip);
 	   }

	   for(jj=0;jj<eplaneD_nhits;jj++){
		 int nstrip = 0;
		 if(eplaneD_chan[jj]>=32 && eplaneD_chan[jj]<44){
			nstrip = (43-eplaneD_chan[jj])*2+26;
		 }
		 if(eplaneD_chan[jj]>=48 && eplaneD_chan[jj]<60){
			nstrip = (59-eplaneD_chan[jj])*2+2;
		 }
		 if(eplaneD_chan[jj]>=0 && eplaneD_chan[jj]<12){
			nstrip = (11-eplaneD_chan[jj])*2+25;
		 }
		 if(eplaneD_chan[jj]>=16 && eplaneD_chan[jj]<28){
			nstrip = (27-eplaneD_chan[jj])*2+1;
		 }
	     hD->Fill(nstrip);	
		 if(jj<6)hD_on->Fill(nstrip);
 	   }
	 }	
	 delete T;

	 TCanvas *c1 =new TCanvas("c1","c1",1500,1500);
	 c1->Divide(2,2);
	 c1->cd(1);
     hA->Draw();
     hA_on->Draw("same");
	 hA_on->SetLineColor(2);
	 hA->GetXaxis()->SetTitle("strip");
	 c1->cd(2);
     hB->Draw();
	 hB->GetXaxis()->SetTitle("strip");
     hB_on->Draw("same");
	 hB_on->SetLineColor(2);
	 c1->cd(3);
     hC->Draw();
	 hC->GetXaxis()->SetTitle("strip");
     hC_on->Draw("same");
	 hC_on->SetLineColor(2);
	 c1->cd(4);
     hD->Draw();
	 hD->GetXaxis()->SetTitle("strip");
     hD_on->Draw("same");
	 hD_on->SetLineColor(2);

}
