#include  "define.h"

//
extern float          velocity,tempr,v_bat,r_term,rth_vs,r_vs;
extern unsigned int   config_word, pressure;
extern unsigned char  manual_store, errors, measure_clock;
extern flags_struct   flags;

void ram_ini(void)
{    
  TURN_OFF;
  velocity= 0;
  tempr=    25;
  v_bat=    3.;
  rth_vs=   0;
  r_vs=     2000;
  r_term=   1.07;
  pressure= 745;
  manual_store= 0;
  errors= 0;
}

/*
void  choose_uart_speed(void)
{
    switch(config_word & SPEED_MASK)
    {
      case    S1200:    U0BR1= 0; U0BR0= 0x1B; U0MCTL= 0x03; break;
      case    S2400:    U0BR1= 0; U0BR0= 0x0D; U0MCTL= 0x6B; break;
      case    S4800:    U0BR1= 0; U0BR0= 0x06; U0MCTL= 0x6F; break;
      case    S9600:    U0BR1= 0; U0BR0= 0x03; U0MCTL= 0x4A; break;
    };
}
*/
void  system_ini(void)
{
  _DINT();
    
  // SFR
  
  IE1=    0x00;
  IE2=    BTIE;
  
  IFG1=   0x00;
  IFG2=   0x00;
  
  ME1=    URXE0 + UTXE0;
  ME2=    0x00;
  
  // Ports init  
  P1SEL=  BIT2;
  P1OUT=  0x00;
  P1IE=   0x00;  
  P1DIR=  BIT2+BIT3+BIT4+BIT5+BIT6+BIT7;
  CAPD=   0;
  
  P2SEL=  0;
  P2OUT=  0;
  P2IE=   0;
  P2DIR=  BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT6 + BIT7;
    
  P3SEL= P3OUT=  0;
  P3DIR=  0x70;
  
  P4SEL=  0x00;
  P4OUT=  0x00;
  P4DIR=  0xff;
  
  P5SEL=  0x00;
  P5OUT=  0x00;
  P5DIR=  0xff;
  
  P6SEL=  0xff;
  P6OUT=  0x00;
  P6DIR=  0x00;
  
  // SVS
  SVSCTL= 0x00;
  
  // Comp_A  
  CACTL1= CACTL2= 0;
  
  // FLL+/**/
  SCFQCTL=    SCFQ_4M;              // MCLK= 4MHz
  SCFI0=      0;        
  FLL_CTL0=   XCAP18PF;
  FLL_CTL1=   XT2OFF + SELM_DCO;
  
  // WDT -on timeout 1s.
  WDT_CLR;
  
  // Basic Timer1 используем как источник прерываний по таймеру 15.6 мс для опроса клавы и проч.
  // CLK2= ACLK/256 interrupt t= 0.0156s
  BTCNT1= BTCNT2=   0x00;
  BTCTL=      BTFRFQ1 + BTFRFQ0 + BT_fCLK2_ACLK_DIV256 + BT_fCLK2_DIV8;
  
  // Timer A - clk = SMCLK используем как ШИМ контроллер
  TACTL=      TASSEL_2 + ID_0 + TACLR;
  TACCTL0=    0x0000;
  TACCTL1=    0x0000;
  TACCR0=     0x0400;
  TACCR1=     0;
  
  // Timer B - clk = SMCLK используем как источник прерываний по таймеру 5 мс для запуска АЦП
  TBCTL=      TBSSEL_2 + TBCLR + ID_0 + TBIE;
  TBCCTL0=    0x0000;
  TBCCTL1=    0x0000;
  TBCCTL2=    0x0000;
  TBCCR0=     20971;
  
  // LCD
  LCDCTL=     LCDON + LCDSON + LCDP0 + LCDP1 + LCDP2;
  
  // USART
//  choose_uart_speed();
  
  // 115200 bit/s
  U0BR1=      0; 
  U0BR0=      36; 
  U0MCTL=     0x4A;
  U0CTL=      CHAR + SWRST;
  U0TCTL=     SSEL1;                // SMCLK
  U0RCTL=     0;
  U0CTL=      CHAR;
  IE1=        URXIE0;
  
  // FLASH  
  FCTL2=      FWKEY + FSSEL_1 + 9;  // 4M/10 ~ 470kHz
  FCTL3=      FWKEY + LOCK;
  
  // ADC12
  ADC12CTL0=  ADC12CTL0 & ~(ENC);
  ADC12CTL0=  0;
  ADC12CTL1=  CSTARTADD_0 + SHP + CONSEQ_1;
  
  ADC12MCTL0= SREF_1 + INCH_0;  
  ADC12MCTL1= SREF_1 + INCH_1;
  ADC12MCTL2= SREF_1 + INCH_2;
  ADC12MCTL3= SREF_0 + INCH_3;
  ADC12MCTL4= SREF_1 + INCH_4;
  ADC12MCTL5= SREF_1 + INCH_9 + EOS;
  
  ADC12IE=     BIT5;
} 