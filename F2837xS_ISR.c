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
*Observa��o: As interrup��es utilizadas aqui devem estar desativadas/comentadas
* no arquivo DeafultISR.c para n�o haver conflitos.
*
**********************************************************************/



// Included Files

#include "global.h"
#include "F2837xS_sdfm_drivers.h"       // SDFM definitions
#include "F2837xS_struct.h"
#include "F2837xS_sdfm.h"



//Definir para usar declara��es inline
//#define USE_INLINE

#ifdef USE_INLINE
    #define INLINE inline
#else
    #define INLINE
#endif


// Estrutura para armazenar os CMPA/B, que s�o os sinais moduladores

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

// ******* Prot�tipos de fun��es - Utilizadas nas interrup��es *******

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

//********** INTERRUP��ES ************//



//Interrup��o lenta de modula��o

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
        //Executa a modula��o necess�ria
        ModulacaoVetorial();
       }


    EPwm7Regs.ETCLR.bit.INT = 1;
    // Acknowledge this interrupt to receive more interrupts from group 3
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}


//Interrup��o lenta para controle
interrupt void ADCA2_ISR(void)
{

   sdfm_index_SD1_vn = 1;               // Quando entra na interrup��o de controle seta o index
   sdfm_index_SD2_vp = 1;
   sdfm_index_SD2_ia = 1;
   sdfm_index_SD2_ib = 1;

   static volatile int pareAqui2=0;

  //Habilita interrup��es simult�neas
    EINT;

  //Limpa flag de estouro do temporizador que disparou o ADC
    CpuTimer1Regs.TCR.bit.TIF = 1;

   //Rotinas de instrumenta��o
    Instrumentacao_Vdc(); // Leitura da tens�o do barramento CC atraves do SDFM
    Instrumentacao_Io(); //Correntes de sa�da
    Instrumentacao_Vo_inv_Vg(); //Tens�es de saida do inversor e da entrada da rede

    if (Vdc_des > 5.f){
        pareAqui2 = 1;
    }

   //Rampa
    if (estado >= _Operando) {
     //Rampa V/F de partida
        Rampa_VF();

    //Rotinas de modula��o
        Modulacoes();
       }

    DINT;

    //Modula��o vetorial
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

// Interrup��o do trip - prote��o de corrente
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

        // veio pelo sw_Reset ou outra prote��o
         asm(" NOP");

         EALLOW;
         EPwm7Regs.TZCLR.bit.OST=0; // Limpa os flags do trip-zone
         EDIS;
        //asm ("  ESTOP0"); // debug only to halt the processor here
    }
}


interrupt void XINT1_ISR(void){

    //Interrup��o do sinal de prote��o  do driver fase C - gpio41

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    TrocarEstado(_Protecao); // vai para o estado de prote��o e for�a o trip

}

interrupt void XINT2_ISR(void){

    //Interrup��o do sinal de prote��o  do driver fase b - gpio87

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    TrocarEstado(_Protecao); // vai para o estado de prote��o e for�a o trip

}

interrupt void XINT3_ISR(void){

    //Interrup��o do sinal de prote��o  do driver fase a - gpio78

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

    TrocarEstado(_Protecao); // vai para o estado de prote��o e for�a o trip

}



// ************************** Fun��es utilizadas **************************************** //

