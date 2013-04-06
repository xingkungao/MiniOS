#include "x86.h"

#define PORT_PIC_MS    0x20
#define PORT_PIC_SL    0xA0

void init_i8259(void) {
	out_byte(PORT_PIC_MS + 1, 0xFF);
	out_byte(PORT_PIC_SL + 1, 0xFF);
	out_byte(PORT_PIC_MS    , 0x11);
	out_byte(PORT_PIC_MS + 1, 32);
	out_byte(PORT_PIC_MS + 1, 1 << 2);
	out_byte(PORT_PIC_MS + 1, 0x3);
	out_byte(PORT_PIC_SL    , 0x11);
	out_byte(PORT_PIC_SL + 1, 32 + 8);
	out_byte(PORT_PIC_SL + 1, 2);
	out_byte(PORT_PIC_SL + 1, 0x3);

	out_byte(PORT_PIC_MS    , 0x68);
	out_byte(PORT_PIC_MS    , 0x0A);
	out_byte(PORT_PIC_SL    , 0x68);
	out_byte(PORT_PIC_SL    , 0x0A);
}
