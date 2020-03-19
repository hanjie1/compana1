void CheckPredictHel(){
	 int runnumber;
	 cout<<"which run? ";
	 cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber);

     TFile *f0 = new TFile(filename);
     if(! (f0->IsOpen()) ) return;
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTP = (TTree*) f0->Get("VTP");

     T->AddFriend(VTP); 

     Int_t ti_helicity,cur_helicity;
	 Int_t mps, last_mps_time;

     T->SetBranchAddress("helicity",&ti_helicity);
     T->SetBranchAddress("current_helicity",&cur_helicity);
     T->SetBranchAddress("MPS",&mps);
     T->SetBranchAddress("last_mps_time",&last_mps_time);

     T->Draw(">>helpat","MPS==1 && last_mps_time>2083100");  // when ti helicity update
     TEventList *helpat;
     gDirectory->GetObject("helpat",helpat);
     T->SetEventList(helpat);

     Int_t nentries = helpat->GetN();
	 cout<<"Get "<<nentries<<" new helicities"<<endl;


     Int_t maxevent = 0;
     cout<<"How many events? (0: total)  ";
     cin>>maxevent;    
	 if(maxevent==0 || maxevent>nentries )maxevent = nentries;

	 int curhel[8]={0};

	 int ndiff = 0;
	 for(int ii=0; ii<maxevent; ii++){
		T->GetEntry(helpat->GetEntry(ii));

		if(ii>7){
		   int tmpdif;
		   tmpdif = ti_helicity - curhel[ii%8];
		   if(tmpdif!=0)ndiff++;
		}
		curhel[ii%8] = cur_helicity;


	 }

	 printf("If assuming 0 deadtime, %d helicities are different !\n",ndiff);

}
