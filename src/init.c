#include "LPC2478.h"
#include "config.h"
#include "LCD.h"

//port1
#define G0      (1<<8)  //PINSEL3
#define G1      (1<<10)
#define G2      (1<<12)
#define G3      (1<<14)
#define G4      (1<<16)
#define G5      (1<<18)
#define B2      (1<<20)
#define B3      (1<<22)
#define B4      (1<<24)
#define B5      (1<<26)

//port 2
#define LCDDCLK (3<<4)  //PINSEL4
#define LCDFP   (3<<6)            //vsync
#define LCDEN   (3<<8)
#define LCDLP   (3<<10)           //hsync
#define R2      (3<<12)
#define R3      (3<<14)
#define R4      (3<<16)
#define R5      (3<<18)
#define B0      (1<<24)
#define B1      (1<<26)

#define CAS	(1<<0)	//PINSEL5
#define RAS	(1<<2)
#define CLKOUT0	(1<<4)
#define	DYCS0	(1<<8)
#define CKE0	(1<<16)
#define DQM0	(1<<24)
#define DQM1	(1<<26)

//port3
#define D0	(1<<0)	//PINSEL6
#define D1	(1<<2)
#define D2	(1<<4)
#define D3	(1<<6)
#define D4	(1<<8)
#define D5	(1<<10)
#define D6	(1<<12)
#define D7	(1<<14)
#define D8	(1<<16)
#define D9	(1<<18)
#define D10	(1<<20)
#define D11	(1<<22)
#define D12	(1<<24)
#define D13	(1<<26)
#define D14	(1<<28)
#define D15	(1<<30)

//port4
#define A0	(1<<0)	//PINSEL8
#define A1	(1<<2)
#define A2	(1<<4)
#define A3	(1<<6)
#define A4	(1<<8)
#define A5	(1<<10)
#define A6	(1<<12)
#define A7	(1<<14)
#define A8	(1<<16)
#define A9	(1<<18)
#define A10	(1<<20)
#define A11	(1<<22)
#define A12	(1<<24)
#define A13	(1<<26)
#define A14	(1<<28)
#define A15 (1<<30)
					//PINSEL9
#define A16 (1<<0)
#define A17	(1<<2)
#define A18	(1<<4)
#define	A19	(1<<6)
#define	A20	(1<<8)
#define	OE	(1<<16)
#define WE	(1<<18)
#define R0	(2<<24)
#define R1	(2<<26)
#define	CS0	(1<<30)


//static ui32 _lcd_buffer = LCD_BUFFER0_BASE_ADDR;

void exram_init();
extern void Timer0IntrHandler (void);
extern void LCDVertSync (void);
extern void UART2IntrHandler(void);
extern void SPIIntrHandler(void);


void clock_init()
{
  // Отключаем PLL
  if(bit_is_set(PLLSTAT, PLLSTAT_PLLC))
  {
    PLLCON = _BV(PLLCON_PLLE);
    PLLFEED = 0xAA;
    PLLFEED = 0x55;
  }
  PLLCON = 0;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;

  // Выбираем в качестве источника Main Osc
  SCS |= _BV(SCS_OSCEN);
  while(!(SCS & _BV(SCS_OSCSTAT)));
  CLKSRCSEL = 0x01;

  // Устанавливаем значения M и N
  PLLCFG = (MSEL_VAL << PLLCFG_MSEL) | (NSEL_VAL << PLLCFG_NSEL);
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  PLLCON = _BV(PLLCON_PLLE);
  PLLFEED = 0xAA;
  PLLFEED = 0x55;

  // Устанавливаем значения делителя частоты
  CCLKCFG = CCLKCFG_VAL;

  // Ждем пока значения будут применены
  while(bit_is_set(PLLSTAT, PLLSTAT_PLOCK));

  // Подключаем PLL
  PLLCON = _BV(PLLCON_PLLE) | _BV(PLLCON_PLLC);
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
}

