void plot(){
     TFile *f1=new TFile("Rootfiles/eDet_944.root");
	 TTree *T = (TTree*) f1->Get("T");

	 TH1F *h1 = new TH1F("h1","clock",100,0,100000);
	 TCanvas *c1 = new TCanvas("c1","c1",1500,1500);
	// T->Draw("clock1>>h1");

	 Int_t clock;
	 T->SetBranchAddress("clock1",&clock);

	 Int_t nentries = T->GetEntries();

	 for(int ii=0;ii<10;ii++){
		T->GetEntry(ii);
		h1->Fill(clock);
	 }
	 h1->Draw();
}
