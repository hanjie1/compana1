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
#include "Fadc250Decode.h"
#include "TIDecode.h"

using namespace std;

#define MAX_BLOCK_SIZE 255
#define MAX_ROCS       2
#define VTP_ROC        3      //VTP ROC ID
#define TI_ROC         1      //Physics modules (FADC, VETROC, Scaler) ROC ID
#define FIRSTWORD_VETROC 0xb0b0b0b4
#define FIRSTWORD_FADC   0xb0b0b0b5
#define FIRSTWORD_SCALER 0xb0b0b0b6

#define NBANK		   3
#define FADC_BANK      3
#define VETROC_BANK    4
#define SCALER_BANK    6


void ClearTreeVar();

//global variables for tree
int tHelicity;  //TS6 bit, helicity level
int tMPS;	   //TS5 bit, TSettle level
int evtype;    // event type;
 /*** FADC tree variable ***/
int fadc_mode;                // FADC mode
Int_t fadc_int[16];	          // ADC integral for the first hit per channel
Int_t fadc_time[16]; 		  // pulse time for the first hit per channel
Int_t fadc_int_1[16];	      // ADC integral for the second hit per channel 
Int_t fadc_time_1[16]; 		  // pulse time for the second hit per channel
Int_t fadc_nhit[16];	      // number of hits per channel


bool verbose = false; 

