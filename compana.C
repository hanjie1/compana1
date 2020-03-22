#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>
#include <fstream>
#include "evio.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TBranch.h"
#include "TH1.h"
#include "simpleLib.h"
#include "SetParams.h"
#include "SetTreeVars.h"
#include "Fadc250Decode.h"
#include "VETROCDecode.h"
#include "VTPDecode.h"
#include "FindHelicity.h"

using namespace std;

// global parameters (eg. maxroc, max fadc channels ...) are set in SetParams.h
// tree variables are set in SetTreeVars.h

unsigned int LSWAP(unsigned int data);
void ClearScaler();
void ClearTreeVar();
bool verbose = false; 
Int_t scaldat[32]={0};

int main ()
{
  int run_number=0;
  int indx, bt, dt, blk, handle, status, nWords, version;
  ULong64_t nevents;
  int pe;
  unsigned int blocklevel = 1;
  uint32_t *buf, dLen, bufLen;
  char *dictionary = NULL;
  bool eventbyevent = true; 
  ULong64_t maxevents = 5e9;
  ULong64_t totalmax=1000;
  bool firstevent = true;
  bool findquad = false;
  int check;
  bool totaldone = false;
  bool has_scaler = false;
  int vet_slotid[4]={EPLANEA_SLOT,EPLANEB_SLOT,EPLANEC_SLOT,EPLANED_SLOT};
  int ndatafile = 0;
  Int_t pre_helicity = 0;
  int helpos = 0;
  Int_t pre_vtp_past_hel[7]={0};    // save the vtp_past_hel in the previous event  
  Int_t pre_last_mps_time=0;         // save the last_mps_time in the previous event  
  Int_t pre_hel_win_cnt_1 = 0;
   

  cout<<"Which run? ";
  cin>>run_number;
  // Initialize root and output 
  TString outfile=Form("Rootfiles/eDet_%d.root",run_number);

  TFile *hfile = new TFile(outfile,"RECREATE","e detector data");
  //if(!hfile->IsOpen()) return;  
  //Build TTree to store TDC data
  TTree *T = new TTree("T","data");
  T->Branch("evtype", &evtype, "evtype/I"); 
  T->Branch("helicity", &tHelicity, "tHelicity/I"); 
  T->Branch("MPS", &tMPS, "tMPS/I"); 
  T->Branch("ti_timpestamp", &ti_timestamp, "ti_timestamp/l"); 
  T->Branch("fadc_trigtime", &fadc_trigtime, "fadc_trigtime/l"); 
  T->Branch("fadc_mode", &fadc_mode, "fadc_mode/I"); 
  T->Branch("fadc_a", fadc_int, Form("fadc_int[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_t", fadc_time, Form("fadc_time[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_a1", fadc_int_1, Form("fadc_int_1[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_t1", fadc_time_1, Form("fadc_time_1[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_nhit", fadc_nhit, Form("fadc_nhit[%i]/I",FADC_NCHAN)); 

  T->Branch("eplaneA_trigtime", &eplaneA_trigtime, "eplaneA_trigtime/l"); 
  T->Branch("eplaneA_nhits", &eplaneA_nhits, "eplaneA_nhits/I");
  T->Branch("eplaneA_nhits_1", &eplaneA_nhits_1, "eplaneA_nhits_1/I");
  T->Branch("eplaneA_chan", eplaneA_chan, "eplaneA_chan[eplaneA_nhits]/I"); 
  T->Branch("eplaneA_rt", eplaneA_rt, "eplaneA_rt[eplaneA_nhits]/I"); 
  T->Branch("eplaneA_ft", eplaneA_ft, "eplaneA_ft[eplaneA_nhits_1]/I"); 
  T->Branch("eplaneA_ft_chan", eplaneA_ft_chan, "eplaneA_ft_chan[eplaneA_nhits_1]/I"); 
  T->Branch("eplaneB_trigtime", &eplaneB_trigtime, "eplaneB_trigtime/l"); 
  T->Branch("eplaneB_nhits", &eplaneB_nhits, "eplaneB_nhits/I");
  T->Branch("eplaneB_nhits_1", &eplaneB_nhits_1, "eplaneB_nhits_1/I");
  T->Branch("eplaneB_chan", eplaneB_chan, "eplaneB_chan[eplaneB_nhits]/I"); 
  T->Branch("eplaneB_rt", eplaneB_rt, "eplaneB_rt[eplaneB_nhits]/I"); 
  T->Branch("eplaneB_ft", eplaneB_ft, "eplaneB_ft[eplaneB_nhits_1]/I"); 
  T->Branch("eplaneB_ft_chan", eplaneB_ft_chan, "eplaneB_ft_chan[eplaneB_nhits_1]/I"); 
  T->Branch("eplaneC_trigtime", &eplaneC_trigtime, "eplaneC_trigtime/l"); 
  T->Branch("eplaneC_nhits", &eplaneC_nhits, "eplaneC_nhits/I");
  T->Branch("eplaneC_nhits_1", &eplaneC_nhits_1, "eplaneC_nhits_1/I");
  T->Branch("eplaneC_chan", eplaneC_chan, "eplaneC_chan[eplaneC_nhits]/I"); 
  T->Branch("eplaneC_rt", eplaneC_rt, "eplaneC_rt[eplaneC_nhits]/I"); 
  T->Branch("eplaneC_ft", eplaneC_ft, "eplaneC_ft[eplaneC_nhits_1]/I"); 
  T->Branch("eplaneC_ft_chan", eplaneC_ft_chan, "eplaneC_ft_chan[eplaneC_nhits_1]/I"); 
  T->Branch("eplaneD_trigtime", &eplaneD_trigtime, "eplaneD_trigtime/l"); 
  T->Branch("eplaneD_nhits", &eplaneD_nhits, "eplaneD_nhits/I");
  T->Branch("eplaneD_nhits_1", &eplaneD_nhits_1, "eplaneD_nhits_1/I");
  T->Branch("eplaneD_chan", eplaneD_chan, "eplaneD_chan[eplaneD_nhits]/I"); 
  T->Branch("eplaneD_rt", eplaneD_rt, "eplaneD_rt[eplaneD_nhits]/I"); 
  T->Branch("eplaneD_ft", eplaneD_ft, "eplaneD_ft[eplaneD_nhits_1]/I"); 
  T->Branch("eplaneD_ft_chan", eplaneD_ft_chan, "eplaneD_ft_chan[eplaneD_nhits_1]/I"); 
  T->Branch("clock1", &clock1, "clock1/I");
  T->Branch("CavPower", &CavPower, "CavPower/I");
  T->Branch("BCM", &BCM, "BCM/I");
  T->Branch("BPM2AX_p", &BPM2AX_p, "BPM2AX_p/I");
  T->Branch("BPM2AX_m", &BPM2AX_m, "BPM2AX_m/I");
  T->Branch("BPM2AY_p", &BPM2AY_p, "BPM2AY_p/I");
  T->Branch("BPM2AY_m", &BPM2AY_m, "BPM2AY_m/I");
  T->Branch("BPM2BX_p", &BPM2BX_p, "BPM2BX_p/I");
  T->Branch("BPM2BX_m", &BPM2BX_m, "BPM2BX_m/I");
  T->Branch("BPM2BY_p", &BPM2BY_p, "BPM2BY_p/I");
  T->Branch("BPM2BY_m", &BPM2BY_m, "BPM2BY_m/I");

  TTree *E = new TTree("E","scaler data");
  E->Branch("scaldat", scaldat, "scaldat[32]/I"); 

  TTree *VTP = new TTree("VTP","vtp data");
  VTP->Branch("vtp_trigtime", &vtp_trigtime, "vtp_trigtime/l"); 
  VTP->Branch("eplaneA_scalcnt",vtp_A_scalcnt,Form("vtp_A_scalcnt[%d]/I",VETROC_NCHAN));
  VTP->Branch("eplaneB_scalcnt",vtp_B_scalcnt,Form("vtp_B_scalcnt[%d]/I",VETROC_NCHAN));
  VTP->Branch("eplaneC_scalcnt",vtp_C_scalcnt,Form("vtp_C_scalcnt[%d]/I",VETROC_NCHAN));
  VTP->Branch("eplaneD_scalcnt",vtp_D_scalcnt,Form("vtp_D_scalcnt[%d]/I",VETROC_NCHAN));
  VTP->Branch("scaldat",vtp_scaldat,"vtp_scaldat[16]/I");
  VTP->Branch("busytime",&busytime,"busytime/I");
  VTP->Branch("livetime",&livetime,"livetime/I");
  VTP->Branch("hel_win_cnt_1",&hel_win_cnt_1,"hel_win_cnt_1/I");
  VTP->Branch("trigcnt",trigcnt,"trigcnt[5]/I");
  VTP->Branch("pattern_num",&pattern_num,"pattern_num/I");
  VTP->Branch("trig_pattern",trig_pattern,"trig_pattern[pattern_num]/I");
  VTP->Branch("trig_pattern_time",trig_pattern_time,"trig_pattern_time[pattern_num]/I");
  VTP->Branch("vtp_past_hel",vtp_past_hel,"vtp_past_hel[6]/I");
  VTP->Branch("last_mps_time",&last_mps_time,"last_mps_time/I");
  VTP->Branch("hel_win_cnt",&hel_win_cnt,"hel_win_cnt/I");
  VTP->Branch("current_helicity", &current_helicity, "current_helicity/I"); 
  VTP->Branch("vtp_helicity", &vtp_helicity, "vtp_helicity/I"); 

  TTree *VTPScal = new TTree("VTPScal","vtp scaler tree for each helicity window");
  VTPScal->Branch("vtp_BCM",&vtp_BCM,"vtp_BCM/I");
  VTPScal->Branch("vtp_CavPower",&vtp_CavPower,"vtp_CavPower/I");


  nevents=1;
  /* Open file  */
  while(ndatafile<20){ // loop all data files
  char datapath[100];
  sprintf(datapath,"/home/compton/data2/vtpCompton_%d.dat.%d",run_number,ndatafile);

  ifstream infile(datapath);
  if(!infile){
	printf("Can't find file %s....stop here\n",datapath);
	break;
  }

  if ( (status = evOpen(datapath, (char*)"r",  &handle)) < 0) 
  {
    printf("Unable to open file %s status = %d\n",datapath,status);
    exit(-1);
  } 
  else
	printf("Open file /home/compton/data2/vtpCompton_%d.dat.%d\n",run_number,ndatafile);

  ndatafile++;

  /* Get evio version # of file */
  status = evIoctl(handle, (char*)"v", &version);
  if (status == S_SUCCESS) 
  {
    printf("Evio file version = %d\n\n", version);
  }

  /* Get a dictionary if there is one */
  status = evGetDictionary(handle, &dictionary, &dLen);
  if (status == S_SUCCESS && dictionary != NULL) 
  {
    printf("Dictionary =\n%s\n\n", dictionary);
    free(dictionary);
  }

  // Unblocking initialize
  // SIMPLE: initialize
  simpleInit();

  /* SIMPLE: Define banks For compton
   *  rocID     bankID     slot   what      endian
   *  1         3            3    fadc250   little
   *  1         4           13    vetroc    little
   *  1         4           14    vetroc    little
   *  1         4           15    vetroc    little
   *  1         4           16    vetroc    little
   *  1         6           ??    ??
   *  3         0x11        ??    ??
   *  3         0x12        ??    ??
   *  3         0x56        11    vtp       big
   *
   *
   *  int
   *  simpleConfigBank(int rocID, int bankID, int num,
   *                    int endian, int isBlocked, void *firstPassRoutine)
   *
   * @param rocID             roc ID
   * @param bankID            Bank ID
   * @param num               NOT USED
   * @param endian            little = 0, big = 1
   * @param isBlocked         no = 0, yes = 1
   * @param firstPassRoutine  Routine to call for first pass processing
   */
  simpleConfigBank(1, 0x3, 0, 0, 1, NULL);
  simpleConfigBank(1, 0x4, 0, 0, 1, NULL);
  simpleConfigBank(1, 0x6, 0, 0, 0, NULL);
  simpleConfigBank(3, 0x56, 20, 1, 1, NULL);

  /* Loop through getting event blocks one at a time and print basic infomation
     about each block */
  while ((status = evReadAlloc(handle, &buf, &bufLen))==0) 
    //while ((evReadAlloc(handle, &buf, &bufLen))!= EOF && (evReadAlloc(handle, &buf, &bufLen))==0) 
  { /* read the event and allocate the correct size buffer */
    indx=0; pe=0;
    nWords = buf[0] + 1;
    bt  = ((buf[1]&0xffff0000)>>16);  /* Bank Tag */
    dt  = ((buf[1]&0xff00)>>8);       /* Data Type */
    blk = buf[1]&0xff;                /* Event Block size */

    if(verbose) printf("    BLOCK #%llu,  Bank tag = 0x%04x, Data type = 0x%04x,  Total len = %d words\n", nevents, bt, dt, nWords);

    /* Check on what type of event block this is */
    if((bt >= 0xff00)> 0) 
    {/* CODA Reserved bank type */
      switch (bt) {
        case 0xffd1:
          if(verbose) printf("    ** Prestart Event **\n");
          break;
        case 0xffd2:
          if(verbose) printf("    ** Go Event **\n");
          break;
        case 0xffd4:
          if(verbose) printf("    ** End Event **\n");
          break;
        case 0xff50:
        case 0xff70:
          if(verbose) printf("    ** Physics Event Block (%d events in Block) **\n",blk);
          pe=1;
          break;
        default:
          if(verbose) printf("    ** Undefined CODA Event Type **\n");
      }
    }
    else
    { /* User event type */
      printf("    ** User Event (Type = %d) **\n",bt);
    }

    if (pe == 0) 
    {
      indx += nWords;
    } 
    else 
    { /* This is a built Physics Event. Disect a bit more... */

	  /**  Scan data to find blocks and banks **/
	  simpleScan(buf, nWords);

      indx += 2;

      /**  Get trigger bank buf **/
      int tbLen1 = 0; // trigger bank time segment
	  int tbLen2 = 0; // trigger bank type segment
	  int tbLenROC[2] = {0}; // trigger bank ROC segment
   
	  unsigned long long *simpTrigBuf1 = NULL;
	  tbLen1 = simpleGetTriggerBankTimeSegment(&simpTrigBuf1);
	  ULong64_t fevtNum = simpTrigBuf1[0];
	  if(fevtNum != nevents) printf("The event number from TI does not match the counter !\n");

	  if(verbose)printf("Event number for the first event in the block = %llu \n",fevtNum);

	  unsigned short *simpTrigBuf2 = NULL;
	  tbLen2 = simpleGetTriggerBankTypeSegment(&simpTrigBuf2);

	  unsigned int *simpTrigRocBuf1 = NULL;
	  tbLenROC[0] = simpleGetTriggerBankRocSegment(TI_ROC,&simpTrigRocBuf1); // TI ROC

	  unsigned int *simpTrigRocBuf2 = NULL;
	  tbLenROC[1] = simpleGetTriggerBankRocSegment(VTP_ROC,&simpTrigRocBuf2); // VTP ROC

	  if(verbose)printf("time len = %d , type len = %d , roc1 len = %d , roc2 len = %d\n",tbLen1,tbLen2,tbLenROC[0],tbLenROC[1]);

	  int BLOCKLEVEL=1;
      check = simpleGetRocBlockLevel(TI_ROC, FADC_BANK, &BLOCKLEVEL);
	  blocklevel = BLOCKLEVEL;
      if(check == -1)printf("Couldn't find block level !\n");
	  if(verbose)printf("Block level = %d\n",blocklevel);

      unsigned int header = 0;
      /* FADC block header */
	  check = simpleGetSlotBlockHeader(TI_ROC, FADC_BANK, FADC_SLOT, &header);
      if(check <= 0) 
		printf("ERROR getting FADC block header\n");
	  else{
       faDataDecode(header);
	   if(verbose)printf("FADC evt blk = %d, num of evts = %d\n",fadc_data.blk_num, fadc_data.n_evts);
	   if(fadc_data.n_evts != blocklevel)
		 printf("ERROR fadc number of events %d is not equal to blocklevel %d !\n",fadc_data.n_evts, blocklevel);
      }
      /* VETROC block header */
      for(int nslot=0;nslot<4;nslot++){
	     check = simpleGetSlotBlockHeader(TI_ROC, VETROC_BANK, vet_slotid[nslot], &header);
         if(check <= 0) 
		   printf("ERROR getting VETROC slot 13 block header\n");
	     else{
	       vetDataDecode(header);
	     if(verbose)printf("VETROC evt blk = %d, num of evts = %d\n",vetroc_data.blk_num, vetroc_data.n_evts);
	     if(vetroc_data.n_evts != blocklevel)
	       printf("ERROR vetroc slot %d number of events %d is not equal to blocklevel %d !\n",vet_slotid[nslot],
			       vetroc_data.n_evts, blocklevel);
	     }
	  }

	  /** VTP block header **/
	  check = simpleGetSlotBlockHeader(VTP_ROC, VTP_BANK, VTP_SLOT, &header);
      if(check <= 0) 
		printf("ERROR getting VTP block header\n");
	  else{
		vtpDataDecode(LSWAP(header));
	    if(verbose)printf("VTP evt blk = %d, num of evts = %d\n",vetroc_data.blk_num, vetroc_data.n_evts);
	    if(vetroc_data.n_evts != blocklevel)
	      printf("VTP number of events %d is not equal to blocklevel %d !\n",vetroc_data.n_evts, blocklevel);
	  }

	  /** scaler data load **/
	  unsigned int *simpScalBuf = NULL;
	  int simpScalLen=0;
      simpScalLen = simpleGetRocBankData(TI_ROC, SCALER_BANK, &simpScalBuf);
	  int nscal=0;
	  if(simpScalLen > 0){
         if( simpScalLen % 32 ==0 ){
		   has_scaler = true;
		   nscal = simpScalLen/32;
		   if(nscal > BLOCKLEVEL)
		      printf("ERROR: number of events in scaler %d is bigger than block level %d *32 \n",simpScalLen,blocklevel);
		 }
		 else{
		   printf("ERROR: number of events in scaler %d is not equal to n*32 \n",simpScalLen);
		   has_scaler = false;
		 }
	  }
	  else has_scaler = false;

	  /**  loop blocks **/
      for(int ii=0;ii<BLOCKLEVEL;ii++){ 
	      ClearTreeVar();
	      if(firstevent)fadc_mode=-1;

	      if(nevents>totalmax && totalmax != 1){
			 totaldone = true;
			 break;
		  }
          if(nevents%100000==0) printf("  Event number = %llu **\n",nevents);
         
		  ti_timestamp = simpTrigBuf1[ii+1]; 
		  evtype = simpTrigBuf2[ii];
		  unsigned int tmpdata;
	      tmpdata = simpTrigRocBuf1[ii*3+2];	  
		  if((tmpdata & 0xffff0000)== 0xda560000){
			tHelicity = InvertBit((tmpdata & 0x20)>>5);    // the helicity is inverted when sent to DAQ
			tMPS = (tmpdata & 0x10)>>4;
		  }
		  else
			printf("Couldn't find helicity bits !!\n");

		  unsigned int *simpDataBuf = NULL;
		  int simpLen=0;
		  /** FADC event data **/
          simpLen = simpleGetSlotEventData(TI_ROC, FADC_BANK, FADC_SLOT, ii, &simpDataBuf);
		  if(simpLen <= 0)
			printf("ERROR fadc event data length %d <= 0 \n",simpLen);
	      else{
			for(int idata = 0; idata < simpLen; idata++)
			   faDataDecode(simpDataBuf[idata]);

			if(firstevent){
				 fadc_mode = GetFadcMode();
				 if(fadc_mode == RAW_MODE)
                    T->Branch("fadc_rawADC", frawdata, Form("frawdata[%i][%i]/I",FADC_NCHAN,MAXRAW)); 
			 }
		  }

		  /** VETROC event data **/
		  for(int nslot=0;nslot<4;nslot++){
			  simpLen = 0;
			  simpDataBuf = NULL;
			  simpLen = simpleGetSlotEventData(TI_ROC, VETROC_BANK, vet_slotid[nslot], ii, &simpDataBuf);
		      if(simpLen <= 0)
		     	printf("ERROR vetroc slot %d event data length %d <= 0 \n",vet_slotid[nslot],simpLen);
	          else{
		    	for(int idata = 0; idata < simpLen; idata++)
			      vetDataDecode(simpDataBuf[idata]);
		      }
		  }

		  /**  VTP event data **/
		  simpLen = 0;
		  simpDataBuf = NULL;
		  simpLen = simpleGetSlotEventData(VTP_ROC, VTP_BANK, VTP_SLOT, ii, &simpDataBuf);
		  if(simpLen <= 0)
			printf("ERROR vtp event data length %d <= 0 \n",simpLen);
	      else{
			for(int idata = 0; idata < simpLen; idata++){
			   unsigned int new_data;
			   new_data = LSWAP(simpDataBuf[idata]);
			   vtpDataDecode(new_data);
			}
			for(int mm=0;mm<6;mm++)
			   vtp_past_hel[mm] = vtp_data.helicity[mm];
	
			vtp_helicity = InvertBit((vtp_past_hel[0] & 0x1));   // most recent helicity seen by VTP
			int pred_bit = 0;   		      // predict bit in 8 windows
			if(firstevent){
			  findquad = FindQuad(vtp_past_hel, &helpos);   // find first quad, and initialize fgShreg and fgShreg_earlier, return helicity window position
			  helpos = helpos + 1;    //vtp_past_hel is one window behind the in-time helicity
			  if(findquad){
				for(int mm=0; mm<delay_win/4-1; mm++)pred_bit=ranBit(2,1);      // fgShreg is update 8 windows
				
				if( helpos<4 ) current_helicity = GetHelicity(&helpos);   // get the helicity for the current window and update "helpos"
				if( helpos==4 ){
				  current_helicity = ranBit(2,1);
				  helpos = 1;
				}
				if( helpos>4 ) printf("The helicity window position %d is bigger than 4 !!\n",helpos);
			  }
			}

			if(findquad && (firstevent == false)){
			   int updateWin = HelicityUpdateWin(pre_vtp_past_hel, vtp_past_hel, pre_last_mps_time, last_mps_time);
			   if(updateWin == 0) current_helicity = pre_helicity;         // helicity is not update
			   else current_helicity = PredictHelicity(updateWin, &helpos);
			   if(verbose) 
				 printf("event %llu  Update %d helicity windows\n",nevents,updateWin); 
			}
			pre_helicity = current_helicity;
 
			for(int mm=0;mm<6;mm++)
			   pre_vtp_past_hel[mm] = vtp_past_hel[mm];

			pre_last_mps_time = last_mps_time;
		  }

		  /** scaler data **/
		  if(has_scaler && ii<nscal){
			for(int nn=0; nn<32; nn++){
				scaldat[nn] = simpScalBuf[ii*32+nn];
			}
   		    clock1 = scaldat[1];
			BPM2AY_m = scaldat[2];
			BPM2AY_p = scaldat[3];
			BPM2AX_m = scaldat[4];
			BPM2AX_p = scaldat[5];
			BPM2BY_m = scaldat[6];
			BPM2BY_p = scaldat[7];
			BPM2BX_m = scaldat[8];
			BPM2BX_p = scaldat[9];
			CavPower = scaldat[12];
			BCM = scaldat[15];

			E->Fill();
		  }


          T->Fill();
		  VTP->Fill();
		
		  if(hel_win_cnt_1 != 0){
		    for(Int_t kk=pre_hel_win_cnt_1; kk<hel_win_cnt_1; kk++){
			   vtp_BCM = 0;
			   vtp_CavPower = 0;
			   VTPScal->Fill();
		    }
		    vtp_BCM = vtp_scaldat[15];
		    vtp_CavPower = vtp_scaldat[12];
		    VTPScal->Fill();
		    pre_hel_win_cnt_1 = hel_win_cnt_1+1;
		  }
          nevents++;

	      if(firstevent) firstevent = false;

          if(eventbyevent) {
      		printf("Hit return for next event or q to exit; hit a or A to replay all events or certain number of events.\n");
      		int typein = getchar(); 
      		if(typein == 113){ totaldone= true; break;} 
      		if(typein == 65 || typein == 97){
			  eventbyevent=false;
	    	  cout<<"How many events? (hit 1 for total;)";
			  cin>>totalmax;
	  		}
    	   }	

    	   if(nevents > maxevents) {
      		printf("Completed %llu events!\n", nevents-1); 
      		totaldone=true;
		    break;	
           }
	  } // loop over block levels


      if(totaldone) break;
    }

    /* free the event buffer and wait for next one */
    free(buf);

  } // End of loop one data file

  if(totaldone) break;
  if ( status == EOF ) 
  {
    printf("Found end-of-file; total %llu events. \n", nevents);
  }
  else if(status != 0)
  {
    printf("Error reading file (status = %d, quit\n",status);
	exit(-1);
  }
  evClose(handle);
  }  // loop all data files

  T->Write(); 
  E->Write(); 
  VTP->Write(); 
  VTPScal->Write(); 
  hfile->Close(); 
 // evClose(handle);

  exit(0);

} // End of main function

unsigned int LSWAP(unsigned int data){
	   unsigned int new_data;
	   new_data = ((data & 0x000000ff)<<24) |
		 		  ((data & 0x0000ff00)<<8)  |
				  ((data & 0x00ff0000)>>8)  |
				  ((data & 0xff000000)>>24) ;
	   return new_data;
}

void ClearTreeVar(){

     tHelicity=0;  
     tMPS=0;	   
     evtype=0;    
	 ti_timestamp = 0;

	 memset(fadc_int, 0, FADC_NCHAN*sizeof(fadc_int[0]));
	 memset(fadc_time, 0, FADC_NCHAN*sizeof(fadc_time[0]));
	 memset(fadc_int_1, 0, FADC_NCHAN*sizeof(fadc_int_1[0]));
	 memset(fadc_time_1, 0, FADC_NCHAN*sizeof(fadc_time_1[0]));
	 memset(fadc_nhit, 0, FADC_NCHAN*sizeof(fadc_nhit[0]));
	 memset(ftdc_nhit, 0, FADC_NCHAN*sizeof(ftdc_nhit[0]));
     memset(frawdata, 0, FADC_NCHAN*MAXRAW*sizeof(frawdata[0][0]));	
	 fadc_trigtime = 0;
	 nrawdata=0;
	 
	 eplaneA_trigtime = 0;
	 eplaneA_nhits=0;
	 eplaneA_nhits_1=0;
	 memset(eplaneA_chan, 0, VETROC_MAXHIT*sizeof(eplaneA_chan[0]));
	 memset(eplaneA_ft_chan, 0, VETROC_MAXHIT*sizeof(eplaneA_ft_chan[0]));
	 memset(eplaneA_rt, 0, VETROC_MAXHIT*sizeof(eplaneA_rt[0]));
	 memset(eplaneA_ft, 0, VETROC_MAXHIT*sizeof(eplaneA_ft[0]));

	 eplaneB_trigtime = 0;
	 eplaneB_nhits=0;
	 eplaneB_nhits_1=0;
	 memset(eplaneB_chan, 0, VETROC_MAXHIT*sizeof(eplaneB_chan[0]));
	 memset(eplaneB_ft_chan, 0, VETROC_MAXHIT*sizeof(eplaneB_ft_chan[0]));
	 memset(eplaneB_rt, 0, VETROC_MAXHIT*sizeof(eplaneB_rt[0]));
	 memset(eplaneB_ft, 0, VETROC_MAXHIT*sizeof(eplaneB_ft[0]));

	 eplaneC_trigtime = 0;
	 eplaneC_nhits=0;
	 eplaneC_nhits_1=0;
	 memset(eplaneC_chan, 0, VETROC_MAXHIT*sizeof(eplaneC_chan[0]));
	 memset(eplaneC_ft_chan, 0, VETROC_MAXHIT*sizeof(eplaneC_ft_chan[0]));
	 memset(eplaneC_rt, 0, VETROC_MAXHIT*sizeof(eplaneC_rt[0]));
	 memset(eplaneC_ft, 0, VETROC_MAXHIT*sizeof(eplaneC_ft[0]));

	 eplaneD_trigtime = 0;
	 eplaneD_nhits=0;
	 eplaneD_nhits_1=0;
	 memset(eplaneD_chan, 0, VETROC_MAXHIT*sizeof(eplaneD_chan[0]));
	 memset(eplaneD_ft_chan, 0, VETROC_MAXHIT*sizeof(eplaneD_ft_chan[0]));
	 memset(eplaneD_rt, 0, VETROC_MAXHIT*sizeof(eplaneD_rt[0]));
	 memset(eplaneD_ft, 0, VETROC_MAXHIT*sizeof(eplaneD_ft[0]));

     ClearScaler();

	 ClearVTP();  // clear some vtp_data 
	 vtp_trigtime = 0;
	 memset(vtp_A_scalcnt, 0, VETROC_NCHAN*sizeof(vtp_A_scalcnt[0]));
	 memset(vtp_B_scalcnt, 0, VETROC_NCHAN*sizeof(vtp_B_scalcnt[0]));
	 memset(vtp_C_scalcnt, 0, VETROC_NCHAN*sizeof(vtp_C_scalcnt[0]));
	 memset(vtp_D_scalcnt, 0, VETROC_NCHAN*sizeof(vtp_D_scalcnt[0]));
	 memset(vtp_scaldat, 0, 16*sizeof(vtp_scaldat[0]));
	 busytime = 0;
	 livetime = 0;
	 hel_win_cnt_1 = 0;
	 memset(trigcnt, 0, 5*sizeof(trigcnt[0]));
	 pattern_num = 0;
	 memset(trig_pattern, 0, 64*sizeof(trig_pattern[0]));
	 memset(trig_pattern_time, 0, 64*sizeof(trig_pattern_time[0]));
     last_mps_time = 0;
	 memset(vtp_past_hel, 0, 6*sizeof(vtp_past_hel[0]));
	 hel_win_cnt = 0;
	 current_helicity = 0;
	 vtp_helicity = 0;

}

void ClearScaler(){

     clock1 = 0;
     CavPower = 0;
     BCM = 0;

	BPM2AY_m = 0;
	BPM2AY_p = 0;
	BPM2AX_m = 0;
	BPM2AX_p = 0;
	BPM2BY_m = 0;
	BPM2BY_p = 0;
	BPM2BX_m = 0;
	BPM2BX_p = 0;

	 memset(scaldat, 0, 32*sizeof(scaldat[0]));

}
