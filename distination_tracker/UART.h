#ifndef _UART_H_
#define _UART_H_
#define CR 0x0D 
void UART_Init_pc(void); // init_uart_pc

void UART_OutChar_pc(char data); // write uart data

char UART_InChar_pc(); // read uart data



void UART_Init_gps(void); // init_uart_gps

void UART_OutChar_gps(char data); // write uart data

char UART_InChar_gps(); // read uart data



#endif
