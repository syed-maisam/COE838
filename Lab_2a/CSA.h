#ifndef CSA_H   
#define CSA_H
#include <systemc.h>       
#define size 8
SC_MODULE(CSA) { 
	sc_in<sc_uint<size> > B_input;
	sc_in<sc_uint<size> > A_input; 
	sc_in<sc_uint<size> > S_input;
	sc_in<sc_uint<size> > carry_in;  
	sc_out<sc_uint<size > > carry_out; 
	sc_out<sc_uint<size > > sum_out;  
        sc_out<sc_uint<size> > lsb_out;
	void CSA_method();     
        SC_CTOR(CSA) { 
 SC_METHOD(CSA_method);
	dont_initialize();
        sensitive << A_input << B_input << S_input << carry_in; 
    }
}; 

#endif 

