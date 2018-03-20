#ifndef LOWLEVEL_H_

#include <libopencm3/stm32/gpio.h>

#define NUM_TESTS 1

struct test_table
{

	uint32_t start;
	uint32_t end;
	uint32_t in;
	uint32_t out;

};

/******** Test_init *********
* Initializes a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_init( struct test_table t );

/******** Test_start *********
* Records a starting value to a test table.
*  Inputs: A test_table struct, uint32_t start time
* Outputs: none
*/
void Test_start( struct test_table t, uint32_t start );

/******** Test_end *********
* Records an ending value to a test table.
*  Inputs: A test_table struct, uint32_t end time
* Outputs: none
*/
void Test_end( struct test_table t, uint32_t end );

/******** Test_in *********
* Records an inputted value to a test table.
*  Inputs: A test_table struct, uint32_t in value
* Outputs: none
*/
void Test_in( struct test_table t, uint32_t in );

/******** Test_out *********
* Records an outputted value to a test table.
*  Inputs: A test_table struct, uint32_t out value
* Outputs: none
*/
void Test_out( struct test_table t, uint32_t out );

/******** Test_calculate *********
* Calculates the differences between values recorded in a test table,
* displaying the results via serial terminal.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_calculate( struct test_table t );

#define TEST_H_ 1
#endif