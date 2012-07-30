/* linux/arch/arm/mach-s5p6442/mach-apollo.c
 *
 * Copyright (c) 2009-2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/pwm_backlight.h>
#include <linux/fb.h>
#include <linux/mmc/host.h>

#include <video/platform_lcd.h>

#include <asm/hardware/vic.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/irq.h>
#include <asm/mach-types.h>
#include <asm/setup.h>


#include <mach/hardware.h>
#include <mach/map.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

#include <plat/regs-serial.h>
#include <plat/gpio-cfg.h>
#include <plat/clock.h>
#include <plat/devs.h>
#include <plat/cpu.h>
#include <plat/iic.h>
#include <plat/pll.h>
#include <plat/adc.h>
#include <plat/ts.h>
#include <plat/s5p-time.h>
#include <plat/backlight.h>
#include <plat/fb.h>
#include <plat/regs-fb.h>
#include <plat/sdhci.h>

#include "common.h"

#define APOLLO_UCON_DEFAULT	(S3C2410_UCON_TXILEVEL |	\
				S3C2410_UCON_RXILEVEL |		\
				S3C2410_UCON_TXIRQMODE |	\
				S3C2410_UCON_RXIRQMODE |	\
				S3C2410_UCON_RXFIFO_TOI |	\
				S3C2443_UCON_RXERR_IRQEN)

#define APOLLO_ULCON_DEFAULT	S3C2410_LCON_CS8

#define APOLLO_UFCON_DEFAULT	(S3C2410_UFCON_FIFOMODE |	\
				S3C2440_UFCON_TXTRIG16 |	\
				S3C2410_UFCON_RXTRIG8)

static struct s3c2410_uartcfg apollo_uartcfgs[] __initdata = {
	[0] = {
		.hwport		= 0,
		.flags		= 0,
		.ucon		= APOLLO_UCON_DEFAULT,
		.ulcon		= APOLLO_ULCON_DEFAULT,
		.ufcon		= APOLLO_UFCON_DEFAULT,
	},
	[1] = {
		.hwport		= 1,
		.flags		= 0,
		.ucon		= APOLLO_UCON_DEFAULT,
		.ulcon		= APOLLO_ULCON_DEFAULT,
		.ufcon		= APOLLO_UFCON_DEFAULT,
	},
	[2] = {
		.hwport		= 2,
		.flags		= 0,
		.ucon		= APOLLO_UCON_DEFAULT,
		.ulcon		= APOLLO_ULCON_DEFAULT,
		.ufcon		= APOLLO_UFCON_DEFAULT,
	},
};

/* Frame Buffer */
static struct s3c_fb_pd_win apollo_fb_win0 = {
	.win_mode = {
		.left_margin	= 8,
		.right_margin	= 13,
		.upper_margin	= 7,
		.lower_margin	= 5,
		.hsync_len	= 3,
		.vsync_len	= 1,
		.xres		= 400,
		.yres		= 240,
	},
	.max_bpp	= 32,
	.default_bpp	= 24,
};

struct platform_device apollo_device_battery = {
		.name	= "apollo-battery",
		.id		= -1,
};

static struct platform_device *apollo_devices[] __initdata = {
	&s3c_device_adc,
	&s3c_device_rtc,
	&s3c_device_i2c0,
	&s3c_device_i2c1,
	&s3c_device_ts,
	&s3c_device_wdt,
	&samsung_asoc_dma,
	&s3c_device_fb,
	&s3c_device_hsmmc0,
	&s3c_device_hsmmc1,
	&s3c_device_hsmmc2,

	&s3c_device_timer[0],
	&s3c_device_timer[1],

};

static struct s3c_sdhci_platdata apollo_hsmmc0_pdata __initdata = {
	.cd_type	= S3C_SDHCI_CD_NONE,
};

static struct s3c_sdhci_platdata apollo_hsmmc1_pdata __initdata = {
	.cd_type	= S3C_SDHCI_CD_NONE,
};

