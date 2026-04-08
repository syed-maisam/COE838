// ALU Units

#ifndef ALU_H   
#define ALU_H

#include <systemc.h>       


void make_ALU();
                                
SC_MODULE(ALU) {
	//sc_in<bool > clock;
	sc_in_clk clock;
	sc_in<bool > OP;
	sc_in<sc_int<8> > A, B;
	sc_out<sc_int<8> > alu_out;	

	void ALU_method();   
 
        SC_CTOR(ALU) {

        SC_METHOD(ALU_method);
		dont_initialize();
        sensitive << clock.pos(); //edge sensitive
    }

};
#endif 
