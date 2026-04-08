// BarrelShift operations
#include "barrelshift.h"

void barrel_shift :: barrel_method(){
	sc_int<8> data;
	int i;

	if(enable.read() == 1){
		data = din.read();
		if(left_right.read() == 0){ //shift left
			cout << "[left]  Data = " << data.to_string(SC_BIN) << endl; 
			for(i = 7; i > 0; i--)
				data[i] = din.read()[i-1];
			data[0] = l_in.read();
			dout.write(data);
 
		}else if(left_right.read() == 1){
		cout << "[right] Data = " << data.to_string(SC_BIN) << endl; 
			for(i = 0; i < 7; i++)//shift right
				data[i] = din.read()[i+1];
			
			data[7] = r_in.read();
			dout.write(data);
		}
	}
	else 
		cout << "Not enabled "<<endl;
}


