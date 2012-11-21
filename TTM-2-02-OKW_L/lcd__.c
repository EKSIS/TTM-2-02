#include  "define.h"

// ext var
extern const unsigned char  NV[NV_LEN];

extern float                velocity,tempr,v_bat,stat_arr[128],r_vs,nts_r,zero,rth_vs,calib_arr[64][2], min_max_val, k_filter;
extern unsigned int         config_word;
extern unsigned char        selector, errors, pow_lev, lcd_clock, manual_store, show_point, measure_clock, last_mode;
extern flags_struct         flags;

void  lcd_process(void)
{        
    unsigned char        a;
    
    if(v_bat >= 2.1)  pow_lev= (v_bat - 2.1)/(3.2 - 2.1) * 100 + .5;
    else              
    { 
      pow_lev = 0;     
      if(selector != OFF_MODE)
      {
        selector= BATFAIL_MODE;
        TURN_OFF;
      }
    }
    
    CLR_F; CLR_DEG; CLR_ERR;
    
    CHECK_FLOAT(velocity);
    CHECK_FLOAT(tempr);
    
    switch(selector)
    {                                         
       case  START_MODE: if(lcd_clock > SEC(5)) { printf("%s",NV); break; };
                         printf("  %2d",lcd_clock>>3);
                         if(lcd_clock <= SEC(1))
                         {
                           if(fabs(r_vs-nts_r)>(0.1*nts_r))
                           {
                              TURN_OFF;
                              errors |=   Z_ERROR;               
                              lcd_clock=  0;
                              selector=   ERROR_MODE;
                           }
                           else errors&= ~Z_ERROR;
                         }
                         
                         if(!lcd_clock) selector= VEL_MODE; 
                         break;

       case  ERROR_MODE:
                         printf("Er-S"); 
                         break; 
                        
       case  VEL_MODE:   if(lcd_clock) 
                         {
                           printf("Ar-0");
                           break;
                         }
                         min_max_val= velocity;   
                                               
vel_show:                         
                         if(min_max_val > 9.995) a=  1;
                         else                    a=  2;
                         LIGHT_F;
                         if(errors & VR_ERROR)  { LIGHT_ERR; printf("----"); }
                         else printf("%#5.*f",a,min_max_val);                         
                         break;

        case  DEG_MODE:                         
                         LIGHT_DEG;
                         if(errors & TR_ERROR)  { LIGHT_ERR; printf("----"); }
                         else printf("%#5.1f", tempr);
                         break;

       case  BAT_MODE:   printf("P%3d", pow_lev);
                         if(!lcd_clock) selector= OFF_MODE;
                         break;
                         
       case  WRITE_MODE: if(manual_store == 128) printf("FULL");
                         else                    printf("A%03d", manual_store);
                         if(!lcd_clock)          selector= last_mode; 
                         break;
                         
       case  POINT_MODE: if(!manual_store) printf("A---");
                         else printf("A%03d", show_point);
                         if(!lcd_clock) selector= OFF_MODE;
                         break;
       
       case  SHOW_MODE:  if(velocity>9.995) a= 1;
                         else               a= 2;
                         printf("%#5.*f",a,stat_arr[show_point-1]); LIGHT_F;
                         if(!lcd_clock) 
                         { 
                            selector= POINT_MODE; 
                            lcd_clock= SEC(3); 
                         }
                         break;
                                                  
       case BATFAIL_MODE: 
                         printf("Er-P");
                         break;
       
       case  COM_MODE:   switch(config_word & 3)
                         {
                            case 0: printf("1200"); break;
                            case 1: printf("2400"); break;
                            case 2: printf("4800"); break;
                            case 3: printf("9600"); break;
                         }
                         if(!lcd_clock) selector= OFF_MODE;
                         break;
                          
       case  OFF_MODE:   printf("    ");
                         break;
                         
       case    MAX_MODE: if(lcd_clock) 
                         {
                           printf(" uP ");
                           break;
                         }                         
                         if(min_max_val < velocity) min_max_val= velocity;
                         goto vel_show;
                         
       case    MIN_MODE: if(lcd_clock) 
                         {
                           printf(" Lo ");
                           break;
                         }       
                         if(min_max_val > velocity) min_max_val= velocity;
                         goto vel_show;
                         
       case    AV1_MODE:
       case    AV2_MODE: if(lcd_clock) 
                         {
                           if(selector == AV1_MODE) printf("Ar-1");
                           else                     printf("Ar-2");
                           break;
                         }
                         
                         min_max_val+= k_filter * (velocity - min_max_val);
                         goto vel_show;

    }
    // about POWER- seg
    if(pow_lev < 10)                FLASH_POWER;
    else if(selector==OFF_MODE)     LIGHT_POWER;
    else                            CLR_POWER;
}
