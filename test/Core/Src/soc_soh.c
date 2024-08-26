#include "soc_soh.h"

uint16_t min_vcell, max_vcell; // Global variables for min and max values
float initial_soc = -1;        // Default soc for new battery(cell) pack
uint16_t pack_current;

// Function definition for get_initial_soc
void get_initial_soc(void)
{
    // Read cell measurements
    uint16_t releasable_capacity = 0; // Initialize releasable capacity
    get_min_max_vcell();              // Get min and max voltage

    //printf("Debug: Minimum Cell Voltage (mV): %u\n", min_vcell);
    //printf("Debug: Initial SOC: %f\n", initial_soc);

    /* Get initial SOC estimation */
    if (initial_soc < 0)
    {
        // Calculate LUT value based on min voltage
        uint16_t lut_value = get_val_lookup_table(g_ocv_q_released_mah_table, 15, (uint16_t)round(min_vcell));
        //printf("Debug: LUT Value from Lookup Table: %u\n", lut_value);

        if (lut_value < CELL_NOMINAL_CAPACITY_MAH)
        {
            releasable_capacity = CELL_NOMINAL_CAPACITY_MAH - lut_value;
            //printf("Debug: Releasable Capacity Calculated: %u\n", releasable_capacity);
        }
        else
        {
            //printf("Debug: LUT value >= CELL_NOMINAL_CAPACITY_MAH, no capacity available\n");
        }
    }
    else
    {
        // Calculate releasable capacity based on initial SOC
        float aux_q = (float)CELL_NOMINAL_CAPACITY_MAH * (initial_soc / 100.0f);
        releasable_capacity = (uint16_t)round(aux_q);
        //printf("Debug: Calculated releasable capacity based on SOC: %u\n", releasable_capacity);
    }

    // You might want to return or further use releasable_capacity here
    //printf("Debug: Final Releasable Capacity: %u\n", releasable_capacity);

    soc_coulomb_count_init.Q_max = CELL_NOMINAL_CAPACITY_MAH;
    soc_coulomb_count_init.Q_rated = CELL_NOMINAL_CAPACITY_MAH;
    soc_coulomb_count_init.Q_releasable = releasable_capacity;

    soc_coulomb_count_init.soc = ((float)soc_coulomb_count_init.Q_releasable / (float)soc_coulomb_count_init.Q_rated) * 100.0f;
    //printf("Debug: SOC calculated as: %.2f%%\n", soc_coulomb_count_init.soc);

    soc_coulomb_count_init.soh = (float)((float)soc_coulomb_count_init.Q_max / (float)soc_coulomb_count_init.Q_rated) * 100.0f;
    soc_coulomb_count_init.dod = soc_coulomb_count_init.soh - soc_coulomb_count_init.soc;
    soc_coulomb_count_init.charge_efficiency = CHARGE_EFFICIENCY;
    soc_coulomb_count_init.discharge_efficiency = DISCHARGE_EFFICIENCY;
    soc_coulomb_count_init.coulomb_count = 0;
    //printf("Updated: dod before calculated as: %.2f%%\n", soc_coulomb_count_init.dod);
}

uint16_t soc_k_update_get(int16_t coulombs_count)
{

    // Calculate the delta DOD based on the given coulombs_count and the rated capacity
    soc_coulomb_count_init.delta_dod = (float)coulombs_count / (float)soc_coulomb_count_init.Q_rated * 100.0f;
    // Update SOC based on the coulombs_count
    if (coulombs_count > 0)
    {
        // Positive coulombs_count indicates charging, which increases SOC
        soc_coulomb_count_init.cummulative_dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.charge_efficiency * soc_coulomb_count_init.delta_dod;
        soc_coulomb_count_init.dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
    }
    else
    {
        // Negative coulombs_count indicates discharging, which decreases SOC
        soc_coulomb_count_init.cummulative_dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
        soc_coulomb_count_init.dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
    }

    // Calculate the updated SOC
    soc_coulomb_count_init.soc = soc_coulomb_count_init.soh - soc_coulomb_count_init.cummulative_dod;
    soc_coulomb_count_init.soc = fmax(0.0f, fminf(100.0f, soc_coulomb_count_init.soc));
    soc_coulomb_count_init.dod = fmax(0.0f, fminf(100.0f, soc_coulomb_count_init.dod));
    //printf("Updated: SOC calculated as: %.2f%%\n", soc_coulomb_count_init.soc);
    //printf("Updated: dod calculated as: %.2f%%\n", soc_coulomb_count_init.dod);

    return (uint16_t)soc_coulomb_count_init.soc; // Return the updated SOC as an integer
}

void soc_full_correct(void)
{
    soc_coulomb_count_init.soh = soc_coulomb_count_init.soh - soc_coulomb_count_init.dod;

    if (soc_coulomb_count_init.soh > 100.0f)
    {
        soc_coulomb_count_init.soh = 100.0f;
    }

    soc_coulomb_count_init.Q_max = (uint16_t)(((float)soc_coulomb_count_init.soh / 100.0f) * soc_coulomb_count_init.Q_rated);

    soc_coulomb_count_init.soc = 100.0f;
    soc_coulomb_count_init.dod = 0.0f;
}

void soc_empty_correct(void)
{

    soc_coulomb_count_init.soh = soc_coulomb_count_init.dod;

    if (soc_coulomb_count_init.soh > 100.0f)
    {
        soc_coulomb_count_init.soh = 100.0f;
    }

    soc_coulomb_count_init.Q_max = (uint16_t)(((float)soc_coulomb_count_init.soh / 100.0f) * soc_coulomb_count_init.Q_rated);

    soc_coulomb_count_init.soc = 0.0f;
    soc_coulomb_count_init.dod = 100.0f;
}
// Function to find the minimum and maximum cell voltages
void get_min_max_vcell(void)
{
    // Initialize min and max to the first element of the array
    min_vcell = CellVoltage[0];
    max_vcell = CellVoltage[0];

    // Iterate through the array of cell voltages
    for (uint8_t i = 0; i < NUM_CELLS; i++)
    {
        // Check if the current cell is selected
        if ((selected_cells.value & (0x01 << i)) == 0)
        {
            continue; // Skip this iteration if the cell is not selected
        }

        // Update minimum voltage if the current voltage is lower
        if (CellVoltage[i] < min_vcell)
        {
            min_vcell = CellVoltage[i];
        }

        // Update maximum voltage if the current voltage is higher
        if (CellVoltage[i] > max_vcell)
        {
            max_vcell = CellVoltage[i];
        }
    }
    // Output the results
    //printf("Min Voltage: %u mV\n", min_vcell);
    //printf("Max Voltage: %u mV\n", max_vcell);
}