/*************************************************************************
                                INEP-UFSC
__________________________________________________________________________

ARQUIVO: global.h
TíTULO : Definições de variáveis globais

__________________________________________________________________________

DATA: 22/12/2020
ATUALIZAÇÃO
__________________________________________________________________________
DESENVOLVIMENTO: ESIO ELOI / ANDRé LANGE
__________________________________________________________________________
OBSERVAÇÕEES:

 *
 * global.h
 *
 *  Created on: 22 de dez de 2020
 *      Author: Esio Eloi
 */

/*
 * Arquivos inclusos
 */


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "definicoes.h"
#include "constantes.h"
#include "typedef.h"
#include "funcoes.h"
#include "tabelas.h"
#include "F2837xS_user_device.h"

// Inserir os .h restantes e necessários


/**********************************************************/
/*                  Variáveis globais                     */
/**********************************************************/

//Frequencia de comutação para a modulação vetorial

#define f_sw_vet 480.0 //480
#define f_sw_sinc 480.0

// Frequência fundamental minima para sincronizar f_sw com a fundamental com modulação vetorial
#define f_min_vet_sinc 100  //5.0

//Frequencia fundamental minima para emprego de modulação sincrona

#define f_min_sinc 22.9

//Frequência da malha de controle
#define f_ctrl 5000

// Constantes matématicas
#define RAIZ2 ((float)1.4142135623730950488016887242097)
#define RAIZ2INV ((float)0.70710678118654752440084436210485)
#define ONE_SIXTH 0.16666666666666666666666666666667
#define ONE_THIRD 0.33333333333333333333333333333333

//Macros da modulação V/f
#define VS 100.0         // Amplitude da tensâo de fase do inversor


// Estados de máquina principal

typedef enum {
    _Protecao = -2,
    _Reinicio = -1,
    _Inicializacao = 0,
    _Operando = 1,
} eEstado;

// Erros
typedef enum {
    ERR_Normal = 0,
} eErro;

//Causas de reinício
typedef enum {
    RST_Normal = 0,
} eReinicio;

// Periodo PWM - Carrier
#define PERIODO_INICIAL 1000
#define TEMPO_MORTO 10

//Periodo EPW11 e EPWM 12 igual f_ctrl
#define PRD_CTRL 1999      // Tclock_pwm = 10e6


//Contadores de tempo
extern unsigned tempo_segundo;
extern unsigned long tempo_cpu;
extern unsigned long tempo_operando;

//Máquina de estados
extern eEstado estado, estado_anterior;
extern eErro erro;
extern eReinicio causa_reinicio;


//Variaveis de controle
extern unsigned angulos, angulos_mod;
extern float M;
extern float fo, fo_mod; //Frequencia fundamental de saida
extern float To, To_mod; //Periodo da fundamental
extern unsigned tempo_morto_ctr;
extern int16_t Tm_angfix, Tm_angfix_mod; //ângulo morto em ponto fixo
extern int16_t ang_min;


// Variaveis da modulação V/F
extern float Vboost;
extern float Vinv;
extern float Maux;
extern float G;


// Variáveis globais
extern uint16_t Mfix;
extern float theta; // Ângulo atual da componente fundamental normalizado em 360 graus
extern int16_t thetafix, theta1fix, theta2fix; //Ângulo atual da componente fundamental em Q12 normalizado em 60 graus

//Flags
extern bool equilibrar;   // 1 para equilibrar o barramento utilizando os estados redundantes
extern bool sw_reset;
extern bool sw_protecao;


//Fatores de conversão entre ponto fixo e flutuante

extern const float Q16_to_F; //Q16 para float
extern const float Q12_to_F; //Q12 para float
extern const float Q12_60deg_to_F; // ângulo normalizado em 60 graus em Q12 para float
extern const float F_to_Q12_60deg;
extern const int16_t Q12M;
extern const int16_t Q12_60deg_360; //Valor em Q12 que corresponde a 360 graus
extern const float Q12_60deg_360f;
extern const int16_t Q12_60deg_180; //Valor em Q12 que corresponde a 180 graus
extern const int16_t Q12_60deg_90; //Valor em Q12 que corresponde a 90 graus
extern const int16_t Q12_60deg_30; //Valor em Q12 que corresponde a 30 graus


//Instrumentação
extern unsigned Io_unsigned[2];    // Correntes de saida
extern int Vdc_signed[2];        // Tensões de barramento
extern int16_t Io_signed[2];        // Variaveis com sinal para leitura simetrica
extern float Io_float[2], Vdc_float[2]; // Valores no SI nao calibrados
extern float Io[3], Vdc[2];      // Valores instantaneos no SI calibracao
extern float Vdc_tot, Vdc_dif, Vdc_des;

extern unsigned Vo_unsigned[2]; //Tensão de saída do inv
extern unsigned Vg_unsigned[2]; //Tensões de de entrada da rede
extern int Vo_signed[2], Vg_signed[2]; //variavel com sinal
extern float Vo_float[2], Vg_float[2]; //Valores no SI não calibrados
extern float Vo[2], Vg[2]; //Valores instantâneos no SI após calibração



//Modulacao vetorial
extern float m_albe[2];
extern float m_abc1[3], m_abc2[3];
extern float mod1[3], mod2[3];
extern float m_max1, m_min1, m_sum1;
extern float m_sum2[3], m_sum2_mod[3];
extern float m_max2, m_min2, m_sum3;
extern float theta_mod1, theta_mod2;
extern float f_sw, Tsw, Tsw_mod, Tsw2_mod;
extern int16_t theta_step;
extern uint16_t n_theta, N_theta, N_theta_mod;
extern uint16_t TBPRD, TBPRD2;
extern int16_t phi_mod;

//Rampa de partida
extern float rampa_vf;
extern float sentido;

//Calibracao
extern float Io_offset[2];
extern float Io_ganho[2];

extern float Vdc_offset[2];
extern float Vdc_ganho[2];

extern float Vo_offset[2];
extern float Vo_ganho[2];

extern float Vg_offset[2];
extern float Vg_ganho[2];

extern int teste;
extern unsigned cont_descarte;

// SDFM variables

#define SDFM_BUF_LEN         7              // SDFM buffer length
#define MAX_SAMPLES          1             // SDFM samples

extern int16_t  Filter3_Result_SD1[MAX_SAMPLES]   ; // Tensao Vn - Vdc
extern int16_t  Filter2_Result_SD2[MAX_SAMPLES]   ; // Corrente IinvA
extern int16_t  Filter3_Result_SD2[MAX_SAMPLES]   ; // Corrente IinvB
extern int16_t  Filter4_Result_SD2[MAX_SAMPLES];// Tensão Vp - Vdc

extern Uint16 VP_measure[SDFM_BUF_LEN], Ia_measure[SDFM_BUF_LEN], Buf[SDFM_BUF_LEN];

extern uint16_t CMPC11, CMPD11;  // Variaveis para sincronizar o SDFM com os PWMs
extern uint16_t CMPC12, CMPD12;


extern int sdfm_index_SD1_vn, sdfm_index_SD2_vp, sdfm_index_SD2_ia, sdfm_index_SD2_ib; // variavel para indexar a amostra para sinc


// Variaveis de teste

extern Uint16 Med_teste;
extern int16_t  var_teste[50];
extern float  var_teste1[50];
extern float  var_teste2[50];

//Funçoes externas
void TrocarEstado(eEstado novo_estado);
void Erro(eErro err);
void Sinalizar(int16_t val);

