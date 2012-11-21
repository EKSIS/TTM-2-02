#include  "define.h"

void  binbuffer_2_binbuffer(Int8U __data20 * pBufferSource, Int8U __data20 * pBufferDestination, Int16U nBytes)
{
  Int16U i;
  for(i= 0; i < nBytes; i++) pBufferDestination[i]= pBufferSource[i];
}

Int8U  writeInfoFlash(pInt8U Buffer, Int16U addr, Int8U bytenum)
{
  return 1;
}

#pragma optimize=none
Int8U USBHW_Init(void)
{
  Int16U  i;
  
  USBCNF=     0;
  for(i= 0; i < 1500; i++);  // 10 ms for power stab
  USBCNF=     USB_EN;
  USBCTL=     0;
  USBPLLDIVB= (3<<8) + 15;    // 12/4*16= 48
  USBPLLCTL=  UPLLEN + UPFDEN;
  for(i= 0; i < 450; i++);   // 3 ms for PLL stab
  USBPLLIR=   0;
  for(i= 0; i < 200; i++);
  if(USBPLLCTL & USBOOLIFG)
  {
      USBCNF= USBPLLCTL= 0;
      return 0;
  }
  USB_Reset();
  USB_CONNECT;
  return 1;
}

void  USB_Reset(void)
{  
  pInt8U  memEp= (pInt8U)USBSTABUFF_;
 
  USBCNF&=    ~USB_EN;
  USBCNF|=     USB_EN;
  USBCTL=      0;
  for(; (Int16U)memEp <= USBTOPBUFF_; memEp++) *memEp= 0;
  
  USBFUNADR=   0;
  USBOEPCNF_0= USBIEPCNF_0= 0;  
  USBOEPCNF_0= USBIEPCNF_0= UBME + USBIIE;
  USBOEPCNT_0= USBIEPCNT_0= USBIEPIFG= USBOEPIFG= USBIFG= 0;
  SET_NAK_IEP_0;
  CLR_NAK_OEP_0;
  USBCTL=  FEN;
  USBFlags.UsbConfigured= 0;  
}

void  setup_packet(void)
{   
  switch(sdRequest)
  {
      case GET_STATUS_DEVICE:
      case GET_STATUS_INTERF:
      case GET_STATUS_ENDPNT: SET_USB_IEP_0_IF;
                              USB_DATA_IN;
                              USB_Buffer[0]=               0;
                              USB_Buffer[1]=               0;
                              ep_struct[ENP_IN].lenght=    2;
                              ep_struct[ENP_IN].pBuffer=   USB_Buffer; 
                              break;                                         
                              
      case GET_DESCRIPTOR_DEVICE:
      case GET_DESCRIPTOR_INTERF:
      case GET_DESCRIPTOR_ENDPNT:        
                              SET_USB_IEP_0_IF;
                              USB_DATA_IN;
                              
             switch(hValue)
             {
                case DEVICE_DESCRIPTOR:
                              ep_struct[ENP_IN].lenght=    wLenght;
                              ep_struct[ENP_IN].pBuffer=   (pInt8U) UsbStandardDeviceDescriptorStr;
                              break;
                              
                case CONFIG_DESCRIPTOR:
                              ep_struct[ENP_IN].lenght=    wLenght;
                              ep_struct[ENP_IN].pBuffer=   (pInt8U) UsbStandardConfigurationDescriptor;
                              break;
                              
                case INTERF_DESCRIPTOR:
                              ep_struct[ENP_IN].lenght=    wLenght;
                              ep_struct[ENP_IN].pBuffer=   (pInt8U) UsbStandardInterfaceDescriptor;
                              break;
                              
                case HID_DESCRIPTOR:
                              ep_struct[ENP_IN].lenght=    wLenght;
                              ep_struct[ENP_IN].pBuffer=   (pInt8U) UsbHidDescriptor;
                              break;
                              
                case REPORT_DESCRIPTOR:
                              ep_struct[ENP_IN].lenght=    wLenght;
                              ep_struct[ENP_IN].pBuffer=   (pInt8U) HidReportDescriptor;
                              break;
                              
                case STRING_DESCRIPTOR:
                      switch(lValue)
                      {   
                        case iLanguagesStr:
                                            ep_struct[ENP_IN].lenght=    wLenght;
                                            ep_struct[ENP_IN].pBuffer=   (pInt8U) UsbLanguagesStr;
                                            break;
                                            
                        case iManufacturerStr:
                                            ep_struct[ENP_IN].lenght=    wLenght;
                                            ep_struct[ENP_IN].pBuffer=   (pInt8U) ManufacturerStr;
                                            break;
                                            
                        case iProductStr:
                                            ep_struct[ENP_IN].lenght=    wLenght;
                                            ep_struct[ENP_IN].pBuffer=   (pInt8U) ProductStr;
                                            break;
                                            
                        case iSerialNumberStr:
                                            ep_struct[ENP_IN].lenght=    wLenght;
                                            ep_struct[ENP_IN].pBuffer=   (pInt8U) SerialNumberStr;
                                            break;
                                            
                        default:            goto stall;                                            
                      }
                      break;                              
                              
                default:      goto stall;
             }
             break;
                              
      case GET_CONFIGURATION: SET_USB_IEP_0_IF;
                              USB_DATA_IN;
                              USB_Buffer[0]=               (USBFlags.UsbConfigured ? 1:0);
                              ep_struct[ENP_IN].lenght=    1;
                              ep_struct[ENP_IN].pBuffer=   USB_Buffer;
                              break;
                                                            
      case SET_ADDRESS:       USB_DATA_IN;
                              SEND_ASK_IEP_0;
                              ep_struct[ENP_IN].lenght=               0;
                              USBFUNADR=                         lValue;
                              break;
                              
      case SET_CONFIGURATION: USB_DATA_IN;
                              SEND_ASK_IEP_0;
                              ep_struct[ENP_IN].lenght=               0;
                              USBFlags.UsbConfigured=                 1;
                              break;

      case GET_REPORT:        if(bReportType != FEATURE_REPORT) goto stall;
      
                              SET_USB_IEP_0_IF;
                              USB_DATA_IN;                              
                              ep_struct[ENP_IN].lenght=        wLenght;
                              ep_struct[ENP_IN].pBuffer=    USB_Buffer;
                              break;                              
                              
      case SET_REPORT:        if(bReportType != FEATURE_REPORT) goto stall;  
                              
                              USB_DATA_OUT;
                              CLR_NAK_OEP_0;           
                              ep_struct[ENP_OUT].pBuffer=   USB_Buffer;
                              ep_struct[ENP_OUT].lenght=       wLenght;
                              break;
                              
      case SET_IDLE:          USB_DATA_IN;
                              SEND_ASK_IEP_0;
                              ep_struct[ENP_IN].lenght=               0;
                              break;
                              
stall:                                    
      default:                USB_SETSTALL_EP_0;
    }  
    USBIFG= 0;
}

