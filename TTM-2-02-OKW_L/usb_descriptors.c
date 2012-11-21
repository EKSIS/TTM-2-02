#include  "define.h"

#pragma data_alignment=4
const UsbStandardDeviceDescriptorStr_t UsbStandardDeviceDescriptorStr[]=
{
  sizeof(UsbStandardDeviceDescriptorStr_t),
  UsbDescriptorDevice,
  0x200,
  0,
  0,
  0,
  8,
  0x3412,
  0x0009, // TTM-2-02
  0x0001,
  iManufacturerStr,
  iProductStr,
  iSerialNumberStr,
  1,
};

#pragma data_alignment=4
const Int8U HidReportDescriptor[] =
{
  0x06,0x00,0xff, // Usage Page
  0x09,0x01, // Usage 1
  0xA1,0x01, // Collection (Application)
  
    0x15,0x00,                // Logical Minimum (0)
    0x26,0xff,0x00,           // Logical Maximum (255)
    
    0x09,0x01,                // Usage 1
    0x75,0x08,                // Report Size 8-bits
    0x95,70,                  // Report Count 70-bytes
    0xB1,0x02,                // Feature (Data,Var,Abs)
    
  0xC0       // End collection
};

#pragma data_alignment=4
const Int8U UsbHidDescriptor[] =
{
  sizeof(UsbHidDescriptor_t),
  Hid,
  _2BL(0x0111),
  0,                                  // country code
  1,                                  // num of RepDiscriptors
  HidReport,                          // type of RepDiscriptors 
  _2BL(sizeof(HidReportDescriptor))   // len of RepDiscriptor
};

#pragma data_alignment=4
const UsbStandardInterfaceDescriptor_t UsbStandardInterfaceDescriptor[] =
{
  sizeof(UsbStandardInterfaceDescriptor_t),
  UsbDescriptorInterface,
  0,
  0,
  0,  // num end points
  UsbDeviceClassHumanInterface,
  NoSubclass,
  None,
  0
};

#pragma data_alignment=4
const Int8U UsbStandardConfigurationDescriptor[] =
{
  sizeof(UsbStandardConfigurationDescriptor_t),
  UsbDescriptorConfiguration,
  _2BL( 
   sizeof(UsbStandardConfigurationDescriptor_t) +
   sizeof(UsbStandardInterfaceDescriptor_t) +
   sizeof(UsbHidDescriptor_t)
       ),
  1,
  1,
  0,
  UsbConfigurationCommmonAttr,
  UsbConfigPower_mA(100),

  sizeof(UsbStandardInterfaceDescriptor_t),
  UsbDescriptorInterface,
  0,
  0,
  0,  // num end points
  UsbDeviceClassHumanInterface,
  NoSubclass,
  None,
  0,

  sizeof(UsbHidDescriptor_t),
  Hid,
  _2BL(0x0111),
  0,
  1,
  HidReport,
  _2BL(sizeof(HidReportDescriptor))
    
};

#pragma data_alignment=4
const Int8U UsbLanguagesStr [] =
{
  // Length of LanguagesStr + 2
  4,
  // Desciptor Type
  UsbDescriptorString,
  // Languages ID
  0x09,0x04,
};

#pragma data_alignment=4
const Int8U ManufacturerStr [] =
{
  12, // length
  UsbDescriptorString,  // Descriptor
  0x2d,4,0x1a,4,0x21,4,0x18,4,0x21,4 // эксис в юникоде
};

#pragma data_alignment=4
const Int8U ProductStr [] =
{
  18, //length
  UsbDescriptorString, // Descriptor
  'I',0,'V',0,'T',0,'M',0,'-',0,'7',0,'M',0,'8',0   // Str
};
