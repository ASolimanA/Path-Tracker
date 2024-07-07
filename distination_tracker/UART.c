
#include "TIVA_HEADERS.h" 
#include "UART.h"
#include <string.h>


void UART_Init_pc(void) // funtion for init uart0
{
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0; // activate uart 0
	while((SYSCTL_PRUART_R & SYSCTL_PRUART_R0)==0){};//delay uart0
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; // activate port A
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R0)==0){};//delay port A
	UART0_CTL_R &= ~UART_CTL_UARTEN; // disable uart0
	UART0_IBRD_R = 0X68;											// IBRD=INT(16*10^6/(9600*16)) =104
	UART0_FBRD_R = 0X0B;											// FBRD=ROUND(0.166666*64) =11
	UART0_LCRH_R = (UART_LCRH_FEN | UART_LCRH_WLEN_8);				// 1110000   ENABLE FIFO + DATA IS 8 BITS
	UART0_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN); // enable RXE, TXE UART
	GPIO_PORTA_AFSEL_R |= 0X03;
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0XFFFFFF00) | (GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
	GPIO_PORTA_DEN_R |= 0X03;	 // enable digital on PA0,PA1
	GPIO_PORTA_AMSEL_R &= ~0X03; // disable analog on PA0,PA1
}


char UART_InChar_pc() // read uart data
{
	while ((UART0_FR_R & UART_FR_RXFE) != 0); // while fifo is empty>>dont read
	return (char)(UART0_DR_R & 0xFF); // we just want the 8 bits
}

void UART_OutChar_pc(char data) // write uart data
{

	while ((UART0_FR_R & UART_FR_TXFF) != 0); // while fifo is full>>dont write
	UART0_DR_R = data; // we just want the 8 bits
}

void UART_Init_gps(void)// for portd uart 2
{
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R2; // activate uart 2 
    while((SYSCTL_PRUART_R & SYSCTL_PRUART_R2)==0);//delay uart2
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3; // activate port D
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R3)==0){};//delay port D
	UART2_CTL_R &= ~UART_CTL_UARTEN; // disable uart2
	UART2_IBRD_R = 0X68;										   // IBRD=INT(16*10^6/(9600*16)) =104 
	UART2_FBRD_R = 0X0B;										   // FBRD=ROUND(0.166666*64) =11
	UART2_LCRH_R = (UART_LCRH_FEN | UART_LCRH_WLEN_8);			   // 1110000   ENABLE FIFO + DATA IS 8 BITS
	UART2_CTL_R |= (UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN); // enable RXE, TXE UART
	GPIO_PORTD_LOCK_R=GPIO_LOCK_KEY;
	GPIO_PORTD_CR_R|=0xC0;
	GPIO_PORTD_AFSEL_R |= ((1 << 6) | (1 << 7)); // Enable alternate function on PD6 and PD7
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0x00FFFFFF) + (GPIO_PCTL_PD6_U2RX | GPIO_PCTL_PD7_U2TX); // Select UART alternate functions for PD6 and PD7
    GPIO_PORTD_DEN_R |= ((1 << 6) | (1 << 7)); // Enable digital I/O on PD6 and PD7
    GPIO_PORTD_AMSEL_R &= ~((1 << 6) | (1 << 7)); // Disable analog functionality on PD6 and PD7

}


char UART_InChar_gps() // read uart data
{
	while ((UART2_FR_R & UART_FR_RXFE) != 0); // while fifo is empty>>dont read
	return (char)UART2_DR_R;  // we just want the 8 bits
}

void UART_OutChar_gps(char data) // write uart data
{

	while ((UART2_FR_R & UART_FR_TXFF) != 0); // while fifo is full>>dont write
	UART2_DR_R = data; // we just want the 8 bits
}





