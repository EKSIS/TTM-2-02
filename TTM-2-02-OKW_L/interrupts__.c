#include  "define.h"

// extern var 
extern flags_struct         flags;
extern float                v_bat;
extern unsigned int         ta_pwm0,mem_0,mem_3,mem_4;
extern unsigned char        uart_buffer[MAX_RX_TX_LEN],tx_pointer,key_code,selector;
extern unsigned char        rx_counter,adc_clock,measure_clock, lcd_clock;

// extern func
extern void                 modus(int mod, int offset);
extern void                 choose_uart_speed(void);
extern void                 choose_uart_speed(void);

// basic timer 1 ISR
interrupt [BASICTIMER_VECTOR] void bt_isr(void)
{    
    static unsigned char  sys_clock= SEC(0.5);
    static unsigned char  key_clock,last_key= NONE;
        
    switch((~P3IN) & 0xf)
    {
       case  BIT0: if(last_key==SW4) key_clock++;
                     else key_clock= 0;
                   last_key= SW4;
                   break;
                   
       case  BIT1: if(last_key==SW1) key_clock++;
                     else key_clock= 0;            
                   last_key= SW1;
                   break;
                   
       case  BIT2: if(last_key==SW2) key_clock++;
                     else key_clock= 0;
                   last_key= SW2;
                   break;
                   
       case  BIT3: if(last_key==SW3) key_clock++;
                     else key_clock= 0;
                   last_key= SW3;
                   break;
     
       default:    if(flags.skey) { flags.key= 1; flags.skey= 0; key_code= last_key; modus(0,2*6); }
                   last_key= NONE;
                   key_clock= 0;
    };
    
    if(!flags.key)
    {
        if(key_clock==KEY_TIME1) { flags.skey= 1; }
        if(key_clock==KEY_TIME2) { key_code= last_key + LONG_MASK; flags.key= 1; flags.skey= 0; modus(0,2*6); }
    };
    
    if(lcd_clock) lcd_clock--;
                
    if(!sys_clock--)
    {
      modus(0,2*6);           // wakeup main loop to clear wdt (0.5s)
      sys_clock= measure_clock;                                        
      flags.systick= 1;
    };
    
    // check for USB
    if(P3IN & BIT7) 
    {
      P2SEL = BIT4 + BIT5;
      if(selector == BATFAIL_MODE) selector = OFF_MODE;
      v_bat = 3.0;
    }
    else            P2SEL = 0;
}

interrupt [TIMERB1_VECTOR] void tb1_isr(void)
{        
    TBCTL&= ~TBIFG;
    ADC12CTL0|=  (ENC + ADC12SC);
}

interrupt [TIMERA1_VECTOR] void ta1_isr(void)
{        
    TACTL&= ~(TAIFG+TAIE);
    TACCR1= ta_pwm0;
}

interrupt [ADC_VECTOR] void adc_isr(void)
{
    static unsigned char    t_clock= 19;
    
    ADC12CTL0&= ~ENC;
    ADC12IFG=    0x00;
    
    if(t_clock==1) T_ON; 
    if(!t_clock--)
    {
      t_clock= 19;
      T_OFF;
      flags.t_ready= 1;
      mem_0= ADC12MEM0-ADC12MEM5;
      mem_3= ADC12MEM3;
      mem_4= ADC12MEM4-ADC12MEM5;
    };
    modus(0,2*6);
    flags.measure_ready= 1;
}

// RXI
interrupt [UART0RX_VECTOR] void uart_rx_isr(void)
{  
     unsigned char            sum, num;
     
     num= U0RCTL;
     sum= U0RXBUF;
     
     if(num & (OE + FE)) { rx_counter=0;  return; };
     if(sum=='$')  rx_counter=0;
      else if(rx_counter==0 )  return;
     
     if(rx_counter==sizeof(uart_buffer)) { rx_counter= 0; return; }
     if(sum != 0x0d) { uart_buffer[rx_counter]= sum; rx_counter++; return; }
     flags.recieve= 1;
     modus(0,2*6); 
     RXI_DISABLE;
}

//UART Transmitter buffer                         *************************
interrupt [UART0TX_VECTOR] void uart_tx_isr(void)
{ 
     void   (*reset_func)(void);
     
     if(!flags.firstsent) { IDLE_TX; flags.firstsent= 1; return; }
     if(tx_pointer && (uart_buffer[tx_pointer-1]==0x0d))
      { 
        if(flags.lastsent) 
            { 
              TXI_DISABLE; flags.lastsent= 0; flags.firstsent= 0;
              if(flags.respeeduart) { flags.respeeduart= 0; /* choose_uart_speed(); */ }
              if(flags.start_boot)  { _BIC_SR(SCG0); reset_func= (void (*)(void))(*((unsigned int *) RESET_VECT)); reset_func(); }
              RXI_ENABLE;
              return;
            }
          else { flags.lastsent= 1; IDLE_TX; return; }
      } 
     U0TXBUF= uart_buffer[tx_pointer];
     tx_pointer++;
}