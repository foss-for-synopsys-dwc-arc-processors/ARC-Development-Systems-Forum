#include "leds.h"
//initialize PORT B as output 
void LEDs_Init(unsigned long ulGPIOBaseAddress)
{
	unsigned long * pPortB_Base = (unsigned long *)(ulGPIOBaseAddress + PORT_OFFSET_B * 4);
	pPortB_Base[PORT_DATA] = LED_ALL;
	pPortB_Base[PORT_DDR] = LED_ALL;
}
//write LEDs
void LEDs_Write(unsigned long ulGPIOBaseAddress, unsigned long ulLEDs)
{
	((unsigned long *)(ulGPIOBaseAddress + PORT_OFFSET_B * 4))[PORT_DATA] =  ulLEDs;
}
