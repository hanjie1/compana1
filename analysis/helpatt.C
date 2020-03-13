/*******  helicity pattern from VTP ********/

void helpatt()
{
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber);

     TFile *f0 = new TFile(filename);
     if(! (f0->IsOpen()) ) return;
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTP = (TTree*) f0->Get("VTP");

	 T->AddFriend(VTP);	

     Int_t past_hel[6];
	 Int_t helicity,last_mps_time,hel_win_cnt,mps;

     T->SetBranchAddress("vtp_past_hel",past_hel);
	 T->SetBranchAddress("hel_win_cnt",&hel_win_cnt);
	 T->SetBranchAddress("last_mps_time",&last_mps_time);
	 T->SetBranchAddress("helicity",&helicity);
	 T->SetBranchAddress("MPS",&mps);

	 Int_t nentries = T->GetEntries();
	 Int_t maxevent = 0;
	 cout<<"How many events?  ";
	 cin>>maxevent;     

	 TGraph *gpasthel[3];
	 TGraph *ghel = new TGraph();
	 TGraph *gmps = new TGraph();
	 TGraph *gmps_las_time = new TGraph();

	 Int_t helbit[173]={-1};
	 int bit_num = 172;
     for(int ii=0; ii<maxevent; ii++){
		T->GetEntry(ii);
	
		gpasthel[ii] = new TGraph();
	    for(int jj=0; jj<6; jj++){
			int nbits = 30;
			if(jj==0) nbits = 23;
			for(int nnbit=0; nnbit<nbits; nnbit++)
		 	 {  
			    helbit[bit_num] = ( past_hel[jj]>>nnbit ) & 0x1;
				bit_num = bit_num-1;
			}
		}	
 		
		for(int jj=0; jj<173; jj++)
		 //gpasthel[ii]->SetPoint(ii*173+jj,ii*173+jj,helbit[jj]);
		 gpasthel[ii]->SetPoint(jj,jj,helbit[jj]);
     } 

	TCanvas *c1 = new TCanvas("c1");
	TMultiGraph *mg1 = new TMultiGraph();
	int color[3]={2,3,7};
	for(int ii=0; ii<3; ii++){
		gpasthel[ii]->SetFillColorAlpha(color[ii],0.5);
		mg1->Add(gpasthel[ii]);
	}
    mg1->Draw("AB1"); 

	TCanvas *c2 = new TCanvas("c2");
    c2->Divide(2,2);
	c2->cd(1);
	gpasthel[0]->Draw("AB1"); 
	c2->cd(2);
	gpasthel[1]->Draw("AB1"); 
	c2->cd(3);
	gpasthel[2]->Draw("AB1"); 

}
