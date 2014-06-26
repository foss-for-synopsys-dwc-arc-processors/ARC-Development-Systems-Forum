#include <stdlib.h>
#include "setvect.h"
#include "cachectrl.h"

void _setvecti(int vector,  void (*target)()) {
    // make uncached so .di suffix used for stores.
     unsigned long * pIVT = (unsigned long *)__builtin_arc_lr(INT_VECTOR_BASE);
     pIVT[vector] = (unsigned long)target;
    // since vectors are data on AV2, need to invalidate dcache, also
    // (flush of line would have been sufficent)
    _invalidate_dcache();
    _invalidate_icache();
    }

void _setvectx(int vector, void (*target)()) {
    _setvecti(vector, ( void (*)()) target);
    }

void _setvecti_prior(int vector,  void (*target)(), unsigned priority) {
    unsigned state = __builtin_arc_clri();
    __builtin_arc_sr(vector, IRQ_INTERRUPT);
    __builtin_arc_sr(priority, IRQ_PRIORITY);
    __builtin_arc_seti(state);
    _setvecti(vector, (void (*)()) target);
    }
void _setvectfi(int vector, void (*target)()) {
    _setvecti_prior(vector, ( void (*)()) target, 0);
    }

