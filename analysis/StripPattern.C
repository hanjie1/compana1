#include "../SetParams.h"

void StripPattern()
{
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTP = (TTree*) f0->Get("VTP");
     TTree *VTPScal = (TTree*) f0->Get("VTPScal");

	 T->AddFriend(VTP);

	 Int_t eplaneA_nhits=0, eplaneB_nhits=0, eplaneC_nhits=0, eplaneD_nhits=0;
     Int_t eplaneA_chan[VETROC_MAXHIT]={0};
     Int_t eplaneB_chan[VETROC_MAXHIT]={0};
     Int_t eplaneC_chan[VETROC_MAXHIT]={0};
     Int_t eplaneD_chan[VETROC_MAXHIT]={0};
	 Int_t hel_win_cnt=0;

     Int_t nentries=0;
     nentries = T->GetEntries();
	 cout<<"Total entries:  "<<nentries<<endl;
	 Int_t maxevent=0;
	 cout<<"How many events ? (0 for total) ";
	 cin>>maxevent;

	 if(maxevent==0)maxevent=nentries;
	 

	 TH1F *hA = new TH1F("hA","plane A hits pattern",50,0,50);
	 TH1F *hA_beamon = new TH1F("hA_beamon","plane A hits pattern beam on",50,0,50);
	 TH1F *hA_laseron = new TH1F("hA_laseron","plane A hits pattern laser on",50,0,50);
	 TH1F *hB = new TH1F("hB","plane B hits pattern",50,0,50);
	 TH1F *hB_beamon = new TH1F("hB_beamon","plane B hits pattern beam on",50,0,50);
	 TH1F *hB_laseron = new TH1F("hB_laseron","plane B hits pattern laser on",50,0,50);
	 TH1F *hC = new TH1F("hC","plane C hits pattern",50,0,50);
	 TH1F *hC_beamon = new TH1F("hC_beamon","plane C hits pattern beam on",50,0,50);
	 TH1F *hC_laseron = new TH1F("hC_laseron","plane C hits pattern laser on",50,0,50);
	 TH1F *hD = new TH1F("hD","plane D hits pattern",50,0,50);
	 TH1F *hD_beamon = new TH1F("hD_beamon","plane D hits pattern beam on",50,0,50);
	 TH1F *hD_laseron = new TH1F("hD_laseron","plane D hits pattern laser on",50,0,50);

     T->SetBranchAddress("eplaneA_nhits",&eplaneA_nhits);
     T->SetBranchAddress("eplaneA_chan",eplaneA_chan);
     T->SetBranchAddress("eplaneB_nhits",&eplaneB_nhits);
     T->SetBranchAddress("eplaneB_chan",eplaneB_chan);
     T->SetBranchAddress("eplaneC_nhits",&eplaneC_nhits);
     T->SetBranchAddress("eplaneC_chan",eplaneC_chan);
     T->SetBranchAddress("eplaneD_nhits",&eplaneD_nhits);
     T->SetBranchAddress("eplaneD_chan",eplaneD_chan);
     T->SetBranchAddress("hel_win_cnt",&hel_win_cnt);

	 Int_t BCM,CavPower;
	 VTPScal->SetBranchAddress("vtp_BCM",&BCM);
	 VTPScal->SetBranchAddress("vtp_CavPower",&CavPower);

     for(int nn=0;nn<maxevent;nn++){
	   T->GetEntry(nn);
	   int jj=0;
	  // cout<<eplaneA_nhits<<endl;
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
	//	 cout<<hel_win_cnt<<"  "<<BCM<<"  "<<CavPower<<endl;
		 VTPScal->GetEntry(hel_win_cnt);
		 if(BCM>3300)hA_beamon->Fill(nstrip);
		 if(BCM>3300 && CavPower>100)hA_laseron->Fill(nstrip);
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
		 VTPScal->GetEntry(hel_win_cnt);
		 if(BCM>3300)hB_beamon->Fill(nstrip);
		 if(BCM>3300 && CavPower>100)hB_laseron->Fill(nstrip);
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
		 VTPScal->GetEntry(hel_win_cnt);
		 if(BCM>3300)hC_beamon->Fill(nstrip);
		 if(BCM>3300 && CavPower>100)hC_laseron->Fill(nstrip);
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
		 VTPScal->GetEntry(hel_win_cnt);
		 if(BCM>3300)hD_beamon->Fill(nstrip);
		 if(BCM>3300 && CavPower>100)hD_laseron->Fill(nstrip);
 	   }
	 }	
	 delete T;

	 TCanvas *c1 =new TCanvas("c1","c1",1500,1500);
	 c1->Divide(2,2);
	 c1->cd(1);
	 hA->SetLineColor(1);
	 hA_beamon->SetLineColor(4);
	 hA_laseron->SetLineColor(2);
     hA->Draw();
     hA_beamon->Draw("same");
     hA_laseron->Draw("same");
	 hA->GetXaxis()->SetTitle("strip");

     auto leg1=new TLegend(0.7,0.6,0.85,0.85);
	 leg1->AddEntry(hA,"all","L");
	 leg1->AddEntry(hA_beamon,"beam on","L");
	 leg1->AddEntry(hA_laseron,"beam and laser on","L");
	 leg1->Draw();

	 c1->cd(2);
	 hB->SetLineColor(1);
	 hB_beamon->SetLineColor(4);
	 hB_laseron->SetLineColor(2);
     hB->Draw();
     hB_beamon->Draw("same");
     hB_laseron->Draw("same");
	 hB->GetXaxis()->SetTitle("strip");

     auto leg2=new TLegend(0.7,0.6,0.85,0.85);
	 leg2->AddEntry(hB,"all","L");
	 leg2->AddEntry(hB_beamon,"beam on","L");
	 leg2->AddEntry(hB_laseron,"beam and laser on","L");
	 leg2->Draw();

	 c1->cd(3);
	 hC->SetLineColor(1);
	 hC_beamon->SetLineColor(4);
	 hC_laseron->SetLineColor(2);
     hC->Draw();
     hC_beamon->Draw("same");
     hC_laseron->Draw("same");
	 hC->GetXaxis()->SetTitle("strip");

     auto leg3=new TLegend(0.7,0.6,0.85,0.85);
	 leg3->AddEntry(hC,"all","L");
	 leg3->AddEntry(hC_beamon,"beam on","L");
	 leg3->AddEntry(hC_laseron,"beam and laser on","L");
	 leg3->Draw();

	 c1->cd(4);
	 hD->SetLineColor(1);
	 hD_beamon->SetLineColor(4);
	 hD_laseron->SetLineColor(2);
     hD->Draw();
     hD_beamon->Draw("same");
     hD_laseron->Draw("same");
	 hD->GetXaxis()->SetTitle("strip");

     auto leg4=new TLegend(0.7,0.6,0.85,0.85);
	 leg4->AddEntry(hD,"all","L");
	 leg4->AddEntry(hD_beamon,"beam on","L");
	 leg4->AddEntry(hD_laseron,"beam and laser on","L");
	 leg4->Draw();

}
