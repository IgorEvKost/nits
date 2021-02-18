/*
    2.25 В 278 изменена проверка на ток.
    Удалено: if(BP1.I.Val > I0+40 )
    Вставлено: if(BP1.I.Val > 0 )
        ***
    2.24 Изменён учет ошибок транзакций в файле bpm.c
*/


#include <stdio.h>
#include <math.h>
#include <LPC2478.h>
#include "widgets.h"
#include "var.h"

//координаты общей компоновки настроек
#define PX0	0						//X
#define	PY0 -60						//Y
#define MSGCOLOR GREEN				//цвет сообщений

#define led2	0x00800000
#define CLED	0x80000000			//светодиод энкодера
#define TXPIN	(1<<19)				//порт 0.19
#define RXPIN	(1<<26)				//порт 4.26

//координаты и размеры кнопок
#define	BTN1	535,25,100,60       //EXEC,
#define	BTN2	535,143,100,60      //START,RET,STOP
#define	BTN4	535,265,100,60      //TUNE,CORR,PAUSE
#define	BTN8	535,385,100,60		//SEL, CONT

//смещение левого и правого столбика параметров
#define XL		-15
#define XR		15

//координаты и размеры рамок параметров
#define	FT1		165+PX0+XL,210+PY0,100,40		// время отпыла
#define	FT2		165+PX0+XL,290+PY0,100,40		// время процесса
#define FI1		365+PX0+XR,210+PY0,85,40		// ток отпыла
#define FI2		365+PX0+XR,290+PY0,85,40		// ток процесса
#define FP		365+PX0+XR,370+PY0,85,40		// пауза
#define	FRZAD	150+PX0+XL,370+PY0,95,40		// цель сопротивления
#define	FTOLM	240+PX0+XL,353+PY0,65,40		// допуск с низу
#define	FTOLP	240+PX0+XL,398+PY0,65,40		// допуск с верху

//координаты строк
#define ST1		100+PX0+XL,220+PY0				// строка "T1="
#define DT1		170+PX0+XL,220+PY0

#define ST2		100+PX0+XL,300+PY0				// строка "T2="
#define DT2		170+PX0+XL,300+PY0

#define	SI1		300+PX0+XR,220+PY0				// строка "I1="
#define DI1		370+PX0+XR,220+PY0

#define	SI2		300+PX0+XR,300+PY0				// строка "I2="
#define DI2		370+PX0+XR,300+PY0

#define	SP		300+PX0+XR,380+PY0				// строка "Tf="
#define DP		370+PX0+XR,380+PY0

#define SR		100+PX0+XL,380+PY0				//строка "R="
#define	DR		155+PX0+XL,380+PY0				//значение после "R="

#define DTOLM	250+PX0+XL,365+PY0				//значение допуска -
#define DTOLP	258+PX0+XL,410+PY0				//значение допуска +

#define S300	50,300				//строка "R>"
#define	S300DATA	220,300			//данные от Щ300

#define	STIME	50,200				//строка "T>"
#define DTIME	220,200				//данные таймера

#define STS		10,5				//строка "md:     proc:     sbp:    spi:    n:    t: "
#define STSMD	20,2
#define STSPROC 50,2
#define STSBP	200,2
#define SMSG0	30,420				//строка УСТАНОВКА ПАРАМЕТРОВ
#define SMSG1	190,420				//строка ОЧИСТКА
#define SMSG2	150,420				//строка ОЖИДАНИЕ
#define SMSG3	150,420				//строка НАНЕСЕНИЕ СЛОЯ
#define SMSG4	150,420				//строка ИЗМЕРЕНИЕ
#define SMSG5	220,450				//строка ОСТАНОВ
#define SMSG6	185,410				//строка ПО ТАЙМЕРУ
#define SMSG7	110,410				//строка ПО СОПРОТИВЛЕНИЮ
#define SMSG8	175,410				//строка ПО ВРАЩЕНИЮ

//статус устройств
#define SBPM	50,45				//строка БПМ
#define	SIZM	180,45				//строка ИЗМ
#define SROT	315,45				//строка ВРАЩ

#define SV		4,465				//версия ПО


//
const ui8 BPM[4]={0xC1,0xCF,0xCC,0};				//БПМ
const ui8 IZM[5]={0xD9,0x33,0x30,0x30,0};			//Щ300
const ui8 ROT[5]={0xC2,0xD0,0xC0,0xD9,0};			//ВРАЩ

//#20

const ui8 MSGVER[14]={'2','.','2','5','_','1','8','.','1','2','.','2','0',0};

const ui8 MSG0[21]={0xD3,0xD1,0xD2,0xC0,0xCD,0xCE,0xC2,0xCA,0xC0,0x20,\
					0xCF,0xC0,0xD0,0xC0,0xCC,0xC5,0xD2,0xD0,0xCE,0xC2,0};								//УСТАНОВКА ПАРАМЕТРОВ
const ui8 MSG1[8]={0xCE,0xD7,0xC8,0xD1,0xD2,0xCA,0xC0,0};												//ОЧИСТКА
const ui8 MSG2[9]={0xCE,0xC6,0xC8,0xC4,0xC0,0xCD,0xC8,0xC5,0};											//ОЖИДАНИЕ
const ui8 MSG3[15]={0xCD,0xC0,0xCD,0xC5,0xD1,0xC5,0xCD,0xC8,0xC5,0x20,0xD1,0xCB,0xCE,0xDF,0};			//НАНЕСЕНИЕ СЛОЯ
const ui8 MSG4[10]={0xC8,0xC7,0xCC,0xC5,0xD0,0xC5,0xCD,0xC8,0xC5,0};									//ИЗМЕРЕНИЕ
const ui8 MSG5[8]={0xCE,0xD1,0xD2,0xC0,0xCD,0xCE,0xC2,0};												//ОСТАНОВ
const ui8 MSG6[11]={0xCF,0xCE,0x20,0xD2,0xC0,0xC9,0xCC,0xC5,0xD0,0xD3,0};								//ПО ТАЙМЕРУ
const ui8 MSG7[17]={0xCF,0xCE,0x20,0xD1,0xCE,0xCF,0xD0,0xCE,0xD2,0xC8,0xC2,0xCB,0xC5,0xCD,0xC8,0xDE,0}; //ПО СОПРОТИВЛЕНИЮ
const ui8 MSG8[12]={0xCF,0xCE,0x20,0xC2,0xD0,0xC0,0xD9,0xC5,0xCD,0xC8,0xDE,0};							//ПО ВРАЩЕНИЮ


extern const uint8_t font16[], font20[], font30[], font60[];
Font fnt16, fnt20, fnt30, fnt60;


void init();
void Timer10IntrHandler();
void LCD_Initialize();
void UART2IntrHandler();
extern void SPIIntrHandler();
void debug();
void rotprint();
void perprint();
//void fazaprint();

//void bpm_get_stat(ui8 adr);
BPM_Struc bpm_get_stat(BPM_Struc);
BPM_Struc bpm_command(BPM_Struc, ui8, ui8);
BPM_Struc bpm_set_point(BPM_Struc);
BPM_Struc bpm_get_data(BPM_Struc, ui8);
BPM_Struc bpm_start(BPM_Struc);
BPM_Struc bpm_stop(BPM_Struc);

Param_Struc encoder(Param_Struc);


ui8 key();
void tprint(char* buff,	ui16 par);
void fprint(char* buff,	float f);



ui16*	ptr;                    //current wr/rd pointer
ui16	fl1;
ui16	Flash=0;
ui16	IOut;		//ток нагрузки (в отсчетах)



Button btn_TUNE __attribute__ ((section (".dram"))) ;					// screen 1
Button btn_START __attribute__ ((section (".dram"))) ;					// screen 1
Button btn_EXEC __attribute__ ((section (".dram"))) ;					// screen 1,2,3
Button btn_SEL __attribute__ ((section (".dram"))) ;					// screen 2
Button btn_CORR __attribute__ ((section (".dram"))) ;					// screen 2
Button btn_RET __attribute__ ((section (".dram"))) ;					// screen 2
Button btn_PAUSE __attribute__ ((section (".dram"))) ;					// screen 3
Button btn_STOP __attribute__ ((section (".dram"))) ;					// screen 3
Button btn_CONT __attribute__ ((section (".dram")));					// screen 3



ui16 btn_TUNE_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_START_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_EXEC_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_SEL_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_CORR_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_RET_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_PAUSE_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_STOP_bk[250*60] __attribute__ ((section (".dram"))) ;
ui16 btn_CONT_bk[250*60] __attribute__ ((section (".dram"))) ;






//ui16 RunMode;					//режим работы
ui16 fScrnOk;					//флаг обновления экрана
ui8 buffer[16];
ui8 irq_buf[16];
//ui16 Temp;
COLOUR_TYPE BackColour;

ui8 fUpd=0;
ui8 fPowerOn;


ui32 TolPVar __attribute__ ((section (".bsram"))) ;
ui32 TolMVar __attribute__ ((section (".bsram"))) ;
ui32 ResVar __attribute__ ((section (".bsram"))) ;
ui32 I1Var __attribute__ ((section (".bsram"))) ;
ui32 I2Var __attribute__ ((section (".bsram"))) ;
ui32 TfVar __attribute__ ((section (".bsram"))) ;
ui32 T1Var __attribute__ ((section (".bsram"))) ;
ui32 T2Var __attribute__ ((section (".bsram"))) ;


