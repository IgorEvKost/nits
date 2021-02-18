#ifndef LPC2478_H
#define LPC2478_H
#include "SFR.h"
#define RAMFUNC __attribute__((section(".ramfunc")))
#define RODATA  __attribute__((section(".rodata")))
#define PACKED  __attribute__((packed))

#define READ_UINT16(ptr)  ((*((ptr) + 1) << 8) | (*((ptr)) << 0))
#define READ_UINT32(ptr)  ((*((ptr) + 3) << 24) | (*((ptr) + 2) << 16) | \
                          (*((ptr) + 1) << 8) | (*((ptr)) << 0))

// On-chip non-volatile memory and Fast I/O
#define FLASH_BASE_ADDR           0x00000000
#define FLASH_END_ADDR            0x0007FFFF
#define FIO_BASE_ADDR             0x3FFFC000
#define FIO_END_ADDR              0x3FFFFFFF

// On-chip RAM
#define RAM_BASE_ADDR             0x40000000
#define RAM_END_ADDR              0x4000FFFF
#define ETHERNET_RAM_BASE_ADDR    0x7FE00000
#define ETHERNET_RAM_END_ADDR     0x7FE03FFF
#define USB_RAM_BASE_ADDR         0x7FD00000
#define USB_RAM_END_ADDR          0x7FD03FFF

// Off-chip memory
#define STATIC_BANK0_BASE_ADDR    0x80000000
#define STATIC_BANK0_END_ADDR     0x80FFFFFF
#define STATIC_BANK1_BASE_ADDR    0x81000000
#define STATIC_BANK1_END_ADDR     0x81FFFFFF
#define STATIC_BANK2_BASE_ADDR    0x82000000
#define STATIC_BANK2_END_ADDR     0x82FFFFFF
#define STATIC_BANK3_BASE_ADDR    0x83000000
#define STATIC_BANK3_END_ADDR     0x83FFFFFF
#define DYNAMIC_BANK0_BASE_ADDR   0xA0000000
#define DYNAMIC_BANK0_END_ADDR    0xAFFFFFFF
#define DYNAMIC_BANK1_BASE_ADDR   0xB0000000
#define DYNAMIC_BANK1_END_ADDR    0xBFFFFFFF
#define DYNAMIC_BANK2_BASE_ADDR   0xC0000000
#define DYNAMIC_BANK2_END_ADDR    0xCFFFFFFF
#define DYNAMIC_BANK3_BASE_ADDR   0xD0000000
#define DYNAMIC_BANK3_END_ADDR    0xDFFFFFFF

// APB peripherals
#define APB_BASE_ADDR             0xE0000000
#define APB_END_ADDR              0xEFFFFFFF

// AHB peripherals
#define AHB_BASE_ADDR             0xF0000000
#define AHB_END_ADDR              0xFFFFFFFF

#define WDT_BASE_ADDR             0xE0000000

/* Timer0 */
#define TIMER0_BASE_ADDR          0xE0004000
#define T0IR                      SFR_IO32(TIMER0_BASE_ADDR + 0x000)
#define T0TCR                     SFR_IO32(TIMER0_BASE_ADDR + 0x004)
#define T0TC                      SFR_IO32(TIMER0_BASE_ADDR + 0x008)
#define T0PR                      SFR_IO32(TIMER0_BASE_ADDR + 0x00C)
#define T0PC                      SFR_IO32(TIMER0_BASE_ADDR + 0x010)
#define T0MCR                     SFR_IO32(TIMER0_BASE_ADDR + 0x014)
#define T0MR0                     SFR_IO32(TIMER0_BASE_ADDR + 0x018)
#define T0MR1                     SFR_IO32(TIMER0_BASE_ADDR + 0x01C)
#define T0MR2                     SFR_IO32(TIMER0_BASE_ADDR + 0x020)
#define T0MR3                     SFR_IO32(TIMER0_BASE_ADDR + 0x024)
#define T0CCR                     SFR_IO32(TIMER0_BASE_ADDR + 0x028)
#define T0CR0                     SFR_IO32(TIMER0_BASE_ADDR + 0x02C)
#define T0CR1                     SFR_IO32(TIMER0_BASE_ADDR + 0x030)
#define T0EMR                     SFR_IO32(TIMER0_BASE_ADDR + 0x03C)
#define T0CTCR                    SFR_IO32(TIMER0_BASE_ADDR + 0x070)

/* Timer1 */
#define TIMER1_BASE_ADDR          0xE0008000
#define T1IR                      SFR_IO32(TIMER1_BASE_ADDR + 0x000)
#define T1IR_MR0Int               (0)
#define T1IR_MR1Int               (1)
#define T1IR_MR2Int               (2)
#define T1IR_MR3Int               (3)
#define T1IR_CR0Int               (4)
#define T1IR_CR1Int               (5)
#define T1IR_CR2Int               (6)
#define T1IR_CR3Int               (7)
#define T1TCR                     SFR_IO32(TIMER1_BASE_ADDR + 0x004)
#define T1TCR_CounterEnable       (0)
#define T1TCR_CounterReset        (1)
#define T1TC                      SFR_IO32(TIMER1_BASE_ADDR + 0x008)
#define T1PR                      SFR_IO32(TIMER1_BASE_ADDR + 0x00C)
#define T1PC                      SFR_IO32(TIMER1_BASE_ADDR + 0x010)
#define T1MCR                     SFR_IO32(TIMER1_BASE_ADDR + 0x014)
#define T1MCR_MR0I                (0)
#define T1MCR_MR0R                (1)
#define T1MCR_MR0S                (2)
#define T1MCR_MR1I                (3)
#define T1MCR_MR1R                (4)
#define T1MCR_MR1S                (5)
#define T1MCR_MR2I                (6)
#define T1MCR_MR2R                (7)
#define T1MCR_MR2S                (8)
#define T1MCR_MR3I                (9)
#define T1MCR_MR3R                (10)
#define T1MCR_MR3S                (11)
#define T1MR0                     SFR_IO32(TIMER1_BASE_ADDR + 0x018)
#define T1MR1                     SFR_IO32(TIMER1_BASE_ADDR + 0x01C)
#define T1MR2                     SFR_IO32(TIMER1_BASE_ADDR + 0x020)
#define T1MR3                     SFR_IO32(TIMER1_BASE_ADDR + 0x024)
#define T1CCR                     SFR_IO32(TIMER1_BASE_ADDR + 0x028)
#define T1CCR_CAP0RE              (0)
#define T1CCR_CAP0FE              (1)
#define T1CCR_CAP0I               (2)
#define T1CCR_CAP1RE              (3)
#define T1CCR_CAP1FE              (4)
#define T1CCR_CAP1I               (5)
#define T1CR0                     SFR_IO32(TIMER1_BASE_ADDR + 0x02C)
#define T1CR1                     SFR_IO32(TIMER1_BASE_ADDR + 0x030)
#define T1EMR                     SFR_IO32(TIMER1_BASE_ADDR + 0x03C)
#define T1EMR_EM0                 (0)
#define T1EMR_EM1                 (1)
#define T1EMR_EM2                 (2)
#define T1EMR_EM3                 (3)
#define T1EMR_EMC0                (4)
#define T1EMR_EMC1                (6)
#define T1EMR_EMC2                (8)
#define T1EMR_EMC3                (10)
#define T1CTCR                    SFR_IO32(TIMER1_BASE_ADDR + 0x070)
#define T1CTCR_Mode                 (0)
#define T1CTCR_InputSelect          (2)

