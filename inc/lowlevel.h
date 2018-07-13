#ifndef LOWLEVEL_H_

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/f0/rcc.h>

// LED for debugging
#define PORT_LED GPIOB
#define GPIO_LED GPIO8
// Pins for troubleshooting timing and logic
#define PORT_SIG GPIOC
#define GPIO_SIG0 GPIO0
#define GPIO_SIG1 GPIO1
#define GPIO_SIG2 GPIO2
#define GPIO_SIG3 GPIO3

// SPI - PORT A
#define PORT_SPI GPIOA
#define SCK	GPIO5 	      // PA5
#define MOSI GPIO7 	      // PA7
#define NSS	GPIO4 	      // PA4
// LED - PORT A
#define PORT_OLED GPIOA
#define RST	GPIO8 	      // PA8
#define DC GPIO9          // PA9

// Buttons - PORT B
#define PORT_BTN GPIOB
#define BTN_LEFT GPIO3 	  // PB3
#define BTN_RIGHT GPIO5   // PB5

// Joystick Left - PORT A
#define JOY_PORT_LEFT GPIOA
#define JOY_X_LEFT GPIO0  // PA0
#define JOY_Y_LEFT GPIO1  // PA1
// Joystick Right - PORT C
#define JOY_PORT_RIGHT GPIOC
#define JOY_X_RIGHT GPIO1 // PC1
#define JOY_Y_LEFT GPIO0  // PC0

// ******* Low_init *******
// Attends to initialization of clocks, pins, and peripherals.
//  Inputs: none
// Outputs: none
void Low_init(void);

// ******* rcc_init *******
// Enables the clock for GPIO and other peripherals.
//  Inputs: none
// Outputs: none
void rcc_init(void);

// ******* gpio_init *******
// Initializes the GPIO registers and sets up peripheral and other output pins.
//  Inputs: none
// Outputs: none
void gpio_init(void);


#define LOWLEVEL_H_ 1
#endif