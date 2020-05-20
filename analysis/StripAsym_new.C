void StripAsym(){
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

     Int_t BCM,CavPower,cur_hel;
     VTPScal->SetBranchAddress("cur_hel",&cur_hel);

     Double_t NplusA_on[8][128], NplusA_off[8][128], NminusA_on[8][128], NminusA_off[8][128];
     Double_t NplusB_on[8][128], NplusB_off[8][128], NminusB_on[8][128], NminusB_off[8][128];
     Double_t NplusC_on[8][128], NplusC_off[8][128], NminusC_on[8][128], NminusC_off[8][128];
     Double_t NplusD_on[8][128], NplusD_off[8][128], NminusD_on[8][128], NminusD_off[8][128];
     Double_t NQplus_on[8]={0}, NQminus_on[8]={0}, NQplus_off[8]={0}, NQminus_off[8]={0};

     for(int ii=0; ii<8; ii++){
		for(int jj=0;jj<128;jj++){
		  NplusA_on[ii][jj]=0; NplusA_off[ii][jj]=0;NminusA_on[ii][jj]=0; NminusA_off[ii][jj]=0;
		  NplusB_on[ii][jj]=0; NplusB_off[ii][jj]=0;NminusB_on[ii][jj]=0; NminusB_off[ii][jj]=0;
		  NplusC_on[ii][jj]=0; NplusC_off[ii][jj]=0;NminusC_on[ii][jj]=0; NminusC_off[ii][jj]=0;
		  NplusD_on[ii][jj]=0; NplusD_off[ii][jj]=0;NminusD_on[ii][jj]=0; NminusD_off[ii][jj]=0;
		}
	 }

     Int_t hel_win_cnt_0=0;;
	 for(int ii=0; ii<maxevent; ii++){
		VTP->GetEntry(ii);
	    if(ii==0) hel_win_cnt_0 = hel_win_cnt;
		if( hel_win_cnt_1<(hel_win_cnt_0+8) )	continue;  // so it's able to check different delay helicity
		if(scaldat[1]==0) continue;   // scaler is not update

		Double_t BCM = scaldat[15];
		Double_t CavPower = scaldat[12];
	    Double_t charge = (BCM*gain/(1./120.)+offset)*(1./120.);    // charge for one helicity window

		for(int jj=0; jj<8; jj++){
		  VTPScal->GetEntry(hel_win_cnt_1-jj);
		  if(BCM>3500 && CavPower>100){
			if(cur_hel==0) NQminus_on[jj]+= charge;
			if(cur_hel==1) NQplus_on[jj]+= charge;
		  }
		  if(BCM>3500 && CavPower<40){
			if(cur_hel==0) NQminus_off[jj]+= charge;
			if(cur_hel==1) NQplus_off[jj] += charge;
		  }

		  for(int kk=0; kk<128; kk++){
		    if(BCM>3500 && CavPower>100){
			 if(cur_hel==0){
			   NminusA_on[jj][kk] += vtpA_scal[kk];
			   NminusB_on[jj][kk] += vtpB_scal[kk];
			   NminusC_on[jj][kk] += vtpC_scal[kk];
			   NminusD_on[jj][kk] += vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_on[jj][kk] += vtpA_scal[kk];
			   NplusB_on[jj][kk] += vtpB_scal[kk];
			   NplusC_on[jj][kk] += vtpC_scal[kk];
			   NplusD_on[jj][kk] += vtpD_scal[kk];
			 }
		    }
		    if(BCM>3500 && CavPower<40){
			 if(cur_hel==0){
			   NminusA_off[jj][kk] += vtpA_scal[kk];
			   NminusB_off[jj][kk] += vtpB_scal[kk];
			   NminusC_off[jj][kk] += vtpC_scal[kk];
			   NminusD_off[jj][kk] += vtpD_scal[kk];
			 }
			 if(cur_hel==1){
			   NplusA_off[jj][kk] += vtpA_scal[kk];
			   NplusB_off[jj][kk] += vtpB_scal[kk];
			   NplusC_off[jj][kk] += vtpC_scal[kk];
			   NplusD_off[jj][kk] += vtpD_scal[kk];
			 }
		    }
		  }
		}
	 }

cout<<NQminus_on[0]<<"  "<<NQplus_on[0]<<endl;

	 TGraph *gA_on[8];
	 TGraph *gA_off[8];
	 TGraph *gB_on[8];
	 TGraph *gB_off[8];
	 TGraph *gC_on[8];
	 TGraph *gC_off[8];
	 TGraph *gD_on[8];
	 TGraph *gD_off[8];

	 for(int ii=0;ii<8;ii++){
		gA_on[ii] = new TGraph();
		gA_off[ii] = new TGraph();
		
		for(int jj=0; jj<128; jj++){
		  Double_t asym_on=0, asym_off=0;
		  if((NplusA_on[ii][jj]+NminusA_on[ii][jj])!=0) asym_on = (NplusA_on[ii][jj]/NQplus_on[ii]-NminusA_on[ii][jj]/NQminus_on[ii])/(NplusA_on[ii][jj]/NQplus_on[ii]+NminusA_on[ii][jj]/NQminus_on[ii]);
		  if((NplusA_off[ii][jj]+NminusA_off[ii][jj])!=0) asym_off = (NplusA_off[ii][jj]/NQplus_off[ii]-NminusA_off[ii][jj]/NQminus_off[ii])/(NplusA_off[ii][jj]/NQplus_off[ii]+NminusA_off[ii][jj]/NQminus_off[ii]);

		  gA_on[ii]->SetPoint(jj,jj,asym_on);
		  gA_off[ii]->SetPoint(jj,jj,asym_off);
		}
	 }

	 for(int ii=0;ii<8;ii++){
		gB_on[ii] = new TGraph();
		gB_off[ii] = new TGraph();
		
		for(int jj=0; jj<128; jj++){
		  Double_t asym_on=0, asym_off=0;
		  if((NplusB_on[ii][jj]+NminusB_on[ii][jj])!=0) asym_on = (NplusB_on[ii][jj]/NQplus_on[ii]-NminusB_on[ii][jj]/NQminus_on[ii])/(NplusB_on[ii][jj]/NQplus_on[ii]+NminusB_on[ii][jj]/NQminus_on[ii]);
		  if((NplusB_off[ii][jj]+NminusB_off[ii][jj])!=0) asym_off = (NplusB_off[ii][jj]/NQplus_off[ii]-NminusB_off[ii][jj]/NQminus_off[ii])/(NplusB_off[ii][jj]/NQplus_off[ii]+NminusB_off[ii][jj]/NQminus_off[ii]);

		  gB_on[ii]->SetPoint(jj,jj,asym_on);
		  gB_off[ii]->SetPoint(jj,jj,asym_off);
		}
	 }

	 for(int ii=0;ii<8;ii++){
		gC_on[ii] = new TGraph();
		gC_off[ii] = new TGraph();
		
		for(int jj=0; jj<128; jj++){
		  Double_t asym_on=0, asym_off=0;
		  if((NplusC_on[ii][jj]+NminusC_on[ii][jj])!=0) asym_on = (NplusC_on[ii][jj]/NQplus_on[ii]-NminusC_on[ii][jj]/NQminus_on[ii])/(NplusC_on[ii][jj]/NQplus_on[ii]+NminusC_on[ii][jj]/NQminus_on[ii]);
		  if((NplusC_off[ii][jj]+NminusC_off[ii][jj])!=0) asym_off = (NplusC_off[ii][jj]/NQplus_off[ii]-NminusC_off[ii][jj]/NQminus_off[ii])/(NplusC_off[ii][jj]/NQplus_off[ii]+NminusC_off[ii][jj]/NQminus_off[ii]);

		  gC_on[ii]->SetPoint(jj,jj,asym_on);
		  gC_off[ii]->SetPoint(jj,jj,asym_off);
		}
	 }

	 for(int ii=0;ii<8;ii++){
		gD_on[ii] = new TGraph();
		gD_off[ii] = new TGraph();
		
		for(int jj=0; jj<128; jj++){
		  Double_t asym_on=0, asym_off=0;
		  if((NplusD_on[ii][jj]+NminusD_on[ii][jj])!=0) asym_on = (NplusD_on[ii][jj]/NQplus_on[ii]-NminusD_on[ii][jj]/NQminus_on[ii])/(NplusD_on[ii][jj]/NQplus_on[ii]+NminusD_on[ii][jj]/NQminus_on[ii]);
		  if((NplusD_off[ii][jj]+NminusD_off[ii][jj])!=0) asym_off = (NplusD_off[ii][jj]/NQplus_off[ii]-NminusD_off[ii][jj]/NQminus_off[ii])/(NplusD_off[ii][jj]/NQplus_off[ii]+NminusD_off[ii][jj]/NQminus_off[ii]);

		  gD_on[ii]->SetPoint(jj,jj,asym_on);
		  gD_off[ii]->SetPoint(jj,jj,asym_off);
		}
	 }

	 TCanvas *c1=new TCanvas("c1","c1",1200,1200);
	 c1->Divide(2,2);
	 c1->cd(1);
	 TMultiGraph *mg1 = new TMultiGraph();
   	 gA_on[0]->SetMarkerColor(2);	 
   	 gA_on[0]->SetMarkerStyle(8);	 
   	 gA_on[0]->SetMarkerSize(1.3);	 
   	 gA_off[0]->SetMarkerColor(4);	 
   	 gA_off[0]->SetMarkerStyle(8);	 
   	 gA_off[0]->SetMarkerSize(1.3);	 

	 mg1->Add(gA_on[0]);
	 mg1->Add(gA_off[0]);
	 mg1->Draw("AP");
	 mg1->SetTitle("plane A asymmetry;chan;asym");

	 auto leg1 = new TLegend(0.7,0.7,0.85,0.85);
	 leg1->AddEntry(gA_on[0],"laser on","P");
	 leg1->AddEntry(gA_off[0],"laser off","P");
	 leg1->Draw(); 

	 c1->cd(2);
	 TMultiGraph *mg2 = new TMultiGraph();
   	 gB_on[0]->SetMarkerColor(2);	 
   	 gB_on[0]->SetMarkerStyle(8);	 
   	 gB_on[0]->SetMarkerSize(1.3);	 
   	 gB_off[0]->SetMarkerColor(4);	 
   	 gB_off[0]->SetMarkerStyle(8);	 
   	 gB_off[0]->SetMarkerSize(1.3);	 

	 mg2->Add(gB_on[0]);
	 mg2->Add(gB_off[0]);
	 mg2->Draw("AP");
	 mg2->SetTitle("plane B asymmetry;chan;asym");

	 auto leg2 = new TLegend(0.7,0.7,0.85,0.85);
	 leg2->AddEntry(gB_on[0],"laser on","P");
	 leg2->AddEntry(gB_off[0],"laser off","P");
	 leg2->Draw(); 

	 c1->cd(3);
	 TMultiGraph *mg3 = new TMultiGraph();
   	 gC_on[0]->SetMarkerColor(2);	 
   	 gC_on[0]->SetMarkerStyle(8);	 
   	 gC_on[0]->SetMarkerSize(1.3);	 
   	 gC_off[0]->SetMarkerColor(4);	 
   	 gC_off[0]->SetMarkerStyle(8);	 
   	 gC_off[0]->SetMarkerSize(1.3);	 

	 mg3->Add(gC_on[0]);
	 mg3->Add(gC_off[0]);
	 mg3->Draw("AP");
	 mg3->SetTitle("plane C asymmetry;chan;asym");

	 auto leg3 = new TLegend(0.7,0.7,0.85,0.85);
	 leg3->AddEntry(gC_on[0],"laser on","P");
	 leg3->AddEntry(gC_off[0],"laser off","P");
	 leg3->Draw(); 

	 c1->cd(4);
	 TMultiGraph *mg4 = new TMultiGraph();
   	 gD_on[0]->SetMarkerColor(2);	 
   	 gD_on[0]->SetMarkerStyle(8);	 
   	 gD_on[0]->SetMarkerSize(1.3);	 
   	 gD_off[0]->SetMarkerColor(4);	 
   	 gD_off[0]->SetMarkerStyle(8);	 
   	 gD_off[0]->SetMarkerSize(1.3);	 

	 mg4->Add(gD_on[0]);
	 mg4->Add(gD_off[0]);
	 mg4->Draw("AP");
	 mg4->SetTitle("plane D asymmetry;chan;asym");

	 auto leg4 = new TLegend(0.7,0.7,0.85,0.85);
	 leg4->AddEntry(gD_on[0],"laser on","P");
	 leg4->AddEntry(gD_off[0],"laser off","P");
	 leg4->Draw(); 

	 TCanvas *c5=new TCanvas("c5","c5",1200,1200);
	 c5->Divide(2,4);
	 c5->cd(1);
	 mg1->Draw("AP");
	 mg1->SetTitle("plane A asymmetry (delay_win 8);chan;asym");
	 leg1->Draw();

	 c5->cd(2);
	 TMultiGraph *mg6 = new TMultiGraph();
   	 gA_on[1]->SetMarkerColor(2);	 
   	 gA_on[1]->SetMarkerStyle(8);	 
   	 gA_on[1]->SetMarkerSize(1.3);	 
   	 gA_off[1]->SetMarkerColor(4);	 
   	 gA_off[1]->SetMarkerStyle(8);	 
   	 gA_off[1]->SetMarkerSize(1.3);	 

	 mg6->Add(gA_on[1]);
	 mg6->Add(gA_off[1]);
	 mg6->Draw("AP");
	 mg6->SetTitle("plane A asymmetry (delay_win 7);chan;asym");

	 auto leg6 = new TLegend(0.7,0.7,0.85,0.85);
	 leg6->AddEntry(gA_on[1],"laser on","P");
	 leg6->AddEntry(gA_off[1],"laser off","P");
	 leg6->Draw(); 

	 c5->cd(3);
	 TMultiGraph *mg7 = new TMultiGraph();
   	 gA_on[2]->SetMarkerColor(2);	 
   	 gA_on[2]->SetMarkerStyle(8);	 
   	 gA_on[2]->SetMarkerSize(1.3);	 
   	 gA_off[2]->SetMarkerColor(4);	 
   	 gA_off[2]->SetMarkerStyle(8);	 
   	 gA_off[2]->SetMarkerSize(1.3);	 

	 mg7->Add(gA_on[2]);
	 mg7->Add(gA_off[2]);
	 mg7->Draw("AP");
	 mg7->SetTitle("plane A asymmetry (delay_win 6);chan;asym");

	 auto leg7 = new TLegend(0.7,0.7,0.85,0.85);
	 leg7->AddEntry(gA_on[2],"laser on","P");
	 leg7->AddEntry(gA_off[2],"laser off","P");
	 leg7->Draw(); 

	 c5->cd(4);
	 TMultiGraph *mg8 = new TMultiGraph();
   	 gA_on[3]->SetMarkerColor(2);	 
   	 gA_on[3]->SetMarkerStyle(8);	 
   	 gA_on[3]->SetMarkerSize(1.3);	 
   	 gA_off[3]->SetMarkerColor(4);	 
   	 gA_off[3]->SetMarkerStyle(8);	 
   	 gA_off[3]->SetMarkerSize(1.3);	 

	 mg8->Add(gA_on[3]);
	 mg8->Add(gA_off[3]);
	 mg8->Draw("AP");
	 mg8->SetTitle("plane A asymmetry (delay_win 5);chan;asym");

	 auto leg8 = new TLegend(0.7,0.7,0.85,0.85);
	 leg8->AddEntry(gA_on[3],"laser on","P");
	 leg8->AddEntry(gA_off[3],"laser off","P");
	 leg8->Draw(); 

	 c5->cd(5);
	 TMultiGraph *mg9 = new TMultiGraph();
   	 gA_on[4]->SetMarkerColor(2);	 
   	 gA_on[4]->SetMarkerStyle(8);	 
   	 gA_on[4]->SetMarkerSize(1.3);	 
   	 gA_off[4]->SetMarkerColor(4);	 
   	 gA_off[4]->SetMarkerStyle(8);	 
   	 gA_off[4]->SetMarkerSize(1.3);	 

	 mg9->Add(gA_on[4]);
	 mg9->Add(gA_off[4]);
	 mg9->Draw("AP");
	 mg9->SetTitle("plane A asymmetry (delay_win 4);chan;asym");

	 auto leg9 = new TLegend(0.7,0.7,0.85,0.85);
	 leg9->AddEntry(gA_on[4],"laser on","P");
	 leg9->AddEntry(gA_off[4],"laser off","P");
	 leg9->Draw(); 

	 c5->cd(6);
	 TMultiGraph *mg10 = new TMultiGraph();
   	 gA_on[5]->SetMarkerColor(2);	 
   	 gA_on[5]->SetMarkerStyle(8);	 
   	 gA_on[5]->SetMarkerSize(1.3);	 
   	 gA_off[5]->SetMarkerColor(4);	 
   	 gA_off[5]->SetMarkerStyle(8);	 
   	 gA_off[5]->SetMarkerSize(1.3);	 

	 mg10->Add(gA_on[5]);
	 mg10->Add(gA_off[5]);
	 mg10->Draw("AP");
	 mg10->SetTitle("plane A asymmetry (delay_win 3);chan;asym");

	 auto leg10 = new TLegend(0.7,0.7,0.85,0.85);
	 leg10->AddEntry(gA_on[5],"laser on","P");
	 leg10->AddEntry(gA_off[5],"laser off","P");
	 leg10->Draw(); 

	 c5->cd(7);
	 TMultiGraph *mg11 = new TMultiGraph();
   	 gA_on[6]->SetMarkerColor(2);	 
   	 gA_on[6]->SetMarkerStyle(8);	 
   	 gA_on[6]->SetMarkerSize(1.3);	 
   	 gA_off[6]->SetMarkerColor(4);	 
   	 gA_off[6]->SetMarkerStyle(8);	 
   	 gA_off[6]->SetMarkerSize(1.3);	 

	 mg11->Add(gA_on[6]);
	 mg11->Add(gA_off[6]);
	 mg11->Draw("AP");
	 mg11->SetTitle("plane A asymmetry (delay_win 2);chan;asym");

	 auto leg11 = new TLegend(0.7,0.7,0.85,0.85);
	 leg11->AddEntry(gA_on[6],"laser on","P");
	 leg11->AddEntry(gA_off[6],"laser off","P");
	 leg11->Draw(); 

	 c5->cd(8);
	 TMultiGraph *mg12 = new TMultiGraph();
   	 gA_on[7]->SetMarkerColor(2);	 
   	 gA_on[7]->SetMarkerStyle(8);	 
   	 gA_on[7]->SetMarkerSize(1.3);	 
   	 gA_off[7]->SetMarkerColor(4);	 
   	 gA_off[7]->SetMarkerStyle(8);	 
   	 gA_off[7]->SetMarkerSize(1.3);	 

	 mg12->Add(gA_on[7]);
	 mg12->Add(gA_off[7]);
	 mg12->Draw("AP");
	 mg12->SetTitle("plane A asymmetry (delay_win 1);chan;asym");

	 auto leg12 = new TLegend(0.7,0.7,0.85,0.85);
	 leg12->AddEntry(gA_on[7],"laser on","P");
	 leg12->AddEntry(gA_off[7],"laser off","P");
	 leg12->Draw(); 

/*
     c1->Print(Form("Asymmetry_%d.pdf[",runnumber));
     c1->Print(Form("Asymmetry_%d.pdf",runnumber));
     c5->Print(Form("Asymmetry_%d.pdf",runnumber));
     c5->Print(Form("Asymmetry_%d.pdf]",runnumber));
*/
}

