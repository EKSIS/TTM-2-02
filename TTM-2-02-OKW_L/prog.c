#include  "define.h"

#define   NO_KEY          127
#define   ABORT           126
#define   ENTR            0

// predeclaration
void  set_date              (void);
S8    key                   (U8 wait);
U8    show_list             (list_sruct list, pS8 p);
S8    set_int               (pS8 input, S8 down_range, S8 up_range);
S8    set_float             (F32 *input, F32 down_range, F32 up_range, F32 delta);

const list_sruct   menu_main=       { 5, "      ПOPГ\0      BPEM\0      3AПC\0      KAPТ\0       ПИТ\0      Bb1X" };
const U8  s1[]= " On ", s2[]= " OFF", s3[]= " ACC", s4[]= " BAt", s5[]= " CБP", s6[]= "ПPOд";

#pragma optimize=medium
void  prog_serv(void)
{
  F32       f;
  U16       d;
  pU8       time;
  U8        low, up;
  U8        mn, sc, hr;
  S8        k, m= 0;
  U8        const *s;
  
  for(;;)
  {
     k= show_list(menu_main, &m);
     if((k == NO_KEY) || (k == ABORT)) return;     
     switch(m)
     {
     case  0:   // T1
                f = TrshVal[0];                
                for(;;)
                {
                  printf("%#6.1f     ", f);
                  LIGHT_oC_UP; LIGHT_HP_UP; 
                  Prog_LCD_load_data();
                  k = set_float( &f, -20., 60., 0.1);
                  
                  if(k == NO_KEY) return;
                  if(k == ENTR)   
                  {
                    WriteFlashString((U32)&TrshVal[0], (pU8)&f, 4);
                    break;
                  }
                }
                // T2
                f = TrshVal[1];
                for(;;)
                {
                  printf("%#6.1f     ", f);
                  LIGHT_oC_UP; LIGHT_BP_UP; 
                  Prog_LCD_load_data();
                  k = set_float( &f, -20., 60., 0.1);
                  
                  if(k == NO_KEY) return;
                  if(k == ENTR)   
                  {
                    WriteFlashString((U32)&TrshVal[1], (pU8)&f, 4);
                    break;
                  }
                }
                // H1
                f = TrshVal[2];                
                for(;;)
                {
                  printf("     %#6.1f", f);
                  LIGHT_pr; LIGHT_HP_LO; 
                  Prog_LCD_load_data();
                  k = set_float( &f, 0., 100., 0.1);
                  
                  if(k == NO_KEY) return;
                  if(k == ENTR)   
                  {
                    WriteFlashString((U32)&TrshVal[2], (pU8)&f, 4);
                    break;
                  }
                }
                // T2
                f = TrshVal[3];
                for(;;)
                {
                  printf("     %#6.1f", f);
                  LIGHT_pr; LIGHT_BP_LO; 
                  Prog_LCD_load_data();
                  k = set_float( &f, 0., 100., 0.1);
                  
                  if(k == NO_KEY) return;
                  if(k == ENTR)   
                  {
                    WriteFlashString((U32)&TrshVal[3], (pU8)&f, 4);
                    break;
                  }
                }
                // звук                                
                d = (ConfigWord & SOUND_ON);
                for(;;)
                {
                  if(d) s = s1;
                  else  s = s2;
                  printf(" %s 3BУK", s);
                  Prog_LCD_load_data();
                  k = key(30);
                  if(k == NO_KEY) return;
                  if(k == ENTR)
                  {
                    d += (ConfigWord & ~SOUND_ON);
                    WriteFlashString((U32)&ConfigWord, (pU8)&d, 2);
                    break;
                  }
                  d ^= SOUND_ON;
                }
                break; 
                
       case  1:  for(U8 i= 0; i < 4; i++)
                 {
st:                printf(" %02d.%02d %02d.%02d", Houres, Minutes, Days, Monthes);
                   Prog_LCD_load_data();
                   switch(i)
                   {
                    case 1: low = 0; up = 59; time = &Minutes; break;
                    case 0: low = 0; up = 23; time = &Houres; break;
                    case 2: low = 1; up = 31; time = &Days; break;
                    case 3: low = 1; up = 12; time = &Monthes; break;
                   }
                   k = set_int( time, low, up);
                   if(k == NO_KEY)  return;
                   if(k == ENTR)    continue;
                   else             goto st;
                 }
                 for(;;)
                 {
                  printf("      20%02d", Years);
                  Prog_LCD_load_data();
                  k = set_int( &Years, 0, 99);
                  if(k == NO_KEY)  return;
                  if(k == ENTR)    break;
                 }
                 break;          
                 
       case  2:  // запись вкл
                 d = (ConfigWord & WRITE_ON);
                 for(;;)
                 {
                  if(d) s = s1;
                  else  s = s2;
                  printf(" %s 3AПC", s);
                  Prog_LCD_load_data();
                  k = key(30);
                  if(k == NO_KEY) return;
                  if(k == ENTR)
                  {
                    d += (ConfigWord & ~WRITE_ON);
                    WriteFlashString((U32)&ConfigWord, (pU8)&d, 2);
                    break;
                  }
                  d ^= WRITE_ON;
                 }
                 if(!(ConfigWord & WRITE_ON)) break;
                 
                 hr =  WritePeriod/3600;
                 mn = (WritePeriod - hr * 3600) / 60;
                 sc =  WritePeriod - hr * 3600 - mn * 60;
                 
                 for(U8 i= 0; i < 3; i++)
                 {
st1:               printf(" %02d.%02d   %2d", mn, sc, hr);
                   Prog_LCD_load_data();
                   switch(i)
                   {
                    case 0: low = 0; up = 59; time = (pU8)&mn; break;
                    case 1: low = 0; up = 59; time = (pU8)&sc; break;
                    case 2: low = 0; up = 17; time = (pU8)&hr; break;
                   }
                   k = set_int( time, low, up);
                   if(k == NO_KEY)  return;
                   if(k == ENTR)    continue;
                   else             goto st1;
                 }
                 if( ((hr + mn) == 0) && (sc < 10) ) sc = 10;
                 d = hr * 3600 + mn * 60 + sc;
                 WriteFlashString((U32)&WritePeriod, (pU8)&d, 2);                 
                 break;

       case  3:  // при вставке карты сбрасывать статистику или продолжать
                 d = (ConfigWord & RESET_STAT);
                 for(;;)
                 {
                  if(d) s = s5;
                  else  s = s6;
                  printf(" %s KAPТ", s);
                  Prog_LCD_load_data();
                  k = key(30);
                  if(k == NO_KEY) return;
                  if(k == ENTR)
                  {
                    d += (ConfigWord & ~RESET_STAT);
                    WriteFlashString((U32)&ConfigWord, (pU8)&d, 2);
                    break;
                  }
                  d ^= RESET_STAT;
                 }
                 break;                 

       case  4:  // питание от аккумулятора или батарейки
                 d = (ConfigWord & RECHARG);
                 for(;;)
                 {
                  if(d) s = s3;
                  else  s = s4;
                  printf(" %s  ПИТ", s);
                  Prog_LCD_load_data();
                  k = key(30);
                  if(k == NO_KEY) return;
                  if(k == ENTR)
                  {
                    d += (ConfigWord & ~RECHARG);
                    WriteFlashString((U32)&ConfigWord, (pU8)&d, 2);
                    break;
                  }
                  d ^= RECHARG;
                 }
                 break;
                 
      case  5:   return; 
     }                      
  } // for(;;)
}

