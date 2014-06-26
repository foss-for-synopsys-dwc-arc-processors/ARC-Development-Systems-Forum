#ifndef _CACHECTRL_H_
#define _CACHECTRL_H_

//cache ctrl registers
#define I_CACHE_BUILD	0x77
#define AUX_IVIC	0x10
#define D_CACHE_BUILD	0x72
#define DC_IVDC	0x47
#define DC_CTRL	0x48
#define FS_MASK	0x100


//invalidate functions
void _invalidate_dcache(void);
void _invalidate_icache(void);

#endif //!_CACHECTRL_H_
