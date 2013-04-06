/* start.S的主要功能是切换在实模式工作的处理器到32位保护模式。
 * C语言代码的功能与游戏基本相同，是将操作系统内核载入内存。
 * 需要特别注意的是，内核使用了分页虚拟存储管理，在链接时指定
 * 的地址是虚拟地址(0xc0100000处)，但装载必须装载在物理地址
 * (0x100000处)，因此必须在此处进行地址转换。
 * 磁盘镜像的结构如下：
    +-----------+------------------+        .-----------------+
    |  引导块   |  内核二进制代码  |   ...                    |
    +-----------+------------------+        '-----------------+
 */

#include "boot.h"

#define SECTSIZE 512
#define VA_TO_PA(addr) ((unsigned)(addr) - 0xc0000000)

void readseg(unsigned char *, int, int);

void
bootmain(void) {
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char* pa, *i;

	/* 因为引导扇区只有512字节，我们设置了堆栈从0x8000向下生长。
	 * 我们需要一块连续的空间来容纳ELF文件头，因此选定了0x8000。 */
	elf = (struct ELFHeader*)0x8000;

	/* 读入ELF文件头 */
	readseg((unsigned char*)elf, 4096, 0);

	/* 把每个program segement依次读入内存 */
	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;
	for(; ph < eph; ph ++) {
		pa = (unsigned char*)VA_TO_PA(ph->paddr); /* 获取物理地址 */
		readseg(pa, ph->filesz, ph->off); /* 读入数据 */
		for (i = pa + ph->filesz; i < pa + ph->memsz; *i ++ = 0);
	}

	((void(*)(void))VA_TO_PA(elf->entry))();
}

void
waitdisk(void) {
	while((in_byte(0x1F7) & 0xC0) != 0x40); /* 等待磁盘完毕 */
}

/* 读磁盘的一个扇区 */
void
readsect(void *dst, int offset) {
	int i;
	waitdisk();
	out_byte(0x1F2, 1);
	out_byte(0x1F3, offset);
	out_byte(0x1F4, offset >> 8);
	out_byte(0x1F5, offset >> 16);
	out_byte(0x1F6, (offset >> 24) | 0xE0);
	out_byte(0x1F7, 0x20);

	waitdisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = in_long(0x1F0);
	}
}

/* 将位于磁盘offset位置的count字节数据读入物理地址pa */
void
readseg(unsigned char *pa, int count, int offset) {
	unsigned char *epa;
	epa = pa + count;
	pa -= offset % SECTSIZE;
	offset = (offset / SECTSIZE) + 1;
	for(; pa < epa; pa += SECTSIZE, offset ++)
		readsect(pa, offset);
}