INLINE void ModulacaoVetorial()
{
       unsigned fase;


    //Registrador de per�odo
      Tsw2_mod = 0.5 * Tsw_mod; //Meio per�odo de comuta��o
      TBPRD2 = ((uint16_t)( f_clk*Tsw2_mod) + 1 ) >> 1; //Meio per�odo arredondando
      TBPRD = TBPRD2 << 1; //Per�odo inteiro

    //Passo do �ngulo
      theta_step = ((int16_t)(fo_mod*Tsw2_mod*Q12_60deg_360f) + 1) >> 1;

    //Avan�o do primeiro �ngulo - gera��o do angulo para formar a moduladora
        if (fo_mod >= f_min_vet_sinc) {

            //�ndice arredondado para o inteiro mais pr�ximo

            if (theta_step > 0) {
                n_theta = ((((uint16_t)thetafix<<1)/(uint16_t)theta_step) + 1) >> 1;
            }

            if (n_theta >= N_theta_mod) n_theta -= N_theta_mod;

            //Atualiza��o do primeiro �ngulo

            theta1fix = n_theta * theta_step;

            if (theta1fix < thetafix) theta1fix = thetafix; //N�o permite retroceder �ngulo
         }
        else
            theta1fix = thetafix;

     //Corre��o da fase devido ao atraso do modulador
        phi_mod = (theta_step + 1) >> 1; //Atraso do ZOH arredondado

        theta_mod1 = (theta1fix + phi_mod) * Q12_60deg_to_F;

     //Gera��o das moduladoras em alfa-beta
        m_albe[0] = M * __cospuf32(theta_mod1);
        m_albe[1] = M * __sinpuf32(theta_mod1);

     //Transforma��o para abc
        alfabeta_to_abc_amp(m_albe, m_abc1);

     //Gera��o da moduladora para modula��o vetorial 3 n�veis
        spacev_3L(m_abc1, mod1);

     //Avan�o do segundo �ngulo
           if (fo_mod >= f_min_vet_sinc) {

               //Avan�o s�ncrono
               if (++n_theta >= N_theta_mod) n_theta -= N_theta_mod;

               theta2fix = n_theta * theta_step;
           }
           else {
               theta2fix = theta1fix + theta_step; //Avan�o ass�ncrono
               if (theta2fix >= Q12_60deg_360) theta2fix -= Q12_60deg_360;
           }

       //Corre��o da fase devido ao atraso do modulador
          theta_mod2 = (theta2fix + phi_mod) * Q12_60deg_to_F;

      //Gera��o das moduladoras em alfa-beta
        m_albe[0] = M * __cospuf32(theta_mod2);
        m_albe[1] = M * __sinpuf32(theta_mod2);

      //Transforma��o para abc
        alfabeta_to_abc_amp(m_albe, m_abc2);

      //Gera��o da moduladora para modula��o vetorial 3 n�veis
        spacev_3L(m_abc2, mod2);

      //Programa��o das moduladoras

      const TCMPRegs *CMPReg = TabCMPRegs;

      for (fase=0; fase<3; fase++) {
           programar_moduladoras(mod1[fase], mod2[fase], CMPReg->S1xCMPA, CMPReg->S1xCMPB, CMPReg->S4xCMPA, CMPReg->S4xCMPB);
           CMPReg++;
           }

      //Avan�a o �ngulo referente ao �ltimo ciclo de portadora
      thetafix = theta2fix + theta_step;
      if (thetafix >= Q12_60deg_360) thetafix -= Q12_60deg_360;


      //Per�odo da portadora
      EPwm7Regs.TBPRD = TBPRD;

}

//Programa os valores de compara��o do modulador PWM na modula��o vetorial
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

          if (CMPA <= TBPRD2) CMPA = TBPRD2+1; //Satura��o na metade

          CMPA1->bit.CMPA = (unsigned)CMPA; //  Quando S1 liga

          CMPB4->bit.CMPB = (unsigned)(TBPRD2+1); // Quando S4 desliga
       }
       else {
         //Semiciclo negativo
          CMPB = (long)((1.0-mod2)*TBPRD2);

           if (CMPB > TBPRD) CMPB = TBPRD; //Satura��o na metade

           CMPB4->bit.CMPB = (unsigned)CMPB; //Quando S4 desliga

           CMPA1->bit.CMPA = (unsigned)TBPRD; // Quando S1 liga
       }


}

//C�lcula a fun��o de modula��o com a tens�o de modo comum para modula��o vetorial de 3-n�veis
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

    //Primeiro c�lculo de m�ximos e m�nimos
       m_max1 = fmaxf(m[0], fmaxf(m[1], m[2]));
       m_min1 = fminf(m[0], fminf(m[1], m[2]));
       m_sum1 = -0.5*(m_max1 + m_min1);

    //Primeiro somat�rio
        for (fase=0; fase<3; fase++) {
            m_sum2[fase] = m[fase] + m_sum1;

             aux = m_sum2_mod + fase;  // aux recebe o endere�o de m_sum2_mod
            *aux = m_sum2[fase];       // m_sum2_mod recebe m_sum2
           //Fun��o mod
            if (*aux < 0.0) *aux += 1.0; // m_sum2_mod = m_sum2_mod + 1
            if (*aux > 1.0) *aux -= 1.0; // // m_sum2_mod = m_sum2_mod - 1
          }

     //Segundo c�lculo de m�ximos e m�nimos
      m_max2 = fmaxf(m_sum2_mod[0], fmaxf(m_sum2_mod[1], m_sum2_mod[2]));
      m_min2 = fminf(m_sum2_mod[0], fminf(m_sum2_mod[1], m_sum2_mod[2]));
      m_sum3 = 0.5 - 0.5*(m_max2 + m_min2);

     //�ltimo somat�rio
      for (fase=0; fase<3; fase++) mod[fase] = m_sum2[fase] + m_sum3;  // inje��o do modo comum formando a modula��o de tr�s n�veis
}

