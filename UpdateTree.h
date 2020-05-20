void UpdateTree(TString rootfile){
     TFile *f1 = new TFile(rootfile,"UPDATE");
	 TTree *T= (TTree *)f1->Get("T");
	 TTree *VTP = (TTree *)f1->Get("VTP");
	 TTree *VTPScal= (TTree *)f1->Get("VTPScal");

	 // add new branch
	 auto newbranch1 = T->Branch("curBCM",&curBCM,"curBCM/I");  
	 auto newbranch2 = T->Branch("curCavPower",&curCavPower,"curCavPower/I");  

	 T->AddFriend(VTP);

	 Int_t this_hel_win_cnt,thisBCM,thisCavPower;

	 //get vtp scaler BCM and CavPower
	 T->SetBranchAddress("hel_win_cnt",&this_hel_win_cnt);
	 VTPScal->SetBranchAddress("vtp_BCM",&thisBCM);
	 VTPScal->SetBranchAddress("vtp_CavPower",&thisCavPower);

	 Int_t nentries = T->GetEntries();

     for(int ii=0;ii<nentries;ii++){
		T->GetEntry(ii);
	    VTPScal->GetEntry(this_hel_win_cnt);	
		curBCM = thisBCM;
		curCavPower = thisCavPower;
		newbranch1->Fill();
		newbranch2->Fill();
	 }
	 T->Write("", TObject::kOverwrite);
	 f1->Close();

}
