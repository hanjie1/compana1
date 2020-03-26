#define HELICITY_INVERT           1
#define VTP_N_HELICITY_WIN_PAST   173
#define VTP_N_HELICITY_WIN_FUTURE 16
#define VTP_N_QUARTET_MIN         ((VTP_N_HELICITY_WIN_PAST-3)/4)

// Debugging options
#define DEBUG_PRINT               1   // Print messages about helicity failures
#define WAIT_KEY_ON_ERROR         1   // Wait for key press on each failure
#define SKIP_DUPLICATE_WINDOWS    1   // Only check events on different helicity windows (removes print many duplicates of the same error for the same window)

UInt_t fgShreg = 0;

// 0=oldest, VTP_N_HELICITY_WIN-1=current VTP helicity, VTP_N_HELICITY+delay=predicted/delayed helcity
Int_t HelicityWindowSamples[VTP_N_HELICITY_WIN_PAST+VTP_N_HELICITY_WIN_FUTURE];
Int_t FirstQuartetPos;

UInt_t RanBit(UInt_t hRead)
{
  UInt_t bit7    = (fgShreg & 0x00000040) != 0;
  UInt_t bit28   = (fgShreg & 0x08000000) != 0;
  UInt_t bit29   = (fgShreg & 0x10000000) != 0;
  UInt_t bit30   = (fgShreg & 0x20000000) != 0;
  UInt_t newbit = (bit30 ^ bit29 ^ bit28 ^ bit7) & 0x1;
  fgShreg = ( (hRead == 2 ? newbit : hRead) | (fgShreg << 1 )) & 0x3FFFFFFF;
  return newbit;
}

void PrintHelicityWindows(int add_space)
{
  printf("Helicity pattern (measured & predicted, oldest->newest):\n");
  for(int n=0; n<VTP_N_HELICITY_WIN_PAST+VTP_N_HELICITY_WIN_FUTURE; n++)
  {
    if((n%4)==FirstQuartetPos && add_space)  // put a space between quartet boundaries
      printf(" ");
    printf("%d", HelicityWindowSamples[n]);
  }
  printf("\n");
}

