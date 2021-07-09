/*
 * funcoes.h
 *
 *  Created on: 26 de dez de 2020
 *      Author: Lange
 */

#ifndef FUNCOES_H_
#define FUNCOES_H_

#include "typedef.h"

#define sqrt_2by3 0.81649658092772603273242802490196
#define sqrt_1by6 0.40824829046386301636621401245098
#define sqrt_1by2 0.70710678118654752440084436210485
#define sqrt3_by_2 0.86602540378443864676372317075294

void alfabeta_to_abc_pow(float *x, float *y);
void alfabeta_to_abc_amp(float *x, float *y);
void wrap_ang_360(float *x);
void wrap_ang_180(float *x);
uint16_t wrap_ang_360_fix(int32_t x);
int16_t wrap_ang_180_fix(int16_t x);

#endif /* FUNCOES_H_ */
