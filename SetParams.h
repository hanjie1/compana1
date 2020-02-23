#define MAX_BLOCK_SIZE 255
#define MAX_ROCS       2
#define VTP_ROC        3      //VTP ROC ID
#define TI_ROC         1      //Physics modules (FADC, VETROC, Scaler) ROC ID
#define FIRSTWORD_VETROC 0xb0b0b0b4
#define FIRSTWORD_FADC   0xb0b0b0b5
#define FIRSTWORD_SCALER 0xb0b0b0b6

#define NBANK          3
#define FADC_BANK      3
#define VETROC_BANK    4
#define SCALER_BANK    6
#define EPLANEA_SLOT   13
#define EPLANEB_SLOT   14
#define EPLANEC_SLOT   15
#define EPLANED_SLOT   16

const int FADC_NCHAN=6;  // FADC maximum number of channels
const int MAXRAW=280;	 // FADC raw mode maximum window width
const int VETROC_NCHAN = 128; //VETROC maximum number of channels;
const int VETROC_MAXHIT = 512; //VETROC maximum hits;
