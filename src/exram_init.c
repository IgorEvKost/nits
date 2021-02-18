#include <stdint.h>
#include <stdbool.h>
#include "LPC2478.h"

#define P2C(p)  ((uint32_t)(((p) / (1000000000.0 / CCLK)) + 1))

#define TRP                       (2)
#define TRAS                      (3)
#define TSREX                     (7)
#define TAPR                      (2)
#define TDAL                      (5)
#define TWR                       (1)
#define TRC                       (5)
#define TRFC                      (5)
#define TXSR                      (7)
#define TRRD                      (1)
#define TMRD                      (2)
#define TREF                      (0x001C)

void exram_init()
{

//  SCS     |= 0x00000002;   // Reset EMC.

//  EMCControl = 0x00000001; // Enable EMC and Disable Address mirror.

//  PCONP   |= 0x00000800;   // Turn on EMC PCLK.


	EMCDynamicRP    = TRP;
	EMCDynamicRAS   = TRAS;
	EMCDynamicSREX  = TSREX;
	EMCDynamicAPR   = TAPR;
	EMCDynamicDAL   = TDAL;
	EMCDynamicWR    = TWR;
	EMCDynamicRC    = TRC;
	EMCDynamicRFC   = TRFC;
	EMCDynamicXSR   = TXSR;
	EMCDynamicRRD   = TRRD;
	EMCDynamicMRD   = TMRD;

	EMCDynamicReadConfig = 1;
	EMCDynamicRasCas0 = 0x00000303;
	EMCDynamicConfig0 = 0x00000680; //

	EMCDynamicControl = 0x00000183; // NOP;

	for(volatile int i = 100 * 72; i; i--) {__asm ("nop");}

	EMCDynamicControl = 0x00000103; // PRECHARGE ALL
	EMCDynamicRefresh = 1;

	for(volatile int i = 128; i; i--) {__asm ("nop");}

	EMCDynamicRefresh = TREF;
	EMCDynamicControl = 0x00000083;


	volatile uint32_t temp=temp;
	temp = *(volatile uint32_t *)(0xA0000000 | (0x33 << 12)); //зачем?
	EMCDynamicControl = 0x00000000;
	EMCDynamicConfig0 |= 0x00080000;

	for(volatile int i = 100 * 72; i; i--) {__asm ("nop");}

	EMCStaticConfig0 = 0x00000081;		// Selects the memory configuration for static chip select 0.
	EMCStaticWaitRd0 = 0x1f;			// Selects the delay from chip select 0 to a read access.
	EMCStaticWaitOen0 = 0x2;			// Selects the delay from chip select 0 or address change, whichever is later, to output enable.
	EMCStaticWaitWen0 = 0x2;			// Selects the delay from chip select 0 to write enable.
	EMCStaticWaitPage0 = 0x1f;			// Selects the delay for asynchronous page mode sequential accesses for chip select 0.
	EMCStaticWaitWr0 = 0x1f;			// Selects the delay from chip select 0 to a write access.
	EMCStaticWaitTurn0 = 0xf;			// Selects the number of bus turnaround cycles for chip select 0.
//	EMC

	for(volatile int i = 100 * 72; i; i--) {__asm ("nop");}


}
