/*
 * Phoenix-RTOS
 *
 * Operating system kernel
 *
 * STM32U575xx basic peripherals control functions
 *
 * Copyright 2017, 2020, 2021 Phoenix Systems
 * Author: Aleksander Kaminski, Pawel Pisarczyk, Hubert Buczynski
 *
 * This file is part of Phoenix-RTOS.
 *
 * %LICENSE%
 */

#ifndef _HAL_STM32U5_H_
#define _HAL_STM32U5_H_

#include "../types.h"


/* Special functions pin description */
typedef struct {
	unsigned int port;
	unsigned int pin;
	unsigned int af;
} gpio_pindesc_t;


/* STM32U5 peripherals */
enum {
	/* AHB1 */
	pctl_gpdma1 = 0, pctl_cordic, pctl_fmac, pctl_mdf1, pctl_flash = 8, pctl_crc = 12,
	pctl_tsc = 16, pctl_ramcfg, pctl_dma2d, pctl_gtzc1 = 24, pctl_bkpsram = 28,
	pctl_dcache1 = 30, pctl_sram1,

	/* AHB2_1  */
	pctl_gpioa = 32, pctl_gpiob, pctl_gpioc, pctl_gpiod, pctl_gpioe, pctl_gpiof, pctl_gpiog, pctl_gpioh, pctl_gpioi,
	pctl_adc1 = 32 + 10, pctl_otgfs = 32 + 12, pctl_otg = 32 + 14,
	pctl_aes = 32 + 16, pctl_hash, pctl_rng, pctl_pka, pctl_saes, pctl_octospim,
	pctl_otfdec1 = 32 + 23, pctl_otfdec2, pctl_sdmmc1 = 32 + 27, pctl_sdmmc2,
	pctl_sram2 = 32 + 30, pctl_sram3,

	/* AHB2_2  */
	pctl_fsmc = 64, pctl_octospi1 = 64 + 4, pctl_octospi2 = 64 + 8,

	/* AHB3 */
	pctl_lpgpio1 = 96, pctl_pwr = 96 + 2, pctl_adc4 = 96 + 5, pctl_dac1,
	pctl_lpdma1 = 96 + 9, pctl_adf1, pctl_gtzc2 = 96 + 12, pctl_sram4 = 96 + 31,

	/* APB1_1 */
	pctl_tim2 = 128, pctl_tim3, pctl_tim4, pctl_tim5, pctl_tim6, pctl_tim7, pctl_wwdg = 128 + 11, pctl_spi2 = 128 + 14, 
	pctl_usart2 = 128 + 17, pctl_usart3, pctl_uart4, pctl_uart5, pctl_i2c1,	pctl_i2c2, pctl_crs = 128 + 24,

	/* APB1_2 */
	pctl_i2c4 = 160 + 1, pctl_lptim2 = 160 + 5, pctl_fdcan1 = 160 + 9, pctl_ucpd1 = 160 + 23,

	/* APB2 */
	pctl_tim1 = 192 + 11, pctl_spi1, pctl_tim8, pctl_usart1, pctl_tim15 = 192 + 16, pctl_tim16, pctl_tim17,
	pctl_sai1 = 192 + 21, pctl_sai2,

	/* APB3 */
	pctl_syscfg = 224 + 1, pctl_spi3 = 224 + 5, pctl_lpuart1, pctl_i2c3,
	pctl_lptim1 = 224 + 11, pctl_lptim3, pctl_lptim4, pctl_opamp, pctl_comp,
	pctl_vref = 224 + 20, pctl_rtcapb,

	/* MISC */
	pctl_rtc = 256
};


