#include  "define.h"

void  xt2_start(void)
{
  if( !(UCSCTL7 & XT2OFFG) && !(UCSCTL6 & XT2OFF) ) return;  // ���� ��� �������� - return

  // ������
  P5SEL   |=  BIT2 + BIT3;
  UCSCTL6 &=  ~XT2OFF;
  
  // ���� ��������� XT2
  while(UCSCTL7 & XT2OFFG) UCSCTL7 &= ~XT2OFFG;
  SFRIFG1 &= ~OFIFG;
}

void  ini(void)
{
  __disable_interrupt(); 
  
  SFRIE1= SFRIFG1= SYSCTL= 0;         // ���������� ACCVIE(���������� flash),SFRIFG1 - �������� ������, SYSCTL - ������ BSL
  SFRRPCR=  BIT2 + BIT3;              // �������� �������� ���
  RCCTL0=   RCKEY ;                   // ������� �� ���
  
  // WDT
  WDTCTL=   WDTPW + WDTSSEL__VLO + WDTCNTCL + WDTIS__32K;    // ~ 2.4c
  
  // PMM
  PMMCTL0=  PMMPW;                    // Vcore = 1,4 �
  SVSMHCTL= SVSHE + SVSHRVL0;         // ��� ������ ���������� �� ����. ����� 2 �
  SVSMLCTL= SVSLE;                    // ��� ������ ���������� �� ����. 1,4 �
  PMMRIE=   SVSHPE + SVSLPE;          // ��� ����� �� ������������ H � L
  
  // Ports init  
 
  // �������� ���� 4
  PMAPKEYID=    PMAPKEY;                //������ ������
  PMAPCTL=      0;                      //Configuration allowed only once//Access to mapping registers is granted
  P4MAP0=       PM_UCA1CLK;
  P4MAP4=       PM_UCA1SIMO;
  PMAPKEYID=    0;
  
  P1SEL=        BIT2;
  P1OUT= P1IE= P1DS= P1REN= 0;
  P1DIR=        255;
  
  P2SEL=        BIT0;
  P2IE= P2DS=   0; 
  P2DIR=        0x1f;  
  P2REN= P2OUT= 0xe0; 
  
  P3SEL=  P3REN= P3OUT= 0; 
  P3DIR=        0xff; 

  P4SEL=        BIT0 + BIT4;
  P4DS=         0;
  P4REN=        0;
  P4DIR=        255;
  P4OUT=        BIT7; //��� 0
    
  P5SEL=        BIT0 + BIT4 + BIT5;    // ���� XT1
  P5OUT=  P5DS= P5REN= 0; 
  P5DIR=        255;
  
  P6SEL=        255;
  P6DS=   P6REN= 0;
  P6DIR=        255; 
  P6OUT=        0;
  
  P7SEL=        BIT0 + BIT1;
  P7OUT=  P7DS= P7REN= 0;
  P7DIR=        255;
  
  P8SEL=  P8OUT= P8DS= P8REN= 0;
  P8DIR=        255;
  
  PJOUT=  PJDS= PJREN= 0;
  PJDIR=        255;  
  
  // set UCS
  UCSCTL6=  XT2OFF + XT2DRIVE0 + XCAP0 + XCAP1;
  
  // ���� ��������� XT1
  while(UCSCTL7 & XT1LFOFFG) UCSCTL7 &= ~XT1LFOFFG; //���� ��������� �� ���, �� 1 ��������������� �����
  
  UCSCTL1=      DCORSEL_1;                                        // 0.5...3.5 MHz
  UCSCTL2=      63;                                               // 32,768*(63+1) ~ 2MHz �� ������ FLL
  UCSCTL3=      FLLREFDIV_0 + SELREF__XT1CLK;                     // 32kHz �� ���� FLL

  UCSCTL5=      DIVA__32;                                         // ACLK = XT1/32 = 1024 Hz
  UCSCTL8=      0;                                                // ��������� ������������ �� ����������

  // ���� ��������� DCO
  while(UCSCTL7 & DCOFFG) UCSCTL7 &= ~DCOFFG; 
  SFRIFG1 &= ~OFIFG; 
  
  UCSCTL4=    SELM__DCOCLKDIV + SELS__DCOCLKDIV + SELA__XT1CLK;    // MCLK - FLL(2MHz), SMCLK - FLL(2MHz), ACLK - XT1/32(1024Hz)

  // TA0
  TA1CTL=     TASSEL_1 + MC__UP + TACLR;//ACLK, Up mode
  TA1CCTL1=   OUTMOD_0 + OUT;  // The output signal OUTn is defined by the OUT bit. The OUTn signal updates immediately when OUT is updated.
  TA1CCR0=    500;
  
  // TB inclk = ACLK (1024Hz)
  TBCTL=      TBCLGRP_0 + CNTL_0 + TBSSEL_1 + ID_0 + MC_1 + TBCLR + TBIE;  // clk= ACLK ���������� ������������� �� �������
  TBCCTL0=    TBCCTL1= TBCCTL2= TBCCTL3= TBCCTL4= TBCCTL5= 0;
  TBCCR0=     49; // ������ ����� 0,05�
  
  // USCA1 - spi
  UCA1CTL1=   UCSSEL__SMCLK + UCSWRST;  // SMCLK (2MHz)
  UCA1CTL0=   UCSYNC + UCMST + UCCKPH;
  UCA1STAT=   0;
  UCA1BRW=    14;                       // bitrate ~ 150 kbit
  UCA1MCTL=   0;
  UCA1CTL1=   UCSSEL__SMCLK; 
  
      
  // COMP_B
  CBCTL0= CBCTL2= CBCTL3= CBINT= 0;
  
  // USB  
  USBKEYPID=        USBKEY;
  USBCNF=           0;                          // ���� USB
  USBPHYCTL=        PUSEL;                      // ��� DP � DM 
  USBVECINT=        0;
  if(CHECK_USB_POWER_BOOT) 
  {
    USBPWRCTL  = VUSBEN + USBDETEN + SLDOAON;   // ��� �������������
    USBPWRCTL |= VBONIFG;                       // ��������� ����������
  }
  else  USBPWRCTL=        USBDETEN + SLDOAON;   // ���� ������������� � ��� ����������� ������� �� VBUS 
  USB_INT_ENABLE; 
  
  // ADC12 + REF
  REFCTL0    =  0;
  ADC12CTL0 &= ~ADC12ENC;
  ADC12CTL0  =  ADC12SHT0_1 + ADC12SHT1_1 + ADC12MSC;                                    // 8 ������ �����, ������������� ��������������
  ADC12CTL1  =  ADC12CSTARTADD_0 + ADC12SHP + ADC12CONSEQ_1 + ADC12DIV_2 + ADC12SSEL_0;  // � �0, ���� ������, ���� ������������������, ADC12OSC / 3 ~ 1.8 MHz
  ADC12CTL2  =  ADC12RES_2 + ADC12REFBURST;                                              //  ���������� 12-���, ������ ����� �� ����������

  ADC12MCTL0=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL1=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL2=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL3=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL4=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL5=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL6=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL7=  ADC12SREF_2 + ADC12INCH_0;   // �����������
  ADC12MCTL8=  ADC12SREF_1 + ADC12INCH_13;  // �������
  ADC12MCTL9=  ADC12SREF_1 + ADC12INCH_12;  // �������� 
  ADC12MCTL10= ADC12SREF_1 + ADC12INCH_1;   // ���������
  ADC12MCTL11= ADC12SREF_1 + ADC12INCH_1;   // ���������
  ADC12MCTL12= ADC12SREF_1 + ADC12INCH_1;   // ���������
  ADC12MCTL13= ADC12SREF_1 + ADC12INCH_1;   // ���������  
  ADC12MCTL14= ADC12SREF_1 + ADC12INCH_1;   // ���������
  ADC12MCTL15= ADC12SREF_1 + ADC12INCH_1 + ADC12EOS;   // ���������  
  ADC12IFG   = 0;
  ADC12IE    = BITF;
  
  // ������������� ���������� �� �������� RAM  
  Tempr = Humidy=  25;
  Power =   3.0;
  Res= PressureParam = HumidyParam = Errors= 0.;
  
  if(WriteAdr > 0x243E0) WriteAdr = 0;
  if((Seconds > 59) || (Minutes > 59) || (Houres > 23)  || (Days > 31) || (Monthes > 12) || (Years < 12) || (Years > 99) )
  {
    Seconds= Minutes= Houres= 0;
    Days= Monthes= 4;
    Years = 12;
  }
    
  AdcGain =     ADC_GAIN / 32768.;
  Ref15 =       1.5 * REF_15CAL / 32768. / 4095.;
}
