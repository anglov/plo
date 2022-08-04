/*
 * Phoenix-RTOS
 *
 * plo - operating system loader
 *
 * STM32L4x6 basic peripherals control functions
 *
 * Copyright 2017, 2019, 2020, 2021 Phoenix Systems
 * Author: Aleksander Kaminski, Jan Sikorski, Hubert Buczynski
 *
 * This file is part of Phoenix-RTOS.
 *
 * %LICENSE%
 */

#include <hal/hal.h>
#include "stm32u5.h"

#include "../../cpu.h"

static struct {
	volatile u32 *rcc;
	volatile u32 *gpio[9];
	volatile u32 *syst; /* TODO: Move to common code */
	volatile u32 *pwr;
	volatile u32 *syscfg;
	volatile u32 *iwdg;
	volatile u32 *flash;

	u32 cpuclk;

	u32 resetFlags;
} stm32_common;

/* clang-format off */
enum { ahb1_begin = pctl_gpdma1, ahb1_end = pctl_sram1, ahb2_1_begin = pctl_gpioa, ahb2_1_end = pctl_sram3,
	ahb2_2_begin = pctl_fsmc, ahb2_2_end = pctl_octospi2, ahb3_begin = pctl_lpgpio1, ahb3_end = pctl_sram4, 
	apb1_1_begin = pctl_tim2, apb1_1_end = pctl_crs, apb1_2_begin = 160, apb1_2_end = pctl_ucpd1,
	apb2_begin = 192, apb2_end = pctl_sai2, apb3_begin = 224, apb3_end = pctl_rtcapb };


enum { rcc_cr = 0, rcc_icscr1 = rcc_cr + 2, rcc_icscr2, rcc_icscr3, rcc_crrcr,
	rcc_cfgr1 = rcc_crrcr + 2, rcc_cfgr2, rcc_cfgr3, rcc_pll1cfgr, rcc_pll2cfgr, rcc_pll3cfgr,
	rcc_pll1divr, pll1fracr, rcc_pll2divr, pll2fracr, rcc_pll3divr, pll3fracr,
	rcc_cier = pll3fracr + 2,  rcc_cifr, rcc_cicr, 
	rcc_ahb1rstr = rcc_cicr + 2, rcc_ahb2rstr1, rcc_ahb2rstr2, rcc_ahb3rstr,
	rcc_apb1rstr1 = rcc_ahb3rstr + 2, rcc_apb1rstr2, rcc_apb2rstr, rcc_apb3rstr,
	rcc_ahb1enr = rcc_apb3rstr + 2, rcc_ahb2enr1, rcc_ahb2enr2, rcc_ahb3enr,
	rcc_apb1enr1 = rcc_ahb3enr + 2, rcc_apb1enr2, rcc_apb2enr, rcc_apb3enr,
	rcc_ahb1smenr = rcc_apb3enr + 2, rcc_ahb2smenr1, rcc_ahb2smenr2, rcc_ahb3smenr,
	rcc_apb1smenr1 = rcc_ahb3smenr + 2, rcc_apb1smenr2, rcc_apb2smenr, rcc_apb3smenr,
	rcc_srdarm = rcc_apb3smenr + 2,
	rcc_ccipr1 = rcc_srdarm + 2, rcc_ccipr2, rcc_ccipr3,
	rcc_bdcr = rcc_ccipr3 + 2, rcc_csr,
	rcc_seccfgr = rcc_csr + 7, rcc_privcfgr	};


enum { gpio_moder = 0, gpio_otyper, gpio_ospeedr, gpio_pupdr, gpio_idr,
	gpio_odr, gpio_bsrr, gpio_lckr, gpio_afrl, gpio_afrh, gpio_brr, gpio_hslvr, gpio_seccfgr };


