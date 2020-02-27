void CheckScal(){
     int runnumber=0;
      cout<<"Which run ?  ";
      cin>>runnumber;

     TString filename = Form("/home/compton/hanjie/compana1/Rootfiles/eDet_%d.root", runnumber); 

     TFile *f0 = new TFile(filename);
     TTree *T = (TTree*) f0->Get("T");

     Int_t clock=0, CavPower=0, BPM2AX_p=0;

     T->SetBranchAddress("clock1",&clock);
     T->SetBranchAddress("CavPower",&CavPower);
     T->SetBranchAddress("BPM2AX_p",&BPM2AX_p);

     Int_t nentries=0;
     nentries = T->GetEntries();
     cout<<"Total entries:  "<<nentries<<endl;

     bool laser_on =false;
     int pre_evt=0;
     int laser_evt[20000000];
     for(int ii=0;ii<nentries;ii++){
          laser_evt[ii] = 0;
        }

     Int_t maxevent=nentries;

	 ofstream outfile;
	 outfile.open(Form("scal_%d.dat",runnumber));
     for(int ii=0;ii<maxevent;ii++){
          T->GetEntry(ii);
           if(clock!=0){
             Double_t cav_rate=CavPower*1.0/(clock*1.0/(40.0e6));

          if(cav_rate>14000){
             laser_on=true;
           }
          else laser_on=false;

          for(int kk=pre_evt;kk<=ii;kk++){
              if(laser_on)laser_evt[kk]=1;
              else laser_evt[kk]=0;
			  outfile<<kk<<"  "<<laser_evt[kk]<<endl;
          }
          pre_evt = ii+1;
        }
      }
      outfile.close();


}