/* STM32U5 Interrupt numbers */
enum { wwdq_irq = 16, pvd_pvm_irq, rtc_irq, rtc_s_irq, tamp_irq, ramcfg_irq, flash_irq,
	flash_s_irq, gtzc_irq, rcc_irq, rcc_s_irq, exti0_irq, exti1_irq, exti2_irq, exti3_irq,
	exti4_irq, exti5_irq, exti6_irq, exti7_irq, exti8_irq, exti9_irq, exti10_irq, exti11_irq,
	exti12_irq, exti13_irq, exti14_irq, exti15_irq, iwdg_irq, saes_irq, gpdma1_ch0_irq,
	gpdma1_ch1_irq, gpdma1_ch2_irq,	gpdma1_ch3_irq, gpdma1_ch4_irq, gpdma1_ch5_irq, gpdma1_ch6_irq,
	gpdma1_ch7_irq, adc1_irq, dac1_irq, fdcan1_it0_irq, fdcan1_it1_irq, tim1_brk_terr_ierr_irq,
	tim1_up_irq, tim1_trg_com_dir_idx_irq, tim1_cc_irq, tim2_irq, tim3_irq, tim4_irq, tim5_irq,
	tim6_irq, tim8_irq, tim8_brk_terr_ierr_irq,	tim8_up_irq, tim8_trg_com_dir_idx_irq, tim8_cc_irq,
	i2c1_ev_irq, i2c1_er_irq, i2c2_ev_irq, i2c2_er_irq, spi1_irq, spi2_irq, usart1_irq, usart2_irq,
	usart3_irq, uart4_irq, uart5_irq, lpuart1_irq, lptim1_irq, lptim2_irq, tim15_irq, tim16_irq, tim17_irq,
	comp_irq, otg_fs_irq, crs_irq, fmc_irq, octospi1_irq, pwr_s3wu_irq, sdmmc1_irq, sdmmc2_irq,
	gpdma1_ch8_irq, gpdma1_ch9_irq, gpdma1_ch10_irq, gpdma1_ch11_irq, gpdma1_ch12_irq, gpdm1_ch13_irq, gpdma1_ch14_irq, 
	gpdma1_ch15_irq, i2c3_ev_irq, i2c3_er_irq, sai1_irq, sai2_irq, tsc_irq, aes_irq, rng_irq, fpu_irq, hash_irq,
	pka_irq, lptim3_irq, spi3_irq, i2c4_er_irq, i2c4_ev_irq, mdf1_flt0_irq, mdf1_flt1_irq, mdf1_flt2_irq, mdf1_flt3_irq,ucpd1_irq, icache_irq, otfdec1_irq, otfdec2_irq, lptim4_irq, dcache1_irq,
	adf1_flt0_irq, adc4_irq, lpdma1_ch0_irq, lpdma1_ch1_irq, lpdma1_ch2_irq, lpdma1_ch3_irq, dma2d_irq,
	dcmi_pssi_irq, octospi2_irq, mdf1_flt4_irq, mdf1_flt5_irq, cordic_irq, fmac_irq, lsecss_msi_pll_unlock_irq };


/* Sets peripheral clock */
extern int _stm32_rccSetDevClock(unsigned int d, u32 hz);


extern int _stm32_rccGetDevClock(unsigned int d, u32 *hz);


extern u32 _stm32_rccGetCPUClock(void);



extern int _stm32_gpioConfig(unsigned int d, u8 pin, u8 mode, u8 af, u8 otype, u8 ospeed, u8 pupd);


extern int _stm32_gpioSet(unsigned int d, u8 pin, u8 val);


extern int _stm32_gpioSetPort(unsigned int d, u16 val);


extern int _stm32_gpioGet(unsigned int d, u8 pin, u8 *val);


extern int _stm32_gpioGetPort(unsigned int d, u16 *val);


extern int _stm32_getFlashBank(void);


extern void _stm32_switchFlashBank(int bank);


/* Range = 0 - forbidden, 1 - 1.8V, 2 - 1.5V, 3 - 1.2V */
extern void _stm32_pwrSetCPUVolt(u8 range);


extern void _stm32_pwrEnterLPRun(u32 state);


extern time_t _stm32_pwrEnterLPStop(time_t us);


extern void _stm32_rtcUnlockRegs(void);


extern void _stm32_rtcLockRegs(void);


extern u32 _stm32_rtcGetms(void);


extern int _stm32_systickInit(u32 interval);


extern void _stm32_systickDone(void);


extern void _stm32_wdgReload(void);


extern int _stm32_systickInit(u32 interval);


extern void _stm32_init(void);

#endif
