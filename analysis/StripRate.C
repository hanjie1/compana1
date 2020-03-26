#include "../SetParams.h"

void StripRate()
{
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *VTP = (TTree*) f0->Get("VTP");
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTPScal = (TTree*) f0->Get("VTPScal");

	 VTP->AddFriend("T");

	 TFile *outroot = new TFile("striprate.root","RECREATE");

	 Int_t hel_win_cnt=0;
     Int_t vtpA_scal[128]={0},vtpB_scal[128]={0},vtpC_scal[128]={0},vtpD_scal[128]={0};
	 Int_t current_hel=0;

     Int_t nentries=0;
     nentries = VTP->GetEntries();
	 cout<<"Total entries:  "<<nentries<<endl;
	 Int_t maxevent=0;
	 cout<<"How many events ? (0 for total) ";
	 cin>>maxevent;

	 if(maxevent==0)maxevent=nentries;

	 TH2F *hA = new TH2F("hA","plane A strip rate",128,0,128,500,0,500);
	 TH2F *hA_off = new TH2F("hA_off","plane A strip rate beam off",128,0,128,500,0,500);
	 TH2F *hA_laseroff = new TH2F("hA_laseroff","plane A strip rate laser off",128,0,128,500,0,500);
	 TH2F *hA_laseron = new TH2F("hA_laseron","plane A strip rate laser on",128,0,128,500,0,500);

	 TH2F *hB = new TH2F("hB","plane B strip rate",128,0,128,500,0,500);
	 TH2F *hB_off = new TH2F("hB_off","plane B strip rate beam off",128,0,128,500,0,500);
	 TH2F *hB_laseroff = new TH2F("hB_laseroff","plane B strip rate laser off",128,0,128,500,0,500);
	 TH2F *hB_laseron = new TH2F("hB_laseron","plane B strip rate laser on",128,0,128,500,0,500);

	 TH2F *hC = new TH2F("hC","plane C strip rate",128,0,128,500,0,500);
	 TH2F *hC_off = new TH2F("hC_off","plane C strip rate beam off",128,0,128,500,0,500);
	 TH2F *hC_laseroff = new TH2F("hC_laseroff","plane C strip rate laser off",128,0,128,500,0,500);
	 TH2F *hC_laseron = new TH2F("hC_laseron","plane C strip rate laser on",128,0,128,500,0,500);

	 TH2F *hD = new TH2F("hD","plane D strip rate",128,0,128,500,0,500);
	 TH2F *hD_off = new TH2F("hD_off","plane D strip rate beam off",128,0,128,500,0,500);
	 TH2F *hD_laseroff = new TH2F("hD_laseroff","plane D strip rate laser off",128,0,128,500,0,500);
	 TH2F *hD_laseron = new TH2F("hD_laseron","plane D strip rate laser on",128,0,128,500,0,500);

     VTP->SetBranchAddress("eplaneA_scalcnt",vtpA_scal);
     VTP->SetBranchAddress("eplaneB_scalcnt",vtpB_scal);
     VTP->SetBranchAddress("eplaneC_scalcnt",vtpC_scal);
     VTP->SetBranchAddress("eplaneD_scalcnt",vtpD_scal);
     VTP->SetBranchAddress("hel_win_cnt",&hel_win_cnt);
     VTP->SetBranchAddress("current_helicity",&current_hel);

	 Int_t BCM,CavPower;
	 VTPScal->SetBranchAddress("vtp_BCM",&BCM);
	 VTPScal->SetBranchAddress("vtp_CavPower",&CavPower);

	 ULong64_t totalQ_off = 0;
	 ULong64_t totalQ_on = 0;
	 Double_t Asym_A_on[128]={0.0},Asym_B_on[128]={0.0},Asym_C_on[128]={0.0},Asym_D_on[128]={0.0};
	 Double_t Asym_A_off[128]={0.0},Asym_B_off[128]={0.0},Asym_C_off[128]={0.0},Asym_D_off[128]={0.0};

     TTree *TStrip = new TTree("TStrip","save some run infomation");
	 TStrip->Branch("totalQ_on",&totalQ_on,"totalQ_on/l");
	 TStrip->Branch("totalQ_off",&totalQ_off,"totalQ_off/l");
	 TStrip->Branch("asym_A_on",Asym_A_on,"Asym_A_on[128]/D");
	 TStrip->Branch("asym_B_on",Asym_B_on,"Asym_B_on[128]/D");
	 TStrip->Branch("asym_C_on",Asym_C_on,"Asym_C_on[128]/D");
	 TStrip->Branch("asym_D_on",Asym_D_on,"Asym_D_on[128]/D");
	 TStrip->Branch("asym_A_off",Asym_A_off,"Asym_A_off[128]/D");
	 TStrip->Branch("asym_B_off",Asym_B_off,"Asym_B_off[128]/D");
	 TStrip->Branch("asym_C_off",Asym_C_off,"Asym_C_off[128]/D");
	 TStrip->Branch("asym_D_off",Asym_D_off,"Asym_D_off[128]/D");

	 Int_t NplusA_on[128]={0}, NplusA_off[128]={0}, NminusA_on[128]={0}, NminusA_off[128]={0};
	 Int_t NplusB_on[128]={0}, NplusB_off[128]={0}, NminusB_on[128]={0}, NminusB_off[128]={0};
	 Int_t NplusC_on[128]={0}, NplusC_off[128]={0}, NminusC_on[128]={0}, NminusC_off[128]={0};
	 Int_t NplusD_on[128]={0}, NplusD_off[128]={0}, NminusD_on[128]={0}, NminusD_off[128]={0};
	 int maxhit=0;
     for(int nn=0;nn<maxevent;nn++){
	   VTP->GetEntry(nn);
       VTPScal->GetEntry(hel_win_cnt);

	   for(int jj=0;jj<128;jj++){
		if(vtpA_scal[jj]>0){
	     hA->Fill(jj,vtpA_scal[jj]);	
		 if(vtpA_scal[jj]>maxhit) maxhit = vtpA_scal[jj];
		}
		if(vtpB_scal[jj]>0){
	     hB->Fill(jj,vtpB_scal[jj]);	
		 if(vtpB_scal[jj]>maxhit) maxhit = vtpB_scal[jj];
		}
		if(vtpC_scal[jj]>0){
	   	 hC->Fill(jj,vtpC_scal[jj]);	
		 if(vtpC_scal[jj]>maxhit) maxhit = vtpC_scal[jj];
		}
		if(vtpD_scal[jj]>0){
	     hD->Fill(jj,vtpD_scal[jj]);	
		 if(vtpD_scal[jj]>maxhit) maxhit = vtpD_scal[jj];
		}
		if(BCM>3350 && CavPower<=115){
		   if(vtpA_scal[jj]>0){ 
			 hA_laseroff->Fill(jj,vtpA_scal[jj]);
			 if(current_hel==0) NminusA_off[jj] += vtpA_scal[jj];
			 if(current_hel==1) NplusA_off[jj] += vtpA_scal[jj];
		   }
		   if(vtpB_scal[jj]>0){ 
			 hB_laseroff->Fill(jj,vtpB_scal[jj]);
			 if(current_hel==0) NminusB_off[jj] += vtpB_scal[jj];
			 if(current_hel==1) NplusB_off[jj] += vtpB_scal[jj];
		   }
		   if(vtpC_scal[jj]>0){ 
			 hC_laseroff->Fill(jj,vtpC_scal[jj]);
			 if(current_hel==0) NminusC_off[jj] += vtpC_scal[jj];
			 if(current_hel==1) NplusC_off[jj] += vtpC_scal[jj];
		   }
		   if(vtpD_scal[jj]>0){ 
			 hD_laseroff->Fill(jj,vtpD_scal[jj]);
			 if(current_hel==0) NminusD_off[jj] += vtpD_scal[jj];
			 if(current_hel==1) NplusD_off[jj] += vtpD_scal[jj];
		   }
		 }
		if(BCM>3350 && CavPower>115){
		   if(vtpA_scal[jj]>0){ 
			 hA_laseron->Fill(jj,vtpA_scal[jj]);
			 if(current_hel==0) NminusA_on[jj] += vtpA_scal[jj];
			 if(current_hel==1) NplusA_on[jj] += vtpA_scal[jj];
		   }
		   if(vtpB_scal[jj]>0){ 
			 hB_laseron->Fill(jj,vtpB_scal[jj]);
			 if(current_hel==0) NminusB_on[jj] += vtpB_scal[jj];
			 if(current_hel==1) NplusB_on[jj] += vtpB_scal[jj];
		   }
		   if(vtpC_scal[jj]>0){ 
			 hC_laseron->Fill(jj,vtpC_scal[jj]);
			 if(current_hel==0) NminusC_on[jj] += vtpC_scal[jj];
			 if(current_hel==1) NplusC_on[jj] += vtpC_scal[jj];
		   }
		   if(vtpD_scal[jj]>0){ 
			 hD_laseron->Fill(jj,vtpD_scal[jj]);
			 if(current_hel==0) NminusD_on[jj] += vtpD_scal[jj];
			 if(current_hel==1) NplusD_on[jj] += vtpD_scal[jj];
		   }
		 }
		if(BCM==0 && CavPower==0){
		   if(vtpA_scal[jj]>0) hA_off->Fill(jj,vtpA_scal[jj]);
		   if(vtpB_scal[jj]>0) hB_off->Fill(jj,vtpB_scal[jj]);
		   if(vtpC_scal[jj]>0) hC_off->Fill(jj,vtpC_scal[jj]);
		   if(vtpD_scal[jj]>0) hD_off->Fill(jj,vtpD_scal[jj]);
		 }
 	   }

	   if(BCM>3350 && CavPower<=115) totalQ_off += BCM;
	   if(BCM>3350 && CavPower>115) totalQ_on += BCM;
	   //cout<<totalQ_on<<" "<<totalQ_off<<endl;
	 }	
	 delete VTP;

	 for(int ii=0;ii<128;ii++){
		if((NplusA_on[ii]+NminusA_on[ii])!=0) Asym_A_on[ii] = 1.0*(NplusA_on[ii]-NminusA_on[ii])/(NplusA_on[ii]*1.0+NminusA_on[ii]*1.0);	
		if((NplusB_on[ii]+NminusB_on[ii])!=0) Asym_B_on[ii] = 1.0*(NplusB_on[ii]-NminusB_on[ii])/(NplusB_on[ii]*1.0+NminusB_on[ii]*1.0);	
		if((NplusC_on[ii]+NminusC_on[ii])!=0) Asym_C_on[ii] = 1.0*(NplusC_on[ii]-NminusC_on[ii])/(NplusC_on[ii]*1.0+NminusC_on[ii]*1.0);	
		if((NplusD_on[ii]+NminusD_on[ii])!=0) Asym_D_on[ii] = 1.0*(NplusD_on[ii]-NminusD_on[ii])/(NplusD_on[ii]*1.0+NminusD_on[ii]*1.0);	

		if((NplusA_off[ii]+NminusA_off[ii])!=0) Asym_A_off[ii] = 1.0*(NplusA_off[ii]-NminusA_off[ii])/(NplusA_off[ii]*1.0+NminusA_off[ii]*1.0);	
		if((NplusB_off[ii]+NminusB_off[ii])!=0) Asym_B_off[ii] = 1.0*(NplusB_off[ii]-NminusB_off[ii])/(NplusB_off[ii]*1.0+NminusB_off[ii]*1.0);	
		if((NplusC_off[ii]+NminusC_off[ii])!=0) Asym_C_off[ii] = 1.0*(NplusC_off[ii]-NminusC_off[ii])/(NplusC_off[ii]*1.0+NminusC_off[ii]*1.0);	
		if((NplusD_off[ii]+NminusD_off[ii])!=0) Asym_D_off[ii] = 1.0*(NplusD_off[ii]-NminusD_off[ii])/(NplusD_off[ii]*1.0+NminusD_off[ii]*1.0);	
	 }


	 TStrip->Fill();
     cout<<"maxhit:   "<<maxhit<<endl;
	 hA->Draw();
	 hA_off->Draw();
	 hA_laseroff->Draw();
	 hA_laseron->Draw();

	 hB->Draw();
	 hB_off->Draw();
	 hB_laseroff->Draw();
	 hB_laseron->Draw();

	 hC->Draw();
	 hC_off->Draw();
	 hC_laseroff->Draw();
	 hC_laseron->Draw();
	 
	 hD->Draw();
	 hD_off->Draw();
	 hD_laseroff->Draw();
	 hD_laseron->Draw();

	 outroot->Write();
}
