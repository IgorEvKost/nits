#include "config.h"
#include <stdio.h>
#include <LPC2478.h>
#include "var.h"

#define TXPIN	(1<<19)				//порт 0.19
#define RXPIN	(1<<26)				//порт 4.26


static void bpm_req_init(ui8 n)
{
	for(int i=0;i<63;i++)			//сброс буфера приемника
	{
		RxBuf[i]=0;
	}


	FIO0PIN|=TXPIN;					//включение передатчика
	FIO4PIN&=~RXPIN;

	U2FCR|=1<<U2FCR_RXFIFOReset;

	RxPtr=0;						//указатель буфера приёмника
	RxByteWait=n;					//ожидаем число байт
	RxByteNumOld=RxByteNum;			//
	RxByteNum=0;					//принятое число байт
	BP1ok=0;
}



//задание уставок
BPM_Struc bpm_set_point(BPM_Struc s)
{
	if(s.Req)							// запускаем цикл UART на БПМ
	{
		bpm_req_init(W_REQ);

		U2THR=s.Adr;					//заполняем стек передатчика
		U2THR='W';
		U2THR=8;
		U2THR=0;
		U2THR=1;
		U2THR=3;
		U2THR=s.Iz.b[0];
		U2THR=s.Iz.b[1];
		U2THR=s.Uz.b[0];
		U2THR=s.Uz.b[1];
		U2THR=s.Pz.b[0];
		U2THR=s.Pz.b[1];
		U2THR=(0-(s.Adr+'W'+8+1+3+s.Iz.b[0]+s.Iz.b[1]+s.Uz.b[0]+s.Uz.b[1]+s.Pz.b[0]+s.Pz.b[1]));				//

		s.Req=false;					//однократно
		s.Rsp=true;						//ожидаем прием
		s.RspTimeOut=50;				//это время (в тиках Tmr0) ожидаем завершение приёма
		s.TmrReq=false;
//		s.Error=255;					//сброс кода ошибки*
//		s.ReqNum=3;						//*
		flSPIEnd=false;
	}

	if(s.Rsp)
	{
		if(RxByteNum==RxByteWait)			//отслеживаем, когда придет всё
		{

			Sum=0;
			for(int i=0;i<RxByteWait-1;i++)
			{
				Sum+=RxBuf[i];				//КС
			}


			if(Sum==0)
			{
//				for(;;) {}

				//принято правильное количество байт, правильная КС,
				if((RxBuf[2]==1) && (RxBuf[3]==3))	// начальный регистр, конечный регистр
				{


					s.Error=0;					//всё правильно
					s.Rsp=false;				//выходим из цикла приема
				}
				else
				{
					if(s.ReqNum>0)
					{
						s.ReqNum--;				//уменьшаем количество попыток
						s.TmrReq=true;
						//s.Req=true;				//новый запрос TODO ввести запрос тайм-аута!!!!!
					}
					else
					{
						s.Error=3;				//Ошибка "СБОЙ ЗАПИСИ УСТАВОК"
//						for(;;){}				>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> переход куда-нибудь
					}
				}
			}
			else
			{
				s.Error=1;					//принято правильное количесво байт с неправильной КС
				s.Rsp=false;				//выходим из цикла приема

				if(s.ReqNum>0)
				{
					s.ReqNum--;				//уменьшаем количество попыток
					s.TmrReq=true;
					//s.Req=true;				//новый запрос TODO ввести запрос тайм-аута!!!!!
				}
				else
				{
//					for(;;){}				>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> переход куда-нибудь
				}
			}
			s.Upd=true;					//?
		}
		else								//ещё не все байты, контролируем тайт-аут приема
		{
			if(s.RspTimeOut==0)
			{
				s.Rsp=false;				//нужного количества байт, выходим из цикла приема

				if(s.ReqNum>0)
				{
					s.ReqNum--;				//уменьшаем количество попыток
					s.TmrReq=true;
					//s.Req=true;				//TODO ввести запрос тайм-аута!!!!!
				}
				else
				{
					s.Error=2;				//Ошибка "ТАЙМ-АУТ ПРИЁМА"
					s.Upd=true;
//					for(;;){}				>>>>>>>> переход куда-нибудь
				}
			}
		}
	}

	return s;
}


// включение
BPM_Struc bpm_start(BPM_Struc s)
{

	return s;
}

// выключение
BPM_Struc bpm_stop(BPM_Struc s)
{

	return s;
}


