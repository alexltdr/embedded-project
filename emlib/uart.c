#include <stdint.h>
#include "uart.h"
#define GPIOAEN        (1U<<0)
#define UART2EN        (1U<<17)
#define DBG_UART_BAUDRATE        115200
#define SYS_FREQ                16000000
#define APB1_CLK                SYS_FREQ
#define CR1_TE                (1U<<3)
#define CR1_UE                (1U<<13)
#define SR_TXE                (1U<<7)
static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate);
static void uart_write(int ch);



int __io_putchar(int ch)
{
    uart_write(ch);
    return ch;
}

#include <sys/types.h>   // pour ssize_t
						 
int _write(int file, char *ptr, int len)
{
	(void)file;  // on ignore le descripteur de fichier
	for (int i = 0; i < len; i++) {
		uart_write(ptr[i]);
	}
	return len;
}
void uart_init(void)
{
    /*Activer l'accès à l'horloge de GPIOA*/
    RCC->AHB1ENR |= GPIOAEN;
    /*Configurer le mode de PA2 en mode fonction alternative*/
    GPIOA->MODER &=~(1U<<4);
    GPIOA->MODER |=(1U<<5);
    /*Définir le type de fonction alternative sur AF7 (UART2_TX)*/
    GPIOA->AFR[0] |=(1U<<8);
    GPIOA->AFR[0] |=(1U<<9);
    GPIOA->AFR[0] |=(1U<<10);
    GPIOA->AFR[0] &=~(1U<<11);
    /*Activer l'accès à l'horloge de UART2*/
     RCC->APB1ENR |=    UART2EN;
    /*Configurer le débit en bauds de l'UART*/
      uart_set_baudrate(APB1_CLK,DBG_UART_BAUDRATE);
    /*Configurer la direction du transfert*/
     USART2->CR1 = CR1_TE;
    /*Activer le module UART*/
     USART2->CR1 |= CR1_UE;
}
static void uart_write(int ch)
{
    /*S'assurer que le registre de données de transmission est vide*/
    while(!(USART2->SR & SR_TXE)){}
    /*Écrire dans le registre de données de transmission*/
    USART2->DR =(ch & 0xFF);
}
static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate)
{
    return((periph_clk + (baudrate/2U))/baudrate);
}
static void uart_set_baudrate(uint32_t periph_clk,uint32_t baudrate)
{
    USART2->BRR = compute_uart_bd(periph_clk,baudrate);
}
