#include  "define.h"

#pragma memory=constseg(EECONST)  // 0000h
unsigned int  config_word=  S9600;
unsigned int  net_address=  1;
float         step_weight=  6.105006e-4;
float         ntc_tempr=    90.0;
float         nts_r=        350.0;
float         tempr_cal=    24.5;
unsigned char calib_len=    18;


#pragma memory=constseg(UNITID)   // 0070h
unsigned char SN[SN_LEN]=   "10000000";
unsigned char ZN[SN_LEN]=   "10000000";

#pragma memory=constseg(CALIB)
float         calib_arr[64][2]= {   0.292, 0.,
                                    0.298, 0.1,
                                    0.311, 0.15,
                                    0.325, 0.2,  
                                    0.343, 0.3,  
                                    0.368, 0.5,  
                                    0.392, 0.75,
                                    0.414, 1.0, 
                                    0.449, 1.5,
                                    0.478, 2.0,
                                    0.521, 3.0,
                                    0.582, 5.0,
                                    0.629, 7.0,
                                    0.689, 10.0,
                                    0.776, 15.0,
                                    0.827, 20.0,
                                    0.877, 25.0,
                                    0.92,  30.0
                                };

#pragma memory=constseg(STAT)
float         stat_arr[128];

#pragma memory=constseg(ADR_CONST)
unsigned char *adr_calib=  (unsigned char *)&calib_arr[0][0];
unsigned char *adr_stat=   (unsigned char *)&stat_arr[0];
#pragma memory=default

const unsigned char   ID[ID_LEN]=  "““Ã-2-02O";
const unsigned char   NV[NV_LEN]=  "r2.00";
