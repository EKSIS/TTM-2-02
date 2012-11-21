#include  "define.h"

Int16U calcCHK16(pU8 buffer, U8 len)
{
  Int16U  i, crc16;
  
  for(crc16= 0, i= 0; i < len; i++) 
  { 
    crc16+= *buffer; 
    buffer++; 
  }
  
  return crc16;
}

Int8U writeFlash(Int32U writeAdr, pInt8U buffer, Int8U num)
{
  Int8U       i;

  FCTL3=     FWKEY;  // unlock write
  for(i= 0; i < num; i++, writeAdr++) 
  {
    if((writeAdr >= 0xfe00) && (writeAdr < 0x10000))          continue;
    if(writeAdr >= 0x4000)                                    continue;
    
    if( !(writeAdr & 0x3ff) || ((writeAdr & 0x3ff) == SEG_SIZE) || (writeAdr == SEG_A)
         || (writeAdr == SEG_B) || (writeAdr == SEG_C) || (writeAdr == SEG_D) )
    {
      FCTL1= FWKEY + ERASE;
      *((Int8U __data20 *)writeAdr)= 0xff;
    }
    if(buffer[i] != 0xff)
    {
      FCTL1= FWKEY + WRT;
      *((Int8U __data20 *)writeAdr)= buffer[i];
      FCTL1= FWKEY;
    }
    if(*((Int8U __data20 *)writeAdr) != buffer[i]) 
    { 
      FCTL3= FWKEY + LOCK + LOCKA; 
      return 0; 
    }
  }
  FCTL3= FWKEY + LOCK + LOCKA;  // lock write
  return 1; 
}

Int8U WriteFlashString(Int32U adr, pInt8U bufpoint, Int8U len)
{
  Int16U  startAdr, crc;
  Int8U   writeNum, shift;
  
  if((adr >= SEG_D) && (adr <= (SEG_D + DataSizeSegD - len)))
  {  
    writeNum=  DataSizeSegD;
    startAdr=  SEG_D;
  }
  else if((adr >= SEG_C) && (adr <= (SEG_C + DataSizeSegC - len)))
  {
    writeNum= DataSizeSegD;
    startAdr=  SEG_C;
  }
  else if((adr >= SEG_B) && (adr <= (SEG_B + 128 - len)) && Flags.DefMemWrite)
  {
    writeNum=  128;
    startAdr=  SEG_B;
  }
  else if((adr >= SEG_A) && (adr <= (SEG_A + 128 - len)) && Flags.DefMemWrite)
  {
    writeNum=  128;
    startAdr=  SEG_A;
  }
  else return 0;
  
  binbuffer_2_binbuffer((Int8U __data20 *)startAdr, FlashBuffer, writeNum);
  shift= adr - startAdr;
  binbuffer_2_binbuffer((Int8U __data20 *)bufpoint, &FlashBuffer[shift], len);
  if(!Flags.DefMemWrite) 
  { 
    crc= calcCHK16(FlashBuffer, writeNum); 
    *((pU16)&FlashBuffer[writeNum])= crc; 
    writeNum += 2;
  }
  Flags.DefMemWrite= 0;
  return writeFlash(startAdr, FlashBuffer, writeNum);
}

void CheckInfoFlash(void)
{ 
  if(DataSizeSegD && (DataSizeSegD < 127))
  {
    if(calcCHK16((pInt8U)SEG_D, DataSizeSegD) != CheckSumD)
    {
      WriteFlashString(SEG_D, (pInt8U)SEG_B, DataSizeSegD); 
    }
  }
  
  if(DataSizeSegC && (DataSizeSegC < 127))
  {
    if(calcCHK16((pInt8U)SEG_C, DataSizeSegC) != CheckSumC)
    {
      WriteFlashString(SEG_C, (pInt8U)SEG_A, DataSizeSegC); 
    }
  }      
}
