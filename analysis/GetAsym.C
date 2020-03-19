#include "../SetParams.h"
void GetAsym(){
     int runnumber;
     cout<<"which run? ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber);

     TFile *f0 = new TFile(filename);
     if(! (f0->IsOpen()) ) return;
     TTree *T = (TTree*) f0->Get("T");
     TTree *VTP = (TTree*) f0->Get("VTP");

     T->AddFriend(VTP); 

     Int_t cur_helicity,photon,photon_time;
	 Int_t fadc_a[FADC_NCHAN],fadc_t[FADC_NCHAN];

     T->SetBranchAddress("current_helicity",&cur_helicity);
     T->SetBranchAddress("fadc_a",&fadc_a);
     T->SetBranchAddress("fadc_t",&fadc_t);

     T->Draw(">>total_photon","fadc_t[0]*0.0625>4");  // when ti helicity update
     TEventList *total_photon;
     gDirectory->GetObject("total_photon",total_photon);
     T->SetEventList(total_photon);

     Int_t nentries = total_photon->GetN();

	 Int_t Nplus=0, Nminus=0;
	 for(int ii=0; ii<nentries; ii++){
	    T->GetEntry(total_photon->GetEntry(ii));
		photon = fadc_a[0];
		photon_time = fadc_t[0];
		if(photon>0){
		 if(cur_helicity==0) Nminus++;
		 if(cur_helicity==1) Nplus++;
		}
	 }

	 Double_t Asy = 0;
	 if( (Nplus+Nminus)!=0 ) Asy = 1.0*(Nplus-Nminus)/(Nplus*1.0+Nminus*1.0);
	 cout<<"Asym:  "<<Asy<<endl;
	 cout<<"Plus:  "<<Nplus<<"   Minus:  "<<Nminus<<endl;


}
