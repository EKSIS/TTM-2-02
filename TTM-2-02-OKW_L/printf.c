#include "define.h"

// LCD definition
#define   c_CHAR      16
#define   L_CHAR      17
#define   o_CHAR      18
#define   t_CHAR      19
#define   P_CHAR      20
#define   n_CHAR      21
#define   U_CHAR      22
#define   u_CHAR      23
#define   r_CHAR      24
#define   Y_CHAR      25
#define   H_CHAR      26
#define   l_CHAR      27
#define   N_CHAR      28
#define   LOW_LINE_C  29
#define   MINUS_C     30
#define   SPACE_C     31
#define   POINT_C     32
#define   R_CHAR      33
#define   g_CHAR      9
#define   J_CHAR      34
#define   M_CHAR      35
#define   RG_CHAR     36
#define   X_CHAR      37
#define   FY_CHAR     38
#define   RU_CHAR     39
#define   K_CHAR      40
#define   RI_CHAR     41
#define   RT_CHAR     42
#define   Rd_CHAR     43
#define   RB_CHAR     44

#define NOSG          0x24

// кодирование сегментов в формате заказного индикатора: старшая тетрада - адрес SEG 0..31, младшая - адрес COM 0..3 (с легким извратом)
// номер кода в массиве соответсвует номеру сегмента по знакогенератору (0...15) + 16*знакоместу (0...9) - всего 160 элементов

const U8 HT1621_coder[]= { 
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */
    NOSG, 0xe7, 0xe7, NOSG, NOSG, NOSG, 0xe6, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG,  // знакоместо 1
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */        
    0xd5, 0xd2, 0xd3, 0xd7, 0xe2, 0xe1, 0xd6, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, 0xc7,  // знакоместо 2
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */        
    0xc1, 0xb6, 0xb7, 0xc3, 0xc6, 0xc5, 0xc2, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, 0xb3,  // знакоместо 3
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */        
    0xa5, 0xa2, 0xa3, 0xa7, 0xb2, 0xb1, 0xa6, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, 0x97,  // знакоместо 4
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */        
    0x91, 0x86, 0x87, 0x93, 0x96, 0x95, 0x92, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG,  // знакоместо 5    
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */    
    NOSG, 0xf7, 0xf7, NOSG, NOSG, NOSG, 0xf6, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG, NOSG,  // знакоместо 6
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */    
    0x80, 0x65, 0x66, 0x73, 0x83, 0x81, 0x82, 0x74, 0x75, 0x70, 0x71, 0x72, 0x76, 0x77, NOSG, 0x67,  // знакоместо 7
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */        
    0x60, 0x45, 0x46, 0x53, 0x63, 0x61, 0x62, 0x54, 0x55, 0x50, 0x51, 0x52, 0x56, 0x57, NOSG, 0x47,  // знакоместо 8
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */        
    0x40, 0x25, 0x26, 0x33, 0x43, 0x41, 0x42, 0x34, 0x35, 0x30, 0x31, 0x32, 0x36, 0x37, NOSG, 0x27,  // знакоместо 9
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */        
    0x20, 0x05, 0x06, 0x13, 0x23, 0x21, 0x22, 0x14, 0x15, 0x10, 0x11, 0x12, 0x16, 0x17, NOSG, NOSG   // знакоместо 10
/*  SegA  SegB  SegC  SegD  SegE  SegF  SegG  SegH  SegI  SegJ  SegK  SegL  SegM  SegN  NOSG  SegP  */    
};

// знакогенератор в стандартный 7 сегментный индикатор
const U16 code_gen[]=  {
                          0x3f,  // 0
                          0x06,  // 1
                          0x5b,  // 2
                          0x4f,  // 3
                          0x66,  // 4
                          0x6d,  // 5
                          0x7d,  // 6
                          0x07,  // 7
                          0x7f,  // 8
                          0x6f,  // 9
                          0x77,  // A
                          0x7c,  // b
                          0x39,  // C
                          0x5e,  // d
                          0x79,  // E
                          0x71,  // F
                          0x58,  // c
                          0x38,  // L
                          0x5c,  // o
                          0x78,  // t
                          0x73,  // P
                          0x54,  // n
                          0x3e,  // U
                          0x1c,  // u
                          0x50,  // r
                          0x6e,  // Y
                          0x76,  // H
                          0x30,  // l,i
                          0x37,  // N
                          0x08,  // _
                          0x40,  // -
                          0x00,  // , ,
                          0x8000,  // .
                          0x31,  // R
                          0x0e,  // J
                          0x0,   // M
                          0x0,   // Г
                          0x0,   // X
                          0x0,   // RY
                          0x6e,  // У
                          0x0,   // K
                          0x0,   // И
                          0x78,  // Т
                          0x5f,  // д                          
                          0x7d,  // Б                                                    
                        }; // c,L,o,t,P,n,U,u,r,Y,H,l,N,'_','-',' ','.',R

