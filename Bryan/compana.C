/**
 *   Simple Compton decoder to demonstrate block indexing
 */

/* Modified to print out decoded vetroc data */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "evio.h"
#include "simpleLib.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TBranch.h"
#include "TH1.h"

#define MAX_BLOCK_SIZE 255
#define MAX_ROCS       255
#define FIRSTWORD_VETROC 0xb0b0b0b4
#define FIRSTWORD_FADC   0xb0b0b0b5
//#define FIRSTWORD_FADC   0xcebaf222
//#define SECONDWORD_FADC 0x22f2bace

#define NSAMPLE 500

/* Define FADC data types */
#define FADC_DATA_BLOCK_HEADER      0x0
#define FADC_DATA_BLOCK_TRAILER     0x1
#define FADC_DATA_EVENT_HEADER      0x2
#define FADC_DATA_TRIGGER_TIME      0x3
#define FADC_DATA_WINDOW_RAW        0x4
#define FADC_DATA_PULSE_RAW         0x6
#define FADC_DATA_PULSE_INT         0x7
#define FADC_DATA_PULSE_TIME        0x8
#define FADC_DATA_PULSE_PAR         0x9
#define FADC_DATA_PULSE_PED         0xa
#define FADC_DATA_SCALAR_DATA       0xc
#define FADC_DUMMY_DATA             0xe
#define FADC_DATA_FILLER            0xf

/* Define VETROC data types */
#define VETROC_DATA_BLOCK_HEADER      0x00000000
#define VETROC_DATA_BLOCK_TRAILER     0x08000000
#define VETROC_DATA_EVENT_HEADER      0x10000000
#define VETROC_DATA_TRIGGER_TIME      0x18000000
#define VETROC_DATA_INVALID           0x70000000
#define VETROC_DATA_FILLER            0x78000000
#define VETROC_DUMMY_DATA             0xf800fafa
#define VETROC_DATA_TDCEVT            0x40000000


#define MIN(a,b) (a<b)? a : b


//const Int_t NSAMPLE = 500;

int trigBankDecode(uint32_t *tb, int blkSize);
void fadcDataDecode(unsigned int data);
void vetrocDataDecode(unsigned int data);

//global variables for tree
int eventID, slotID, blkID, nWd;
int chID; //channel ID
int TDC, edge; // TDC and edge (0 for rasing, 1 for falling)
//int nhits1, nhits2, nhits3, nhits4; // number of channel fired for each plane
Int_t iADC[NSAMPLE]; //raw adc sample
float pulse_time;  // pulse time (pass threshold)
int pulse_time_Q; // time quality
int pulse_peak; // pulse peak
int pulse_integral; // pulse integral NSB+NSA samples
int pulse_Q; // pulse integral quality
int pulse_n_sample; // numer of smaple above threshold
int ped_sum; // pedestal integral
int ped_Q; //pedestal quality
ULong64_t fadc_trgTime = 0;
ULong64_t vetroc_trgTime = 0;


int DATATYPE = 0;
int idx=0;
int idx3=0;
int idx4=0;
int idx6=0;
int idx7=0;
int idx8=0;
int idx9=0;

unsigned long long trgtime0 = 0;
unsigned long long trgtime1 = 0;
unsigned long long trgtime = 0;


typedef struct trigBankObject {
  int      blksize;              /* total number of triggers in the Bank */
  uint16_t tag;                  /* Trigger Bank Tag ID = 0xff2x */
  uint16_t nrocs;                /* Number of ROC Banks in the Event Block (val = 1-256) */
  uint32_t len;                  /* Total Length of the Trigger Bank - including Bank header */
  int      withTimeStamp;        /* =1 if Time Stamps are available */
  int      withRunInfo;          /* =1 if Run Informaion is available - Run # and Run Type */
  uint64_t evtNum;               /* Starting Event # of the Block */
  uint64_t runInfo;              /* Run Info Data */
  uint32_t *start;               /* Pointer to start of the Trigger Bank */
  uint64_t *evTS;                /* Pointer to the array of Time Stamps */
  uint16_t *evType;              /* Pointer to the array of Event Types */
} TBOBJ;

TBOBJ tbank;


bool verbose = false;

