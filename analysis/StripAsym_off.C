void StripAsym_off(){
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *VTP = (TTree*) f0->Get("VTP");
     TTree *VTPScal = (TTree*) f0->Get("VTPScal");

     Int_t nentries=0;
     nentries = VTP->GetEntries();
     cout<<"Total entries:  "<<nentries<<endl;
     Int_t maxevent=0;
     cout<<"How many events ? (0 for total) ";
     cin>>maxevent;
     

     if(maxevent==0)maxevent=nentries;

     Int_t hel_win_cnt_1=0,hel_win_cnt=0;
     Int_t vtpA_scal[128]={0},vtpB_scal[128]={0},vtpC_scal[128]={0},vtpD_scal[128]={0};
	 Int_t scaldat[16]={0};

     VTP->SetBranchAddress("eplaneA_scalcnt",vtpA_scal);
     VTP->SetBranchAddress("eplaneB_scalcnt",vtpB_scal);
     VTP->SetBranchAddress("eplaneC_scalcnt",vtpC_scal);
     VTP->SetBranchAddress("eplaneD_scalcnt",vtpD_scal);
     VTP->SetBranchAddress("scaldat",scaldat);
     VTP->SetBranchAddress("hel_win_cnt",&hel_win_cnt);
     VTP->SetBranchAddress("hel_win_cnt_1",&hel_win_cnt_1);

     Int_t BCM,CavPower,cur_hel;
     VTPScal->SetBranchAddress("cur_hel",&cur_hel);

     Double_t NplusA_on[128], NplusA_off[128], NminusA_on[128], NminusA_off[128];
     Double_t NplusB_on[128], NplusB_off[128], NminusB_on[128], NminusB_off[128];
     Double_t NplusC_on[128], NplusC_off[128], NminusC_on[128], NminusC_off[128];
     Double_t NplusD_on[128], NplusD_off[128], NminusD_on[128], NminusD_off[128];
     Double_t NQplus_on={0}, NQminus_on={0}, NQplus_off={0}, NQminus_off={0};

	for(int jj=0;jj<128;jj++){
		  NplusA_on[jj]=0; NplusA_off[jj]=0;NminusA_on[jj]=0; NminusA_off[jj]=0;
		  NplusB_on[jj]=0; NplusB_off[jj]=0;NminusB_on[jj]=0; NminusB_off[jj]=0;
		  NplusC_on[jj]=0; NplusC_off[jj]=0;NminusC_on[jj]=0; NminusC_off[jj]=0;
		  NplusD_on[jj]=0; NplusD_off[jj]=0;NminusD_on[jj]=0; NminusD_off[jj]=0;
	}

     Int_t hel_win_cnt_0=0;
	 for(int ii=0; ii<maxevent; ii++){
		VTP->GetEntry(ii);
		if(ii==0) hel_win_cnt_0 = hel_win_cnt;

		if(hel_win_cnt_1<hel_win_cnt_0)	continue; 
		if(scaldat[1]==0) continue;   // scaler is not update

		Double_t BCM = scaldat[15];
		Double_t CavPower = scaldat[12];

		VTPScal->GetEntry(hel_win_cnt_1);
		if(BCM==0 && CavPower>100){
		  if(cur_hel==0) NQminus_on += 1;
		  if(cur_hel==1) NQplus_on += 1;
		}
		if(BCM==0 && CavPower<40){
		  if(cur_hel==0) NQminus_off += 1;
	   	  if(cur_hel==1) NQplus_off += 1;
		}

	   for(int kk=0; kk<128; kk++){
		    if(BCM==0 && CavPower>100){
			 if(cur_hel==0){
			   NminusA_on[kk] += vtpA_scal[kk];
			   NminusB_on[kk] += vtpB_scal[kk];
			   NminusC_on[kk] += vtpC_scal[kk];
			   NminusD_on[kk] += vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_on[kk] += vtpA_scal[kk];
			   NplusB_on[kk] += vtpB_scal[kk];
			   NplusC_on[kk] += vtpC_scal[kk];
			   NplusD_on[kk] += vtpD_scal[kk];
			 }
		    }
		    if(BCM==0 && CavPower<40){
			 if(cur_hel==0){
			   NminusA_off[kk] += vtpA_scal[kk];
			   NminusB_off[kk] += vtpB_scal[kk];
			   NminusC_off[kk] += vtpC_scal[kk];
			   NminusD_off[kk] += vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_off[kk] += vtpA_scal[kk];
			   NplusB_off[kk] += vtpB_scal[kk];
			   NplusC_off[kk] += vtpC_scal[kk];
			   NplusD_off[kk] += vtpD_scal[kk];
			 }
		    }
		  }
	 }

	 cout<<NQplus_on<<"  "<<NQminus_on<<endl;

	 TGraph *gA_on = new TGraph();
	 TGraph *gA_off = new TGraph();
	 TGraph *gB_on = new TGraph();
	 TGraph *gB_off = new TGraph();
	 TGraph *gC_on = new TGraph();
	 TGraph *gC_off = new TGraph();
	 TGraph *gD_on = new TGraph();
	 TGraph *gD_off = new TGraph();

	for(int jj=0; jj<128; jj++){
	  Double_t asym_on=0, asym_off=0;
	  if((NplusA_on[jj]+NminusA_on[jj])!=0) asym_on = (NplusA_on[jj]/NQplus_on-NminusA_on[jj]/NQminus_on)/(NplusA_on[jj]/NQplus_on+NminusA_on[jj]/NQminus_on);
	  if((NplusA_off[jj]+NminusA_off[jj])!=0) asym_off = (NplusA_off[jj]/NQplus_off-NminusA_off[jj]/NQminus_off)/(NplusA_off[jj]/NQplus_off+NminusA_off[jj]/NQminus_off);

	  gA_on->SetPoint(jj,jj,asym_on);
	  gA_off->SetPoint(jj,jj,asym_off);
	}

	for(int jj=0; jj<128; jj++){
	  Double_t asym_on=0, asym_off=0;
	  if((NplusB_on[jj]+NminusB_on[jj])!=0) asym_on = (NplusB_on[jj]/NQplus_on-NminusB_on[jj]/NQminus_on)/(NplusB_on[jj]/NQplus_on+NminusB_on[jj]/NQminus_on);
	  if((NplusB_off[jj]+NminusB_off[jj])!=0) asym_off = (NplusB_off[jj]/NQplus_off-NminusB_off[jj]/NQminus_off)/(NplusB_off[jj]/NQplus_off+NminusB_off[jj]/NQminus_off);

	  gB_on->SetPoint(jj,jj,asym_on);
	  gB_off->SetPoint(jj,jj,asym_off);
	}

	for(int jj=0; jj<128; jj++){
	  Double_t asym_on=0, asym_off=0;
	  if((NplusC_on[jj]+NminusC_on[jj])!=0) asym_on = (NplusC_on[jj]/NQplus_on-NminusC_on[jj]/NQminus_on)/(NplusC_on[jj]/NQplus_on+NminusC_on[jj]/NQminus_on);
	  if((NplusC_off[jj]+NminusC_off[jj])!=0) asym_off = (NplusC_off[jj]/NQplus_off-NminusC_off[jj]/NQminus_off)/(NplusC_off[jj]/NQplus_off+NminusC_off[jj]/NQminus_off);

	  gC_on->SetPoint(jj,jj,asym_on);
	  gC_off->SetPoint(jj,jj,asym_off);
	}

	for(int jj=0; jj<128; jj++){
	  Double_t asym_on=0, asym_off=0;
	  if((NplusD_on[jj]+NminusD_on[jj])!=0) asym_on = (NplusD_on[jj]/NQplus_on-NminusD_on[jj]/NQminus_on)/(NplusD_on[jj]/NQplus_on+NminusD_on[jj]/NQminus_on);
	  if((NplusD_off[jj]+NminusD_off[jj])!=0) asym_off = (NplusD_off[jj]/NQplus_off-NminusD_off[jj]/NQminus_off)/(NplusD_off[jj]/NQplus_off+NminusD_off[jj]/NQminus_off);

	  gD_on->SetPoint(jj,jj,asym_on);
	  gD_off->SetPoint(jj,jj,asym_off);
	}

	 TCanvas *c1=new TCanvas("c1","c1",1200,1200);
	 c1->Divide(2,2);
	 c1->cd(1);
	 TMultiGraph *mg1 = new TMultiGraph();
   	 gA_on->SetMarkerColor(2);	 
   	 gA_on->SetMarkerStyle(8);	 
   	 gA_on->SetMarkerSize(1.3);	 
   	 gA_off->SetMarkerColor(4);	 
   	 gA_off->SetMarkerStyle(8);	 
   	 gA_off->SetMarkerSize(1.3);	 

	 mg1->Add(gA_on);
	 mg1->Add(gA_off);
	 mg1->Draw("AP");
	 mg1->SetTitle("plane A asymmetry;chan;asym");

	 auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	 leg1->AddEntry(gA_on,"laser on","P");
	 leg1->AddEntry(gA_off,"laser off","P");
	 leg1->Draw(); 

	 c1->cd(2);
	 TMultiGraph *mg2 = new TMultiGraph();
   	 gB_on->SetMarkerColor(2);	 
   	 gB_on->SetMarkerStyle(8);	 
   	 gB_on->SetMarkerSize(1.3);	 
   	 gB_off->SetMarkerColor(4);	 
   	 gB_off->SetMarkerStyle(8);	 
   	 gB_off->SetMarkerSize(1.3);	 

	 mg2->Add(gB_on);
	 mg2->Add(gB_off);
	 mg2->Draw("AP");
	 mg2->SetTitle("plane B asymmetry;chan;asym");

	 auto leg2 = new TLegend(0.7,0.7,0.85,0.85);
	 leg2->AddEntry(gB_on,"laser on","P");
	 leg2->AddEntry(gB_off,"laser off","P");
	 leg2->Draw(); 

	 c1->cd(3);
	 TMultiGraph *mg3 = new TMultiGraph();
   	 gC_on->SetMarkerColor(2);	 
   	 gC_on->SetMarkerStyle(8);	 
   	 gC_on->SetMarkerSize(1.3);	 
   	 gC_off->SetMarkerColor(4);	 
   	 gC_off->SetMarkerStyle(8);	 
   	 gC_off->SetMarkerSize(1.3);	 

	 mg3->Add(gC_on);
	 mg3->Add(gC_off);
	 mg3->Draw("AP");
	 mg3->SetTitle("plane C asymmetry;chan;asym");

	 auto leg3 = new TLegend(0.7,0.7,0.85,0.85);
	 leg3->AddEntry(gC_on,"laser on","P");
	 leg3->AddEntry(gC_off,"laser off","P");
	 leg3->Draw(); 

	 c1->cd(4);
	 TMultiGraph *mg4 = new TMultiGraph();
   	 gD_on->SetMarkerColor(2);	 
   	 gD_on->SetMarkerStyle(8);	 
   	 gD_on->SetMarkerSize(1.3);	 
   	 gD_off->SetMarkerColor(4);	 
   	 gD_off->SetMarkerStyle(8);	 
   	 gD_off->SetMarkerSize(1.3);	 

	 mg4->Add(gD_on);
	 mg4->Add(gD_off);
	 mg4->Draw("AP");
	 mg4->SetTitle("plane D asymmetry;chan;asym");

	 auto leg4 = new TLegend(0.7,0.7,0.85,0.85);
	 leg4->AddEntry(gD_on,"laser on","P");
	 leg4->AddEntry(gD_off,"laser off","P");
	 leg4->Draw(); 

//     c1->Print(Form("Asymmetry_%d.pdf",runnumber));

}