void  ep0_out_packet(void)
{
  Int8U   RxLen;
    
  RxLen= USBOEPCNT_0 & 0xf;
  if(RxLen)
  {
    binbuffer_2_binbuffer(USB_OEP0_BUF, ep_struct[ENP_OUT].pBuffer, RxLen); 
    ep_struct[ENP_OUT].pBuffer= (pInt8U) ((Int16U)ep_struct[ENP_OUT].pBuffer + RxLen);    
    ep_struct[ENP_OUT].lenght-= RxLen;
    if(!ep_struct[ENP_OUT].lenght)
    {
      USBFlags.UsbDataReady= 1;
      USB_DATA_IN;
      SEND_ASK_IEP_0;
      ep_struct[ENP_IN].lenght= USBFlags.UsbSendZeroReq= 0;
    }
  }
  else if(USBFlags.BootStartReq)     SOFT_RESET;
  
  CLR_NAK_OEP_0;
  CLR_USB_OEP_0_IF;
}

void  ep0_in_packet(void)
{
  Int8U     TxLen;

  CLR_USB_IEP_0_IF;
  if(ep_struct[ENP_IN].lenght == 0)  // send ASK, zero packet or last packet sent
  {
    if(USBFlags.UsbSendZeroReq)
    {
      USBFlags.UsbSendZeroReq= 0;
      SEND_ASK_IEP_0;
      return;
    }
    USB_DATA_OUT;
    CLR_NAK_OEP_0;
    return;
  }
    
  if(ep_struct[ENP_IN].lenght > SIZE_ENP_IN) TxLen= SIZE_ENP_IN;
  else                                       TxLen= ep_struct[ENP_IN].lenght;
  binbuffer_2_binbuffer(ep_struct[ENP_IN].pBuffer, USB_IEP0_BUF, TxLen);  

  if(ep_struct[ENP_IN].lenght == SIZE_ENP_IN)  USBFlags.UsbSendZeroReq= 1;
  ep_struct[ENP_IN].lenght-= TxLen;
  ep_struct[ENP_IN].pBuffer= (pInt8U) ((Int16U)ep_struct[ENP_IN].pBuffer + TxLen);
  USBIEPCNT_0=  TxLen;
}

