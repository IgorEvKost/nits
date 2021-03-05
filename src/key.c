#include "config.h"
#include <stdio.h>
#include <LPC2478.h>
#include "var.h"

//опрос и формирование кода клавиш с портов 0.12 0.27 0.28 2.25 3.26
//                                           |    |    |    |    +--- кнопка
//                                           |    |    |    +-------- кнопка
//                                           |    |    +------------- кнопка
//                                           |    +------------------ кнопка
//                                           +----------------------- кнопка энкодера
//                 _____________________________________
//возврат значения | X | X | X |But|Key8|Key4|Key2|Key1|
//                 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
ui8 key()
{
	ui8 t,res;

	if(KeyCnt>KEYCNT)
	{

		res=0;
		t=FIO0PIN3;
		res|=(t&0b00001000)>>3;					//0.27
		res|=(t&0b00010000)>>2;					//0.28
		res|=(FIO0PIN1&0b00010000);				//0.12
		res|=(FIO2PIN3&0b00000010)<<2;			//2.25
		res|=(FIO3PIN3&0b00000100)>>1;			//3.26
		res|=0b11100000;
		KeyCnt=0;
	}
	return ~res;
}

ui8 rd_port()
{
	ui8 t;

	t=FIO3PIN2;
	return t;
}

void set_port(ui8 data)					//номер порта
{
	switch(data)
	{
		case 0:
		{
			FIO3PIN|=1<<25;
		}
		break;

		case 1:
		{
			FIO3PIN|=1<<27;
		}
		break;

		case 2:
		{
			FIO3PIN|=1<<30;
		}
		break;

		case 3:
		{
			FIO3PIN|=1<<31;
		}
		break;
	}
}

void res_port(ui8 data)					//номер порта
{
	switch(data)
	{
		case 0:
		{
			FIO3PIN&=~(1<<25);
		}
		break;

		case 1:
		{
			FIO3PIN&=~(1<<27);
		}
		break;

		case 2:
		{
			FIO3PIN&=~(1<<30);
		}
		break;

		case 3:
		{
			FIO3PIN&=~(1<<31);
		}
		break;
	}
}