int main (int argc, char **argv)
{

  int plen, i=0, nROC,  indx, bt, dt, blk, handle, nevents, status, nWords, version;
  int num;
  int pe, ptb, pr;
  Int_t tbLen, rocLen[MAX_ROCS], rocID[MAX_ROCS];
  Int_t rbankLen[NBANK],rbankTag[NBANK];
  uint32_t *buf, dLen, bufLen;
  char *dictionary = NULL;
  bool eventbyevent = true; 
  int maxevents = 1e9;
  Int_t totalmax=1000;

  if (argc < 2) 
  {
    printf("Incorrect number of arguments:\n");
    printf("  usage: %s <filename> [-t -r]\n", argv[0]);
    printf("  optional: -t Print Trigger Bank\n");
    printf("            -r Print ROC Banks\n");
    printf("            -v Print event by event\n");
    exit(-1);
  }

  if (argc > 2) 
  {
    maxevents = atoi(argv[2]);
  }
  if (argc > 3)
  {
    if(strncmp(argv[3],"-t",2)==0) ptb = 1;
    if(strncmp(argv[3],"-r",2)==0) pr = 1;
    if(strncmp(argv[3],"-v",2)==0) verbose = 1;
  }

  if (argc > 4) 
  {
    if(strncmp(argv[4],"-t",2)==0) ptb = 1;
    if(strncmp(argv[4],"-r",2)==0) pr = 1;
    if(strncmp(argv[4],"-v",2)==0) verbose = true;
  }


  // Initialize root and output 
  TROOT vetrocana("vetrocroot","Edet VETROC analysis");
  int run_number=0;
  run_number=atoi(argv[1]);
  TString outfile=Form("Rootfiles/eDet_%d.root",run_number);

  TFile *hfile = new TFile(outfile,"RECREATE","e detector data");
  //if(!hfile->IsOpen()) return;  
  //Build TTree to store TDC data
  TTree *T = new TTree("T","data");
  T->Branch("evtype", &evtype, "evtype/I"); 
  T->Branch("helicity", &tHelicity, "tHelicity/I"); 
  T->Branch("MPS", &tMPS, "tMPS/I"); 
  T->Branch("fadc_mode", &fadc_mode, "fadc_mode/I"); 
  T->Branch("fadc_a", fadc_int, "fadc_int[16]/I"); 
  T->Branch("fadc_t", fadc_time, "fadc_time[16]/I"); 
  T->Branch("fadc_a1", fadc_int_1, "fadc_int_1[16]/I"); 
  T->Branch("fadc_t1", fadc_time_1, "fadc_time_1[16]/I"); 
  T->Branch("fadc_nhit", fadc_nhit, "fadc_nhit[16]/I"); 

  /* Open file  */
  char datapath[100];
  sprintf(datapath,"/home/compton/data/vtpCompton_%d.dat.0",run_number);

  if ( (status = evOpen(datapath, (char*)"r",  &handle)) < 0) 
  {
    printf("Unable to open file %s status = %d\n",datapath,status);
    exit(-1);
  } 
  else 
  {
    printf("Opened %s for reading, hit <enter> to see each individual event (pr=%d ptb=%d)\n\n",datapath,pr,ptb);
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

  /* Loop through getting event blocks one at a time and print basic infomation
     about each block */
  nevents=0;
  while ((status = evReadAlloc(handle, &buf, &bufLen))==0) 
    //while ((evReadAlloc(handle, &buf, &bufLen))!= EOF && (evReadAlloc(handle, &buf, &bufLen))==0) 
  { /* read the event and allocate the correct size buffer */
	if(nevents>totalmax && totalmax != 1)break;
    nevents++;
    indx=0; pe=0;
    nWords = buf[0] + 1;
    bt  = ((buf[1]&0xffff0000)>>16);  /* Bank Tag */
    dt  = ((buf[1]&0xff00)>>8);       /* Data Type */
    blk = buf[1]&0xff;                /* Event Block size */

    num=10000;
    if(nWords<num) num=nWords;

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
      /* Print out some of the Event to std out */
      if (nWords > 64) 
        plen = 64;
      else
        plen = nWords;

      for (int ii=0;ii<plen;ii++) 
      {
        if ((ii % 8) == 0 ) printf("\n%3d : ",ii);
        printf("%08x ",buf[indx+ii]);
      }

      indx += nWords;
      if(plen < nWords)
        printf("\n      ...\n\n");
      else
        printf("\n\n");

    } 
    else 
    { /* This is a built Physics Event. Disect a bit more... */
      /* Check the Trigger Bank and how many ROC banks it has */
      indx += 2;
      nROC=0;

	  ClearTreeVar();

      /* Decode Trigger Bank into the Structure */
      tbLen = trigBankDecode(&buf[indx], blk);
	  evtype = tbank.evType[0];
	  tHelicity = tbank.helicity; 
	  tMPS = tbank.mps; 
      indx += tbLen;
      if(verbose) printf("    ** index %d ... Starting Event number = %llu **\n",indx,(long long)tbank.evtNum);
      if((long long)tbank.evtNum%1000==0) printf("  Event number = %llu **\n",(long long)tbank.evtNum);

      if (ptb) 
      {
        /* Print Trigger Bank information */
        if(verbose) printf("    Trigger BANK INFO (TAG = 0x%4x):\n",tbank.tag);
        if(verbose) printf("         Event #       Time Stamp       Event Type\n");
        if(verbose) printf("         -------       ----------       ----------\n");
        for(i=0; i<tbank.blksize; i++) 
        {
		  if(!verbose) break; 
          if(tbank.evTS != NULL)
            printf("      %10d  0x%016llx    %d\n",(int) (tbank.evtNum + i), (long long unsigned int )tbank.evTS[i], tbank.evType[i]);
          else
            printf("      %10d  (No Time Stamp)    %d\n",(int) (tbank.evtNum + i), tbank.evType[i]);
        }
        if(verbose) printf("\n");
      }

      /* Decode ROC data */
      while(nROC<tbank.nrocs){
		rocLen[nROC] = buf[indx]+1;
		rocID[nROC] = (buf[indx+1]&0xFFF0000)>>16;

		printf("roc len = %d (data = 0x%x);  roc ID = %d (data = 0x%x)\n",rocLen[nROC],buf[indx],rocID[nROC],buf[indx+1]);

	    Int_t nnWd = 2; // ROC data words counter
		if(rocID[nROC]==TI_ROC){  //TI ROC have FADC, VETROC, SCALER
		  while(nnWd<rocLen[nROC]){

			int tmplen = buf[indx+nnWd]+1;   //bank length
	        nnWd++;
			int tmpBank = (buf[indx+nnWd]&0xFFF0000)>>16; // bank tag
			nnWd++;
		    printf("bank len = %d (data = 0x%x);  bank tag = %d (data = 0x%x)\n",tmplen,buf[indx+2],tmpBank,buf[indx+3]);

			if(tmpBank == FADC_BANK){
			   if( buf[indx+nnWd] == FIRSTWORD_FADC) nnWd++; // skip the FADC special header;
			   Int_t fadc_words = buf[indx+nnWd]+1;
			   for(int kk=1;kk<fadc_words;kk++){
				   faDataDecode(buf[indx+nnWd+kk]);
			   }
			   fadc_mode = GetFadcMode();
			   nnWd += fadc_words; 
			} //FADC bank

		  } // loop one ROC data
		}  // TI ROC

		indx += rocLen[nROC];
		nROC++;
	  } // loop ROCs


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
  hfile->Close(); 
  evClose(handle);

  exit(0);

} // End of main function

void ClearTreeVar(){

     tHelicity=0;  
     tMPS=0;	   
     evtype=0;    

     fadc_mode=0;                // FADC mode
	 memset(fadc_int, 0, 16*sizeof(fadc_int[0]));
	 memset(fadc_time, 0, 16*sizeof(fadc_time[0]));
	 memset(fadc_int_1, 0, 16*sizeof(fadc_int_1[0]));
	 memset(fadc_time_1, 0, 16*sizeof(fadc_time_1[0]));
	 memset(fadc_nhit, 0, 16*sizeof(fadc_nhit[0]));

}
