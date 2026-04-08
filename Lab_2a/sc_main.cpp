#include <systemc.h>
#include "CPA.h"
#include "CSA.h"
#define size 8
int sc_main(int argc, char* argv[]){ 
         int  truth_it = 8; 
         int i = 0; 
         int time = 9;
	 sc_trace_file *tf;                  // Create VCD file for tracing 

         sc_signal<sc_uint<size> > sum_cpa;  
         sc_signal<sc_uint<size> > carrout_cpa;
         sc_signal<sc_uint<size> > ain_cpa; 
         sc_signal<sc_uint<size> > bin_cpa;  
         sc_signal<sc_uint<size> > carrin_cpa;      

         sc_signal<sc_uint<size> > lsb_out_cpa;    
         sc_signal<sc_uint<size> > sum_out_csa;  
         sc_signal<sc_uint<size> > carry_out_csa;  

         sc_signal<sc_uint<size> > msb_out_cpa;   
         sc_signal<sc_uint<size> > lsb_in_cpa;  
         sc_signal<sc_uint<size*2> > com_byte; 

         sc_signal<sc_uint<size> > sum_csa;  
         sc_signal<sc_uint<size> > carrout_csa;
         sc_signal<sc_uint<size> > ain_csa; 
         sc_signal<sc_uint<size> > bin_csa;  
         sc_signal<sc_uint<size> > sin_csa;  
         sc_signal<sc_uint<size> > carrin_csa;    
         sc_signal<sc_uint<size> > lsb_out_csa;  
         
	 sc_clock clk("clk",10,SC_NS,0.5);   //Create a clock signal 

   	 CPA testing_CPA("CPA");            //Create Device Under Test (DUT) 

        
         testing_CPA.B_input(bin_cpa); 
         testing_CPA.A_input(ain_cpa);
         testing_CPA.carry_in(carrin_cpa);
         testing_CPA.carry_out(carrout_cpa);
         testing_CPA.sum_out(sum_cpa);   

         testing_CPA.lsb_out(lsb_out_cpa);  
         testing_CPA.sum_out_csa(sum_out_csa); 
         testing_CPA.carry_out_csa(carry_out_csa); 
         

         testing_CPA.msb_out(msb_out_cpa); 
         testing_CPA.complete(com_byte);  
         testing_CPA.least_sig(lsb_in_cpa); 

       // Create wave file and trace the signals executing
	tf = sc_create_vcd_trace_file("trace_file"); 

        tf->set_time_unit(1, SC_NS); 

	sc_trace(tf, msb_out_cpa, "MSB_out_cpa");    
	sc_trace(tf, com_byte, "Complete");    


        sc_trace(tf, sum_out_csa, "Sum_CSA_out");
        sc_trace(tf, carry_out_csa, "Carry_CSA_out");  
        sc_trace(tf, bin_cpa, "B");
        sc_trace(tf, ain_cpa, "A");  


        sc_trace(tf, lsb_out_cpa, "LSB_out_cpa");


	//cout << "\nExecuting CPA example... check .vcd produced"<<endl;
	//start the testbench   

        ain_cpa.write(4);   
        bin_cpa.write(8);        
	sc_start(20, SC_NS);    

        ain_cpa.write(255);    
        bin_cpa.write(255);        
	sc_start(20, SC_NS);   

        ain_cpa.write(55);    
        bin_cpa.write(5);       
	sc_start(20, SC_NS);   

        ain_cpa.write(200);    
        bin_cpa.write(600);     
	sc_start(20, SC_NS);     

        ain_cpa.write(20);   
        bin_cpa.write(55);         
	sc_start(20, SC_NS);    

        ain_cpa.write(90);    
        bin_cpa.write(20);       
	sc_start(20, SC_NS);    
   	sc_close_vcd_trace_file(tf);
   	
   	return 0;                         
}

