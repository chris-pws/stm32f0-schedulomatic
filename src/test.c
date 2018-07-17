#include "test.h"

volatile test_table_t a_test_table;

/******** Test_init *********
* Initializes a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_init( volatile test_table_t *t )
{

	t->start = 0;
	t->end = 0;
	t->in = 0;
	t->out = 0;
}

/******** Test_start *********
* Records a starting value to a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_start( volatile test_table_t *t )
{

	uint32_t now = Systick_timeGetCount();
	t->start = now;

}

/******** Test_end *********
* Records an ending value to a test table.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_end( volatile test_table_t *t )
{

	uint32_t now = Systick_timeGetCount();
	t->end = now;

}

/******** Test_in *********
* Records x elements queued to a test table.
*  Inputs: A test_table struct, uint32_t in value
* Outputs: none
*/
void Test_in( volatile test_table_t *t, int in )
{

	t->in += in;

}

/******** Test_out *********
* Records x outbound queued elements to a test table.
*  Inputs: A test_table struct, uint32_t out value
* Outputs: none
*/
void Test_out( volatile test_table_t *t, int out )
{

	t->out += out;

}

/******** Test_calculate *********
* Calculates the differences between values recorded in a test table,
* displaying the results via serial terminal.
*  Inputs: A test_table struct
* Outputs: none
*/
void Test_calculate( volatile test_table_t *t )
{
	char out_time_buf[120];
	char out_element_buf[120];
	int s;

	int64_t in_out_diff = t->in - t->out;
	int start_end_diff = Systick_timeDelta( t->start, t->end );

	s = sprintf( out_time_buf, " Start: %u End: %u Duration: %u ", t->start, t->end,
				 start_end_diff );
	Uart_send( out_time_buf, s );

	s = sprintf( out_element_buf, " In: %u Out: %u Difference: %lli ", t->in, t->out,
				 in_out_diff );
	Uart_send( out_element_buf, s );
}