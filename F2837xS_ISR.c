//###########################################################################
//
// FILE:   F2837xS_ISR.c
//
// TITLE:  F2837xS Device Default Interrupt Service Routines
//
//###########################################################################
// $TI Release: F2837xS Support Library v200 $
// $Release Date: Tue Jun 21 13:52:16 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################


/****************************************************************************
*Observação: As interrupções utilizadas aqui devem estar desativadas/comentadas
* no arquivo DeafultISR.c para não haver conflitos.
*
**********************************************************************/



// Included Files

#include "global.h"
#include "F2837xS_sdfm_drivers.h"       // SDFM definitions
#include "F2837xS_struct.h"
#include "F2837xS_sdfm.h"



//Definir para usar declarações inline
//#define USE_INLINE

#ifdef USE_INLINE
    #define INLINE inline
#else
    #define INLINE
#endif


// Estrutura para armazenar os CMPA/B, que são os sinais moduladores

typedef struct TCMP {
    volatile union CMPA_REG *S1xCMPA;   // S1 on
    volatile union CMPB_REG *S1xCMPB;   // S1 off
    volatile union CMPA_REG *S4xCMPA;   //S4 on
    volatile union CMPB_REG *S4xCMPB;   //S4 off
} TCMPRegs;

const TCMPRegs TabCMPRegs[3] = {
    {&EPwm7Regs.CMPA, &EPwm7Regs.CMPB, &EPwm8Regs.CMPA, &EPwm8Regs.CMPB},
    {&EPwm10Regs.CMPA, &EPwm10Regs.CMPB, &EPwm2Regs.CMPA, &EPwm2Regs.CMPB},
    {&EPwm6Regs.CMPA, &EPwm6Regs.CMPB, &EPwm9Regs.CMPA, &EPwm9Regs.CMPB},
};

// ******* Protótipos de funções - Utilizadas nas interrupções *******

INLINE void Instrumentacao_Io();
INLINE void Instrumentacao_Vdc();
INLINE void Instrumentacao_Vo_inv_Vg();
INLINE void Rampa_VF();
INLINE void Modulacoes();
INLINE void ModulacaoVetorial();
INLINE void ConfigurarModulacaoVetorial();

void programar_moduladoras(float mod1, float mod2, volatile union CMPA_REG *CMPA1, volatile union CMPB_REG *CMPB1, volatile union CMPA_REG *CMPA4, volatile union CMPB_REG *CMPB4);
void spacev_3L(const float *m, float *mod);

//index sdfm samples
//int sdfm_index;

//********** INTERRUPÇÕES ************//



//Interrupção lenta de modulação

