/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Common Header for S5P6442 machines
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_S5P6442_COMMON_H
#define __ARCH_ARM_MACH_S5P6442_COMMON_H

void s5p6442_register_clocks(void);
void s5p6442_setup_clocks(void);

extern  int s5p6442_init(void);
extern void s5p6442_init_irq(void);
extern void s5p6442_map_io(void);
extern void s5p6442_init_clocks(int xtal);

extern void s5p6442_init_uarts(struct s3c2410_uartcfg *cfg, int no);

#endif /* __ARCH_ARM_MACH_S5P6442_COMMON_H */
