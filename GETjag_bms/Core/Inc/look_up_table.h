#ifndef LOOK_UP_TABLE_H
#define LOOK_UP_TABLE_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define FULL_VCELL_MV               (4400.0f)
#define FULL_I_MA                   (5.0f)
#define VCELL_MAX_TH_MV             (4300.0f)
#define VCELL_MIN_TH_MV             (3000.0f)
#define CELL_NOMINAL_CAPACITY_MAH   (4800U)


typedef struct {
    uint16_t voltage_mV;   // x value: Voltage in millivolts
    uint16_t capacity_mAh; // y value: Released capacity in milliamp-hours
} VoltageCapacityPair;

// Declare the variable as extern
extern const VoltageCapacityPair g_ocv_q_released_mah_table[];

extern const size_t NUM_PAIRS;

extern uint16_t get_val_lookup_table(const VoltageCapacityPair* table, size_t numPairs, uint16_t in_value);


#endif //LOOK_UP_TABLE_H