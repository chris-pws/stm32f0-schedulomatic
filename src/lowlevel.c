#include "lowlevel.h"

// ******* sys_tick_handler *******
// Built-in SysTick ISR function. Runs the event manager and increments the counter.
// Inputs: none
// Outputs: none
void rcc_init(void) {
	rcc_clock_setup_in_hsi_out_48mhz();
	rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_DMA);
}

// ******* sys_tick_handler *******
// Built-in SysTick ISR function. Runs the event manager and increments the counter.
// Inputs: none
// Outputs: none
void gpio_init(void) {

	// USART peripheral for serial debug via /tty/ACMx
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2|GPIO3);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO2|GPIO3);

	// LED for debugging purposes
	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LED);
	// Line for timing/debugging measurements
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
	
	// SPI software
	gpio_mode_setup(SPI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, RST|CSN|SCK|MOSI);
	gpio_set_output_options(SPI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, RST|CSN|SCK|MOSI);
	//gpio_mode_setup(SPI_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, MISO);

	// SPI peripheral pins
}

