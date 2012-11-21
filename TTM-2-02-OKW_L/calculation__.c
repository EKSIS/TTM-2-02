#include  "define.h"

// extern var
extern float          velocity,tempr,v_bat,rth_vs,r_term,r_vs,ntc_tempr,pid_out,nts_r;
extern float          err_arr[3],r1,i1,calib_arr[64][2],tempr_cal,step_weight,k1,zero;
extern unsigned int   ta_pwm0,pressure,mem_0,mem_3,mem_4;
extern unsigned char  errors, selector, lcd_clock, calib_len, measure_clock;
extern flags_struct   flags;

float   pwr;

float p_calc(float x);

void  measure_process(void)
{     
     float            f2,f3;
     float            f,kfv,kft;
     
     flags.measure_ready= 0;
     
     f2= (float)ADC12MEM1*3.12765957;
     f3= (float)ADC12MEM2;
     if(ADC12MEM2) r_vs= 100.*(f2/f3-1.);        // R NTC
          
     // filter
     if((selector == START_MODE)&&(lcd_clock > SEC(1))) { kft= .75; kfv= 0.75; }
      else 
      {
        kft= 0.05;
        if(fabs(pwr-f2)>0.01*f2)  kfv= 0.02;
          else                    kfv= 0.001;
      }
         
     i1+=  kfv*(f2 - i1);
     pwr+= kfv*(i1 - pwr);
     
     //pid
     err_arr[2]= err_arr[1];
     err_arr[1]= err_arr[0];
     err_arr[0]= r_vs - nts_r;
     
     f= 0.0051*err_arr[0] - 0.005*err_arr[1] + 0.0001*err_arr[2];
     
     if(f > .05)        f= 0.05;
     else if(f < -.05)  f= -.05;
     
     pid_out+= f;
     if(pid_out>1)          pid_out= 1;
     else if(pid_out<0)     pid_out= 0;
     ta_pwm0=               700 * pid_out;
     
     TACTL&=                ~TAIFG;
     TACTL|=                TAIE;
     
     // bat & temp
     if(flags.t_ready)
     {
       flags.t_ready= 0;
       v_bat += 0.5*((float)mem_3 * 7.8144e-4 - v_bat);
       if(mem_4)
       {
          f= (float)mem_0/(float)mem_4 - 1.0;
          if(f!=0) r_term+= kft*(1./f - r_term);
            else  errors|=  Z_ERROR;
          if(!f3) errors|=  Z_ERROR;
       };
     };
}

void  calc(void)
{   
    if((errors & Z_ERROR) && (selector != START_MODE)) 
    { 
      TURN_OFF;
      selector=   ERROR_MODE;
      return; 
    }

    tempr=    -2.45657E+02 + 2.35654E+02*r_term + 1.01054E+01*r_term*r_term - .2;   // temper of flow
    if((tempr<-50)||(tempr>90)) errors|=  TR_ERROR;
    else                        errors&= ~TR_ERROR;
    
    rth_vs=   pwr*pwr*k1/(ntc_tempr-tempr);
    velocity= p_calc(rth_vs);
    
    if(velocity > 40.0) errors|=  VR_ERROR;
    else                errors&= ~VR_ERROR;
}

float p_calc(float x) 
{
    float x1, x2, x3, y1, y2, y3, s1, s2, s3, s4, a, b, c;
    unsigned char   i= 1;    
    
    if(x<=calib_arr[0][0]) return 0.;

    while(i<calib_len)
    {
      if(x<=calib_arr[i][0]) break;
      i++;
    };
    
    if(i>=(calib_len-1))    i= calib_len-2;
    x2= calib_arr[i][0];    y2= calib_arr[i][1];
    x1= calib_arr[i-1][0];  y1= calib_arr[i-1][1];
    x3= calib_arr[i+1][0];  y3= calib_arr[i+1][1];

    s1= (y3-y1)/(x3-x1);
    s2= (y3-y2)/(x3-x2);
    s3= (x3*x3-x1*x1)/(x3-x1);
    s4= (x3*x3-x2*x2)/(x3-x2);
    a=  (s1-s2)/(s3-s4);
    b=  s1 - a*s3;
    c=  y1 - a*x1*x1 - b*x1;

    y1=a*x*x + b*x + c;
    
    if(y1<0.01) y1= 0;
    
    return y1;
}