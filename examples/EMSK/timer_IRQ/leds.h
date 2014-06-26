#ifndef _LEDS_H_
#define _LEDS_H_

#define	GPIO_BASE_ADDR	0x00002000U

#define PORT_DATA 0
#define PORT_DDR  1
#define PORT_CTRL 2

#define PORT_OFFSET_A (0 * 3)
#define PORT_OFFSET_B (1 * 3)
#define PORT_OFFSET_C (2 * 3)
#define PORT_OFFSET_D (3 * 3)

// LED masks
#define	LED0		(1 << 0)
#define	LED1		(1 << 1)
#define	LED2		(1 << 2)
#define	LED3		(1 << 3)
#define	LED4		(1 << 4)
#define	LED5		(1 << 5)
#define	LED6		(1 << 6)
#define	LED7		(1 << 7)
#define	LED8		(1 << 8)
#define LED_ALL		(LED0 | LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7 | LED8)



void LEDs_Init(unsigned long ulGPIOBaseAddress);
void LEDs_Write(unsigned long ulGPIOBaseAddress, unsigned long ulLEDs);

#endif //!_LEDS_H_
