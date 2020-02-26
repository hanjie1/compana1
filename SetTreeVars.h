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
Int_t clock2;						 // 40 MHz clock in Run scaler channel 2
Int_t clock3;						 // 40 MHz clock in Run scaler channel 3
Int_t scalMPS;                        // MPS in scaler

/***  VTP tree variables ***/
Int_t vtp_A_scalcnt[VETROC_NCHAN];   // vetroc channel number
Int_t vtp_B_scalcnt[VETROC_NCHAN];   // vetroc channel number
Int_t vtp_C_scalcnt[VETROC_NCHAN];   // vetroc channel number
Int_t vtp_D_scalcnt[VETROC_NCHAN];   // vetroc channel number




