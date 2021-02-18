#include "config.h"
#include <stdio.h>
#include <LPC2478.h>
#include "var.h"

extern ui8 key();

Param_Struc encoder(Param_Struc v)
{
    ui16 Temp;

    Temp=v.Var;

	Enc=(ui8)FIO2PIN3>>6;               //P2.30, P2.31


	if(Enc!=3&&fEnc==0)
	{
		if(Enc==0)  fEnc=1;
		if(Enc==2)  fEnc=2;
	}

	if(fEnc==1) Enc1++;

	if(fEnc==2) Enc2++;

	if(Enc==3&&(fEnc==1||fEnc==2))      //нормальное состояние Enc==3;
	{
		if(Enc2>0)
		{
			if(Temp>=v.Min+v.Delta)
			{
				Temp-=v.Delta;
				v.Upd=1;
			}
			else
			{
				v.Upd=0;
			}
		}

		if(Enc1>0)
		{
			if(Temp<=v.Max-v.Delta)
			{
				Temp+=v.Delta;
				v.Upd=1;
			}
			else
			{
				v.Upd=0;
			}
		}

		Enc1=0;
		Enc2=0;
		fEnc=0;

		if(v.VarOld!=v.Var)
		{
			v.VarOld=v.Var;
			ModeCnt=0;                  //продлеваем таймаут возврата
		}
		v.Var=Temp;
	}
    return v;
}
