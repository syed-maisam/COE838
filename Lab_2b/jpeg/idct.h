#include "systemc.h"
#include <math.h>

#ifndef IDCT_H
#define IDCT_H

struct idct : sc_module {
    // Ports for JPEG data (e.g., 8x8 blocks)
    sc_in<bool> clk;
    sc_in<float> input_buffer[64];
    sc_out<float> output_buffer[64];

    // Function Declaration (This fixes the error)
    void calculate_idct();

    // Constructor
    SC_CTOR(idct) {
        SC_METHOD(calculate_idct);
        sensitive << clk.pos(); // Or trigger based on a 'start' signal
    }
};

#endif