void usb_exchange(void)
{
  Int32U          addr;
  Int8U           bytenum, command, i, k;
  
  addr=           USB_Buffer[0] + ((Int32U)USB_Buffer[1] << 8) + ((Int32U)USB_Buffer[2] << 16) + ((Int32U)USB_Buffer[3] << 24);
  command=        USB_Buffer[4];
  bytenum=        USB_Buffer[5];
  USB_Buffer[0]=  ASK_USBEXCH;
    
  switch(command)
  {                        
    // WR
    case 0x00:    if( (bytenum > USB_WDATA_LEN) || ((addr + bytenum) > RAMSIZE) ) goto fail;
                  binbuffer_2_binbuffer((Int8U __data20 *)&USB_Buffer[6], (Int8U __data20 *)(RamBeginAddr + addr), bytenum);
                  break;
    // WI
    case 0x02:    if(bytenum > USB_WDATA_LEN)  goto fail; 
                  if(!WriteFlashString(addr, &USB_Buffer[6], bytenum))  goto fail;
                  break;                  
    // RR             
    case 0x80:    if(bytenum > USB_RDATA_LEN)  goto fail;
                  if((addr + bytenum) <=  RAMSIZE) binbuffer_2_binbuffer( (Int8U __data20 *)(RamBeginAddr + addr), (Int8U __data20 *)&USB_Buffer[1], bytenum);
                  else goto fail;
                  break;         
                  
    // RF             
    case 0x81:    if(bytenum > USB_RDATA_LEN)  goto fail;
                  if(!SD_Flags.sd_initialized) goto fail;
                  SD_CS_LOW;
                  sd_read_block(addr, &USB_Buffer[1]);
                  SD_CS_HIGH;
                  if(Errors & SD_ERROR)
                  {
                    SD_Flags.sd_valid = SD_Flags.sd_initialized = 0;                    
                    goto fail;
                  }
                  break;         
                  
    // RI
    case 0x82:    if(bytenum > USB_RDATA_LEN)  goto fail;
                  if((addr + bytenum) <=  INFOENDADR) binbuffer_2_binbuffer( (Int8U __data20 *)(addr), (Int8U __data20 *)&USB_Buffer[1], bytenum);
                  else goto fail;
                  break; 
    // IR
    case 0x8F:    k = 1;
                  for(i = 0; i < 18; i++)     USB_Buffer[k++]= SerialNumberStr[i];
                  for(i = 0; ID[i]; i++)      USB_Buffer[k++]= ID[i];
                  for(i = 0; Version[i]; i++) USB_Buffer[k++]= Version[i];
                  USB_Buffer[k]=   0;
                  break;
    // BS
    case 0x55:    USBFlags.BootStartReq= 1;
                  LCD_off;
                  break;
    // DW                  
    case 0x56:    Flags.DefMemWrite= 1;
                  break;                  
      
fail:             
    default:      USB_Buffer[0]=  NAK_USBEXCH;
  }
}

void  usb_control(void)
{     
  if(CHECK_USB_POWER) 
  {
    if(USBPWRCTL & VBOFFIFG)
    {
      Flags.measure_ready= USBFlags.UsbConnected= USBFlags.UsbConfigured= USBCTL= USBPLLCTL= USBCNF= 0;
      USBPWRCTL=          USBDETEN + SLDOAON;               // вкл определение питания на VBUS, выкл стабилизаторы
      XT2_STOP;
      USBPWRCTL &=       ~VBOFFIFG;
    }    
    else
    {
      if(USBPWRCTL & VBONIFG)
      {
        USBPWRCTL= VUSBEN + USBDETEN + SLDOAON;             // вкл стабилизаторы
        xt2_start();
        USBHW_Init();
        USBFlags.UsbConnected= 1;         
        USBPWRCTL &= ~VBONIFG;
      }
    }
  }
    
  // usb reset
  if(CHECK_USB_RESET) 
  { 
    USB_Reset(); 
  }
  
  // setup packets
  if(CHECK_USB_SETUP) 
  { 
    setup_packet(); 
  }
  
  // EP0 OUT packet
  if(CHECK_USB_OEP_0_IF) ep0_out_packet();
  
  // EP0 IN packet
  if(CHECK_USB_IEP_0_IF) ep0_in_packet();
  
  // data exchange
  if(USBFlags.UsbDataReady)
  {
    USBFlags.UsbDataReady= 0;
    usb_exchange();
  }
  
  USBFlags.UsbEvent= 0;
  USB_INT_ENABLE;
}
