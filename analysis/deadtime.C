void deadtime()
{
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTP = (TTree*) f0->Get("VTP");
	 T->AddFriend(VTP);

     Double_t gain = 0.00033;
     Double_t offset = -1.1678;

     Int_t nentries=0;
     nentries = VTP->GetEntries();
     cout<<"Total entries:  "<<nentries<<endl;
     Int_t maxevent=0;
     cout<<"How many events ? (0 for total) ";
     cin>>maxevent;

     if(maxevent==0)maxevent=nentries;

	 Int_t evtype,BCM,CavPower,hel_win_cnt,hel_win_cnt_1;
	 Int_t trigcnt[5]={0},scaldat[16]={0};

     T->SetBranchAddress("evtype",&evtype);
     T->SetBranchAddress("hel_win_cnt",&hel_win_cnt);
     T->SetBranchAddress("hel_win_cnt_1",&hel_win_cnt_1);
     T->SetBranchAddress("trigcnt",trigcnt);
     T->SetBranchAddress("scaldat",scaldat);

	 Int_t ti_laseron[4]={0}, ti_laseroff[4]={0}, ti_beamoff[4]={0}, tmp_trigcnt[4]={0};
	 Int_t vtp_laseron[4]={0}, vtp_laseroff[4]={0}, vtp_beamoff[4]={0};
	 Int_t hel_win_cnt_0=0;
	 for(int ii=0;ii<maxevent;ii++){
		T->GetEntry(ii);
		if(ii==0) hel_win_cnt_0 = hel_win_cnt;
	
	    for(int kk=0;kk<4; kk++){
		  if( ((evtype>>kk)&0x1)==1 )
			tmp_trigcnt[kk]++;
		} 
			
	    if(scaldat[1]>0 && hel_win_cnt_1>=hel_win_cnt_0){   // scaler update
		   BCM = scaldat[15]*1.0/(1./120.)*gain+offset;
		   CavPower = scaldat[12];
		  
		   for(int jj=0; jj<4; jj++){
		     if(BCM>130 && CavPower>100){
				ti_laseron[jj] += tmp_trigcnt[jj];
			    vtp_laseron[jj] += trigcnt[jj];				
		     }
		     if(BCM>130 && CavPower<40){
				ti_laseroff[jj] += tmp_trigcnt[jj];
			    vtp_laseroff[jj] += trigcnt[jj];				
		     }
		     if(scaldat[15]==0){
				ti_beamoff[jj] += tmp_trigcnt[jj];
			    vtp_beamoff[jj] += trigcnt[jj];				
		     }

			 tmp_trigcnt[jj]=0;
		   }
		}
 	    //cout<<tmp_trigcnt[0]<<"  "<<ti_beamoff[0]<<"  "<<vtp_beamoff[0]<<endl;
	 }

	 Double_t DT_laseron[4]={0}, DT_laseroff[4]={0}, DT_beamoff[4]={0};
	 Double_t DT_laseron_err[4]={0}, DT_laseroff_err[4]={0}, DT_beamoff_err[4]={0};
	 for(int ii=0; ii<4; ii++){
	   if(ii==2){
		DT_laseron[ii] = 1 - 1000.0*ti_laseron[ii]/(1.0*vtp_laseron[ii]); 
		DT_laseroff[ii] = 1 - 1000.0*ti_laseroff[ii]/(1.0*vtp_laseroff[ii]); 
		DT_beamoff[ii] = 1 - 1000.0*ti_beamoff[ii]/(1.0*vtp_beamoff[ii]); 
		
		DT_laseron_err[ii] = 1000.0*ti_laseron[ii]/(1.0*vtp_laseron[ii])*sqrt(1.0/(1.0*ti_laseron[ii])-1.0/(1.0*vtp_laseron[ii]));
		DT_laseroff_err[ii] = 1000.0*ti_laseroff[ii]/(1.0*vtp_laseroff[ii])*sqrt(1.0/(1.0*ti_laseroff[ii])-1.0/(1.0*vtp_laseroff[ii]));
		DT_beamoff_err[ii] = 1000.0*ti_beamoff[ii]/(1.0*vtp_beamoff[ii])*sqrt(1.0/(1.0*ti_beamoff[ii])-1.0/(1.0*vtp_beamoff[ii]));
	   }
	   if(ii==0){
		DT_laseron[ii] = 1 - 1.0*ti_laseron[ii]/(1.0*vtp_laseron[ii]); 
		DT_laseroff[ii] = 1 - 1.0*ti_laseroff[ii]/(1.0*vtp_laseroff[ii]); 
		DT_beamoff[ii] = 1 - 1.0*ti_beamoff[ii]/(1.0*vtp_beamoff[ii]); 

		DT_laseron_err[ii] = 1.0*ti_laseron[ii]/(1.0*vtp_laseron[ii])*sqrt(1.0/(1.0*ti_laseron[ii])-1.0/(1.0*vtp_laseron[ii]));
		DT_laseroff_err[ii] = 1.0*ti_laseroff[ii]/(1.0*vtp_laseroff[ii])*sqrt(1.0/(1.0*ti_laseroff[ii])-1.0/(1.0*vtp_laseroff[ii]));
		DT_beamoff_err[ii] = 1.0*ti_beamoff[ii]/(1.0*vtp_beamoff[ii])*sqrt(1.0/(1.0*ti_beamoff[ii])-1.0/(1.0*vtp_beamoff[ii]));
	   }
	 }

	 cout<<"Dead time:     T1     err    T3    err"<<endl;
	 cout<<"laser on:     "<<DT_laseron[0]<<"   "<<DT_laseron_err[0]<<"    "<<DT_laseron[2]<<"    "<<DT_laseron_err[2]<<endl;
	 cout<<"laser off:    "<<DT_laseroff[0]<<"   "<<DT_laseroff_err[0]<<"    "<<DT_laseroff[2]<<"    "<<DT_laseroff_err[2]<<endl;
	 cout<<"beam off:     "<<DT_beamoff[0]<<"   "<<DT_beamoff_err[0]<<"    "<<DT_beamoff[2]<<"    "<<DT_beamoff_err[2]<<endl;

/*
     TCanvas *c1 = new TCanvas("c1","c1",1500,1500);
	 TH1F *hrate[4];
	 int color[4]={2,8,4,1};
     for(int ii=0; ii<4; ii++){  
	   hrate[ii] = new TH1F(Form("hrate%d",ii),Form("trigger %d rate",ii+1),500,0,200000);  
	   if(ii==0) T->Draw(Form("trigcnt[%d]*1.0/(1./120.)>>hrate%d",ii,ii));
	   if(ii==2)  T->Draw(Form("trigcnt[%d]*1.0/(1./120.)>>hrate%d",ii,ii),"","same");
	   hrate[ii]->SetLineColor(color[ii]);
	   hrate[ii]->SetLineWidth(2);
	 }

     TLegend *leg = new TLegend(0.75,0.75,0.85,0.85);
     leg->AddEntry(hrate[0],"T1","L");
     leg->AddEntry(hrate[2],"T3","L");
	 leg->SetMargin(0.4);
	 leg->Draw(); 
*/
}
