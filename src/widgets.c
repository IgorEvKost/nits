#include "widgets.h"
#include <malloc.h>
#include <string.h>


//void new_button(Button p)
//{
	//p.shadow=(ui32*)malloc(p.w*p.h*2);
	//p.shadow=(ui32*)malloc(100);
//}


void rect(int x0, int y0, int w, int h, COLOUR_TYPE c)
{
	int _x0=x0;
	int _y0=y0;
	int _x1=x0+w;                                   //координаты второго угла прямоугольника
	int _y1=y0+h;
	for(int i=0;i<3;i++)
	{
		DrawLine(_x0,_y0,_x0,_y1,c);
		DrawLine(_x0,_y1,_x1,_y1,c);
		DrawLine(_x0,_y0,_x1,_y0,c);
		DrawLine(_x1,_y0,_x1,_y1,c);
		_x0++; _x1--;
		_y0++; _y1--;
	}
}


void button(Button p)
{
	int _x0=p.x0;
	int _y0=p.y0;
	int _x1=p.x0+p.w;                                   //координаты второго угла прямоугольника
	int _y1=p.y0+p.h;
	int _x2, _y2;                                       //координаты строки
	p.on_txt_w=GetStringWidth(p.fnt,p.on_txt);          //ширина строки
	p.off_txt_w=GetStringWidth(p.fnt,p.off_txt);

	if(p.st==0)											//st==0: invisible
	{													//восстанавливаем фон
		RegionPop(_x0,_y0,p.w,p.h,p.img_bk);
	}


//	if(p.st==1 || p.st==2)
	if(p.st==255)
	{
		RegionPush(_x0,_y0,p.w,p.h,p.img_bk);
	}

	if(p.st==1)													//st==1 visible & on
	{
      for(int i=0;i<p.b_w;i++)
        {
          DrawLine(_x0,_y0,_x0,_y1,p.brd_color);
          DrawLine(_x0,_y1,_x1,_y1,p.brd_color);
          DrawLine(_x0,_y0,_x1,_y0,p.brd_color);
          DrawLine(_x1,_y0,_x1,_y1,p.brd_color);
          _x0++; _x1--;
          _y0++; _y1--;
        }
      RegionFill(_x0,_y0,_x1-_x0+1,_y1-_y0+1,p.on_fill_color);
      _y2=_y0+(_y1-_y0+1)/2- p.fnt->height/2+3;                   //выравнивание по высоте
      _x2=_x0+(_x1-_x0+1)/2-p.on_txt_w/2;                         //горизонтали
	  DrawString(_x2, _y2, p.fnt, p.on_txt, p.on_txt_color);
	}

	if(p.st==2)													//st==2: visible & off
	{
		for(int i=0;i<p.b_w;i++)
		{
			DrawLine(_x0,_y0,_x0,_y1,p.brd_color);
			DrawLine(_x0,_y1,_x1,_y1,p.brd_color);
			DrawLine(_x0,_y0,_x1,_y0,p.brd_color);
			DrawLine(_x1,_y0,_x1,_y1,p.brd_color);
			_x0++; _x1--;
			_y0++; _y1--;
		}
		RegionFill(_x0,_y0,_x1-_x0+1,_y1-_y0+1,p.off_fill_color);
		_y2=_y0+(_y1-_y0+1)/2- p.fnt->height/2+3;                   //выравнивание по высоте
		_x2=_x0+(_x1-_x0+1)/2-p.off_txt_w/2;                         //горизонтали
		DrawString(_x2, _y2, p.fnt, p.off_txt, p.off_txt_color);
	}
}


