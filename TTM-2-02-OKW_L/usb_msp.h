#define GET_STATUS_DEVICE         0x8000
#define GET_STATUS_INTERF         0x8100
#define GET_STATUS_ENDPNT         0x8200

#define SET_ADDRESS               0x0005

#define GET_DESCRIPTOR_DEVICE     0x8006
#define GET_DESCRIPTOR_INTERF     0x8106
#define GET_DESCRIPTOR_ENDPNT     0x8206

#define DEVICE_DESCRIPTOR         0x01
#define CONFIG_DESCRIPTOR         0x02
#define STRING_DESCRIPTOR         0x03
#define INTERF_DESCRIPTOR         0x04
#define ENDPNT_DESCRIPTOR         0x05
#define HID_DESCRIPTOR            0x21
#define REPORT_DESCRIPTOR         0x22

#define GET_CONFIGURATION         0x8008
#define SET_CONFIGURATION         0x0009

#define GET_INTERFACE             0x810A
#define SET_INTERFACE             0x010B
#define GET_REPORT                0xA101
#define SET_REPORT                0x2109
#define SET_IDLE                  0x210A

#define INPUT_REPORT              1
#define OUTPUT_REPORT             2
#define FEATURE_REPORT            3

#define ENP_IN                    0
#define ENP_OUT                   1

#define ASK_USBEXCH               0
#define NAK_USBEXCH               255

#define USB_DISCONNECT            USBCNF&= ~PUR_EN
#define USB_CONNECT               USBCNF|=  PUR_EN
#define CHECK_USB_POWER_BOOT      (USBPWRCTL & USBBGVBV)
#define CHECK_USB_POWER           (USBPWRCTL & (VBOFFIFG + VBONIFG))
#define CHECK_USB_RESET           USBIFG & RSTRIFG
#define CHECK_USB_SETUP           USBIFG & SETUPIFG
#define CHECK_USB_OEP_0_IF        USBOEPIFG & BIT0
#define CHECK_USB_IEP_0_IF        USBIEPIFG & BIT0
#define SET_USB_IEP_0_IF          USBIEPIFG|=  BIT0
#define CLR_USB_IEP_0_IF          USBIEPIFG&= ~BIT0
#define CLR_USB_OEP_0_IF          USBOEPIFG&= ~BIT0
#define USB_DATA_IN               USBCTL|=  DIR
#define USB_DATA_OUT              USBCTL&= ~DIR
#define CLR_NAK_OEP_0             USBOEPCNT_0&= ~NAK
#define CLR_NAK_IEP_0             USBIEPCNT_0&= ~NAK
#define SET_NAK_OEP_0             USBOEPCNT_0|=  NAK
#define SET_NAK_IEP_0             USBIEPCNT_0|=  NAK
#define USB_SETSTALL_EP_0         { USBIEPCNF_0|= STALL; USBOEPCNF_0|= STALL; }
#define SEND_ASK_IEP_0            USBIEPCNT_0=   0
#define SEND_ASK_OEP_0            USBOEPCNT_0=   0

#define USB_Command_Buffer(A)     *(pInt8U)(USBSUBLK_+(A))
#define sdRequest                 ((Int16U) (USB_Command_Buffer(0)<<8) + USB_Command_Buffer(1))
#define wValue                    (*((pInt16U) &USB_Command_Buffer(2)))
#define wIndex                    (*((pInt16U) &USB_Command_Buffer(4)))
#define wLenght                   (*((pInt16U) &USB_Command_Buffer(6)))
#define bReportID                 USB_Command_Buffer(2)
#define bReportType               USB_Command_Buffer(3)
#define lValue                    USB_Command_Buffer(2)
#define hValue                    USB_Command_Buffer(3)

#define USB_SETUP_BUF             (pInt8U)USBSUBLK_
#define USB_OEP0_BUF              (pInt8U)USBOEP0BUF_
#define USB_IEP0_BUF              (pInt8U)USBIEP0BUF_

#define SIZE_ENP_IN               8
#define SIZE_ENP_OUT              8

void                              setup_packet(void);
Int8U                             USBHW_Init(void);
void                              USB_Reset(void);
void                              binbuffer_2_binbuffer(Int8U __data20 * pBufferSource,Int8U __data20 * pBufferDestination,Int16U nBytes);
void                              ep0_out_packet(void);
void                              ep0_in_packet(void);
void                              usb_exchange(void);
void                              usb_control(void);
