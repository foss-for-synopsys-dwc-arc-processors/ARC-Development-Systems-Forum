#ifndef _SETVECT_H_
#define _SETVECT_H_

#if __arc__
    // the vector base reg contains the full 32-bit address of the vector table
    #define INT_VECTOR_BASE 0x25
#else
    #error Why not ARC?
#endif

    #define AV2_INTERRUPTS 1
    #define IRQ_INTERRUPT 0x40b
    #define IRQ_PRIORITY  0x206


void _setvecti(int vector,  void (*target)()) ;
void _setvectx(int vector,  void (*target)());
void _setvecti_prior(int vector,  void (*target)(), unsigned priority) ;
void _setvectfi(int vector,  void (*target)()) ;

#endif //!_SETVECT_H_
