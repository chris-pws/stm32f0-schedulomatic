#ifndef LOWLEVEL_H_

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

// ******* sys_tick_handler *******
// Built-in SysTick ISR function. Runs the event manager and increments the counter.
// Inputs: none
// Outputs: none
void rcc_init(void);

// ******* sys_tick_handler *******
// Built-in SysTick ISR function. Runs the event manager and increments the counter.
// Inputs: none
// Outputs: none
void gpio_init(void);

// LED for debugging
#define PORT_LED GPIOB
#define GPIO_LED GPIO8

//PORT A
#define SPI_PORT GPIOA
//PA8
#define RST	GPIO8
//PA4
#define CSN	GPIO4
//PA5
#define SCK	GPIO5
//PA7
#define MOSI GPIO7
//PA6
#define MISO GPIO6


#define LOWLEVEL_H_ 1
#endif