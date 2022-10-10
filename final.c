#include <reg51.h>
#include <intrins.h>
#include <string.h>
#include "kb44.h"
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
void delay(int count);
void delaylcd(int cnt);
void show(int a,int b);
void init_lcd();
void lcd_data(char data1);
void lcd_cmd(char cmd);
void disp_str(char addr1,char *str,int delay);
void RR_str(char addr1,char *str,char offset);
void clear();
void sound(int j);
unsigned int table[7]={d0,re,mi,fa,so,la,si};
unsigned int tmp;
unsigned int code music[5]={d0,si,d0,si,si};
unsigned char location[]={0,1,2,3,4,5,6,7};
char num[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,~0x88,0x7c,0x39,0x5e,0x79,0x71};//0~f
char ledd[]={~0x00,~0x80,~0xc0,~0xe0,~0x70,~0x38,~0x1c,~0x0e,~0x07,~0x03,~0x01};//右璇
char code r1[]={"U0424041            "};
char code nothing[]="                  ";
char hour=0,min=0,sec=0,i=0,exit;
bit mask=0;

void main(){
 int i=0,j;
 int kbin;
 init_lcd();
 P2=0;
 while(1){
	kbin=kbscan();
	exit=0;
	EA=1;//致能
  ET1=0;//計時器T1
  EX0=1;//外部中斷0
  IE=0X88;//ET1=1
  TMOD=0x11;//計時器模組皆模式一
  TH1=(65536-10000)/256; 
  TL1=(65536-10000)%256;
	TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
  TR1=0;//計時器不啟動

	 
	while(kbin==0){ 
		clear();
		disp_str(0x80,r1,100);
		disp_str(0xc0,r1,100);
		sound(4);
		break;
 }
 	while(kbin==1){ 
		clear();
		sound(1);
		break;
 }
	while(kbin==2){
		if(exit==1){
			break;}
		IE=0X83;;
		TR0=1;
		P1=0X00;
		P2=num[min/10];
		delay(50);	
		P1=0X01;
		P2=num[min%10];
		delay(50);	
		P1=2;
		P2=0X40;
		delay(50);	
		P1=0X03;
		P2=num[sec/10];
		delay(50);
		P1=0X04;
		P2=num[sec%10];
		delay(50);
	}
	while(kbin==3){
		if(exit==1){
			break;}
		IE=0X83;
		for(j=0;j<strlen(r1);j++){
			if(exit==1){
			break;}
			RR_str(0xc0,r1,j);
			RR_str(0x80,r1,j);
			delaylcd(500);
		}
	}


 }
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

void RR_str(char addr1,char *str,char offset){
	char i;
	char *start=str;
	lcd_cmd(addr1);
	str+=20-offset;
	while(*str!=0)
		lcd_data(*str++);
	for(i=0;i<20-offset;i++)
		lcd_data(start[i]);
}
void EXT0(void) interrupt 0{//中斷0
	int m;
	for(m=0;m<11;m++){
		P2=ledd[m];
		delay1ms(100);	
	}
	IE=0X80;
	exit=1;
 }
void T0_int(void) interrupt 1 /* T0=100us */ //計時
{
  TH0=(65536-50000)/256;//50ms
  TL0=(65536-50000)%256;
  i++;
	if(i==20){ //1sec=50*20ms
  	i=0;
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
} /* T0_int */	
 
void T1_int(void) interrupt 3 using 1  //蜂鳴器
 {
 TH1=tmp/256; //song[j];
 TL1=tmp%256; //song[j+1];
 SPK = !SPK;
} /* T1_int */ 
 

void sound(int j){
 int i;
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
 }
 