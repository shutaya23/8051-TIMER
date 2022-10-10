/*ADC0831轉換程式，請調整精密可變電阻,或有加裝其它感測器(如光敏電阻,濕度感測器,等等)，看看會如何變化*/

#include <reg51.h>
#include <intrins.h>

sbit SCL2=P1^3;		//SCL2定義為P1的第3腳，連接ADC0831SCL腳
sbit SDA2=P1^4;		//SDA2定義為P1的第4腳，連接ADC0831SDA腳
sbit CS2=P1^6;		//CS2定義為P1的第4腳，連接ADC0831CS腳
sbit RST = P1^5;// DS1302片選,因這兩晶片採用同一資料腳（SDA，SCL），所以我們要定義它，將DS1302片選關掉，否則發生衝突


code unsigned char table[]=
			{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
			//七段顯示器 0-9表
unsigned char l_tmpdate[]={0,0,0};//定義數組變量
void delay();//延時子函數，5個空指令
void display(unsigned char *lp,unsigned char lc);//數字的顯示函數；lp為指向數組的位址，lc為顯示的個數
unsigned char ad0831read(void);	//定義該函數為讀取ADC0831的資料


void main(void)     //程式開始
{
	unsigned char i=254,tmp;
	RST=0;	
	while(1){
		i++;
		if(i==255){		
			tmp=ad0831read();		//這裡為循環255個週期讀取一次0831，因CPU執行比較快，沒必要每次循環都去讀取
			i=0;		
			l_tmpdate[0]=tmp/100;
			tmp=tmp%100;
			l_tmpdate[1]=tmp/10;
			l_tmpdate[2]=tmp%10;		//因讀到的資料為8位的二進製數，即0~255，我們將其分開放入l_tmpdate中
		}
		display(l_tmpdate,3);		//輸出顯示
	}
}
void display(unsigned char *lp,unsigned char lc)//顯示
{
	unsigned char i;		//定義變量
	P2=0;					//清除為0
	P1=P1&0xF8;				//將P1的前3位輸出0，對應138輸入腳，全0為第一位七段顯示器
	for(i=0;i<lc;i++){		//循環顯示
	P2=table[lp[i]];		//查表法得到要顯示數字
	delay();				//延時5個空指令	
	if(i==7)				//檢測顯示完8位否，完成直接退出，不讓P1再加1，否則進位影響到第四位資料
		break;
	P2=0;					//清除為0，準備顯示下位
	P1++;					//下一位七段顯示器
	}
}
void delay(void)								//空5個指令
{
	_nop_();_nop_();_nop_();_nop_();_nop_();
}

unsigned char ad0831read(void)//請先解ADC0831轉換器的串列協議，再來讀本函數，
{								//本函數是類比0831的串列協議進行的，
								//以後，對於一個硬體這樣的就簡單多
	unsigned char i=0,tmp=0;
		SDA2=1;
		CS2=0;
		_nop_();
		_nop_();
		SCL2=0;
		_nop_();
		_nop_();
		SCL2=1;
		_nop_();
		_nop_();
		SCL2=0;
		_nop_();
		_nop_();
		SCL2=1;
		_nop_();
		_nop_();
		SCL2=0;
		_nop_();
		_nop_();
		for(i=0;i<8;i++){
			tmp<<=1;
			if(SDA2)
				tmp++;			
			SCL2=1;
			_nop_();
			_nop_();
			SCL2=0;
			_nop_();
			_nop_();
		}
		CS2=1;
	return tmp;
}


