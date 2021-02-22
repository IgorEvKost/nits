#include "config.h"

typedef union ui16_Un
{
	ui16 Val;
	ui8 b[2];
}ui16_Un;


typedef struct Device
{
	bool	On;
	ui16	Val;
	ui8		buf[16];		//текстовый буфер для экрана
}Device_Struc;


typedef struct Param
{

	bool	Upd;
	ui16	Delta;
	ui16	Var;
	ui16	VarOld;
	ui16	Min;
	ui16	Max;
	ui8		buf[16];		//текстовый буфер для экрана
}Param_Struc;

typedef struct
{
	bool Upd;		//
	bool TmrReq;	//==true запрос на тайм-аут
	bool Req;		//==true стадия запроса
	bool Rsp;		//==true стадия приема ответа
	ui8 ReqNum;		//максимальное количество запросов (при каждом запросе -1)
	ui8 RspTimeOut;	//длительность ожидания завершения ответа
	ui8 Error;		//код ошибки
	ui8 Adr;		//адрес
	ui16_Un Par;
	ui16_Un ParRd;
	ui16_Un I;		//текущий ток
	ui16_Un U;		//текущее напряжение
	ui16_Un P;		//текущая мощность
	ui16_Un St;		//регистр статуса
} BPM_Struc;

typedef struct
{
	bool Upd;		//
	bool TmrReq;	//==true запрос на тайм-аут
	bool Req;		//==true стадия запроса
	bool Rsp;		//==true стадия приема ответа
	ui8 ReqNum;		//максимальное количество запросов (при каждом запросе -1)
	ui8 RspTimeOut;	//длительность ожидания завершения ответа
	ui8 Error;		//код ошибки
	ui8 Proc;		//текущий процесс
	ui8 ProcNew;
	ui8 Kvit;
	ui16_Un Par;	//датчик
} MPU_Struct;


#define RXBUFSIZE	64
#define RXPTRMAX	7
#define SEC1		100
#define BEEPS		3	//короткий BEEP
#define KEYCNT		3
#define MODECNT		8000 //@

ui16 Cnt1;			//счетчик периода датчика вращения
ui16 Cnt11;			//счетчик периода датчика вращения
ui16 Cnt2;			//счетчик для всяких задержек
ui16 BeepCnt;		//длительность сигнала
ui16 SecCnt;		//для секунд
ui16 KeyCnt;		//для опроса кнопок
ui16 ModeCnt;		//
ui16 WaitBP;		// пауза для источника (заряд фильра питания)
ui16 FazaCnt;		//счетчик фазы появления тока
ui16 Faza;
ui16 FazaOld;

//bool fSecCntOn;		//
bool flSec;			//прошел 1 сек. интервал


/*		Щ300		*/
/* Идентификатор пределов */
#define K_1			0x10
#define	K_10		0x11
#define	K_100		0x12
#define	M_1			0x13
#define	M_10		0x14
#define	M_100		0x15
#define	G_1			0x16
#define	MV_1		0x17
#define	MV_10		0x18
#define	MV_100		0x19
#define	V_1			0x20
#define	V_10		0x21
#define	V_100		0x22
#define	V_1000		0x23
#define	S300MODE	K_10


Param_Struc	S300Data;			//данные от Щ300
Param_Struc	S300Mode;

ui8	S300ok;		/*	признак Щ300 на связи, при поступлении правильного ответа  S300ok <- 10
					при ошибке S300ok <- S300ok-1, при S300ok=0 готовность Щ300 снимается */


/***Конец блока	Щ300*****/



/*		БПМ		*/

// уставки по напряжению и мощности
#define	U_PROC	3522			// 860 в.
#define P_PROC	500				//

//DD7
#define SWC			(ui8)(0x01)		// отключение коммутации ключа
#define ON			(ui8)(0x02)		// вкл/выкл силового питания
#define	MOD1		(ui8)(0x04)		// режим 1
#define MOD2		(ui8)(0x08)		// режим 2
#define	LEV1		(ui8)(0x10)		// защита ключа
#define LEV2		(ui8)(0x40)		// защита ключа