static struct s3c_sdhci_platdata apollo_hsmmc2_pdata __initdata = {
	.cd_type	= S3C_SDHCI_CD_NONE,
};

void s5p6442_i2c0_cfg_gpio(struct platform_device *dev)
{
	//s3c_gpio_cfgall_range(S5P6442_GPB(5), 2,
	//		      S3C_GPIO_SFN(2), S3C_GPIO_PULL_UP);
}

void s5p6442_i2c1_cfg_gpio(struct platform_device *dev)
{
	//s3c_gpio_cfgall_range(S5P6442_GPR(9), 2,
	//		      S3C_GPIO_SFN(6), S3C_GPIO_PULL_UP);
}	

static struct s3c2410_platform_i2c s5p6442_i2c0_data __initdata = {
	.flags		= 0,
	.slave_addr	= 0x10,
	.frequency	= 100*1000,
	.sda_delay	= 100,
	.cfg_gpio	= s5p6442_i2c0_cfg_gpio,
};

static struct s3c2410_platform_i2c s5p6442_i2c1_data __initdata = {
	.flags		= 0,
	.bus_num	= 1,
	.slave_addr	= 0x10,
	.frequency	= 100*1000,
	.sda_delay	= 100,
	.cfg_gpio	= s5p6442_i2c1_cfg_gpio,
};

static struct i2c_board_info apollo_i2c_devs0[] __initdata = {
	{ I2C_BOARD_INFO("24c08", 0x57), },
	{ I2C_BOARD_INFO("wm8580", 0x1a), },
};

static struct i2c_board_info apollo_i2c_devs1[] __initdata = {
	/* To be populated */
};

/* LCD Backlight data */

/* ADD LCD BACKLIGHT CODE HERE */
/* ADD LCD CODE HERE */
/* ADD LCD POWER CODE HERE */

static void __init apollo_map_io(void)
{
	s5p_init_io(NULL, 0, S5P_VA_CHIPID);
//	s5p_init_io(NULL, 0);
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(apollo_uartcfgs, ARRAY_SIZE(apollo_uartcfgs));
	s5p_set_timer_source(S5P_PWM3, S5P_PWM4);
}

static void __init apollo_fixup(struct tag *tags, char **cmdline,
	struct meminfo *mi)
{
	mi->nr_banks = 1;
	mi->bank[0].start = PHYS_OFFSET;
	mi->bank[0].size = 256 * SZ_1M;
}

static void __init apollo_machine_init(void)
{
	printk("%s\n", __func__);
	
	s3c24xx_ts_set_platdata(NULL);

	s3c_i2c0_set_platdata(&s5p6442_i2c0_data);
	s3c_i2c1_set_platdata(&s5p6442_i2c1_data);
	i2c_register_board_info(0, apollo_i2c_devs0,
			ARRAY_SIZE(apollo_i2c_devs0));
	i2c_register_board_info(1, apollo_i2c_devs1,
			ARRAY_SIZE(apollo_i2c_devs1));

	//samsung_bl_set(&apollo_bl_gpio_info, &apollo_bl_data);
	//s5p6440_set_lcd_interface();
	//s3c_fb_set_platdata(&apollo_lcd_pdata);

	s3c_sdhci0_set_platdata(&apollo_hsmmc0_pdata);
	s3c_sdhci1_set_platdata(&apollo_hsmmc1_pdata);
	s3c_sdhci2_set_platdata(&apollo_hsmmc2_pdata);

	platform_add_devices(apollo_devices, ARRAY_SIZE(apollo_devices));
}

MACHINE_START(APOLLO, "APOLLO")
	/* Maintainers: Mark Kennard <komcomputers@gmail.com>
			Miki Dahab <codermoikop@gmail.com>*/
	.atag_offset	= 0x100,

	.fixup		= apollo_fixup,

	.init_irq	= s5p6442_init_irq,
	.handle_irq	= vic_handle_irq,
	.map_io		= apollo_map_io,
	.init_machine	= apollo_machine_init,
	.timer		= &s5p_timer,
	.restart	= s5p6442_restart,
MACHINE_END
