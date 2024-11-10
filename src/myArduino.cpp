#include <myArduino.h>


double max(double a, double b) {
    return fmax(a, b);
}

uint32_t micros(void) {
    uint32_t cycles = k_cycle_get_32();
    uint32_t us = (uint32_t)k_cyc_to_us_near32(cycles);
    return us;
}