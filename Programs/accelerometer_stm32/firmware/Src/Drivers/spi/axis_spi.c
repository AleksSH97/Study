/**
 ******************************************************************************
 * @file           : axis_spi.c
 * @author         : Aleksandr Shabalin       <alexnv97@gmail.com>
 * @brief          : SPI1 usage file
 ******************************************************************************
 * ----------------- Copyright (c) 2022 Aleksandr Shabalin------------------- *
 ******************************************************************************
 ******************************************************************************
 */

/******************************************************************************/
/* Includes ----------------------------------------------------------------- */
/******************************************************************************/
#include "axis_spi.h"

/******************************************************************************/
/* Private variables -------------------------------------------------------- */
/******************************************************************************/
SPI_HandleTypeDef hspi1;

/******************************************************************************/
/* Private function prototypes ---------------------------------------------- */
/******************************************************************************/
static uint8_t axis_spi_write_read(uint8_t byte);

/**
 * @brief           SPI1 init fn
 */
void axis_spi_init(void)
{
    axis_io_it_config();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = AXIS_SCK_PIN|AXIS_MISO_PIN|AXIS_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_RCC_SPI1_CLK_ENABLE();

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT; // Check this our again. Was configured with error! SPI was not transferring
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    hspi1.Init.CRCPolynomial = 7; // Check this our again. Was configured with error! SPI was not transferring

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
      Error_Handler();
    }
}
/******************************************************************************/

/**
 * @brief          CS pin init fn
 */
void axis_cs_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = AXIS_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(AXIS_CS_GPIO_PORT, &GPIO_InitStruct);

    AXIS_CS_HIGH();
}
/******************************************************************************/

/**
  * @brief  Configures the Accelerometer INT2.
  *         EXTI0 is already used by user button so INT1 is not configured here.
  * @param  None
  * @retval None
  */
void axis_io_it_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = AXIS_INT2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    HAL_NVIC_SetPriority((IRQn_Type)AXIS_INT2_EXTI_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ((IRQn_Type)AXIS_INT2_EXTI_IRQn);
}
/******************************************************************************/

/**
  * @brief  Writes one byte to the Accelerometer.
  * @param  buf_ptr: pointer to the buffer containing the data to be written to the Accelerometer.
  * @param  write_addr: Accelerometer's internal address to write to.
  * @param  num_byte_to_write: Number of bytes to write.
  * @retval None
  */
void axis_io_write(uint8_t *buf_ptr, uint8_t write_addr, uint16_t num_byte_to_write)
{
    if (num_byte_to_write > 0x01) {
        write_addr |= (uint8_t)MULTIPLEBYTE_CMD;
    }

    AXIS_CS_LOW();

    axis_spi_write_read(write_addr);

    while (num_byte_to_write >= 0x01) {
        axis_spi_write_read(*buf_ptr);
        num_byte_to_write--;
        buf_ptr++;
    }

    AXIS_CS_HIGH();
}
/******************************************************************************/

/**
  * @brief  Reads a block of data from the Accelerometer.
  * @param  buf_ptr: pointer to the buffer that receives the data read from the Accelerometer.
  * @param  read_addr: Accelerometer's internal address to read from.
  * @param  num_byte_to_read: number of bytes to read from the Accelerometer.
  * @retval None
  */
void axis_io_read(uint8_t *buf_ptr, uint8_t read_addr, uint16_t num_byte_to_read)
{
    if (num_byte_to_read > 0x01) {
        read_addr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
    }
    else {
        read_addr |= (uint8_t)READWRITE_CMD;
    }

    AXIS_CS_LOW();

    axis_spi_write_read(read_addr);

    while (num_byte_to_read > 0x01) {
        *buf_ptr = axis_spi_write_read(DUMMY_BYTE);
        num_byte_to_read--;
        buf_ptr++;
    }

    AXIS_CS_HIGH();
}
/******************************************************************************/

/**
 * @brief          SPI1 Transmit and Receive fn
 */
uint8_t axis_spi_write_read(uint8_t byte)
{
    uint8_t received_byte = 0;

    if (HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&byte, (uint8_t*)&received_byte, 1, AXIS_SPI_TIMEOUT) != HAL_OK) {
        log_printf_crlf("Error transition SPI");
    }

    return received_byte;
}
/******************************************************************************/

/**
 * @brief          Error callback fn
 */
void axis_spi_error(void)
{
    // TODO code with error statement
}
/******************************************************************************/