interrupt void EPWM7_ISR(void)
{

    static int GPIO4_count = 0;            // Counter for pin toggle


      // Debug - led blink in interrupt
     if(GPIO4_count++ > 3500)                  // Toggle slowly to see the LED blink
        {
           GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1;  // Toggle the pin
           GPIO4_count = 0;                       // Reset the counter
        }

    if (estado >= _Operando) {
        //Executa a modulação necessária
        ModulacaoVetorial();
       }


    EPwm7Regs.ETCLR.bit.INT = 1;
    // Acknowledge this interrupt to receive more interrupts from group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}


//Interrupção lenta para controle
interrupt void ADCA2_ISR(void)
{

   sdfm_index_SD1_vn = 1;               // Quando entra na interrupção de controle seta o index
   sdfm_index_SD2_vp = 1;
   sdfm_index_SD2_ia = 1;
   sdfm_index_SD2_ib = 1;

   static volatile int pareAqui2=0;

  //Habilita interrupções simultâneas
    EINT;

  //Limpa flag de estouro do temporizador que disparou o ADC
    CpuTimer1Regs.TCR.bit.TIF = 1;

   //Rotinas de instrumentação
    Instrumentacao_Vdc(); // Leitura da tensão do barramento CC atraves do SDFM
    Instrumentacao_Io(); //Correntes de saída
    Instrumentacao_Vo_inv_Vg(); //Tensões de saida do inversor e da entrada da rede

    if (Vdc_des > 5.f){
        pareAqui2 = 1;
    }

   //Rampa
    if (estado >= _Operando) {
     //Rampa V/F de partida
        Rampa_VF();

    //Rotinas de modulação
        Modulacoes();
       }

    DINT;

    //Modulação vetorial
      fo_mod = fo;
      Tsw_mod = Tsw;
      N_theta_mod = N_theta;


    // To receive more interrupts from this PIE group, acknowledge this interrupt.
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

}

interrupt void SD1_ISR(void)
{
    // Leitura de Vn - SD1 -Filter 3

   //Uint32 sdfmReadFlagRegister1 = 0;
   //static uint16_t loopCounter1 = 0;
   union SDIFLG_REG sdfmReadFlagRegister1;
   static int index1_vn;

   sdfmReadFlagRegister1.all = Sdfm_readFlagRegister(SDFM1);

   if(sdfmReadFlagRegister1.bit.AF3 == 1){

       if(sdfm_index_SD1_vn == 1){

           sdfm_index_SD1_vn = 0;
           index1_vn = 0;
       }

    index1_vn = index1_vn + 1;

   }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;         // Must acknowledge the PIE group


    if (index1_vn == 7){
        Filter3_Result_SD1 [0] = ~(SDFM1_READ_FILTER3_DATA_16BIT-1); // Vn
    }

    Sdfm_clearFlagRegister(SDFM1,sdfmReadFlagRegister1.all);

    //sdfmReadFlagRegister1 = Sdfm_readFlagRegister(SDFM1);

}


interrupt void SD2_ISR(void)
{
    // Leitura de Ia - SD2 -Filter 2, Ib SD2 Filter 3, Vp SD2 Filter 4

    //Uint32 sdfmReadFlagRegister2 = 0;
    //static uint16_t loopCounter2 = 0;
    union SDIFLG_REG sdfmReadFlagRegister2;
    static volatile int index2_vp;
    static volatile int index2_ia;
    static volatile int index2_ib;
    static volatile int pareAqui = 0;

    sdfmReadFlagRegister2.all = Sdfm_readFlagRegister(SDFM2);

    if(sdfmReadFlagRegister2.bit.AF4 == 1){

        if(sdfm_index_SD2_vp == 1){
            sdfm_index_SD2_vp = 0;
            index2_vp = 0;
        }

        index2_vp = index2_vp + 1;

    }

    if(sdfmReadFlagRegister2.bit.AF2 == 1){

        if(sdfm_index_SD2_ia == 1){
            sdfm_index_SD2_ia = 0;
            index2_ia = 0;
        }

        index2_ia = index2_ia + 1;

    }

    if(sdfmReadFlagRegister2.bit.AF3 == 1){

        if(sdfm_index_SD2_ib == 1){
            sdfm_index_SD2_ib = 0;
            index2_ib = 0;
        }

        index2_ib = index2_ib + 1;

    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;         // Must acknowledge the PIE group


    if (index2_vp == 7){

        Filter4_Result_SD2[0] = ~(SDFM2_READ_FILTER4_DATA_16BIT-1);  // inverter o sinal VP

        if (Filter4_Result_SD2[0] < 10){
            pareAqui = 1;
        }
    }

    if (index2_ia == 7){

        Filter2_Result_SD2[0] = SDFM2_READ_FILTER2_DATA_16BIT; // Ia
    }

    if (index2_ib == 7){

        Filter3_Result_SD2[0] = SDFM2_READ_FILTER3_DATA_16BIT;  // Ib
    }

     //Sdfm2Regs.SDIFLGCLR.bit.MIF = 1;
    Sdfm_clearFlagRegister(SDFM2,sdfmReadFlagRegister2.all);
    //sdfmReadFlagRegister2.all = Sdfm_readFlagRegister(SDFM2);



}

// Interrupção do trip - proteção de corrente
interrupt void EPWM7_TZ_ISR(void)
{

    // To receive more interrupts from this PIE group,
    // acknowledge this interrupt.
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;


    // veio por sobrecorrente no sensor
    if(EPwm7Regs.TZOSTFLG.bit.DCAEVT1 == 1){

        TrocarEstado(_Protecao);
        EPwm7Regs.TZEINT.bit.OST = 0; // disable interrupt

    }
    else
    {

        // veio pelo sw_Reset ou outra proteção
         asm(" NOP");

         EALLOW;
         EPwm7Regs.TZCLR.bit.OST=0; // Limpa os flags do trip-zone
         EDIS;
        //asm ("  ESTOP0"); // debug only to halt the processor here
    }
}


interrupt void XINT1_ISR(void){

    //Interrupção do sinal de proteção  do driver fase C - gpio41

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    TrocarEstado(_Protecao); // vai para o estado de proteção e força o trip

}

interrupt void XINT2_ISR(void){

    //Interrupção do sinal de proteção  do driver fase b - gpio87

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    TrocarEstado(_Protecao); // vai para o estado de proteção e força o trip

}

interrupt void XINT3_ISR(void){

    //Interrupção do sinal de proteção  do driver fase a - gpio78

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    TrocarEstado(_Protecao); // vai para o estado de proteção e força o trip

}



// ************************** Funções utilizadas **************************************** //

INLINE void ModulacaoVetorial()
{
       unsigned fase;


    //Registrador de período
      Tsw2_mod = 0.5 * Tsw_mod; //Meio período de comutação
      TBPRD2 = ((uint16_t)( f_clk*Tsw2_mod) + 1 ) >> 1; //Meio período arredondando
      TBPRD = TBPRD2 << 1; //Período inteiro

    //Passo do ângulo
      theta_step = ((int16_t)(fo_mod*Tsw2_mod*Q12_60deg_360f) + 1) >> 1;

    //Avanço do primeiro ângulo - geração do angulo para formar a moduladora
        if (fo_mod >= f_min_vet_sinc) {

            //Índice arredondado para o inteiro mais próximo

            if (theta_step > 0) {
                n_theta = ((((uint16_t)thetafix<<1)/(uint16_t)theta_step) + 1) >> 1;
            }

            if (n_theta >= N_theta_mod) n_theta -= N_theta_mod;

            //Atualização do primeiro ângulo

            theta1fix = n_theta * theta_step;

            if (theta1fix < thetafix) theta1fix = thetafix; //Não permite retroceder ângulo
         }
        else
            theta1fix = thetafix;

     //Correção da fase devido ao atraso do modulador
        phi_mod = (theta_step + 1) >> 1; //Atraso do ZOH arredondado

        theta_mod1 = (theta1fix + phi_mod) * Q12_60deg_to_F;

     //Geração das moduladoras em alfa-beta
        m_albe[0] = M * __cospuf32(theta_mod1);
        m_albe[1] = M * __sinpuf32(theta_mod1);

     //Transformação para abc
        alfabeta_to_abc_amp(m_albe, m_abc1);

     //Geração da moduladora para modulação vetorial 3 níveis
        spacev_3L(m_abc1, mod1);

     //Avanço do segundo ângulo
           if (fo_mod >= f_min_vet_sinc) {

               //Avanço síncrono
               if (++n_theta >= N_theta_mod) n_theta -= N_theta_mod;

               theta2fix = n_theta * theta_step;
           }
           else {
               theta2fix = theta1fix + theta_step; //Avanço assíncrono
               if (theta2fix >= Q12_60deg_360) theta2fix -= Q12_60deg_360;
           }

       //Correção da fase devido ao atraso do modulador
          theta_mod2 = (theta2fix + phi_mod) * Q12_60deg_to_F;

      //Geração das moduladoras em alfa-beta
        m_albe[0] = M * __cospuf32(theta_mod2);
        m_albe[1] = M * __sinpuf32(theta_mod2);

      //Transformação para abc
        alfabeta_to_abc_amp(m_albe, m_abc2);

      //Geração da moduladora para modulação vetorial 3 níveis
        spacev_3L(m_abc2, mod2);

      //Programação das moduladoras

      const TCMPRegs *CMPReg = TabCMPRegs;

      for (fase=0; fase<3; fase++) {
           programar_moduladoras(mod1[fase], mod2[fase], CMPReg->S1xCMPA, CMPReg->S1xCMPB, CMPReg->S4xCMPA, CMPReg->S4xCMPB);
           CMPReg++;
           }

      //Avança o ângulo referente ao último ciclo de portadora
      thetafix = theta2fix + theta_step;
      if (thetafix >= Q12_60deg_360) thetafix -= Q12_60deg_360;


      //Período da portadora
      EPwm7Regs.TBPRD = TBPRD;

}

//Programa os valores de comparação do modulador PWM na modulação vetorial
void programar_moduladoras(float mod1, float mod2, volatile union CMPA_REG *CMPA1, volatile union CMPB_REG *CMPB1, volatile union CMPA_REG *CMPA4, volatile union CMPB_REG *CMPB4)
{
    long CMPA, CMPB;

    //Primeiro estado
    if (mod1 >= 0) {
    //Semiciclo positivo

        CMPB = (long)(mod1 * TBPRD2);

        if (CMPB > TBPRD2) CMPB = TBPRD2;

        CMPB1->bit.CMPB = (unsigned)CMPB; //  Quando S1 desliga

        CMPA4->bit.CMPA = (unsigned)TBPRD2; //  Quando S4 liga
    }
    else {
    //Semiciclo negativo
         CMPA = (long)((1.0+mod1)*TBPRD2);

         if (CMPA < 0) CMPA = 0;

         CMPA4->bit.CMPA = (unsigned)CMPA; //  Quando S4 liga

         CMPB1->bit.CMPB = 0;  // Quando S1 desliga

    }

    //Segundo estado
       if (mod2 >= 0) {
        //Semiciclo positivo
          CMPA = TBPRD - (long)(mod2*TBPRD2);

          if (CMPA <= TBPRD2) CMPA = TBPRD2+1; //Saturação na metade

          CMPA1->bit.CMPA = (unsigned)CMPA; //  Quando S1 liga

          CMPB4->bit.CMPB = (unsigned)(TBPRD2+1); // Quando S4 desliga
       }
       else {
         //Semiciclo negativo
          CMPB = (long)((1.0-mod2)*TBPRD2);

           if (CMPB > TBPRD) CMPB = TBPRD; //Saturação na metade

           CMPB4->bit.CMPB = (unsigned)CMPB; //Quando S4 desliga

           CMPA1->bit.CMPA = (unsigned)TBPRD; // Quando S1 liga
       }


}

//Cálcula a função de modulação com a tensão de modo comum para modulação vetorial de 3-níveis
void spacev_3L(const float *m, float *mod)
{

    unsigned fase;
    float *aux;

    //Caso especial com moduladoras zeradas
        if ((m[0] == 0) && (m[1] == 0) && (m[2] == 0)) {
            mod[0] = 0;
            mod[1] = 0;
            mod[2] = 0;
            return;
        }

    //Primeiro cálculo de máximos e mínimos
       m_max1 = fmaxf(m[0], fmaxf(m[1], m[2]));
       m_min1 = fminf(m[0], fminf(m[1], m[2]));
       m_sum1 = -0.5*(m_max1 + m_min1);

    //Primeiro somatório
        for (fase=0; fase<3; fase++) {
            m_sum2[fase] = m[fase] + m_sum1;

             aux = m_sum2_mod + fase;  // aux recebe o endereço de m_sum2_mod
            *aux = m_sum2[fase];       // m_sum2_mod recebe m_sum2
           //Função mod
            if (*aux < 0.0) *aux += 1.0; // m_sum2_mod = m_sum2_mod + 1
            if (*aux > 1.0) *aux -= 1.0; // // m_sum2_mod = m_sum2_mod - 1
          }

     //Segundo cálculo de máximos e mínimos
      m_max2 = fmaxf(m_sum2_mod[0], fmaxf(m_sum2_mod[1], m_sum2_mod[2]));
      m_min2 = fminf(m_sum2_mod[0], fminf(m_sum2_mod[1], m_sum2_mod[2]));
      m_sum3 = 0.5 - 0.5*(m_max2 + m_min2);

     //Último somatório
      for (fase=0; fase<3; fase++) mod[fase] = m_sum2[fase] + m_sum3;  // injeção do modo comum formando a modulação de três níveis
}

//Implementação da rampa V/F
INLINE void Rampa_VF() {

    //static float *Ptr_buf1 = var_teste1;

    //Cálculo da rampa V/F -  Vai de zero a A
    rampa_vf += rampa_inc*sentido;
    rampa_vf = fminf(fmaxf(rampa_vf, 0), 1.0);   //Limita entre 0.0 e 1.0


    //Frequência fundamental e índice de modulação - Aplica-se o ganho G
    if (sentido != 0) {

        fo = fo_nom * rampa_vf;

        //Vboost = 0;
        Vinv = G*fo;
        Maux = (2*Vinv)/Vdc_tot;
        M = Mnom * rampa_vf;
    }


    //Período da fundamental.
    To = 1/fo;


}


//Escolha da modulação - por enquanto só tem a vetorial
INLINE void Modulacoes() {

    ConfigurarModulacaoVetorial();

    //Período de comutação
    Tsw = 1.0 / f_sw;
}

//Configura a modulação vetorial
INLINE void ConfigurarModulacaoVetorial()
{
    //Cálculo da frequência de comutacao
    if (fo >= f_min_vet_sinc) {
        //Frequência múltipla de 3 vezes a fundamental

        uint16_t nf = (uint16_t)(ONE_THIRD * To * f_sw_vet); //Arredonda para baixo

        f_sw = 3*nf*fo; //Frequência de comutação do interruptor sincronizada com a fundamental

        N_theta = 12*nf; //Número de passos para o ângulo theta
    }
    else {
        //Frequência de comutação assíncrona

        f_sw = f_sw_vet;

        N_theta = 0;
    }
}


//Instrumentação das tensões de saida do inversor e da tensão da rede lidas pelo ADC
INLINE void Instrumentacao_Vo_inv_Vg()
{
    unsigned fase;

   // Valores brutos - leitura do AD
    Vo_unsigned[0] = AdcaResultRegs.ADCRESULT0; //linha Vinv_AB

    Vg_unsigned[0] = AdcbResultRegs.ADCRESULT1; //linha Vg_AB

    Vo_unsigned[1] = AdcaResultRegs.ADCRESULT3; //linha Vinv_BC

    Vg_unsigned[1] = AdcbResultRegs.ADCRESULT4; //linha Vg_BC

    for (fase=0; fase<2; fase++)
    {
      //Remove o offset da leitura simétrica (Vref/2)
        Vo_signed[fase] =  Vo_unsigned[fase] - Offset_Vo;
        Vo_signed[fase] =  Vo_unsigned[fase] - Offset_Vo;
        Vg_signed[fase] =  Vg_unsigned[fase] - Offset_Vg;
        Vg_signed[fase] =  Vg_unsigned[fase] - Offset_Vg;

        //Aplica o ganho para obtenção do valor no SI
        #ifndef CONGELAR_MEDICAO_VO_Vg
        Vo_float[fase] = (float)Vo_signed[fase] * Ganho_Vo;

        Vg_float[fase] = (float)Vg_signed[fase] * Ganho_Vg;

        #endif

        //Calibração
        Vo[fase] = (Vo_float[fase] + Vo_offset[fase]) * Vo_ganho[fase];
        Vg[fase] = (Vg_float[fase] + Vg_offset[fase]) * Vg_ganho[fase];

    }

    // Se necessário calcular a terceira fase da tensão

}

//Instrumentação da tensão do barramento CC
INLINE void Instrumentacao_Vdc()
{
    unsigned i;
    static float *Ptr_buf = var_teste1;

    Vdc_signed[0] = Filter4_Result_SD2[0] + Offset_vdc_p;                    // Vp lado positivo
    Vdc_signed[1] = Filter3_Result_SD1[0] + Offset_vdc_n;                    // Vn lado negativo

    //Obtém o valor instantâneo das tensões individuais
        for (i=0; i<2; i++) {
            Vdc_float[i] = (float)Vdc_signed[i] * Ganho_Vdc; //Aplica o ganho para obtenção do valor no SI

            //Calibração
           #ifndef CONGELAR_MEDICAO_VDC
           Vdc[i] = (Vdc_float[i] + Vdc_offset[i]) * Vdc_ganho[i]; //Valor calibrado
           #endif
        }

        //Tensão total do barramento
         Vdc_tot = Vdc[0] + Vdc[1];

         //Diferença entre as tensões (lado positivo menos o lado negativo)
         Vdc_dif = Vdc[0] - Vdc[1];

         //Desbalanço de tensão em módulo
         Vdc_des = fabsf(Vdc_dif);    // abs para float com single precision

         // Proteção sobre tensão
         if( (Vdc_tot > VDC_MAX) || (Vdc_des > VDC_DES )){
             ForceTrip();
             TrocarEstado(_Protecao);
         }

         *Ptr_buf++ = Vdc[0];

              if( Ptr_buf == (var_teste1 + 50) )
                {
                Ptr_buf = var_teste1;           // Rewind the pointer to beginning
                }

}

INLINE void Instrumentacao_Io()
{
    unsigned fase;
    static float *Ptr_buf1 = var_teste2;

    Io_signed[0] = Filter2_Result_SD2[0]- Offset_Io_a;
    Io_signed[1] = Filter3_Result_SD2[0]- Offset_Io_b;


    for (fase=0; fase<2; fase++) {
    //Aplica o ganho para obtenção do valor no SI
    #ifndef CONGELAR_MEDICAO_IO
    Io_float[fase] = (float)Io_signed[fase] * Ganho_Io;
    #endif

    //Calibração
     Io[fase] = (Io_float[fase] + Io_offset[fase]) * Io_ganho[0];

    }

    //Calcula a terceira corrente com as duas primeiras e sobrescreve
    Io[2] = -(Io[0] + Io[1]);

    //Proteção sobre a terceira corrente
    if( Io[2] > Current_prot){
                 ForceTrip();
                 TrocarEstado(_Protecao);
             }

    *Ptr_buf1++ = Io[0];

    if( Ptr_buf1 == (var_teste2 + 50) )
        {
        Ptr_buf1 = var_teste2;           // Rewind the pointer to beginning
        }

}

