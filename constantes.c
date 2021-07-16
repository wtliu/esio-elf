/*
 * constantes.c
 *
 *  Created on: 13 de ago de 2020
 *      Author: Lange
 */


#include "global.h"


// Macros de prote��o
#define CURR_PROT 12.0     // valor maximo de corrente para a prote��o do SDFM


//Macros da modula��o V/f

#define Kir 2.5          // Ganho do Vboost
#define InitV 6.0        // freq

// Constantes de prote��o
const float VDC_MAX = 600.0;      // tens�o m�xima do barramento
const float VDC_DES = 85.0;     // tens�o m�xima de desbalan�o do barramento
const float Current_prot = CURR_PROT;

//Reinicializa��o
const unsigned long Tempo_base_reinicio = 5000; //ms
const unsigned Multiplicador_tempo_reinicio = 1;
const unsigned long Tempo_operacao_OK = 600; //segundos

//Especifica��es
const float f_clk = 10e6;      //Frequ�ncia de clock do modulador PWM
const float Tm = 1e-6;         //Tempo morto [s]
float Tperm_min = 10e-6;       //Tempo m�nimo de perman�ncia de uma sequ�ncia de dois vetores
const float Mnom = 1.0;        //�ndice de modula��o para condi��o nominal
const float fo_nom = 50.0;     //Frequ�ncia fundamental nominal = 60 (default)

//Rampa de partida
const float rampa_inc = 1.0 / (60.0*f_ctrl); //Inclina��o da rampa de partida [pu/200us]




//Constantes de Instrumenta��o   // realizar ajustes

const int16_t Offset_Io_a = 38;
const int16_t Offset_Io_b = 42;

const int16_t Offset_vdc_p = 145;
const int16_t Offset_vdc_n = 5;

const int16_t Offset_Vo = 2045;
const int16_t Offset_Vg = 1861;

const float Ganho_Vo = 0.515625; // vac = (3.0/4096)*(88k/2.5*100)
const float Ganho_Vg = ((3.0/4096)*4.93) * 100.0; //FE = V

const float Ganho_Io = 32/(0.16 * 65536); //  convers�o para o valor real: 32/(0.16*2^16)

const float Ganho_Vdc = ((0.32/32767)*(40000/30));  // convers�o para o valor real: Vdc1 = (0,32/32767) *((2.5*12)/40k)* SD - sigma delta

// limite superior de corrente do comparador SDFM
const uint16_t   Lim_sup_I = (uint16_t)( (((0.16/32) * CURR_PROT) + 0.5) * 32768 + 0.5 );
const uint16_t   Lim_inf_I = (uint16_t)( (((0.16/32) * (-CURR_PROT)) + 0.5) * 32768 + 0.5 );



