#ifndef __IRQ_H__
#define __IRQ_H__

void init_idt(void);
void init_i8259(void);

// interrupt and exception entries
void irq0();
void irq1();
void irq14();
void vec0();
void vec1();
void vec2();
void vec3();
void vec4();
void vec5();
void vec6();
void vec7();
void vec8();
void vec9();
void vec10();
void vec11();
void vec12();
void vec13();
void vec14();
void vecsys();
void irqall();

#endif
