在打开中断前执行：

init_hal() in hal.c
	hal是hardware abstraction layer的缩写，提供设备的注册、统一的读写机制
	
init_timer() in time/timer.c
	初始化时钟驱动相关的内容，包括添加中断处理程序、读取启动时的时间、设置8253定时器等
	
init_tty() in tty/tty.c
	初始化终端驱动相关的内容，包括添加中断处理程序、初始化界面等
	
创建一个入口地址为ttyd (tty/ttyd.c) 的内核线程，并设置变量TTY为它的线程号
	这是终端驱动程序的主线程
	
test() in test.c
	为每个终端都创建一个线程，用于处理终端的输入
