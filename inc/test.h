#ifndef TEST_H_

#include <stdio.h>
#include <libopencm3/stm32/gpio.h>

#include "systick.h"

#define NUM_TESTS 1

struct test_table
{

	uint32_t start;
	uint32_t end;
	uint32_t in;
	uint32_t out;

};

typedef struct test_table test_table_t;

extern volatile test_table_t a_test_table;

/******** Test_init *********
* Initializes a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_init( volatile test_table_t *t );

/******** Test_start *********
* Records a starting value to a test table.
*  Inputs: A test_table struct, uint32_t start time
* Outputs: none
*/
void Test_start( volatile test_table_t *t );

/******** Test_end *********
* Records an ending value to a test table.
*  Inputs: A test_table struct, uint32_t end time
* Outputs: none
*/
void Test_end( volatile test_table_t *t );

/******** Test_in *********
* Records an inputted value to a test table.
*  Inputs: A test_table struct, uint32_t in value
* Outputs: none
*/
void Test_in( volatile test_table_t *t, uint32_t in );

/******** Test_out *********
* Records an outputted value to a test table.
*  Inputs: A test_table struct, uint32_t out value
* Outputs: none
*/
void Test_out( volatile test_table_t *t, uint32_t out );

/******** Test_calculate *********
* Calculates the differences between values recorded in a test table,
* displaying the results via serial terminal.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_calculate( volatile test_table_t *t );

// ******* Uart_send *******
// Adds arbitrary number of bytes to the UART transmission buffer.
//  Inputs: pointer to a contiguous block of data, the number of bytes
// Outputs: none
extern void Uart_send( volatile void* data, uint16_t length );

#define TEST_H_ 1
#endif