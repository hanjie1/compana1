void StripAsym_clock(){
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

     VTP->SetBranchAddress("scaldat",scaldat);
     VTP->SetBranchAddress("hel_win_cnt",&hel_win_cnt);
     VTP->SetBranchAddress("hel_win_cnt_1",&hel_win_cnt_1);

     Int_t BCM,CavPower,cur_hel;
     VTPScal->SetBranchAddress("cur_hel",&cur_hel);

     Double_t Nplus_on[8]={0}, Nplus_off[8]={0}, Nminus_on[8]={0}, Nminus_off[8]={0};
	 Double_t Nplus_beamoff[8]={0}, Nminus_beamoff[8]={0};

	 Double_t NQplus_on[8]={0},NQminus_on[8]={0};
	 Double_t NQplus_off[8]={0},NQminus_off[8]={0};
	 Double_t NQplus_beamoff[8]={0},NQminus_beamoff[8]={0};

     Int_t hel_win_cnt_0=0;;
	 for(int ii=0; ii<maxevent; ii++){
		VTP->GetEntry(ii);
	    if(ii==0) hel_win_cnt_0 = hel_win_cnt;
		if( hel_win_cnt_1<(hel_win_cnt_0+8) )	continue;  // so it's able to check different delay helicity
		if(scaldat[1]==0) continue;   // scaler is not update

		Double_t BCM = scaldat[15];
		Double_t CavPower = scaldat[12];

		for(int jj=0; jj<8; jj++){
		  VTPScal->GetEntry(hel_win_cnt_1-jj);

		  if(BCM>3500 && CavPower>100){
			 if(cur_hel==0){
			   Nminus_on[jj] += scaldat[1];
			   NQminus_on[jj] += 1;
			 }
			 if(cur_hel==1){
			   Nplus_on[jj] += scaldat[1];
			   NQplus_on[jj] += 1;
			 }
		  }
		  if(BCM>3500 && CavPower<40){
			 if(cur_hel==0){
			   Nminus_off[jj] += scaldat[1];
			   NQminus_off[jj] += 1;
			 }
			 if(cur_hel==1){
			   Nplus_off[jj] += scaldat[1];
			   NQplus_off[jj] += 1;
			 }
		  }
		  if(BCM==0){
			 if(cur_hel==0){
			   Nminus_beamoff[jj] += scaldat[1];
			   NQminus_beamoff[jj] += 1;
			 }
			 if(cur_hel==1){
			   Nplus_beamoff[jj] += scaldat[1];
			   NQplus_beamoff[jj] += 1;
			 }
		  }
		}
	 }

	 TGraph *gon = new TGraph();
	 TGraph *goff = new TGraph();
	 TGraph *gbeamoff = new TGraph();


	 for(int ii=0;ii<8;ii++){
	    Double_t tmpPlus_on = Nplus_on[ii]/NQplus_on[ii];
	    Double_t tmpMinus_on = Nminus_on[ii]/NQminus_on[ii];

	    Double_t tmpPlus_off = Nplus_off[ii]/NQplus_off[ii];
	    Double_t tmpMinus_off = Nminus_off[ii]/NQminus_off[ii];

	    Double_t tmpPlus_beamoff = Nplus_beamoff[ii]/NQplus_beamoff[ii];
	    Double_t tmpMinus_beamoff = Nminus_beamoff[ii]/NQminus_beamoff[ii];

		Double_t asym_on=0, asym_off=0,asym_beamoff=0;
		if((Nplus_on[ii]+Nminus_on[ii])!=0) {
		   asym_on = (tmpPlus_on - tmpMinus_on)/(tmpPlus_on + tmpMinus_on);
		}
		if((Nplus_off[ii]+Nminus_off[ii])!=0) {
		   asym_off = (tmpPlus_off - tmpMinus_off)/(tmpPlus_off + tmpMinus_off);
		}
		if((Nplus_beamoff[ii]+Nminus_beamoff[ii])!=0) {
		   asym_beamoff = (tmpPlus_beamoff - tmpMinus_beamoff)/(tmpPlus_beamoff + tmpMinus_beamoff);
		}

	   gon->SetPoint(ii,8-ii,asym_on);
	   goff->SetPoint(ii,8-ii,asym_off);
	   gbeamoff->SetPoint(ii,8-ii,asym_beamoff);

	   cout<<ii<<"   "<<asym_on<<"   "<<asym_off<<"   "<<asym_beamoff<<endl;
	 }

	 TCanvas *c1=new TCanvas("c1","c1",1200,1200);
   	 gon->SetMarkerColor(2);	 
   	 gon->SetMarkerStyle(8);	 
   	 gon->SetMarkerSize(1.3);	 
   	 goff->SetMarkerColor(4);	 
   	 goff->SetMarkerStyle(8);	 
   	 goff->SetMarkerSize(1.3);	 
   	 gbeamoff->SetMarkerColor(8);	 
   	 gbeamoff->SetMarkerStyle(8);	 
   	 gbeamoff->SetMarkerSize(1.3);	 
 	
	 TMultiGraph *mg1 = new TMultiGraph();
	 mg1->Add(gon);
	 mg1->Add(goff);
	 mg1->Add(gbeamoff);
	 mg1->Draw("AP");
	 mg1->SetTitle("clock asymmetry;delay win;asym");
	 //mg1->GetYaxis()->SetRangeUser(-0.04,0.04);

	 mg1->Draw("AP");

	 auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	 leg1->AddEntry(gon,"laser on","P");
	 leg1->AddEntry(goff,"laser off","P");
	 leg1->AddEntry(gbeamoff,"beam off","P");
	 leg1->Draw(); 

}

