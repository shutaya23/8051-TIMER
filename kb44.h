#define KBINP P0
void delay1ms(int count);

int kbscan(){
	unsigned char kbp;
	unsigned char scanln[]={0xFE,0xFD,0xFB,0xF7};
	unsigned char row,col,kbtemp;
	kbp=0xFF;
	for(row=0;row<4;++row){
		KBINP=scanln[row];
		kbtemp=KBINP&0xF0;
		if(kbtemp != 0xF0){
			delay1ms(20);
			for(col=0;col<4;++col){
				if(kbtemp == ((scanln[col]&0x0F)*16)){
					kbp=row*4+col;
					while((KBINP&0xF0)!=0xF0);
			  }
		  }
	  }
  }	
  return kbp;
}

void delay1ms(int count){
	int i,j;
	for(i=0;i<count;i++)
		for(j=0;j<122;j++);
}