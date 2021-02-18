#ifndef LCD_H
#define LCD_H
#include "config.h"

//const uint32_t _lcd_buffer = LCD_BUFFER0_BASE_ADDR;

#define BLACK           (0x0000)		/*   0,   0,   0 */
#define NAVY            (0x000F)		/*   0,   0, 128 */
#define DGREEN          (0x03E0)		/*   0, 128,   0 */
#define DCYAN           (0x03EF)		/*   0, 128, 128 */
#define MAROON          (0x7800)		/* 128,   0,   0 */
#define PURPLE          (0x780F)		/* 128,   0, 128 */
#define OLIVE           (0x7BE0)		/* 128, 128,   0 */
#define LGRAY           (0xC618)		/* 192, 192, 192 */
#define DGRAY           (0x7BEF)		/* 128, 128, 128 */
#define BLUE            (0x001F)		/*   0,   0, 255 */
#define GREEN           (0x07E0)		/*   0, 255,   0 */
#define CYAN            (0x07FF)		/*   0, 255, 255 */
#define RED             (0xF800)		/* 255,   0,   0 */
#define MAGENTA         (0xF81F)		/* 255,   0, 255 */
#define YELLOW          (0xFFE0)		/* 255, 255,   0 */
#define WHITE           (0xFFFF)		/* 255, 255, 255 */
#define ORANGE          (0xFD20)		/* 255, 165,   0 */
#define GREENYELLOW     (0xAFE5)		/* 173, 255,  47 */
#define BROWN           (0XBC40)		//
#define BRRED           (0XFC07)		//
#define DDGRAY          (0x39E7)


ui32 _lcd_buffer_0; //= LCD_BUFFER0_BASE_ADDR;
ui32 _lcd_buffer_1;



typedef struct
{
  const uint8_t *data;
  uint16_t version;
  uint16_t type;
  uint16_t ascent;
  uint16_t width;
  uint16_t height;
  uint8_t first;
  uint8_t last;
  uint32_t offset;
} Font;

//typedef struct
//{
//	int x0;
//	int y0;
//	int w;
//	int h;
//	int b_w;
//	Font *fnt;
//	char on_txt[16];
//	char off_txt[16];
//	int brd_color;
//	int on_fill_color;
//	int off_fill_color;
//	int on_txt_color;
//	int off_txt_color;
//	int st;
//	int on_txt_w;		    //?
//	int off_txt_w;		    //?
//}Button;

void LCD_Initialize();
void DrawPoint(int x, int y, COLOUR_TYPE colour);
void DrawLine(int x0, int y0, int xn, int yn, COLOUR_TYPE colour);
void RegionFill(int x0, int y0 , int width, int height, COLOUR_TYPE colour);
void RegionPop(int x0, int y0, int Width, int height, ui16* p);
void RegionPush(int x0, int y0, int Width, int height, ui16* p);
ui16  ReadPoint(int x, int y);
ui16 GetCharWidth(const Font *font, char c);
ui16 GetStringWidth(const Font *font, const char *str);
bool LoadFont(Font *font, const uint8_t *data);
ui16 DrawChar(int x, int y, const Font *font, char c, COLOUR_TYPE colour);
ui16 DrawString(int x, int y, const Font *font, const char *str, COLOUR_TYPE colour);

#endif // LCD_H

