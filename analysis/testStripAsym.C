void testStripAsym(){
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *VTP = (TTree*) f0->Get("VTP");
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTPScal = (TTree*) f0->Get("VTPScal");

     Int_t nentries=0;
     nentries = VTP->GetEntries();
     cout<<"Total entries:  "<<nentries<<endl;
     Int_t maxevent=0;
     cout<<"How many events ? (0 for total) ";
     cin>>maxevent;
     
     Double_t gain = 0.00033;
     Double_t offset = -1.1678;

     if(maxevent==0)maxevent=nentries;

     Int_t hel_win_cnt_1=0,hel_win_cnt=0;
     Int_t vtpA_scal[128]={0},vtpB_scal[128]={0},vtpC_scal[128]={0},vtpD_scal[128]={0};
	 Int_t scaldat[16]={0};

     VTP->SetBranchAddress("eplaneA_scalcnt",vtpA_scal);
     VTP->SetBranchAddress("scaldat",scaldat);
     VTP->SetBranchAddress("hel_win_cnt",&hel_win_cnt);
     VTP->SetBranchAddress("hel_win_cnt_1",&hel_win_cnt_1);

     Int_t cur_hel;
     VTPScal->SetBranchAddress("cur_hel",&cur_hel);

	 TH1F *hA_rate_0 = new TH1F("hA_rate_0","plane A chan 51 strip rate (hel=0)",1000,0,1000);
	 TH1F *hA_rate_1 = new TH1F("hA_rate_1","plane A chan 51 strip rate (hel=1)",1000,0,1000);
	
	 Double_t NA0=0,NA1=0;  
	 Double_t totalQ_0=0,totalQ_1=0;
     for(int ii=0;ii<maxevent;ii++){
	     VTP->GetEntry(ii);
		 if(hel_win_cnt_1<568)continue;

		 Int_t BCM = scaldat[15];
		 Int_t CavPower = scaldat[12];
		 VTPScal->GetEntry(hel_win_cnt_1);
		
		 Double_t charge = (BCM*gain/(1./120.)+offset)*(1./120.);

		 if(BCM>3350 && CavPower>100){
		    if(cur_hel==0){
			  hA_rate_0->Fill(vtpA_scal[51]);
			  NA0 += vtpA_scal[51];
			  totalQ_0 += charge;
			}
		    if(cur_hel==1){
			  hA_rate_1->Fill(vtpA_scal[51]);
			  NA1 += vtpA_scal[51];
			  totalQ_1 += charge;
			}
		 }
	 }

	hA_rate_0->Draw();
	hA_rate_1->Draw("same");
	hA_rate_1->SetLineColor(2);


    cout<<"hel 0: "<<NA0<<"  "<<totalQ_0<<"  "<<NA0/totalQ_0<<endl;
    cout<<"hel 1: "<<NA1<<"  "<<totalQ_1<<"  "<<NA1/totalQ_1<<endl;
	cout<<(NA0/totalQ_0 - NA1/totalQ_1)/(NA0/totalQ_0+NA1/totalQ_1)<<endl;
}

