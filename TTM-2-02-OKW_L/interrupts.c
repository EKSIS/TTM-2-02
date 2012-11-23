#include  "define.h"

#pragma vector=ADC12_VECTOR
__interrupt void adc_isr(void)
{   
  ALL_SENS_OFF;
  ADC12CTL0 &= ~ADC12ENC;
  ADC12CTL0 &= ~(ADC12ON + ADC12REFON);
  ADC12IFG   =  0;
  Flags.measure_ready= 1;
  __low_power_mode_off_on_exit();
}

#pragma vector=TIMERB1_VECTOR
__interrupt void tb_isr(void)
{          
  static U16   rtc;  
  static U8    last_key = NONE;
  static U8    state = WAITING;
  
  TBCTL &= ~TBIFG;
  
  // sd таймер
  if(SD_Clock > (TBCCR0 + 1)) SD_Clock -= (TBCCR0 + 1);
  else                        SD_Clock  = 0;
  
  // RTC 
  rtc += (TBCCR0 + 1);
  if(rtc >= 1024)
  {
    rtc -= 1024;
    if(Time2Write)    Time2Write--;
    if(GenClock1)     GenClock1--;
    Flags.systick=    1;
    __low_power_mode_off_on_exit();                  // для сброса WDT и подсчета времени
  }

  switch( ~(P2IN & (BIT5+BIT6+BIT7)) & 0xe0)
  {
     case  BIT5: if(last_key == SW1) KeyClock++;
                 else KeyClock = 0; 
                 last_key       = SW1; 
                 break;
                 
     case  BIT6: if(last_key == SW2) KeyClock++;
                 else KeyClock = 0; 
                 last_key       = SW2;
                 break;
                 
     case  BIT7: if(last_key == SW3) KeyClock++;
                 else KeyClock = 0; 
                 last_key       = SW3; 
                 break;                 
                 
     default:    last_key  = NONE; 
                 KeyClock = 0; 
  }
  
  if(KeyClock == 3) 
  {
    Flags.key = 1; 
    KeyCode   = last_key; 
    __low_power_mode_off_on_exit(); 
  }

  if(KeyClock == 15) 
  { 
    Flags.key = 1; 
    KeyCode   = last_key + LONGKEY_F; 
    KeyClock = 13; 
    __low_power_mode_off_on_exit(); 
  }
    
  switch(state)
  {
    case WAITING:     if(!(Flags.systick && Flags.measureEn)) 
                      {
                        TBCCR0  = 49; 
                        break;
                      }

                      HS_ON;
                      if(ConfigWord & PRES)
                      {
                        TBCCR0  = 19;
                        state   = START_PRES;
                      }
                      else
                      {
                        TBCCR0  = 39;
                        state   = START_RES;
                      }
                      break;

    case START_PRES:  PRES_ON;
                      state   = START_RES;
                      TBCCR0  = 19; 
                      break;
                        
    case START_RES:   TS_ON;
                      state   = START_MES;
                      TBCCR0  = 9;
                      break;
               
    case START_MES:   ADC12CTL0 |= (ADC12REFON + ADC12ON);  // ADC12 ON
                      state      = WAITING;
                      //BAT_ON;
                      TBCCR0     = 49;
                      ADC12CTL0 |= (ADC12ENC + ADC12SC);    // Start Convertion 
                      
                      break;
  }    
}

// USB
#pragma vector = USB_UBM_VECTOR
__interrupt void usb_isr(void)
{
  USBFlags.UsbEvent= 1; 
  USB_INT_DISABLE; 
  __low_power_mode_off_on_exit(); 
}

// софт календарь
void  date_time(void)
{  
  if(Seconds++ == 59)
  {
    Seconds= 0;
    if(Minutes++ == 59)
    {
       Minutes= 0;
       if(Houres++ == 23)
       {
          Houres= 0;
          if(Days++ == MonthSize[Monthes-1])
          {
             Days= 1;
             if(Monthes++ == 12)
             {
                Monthes= 1;
                if(Years++ == 99) Years= 0;
             }
          }
       }
    }
  }
}
