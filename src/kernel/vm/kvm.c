#include "vm.h"
#include "x86.h"
#include "string.h"

#define PG_ALIGN __attribute((aligned(PG_SIZE)))
PDE kpdir[NR_PDE] PG_ALIGN;            // the kernel page directory
PTE kptab[MEM_MAX / PG_SIZE] PG_ALIGN; // the kernel page tables
SegDesc gdt[NR_SEG];                   // the new GDT
TSS tss;                               // one TSS for all ring 3 processes

void
init_kvm(void) { // setup kernel memory mapping
	uint32_t pa;
	PTE *ptab = (PTE*)va_to_pa(kptab);
	PDE *pdir = (PDE*)va_to_pa(kpdir);
	for (pa = 0; pa < MEM_MAX; pa += PG_SIZE) {
		*ptab = PTE_P | PTE_W | pa;
		ptab ++;
	}
	memset((void*)va_to_pa(pdir), 0, PG_SIZE);
	for (pa = 0; pa < MEM_MAX; pa += PG_SIZE * NR_PTE) {
		*(pdir + KOFFSET / PG_SIZE / NR_PTE) = *pdir =
			PTE_P | PTE_W | va_to_pa(&kptab[pa / PG_SIZE]);
		pdir ++;
	}
	set_cr3((void*)va_to_pa(kpdir));  // set PDBR(CR3)
	set_cr0(get_cr0() | CR0_PG);      // enable paging
}

void
init_seg() { // setup kernel segements
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_KERN);
	gdt[SEG_KDATA] = SEG(STA_W,         0,       0xffffffff, DPL_KERN);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA] = SEG(STA_W,         0,       0xffffffff, DPL_USER);
	gdt[SEG_TSS] = SEG16(STS_T32A,      &tss, sizeof(tss)-1, DPL_KERN);
	set_gdt(gdt, sizeof(gdt));
}
