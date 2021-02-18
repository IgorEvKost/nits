#ifndef CONFIG_H
#define CONFIG_H
#include "LPC2478.h"

#define ui32  uint32_t
#define ui16  uint16_t
#define ui8   uint8_t


// Рабочая частота
#define FOSC                    (12000000)
#define MSEL_VAL                (12 - 1)
#define NSEL_VAL                (1 - 1)
#define CCLKCFG_VAL             (4 - 1)
#define CCLK                    (2 * FOSC * (MSEL_VAL + 1) / \
                                ((NSEL_VAL + 1) * (CCLKCFG_VAL + 1)))       // (2*12.0*1)/(1*4)=6


// MAM
#define MAM_FETCH               (4)
#define MAM_MODE                (2)

// MEMMAP
#define MEMMAP_VAL              (1)

#define EXRAM_SIZE              ((16 * 1024 * 1024 * 16) / 8)

#define EXRAM_BASE_ADDR         (DYNAMIC_BANK0_BASE_ADDR)
#define EXRAM_END_ADDR          (EXRAM_BASE_ADDR + EXRAM_SIZE)

//LCD
//#define LCD_BUFFER0_BASE_ADDR   (DYNAMIC_BANK0_BASE_ADDR)
#define _16BPP        (16)
#define _24BPP        (32)
#define COLOUR_DEPTH  _16BPP
#define COLOUR_SIZE   (COLOUR_DEPTH / 8)

#if COLOUR_SIZE == 2
#define COLOUR_TYPE   uint16_t
#elif COLOUR_SIZE == 4
#define COLOUR_TYPE   uint32_t
#endif // COLOUR_SIZE

#define LCD_WIDTH     (640)
#define LCD_HEIGHT    (480)

#define LCD_BUFFER_SIZE         (LCD_WIDTH * LCD_HEIGHT * COLOUR_SIZE)
#define LCD_BUFFER0_BASE_ADDR   (EXRAM_BASE_ADDR)
#define LCD_BUFFER0_END_ADDR    (LCD_BUFFER0_BASE_ADDR + LCD_BUFFER_SIZE)
#define LCD_BUFFER1_BASE_ADDR   (LCD_BUFFER0_END_ADDR)
#define LCD_BUFFER1_END_ADDR    (LCD_BUFFER1_BASE_ADDR + LCD_BUFFER_SIZE)

// Heap
#define HEAP_SIZE               (EXRAM_SIZE - LCD_BUFFER_SIZE * 2-1048576*2)
#define HEAP_BASE_ADDR          (LCD_BUFFER1_END_ADDR+1048576*2)
#define HEAP_END_ADDR           (EXRAM_END_ADDR)



// Touchscreen
#define XADCDR                  AD0DR3
#define XADCNUM                 (3)
#define YADCDR                  AD0DR1
#define YADCNUM                 (1)

// X1
#define X1BIT                   (24)
#define X1DIR                   FIO0DIR
#define X1SET                   FIO0SET
#define X1CLR                   FIO0CLR
#define X1PIN                   FIO0PIN
#define X1SELBIT                (16)
#define X1SEL                   PINSEL1
#define X1MODE                  PINMODE1
#define X1ADCSEL                (1)

// X2
#define X2BIT                   (29)
#define X2DIR                   FIO3DIR
#define X2SET                   FIO3SET
#define X2CLR                   FIO3CLR
#define X2PIN                   FIO3PIN
#define X2SELBIT                (26)
#define X2SEL                   PINSEL7
#define X2MODE                  PINMODE7

// Y1
#define Y1BIT                   (26)
#define Y1DIR                   FIO0DIR
#define Y1SET                   FIO0SET
#define Y1CLR                   FIO0CLR
#define Y1PIN                   FIO0PIN
#define Y1SELBIT                (20)
#define Y1SEL                   PINSEL1
#define Y1MODE                  PINMODE1
#define Y1ADCSEL                (1)

// Y2
#define Y2BIT                   (28)
#define Y2DIR                   FIO3DIR
#define Y2SET                   FIO3SET
#define Y2CLR                   FIO3CLR
#define Y2PIN                   FIO3PIN
#define Y2SELBIT                (24)
#define Y2SEL                   PINSEL7
#define Y2MODE                  PINMODE7

#endif // CONFIG_H