// знакогенератор в стандартный 13 сегментный индикатор
const U16 code_gen_L[]=  {
                          0x03f,  // 0
                          0x006,  // 1
                          0x45b,  // 2
                          0x44f,  // 3
                          0x466,  // 4
                          0x46d,  // 5
                          0x47d,  // 6
                          0x007,  // 7
                          0x47f,  // 8
                          0x46f,  // 9
                          0x477,  // A
                          0x47c,  // b
                          0x039,  // C
                          0x45e,  // d
                          0x079,  // E
                          0x071,  // F
                          0x458,  // c
                          0x038,  // L
                          0x45c,  // o
                          0x078,  // t
                          0x473,  // P
                          0x454,  // n
                          0x03e,  // U
                          0x01c,  // u
                          0x450,  // r
                          0x46e,  // Y
                          0x476,  // H
                          0x030,  // l,i
                          0x037,  // N
                          0x008,  // _
                          0x440,  // -
                          0x000,  // , ,
                          0x8000, // .
                          0xc33,  // R
                          0x00e,  // J
                          0x2b6,  // M
                          0x031,  // Г
                          0x2A80, // X
                          0x1280, // Y
                          0x46e,  // У
                          0xa70,  // K
                          0x2236, // И
                          0x1101, // Т                          
                          0x45f,  // д
                          0x47d,  // Б                          
                        };

void LCD_send_com(U8 command)
{
  U8    b;
  
  P4SEL &= ~(BIT0 + BIT4);
  CS_LCD_LOW;  
  LCD_ONE;  LCD_CLK;  // 1000
  LCD_ZERO; LCD_CLK;
  LCD_ZERO; LCD_CLK;
  LCD_ZERO; LCD_CLK;
  P4SEL |=  (BIT0 + BIT4);
  UCA1CTL0 |= UCMSB;
  SHIFT_BYTE_LCD(command, b);
  CS_LCD_HIGH;  
}

void LCD_load_data(void)
{
  P4SEL &= ~(BIT0 + BIT4);
  CS_LCD_LOW;  
  LCD_ONE;  LCD_CLK;  // 101 000000 запись данных с адреса 0h
  LCD_ZERO; LCD_CLK;
  LCD_ONE;  LCD_CLK;
  LCD_ZERO; LCD_CLK;
  LCD_ZERO; LCD_CLK;
  LCD_ZERO; LCD_CLK;
  LCD_ZERO; LCD_CLK;
  LCD_ZERO; LCD_CLK;
  LCD_ZERO; LCD_CLK;  
  P4SEL |=  (BIT0 + BIT4);
  
  UCA1CTL0 &= ~UCMSB;
  for(U8 i = 0, b; i < 16; i++) SHIFT_BYTE_LCD(HT1621_Buffer[i], b); // HT1621_Buffer[i]
  CS_LCD_HIGH;
}

void LCD_on(void)
{
  POW_LCD_ON;
  LCD_send_com(0x00);   // SYS DIS
  LCD_send_com(0x30);   // RC 256K 
  LCD_send_com(0x02);   // SYS EN
  LCD_send_com(0x52);   // BIAS 1/3
  LCD_send_com(0x06);   // LCD ON
}

void  convert_2_HT1621(void)
{ 
  for(U8 i= 0; i < 16; i++) HT1621_Buffer[i]= 0;
  for(U8 i= 0; i < 10; i++)
    for(U8 j= 0; j < 16; j++)
      if(Lcd_Buffer[i] & ((U16)1 << j)) HT1621_Buffer[HT1621_coder[(i << 4) + j] >> 4] |= (1 << (HT1621_coder[(i << 4) + j] & 0x07) );  
}

