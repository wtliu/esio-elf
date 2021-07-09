/*
 * funcoes.c
 *
 *  Created on: 26 de jan de 2020
 *      Author: Lange
 */


#include "global.h"

void alfabeta_to_abc_pow(float *x, float *y) {
    y[0] = sqrt_2by3*x[0];
    y[1] = sqrt_1by6*x[0] + sqrt_1by2*x[1];
    y[2] = sqrt_1by6*x[0] - sqrt_1by2*x[1];
}

void alfabeta_to_abc_amp(float *x, float *y) {
    y[0] = x[0];
    y[1] = -0.5*x[0] + sqrt3_by_2*x[1];
    y[2] = -0.5*x[0] - sqrt3_by_2*x[1];
}


void wrap_ang_180(float *x) {
    if (*x >= 0.5)
        *x -= 1.0;
    else if (*x < -0.5)
        *x += 1.0;
}

uint16_t wrap_ang_360_fix(int32_t x) {
    if (x >= Q12_60deg_360)
        return (uint16_t)(x - Q12_60deg_360);
    if (x < 0)
        return (uint16_t)(x + Q12_60deg_360);
    return x;
}

int16_t wrap_ang_180_fix(int16_t x) {
    if (x >= Q12_60deg_180)
        return x - Q12_60deg_360;
    if (x < -Q12_60deg_180)
        return x + Q12_60deg_360;
    return x;
}
