#include "soc.h"

uint16_t min_vcell;
uint16_t max_vcell; // Global variables for min and max values       // Default soc for new battery(cell) pack
float initial_soc = -1;
uint16_t pack_current;
bool count_init = true;
float difference, difference, previous_count;
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
    	uint16_t lut_value;
        // Calculate LUT value based on min voltage
        //uint16_t lut_value = get_val_lookup_table(g_ocv_q_released_mah_table, NUM_CELLS, (uint16_t)round(min_vcell));
        //printf("Debug: LUT Value from Lookup Table: %u\n", lut_value);
    	// Ensure capacity is within the bounds of uint16_t

    	double capacity = (CELL_NOMINAL_CAPACITY_MAH * ( soc_from_ocv(min_vcell)/100));

		if (capacity < 0) {
			lut_value = 0;  // Set to minimum if capacity is negative
		} else if (capacity > UINT16_MAX) {
			lut_value = UINT16_MAX;  // Set to maximum if capacity exceeds uint16_t range
		} else {
			lut_value = (uint16_t)capacity;  // Cast the capacity to uint16_t
		}

        if (lut_value < CELL_NOMINAL_CAPACITY_MAH)
        {
            //releasable_capacity = CELL_NOMINAL_CAPACITY_MAH - lut_value;
            //printf("Debug: Releasable Capacity Calculated: %u\n", releasable_capacity);
        	releasable_capacity = lut_value;
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


uint16_t soc_k_update_get(float coulombs_count)
{

//    // Calculate the delta DOD based on the given coulombs_count and the rated capacity
//    soc_coulomb_count_init.delta_dod = (float)coulombs_count / (float)soc_coulomb_count_init.Q_rated * 100.0f;
//    // Update SOC based on the coulombs_count
//    if (coulombs_count > 0)
//    {
//        // Positive coulombs_count indicates charging, which increases SOC
//        soc_coulomb_count_init.cummulative_dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.charge_efficiency * soc_coulomb_count_init.delta_dod;
//        soc_coulomb_count_init.dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
//    }
//    else
//    {
//        // Negative coulombs_count indicates discharging, which decreases SOC
//        soc_coulomb_count_init.cummulative_dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
//        soc_coulomb_count_init.dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
//    }
//
//    // Calculate the updated SOC
//    soc_coulomb_count_init.soc = soc_coulomb_count_init.soh - soc_coulomb_count_init.cummulative_dod;
//    soc_coulomb_count_init.soc = fmax(0.0f, fminf(100.0f, soc_coulomb_count_init.soc));
//    soc_coulomb_count_init.dod = fmax(0.0f, fminf(100.0f, soc_coulomb_count_init.dod));
//    //printf("Updated: SOC calculated as: %.2f%%\n", soc_coulomb_count_init.soc);
//    //printf("Updated: dod calculated as: %.2f%%\n", soc_coulomb_count_init.dod);
//
//    return (uint16_t)soc_coulomb_count_init.soc; // Return the updated SOC as an integer
	// Calculate the delta DOD based on the given coulombs_count and the rated capacity

	if (count_init) {
		        // For the first reading, return it itself as the difference
		        previous_count = coulombs_count;
		        count_init = false;
		        difference = coulombs_count;
		    } else {
		        // Calculate the difference for consecutive readings
		        difference = coulombs_count - previous_count;

		        // Update the previous count for next calculations
		        previous_count = coulombs_count;
		    }


	    //printf("Updated: difference calculated as: %.2f\n",  difference);

	    // Calculate the delta DOD based on the given coulombs_count and the rated capacity
	    soc_coulomb_count_init.delta_dod = (-1.0f) * (float)difference / (float)soc_coulomb_count_init.Q_rated * 100.0f;
	    //soc_coulomb_count_init.cummulative_dod = soc_coulomb_count_init.dod + soc_coulomb_count_init.charge_efficiency * soc_coulomb_count_init.delta_dod;
	    // Update SOC based on the coulombs_count
	    if (difference > 0)
	    {
	        // Positive coulombs_count indicates charging, which increases SOC

	        soc_coulomb_count_init.dod += soc_coulomb_count_init.charge_efficiency * soc_coulomb_count_init.delta_dod;
	    }
	    else if(difference < 0)
	    {
	        // Negative coulombs_count indicates discharging, which decreases SOC
	        //soc_coulomb_count_init.cummulative_dod = soc_coulomb_count_init.dod - soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
	        soc_coulomb_count_init.dod += soc_coulomb_count_init.discharge_efficiency * soc_coulomb_count_init.delta_dod;
	    }

	    // Calculate the updated SOC
	    soc_coulomb_count_init.soc = soc_coulomb_count_init.soh - soc_coulomb_count_init.dod;
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

    // Initialize index for min and max
    uint8_t min_index = 0;
    uint8_t max_index = 0;

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
            min_index = i; // Update the index of the minimum value
        }

        // Update maximum voltage if the current voltage is higher
        if (CellVoltage[i] > max_vcell)
        {
            max_vcell = CellVoltage[i];
            max_index = i; // Update the index of the maximum value
        }
    }

    // Output the results including the indices
    //printf("Min Voltage: %u mV at index %u\n", min_vcell, min_index);
    //printf("Max Voltage: %u mV at index %u\n", max_vcell, max_index);
}


uint8_t voltage_to_percentage(uint16_t voltage)
{
    // Define the lower and upper limits of the voltage range in mV
    const uint16_t min_voltage = 3000;
    const uint16_t max_voltage = 4300;
    const uint16_t error_voltage = 4500;

    // Guard: If the voltage is below 3000 mV, return 0%
    if ((voltage < min_voltage) || (voltage > error_voltage))
    {
        return 0;
    }

    // Guard: If the voltage is above 4300 mV, return 100%
    if (voltage > max_voltage)
    {
        return 100;
    }

    // Calculate the SoC percentage for voltages within the valid range
    float soc_percentage = ((float)(voltage - min_voltage) / (float)(max_voltage - min_voltage)) * 100.0f;

    // Return the calculated SoC percentage as an integer
    return (uint16_t)soc_percentage;
}

double soc_from_ocv(uint16_t ocv_min_milivolts){

	// Convert millivolts to volts
	    double ocv = ocv_min_milivolts / 1000.0;

	// Polynomial coefficients
	   double a1 = 268.4970355259198;   // Coefficient for OCV^8
	   double a2 = -6879.270367122276;  // Coefficient for OCV^7
	   double a3 = 76716.49575172913;   // Coefficient for OCV^6
	   double a4 = -486365.6733759814;  // Coefficient for OCV^5
	   double a5 = 1917287.1707991716;  // Coefficient for OCV^4
	   double a6 = -4812471.06572991;   // Coefficient for OCV^3
	   double a7 = 7511312.300797121;   // Coefficient for OCV^2
	   double a8 = -6665390.783393391;  // Coefficient for OCV^1
	   double a9 = 2574719.229612701;   // Constant term

	   // Return the polynomial evaluated for the given OCV
	   return (a1 * pow(ocv, 8)) + (a2 * pow(ocv, 7)) + (a3 * pow(ocv, 6)) +
			  (a4 * pow(ocv, 5)) + (a5 * pow(ocv, 4)) + (a6 * pow(ocv, 3)) +
			  (a7 * pow(ocv, 2)) + (a8 * ocv) + a9;

}