/* UART0 */
#define UART0_BASE_ADDR           0xE000C000
#define U0RBR                     SFR_IO32(UART0_BASE_ADDR + 0x000)
#define U0THR                     SFR_IO32(UART0_BASE_ADDR + 0x000)
#define U0DLL                     SFR_IO32(UART0_BASE_ADDR + 0x000)
#define U0DLM                     SFR_IO32(UART0_BASE_ADDR + 0x004)
#define U0IER                     SFR_IO32(UART0_BASE_ADDR + 0x004)
#define U0IER_RBRIntEn            (0)
#define U0IER_THREIntEn           (1)
#define U0IER_RXLSIntEn           (2)
#define U0IER_ABEOIntEn           (8)
#define U0IER_ABTOIntEn           (9)
#define U0IIR                     SFR_IO32(UART0_BASE_ADDR + 0x008)
#define U0IIR_IntStatus           (0)
#define U0IIR_IntId               (1)
#define U0IIR_FIFOEn              (6)
#define U0IIR_ABEOInt             (8)
#define U0IIR_ABTOInt             (9)
#define U0FCR                     SFR_IO32(UART0_BASE_ADDR + 0x008)
#define U0FCR_FIFOEn              (0)
#define U0FCR_RXFIFOReset         (1)
#define U0FCR_TXFIFOReset         (2)
#define U0FCR_RXTriggerLevel      (6)
#define U0LCR                     SFR_IO32(UART0_BASE_ADDR + 0x00C)
#define U0LCR_WLS                 (0)
#define U0LCR_SBS                 (2)
#define U0LCR_PE                  (3)
#define U0LCR_PS                  (4)
#define U0LCR_BC                  (6)
#define U0LCR_DLAB                (7)
#define U0LSR                     SFR_IO32(UART0_BASE_ADDR + 0x014)
#define U0LSR_RDR                 (0)
#define U0LSR_OE                  (1)
#define U0LSR_PE                  (2)
#define U0LSR_FE                  (3)
#define U0LSR_BI                  (4)
#define U0LSR_THRE                (5)
#define U0LSR_TEMT                (6)
#define U0LSR_RXFE                (7)
#define U0SCR                     SFR_IO32(UART0_BASE_ADDR + 0x01C)
#define U0ACR                     SFR_IO32(UART0_BASE_ADDR + 0x020)
#define U0FDR                     SFR_IO32(UART0_BASE_ADDR + 0x028)
#define U0FDR_DIVADDVAL           (0)
#define U0FDR_MULVAL              (4)
#define U0TER                     SFR_IO32(UART0_BASE_ADDR + 0x030)

/* UART1 */
#define UART1_BASE_ADDR           0xE0010000
#define U1RBR                     SFR_IO32(UART1_BASE_ADDR + 0x000)
#define U1THR                     SFR_IO32(UART1_BASE_ADDR + 0x000)
#define U1DLL                     SFR_IO32(UART1_BASE_ADDR + 0x000)
#define U1DLM                     SFR_IO32(UART1_BASE_ADDR + 0x004)
#define U1IER                     SFR_IO32(UART1_BASE_ADDR + 0x004)
#define U1IIR                     SFR_IO32(UART1_BASE_ADDR + 0x008)
#define U1FCR                     SFR_IO32(UART1_BASE_ADDR + 0x008)
#define U1MCR                     SFR_IO32(UART1_BASE_ADDR + 0x010)
#define U1LCR                     SFR_IO32(UART1_BASE_ADDR + 0x00C)
#define U1LSR                     SFR_IO32(UART1_BASE_ADDR + 0x014)
#define U1MSR                     SFR_IO32(UART1_BASE_ADDR + 0x018)
#define U1SCR                     SFR_IO32(UART1_BASE_ADDR + 0x01C)
#define U1ACR                     SFR_IO32(UART1_BASE_ADDR + 0x020)
#define U1FDR                     SFR_IO32(UART1_BASE_ADDR + 0x028)
#define U1TER                     SFR_IO32(UART1_BASE_ADDR + 0x030)


/* UART2 */
#define UART2_BASE_ADDR           0xE0078000
#define U2RBR                     SFR_IO32(UART2_BASE_ADDR + 0x000)
#define U2THR                     SFR_IO32(UART2_BASE_ADDR + 0x000)
#define U2DLL                     SFR_IO32(UART2_BASE_ADDR + 0x000)
#define U2DLM                     SFR_IO32(UART2_BASE_ADDR + 0x004)
#define U2IER                     SFR_IO32(UART2_BASE_ADDR + 0x004)
#define U2IER_RBRIntEn            (0)
#define U2IER_THREIntEn           (1)
#define U2IER_RXLSIntEn           (2)
#define U2IER_ABEOIntEn           (8)
#define U2IER_ABTOIntEn           (9)
#define U2IIR                     SFR_IO32(UART2_BASE_ADDR + 0x008)
#define U2IIR_IntStatus           (0)
#define U2IIR_IntId               (1)
#define U2IIR_FIFOEn              (6)
#define U2IIR_ABEOInt             (8)
#define U2IIR_ABTOInt             (9)
#define U2FCR                     SFR_IO32(UART2_BASE_ADDR + 0x008)
#define U2FCR_FIFOEn              (0)
#define U2FCR_RXFIFOReset         (1)
#define U2FCR_TXFIFOReset         (2)
#define U2FCR_RXTriggerLevel      (6)
#define U2LCR                       SFR_IO32(UART2_BASE_ADDR + 0x00C)
#define U2LCR_WLS                   (0)
#define U2LCR_SBS                   (2)
#define U2LCR_PE                    (3)
#define U2LCR_PS                    (4)
#define U2LCR_BC                    (6)
#define U2LCR_DLAB                  (7)
#define U2LSR                     SFR_IO32(UART2_BASE_ADDR + 0x014)
#define U2LSR_RDR                 (0)
#define U2LSR_OE                  (1)
#define U2LSR_PE                  (2)
#define U2LSR_FE                  (3)
#define U2LSR_BI                  (4)
#define U2LSR_THRE                (5)
#define U2LSR_TEMT                (6)
#define U2LSR_RXFE                (7)
#define U2SCR                     SFR_IO32(UART2_BASE_ADDR + 0x01C)
#define U2ACR                     SFR_IO32(UART2_BASE_ADDR + 0x020)
#define U2FDR                     SFR_IO32(UART2_BASE_ADDR + 0x028)
#define U2FDR_DIVADDVAL           (0)
#define U2FDR_MULVAL              (4)
#define U2TER                     SFR_IO32(UART2_BASE_ADDR + 0x030)


/* UART3 */
#define UART3_BASE_ADDR           0xE007С000
#define U3RBR                     SFR_IO32(UART3_BASE_ADDR + 0x000)
#define U3THR                     SFR_IO32(UART3_BASE_ADDR + 0x000)
#define U3DLL                     SFR_IO32(UART3_BASE_ADDR + 0x000)
#define U3DLM                     SFR_IO32(UART3_BASE_ADDR + 0x004)
#define U3IER                     SFR_IO32(UART3_BASE_ADDR + 0x004)
#define U3IER_RBRIntEn            (0)
#define U3IER_THREIntEn           (1)
#define U3IER_RXLSIntEn           (2)
#define U3IER_ABEOIntEn           (8)
#define U3IER_ABTOIntEn           (9)
#define U3IIR                     SFR_IO32(UART3_BASE_ADDR + 0x008)
#define U3IIR_IntStatus           (0)
#define U3IIR_IntId               (1)
#define U3IIR_FIFOEn              (6)
#define U3IIR_ABEOInt             (8)
#define U3IIR_ABTOInt             (9)
#define U3FCR                     SFR_IO32(UART3_BASE_ADDR + 0x008)
#define U3FCR_FIFOEn              (0)
#define U3FCR_RXFIFOReset         (1)
#define U3FCR_TXFIFOReset         (2)
#define U3FCR_RXTriggerLevel      (6)
#define U3LCR                     SFR_IO32(UART3_BASE_ADDR + 0x00C)
#define U3LCR_WLS                 (0)
#define U3LCR_SBS                 (2)
#define U3LCR_PE                  (3)
#define U3LCR_PS                  (4)
#define U3LCR_BC                  (6)
#define U3LCR_DLAB                (7)
#define U3LSR                     SFR_IO32(UART3_BASE_ADDR + 0x014)
#define U3LSR_RDR                 (0)
#define U3LSR_OE                  (1)
#define U3LSR_PE                  (2)
#define U3LSR_FE                  (3)
#define U3LSR_BI                  (4)
#define U3LSR_THRE                (5)
#define U3LSR_TEMT                (6)
#define U3LSR_RXFE                (7)
#define U3SCR                     SFR_IO32(UART3_BASE_ADDR + 0x01C)
#define U3ACR                     SFR_IO32(UART3_BASE_ADDR + 0x020)
#define U3FDR                     SFR_IO32(UART3_BASE_ADDR + 0x028)
#define U3FDR_DIVADDVAL           (0)
#define U3FDR_MULVAL              (4)
#define U3TER                     SFR_IO32(UART3_BASE_ADDR + 0x030)


