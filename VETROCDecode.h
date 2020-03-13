struct vetroc_data_struct
{
  unsigned int new_type;
  unsigned int type;
  unsigned int slot_id_hd;
  unsigned int slot_id_tr;
  unsigned int slot_id_evt;
  unsigned int n_evts;
  unsigned int blk_num;
  unsigned int blk_size;
  unsigned int n_words;
  unsigned int evt_num;
  unsigned int time_L;
  unsigned int time_H;
  unsigned int trigtime;
  unsigned int chan;
  unsigned int edge;
  unsigned int tdc;
}vetroc_data;

bool newhit = true;

void vetDataDecode(unsigned int data)
{
  int i_print = 0;
  static unsigned int type_last = 15;   /* initialize to type FILLER WORD */
  static unsigned int iword=0;

// Note,  vetroc_data_struct vetroc_data is global, see top of code

  if (i_print) printf("%3d: ",iword++);

  if( data & 0x80000000 )       /* data type defining word */
    {
      vetroc_data.new_type = 1;
      vetroc_data.type = (data & 0x78000000) >> 27;
    }
  else
    {
      vetroc_data.new_type = 0;
      vetroc_data.type = type_last;
    }

  switch( vetroc_data.type )
    {   
    case 0:     /* BLOCK HEADER */
      if(vetroc_data.new_type){
        vetroc_data.slot_id_hd = ((data) & 0x7C00000) >> 22; 
        vetroc_data.blk_num = (data >> 8 ) & 0x3FF;
        vetroc_data.n_evts = (data & 0xFF);
        if( i_print )
            printf("%8X - BLOCK HEADER - slot = %d   n_evts = %d   n_blk = %d\n",
           data, vetroc_data.slot_id_hd, vetroc_data.n_evts, vetroc_data.blk_num);
      }   
      break;
    case 1:     /* BLOCK TRAILER */
      vetroc_data.slot_id_tr = (data & 0x7C00000) >> 22; 
      vetroc_data.n_words = (data & 0x3FFFFF);
      if( i_print )
        printf("%8X - BLOCK TRAILER - slot = %d   n_words = %d\n",
           data, vetroc_data.slot_id_tr, vetroc_data.n_words);
      break;
    case 2:     /* EVENT HEADER */
      if( vetroc_data.new_type )
    {   
	  vetroc_data.slot_id_evt = (data >> 22) & 0x1F;
      //if(vetroc_data.slot_id_evt != vetroc_data.slot_id_hd)
        //printf("VETROC Warning: event slot id is not the same as the block slot id !\n");

      vetroc_data.evt_num = (data & 0x3FFFFF);
      if( i_print )
        printf("%8X - EVENT HEADER 1 - evt_num = %d\n", data, vetroc_data.evt_num);
    }   
      break;
    case 3:     /* TRIGGER TIME */
      if( vetroc_data.new_type )
    {
      vetroc_data.time_L = (data & 0xFFFFFF);
      if( i_print )
        printf("%8X - TRIGGER TIME lower - time = %08x\n", data, vetroc_data.time_L);
    }
      else
    {
      vetroc_data.time_H = (data & 0xFFFFFF);
      if( i_print )
        printf("%8X - TRIGGER TIME upper - time = %08x\n", data, vetroc_data.time_H);

	  if(vetroc_data.slot_id_evt == EPLANEA_SLOT){
	   	eplaneA_trigtime = (vetroc_data.time_H << 24) | vetroc_data.time_L;
	  }
	  if(vetroc_data.slot_id_evt == EPLANEB_SLOT){ 
		eplaneB_trigtime = (vetroc_data.time_H << 24) | vetroc_data.time_L;
	  }
	  if(vetroc_data.slot_id_evt == EPLANEC_SLOT){
	 	 eplaneC_trigtime = (vetroc_data.time_H << 24) | vetroc_data.time_L;
	  }
	  if(vetroc_data.slot_id_evt == EPLANED_SLOT){
	   	eplaneD_trigtime = (vetroc_data.time_H << 24) | vetroc_data.time_L;
	  }
    }
      break;

    case 8:         /* TDC Hit */
	  vetroc_data.edge = (data >> 26) &0x1;
	  vetroc_data.chan = (data >> 16) & 0xFF;
	  vetroc_data.tdc = data & 0xFFFF; 

	  if(vetroc_data.chan>127)
		printf("VETROC Warning: the channel number %d is bigger than 128, data = %8X !\n",vetroc_data.chan,data);

      if(vetroc_data.slot_id_evt == EPLANEA_SLOT){    /* vetroc plane A */
		 if(vetroc_data.edge==0){
		   newhit=true;
		   eplaneA_chan[eplaneA_nhits] = vetroc_data.chan;
		   eplaneA_rt[eplaneA_nhits] = vetroc_data.tdc;
		   eplaneA_nhits++;
		   if(eplaneA_nhits>VETROC_MAXHIT)
			 printf("VETROC Warning: too many hits (%d) in plane A !!\n",eplaneA_nhits);
		 }
		 else{
		   if(newhit==true){
			 eplaneA_ft[eplaneA_nhits-1] = vetroc_data.tdc;
			 newhit = false;
		   }
		 }
	   }

      if(vetroc_data.slot_id_evt == EPLANEB_SLOT){    /* vetroc plane B */
		 if(vetroc_data.edge==0){
		   newhit=true;
		   eplaneB_chan[eplaneB_nhits] = vetroc_data.chan;
		   eplaneB_rt[eplaneB_nhits] = vetroc_data.tdc;
		   eplaneB_nhits++;
		   if(eplaneB_nhits>VETROC_MAXHIT)
			 printf("VETROC Warning: too many hits (%d) in plane B !!\n",eplaneB_nhits);
		 }
		 else{
		   if(newhit==true){
			 eplaneB_ft[eplaneB_nhits-1] = vetroc_data.tdc;
			 newhit = false;
		   }
		 }
	   }

      if(vetroc_data.slot_id_evt == EPLANEC_SLOT){    /* vetroc plane C */
		 if(vetroc_data.edge==0){
		   newhit=true;
		   eplaneC_chan[eplaneC_nhits] = vetroc_data.chan;
		   eplaneC_rt[eplaneC_nhits] = vetroc_data.tdc;
		   eplaneC_nhits++;
		   if(eplaneC_nhits>VETROC_MAXHIT)
			 printf("VETROC Warning: too many hits (%d) in plane C !!\n",eplaneC_nhits);
		 }
		 else{
		   if(newhit==true){
			 eplaneC_ft[eplaneC_nhits-1] = vetroc_data.tdc;
			 newhit = false;
		   }
		 }
	   }

      if(vetroc_data.slot_id_evt == EPLANED_SLOT){    /* vetroc plane D */
		 if(vetroc_data.edge==0){
		   newhit=true;
		   eplaneD_chan[eplaneD_nhits] = vetroc_data.chan;
		   eplaneD_rt[eplaneD_nhits] = vetroc_data.tdc;
		   eplaneD_nhits++;
		   if(eplaneD_nhits>VETROC_MAXHIT)
			 printf("VETROC Warning: too many hits (%d) in plane D !!\n",eplaneD_nhits);
		 }
		 else{
		   if(newhit==true){
			 eplaneD_ft[eplaneD_nhits-1] = vetroc_data.tdc;
			 newhit = false;
		   }
		 }
	   }

	  if( i_print ){
		printf("%8X - TDC Hits, Edge = %d, chan = %d, TDC = %d\n",data, vetroc_data.edge,vetroc_data.chan,
				vetroc_data.tdc);
	  }
      break;

    case 14:        /* DATA NOT VALID (no data available) */
      if( i_print )
    printf("%8X - DATA NOT VALID = %d\n", data, vetroc_data.type);
      break;

    case 15:        /* FILLER WORD */
      if( i_print )
        printf("%8X - FILLER WORD = %d\n", data, vetroc_data.type);
      break;
   }   
  
   type_last = vetroc_data.type;

}
