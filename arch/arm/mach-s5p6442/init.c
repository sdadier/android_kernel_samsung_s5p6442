/* linux/arch/arm/mach-s5p6442/s5p6442-init.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/serial_core.h>

#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/regs-serial.h>
#include <mach/map.h>

/* uart registration process */
void __init s5p6442_init_uarts(struct s3c2410_uartcfg *cfg, int no)
{
	int uart;

	for (uart = 0; uart < no; uart++) {
		s5p_uart_resources[uart].resources->start = S5P_PA_UART(uart);
		s5p_uart_resources[uart].resources->end = S5P_PA_UART(uart) + S5P_SZ_UART;
	}

	s3c24xx_init_uartdevs("s5pv210-uart", s5p_uart_resources, cfg, no);
}