#define PWM0_BASE_ADDR            0xE0014000

#define PWM1_BASE_ADDR            0xE0018000
#define PWM1IR                    SFR_IO32(PWM1_BASE_ADDR + 0x000)
#define PWM1TCR                   SFR_IO32(PWM1_BASE_ADDR + 0x004)
#define PWM1TC                    SFR_IO32(PWM1_BASE_ADDR + 0x008)
#define PWM1PR                    SFR_IO32(PWM1_BASE_ADDR + 0x00C)
#define PWM1PC                    SFR_IO32(PWM1_BASE_ADDR + 0x010)
#define PWM1MCR                   SFR_IO32(PWM1_BASE_ADDR + 0x014)
#define PWM1MR0                   SFR_IO32(PWM1_BASE_ADDR + 0x018)
#define PWM1MR1                   SFR_IO32(PWM1_BASE_ADDR + 0x01C)
#define PWM1MR2                   SFR_IO32(PWM1_BASE_ADDR + 0x020)
#define PWM1MR3                   SFR_IO32(PWM1_BASE_ADDR + 0x024)
#define PWM1CCR                   SFR_IO32(PWM1_BASE_ADDR + 0x028)
#define PWM1CR1                   SFR_IO32(PWM1_BASE_ADDR + 0x030)
#define PWM1MR4                   SFR_IO32(PWM1_BASE_ADDR + 0x040)
#define PWM1MR5                   SFR_IO32(PWM1_BASE_ADDR + 0x044)
#define PWM1MR6                   SFR_IO32(PWM1_BASE_ADDR + 0x048)
#define PWM1PCR                   SFR_IO32(PWM1_BASE_ADDR + 0x04C)
#define PWM1LER                   SFR_IO32(PWM1_BASE_ADDR + 0x050)
#define PWM1CTCR                  SFR_IO32(PWM1_BASE_ADDR + 0x070)

#define U0FDR_DIVADDVAL           (0)
#define U0FDR_MULVAL              (4)
#define I2C0_BASE_ADDR            0xE001C000

/*    SPI     */
#define SPI_BASE_ADDR               0xE0020000

#define S0SPCR                      SFR_IO32(SPI_BASE_ADDR)         //SPI Control Register
#define S0SPCR_BitEnable            (2)
#define S0SPCR_CPHA                 (3)
#define S0SPCR_CPOL                 (4)
#define S0SPCR_MSTR                 (5)
#define S0SPCR_LSBF                 (6)
#define S0SPCR_SPIE                 (7)
#define S0SPCR_BITS                 (8)

#define S0SPSR                      SFR_IO32(SPI_BASE_ADDR+0x004)   //SPI Status Register
#define S0SPSR_ABRT                 (3) //Slave abort
#define S0SPSR_MODF                 (4) //Mode fault
#define S0SPSR_ROVR                 (5) //Read overrun
#define S0SPSR_WCOL                 (6) //Write collision
#define S0SPSR_SPIF                 (7) //SPI transfer complete flag

#define S0SPDR                      SFR_IO32(SPI_BASE_ADDR+0x008)   //SPI Data Register
#define S0SPCCR                     SFR_IO32(SPI_BASE_ADDR+0x00C)   //SPI0 Clock counter setting (8 bit)
#define SPTCR                       SFR_IO32(SPI_BASE_ADDR+0x010)   //SPI Test Control Register
#define SPTCR_Test                  (1)                             //SPI test mode
#define SPTSR                       SFR_IO32(SPI_BASE_ADDR+0x014)   //SPI Test Status Register
#define S0SPINT                     SFR_IO32(SPI_BASE_ADDR+0x01C)   //SPI Interrupt Register
#define S0SPINT_SPIIF               (0)                             //SPI interrupt flag
/* End SPI definitions */




#define RTC_BASE_ADDR             0xE0024000

/* General Purpose Input/Output */
#define GPIO_BASE_ADDR            0xE0028000

/* Legacy APB accessible registers */
#define IO0PIN                    SFR_IO32(GPIO_BASE_ADDR + 0x000)
#define IO0SET                    SFR_IO32(GPIO_BASE_ADDR + 0x004)
#define IO0DIR                    SFR_IO32(GPIO_BASE_ADDR + 0x008)
#define IO0CLR                    SFR_IO32(GPIO_BASE_ADDR + 0x00C)
// MAC registersSFR_IO32(GPIO_BASE_ADDR + 0x00C)
#define IO1PIN                    SFR_IO32(GPIO_BASE_ADDR + 0x010)
#define IO1SET                    SFR_IO32(GPIO_BASE_ADDR + 0x014)
#define IO1DIR                    SFR_IO32(GPIO_BASE_ADDR + 0x018)
#define IO1CLR                    SFR_IO32(GPIO_BASE_ADDR + 0x01C)
/* Local bus accessible registers */
#define FIO0DIR                   SFR_IO32(FIO_BASE_ADDR + 0x000)
#define FIO1DIR                   SFR_IO32(FIO_BASE_ADDR + 0x020)
#define FIO2DIR                   SFR_IO32(FIO_BASE_ADDR + 0x040)
#define FIO3DIR                   SFR_IO32(FIO_BASE_ADDR + 0x060)
#define FIO4DIR                   SFR_IO32(FIO_BASE_ADDR + 0x080)
#define FIO0MASK                  SFR_IO32(FIO_BASE_ADDR + 0x010)
#define FIO1MASK                  SFR_IO32(FIO_BASE_ADDR + 0x030)
#define FIO2MASK                  SFR_IO32(FIO_BASE_ADDR + 0x050)
#define FIO3MASK                  SFR_IO32(FIO_BASE_ADDR + 0x070)
#define FIO4MASK                  SFR_IO32(FIO_BASE_ADDR + 0x090)
#define FIO0PIN                   SFR_IO32(FIO_BASE_ADDR + 0x014)
#define FIO1PIN                   SFR_IO32(FIO_BASE_ADDR + 0x034)
#define FIO2PIN                   SFR_IO32(FIO_BASE_ADDR + 0x054)


#define FIO0PIN0                  SFR_IO32(FIO_BASE_ADDR + 0x014)
#define FIO1PIN0                  SFR_IO32(FIO_BASE_ADDR + 0x034)
#define FIO2PIN0                  SFR_IO32(FIO_BASE_ADDR + 0x054)
#define FIO3PIN0                  SFR_IO32(FIO_BASE_ADDR + 0x074)
#define FIO4PIN0                  SFR_IO32(FIO_BASE_ADDR + 0x094)

#define FIO0PIN1                  SFR_IO32(FIO_BASE_ADDR + 0x015)
#define FIO1PIN1                  SFR_IO32(FIO_BASE_ADDR + 0x035)
#define FIO2PIN1                  SFR_IO32(FIO_BASE_ADDR + 0x055)
#define FIO3PIN1                  SFR_IO32(FIO_BASE_ADDR + 0x075)
#define FIO4PIN1                  SFR_IO32(FIO_BASE_ADDR + 0x095)

#define FIO0PIN2                  SFR_IO32(FIO_BASE_ADDR + 0x016)
#define FIO1PIN2                  SFR_IO32(FIO_BASE_ADDR + 0x036)
#define FIO2PIN2                  SFR_IO32(FIO_BASE_ADDR + 0x056)
#define FIO3PIN2                  SFR_IO32(FIO_BASE_ADDR + 0x076)
#define FIO4PIN2                  SFR_IO32(FIO_BASE_ADDR + 0x096)


#define FIO0PIN3                  SFR_IO32(FIO_BASE_ADDR + 0x017)
#define FIO1PIN3                  SFR_IO32(FIO_BASE_ADDR + 0x037)
#define FIO2PIN3                  SFR_IO32(FIO_BASE_ADDR + 0x057)
#define FIO3PIN3                  SFR_IO32(FIO_BASE_ADDR + 0x077)
#define FIO4PIN3                  SFR_IO32(FIO_BASE_ADDR + 0x097)

