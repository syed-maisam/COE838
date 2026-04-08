/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
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
volatile uint32_t *h2p_lw_sw_addr  = NULL;

int main(int argc, char **argv)
{
    void *virtual_base;
    int fd;
    uint32_t switch_value;

    /* open /dev/mem */
    if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
        printf("ERROR: could not open /dev/mem\n");
        return 1;
    }

    /* map FPGA memory */
    virtual_base = mmap(NULL, LW_SIZE,
                        (PROT_READ | PROT_WRITE),
                        MAP_SHARED,
                        fd,
                        LWHPS2FPGA_BASE);

    if (virtual_base == MAP_FAILED) {
        printf("ERROR: mmap() failed\n");
        close(fd);
        return 1;
    }

    /* map peripherals */
    h2p_lw_led_addr = (volatile uint32_t *)((char *)virtual_base + LED_PIO_BASE);
    h2p_lw_hex_addr = (volatile uint32_t *)((char *)virtual_base + SEG7_IF_0_BASE);
    h2p_lw_sw_addr  = (volatile uint32_t *)((char *)virtual_base + SWITCH_PIO_BASE);

    printf("Switch → LED control started\n");

    /* optional HEX demo */
    SEG7_All_Number();

    /* show HAtE */
    display_msg();

    while (1)
    {
        switch_value = alt_read_word((uint32_t)h2p_lw_sw_addr);
        alt_write_word((uint32_t)h2p_lw_led_addr, switch_value);
        usleep(50000);
    }

    munmap(virtual_base, LW_SIZE);
    close(fd);

    return 0;
}
