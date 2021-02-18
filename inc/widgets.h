#include "LCD.h"
#include "config.h"
#include <math.h>


typedef struct
{
	int st;					//статус кнопки
	int x0;					//x
	int y0;					//y
	int w;					//ширина
	int h;					//высота
	int b_w;				//ширина рамки
	Font* fnt;				//шрифт
	char on_txt[16];		//текст на включенной кнопке
	char off_txt[16];		//текст на выключенной кнопке
	int brd_color;			//цвет рамки
	int on_fill_color;		//цвет заливки вкл. кнопки
	int off_fill_color;		//выкл. кнопки
	int on_txt_color;		//цвет текста на вкл. кнопке
	int off_txt_color;		//выкл. кнопке

	int on_txt_w;		    //ширина текста на включ. кнопке (ВЫЧИСЛЯЕМ !!!)
	int off_txt_w;		    //ширина текста на выкл. кнопке (ВЫЧИСЛЯЕМ !!!)
	ui16* img_bk;
}Button;


typedef struct Sprite
{
	ui16 id;					//ID
	ui16 st;					//status
	ui16 x0;					//x0
	ui16 y0;					//y0
	ui16* img;					//указатель на массив COLOUR_TYPE
	ui16* img_bk;				//массив COLOUR_TYPE
}Sprite;

void rect(int x0, int y0, int w, int h, COLOUR_TYPE c);
void button(Button p);
void sprite(Sprite p);
Sprite sprite_init(ui16 _id, ui16 _st, ui16 _x, ui16 _y, const ui16 _img_rom[], ui16 _img[], ui16 _img_bk[]);



/*


 */