enum { pwr_cr1 = 0, pwr_cr2, pwr_cr3, pwr_vosr, pwr_svmcr, pwr_wucr1, pwr_wucr2, pwr_wucr3,
	pwr_bdcr1, pwr_bdcrr2,pwr_dbpr, pwr_ucpdr, pwr_seccfgr, pwr_privcfgr, pwr_sr, pwr_svmsr,
	pwr_bdsr, pwr_wusr, pwr_wuscr, pwr_apcr, pwr_pucra, pwr_pdcra, pwr_pucrb, pwr_pdcrb,
	pwr_pucrc, pwr_pdcrc, pwr_pucrd, pwr_pdcrd, pwr_pucre, pwr_pdcre, pwr_pucrf, pwr_pdcrf,
	pwr_pucrg, pwr_pdcrg, pwr_pucrh, pwr_pdcrh, pwr_pucri, pwr_pdcri  };


/* TODO: Move to common code */
enum { syst_csr = 0, syst_rvr, syst_cvr, syst_calib };


enum { syscfg_seccfgr = 0, syscfg_cfgr1, syscfg_fpuimr, syscfg_cnslckr, syscfg_cslockr,
	syscfg_cfgr2, syscfg_mesr, syscfg_cccsr, syscfg_ccvr, syscfg_cccr, syscfg_rsscmdr };


enum { iwdg_kr = 0, iwdg_pr, iwdg_rlr, iwdg_sr, iwdg_winr, iwdg_ewcr };

/* clang-format on*/


/* RCC (Reset and Clock Controller) */


int _stm32_rccSetDevClock(unsigned int d, u32 hz)
{
	u32 t;

	hz = !!hz;

	if (d <= ahb1_end) {
		t = *(stm32_common.rcc + rcc_ahb1enr) & ~(1 << (d - ahb1_begin));
		*(stm32_common.rcc + rcc_ahb1enr) = t | (hz << (d - ahb1_begin));
	}
	else if (d <= ahb2_1_end) {
		t = *(stm32_common.rcc + rcc_ahb2enr1) & ~(1 << (d - ahb2_1_begin));
		*(stm32_common.rcc + rcc_ahb2enr1) = t | (hz << (d - ahb2_1_begin));
	}
	else if (d <= ahb2_2_end) {
		t = *(stm32_common.rcc + rcc_ahb2enr2) & ~(1 << (d - ahb2_2_begin));
		*(stm32_common.rcc + rcc_ahb2enr2) = t | (hz << (d - ahb2_2_begin));
	}
	else if (d <= ahb3_end) {
		t = *(stm32_common.rcc + rcc_ahb3enr) & ~(1 << (d - ahb3_begin));
		*(stm32_common.rcc + rcc_ahb3enr) = t | (hz << (d - ahb3_begin));
	}
	else if (d <= apb1_1_end) {
		t = *(stm32_common.rcc + rcc_apb1enr1) & ~(1 << (d - apb1_1_begin));
		*(stm32_common.rcc + rcc_apb1enr1) = t | (hz << (d - apb1_1_begin));
	}
	else if (d <= apb1_2_end) {
		t = *(stm32_common.rcc + rcc_apb1enr2) & ~(1 << (d - apb1_2_begin));
		*(stm32_common.rcc + rcc_apb1enr2) = t | (hz << (d - apb1_2_begin));
	}
	else if (d <= apb2_end) {
		t = *(stm32_common.rcc + rcc_apb2enr) & ~(1 << (d - apb2_begin));
		*(stm32_common.rcc + rcc_apb2enr) = t | (hz << (d - apb2_begin));
	}
	else if (d <= apb3_end) {
		t = *(stm32_common.rcc + rcc_apb3enr) & ~(1 << (d - apb3_begin));
		*(stm32_common.rcc + rcc_apb3enr) = t | (hz << (d - apb3_begin));
	}
	else if (d == pctl_rtc) {
		t = *(stm32_common.rcc + rcc_bdcr) & ~(1 << 15);
		*(stm32_common.rcc + rcc_bdcr) = t | (hz << 15);
	}
	else {
		return -1;
	}

	hal_cpuDataMemoryBarrier();

	return 0;
}


