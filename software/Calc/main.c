/*
 * main.c
 *
 * Calculator:
 * SW[7:0]  = input value
 * SW[8]    = op bit 0
 * SW[9]    = op bit 1
 *
 * 00 -> no op
 * 01 -> addition
 * 10 -> subtraction
 * 11 -> multiplication
 *
 * KEY0 -> store A
 * KEY1 -> store B
 * KEY2 -> compute result
 * KEY3 -> clear A, B, result
 *
 * Behavior:
 * - Whenever SW[7:0] changes, the current input value is shown on the 7-seg.
 * - Pressing KEY0 shows/stores A.
 * - Pressing KEY1 shows/stores B.
 * - Pressing KEY2 computes and shows result.
 * - Pressing KEY3 clears everything and shows 0.
 *
 * Notes:
 * - Keys are active-low on DE1-SoC, so they are inverted in software.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <stdint.h>

#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"
#include "led.h"
#include "seg7.h"

#define LW_SIZE 0x00200000
#define LWHPS2FPGA_BASE 0xff200000

volatile uint32_t *h2p_lw_led_addr = NULL;
volatile uint32_t *h2p_lw_hex_addr = NULL;
volatile uint32_t *h2p_lw_key_addr = NULL;
volatile uint32_t *h2p_lw_sw_addr  = NULL;

static uint32_t read_switches(void){
    return alt_read_word(h2p_lw_sw_addr) & 0x3FF;   // SW[9:0]
}

static uint32_t read_keys(void){
    // active-low keys -> invert
    return (~alt_read_word(h2p_lw_key_addr)) & 0x0F;   // KEY[3:0]
}

static int32_t do_operation(uint8_t A, uint8_t B, uint8_t op_sel){
    switch(op_sel){
        case 0x0: return 0;                 // 00 = no op
        case 0x1: return (int32_t)A + B;   // 01 = add
        case 0x2: return (int32_t)A - B;   // 10 = subtract
        case 0x3: return (int32_t)A * B;   // 11 = multiply
        default:  return 0;
    }
}

static void show_value_on_seg7(int32_t value){
    if(value < 0){
        SEG7_Decimal((unsigned long)(-value), 0x00);
    }else{
        SEG7_Decimal((unsigned long)value, 0x00);
    }
}

static const char *op_name(uint8_t op_sel){
    switch(op_sel){
        case 0x0: return "NO OP";
        case 0x1: return "ADD";
        case 0x2: return "SUB";
        case 0x3: return "MUL";
        default:  return "UNKNOWN";
    }
}

int main(int argc, char **argv){

    void *virtual_base;
    int fd;

    uint8_t A = 0;
    uint8_t B = 0;
    int32_t result = 0;

    uint32_t prev_keys = 0;
    uint32_t prev_input_value = 0xFFFFFFFF;

    if((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1){
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return 1;
    }

    virtual_base = mmap(NULL, LW_SIZE, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LWHPS2FPGA_BASE);

    if(virtual_base == MAP_FAILED){
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return 1;
    }

    h2p_lw_hex_addr = (volatile uint32_t *)((char *)virtual_base + (uint32_t)SEG7_IF_0_BASE);
    h2p_lw_key_addr = (volatile uint32_t *)((char *)virtual_base + (uint32_t)KEY_PIO_BASE);
    h2p_lw_sw_addr  = (volatile uint32_t *)((char *)virtual_base + (uint32_t)SWITCH_PIO_BASE);
    h2p_lw_led_addr = (volatile uint32_t *)((char *)virtual_base + (uint32_t)LED_PIO_BASE);

    SEG7_Clear();
    LEDR_AllOff();

    printf("Calculator started.\n");
    printf("SW[7:0] = input, SW[8:9] = op, KEY0 = store A, KEY1 = store B, KEY2 = compute, KEY3 = clear\n");

    while(1){
        uint32_t sw = read_switches();
        uint32_t keys = read_keys();
        uint32_t new_keys = keys & (~prev_keys);

        uint8_t input_value = (uint8_t)(sw & 0xFF);          // SW0..SW7
        uint8_t op_sel = (uint8_t)((sw >> 8) & 0x03);        // SW8..SW9

        // mirror switch positions on LEDs
        alt_write_word(h2p_lw_led_addr, sw);

        // live display of current input whenever SW[7:0] changes
        if(input_value != prev_input_value){
            show_value_on_seg7(input_value);
            prev_input_value = input_value;
        }

        // KEY0 -> store A and show A
        if(new_keys & 0x1){
            A = input_value;
            show_value_on_seg7(A);
            printf("Stored A = %u\n", A);
            fflush(stdout);
        }

        // KEY1 -> store B and show B
        if(new_keys & 0x2){
            B = input_value;
            show_value_on_seg7(B);
            printf("Stored B = %u\n", B);
            fflush(stdout);
        }

        // KEY2 -> compute result and show result
        if(new_keys & 0x4){
            result = do_operation(A, B, op_sel);
            show_value_on_seg7(result);

            printf("A = %u, B = %u, Operation = %s, Result = %d\n",
                   A, B, op_name(op_sel), result);
            fflush(stdout);
        }

        // KEY3 -> clear everything
        if(new_keys & 0x8){
            A = 0;
            B = 0;
            result = 0;
            show_value_on_seg7(0);
            printf("Cleared A, B, and result\n");
            fflush(stdout);
        }

        prev_keys = keys;
        usleep(50000);
    }

    if(munmap(virtual_base, LW_SIZE) != 0){
        printf("ERROR: munmap() failed...\n");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
