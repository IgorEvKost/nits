#include "config.h"
#include <math.h>
#include "LCD.h"


//const uint32_t _lcd_buffer_0 = LCD_BUFFER0_BASE_ADDR;
//const uint32_t _lcd_buffer_1 = LCD_BUFFER1_BASE_ADDR;


static inline void draw_point(int x, int y, COLOUR_TYPE colour)
{
    x = LCD_WIDTH - x;
//    y = LCD_HEIGHT - y;
//  if((x < 0) || (x > LCD_WIDTH) || (y < 0) || (y > LCD_HEIGHT)) return;					//*****
   *(COLOUR_TYPE *)(_lcd_buffer_0 + (x + y * LCD_WIDTH) * COLOUR_SIZE) = colour;

//	*(COLOUR_TYPE *)(LCD_BUFFER0_BASE_ADDR + (x + y * LCD_WIDTH) * COLOUR_SIZE) = colour;
//	*(COLOUR_TYPE *)(LCD_BUFFER1_BASE_ADDR + (x + y * LCD_WIDTH) * COLOUR_SIZE) = colour;
}

static inline int min(int a, int b)
{
  if(a < b) return a;
  else return b;
}

static inline int max(int a, int b)
{
  if(a > b) return a;
  else  return b;
}

void DrawPoint(int x, int y, COLOUR_TYPE colour)
{
  draw_point(x, y, colour);
}

ui16  ReadPoint(int x, int y)
{
    x = LCD_WIDTH - x;
	return *(COLOUR_TYPE *)(_lcd_buffer_0 + (x + y * LCD_WIDTH) * COLOUR_SIZE);
}

void DrawLine(int x0, int y0, int xn, int yn, COLOUR_TYPE colour)
{
  if(x0 == xn)
  {
    int yi = min(y0, yn);
    while(yi <= max(y0, yn))
    {
      draw_point(x0, yi++, colour);
    }
  }
  else if(y0 == yn)
  {
    int xi = min(x0, xn);
    while(xi <= max(x0, xn))
    {
      draw_point(xi++, y0, colour);
    }
  }
  else
  {
    int xi = min(x0, xn);
    while(xi <= max(x0, xn))
    {
      draw_point(xi, (y0 + (xi - x0) * (yn - y0) / (xn - x0)), colour);
      xi++;
    }
    int yi = min(y0, yn);
    while(yi <= max(y0, yn))
    {
      draw_point((x0 + (yi - y0) * (xn - x0) / (yn - y0)), yi, colour);
      yi++;
    }
  }
}

void RegionFill(int x0, int y0 , int width, int height, COLOUR_TYPE colour)
{
  ui32 y;
  for(int yi = y0; yi < y0 + height; yi++)
  {
    y=yi * LCD_WIDTH;

    for(int xi = x0; xi < x0 + width; xi++)
    {
		*(COLOUR_TYPE *)(_lcd_buffer_0 + (LCD_WIDTH -xi + y) * COLOUR_SIZE) = colour;
//		*(COLOUR_TYPE *)(LCD_BUFFER1_BASE_ADDR + (LCD_WIDTH -xi + y) * COLOUR_SIZE) = colour;
    }
  }
}

void RegionPop(int x0, int y0 , int width, int height, ui16* p)
{
	ui32 y;
	ui32 i=0;
	for(int yi = y0; yi < y0 + height+1; yi++)
	{
		y=yi * LCD_WIDTH;

		for(int xi = x0; xi < x0 + width+1; xi++)
		{
			*(COLOUR_TYPE *)(_lcd_buffer_0 + (LCD_WIDTH -xi + y) * COLOUR_SIZE)=*(p+i);
//			*(COLOUR_TYPE *)(LCD_BUFFER1_BASE_ADDR + (LCD_WIDTH -xi + y) * COLOUR_SIZE)=*(p+i);
			i+=2;
		}
	}
}

void RegionPush(int x0, int y0 , int width, int height, ui16* p)
{
	ui32 y;
	ui32 i=0;

	for(int yi = y0; yi < y0 + height+1; yi++)
	{
		y=yi * LCD_WIDTH;

		for(int xi = x0; xi < x0 + width+1; xi++)
		{
			*(p+i)=*(COLOUR_TYPE *)(_lcd_buffer_0 + (LCD_WIDTH -xi + y) * COLOUR_SIZE);
//			*(p+i)=*(COLOUR_TYPE *)(LCD_BUFFER1_BASE_ADDR + (LCD_WIDTH -xi + y) * COLOUR_SIZE);
			i+=2;
		}
	}
}




bool LoadFont(Font *font, const uint8_t *data)
{
  font->data = data;
  font->version = READ_UINT16(font->data);
  font->type = READ_UINT16(font->data + 66);
  font->ascent = READ_UINT16(font->data + 74);
  font->width = READ_UINT16(font->data + 86);
  font->height = READ_UINT16(font->data + 88);
  font->first = *(font->data + 95);
  font->last = *(font->data + 96);
  font->offset = READ_UINT32(font->data + 113);
  if(font->version != 0x0200) return false;
  if((font->type & 0x0001) != 0)  return false;
  return true;
}


uint16_t GetCharWidth(const Font *font, char c)
{

  if((c < font->first) || (c > font->last)) return 0;
  c = c - font->first;

  const uint8_t *table = font->data + 118;
  uint16_t width = font->width;
  if(width == 0)
  {
    width = READ_UINT16(table + 4 * c);
  }
  return width;
}

uint16_t GetStringWidth(const Font *font, const char *str)
{
  register uint32_t i = 0;
  uint16_t dx = 0;
  while(str[i] != '\0')
  {
    dx += GetCharWidth(font, str[i]);
    i++;
  }
  return dx;
}

uint16_t DrawChar(int x, int y, const Font *font, char c, COLOUR_TYPE colour)
{

  if((c < font->first) || (c > font->last)) return 0;
  c = c - font->first;

  const uint8_t *table = font->data + 118;
  uint16_t width = font->width;
  if(width == 0)
  {
    width = READ_UINT16(table + 4 * c);
  }

  uint16_t height = font->height;
  uint16_t bytes = (width - 1) / 8;
  uint16_t ascent = font->ascent;
  uint32_t offset = READ_UINT16(table + 4 * c + 2);
  for(int i = 0; i <= bytes; i++)
  {
    for(int yi = 0; yi < height; yi++)
    {
      uint8_t line = *(font->data + offset + height * i + yi);
      for(int xi = 0; xi < 8; xi++)
      {
        if((line >> (7 - xi)) & 0x01)
        {
          draw_point(x + xi + 8 * i, y + ascent - yi, colour);
        }
      }
    }
  }
  return width;
}

uint16_t DrawString(int x, int y, const Font *font, const char *str, COLOUR_TYPE colour)
{
  register uint32_t i = 0;
  uint16_t dx = 0;
  while(str[i] != '\0')
  {
    dx += DrawChar(x + dx, y, font, str[i], colour);
    i++;
  }
  return dx;
}
