#ifndef SYSTEM_H
#define SYSTEM_H

#include "air32f10x.h"
#include <stdint.h>

#define LOG_ON 1

/*****************************************************************************
 * @brief 位带区转位带别名区地址
 *        bit_word_addr =bit_band_base + (byte_offset×32) + (bit_number×4)
 ****************************************************************************/
#define BITBAND(addr, bitnum)       ((addr & 0xF0000000) + 0x02000000) + \
                                    ((addr & 0x000FFFFF) << 5) + (bitnum << 2)
#define MEM_ADDR(addr)              (*((volatile uint32_t *)(addr)))
#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))

/* IO口输出地址映射 */
#define GPIOA_ODR_Addr    (GPIOA_BASE + 12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE + 12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE + 12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE + 12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE + 12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE + 12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE + 12) //0x40011E0C    
/* IO口输入地址映射 */
#define GPIOA_IDR_Addr    (GPIOA_BASE + 8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE + 8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE + 8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE + 8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE + 8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE + 8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE + 8) //0x40011E08 

/* 输出 */
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr, n)
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr, n)
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr, n)
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr, n)
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr, n)
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr, n) 
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr, n)
/* 输入 */
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr, n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr, n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr, n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr, n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr, n)
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr, n)
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr, n)

#if (LOG_ON != 0)
#include <stdio.h>

#define LOG_USART           USART1
#define LOG_CLK                    RCC_APB2Periph_USART1 
#define LOG_TX_PIN                 GPIO_Pin_9
// #define LOG_RX_PIN                 GPIO_Pin_10
#define LOG_GPIO_PORT              GPIOA
#define LOG_GPIO_CLK               RCC_APB2Periph_GPIOA
void log_init(u32 bound);
#define LOG_INIT(band)      log_init(band)
#define LOG(format, ...)    printf(format, ## __VA_ARGS__)
// #define LOG(...)            printf(__VA_ARGS__)
#else
#define LOG_INIT(band)
#define LOG(format, ...)
#endif /* LOG_ON */

void system_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_s(uint8_t ns);

#define err() while(1) {LED1_OUT = 0;}

#endif