int main (int argc, char **argv)
{

  int plen, i=0, ii, j, indx, bt, dt, blk, handle, nevents, status, nWords, version;
  int num, numwords_fadc = 0;
  int numwords_vetroc = 0;
  int pe, ptb, pr, tbLen, rocLen[MAX_ROCS], rocID[MAX_ROCS];
  uint32_t *buf, dLen, bufLen;
  uint32_t *rocP[MAX_ROCS];
  int rocPtr[MAX_ROCS];  /* pointer to buffer location */
  char *dictionary = NULL;
  bool eventbyevent = true;
  int maxevents = 1e9;
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
  char outfile[100];
  strcpy(outfile,argv[1]);
  strcat(outfile,".root");
  printf("%s\n",outfile);
  TFile *hfile = new TFile(outfile,"RECREATE","VETROC data");
  //if(!hfile->IsOpen()) return;
  //Build TTree to store TDC data
  TTree *eventtree = new TTree("eventtree","TDC data");
  eventtree->Branch("eventID",&eventID, "eventID/I");
  eventtree->Branch("slotID", &slotID, "slotID/I");
  eventtree->Branch("blkID", &blkID, "blkID/I");
  eventtree->Branch("nWd", &nWd, "nWd/I");
  eventtree->Branch("fadc_trgTime", &fadc_trgTime, "fadc_trgTime/l");
  eventtree->Branch("vetroc_trgTime", &vetroc_trgTime, "vetroc_trgTime/l");
  eventtree->Branch("iADC", iADC, TString::Format("iADC[%d]/I",NSAMPLE));
  eventtree->Branch("ped_sum", &ped_sum, "ped_sum/I");
  eventtree->Branch("ped_Q", &ped_Q, "ped_Q/I");
  eventtree->Branch("pulse_integral", &pulse_integral, "pulse_integral/I");
  eventtree->Branch("pulse_Q", &pulse_Q, "pulse_Q/I");
  eventtree->Branch("pulse_peak", &pulse_peak, "pulse_peak/I");
  eventtree->Branch("pulse_time", &pulse_time, "pulse_time/F");
  eventtree->Branch("pulse_time_Q", &pulse_time_Q, "pulse_time_Q/I");
  eventtree->Branch("chID", &chID, "chID/I");
  eventtree->Branch("TDC", &TDC, "TDC/I");
  eventtree->Branch("edge", &edge, "edge/I");
  //eventtree->Branch("nhits1", &nhits1, "nhits1/I");
  //eventtree->Branch("nhits2", &nhits2, "nhits2/I");
  //eventtree->Branch("nhits3", &nhits3, "nhits3/I");
  //eventtree->Branch("nhits4", &nhits4, "nhits4/I");


  /* Open file  */
  if ( (status = evOpen(argv[1], (char*)"r",  &handle)) < 0)
    {
      printf("Unable to open file %s status = %d\n",argv[1],status);
      exit(-1);
    }
  else
    {
      printf("Opened %s for reading, hit <enter> to see each individual event (pr=%d ptb=%d)\n\n",argv[1],pr,ptb);
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
  simpleConfigBank(3, 0x11, 20, 1, 0, NULL);
  simpleConfigBank(3, 0x12, 20, 1, 0, NULL);



  /* Loop through getting event blocks one at a time and print basic infomation
     about each block */
  nevents=0;
  while ((status = evReadAlloc(handle, &buf, &bufLen))==0)
    //while ((evReadAlloc(handle, &buf, &bufLen))!= EOF && (evReadAlloc(handle, &buf, &bufLen))==0)
    { /* read the event and allocate the correct size buffer */
      nevents++;
      indx=0; pe=0;
      nWords = buf[0] + 1;
      bt  = ((buf[1]&0xffff0000)>>16);  /* Bank Tag */
      dt  = ((buf[1]&0xff00)>>8);       /* Data Type */
      blk = buf[1]&0xff;                /* Event Block size */

      num=10000;
      if(nWords<num) num=nWords;
      /*
        for (i=0; i<num; i++)
        {
        printf("(1st %d, raw  evbuffer [%d] = 0x%x = dec %d \n",num,i,buf[i],buf[i]);
        }
      */

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

	  for (ii=0;ii<plen;ii++)
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
	  /* SIMPLE: index the data */
	  simpleScan(buf, nWords);


#ifdef OLDWAY
	  /* Check the Trigger Bank and how many ROC banks it has */
	  indx += 2;
	  ii=0;

	  /* Decode Trigger Bank into the Structure */
	  tbLen = trigBankDecode(&buf[indx], blk);
	  indx += tbLen;
	  if(verbose) printf("    ** index %d ... Starting Event number = %llu **\n",indx,(long long)tbank.evtNum);
	  if((long long)tbank.evtNum%1000==0) printf("  Event number = %llu **\n",(long long)tbank.evtNum);

	  while (indx<nWords)
	    {
	      rocPtr[ii] = indx;
	      rocP[i] = (uint32_t *) &buf[indx];       /* pointer to start of ROC Bank */
	      if(verbose) printf("really ?  %d %d %d  \n",ii,indx,rocPtr[ii]);
	      rocLen[ii] = (buf[indx]+1);               /* total Length of ROC Bank */
	      rocID[ii] = (buf[indx+1]&0xffff0000)>>16; /* ID of ROC */
	      indx += rocLen[ii];
	      ii++;
	    }

	  /* Check number of ROCs  matches */
	  if(ii != tbank.nrocs)
	    printf(" ****ERROR: Trigger and Physics Block sizes do not match (%d != %d)\n",ii,tbank.nrocs);

	  if(verbose) printf("    Trigger Bank Len = %d words (There are %d ROC Banks)\n",tbLen, ii);
	  for(i=0;i<ii;i++)
	    {
	      if(verbose) printf("     ROC %d  ID = %3d  Ptr = %d  Len = %6d words \n",i,rocID[i],rocPtr[i],rocLen[i]);
	    }
	  if(verbose) printf("\n");
#endif /* OLDWAY */

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


	  if (0)
	    {
	      // here it doesn't work for fadc
	      /* Print ROC Bank Data */
	      for (ii=0; ii<tbank.nrocs ; ii++)
		{
		  if(verbose) printf("    ROC BANK DATA (ID=%d Len=%d words):",rocID[ii],rocLen[ii]);
		  for(i=0; i<rocLen[ii];i++)
		    {
		      if ((i % 8) == 0 ) printf("\n    %3d : ",i);
		      printf("0x%08x ",rocP[ii][i]);
		    }
		  printf("\n\n");
		}
	    }

	  /* Decode raw data */

	  if(verbose) printf("Looking for firstword = 0x%x \n",FIRSTWORD_FADC);

	  // clear tree variables
	  eventID=slotID=blkID=nWd=-9999;
	  chID=TDC=edge=-9999;
	  //nhits1=nhits2=nhits3=nhits4=0;
	  eventID = nevents;
	  pulse_peak = 0;
	  pulse_time = 0;
	  pulse_integral = 0;
	  pulse_Q = -99;
	  ped_sum = 0;
	  ped_Q = 0;
	  memset(iADC, 0, NSAMPLE*sizeof(iADC[0]));
	  if(verbose) printf("eventID = %d\n", eventID);

	  int k = 0; // word index for each fadc
	  int l = 0; // word index for each vetroc


	  DATATYPE = 0;

	  trgtime0 = 0;
	  trgtime1 = 0;
	  trgtime = 0;

	  if(verbose) printf("num = %d\n",num);


	  /* Start with data that's not blocked */
	  unsigned int header = 0;
	  int rocID = 1, bankID = 3, slot = 3;

	  /* FADC data */
	  if(simpleGetSlotBlockHeader(1 /* rocID */,
				      3 /* bankID */,
				      3 /* slot */,
				      &header) <= 0)
	    {
	      printf("ERROR getting block header\n");
	    }
	  fadcDataDecode(header);



	  /* SIMPLE: Loop through blocked data - Block level determined from trigger bank */
	  int iev = 0;  /* event of block index := [0, block level - 1] */
	  for(iev = 0; iev < tbank.blksize; iev++)
	    {
	      int idata = 0;
	      unsigned int *simpBuf = NULL;
	      int simpLen = 0;

	      rocID = 1; bankID = 3; slot = 3;  // FADC Slot 3
	      simpLen = simpleGetSlotEventData(rocID, bankID, slot, iev, &simpBuf);
	      if(simpLen <= 0)
		{
		  printf("ERROR getting rocID = 0x%x, bankID = 0x%x, slot %d event data\n",
			 rocID, bankID, slot);
		}
	      for(idata = 0; idata < simpLen; idata++)
		{
		  fadcDataDecode(simpBuf[idata]);
		}

	      rocID = 1; bankID = 4;  // Vetroc Slot 13-16
	      for(slot = 13; slot < 17; slot++)
		{
		  simpLen = simpleGetSlotEventData(rocID, bankID, slot, iev, &simpBuf);
		  if(simpLen <= 0)
		    {
		      printf("ERROR getting rocID = 0x%x, bankID = 0x%x, slot %d event data\n",
			     rocID, bankID, slot);
		    }
		  for(idata = 0; idata < simpLen; idata++)
		    {
		      vetrocDataDecode(simpBuf[idata]);
		    }
		}

	      eventtree->Fill();
	    }

#ifdef OLDSTUFF
	  for (j=0; j<num; j++)
	    {
	      //if(verbose) printf("j = %d, word = 0x%x\n", j, buf[j]);
	      //printf("(1st %d, raw  evbuffer [%d] = 0x%x = dec %d \n",num,i,buf[i],buf[i]);
	      if((buf[j] != 0xffffffff) && ((buf[j] & FIRSTWORD_FADC) ==  FIRSTWORD_FADC))// && ((buf[j] & SECONDWORD_FADC) == SECONDWORD_FADC))
		{
		  numwords_fadc = buf[j+1];
		  if(verbose) printf("\nFound FADC firstword = 0x%x , followed by %d words\n", buf[j], numwords_fadc);
		  k = j+2;
		  continue;
		}
	      if((buf[j] != 0xffffffff) && ((buf[j] & FIRSTWORD_VETROC) ==  FIRSTWORD_VETROC))
		{
		  numwords_vetroc = buf[j+1];
		  if(verbose) printf("\n Found VETROC firstword = 0x%x , followed by %d words\n", buf[j], numwords_vetroc);
		  l=j+2;
		  continue;
		}
	      if ( k>0 && j>=k && l==0 )
		{
		  fadcDataDecode(buf[j]);
		  nWd = numwords_fadc; // data(TDC) words
		  if(idx4 == NSAMPLE/2){ // be careful if n_{samples} is odd
		    eventtree->Fill();
		    idx4=0;
		    if(verbose) printf(" filled tree for channel %d\n\n",chID);
		  }
		  if(idx9 == 3){
		    eventtree->Fill();
		    idx9=0;
		    if(verbose) printf(" filled tree for channel = %d\n\n",chID);
		  }
		  if(idx7 == 1 && idx8 ==1 ) {
		    eventtree->Fill();
		    idx7 = 0; idx8 = 0;
		    if(verbose) printf(" filled tree for channel = %d\n\n",chID);
		  }

		}
	      if (l>0 && j>=l && j<l+numwords_vetroc) {
		vetrocDataDecode(buf[j]);
		nWd = numwords_vetroc - 6; // data(TDC) words
		if(j >= l+4 && j < l+numwords_vetroc-2 ) {
		  eventtree->Fill();
		}
	      }
	    }
#endif /* OLDSTUFF */

	}

      /* free the event buffer and wait for next one */
      free(buf);

      //uncomment the following to lines to view event by event
      if(eventbyevent) {
	printf("Hit return for next event or q to exit; hit a or A to replay all events.\n");
	int typein = getchar();
	if(typein == 113) break;
	if(typein == 65 || typein == 97) eventbyevent=false;
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


  eventtree->Write();
  hfile->Close();
  evClose(handle);

  exit(0);

} // End of main function



int
trigBankDecode(uint32_t *tb, int blkSize)
{

  memset((void *)&tbank, 0, sizeof(TBOBJ));

  tbank.start = (uint32_t *)tb;
  tbank.blksize = blkSize;
  tbank.len = tb[0] + 1;
  tbank.tag = (tb[1]&0xffff0000)>>16;
  tbank.nrocs = (tb[1]&0xff);
  tbank.evtNum = tb[3];

  if((tbank.tag)&1)
    tbank.withTimeStamp = 1;
  if((tbank.tag)&2)
    tbank.withRunInfo = 1;

  if(tbank.withTimeStamp) {
    tbank.evTS = (uint64_t *)&tb[5];
    if(tbank.withRunInfo) {
      tbank.evType = (uint16_t *)&tb[5 + 2*blkSize + 3];
    }else{
      tbank.evType = (uint16_t *)&tb[5 + 2*blkSize + 1];
    }
  }else{
    tbank.evTS = NULL;
    if(tbank.withRunInfo) {
      tbank.evType = (uint16_t *)&tb[5 + 3];
    }else{
      tbank.evType = (uint16_t *)&tb[5 + 1];
    }
  }


  return(tbank.len);
}


void
fadcDataDecode(unsigned int data)
{

  //if(verbose) printf("Data word = 0x%08X  ", data);

  if(data & 0x80000000)  /* If a valid data type */
    {
      DATATYPE = data & 0x78000000;  /* Determine data type */
      idx3 = 0;
      idx4 = 0;
      idx6 = 0;
      idx9 = 0;
    }

  //int mydat;
  //mydat = data;
  //if(idx3==0 && idx4 ==0 && idx4 == 0)printf("DATATYPE = 0x%x \n",DATATYPE>>27);
  /*
    char strout[80];
    if((mydat>>16)&0x7f != 1){
    sprintf(strout,"%d\n",(mydat>>0)&0xFFFF);
    FILE *f;
    f = fopen("data.dat","a");
    fputs(strout, f);
    fclose(f);
    }*/

  switch(DATATYPE>>27)
    {
    case FADC_DATA_BLOCK_HEADER:
      if(verbose) printf(" {BLKHDR} SLOTID: %d", (data>>22)&0x1f);
      if(verbose) printf(" ModuleID: %d", (data>>18)&0xf);
      if(verbose) printf(" BlockID: %d", (data>>8)&0x3ff);
      if(verbose) printf(" NEVENTS: %d\n", (data>>0)&0xff);
      slotID = (data>>22)&0x1f;
      blkID = (data>>8)&0x3ff;
      break;
    case FADC_DATA_BLOCK_TRAILER:
      if(verbose) printf(" {BLKTLR} SLOTID: %d", (data>>22)&0x1f);
      if(verbose) printf(" NWORDS: %d\n", (data>>0)&0x3fffff);
      break;
    case FADC_DATA_EVENT_HEADER:
      if(verbose) printf(" {EVTHDR} SLOTID: %d", (data>>22)&0x1f);
      if(verbose) printf(" {EVTHDR} EVENT: %d\n", (data>>0)&0x3fffff);
      break;
    case FADC_DATA_TRIGGER_TIME:
      if(idx3 == 0)
	{
	  trgtime0 = data & 0xffffff;
	  if(verbose) printf(" trigger time word 1: %08x\n", data);
	  if(verbose) printf(" {TRGTIME} 0: %06X = %d\n", data & 0xffffff, data & 0xffffff);
	}
      if(idx3 == 1)
	{
	  if(verbose) printf(" trigger time word 2: %08x\n", data);
	  trgtime1 = data & 0xffffff;
	  if(verbose) printf(" {TRGTIME} 1: %06X = %d\n", data & 0xffffff, data & 0xffffff);
	}
      trgtime = (trgtime1 & 0Xffffff) + ((trgtime0 & 0xffffff)<<24); //4ns in case of 250MHz
      //trgtime = (trgtime0 & 0Xffffff) + ((trgtime1 & 0xffffff)<<24); //4ns in case of 250MHz
      if(idx3 == 1) {
        fadc_trgTime = (ULong64_t)trgtime*4; // units ns
        vetroc_trgTime = 0;
        if(verbose) printf(" {TRGTIME} : %llu (4ns) = %llu ns\n", trgtime, fadc_trgTime);
      }
      idx3++;
      break;
    case FADC_DATA_WINDOW_RAW:
      if(idx4==0) {
        if(verbose) printf(" {WINDOW RAW} CH: %d", (data>>23)&0xf);
        if(verbose) printf(" WINDOW WIDTH: %d samples\n", (data>>0)&0xfff); //
        chID = (data>>23)&0xF;
        // clean up VETROC values
        edge = -99; TDC = -99;
      }
      if(idx4%100 == 1){
        if(verbose) printf(" adc sample %3d and %3d: %d", 2*idx4-1, 2*idx4, (data>>16)&0x1fff);
        if(verbose) printf(" , %d\n", (data>>0)&0x1fff);
      }
      if(idx4>0) {
        iADC[2*idx4-2] = (int) (data>>16)&0x1fff;
        iADC[2*idx4-1] = (int) (data>>0)&0x1fff;
      }
      idx4++;
      break;
    case FADC_DATA_PULSE_RAW:
      if(idx6==0) {
        if(verbose) printf(" {PluseRAW} CH: %d", (data>>23)&0xf);
        if(verbose) printf(" PULSE NUM: %d", (data>>21)&0x3);
        if(verbose) printf(" First sample: %d\n", (data>>0)&0x3ff);
      }
      else if(idx6 < 5){
        if(verbose) printf(" ADC sample %3d and %3d: %d", 2*(idx6-1)+1, 2*(idx6-1)+2, (data>>16)&0x1fff);
        if(verbose) printf(" , %d\n", (data>>0)&0x1fff);
      }
      idx6++;
      break;
    case FADC_DATA_PULSE_PAR:
      if(idx9==0) {
        if(verbose) printf(" {PlusePAR} event #: %d", (data>>18)&0xff);
        if(verbose) printf(" CH: %d\n", (data>>15)&0xf);
        if(verbose) printf("  Pedestal Quality: %d\n", (data>>14)&0x1);
        if(verbose) printf("  Pedestal Sum: %d\n", (data>>0)&0x3fff);
        chID = (data>>15)&0xf;
        ped_sum = (data>>0)&0x3fff;
        ped_Q = (data>>14)&0x1;
      }
      if(idx9==1 && ((data>>30)&0x1) == 1){
        if(verbose) printf("  Pulse integral: %d\n", (data>>12)&0x3ffff);
        if(verbose) printf("  Integral Quality: %d\n", (data>>9)&0x7);
        if(verbose) printf("  N_{samples above threshold}: %d\n", (data>>0)&0x1ff);
        pulse_integral = (data>>12)&0x3ffff;
        pulse_Q = (data>>9)&0x7;
        pulse_n_sample = (data>>0)&0x1ff;
      }
      if(idx9==2 && ((data>>30)&0x1) == 0){
        if(verbose) printf("  Coarse time: %d (4 ns)\n", (data>>21)&0x1ff); // 4ns
        if(verbose) printf("  Fine time: %d (1/16 ns) \n", (data>>15)&0x3f); // 1/16ns
        if(verbose) printf("  Pulse peak: %d\n", (data>>3)&0xfff);
        if(verbose) printf("  Time quality: %d\n", (data>>0)&0x7);
        pulse_time = ((data>>21)&0x1ff)*4 + ((data>>15)&0x3f)/16.; // coarse + fine (ns)
        pulse_peak = (data>>3)&0xfff;  //pulse peak
        pulse_time_Q = (data>>0)&0x7;  // time quality
      }
      idx9++;
      break;
    case FADC_DATA_PULSE_INT: //0x7
      if(verbose) printf("{PULSEINTEGRAL} CH: %d", (data>>23)&0xf);
      if(verbose) printf(" PULSE NUM: %d", (data>>21)&0x3);
      if(verbose) printf(" Quaility Factor: %d", (data>>19)&0x3);
      if(verbose) printf(" Pulse integral: %d\n", (data>>0)&0x7ffff);
      chID = (data>>23)&0xf;
      pulse_integral = (data>>0)&0x7ffff;
      pulse_Q = (data>>19)&0x3;
      idx7++;
      break;
    case FADC_DATA_PULSE_TIME: //0x8
      if(verbose) printf("{PULSETIME} CH: %d", (data>>23)&0xf);
      if(verbose) printf(" PULSE NUM: %d", (data>>21)&0x3);
      if(verbose) printf(" Quaility Factor: %d\n", (data>>19)&0x3);
      if(verbose) printf(" Pulse time (coarse): %d (4 ns) \n", (data>>6)&0x3ff);
      if(verbose) printf(" Pulse time (fine) : %d (1/16 ns) \n", (data>>0)&0x3f);
      if(verbose) printf(" Pulse time: %.2f\n", 4*((data>>6)&0x3ff) + ((data>>0)&0x3f)/16.);
      chID = (data>>23)&0xf;
      pulse_time = 4*((data>>6)&0x3ff) + ((data>>0)&0x3f)/16.;
      idx8++;
      break;
    case FADC_DATA_PULSE_PED:
      if(verbose) printf("{PULSEPED} CH %d", (data>>23)&0xf);
      if(verbose) printf(" PULSE NUM: %d", (data>>21)&0x3);
      if(verbose) printf(" PULSE peak: %d\n", (data>>0)&0xfff);
      break;
    case FADC_DATA_FILLER:
      if(verbose) printf(" {FILLER}\n");
      break;
    default:
      if(verbose) printf(" {***DATATYPE ERROR***}\n");
      break;
    }
  return;

} // End of fadcDataDecode()

void
vetrocDataDecode(unsigned int data)
{

  idx = 1;
  //if(verbose) printf("Data word = 0x%08X  ", data);

  if(data & 0x80000000)  /* If a valid data type */
    {
      DATATYPE = data & 0x78000000;  /* Determine data type */
      idx = 0;
    }

  //int mydat;
  //mydat = data;
  //if(verbose) printf("DATATYPE = 0x%x   idx = %d \n",DATATYPE>>27,idx);
  /*
    char strout[80];
    if((mydat>>16)&0x7f != 1){
    sprintf(strout,"%d\n",(mydat>>0)&0xFFFF);
    FILE *f;
    f = fopen("data.dat","a");
    fputs(strout, f);
    fclose(f);
    }*/

  switch(DATATYPE)
    {
    case VETROC_DATA_BLOCK_HEADER:
      if(verbose) printf(" {BLKHDR} SLOTID: %d", (data>>22)&0x1f);
      if(verbose) printf(" BLOCKNUM: %d", (data>>8)&0x3ff);
      if(verbose) printf(" NEVENTS: %d\n", (data>>0)&0xff);
      slotID = (data>>22)&0x1f;
      blkID = (data>>8)&0x3ff;

      //ndataevents++;
      break;
    case VETROC_DATA_BLOCK_TRAILER:
      if(verbose) printf(" {BLKTLR} SLOTID: %d", (data>>22)&0x1f);
      if(verbose) printf(" NWORDS: %d\n", (data>>0)&0x3fffff);
      nWd=(data>>0)&0x3fffff;
      //ndataevents++;
      break;
    case VETROC_DATA_EVENT_HEADER:
      if(verbose) printf(" {EVTHDR} EVENT: %d\n", (data>>0)&0x7ffffff);
      //ndataevents++;
      break;
    case VETROC_DATA_TRIGGER_TIME:
      if(idx == 0)
	{
	  trgtime0 = data & 0xffffff;
	  //if(verbose) printf(" {TRGTIME} 0: %08X = %d\n", data & 0xffffff, data & 0xffffff);
	}
      if(idx == 1)
	{
	  trgtime1 = data & 0xffffff;
	  //if(verbose) printf(" {TRGTIME} 1: %08X = %d\n", data & 0xffffff, data & 0xffffff);
	}
      //ndataevents++;
      trgtime = (trgtime0 & 0Xffffff) + ((trgtime1 & 0xffffff)<<24); //4ns in case of 250MHz
      if(idx == 1)
	{
	  vetroc_trgTime = (ULong64_t)trgtime*8; // units ns
	  fadc_trgTime = 0;
	  if(verbose) printf(" {TRIGGER TIME} : %llx = %llu (8ns) = %llu ns \n", trgtime, trgtime, vetroc_trgTime);
	}
      break;
    case VETROC_DATA_TDCEVT:
      if(verbose) printf(" {TDCEVT} CH: %d", (data>>16)&0x7f);
      if(verbose) printf(" TIME: %dns", (data>>0)&0xFFFF);
      if(verbose) printf(" Edge: %d\n", (data>>26)& 0x1); // 0 for rising; 1 for falling
      edge = (data>>26)& 0x1;
      chID = (data>>16)&0xFF;
      TDC = (data>>0)&0xFFFF;

      // clean up FADC values
      pulse_peak = 0;
      pulse_time = 0;
      pulse_time_Q = 0;
      pulse_integral = 0;
      pulse_Q = 0;
      pulse_n_sample = 0;
      ped_Q = 0;
      ped_sum = 0;

      break;
    case VETROC_DATA_INVALID:
      if(verbose) printf(" {***DNV***}\n");
      break;
    case VETROC_DATA_FILLER:
      if(verbose) printf(" {FILLER}\n");
      break;
    default:
      if(verbose) printf(" {***DATATYPE ERROR***}\n");
      break;
    }
  return;

} // End of vetrocDataDecode()
