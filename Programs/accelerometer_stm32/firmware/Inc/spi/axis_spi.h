/**
 ******************************************************************************
 * @file           : axis_spi.h
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : SPI1 header
 ******************************************************************************
 * ----------------- Copyright (c) 2022 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AXIS_SPI_H__
#define __AXIS_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lis302dl.h"

#define AXIS_CS_PIN            GPIO_PIN_3                   /* PE.03 */
#define AXIS_CS_GPIO_PORT      GPIOE

#define AXIS_SCK_PIN           GPIO_PIN_5
#define AXIS_MISO_PIN          GPIO_PIN_6
#define AXIS_MOSI_PIN          GPIO_PIN_7

#define AXIS_CS_LOW()          HAL_GPIO_WritePin(AXIS_CS_GPIO_PORT, AXIS_CS_PIN, GPIO_PIN_RESET)
#define AXIS_CS_HIGH()         HAL_GPIO_WritePin(AXIS_CS_GPIO_PORT, AXIS_CS_PIN, GPIO_PIN_SET)

#define AXIS_INT1_PI           GPIO_PIN_0                 /* PE.00 */
#define AXIS_INT1_EXTI_IRQn    EXTI0_IRQn
#define AXIS_INT2_PIN          GPIO_PIN_1                 /* PE.01 */
#define AXIS_INT2_EXTI_IRQn    EXTI1_IRQn

#define AXIS_SPI_TIMEOUT       (uint32_t)0x1000
#define AXIS_NUM_OF_AXES       3u

extern SPI_HandleTypeDef hspi1;
extern struct axis_spi axis_spi;

struct axis_spi {
    int16_t xyz_buf[AXIS_NUM_OF_AXES];
    volatile bool flag;
};

void axis_spi_init(void);
void axis_cs_init(void);
void axis_io_read(uint8_t *buf_ptr, uint8_t read_addr, uint16_t num_byte_to_read);
void axis_io_write(uint8_t *buf_ptr, uint8_t write_addr, uint16_t num_byte_to_write);
void axis_io_it_config(void);

#ifdef __cplusplus
}
#endif

#endif /* __AXIS_SPI_H__ */

