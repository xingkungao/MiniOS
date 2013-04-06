#ifndef __VM_H__
#define __VM_H__

#define KOFFSET         0xC0000000
#define MEM_MAX         (128 * 1024 * 1024)

#define va_to_pa(addr)  (((uint32_t)(addr)) - KOFFSET)
#define pa_to_va(addr)  (((uint32_t)(addr)) + KOFFSET)

void init_kvm(void);
void init_seg(void);

#endif