// получение статуса
BPM_Struc bpm_get_stat(BPM_Struc s)
{
	if(s.Req)							// запускаем цикл UART на БПМ
	{
		bpm_req_init(R_REQ);

		U2THR=s.Adr;						//заполняет буфер передатчика
		U2THR=0x52;
		U2THR=0x02;
		U2THR=0x00;
		U2THR=0x06;
		U2THR=0x06;
		U2THR=0xA1;

		s.Req=false;					//однократно
		s.Rsp=true;						//ожидаем прием
		s.RspTimeOut=20;				//это время (в тиках Tmr0) ожидаем завершение приёма
		s.Error=255;					//сброс кода ошибки
//		s.ReqNum=3;						//количество попыток (ставим снаружи, перед вызовом функции)
		flSPIEnd=false;
		ReqNum=s.ReqNum;
	}

	if(s.Rsp)
	{
		if(RxByteNum==RxByteWait)			//отслеживаем, когда придет всё
		{
			Sum=0;
			for(int i=0;i<RxByteWait-1;i++)
			{
				Sum+=RxBuf[i];				//КС
			}

			if(Sum==0)
			{
				//принято правильное количество байт с правильной КС
				s.Error=0;
				s.ReqNum=ReqNum;			//восстанавливаем s.ReqNum после удачного приема
				s.St.b[0]=RxBuf[6];			// код ошибки
				s.St.b[1]=RxBuf[7];			// код состояния
			}
			else
			{
				//принято правильное количесво байт с неправильной КС
				s.Error=1;
			}
			s.Rsp=false;
			s.Upd=true;
		}
		else
		{

			if(s.RspTimeOut==0)
			{
				s.Rsp=false;				//нужного количества байт, выходим из цикла приема

				if(s.ReqNum>0)
				{
//					s.ReqNum--;				//уменьшаем количество попыток
					s.Req=true;
				}
				else
				{
					s.Error=2;
//					BPChk=false;			//за отведенное время и за n количества попыток не принято
					s.Upd=true;
//					for(;;){}				>>>>>>>> переход куда-нибудь
				}
//				s.Upd=true;
			}


//			if(s.RspTimeOut==0)
//			{
//				s.Error=2;
//  //				BPChk=false;				//за отведенное время не принято нужного количества байт,
//				s.Rsp=false;				//выходим из цикла приема
//				flBPUpd=true;
//			}
		}
	}
	return s;
}


//чтение уставок или текущих значений
BPM_Struc bpm_get_data(BPM_Struc s, ui8 n)	// для уставок n=0, для текучки n=1;
{
	if(s.Req)							// запускаем цикл UART на БПМ
	{
		bpm_req_init(R_3_REQ);			//получаем данные от трёх регистров

		U2THR=s.Adr;					//заполняем стек передатчика
		U2THR='R';
		U2THR=2;
		U2THR=0;
		if(n==0)						//регистры и расчет КС для уставок
		{
			U2THR=1;
			U2THR=3;
			U2THR=(0-(s.Adr+'R'+2+1+3));	//
		}

		if(n==1)						//регистры и расчет КС для текучки
		{
			U2THR=7;
			U2THR=9;
			U2THR=(0-(s.Adr+'R'+2+7+9));	//
		}


		s.Req=false;					//однократно
		s.Rsp=true;						//ожидаем прием
		s.RspTimeOut=50;				//это время (в тиках Tmr0) ожидаем завершение приёма
		s.Error=255;					//сброс кода ошибки*
//		s.ReqNum=3;						//*
		flSPIEnd=false;
	}

	if(s.Rsp)
	{
		if(RxByteNum==RxByteWait+1)			//отслеживаем, когда придет всё
		{
			Sum=0;

			for(int i=0;i<RxByteWait;i++)
			{
				Sum+=RxBuf[i];						//КС
			}

			if(Sum==0)								//принято правильное количество байт, правильная КС,
			{
				if(n==1)							//парсим текучку
				{
					s.I.b[0]=RxBuf[6];
					s.I.b[1]=RxBuf[7];
					s.U.b[0]=RxBuf[8];
					s.U.b[1]=RxBuf[9];
					s.P.b[0]=RxBuf[10];
					s.P.b[1]=RxBuf[11];

					RxBuf[6]=255;

					s.Error=0;						//всё правильно
					s.Rsp=false;					//выходим из цикла приема
				}


				if(n==0)							//дополнительный контроль уставок
				{

//					if(	RxBuf[6]==s.Iz.b[0] && RxBuf[7]==s.Iz.b[1] && RxBuf[8]==s.Uz.b[0] && RxBuf[9]==s.Uz.b[1] && RxBuf[10]==s.Pz.b[0] && RxBuf[11]==s.Pz.b[1] )
					if(	RxBuf[6]==s.Iz.b[0] && RxBuf[7]==s.Iz.b[1])
					{
						s.Error=0;					//всё правильно
						s.Rsp=false;				//выходим из цикла приема
					}
					else
					{
						//if(s.ReqNum>0)
						//{
						//	s.ReqNum--;			//уменьшаем количество попыток
						//	s.TmrReq=true;
						//}
						//else
						//{
						s.Error=4;			//Ошибка "ОШИБКА ПРИ СРАВНЕНИИ УСТАВОК" >>>>>>>>>>>>> переход куда-нибудь
						//}
					}
				}
			}
			else
			{
				s.Error=1;					//Ошибка "НЕПРАВИЛЬНАЯ КС"
				s.Rsp=false;				//выходим из цикла приема

				if(s.ReqNum>0)
				{
					s.ReqNum--;				//уменьшаем количество попыток
					s.TmrReq=true;
					//s.Req=true;				//новый запрос
				}
				else
				{
//					for(;;){}				>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> переход куда-нибудь
				}
			}
			s.Upd=true;					//?
		}
		else								//ещё не все байты, контролируем тайт-аут приема
		{
			if(s.RspTimeOut==0)
			{
				s.Rsp=false;				//нужного количества байт, выходим из цикла приема

				if(s.ReqNum>0)
				{
					s.ReqNum--;				//уменьшаем количество попыток
					s.TmrReq=true;
//					s.Req=true;
				}
				else
				{
					s.Error=2;				//Ошибка "ТАЙМ-АУТ ПРИЁМА"
					s.Upd=true;
//					for(;;){}				>>>>>>>> переход куда-нибудь
				}
			}
		}
	}
	return s;
}