static void pins_init()
{
  PINSEL3=0;
  PINSEL4=0;
  PINSEL5=0;
  PINSEL6=0;
  PINSEL8=0;
  PINSEL9=0;

  PINSEL3|=G0|G1|G2|G3|G4|G5|B2|B3|B4|B5;
  PINSEL4|=LCDDCLK|LCDFP|LCDEN|LCDLP|R2|R3|R4|R5|B0|B1;
  PINSEL5|=CAS|RAS|CLKOUT0|DYCS0|CKE0|DQM0|DQM1;
  PINSEL6|=D0|D1|D2|D3|D4|D5|D6|D7|D8|D9|D10|D11|D12|D13|D14|D15;
  PINSEL8|=A0|A1|A2|A3|A4|A5|A6|A7|A8|A9|A10|A11|A12|A13|A14|A15;
  PINSEL9|=A16|A17|A18|A19|A20|OE|WE|CS0|R0|R1;
  PINSEL11|=0x0B;



  //P2.00 & P2.01
  // PINSEL4|=0x0A;      //альтернативная функция P2.00=TXD1 P2.01=RXD1
  FIO2DIR=1;            //светодиод на плате на P2.00


  //UART2 & SPI
  //P0.10 < TXD2
  //P0.11 < RXD2
  //P0.15 < SCK
  PINSEL0|=1<<22|1<<20|3<<30;
//            |     |     +--------- P0.15 <- SCK
//            |     +--------------- P0.10 <- TXD2
//            +--------------------- P0.11 <- RXD2
  //P0.17 < MISO
  PINSEL1 |= 3<<2;
//              +------------------- P0.17 <- MISO


  PINMODE0|=0<<22|2<<20|2<<30;
  PINMODE1|=2<<2;

  //по reset вход с pull-up
  //P0.12
  //P0.27
  //P0.28

  //P0.19
  FIO0DIR|=1<<23|1<<19|1<<16;				//порта P0.23, P0.19, P0.16 ? на вывод
  FIO0PIN&=~(1<<23);						//выкл сигнал (BEEP)


  //P1.31
  FIO1DIR=1<<31;					//порт P1.31 на вывод (светодиод энкодера)


  //P2.25
  //P2.30
  //P2.31

  //P3.26

  //P3.30							// по сбросу вход с подтяжкой на + (для ДХ)

	//P4.23
	//P4.26
	FIO4DIR=1<<26|1<<23;
																					   //    3130 2928 2726 2524 2322 2120 1918 1716
//	PINMODE9|=0x00208000;       //светодиод на плате на выводе P4.23 и Tx/Rx на P4.26  //  0b0000 0000 0010 0000 1000 0000 0000 0000;




//    PINSEL0 = 0x03055550;
//    PINSEL1 = 0x00000000;
//    PINSEL2 = 0x0280A8A0;
//    PINSEL3 = 0xC5555500;
//    PINSEL4 = 0x050FFFF0;
//    PINSEL4 = 0x050FFFFF;
//    PINSEL5 = 0x05F13115;
//    PINSEL6 = 0x55555555;
//    PINSEL8 = 0x55555555;
//    PINSEL9 = 0x1A055555;
//    PINSEL11 = 0x0000000B;


}

void LCD_Initialize()
{
  LCD_CFG = 0 << LCD_CFG_CLKDIV;
   LCD_TIMH = (134 << LCD_TIMH_HBP) | (16 << LCD_TIMH_HFP) |            //
              (10 << LCD_TIMH_HSW) | ((640 / 16 - 1) << LCD_TIMH_PPL);
   LCD_TIMV = (11 << LCD_TIMV_VBP) | (64 << LCD_TIMV_VFP) |				//(32 << LCD_TIMV_VFP)
              (2 << LCD_TIMV_VSW) | (480 << LCD_TIMV_LPP);
   LCD_POL = (0 << LCD_POL_PCD_HI) | (0 << LCD_POL_BCD) |
             ((640 / 1 - 1) << LCD_POL_CPL) | (0 << LCD_POL_IOE) |
             (0 << LCD_POL_IPC) | (1 << LCD_POL_IHS) | (1 << LCD_POL_IVS) |
             (0 << LCD_POL_ACB) | (0 << LCD_POL_CLKSEL) |
	     (2 << LCD_POL_PCD_LO);                                     //было 2
   LCD_CTRL = (0 << LCD_CTRL_WATERMARK) | (3 << LCD_CTRL_LcdVComp) |
              (0 << LCD_CTRL_LcdPwr) | (0 << LCD_CTRL_BEPO) |
			  (0 << LCD_CTRL_BEBO) | (1 << LCD_CTRL_BGR) |			// (0 << LCD_CTRL_BEBO)
              (0 << LCD_CTRL_LcdDual) | (0 << LCD_CTRL_LcdMono8) |
              (1 << LCD_CTRL_LcdTFT) | (0 << LCD_CTRL_LcdBW) |
			  (6 << LCD_CTRL_LcdBpp) | (0 << LCD_CTRL_LcdEn);		//(4 << LCD_CTRL_LcdBpp)
   LCD_INTMSK=(1<<LCD_INTMSK_VCompIM);
   LCD_UPBASE = LCD_BUFFER0_BASE_ADDR;
   LCD_LPBASE = LCD_BUFFER0_BASE_ADDR;
   uint32_t i = 0;
   for(i = 0; i < 100000; i++);
   LCD_CTRL |= (1 << LCD_CTRL_LcdEn);
   for(i = 0; i < 100000; i++);
   LCD_CTRL |= (1 << LCD_CTRL_LcdPwr);

   _lcd_buffer_0=LCD_BUFFER0_BASE_ADDR;
//   _lcd_buffer_1=LCD_BUFFER1_BASE_ADDR;

  }

