/*
 * Phoenix-RTOS
 *
 * plo - operating system loader
 *
 * Peripherals definitions for armv7m7-stm32l4x6
 *
 * Copyright 2020, 2021 Phoenix Systems
 * Author: Hubert Buczynski, Aleksander Kaminski
 *
 * This file is part of Phoenix-RTOS.
 *
 * %LICENSE%
 */


#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/* Periperals configuration */

/* Interrupts */
#define SIZE_INTERRUPTS (125 + 16)

/* UART */
#define UART_MAX_CNT 5

#ifndef PLO_UART1
#define PLO_UART1 1
#endif

#ifndef PLO_UART2
#define PLO_UART2 0
#endif

#ifndef PLO_UART3
#define PLO_UART3 0
#endif

#ifndef PLO_UART4
#define PLO_UART4 0
#endif

#ifndef PLO_UART5
#define PLO_UART5 0
#endif

#ifndef UART_CONSOLE
#define UART_CONSOLE 1
#endif

/* TODO: USART and LPUART */

/* UART1 */
#define UART1_RX_PA10 { .port = pctl_gpioa, .pin = 10, .af = 7 }
#define UART1_RX_PB7  { .port = pctl_gpiob, .pin = 7, .af = 7 }
#define UART1_RX_PG10 { .port = pctl_gpiog, .pin = 10, .af = 7 }

#define UART1_TX_PA9 { .port = pctl_gpioa, .pin = 9, .af = 7 }
#define UART1_TX_PB6 { .port = pctl_gpiob, .pin = 6, .af = 7 }
#define UART1_TX_PG9 { .port = pctl_gpiog, .pin = 9, .af = 7 }

/* UART2 */
#define UART2_RX_PA3  { .port = pctl_gpioa, .pin = 3, .af = 7 }
#define UART2_RX_PD6  { .port = pctl_gpiod, .pin = 6, .af = 7 }
#define UART2_RX_PA15 { .port = pctl_gpioa, .pin = 15, .af = 3 }

#define UART2_TX_PA2 { .port = pctl_gpioa, .pin = 2, .af = 7 }
#define UART2_TX_PD5 { .port = pctl_gpiod, .pin = 5, .af = 7 }

/* UART3 */
#define UART3_RX_PA5  { .port = pctl_gpioa, .pin = 5, .af = 7 }
#define UART3_RX_PB11 { .port = pctl_gpiob, .pin = 11, .af = 7 }
#define UART3_RX_PC5  { .port = pctl_gpioc, .pin = 5, .af = 7 }
#define UART3_RX_PC11 { .port = pctl_gpioc, .pin = 11, .af = 7 }
#define UART3_RX_PD9  { .port = pctl_gpiod, .pin = 9, .af = 7 }

#define UART3_TX_PA7  { .port = pctl_gpioa, .pin = 7, .af = 7 }
#define UART3_TX_PB10 { .port = pctl_gpiob, .pin = 10, .af = 7 }
#define UART3_TX_PC4  { .port = pctl_gpioc, .pin = 4, .af = 7 }
#define UART3_TX_PC10 { .port = pctl_gpioc, .pin = 10, .af = 7 }
#define UART3_TX_PD8  { .port = pctl_gpiod, .pin = 8, .af = 7 }

/* UART4 */
#define UART4_RX_PA1  { .port = pctl_gpioa, .pin = 1, .af = 8 }
#define UART4_RX_PC11 { .port = pctl_gpioc, .pin = 11, .af = 8 }

#define UART4_TX_PA0  { .port = pctl_gpioa, .pin = 0, .af = 8 }
#define UART4_TX_PC10 { .port = pctl_gpioc, .pin = 10, .af = 8 }


/* UART5 */
#define UART5_TX_PD2  { .port = pctl_gpiod, .pin = 2, .af = 8 }

#define UART5_TX_PC12 { .port = pctl_gpioc, .pin = 12, .af = 8 }

#ifndef UART1_RX
#define UART1_RX UART1_RX_PA10
#endif

#ifndef UART1_TX
#define UART1_TX UART1_TX_PA9
#endif

#ifndef UART2_RX
#define UART2_RX UART2_RX_PD6
#endif

#ifndef UART2_TX
#define UART2_TX UART2_TX_PD5
#endif

#ifndef UART3_RX
#define UART3_RX UART3_RX_PA5
#endif

#ifndef UART3_TX
#define UART3_TX UART3_TX_PA7
#endif

#ifndef UART4_RX
#define UART4_RX UART4_RX_PA1
#endif

#ifndef UART4_TX
#define UART4_TX UART4_TX_PA0
#endif

#ifndef UART5_RX
#define UART5_RX UART5_TX_PD2
#endif

#ifndef UART5_TX
#define UART5_TX UART5_TX_PC12
#endif


#define UART_BAUDRATE 115200

#define UART1_BASE ((void *)0x50013800)
#define UART2_BASE ((void *)0x50004400)
#define UART3_BASE ((void *)0x50004800)
#define UART4_BASE ((void *)0x50004c00)
#define UART5_BASE ((void *)0x50005000)

#define UART1_CLK pctl_usart1
#define UART2_CLK pctl_usart2
#define UART3_CLK pctl_usart3
#define UART4_CLK pctl_uart4
#define UART5_CLK pctl_uart5

#define UART1_IRQ usart1_irq
#define UART2_IRQ usart2_irq
#define UART3_IRQ usart3_irq
#define UART4_IRQ uart4_irq
#define UART5_IRQ uart5_irq

#define FLASH_PROGRAM_1_ADDR    0x0c000000
#define FLASH_PROGRAM_2_ADDR    0x0c100000
#define FLASH_PROGRAM_BANK_SIZE (1024 * 1024)

#endif