#define FIO0PINL                  SFR_IO32(FIO_BASE_ADDR + 0x014)
#define FIO1PINL                  SFR_IO32(FIO_BASE_ADDR + 0x034)
#define FIO2PINL                  SFR_IO32(FIO_BASE_ADDR + 0x054)
#define FIO3PINL                  SFR_IO32(FIO_BASE_ADDR + 0x074)
#define FIO4PINL                  SFR_IO32(FIO_BASE_ADDR + 0x094)

#define FIO0PINU                  SFR_IO32(FIO_BASE_ADDR + 0x016)
#define FIO1PINU                  SFR_IO32(FIO_BASE_ADDR + 0x036)
#define FIO2PINU                  SFR_IO32(FIO_BASE_ADDR + 0x056)
#define FIO3PINU                  SFR_IO32(FIO_BASE_ADDR + 0x076)
#define FIO4PINU                  SFR_IO32(FIO_BASE_ADDR + 0x096)


#define FIO3PIN                   SFR_IO32(FIO_BASE_ADDR + 0x074)
#define FIO4PIN                   SFR_IO32(FIO_BASE_ADDR + 0x094)
#define FIO0SET                   SFR_IO32(FIO_BASE_ADDR + 0x018)
#define FIO1SET                   SFR_IO32(FIO_BASE_ADDR + 0x038)
#define FIO2SET                   SFR_IO32(FIO_BASE_ADDR + 0x058)
#define FIO3SET                   SFR_IO32(FIO_BASE_ADDR + 0x078)
#define FIO4SET                   SFR_IO32(FIO_BASE_ADDR + 0x098)
#define FIO0CLR                   SFR_IO32(FIO_BASE_ADDR + 0x01C)
#define FIO1CLR                   SFR_IO32(FIO_BASE_ADDR + 0x03C)
#define FIO2CLR                   SFR_IO32(FIO_BASE_ADDR + 0x05C)
#define FIO3CLR                   SFR_IO32(FIO_BASE_ADDR + 0x07C)
#define FIO4CLR                   SFR_IO32(FIO_BASE_ADDR + 0x09C)
/* GPIO interrupt register map */
#define IO0IntEnR                 SFR_IO32(GPIO_BASE_ADDR + 0x090)
#define IO2IntEnR                 SFR_IO32(GPIO_BASE_ADDR + 0x0B0)
#define IO0IntEnF                 SFR_IO32(GPIO_BASE_ADDR + 0x094)
#define IO2IntEnF                 SFR_IO32(GPIO_BASE_ADDR + 0x0B4)
#define IO0StatR                  SFR_IO32(GPIO_BASE_ADDR + 0x084)
#define IO1StatR                  SFR_IO32(GPIO_BASE_ADDR + 0x0A4)
#define IO0StatF                  SFR_IO32(GPIO_BASE_ADDR + 0x088)
#define IO1StatF                  SFR_IO32(GPIO_BASE_ADDR + 0x0A8)
#define IO0Clr                    SFR_IO32(GPIO_BASE_ADDR + 0x08C)
#define IO1Clr                    SFR_IO32(GPIO_BASE_ADDR + 0x0AC)
#define IOIntStatus               SFR_IO32(GPIO_BASE_ADDR + 0x080)

/* Pin Connect Block */
#define PINSEL_BASE_ADDR          0xE002C000
#define PINSEL0                   SFR_IO32(PINSEL_BASE_ADDR + 0x000)
#define PINSEL1                   SFR_IO32(PINSEL_BASE_ADDR + 0x004)
#define PINSEL2                   SFR_IO32(PINSEL_BASE_ADDR + 0x008)
#define PINSEL3                   SFR_IO32(PINSEL_BASE_ADDR + 0x00C)
#define PINSEL4                   SFR_IO32(PINSEL_BASE_ADDR + 0x010)
#define PINSEL5                   SFR_IO32(PINSEL_BASE_ADDR + 0x014)
#define PINSEL6                   SFR_IO32(PINSEL_BASE_ADDR + 0x018)
#define PINSEL7                   SFR_IO32(PINSEL_BASE_ADDR + 0x01C)
#define PINSEL8                   SFR_IO32(PINSEL_BASE_ADDR + 0x020)
#define PINSEL9                   SFR_IO32(PINSEL_BASE_ADDR + 0x024)
#define PINSEL10                  SFR_IO32(PINSEL_BASE_ADDR + 0x028)
#define PINSEL11                  SFR_IO32(PINSEL_BASE_ADDR + 0x02C)
#define PINMODE0                  SFR_IO32(PINSEL_BASE_ADDR + 0x040)
#define PINMODE1                  SFR_IO32(PINSEL_BASE_ADDR + 0x044)
#define PINMODE2                  SFR_IO32(PINSEL_BASE_ADDR + 0x048)
#define PINMODE3                  SFR_IO32(PINSEL_BASE_ADDR + 0x04C)
#define PINMODE4                  SFR_IO32(PINSEL_BASE_ADDR + 0x050)
#define PINMODE5                  SFR_IO32(PINSEL_BASE_ADDR + 0x054)
#define PINMODE6                  SFR_IO32(PINSEL_BASE_ADDR + 0x058)
#define PINMODE7                  SFR_IO32(PINSEL_BASE_ADDR + 0x05C)
#define PINMODE8                  SFR_IO32(PINSEL_BASE_ADDR + 0x060)
#define PINMODE9                  SFR_IO32(PINSEL_BASE_ADDR + 0x064)

/* SSP1 */
#define SSP1_BASE_ADDR            0xE0030000

/* ADC */
#define ADC_BASE_ADDR             0xE0034000
#define AD0CR                     SFR_IO32(ADC_BASE_ADDR + 0x000)
#define AD0CR_SEL                 (0)
#define AD0CR_CLKDIV              (8)
#define AD0CR_BURST               (16)
#define AD0CR_CLKS                (17)
#define AD0CR_PDN                 (21)
#define AD0CR_START               (24)
#define AD0CR_EDGE                (27)
#define AD0GDR                    SFR_IO32(ADC_BASE_ADDR + 0x004)
#define AD0GDR_V_VREF             (6)
#define AD0GDR_CHN                (24)
#define AD0GDR_OVERRUN             (30)
#define AD0GDR_DONE               (31)
#define AD0STAT                   SFR_IO32(ADC_BASE_ADDR + 0x030)
#define AD0STAT_Done0             (0)
#define AD0STAT_Done1             (1)
#define AD0STAT_Done2             (2)
#define AD0STAT_Done3             (3)
#define AD0STAT_Done4             (4)
#define AD0STAT_Done5             (5)
#define AD0STAT_Done6             (6)
#define AD0STAT_Done7             (7)
#define AD0STAT_Overrun0          (8)
#define AD0STAT_Overrun1          (9)
#define AD0STAT_Overrun2          (10)
#define AD0STAT_Overrun3          (11)
#define AD0STAT_Overrun4          (12)
#define AD0STAT_Overrun5          (13)
#define AD0STAT_Overrun6          (14)
#define AD0STAT_Overrun7          (15)
#define ADINT                     (16)
#define AD0INTEN                  SFR_IO32(ADC_BASE_ADDR + 0x00C)
#define AD0INTEN_ADINTEN0         (0)
#define AD0INTEN_ADINTEN1         (1)
#define AD0INTEN_ADINTEN2         (2)
#define AD0INTEN_ADINTEN3         (3)
#define AD0INTEN_ADINTEN4         (4)
#define AD0INTEN_ADINTEN5         (5)
#define AD0INTEN_ADINTEN6         (6)
#define AD0INTEN_ADINTEN7         (7)
#define AD0INTEN_ADGINTEN         (8)
#define AD0DR0                    SFR_IO32(ADC_BASE_ADDR + 0x010)
#define AD0DR0_V_VREF             (6)
#define AD0DR0_OVERRUN            (30)
#define AD0DR0_DONE               (31)
#define AD0DR1                    SFR_IO32(ADC_BASE_ADDR + 0x014)
#define AD0DR1_V_VREF             (6)
#define AD0DR1_OVERRUN            (30)
#define AD0DR1_DONE               (31)
#define AD0DR2                    SFR_IO32(ADC_BASE_ADDR + 0x018)
#define AD0DR2_V_VREF             (6)
#define AD0DR2_OVERRUN            (30)
#define AD0DR2_DONE               (31)
#define AD0DR3                    SFR_IO32(ADC_BASE_ADDR + 0x01C)
#define AD0DR3_V_VREF             (6)
#define AD0DR3_OVERRUN            (30)
#define AD0DR3_DONE               (31)
#define AD0DR4                    SFR_IO32(ADC_BASE_ADDR + 0x020)
#define AD0DR4_V_VREF             (6)
#define AD0DR4_OVERRUN            (30)
#define AD0DR4_DONE               (31)
#define AD0DR5                    SFR_IO32(ADC_BASE_ADDR + 0x024)
#define AD0DR5_V_VREF             (6)
#define AD0DR5_OVERRUN            (30)
#define AD0DR5_DONE               (31)
#define AD0DR6                    SFR_IO32(ADC_BASE_ADDR + 0x028)
#define AD0DR6_V_VREF             (6)
#define AD0DR6_OVERRUN            (30)
#define AD0DR6_DONE               (31)
#define AD0DR7                    SFR_IO32(ADC_BASE_ADDR + 0x02C)
#define AD0DR7_V_VREF             (6)
#define AD0DR7_OVERRUN            (30)
#define AD0DR7_DONE               (31)

