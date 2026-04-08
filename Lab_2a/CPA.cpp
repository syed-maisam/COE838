#include "CPA.h"    
#include "CSA.h" 

#define size 8 
// CSA Definition
sc_uint<size > sum;  
sc_uint<size > carrout;
sc_uint<size > ain; 
sc_uint<size > bin;    
sc_uint<size > A_combined;   
sc_uint<size > sin_input;   
sc_uint<size > ainandbin; 
sc_uint<size > lsb_val;
sc_uint<size > carrin;     
sc_uint<size > sum_actual; 
sc_uint<size > carr_actual; 
sc_uint<size > sum_simplier; 
sc_uint<size > carr_simplier;
int i = 0; 
int j = 0;     
int y = 0;
int sum_holder[size][size];
int carrout_holder[size][size];
// CSA Definition


// CPA definition
sc_uint<size> sum_cpa;  
sc_uint<size> carrout_cpa;   

sc_uint<size> sum_holder_cpa;  
sc_uint<size> carrout_holder_cpa;  

sc_uint<size> sum_csa;  

sc_uint<size> carrout_csa;
sc_uint<size> ain_cpa; 
sc_uint<size> bin_cpa;   
sc_uint<size> carrin_cpa;      

sc_uint<size> least_significant;      
sc_uint<size*2> complete_output; 

sc_uint<size > msb_val;
int x, w = 0;
int z = 0;  
// CPA definition

 void CPA :: CPA_method() {     
	cout << endl;   

// CSA Method
        	ain = A_input.read();  
        	bin = B_input.read();        
                A_combined = ain & bin;
       		for (i = 0; i < size; i ++ ){ 

                  for (j = 0 ; j < size; j ++){ 
                    if (i == 0){   
                      sin_input = 0; 
                      carrin = 0;    
                      
                      A_combined = ain[j] & bin[i];
                      sum = (( (A_combined) ^ (sin_input)) ^ (carrin));   
                      carrout = ((A_combined) & sin_input)  | (sin_input & carrin) | (A_combined & carrin);   
                      if (j == 0){  
                        lsb_val[i] = sum[j]; 
                      } 
                      sum_holder[i][j] = sum; 
                      carrout_holder[i][j] = carrout;        
                    }   
                    
                    else if (j == (size - 1) && (i != 0)){  

                      A_combined = ain[j] & bin[i];
                      carrin = carrout_holder[i - 1][j];  
                      sin_input = 0;
                      sum = (( (A_combined) ^ (sin_input)) ^ (carrin));  
                      carrout = ((A_combined) & sin_input)  | (sin_input & carrin) | (A_combined & carrin); 
                      
                      sum_holder[i][j] = sum; 
                      carrout_holder[i][j] = carrout;      
                      
                    }   

                    else{  

                      A_combined = ain[j] & bin[i];
                      carrin = carrout_holder[i - 1][j];  
                      sin_input = sum_holder[i-1][j+1]; 
                      sum = (( (A_combined) ^ (sin_input)) ^ (carrin));  
                      carrout = ((A_combined) & sin_input)  | (sin_input & carrin) | (A_combined & carrin); 
                      
                      sum_holder[i][j] = sum; 
                      carrout_holder[i][j] = carrout;        

                      if (j == 0){  
                        lsb_val[i] = sum[j]; 
                      }  
                      
                    }       

                  } 
		}     

                for (y = 0; y < size; y++){  
                  sum_simplier[y] = sum_holder[size-1][y];
                  carr_simplier[y] = carrout_holder[size-1][y];
                } 

                lsb_out.write(lsb_val); 
                carry_out_csa.write(carr_simplier); 
                sum_out_csa.write(sum_simplier);      

// CSA Method

                sum_csa = sum_simplier; 
                carrout_csa = carr_simplier; 
                least_significant = lsb_val;
        	


                for (x = 0; x < size; x ++){    
                    if (x == 0){  
                      carrin_cpa = 0; 
                      ain_cpa = sum_csa[x+1];  
                      bin_cpa = carrout_csa[x];   

                      sum_cpa = ((ain_cpa ^ bin_cpa) ^ (carrin_cpa));  
                      carrout_cpa = ((ain_cpa & bin_cpa) | (carrin_cpa & bin_cpa) | (ain_cpa & carrin_cpa));    
                      
                      sum_holder_cpa[x] = sum_cpa;   
                      msb_val[x] = sum_cpa; 
                      carrout_holder_cpa[x] = carrout_cpa;  
                      
                    }  

                    else if (x == (size - 1)){ 
                      carrin_cpa = 0; 
                      bin_cpa = carrout_holder_cpa[x-1]; 
                      ain_cpa = carrout_csa[x];  

                      sum_cpa = ((ain_cpa ^ bin_cpa) ^ (carrin_cpa));  
                      carrout_cpa = ((ain_cpa & bin_cpa) | (carrin_cpa & bin_cpa) | (ain_cpa & carrin_cpa));    
                      
                      sum_holder_cpa[x] = sum_cpa;   
                      msb_val[x] = sum_cpa; 
                      carrout_holder_cpa[x] = carrout_cpa;   


                    } 
                    else{ 
                      carrin_cpa = carrout_holder_cpa[x-1]; 
                      bin_cpa = carrout_csa[x]; 
                      ain_cpa = sum_csa[x+1];  

                      sum_cpa = ((ain_cpa ^ bin_cpa) ^ (carrin_cpa));  
                      carrout_cpa = ((ain_cpa & bin_cpa) | (carrin_cpa & bin_cpa) | (ain_cpa & carrin_cpa));    
                      
                      sum_holder_cpa[x] = sum_cpa;   
                      msb_val[x] = sum_cpa; 
                      carrout_holder_cpa[x] = carrout_cpa;  
                      

                    }  
                    cout << sum_holder_cpa[x];
                }    

                msb_out.write(msb_val); 


               for (z = 0; z < (size*2); z ++){ 
                 if (z < (size)){ 
                   complete_output[z] =  least_significant[z]; 
                 }  
                 else { 
                   complete_output[z] =  msb_val[z - size]; 
                 }
               } 

               complete.write(complete_output);
                
 }

