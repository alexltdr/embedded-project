#include "spi.h"
#include "gpio.h"


void spi_gpio_init(GPIO_TypeDef *port, uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t af) {
	rcc_gpio_enable(port);
	gpio_init(port, sck, Alternate);
	gpio_init(port, miso, Alternate);
	gpio_init(port, mosi, Alternate);
	gpio_set_af(MY_GPIOA, sck, 5);  // PA5 → AF5 (SPI1_SCK)
	gpio_set_af(MY_GPIOA, miso, 5);  // PA6 → AF5 (SPI1_MISO)
	gpio_set_af(MY_GPIOA, mosi, 5);  // PA7 → AF5 (SPI1_MOSI)
}

void spi_config(SPI_TypeDef *spi, uint8_t cpol, uint8_t cpha, uint8_t baud_rate) {
    // Activer l'horloge SPI dans RCC
    if (spi == SPI1) {
		rcc_spi1_enable();
    }
    else if (spi == SPI2) {
        RCC->APB1ENR |= (1U << 14);
    }
    else if (spi == SPI3) {
        RCC->APB1ENR |= (1U << 15);
    }

    // Desactiver SPE avant de configurer (obligatoire pour modifier CR1)
    spi->CR1 &= ~SPI_CR1_SPE;

    // 3. Baud rate : bits BR[2:0] = CR1[5:3]
    spi->CR1 &= ~(0x7U << 3);                    // clear BR
    spi->CR1 |=  ((baud_rate & 0x7U) << 3);      // set BR

    // 4. CPOL (bit 1) et CPHA (bit 0)
    spi->CR1 &= ~(1U << 1);
    spi->CR1 |=  ((cpol & 0x1U) << 1);

    spi->CR1 &= ~(1U << 0);
    spi->CR1 |=  ((cpha & 0x1U) << 0);

    // 5. MSTR (bit 2) = 1 → mode maître
    spi->CR1 |= (1U << 2);

    // 6. SSM (bit 9) = 1 et SSI (bit 8) = 1
    //    Gestion logicielle du NSS pour eviter le passage forcé en mode slave
    spi->CR1 |= (1U << 9);   // SSM
    spi->CR1 |= (1U << 8);   // SSI

    // 7. DFF (bit 11) = 0 → 8 bits
    spi->CR1 &= ~(1U << 11);

    // 8. LSBFIRST (bit 7) = 0 → MSB first
    spi->CR1 &= ~(1U << 7);

    // 9. SPE (bit 6) = 1 → activer le SPI (en dernier !)
    spi->CR1 |= (1U << 6);
}

void spi_receive(SPI_TypeDef *spi, uint8_t *data, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        // 1. Attendre que TXE = 1 (buffer d'envoi vide, prêt à écrire)
        while (!(spi->SR & (1U << 1)));

        // 2. Envoyer un octet dummy (0xFF) pour générer 8 coups d'horloge
        //    En SPI, sans envoi il n'y a pas de SCK, donc pas de réception possible
        spi->DR = 0xFF;

        // 3. Attendre que RXNE = 1 (un octet a été reçu sur MISO)
        while (!(spi->SR & (1U << 0)));

        // 4. Lire DR pour récupérer l'octet reçu et le stocker
        data[i] = (uint8_t)spi->DR;
    }
}

uint8_t spi_transfer_byte(SPI_TypeDef *spi, uint8_t byte) {
    // 1. Attendre que TXE = 1 (buffer d'envoi libre)
    while (!(spi->SR & (1U << 1)));

    // 2. Écrire l'octet à envoyer
    spi->DR = byte;

    // 3. Attendre que RXNE = 1 (réponse reçue)
    while (!(spi->SR & (1U << 0)));

    // 4. Retourner l'octet reçu simultanément
    return (uint8_t)spi->DR;
}