#define CAN_ACCEPT_RAM_BASE_ADDR  0xE0038000
#define CAN_ACCEPT_BASE_ADDR      0xE003C000
#define CAN_COMMON_BASE_ADDR      0xE0040000
#define CAN1_BASE_ADDR            0xE0044000
#define CAN2_BASE_ADDR            0xE0048000
#define I2C1_BASE_ADDR            0xE005C000

/* SSP0 */
#define SSP0_BASE_ADDR            0xE0068000
#define SSP0CR0                   SFR_IO32(SSP0_BASE_ADDR + 0x000)
#define SSP0CR0_DSS               (0)
#define SSP0CR0_FRF               (4)
#define SSP0CR0_CPOL              (6)
#define SSP0CR0_CPHA              (7)
#define SSP0CR0_SCR               (8)
#define SSP0CR1                   SFR_IO32(SSP0_BASE_ADDR + 0x004)
#define SSP0CR1_LBM               (0)
#define SSP0CR1_SSE               (1)
#define SSP0CR1_MS                (2)
#define SSP0CR1_SOD               (3)
#define SSP0DR                    SFR_IO32(SSP0_BASE_ADDR + 0x008)
#define SSP0SR                    SFR_IO32(SSP0_BASE_ADDR + 0x00C)
#define SSP0SR_TFE                (0)
#define SSP0SR_TNF                (1)
#define SSP0SR_RNE                (2)
#define SSP0SR_RFF                (3)
#define SSP0SR_BSY                (4)
#define SSP0CPSR                  SFR_IO32(SSP0_BASE_ADDR + 0x010)
#define SSP0IMSC                  SFR_IO32(SSP0_BASE_ADDR + 0x014)
#define SSP0IMSC_RORIM            (0)
#define SSP0IMSC_RTIM             (1)
#define SSP0IMSC_RXIM             (2)
#define SSP0IMSC_TXIM             (3)
#define SSP0RIS                   SFR_IO32(SSP0_BASE_ADDR + 0x018)
#define SSP0RIS_RORRIS            (0)
#define SSP0RIS_RTRIS             (1)
#define SSP0RIS_RXRIS             (2)
#define SSP0RIS_TXRIS             (3)
#define SSP0MIS                   SFR_IO32(SSP0_BASE_ADDR + 0x01C)
#define SSP0MIS_RORMIS            (0)
#define SSP0MIS_RTMIS             (1)
#define SSP0MIS_RXMIS             (2)
#define SSP0MIS_TXMIS             (3)
#define SSP0ICR                   SFR_IO32(SSP0_BASE_ADDR + 0x020)
#define SSP0ICR_RORIC             (0)
#define SSP0ICR_RTIC              (1)
#define SSP0DMACR                 SFR_IO32(SSP0_BASE_ADDR + 0x024)
#define SSP0DMACR_RXDMAE          (0)
#define SSP0DMACR_TXDMAE          (1)

#define DAC_BASE_ADDR             0xE006C000
#define TIMER2_BASE_ADDR          0xE0070000
#define TIMER3_BASE_ADDR          0xE0074000


#define I2C2_BASE_ADDR            0xE0080000
#define BAT_RAM_BASE_ADDR         0xE0084000
#define BAT_RAM_END_ADDR          0xE0087FFF
#define I2S_BASE_ADDR             0xE0088000

/* MMC/SD Card Controller */
#define MCI_BASE_ADDR             0xE008C000
#define MCIPower                  SFR_IO32(MCI_BASE_ADDR + 0x000)
#define MCIPower_Ctrl             (0)
#define MCIPower_OpenDrain        (6)
#define MCIPower_Rod              (7)
#define MCIClock                  SFR_IO32(MCI_BASE_ADDR + 0x004)
#define MCIClock_ClkDiv           (0)
#define MCIClock_Enable           (8)
#define MCIClock_PwrSave          (9)
#define MCIClock_Bypass           (10)
#define MCIClock_WideBus          (11)
#define MCIArgument               SFR_IO32(MCI_BASE_ADDR + 0x008)
#define MCICommand                SFR_IO32(MCI_BASE_ADDR + 0x00C)
#define MCICommand_CmdIndex       (0)
#define MCICommand_Response       (6)
#define MCICommand_LongRsp        (7)
#define MCICommand_Interrupt      (8)
#define MCICommand_Pending        (9)
#define MCICommand_Enable         (10)
#define MCIRespCmd                SFR_IO32(MCI_BASE_ADDR + 0x010)
#define MCIResponse0              SFR_IO32(MCI_BASE_ADDR + 0x014)
#define MCIResponse1              SFR_IO32(MCI_BASE_ADDR + 0x018)
#define MCIResponse2              SFR_IO32(MCI_BASE_ADDR + 0x01C)
#define MCIResponse3              SFR_IO32(MCI_BASE_ADDR + 0x020)
#define MCIDataTimer              SFR_IO32(MCI_BASE_ADDR + 0x024)
#define MCIDataLength             SFR_IO32(MCI_BASE_ADDR + 0x028)
#define MCIDataCtrl               SFR_IO32(MCI_BASE_ADDR + 0x02C)
#define MCIDataCtrl_Enable        (0)
#define MCIDataCtrl_Direction     (1)
#define MCIDataCtrl_Mode          (2)
#define MCIDataCtrl_DMAEnable     (3)
#define MCIDataCtrl_BlockSize     (4)
#define MCIDataCnt                SFR_IO32(MCI_BASE_ADDR + 0x030)
#define MCIStatus                 SFR_IO32(MCI_BASE_ADDR + 0x034)
#define MCIStatus_CmdCrcFail      (0)
#define MCIStatus_DataCrcFail     (1)
#define MCIStatus_CmdTimeOut      (2)
#define MCIStatus_DataTimeOut     (3)
#define MCIStatus_TxUnderrun      (4)
#define MCIStatus_RxOverrun       (5)
#define MCIStatus_CmdRespEnd      (6)
#define MCIStatus_CmdSent         (7)
#define MCIStatus_DataEnd         (8)
#define MCIStatus_StartBitErr     (9)
#define MCIStatus_DataBlockEnd    (10)
#define MCIStatus_CmdActive       (11)
#define MCIStatus_TxActive        (12)
#define MCIStatus_RxActive        (13)
#define MCIStatus_TxFifoHalfEmpty (14)
#define MCIStatus_RxFifoHalfFull  (15)
#define MCIStatus_TxFifoFull      (16)
#define MCIStatus_RxFifoFull      (17)
#define MCIStatus_TxFifoEmpty     (18)
#define MCIStatus_RxFifoEmpty     (19)
#define MCIStatus_TxDataAvlbl     (20)
#define MCIStatus_RxDataAvlbl     (21)
#define MCIClear                  SFR_IO32(MCI_BASE_ADDR + 0x038)
#define MCIMask0                  SFR_IO32(MCI_BASE_ADDR + 0x03C)
#define MCIFifoCnt                SFR_IO32(MCI_BASE_ADDR + 0x048)
#define MCI_FIFO_BASE_ADDR        SFR_IO32(MCI_BASE_ADDR + 0x080)
#define MCI_FIFO_END_ADDR         SFR_IO32(MCI_BASE_ADDR + 0x0BC)

