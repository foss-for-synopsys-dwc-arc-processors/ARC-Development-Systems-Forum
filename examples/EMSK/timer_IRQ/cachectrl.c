#include "cachectrl.h"

//invalidate I-cache
void _invalidate_dcache(void)
{
	unsigned int bcr = __builtin_arc_lr(I_CACHE_BUILD);
	if((bcr & 0xFF) >= 0x04)//ARC v2 caches
	{
		__builtin_arc_sr(0xDeadBeef, AUX_IVIC);//invalidate it
		__builtin_arc_nop();//three NOPs (according ISA)
		__builtin_arc_nop();
		__builtin_arc_nop();
	}
}


//invalidate D-cache
void _invalidate_icache(void)
{

	unsigned int bcr = __builtin_arc_lr(D_CACHE_BUILD);
	if((bcr & 0xFF) >= 0x04)//ARC v2 caches
	{
		unsigned int dcctrl = __builtin_arc_lr(DC_CTRL);//save the D-cache state
		__builtin_arc_sr(0xC2, DC_CTRL);//set to invalidate and flush, flush locked lines
		__builtin_arc_sr(0x01, DC_IVDC);//invalidate (and flush)
		__builtin_arc_nop();
		while(__builtin_arc_lr(DC_CTRL) & FS_MASK)
		{
			__builtin_arc_nop();
		}
		__builtin_arc_sr(dcctrl, DC_CTRL);//restore state
	}
}
