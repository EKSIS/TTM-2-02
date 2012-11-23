#include  "define.h"

// глобальные
F32                       AdcGain;
F32                       Ref15;
F32                       Power = 3.0;
U16                       Lcd_Buffer[10];
U16                       Time2Write   = 30;
U16                       SD_Clock;
U8                        FlashBuffer[128];
U8                        USB_Buffer[USB_BUF_LEN];
U8                        HT1621_Buffer[16];
U8                        LcdPointer;
U8                        Selector;
U8                        KeyCode;
U8                        KeyClock;
U8                        GenClock1;
U8                        HumidyUnits;
U8                        PowLevel = 3;
U8                        PowerShow = 99;

volatile Flags_t          Flags;
volatile UsbFlags_t       USBFlags;
volatile sd_flags_struct  SD_Flags;
ep_struct_t               ep_struct[2];

// глобальные в сегменте RAM - начальный внешний адрес = 0х0000
#pragma dataseg= RAM
__root __no_init F32      Tempr;
__root __no_init F32      Humidy;
__root __no_init F32      Pressure;
__root __no_init F32      HumidyParam;
__root __no_init F32      Res;
__root __no_init F32      PressureParam;
__root __no_init U32      WriteAdr;
__root __no_init U16      Errors;
__root __no_init U8       Seconds, Minutes, Houres, Days, Monthes, Years;
#pragma memory= default

//  константы в спец сегментах
#pragma constseg= INFOD   // 0x1800
__root const U16        ConfigWord =  WRITE_ON;
__root const U16        WritePeriod=  20;
__root const F32        Tcorr = 0.0;
__root const F32        TrshVal[4]=   { -20.0, 60.0, 1.0, 99.0 };
__root const U16        CheckSumD;
__root const U16        CheckSumC;

#pragma constseg= INFOB   // 0x1900
__root const U16        __ConfigWord =  WRITE_ON;
__root const U16        __WritePeriod=  20;
__root const F32        __Tcorr = 0.0;
__root const F32        __TrshVal[4]=   { -20.0, 60.0, 1.0, 99.0 };
__root const U8         DataSizeSegD=   24;
__root const U8         DataSizeSegC;

#pragma location = 0x1920
__root const F32      W1;
#pragma location = 0x1924
__root const F32      W2;
#pragma location = 0x1928
__root const F32      W11;
#pragma location = 0x192C
__root const F32      W12;
#pragma location = 0x1930
__root const F32      W21;
#pragma location = 0x1934
__root const F32      W22;
#pragma location = 0x1938
__root const F32      B2=     50.;
#pragma location = 0x193C
__root const F32      B11;
#pragma location = 0x1940
__root const F32      B12;
#pragma location = 0x1944
__root const F32      C0_H=   1.;
#pragma location = 0x1948
__root const F32      C1_H;
#pragma location = 0x194C
__root const F32      C1_P=   0.375;
#pragma location = 0x1950
__root const U8      SerialNumberStr[] =
{
  18, //length
  UsbDescriptorString, // Descriptor
  '1',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0,'0',0
};

#pragma location= ADR_RESET_VECT
 __root const U16     RST= TRUE_RESET_VECT;   //  для подсчета корректной контрольной суммы


#pragma constseg= STATISTIC
#pragma data_alignment=512
__root const Int8U   StatisticArray= 0xff;         //  обозначаем начало массива статистики 
#pragma constseg= default

__root const F32  PowerTreshLev[2][4][2] = 
{ 2.32, 0., 2.62, 2.28, 2.92, 2.58, 10., 2.88, 2.32, 0., 2.42, 2.28, 2.52, 2.38, 10., 2.48 };
__root const F32  Polynoms[6]=       { -2.43880E+02, 2.30354E+02, 1.57958E+01, -3.05764E+00, 7.82640E-01, -5.02791E-02 };
__root const U8   ID[]=              "TTМ-2-02";
__root const U8   Version[]=         "r1.00";
__root const U8   MagicWord[]=       "STATISTIC";
__root const U8   MonthSize[12]=     { 31,28,31,30,31,30,31,31,30,31,30,31 };
__root const U16  StartStatistic=    (Int16U)&StatisticArray;
__root const F32  E_TAB[101]= {
6.106359361,
5.621913509,
5.172714271,
4.756444744,
4.370922676,
4.014093781,
3.684025324,
3.378899958,
3.097009813,
2.836750838,
2.596617372,
2.375196944,
2.171165305,
1.983281669,
1.810384169,
1.651385507,
1.505268814,
1.371083691,
1.247942432,
1.135016429,
1.031532749,
0.936770871,
0.850059584,
0.770774042,
0.698332957,
0.632195945,
0.571861002,
0.516862109,
0.466766967,
0.421174853,
0.379714586,
0.342042608,
0.307841171,
0.276816624,
0.248697798,
0.223234477,
0.200195972,
0.179369761,
0.160560224,
0.143587446,
0.128286096,
0.114504374,
0.102103021,
0.090954399,
0.080941624,
0.071957751,
0.063905025,
0.056694166,
0.050243716,
0.044479422,
0.039333663,
0.034744918,
0.03065727,
0.027019947,
0.023786896,
0.020916383,
0.018370633,
0.016115488,
0.014120092,
0.012356603,
0.010799927,
0.009427472,
0.008218921,
0.007156022,
0.0062224,
0.005403381,
0.004685829,
0.004058,
0.003509408,
0.003030703,
0.002613554,
0.002250554,
0.001935119,
0.001661411,
0.001424256,
0.001219076,
0.001041826,
0.000888937,
0.000757265,
0.000644044,
0.000546844,
0.000463532,
0.000392241,
0.00033134,
0.000279401,
0.000235182,
0.000197602,
0.00016572,
0.000138721,
0.0001159,
9.66459E-05,
8.04317E-05,
6.6804E-05,
5.53725E-05,
4.58024E-05,
3.78068E-05,
3.11403E-05,
2.55937E-05,
2.09886E-05,
1.71735E-05,
1.40198E-05
};
