#include  "define.h"

void main(void)
{
  ini();
  
  Selector == OFF_MODE;
  POWER_OFF;
  __enable_interrupt();

  
    for(U8  a= 0;; WDRCLR)
    {
      a=  Flags.measure_ready || USBFlags.UsbEvent || Flags.systick || Flags.key;
      if(!a)    
      {
        if(USBFlags.UsbConnected) LPM0;
        else                      LPM4;
      }
  
      if(Flags.key)               key_process();    
      if(USBFlags.UsbEvent)       usb_control(); 
      if(Flags.measure_ready)     measure_process();
//  
      if(Flags.systick)            
      {
        Flags.lcd_update = 1;
        Flags.systick    = 0;
        date_time();
       
        CheckInfoFlash();
        tresholds_process();
      }
            if(Flags.lcd_update)        lcd_process();
      
    } // for
    
}
