#include  "define.h"

void main(void)
{
  ini();
  
  
  POWER_OFF;
  __enable_interrupt();
  LCD_off;
  LCD_on();
  
  
//    for(U8  a= 0;; WDRCLR)
      for(;;)
    {
      WDRCLR;
//      a=  Flags.measure_ready || USBFlags.UsbEvent || Flags.systick || Flags.key;
//      if(!a)    
//      {
//        if(USBFlags.UsbConnected) LPM0;
//        else                      LPM3;
//      }
//  
//      if(Flags.key)               key_process();    
//      if(USBFlags.UsbEvent)       usb_control(); 
//      if(Flags.measure_ready)     measure_process();
//  
      if(Flags.systick)            
      {
        Flags.lcd_update = 1;
        Flags.systick    = 0;
//        date_time();
//  
//        if(!Time2Write)           statistic_process();            
//        CheckInfoFlash();
//        tresholds_process();
//        sd_process();
      }
            if(Flags.lcd_update)        lcd_process();
      
      
    } // for
    
}
