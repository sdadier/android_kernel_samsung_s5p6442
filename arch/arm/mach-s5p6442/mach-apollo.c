/* linux/arch/arm/mach-s5p6442/mach-apollo.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * Maintainers: Mark Kennard	<komcomputers@gmail.com>
 *		Miki Dahab	<codermoikop@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/i2c.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <mach/map.h>
#include <mach/regs-clock.h>

#include <plat/regs-serial.h>
#include <plat/s5p6442.h>
#include <plat/devs.h>
#include <plat/cpu.h>
#include <plat/iic.h>

#include <asm/hardware/vic.h>

/* Following are default values for UCON, ULCON and UFCON UART registers */
#define apollo_UCON_DEFAULT	(S3C2410_UCON_TXILEVEL |	\
				 S3C2410_UCON_RXILEVEL |	\
				 S3C2410_UCON_TXIRQMODE |	\
				 S3C2410_UCON_RXIRQMODE |	\
				 S3C2410_UCON_RXFIFO_TOI |	\
				 S3C2443_UCON_RXERR_IRQEN)

#define apollo_ULCON_DEFAULT	S3C2410_LCON_CS8

#define apollo_UFCON_DEFAULT	(S3C2410_UFCON_FIFOMODE |	\
				 S5PV210_UFCON_TXTRIG4 |	\
				 S5PV210_UFCON_RXTRIG4)

static struct s3c2410_uartcfg apollo_uartcfgs[] __initdata = {
	[0] = {
		.hwport		= 0,
		.flags		= 0,
		.ucon		= apollo_UCON_DEFAULT,
		.ulcon		= apollo_ULCON_DEFAULT,
		.ufcon		= apollo_UFCON_DEFAULT,
	},
	[1] = {
		.hwport		= 1,
		.flags		= 0,
		.ucon		= apollo_UCON_DEFAULT,
		.ulcon		= apollo_ULCON_DEFAULT,
		.ufcon		= apollo_UFCON_DEFAULT,
	},
	[2] = {
		.hwport		= 2,
		.flags		= 0,
		.ucon		= apollo_UCON_DEFAULT,
		.ulcon		= apollo_ULCON_DEFAULT,
		.ufcon		= apollo_UFCON_DEFAULT,
	},
};

static struct platform_device *apollo_devices[] __initdata = {
	&s3c_device_i2c0,
	&samsung_asoc_dma,
	&s5p6442_device_iis0,
	&s3c_device_wdt,
};

static struct i2c_board_info apollo_i2c_devs0[] __initdata = {
	{ I2C_BOARD_INFO("wm8580", 0x1b), },
};

static void __init apollo_map_io(void)
{
	s5p_init_io(NULL, 0, S5P_VA_CHIPID);
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(apollo_uartcfgs, ARRAY_SIZE(apollo_uartcfgs));
}

static void __init apollo_machine_init(void)
{
	s3c_i2c0_set_platdata(NULL);
	i2c_register_board_info(0, apollo_i2c_devs0,
			ARRAY_SIZE(apollo_i2c_devs0));
	platform_add_devices(apollo_devices, ARRAY_SIZE(apollo_devices));
}

static void __init apollo_fixup(struct tag *tags, char **cmdline,
	struct meminfo *mi)
{
	mi->nr_banks = 1;
	mi->bank[0].start = PHYS_OFFSET;
	mi->bank[0].size = 256 * SZ_1M;
}

MACHINE_START(APOLLO, "APOLLO")
	.atag_offset	= 0x100,

	.fixup		= apollo_fixup,

	.init_irq	= s5p6442_init_irq,
	.handle_irq	= vic_handle_irq,
	.map_io		= apollo_map_io,
	.init_machine	= apollo_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END

