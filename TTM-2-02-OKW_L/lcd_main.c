#include  "define.h"

void Prog_LCD_load_data(void)
{
    // ��������� ����������� USB
//    if(USBFlags.UsbConfigured)  LIGHT_USB;
    if(USBFlags.UsbConnected)   LIGHT_USB;
//    else {Minutes = 0; Seconds = -1;};

    // ��������� ������ �������
//    switch(PowLevel)
//    {
//      case 3: LIGHT_BAT3;
//      case 2: LIGHT_BAT2;
//      case 1: LIGHT_BAT1;
//    }
//    LIGHT_BAT0;
    
    
    // �������� ���������� �������
    LCD_load_data();
}

void  lcd_process(void)
{
      
    F32        w, d;
    U8         p;
        
      Flags.lcd_update= 0;
    if(Selector == OFF_MODE) return;
    if(Errors & INFOMEM_ERROR)
    {
      printf(" FAIL MEM");
      if (Seconds%2) LIGHT_BAT0;
      else printf(" ");
      LCD_load_data();
      return;
    }
    
    w = hum_re_calc( &p);
    if(w > 19999.4)          w =  19999.;
    else if (w < -19999.4)   w = -19999.;
    
    switch(p)
    {
      case 3: if((w < 19.9994) && (w > -19.9994)) break;
              p--;
            
      case 2: if((w < 199.994) && (w > -199.994))  break;
              p--;
            
      case 1: if((w < 1999.94) && (w > -1999.94))  break;
              p--;
    }
        
    switch(Selector)
    {
    case  TV_SHOW: switch(Errors & (TR_ERROR + VR_ERROR))
                   {
                    case TR_ERROR:            printf(" Err %#6.*f", velocity);    break;
                    case VR_ERROR:            printf("%#6.1f Err ", temperature);   break;
                    case (TR_ERROR+VR_ERROR): printf(" Err  Err ");           break;
                    
                    default:                  printf("%#6.1f%#6.*f", temperature, velocity);
                   }
                   LIGHT_oC_UP;
                   
                   // ������ �� �����������
                   if(Errors & T1_ERROR) LIGHT_HP_UP;
                   if(Errors & T2_ERROR) LIGHT_BP_UP;                   
//                   goto sel_w_seg;
            
//    case  PH_SHOW: d = Pressure;
//                   if(Flags.PressGPa) d *= 1.3332;  // � ���
//      
//                   switch(Errors & (PRESS_ERROR + HS_ERROR))
//                   { 
//                    case  HS_ERROR:                 printf("%#6.1f Err ", d);     break;
//                    case  PRESS_ERROR:              printf(" Err %#6.*f", p, w);  break;
//                    case (PRESS_ERROR + HS_ERROR):  printf(" Err  Err ");         break;
//                        
//                    default:                        printf("%#6.1f%#6.*f", d, p, w);
//                   } 
//                   
//                   if(Flags.PressGPa) LIGHT_gPa;
//                   else               LIGHT_mmptst;
//sel_w_seg:                   
//                   switch(HumidyUnits)
//                   {
//                    case H1_MODE: LIGHT_pr;              break;
//                    case H2_MODE: LIGHT_oC_LO; LIGHT_tr; break;
//                    case H3_MODE: LIGHT_oC_LO; LIGHT_m;  break;
//                    case H4_MODE: LIGHT_gm3;             break;
//                    case H5_MODE: LIGHT_ppm;             break;
//                   }
//                   
                   // ������ �� ��������
                   if(Errors & V1_ERROR) LIGHT_HP_LO;
                   if(Errors & V2_ERROR) LIGHT_BP_LO;
//                   break;
//                   
//    case  SD_SHOW:                    
//                   break;
                                     
    case  START_MODE:                   
                   printf(" %s P%3d", Version, PowerShow); LIGHT_pr;
                   if(GenClock1 < 3) Flags.measureEn = 1;
                   if(GenClock1)     break;
                   Selector = TV_SHOW;
                   break;
                    
    default:       Selector= TV_SHOW;                  
    }
    
    // �������� ���������� ������� +
    Prog_LCD_load_data( );
}