int _stm32_rccGetDevClock(unsigned int d, u32 *hz)
{
	if (d <= ahb1_end) {
		*hz = !!(*(stm32_common.rcc + rcc_ahb1enr) & (1 << (d - ahb1_begin)));
	}
	else if (d <= ahb2_1_end) {
		*hz = !!(*(stm32_common.rcc + rcc_ahb2enr1) & (1 << (d - ahb2_1_begin)));
	}
	else if (d <= ahb2_2_end) {
		*hz = !!(*(stm32_common.rcc + rcc_ahb2enr2) & (1 << (d - ahb2_2_begin)));
	}
	else if (d <= ahb3_end) {
		*hz = !!(*(stm32_common.rcc + rcc_ahb3enr) & (1 << (d - ahb3_begin)));
	}
	else if (d <= apb1_1_end) {
		*hz = !!(*(stm32_common.rcc + rcc_apb1enr1) & (1 << (d - apb1_1_begin)));
	}
	else if (d <= apb1_2_end) {
		*hz = !!(*(stm32_common.rcc + rcc_apb1enr2) & (1 << (d - apb1_2_begin)));
	}
	else if (d <= apb2_end){
		*hz = !!(*(stm32_common.rcc + rcc_apb2enr) & (1 << (d - apb2_begin)));
	}
	else if (d <= apb3_end){
		*hz = !!(*(stm32_common.rcc + rcc_apb3enr) & (1 << (d - apb3_begin)));
	}
	else if (d == pctl_rtc) {
		*hz = !!(*(stm32_common.rcc + rcc_bdcr) & (1 << 15));
	}
	else {
		return -1;
	}

	return 0;
}


u32 _stm32_rccGetCPUClock(void)
{
	return stm32_common.cpuclk;
}


/* SysTick */


int _stm32_systickInit(u32 interval)
{
	u64 load = ((u64)interval * stm32_common.cpuclk) / 1000000;
	if (load > 0x00ffffff)
		return -1;

	*(stm32_common.syst + syst_rvr) = (u32)load;
	*(stm32_common.syst + syst_cvr) = 0;

	/* Enable systick */
	*(stm32_common.syst + syst_csr) |= 0x7;

	return 0;
}


void _stm32_systickDone(void)
{
	*(stm32_common.syst + syst_csr) = 0;
}


/* GPIO */


int _stm32_gpioConfig(unsigned int d, u8 pin, u8 mode, u8 af, u8 otype, u8 ospeed, u8 pupd)
{
	volatile u32 *base;
	u32 t;

	if (d > pctl_gpioi || pin > 15) {
		return -1;
	}

	base = stm32_common.gpio[d - pctl_gpioa];

	t = *(base + gpio_moder) & ~(0x3 << (pin << 1));
	*(base + gpio_moder) = t | (mode & 0x3) << (pin << 1);

	t = *(base + gpio_otyper) & ~(1 << pin);
	*(base + gpio_otyper) = t | (otype & 1) << pin;

	t = *(base + gpio_ospeedr) & ~(0x3 << (pin << 1));
	*(base + gpio_ospeedr) = t | (ospeed & 0x3) << (pin << 1);

	t = *(base + gpio_pupdr) & ~(0x03 << (pin << 1));
	*(base + gpio_pupdr) = t | (pupd & 0x3) << (pin << 1);

	if (pin < 8) {
		t = *(base + gpio_afrl) & ~(0xf << (pin << 2));
		*(base + gpio_afrl) = t | (af & 0xf) << (pin << 2);
	}
	else {
		t = *(base + gpio_afrh) & ~(0xf << ((pin - 8) << 2));
		*(base + gpio_afrh) = t | (af & 0xf) << ((pin - 8) << 2);
	}

	return 0;
}


/* TODO: change to use gpio_bsrr/gpio_brr? */
int _stm32_gpioSet(unsigned int d, u8 pin, u8 val)
{
	volatile u32 *base;
	u32 t;

	if (d > pctl_gpioi || pin > 15) {
		return -1;
	}

	base = stm32_common.gpio[d - pctl_gpioa];

	t = *(base + gpio_odr) & ~(!(u32)val << pin);
	*(base + gpio_odr) = t | !!(u32)val << pin;

	return 0;
}


