const int maxbits = 173;            // number of helicity windows in vtp_past_hel
const int hel_pat1[4] = {1,0,0,1};  // helicity pattern 1: +--+
const int hel_pat2[4] = {0,1,1,0};  // helicity pattern 2: -++-
UInt_t fgShreg = 0;
UInt_t fgShreg_earlier = 0;         // it's actually the 30 bits for the previsous event

bool CheckPattern(Int_t bit0, Int_t bit1, Int_t bit2, Int_t bit3){
    bool match = false;

	if( (bit0==hel_pat1[0]) && (bit1==hel_pat1[1]) && (bit2==hel_pat1[2]) && (bit3==hel_pat1[3]) ) // pattern 1
	    match = true;

	if( (bit0==hel_pat2[0]) && (bit1==hel_pat2[1]) && (bit2==hel_pat2[2]) && (bit3==hel_pat2[3]) ) // pattern 2
		match = true;

	return match;
}

int FindQuad(Int_t past_hel[6]){
//  Find the first quad; initialize fgShreg; 

    Int_t helbit[maxbits]={-1};
	int bit_num = maxbits-1;
    for(int jj=0; jj<6; jj++){
        int nbits = 30; 
        if(jj==0) nbits = 23; 
        for(int nnbit=0; nnbit<nbits; nnbit++)
         {
            helbit[bit_num] = ( past_hel[jj]>>nnbit ) & 0x1;   // the bit 0 in vtp_past_hel[0] is the most recent helicity
            bit_num = bit_num-1;
         }
    }

	bool findpat = false;
	bool firstquad = true;
	int quadstart = 0;  // quartet start bit
	int npatt = 0;      // number of quartets matched 
	bool match = false;

    for(int ii=0; ii<maxbits; ii++){   // pointer to the first quad
	  npatt = 0;
	  match = false;
	  firstquad = true;
	  for(int jj=ii; jj<maxbits-4+1;){ // pointer to check the helicity match after the first quad

		match = CheckPattern(helbit[jj], helbit[jj+1], helbit[jj+2], helbit[jj+3]);	
	
		if(match == false){
			quadstart = 0;
			break;
		}
	    npatt++;
	    if(firstquad){
		   quadstart = jj;
		   firstquad = false;
		}
		   jj += 4;		   
	  } // second pointer (jj)
	
	  if(quadstart!=0)break;
	}  // first pointer (ii)

    if(quadstart >0 && quadstart<=(maxbits-120) ) findpat = true;  // at least have 30 bits for register

    if(findpat){
	 unsigned long  pred_bit = 0;
	 int nquad = 0;
	 for(int ii=quadstart; ii<maxbits;){         // initialize fgShreg
		pred_bit = ranBit(helbit[ii],1);   
		nquad++;
		if(nquad>30 && (pred_bit != helbit[ii+4]) ){                  // check if helicity prediction is true
			 printf("The prediction helicity does not match the real one !!\n")
			 break;
	    }
		ii += 4;
	 }

	 printf("fgShreg = 0X%8x \n",fgShreg);
	 fgShreg_earlier = fgShreg;
	}
	else{
	 printf("Can't find the start of helicity quad !! \n");
	}
	return findpat;
}


int HelicityUpdateWin(Int_t past_hel[6]){
// check if the last 28 bits of the vtp_past_hel is update
// if it is updated, return the number of windows that is updated

    UInt_t helbit[28]={0};
	int bit_num = 0;
	int nbits = 0;
    for(int jj=0; jj<2; jj++){
        if(jj==0) nbits = 23; 
        if(jj==1) nbits = 5; 
        for(int nnbit=0; nnbit<nbits; nnbit++)
         {
            helbit[bit_num] = ( past_hel[jj]>>nnbit ) & 0x1;   // the bit 0 in vtp_past_hel[0] is the most recent helicity
            bit_num ++;
         }
    }
   
	int nupdate = 0;
	for(int ii=0; ii<30; ii++){
	  if( ( (fgShreg_earlier>>ii)&0x1 ) != helbit[ii]){
		nupdate++;
	  }
	}

	if(nupdate>0){
	 fgShreg_earlier = 0;
	 for(int ii=0; ii<30; ii++)
   	  fgShreg_earlier = ( helbit[29-ii] | (fgShreg_earlier<<1) ) & 0x3fffffff;
	}

	if(nupdate > 20) printf("%d helicity windows are missing !! \n",nupdate);
	return nupdate;
}

int PredictHelicity(Int_t past_hel[6]){
	
	int nupdate = 0;
	nupdate = HelicityUpdateWin(past_hel);   // check if the helicity is updated in vtp_past_hel
	if(nupdate==0)return -1;

	
	
}


/****************************************************************/
/* This is the random bit generator according to the Qweak      */
/* algorithm described in "Helicity Control Board User's        */
/* Guide (Draft 2)" by                                          */
/* R. Flood, S. Higgins, R. Suleiman, Feb 4, 2010.              */
/*                                                              */
/* Argument:                                                    */
/*     hRead = 0, 1, or 2                                       */
/*       0 or 1 = read helicity, so we update the seed          */
/*       2 means return new bit from the prediction             */
/*     which = 0 or 1                                           */
/*       0 then fgShreg_earlier is modified                       */
/*       1 then fgShreg is modified                               */
/* Return value:                                                */
/*        helicity (0 or 1)                                     */
/****************************************************************/

unsigned long ranBit(int hRead, int which) {

  unsigned long bit7, bit28, bit29, bit30;
  unsigned long newbit;

  if (which == 0) {

       bit7  = (fgShreg_earlier & 0x00000040) != 0;
       bit28 = (fgShreg_earlier & 0x08000000) != 0;
       bit29 = (fgShreg_earlier & 0x10000000) != 0;
       bit30 = (fgShreg_earlier & 0x20000000) != 0;
       newbit = (bit30 ^ bit29 ^ bit28 ^ bit7) & 0x1;
       fgShreg_earlier = ( (hRead == 2 ? newbit : hRead) | (fgShreg_earlier << 1 )) & 0x3FFFFFFF;

  } else {

       bit7  = (fgShreg & 0x00000040) != 0;
       bit28 = (fgShreg & 0x08000000) != 0;
       bit29 = (fgShreg & 0x10000000) != 0;
       bit30 = (fgShreg & 0x20000000) != 0;
       newbit = (bit30 ^ bit29 ^ bit28 ^ bit7) & 0x1;
       fgShreg = ( (hRead == 2 ? newbit : hRead) | (fgShreg << 1 )) & 0x3FFFFFFF;

  }

  return newbit;

};



