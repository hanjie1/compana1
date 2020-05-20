void StripAsym_morecuts(){
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
     VTP->SetBranchAddress("eplaneB_scalcnt",vtpB_scal);
     VTP->SetBranchAddress("eplaneC_scalcnt",vtpC_scal);
     VTP->SetBranchAddress("eplaneD_scalcnt",vtpD_scal);
     VTP->SetBranchAddress("scaldat",scaldat);
     VTP->SetBranchAddress("hel_win_cnt",&hel_win_cnt);
     VTP->SetBranchAddress("hel_win_cnt_1",&hel_win_cnt_1);

     Int_t vtp_BCM,vtp_CavPower,cur_hel;
     VTPScal->SetBranchAddress("cur_hel",&cur_hel);
     VTPScal->SetBranchAddress("vtp_BCM",&vtp_BCM);
     VTPScal->SetBranchAddress("vtp_CavPower",&vtp_CavPower);

     Double_t NplusA_on[4][128], NplusA_off[4][128], NminusA_on[4][128], NminusA_off[4][128];
     Double_t NplusB_on[4][128], NplusB_off[4][128], NminusB_on[4][128], NminusB_off[4][128];
     Double_t NplusC_on[4][128], NplusC_off[4][128], NminusC_on[4][128], NminusC_off[4][128];
     Double_t NplusD_on[4][128], NplusD_off[4][128], NminusD_on[4][128], NminusD_off[4][128];
     Double_t NQplus_on[4]={0}, NQminus_on[4]={0}, NQplus_off[4]={0}, NQminus_off[4]={0};

     Double_t NplusA_beamoff_on[4][128], NplusA_beamoff_off[4][128], NminusA_beamoff_on[4][128], NminusA_beamoff_off[4][128];
     Double_t NplusB_beamoff_on[4][128], NplusB_beamoff_off[4][128], NminusB_beamoff_on[4][128], NminusB_beamoff_off[4][128];
     Double_t NplusC_beamoff_on[4][128], NplusC_beamoff_off[4][128], NminusC_beamoff_on[4][128], NminusC_beamoff_off[4][128];
     Double_t NplusD_beamoff_on[4][128], NplusD_beamoff_off[4][128], NminusD_beamoff_on[4][128], NminusD_beamoff_off[4][128];
     Double_t NQplus_beamoff_on[4]={0}, NQminus_beamoff_on[4]={0}, NQplus_beamoff_off[4]={0}, NQminus_beamoff_off[4]={0};

	 for(int ii=0;ii<4;ii++){
	  for(int jj=0;jj<128;jj++){
	    NplusA_on[ii][jj]=0; NplusA_off[ii][jj]=0;NminusA_on[ii][jj]=0; NminusA_off[ii][jj]=0;
	    NplusB_on[ii][jj]=0; NplusB_off[ii][jj]=0;NminusB_on[ii][jj]=0; NminusB_off[ii][jj]=0;
	    NplusC_on[ii][jj]=0; NplusC_off[ii][jj]=0;NminusC_on[ii][jj]=0; NminusC_off[ii][jj]=0;
	    NplusD_on[ii][jj]=0; NplusD_off[ii][jj]=0;NminusD_on[ii][jj]=0; NminusD_off[ii][jj]=0;

	    NplusA_beamoff_on[ii][jj]=0; NplusA_beamoff_off[ii][jj]=0;NminusA_beamoff_on[ii][jj]=0; NminusA_beamoff_off[ii][jj]=0;
	    NplusB_beamoff_on[ii][jj]=0; NplusB_beamoff_off[ii][jj]=0;NminusB_beamoff_on[ii][jj]=0; NminusB_beamoff_off[ii][jj]=0;
	    NplusC_beamoff_on[ii][jj]=0; NplusC_beamoff_off[ii][jj]=0;NminusC_beamoff_on[ii][jj]=0; NminusC_beamoff_off[ii][jj]=0;
	    NplusD_beamoff_on[ii][jj]=0; NplusD_beamoff_off[ii][jj]=0;NminusD_beamoff_on[ii][jj]=0; NminusD_beamoff_off[ii][jj]=0;
	  }
	 }

	 Int_t nevents[4]={0};
     nevents[0] = maxevent/4;
	 nevents[1] = maxevent/4;
	 nevents[2] = maxevent/4;
	 nevents[3] = maxevent - 3 * maxevent/4;
cout<<nevents[0]<<"  "<<nevents[1]<<"  "<<nevents[2]<<"  "<<nevents[3]<<endl;
	 int nn=0;
	 Int_t hel_win_cnt_0=0;
	 Int_t past_BCM=0;
	 Int_t future_BCM=0;
	 Int_t past_Cav=0;
	 Int_t future_Cav=0;
	 for(int ii=0; ii<maxevent; ii++){
		VTP->GetEntry(ii);
		if(ii==0) hel_win_cnt_0 = hel_win_cnt;
		if(scaldat[1]==0) continue;   // scaler is not update
		if(hel_win_cnt_1<(hel_win_cnt_0+1)) continue;   

		Double_t BCM = scaldat[15];
		Double_t CavPower = scaldat[12];
	    Double_t charge = (BCM*gain/(1./120.)+offset)*(1./120.);    // charge for one helicity window

	    if( ii<=nevents[0]) nn=0;
	    if( ii>nevents[0] && ii<=(nevents[0]+nevents[1])) nn=1;
	    if( ii>(nevents[0]+nevents[1]) && ii<=(nevents[0]+nevents[1]+nevents[2])) nn=2;
	    if( ii>(nevents[0]+nevents[1]+nevents[2])) nn=3;

	    VTPScal->GetEntry(hel_win_cnt_1-1);
		past_BCM = vtp_BCM;
		past_Cav = vtp_CavPower;
	    VTPScal->GetEntry(hel_win_cnt_1+1);
		future_BCM = vtp_BCM;
		future_Cav = vtp_CavPower;
		
	    VTPScal->GetEntry(hel_win_cnt_1);
		//cout<<ii<<"  "<<hel_win_cnt_1<<"  "<<hel_win_cnt_0<<"  "<<past_BCM<<"  "<<vtp_BCM<<"  "<<future_BCM<<"  "<<cur_hel<<endl;
		//cout<<ii<<"  "<<hel_win_cnt_1<<"  "<<hel_win_cnt_0<<"  "<<past_Cav<<"  "<<CavPower<<"  "<<future_Cav<<"  "<<cur_hel<<endl;

		if(BCM>3500 && past_BCM>3500 && future_BCM>3500){
		  if( CavPower>100 && past_Cav>100 && future_Cav>100){   // beam on laser on
			if(cur_hel==0) NQminus_on[nn] += charge;
			if(cur_hel==1) NQplus_on[nn] += charge;
		  }
		  if(CavPower<40 && past_Cav<40 && future_Cav<40){    // beam on laser off
			if(cur_hel==0) NQminus_off[nn] += charge;
			if(cur_hel==1) NQplus_off[nn] += charge;
		  }
		}
		if(BCM==0 && past_BCM==0 && future_BCM==0){ 
		   if(CavPower<40 && past_Cav<40 && future_Cav<40){     // beam off laser off
			 if(cur_hel==0) NQminus_beamoff_off[nn] += 1;
			 if(cur_hel==1) NQplus_beamoff_off[nn] += 1;
		   }
		   if(CavPower>100 && past_Cav>100 && future_Cav>100){    //  beam off laser on
			if(cur_hel==0) NQminus_beamoff_on[nn] += 1;
			if(cur_hel==1) NQplus_beamoff_on[nn] += 1;
		   }
		}

	    for(int kk=0; kk<128; kk++){
		  if(BCM>3500 && past_BCM>3500 && future_BCM>3500){
			if( CavPower>100 && past_Cav>100 && future_Cav>100){
			 if(cur_hel==0){
			   NminusA_on[nn][kk]+= vtpA_scal[kk];
			   NminusB_on[nn][kk]+= vtpB_scal[kk];
			   NminusC_on[nn][kk]+= vtpC_scal[kk];
			   NminusD_on[nn][kk]+= vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_on[nn][kk]+= vtpA_scal[kk];
			   NplusB_on[nn][kk]+= vtpB_scal[kk];
			   NplusC_on[nn][kk]+= vtpC_scal[kk];
			   NplusD_on[nn][kk]+= vtpD_scal[kk];
			 }
		   }
		   if(CavPower<40 && past_Cav<40 && future_Cav<40){
			 if(cur_hel==0){
			   NminusA_off[nn][kk]+= vtpA_scal[kk];
			   NminusB_off[nn][kk]+= vtpB_scal[kk];
			   NminusC_off[nn][kk]+= vtpC_scal[kk];
			   NminusD_off[nn][kk]+= vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_off[nn][kk]+= vtpA_scal[kk];
			   NplusB_off[nn][kk]+= vtpB_scal[kk];
			   NplusC_off[nn][kk]+= vtpC_scal[kk];
			   NplusD_off[nn][kk]+= vtpD_scal[kk];
			 }
		    }
		  }
		  if(BCM==0 && past_BCM==0 && future_BCM==0){ 
			if( CavPower>100 && past_Cav>100 && future_Cav>100){
			 if(cur_hel==0){
			   NminusA_beamoff_on[nn][kk]+= vtpA_scal[kk];
			   NminusB_beamoff_on[nn][kk]+= vtpB_scal[kk];
			   NminusC_beamoff_on[nn][kk]+= vtpC_scal[kk];
			   NminusD_beamoff_on[nn][kk]+= vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_beamoff_on[nn][kk]+= vtpA_scal[kk];
			   NplusB_beamoff_on[nn][kk]+= vtpB_scal[kk];
			   NplusC_beamoff_on[nn][kk]+= vtpC_scal[kk];
			   NplusD_beamoff_on[nn][kk]+= vtpD_scal[kk];
			 }
		    }
		    if(CavPower<40 && past_Cav<40 && future_Cav<40){
			 if(cur_hel==0){
			   NminusA_beamoff_off[nn][kk]+= vtpA_scal[kk];
			   NminusB_beamoff_off[nn][kk]+= vtpB_scal[kk];
			   NminusC_beamoff_off[nn][kk]+= vtpC_scal[kk];
			   NminusD_beamoff_off[nn][kk]+= vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_beamoff_off[nn][kk]+= vtpA_scal[kk];
			   NplusB_beamoff_off[nn][kk]+= vtpB_scal[kk];
			   NplusC_beamoff_off[nn][kk]+= vtpC_scal[kk];
			   NplusD_beamoff_off[nn][kk]+= vtpD_scal[kk];
			 }
		    }
		   }
	   }
	 }


	 TGraphErrors *gA_on[4];
	 TGraphErrors *gA_off[4];
	 TGraphErrors *gB_on[4];
	 TGraphErrors *gB_off[4];
	 TGraphErrors *gC_on[4];
	 TGraphErrors *gC_off[4];
	 TGraphErrors *gD_on[4];
	 TGraphErrors *gD_off[4];

	 TGraphErrors *gA_beamoff_on[4];
	 TGraphErrors *gA_beamoff_off[4];
	 TGraphErrors *gB_beamoff_on[4];
	 TGraphErrors *gB_beamoff_off[4];
	 TGraphErrors *gC_beamoff_on[4];
	 TGraphErrors *gC_beamoff_off[4];
	 TGraphErrors *gD_beamoff_on[4];
	 TGraphErrors *gD_beamoff_off[4];

	 TGraphErrors *gA_beamoff_on_plus[4];
	 TGraphErrors *gA_beamoff_on_minus[4];


	 for(int ii=0;ii<4;ii++){
		gA_on[ii] = new TGraphErrors();
		gA_off[ii] = new TGraphErrors();
		gA_beamoff_on[ii] = new TGraphErrors();
		gA_beamoff_off[ii] = new TGraphErrors();
		gA_beamoff_on_plus[ii] = new TGraphErrors();
		gA_beamoff_on_minus[ii] = new TGraphErrors();
	
		for(int jj=0; jj<128; jj++){
		  Double_t tmpPlus_on=0, tmpMinus_on=0, tmpPlus_on_err=0, tmpMinus_on_err=0;
		  Double_t tmpPlus_off=0, tmpMinus_off=0, tmpPlus_off_err=0, tmpMinus_off_err=0;
		  Double_t tmpPlus_beamoff_on=0, tmpMinus_beamoff_on=0, tmpPlus_beamoff_on_err=0, tmpMinus_beamoff_on_err=0;
		  Double_t tmpPlus_beamoff_off=0, tmpMinus_beamoff_off=0, tmpPlus_beamoff_off_err=0, tmpMinus_beamoff_off_err=0;

	      if(NQplus_on[ii]!=0){
		    	tmpPlus_on = NplusA_on[ii][jj]/NQplus_on[ii];
		        tmpPlus_on_err = sqrt(NplusA_on[ii][jj])/NQplus_on[ii];}
	      if(NQplus_off[ii]!=0){
		    	tmpPlus_off = NplusA_off[ii][jj]/NQplus_off[ii];
		        tmpPlus_off_err = sqrt(NplusA_off[ii][jj])/NQplus_off[ii];}

	      if(NQminus_on[ii]!=0){
		    	tmpMinus_on = NminusA_on[ii][jj]/NQminus_on[ii];
		        tmpMinus_on_err = sqrt(NminusA_on[ii][jj])/NQminus_on[ii];}
	      if(NQminus_off[ii]!=0){
		    	tmpMinus_off = NminusA_off[ii][jj]/NQminus_off[ii];
		        tmpMinus_off_err = sqrt(NminusA_off[ii][jj])/NQminus_off[ii];}

	      if(NQplus_beamoff_on[ii]!=0){
		    	tmpPlus_beamoff_on = NplusA_beamoff_on[ii][jj]/NQplus_beamoff_on[ii];
		        tmpPlus_beamoff_on_err = sqrt(NplusA_beamoff_on[ii][jj])/NQplus_beamoff_on[ii];}
	      if(NQplus_beamoff_off[ii]!=0){
		    	tmpPlus_beamoff_off = NplusA_beamoff_off[ii][jj]/NQplus_beamoff_off[ii];
		        tmpPlus_beamoff_off_err = sqrt(NplusA_beamoff_off[ii][jj])/NQplus_beamoff_off[ii];}

	      if(NQminus_beamoff_on[ii]!=0){
		    	tmpMinus_beamoff_on = NminusA_beamoff_on[ii][jj]/NQminus_beamoff_on[ii];
		        tmpMinus_beamoff_on_err = sqrt(NminusA_beamoff_on[ii][jj])/NQminus_beamoff_on[ii];}
	      if(NQminus_beamoff_off[ii]!=0){
		    	tmpMinus_beamoff_off = NminusA_beamoff_off[ii][jj]/NQminus_beamoff_off[ii];
		        tmpMinus_beamoff_off_err = sqrt(NminusA_beamoff_off[ii][jj])/NQminus_beamoff_off[ii];}

		  Double_t asym_on=0, asym_off=0;
		  Double_t asym_on_err=0, asym_off_err=0;
		  Double_t asym_beamoff_on=0, asym_beamoff_off=0;
		  Double_t asym_beamoff_on_err=0, asym_beamoff_off_err=0;

		  if((NplusA_on[ii][jj]+NminusA_on[ii][jj])!=0) {
			asym_on = (tmpPlus_on - tmpMinus_on)/(tmpPlus_on + tmpMinus_on);
			asym_on_err = 2.0/pow(tmpPlus_on+tmpMinus_on,2) * sqrt(pow(tmpMinus_on*tmpPlus_on_err,2) + pow(tmpPlus_on*tmpMinus_on_err,2));	
		  }
		  if((NplusA_off[ii][jj]+NminusA_off[ii][jj])!=0) {
			asym_off = (tmpPlus_off - tmpMinus_off)/(tmpPlus_off + tmpMinus_off);
			asym_off_err = 2.0/pow(tmpPlus_off+tmpMinus_off,2) * sqrt(pow(tmpMinus_off*tmpPlus_off_err,2) + pow(tmpPlus_off*tmpMinus_off_err,2));	
		  }
		  if((NplusA_beamoff_on[ii][jj]+NminusA_beamoff_on[ii][jj])!=0) {
			asym_beamoff_on = (tmpPlus_beamoff_on - tmpMinus_beamoff_on)/(tmpPlus_beamoff_on + tmpMinus_beamoff_on);
			asym_beamoff_on_err = 2.0/pow(tmpPlus_beamoff_on+tmpMinus_beamoff_on,2) * sqrt(pow(tmpMinus_beamoff_on*tmpPlus_beamoff_on_err,2) + pow(tmpPlus_beamoff_on*tmpMinus_beamoff_on_err,2));	
		  }
		  if((NplusA_beamoff_off[ii][jj]+NminusA_beamoff_off[ii][jj])!=0) {
			asym_beamoff_off = (tmpPlus_beamoff_off - tmpMinus_beamoff_off)/(tmpPlus_beamoff_off + tmpMinus_beamoff_off);
			asym_beamoff_off_err = 2.0/pow(tmpPlus_beamoff_off+tmpMinus_beamoff_off,2) * sqrt(pow(tmpMinus_beamoff_off*tmpPlus_beamoff_off_err,2) + pow(tmpPlus_beamoff_off*tmpMinus_beamoff_off_err,2));	
		  }

		  gA_on[ii]->SetPoint(jj,jj,asym_on);
		  gA_on[ii]->SetPointError(jj,0,asym_on_err);
		  gA_off[ii]->SetPoint(jj,jj,asym_off);
		  gA_off[ii]->SetPointError(jj,0,asym_off_err);

		  gA_beamoff_on[ii]->SetPoint(jj,jj,asym_beamoff_on);
		  gA_beamoff_on[ii]->SetPointError(jj,0,asym_beamoff_on_err);
		  gA_beamoff_off[ii]->SetPoint(jj,jj,asym_beamoff_off);
		  gA_beamoff_off[ii]->SetPointError(jj,0,asym_beamoff_off_err);

		  gA_beamoff_on_plus[ii]->SetPoint(jj,jj,tmpPlus_beamoff_on);
		  gA_beamoff_on_plus[ii]->SetPointError(jj,0,tmpPlus_beamoff_on_err);
		  gA_beamoff_on_minus[ii]->SetPoint(jj,jj,tmpMinus_beamoff_on);
		  gA_beamoff_on_minus[ii]->SetPointError(jj,0,tmpMinus_beamoff_on_err);
		}
	 }
/*
	 for(int ii=0;ii<4;ii++){
		gB_on[ii] = new TGraphErrors();
		gB_off[ii] = new TGraphErrors();
		gB_beamoff_on[ii] = new TGraphErrors();
		gB_beamoff_off[ii] = new TGraphErrors();
	
		for(int jj=0; jj<128; jj++){
	      tmpPlus_on = NplusB_on[ii][jj]/NQplus_on[ii];
	       tmpMinus_on = NminusB_on[ii][jj]/NQminus_on[ii];
	       tmpPlus_off = NplusB_off[ii][jj]/NQplus_off[ii];
	       tmpMinus_off = NminusB_off[ii][jj]/NQminus_off[ii];

		   tmpPlus_on_err = sqrt(NplusB_on[ii][jj])/NQplus_on[ii];
		   tmpPlus_off_err = sqrt(NplusB_off[ii][jj])/NQplus_off[ii];
		   tmpMinus_on_err = sqrt(NminusB_on[ii][jj])/NQminus_on[ii];
		   tmpMinus_off_err = sqrt(NminusB_off[ii][jj])/NQminus_off[ii];

	       tmpPlus_beamoff_on = NplusB_beamoff_on[ii][jj]/NQplus_beamoff_on[ii];
	       tmpMinus_beamoff_on = NminusB_beamoff_on[ii][jj]/NQminus_beamoff_on[ii];
	       tmpPlus_beamoff_off = NplusB_beamoff_off[ii][jj]/NQplus_beamoff_off[ii];
	       tmpMinus_beamoff_off = NminusB_beamoff_off[ii][jj]/NQminus_beamoff_off[ii];

		   tmpPlus_beamoff_on_err = sqrt(NplusB_beamoff_on[ii][jj])/NQplus_beamoff_on[ii];
		   tmpPlus_beamoff_off_err = sqrt(NplusB_beamoff_off[ii][jj])/NQplus_beamoff_off[ii];
		   tmpMinus_beamoff_on_err = sqrt(NminusB_beamoff_on[ii][jj])/NQminus_beamoff_on[ii];
		   tmpMinus_beamoff_off_err = sqrt(NminusB_beamoff_off[ii][jj])/NQminus_beamoff_off[ii];

		  Double_t asym_on=0, asym_off=0;
		  Double_t asym_on_err=0, asym_off_err=0;
		  Double_t asym_beamoff_on=0, asym_beamoff_off=0;
		  Double_t asym_beamoff_on_err=0, asym_beamoff_off_err=0;

		  if((NplusB_on[ii][jj]+NminusB_on[ii][jj])!=0) {
			asym_on = (tmpPlus_on - tmpMinus_on)/(tmpPlus_on + tmpMinus_on);
			asym_on_err = 2.0/pow(tmpPlus_on+tmpMinus_on,2) * sqrt(pow(tmpMinus_on*tmpPlus_on_err,2) + pow(tmpPlus_on*tmpMinus_on_err,2));	
		  }
		  if((NplusB_off[ii][jj]+NminusB_off[ii][jj])!=0) {
			asym_off = (tmpPlus_off - tmpMinus_off)/(tmpPlus_off + tmpMinus_off);
			asym_off_err = 2.0/pow(tmpPlus_off+tmpMinus_off,2) * sqrt(pow(tmpMinus_off*tmpPlus_off_err,2) + pow(tmpPlus_off*tmpMinus_off_err,2));	
		  }
		  if((NplusB_beamoff_on[ii][jj]+NminusB_beamoff_on[ii][jj])!=0) {
			asym_beamoff_on = (tmpPlus_beamoff_on - tmpMinus_beamoff_on)/(tmpPlus_beamoff_on + tmpMinus_beamoff_on);
			asym_beamoff_on_err = 2.0/pow(tmpPlus_beamoff_on+tmpMinus_beamoff_on,2) * sqrt(pow(tmpMinus_beamoff_on*tmpPlus_beamoff_on_err,2) + pow(tmpPlus_beamoff_on*tmpMinus_beamoff_on_err,2));	
		  }
		  if((NplusB_beamoff_off[ii][jj]+NminusB_beamoff_off[ii][jj])!=0) {
			asym_beamoff_off = (tmpPlus_beamoff_off - tmpMinus_beamoff_off)/(tmpPlus_beamoff_off + tmpMinus_beamoff_off);
			asym_beamoff_off_err = 2.0/pow(tmpPlus_beamoff_off+tmpMinus_beamoff_off,2) * sqrt(pow(tmpMinus_beamoff_off*tmpPlus_beamoff_off_err,2) + pow(tmpPlus_beamoff_off*tmpMinus_beamoff_off_err,2));	
		  }

		  gB_on[ii]->SetPoint(jj,jj,asym_on);
		  gB_on[ii]->SetPointError(jj,0,asym_on_err);
		  gB_off[ii]->SetPoint(jj,jj,asym_off);
		  gB_off[ii]->SetPointError(jj,0,asym_off_err);

		  gB_beamoff_on[ii]->SetPoint(jj,jj,asym_beamoff_on);
		  gB_beamoff_on[ii]->SetPointError(jj,0,asym_beamoff_on_err);
		  gB_beamoff_off[ii]->SetPoint(jj,jj,asym_beamoff_off);
		  gB_beamoff_off[ii]->SetPointError(jj,0,asym_beamoff_off_err);
		}
	 }

	 for(int ii=0;ii<4;ii++){
		gC_on[ii] = new TGraphErrors();
		gC_off[ii] = new TGraphErrors();
		gC_beamoff_on[ii] = new TGraphErrors();
		gC_beamoff_off[ii] = new TGraphErrors();
	
		for(int jj=0; jj<128; jj++){
	      Double_t tmpPlus_on = NplusC_on[ii][jj]/NQplus_on[ii];
	      Double_t tmpMinus_on = NminusC_on[ii][jj]/NQminus_on[ii];
	      Double_t tmpPlus_off = NplusC_off[ii][jj]/NQplus_off[ii];
	      Double_t tmpMinus_off = NminusC_off[ii][jj]/NQminus_off[ii];

		  Double_t tmpPlus_on_err = sqrt(NplusC_on[ii][jj])/NQplus_on[ii];
		  Double_t tmpPlus_off_err = sqrt(NplusC_off[ii][jj])/NQplus_off[ii];
		  Double_t tmpMinus_on_err = sqrt(NminusC_on[ii][jj])/NQminus_on[ii];
		  Double_t tmpMinus_off_err = sqrt(NminusC_off[ii][jj])/NQminus_off[ii];

	      Double_t tmpPlus_beamoff_on = NplusC_beamoff_on[ii][jj]/NQplus_beamoff_on[ii];
	      Double_t tmpMinus_beamoff_on = NminusC_beamoff_on[ii][jj]/NQminus_beamoff_on[ii];
	      Double_t tmpPlus_beamoff_off = NplusC_beamoff_off[ii][jj]/NQplus_beamoff_off[ii];
	      Double_t tmpMinus_beamoff_off = NminusC_beamoff_off[ii][jj]/NQminus_beamoff_off[ii];

		  Double_t tmpPlus_beamoff_on_err = sqrt(NplusC_beamoff_on[ii][jj])/NQplus_beamoff_on[ii];
		  Double_t tmpPlus_beamoff_off_err = sqrt(NplusC_beamoff_off[ii][jj])/NQplus_beamoff_off[ii];
		  Double_t tmpMinus_beamoff_on_err = sqrt(NminusC_beamoff_on[ii][jj])/NQminus_beamoff_on[ii];
		  Double_t tmpMinus_beamoff_off_err = sqrt(NminusC_beamoff_off[ii][jj])/NQminus_beamoff_off[ii];

		  Double_t asym_on=0, asym_off=0;
		  Double_t asym_on_err=0, asym_off_err=0;
		  Double_t asym_beamoff_on=0, asym_beamoff_off=0;
		  Double_t asym_beamoff_on_err=0, asym_beamoff_off_err=0;

		  if((NplusC_on[ii][jj]+NminusC_on[ii][jj])!=0) {
			asym_on = (tmpPlus_on - tmpMinus_on)/(tmpPlus_on + tmpMinus_on);
			asym_on_err = 2.0/pow(tmpPlus_on+tmpMinus_on,2) * sqrt(pow(tmpMinus_on*tmpPlus_on_err,2) + pow(tmpPlus_on*tmpMinus_on_err,2));	
		  }
		  if((NplusC_off[ii][jj]+NminusC_off[ii][jj])!=0) {
			asym_off = (tmpPlus_off - tmpMinus_off)/(tmpPlus_off + tmpMinus_off);
			asym_off_err = 2.0/pow(tmpPlus_off+tmpMinus_off,2) * sqrt(pow(tmpMinus_off*tmpPlus_off_err,2) + pow(tmpPlus_off*tmpMinus_off_err,2));	
		  }
		  if((NplusC_beamoff_on[ii][jj]+NminusC_beamoff_on[ii][jj])!=0) {
			asym_beamoff_on = (tmpPlus_beamoff_on - tmpMinus_beamoff_on)/(tmpPlus_beamoff_on + tmpMinus_beamoff_on);
			asym_beamoff_on_err = 2.0/pow(tmpPlus_beamoff_on+tmpMinus_beamoff_on,2) * sqrt(pow(tmpMinus_beamoff_on*tmpPlus_beamoff_on_err,2) + pow(tmpPlus_beamoff_on*tmpMinus_beamoff_on_err,2));	
		  }
		  if((NplusC_beamoff_off[ii][jj]+NminusC_beamoff_off[ii][jj])!=0) {
			asym_beamoff_off = (tmpPlus_beamoff_off - tmpMinus_beamoff_off)/(tmpPlus_beamoff_off + tmpMinus_beamoff_off);
			asym_beamoff_off_err = 2.0/pow(tmpPlus_beamoff_off+tmpMinus_beamoff_off,2) * sqrt(pow(tmpMinus_beamoff_off*tmpPlus_beamoff_off_err,2) + pow(tmpPlus_beamoff_off*tmpMinus_beamoff_off_err,2));	
		  }

		  gC_on[ii]->SetPoint(jj,jj,asym_on);
		  gC_on[ii]->SetPointError(jj,0,asym_on_err);
		  gC_off[ii]->SetPoint(jj,jj,asym_off);
		  gC_off[ii]->SetPointError(jj,0,asym_off_err);

		  gC_beamoff_on[ii]->SetPoint(jj,jj,asym_beamoff_on);
		  gC_beamoff_on[ii]->SetPointError(jj,0,asym_beamoff_on_err);
		  gC_beamoff_off[ii]->SetPoint(jj,jj,asym_beamoff_off);
		  gC_beamoff_off[ii]->SetPointError(jj,0,asym_beamoff_off_err);
		}
	 }

	 for(int ii=0;ii<4;ii++){
		gD_on[ii] = new TGraphErrors();
		gD_off[ii] = new TGraphErrors();
		gD_beamoff_on[ii] = new TGraphErrors();
		gD_beamoff_off[ii] = new TGraphErrors();
	
		for(int jj=0; jj<128; jj++){
	      Double_t tmpPlus_on = NplusD_on[ii][jj]/NQplus_on[ii];
	      Double_t tmpMinus_on = NminusD_on[ii][jj]/NQminus_on[ii];
	      Double_t tmpPlus_off = NplusD_off[ii][jj]/NQplus_off[ii];
	      Double_t tmpMinus_off = NminusD_off[ii][jj]/NQminus_off[ii];

		  Double_t tmpPlus_on_err = sqrt(NplusD_on[ii][jj])/NQplus_on[ii];
		  Double_t tmpPlus_off_err = sqrt(NplusD_off[ii][jj])/NQplus_off[ii];
		  Double_t tmpMinus_on_err = sqrt(NminusD_on[ii][jj])/NQminus_on[ii];
		  Double_t tmpMinus_off_err = sqrt(NminusD_off[ii][jj])/NQminus_off[ii];

	      Double_t tmpPlus_beamoff_on = NplusD_beamoff_on[ii][jj]/NQplus_beamoff_on[ii];
	      Double_t tmpMinus_beamoff_on = NminusD_beamoff_on[ii][jj]/NQminus_beamoff_on[ii];
	      Double_t tmpPlus_beamoff_off = NplusD_beamoff_off[ii][jj]/NQplus_beamoff_off[ii];
	      Double_t tmpMinus_beamoff_off = NminusD_beamoff_off[ii][jj]/NQminus_beamoff_off[ii];

		  Double_t tmpPlus_beamoff_on_err = sqrt(NplusD_beamoff_on[ii][jj])/NQplus_beamoff_on[ii];
		  Double_t tmpPlus_beamoff_off_err = sqrt(NplusD_beamoff_off[ii][jj])/NQplus_beamoff_off[ii];
		  Double_t tmpMinus_beamoff_on_err = sqrt(NminusD_beamoff_on[ii][jj])/NQminus_beamoff_on[ii];
		  Double_t tmpMinus_beamoff_off_err = sqrt(NminusD_beamoff_off[ii][jj])/NQminus_beamoff_off[ii];

		  Double_t asym_on=0, asym_off=0;
		  Double_t asym_on_err=0, asym_off_err=0;
		  Double_t asym_beamoff_on=0, asym_beamoff_off=0;
		  Double_t asym_beamoff_on_err=0, asym_beamoff_off_err=0;

		  if((NplusD_on[ii][jj]+NminusD_on[ii][jj])!=0) {
			asym_on = (tmpPlus_on - tmpMinus_on)/(tmpPlus_on + tmpMinus_on);
			asym_on_err = 2.0/pow(tmpPlus_on+tmpMinus_on,2) * sqrt(pow(tmpMinus_on*tmpPlus_on_err,2) + pow(tmpPlus_on*tmpMinus_on_err,2));	
		  }
		  if((NplusD_off[ii][jj]+NminusD_off[ii][jj])!=0) {
			asym_off = (tmpPlus_off - tmpMinus_off)/(tmpPlus_off + tmpMinus_off);
			asym_off_err = 2.0/pow(tmpPlus_off+tmpMinus_off,2) * sqrt(pow(tmpMinus_off*tmpPlus_off_err,2) + pow(tmpPlus_off*tmpMinus_off_err,2));	
		  }
		  if((NplusD_beamoff_on[ii][jj]+NminusD_beamoff_on[ii][jj])!=0) {
			asym_beamoff_on = (tmpPlus_beamoff_on - tmpMinus_beamoff_on)/(tmpPlus_beamoff_on + tmpMinus_beamoff_on);
			asym_beamoff_on_err = 2.0/pow(tmpPlus_beamoff_on+tmpMinus_beamoff_on,2) * sqrt(pow(tmpMinus_beamoff_on*tmpPlus_beamoff_on_err,2) + pow(tmpPlus_beamoff_on*tmpMinus_beamoff_on_err,2));	
		  }
		  if((NplusD_beamoff_off[ii][jj]+NminusD_beamoff_off[ii][jj])!=0) {
			asym_beamoff_off = (tmpPlus_beamoff_off - tmpMinus_beamoff_off)/(tmpPlus_beamoff_off + tmpMinus_beamoff_off);
			asym_beamoff_off_err = 2.0/pow(tmpPlus_beamoff_off+tmpMinus_beamoff_off,2) * sqrt(pow(tmpMinus_beamoff_off*tmpPlus_beamoff_off_err,2) + pow(tmpPlus_beamoff_off*tmpMinus_beamoff_off_err,2));	
		  }

		  gD_on[ii]->SetPoint(jj,jj,asym_on);
		  gD_on[ii]->SetPointError(jj,0,asym_on_err);
		  gD_off[ii]->SetPoint(jj,jj,asym_off);
		  gD_off[ii]->SetPointError(jj,0,asym_off_err);

		  gD_beamoff_on[ii]->SetPoint(jj,jj,asym_beamoff_on);
		  gD_beamoff_on[ii]->SetPointError(jj,0,asym_beamoff_on_err);
		  gD_beamoff_off[ii]->SetPoint(jj,jj,asym_beamoff_off);
		  gD_beamoff_off[ii]->SetPointError(jj,0,asym_beamoff_off_err);
		}
	 }
*/
	 TCanvas *c1=new TCanvas("c1","c1",1200,1200);
	 c1->Divide(2,2);
	 TMultiGraph *mgA[4];
	 for(int ii=0; ii<4; ii++){
	   c1->cd(ii+1);
	   mgA[ii] = new TMultiGraph();
   	   gA_on[ii]->SetMarkerColor(2);	 
   	   gA_on[ii]->SetMarkerStyle(8);	 
   	   gA_on[ii]->SetMarkerSize(1.3);	 
   	   gA_off[ii]->SetMarkerColor(4);	 
   	   gA_off[ii]->SetMarkerStyle(8);	 
   	   gA_off[ii]->SetMarkerSize(1.3);	 
   	   gA_beamoff_on[ii]->SetMarkerColor(8);	 
   	   gA_beamoff_on[ii]->SetMarkerStyle(8);	 
   	   gA_beamoff_on[ii]->SetMarkerSize(1.3);	 
   	   gA_beamoff_off[ii]->SetMarkerColor(1);	 
   	   gA_beamoff_off[ii]->SetMarkerStyle(8);	 
   	   gA_beamoff_off[ii]->SetMarkerSize(1.3);	 
	
	   mgA[ii]->Add(gA_on[ii]);
	   mgA[ii]->Add(gA_off[ii]);
	   mgA[ii]->Add(gA_beamoff_on[ii]);
	   mgA[ii]->Add(gA_beamoff_off[ii]);
	   mgA[ii]->Draw("AP");
	   mgA[ii]->SetTitle(Form("plane A asymmetry part %d;chan;asym",ii+1));
	  // mgA[ii]->GetYaxis()->SetRangeUser(-0.04,0.04);

	   auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	   leg1->AddEntry(gA_on[ii],"beam on laser on","P");
	   leg1->AddEntry(gA_off[ii],"beam on laser off","P");
	   leg1->AddEntry(gA_beamoff_on[ii],"beam off laser on","P");
	   leg1->AddEntry(gA_beamoff_off[ii],"beam off laser off","P");
	   leg1->Draw(); 
     }
/*
	 TCanvas *c2=new TCanvas("c2","c2",1200,1200);
	 c2->Divide(2,2);
	 TMultiGraph *mgB[4];
	 for(int ii=0; ii<4; ii++){
	   c2->cd(ii+1);
	   mgB[ii] = new TMultiGraph();
   	   gB_on[ii]->SetMarkerColor(2);	 
   	   gB_on[ii]->SetMarkerStyle(8);	 
   	   gB_on[ii]->SetMarkerSize(1.3);	 
   	   gB_off[ii]->SetMarkerColor(4);	 
   	   gB_off[ii]->SetMarkerStyle(8);	 
   	   gB_off[ii]->SetMarkerSize(1.3);	 
   	   gB_beamoff_on[ii]->SetMarkerColor(8);	 
   	   gB_beamoff_on[ii]->SetMarkerStyle(8);	 
   	   gB_beamoff_on[ii]->SetMarkerSize(1.3);	 
   	   gB_beamoff_off[ii]->SetMarkerColor(1);	 
   	   gB_beamoff_off[ii]->SetMarkerStyle(8);	 
   	   gB_beamoff_off[ii]->SetMarkerSize(1.3);	 
	
	   mgB[ii]->Add(gB_on[ii]);
	   mgB[ii]->Add(gB_off[ii]);
	   mgB[ii]->Add(gB_beamoff_on[ii]);
	   mgB[ii]->Add(gB_beamoff_off[ii]);
	   mgB[ii]->Draw("AP");
	   mgB[ii]->SetTitle(Form("plane B asymmetry part %d;chan;asym",ii+1));
	   //mgB[ii]->GetYaxis()->SetRangeUser(-0.04,0.04);

	   auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	   leg1->AddEntry(gB_on[ii],"beam on laser on","P");
	   leg1->AddEntry(gB_off[ii],"beam on laser off","P");
	   leg1->AddEntry(gB_beamoff_on[ii],"beam off laser on","P");
	   leg1->AddEntry(gB_beamoff_off[ii],"beam off laser off","P");
	   leg1->Draw(); 
     }

	 TCanvas *c3=new TCanvas("c3","c3",1200,1200);
	 c3->Divide(2,2);
	 TMultiGraph *mgC[4];
	 for(int ii=0; ii<4; ii++){
	   c3->cd(ii+1);
	   mgC[ii] = new TMultiGraph();
   	   gC_on[ii]->SetMarkerColor(2);	 
   	   gC_on[ii]->SetMarkerStyle(8);	 
   	   gC_on[ii]->SetMarkerSize(1.3);	 
   	   gC_off[ii]->SetMarkerColor(4);	 
   	   gC_off[ii]->SetMarkerStyle(8);	 
   	   gC_off[ii]->SetMarkerSize(1.3);	 
   	   gC_beamoff_on[ii]->SetMarkerColor(8);	 
   	   gC_beamoff_on[ii]->SetMarkerStyle(8);	 
   	   gC_beamoff_on[ii]->SetMarkerSize(1.3);	 
   	   gC_beamoff_off[ii]->SetMarkerColor(1);	 
   	   gC_beamoff_off[ii]->SetMarkerStyle(8);	 
   	   gC_beamoff_off[ii]->SetMarkerSize(1.3);	 
	
	   mgC[ii]->Add(gC_on[ii]);
	   mgC[ii]->Add(gC_off[ii]);
	   mgC[ii]->Add(gC_beamoff_on[ii]);
	   mgC[ii]->Add(gC_beamoff_off[ii]);
	   mgC[ii]->Draw("AP");
	   mgC[ii]->SetTitle(Form("plane C asymmetry part %d;chan;asym",ii+1));
	   //mgC[ii]->GetYaxis()->SetRangeUser(-0.04,0.04);

	   auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	   leg1->AddEntry(gC_on[ii],"beam on laser on","P");
	   leg1->AddEntry(gC_off[ii],"beam on laser off","P");
	   leg1->AddEntry(gC_beamoff_on[ii],"beam off laser on","P");
	   leg1->AddEntry(gC_beamoff_off[ii],"beam off laser off","P");
	   leg1->Draw(); 
     }

	 TCanvas *c4=new TCanvas("c4","c4",1200,1200);
	 c4->Divide(2,2);
	 TMultiGraph *mgD[4];
	 for(int ii=0; ii<4; ii++){
	   c4->cd(ii+1);
	   mgD[ii] = new TMultiGraph();
   	   gD_on[ii]->SetMarkerColor(2);	 
   	   gD_on[ii]->SetMarkerStyle(8);	 
   	   gD_on[ii]->SetMarkerSize(1.3);	 
   	   gD_off[ii]->SetMarkerColor(4);	 
   	   gD_off[ii]->SetMarkerStyle(8);	 
   	   gD_off[ii]->SetMarkerSize(1.3);	 
   	   gD_beamoff_on[ii]->SetMarkerColor(8);	 
   	   gD_beamoff_on[ii]->SetMarkerStyle(8);	 
   	   gD_beamoff_on[ii]->SetMarkerSize(1.3);	 
   	   gD_beamoff_off[ii]->SetMarkerColor(1);	 
   	   gD_beamoff_off[ii]->SetMarkerStyle(8);	 
   	   gD_beamoff_off[ii]->SetMarkerSize(1.3);	 
	
	   mgD[ii]->Add(gD_on[ii]);
	   mgD[ii]->Add(gD_off[ii]);
	   mgD[ii]->Add(gD_beamoff_on[ii]);
	   mgD[ii]->Add(gD_beamoff_off[ii]);
	   mgD[ii]->Draw("AP");
	   mgD[ii]->SetTitle(Form("plane D asymmetry part %d;chan;asym",ii+1));
	   //mgD[ii]->GetYaxis()->SetRangeUser(-0.04,0.04);

	   auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	   leg1->AddEntry(gD_on[ii],"beam on laser on","P");
	   leg1->AddEntry(gD_off[ii],"beam on laser off","P");
	   leg1->AddEntry(gD_beamoff_on[ii],"beam off laser on","P");
	   leg1->AddEntry(gD_beamoff_off[ii],"beam off laser off","P");
	   leg1->Draw(); 
     }

     c1->Print(Form("Asymmetry_%d.pdf[",runnumber));
     c1->Print(Form("Asymmetry_%d.pdf",runnumber));
     c2->Print(Form("Asymmetry_%d.pdf",runnumber));
     c3->Print(Form("Asymmetry_%d.pdf",runnumber));
     c4->Print(Form("Asymmetry_%d.pdf",runnumber));
     c4->Print(Form("Asymmetry_%d.pdf]",runnumber));
*/

  TCanvas *c5 = new TCanvas("c5","c5",1500,1500);
     c5->Divide(2,2);
	 TMultiGraph *mgA_single[4];
	 for(int ii=0; ii<4; ii++){
	   c5->cd(ii+1);
	   mgA_single[ii] = new TMultiGraph();
   	   gA_beamoff_on_plus[ii]->SetMarkerColor(2);	 
   	   gA_beamoff_on_plus[ii]->SetMarkerStyle(8);	 
   	   gA_beamoff_on_plus[ii]->SetMarkerSize(1.3);	 
   	   gA_beamoff_on_minus[ii]->SetMarkerColor(4);	 
   	   gA_beamoff_on_minus[ii]->SetMarkerStyle(8);	 
   	   gA_beamoff_on_minus[ii]->SetMarkerSize(1.3);	 
	
	   mgA_single[ii]->Add(gA_beamoff_on_plus[ii]);
	   mgA_single[ii]->Add(gA_beamoff_on_minus[ii]);
	   mgA_single[ii]->Draw("AP");
	   mgA_single[ii]->SetTitle(Form("plane A cnt/charge part %d;chan;",ii+1));
	   //mgA_single[ii]->GetYaxis()->SetRangeUser(-0.04,0.04);

	   auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	   leg1->AddEntry(gA_beamoff_on_plus[ii],"beam off laser on (+)","P");
	   leg1->AddEntry(gA_beamoff_on_minus[ii],"beam off laser on (-)","P");
	   leg1->Draw(); 
     }

}

