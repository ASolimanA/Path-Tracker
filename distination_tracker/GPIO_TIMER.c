#include "TIVA_HEADERS.h"
#include "GPIO_TIMER.h"

void turn_On_LED(unsigned char data)
{
	GPIO_PORTF_DATA_R |= data;
}

void LED_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x20;		 										 	// Activate PortF (IF NOT ACTIVATED)
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0) {};	// Waiting for Ready
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;		 							// UNLOCK PORTF 
	GPIO_PORTF_CR_R |= LED_MASK;											  // Allow changes to LED ports
	GPIO_PORTF_AFSEL_R &= ~LED_MASK; 										// Disable Alternative Function
	GPIO_PORTF_AMSEL_R &= ~LED_MASK; 										// Disable Analog Function
	GPIO_PORTF_DEN_R |= LED_MASK; 											// Enable Digital Function
	GPIO_PORTF_PCTL_R &= ~0xFFF0; 											// Clear PCTL for digital function
	GPIO_PORTF_DIR_R |= LED_MASK; 											// Set LEDs to output
	GPIO_PORTF_DATA_R &= ~LED_MASK; 										// Clear Data
}

void SW_Init(void) {
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= SW_MASK; //allow changes for port 1,2,3 for portf
	GPIO_PORTF_AMSEL_R &= ~SW_MASK;
	GPIO_PORTF_AFSEL_R &= ~SW_MASK;
	GPIO_PORTF_PCTL_R &= ~GPIO_PCTL_PF0_M; //clear bits in PCTL
	GPIO_PORTF_PCTL_R &= ~GPIO_PCTL_PF4_M;
	GPIO_PORTF_DIR_R &= ~SW_MASK;
	GPIO_PORTF_DEN_R |= SW_MASK;
	GPIO_PORTF_PUR_R = SW_MASK;
}

void turn_Off_LED(unsigned char data)
{
	GPIO_PORTF_DATA_R &= ~data;
}
void LED_Reset(void)
{
	GPIO_PORTF_DATA_R &= ~LED_MASK;
}

void LED_Wait(void)
{
	GPIO_PORTF_DATA_R |= (Red | Green);
}

unsigned char getData_LED(void)
{
	return GPIO_PORTF_DATA_R & LED_MASK;
}

void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0x00FFFFFF;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x5;
}

void SysTick_Delay10ms(void)
{
	NVIC_ST_RELOAD_R = 159999;
	NVIC_ST_CURRENT_R = 0;
	while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {};
}


void Delay_in_seconds(unsigned char seconds)
{
	int i;
	for (i = 0; i < 100 * seconds; i++) SysTick_Delay10ms();
}

unsigned char SW2_Input(void)
{
	return GPIO_PORTF_DATA_R & SW2_MASK;
}
unsigned char SW1_Input(void)
{
	return GPIO_PORTF_DATA_R & SW1_MASK;
}

unsigned char SW1_ON(void)
{
	return (!(GPIO_PORTF_DATA_R & SW1_MASK));
}

unsigned char SW2_ON(void)
{
	return (!(GPIO_PORTF_DATA_R & SW2_MASK));
}

