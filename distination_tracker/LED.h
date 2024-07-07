#ifndef _LED_H_
#define _LED_H_


/* ------- Important Notes for LED colors ------- */

// Red -> ERROR
// Green -> DONE
// Blue -> In Progress
// Yellow -> Waiting for interaction OR Waiting for GPS

/* ---------------------------------------------- */
#define LED_MASK 0xE // 0b01110
#define PF_MASK 0x1F // 0b11111
#define SW_MASK 0x11
#define SW1_MASK  0x10
#define SW2_MASK	0x01

#define Red 0x02
#define Blue 0x04
#define Green 0x08


void LED_Init(void);
// LED Initialization

void SW_Init(void);

void turn_On_LED(unsigned char data);
// Turns on Red, Blue or Green LED

void turn_Off_LED(unsigned char data);
// Turns off Red, Blue or Green LED

void LED_Reset(void);
// Turns off ALL LEDs


void LED_Wait(void);
// Turns on Yellow (Wait Indicator)

unsigned char getData_LED(void);
// Read current LED status (PF321 - G B R)

unsigned char SW1_Input(void);
// Reads input from SW1

unsigned char SW2_Input(void);
// Reads input from SW2

unsigned char SW1_ON(void);
// Check if SW1 is ON

unsigned char SW2_ON(void);
// Check if SW2 is ON

void SysTick_Init(void);
// Timer Initialization

void SysTick_Delay10ms(void);
// 10ms Delay (Clock = 16 MHz)

void Delay_in_seconds(unsigned char seconds);
// Utility Function for whatever seconds delay!



#endif
