#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#include "spi.h"
void spi_gpio_init(GPIO_TypeDef *port, uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t af);
void spi_config(SPI_TypeDef *spi, uint8_t cpol, uint8_t cpha, uint8_t baud_rate);
uint8_t spi_transfer_byte(SPI_TypeDef *spi, uint8_t byte);
void spi_receive(SPI_TypeDef *spi, uint8_t *data, uint32_t size);
 
#endif 
