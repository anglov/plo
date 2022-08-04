/*
 * Phoenix-RTOS
 *
 * plo - operating system loader
 *
 * Platform configuration file
 *
 * Copyright 2020-2022 Phoenix Systems
 * Author: Hubert Buczynski, Aleksander Kaminski, Andrzej Glowinski
 *
 * This file is part of Phoenix-RTOS.
 *
 * %LICENSE%
 */


#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifndef __ASSEMBLY__

#include "stm32u5.h"
#include "peripherals.h"
#include "../types.h"

#include <phoenix/arch/syspage-stm32.h>
#include <phoenix/syspage.h>

#include "../../cpu.h"

#define PATH_KERNEL "phoenix-armv8m33-stm32u575xx.elf"

#endif


/* Import platform specific definitions */
#include "ld/armv8m33-stm32u575xx.ldt"

#endif
