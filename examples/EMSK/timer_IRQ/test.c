#include <stdio.h>
#include "setvect.h"
#include "leds.h"

//set timer number
#define TIMER_NUMBER 0

//timers BCR
#define TIMER_BUILD 0x75
#define TIMER_BUILD_T0 (1 << 8)
#define TIMER_BUILD_T1 (1 << 9)

//use this register to automatic save context in the stack
#define AUX_IRQ_CTRL   0x0E
//0x1F = NR, number of general purpose regs
//1 << 9 = B, save blink
//1 << 10 = L, save LP_* regs
//1 << 11 = U, save in user space stack
//#define AUX_IRQ_CTRL_SAVE_ALL (0x1F | (1 << 9) | (1 << 10) | (1 << 11))
#define AUX_IRQ_CTRL_SAVE_ALL (0x1F | (1 << 9) | (1 << 10))//use kernel stack (it does not matter for baremetal app)

//timer registers
#define TIMER0_COUNT   0x21
#define TIMER0_CONTROL 0x22
#define TIMER0_LIMIT   0x23

#define TIMER1_COUNT   0x100
#define TIMER1_CONTROL 0x101
#define TIMER1_LIMIT   0x102

#define TIMER_CONTROL_IE (1 << 0)
#define TIMER_CONTROL_NH (1 << 1)
#define TIMER_CONTROL_IP (1 << 3)

//Enables interrupt bit for SETI
#define ENABLE_INTERRUPTS (1 << 4)

//peripheral base address
#define PERIPHERAL_BASE_ADDRESS		0x20a

//counter for timer 0 or 1
volatile unsigned int gn_TimerCount = 0;

//low-level IRQ handler prototypes
//extern void timer0_irq(void);
//extern void timer1_irq(void);
void IRQ_Timer0(void) __attribute__ ((interrupt ("ilink")));
void IRQ_Timer1(void) __attribute__ ((interrupt ("ilink")));

//timer 0 handler, clears interrupt pending bit (IP) and decrements counter
void IRQ_Timer0(void)
{
	unsigned int t_ctrl = __builtin_arc_lr(TIMER0_CONTROL);
	t_ctrl &= ~TIMER_CONTROL_IP;//clear IP bit
	__builtin_arc_sr(t_ctrl, TIMER0_CONTROL);

	gn_TimerCount--;
}

//timer 1 handler, clears interrupt pending bit (IP) and increments counter
void IRQ_Timer1(void)
{
	unsigned int t_ctrl = __builtin_arc_lr(TIMER1_CONTROL);
	t_ctrl &= ~TIMER_CONTROL_IP;//clear IP bit
	__builtin_arc_sr(t_ctrl, TIMER1_CONTROL);

	gn_TimerCount++;
}


//main function
int main(void)
{
	//calculate GPIO base address
	unsigned long ulGPIOBaseAddress = __builtin_arc_lr(PERIPHERAL_BASE_ADDRESS) + GPIO_BASE_ADDR;

	//disable all interrupts 
	__builtin_arc_clri();

	//setup GPIO for LEDs
	LEDs_Init(ulGPIOBaseAddress);

	
	//set timer's vectors for assembler wrapper 
	_setvecti(16, IRQ_Timer0);
	_setvecti(17, IRQ_Timer1);

	//setup automatic (hardware) context saving feature
	__builtin_arc_sr(AUX_IRQ_CTRL_SAVE_ALL, AUX_IRQ_CTRL);

	//setup timer
#if TIMER_NUMBER == 0
	__builtin_arc_sr(0x1FFFFF, TIMER0_LIMIT);//set limit 
	__builtin_arc_sr(TIMER_CONTROL_IE|TIMER_CONTROL_NH, TIMER0_CONTROL);//enable interrupt
	__builtin_arc_sr(0x00, TIMER0_COUNT);//reset counter
	//disable interrupt from second timer (if exists)
	if(__builtin_arc_lr(TIMER_BUILD) & TIMER_BUILD_T1) __builtin_arc_sr(__builtin_arc_lr(TIMER1_CONTROL) & ~(TIMER_CONTROL_IE | TIMER_CONTROL_IP), TIMER1_CONTROL);
#else 
	__builtin_arc_sr(0xFFFFF, TIMER1_LIMIT);//set limit 
	__builtin_arc_sr(TIMER_CONTROL_IE, TIMER1_CONTROL);//enable interrupt
	__builtin_arc_sr(0x00, TIMER1_COUNT);//reset counter

	//disable interrupt from second timer (if exists)
	if(__builtin_arc_lr(TIMER_BUILD) & TIMER_BUILD_T0) __builtin_arc_sr(__builtin_arc_lr(TIMER0_CONTROL) & ~(TIMER_CONTROL_IE | TIMER_CONTROL_IP), TIMER0_CONTROL);

#endif 

	//enable interrupts for levels 2 and lower
	__builtin_arc_seti(ENABLE_INTERRUPTS | 2);
	while(1)
	{
		//set leds
		LEDs_Write(ulGPIOBaseAddress, ~gn_TimerCount);
	}
	//never reach here
	__builtin_unreachable ();
	return 0;
}
