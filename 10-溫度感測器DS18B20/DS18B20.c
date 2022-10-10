#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char

#define uint unsigned int

sbit DS=P3^3;           //�w�qDS18B20�ɭ�

int temp;             

uchar flag1;            

void display(unsigned char *lp,unsigned char lc);//�Ʀr����ܨ�ơFlp�����V�Ʋժ���}�Alc����ܪ��Ӽ�

void delay();//���ɤl��ơA5�Ӫū��O

code unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0x08,0x00};
			//�C�q��ܾ� 0-9 - _ �� ��

unsigned char l_tmpdate[8]={0,0,10,0,0,0,0,0};//�w�q�Ʋ��ܶq�A�õ���1�A2�A3�A4�A5�A6�A7�A8�A�N�O���{����ܪ��K�Ӽ�
int tmp(void);
void tmpchange(void);
void tmpwritebyte(uchar dat);
uchar tmpread(void);
bit tmpreadbit(void);
void dsreset(void);
void delayb(uint count);


void main()               //�{���}�l

{
uchar i;
int l_tmp;
  while(1)
  {
  tmpchange();      //�ū��ഫ
	l_tmp=tmp();
	if(l_tmp<0)		
	l_tmpdate[0]=10;	//�P�_�ū׬��t�ūסA�e���["-"
	else
	{
	l_tmpdate[0]=temp/1000;	//��ܦʦ�A�o�̥�1000�A�O�]���ڭ̤��e�ĥH10��F
	if(l_tmpdate[0]==0)		
		l_tmpdate[0]=12;//�P�_�ū׬����ūץB�S���W�ʡA�e�������
	}
l_tmp=temp%1000;
l_tmpdate[1]=l_tmp/100;//�I���Q��
l_tmp=l_tmp%100;
l_tmpdate[2]=l_tmp/10;//�I���Ӧ�
l_tmpdate[3]=11;	
l_tmpdate[4]=l_tmp%10;//�I���p�ƲĤ@��

for(i=0;i<10;i++){		//�`����X10���A���ɫG��
display(l_tmpdate,5);   

   }              
}
  

}

void display(unsigned char *lp,unsigned char lc)//���
{
	unsigned char i;		//�w�q�ܶq
	P2=0;					//P2����X
	P1=P1&0xF8;				//�NP1���e3���X0�A����138��J�}�A��0���Ĥ@��C�q��ܾ�
	for(i=0;i<lc;i++){		//�`�����
	P2=table[lp[i]];		//�d��k�o��n��ܼƦr
	delay();				//����5�Ӫū��O	
	if(i==7)				//�˴���ܧ�8��_�A���������h�X�A����P1�A�[1�A�_�h�i��v�T��ĥ|����
		break;
	P2=0;					//�M����0�A�ǳ���ܤU�@���
	P1++;					//�U�@��C�q��ܾ�
	}
}
void delay(void)								//��5�ӫ��O
{
_nop_();_nop_();_nop_();_nop_();_nop_();
}
void delayb(uint count)      //delay

{

  uint i;

  while(count)

  {

    i=200;

    while(i>0)

    i--;

    count--;

  }

}


void dsreset(void)       //DS18B20��l��

{

  uint i;                

  DS=0;

  i=103;

  while(i>0)i--;

  DS=1;

  i=4;

  while(i>0)i--;

}

 

bit tmpreadbit(void)       // Ū�@��

{

   uint i;

   bit dat;

   DS=0;i++;          //�p���ɤ@�U

   DS=1;i++;i++;

   dat=DS;

   i=8;while(i>0)i--;

   return (dat);

}

 

uchar tmpread(void)   //Ū�@�Ӧ줸��

{

  uchar i,j,dat;

  dat=0;

  for(i=1;i<=8;i++)

  {

    j=tmpreadbit();

    dat=(j<<7)|(dat>>1);   //Ū�X����Ƴ̧C��b�̫e���A�o�˭�n//�@�Ӧ줸�զbDAT��

  }

  return(dat);             //�N�@�Ӧ줸�ո�ƪ�^

}

 

void tmpwritebyte(uchar dat)   

{                           //�g�@�Ӧ줸�ը�DS18B20��

  uint i;

  uchar j;

  bit testb;

  for(j=1;j<=8;j++)

  {

    testb=dat&0x01;

    dat=dat>>1;

    if(testb)     // �g1����

    {

      DS=0;

      i++;i++;

      DS=1;

      i=8;while(i>0)i--;

    }

    else

    {

      DS=0;       //�g0����

      i=8;while(i>0)i--;

      DS=1;

      i++;i++;

    }

  }

}

 

void tmpchange(void)  //�o�e�ū��ഫ�R�O

{

  dsreset();             //��l��DS18B20

  delayb(1);             //����

  tmpwritebyte(0xcc);  // ���L�ǦC���R�O

  tmpwritebyte(0x44);  //�o�e�ū��ഫ�R�O

}

int tmp()               //��o�ū�

{

  float tt;

  uchar a,b;

  dsreset();

  delayb(1);

  tmpwritebyte(0xcc);   

  tmpwritebyte(0xbe);    //�o�eŪ����ƩR�O

  a=tmpread();          //�s��Ū��Ӧ줸�ո��

  b=tmpread();

  temp=b;

  temp<<=8;             

  temp=temp|a;         //��줸�զX���@�Ӿ㫬�ܶq�C

  tt=temp*0.0625;       //�o��u��Q�i�s�ū׭ȡA�]��DS18B20

//�i�H��T��0.0625�סA�ҥHŪ�^��ƪ��̧C��N���O

//0.0625�סC

  temp=tt*10+0.5;   //��j�Q���A�o�˰����ت��N�p���I��Ĥ@��

//�]�ഫ���i��ܼƦr�A�P�ɶi��@�ӥ|�ˤ��J�ާ@�C

  return temp;     //��^�ū׭�

}

void readrom()          //read the serial Ū���ū׷P�������ǦC��

{                      //���{�����S���Ψ즹���

  uchar sn1,sn2;

  dsreset();

  delayb(1);

  tmpwritebyte(0x33);

  sn1=tmpread();

  sn2=tmpread();

}

void delay10ms()            
  {

    uchar a,b;

    for(a=10;a>0;a--)

      for(b=60;b>0;b--);

   }

