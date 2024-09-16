#ifndef SOC_SOH_H
#define SOC_SOH_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "look_up_table.h"

#define NUM_CELLS 16 // Define the number of cells
#define CHARGE_EFFICIENCY 0.95f
#define DISCHARGE_EFFICIENCY 0.95f

// Structure to hold the cell select register value
typedef struct {
    uint16_t value; // Bitmask for selected cells
} CellSelectRegister;


typedef struct {
    float soc;
    float soh;
    float dod;
    float delta_dod;
    float charge_efficiency;
    float discharge_efficiency;
    float coulomb_count;
    float cummulative_dod;
    uint16_t Q_rated;
    uint16_t Q_releasable;
    uint16_t Q_max;
} Soc_coulomb_counting;


// Function declaration
void get_min_max_vcell(void);
void get_initial_soc(void);
uint8_t voltage_to_percentage(uint16_t voltage);
uint16_t soc_k_update_get(float coulombs_count);

// Global variables declarations
extern uint16_t CellVoltage[16];
//extern uint16_t cell_voltages[NUM_CELLS];
extern CellSelectRegister selected_cells;
extern Soc_coulomb_counting soc_coulomb_count_init;
extern uint16_t min_vcell;
extern uint16_t max_vcell;



//uint16_t get_val_lookup_table(const VoltageCapacityPair* table, size_t numPairs, uint16_t in_value);
void get_initial_soc(void);



#endif // SOC_SOH_H
