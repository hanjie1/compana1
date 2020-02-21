//global variables for tree

/*** TI tree variables ***/
int tHelicity;  //TS6 bit, helicity level
int tMPS;      //TS5 bit, TSettle level
int evtype;    // event type;


/***  FADC tree varibles  **/
int fadc_mode;                // FADC mode
Int_t fadc_int[FADC_NCHAN];           // ADC integral for the first hit per channel
Int_t fadc_time[FADC_NCHAN];          // pulse time for the first hit per channel
Int_t fadc_int_1[FADC_NCHAN];         // ADC integral for the second hit per channel 
Int_t fadc_time_1[FADC_NCHAN];        // pulse time for the second hit per channel
Int_t fadc_nhit[FADC_NCHAN];          // number of hits per channel

Int_t frawdata[FADC_NCHAN][MAXRAW];   // raw mode ADC samples
