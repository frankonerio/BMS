#include <stdint.h>
#include <stddef.h>
#include "look_up_table.h"


// Define the array using the struct
//const VoltageCapacityPair g_ocv_q_released_mah_table[] = {
//    {3000U, 4800U}, // Adjusted: Set full charge capacity at 5000 mV
//    {2837U, 4600U}, // Example reduction based on voltage increase
//    {2911U, 4550U},
//    {2992U, 4500U},
//    {3150U, 4400U},
//    {3219U, 4300U},
//    {3347U, 4200U},
//    {3414U, 4100U},
//    {3505U, 3800U},
//    {3604U, 3000U},
//    {3706U, 2000U},
//    {3812U, 1200U},
//    {3915U, 600U},
//    {4004U, 300U},
//    {4101U, 100U},
//    {4300U, 0U} // Adjusted: Last pair set to 0 capacity at full charge voltage
//};

const VoltageCapacityPair g_ocv_q_released_mah_table[] = {
    {3000U, 4800U}, // Adjusted: Set full charge capacity at 5000 mV
    {2837U, 4600U}, // Example reduction based on voltage increase
    {2911U, 4550U},
    {2992U, 4500U},
    {3150U, 4400U},
    {3219U, 4300U},
    {3347U, 4200U},
    {3414U, 4100U},
    {3505U, 3800U},
    {3604U, 3000U},
    {3706U, 2000U},
    {3812U, 1200U},
    {3915U, 600U},
    {4004U, 300U},
    {4101U, 100U},
    {4300U, 0U} // Adjusted: Last pair set to 0 capacity at full charge voltage
};
// Define NUM_PAIRS based on the size of the array
const size_t NUM_PAIRS = sizeof(g_ocv_q_released_mah_table) / sizeof(g_ocv_q_released_mah_table[0]);

uint16_t get_val_lookup_table(const VoltageCapacityPair *table, size_t numPairs, uint16_t in_value)
{
    uint16_t y_high;
    uint16_t y_low;
    uint16_t x_input_diff;
    uint16_t x_diff;
    uint16_t y_input_diff;
    uint16_t y_return;

    if (numPairs == 0)
    {
        return 0; // Handle zero pairs gracefully
    }

    if (in_value <= table[0].voltage_mV)
    {
        return table[0].capacity_mAh;
    }

    if (in_value >= table[numPairs - 1].voltage_mV)
    {
        return table[numPairs - 1].capacity_mAh;
    }

    for (size_t i = 0; i < numPairs - 1; i++)
    {
        uint16_t x_low = table[i].voltage_mV;
        uint16_t x_high = table[i + 1].voltage_mV;

        if (in_value >= x_low && in_value <= x_high)
        {
            y_low = table[i].capacity_mAh;
            y_high = table[i + 1].capacity_mAh;

            if (in_value == x_low)
            {
                return y_low; // Exact match with a known x value
            }

            x_input_diff = in_value - x_low;
            x_diff = x_high - x_low;

            if (y_high > y_low)
            {
                y_input_diff = ((y_high - y_low) * x_input_diff) / x_diff;
                y_return = y_low + y_input_diff;
            }
            else
            {
                y_input_diff = ((y_low - y_high) * x_input_diff) / x_diff;
                y_return = y_low - y_input_diff;
            }

            return y_return;
        }
    }

    return table[0].capacity_mAh; // Fallback return
}
