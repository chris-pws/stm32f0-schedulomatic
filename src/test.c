#include "test.h"

struct a_test_table test_table[NUM_TESTS];

/******** Test_init *********
* Initializes a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_init( struct test_table t )
{

	t.start = 0;
	t.end = 0;
	t.in = 0;
	t.out = 0;
}

/******** Test_start *********
* Records a starting value to a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_start( struct test_table t )
{

	now = Systick_timeGetCount();
	t.start = now;

}

/******** Test_end *********
* Records an ending value to a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_end( struct test_table t )
{

	now = Systick_timeGetCount();
	t.end = now;

}

/******** Test_in *********
* Records an inputted value to a test table.
*  Inputs: A test_table struct, uint32_t in value
* Outputs: none
*/
void Test_in( struct test_table t, uint32_t in )
{

	t.start = in;

}

/******** Test_out *********
* Records an outputted value to a test table.
*  Inputs: A test_table struct, uint32_t out value
* Outputs: none
*/
void Test_out( struct test_table t, uint32_t out )
{

	t.start = out;

}

/******** Test_calculate *********
* Calculates the differences between values recorded in a test table,
* displaying the results via serial terminal.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_calculate( struct test_table t )
{

	int64_t in_out_diff = t.in - t.out;
	uint32_t start_end_diff = Systick_timeDelta( t.start, t.end );
}