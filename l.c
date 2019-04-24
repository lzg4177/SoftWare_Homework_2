#include<reg51.h>
#include<absacc.h>
#include <intrins.h>
#include <stdlib.h>
#include<absacc.h>

#define uint unsigned int
#define uchar unsigned char

#define duanma     XBYTE[0X8004]		  
#define weima      XBYTE[0x8002]		 
#define keyport    XBYTE[0x8001]	
xdata unsigned char CS0832 _at_ 0x9000;
unsigned char code led[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,        //0~8
                              0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x38,0x54};  //9~f  L     
unsigned char i;															
uchar xianshi[8]; 
uchar key[6];	
uchar key_va;	
uchar key_flg=0;		
uint key_cnt=0;																													
uint time=0;																
uint num=0;	
uchar m=0;
uchar number=0;															
																
		
uchar read(void);
void read_display(void);
void display(void);												
void Write0832(unsigned char b);
void readkey(void);															
															
code unsigned char KeyTable[] = {   // 键码定义
  0x16, 0x15, 0x14, 0xff,
  0x13, 0x12, 0x11, 0x10,
  0x0d, 0x0c, 0x0b, 0x0a,
  0x0e, 0x03, 0x06, 0x09,
  0x0f, 0x02, 0x05, 0x08,
  0x00, 0x01, 0x04, 0x07
};
     
void Write0832(unsigned char b)
{
   CS0832=b;
}

 void delay(uint z)
{
   uint a,b;
   for(a=z;a>0;a--)
     for(b=122;b>0;b--);
}

void readkey(void)
{

weima=0x20;
duanma=xianshi[0]; 		
delay(3);
weima=0x00;
	
weima=0x10;
duanma=xianshi[1]; 
delay(3);
weima=0x00;			
	
weima=0x08;
duanma=xianshi[2]; 	
delay(3);
weima=0x00;	
		
weima=0x04;
duanma=xianshi[3]; 	
delay(3);
weima=0x00;
	
weima=0x02;
duanma=xianshi[4]; 
delay(3);
weima=0x00;
		
weima=0x01; 
duanma=xianshi[5]; 
delay(3);
weima=0x00;
delay(2);

}

void main()
{ 
	TMOD=0X01;
  TH0=(65536-50000)/256; 
  TL0=(65536-50000)%256;
  ET0=1;
  TR0=1;
  EA=1;
	
xianshi[0]=0; 	
xianshi[1]=0; 
xianshi[2]=0; 
xianshi[3]=0; 
xianshi[4]=0; 
xianshi[5]=0; 
xianshi[6]=0; 
xianshi[7]=0; 
Write0832(0);
while(1)
{
	  readkey();
		weima=0x00;
    delay(1);
    key_va=read();
		weima=0x00;
    delay(1);
    if(key_cnt>2) 
{     read_display();
	    weima=0xa0;
      delay(1);
	}   
  display();
   delay(5);
 }
}

void display()
{
	if(m==0x01)
	{
	  if(num==0)
	    time=0;
	  else if(time>19)
	  {
		 time=1;
     num--;
		 if((num/10000)!=0)
			xianshi[0]=led[num/10000%10];
     else
      xianshi[0]=0;		

    if((num/1000)!=0)
			xianshi[1]=led[num/1000%10];	
     else
      xianshi[1]=0;	

     if((num/100)!=0)
			xianshi[2]=led[num/100%10];	
     else
      xianshi[2]=0;	
		 
		 if((num/10)!=0)
			xianshi[3]=led[num/10%10];	
     else
      xianshi[3]=0;	
		 
			xianshi[4]=led[num%10];	
			if(num ==0)
{
    Write0832(0);
}	 
    }
  }
  		
}


void read_display(void)
{
	   uint i;
	 static uchar old=0xff;
    if(key_va==0xff)
		{
			  
		}
	  else if(old!=key_va)
		{
			if((key_va==11)||(key_va==12)||(key_va==13)||(key_va==15 )||(key_va==14))
			{
				old=key_va;
				if(key_va==11)
				{
					 if(xianshi[5]==led[16])
					 {
					    time=1;
					 }
				
					 else
					 {
						 time=0;
					 }					  
				}
        else if(key_va==12)
				{
				
          		   Write0832(0x50);
			
				}
        else if(key_va==13)
				{
                Write0832(0xa0);
        }
				else if(key_va==14)
				{
				      Write0832(0xff);
        }
			  else if(key_va==15)
				{			
				  Write0832(0);	
					num=0;
					number=0;
					time=0;
					m=0;
					time=0;num=0;				
			  for(i=0;i<8;i++)
						xianshi[i]=0;
        }
			}
			else if(number==0 &&((key_va!=11)&&(key_va!=12)&&(key_va!=13)&&(key_va!=15)))
			{
				if(xianshi[5]!=led[16])
				{
					  for(i=0;i<5;i++)
						xianshi[i]=xianshi[i+1];
				}
					
				if((key_va>=0)&&(key_va<10))
				{
				   xianshi[5]=led[key_va];
					num=num*10+key_va;
				}
				else if(key_va==10)
				{
				   xianshi[5]=led[16];	
					 m=0x01;
					 number=1;
				}	
				old=key_va;
			}
	 }
}


uchar read()	  // 得到的是哪个按键
{
	uchar Pos;
	uchar i;
	uchar k;

	i = 6;
	Pos = 0x20;     // 找出按键所在列
	do
	{
		XBYTE[0x8002] = ~ Pos;	  //把某一列置低电平
		Pos>>= 1;
		k = ~XBYTE[0x8001] & 0x0f; //读取行线状态
	} while ((--i != 0) && (k == 0));// 键值 = 列 x 4 + 行
	
	if (k != 0)
	{
		i *= 4;
		if (k & 2)
			i += 1;
		else if (k & 4)
			i += 2;
		else if (k & 8)
			i += 3;

		XBYTE[0x8002] = 0;	 //将所有列再置0
		return(KeyTable[i]);  // 取出键码
	} 
	else 
		return(0xff);
}



void time0(void) interrupt 1 
 { 

  TH0=(65536-50000)/256; 
  TL0=(65536-50000)%256;
	TF0=0;
  if(key_va!=0xff)
		key_cnt++;
	else
		key_cnt=0;	
   if(time)
     time++;	
  
	 
}
