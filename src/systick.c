#include "systick.h"

uint32_t systickCount = UINT32_MAX - 5000;

// ******* Systick_init *******
// Initializes the SysTick interrupt timer.
// Inputs: none
// Outputs: none
void Systick_init(void)
{
	// 48MHz 
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);

	// 48000000/100000 = 480 overflows per second - every 10 micros one interrupt
	// SysTick interrupt every N clock pulses: set reload to N-1 
	systick_set_reload(47999);
	systick_interrupt_enable();
	// Start counting
	systick_counter_enable();
}

// ******* Systick_timeGetCount *******
// Returns the elapsed program time based on the count of SysTick interrupts.
// Inputs: none
// Outputs: A count of elapsed cycles.
uint32_t Systick_timeGetCount(void)
{
    return systickCount;
}

// ******* Systick_timeDelta *******
// Provides the difference between two program times, as provided by 
// Systick_get_time().
// Inputs: A start time and an end time.
// Outputs: The delta between two points in execution time.
uint32_t Systick_timeDelta( uint32_t start, uint32_t end )
{
    uint32_t diff;

    // Detect if roll-over has occurred and calculate accordingly.
    if ( end > start )
    {
        diff = end - start;
    }
    else
    {
        diff = UINT32_MAX - ( start-end ) + 1;
    }

    return diff;
}