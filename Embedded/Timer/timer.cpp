/*
 - Two memory mapped registers one providing 32bit LSB and the other MSB 
   values of a timer. The timer always increases in value. Write a gettime() 
   that returns time.
*/

#include <cstdint>

#define TIMER_LSB_ADDR 0x40000000
#define TIMER_MSB_ADDR 0x40000004

volatile uint32_t * const timer_lsb = (volatile uint32_t *)TIMER_LSB_ADDR;
volatile uint32_t * const timer_msb = (volatile uint32_t *)TIMER_MSB_ADDR;

uint64_t gettime() {
    uint32_t msb1, lsb, msb2;

    do {
        msb1 = *timer_msb;  // Read MSB first
        lsb = *timer_lsb;   // Read LSB
        msb2 = *timer_msb;  // Read MSB again
    } while(msb1 != msb2);  // Repeat if MSB changed (overflow occurred)

    return (msb2 << 32) | lsb;
}