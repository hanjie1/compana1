#include "../SetParams.h"
void plotVetroc()
{
     int runnumber=0;
     cout<<"Which run ?  ";
     cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *VTP = (TTree*) f0->Get("VTP");
     TTree *T = (TTree*) f0->Get("T");
     T->AddFriend(VTP);

     Int_t nentries=0;
     nentries = T->GetEntries();
     cout<<"Total entries:  "<<nentries<<endl;
     Int_t maxevent=0;
     cout<<"How many events ? (0 for total) ";
     cin>>maxevent;

     if(maxevent==0)maxevent=nentries;

	 Int_t planeA_nhits=0;
	 Int_t planeA_chan[VETROC_MAXHIT],planeA_rt[VETROC_MAXHIT],planeA_scalcnt[128];
	 Int_t current_helicity, BCM, CavPower;
	 Int_t hel_win_cnt, hel_win_cnt_1;

     T->SetBranchAddress("eplaneA_chan",planeA_chan);
     T->SetBranchAddress("eplaneA_rt",planeA_rt);
     T->SetBranchAddress("eplaneA_scalcnt",planeA_scalcnt);
     T->SetBranchAddress("current_helicity",&current_helicity);
	 
	 


}
