#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>
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
#include "TIDecode.h"
#include "VETROCDecode.h"
#include "VTPDecode.h"

using namespace std;

// global parameters (eg. maxroc, max fadc channels ...) are set in SetParams.h
// tree variables are set in SetTreeVars.h

unsigned int LSWAP(unsigned int data);
void ClearTreeVar();
bool verbose = false; 
Int_t scaldat[32]={0};

int main ()
{
  int run_number=0;
  int nROC,  indx, bt, dt, blk, handle, nevents, status, nWords, version;
  int pe;
  unsigned int blocklevel = 1;
  Int_t tbLen,rocLen[MAX_ROCS], rocID[MAX_ROCS];
  uint32_t *buf, dLen, bufLen;
  char *dictionary = NULL;
  bool eventbyevent = true; 
  int maxevents = 1e9;
  Int_t totalmax=1000;
  bool firstevent = true;
  int check;
  bool totaldone = false;
  int vet_slotid[4]={EPLANEA_SLOT,EPLANEB_SLOT,EPLANEC_SLOT,EPLANED_SLOT};
   

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
  T->Branch("fadc_mode", &fadc_mode, "fadc_mode/I"); 
  T->Branch("fadc_a", fadc_int, Form("fadc_int[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_t", fadc_time, Form("fadc_time[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_a1", fadc_int_1, Form("fadc_int_1[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_t1", fadc_time_1, Form("fadc_time_1[%i]/I",FADC_NCHAN)); 
  T->Branch("fadc_nhit", fadc_nhit, Form("fadc_nhit[%i]/I",FADC_NCHAN)); 
  T->Branch("eplaneA_nhits", &eplaneA_nhits, "eplaneA_nhits/I");
  T->Branch("eplaneA_chan", eplaneA_chan, "eplaneA_chan[eplaneA_nhits]/I"); 
  T->Branch("eplaneA_rt", eplaneA_rt, "eplaneA_rt[eplaneA_nhits]/I"); 
  T->Branch("eplaneA_ft", eplaneA_ft, "eplaneA_ft[eplaneA_nhits]/I"); 
  T->Branch("eplaneB_nhits", &eplaneB_nhits, "eplaneB_nhits/I");
  T->Branch("eplaneB_chan", eplaneB_chan, "eplaneB_chan[eplaneB_nhits]/I"); 
  T->Branch("eplaneB_rt", eplaneB_rt, "eplaneB_rt[eplaneB_nhits]/I"); 
  T->Branch("eplaneB_ft", eplaneB_ft, "eplaneB_ft[eplaneB_nhits]/I"); 
  T->Branch("eplaneC_nhits", &eplaneC_nhits, "eplaneC_nhits/I");
  T->Branch("eplaneC_chan", eplaneC_chan, "eplaneC_chan[eplaneC_nhits]/I"); 
  T->Branch("eplaneC_rt", eplaneC_rt, "eplaneC_rt[eplaneC_nhits]/I"); 
  T->Branch("eplaneC_ft", eplaneC_ft, "eplaneC_ft[eplaneC_nhits]/I"); 
  T->Branch("eplaneD_nhits", &eplaneD_nhits, "eplaneD_nhits/I");
  T->Branch("eplaneD_chan", eplaneD_chan, "eplaneD_chan[eplaneD_nhits]/I"); 
  T->Branch("eplaneD_rt", eplaneD_rt, "eplaneD_rt[eplaneD_nhits]/I"); 
  T->Branch("eplaneD_ft", eplaneD_ft, "eplaneD_ft[eplaneD_nhits]/I"); 
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


  /* Open file  */
  char datapath[100];
  sprintf(datapath,"/home/compton/data2/vtpCompton_%d.dat.0",run_number);

  if ( (status = evOpen(datapath, (char*)"r",  &handle)) < 0) 
  {
    printf("Unable to open file %s status = %d\n",datapath,status);
    exit(-1);
  } 

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
  nevents=1;
  while ((status = evReadAlloc(handle, &buf, &bufLen))==0) 
    //while ((evReadAlloc(handle, &buf, &bufLen))!= EOF && (evReadAlloc(handle, &buf, &bufLen))==0) 
  { /* read the event and allocate the correct size buffer */
    indx=0; pe=0;
    nWords = buf[0] + 1;
    bt  = ((buf[1]&0xffff0000)>>16);  /* Bank Tag */
    dt  = ((buf[1]&0xff00)>>8);       /* Data Type */
    blk = buf[1]&0xff;                /* Event Block size */

    if(verbose) printf("    BLOCK #%d,  Bank tag = 0x%04x, Data type = 0x%04x,  Total len = %d words\n", nevents, bt, dt, nWords);

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
	  int fevtNum = simpTrigBuf1[0];
	  if(fevtNum != nevents) printf("The event number from TI does not match the counter !\n");

	  if(verbose)printf("Event number for the first event in the block = %d \n",fevtNum);

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


	  /**  loop blocks **/
      for(int ii=0;ii<blocklevel;ii++){ 
	      ClearTreeVar();
	      if(firstevent)fadc_mode=-1;

	      if(nevents>totalmax && totalmax != 1){
			 totaldone = true;
			 break;
		  }
          if((long long)nevents%10000==0) printf("  Event number = %llu **\n",(long long)nevents);
          
		  evtype = simpTrigBuf2[ii];
		  unsigned int tmpdata;
	      tmpdata = simpTrigRocBuf1[ii*3+2];	  
		  if((tmpdata & 0xffff0000)== 0xda560000){
			tHelicity = (tmpdata & 0x20)>>5;
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
		  }

          T->Fill();
		  VTP->Fill();
          nevents++;
	  } // loop over block levels

	  /**  scaler data decode **/
/*	  unsigned int *simpScalBuf = NULL;
	  int simpScalLen=0;
      simpScalLen = simpleGetSlotEventData(TI_ROC, SCALER_BANK, SCALER_SLOT, ii, &simpDataBuf);
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

	  for(int ii=0;ii<blocklevel;ii++){
	
	  }
*/
      if(totaldone) break;


/*
		if(rocID[nROC]==TI_ROC){  //TI ROC have FADC, VETROC, SCALER
		  while(nnWd<rocLen[nROC]){

			int tmplen = buf[indx+nnWd]+1;   //bank length
		    if(verbose)printf("bank len = %d (data = 0x%x);\n",tmplen,buf[indx+nnWd]);
	        nnWd++;
			int tmpBank = (buf[indx+nnWd]&0xFFF0000)>>16; // bank tag
		    if(verbose)printf("bank tag = %d (data = 0x%x)\n",tmpBank,buf[indx+nnWd]);
			nnWd++;


			if(tmpBank == SCALER_BANK){
			  int scaler_nwds = tmplen-2;
		      if(scaler_nwds%32!=0)
				printf("Scaler Warning:  event %d could have missing channels ! scaler channel %d\n",nevents, scaler_nwds);
			  else{
				if(scaler_nwds/32==1){
			      for(int kk=0;kk<tmplen-2;kk++){
				     scaldat[kk]=buf[indx+nnWd+kk];	
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
				else{
				  if(scaler_nwds/32>1)
				    printf("Scaler Warning: event %d has multiple scaler events (%d)! so far only 1 event is read out\n",nevents,scaler_nwds/32);
				}
			  }
			  nnWd += tmplen-2; 

			} //SCALER bank
		  } // loop one ROC data
		}  // TI ROC

		indx += rocLen[nROC];
		nROC++;
	  } // loop ROCs

      T->Fill();
*/	  if(firstevent) firstevent = false;
    }

    /* free the event buffer and wait for next one */
    free(buf);

    //uncomment the following to lines to view event by event
    if(eventbyevent) {
      printf("Hit return for next event or q to exit; hit a or A to replay all events or certain number of events.\n");
      int typein = getchar(); 
      if(typein == 113) break; 
      if(typein == 65 || typein == 97){
		eventbyevent=false;
	    cout<<"How many events? (hit 1 for total;)";
		cin>>totalmax;
	  }
    }

    if(nevents > maxevents) {
      printf("Completed %d events!\n", nevents-1); 
      break; 
    }

  } // End of loop through event blocks

  if ( status == EOF ) 
  {
    printf("Found end-of-file; total %d events. \n", nevents);
  }
  else if(status != 0)
  {
    printf("Error reading file (status = %d, quit\n",status);
  }


  T->Write(); 
  E->Write(); 
  VTP->Write(); 
  hfile->Close(); 
  evClose(handle);

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

	 memset(fadc_int, 0, FADC_NCHAN*sizeof(fadc_int[0]));
	 memset(fadc_time, 0, FADC_NCHAN*sizeof(fadc_time[0]));
	 memset(fadc_int_1, 0, FADC_NCHAN*sizeof(fadc_int_1[0]));
	 memset(fadc_time_1, 0, FADC_NCHAN*sizeof(fadc_time_1[0]));
	 memset(fadc_nhit, 0, FADC_NCHAN*sizeof(fadc_nhit[0]));
	 memset(ftdc_nhit, 0, FADC_NCHAN*sizeof(ftdc_nhit[0]));
     memset(frawdata, 0, FADC_NCHAN*MAXRAW*sizeof(frawdata[0][0]));	
	 nrawdata=0;
	 
	 eplaneA_nhits=0;
	 memset(eplaneA_chan, 0, VETROC_MAXHIT*sizeof(eplaneA_chan[0]));
	 memset(eplaneA_rt, 0, VETROC_MAXHIT*sizeof(eplaneA_rt[0]));
	 memset(eplaneA_ft, 0, VETROC_MAXHIT*sizeof(eplaneA_ft[0]));

	 eplaneB_nhits=0;
	 memset(eplaneB_chan, 0, VETROC_MAXHIT*sizeof(eplaneB_chan[0]));
	 memset(eplaneB_rt, 0, VETROC_MAXHIT*sizeof(eplaneB_rt[0]));
	 memset(eplaneB_ft, 0, VETROC_MAXHIT*sizeof(eplaneB_ft[0]));

	 eplaneC_nhits=0;
	 memset(eplaneC_chan, 0, VETROC_MAXHIT*sizeof(eplaneC_chan[0]));
	 memset(eplaneC_rt, 0, VETROC_MAXHIT*sizeof(eplaneC_rt[0]));
	 memset(eplaneC_ft, 0, VETROC_MAXHIT*sizeof(eplaneC_ft[0]));

	 eplaneD_nhits=0;
	 memset(eplaneD_chan, 0, VETROC_MAXHIT*sizeof(eplaneD_chan[0]));
	 memset(eplaneD_rt, 0, VETROC_MAXHIT*sizeof(eplaneD_rt[0]));
	 memset(eplaneD_ft, 0, VETROC_MAXHIT*sizeof(eplaneD_ft[0]));

     clock1 = 0;
     CavPower = 0;
     BCM = 0;

	 memset(scaldat, 0, 32*sizeof(scaldat[0]));

	 ClearVTP();  // clear some vtp_data 
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

}