S8 set_float(F32 *input, F32 down_range, F32 up_range, F32 delta)
{   
  S8  k = key(30);
  
  if(k == NO_KEY)  return k;
  *input += k * delta;
  if (*input < down_range) *input= up_range;
  if (*input > up_range)   *input= down_range;
  
  return k;
}                  

S8 set_int(pS8 input, S8 down_range, S8 up_range)
{
  S8  k = key(30);

  if(k == NO_KEY)  return k;
  *input += k;
  if (*input < down_range) *input = up_range;
  if (*input > up_range)   *input = down_range;
  return k;
}

S8 key(U8 wait)
{
bg:  
  Flags.key = 0;
  GenClock1 = wait;
   
  for(;;)
  { 
    WDRCLR;
    if(USBFlags.UsbEvent)       usb_control(); 
    if(Flags.measure_ready)     measure_process();
    
    if(Flags.systick)            
    {
//      if(!Time2Write)            statistic_process();      
      date_time();
      CheckInfoFlash();
      tresholds_process();
      Flags.systick    = 0;
    }
    if(Flags.key) break;
    if(!GenClock1) return NO_KEY;
    if(USBFlags.UsbConnected) LPM0;
    else                      LPM3; 
  }
    
  switch(KeyCode)
  {
    case SW1:       return   1;
    case LONG_SW1:  return   1;
    case SW2:       return   ENTR;
    case LONG_SW2:  WAIT_UNPRESS; goto bg;
    case SW3:       return  -1;
    case LONG_SW3:  return  -1;
  }
  return(NO_KEY);
}

Int8U show_list(list_sruct  list, pInt8S  p)
{
  unsigned char *s;
  signed char    k;
  
  for(;;)
  {
    if(*p > list.up) *p = 0;
    else if(*p < 0)  *p = list.up;
    s= (unsigned char *)((unsigned int)list.strings_p + (11 * *p));
  
    printf("%s",s);
    Prog_LCD_load_data();
    
    k= key(30);
    if(k == NO_KEY)       return NO_KEY;
    else if(k == ENTR)    return ENTR;
    else if(k == ABORT)   return ABORT;
    (*p) += k;
  }
}
