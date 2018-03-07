#ifndef SYSTICK_H_

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/f0/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/cortex.h>
#include <stdint.h>

// ******* Systick_init *******
// Initializes the SysTick interrupt timer.
//  Inputs: none
// Outputs: none
void Systick_init(void);

// ******* Systick_timeGetCount *******
// Returns the elapsed program time based on the count of SysTick interrupts.
//  Inputs: none
// Outputs: A count of elapsed cycles.
uint32_t Systick_timeGetCount(void);

// ******* Systick_timeDelta *******
// Provides the difference between two program times, as provided by 
// Systick_get_time().
//  Inputs: A start time and an end time.
// Outputs: The delta between two points in execution time.
uint32_t Systick_timeDelta( uint32_t start, uint32_t end );

// ******* Systick_delayTicks *******
// Delays execution for x ticks based on time reporting from 
// Systick_get_time(). Non-blocking.
//  Inputs: Number of ticks to delay execution.
// Outputs: none
void Systick_delayTicks( uint32_t ticks );


#define SYSTICK_H_ 1
#endif