int putchar(int c)
{
  pU16   code_gen_reg;
  
  if(LcdPointer < 5)  code_gen_reg = (pU16)code_gen;
  else                code_gen_reg = (pU16)code_gen_L;

  if((c<58)&&(c>47)) Lcd_Buffer[LcdPointer]= code_gen_reg[c-48];
  else
        switch(c) {
                  case ' ': Lcd_Buffer[LcdPointer]= code_gen_reg[SPACE_C];
                            break;

                  case '.': if(LcdPointer) --LcdPointer;
                            else   LcdPointer= 9;
                            Lcd_Buffer[LcdPointer]|= code_gen_reg[POINT_C];
                            break;

                  case '-': Lcd_Buffer[LcdPointer]= code_gen_reg[MINUS_C];
                            break;

                  case '_': Lcd_Buffer[LcdPointer]= code_gen_reg[LOW_LINE_C];
                            break;

                  case 'a':
                  case 'A': Lcd_Buffer[LcdPointer]= code_gen_reg[0x0A];
                            break;

                  case 'B': Lcd_Buffer[LcdPointer]= code_gen_reg[8];
                            break;

                  case 'b': Lcd_Buffer[LcdPointer]= code_gen_reg[0x0B];
                            break;

                  case 'c': Lcd_Buffer[LcdPointer]= code_gen_reg[c_CHAR];
                            break;

                  case 'C': Lcd_Buffer[LcdPointer]= code_gen_reg[0x0C];
                            break;

                  case 'D': Lcd_Buffer[LcdPointer]= code_gen_reg[0];
                            break;

                  case 'd': Lcd_Buffer[LcdPointer]= code_gen_reg[0x0D];
                            break;

                  case 'e':
                  case 'E': Lcd_Buffer[LcdPointer]= code_gen_reg[0x0E];
                            break;

                  case 'f':
                  case 'F': Lcd_Buffer[LcdPointer]= code_gen_reg[0x0F];
                            break;

                  case 'O': Lcd_Buffer[LcdPointer]= code_gen_reg[0x00];
                            break;

                  case 'o': Lcd_Buffer[LcdPointer]= code_gen_reg[o_CHAR];
                            break;

                  case 'R': Lcd_Buffer[LcdPointer]= code_gen_reg[R_CHAR];
                            break;

                  case 'r': Lcd_Buffer[LcdPointer]= code_gen_reg[r_CHAR];
                            break;

                  case 'П':
                  case 'N': Lcd_Buffer[LcdPointer]= code_gen_reg[N_CHAR];
                            break;

                  case 'n': Lcd_Buffer[LcdPointer]= code_gen_reg[n_CHAR];
                            break;

                  case 'T':
                  case 't': Lcd_Buffer[LcdPointer]= code_gen_reg[t_CHAR];
                            break;

                  case 'U': Lcd_Buffer[LcdPointer]= code_gen_reg[U_CHAR];
                            break;

                  case 'u': Lcd_Buffer[LcdPointer]= code_gen_reg[u_CHAR];
                            break;

                  case 's':
                  case 'S': Lcd_Buffer[LcdPointer]= code_gen_reg[0x05];
                            break;

                  case 'Y': Lcd_Buffer[LcdPointer]= code_gen_reg[Y_CHAR];
                            break;

                  case 'L': Lcd_Buffer[LcdPointer]= code_gen_reg[L_CHAR];
                            break;

                  case 'i':
                  case 'I':
                  case 'l': Lcd_Buffer[LcdPointer]= code_gen_reg[l_CHAR];
                            break;
                  case 'p':
                  case 'P': Lcd_Buffer[LcdPointer]= code_gen_reg[P_CHAR];
                            break;

                  case 'H': Lcd_Buffer[LcdPointer]= code_gen_reg[H_CHAR];
                            break;

                  case 'h': Lcd_Buffer[LcdPointer]= code_gen_reg[H_CHAR];
                            break;

                  case 'g': Lcd_Buffer[LcdPointer]= code_gen_reg[g_CHAR];
                            break;

                  case 'J': Lcd_Buffer[LcdPointer]= code_gen_reg[J_CHAR];
                            break;
                            
                  case 'M': Lcd_Buffer[LcdPointer]= code_gen_reg[M_CHAR];
                            break;

                  case 'X': Lcd_Buffer[LcdPointer]= code_gen_reg[X_CHAR];
                            break;

                  case 'Г': Lcd_Buffer[LcdPointer]= code_gen_reg[RG_CHAR];
                            break;                            
                            
                  case 'У': Lcd_Buffer[LcdPointer]= code_gen_reg[RU_CHAR];
                            break;

                  case 'K': Lcd_Buffer[LcdPointer]= code_gen_reg[K_CHAR];
                            break;                            

                  case 'И': Lcd_Buffer[LcdPointer]= code_gen_reg[RI_CHAR];
                            break;

                  case 'Т': Lcd_Buffer[LcdPointer]= code_gen_reg[RT_CHAR];
                            break;

                  case 'д': Lcd_Buffer[LcdPointer]= code_gen_reg[Rd_CHAR];
                            break;
                            
                  case 'Б': Lcd_Buffer[LcdPointer]= code_gen_reg[RB_CHAR];
                            break;                            
                            
                  default:  Lcd_Buffer[LcdPointer]= code_gen_reg[SPACE_C];
                  }

  if(++LcdPointer == 10) LcdPointer= 0;

  return c;
}

static void put_one_char(char c, void *dummy)
{
  putchar (c);
  (void)dummy;  /* Warning on this line OK (Optimized Away) */
}

int printf(const char *format, ...)                    /* Our main entry */
{
  va_list ap;
  int nr_of_chars;

  LcdPointer= 0;
  va_start(ap, format);      /* Variable argument begin */
  nr_of_chars = _formatted_write(format, put_one_char, (void *) 0, ap);
  va_end(ap);                /* Variable argument end */
  convert_2_HT1621();
  return nr_of_chars;        /* According to ANSI */
}
