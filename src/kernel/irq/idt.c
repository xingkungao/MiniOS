#include "x86.h"
#include "irq.h"

struct GateDesc idt[NR_IRQ];

void
init_idt(void) {
	int i;
	for (i = 0; i < NR_IRQ; i ++) {
		idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), irqall, DPL_KERN);
	}

	// exceptions
	idt[0]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec0,   DPL_KERN);
	idt[1]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec1,   DPL_KERN);
	idt[2]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec2,   DPL_KERN);
	idt[3]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec3,   DPL_KERN);
	idt[4]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec4,   DPL_KERN);
	idt[5]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec5,   DPL_KERN);
	idt[6]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec6,   DPL_KERN);
	idt[7]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec7,   DPL_KERN);
	idt[8]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec8,   DPL_KERN);
	idt[9]    = GATE(STS_TG32, KSEL(SEG_KCODE), vec9,   DPL_KERN);
	idt[10]   = GATE(STS_TG32, KSEL(SEG_KCODE), vec10,  DPL_KERN);
	idt[11]   = GATE(STS_TG32, KSEL(SEG_KCODE), vec11,  DPL_KERN);
	idt[12]   = GATE(STS_TG32, KSEL(SEG_KCODE), vec12,  DPL_KERN);
	idt[13]   = GATE(STS_TG32, KSEL(SEG_KCODE), vec13,  DPL_KERN);
	idt[14]   = GATE(STS_TG32, KSEL(SEG_KCODE), vec14,  DPL_KERN);

	// interrupts
	idt[32]   = GATE(STS_IG32, KSEL(SEG_KCODE), irq0,   DPL_KERN);
	idt[33]   = GATE(STS_IG32, KSEL(SEG_KCODE), irq1,   DPL_KERN);
	idt[46]   = GATE(STS_IG32, KSEL(SEG_KCODE), irq14,  DPL_KERN);

	// system call
	idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_USER);

	set_idt(idt, sizeof(idt));
}

