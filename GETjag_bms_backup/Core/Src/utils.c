/*
 * utils.c
 *
 *  Created on: Jul 27, 2024
 *      Author: frank
 */


#include "utils.h"


float interpolate(const float a[], const float b[], size_t size, float value_a) {
    if (size < 2) {
        // Not enough points to interpolate
        return 0.0;
    }

    // Check if 'a' array is in ascending order or not
    if (a[0] < a[size - 1]) {
        // Ascending order
        for (size_t i = 0; i < size; i++) {
            if (value_a <= a[i]) {
                if (i == 0) {
                    return b[0]; // value_a smaller than the first element
                } else {
                    // Linear interpolation formula
                    return b[i - 1] + (b[i] - b[i - 1]) * (value_a - a[i - 1]) / (a[i] - a[i - 1]);
                }
            }
        }
        return b[size - 1]; // value_a larger than the last element
    } else {
        // Descending order
        for (size_t i = 0; i < size; i++) {
            if (value_a >= a[i]) {
                if (i == 0) {
                    return b[0]; // value_a larger than the first element in descending array
                } else {
                    // Linear interpolation formula
                    return b[i - 1] + (b[i] - b[i - 1]) * (value_a - a[i - 1]) / (a[i] - a[i - 1]);
                }
            }
        }
        return b[size - 1]; // value_a smaller than the last element in descending array
    }
}

float interpolate_SOC(const float OCV[], const float SOC[], int size, float OCV_value){
    // Check if the OCV_value is outside the bounds of the OCV array
    if (OCV_value <= OCV[size - 1]) {
        return SOC[size - 1];
    }
    if (OCV_value >= OCV[0]) {
        return SOC[0];
    }

    // Find the interval in which OCV_value is located
    for (int i = 0; i < size - 1; i++) {
        if (OCV[i] >= OCV_value && OCV_value >= OCV[i + 1]) {
            // Perform linear interpolation
            float OCV1 = OCV[i], OCV2 = OCV[i + 1];
            float SOC1 = SOC[i], SOC2 = SOC[i + 1];
            float SOC_value = SOC1 + (OCV_value - OCV1) * (SOC2 - SOC1) / (OCV2 - OCV1);
            return SOC_value;
        }
    }

    // If value is not found in the range (should not happen)
    return 0.0;
}

