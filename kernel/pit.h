#ifndef pit_included
#define pit_included

#include <kernel/hardware.h>

//-----------------------------------------------
//    Programmable Interval Timer
//-----------------------------------------------
//-----------------------------------------------
//    Operational Command Bit masks
//-----------------------------------------------
#define    PIT_OCW_MASK_BINCOUNT       1       //00000001
#define    PIT_OCW_MASK_MODE           0xE     //00001110
#define    PIT_OCW_MASK_RL             0x30    //00110000
#define    PIT_OCW_MASK_COUNTER        0xC0    //11000000

//-----------------------------------------------
//    Operational Command control bits
//-----------------------------------------------
//! Use when setting binary count mode
#define    PIT_OCW_BINCOUNT_BINARY     0    //0
#define    PIT_OCW_BINCOUNT_BCD        1    //1

//! Use when setting counter mode
#define    PIT_OCW_MODE_TERMINALCOUNT    0          //0000
#define    PIT_OCW_MODE_ONESHOT          0x2        //0010
#define    PIT_OCW_MODE_RATEGEN          0x4        //0100
#define    PIT_OCW_MODE_SQUAREWAVEGEN    0x6        //0110
#define    PIT_OCW_MODE_SOFTWARETRIG     0x8        //1000
#define    PIT_OCW_MODE_HARDWARETRIG     0xA        //1010

//! Use when setting data transfer
#define    PIT_OCW_RL_LATCH            0       //000000
#define    PIT_OCW_RL_LSBONLY          0x10    //010000
#define    PIT_OCW_RL_MSBONLY          0x20    //100000
#define    PIT_OCW_RL_DATA             0x30    //110000

//! Use when setting the counter we are working with
#define    PIT_OCW_COUNTER_0            0        //00000000
#define    PIT_OCW_COUNTER_1            0x40    //01000000
#define    PIT_OCW_COUNTER_2            0x80    //10000000

#define    PIT_REG_COUNTER0    0x40
#define    PIT_REG_COUNTER1    0x41
#define    PIT_REG_COUNTER2    0x42
#define    PIT_REG_COMMAND     0x43

extern void PIT_init();

extern void PIT_start_counter(uint16_t freq, uint8_t counter, uint8_t mode);

#endif  // pit_included
