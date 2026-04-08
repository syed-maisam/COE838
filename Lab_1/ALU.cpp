// ALU Operations
#include "ALU.h"

void ALU :: ALU_method(){
	if(OP.read() == 1){ //addition
		alu_out.write(A.read()+B.read());		
	}
	else{ //subtraction
		alu_out.write(A.read()-B.read());
	}
}