//Implementa��o da rampa V/F
INLINE void Rampa_VF() {

    //static float *Ptr_buf1 = var_teste1;

    //C�lculo da rampa V/F -  Vai de zero a A
    rampa_vf += rampa_inc*sentido;
    rampa_vf = fminf(fmaxf(rampa_vf, 0), 1.0);   //Limita entre 0.0 e 1.0


    //Frequ�ncia fundamental e �ndice de modula��o - Aplica-se o ganho G
    if (sentido != 0) {

        fo = fo_nom * rampa_vf;

        //Vboost = 0;
        Vinv = G*fo;
        Maux = (2*Vinv)/Vdc_tot;
        M = Mnom * rampa_vf;
    }


    //Per�odo da fundamental.
    To = 1/fo;


}


//Escolha da modula��o - por enquanto s� tem a vetorial
INLINE void Modulacoes() {

    ConfigurarModulacaoVetorial();

    //Per�odo de comuta��o
    Tsw = 1.0 / f_sw;
}

//Configura a modula��o vetorial
INLINE void ConfigurarModulacaoVetorial()
{
    //C�lculo da frequ�ncia de comutacao
    if (fo >= f_min_vet_sinc) {
        //Frequ�ncia m�ltipla de 3 vezes a fundamental

        uint16_t nf = (uint16_t)(ONE_THIRD * To * f_sw_vet); //Arredonda para baixo

        f_sw = 3*nf*fo; //Frequ�ncia de comuta��o do interruptor sincronizada com a fundamental

        N_theta = 12*nf; //N�mero de passos para o �ngulo theta
    }
    else {
        //Frequ�ncia de comuta��o ass�ncrona

        f_sw = f_sw_vet;

        N_theta = 0;
    }
}


//Instrumenta��o das tens�es de saida do inversor e da tens�o da rede lidas pelo ADC
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
      //Remove o offset da leitura sim�trica (Vref/2)
        Vo_signed[fase] =  Vo_unsigned[fase] - Offset_Vo;
        Vo_signed[fase] =  Vo_unsigned[fase] - Offset_Vo;
        Vg_signed[fase] =  Vg_unsigned[fase] - Offset_Vg;
        Vg_signed[fase] =  Vg_unsigned[fase] - Offset_Vg;

        //Aplica o ganho para obten��o do valor no SI
        #ifndef CONGELAR_MEDICAO_VO_Vg
        Vo_float[fase] = (float)Vo_signed[fase] * Ganho_Vo;

        Vg_float[fase] = (float)Vg_signed[fase] * Ganho_Vg;

        #endif

        //Calibra��o
        Vo[fase] = (Vo_float[fase] + Vo_offset[fase]) * Vo_ganho[fase];
        Vg[fase] = (Vg_float[fase] + Vg_offset[fase]) * Vg_ganho[fase];

    }

    // Se necess�rio calcular a terceira fase da tens�o

}

//Instrumenta��o da tens�o do barramento CC
INLINE void Instrumentacao_Vdc()
{
    unsigned i;
    static float *Ptr_buf = var_teste1;

    Vdc_signed[0] = Filter4_Result_SD2[0] + Offset_vdc_p;                    // Vp lado positivo
    Vdc_signed[1] = Filter3_Result_SD1[0] + Offset_vdc_n;                    // Vn lado negativo

    //Obt�m o valor instant�neo das tens�es individuais
        for (i=0; i<2; i++) {
            Vdc_float[i] = (float)Vdc_signed[i] * Ganho_Vdc; //Aplica o ganho para obten��o do valor no SI

            //Calibra��o
           #ifndef CONGELAR_MEDICAO_VDC
           Vdc[i] = (Vdc_float[i] + Vdc_offset[i]) * Vdc_ganho[i]; //Valor calibrado
           #endif
        }

        //Tens�o total do barramento
         Vdc_tot = Vdc[0] + Vdc[1];

         //Diferen�a entre as tens�es (lado positivo menos o lado negativo)
         Vdc_dif = Vdc[0] - Vdc[1];

         //Desbalan�o de tens�o em m�dulo
         Vdc_des = fabsf(Vdc_dif);    // abs para float com single precision

         // Prote��o sobre tens�o
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
    //Aplica o ganho para obten��o do valor no SI
    #ifndef CONGELAR_MEDICAO_IO
    Io_float[fase] = (float)Io_signed[fase] * Ganho_Io;
    #endif

    //Calibra��o
     Io[fase] = (Io_float[fase] + Io_offset[fase]) * Io_ganho[0];

    }

    //Calcula a terceira corrente com as duas primeiras e sobrescreve
    Io[2] = -(Io[0] + Io[1]);

    //Prote��o sobre a terceira corrente
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

