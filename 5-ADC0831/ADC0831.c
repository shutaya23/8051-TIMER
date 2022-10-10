/*ADC0831�ഫ�{���A�нվ��K�i�ܹq��,�Φ��[�˨䥦�P����(�p���ӹq��,��׷P����,����)�A�ݬݷ|�p���ܤ�*/

#include <reg51.h>
#include <intrins.h>

sbit SCL2=P1^3;		//SCL2�w�q��P1����3�}�A�s��ADC0831SCL�}
sbit SDA2=P1^4;		//SDA2�w�q��P1����4�}�A�s��ADC0831SDA�}
sbit CS2=P1^6;		//CS2�w�q��P1����4�}�A�s��ADC0831CS�}
sbit RST = P1^5;// DS1302����,�]�o�ⴹ���ĥΦP�@��Ƹ}�]SDA�ASCL�^�A�ҥH�ڭ̭n�w�q���A�NDS1302���������A�_�h�o�ͽĬ�


code unsigned char table[]=
			{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
			//�C�q��ܾ� 0-9��
unsigned char l_tmpdate[]={0,0,0};//�w�q�Ʋ��ܶq
void delay();//���ɤl��ơA5�Ӫū��O
void display(unsigned char *lp,unsigned char lc);//�Ʀr����ܨ�ơFlp�����V�Ʋժ���}�Alc����ܪ��Ӽ�
unsigned char ad0831read(void);	//�w�q�Ө�Ƭ�Ū��ADC0831�����


void main(void)     //�{���}�l
{
	unsigned char i=254,tmp;
	RST=0;	
	while(1){
		i++;
		if(i==255){		
			tmp=ad0831read();		//�o�̬��`��255�Ӷg��Ū���@��0831�A�]CPU�������֡A�S���n�C���`�����hŪ��
			i=0;		
			l_tmpdate[0]=tmp/100;
			tmp=tmp%100;
			l_tmpdate[1]=tmp/10;
			l_tmpdate[2]=tmp%10;		//�]Ū�쪺��Ƭ�8�쪺�G�i�s�ơA�Y0~255�A�ڭ̱N����}��Jl_tmpdate��
		}
		display(l_tmpdate,3);		//��X���
	}
}
void display(unsigned char *lp,unsigned char lc)//���
{
	unsigned char i;		//�w�q�ܶq
	P2=0;					//�M����0
	P1=P1&0xF8;				//�NP1���e3���X0�A����138��J�}�A��0���Ĥ@��C�q��ܾ�
	for(i=0;i<lc;i++){		//�`�����
	P2=table[lp[i]];		//�d��k�o��n��ܼƦr
	delay();				//����5�Ӫū��O	
	if(i==7)				//�˴���ܧ�8��_�A���������h�X�A����P1�A�[1�A�_�h�i��v�T��ĥ|����
		break;
	P2=0;					//�M����0�A�ǳ���ܤU��
	P1++;					//�U�@��C�q��ܾ�
	}
}
void delay(void)								//��5�ӫ��O
{
	_nop_();_nop_();_nop_();_nop_();_nop_();
}

unsigned char ad0831read(void)//�Х���ADC0831�ഫ������C��ĳ�A�A��Ū����ơA
{								//����ƬO����0831����C��ĳ�i�檺�A
								//�H��A���@�ӵw��o�˪��N²��h
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


