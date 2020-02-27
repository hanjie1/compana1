//global variables for tree

/*** TI tree variables ***/
int tHelicity;  //TS6 bit, helicity level
int tMPS;      //TS5 bit, TSettle level
int evtype;    // event type;


/***  FADC tree varibles  ***/
int fadc_mode;                // FADC mode
Int_t fadc_int[FADC_NCHAN];           // ADC integral for the first hit per channel
Int_t fadc_time[FADC_NCHAN];          // pulse time for the first hit per channel
Int_t fadc_int_1[FADC_NCHAN];         // ADC integral for the second hit per channel 
Int_t fadc_time_1[FADC_NCHAN];        // pulse time for the second hit per channel
Int_t fadc_nhit[FADC_NCHAN];          // number of hits per channel

Int_t frawdata[FADC_NCHAN][MAXRAW];   // raw mode ADC samples

/***  VETROC tree variables ***/
Int_t eplaneA_nhits;				  // plane A total number of hits
Int_t eplaneA_chan[VETROC_MAXHIT];   // vetroc channel number
Int_t eplaneA_rt[VETROC_MAXHIT];     // vetroc hit tdc rising time
Int_t eplaneA_ft[VETROC_MAXHIT];     // vetroc hit tdc falling time

Int_t eplaneB_nhits;				  // plane B total number of hits
Int_t eplaneB_chan[VETROC_MAXHIT];   // vetroc channel number
Int_t eplaneB_rt[VETROC_MAXHIT];     // vetroc hit tdc rising time
Int_t eplaneB_ft[VETROC_MAXHIT];     // vetroc hit tdc falling time

Int_t eplaneC_nhits;				  // plane C total number of hits
Int_t eplaneC_chan[VETROC_MAXHIT];   // vetroc channel number
Int_t eplaneC_rt[VETROC_MAXHIT];     // vetroc hit tdc rising time
Int_t eplaneC_ft[VETROC_MAXHIT];     // vetroc hit tdc falling time

Int_t eplaneD_nhits;				  // plane D total number of hits
Int_t eplaneD_chan[VETROC_MAXHIT];   // vetroc channel number
Int_t eplaneD_rt[VETROC_MAXHIT];     // vetroc hit tdc rising time
Int_t eplaneD_ft[VETROC_MAXHIT];     // vetroc hit tdc falling time

/***  Scaler tree variables ***/
Int_t clock1;						 // 40MHz clock in IPscaler channel 1
Int_t CavPower;						 // cavity power
Int_t BCM;							 // BCM
Int_t BPM2AX_p;                       // BPM2A X+
Int_t BPM2AX_m;                       // BPM2A X-
Int_t BPM2AY_p;                       // BPM2A Y+
Int_t BPM2AY_m;                       // BPM2A Y-
Int_t BPM2BX_p;                       // BPM2B X+
Int_t BPM2BX_m;                       // BPM2B X-
Int_t BPM2BY_p;                       // BPM2B Y+
Int_t BPM2BY_m;                       // BPM2B Y-

/***  VTP tree variables ***/
Int_t vtp_A_scalcnt[VETROC_NCHAN];   // vtp helicity based scaler eplane A strip counts
Int_t vtp_B_scalcnt[VETROC_NCHAN];   // vtp helicity based scaler eplane B strip counts
Int_t vtp_C_scalcnt[VETROC_NCHAN];   // vtp helicity based scaler eplane C strip counts
Int_t vtp_D_scalcnt[VETROC_NCHAN];   // vtp helicity based scaler eplane D strip counts
Int_t vtp_scaldat[16];               // vtp helicity based scaler counts
Int_t trigcnt[5];                    // vtp helicity based trigger counts
Int_t busytime;                      // busy time
Int_t livetime;                      // live time
Int_t hel_win_cnt_1;                 // helicity window counts in helicity based trigger scalers
Int_t trig_pattern[64];				 // trigger bit pattern
Int_t trig_pattern_time[64];		 // trigger bit pattern time
Int_t pattern_num;					 // number of trigger bit patterns
Int_t last_mps_time;				 // last MPS time for the past helicity
Int_t vtp_past_hel[6]; 			     // last 173 helicity windows
Int_t hel_win_cnt;                   // helicity window counts in past helicity 