int main(void)
{

//	TolPVar=100;
//	TolMVar=100;
//	ResVar=2000;
//	I1Var=400;
//	I2Var=500;
//	TfVar=100;
//	T1Var=5;
//	T2Var=240;


//#19

	Param_Struc RunMode={false,1,1,1,1,3,""};
	Param_Struc ParNum={false,1,1,1,1,8,""};				//селектор параметра для коррекции
	Param_Struc TolP={false,10,20,20,0,500,""};

	if(TolPVar>TolP.Max)
	{
		TolPVar=0;
	}


	TolP.Var=TolPVar;
	TolP.VarOld=TolP.Var;

	Param_Struc TolM={false,10,20,20,0,500,""};

	if(TolMVar>TolM.Max)
	{
		TolMVar=0;
	}

	TolM.Var=TolMVar;
	TolM.VarOld=TolM.Var;

	Param_Struc Res={false,10,500,500,0,12000,""};

	if(ResVar>Res.Max)
	{
		ResVar=0;
	}

	Res.Var=ResVar;
	Res.VarOld=Res.Var;

	Param_Struc I1={false,8,0,0,0,2000,""};			//ток отпыла на задвижку

	if(I1Var>I1.Max)
	{
		I1Var=0;
	}

	I1.Var=I1Var;
	I1.VarOld=I1.Var;

	Param_Struc I2={false,8,0,0,0,2000,""};			//ток процесса

	if(I2Var>I2.Max)
	{
		I2Var=0;
	}

	I2.Var=I2Var;
	I2.VarOld=I2.Var;

	Param_Struc Tf={false,1,0,0,0,200,""};				//пауза

	if(TfVar>Tf.Max)
	{
		TfVar=0;
	}

	Tf.Var=TfVar;
	Tf.VarOld=Tf.Var;

	Param_Struc T1={false,1,0,0,0,3600,""};			//время отпыла на задвижку

	if(T1Var>T1.Max)
	{
		T1Var=0;
	}

	T1.Var=T1Var;
	T1.VarOld=T1.Var;

	Param_Struc T2={false,1,0,0,0,3600,""};			//время процесса (максимальное)

	if(T2Var>T2.Max)
	{
		T2Var=0;
	}

	T2.Var=T2Var;
	T2.VarOld=T2.Var;

	BP1.Adr=0xFF;
	BP1.Iz.Val=0;
	BP1.Uz.Val=0;
	BP1.Pz.Val=0;

	S300Data.VarOld=Res.Max;


	init();

	LCD_Initialize();			//при переносе в init() зависает!!!




	LoadFont(&fnt16, font16);
	LoadFont(&fnt20, font20);
	LoadFont(&fnt30, font30);
	LoadFont(&fnt60, font60);


//	FIO2PIN=1;

	BackColour=DDGRAY;

	/*рисуем рабочий стол в двух кадровых буферах*/
	for(ptr=(ui16*)LCD_BUFFER0_BASE_ADDR; ptr<(ui16*)(LCD_BUFFER0_END_ADDR);ptr++)
	{
		*ptr=BackColour;
	}





//	FIO2PIN=0;
//	FIO4PIN|=led2;

	Button tmp={255,BTN1,2,&fnt20,{0xC8,0xD1,0xCF,0},{0xC8,0xD1,0xCF,0},/*"EXEC","EXEC",*/BLACK,RED,DGRAY,WHITE,BLACK,0,0,btn_EXEC_bk};
	btn_EXEC=tmp;
	button(btn_EXEC);
	btn_EXEC.st=2;
	button(btn_EXEC);


	Button tmp1={255,BTN4,2,&fnt20,{0xD3,0xD1,0xD2,0}/*"TUNE"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_TUNE_bk};
	btn_TUNE=tmp1;
	button(btn_TUNE);
	btn_TUNE.st=0;
	button(btn_TUNE);

	Button tmp2={255,BTN4,2,&fnt20,{0xCA,0xCE,0xD0,0xD0,0}/*"CORR"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_CORR_bk};
	btn_CORR=tmp2;
	button(btn_CORR);
	btn_CORR.st=0;
	button(btn_CORR);

	Button tmp3={255,BTN4,2,&fnt20,{0xCF,0xC0,0xD3,0xC7,0xC0,0}/*"PAUSE"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_PAUSE_bk};
	btn_PAUSE=tmp3;
	button(btn_PAUSE);
	btn_PAUSE.st=0;
	button(btn_PAUSE);

	Button tmp4={255,BTN2,2,&fnt20,{0xD1,0xD2,0xC0,0xD0,0xD2,0}/*"START"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_START_bk};
	btn_START=tmp4;
	button(btn_START);
	btn_START.st=0;
	button(btn_START);

	Button tmp5={255,BTN2,2,&fnt20,{0xC2,0xCE,0xC7,0xC2,0xD0,0}/*"RET"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_RET_bk};
	btn_RET=tmp5;
	button(btn_RET);
	btn_RET.st=0;
	button(btn_RET);

	Button tmp6={255,BTN2,2,&fnt20,{0xD1,0xD2,0xCE,0xCF,0}/*"STOP"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_STOP_bk};
	btn_STOP=tmp6;
	button(btn_STOP);
	btn_STOP.st=0;
	button(btn_STOP);

	Button tmp7={255,BTN8,2,&fnt20,{0xC2,0xDB,0xC1,0}/*"SEL"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_SEL_bk};
	btn_SEL=tmp7;
	button(btn_SEL);
	btn_SEL.st=0;
	button(btn_SEL);

	Button tmp8={255,BTN8,2,&fnt20,{0xCF,0xD0,0xCE,0xC4,0}/*"CONT"*/,"",BLACK,0xfffd,WHITE,RED,WHITE,0,0,btn_CONT_bk};
	btn_CONT=tmp8;
	button(btn_CONT);
	btn_CONT.st=0;
	button(btn_CONT);

//#18

	DrawString(SV,&fnt16,MSGVER,GREEN);				//версия

	DrawString(STS,&fnt20,"md:             proc:           sbp:             spi:       n:        t:",GREEN);

	while(1)						//крутимся здесь
	{

		if(RunMode.Var==0)
		{

		}



//*******************
//*    Режим 1	    *
//*******************
		if(RunMode.Var==1)
	    {
			if(Proc==60 || Proc==61 || Proc==62) {}// Proc=10;
			else Proc=0;

			Timer1.On=false;

			if(fScrnOk==0)
			{						  //отрисовка экрана
				btn_PAUSE.st=0;
				button(btn_PAUSE);

				btn_STOP.st=0;
				button(btn_STOP);

				btn_SEL.st=0;
				button(btn_SEL);

				btn_CORR.st=0;
				button(btn_CORR);

				btn_TUNE.st=1;
				btn_TUNE.brd_color=BLACK;
				btn_TUNE.b_w=2;
				button(btn_TUNE);

				btn_START.st=1;
				btn_START.brd_color=BLACK;
				btn_START.b_w=2;
				button(btn_START);

				btn_EXEC.st=2;
				button(btn_EXEC);

				switch(ParNum.Var)
				{
					case 1:		rect(FT1,BackColour);		//время отпыла
					break;

					case 2:		rect(FT2,BackColour);		//время процесса
					break;

					case 3:		rect(FRZAD,BackColour);		//сопротивление
					break;

					case 4:		rect(FTOLM,BackColour);		//допуск с низу
					break;

					case 5:		rect(FTOLP,BackColour);		//допуск с верху
					break;

					case 6:		rect(FI1,BackColour);		//ток отпыла
					break;

					case 7:		rect(FI2,BackColour);		//ток процесса
					break;

					case 8:		rect(FP,BackColour);		//ток подгонки
					break;
				}


				DrawString(DR,&fnt30,Res.buf,BackColour);
				DrawString(DTOLP,&fnt20,TolP.buf,BackColour);
				DrawString(DTOLM,&fnt20,TolM.buf,BackColour);
				DrawString(DI1,&fnt30,I1.buf,BackColour);
				DrawString(DI2,&fnt30,I2.buf,BackColour);
				DrawString(DP,&fnt30,Tf.buf,BackColour);
				DrawString(DT1,&fnt30,T1.buf,BackColour);
				DrawString(DT2,&fnt30,T2.buf,BackColour);
				DrawString(SR,&fnt30,"R=",BackColour);
				DrawString(SI1,&fnt30,"I1=",BackColour);
				DrawString(SI2,&fnt30,"I2=",BackColour);
				DrawString(SP,&fnt30,"Tf=",BackColour);
				DrawString(ST1,&fnt30,"T1=",BackColour);
				DrawString(ST2,&fnt30,"T2=",BackColour);


				//при переходе из режимов 3,34,32
//#17

				DrawString(S300DATA,&fnt60,S300Data.buf,BackColour);
				DrawString(S300,&fnt60,"R>",BackColour);

				DrawString(STIME,&fnt60,"T>",BackColour);
				DrawString(DTIME,&fnt60,Timer1.buf,BackColour);

				DrawString(SBPM,&fnt30,BPM,BackColour);
				DrawString(SIZM,&fnt30,IZM,BackColour);
				DrawString(SROT,&fnt30,ROT,BackColour);

//@1				MsgIdNew=255;									//удаляем УСТАНОВКА ПАРАМЕТРОВ


				fScrnOk=1;

				S300ok=0;
				BP1ok=0;
				flRotOk=false;

				S300Mode.Var=1;
				S300Mode.VarOld=0;
				S300Mode.Upd=0;

				xsprintf(buffer,"%d", RunMode.VarOld);			//##
				DrawString(55,5,&fnt20,buffer,BackColour);		//##
				xsprintf(buffer,"%d", RunMode.Var);				//##
				DrawString(55,5,&fnt20,buffer,GREEN);			//##

				proc();
				spi();
				spb();
//				fazaprint();

				Rotator.On=false;
				Rotator.Val=0;
				rotprint();

				MsgIdNew=255;

				ProcCount=0;

			}
	    }

//***************************************
//*				Режим 12                *
//*		(в режиме 1 нажата клавиша 2)	*
//***************************************
		if(RunMode.Var==12)
		{
			if(fScrnOk==0)
			{
				btn_TUNE.brd_color=BLACK;
				btn_TUNE.b_w=2;
				button(btn_TUNE);
				btn_START.brd_color=GREEN;
				btn_START.b_w=5;
				button(btn_START);
				btn_EXEC.st=1;
				button(btn_EXEC);
				fScrnOk=1;
			}
		}

//***************************************
//*				Режим 14                *
//*		(в режиме 1 нажата клавиша 4)	*
//***************************************
		if(RunMode.Var==14)
		{
			if(fScrnOk==0)
			{
				btn_START.brd_color=BLACK;
				btn_START.b_w=2;
				button(btn_START);
				btn_TUNE.brd_color=GREEN;
				btn_TUNE.b_w=5;
				button(btn_TUNE);
				btn_EXEC.st=1;
				button(btn_EXEC);
				fScrnOk=1;
			}
		}


//*******************
//*     Режим 2	    *
//*******************

	    if(RunMode.Var==2)
	    {
			if(fScrnOk==0)
			{
				btn_TUNE.st=0;
				button(btn_TUNE);

				btn_START.st=0;
				button(btn_START);

				btn_SEL.st=1;
				btn_SEL.brd_color=BLACK;
				btn_SEL.b_w=2;
				button(btn_SEL);

				btn_CORR.st=1;
				btn_CORR.brd_color=BLACK;
				btn_CORR.b_w=2;
				button(btn_CORR);

				btn_RET.st=1;
				btn_RET.brd_color=BLACK;
				btn_RET.b_w=2;
				button(btn_RET);

				btn_EXEC.st=2;
				button(btn_EXEC);

				DrawString(SR,&fnt30,"R=",GREEN);

				xsprintf(Res.buf,"%d", Res.Var);
				DrawString(DR,&fnt30,Res.buf,GREEN);

				xsprintf(TolP.buf,"%d", TolP.Var);
				DrawString(DTOLP,&fnt20,TolP.buf,GREEN);		//верхний допуск

				xsprintf(TolM.buf,"%d", -1*TolM.Var);
				DrawString(DTOLM,&fnt20,TolM.buf,GREEN);		//нижний допуск

				DrawString(SI1,&fnt30,"I1=",GREEN);
				fprint(I1.buf, I1.Var*KI);
				DrawString(DI1,&fnt30,I1.buf,GREEN);

				DrawString(SI2,&fnt30,"I2=",GREEN);
				fprint(I2.buf, I2.Var*KI);
				DrawString(DI2,&fnt30,I2.buf,GREEN);

				DrawString(SP,&fnt30,"Tf=",GREEN);
				xsprintf(Tf.buf,"%d", Tf.Var);
				DrawString(DP,&fnt30,Tf.buf,GREEN);

				DrawString(ST1,&fnt30,"T1=",GREEN);
				tprint(T1.buf,T1.Var);
				DrawString(DT1,&fnt30,T1.buf,GREEN);

				DrawString(ST2,&fnt30,"T2=",GREEN);
				tprint(T2.buf,T2.Var);
				DrawString(DT2,&fnt30,T2.buf,GREEN);

				MsgIdNew=0;								//УСТАНОВКА ПАРАМЕТРОВ

				fScrnOk=1;
			}
	    }

//***************************************
//*				Режим 22                *
//*  (в режиме 2 нажата клавиша 2(RET))	*
//***************************************
		if(RunMode.Var==22)
		{
			if(fScrnOk==0)
			{
				btn_SEL.brd_color=BLACK;
				btn_SEL.b_w=2;
				button(btn_SEL);
				btn_CORR.brd_color=BLACK;
				btn_CORR.b_w=2;
				button(btn_CORR);
				btn_RET.brd_color=GREEN;
				btn_RET.b_w=5;
				button(btn_RET);

				btn_EXEC.st=1;
				button(btn_EXEC);


				fScrnOk=1;
			}
		}


//***************************************
//*				Режим 24                *
//* (в режиме 2 нажата клавиша 4(CORR))	*
//***************************************
	if(RunMode.Var==24)
	{
		if(fScrnOk==0)
		{
			btn_SEL.brd_color=BLACK;
			btn_SEL.b_w=2;
			button(btn_SEL);
			btn_CORR.brd_color=GREEN;
			btn_CORR.b_w=5;
			button(btn_CORR);
			btn_RET.brd_color=BLACK;
			btn_RET.b_w=2;
			button(btn_RET);

			btn_EXEC.st=1;
			button(btn_EXEC);


			fScrnOk=1;
		}

		if(flParNum==0)
		{
			switch(ParNum.Var)						// отрисовка рамок выбора параметра
			{
				case 1:								// время отпыла
				{
					rect(FT1,GREEN);
				}
				break;

				case 2:								// время процесса
				{
					rect(FT2,GREEN);
				}
				break;

				case 3:								// сопротивление
				{
					rect(FRZAD,GREEN);
				}
				break;

				case 4:								// допуск с низу
				{
					rect(FTOLM,GREEN);
				}
				break;

				case 5:								// допуск с верху
				{
					rect(FTOLP,GREEN);
				}
				break;

				case 6:								// ток отпыла
				{
					rect(FI1,GREEN);
				}
				break;

				case 7:								// ток процесса
				{
					rect(FI2,GREEN);
				}
				break;

				case 8:
				{
					rect(FP,GREEN);					// пауза
				}
				break;

			}
			flParNum=1;
		}
	}


//***************************************
//*	        Режим 28					*
//*(в режиме 2 нажата клавиша 8(SEL))	*
//***************************************
	if(RunMode.Var==28)
	{
		if(fScrnOk==0)
		{
			btn_SEL.brd_color=GREEN;
			btn_SEL.b_w=5;
			button(btn_SEL);
			btn_CORR.brd_color=BLACK;
			btn_CORR.b_w=2;
			button(btn_CORR);
			btn_RET.brd_color=BLACK;
			btn_RET.b_w=2;
			button(btn_RET);

			btn_EXEC.st=1;
			button(btn_EXEC);


			fScrnOk=1;
		}

		if(flParNum==0)
		{
		switch(ParNum.Var)						// отрисовка рамок выбора параметра
		{
			case 1:
			{
				rect(FP,BackColour);			// ток подгонки
				rect(FT1,GREEN);				// время отпыла
			}
			break;

			case 2:
			{
				rect(FT1,BackColour);			// время отпыла
				rect(FT2,GREEN);				// время процесса
			}
			break;

			case 3:
			{
				rect(FT2,BackColour);			// время процесса
				rect(FRZAD,GREEN);				// сопротивление
			}
			break;

			case 4:
			{
				rect(FRZAD,BackColour);			// сопротивление
				rect(FTOLM,GREEN);				// допуск с низу
			}
			break;

			case 5:
			{
				rect(FTOLM,BackColour);			// допуск снизу
				rect(FTOLP,GREEN);				// допуск с верху
			}
			break;

			case 6:
			{
				rect(FTOLP,BackColour);			// допуск с верху
				rect(FI1,GREEN);				// ток отпыла
			}
			break;

			case 7:
			{
				rect(FI1,BackColour);			// ток отпыла
				rect(FI2,GREEN);				// ток процесса
			}
			break;

			case 8:
			{
				rect(FI2,BackColour);			// ток процесса
				rect(FP,GREEN);				// ток подгонки
			}
			break;
		}
		flParNum=1;
		}
	}



//*******************
//*     Режим 3	    *
//*******************
	    if(RunMode.Var==3)
	    {
			if(fScrnOk==0)				//отрисовка экрана при входе в режим 3
			{

				Timer1.Val=T1.Var;		//время отпыла на задвижку
				S300Mode.Upd=1;
				RxBuf[12]=0;

				btn_SEL.st=0;
				button(btn_SEL);
			//	RegionFill(btn_SEL.x0,btn_SEL.y0,btn_SEL.w+1,btn_SEL.h+1,BackColour);
				btn_CORR.st=0;
				button(btn_CORR);
				btn_RET.st=0;
				button(btn_RET);
//				btn_PAUSE.st=1;
//				button(btn_PAUSE);
				btn_STOP.st=1;
				button(btn_STOP);

				btn_EXEC.st=2;
				button(btn_EXEC);

//#16

				DrawString(S300,&fnt60,"R>",GREEN);
//				xsprintf(S300Data.buf,"%d", S300Data.VarOld);
				DrawString(S300DATA,&fnt60,S300Data.buf,BackColour);
				xsprintf(S300Data.buf,"%04d", S300Data.Var);
				DrawString(S300DATA,&fnt60,S300Data.buf,GREEN);
				S300Data.VarOld=S300Data.Var;

				DrawString(STIME,&fnt60,"T>",GREEN);
//				tprint(Timer1.buf,Timer1.VarOld);
				DrawString(DTIME,&fnt60,Timer1.buf,BackColour);
				tprint(Timer1.buf,Timer1.Val);
				DrawString(DTIME,&fnt60,Timer1.buf,GREEN);






				DrawString(SBPM,&fnt30,BPM,BLACK);
				DrawString(SIZM,&fnt30,IZM,BLACK);
				DrawString(SROT,&fnt30,ROT,BLACK);

//#15
				MsgIdNew=1;									//ОЧИСТКА

				fScrnOk=1;
				Proc=1;										//проверка Щ300, БПМ, вращения
				flSPI=0;

				BP1.Req=false;
				BP1.Rsp=false;

				flSPIEnd=true;

				flRotUpd=true;

			}

			//
			//
			//фоновые процессы режима 3
			//
			//
//#14
	    }

//***************************************
//*		       Режим 30					*
//*		*
//***************************************
		if(RunMode.Var==30)
		{
			if(fScrnOk==0)
			{
				btn_CORR.st=0;
				button(btn_CORR);

//				btn_PAUSE.st=1;
//				btn_PAUSE.brd_color=BLACK;
//				btn_PAUSE.b_w=2;
//				button(btn_PAUSE);

				btn_STOP.brd_color=BLACK;
				btn_STOP.b_w=2;
				button(btn_STOP);

				btn_CONT.st=0;
				button(btn_CONT);

				btn_EXEC.st=2;
				button(btn_EXEC);

				MsgIdNew=3;						//НАНЕСЕНИЕ СЛОЯ

//@				Proc=24;
				fScrnOk=1;
			}
		}


//***************************************
//*		       Режим 300				*
//*										*
//***************************************
		if(RunMode.Var==300)
		{
			if(fScrnOk==0)
			{
//				btn_PAUSE.st=0;
//				button(btn_PAUSE);

				btn_CONT.st=0;
				button(btn_CONT);

				btn_STOP.brd_color=BLACK;
				btn_STOP.b_w=2;
				button(btn_STOP);

				btn_EXEC.st=2;
				button(btn_EXEC);

				fScrnOk=1;
			}
		}


//***************************************
//*		       Режим 3002				*
//*	в режиме 300 нажата клавиша 2 (STOP)*
//***************************************
		if(RunMode.Var==3002)
		{
			if(fScrnOk==0)
			{
				btn_STOP.brd_color=GREEN;
				btn_STOP.b_w=5;
				button(btn_STOP);

				btn_EXEC.st=1;
				button(btn_EXEC);

				fScrnOk=1;
			}
		}



//***************************************
//*		       Режим 32					*
//*	в режиме 3 нажата клавиша 2 (STOP)	*
//***************************************
		if(RunMode.Var==32 || RunMode.Var==302)
		{
			if(fScrnOk==0)
			{
				btn_STOP.brd_color=GREEN;
				btn_STOP.b_w=5;
				button(btn_STOP);

//				btn_PAUSE.brd_color=BLACK;
//				btn_PAUSE.b_w=2;
//				button(btn_PAUSE);

				btn_EXEC.st=1;
				button(btn_EXEC);

				fScrnOk=1;
			}
		}


//******************************************
//*		        Режим 34            	   *
//*	в режиме 3 нажата клавиша 4 PAUSE	   *
//******************************************
		if(RunMode.Var==34 || RunMode.Var==304)
		{
			if(fScrnOk==0)
			{
				btn_PAUSE.brd_color=GREEN;
				btn_PAUSE.b_w=5;
				button(btn_PAUSE);

				btn_STOP.brd_color=BLACK;
				btn_STOP.b_w=2;
				button(btn_STOP);

				btn_EXEC.st=1;
				button(btn_EXEC);

				fScrnOk=1;
			}
		}

//******************************************
//*		        Режим 35            	   *
//*										   *
//******************************************

		if(RunMode.Var==35)
		{
			if(fScrnOk==0)
			{
				btn_PAUSE.st=0;
				button(btn_PAUSE);

				btn_CONT.st=1;
				btn_CONT.brd_color=BLACK;
				btn_CONT.b_w=2;
				button(btn_CONT);

				btn_STOP.brd_color=BLACK;
				btn_STOP.b_w=2;
				button(btn_STOP);

				btn_EXEC.st=2;
				button(btn_EXEC);

				MsgIdNew=2;							//ОЖИДАНИЕ

				fScrnOk=1;
			}
		}


//***************************************
//*		           Режим 352			*
//*	в режиме 35 нажата клавиша 2 (STOP)	*
//***************************************
		if(RunMode.Var==352)
		{
			if(fScrnOk==0)
			{
				btn_CONT.brd_color=BLACK;
				btn_CONT.b_w=2;
				button(btn_CONT);

				btn_STOP.brd_color=GREEN;
				btn_STOP.b_w=5;
				button(btn_STOP);

				btn_EXEC.st=1;
				button(btn_EXEC);

				fScrnOk=1;
			}
		}

//***************************************
//*		           Режим 358           	*
//*	в режиме 35 нажата клавиша 8 (CONT)	*
//***************************************
		if(RunMode.Var==358)
		{
			if(fScrnOk==0)
			{
				btn_CONT.brd_color=GREEN;
				btn_CONT.b_w=5;
				button(btn_CONT);

				btn_STOP.brd_color=BLACK;
				btn_STOP.b_w=2;
				button(btn_STOP);

				btn_EXEC.st=1;
				button(btn_EXEC);

				fScrnOk=1;
			}
		}



//***************************************
//***************************************
//**      Конец отрисовки режимов      **
//***************************************
//***************************************





//*******************************
//*		Фоновые процедуры		*
//*******************************


//
//нажатия в режиме 1
//
		if(key()==0x01&&RunMode.Var==12)	//START в зелёной рамке
		{
			btn_STOP.brd_color=BLACK;
			btn_STOP.b_w=2;
			RunMode.Var=3;
			//Proc=0;							//инициализируем состояние тех.процесса
			Proc=1;
//@			RunMode.Upd=1;
			BeepCnt=0;
			BEEP_0n;
		}

		if(key()==0x01&&RunMode.Var==14)	//TUNE в зелёной рамке
		{
			RunMode.Var=2;
//@			RunMode.Upd=1;
			BeepCnt=0;
			BEEP_0n;
		}

		if(key()==0x02&&/*flKey2==0&&*/(RunMode.Var==1||RunMode.Var==14))
		{
			RunMode.Var=12;
//@			RunMode.Upd=1;
//			flKey2=1;
			BeepCnt=0;
			ModeCnt=0;
			BEEP_0n;
		}

		if(key()==0x04&&/*flKey4==0&&*/(RunMode.Var==1||RunMode.Var==12))
		{
			RunMode.Var=14;
//@			RunMode.Upd=1;
//			flKey4=1;
			BeepCnt=0;
			ModeCnt=0;
			BEEP_0n;
		}

//
//нажатия в режиме 2
//

		if(key()==0x01&&flKey1==0&&RunMode.Var==22)
		{
			btn_TUNE.brd_color=BLACK;
			btn_TUNE.b_w=2;
			btn_RET.brd_color=BLACK;
			btn_RET.b_w=2;

			FIO1PIN&=~CLED;					// P1.31 гасим

			T1Var=(ui32)T1.Var;				//сохраняем в батарейном озу
			T2Var=(ui32)T2.Var;
			I1Var=(ui32)I1.Var;
			I2Var=(ui32)I2.Var;
			TfVar=(ui32)Tf.Var;
			ResVar=(ui32)Res.Var;
			TolMVar=(ui32)TolM.Var;
			TolPVar=(ui32)TolP.Var;

			RunMode.Var=1;
//@			RunMode.Upd=1;
			flKey1=1;
			BeepCnt=0;
			BEEP_0n;

		}

		if(key()==0x01&&flKey1==0&&RunMode.Var==24)		//нажата key1 при зеленой рамке на CORR
		{
			RunMode.Var=241;
			FIO1PIN|=CLED;								//зажигаем
//			RunMode.Var=12;
			RunMode.Upd=1;
			flKey1=1;
			BeepCnt=0;
			BEEP_0n;
		}

		if(key()==0x01&&flKey1==0&&RunMode.Var==28)		//выбор параметра
		{
			if(ParNum.Var<ParNum.Max)
			{
				ParNum.VarOld=ParNum.Var;
				ParNum.Var++;
			}
			else
			{
				ParNum.VarOld=ParNum.Var;
				ParNum.Var=ParNum.Min;
			}
			flParNum=0;
//@			RunMode.Upd=1;
			flKey1=1;
//#13
			ModeCnt=0;
			BeepCnt=0;
			BEEP_0n;
		}


		if(key()==0x02&&flKey2==0&&(RunMode.Var==2||RunMode.Var==24||RunMode.Var==28 || RunMode.Var==241))
		{
			RunMode.Var=22;
			flKey2=1;
			BeepCnt=0;
			ModeCnt=0;
			FIO1PIN&=~CLED;								//гасим
			BEEP_0n;
		}

		if(key()==0x04&&flKey4==0&&(RunMode.Var==2||RunMode.Var==22||RunMode.Var==28))
		{
			flParNum=0;
			RunMode.Var=24;
			flKey4=1;
			BeepCnt=0;
			ModeCnt=0;
			BEEP_0n;
		}

		if(key()==0x08&&flKey8==0&&(RunMode.Var==2||RunMode.Var==22||RunMode.Var==24||RunMode.Var==241))
		{
			RunMode.Var=28;
			flParNum=0;
			flKey8=1;
			BeepCnt=0;
			ModeCnt=0;
			FIO1PIN&=~CLED;								//гасим
			BEEP_0n;
		}


//
//нажатия в режиме 3xx
//
		if(key()==0x01&&(RunMode.Var==32 || RunMode.Var==302 || RunMode.Var==352 || RunMode.Var==3002))	//STOP в зелёной рамке
		{
			btn_START.brd_color=BLACK;
			btn_START.b_w=2;
			MsgIdNew=255;
			RunMode.Var=0; //@
			Proc=10;
		}

		if(key()==0x01&&(RunMode.Var==34||RunMode.Var==304))	//PAUSE в зелёной рамке
		{
			RunMode.Var=3;
		}

		if(key()==0x01&&RunMode.Var==358)	//CONT в зелёной рамке
		{
			Proc=24;						//@
			RunMode.Var=30;
		}

		if(key()==0x02&&/*flKey2==0&&*/(RunMode.Var==3||RunMode.Var==34))
		{
			RunMode.Var=32;
//			flKey2=1;
		}

//#12

		if(key()==0x08&&/*flKey4==0&&*/(RunMode.Var==35||RunMode.Var==352))
		{
			ModeCnt=0;
			RunMode.Var=358;
//			flKey4=1;
		}

		if(key()==0x02&&/*flKey4==0&&*/(RunMode.Var==35||RunMode.Var==358))
		{
			ModeCnt=0;
			RunMode.Var=352;
//			flKey4=1;
		}

		if(key()==0x02&&/*flKey4==0&&*/(RunMode.Var==30||RunMode.Var==304))
		{
			ModeCnt=0;
			RunMode.Var=302;
//			flKey4=1;
		}

//#11
		if(key()==0x02&&RunMode.Var==300)
		{
			ModeCnt=0;
			RunMode.Var=3002;
		}

//***************************************
//***************************************
//**      Конец обработки нажатий      **
//***************************************
//***************************************



		if(key()==0)
		{
			flKey1=0;
			flKey2=0;
			flKey4=0;
			flKey8=0;
		}

//#1

//=====================================================================================================
//											  Коррекция параметров
//=====================================================================================================

		if(RunMode.Var==241)
		{
			switch(ParNum.Var)						//коррекция параметров
			{
				case 1:								//время отпыла
				{
					T1=encoder(T1);
					if(T1.Upd==1)
					{
						tprint(T1.buf, T1.VarOld);
						DrawString(DT1,&fnt30,T1.buf,BackColour);
						tprint(T1.buf,T1.Var);
						DrawString(DT1,&fnt30,T1.buf,GREEN);
						T1.Upd=0;
					}
				}
				break;

				case 2:								//время процесса
				{
					T2=encoder(T2);
					if(T2.Upd==1)
					{
						tprint(T2.buf, T2.VarOld);
						DrawString(DT2,&fnt30,T2.buf,BackColour);
						tprint(T2.buf,T2.Var);
						DrawString(DT2,&fnt30,T2.buf,GREEN);
						T2.Upd=0;
					}
				}
				break;

				case 3:								//задание сопротивления
				{
					Res=encoder(Res);
					if(Res.Upd==1)
					{
						xsprintf(Res.buf,"%d",Res.VarOld);
						DrawString(DR,&fnt30,Res.buf,BackColour);
						xsprintf(Res.buf,"%d", Res.Var);
						DrawString(DR,&fnt30,Res.buf,GREEN);
						Res.Upd=0;
					}
				}
				break;

				case 4:								//допуск с низу
				{
					TolM=encoder(TolM);
					if(TolM.Upd==1)
					{
						xsprintf(TolM.buf,"%d",  -1*TolM.VarOld);
						DrawString(DTOLM,&fnt20,TolM.buf,BackColour);
						xsprintf(TolM.buf,"%d",  -1*TolM.Var);
						DrawString(DTOLM,&fnt20,TolM.buf,GREEN);
						TolM.Upd=0;
					}
				}
				break;

				case 5:								// с верху
				{
					TolP=encoder(TolP);
					if(TolP.Upd==1)
					{
						xsprintf(TolP.buf,"%d", TolP.VarOld);
						DrawString(DTOLP,&fnt20,TolP.buf,BackColour);
						xsprintf(TolP.buf,"%d", TolP.Var);
						DrawString(DTOLP,&fnt20,TolP.buf,GREEN);
						TolP.Upd=0;
					}

				}
				break;

				case 6:								// ток отпыла
				{
					I1=encoder(I1);
					I1Var=(ui32)I1.Var;
					if(I1.Upd==1)
					{
						fprint(I1.buf, I1.VarOld*KI);
						DrawString(DI1,&fnt30,I1.buf,BackColour);
						fprint(I1.buf, I1.Var*KI);
						DrawString(DI1,&fnt30,I1.buf,GREEN);
						I1.Upd=0;
					}
				}
				break;

				case 7:								//ток процесса
				{
					I2=encoder(I2);
					if(I2.Upd==1)
					{
						fprint(I2.buf, I2.VarOld*KI);
						DrawString(DI2,&fnt30,I2.buf,BackColour);
						fprint(I2.buf, I2.Var*KI);
						DrawString(DI2,&fnt30,I2.buf,GREEN);
						I2.Upd=0;
					}
				}
				break;

				case 8:								//фаза выключения
				{
					Tf=encoder(Tf);
					if(Tf.Upd==1)
					{
						xsprintf(Tf.buf,"%d", Tf.VarOld);
						DrawString(DP,&fnt30,Tf.buf,BackColour);
						xsprintf(Tf.buf,"%d", Tf.Var);
						DrawString(DP,&fnt30,Tf.buf,GREEN);
						Tf.Upd=0;
					}
				}
				break;
			}
		}
//=====================================================================================================
//								       Конец коррекции параметров
//=====================================================================================================
/*

							  #####  ###   ###   #   #   ####   ###    ###
							  #   #  #  # #   #  #   #   #     #   #  #   #
							  #   #  ###  #   #  #   #   ###   #      #
							  #   #  #    #   #  #####   #     #   #  #   #
							  #   #  #     ###       #   ####   ###    ###
*/
//=====================================================================================================
//											  Нанесение слёв
//=====================================================================================================
		switch (Proc)
        {
			case 10:								//отключение блока
			{
				BP1.TmrReq=false;
				BP1.Req=true;
				Cnt2=0;
				Proc=11;
			}
			break;

			case 11:								// тут крутимся пока в режиме 3 Proc <- 1
			{
				if((BP1.St.b[1]&STRT)==STRT)		// блок включен
				{
					if(Cnt2>20)
					{

						BP1=bpm_command(BP1,3,0);	//3-выключить источник ("СТОП")

						if(BP1.Error==0)
						{
							BP1.Upd=false;
							S300Mode.Upd=false;
							Cnt2=50;
							Proc=12;
						}

						if(BP1.TmrReq)
						{

							BP1.TmrReq=false;
							BP1.ReqNum=3;
							BP1.Req=true;			//разрешаем повторные запросы
							Cnt2=0;
						}
					}
				}
				else
				{
					RunMode.Var=1;
					Proc=0;
				}
			}
			break;

			case 12:
			{
				if(Cnt2>100)						//надо подождать
				{
					BP1=bpm_get_stat(BP1);

					if(BP1.Error==0)
					{
						RunMode.Var=1;
						Proc=0;
					}


					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=80;				//повторные запросы без секундных пауз
					}
				}
			}
			break;



			//ожидание готовности БПМ, Щ300, вращения
			//сюда попадаем из режима 3
			case 1:
            {

//#2
				//опрос статуса Щ300
				if(flSPI==0)    //при 0 запускаем цикл spi на Щ300, flSPI в прерывании
                {
					//S300ok=0;

					if(flSPIEnd)
                    {
                        FIO4PIN&=~RXPIN;
                        for(volatile ui32 c=0;c<0x8F;c++)	c++;

                        FIO4PIN|=RXPIN;
                        for(volatile ui32 c=0;c<0x8F;c++)	c++;

                        flSPIEnd=false;
                        S0SPDR=0;
                    }
                }


                //if(flSPI==2)
                if(flSPIEnd)                                //данные от Щ300 получены и обработаны
                {
					if(S300Mode.Var!=S300Mode.VarOld)
					{
						S300Mode.Upd=1;                     //требуется обновление
					}

//					if(S300Data.Var!=S300Data.VarOld)
//					{
						// меньше меньшего
						if(S300Data.Var<Res.Var-TolM.Var)
						{
							Proc=30;
						}

						// в диапазоне
						if((S300Data.Var>Res.Var-TolM.Var)&&(S300Data.Var<Res.Var+TolP.Var))	//
						{
							Proc=30;
						}


						if(S300Data.Var<(Res.Var+TolP.Var)*2)
						{
							Rotator.Val=ROT_N3-N0;				// если меньше двукратного превышения
						}
						else
						{
							Rotator.Val=ROT_N2-N0;				// если больше двукратного превышения
						}
						TempI=I2.Var;						// временно положим сюда ток I2

						WaitBP=20;
						flBPOff=false;
						flRotUpdEn=true;

						if(S300Data.Var<((Res.Var+TolP.Var)+(Res.Var+TolP.Var)/2))	// если меньше чем 1.5*R
						{
							Rotator.Val=ROT_N4-N0;			// уменьшаем количество проходов
							TempI=I2.Var;					// временно положим сюда ток процесса
							flBPOff=true;					// начинаем проводить замеры в стопе
							WaitBP=600;
						}
						S300Data.Upd=1;						//@2409
//					}

					BP1.Req=true;							//запускаем цикл обмена с БПМ
					BP1.ReqNum=3;							//количество запросов без ответа
					Cnt2=0;
                }

				BP1=bpm_get_stat(BP1);


				if(BP1.Upd)							//по RS всё получено, запускаем SPI
                {
                    flSPI=0;
                    flSPIEnd=true;
					BP1.Req=false;
                }

				if(S300ok>0 && BP1ok>0 && flRotOk)
				{
					BP1.Req=true;					//разрешаем запрос
					BP1.ReqNum=3;					//количество попыток
					Cnt2=0;
					Proc=21;
				}

            }
            break;

			case 21:								//сброс ошибок статуса
			{
				if(Cnt2>20)							//задержка (в тиках Tmr0) при переходе
				{
					if(BP1.St.b[0]!=0)				//если в статусе есть ошибки, то команда сброса
					{
						BP1=bpm_command(BP1,5,0);	//5-сброс ошибок статуса
						if(BP1.Error==0)
						{
							BP1.Req=true;			//разрешаем запрос
							BP1.ReqNum=3;			//количество попыток
							Cnt2=0;
							Proc=211;
						}
					}
					else							//если в статусе ошибок нет то перескакиваем на 22,
					{
						BP1.Req=true;
						BP1.ReqNum=3;				//количество попыток
						Cnt2=0;
						Proc=22;					//загрузка заданий
					}
				}
            }
            break;

            case 211:       //проверка сброса ошибок БПМ
            {
				if(Cnt2>20)
				{
					BP1=bpm_get_stat(BP1);
					if(BP1.Error==0)
					{
						if(BP1.St.b[0]==0)
						{
							BP1.Req=true;			// разрешаем запрос
							Cnt2=0;
							Proc=22;				// загрузка заданий
						}
						else
						{
							//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
						}
					}
				}
            }
            break;

			/********************************************
			*			   ОТПЫЛ НА ЗАДВИЖКУ
			*********************************************/

			case 22:							//загрузка уставок U, I, P отпыла на задвижку
            {
				if(Cnt2>20)
				{
					BP1.Iz.Val=I1.Var;
					BP1.Uz.Val=U_PROC;			//
					BP1.Pz.Val=P_PROC;
					BP1=bpm_set_point(BP1);

					if(BP1.Error==0)
					{
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
						Proc=221;				//контроль уставок
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
//#3
				}
            }
            break;

			case 221:							//проверка заданий U, I, P
            {
				if(Cnt2>20)
				{
					BP1=bpm_get_data(BP1,0);
					if(BP1.Error==0)
					{
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
						Proc=23;				//
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*

					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
//#4
				}
            }
            break;

			case 23:							// "Пуск" БПМ для отпыла на задвижку
            {
				if(Cnt2>20)
				{
					BP1=bpm_command(BP1,2,0);	//2- включить источник ("ПУСК"), 3-выключить источник ("СТОП")
					if(BP1.Error==0)
					{
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
						Proc=231;				//проверка заданий
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}

            }
            break;


			// 231,232 цикл отпыла на задвижку
			case 231:								//проверка ДУ и "ПУСК"
            {
				if(Cnt2>20)
				{
					BP1=bpm_get_stat(BP1);

					if(BP1.Error==0)
					{
						if((BP1.St.b[1]&(DU|STRT))==(DU|STRT))
						{
							if(Timer1.On)			//таймер включен
							{
//#5
								Proc=232;			//контроль таймера
							}
							else
							{
								Timer1.On=true;		//таймер выключен, включаем
								SecCnt=SEC1;		//нач. уст.
							}						//
						}
						else
						{
							Timer1.On=false;
							//for(;;) {}
							//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
						}
					}
					else
					{
						Cnt2=0;
					}

				}
            }
            break;

			case 232:								//контроль таймера отпыла на задвижку
			{
				if(Timer1.Val>0)
				{
					Proc=231;						//на проверку проверка ДУ и "ПУСК"
					BP1.Req=true;
					Cnt2=0;
				}
				else
				{
					Proc=233;						//загрузка рабочих уставок
					BP1.Req=true;
					Cnt2=0;
				}

			}
			break;


			/******************************************
			*			  РАБОЧИЙ СЛОЙ
			*******************************************/

			case 233:							//
			{
				if(Cnt2>20)
				{
					BP1.Iz.Val=0;				//блок не отключаем, гасим блок по току
					BP1.Uz.Val=U_PROC;
					BP1.Pz.Val=P_PROC;
					BP1=bpm_set_point(BP1);

					if(BP1.Error==0)
					{
						Proc=234;				//контроль рабочих уставок I
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}
			}
			break;

			case 234:							//контроль рабочих уставок I
			{
				if(Cnt2>20)
				{
					BP1=bpm_get_data(BP1,0);
					if(BP1.Error==0)
					{
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
						RunMode.Var=35;			//кнопка "ПРОДОЛЖИТЬ"
//@						RunMode.Upd=1;
						Proc=235;
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>> ошибка уставок >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}
			}
			break;

			//крутимся по 235, проходим дальше по кнопке "CONT" или останавливаемся по "STOP"
			case 235:
			{
				if(Cnt2>20)
				{
					BP1=bpm_get_data(BP1,1);
					if(BP1.Error==0)
					{
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
//						Proc=235;
						BP1.TmrReq=true;
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>> ошибка уставок >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}
			}
			break;


			//после нажатия кнопки "CONT"
			case 24:							//по кнопке "CONT"
			{									//загрузка рабочих уставок II
				if(Cnt2>20)
				{
					BP1.Iz.Val=TempI;			//ток процесса назначали в Proc=1 в зависимости от величины Res
//					BP1.Uz.Val=U_PROC;
//					BP1.Pz.Val=P_PROC;



//					BP1=bpm_set_point(BP1);

//					if(BP1.Error==0)
//					{
//						flRotValUpd=true;
//						BP1.Req=true;			//разрешаем запрос
//						Cnt2=0;
						Proc=241;				//на контроль уставок II
//					}

//					if(BP1.TmrReq)
//					{
//						BP1.TmrReq=false;
//						BP1.Req=true;			//разрешаем повторные запросы
//						Cnt2=0;
//					}
				}
			}
			break;

			//контроль уставок II
			case 241:
            {
				if(Cnt2>20)
				{
//					BP1=bpm_get_data(BP1,0);
//					if(BP1.Error==0)
//					{
						Timer1.Val=T2.Var;		//
						Timer1.On=true;			// разрешаем таймер
						BP1.Req=true;			// разрешаем запрос
						Cnt2=0;
						Rotator.On=true;

//						Proc=242;				//таймер процесса
//					}
//					else
//					{
						//>>>>>>>>>>>>>>>>>>>>>> ошибка уставок >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
//					}

//					if(BP1.TmrReq)
//					{
//						BP1.TmrReq=false;
//						BP1.Req=true;			//разрешаем повторные запросы
//						Cnt2=0;
//					}

					Proc=274;
				}
            }
            break;

			//контроль таймера процесса
			case 242:
			{
				if(Timer1.Val==0)
				{
					Proc=32;					//выход из тп по таймеру
				}
				else
				{
					Proc=243;					//счетчик оборотов процесса
				}
			}
			break;


			//контроль счетчика оборотов
			case 243:
			{
				if(Rotator.Val==0)				// назначается в зависимости от величины Res
				{
					Rotator.On=false;
					MsgIdNew=4;					//ИЗМЕРЕНИЕ
					FazaCnt=Faza;
					Proc=245;					//компенсация фазы
				}
				else
				{
					Proc=244;					//продолжаем, таймер не сработал, счетчик оборотов не обнулился
				}

				BP1.Req=true;					//разрешаем повторные запросы
				Cnt2=0;
			}
			break;

			case 244:
			{
				if(Cnt2>20)
				{
					BP1=bpm_get_data(BP1,1);

					if(BP1.Error==0)
					{
//						Proc=242;				//контроль прошел
//						BP1.Req=true;			//разрешаем запрос
//						Cnt2=0;
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>> ошибка данных >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
						BP1.TmrReq=false;
					}
//
					BP1.Req=true;				//разрешаем повторные запросы
					Cnt2=0;
					Proc=242;					//зацикливаем
				}
			}
			break;

//#21
			//компенсация фазы
			case 245:
			{
				//if(FazaCnt>Faza+Tf.Var)				//ожидаем
				//{
					Proc=240;					//отключение блока, замер сопротивления
				//}
			}
			break;




			case 240:
			{
				if(Cnt2>20)
				{
					BP1.Iz.Val=0;				//загружаем ток =0
					BP1.Uz.Val=U_PROC;
					BP1.Pz.Val=P_PROC;
					BP1=bpm_set_point(BP1);

					if(BP1.Error==0)
					{
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
						Proc=247;				//контроль уставок
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}
			}
			break;

			//контроль уставок
			case 247:							// ex 272
			{
				if(Cnt2>20)
				{
					BP1=bpm_get_data(BP1,0);
					if(BP1.Error==0)
					{
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;

						flSPI=0;
						flSPIEnd=true;

						Proc=248;				//СТОП блока
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>> ошибка уставок >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;				//разрешаем повторные запросы
						Cnt2=0;
					}
				}

			}
			break;

			//стоп по оборотам, отключение блока для замера сопротивления
			case 248:								//240 -> 247 -> 248 (по количеству оборотов)
			{
				if(flBPOff)							//если требуется стоп
				{
					if(Cnt2>20)
					{
						BP1=bpm_command(BP1,3,0);	//3-выключить источник ("СТОП")

						if(BP1.Error==0)
						{
							BP1.ReqNum=3;
							BP1.Req=true;			//разрешаем повторные запросы
							Cnt2=0;
							Proc=249;				//получение статуса источника
						}

						if(BP1.TmrReq)
						{
							BP1.TmrReq=false;
							BP1.ReqNum=3;
							BP1.Req=true;			//разрешаем повторные запросы
							Cnt2=0;
						}
					}
				}
				else								//
				{
					Proc=250;
				}
			}
			break;

			//получение статуса источника
			case 249:								//248 ->
			{
				if(Cnt2>120)
				{
					BP1=bpm_get_stat(BP1);

					if(BP1.Error==0)
					{
						if((BP1.St.b[1]&(DU|STRT))==(DU))	// бит START не контролируем
						{
//#6
							Cnt2=0;
							flSPI=0;
							flSPIEnd=true;
							Proc=250;						// блок или в "СТОП"е или
															// ток сброшен, замер сопротивления
						}
						else
						{
							//for(;;) {}
							//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
						}
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=100;				//укорачиваем паузу на повторах
					}

				}
			}
			break;


			// пауза перед получением данных от Щ300
			case 250:
			{
				if(Cnt2>150)
				{
					Cnt2=0;
					Proc=251;
				}

			}
			break;

			//плучение данных от Щ300
			case 251:							// 249 -> загрузка тока в БПМ
            {
				if(Cnt2>20)
				{
					if(flSPI==0)								//при 0 запускаем цикл spi на Щ300, flSPI в прерывании
					{
						if(flSPIEnd)
						{
							FIO4PIN&=~RXPIN;
							for(volatile ui32 c=0;c<0x8F;c++)	c++;

							FIO4PIN|=RXPIN;
							for(volatile ui32 c=0;c<0x8F;c++)	c++;

							flSPIEnd=false;
							S0SPDR=0;
						}
					}

					if(flSPIEnd)                                //данные от Щ300 получены и обработаны
					{

						if(S300Data.Var!=S300Data.VarOld)
						{
							S300Data.Upd=1;
						}

						if(S300Mode.Var!=S300Mode.VarOld)
						{
							S300Mode.Upd=1;                     //требуется обновление
						}
//						BP1.Req=true;							//запускаем цикл обмена с БПМ
						Cnt2=0;
						Proc=26;								//проверка сопротивления

					}
				}
            }
            break;

			//проверка сопротивления
			case 26:
            {
				if(Timer1.Val!=0)						//перед включением проверим таймер
				{
					MsgIdNew=3;							//НАНЕСЕНИЕ СЛОЯ
					Cnt2=0;
					BP1.TmrReq=false;
					BP1.ReqNum=3;
					BP1.Req=true;
					Proc=270;							//источник в "ПУСК"
				}
				else
				{
					Proc=32;
				}


				if(S300Data.Var<Res.Var-TolM.Var)			//меньше меньшего
				{
					Proc=30;
				}
				else if (S300Data.Var<Res.Var+TolP.Var)		//в норме
				{
					Proc=30;
				}

            }
            break;

			//включение блока
			case 270:
			{
				if(Cnt2>20)
				{
					BP1=bpm_command(BP1,2,0);	//2- включить источник ("ПУСК"), 3-выключить источник ("СТОП")
					if(BP1.Error==0)
					{
						BP1.ReqNum=3;
						BP1.Req=true;			// разрешаем запрос
						Cnt2=0;
						if(~flBPOff)
						{
//@							Cnt2=580;			// сокращаем ожидание до 20 тиков
							Cnt2=20;	//@
						}
						Proc=271;				// проверка статуса
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}
			}
			break;

			//проверка статуса
			case 271:
			{
//@				if(Cnt2>600)						//надо подождать секунд 10
				if(Cnt2>20)					//@
				{
					BP1=bpm_get_stat(BP1);

					if(BP1.Error==0)
					{
						if((BP1.St.b[1]&(DU|STRT))==(DU|STRT))
						{
							BP1.ReqNum=3;
							BP1.Req=true;			//разрешаем запрос
							Cnt2=0;
							Proc=272;				// замер смещения канала тока
						}
						else
						{

							//for(;;) {}
							//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
						}
						BP1.Upd=true;
					}


					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=580;				//повторные запросы без секундных пауз
					}
				}
			}
			break;


			case 272:
			{
				if(Cnt2>20)
				{
					BP1=bpm_get_data(BP1,1);		// получение текущих U, I, P в структуру BP1

					if(BP1.Error==0)
					{
						I0=BP1.I.Val;				// смещение канала тока
						Proc=274;					//установка счетчика оборотов и тока подгонки
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>> ошибка уставок >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
					}

//					if(BP1.TmrReq)
//					{
//						BP1.TmrReq=false;
//						BP1.Req=true;				//разрешаем повторные запросы
//						Cnt2=0;
//					}
				}

			}
			break;


			case 273:
			{

			}
			break;



			case 274:
			{

				if(S300Data.Var<(Res.Var+TolP.Var)*2)
				{
					Rotator.Val=ROT_N3;				// если меньше двукратного превышения
				}
				else
				{
					Rotator.Val=ROT_N2;				// если больше двукратного превышения
				}
				BP1.Iz.Val=I2.Var;					// устанавливаем ток II

				WaitBP=20;
				flBPOff=false;

				if(S300Data.Var<((Res.Var+TolP.Var)+(Res.Var+TolP.Var)/2))	// если меньше чем 1.5*R
				{
					Rotator.Val=ROT_N4;			// уменьшаем количество проходов
					BP1.Iz.Val=I2.Var;			// устанавливаем ток подгонки = ток процесса
					flBPOff=true;				// начинаем проводить замеры в стопе
					WaitBP=600;					// пауза для источника (заряд фильра питания)
				}

				Cnt2=0;							//
				RotVal=Rotator.Val;
				flRotUpdEn=false;
				Proc=275;						// ожидание загрузки уставок
			}
			break;

			//ожидаем момента загрузки уставок
			case 275:
			{
				if(Cnt2>WaitBP)					// пауза для источника (заряд фильра питания)
				{
					Rotator.On=true;			// разрешаем счетчик
					if(Rotator.Val<RotVal-1)	// как минимум один оборот прошел
					{
						Rotator.Val=RotVal-N0;
						flRotValUpd=true;
						flRotUpdEn=true;
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем запрос
						Cnt2=0;
						Proc=276;				// загрузка уставок
					}
				}
			}
			break;

			case 276:						// загрузка уставок
			{
				BP1.Uz.Val=U_PROC;
				BP1.Pz.Val=P_PROC;
				BP1=bpm_set_point(BP1);

				if(BP1.Error==0)
				{
//					FazaCnt=0;				// сброс счетчика фазы
					Proc=277;				// контроль уставок ???
				}

				if(BP1.TmrReq)
				{
					BP1.TmrReq=false;
					BP1.Req=true;			// разрешаем повторные запросы
					Cnt2=0;
				}

			}
			break;

			// ожидаем появление тока
			case 277:
			{
				BP1.ReqNum=3;
				BP1.Req=true;			// разрешаем запрос
				Cnt2=0;
				Proc=278;
			}
			break;

			// получение текучки, определение фазы
			case 278:
			{
				if(Cnt2>20)
				{
					BP1=bpm_get_data(BP1,1);		// получение текущих U, I, P в структуру BP1

					if(BP1.Error==0)
					{
//						if(BP1.I.Val > I0+40 )		// текучка больше, чем смещение + 100
						if(BP1.I.Val > 0 )
						{
//							FazaOld=Faza;			// сохраняем счетчик фазы
//							Faza=FazaCnt;			// сохраняем измереннную фазу
//							faza();
							Proc=242;				// контроль таймера
						}
						else						// не равна
						{

							Proc=277;				// ожидаем
						}
					}
					else
					{
						//>>>>>>>>>>>>>>>>>>>>>> ошибка уставок >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}
			}
			break;

			case 279:
			{

			}
			break;




			// выход
			// сопротивление в норме или сопротивление меньше
			case 30:
			{
				MsgIdNew=57;								//ОСТАНОВ + ПО СОПРОТИВЛЕНИЮ
				Timer1.On=false;
//				Timer1.Val=0;								//принудительный сброс таймера
				Proc=33;
			}
			break;

			case 31:
			{
				MsgIdNew=58;									//ОСТАНОВ + ПО ВРАЩЕНИЮ
				Timer1.On=false;
				Proc=33;
			}
			break;

			// выход
			// таймер
			case 32:
			{
				MsgIdNew=56;									//ОСТАНОВ + ПО ТАЙМЕРУ
				Rotator.Val=0;
				flRotValUpd=true;
				Proc=33;
			}
			break;

			case 33:
			{
				BP1.ReqNum=3;
				BP1.Req=true;			// разрешаем запрос
				Cnt2=0;
				Proc=34;
			}
			break;


			case 34:
			{
				if(Cnt2>20)
				{
					BP1=bpm_command(BP1,3,0);	//3-выключить источник ("СТОП")

					if(BP1.Error==0)
					{
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
						Proc=35;				//получение статуса источника
					}

					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=0;
					}
				}
			}
			break;

			case 35:
			{
				if(Cnt2>100)						//надо подождать
				{
					BP1=bpm_get_stat(BP1);

					if(BP1.Error==0)
					{
						BP1.Upd=true;
						Proc=40;
					}


					if(BP1.TmrReq)
					{
						BP1.TmrReq=false;
						BP1.ReqNum=3;
						BP1.Req=true;			//разрешаем повторные запросы
						Cnt2=80;				//повторные запросы без секундных пауз
					}
				}
			}
			break;


			case 40:
			{
				RunMode.Var=300;
//@				RunMode.Upd=1;
				Proc=50;
			}
			break;

			case 50:
			{

			}
			break;

            default:
            break;
		} //switch (Proc)


//=====================================================================================================
//											Конец нанесения слёв
//=====================================================================================================


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//								Таймеры, счетчики, индикаторы, сообщения
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


		// таймер процесса
		if(Timer1.On)				//если Timer1 включен, то отсчитываем
		{
			if(flSec)				//1 sec импульс
			{
				flSec=false;

				if(Timer1.Val>0)
				{
					Timer1.Val--;

					DrawString(DTIME,&fnt60,Timer1.buf,BackColour);
					tprint(Timer1.buf,Timer1.Val);
					DrawString(DTIME,&fnt60,Timer1.buf,GREEN);
				}
			}
		}


		//
		//индикаторы
		//

		//индикатор Щ300
		if(S300Mode.Upd==1)
		{
			if(S300Mode.Var==S300MODE)
			{
				DrawString(SIZM,&fnt30,IZM,GREEN);
				S300ok=10;
			}
			else
			{
				DrawString(SIZM,&fnt30,IZM,BLACK);
				if(S300ok>0)
				{
					S300ok--;
				}
			}
			S300Mode.VarOld=S300Mode.Var;
			S300Mode.Upd=0;
		}

		//данные Щ300
		if(S300Data.Upd==1)
		{
			xsprintf(S300Data.buf,"%04d", S300Data.VarOld);
			DrawString(S300DATA,&fnt60,S300Data.buf,BackColour);

			xsprintf(S300Data.buf,"%04d", S300Data.Var);
			if (S300Data.Var<Res.Var-TolM.Var)
			{
				DrawString(S300DATA,&fnt60,S300Data.buf,RED);
			}
			else
			{
				DrawString(S300DATA,&fnt60,S300Data.buf,GREEN);
			}

			S300Data.VarOld=S300Data.Var;
			S300Data.Upd=0;
		}



		//индикатор блока питания
		if(BP1.Upd)
		{
			BP1.Upd=false;

			if(BP1.Error==0)
			{

				spb();

				if((BP1.St.b[1]&DU)==0)								//|| (BP1.St.b[0]==0))
				{
					DrawString(SBPM,&fnt30,BPM,BLACK);
					if(BP1ok>0)
					{
						BP1ok--;
					}
				}
				else
				{
					if((BP1.St.b[1]&STRT)==0)
					{
						DrawString(SBPM,&fnt30,BPM,GREEN);	//в режиме ДУ
						BP1ok=10;
					}
					else
					{
						DrawString(SBPM,&fnt30,BPM,YELLOW);	//в режиме ДУ и пуске
						BP1ok=10;
					}
				}
			}
			else
			{
				if(BP1.Error!=255)
				{
					DrawString(SBPM,&fnt30,BPM,BLACK);
					if(BP1ok>0)
					{
						BP1ok--;
					}
				}
			}

			S300Mode.Upd=1;				//Исправлена работа индикатора Щ300 при входе в Proc=21.
		}


		//период вращения (в условных тиках)
		//блок должен находиться ПЕРЕД счетчиком оборотов и перед коррекцией момента выключения!
		if(flRot1 && flRot0)
		{
			if(RotPer!=RotPerCnt)
			{
				RotPer=RotPerCnt;
				perprint();
			}
			RotPerCnt=0;
			if(!Rotator.On)				//если подсчет оборотов выключен, то flRot1 и flRot0 сбрасываем здесь, иниче сбрасываем в счетчике оборотов
			{
				flRot1=false;
				flRot0=false;
			}
		}


		//на последнем обороте производим коррекцию момента выключения тока
		//блок должен находиться ПЕРЕД счетчиком оборотов!
		if(Rotator.On && Rotator.Val==1)
		{
			if(RotPerCnt>RotPer-150+Tf.Var)
			{
				Rotator.Val--;
				if(flRotUpdEn)
				{
					flRotValUpd=true;
				}
			}
			flRot1=false;
			flRot0=false;
		}

		//счетчик оборотов
		if(Rotator.On && flRot1 && flRot0)
		{
			if(Rotator.Val>0)
			{
				Rotator.Val--;
				if(flRotUpdEn)
				{
					flRotValUpd=true;
				}
			}
			flRot1=false;
			flRot0=false;
		}


		//вывод счетчика оборотов
		if(flRotValUpd)
		{
			rotprint();
			flRotValUpd=false;
		}


		//индикатор вращения
		if(((1<<6)&(ui8)FIO3PIN3)==0)	//P3.30 порт датчика вращения
		{
			Cnt1=0;						//магнит не под ДХ, считаем в Timer0IntrHandler()
			flRot0=true;
		}
		else
		{
			Cnt11=0;					//магнит под ДХ, считаем в Timer0IntrHandler()
			if(flRot0)	flRot1=true;
		}



		flRotOkOld=flRotOk;
		if(Cnt1<ROTPER && Cnt11<ROTPER)	//если Cnt1 и Cnt11 < ROTPER, то считаем что вращение в норме
		{
			flRotOk=true;
		}
		else
		{
			flRotOk=false;
		}

		if(flRotOkOld!=flRotOk)
		{
			flRotUpd=true;						//обновляем индикатор на экране
		}


		if(flRotUpd&&(RunMode.Var==3|RunMode.Var==35|RunMode.Var==358|RunMode.Var==352|RunMode.Var==30|RunMode.Var==300|RunMode.Var==3002))
		{
			if(flRotOk)
			{
				DrawString(SROT,&fnt30,ROT,GREEN);
			}
			else
			{
				DrawString(SROT,&fnt30,ROT,BLACK);
				flRot1=flRot0=false;
				Proc=31;
			}
			flRotUpd=0;
		}

//#9
		if(Proc!=ProcOld)
		{
			proc();
		}


		if(RunMode.Var!=RunMode.VarOld)
		{
			xsprintf(buffer,"%d", RunMode.VarOld);			//##
			DrawString(55,5,&fnt20,buffer,BackColour);		//##
			xsprintf(buffer,"%d", RunMode.Var);				//##
			DrawString(55,5,&fnt20,buffer,GREEN);			//##

			RunMode.VarOld=RunMode.Var;

			if(BeepCnt>BEEPS)
			{
				BeepCnt=0;
				BEEP_0n;
			}

			fScrnOk=0;
		}


		if(flSPI!=flSPIOld)
		{
			spi();
		}


		if(BeepCnt>BEEPS)
		{
			BEEP_0ff;
		}

		if(RunMode.Var==12 || RunMode.Var==14)
		{
			if(ModeCnt>MODECNT)
			{
				RunMode.Var=1;
			}
		}

		if(RunMode.Var==22 || RunMode.Var==24 || RunMode.Var==28 || RunMode.Var==241)
		{
			if(ModeCnt>MODECNT)
			{
				FIO1PIN&=~CLED;								//гасим
				RunMode.Var=2;
			}
		}

		if(RunMode.Var==352 || RunMode.Var==358)
		{
			if(ModeCnt>MODECNT)
			{
				RunMode.Var=35;
			}
		}

		if(RunMode.Var==302 || RunMode.Var==304)
		{
			if(ModeCnt>MODECNT)
			{
				RunMode.Var=30;
			}
		}

		if(RunMode.Var==3002)
		{
			if(ModeCnt>MODECNT)
			{
				RunMode.Var=300;
			}
		}

		if(MsgId!=MsgIdNew)
		{
			switch (MsgId)
			{
				case 0:	DrawString(SMSG0,&fnt30,MSG0,BackColour);	//УСТАНОВКА ПАРАМЕТРОВ
				break;

				case 1:	DrawString(SMSG1,&fnt30,MSG1,BackColour);	//ОЧИСТКА
				break;

				case 2:	DrawString(SMSG2,&fnt30,MSG2,BackColour);	//ОЖИДАНИЕ
				break;

				case 3: DrawString(SMSG3,&fnt30,MSG3,BackColour);	//НАНЕСЕНИЕ СЛОЯ
				break;

				case 4: DrawString(SMSG4,&fnt30,MSG4,BackColour);	//ИЗМЕРЕНИЕ
				break;

				case 56:
				{
					DrawString(SMSG5,&fnt30,MSG5,BackColour);		//ОСТАНОВ
					DrawString(SMSG6,&fnt30,MSG6,BackColour);		//ПО ТАЙМЕРУ
				}
				break;

				case 57:
				{
					DrawString(SMSG5,&fnt30,MSG5,BackColour);		//ОСТАНОВ
					DrawString(SMSG7,&fnt30,MSG7,BackColour);		//ПО СОПРОТИВЛЕНИЮ
				}
				break;

				case 58:
				{
					DrawString(SMSG5,&fnt30,MSG5,BackColour);			//ОСТАНОВ
					DrawString(SMSG8,&fnt30,MSG8,BackColour);			//ПО ВРАЩЕНИЮ
				}

				default:
				break;
			}

			switch (MsgIdNew)
			{
				case 0:	DrawString(SMSG0,&fnt30,MSG0,MSGCOLOR);		//УСТАНОВКА ПАРАМЕТРОВ
				break;

				case 1:	DrawString(SMSG1,&fnt30,MSG1,MSGCOLOR);		//ОЧИСТКА
				break;

				case 2:	DrawString(SMSG2,&fnt30,MSG2,MSGCOLOR);		//ОЖИДАНИЕ
				break;

				case 3: DrawString(SMSG3,&fnt30,MSG3,MSGCOLOR);		//НАНЕСЕНИЕ СЛОЯ
				break;

				case 4:	DrawString(SMSG4,&fnt30,MSG4,MSGCOLOR);		//ИЗМЕРЕНИЕ
				break;

				case 56:
				{
					DrawString(SMSG5,&fnt30,MSG5,MSGCOLOR);			//ОСТАНОВ
					DrawString(SMSG6,&fnt30,MSG6,MSGCOLOR);			//ПО ТАЙМЕРУ
				}
				break;

				case 57:
				{
					DrawString(SMSG5,&fnt30,MSG5,MSGCOLOR);			//ОСТАНОВ
					DrawString(SMSG7,&fnt30,MSG7,MSGCOLOR);			//ПО СОПРОТИВЛЕНИЮ
				}
				break;

				case 58:
				{
					DrawString(SMSG5,&fnt30,MSG5,MSGCOLOR);			//ОСТАНОВ
					DrawString(SMSG8,&fnt30,MSG8,MSGCOLOR);			//ПО ВРАЩЕНИЮ
				}

				default:
				break;
			}

			MsgId=MsgIdNew;
		}



	} //while(1)
}


void tprint(char* buff,	ui16 par)
{
	ui16 m;
	ui16 s;
	m=par/60;
	s=par-m*60;
	xsprintf(buff,"%02u", m);
	buff[2]=':';
	xsprintf(buff+3,"%02u", s);
	buff[5]=0;
}

void fprint(char* buff,	float f)			//для двухзначных чисел с одним знаком после запятой!
{

    float Temp, Intg;
	uint n0, n1, n2, n3, n4;

    n0=0;
    n1=0;
    n2=0;
    f=f*10;

    Temp=f;

    for(int k=0; k<3; k++)
    {
        if(k==0)
        {
            Temp=f/100.0f;
            modff(Temp,&Intg);
            n2=(uint)Intg;
        }
        else
        {
            if(k==1)
            {
                Temp=(f-100*n2)/10.0f;
                modff(Temp,&Intg);
                n1=(uint)Intg;
            }
            else
            {
                Temp=f-100*n2-10*n1;
                modff(Temp,&Intg);
                n0=(uint)Intg;
            }
        }
    }
//  return 100*n2+10*n1+n0;
    xsprintf(buff,"%2u", 10*n2+1*n1);
    buff[2]='.';
    xsprintf(buff+3,"%01u", n0);
    buff[4]=0;
}


void rotprint()
{
	RegionFill(468,5,30,20,BackColour);
	xsprintf(buffer,"%02d", Rotator.Val);
	DrawString(470,5,&fnt20,buffer,GREEN);
}


void spi()
{
	RegionFill(402,5,30,20,BackColour);
	xsprintf(buffer,"%02x", flSPI);
	DrawString(406,5,&fnt20,buffer,GREEN);
	flSPIOld=flSPI;
}

void spb ()
{
	RegionFill(292,5,70,20,BackColour);
	xsprintf(buffer,"%02x", BP1.St.b[1]);
	DrawString(293,5,&fnt20,buffer,GREEN);
	xsprintf(buffer,"%02x", BP1.St.b[0]);
	DrawString(320,5,&fnt20,buffer,GREEN);
}

void proc()
{
	xsprintf(buffer,"%d", ProcOld);					//
	DrawString(187,5,&fnt20,buffer,BackColour);		//
	xsprintf(buffer,"%d", Proc);					//
	DrawString(187,5,&fnt20,buffer,GREEN);			//
	ProcOld=Proc;
}

//void fazaprint()
//{
//	RegionFill(525,5,70,20,BackColour);
//	xsprintf(buffer,"%ld", Faza);
//	DrawString(530,5,&fnt20,buffer,GREEN);
//}

void perprint()
{
	RegionFill(525,5,70,20,BackColour);
	xsprintf(buffer,"%ld", RotPer);
	DrawString(530,5,&fnt20,buffer,GREEN);
}



void Timer0IntrHandler ()
{

//#10


	if(Cnt1<ROTPER)
	{
		Cnt1++;
	}

	if(Cnt11<ROTPER)
	{
		Cnt11++;
	}

	Cnt2++;						// счетчик задержки для работы интерфейса БП
	BeepCnt++;					// счетчик длительности "beep"ера
	KeyCnt++;
	ModeCnt++;					// счетчик паузы автоматического возврата из меню
//	FazaCnt++;
	RotPerCnt++;

	if(Timer1.On)
	{
		if(SecCnt>0)	SecCnt--;
		else
		{
			SecCnt=SEC1;
			flSec=true;
		}
	}


	if(BP1.RspTimeOut>0)
	{
		BP1.RspTimeOut--;
	}

	T0IR=0xff;
	VICAddress=0;
}


//ui8 VertSync=0;
void LCDVertSync()
{
	LCD_INTCLR=(1<<LCD_INTCLR_VCompIC);		//сброс прерывания
	VICAddress=0;
}

//ui32 UART2Status;

void UART2IntrHandler()
{
	RxByteNum++;

    ui8 IIRValue, LSRValue;
	volatile ui32 c;

    IIRValue = U2IIR;

    IIRValue >>= 1;             /* skip pending bit in IIR */
    IIRValue &= 0x07;           /* check bit 1~3, interrupt identification */

    if ( IIRValue == 0x03 )      /* Receive Line Status */
    {
		LSRValue = U2LSR;
        if ( (LSRValue & (1<<U2LSR_OE|1<<U2LSR_PE|1<<U2LSR_FE|1<<U2LSR_RXFE|1<<U2LSR_BI))==0)
        {
			VICAddress = 0;        /* Acknowledge Interrupt */
            return;
        }

        if ( (LSRValue & (1<<U2LSR_RDR))==1<<U2LSR_RDR )   /* Receive Data Ready */
        {
            RxBuf[RxPtr]=U2RBR;
            RxPtr++;
		}
	}
    else if ( IIRValue == 0x02 ) /* Receive Data Available */
    {
        RxBuf[RxPtr]=U2RBR;
        RxPtr++;
    }
    else if ( IIRValue == 0x06 ) /* Character timeout indicator */
    {
         RxBuf[RxPtr]=U2RBR;
         RxPtr++;
	}
    else if ( IIRValue == 0x01 )    /* THRE, transmit holding register empty */
    {
		for(c=0;c<0x2FFF;c++)
		{
			c++;
		}

        FIO0PIN&=~TXPIN;
        FIO4PIN|=RXPIN;
        FIO4PIN&=~led2;
        U2FCR|=1<<U2FCR_RXFIFOReset;
        RxPtr = 0;

    }
    VICAddress = 0x00;        /* Acknowledge Interrupt */
}



//	SPIIntrHandler
void SPIIntrHandler()
{
	if(S0SPSR>>S0SPSR_SPIF==1)      //передача завершена
    {
        ui16_Un t;
        t.Val =S0SPDR;

        if(flSPI==0)                //прием первых 2 байт
        {
			TempBuf[0]=~t.b[0];
			TempBuf[1]=~t.b[1];
			flSPI=1;
			S0SPDR=0;               //для получения ещё 2 байт
        }
        else                        //прием вторых 2 байт
        {
            S300Mode.Var=(ui8)(~t.b[0]);        //здесь идентификатор пределов Щ300
            TempBuf[2]=~t.b[1];					//здесь значение 0 или 1

			//преодразование двоично-десятичного числа в двоичное
			//и формирования значения
			TempBuf[0]=(ui8)(((TempBuf[0]&0xF0)>>4)*0x0A) + (ui8)(TempBuf[0]&0x0F);
			TempBuf[1]=(ui8)(((TempBuf[1]&0xF0)>>4)*0x0A) + (ui8)(TempBuf[1]&0x0F);
			S300Data.Var=TempBuf[2]*10000 + TempBuf[1]*100 + TempBuf[0];
			flSPIEnd=true;
		}
    }

    S0SPINT=1;
	VICAddress = 0x00;        // Acknowledge Interrupt
}

/*
void debug()
{
	RegionFill(8,360,480,30,BackColour);
	xsprintf(buffer,"%02x", RxBuf[6]);
	DrawString(10,370,&fnt20,buffer,WHITE);

	xsprintf(buffer,"%02x", BP1.Iz.b[0]);
	DrawString(40,370,&fnt20,buffer,RED);

	xsprintf(buffer,"%02x", RxBuf[7]);
	DrawString(70,370,&fnt20,buffer,WHITE);

	xsprintf(buffer,"%02x", BP1.Iz.b[1]);
	DrawString(100,370,&fnt20,buffer,RED);

	xsprintf(buffer,"%02x", RxBuf[8]);
	DrawString(130,370,&fnt20,buffer,WHITE);

	xsprintf(buffer,"%02x", BP1.Uz.b[0]);
	DrawString(160,370,&fnt20,buffer,RED);

	xsprintf(buffer,"%02x", RxBuf[9]);
	DrawString(190,370,&fnt20,buffer,WHITE);

	xsprintf(buffer,"%02x", BP1.Uz.b[1]);
	DrawString(220,370,&fnt20,buffer,RED);

	xsprintf(buffer,"%02x", RxBuf[10]);
	DrawString(250,370,&fnt20,buffer,WHITE);

	xsprintf(buffer,"%02x",BP1.Pz.b[0]);
	DrawString(280,370,&fnt20,buffer,RED);

	xsprintf(buffer,"%02x", RxBuf[11]);
	DrawString(310,370,&fnt20,buffer,WHITE);

	xsprintf(buffer,"%02x", BP1.Pz.b[1]);
	DrawString(340,370,&fnt20,buffer,RED);
}
*/

