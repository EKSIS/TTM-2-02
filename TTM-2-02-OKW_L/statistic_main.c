#include  "define.h"

void  statistic_process(void)
{
  S16           int10Val;
  U16           writeShift, unint10Val;
  ldiv_t         n;
    
  // проверка на разряд батареи и режим "СТОП"
  if( ((Errors &  PW_ERROR) && !USBFlags.UsbConnected) || Flags.stopmode)
  {
    Time2Write=   Time2Measure + 2;
    OldPeriod=    0;
    return;
  }
  
  // проверка на переполнение и тип статистики
  if((Errors & STAT_OVR) && (ConfigWord & STATISTIC_F)) return;
       
  WriteAdr &= 0xfffffffc;
  Time2Write=                 WritePeriod;
  if(!USBFlags.UsbConnected)  Time2Measure= WritePeriod - 2;

  if((WriteAdr + StartStatistic) >= 0xfe00) writeShift= 512;
  else                                      writeShift= 0;
  
  n= ldiv(WriteAdr,512);
  if(!n.rem || (WritePeriod != OldPeriod))
  {
    if(n.rem <= 496)
    {
      writeFlash(WriteAdr + StartStatistic + writeShift, (pInt8U)"TIME", 4);
      WriteAdr+= 4;
      writeFlash(WriteAdr + StartStatistic + writeShift, &Seconds, 6);
      WriteAdr+= 6;
      writeFlash(WriteAdr + StartStatistic + writeShift, (pInt8U)&WritePeriod, 2);
      WriteAdr+= 2;
      OldPeriod= WritePeriod;
    }
  }
                  
  int10Val= (S16)(Tempr*10 + .5);
  if((int10Val == 17742) || (int10Val == 18772)) int10Val++;  // check for words NEXT & TIME
  writeFlash(WriteAdr + StartStatistic + writeShift,(pInt8U)&int10Val,2);
  WriteAdr+= 2;
  
  U8 h = Humidy + .5;
  unint10Val = h;
  unint10Val |= ((U16)PressureRel << 8);
  if((unint10Val == 17742) || (unint10Val == 18772)) unint10Val += 0x100;  // check for words NEXT & TIME
  writeFlash(WriteAdr + StartStatistic + writeShift, (pInt8U)&unint10Val, 2);
  WriteAdr += 2;
           
  if((WriteAdr + StartStatistic + writeShift) >= 0x243E0)
  {
    writeFlash(WriteAdr + StartStatistic + writeShift, (pInt8U)"NEXT", 4);
    WriteAdr=   0;
    Errors|=    STAT_OVR;
    OldPeriod=  0;
  }
}