//DD1
#define DD1_RP		(ui8)(0x01)
#define DD1_RV		(ui8)(0x02)
#define DD1_PWR		(ui8)(0x04)
#define DD1_PHZ		(ui8)(0x08)
#define DD1_IPSF	(ui8)(0x10)
#define DD1_RI		(ui8)(0x40)


//DD2
#define	DD2_OV      (ui8)(0x01)
#define DD2_SWIT    (ui8)(0x02)
#define DD2_AR      (ui8)(0x04)
#define DD2_OT      (ui8)(0x08)
#define DD2_OL      (ui8)(0x10)
#define DD2_LOCK    (ui8)(0x20)



#define KI		(0.0122f)		// N*KI=i


ui8	BP1ok;			/*	признак блока питания на связи, при поступлении правильного ответа BPok <- 10
					при ошибке BPok <- BPok-1, при BPok=0 готовность блока снимается	*/
bool flBPOff;		// ==true выключаем блок при измерении R

ui16 I0;			// значение тока в стопе (!= 0 из-за смещений в канале измерения)

ui8	R06L,R06H;		// регистр 6 блока питания
ui8 Sum;
ui8 ReqNum;			//количество попыток (используется для операций чтения/записи)
ui8 RxByteWait;		//==ожидаемое количество байт
ui8 RxByteNum;		//==принятое количество байт
ui8 RxByteNumOld;	//
BPM_Struc BP1;
MPU_Struct MPU;

/*****Конец блока БПМ*****/


/*		Вращение		*/

// #define ROT_N1	25	//количество оборотов до первого замера

#define N0		10		//константа для корректного определения оборотов в Proc 275
#define ROT_N2	2 + N0	//количество оборотов при превышении 2Х
#define ROT_N3	1 + N0	//меньше 2Х, но больше порога начала подгонки
#define ROT_N4	1 + N0	//при подгонке //2


#define ROTPER	500U	//максимальное количество тиков за оборот, если больше то ошибка //250U

bool	flRotUpd;		//требуется обновления индикатора состояния вращения
bool	flRotOk;		//признак вращения, при поступлении импульса
bool	flRotOkOld;		//
bool	flRot1;			//ставим при переходе сигнала от 0 к 1, т.е. при полном обороте
bool	flRot0;			//признак того, что оборот ещё не засчитан
ui16	RotVal;
bool	flRotValUpd;
bool	flRotUpdEn;
ui16	RotPer;			//период вращения в условных единицах
ui16	RotPerCnt;		//счетчик периода

/*****Конец блока вращение*****/




Device_Struc Timer1;
Device_Struc Rotator;

ui16 TempI;							// для временного хранения задания по току
ui16 Enc1,Enc2;
ui8 Enc, fEnc;						//для предотвращения многократной обработки
ui8 But;

ui16 Proc;							//текущее состояние тех.процесса
									//=1 проверяем наличие Щ300, и его режим работы, включаем после отрисовки экрана 3
									// и БПМ

ui16 ProcOld;						//индикатор "proc"

ui16 MsgId;							//ID текущего сообщения
ui16 MsgIdNew;						//ID сообщения которое будет

ui8 TxBuf[32];
ui8 RxBuf[RXBUFSIZE];
ui8 RxPtr;
ui8 TempBuf[4];

ui8 flKey1, flKey2, flKey4, flKey8, flParNum;

ui8 flPar;
ui8 flParOld;


bool flSPIEnd;						//SPI закончил
ui16 SPIBuf[2];


ui8 ProcCount;

#define BEEP_0ff;	    FIO0PIN&=~(1<<23)
#define BEEP_0n;	    FIO0PIN|=(1<<23)


//Ui16Var Res={500,500,0,1000,0};
//Ui16Var RunMode={0,0,0,3,0};

ui16 Debug;
