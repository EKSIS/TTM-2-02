#include  <msp430.h>
#include  "msp430_comm.h"
#include  "short_types.h"
#include  "usb_desc.h"
#include  "hid.h"
#include  "usb_msp.h"
#include  "sd.h"
#include  "limits.h"
#include  "math.h"
#include  "stdio.h"
#include  "stdlib.h"
#include  "stdarg.h"
#include  "icclbutl.h"

// typedef
typedef struct
{
  Int8U UsbEvent:         1;
  Int8U UsbConnected:     1;
  Int8U UsbDataReady:     1;
  Int8U UsbConfigured:    1;
  Int8U UsbSendZeroReq:   1;
  Int8U BootStartReq:     1;  
}
UsbFlags_t;

typedef struct 
{
  pInt8U        pBuffer;
  Int16U        lenght;
} ep_struct_t;

typedef struct 
{ 
   char    HCSD:        1;
   char    Version:     1;
   char    data_sector: 1;
   char    sd_inserted: 1;
   char    sd_valid:    1;
   char    sd_initialized: 1;
}  sd_flags_struct;

typedef struct 
{ 
   char    up;
   char    *strings_p;
}  list_sruct;

typedef struct 
{ 
   Int16U    timeout:       1;
   Int16U    systick:       1;   
   Int16U    start_boot:    1;
   Int16U    DefMemWrite:   1;
   Int16U    lcd_update:    1;
   Int16U    measure_ready: 1;
   Int16U    calcDate:      1;
   Int16U    show_power:    1;
   Int16U    PressGPa:      1;
   Int16U    key:           1;
   Int16U    measureEn:     1;
} Flags_t;

#define USB_BUF_LEN           128
#define UART_BUF_LEN          64
#define USB_WDATA_LEN         64
#define USB_RDATA_LEN         69
#define SPI_BUF_LEN           16
#define RAMSIZE               256
#define INFOENDADR            0x19FF
#define RamBeginAddr          (Int16U)&Tempr

#define SERVICE_ADR           0xFFFF
#define ADR_RESET_VECT        0xFFFE
#define TRUE_RESET_VECT       0x4400

#define SEG_SIZE              0x0200
#define SEG_A                 0x1980
#define SEG_B                 0x1900
#define SEG_C                 0x1880
#define SEG_D                 0x1800

// keys definition
#define NONE                  0
#define SW1                   1
#define SW2                   2
#define SW3                   3
#define LONG_SW1              5
#define LONG_SW2              6
#define LONG_SW3              7
#define LONGKEY_F             4 

// config's bits
#define NEW_PR                BIT9
#define MK2                   BITA
#define PRES                  BITB
#define RECHARG               BITC
#define RESET_STAT            BITD
#define SOUND_ON              BITE
#define WRITE_ON              BITF

// error bits
#define SD_ERROR              BIT0
#define TS_ERROR              BIT1
#define HS_ERROR              BIT2
#define PW_ERROR              BIT3
#define T1_ERROR              BIT4
#define T2_ERROR              BIT5
#define H1_ERROR              BIT6
#define H2_ERROR              BIT7
#define STAT_OVR              BIT8
#define PRESS_ERROR           BIT9
#define INFOMEM_ERROR         BITA
  /* резерв */

// hum units
#define H1_MODE               0
#define H2_MODE               1
#define H3_MODE               2
#define H4_MODE               3
#define H5_MODE               4

// lcd modes
#define OFF_MODE              0
#define START_MODE            1
#define TH_SHOW               2
#define PH_SHOW               3
#define SD_SHOW               4

// measure modes
#define WAITING               0
#define START_PRES            1
#define START_RES             2
#define START_MES             3

// time
#define SEC(A)                  (U16)(1024*(A))

// TLV
#define ADC_GAIN                *((pU16)0x1A16)
#define ADC_OFFSET              *((pS16)0x1A18)
#define REF_15CAL               *((pU16)0x1A28)
#define REF_20CAL               *((pU16)0x1A2A)
#define REF_25CAL               *((pU16)0x1A2C)

// LCD
#define LCD_SIZE                10

// other
#define MAX_ADC_VAL             4095
#define POLY_LEN                6

// macroses
#define POW_LCD_OFF             P1OUT &= ~BIT0
#define POW_LCD_ON              P1OUT |=  BIT0
#define CS_LCD_LOW              P4OUT &= ~BIT7
#define CS_LCD_HIGH             P4OUT |=  BIT7
#define LCD_CLK                 { P4OUT |= BIT0; P4OUT |= BIT0; P4OUT &= ~BIT0; P4OUT &= ~BIT0; }
#define LCD_ONE                 P4OUT |=  BIT4
#define LCD_ZERO                P4OUT &= ~BIT4
#define LCD_off                 { LCD_send_com(0x00); CS_LCD_LOW; POW_LCD_OFF; }

#define LIGHT_BAT3              HT1621_Buffer[14] |= 0x01
#define LIGHT_BAT2              HT1621_Buffer[13] |= 0x01
#define LIGHT_BAT1              HT1621_Buffer[13] |= 0x10
#define LIGHT_BAT0              HT1621_Buffer[11] |= 0x10