void init()
{



	/* настройка */
//	PCONP&=~(_BV(PCONP_PCSSP0)|_BV(PCONP_PCSSP1));
	PCONP = _BV(PCONP_PCTIM0) | _BV(PCONP_PCEMC) | _BV(PCONP_PCAD) | _BV(PCONP_PCLCD) | _BV(PCONP_PCUART2) | _BV(PCONP_PCSPI);
//																			|					|																	|					|					|
//																			|					|
//																			|					+---------- питание контроллера UART2
//																			+------------------------------ питание контроллера LDC
	// Init timer 0
	T0TC=0x0;								//счетчик таймера
	T0PR=0x2A;								//период прескаллера
	T0MR0=0xfff;							// set timer 0 period = 99.887 Hz

	T0TCR=2;								// set reset & counting  disable
	T0TCR=0;								//             counting  disable
	T0CTCR=0;								// Timer Mode: every rising PCLK edge
	T0MCR=3;								// Enable Interrupt on MR0, Enable reset on MR0

	//interrupt
	T0IR|=1;								// clear pending interrupt

	//VIC_Init();
	VICProtection = 0;
	VICAddress = 0;                         //адрес текущего прерывания
	VICIntEnClr=0;
	VICIntSelect=0;                         //все PIQ

	VICVectAddr4 = (ui32) Timer0IntrHandler;
//  VICVectAddr5 = (ui32) Timer1IntrHandler;
	VICVectAddr10 = (ui32) SPIIntrHandler;
	VICVectAddr16 = (ui32) LCDVertSync;
	VICVectAddr28=(ui32) UART2IntrHandler;

	VICIntEnable |= 0x00000010;             //разрешаем прерывание от T0
//  VICIntEnable |= 0x00000020;             //разрешаем прерывание от T1
	VICIntEnable |= 0x00010000;				//разрешаем прерывания от LCD
	VICIntEnable |= 0x10000000;				//разрешаем прерывание от UART2
	VICIntEnable |= 0x00000400;				//разрешаем прерывание от SPI/SSP0

	T0TCR=1;                                // TIMER0 counting enable
//  T1TCR=1;

	//UART2
	PCLKSEL1 |= 2<<PCLK_UART2;				//Peripheral clock CCLK/2
	U2LCR |= (3<<U2LCR_WLS)|(0<<U2LCR_SBS)|(1<< U2LCR_DLAB);
	U2DLL=230;
	U2DLM=0;
	//U2FDR|=13<<U2FDR_MULVAL|4<<U2FDR_DIVADDVAL;
	U2FDR|=1<<U2FDR_MULVAL|0<<U2FDR_DIVADDVAL;
	U2FCR|=(1<<U2FCR_FIFOEn)|(1<<U2FCR_RXTriggerLevel);
	U2LCR&=~(1<< U2LCR_DLAB);
	U2IER|=(1<<U2IER_THREIntEn)|(1<<U2IER_RBRIntEn)|(1<<U2IER_RXLSIntEn);
//	U2IER|=(1<<U2IER_THREIntEn);
//	U2IER|=(1<<U2IER_THREIntEn)|(1<<U2IER_RBRIntEn);

    //SPI
    PCLKSEL0|=3<<PCLK_SPI;                          //Peripheral clock CCLK/8
    S0SPCR = 0x00;
    S0SPCR|=(0<<S0SPCR_CPHA)|(1<<S0SPCR_BitEnable)|(1<<S0SPCR_MSTR)|(1<<S0SPCR_SPIE)|(1<<S0SPCR_CPOL);     //по 16 бит, мастер
    S0SPCCR=0xFE;


    MAMTIM = MAM_FETCH;
    MAMCR = MAM_MODE;
    MEMMAP = MEMMAP_VAL;
    SCS |= _BV(SCS_GPIOM);
    clock_init();
    pins_init();
    EMCControl = 0x00000001;
    exram_init();
}