void sprite(Sprite s)						//работа со спрайтом из массива пикселей
{
	ui16 x,y,xs,ys,ls,X,Y,XX;
	ui16 temp;
	ui32 _lcd_adr;

	x=s.x0;
	y=s.y0;

	if(s.st==0)											//st==0: invisible
	{													//восстанавливаем фон
		for(int i=1; i<=s.img_bk[0];)						//размер массива
		{
			xs=s.img_bk[i];
			i++;
			ys=s.img_bk[i];
			i++;
			ls=s.img_bk[i];								//длина текущей строки
			i++;										//начало строки


			Y=y+ys;

			for(int l=0;l<ls;l++)
			{
				ui16 temp=x+xs+l;

				if(temp>LCD_WIDTH) temp=temp-LCD_WIDTH;

				X=LCD_WIDTH-temp;

				_lcd_adr=_lcd_buffer_0 + (X + Y * LCD_WIDTH) * COLOUR_SIZE;

				if(*(COLOUR_TYPE *)(_lcd_adr+614400)==s.id)
				{

					*(COLOUR_TYPE *)_lcd_adr = *(s.img_bk+i);	//выводим фон
					*(COLOUR_TYPE *)(_lcd_adr + 614400) = 0;	//удаляем ID
				}


//				*(COLOUR_TYPE *)(_lcd_buffer_0 + (X + Y * LCD_WIDTH) * COLOUR_SIZE) = *(s.img_bk+i); //выводим фон

//				*(COLOUR_TYPE *)(_lcd_buffer_1 + (X + Y * LCD_WIDTH) * COLOUR_SIZE) = 0;			 //удаляем ID
				i++;
			}
		}
	}


	if(s.st==1)
	{
		for(int i=1; i<=s.img[0];)						//размер массива
		{
			xs=s.img[i];
			i++;
			ys=s.img[i];
			i++;
			ls=s.img[i];								//длина текущей строки
			i++;										//начало строки

			Y=y+ys;

			for(int l=0;l<ls;l++)
			{
				temp=x+xs+l;
				if(temp>LCD_WIDTH) temp=temp-LCD_WIDTH;
				X=LCD_WIDTH-temp;

				_lcd_adr=_lcd_buffer_0 + (X + Y * LCD_WIDTH) * COLOUR_SIZE;

				if(*(COLOUR_TYPE *)(_lcd_adr+614400)==0)
				{
					*(s.img_bk+i)= *(COLOUR_TYPE *)_lcd_adr;	//сохраняем фон
					*(COLOUR_TYPE *)_lcd_adr =*(s.img+i);		//выводим img
					*(COLOUR_TYPE *)(_lcd_adr+614400) =s.id;	//ставим ID
				}



//				*(s.img_bk+i)= *(COLOUR_TYPE *)(_lcd_buffer_0 + (X + Y * LCD_WIDTH) * COLOUR_SIZE);	//сохраняем фон


//				*(COLOUR_TYPE *)(_lcd_buffer_0 + (X + Y * LCD_WIDTH) * COLOUR_SIZE) =*(s.img+i);		//выводим img

				i++;
			}
		}

		if(s.st==2)
		{
			for(int i=1; i<=s.img_bk[0];)						//размер массива
			{
				xs=s.img_bk[i];
				i++;
				ys=s.img_bk[i];
				i++;
				ls=s.img_bk[i];								//длина текущей строки
				i++;										//начало строки


				Y=y+ys;

				for(int l=0;l<ls;l++)
				{

					X=LCD_WIDTH-(x+xs+l);

					_lcd_adr=_lcd_buffer_0 + (X + Y * LCD_WIDTH) * COLOUR_SIZE;

//					if()
					*(s.img_bk+i)= *(COLOUR_TYPE *)_lcd_adr; //сохраняем точку фона


//					*(s.img_bk+i)= *(COLOUR_TYPE *)(_lcd_buffer_0 + (X + Y * LCD_WIDTH) * COLOUR_SIZE); //сохраняем точку фона
					i++;
				}
			}
		}
	}
}





Sprite  sprite_init(ui16 _id, ui16 _st, ui16 _x, ui16 _y, const ui16 _img_rom[], ui16 _img[], ui16 _img_bk[])
{
	Sprite s;

	s.id=_id;
	s.st=_st;
	s.x0=_x;
	s.y0=_y;

	for(int i=0;i<=_img_rom[0];i++)
	{
		_img[i]=_img_rom[i];			//копируем из ROM в RAM
		_img_bk[i]=_img_rom[i];			//сюда чтобы сохранились зоголовки строк
	}
	s.img=_img;						//ссылка на RAM
	s.img_bk=_img_bk;				//ссылка на задний фон
	return s;
}