#define LIGHT_oC_UP             HT1621_Buffer[10] |= 0x01
#define LIGHT_oC_LO             HT1621_Buffer[0]  |= 0x80
#define LIGHT_SD                HT1621_Buffer[13] |= 0x02
#define LIGHT_USB               HT1621_Buffer[12] |= 0x10
#define LIGHT_RF                HT1621_Buffer[12] |= 0x01
#define LIGHT_k_UP              HT1621_Buffer[11] |= 0x20
#define LIGHT_Kdm2              HT1621_Buffer[11] |= 0x01
#define LIGHT_mmptst            HT1621_Buffer[10] |= 0x10
#define LIGHT_gPa               HT1621_Buffer[9]  |= 0x10
#define LIGHT_ms                HT1621_Buffer[8]  |= 0x10
#define LIGHT_k_LO              HT1621_Buffer[8]  |= 0x20
#define LIGHT_Lk                HT1621_Buffer[10] |= 0x02
#define LIGHT_pr                HT1621_Buffer[0]  |= 0x10
#define LIGHT_ppm               HT1621_Buffer[0]  |= 0x01
#define LIGHT_m                 HT1621_Buffer[0]  |= 0x04
#define LIGHT_gm3               HT1621_Buffer[0]  |= 0x02
#define LIGHT_tr                HT1621_Buffer[0]  |= 0x08

#define LIGHT_BP_UP             HT1621_Buffer[14] |= 0x10
#define LIGHT_HP_UP             HT1621_Buffer[15] |= 0x08
#define LIGHT_BP_LO             HT1621_Buffer[15] |= 0x02
#define LIGHT_HP_LO             HT1621_Buffer[15] |= 0x10


#define WDRCLR                  WDTCTL  = WDTPW + WDTSSEL__VLO + WDTCNTCL + WDTIS__32K
#define SOFT_RESET              PMMCTL0 = PMMPW + PMMSWBOR

#define WriteFlashChar(adr,b)   WriteFlashString((Int16U)(adr),(pInt8U)&(b),sizeof(Int8U))
#define WriteFlashInt(adr,i)    WriteFlashString((Int16U)(adr),(pInt8U)&(i),sizeof(Int16U))
#define WriteFlashFloat(adr,f)  WriteFlashString((Int16U)(adr),(pInt8U)&(f),sizeof(Flo32))

#define USB_INT_DISABLE         { USBPWRCTL &= ~(VBOFFIE + VBONIE); USBIEPIE= USBOEPIE= USBIE= 0; }
#define USB_INT_ENABLE          { USBPWRCTL |=  (VBOFFIE + VBONIE); USBIEPIE= USBOEPIE= BIT0; USBIE= SETUPIE + RSTRIE; }

#define SHIFT_BYTE_LCD(A,B)     { UCA1TXBUF= (A); while(UCA1STAT & UCBUSY); (B)= UCA1RXBUF; }

#define SHIFT_BYTE_SD(A,B)      { UCB1TXBUF= (A); while(UCB1STAT & UCBUSY); (B)= UCB1RXBUF; }
#define SET_SD_SPI_300K         UCB1BRW=    7
#define SET_SD_SPI_1M           UCB1BRW=    2

#define SOUND_OFF               TA1CCTL1=  OUTMOD_0 + OUT
#define SOUND_ON1               { TA1CCR0= 49152; TA1CCR1= 9830; TA1CCTL1= OUTMOD_3; } 
#define SOUND_ON2               { TA1CCR0= 16384; TA1CCR1= 8192; TA1CCTL1= OUTMOD_3; } 
#define SOUND_ON3               { TA1CCR0= 16384; TA1CCR1= 4096; TA1CCTL1= OUTMOD_3; } 

#define POWER_ON                P2OUT |=  BIT4
#define POWER_OFF               P2OUT &= ~BIT4
#define BAT_ON                  P6SEL &= ~(BIT3 + BIT4)
#define PRES_ON                 P2OUT |=  (BIT1 + BIT2)
#define PRES_OFF                P2OUT &= ~(BIT1 + BIT2)
#define HS_ON                   P3OUT |=  (BIT6 + BIT7)
#define HS_OFF                  P3OUT |= ~(BIT6 + BIT7)
#define TS_ON                   P3OUT |=  (BIT4 + BIT5)
#define TS_OFF                  P3OUT &= ~(BIT4 + BIT5)
#define ALL_SENS_OFF            { P3OUT &= 0x0f; P2OUT &= ~(BIT1+BIT2); P6SEL |= (BIT3+BIT4); }

#define WAIT_UNPRESS            KeyClock= 50

#define XT2_STOP                { UCSCTL6 |= XT2OFF; P5SEL &= ~(BIT2 + BIT3); }
#define XT2_CHECK               (UCSCTL7 & XT2LFOFFG)

#pragma segment= "DEFCONF"
#pragma segment= "INFO"
#pragma segment= "INFOA"

#include "global_declaration.h"
