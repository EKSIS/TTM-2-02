#ifndef _GLOBAL_VAR_H
#define _GLOBAL_VAR_H

// predeclaration
extern volatile Flags_t           Flags; 
extern volatile UsbFlags_t        USBFlags; 
extern volatile sd_flags_struct   SD_Flags;
extern ep_struct_t                ep_struct[]; 

extern F32                    Tempr, Humidy, Res, Pressure, Power, AdcGain, Ref15, PressureParam, HumidyParam; 
extern U32                    WriteAdr;
extern U16                    Lcd_Buffer[], Time2Write, Time2Measure, Errors, SD_Clock;
extern U8                     USB_Buffer[], FlashBuffer[];
extern U8                     HT1621_Buffer[], LcdPointer, Selector, KeyCode, GenClock1, HumidyUnits, PowLevel, PowerShow;
extern U8                     Seconds, Minutes, Houres, Days, Monthes, Years, KeyClock;

extern const F32              W1, W2, W11, W12, W21, W22, B2, B11, B12, Polynoms[], Tcorr, E_TAB[], C1_P, C0_H, C1_H, PowerTreshLev[2][4][2], TrshVal[];
extern const U16              ConfigWord, WritePeriod, CheckSumD, CheckSumC, StartStatistic;
extern const U8               Version[], SerialNumberStr[], MonthSize[], ID[], MagicWord[];
extern const U8               DataSizeSegC, DataSizeSegD;

void LCD_load_data            (void);
void LCD_on                   (void);
void LCD_send_com             (U8 command);
void ini                      (void);
void lcd_process              (void); 
void measure_process          (void);
void statistic_process        (void);
void tresholds_process        (void);
void sd_process               (void);
void key_process              (void);
void date_time                (void);
void prog_serv                (void);
void xt2_start                (void);
void CheckInfoFlash           (void);
void Prog_LCD_load_data       (void);

U16  calcCHK16                (pU8 buffer, U8 len);
U8   WriteFlashString         (U32 adr, pU8 bufpoint, U8 len);
U8   writeFlash               (U32 writeAdr, pU8 buffer, U8 num);

F32  hum_re_calc              (pU8 accure);
F32  calc                     (F32 x);
F32  serj_func                (F32 x);
#endif
