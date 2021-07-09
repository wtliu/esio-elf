/*************************************************************************
                                INEP-UFSC
__________________________________________________________________________

ARQUIVO: global.c
TíTULO : Definições de variáveis globais e funcões

__________________________________________________________________________

DATA: 26/12/2020
ATUALIZAÇÃO
__________________________________________________________________________
DESENVOLVIMENTO: ESIO ELOI / ANDRé LANGE
__________________________________________________________________________
OBSERVAÇÕES:
**************************/

#include "global.h"

/**********************************************************/
/*                  Variáveis globais                 */
/**********************************************************/

//Contadores de tempo
unsigned tempo_segundo; //Contador para base de tempo de 1 segundo [ms]
unsigned long tempo_cpu; //Tempo desde a inicialização da CPU [s]
unsigned long tempo_operando; //Tempo com o conversor ligado [s]


//Variáveis de controle
unsigned angulos= 0, angulos_mod;
float M;
float fo, fo_mod;      //Frequência fundamental de saída
float To, To_mod;      //Período da fundamental
unsigned tempo_morto_ctr;
int16_t Tm_angfix, Tm_angfix_mod;    //Ângulo morto em ponto fixo
int16_t ang_min;

// Variaveis da modulação V/F
float Vboost;
float Vinv;
float Maux;
float G;


//Variáveis globais
uint16_t Mfix;
float theta; //Ângulo atual da componente fundamental normalizado em 360 graus
int16_t thetafix, theta1fix, theta2fix; //Ângulo atual da componente fundamental em Q12 normalizado em 60 graus


//Fatores de conversão entre ponto fixo e flutuante
const float Q16_to_F = 1.0/65536.0; //Q16 para float
const float Q12_to_F = 1.0/4096.0; //Q12 para float
const float Q12_60deg_to_F = (1.0/4096.0) * (60/360.0); //Ângulo normalizado em 60 graus em Q12 para float
const float F_to_Q12_60deg = 4096.0 * 6;  //Float to Q12: (4096)x(Ang)*(6/36)
const int16_t Q12M = 0x0fff;
const int16_t Q12_60deg_360 = (1 << 12) * 6; //Valor em Q12 que corresponde a 360 graus
const float Q12_60deg_360f = (1 << 12) * 6.0;
const int16_t Q12_60deg_180 = (1 << 12) * 3; //Valor em Q12 que corresponde a 180 graus
const int16_t Q12_60deg_90 = (1 << 12) * 1.5; //Valor em Q12 que corresponde a 90 graus
const int16_t Q12_60deg_30 = (1 << 11); //Valor em Q12 que corresponde a 30 graus


//Instrumentação

unsigned Io_unsigned[2]; //Correntes de saída
int16_t Io_signed[2]; //Variáveis com sinal para leitura simétrica
float Io_float[2], Vdc_float[2]; //Valores no SI não calibrados
float Io[3], Vdc[2]; //Valores instantâneos no SI após calibração

int Vdc_signed[2]; //Tensões de barramento
float Vdc_tot, Vdc_dif, Vdc_des; //Tensão total e desbalanço

unsigned Vo_unsigned[2]; //Tensão de saída do inv
unsigned Vg_unsigned[2]; //Tensões de de entrada da rede
int Vo_signed[2], Vg_signed[2]; //variavel com sinal
float Vo_float[2], Vg_float[2]; //Valores no SI não calibrados
float Vo[2], Vg[2]; //Valores instantâneos no SI após calibração


//Modulação vetorial
float m_albe[2];
float m_abc1[3], m_abc2[3];
float mod1[3], mod2[3];
float m_max1, m_min1, m_sum1;
float m_sum2[3], m_sum2_mod[3];
float m_max2, m_min2, m_sum3;
float theta_mod1, theta_mod2;
float f_sw, Tsw, Tsw_mod, Tsw2_mod;
int16_t theta_step;
uint16_t n_theta, N_theta, N_theta_mod;
uint16_t TBPRD, TBPRD2;
int16_t phi_mod;

//Rampa de partida
float rampa_vf; //Rampa de frequência fundamental em pu
float sentido; //Sentido da rampa

//Calibração
float Io_offset[2] = {0.0, 0.0};
float Io_ganho[2] = {1.0, 1.0};

float Vdc_offset[2] = {0.0, 0.0};
float Vdc_ganho[2] = {1.0, 1.0};

float Vo_offset[2] = {0.0, 0.0};
float Vo_ganho[2] = {1.0, 1.0};

float Vg_offset[2] = {0.0, 0.0};
float Vg_ganho[2] = {1.0, 1.0};

int teste;
unsigned cont_descarte;

//Flags
bool equilibrar;  // 1 para equilibrar o barramento utilizando os estados redundantes
bool sw_reset;
bool sw_protecao;


// Variaveis do SDFM

int16_t  Filter3_Result_SD1[MAX_SAMPLES]; // Tensao Vn - Vdc
int16_t  Filter2_Result_SD2[MAX_SAMPLES]; // Corrente IinvA
int16_t  Filter3_Result_SD2[MAX_SAMPLES]; // Corrente IinvB
int16_t  Filter4_Result_SD2[MAX_SAMPLES];   // Tensão Vp - Vdc


// buffer do SDFM
 Uint16  VP_measure[SDFM_BUF_LEN];
 Uint16 Ia_measure[SDFM_BUF_LEN];
 Uint16 Buf[SDFM_BUF_LEN];


 // Variaveis de teste - buffers para visualização no grph signal
 int16_t  var_teste[50];
 float  var_teste1[50];

 Uint16 Med_teste;

//Máquina de estados

eEstado estado, estado_anterior;
eErro erro;
eReinicio causa_reinicio;

//Troca o estado atual
void TrocarEstado(eEstado novo_estado) {
    DINT;
    estado = novo_estado;
    EINT;
}

//Rotina de tratamento de erro
void Erro(eErro novo_erro) {
    //Retira os sinais de comando
    ForceTrip();

    //Registra o erro
    erro = novo_erro;

    //Muda o estado para proteção
    estado = _Protecao;
}

//Atualiza LEDs de sinalização
void Sinalizar(int val) {

}