/* System Control Block */
#define SCB_BASE_ADDR             0xE01FC000

/* Memory Accelerator Module */
#define MAMCR                     SFR_IO32(SCB_BASE_ADDR + 0x000)
#define MAMTIM                    SFR_IO32(SCB_BASE_ADDR + 0x004)

/* Memory mapping control */
#define MEMMAP                    SFR_IO32(SCB_BASE_ADDR + 0x040)

/* External interrupts */
#define EXTINT                    SFR_IO32(SCB_BASE_ADDR + 0x140)
#define EXTMODE                   SFR_IO32(SCB_BASE_ADDR + 0x148)
#define EXTPOLAR                  SFR_IO32(SCB_BASE_ADDR + 0x14C)

/* Reset */
#define RSID                      SFR_IO32(SCB_BASE_ADDR + 0x180)

/* AHB priority scheduling registers */
#define AHBCFG1                   SFR_IO32(SCB_BASE_ADDR + 0x188)
#define AHBCFG2                   SFR_IO32(SCB_BASE_ADDR + 0x18C)

/* Syscon miscellaneous registers */
#define SCS                       SFR_IO32(SCB_BASE_ADDR + 0x1A0)
#define SCS_GPIOM                 (0)
#define SCS_EMCResetDisable       (1)
#define SCS_EMCBurstControl       (2)
#define SCS_MCIPWRActiveLevel     (3)
#define SCS_OSCRANGE              (4)
#define SCS_OSCEN                 (5)
#define SCS_OSCSTAT               (6)

/* Clock source selection */
#define CLKSRCSEL                 SFR_IO32(SCB_BASE_ADDR + 0x10C)

/* Phase locked loop */
#define PLLCON                    SFR_IO32(SCB_BASE_ADDR + 0x080)
#define PLLCON_PLLE               (0)
#define PLLCON_PLLC               (1)
#define PLLCFG                    SFR_IO32(SCB_BASE_ADDR + 0x084)
#define PLLCFG_MSEL               (0)
#define PLLCFG_NSEL               (16)
#define PLLSTAT                   SFR_IO32(SCB_BASE_ADDR + 0x088)
#define PLLSTAT_MSEL              (0)
#define PLLSTAT_NSEL              (16)
#define PLLSTAT_PLLE              (24)
#define PLLSTAT_PLLC              (25)
#define PLLSTAT_PLOCK             (26)
#define PLLFEED                   SFR_IO32(SCB_BASE_ADDR + 0x08C)

/* Clock dividers */
#define CCLKCFG                   SFR_IO32(SCB_BASE_ADDR + 0x104)
#define USBCLKCFG                 SFR_IO32(SCB_BASE_ADDR + 0x108)
#define IRCTRIM                   SFR_IO32(SCB_BASE_ADDR + 0x1A4)
#define PCLKSEL0                  SFR_IO32(SCB_BASE_ADDR + 0x1A8)
#define	PCLK_WDT		(0)
#define	PCLK_TIMER0		(2)
#define	PCLK_TIMER1		(4)
#define	PCLK_UART0		(6)
#define	PCLK_UART1		(8)
#define PCLK_SPI		(16)

#define PCLKSEL1                  SFR_IO32(SCB_BASE_ADDR + 0x1AC)
#define PCLK_UART2		(16)
#define PCLK_UART3		(18)

/* Power control */
#define PCON                      SFR_IO32(SCB_BASE_ADDR + 0x0C0)
#define INTWAKE                   SFR_IO32(SCB_BASE_ADDR + 0x144)
#define PCONP                     SFR_IO32(SCB_BASE_ADDR + 0x0C4)
#define PCONP_PCTIM0              (1)
#define PCONP_PCTIM1              (2)
#define PCONP_PCUART0             (3)
#define PCONP_PCUART1             (4)
#define PCONP_PCPWM0              (5)
#define PCONP_PCPWM1              (6)
#define PCONP_PCI2C0              (7)
#define PCONP_PCSPI               (8)
#define PCONP_PCRTC               (9)
#define PCONP_PCSSP1              (10)
#define PCONP_PCEMC               (11)
#define PCONP_PCAD                (12)
#define PCONP_PCCAN1              (13)
#define PCONP_PCCAN2              (14)
#define PCONP_PCI2C1              (19)
#define PCONP_PCLCD               (20)
#define PCONP_PCSSP0              (21)
#define PCONP_PCTIM2              (22)
#define PCONP_PCTIM3              (23)
#define PCONP_PCUART2             (24)
#define PCONP_PCUART3             (25)
#define PCONP_PCI2C2              (26)
#define PCONP_PCI2S               (27)
#define PCONP_PCSDC               (28)
#define PCONP_PCGPDMA             (29)
#define PCONP_PCENET              (30)
#define PCONP_PCUSB               (31)

/* External Memory Controller */
#define EMC_BASE_ADDR             0xFFE08000
#define EMCControl                SFR_IO32(EMC_BASE_ADDR + 0x000)
#define EMCStatus                 SFR_IO32(EMC_BASE_ADDR + 0x004)
#define EMCConfig                 SFR_IO32(EMC_BASE_ADDR + 0x008)
#define EMCDynamicControl         SFR_IO32(EMC_BASE_ADDR + 0x020)
#define EMCDynamicRefresh         SFR_IO32(EMC_BASE_ADDR + 0x024)
#define EMCDynamicReadConfig      SFR_IO32(EMC_BASE_ADDR + 0x028)
#define EMCDynamicRP              SFR_IO32(EMC_BASE_ADDR + 0x030)
#define EMCDynamicRAS             SFR_IO32(EMC_BASE_ADDR + 0x034)
#define EMCDynamicSREX            SFR_IO32(EMC_BASE_ADDR + 0x038)
#define EMCDynamicAPR             SFR_IO32(EMC_BASE_ADDR + 0x03C)
#define EMCDynamicDAL             SFR_IO32(EMC_BASE_ADDR + 0x040)
#define EMCDynamicWR              SFR_IO32(EMC_BASE_ADDR + 0x044)
#define EMCDynamicRC              SFR_IO32(EMC_BASE_ADDR + 0x048)
#define EMCDynamicRFC             SFR_IO32(EMC_BASE_ADDR + 0x04C)
#define EMCDynamicXSR             SFR_IO32(EMC_BASE_ADDR + 0x050)
#define EMCDynamicRRD             SFR_IO32(EMC_BASE_ADDR + 0x054)
#define EMCDynamicMRD             SFR_IO32(EMC_BASE_ADDR + 0x058)
#define EMCStaticExtendedWait     SFR_IO32(EMC_BASE_ADDR + 0x080)
#define EMCDynamicConfig0         SFR_IO32(EMC_BASE_ADDR + 0x100)
#define EMCDynamicRasCas0         SFR_IO32(EMC_BASE_ADDR + 0x104)
#define EMCDynamicConfig1         SFR_IO32(EMC_BASE_ADDR + 0x120)
#define EMCDynamicRasCas1         SFR_IO32(EMC_BASE_ADDR + 0x124)
#define EMCDynamicConfig2         SFR_IO32(EMC_BASE_ADDR + 0x140)
#define EMCDynamicRasCas2         SFR_IO32(EMC_BASE_ADDR + 0x144)
#define EMCDynamicConfig3         SFR_IO32(EMC_BASE_ADDR + 0x160)
#define EMCDynamicRasCas3         SFR_IO32(EMC_BASE_ADDR + 0x164)
#define EMCStaticConfig0          SFR_IO32(EMC_BASE_ADDR + 0x200)
#define EMCStaticWaitWen0         SFR_IO32(EMC_BASE_ADDR + 0x204)
#define EMCStaticWaitOen0         SFR_IO32(EMC_BASE_ADDR + 0x208)
#define EMCStaticWaitRd0          SFR_IO32(EMC_BASE_ADDR + 0x20C)
#define EMCStaticWaitPage0        SFR_IO32(EMC_BASE_ADDR + 0x210)
#define EMCStaticWaitWr0          SFR_IO32(EMC_BASE_ADDR + 0x214)
#define EMCStaticWaitTurn0        SFR_IO32(EMC_BASE_ADDR + 0x218)
#define EMCStaticConfig1          SFR_IO32(EMC_BASE_ADDR + 0x220)
#define EMCStaticWaitWen1         SFR_IO32(EMC_BASE_ADDR + 0x224)
#define EMCStaticWaitOen1         SFR_IO32(EMC_BASE_ADDR + 0x228)
#define EMCStaticWaitRd1          SFR_IO32(EMC_BASE_ADDR + 0x22C)
#define EMCStaticWaitPage1        SFR_IO32(EMC_BASE_ADDR + 0x230)
#define EMCStaticWaitWr1          SFR_IO32(EMC_BASE_ADDR + 0x234)
#define EMCStaticWaitTurn1        SFR_IO32(EMC_BASE_ADDR + 0x238)
#define EMCStaticConfig2          SFR_IO32(EMC_BASE_ADDR + 0x240)
#define EMCStaticWaitWen2         SFR_IO32(EMC_BASE_ADDR + 0x244)
#define EMCStaticWaitOen2         SFR_IO32(EMC_BASE_ADDR + 0x248)
#define EMCStaticWaitRd2          SFR_IO32(EMC_BASE_ADDR + 0x24C)
#define EMCStaticWaitPage2        SFR_IO32(EMC_BASE_ADDR + 0x250)
#define EMCStaticWaitWr2          SFR_IO32(EMC_BASE_ADDR + 0x254)
#define EMCStaticWaitTurn2        SFR_IO32(EMC_BASE_ADDR + 0x258)
#define EMCStaticConfig3          SFR_IO32(EMC_BASE_ADDR + 0x260)
#define EMCStaticWaitWen3         SFR_IO32(EMC_BASE_ADDR + 0x264)
#define EMCStaticWaitOen3         SFR_IO32(EMC_BASE_ADDR + 0x268)
#define EMCStaticWaitRd3          SFR_IO32(EMC_BASE_ADDR + 0x26C)
#define EMCStaticWaitPage3        SFR_IO32(EMC_BASE_ADDR + 0x270)
#define EMCStaticWaitWr3          SFR_IO32(EMC_BASE_ADDR + 0x274)
#define EMCStaticWaitTurn3        SFR_IO32(EMC_BASE_ADDR + 0x278)