/*
 * Формируем пакет  для регистра команд с кодом сброса ошибки регистра статуса.
 * Ожидаем прихода ответной квитанции. При получении требуемого количества байт подстчитываем кс,
 * выходим немедленно, если кс=0, если кс!= 0.
 * Если при получении требуемого количества байт превышено время ожидания, то уменьшаем количесво попыток
 * на 1 и заново формируем пакет. При исчерпании количества попыток ставим флаг  и выходим из приема.
 */

BPM_Struc bpm_command(BPM_Struc s, ui8 cmd, ui8 par)
{
	if(s.Req)							// запускаем цикл UART на БПМ
	{
		bpm_req_init(W_REQ);

		U2THR=s.Adr;					//заполняет буфер передатчика
		U2THR='W';
		U2THR=6;
		U2THR=0;
		U2THR=0;
		U2THR=0;
		U2THR=cmd;
		U2THR=par;
		U2THR=cmd;
		U2THR=par;
		U2THR=(0-(s.Adr+'W'+6+cmd+par+cmd+par));

		s.Req=false;					//однократная передача
		s.Rsp=true;						//ожидаем прием
		s.RspTimeOut=50;				//это время (в тиках Tmr0) ожидаем завершение приёма
//		s.Error=255;					//сброс кода ошибки*
//		s.ReqNum=3;						//*
		flSPIEnd=false;
	}

	if(s.Rsp)
	{
		if(RxByteNum==RxByteWait)			//отслеживаем, когда придет всё
		{

			Sum=0;
			for(int i=0;i<RxByteWait-1;i++)
			{
				Sum+=RxBuf[i];				//КС
			}

			if(Sum==0)
			{
				if(RxBuf[2]==0 && RxBuf[3]==0)
				{

					s.Error=0;
//					BPChk=true;				//принято правильное количество байт, правильная КС,
											//в RxBuf[2] и RxBuf[3] 0 (регистр управления)
					s.Rsp=false;			//снимаем контроль ответа
				}
			}
			else
			{
				s.Error=1;					//Ошибка "НЕПРАВИЛЬНАЯ КС"
//				BPChk=false;
				s.Rsp=false;				//выходим из цикла приема

				if(s.ReqNum>0)
				{
					s.ReqNum--;				//уменьшаем количество попыток
					s.TmrReq=true;
					//s.Req=true;				//новый запрос
				}
				else
				{
//					for(;;){}				>>>>>>>> переход куда-нибудь
				}

			}
			s.Upd=true;					//?
		}
		else								//ещё не все байты, контролируем тайт-аут приема
		{
			if(s.RspTimeOut==0)
			{
				s.Rsp=false;				//нужного количества байт, выходим из цикла приема

				if(s.ReqNum>0)
				{
					s.ReqNum--;				//уменьшаем количество попыток
					s.TmrReq=true;
					//s.Req=true;
				}
				else
				{
					s.Error=2;				//Ошибка "ТАЙМ-АУТ ПРИЁМА"
//					BPChk=false;			//за отведенное время и за n количества попыток не принято
					s.Upd=true;
//					for(;;){}				>>>>>>>> переход куда-нибудь
				}
			}
		}
	}

	return s;
}


/*
			if(Sum==0)
			{


			RegionFill(8,360,620,30,BackColour);
			xsprintf(buffer,"%02x", RxBuf[0]);
			DrawString(10,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[1]);
			DrawString(60,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[2]);
			DrawString(110,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[3]);
			DrawString(160,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[4]);
			DrawString(210,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[5]);
			DrawString(260,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[6]);
			DrawString(310,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[7]);
			DrawString(360,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[8]);
			DrawString(410,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[9]);
			DrawString(460,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[10]);
			DrawString(510,370,&fnt20,buffer,GREEN);

			xsprintf(buffer,"%02x", RxBuf[11]);
			DrawString(560,370,&fnt20,buffer,GREEN);


		}
*/
