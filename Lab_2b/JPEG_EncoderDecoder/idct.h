#include "systemc.h"
#include <math.h>

#ifndef IDCT_H
#define IDCT_H

struct idct : sc_module{
	sc_out<char> out64[8][8]; // the input 8x8 block
	sc_in<double> in64[8][8]; // the dc transformed 8x8 block
	sc_in<double> fcosine[8][8]; // cosine table input
	//sc_out<FILE *> sc_output; // output file pointer port
	sc_in<bool> clk; // clock signal

	//char input_data[8][8]; // the data read from the input file

	void read_data( void ); // read the 8x8 block
	void calculate_idct( void ); // perform dc transform

	// define fdct as a constructor
	SC_CTOR( idct ) {
		SC_METHOD( calculate_idct ); // define calculate_idct as a method
		dont_initialize();
		sensitive << clk.neg();
	}
};

#endif
