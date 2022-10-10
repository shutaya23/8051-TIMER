#include <reg51.h>
#include <intrins.h>
#include "time.h"
#include <string.h>
sbit SPK=P3^4;
#define DBPORT P2
sbit LCD_RS=P1^0;
sbit LCD_RW=P1^1;
sbit LCD_E=P1^2;
#define s 65536
#define t 12000000/12/2
#define d0 s-t/262
#define re s-t/294
#define mi s-t/330
#define fa s-t/349
#define so s-t/392
#define la s-t/440
#define si s-t/494
#define ss s-t/1
void delay(int count);
void delay10ms(int count);
void delaylcd(int cnt);
void show(int a,int b);
void init_lcd();
void lcd_data(char data1);
void lcd_cmd(char cmd);
void disp_str(char addr1,char *str,int delay);
void clear();
void sound(int j);
unsigned int table[7]={d0,re,mi,fa,so,la,si};
unsigned int tmp;
unsigned int code music[5]={d0,si,d0,si,si};
unsigned char location[]={0,1,2,3,4,5,6,7};
char num[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,~0x88,0x7c,0x39,0x5e,0x79,0x71};
char code menu[]="WELCOME ALARM";
char code menu2[]="CLOCK SIMULATION";
char code ring[]="ring sound";
char code nothing[]="                  ";
char code set_time[]={"SET TIME"};
char code current_time[]={"CURRENT TIME"};
char code set_clock[]={"SET CLOCK"};
char code r1[]={"U0424019"};
char code r2[]={"U0424041"};
char code rer[]={"CREATORS"};

void main(){
 int i=0;
 int kbin;
 P2=0X00;
 init_lcd();
 disp_str(0x80,menu,0);
 disp_str(0xc0,menu2,0);
 P2=0X00;
 while(1){
	delay1ms(1);
	i++;
	if (i>70){
	tk();
	i=i-720;
	}  
 kbin=kbscan(); 
 u=0;
 EA=1;//致能
 ET1=0;//計時器T1
 EX0=1;//外部中斷1
 IE=0X88;//ET1=1
 TMOD=0x10;//計時器模組
 TH1=(65536-10000)/256; 
 TL1=(65536-10000)%256;
 TR1=0;//停?I-pRE

 while(kbin==0){ 
 clear();
 disp_str(0x80,ring,100);
 sound(4);
 break;
 }
 while(kbin==1){ 
 clear();
 disp_str(0x80,current_time,0);
 time();
 sound(1);
 P2=0;
 IE=0X89;
 if (u==1){
	 u=0;
	 clear();	 
	 break;
 }
 if (timeup==1){
	 timeup=0;
 }
 }
 while(kbin==2){
 clear();
 disp_str(0x80,set_time,0);	 
 settime(1);
 sound(1);
 P1=0;
 P2=0;
 clear();	 
 break;
 }
 while(kbin==6){
 clear();
 disp_str(0x80,menu,0);
 disp_str(0xc0,menu2,0);
 P2=0X00;
 IE=0X89;
 break;
 }
 while(kbin==3){
 clear();	 
 disp_str(0xc0,set_clock,0);
 settime(2);
 clear();
 P1=0;
 P2=0;
 	 
 break;
 }
while(kbin==5){
 clear();
 disp_str(0x80,rer,100);
 sound(1);
 delay(5000);
 clear();
 disp_str(0x80,r1,100);
 sound(2);
 disp_str(0xc0,r2,100);	
 sound(3);
 break;
}

while(kbin==4){
 clear();
 break;
}
}
 
}
 void show(int a,int b){
 P2=num[a];
 P1=location[b];
 delay1ms(1);
 } 
 void delay(int count){
 int i;
 for(i=0;i<count;i++);
 }
 
 void init_lcd(){
 delaylcd(5);
 lcd_cmd(0x3f);
 lcd_cmd(0xe);
 lcd_cmd(0x1);
 delaylcd(2);
 lcd_cmd(0x80);
 } 
 
void clear(){
 init_lcd();
 disp_str(0x80,nothing,0);
 disp_str(0xc0,nothing,0);
}
 
 void lcd_data(char data1){
 int i;
 DBPORT=data1;
 LCD_RS=1;LCD_RW=0;LCD_E=1;
 for(i=0;i<40;++i);
 LCD_RS=1;LCD_RW=0;LCD_E=0;
 }
 void lcd_cmd(char cmd){
 int i;
 DBPORT=cmd;
 LCD_RS=0;LCD_RW=0;LCD_E=1;
 for(i=0;i<40;++i);
 LCD_RS=0;LCD_RW=0;LCD_E=0; 
 } 
 void disp_str(char addr1,char *str,int delay){
 lcd_cmd(addr1);
 while(*str!=0){
 lcd_data(*str++);
 P2=0;
 delaylcd(delay);}

 }
 
 void T1_int(void) interrupt 3 using 1
 {
 TH1=tmp/256; //song[j];
 TL1=tmp%256; //song[j+1];
 SPK = !SPK;

} /* T1_int */ 
void sound(int j){

 ET1=1;
 for(i=0;i<j;i++)
 { tmp=music[4];
 TR1=1;
 delay(4000);
 TR1=0;
 delay(4000);
 delay(1280);
 }
 TR1=0;
 IE=0X89;
 ET1=0; 
 }
