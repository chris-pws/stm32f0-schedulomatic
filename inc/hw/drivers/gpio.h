#define GPIO_PORT(port, rcc) ((const uint32_t[2]){port, rcc})

#define GPIO_PIN(gpio_port, pin) ((const uint32_t[3]){gpio_port[0], pin, gpio_port[1]})

#define GPIO_CONF(gpio_pin, mode, pupd, otype, ospeed) ((const uint32_t[7]){gpio_pin[0], gpio_pin[1], gpio_pin[2], mode, pupd, otype, ospeed})

#define PORTA GPIO_PORT(GPIOA, RCC_GPIOA)
#define PORTB GPIO_PORT(GPIOB, RCC_GPIOB)
#define PORTC GPIO_PORT(GPIOC, RCC_GPIOC)

#define PA0 GPIO_PIN(PORTA, GPIO0)
#define PA1 GPIO_PIN(PORTA, GPIO1)
#define PA2 GPIO_PIN(PORTA, GPIO2)
#define PA3 GPIO_PIN(PORTA, GPIO3)
#define PA4 GPIO_PIN(PORTA, GPIO4)
#define PA5 GPIO_PIN(PORTA, GPIO5)
#define PA6 GPIO_PIN(PORTA, GPIO6)
#define PA7 GPIO_PIN(PORTA, GPIO7)
#define PA8 GPIO_PIN(PORTA, GPIO8)
#define PA9 GPIO_PIN(PORTA, GPIO9)
#define PA10 GPIO_PIN(PORTA, GPIO10)
#define PA11 GPIO_PIN(PORTA, GPIO11)
#define PA12 GPIO_PIN(PORTA, GPIO12)
#define PA13 GPIO_PIN(PORTA, GPIO13)
#define PA14 GPIO_PIN(PORTA, GPIO14)
#define PA15 GPIO_PIN(PORTA, GPIO15)
#define PB0 GPIO_PIN(PORTB, GPIO0)
#define PB1 GPIO_PIN(PORTB, GPIO1)
#define PB2 GPIO_PIN(PORTB, GPIO2)
#define PB3 GPIO_PIN(PORTB, GPIO3)
#define PB4 GPIO_PIN(PORTB, GPIO4)
#define PB5 GPIO_PIN(PORTB, GPIO5)
#define PB6 GPIO_PIN(PORTB, GPIO6)
#define PB7 GPIO_PIN(PORTB, GPIO7)
#define PB8 GPIO_PIN(PORTB, GPIO8)
#define PB9 GPIO_PIN(PORTB, GPIO9)
#define PB10 GPIO_PIN(PORTB, GPIO10)
#define PB11 GPIO_PIN(PORTB, GPIO11)
#define PB12 GPIO_PIN(PORTB, GPIO12)
#define PB13 GPIO_PIN(PORTB, GPIO13)
#define PB14 GPIO_PIN(PORTB, GPIO14)
#define PB15 GPIO_PIN(PORTB, GPIO15)
#define PC0 GPIO_PIN(PORTC, GPIO0)
#define PC1 GPIO_PIN(PORTC, GPIO1)
#define PC2 GPIO_PIN(PORTC, GPIO2)
#define PC3 GPIO_PIN(PORTC, GPIO3)
#define PC4 GPIO_PIN(PORTC, GPIO4)
#define PC5 GPIO_PIN(PORTC, GPIO5)
#define PC6 GPIO_PIN(PORTC, GPIO6)
#define PC7 GPIO_PIN(PORTC, GPIO7)
#define PC8 GPIO_PIN(PORTC, GPIO8)
#define PC9 GPIO_PIN(PORTC, GPIO9)
#define PC10 GPIO_PIN(PORTC, GPIO10)
#define PC11 GPIO_PIN(PORTC, GPIO11)
#define PC12 GPIO_PIN(PORTC, GPIO12)
#define PC13 GPIO_PIN(PORTC, GPIO13)
#define PC14 GPIO_PIN(PORTC, GPIO14)
#define PC15 GPIO_PIN(PORTC, GPIO15)

#define GPIO_Init_Pin(pin) \
	rcc_periph_clock_enable(pin[2]); \
	gpio_mode_setup(pin[0], pin[3], pin[4], pin[1]); \
	if (pin[3] == 0x1) { \
		gpio_set_output_options(pin[0], pin[5], pin[6], pin[1]); \
	}

#define GPIO_Toggle(pin) \
	gpio_toggle(pin[0], pin[1]);

#define GPIO_In(pin) gpio_get(pin[0], pin[1])

#define GPIO_Out(pin, value) 		\
	if (value) {					\
		gpio_set(pin[0], pin[1]);	\
	} else {						\
		gpio_clear(pin[0], pin[1]);	\
	}

#define GPIO_Set(pin) 		\
	gpio_set(pin[0], pin[1]);

#define GPIO_Clear(pin) 		\
	gpio_clear(pin[0], pin[1]);