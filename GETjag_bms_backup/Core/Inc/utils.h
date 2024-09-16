/*
 * utils.h
 *
 *  Created on: Jul 27, 2024
 *      Author: frank
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>



float interpolate(const float a[], const float b[], size_t size, float value_a);

float interpolate_SOC(const float OCV[], const float SOC[], int size, float OCV_value);

#endif /* INC_UTILS_H_ */
