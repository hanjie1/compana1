const int maxbits = 173;            // number of helicity windows in vtp_past_hel
const unsigned int hel_pat1[4] = {1,0,0,1};  // helicity pattern 1: +--+
const unsigned int hel_pat2[4] = {0,1,1,0};  // helicity pattern 2: -++-
UInt_t fgShreg = 0;
UInt_t fgShreg_earlier = 0;         // it's actually the 30 bits for the previsous event

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

UInt_t ranBit(UInt_t hRead, int which) {

  UInt_t bit7, bit28, bit29, bit30;
  UInt_t newbit;

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

UInt_t InvertBit(UInt_t helbit){
  	UInt_t invbit=2;
	if(helbit==0)invbit=1;
	if(helbit==1)invbit=0;

	if(invbit==2) printf("InvertBit: somehting wrong with the input bit !\n");
	return invbit;
}

bool CheckPattern(UInt_t bit0, UInt_t bit1, UInt_t bit2, UInt_t bit3){
    bool match = false;

	if( (bit0==hel_pat1[0]) && (bit1==hel_pat1[1]) && (bit2==hel_pat1[2]) && (bit3==hel_pat1[3]) ) // pattern 1
	    match = true;

	if( (bit0==hel_pat2[0]) && (bit1==hel_pat2[1]) && (bit2==hel_pat2[2]) && (bit3==hel_pat2[3]) ) // pattern 2
		match = true;

	return match;
}

bool FindQuad(Int_t past_hel[6], int *helpos){
//  Find the first quad; initialize fgShreg; return the current helicity is in which window of that pattern

    *helpos = 0; 
    UInt_t helbit[maxbits]={0};
	int bit_num = maxbits-1;
    for(int jj=0; jj<6; jj++){
        int nbits = 30; 
        if(jj==0) nbits = 23; 
        for(int nnbit=0; nnbit<nbits; nnbit++)
         {
		    UInt_t tmpbit = ( past_hel[jj]>>nnbit ) & 0x1;   // the bit 0 in vtp_past_hel[0] is the most recent helicity
            helbit[bit_num] = InvertBit(tmpbit);
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
	 UInt_t  pred_bit = 0;
	 int nquad = 0;
	 int nn = 0;
	 for(nn=quadstart; nn<maxbits;){         // initialize fgShreg
		if(nquad<=30)pred_bit = ranBit(helbit[nn],1);   
		if(nquad>=30)pred_bit = ranBit(2,1);   
		nquad++;
		if(nquad>30 && (pred_bit != helbit[nn+4]) && (nn+4)<maxbits ){                  // check if helicity prediction is true
			 printf("The prediction helicity does not match the real one !!\n");
			 break;
	    }
		nn += 4;
	 }
	 *helpos = 4-(nn-maxbits+1);

	 fgShreg_earlier = fgShreg;
	}
	else{
	 printf("Can't find the start of helicity quad !! \n");
	}
	return findpat;
}


int HelicityUpdateWin(Int_t pre_past_hel[6], Int_t past_hel[6], Int_t pre_last_mps_time, Int_t last_mps_time){
// check if the last 28 bits of the vtp_past_hel is update
// if it is updated, return the number of windows that is updated

	int bit_num = 0;
	int nbits = 0;

	ULong64_t helbit[3];      // 3 words to save the 173 helicity bits; the 0 bit of helbit[0] is the most recent one
	ULong64_t pre_helbit[3];


    helbit[0]=0ull;
    helbit[1]=0ull;
    helbit[2]=0ull;
    pre_helbit[0]=0ull;
    pre_helbit[1]=0ull;
    pre_helbit[2]=0ull;


	int nnw = 0;
    for(int ii=0; ii<6; ii++){
	  	nbits = 30;
        if(ii==0) nbits = 23; 
        for(int nnbit=0; nnbit<nbits; nnbit++)
         {
		   	ULong64_t tmpbit;
			tmpbit = ( past_hel[ii]>>nnbit ) & 0x1;
			tmpbit = InvertBit( tmpbit );
            helbit[nnw] = (tmpbit << bit_num) | helbit[nnw];   // the bit 0 in vtp_past_hel[0] is the most recent helicity

			tmpbit = ( pre_past_hel[ii]>>nnbit ) & 0x1;
			tmpbit = InvertBit( tmpbit );
            pre_helbit[nnw] = (tmpbit << bit_num) | pre_helbit[nnw];   // the bit 0 in vtp_past_hel[0] is the most recent helicity

            bit_num ++;
		    if( (bit_num%64) == 0 ){
		     	nnw++;
			    bit_num = 0;
		    }
         }
    }

	int nupdate = -1;
	if( (helbit[0]==pre_helbit[0]) && (helbit[1]==pre_helbit[1]) && (helbit[2]==pre_helbit[2]) ) nupdate = 0;
	if(nupdate == 0 ){   // there is no update of vtp_past_hel
	  if(last_mps_time > pre_last_mps_time) return nupdate;     // last_mps_time should increase
	  else{
		printf("vtp_past_hel[6] are not updated but the current last_mps_time %d <= the previous one %d !!\n",last_mps_time,pre_last_mps_time);
		return -1;
	  }
	}

	ULong64_t bitMark = 0;
    for(int ii=1; ii<3; ii++){
	  	bitMark = ( 0x1 << (ii-1)) | bitMark;
		ULong64_t tmphel0 = ( helbit[0]>>ii ) | ( (helbit[1] & bitMark) << (64-ii) );
		ULong64_t tmphel1 = ( helbit[1]>>ii ) | ( (helbit[2] & bitMark) << (64-ii) );
		ULong64_t tmphel2 = helbit[2]>>ii;
		if( (tmphel0==pre_helbit[0]) && (tmphel1==pre_helbit[1]) && ((tmphel2 & pre_helbit[2]) == tmphel2) && (pre_helbit[2]!=0x1fffffffffff) ){
			nupdate = ii;
			break;
		 }
	}


	if(nupdate > 20) printf("%d helicity windows are missing !! \n",nupdate);
	return nupdate;
}

int GetHelicity(int *helpos){
  	int tmp_helpos = *helpos;
	if( tmp_helpos>3 ) return -1;          // helpos should be 0,1,2,3

	int newbit = 0;
	int startbit = fgShreg & 0x1;      // pattern start bit
	if(startbit==1 )newbit = hel_pat1[tmp_helpos];
	if(startbit==0 )newbit = hel_pat2[tmp_helpos];

	if(tmp_helpos==3){
	  ranBit(2,1);
	  tmp_helpos = 0;
	}
	else tmp_helpos++;

	*helpos = tmp_helpos;

	return newbit;
}

int PredictHelicity(int nupdate, int *helpos){
	
    int newpos = nupdate + *helpos-1;
	int nupdate_seed = newpos/4;
	int new_helpos = newpos%4;

	int startbit = 0;
	int newbit = 0;
	if(nupdate_seed==0) startbit = fgShreg & 0x1;
	else{
	  for(int ii=0; ii<nupdate_seed; ii++) startbit=ranBit(2,1);
	}
	
	if(new_helpos==0 ) {
	  newbit = startbit;
	  new_helpos++;
	}
	else newbit = GetHelicity( &new_helpos );

	*helpos = new_helpos;

	return newbit;
	
}