int _stm32_gpioSetPort(unsigned int d, u16 val)
{
	volatile u32 *base;

	if (d > pctl_gpioi)
		return -1;

	base = stm32_common.gpio[d - pctl_gpioa];
	*(base + gpio_odr) = val;

	return 0;
}


int _stm32_gpioGet(unsigned int d, u8 pin, u8 *val)
{
	volatile u32 *base;

	if (d > pctl_gpioi || pin > 15)
		return -1;

	base = stm32_common.gpio[d - pctl_gpioa];
	*val = !!(*(base + gpio_idr) & (1 << pin));

	return 0;
}


int _stm32_gpioGetPort(unsigned int d, u16 *val)
{
	volatile u32 *base;

	if (d > pctl_gpioi)
		return -1;

	base = stm32_common.gpio[d - pctl_gpioa];
	*val = *(base + gpio_idr);

	return 0;
}


/* Watchdog */


void _stm32_wdgReload(void)
{
#if defined(WATCHDOG) && defined(NDEBUG)
	*(stm32_common.iwdg + iwdg_kr) = 0xaaaa;
#endif
}


/* TODO: ICACHE/DCACHE */
void _stm32_init(void)
{
	u32 i;
	static const int gpio2pctl[] = { pctl_gpioa, pctl_gpiob, pctl_gpioc,
		pctl_gpiod, pctl_gpioe, pctl_gpiof, pctl_gpiog, pctl_gpioh, pctl_gpioi };

	/* Base addresses init */
	stm32_common.rcc = (void *)0x56020c00;
	stm32_common.pwr = (void *)0x56020800;
	stm32_common.syst = (void *)0xe000e010;
	stm32_common.syscfg = (void *)0x56000400;
	stm32_common.iwdg = (void *)0x50003000;
	stm32_common.gpio[0] = (void *)0x52020000; /* GPIOA */
	stm32_common.gpio[1] = (void *)0x52020400; /* GPIOB */
	stm32_common.gpio[2] = (void *)0x52020800; /* GPIOC */
	stm32_common.gpio[3] = (void *)0x52020c00; /* GPIOD */
	stm32_common.gpio[4] = (void *)0x52021000; /* GPIOE */
	stm32_common.gpio[5] = (void *)0x52021400; /* GPIOF */
	stm32_common.gpio[6] = (void *)0x52021800; /* GPIOG */
	stm32_common.gpio[7] = (void *)0x52021c00; /* GPIOH */
	stm32_common.gpio[8] = (void *)0x52022000; /* GPIOI */

	/* Enable System configuration controller */
	_stm32_rccSetDevClock(pctl_syscfg, 1);

	/* Enable power module */
	_stm32_rccSetDevClock(pctl_pwr, 1);

	/* CPU is set to 4 MHz with MSI clock by default */
	stm32_common.cpuclk = 4 * 1000 * 1000;

	/* Disable all interrupts */
	*(stm32_common.rcc + rcc_cier) = 0;

	hal_cpuDataMemoryBarrier();

	/* GPIO init */
	for (i = 0; i < sizeof(stm32_common.gpio) / sizeof(stm32_common.gpio[0]); ++i) {
		_stm32_rccSetDevClock(gpio2pctl[i], 1);
	}

	hal_cpuDataMemoryBarrier();

#if defined(WATCHDOG) && defined(NDEBUG)
	/* Init watchdog */
	/* Enable write access to IWDG */
	*(stm32_common.iwdg + iwdg_kr) = 0x5555;

	/* Set prescaler to 256, ~30s interval */
	*(stm32_common.iwdg + iwdg_pr) = 0x06;
	*(stm32_common.iwdg + iwdg_rlr) = 0xfff;

	_stm32_wdgReload();

	/* Enable watchdog */
	*(stm32_common.iwdg + iwdg_kr) = 0xcccc;
#endif
}