/* Vectored Interrupt Controller */
#define VIC_BASE_ADDR             0xFFFFF000
#define VICIRQStatus              SFR_IO32(VIC_BASE_ADDR + 0x000)
#define VICFIQStatus              SFR_IO32(VIC_BASE_ADDR + 0x004)
#define VICRawIntr                SFR_IO32(VIC_BASE_ADDR + 0x008)
#define VICIntSelect              SFR_IO32(VIC_BASE_ADDR + 0x00C)
#define VICIntEnable              SFR_IO32(VIC_BASE_ADDR + 0x010)
#define VICIntEnClr               SFR_IO32(VIC_BASE_ADDR + 0x014)
#define VICSoftInt                SFR_IO32(VIC_BASE_ADDR + 0x018)
#define VICSoftIntClear           SFR_IO32(VIC_BASE_ADDR + 0x01C)
#define VICProtection             SFR_IO32(VIC_BASE_ADDR + 0x020)
#define VICSWPriorityMask         SFR_IO32(VIC_BASE_ADDR + 0x024)
#define VIC_ADDR_BASE_ADDR        (VIC_BASE_ADDR + 0x100)
#define VICVectAddr0              SFR_IO32(VIC_BASE_ADDR + 0x100)
#define VICVectAddr1              SFR_IO32(VIC_BASE_ADDR + 0x104)
#define VICVectAddr2              SFR_IO32(VIC_BASE_ADDR + 0x108)
#define VICVectAddr3              SFR_IO32(VIC_BASE_ADDR + 0x10C)
#define VICVectAddr4              SFR_IO32(VIC_BASE_ADDR + 0x110)
#define VICVectAddr5              SFR_IO32(VIC_BASE_ADDR + 0x114)
#define VICVectAddr6              SFR_IO32(VIC_BASE_ADDR + 0x118)
#define VICVectAddr7              SFR_IO32(VIC_BASE_ADDR + 0x11C)
#define VICVectAddr8              SFR_IO32(VIC_BASE_ADDR + 0x120)
#define VICVectAddr9              SFR_IO32(VIC_BASE_ADDR + 0x124)
#define VICVectAddr10             SFR_IO32(VIC_BASE_ADDR + 0x128)
#define VICVectAddr11             SFR_IO32(VIC_BASE_ADDR + 0x12C)
#define VICVectAddr12             SFR_IO32(VIC_BASE_ADDR + 0x130)
#define VICVectAddr13             SFR_IO32(VIC_BASE_ADDR + 0x134)
#define VICVectAddr14             SFR_IO32(VIC_BASE_ADDR + 0x138)
#define VICVectAddr15             SFR_IO32(VIC_BASE_ADDR + 0x13C)
#define VICVectAddr16             SFR_IO32(VIC_BASE_ADDR + 0x140)
#define VICVectAddr17             SFR_IO32(VIC_BASE_ADDR + 0x144)
#define VICVectAddr18             SFR_IO32(VIC_BASE_ADDR + 0x148)
#define VICVectAddr19             SFR_IO32(VIC_BASE_ADDR + 0x14C)
#define VICVectAddr20             SFR_IO32(VIC_BASE_ADDR + 0x150)
#define VICVectAddr21             SFR_IO32(VIC_BASE_ADDR + 0x154)
#define VICVectAddr22             SFR_IO32(VIC_BASE_ADDR + 0x158)
#define VICVectAddr23             SFR_IO32(VIC_BASE_ADDR + 0x15C)
#define VICVectAddr24             SFR_IO32(VIC_BASE_ADDR + 0x160)
#define VICVectAddr25             SFR_IO32(VIC_BASE_ADDR + 0x164)
#define VICVectAddr26             SFR_IO32(VIC_BASE_ADDR + 0x168)
#define VICVectAddr27             SFR_IO32(VIC_BASE_ADDR + 0x16C)
#define VICVectAddr28             SFR_IO32(VIC_BASE_ADDR + 0x170)
#define VICVectAddr29             SFR_IO32(VIC_BASE_ADDR + 0x174)
#define VICVectAddr30             SFR_IO32(VIC_BASE_ADDR + 0x178)
#define VICVectAddr31             SFR_IO32(VIC_BASE_ADDR + 0x17C)
#define VIC_PRIOR_BASE_ADDR       (VIC_BASE_ADDR + 0x200)
#define VICVectPriority0          SFR_IO32(VIC_BASE_ADDR + 0x200)
#define VICVectPriority1          SFR_IO32(VIC_BASE_ADDR + 0x204)
#define VICVectPriority2          SFR_IO32(VIC_BASE_ADDR + 0x208)
#define VICVectPriority3          SFR_IO32(VIC_BASE_ADDR + 0x20C)
#define VICVectPriority4          SFR_IO32(VIC_BASE_ADDR + 0x210)
#define VICVectPriority5          SFR_IO32(VIC_BASE_ADDR + 0x214)
#define VICVectPriority6          SFR_IO32(VIC_BASE_ADDR + 0x218)
#define VICVectPriority7          SFR_IO32(VIC_BASE_ADDR + 0x21C)
#define VICVectPriority8          SFR_IO32(VIC_BASE_ADDR + 0x220)
#define VICVectPriority9          SFR_IO32(VIC_BASE_ADDR + 0x224)
#define VICVectPriority10         SFR_IO32(VIC_BASE_ADDR + 0x228)
#define VICVectPriority11         SFR_IO32(VIC_BASE_ADDR + 0x22C)
#define VICVectPriority12         SFR_IO32(VIC_BASE_ADDR + 0x230)
#define VICVectPriority13         SFR_IO32(VIC_BASE_ADDR + 0x234)
#define VICVectPriority14         SFR_IO32(VIC_BASE_ADDR + 0x238)
#define VICVectPriority15         SFR_IO32(VIC_BASE_ADDR + 0x23C)
#define VICVectPriority16         SFR_IO32(VIC_BASE_ADDR + 0x240)
#define VICVectPriority17         SFR_IO32(VIC_BASE_ADDR + 0x244)
#define VICVectPriority18         SFR_IO32(VIC_BASE_ADDR + 0x248)
#define VICVectPriority19         SFR_IO32(VIC_BASE_ADDR + 0x24C)
#define VICVectPriority20         SFR_IO32(VIC_BASE_ADDR + 0x250)
#define VICVectPriority21         SFR_IO32(VIC_BASE_ADDR + 0x254)
#define VICVectPriority22         SFR_IO32(VIC_BASE_ADDR + 0x258)
#define VICVectPriority23         SFR_IO32(VIC_BASE_ADDR + 0x25C)
#define VICVectPriority24         SFR_IO32(VIC_BASE_ADDR + 0x260)
#define VICVectPriority25         SFR_IO32(VIC_BASE_ADDR + 0x264)
#define VICVectPriority26         SFR_IO32(VIC_BASE_ADDR + 0x268)
#define VICVectPriority27         SFR_IO32(VIC_BASE_ADDR + 0x26C)
#define VICVectPriority28         SFR_IO32(VIC_BASE_ADDR + 0x270)
#define VICVectPriority29         SFR_IO32(VIC_BASE_ADDR + 0x274)
#define VICVectPriority30         SFR_IO32(VIC_BASE_ADDR + 0x278)
#define VICVectPriority31         SFR_IO32(VIC_BASE_ADDR + 0x27C)
#define VICAddress                SFR_IO32(VIC_BASE_ADDR + 0xF00)

