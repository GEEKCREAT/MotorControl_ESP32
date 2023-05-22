#include "oled12864.h"

/*
通过IIC发送指令到OLED
*/
void OLED_WrCmd(unsigned char IIC_Command)
{
	uint8_t *pData;
	pData = &IIC_Command;
	IIC_SAND_DATE(OLED_ADDRESS,0x00,pData,1);
}
/*
通过IIC发送数据到OLED
*/
void OLED_WrDat(unsigned char IIC_Data)
{
	uint8_t *pData;
	pData = &IIC_Data;
	IIC_SAND_DATE(OLED_ADDRESS,0x40,pData,1);
}
/*
设置硬件内的光标
*/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
  OLED_WrCmd(    0xb0  +   7-y   );
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd( (x&0x0f)    |0x00);
}
/*
初始化硬件内存（显存）
*/
void OLED_Init_Display_Buffer(char a)
{

	for(unsigned char y = 0;y < Y_WIDTH_;y++)
	{
		OLED_Set_Pos(0,y);
		for(unsigned char x = 0;x < X_WIDTH;x++)
		{

			OLED_WrDat(a);
		}
	}


}
/*
设置屏幕亮度
*/
void OLED_Setting_luminance(unsigned char a)
{
	OLED_WrCmd(0x81);
	OLED_WrCmd(a*2.55);

}
/*
初始化OLED

硬件扫描方式

从左到右从上到下
纵向8点上高位

*/
void OLED_Init(void)
{
	//HAL_Delay(200);
	OLED_WrCmd(0xAE); //关闭显示
	OLED_WrCmd(0xD5); //设置时钟分频因子,震荡频率
	OLED_WrCmd(  80); //[3:0],分频因子;[7:4],震荡频率
	OLED_WrCmd(0xA8); //设置驱动路数
	OLED_WrCmd(0X3F); //默认0X3F(1/64)
	OLED_WrCmd(0xD3); //设置显示偏移
	OLED_WrCmd(0X00); //默认为0
	OLED_WrCmd(0x40); //设置显示开始行 [5:0],行数.
	OLED_WrCmd(0x8D); //电荷泵设置
	OLED_WrCmd(0x14); //bit2，开启/关闭
	OLED_WrCmd(0x20); //设置内存地址模式
	OLED_WrCmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WrCmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WrCmd(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	//OLED_WrCmd(0xDA); //设置COM硬件引脚配置
	//OLED_WrCmd(0x12); //[5:4]配置
	//显示方向设置
	//OLED_WrCmd(0xc8);//OLED_WrCmd(0xa0);
	 //行扫描顺序：从上到下
	 //列扫描顺序：从左到右
	OLED_WrCmd(0xD9); //设置预充电周期
	OLED_WrCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WrCmd(0xDB); //设置VCOMH 电压倍率
	OLED_WrCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WrCmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WrCmd(0xA6);
	 //设置显示方式;bit0:1,反相显示;0,正常显示
	OLED_WrCmd(0xAF); //开启显示
	
	OLED_Init_Display_Buffer(0);
}

/*************************************************************************************************************************************************************************************/
/*硬件与软件之间的部分*/


/*
定义显存
*/
char OLED_buff[Y_WIDTH_][X_WIDTH];

/*
加速缓存
*/
char OLED_speedup[Y_WIDTH_][X_WIDTH];
/*
将显存发送到硬件
*/
void OLED_Cache_to_hardware()
{
	char b=0;
	for(uint8_t y=0;y<Y_WIDTH_;y++)
	{
		OLED_Set_Pos(0,y);
		for(uint8_t x=0;x<X_WIDTH;x++)
		{
			if(OLED_buff[y][x]!=OLED_speedup[y][x])
			{
				
				if(b==0){b=1;OLED_Set_Pos(x,y);}			
				OLED_WrDat(OLED_buff[y][x]);			
				OLED_speedup[y][x]=OLED_buff[y][x];
				
			}else
			{b=0;}
			//OLED_buff[y][x]=0x00;
		}
	}
}




/*
打点函数
打点类型
dot_type=0 清除点
				=1 打点
				=2 取反点
*/
void OLED_set_dot(unsigned char x,unsigned char y,unsigned char dot_type)
{
	if(x>=X_WIDTH||y>=Y_WIDTH)
	{
		return ;
	}
	uint8_t y1=y/8;
	uint8_t y2=y%8;
	uint8_t y3=(0x80>>y2);
	
	char temp=OLED_buff[y1][x];
	switch(dot_type)
	{
		case 0:
			temp &= ~y3;
			break;
		case 1:
			temp |= y3;
			break;
		case 2:
			temp ^= y3;
			break;
		case 3:break;
	}
	
	
	OLED_buff[y1][x]=temp;
		
		
	
	
}
/*
绘图函数
数据格式 从左到右，从上到下，横向8点右高位
type=0 清空区域
		=1 区域图层
		=2 正片叠底
		=3 正片清空
		=4 负片
		=5 正片负片
*/
void OLED_Pix(unsigned char x,unsigned char y,unsigned char w,unsigned char h,const char *p,unsigned char type)
{
	
	unsigned char b=0;
	unsigned int  c=0;
	
	for(uint8_t h1=0;h1<h;h1++)
	{
		for(uint8_t w1=0;w1<w;w1++)
		{
			if(p[c]&0x80>>b)
			{
				switch(type)
				{
					case 0:
						OLED_set_dot(w1+x,h1+y,0);
						break;
					case 1:
						OLED_set_dot(w1+x,h1+y,1);
						break;
					case 2:
						OLED_set_dot(w1+x,h1+y,1);
						break;		
					case 3:
						OLED_set_dot(w1+x,h1+y,0);
						break;		
					case 4:
						OLED_set_dot(w1+x,h1+y,0);
						break;	
					case 5:
						OLED_set_dot(w1+x,h1+y,2);
						break;						
				}
				
			}else
			{
				switch(type)
				{
					case 0:
						OLED_set_dot(w1+x,h1+y,0);
						break;
					case 1:
						OLED_set_dot(w1+x,h1+y,0);
						break;
					case 2:

						break;	
					case 4:
						OLED_set_dot(w1+x,h1+y,1);
						break;
				}
			}
			b++;
			if(b==8)
			{
				b=0;c++;
			}
		}
		if(b!=0)
		{
			b=0;c++;
		}
		
	}
		
	

}
/*
显示字符
数据格式 从左到右，从上到下，横向8点左高位 （同一个函数实现的显示不知道为何这里要用左高位才能正常显示）
type=0 清空区域
		=1 区域图层
		=2 正片叠底
		=3 正片清空
		=4 负片
*/
void OLED_Ascii(unsigned char x,unsigned char y,unsigned char size,char *chr,unsigned char type)
{
	if(*chr>=' '&&*chr<='~')
	{
		switch(size)
		{
			case 8:
				OLED_Pix(x,y,5,8,asc2_0805[*chr-' '],type);
				break;			
			case 12:
				OLED_Pix(x,y,6,12,asc2_1206[*chr-' '],type);
				break;
			case 16:
				OLED_Pix(x,y,8,16,asc2_1608[*chr-' '],type);
				break;
		}	
	}

}

void OLED_Ch(unsigned char x,unsigned char y,unsigned char size,char *chr,unsigned char type)
{
    //顯示中文

}

void OLED_Str(unsigned char x,unsigned char y,unsigned char size,char *str,unsigned char lenconst,unsigned char type)
{
	unsigned char size2;
	unsigned char tempx=x;
	unsigned char templenstard=1;
	switch(size)
	{
		case 8:
			size2=6;
		break;
		case 12:
			size2=7;
		break;
		case 16:
			size2=8;
		break;
	}	
	
	while(*str!='\0')
	{

		if((x+size2>=X_WIDTH)||(*str=='\n'))
		{
			if(lenconst!=0)
			{
				if(templenstard>=lenconst)
				{
					return;
				}
			}
			templenstard++;
			y+=size;
			if(y>=Y_WIDTH)
			{
				//y=0;
				return;
			}
			x=tempx;
		}else
		{
			if(*str<127)
			{
				OLED_Ascii(x,y,size,str,type);
				x+=size2;
			}else
			{
				OLED_Ch(x,y,size,str,type);
				str++;
				x+=16;

			}
			
		}
		

		str++;
		
		
		
		

	}

}

void OLED_Str_list(unsigned char x,unsigned char y,unsigned char size,char *str,unsigned char lenstard,unsigned char lenconst,unsigned char type)
{
	unsigned char templenstard=0;
	while(templenstard!=lenstard)//跳转到指定行
	{
		
		switch(*str)
		{
			case '\n':  //如果是回车 行加加
				templenstard++;
				break;
			case '\0':	//如果字符串提前结束 报错 直接返回
				return;
		}
		
		str++;
	}
	OLED_Str(x,y,size,str,lenconst,type);

}

/*
几何绘图
*/


/*画横线*/
void OLED_HL(unsigned char x,unsigned char y,unsigned char size,unsigned char dot_type)
{

}

/*画竖线*/
void OLED_VL(unsigned char x,unsigned char y,unsigned char size,unsigned char dot_type)
{

}

/*画任意线*/
void OLED_AL(int sx,int sy,int ex,int ey,unsigned char dot_type)
{

	int dx,dy,absx,absy;
	
	//计算差
	dx=ex-sx;
	dy=ey-sy;
	//取绝对值 就是轴长
	absx=dx<0?0-dx:dx;
	absy=dy<0?0-dy:dy;
	
	
	if(dx==0)
	{
		if(dy>0)
		{
			for(int y=ey;y>sy;y--)
			{
				OLED_set_dot(sx,y,dot_type);
			}
		}else 
		if(dy<0)
		{
			for(int y=ey;y<sy;y++)
			{
				OLED_set_dot(sx,y,dot_type);
			}
		}
		
		return ;
	}

	if(dy==0)
	{
		if(dx>0)
		{
			for(int x=ex;x>sx;x--)
			{
				OLED_set_dot(x,sy,dot_type);
			}
		}else
		if(dx<0)
		{
			for(int x=ex;x<sx;x++)
			{
				OLED_set_dot(x,sy,dot_type);
			}
		}
		
		return ;
	}
	
	if(dx==0&&dy==0)
	{
		//OLED_set_dot(sx,sy,dot_type);
		return ;
	}
}


/*画方框*/
void OLED_square(int Start_x, int Start_y, int End_x, int End_y, char type)
{
	if(Start_x > End_x){int i1 = Start_x; Start_x = End_x; End_x=i1;}
	if(Start_y > End_y){int i1 = Start_y; Start_y = End_y; End_y=i1;}
	for(; Start_x < End_x ; Start_x++)
	{
		for(int y2 = Start_y ; y2 < End_y ; y2++)
		{
			OLED_set_dot(Start_x, y2, type);
		}
	}
}