void CheckVTPHelicity()
{
  int runnumber=1073;
  cout<<"Which run ?  ";
  cin>>runnumber;

  TString filename = Form("../Rootfiles/eDet_%d.root", runnumber);
  TFile *f0 = new TFile(filename);
  if(! (f0->IsOpen()) ) return;
  TTree *T = (TTree*) f0->Get("T");
  TTree *VTP = (TTree*) f0->Get("VTP");
  T->AddFriend(VTP);

  Int_t current_helicity, hel_win_cnt, vtp_past_hel[6], ti_helicity, ti_evtype, vtp_last_mps_time;
  T->SetBranchAddress("VTP.current_helicity", &current_helicity);
  T->SetBranchAddress("VTP.vtp_past_hel", &vtp_past_hel[0]);
  T->SetBranchAddress("VTP.hel_win_cnt", &hel_win_cnt);
  T->SetBranchAddress("VTP.last_mps_time", &vtp_last_mps_time);
  T->SetBranchAddress("helicity", &ti_helicity);
  T->SetBranchAddress("evtype", &ti_evtype);

  Long64_t nTotal = 0, nTotalEvents = 0, nPassed = 0, nFailed = 0;
  Int_t last_hel_win_cnt = 0;
  Long64_t n = T->GetEntries();
  for(Long64_t i=0; i<n; i++)
  {
    T->GetEntry(i);

    nTotalEvents++;

    if(!(nTotal % 10000))
      printf("Total Helicity Windows: %lld, Total Events: %lld, Total Matches: %lld, Total Failures: %lld\n", nTotal, nTotalEvents, nPassed, nFailed);

    if(last_hel_win_cnt != hel_win_cnt)
    {
      last_hel_win_cnt = hel_win_cnt;
      nTotal++;
    }
#if SKIP_DUPLICATE_WINDOWS
    else
      continue;
#endif

#if HELICITY_INVERT
    for(int j=0;j<6;j++)
      vtp_past_hel[j] = ~vtp_past_hel[j];
#endif

    // Fill HelicityWindowSamples old->current
    int n=0, len;
    for(int j=5;j>=0;j--)
    {
      if(!j) len = 23-1;
      else   len = 30-1;
      for(int k=len;k>=0;k--)
        HelicityWindowSamples[n++] = (vtp_past_hel[j] & (1<<k)) ? 1 : 0;
    }

    // Identify first (oldest) start position for quartet frame
    int err;
    for(FirstQuartetPos=0; FirstQuartetPos<4; FirstQuartetPos++)
    {
      err = 0;
      for(int q=0; q<VTP_N_QUARTET_MIN; q++)
      {
        if( (HelicityWindowSamples[FirstQuartetPos+q*4+0]!=HelicityWindowSamples[FirstQuartetPos+q*4+3]) ||
            (HelicityWindowSamples[FirstQuartetPos+q*4+1]!=HelicityWindowSamples[FirstQuartetPos+q*4+2]) ||
            (HelicityWindowSamples[FirstQuartetPos+q*4+0]==HelicityWindowSamples[FirstQuartetPos+q*4+1]) )
        {
          err = 1;
          break;
        }
      }
      if(!err)
        break;
    }

    if(err)
    {
      printf("Unable to finder quartet boundaries for entry %lld\n", i);
      continue;
    }

    // Loop over HelicityWindowSamples, starting with first valid quartet
    //   Seed PRBS for first full 30 quartets, check PRBS for existing VTP samples, fill remaining helicity samples
    UInt_t s = 0, q = 0, b, newbit;
    for(int n=FirstQuartetPos; n<VTP_N_HELICITY_WIN_PAST+VTP_N_HELICITY_WIN_FUTURE; n++)
    {
      b = s++ % 4;  // bit position in quartet frame
      if(q<30)    // less than 30 quartets seen, prime PRBS generator
      {
        if(b==0)  // is this a start of a new quartet
        {
          int hRead = HelicityWindowSamples[n];
          newbit = RanBit(hRead);
        }
      }
      else        // PRBS generator is primed, predict helicity windows
      {
        if(b==0)  // is this a start of a new quartet
          newbit = RanBit(2);
        else if(b==1 || b==3) // flip at these points in quartet window
          newbit = !newbit;
        
        if(n < VTP_N_HELICITY_WIN_PAST) // We can test PRBS synchronization if we still have samples from VTP
        {
          if(HelicityWindowSamples[n] != newbit)
            printf("Helicity PRBS check failure: entry %lld, n=%d, VTP=%d, predict=%d\n", i, n, HelicityWindowSamples[n], newbit);
        }
        else      // We have to predict these
          HelicityWindowSamples[n] = newbit;
      }
      if(b==3)    // end of current quartet
        q++;
    }

    int fail = 0;
    if(current_helicity != HelicityWindowSamples[VTP_N_HELICITY_WIN_PAST+8])
    {
      fail = 1;
#if DEBUG_PRINT
      printf("current_helicty != VTP past_hel prediction\n");
#endif
    }
    if((ti_helicity      != HelicityWindowSamples[VTP_N_HELICITY_WIN_PAST+0])  && (vtp_last_mps_time > 100))
    {
      fail = 1;
#if DEBUG_PRINT
      printf("ti_helicity != VTP past_hel prediction\n");
#endif
    }

    if(fail)
    {
      nFailed++;
      PrintHelicityWindows(1);

#if DEBUG_PRINT
      printf("entry number: %lld", i);
      printf(",current_helicity: %d", current_helicity);
      printf(",ti_helicity: %d ", ti_helicity);
      printf(",ti_evtype: %d", ti_evtype);
      printf(",last_mps_time: %d", vtp_last_mps_time);
      printf(",delayed helcity(0): %d", HelicityWindowSamples[VTP_N_HELICITY_WIN_PAST+0]);
      printf(",delayed helcity(8): %d", HelicityWindowSamples[VTP_N_HELICITY_WIN_PAST+8]);
      printf("\n\n");
#endif

#if WAIT_KEY_ON_ERROR
      getchar();
#endif
    }
    else
      nPassed++;
  }
  printf("Completed: Total Helicity Windows: %lld, Total Events: %lld, Total Matches: %lld, Total Failures: %lld\n", nTotal, nTotalEvents, nPassed, nFailed);
}