/* Ethernet */
#define ETH_BASE_ADDR        0xFFE00000
// MAC registers
#define ETH_MAC1                  SFR_IO32(ETH_BASE_ADDR + 0x000)
#define ETH_MAC2
#define ETH_IPGT
#define ETH_IPGR
#define ETH_CLRT
#define ETH_MAXF
#define ETH_SUPP
#define ETH_TEST
#define ETH_MCFG
#define ETH_MCMD
#define ETH_MADR
#define ETH_MWTD
#define ETH_MRDD
#define ETH_MIND
#define ETH_SA0
#define ETH_SA1
#define ETH_SA2
// Control registers
#define ETH_Command
#define ETH_Status
#define ETH_RxDescriptor
#define ETH_RxStatus
#define ETH_RxDescriptorNumber
#define ETH_RxProduceIndex
#define ETH_RxConsumeIndex
#define ETH_TxDescriptor
#define ETH_TxStatus
#define ETH_TxDescriptorNumber
#define ETH_TxProduceIndex
#define ETH_TxConsumeIndex
#define ETH_TSV0
#define ETH_TSV1
#define ETH_RSV
#define ETH_FlowControlCounter
#define ETH_FlowControlStatus
// Rx filter registers
#define ETH_RxFilterCtrl
#define ETH_RxFilterWoLStatus
#define ETH_RxFilterWoLClear
#define ETH_HashFilterL
#define ETH_HashFilterH
// Module control registers
#define ETH_IntStatus
#define ETH_IntEnable
#define ETH_Clear
#define ETH_IntSet
#define ETH_PowerDown

/* LCD controller */
#define LCD_BASE_ADDR             0xFFE10000
#define LCD_CFG                   SFR_IO32(0xE01FC1B8)
#define LCD_CFG_CLKDIV            (0)
#define LCD_TIMH                  SFR_IO32(LCD_BASE_ADDR + 0x000)
#define LCD_TIMH_HBP              (24)
#define LCD_TIMH_HFP              (16)
#define LCD_TIMH_HSW              (8)
#define LCD_TIMH_PPL              (2)
#define LCD_TIMV                  SFR_IO32(LCD_BASE_ADDR + 0x004)
#define LCD_TIMV_VBP              (24)
#define LCD_TIMV_VFP              (16)
#define LCD_TIMV_VSW              (10)
#define LCD_TIMV_LPP              (0)
#define LCD_POL                   SFR_IO32(LCD_BASE_ADDR + 0x008)
#define LCD_POL_PCD_HI            (27)
#define LCD_POL_BCD               (26)
#define LCD_POL_CPL               (16)
#define LCD_POL_IOE               (14)
#define LCD_POL_IPC               (13)
#define LCD_POL_IHS               (12)
#define LCD_POL_IVS               (11)
#define LCD_POL_ACB               (6)
#define LCD_POL_CLKSEL            (5)
#define LCD_POL_PCD_LO            (0)
#define LCD_LE                    SFR_IO32(LCD_BASE_ADDR + 0x00C)
#define LCD_LE_LEE                (16)
#define LCD_LE_LED                (0)
#define LCD_UPBASE                SFR_IO32(LCD_BASE_ADDR + 0x010)
#define LCD_UPBASE_LCDUPBASE      (3)
#define LCD_LPBASE                SFR_IO32(LCD_BASE_ADDR + 0x014)
#define LCD_LPBASE_LCDLPBASE      (3)
#define LCD_CTRL                  SFR_IO32(LCD_BASE_ADDR + 0x018)
#define LCD_CTRL_WATERMARK        (16)
#define LCD_CTRL_LcdVComp         (12)
#define LCD_CTRL_LcdPwr           (11)
#define LCD_CTRL_BEPO             (10)
#define LCD_CTRL_BEBO             (9)
#define LCD_CTRL_BGR              (8)
#define LCD_CTRL_LcdDual          (7)
#define LCD_CTRL_LcdMono8         (6)
#define LCD_CTRL_LcdTFT           (5)
#define LCD_CTRL_LcdBW            (4)
#define LCD_CTRL_LcdBpp           (1)
#define LCD_CTRL_LcdEn            (0)
#define LCD_INTMSK                SFR_IO32(LCD_BASE_ADDR + 0x01C)
#define LCD_INTMSK_VCompIM		  (3)
#define LCD_INTRAW                SFR_IO32(LCD_BASE_ADDR + 0x020)
#define LCD_INTSTAT               SFR_IO32(LCD_BASE_ADDR + 0x024)
#define LCD_INTCLR                SFR_IO32(LCD_BASE_ADDR + 0x028)
#define LCD_INTCLR_VCompIC		  (3)
#define LCD_UPCURR                SFR_IO32(LCD_BASE_ADDR + 0x02C)
#define LCD_LPCURR                SFR_IO32(LCD_BASE_ADDR + 0x030)
#define LCD_PAL_BASE_ADDR         SFR_IO32(LCD_BASE_ADDR + 0x200)
#define CRSR_IMG_BASE_ADDR        SFR_IO32(LCD_BASE_ADDR + 0x800)
#define CRSR_CTRL                 SFR_IO32(LCD_BASE_ADDR + 0xC00)
#define CRSR_CFG                  SFR_IO32(LCD_BASE_ADDR + 0xC04)
#define CRSR_PAL0                 SFR_IO32(LCD_BASE_ADDR + 0xC08)
#define CRSR_PAL1                 SFR_IO32(LCD_BASE_ADDR + 0xC0C)
#define CRSR_XY                   SFR_IO32(LCD_BASE_ADDR + 0xC10)
#define CRSR_CLIP                 SFR_IO32(LCD_BASE_ADDR + 0xC14)
#define CRSR_INTMSK               SFR_IO32(LCD_BASE_ADDR + 0xC20)
#define CRSR_INTCLR               SFR_IO32(LCD_BASE_ADDR + 0xC24)
#define CRSR_INTRAW               SFR_IO32(LCD_BASE_ADDR + 0xC28)
#define CRSR_INTSTAT              SFR_IO32(LCD_BASE_ADDR + 0xC2C)

// Watch dog
#define WDMOD                     SFR_IO32(0xE0000000)

#endif // LPC2478_H

