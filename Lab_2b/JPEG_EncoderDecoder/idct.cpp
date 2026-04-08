#include "idct.h"

//inverse discrete cosine transform
void idct :: calculate_idct(void){
	
	unsigned char 	u, v, x, y;
	double 	temp;

	// do inverse discrete cosine transform
	for (x = 0; x < 8; x++)
		for (y = 0; y < 8; y++) {
			temp = 0.0;
			for (u = 0; u < 8; u++){
				for (v = 0; v < 8; v++)
					
					temp += in64[u][v] * fcosine[x][u].read() * fcosine[y][v].read();

					if ((u == 0) && (v == 0))
						temp /= 2.0;
					else if (((u == 0) && (v != 0)) || ((u != 0) && (v == 0)))
						temp /= sqrt(2.0);
			}
			//temp /= 7.0;	//worse
			//temp /= 6.5;		//least green distortion		
			temp /= 6.0;		//best	
			//temp /= 5.5; 
			//temp /= 5.0;
			//temp /= 4.5; //more green distortion
			//temp /= 4.0; //expected value
			out64[x][y].write(temp);
		}

	printf(".");
}
