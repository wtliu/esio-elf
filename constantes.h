/*
 * constantes.h
 *
 *  Created on: 22 de dez de 2021
 *      Author: Lange
 */


#ifndef CONSTANTES_H_
#define CONSTANTES_H_


// Constantes de proteção
extern const float VDC_MAX;      // tensão máxima do barramento
extern const float VDC_DES;     // tensão máxima de desbalanço do barramento
extern const float Current_prot;




//Reinicialização
extern const unsigned long Tempo_base_reinicio;
extern const unsigned Multiplicador_tempo_reinicio;
extern const unsigned long Tempo_operacao_OK;

//Especificações
extern const float f_clk;
extern const float Tm;
extern const float Mnom;
extern const float fo_nom;
extern float Tperm_min;

//Rampa de partida
extern const float rampa_inc; //Inclinação da rampa de partida [pu/segundo]



//Constantes de Instrumentação

extern const int16_t Offset_Io_a;
extern const int16_t Offset_Io_b;
extern const int16_t Offset_vdc_p;
extern const int16_t Offset_vdc_n;

extern const int16_t Offset_Vo;
extern const int16_t Offset_Vg;

extern const float Ganho_Io;
extern const float Ganho_Vdc;

extern const float Ganho_Vo;
extern const float Ganho_Vg;

extern const uint16_t   Lim_sup_I;  // limite superior de corrente do comparador SDFM
extern const uint16_t   Lim_inf_I;  // limite inferior de corrente do comparador SDFM


#endif /* CONSTANTES_H_ */
