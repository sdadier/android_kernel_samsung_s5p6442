/* linux/arch/arm/mach-s5p6442/include/mach/memory.h
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * S5P6442 - Memory definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#ifdef CONFIG_MACH_APOLLO
#define PHYS_OFFSET_ONEDRAM		UL(0x40000000)
#define PHYS_SIZE_ONEDRAM		(80 * SZ_1M)
#define PHYS_SIZE_DDR			(256 * SZ_1M)
#endif

#define CONSISTENT_DMA_SIZE		SZ_8M

#endif /* __ASM_ARCH_MEMORY_H */
