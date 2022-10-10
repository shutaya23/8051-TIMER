#include "reg51.h"
#include "kb44.h"
unsigned int i=0;
void tk();
int u=0;

char table1[]={~0xc0,~0xf9,~0xa4,~0xb0,~0x99, /* 0,1,2,3,4 */
 ~0x92,~0x82,~0xf8,~0x80,~0x90};/* 5,6,7,8,9 */

 char ledd[]={~0x00,~0x80,~0xc0,~0xe0,~0x70,~0x38,~0x1c,~0x0e,~0x07,~0x03,~0x01};
int hour=0,min=0,sec=0;
int tickhour=0,tickmin=0;
int timeup=0;
bit mask=0;
void time()
 { 
 void delay1(int); 
 EA=1;
 ET0=1;
 EX0=1;
 IE=0X83;
 TMOD=0x01;
 TH0=(65536-50000)/256;
 TL0=(65536-50000)%256;
 TR0=1; 
 while(1)
 {
	if (sec==0&&hour==tickhour&&min==tickmin){
	timeup=1;
		break;
	}
 if (u==1){
	break;
 }

 P1=0;
 P2=table1[hour/10];
 delay1(50);
 P1=1;
 P2=table1[hour%10];
 delay1(50);

	if (sec%2==0){
		P1=2;
		P2=0X40; //ob1
		delay1(50);
	}

 P1=3;
 P2=table1[min/10];
 delay1(50); 
 P1=4;
 P2=table1[min%10];
 delay1(50);
 }
 } /* main */


 void EXT0(void) interrupt 0{//い耞
	int m;
	for(m=0;m<11;m++){
		P2=ledd[m];
	delay1ms(100);	
	}
	u=1;
	IE=0X80;//
 }

void delay1(int count)
 {
 int i;
 for(i=0;i<count;i++)
 ;
 } 

 void delaylcd(int cnt){
 int i,j;
 for(i=0;i<cnt;i++)
 for(j=0;j<122;j++)
 ;
 }
void settime(int qq){
 int i,j,k,l,m,n; //i计 j计 kだ计 lだ计
	n=0;
 EA=1;
 EX0=1;
 IE=0X80;
 TMOD=0x01;
 while(1){
	P1=0;
	P2=0X80;
	delay1(100);
	n++;
	 if(n>560){
		 tk();
		 n=n-560;
	 }
	P1=2;
	P2=0x40;
	i=kbscan();
 if (i<3){
	break;
 }
 delay1(100);
 n++;
 }
 while(1){
	P1=0;
	P2=table1[i];
	delay1(100);
	P1=1;
	P2=0X80;
	delay1(100);
	P1=2;
	P2=0x40;
	 n=n+2;
	 if(n>560){
		 tk();
		 n=n-560;
	 }
	j=kbscan();
 if (i==2&&j<4){
	break;
 }
 else if (i<2&&j<10){
	break;
 }
 delay1(100);
 n++;
 }
 while(1){
	P1=0;
	P2=table1[i];
	delay1(100);
	P1=1;
	P2=table1[j];
	delay1(100);
	P1=2;
	P2=0x40;
	delay1(100);
	P1=3;
	P2=0x80;
	 n=n+3;
	 if(n>560){
		 tk();
		n=n-560;
	 }
	k=kbscan();
 if (k<6){
	break;
 }
 delay1(100);
 n++;
 }
 while(1){
	P1=0;
	P2=table1[i];
	delay1(100);
	P1=1;
	P2=table1[j];
	delay1(100);
	P1=2;
	P2=0x40;
	delay1(100);
	P1=3;
	P2=table1[k];
	delay1(100);
	P1=4;
	P2=0x80;
	 n=n+4;
	 if(n>560){
		 tk();
		 n=n-560;
	 }
	l=kbscan();
 if (l<10){
	break;
 }
 delay1(100);
 }
 while(1){
	P1=0;
	P2=table1[i];
	delay1(100);
	P1=1;
	P2=table1[j];
	delay1(100);
	P1=2;
	P2=0x40;
	delay1(100);
	P1=3;
	P2=table1[k];
	delay1(100);
	P1=4;
	P2=table1[l];
	 n=n+4;
	 if(n>560){
		 tk();
		 n=n-560;
	 }	 
	m=kbscan();
 if (m<16){
	break;
 }
 delay1(100);
 n++;
 }
 if (qq==1){
	hour=i*10+j;
  min=k*10+l;
 }
 else{
 tickhour=i*10+j;
 tickmin=k*10+l;
 }
}
void T0_int() interrupt 1 /* T0=100us */
 {
 TH0=(65536-50000)/256;
 TL0=(65536-50000)%256;
 i++;
 if(i==20){
	i=0;
	sec++;
	if(sec>59)
	{
		min++;
		sec=0;		
	if(min>59)
	{
		hour++;
		min=0;		
	if(hour>23)
		hour=0;
	}
	}
	}
 } /* T0_int1 */ 
 void tk(){
	sec++;
	if(sec>59)
	{
		sec=0;
		min++;
	if(min>59)
	{
		min=0;
		hour++;
	if(hour>23)
		hour=0;
	}
	}
}