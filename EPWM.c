/* ###########################################################################
 *
 * Arquivo: EPWM.c
 *
 * Funções de inicialização, configuração e suporte dos módulos EPwm
 *
 * Esse arquivo não ajusta os pinos de entrada e saída, esses precisam
 * ser ajustados no arquivo: F2837xS_Gpio.c
 * Ajustes necessários:
 *          Configurar o MUX para a porta desejada
 *          Configurar o GMUX para essa porta
 *          Desativar os Pull-ups internos
 *          Configurar o pino como assincrono, quando for o caso
 *
 *
 * ###########################################################################
 * Versão 0.1a
 * Autor: César Daltoé Berci
 * Setembro de 2016
 * cesar@berci.com.br
 * ###########################################################################
 */


//
// Included Files
//

#include "global.h"
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"
#include "definicoes.h"


/*
 * Definições
 */

#define EPWM_CMP_UP           1
#define EPWM_CMP_DOWN         0

uint16_t CMPC11,CMPD11;
uint16_t CMPC12,CMPD12;

//S1A,S3A
void InitEPwm7()
{
    //
    // Setup TBCLK
    //
    EPwm7Regs.TBCTL.bit.PRDLD = 0; //Shadow period load
    EPwm7Regs.TBPRD = PERIODO_INICIAL;         // Set timer period
    EPwm7Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0
    EPwm7Regs.TBCTR = 0x0000;                    // Clear counter

    /*
     * Comportamento em eventos de emulação
     */

    #ifdef POTENCIA
    EPwm7Regs.TBCTL.bit.FREE_SOFT = 0;           //Stop after the next time-base counter increment or decrement
    #else
    EPwm7Regs.TBCTL.bit.FREE_SOFT = 0b10;         //Free run.
    #endif

    //
    // Setup counter mode
    //
    EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm7Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm7Regs.TBCTL.bit.CLKDIV = 0b000;

    //
    // Setup shadowing
    //
    EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set Compare values
    //
    EPwm7Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;    // Set compare A value
    EPwm7Regs.CMPB.bit.CMPB = 0;    // Set Compare B value

    //
    // Set actions
    //
    EPwm7Regs.AQCTLA.bit.CAU = AQ_SET;     // Set PWMxA on event A, up count
    EPwm7Regs.AQCTLA.bit.CBU = AQ_CLEAR;   // Clear PWMxA on event B, up

    /*
     * Controle de banda morta
     * Registradores: DBRED - Dead-Band Generator Rising Edge Delay
     *                  High Resolution Mirror Register, Página 1587
     *                DBCTL - Dead-Band Generator Control Register, Página 1554
     */
    EPwm7Regs.DBRED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de subida
    EPwm7Regs.DBFED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de descida
    EPwm7Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;    //Active Hi complementary
    EPwm7Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //enable Dead-band module

    //
    // Interrupt where we will change the Compare Values
    //
    EPwm7Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    // Select INT on Zero event
    EPwm7Regs.ETSEL.bit.INTEN = 1;               // Enable INT
    EPwm7Regs.ETPS.bit.INTPRD = ET_1ST;          // Generate INT on 1st event

    /*
     * Configuração da zona de trip para proteção das saídas
     */
    EALLOW;

    //Gerar trip ao interromper o programa no debugger
    #ifdef POTENCIA
    EPwm7Regs.TZSEL.bit.OSHT6 = 1;             // Enable /TZ6 (Emulation Stop) as a one-shot trip source for this ePWM module
    #endif


    //Trip Zone Control Register, Ações para os eventos A e B
    EPwm7Regs.TZCTL.bit.TZA = 0B10;               // 00->alta impedância, 01->força para cima,
    EPwm7Regs.TZCTL.bit.TZB = 0B10;              // 10->força para baixo 11-> não faz nada

    //Trip zone enable interrupt register
    EPwm7Regs.TZEINT.bit.OST = 0;                // 1 habilita a interrupção do evento, 0 desabilita
    EPwm7Regs.TZFRC.all = 0b100;                // Força evento one-shot via software para inciar com os sinais de comando desabilitados pelo trip

    // Trip para proteção contra picos de correntes
    EPwm7Regs.TZSEL.bit.DCAEVT1 = 1;                // enable DCAEVT as  trip source for pwm
    EPwm7Regs.TZDCSEL.bit.DCAEVT1 = 0b010;         // configure DCAevt1 event:  DCAH High and DCAL don't care

    EPwm7Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0b1111;         // configure Digital compare A High input as combination as DCAHTRIPSEL
    EPwm7Regs.DCACTL.bit.EVT1SRCSEL = 0;                  // DCAEVT source signal: DCAEVT1 signal
    EPwm7Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1;              // DCAEVT force synch: Source is passed through asynchronously

    EPwm7Regs.DCAHTRIPSEL.bit.TRIPINPUT4 = 1; // trip4 as input mux
    EPwm7Regs.DCAHTRIPSEL.bit.TRIPINPUT5 = 1; // trip 5 as input mux

    DELAY_US(1);

    //Reconfigura MUX para periférico ePWM
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;

    EDIS;
}

//S4A,S2A
void InitEPwm8()
{
    //
    // Setup TBCLK
    //
    EPwm8Regs.TBCTL.bit.PRDLD = 0; //Shadow period load
    EPwm8Regs.TBPRD = PERIODO_INICIAL;         // Set timer period
    EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0
    EPwm8Regs.TBCTR = 0x0000;                    // Clear counter

    /*
     * Comportamento em eventos de emulação
     */

    #ifdef POTENCIA
    EPwm8Regs.TBCTL.bit.FREE_SOFT = 0;         //Stop after the next time-base counter increment or decrement
    #else
    EPwm8Regs.TBCTL.bit.FREE_SOFT = 0b10;         //Free run.
    #endif

    //
    // Setup counter mode
    //
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm8Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm8Regs.TBCTL.bit.CLKDIV = 0b000;

    //
    // Setup shadowing
    //
    EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Configura o link do período com o EPwm7
    EPwm8Regs.EPWMXLINK.bit.TBPRDLINK = 0b0110;

    //
    // Set Compare values
    //
    EPwm8Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;    // Set compare A value
    EPwm8Regs.CMPB.bit.CMPB = 0;    // Set Compare B value

    //
    // Set actions
    //
    EPwm8Regs.AQCTLA.bit.CAU = AQ_SET;     // Set PWMxA on event A, up count
    EPwm8Regs.AQCTLA.bit.CBU = AQ_CLEAR;   // Clear PWMxA on event B, up

    /*
     * Controle de banda morta
     * Registradores: DBRED - Dead-Band Generator Rising Edge Delay
     *                  High Resolution Mirror Register, Página 1587
     *                DBCTL - Dead-Band Generator Control Register, Página 1554
     */
    EPwm8Regs.DBRED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de subida
    EPwm8Regs.DBFED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de descida
    EPwm8Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; //Active Hi complementary
    EPwm8Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //enable Dead-band module

    /*
     * Configuração da zona de trip para proteção das saídas
     */
    EALLOW;

    //Gerar trip ao interromper o programa no debugger
    #ifdef POTENCIA
    EPwm8Regs.TZSEL.bit.OSHT6 = 1;             // Enable /TZ6 (Emulation Stop) as a one-shot trip source for this ePWM module
    #endif

    //Trip Zone Control Register, Ações para os eventos A e B
    EPwm8Regs.TZCTL.bit.TZA = 0b10;             //00->alta impedância, 01->força para cima,
    EPwm8Regs.TZCTL.bit.TZB = 0B10;             //10->força para baixo 11-> não faz nada
    //Trip zone enable interrupt register
    EPwm8Regs.TZEINT.bit.OST = 0;               //1 habilita a interrupção do evento, 0 desabilita
    EPwm8Regs.TZFRC.all = 0b100;               // Força evento one-shot via software para inciar com os sinais de comando desabilitados pelo trip

    // Trip para proteção contra picos de correntes
    EPwm8Regs.TZSEL.bit.DCAEVT1 = 1;                // enable DCAEVT as  trip source for pwm
    EPwm8Regs.TZDCSEL.bit.DCAEVT1 = 0b010;         // configure DCAevt1 event:  DCAH High and DCAL don't care

    EPwm8Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0b1111;         // configure Digital compare A High input as combination as DCAHTRIPSEL
    EPwm8Regs.DCACTL.bit.EVT1SRCSEL = 0;                  // DCAEVT source signal: DCAEVT1 signal
    EPwm8Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1;              // DCAEVT force synch: Source is passed through asynchronously

    EPwm8Regs.DCAHTRIPSEL.bit.TRIPINPUT4 = 1; // trip4 as input mux
    EPwm8Regs.DCAHTRIPSEL.bit.TRIPINPUT5 = 1; // trip 5 as input mux


    DELAY_US(1);

    //Reconfigura MUX para periférico ePWM
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;

    EDIS;
}

//S1B,S3B
void InitEPwm10()
{
    //
    // Setup TBCLK
    //
    EPwm10Regs.TBCTL.bit.PRDLD = 0; //Shadow period load
    EPwm10Regs.TBPRD = PERIODO_INICIAL;         // Set timer period
    EPwm10Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0
    EPwm10Regs.TBCTR = 0x0000;                    // Clear counter

    /*
     * Comportamento em eventos de emulação
     */

    #ifdef POTENCIA
    EPwm10Regs.TBCTL.bit.FREE_SOFT = 0;         //Stop after the next time-base counter increment or decrement
    #else
    EPwm10Regs.TBCTL.bit.FREE_SOFT = 0b10;         //Free run.
    #endif

    //
    // Setup counter mode
    //
    EPwm10Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm10Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm10Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm10Regs.TBCTL.bit.CLKDIV = 0b000;

    //
    // Setup shadowing
    //
    EPwm10Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm10Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Configura o link do período com o EPwm7
    EPwm10Regs.EPWMXLINK.bit.TBPRDLINK = 0b0110;


    //
    // Set Compare values
    //
    EPwm10Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;    // Set compare A value
    EPwm10Regs.CMPB.bit.CMPB = 0;    // Set Compare B value

    //
    // Set actions
    //
    EPwm10Regs.AQCTLA.bit.CAU = AQ_SET;     // Set PWMxA on event A, up count
    EPwm10Regs.AQCTLA.bit.CBU = AQ_CLEAR;   // Clear PWMxA on event B, up

    /*
     * Controle de banda morta
     * Registradores: DBRED - Dead-Band Generator Rising Edge Delay
     *                High Resolution Mirror Register, Página 1587
     *                DBCTL - Dead-Band Generator Control Register, Página 1554
     */
    EPwm10Regs.DBRED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de subida
    EPwm10Regs.DBFED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de descida
    EPwm10Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; //Active Hi complementary
    EPwm10Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //enable Dead-band module

    /*
     * Configuração da zona de trip para proteção das saídas
     */
    EALLOW;

    //Gerar trip ao interromper o programa no debugger
    #ifdef POTENCIA
    EPwm10Regs.TZSEL.bit.OSHT6 = 1;             // Enable /TZ6 (Emulation Stop) as a one-shot trip source for this ePWM module
    #endif

    //Trip Zone Control Register, Ações para os eventos A e B
    EPwm10Regs.TZCTL.bit.TZA = 0b10;             //00->alta impedância, 01->força para cima,
    EPwm10Regs.TZCTL.bit.TZB = 0B10;             //10->força para baixo 11-> não faz nada
    //Trip zone enable interrupt register
    EPwm10Regs.TZEINT.bit.OST = 0;               //1 habilita a interrupção do evento, 0 desabilita
    EPwm10Regs.TZFRC.all = 0b100;               // Força evento one-shot via software para inciar com os sinais de comando desabilitados pelo trip


    // Trip para proteção contra picos de correntes
    EPwm10Regs.TZSEL.bit.DCAEVT1 = 1;                // enable DCAEVT as  trip source for pwm
    EPwm10Regs.TZDCSEL.bit.DCAEVT1 = 0b010;         // configure DCAevt1 event:  DCAH High and DCAL don't care

    EPwm10Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0b1111;         // configure Digital compare A High input as combination as DCAHTRIPSEL
    EPwm10Regs.DCACTL.bit.EVT1SRCSEL = 0;                  // DCAEVT source signal: DCAEVT1 signal
    EPwm10Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1;              // DCAEVT force synch: Source is passed through asynchronously

    EPwm10Regs.DCAHTRIPSEL.bit.TRIPINPUT4 = 1; // trip4 as input mux
    EPwm10Regs.DCAHTRIPSEL.bit.TRIPINPUT5 = 1; // trip 5 as input mux



    DELAY_US(1);

    //Reconfigura MUX para periférico ePWM
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;

    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;

    EDIS;
}


//S4B,S2B
void InitEPwm2()
{

//
    // Setup TBCLK
    //
    EPwm2Regs.TBCTL.bit.PRDLD = 0; //Shadow period load
    EPwm2Regs.TBPRD = PERIODO_INICIAL;         // Set timer period
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                    // Clear counter

    /*
     * Comportamento em eventos de emulação
     */

    #ifdef POTENCIA
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 0;         //Stop after the next time-base counter increment or decrement
    #else
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 0b10;         //Free run.
    #endif

    //
    // Setup counter mode
    //
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = 0b000;

    //
    // Setup shadowing
    //
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Configura o link do período com o EPwm7
    EPwm2Regs.EPWMXLINK.bit.TBPRDLINK = 0b0110;

    //
    // Set Compare values
    //
    EPwm2Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;    // Set compare A value
    EPwm2Regs.CMPB.bit.CMPB = 0;    // Set Compare B value

    //
    // Set actions
    //
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;     // Set PWMxA on event A, up count
    EPwm2Regs.AQCTLA.bit.CBU = AQ_CLEAR;   // Clear PWMxA on event B, up

    /*
     * Controle de banda morta
     * Registradores: DBRED - Dead-Band Generator Rising Edge Delay
     *                  High Resolution Mirror Register, Página 1587
     *                DBCTL - Dead-Band Generator Control Register, Página 1554
     */
    EPwm2Regs.DBRED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de subida
    EPwm2Regs.DBFED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de descida
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; //Active Hi complementary
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //enable Dead-band module


    /*
     * Configuração da zona de trip para proteção das saídas
     */
    EALLOW;

    //Gerar trip ao interromper o programa no debugger
    #ifdef POTENCIA
    EPwm2Regs.TZSEL.bit.OSHT6 = 1;             // Enable /TZ6 (Emulation Stop) as a one-shot trip source for this ePWM module
    #endif

    //Trip Zone Control Register, Ações para os eventos A e B
    EPwm2Regs.TZCTL.bit.TZA = 0b10;             //00->alta impedância, 01->força para cima,
    EPwm2Regs.TZCTL.bit.TZB = 0B10;             //10->força para baixo 11-> não faz nada
    //Trip zone enable interrupt register
    EPwm2Regs.TZEINT.bit.OST = 0;               //1 habilita a interrupção do evento, 0 desabilita
    EPwm2Regs.TZFRC.all = 0b100;               // Força evento one-shot via software para inciar com os sinais de comando desabilitados pelo trip

    // Trip para proteção contra picos de correntes
    EPwm2Regs.TZSEL.bit.DCAEVT1 = 1;                // enable DCAEVT as  trip source for pwm
    EPwm2Regs.TZDCSEL.bit.DCAEVT1 = 0b010;         // configure DCAevt1 event:  DCAH High and DCAL don't care

    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0b1111;         // configure Digital compare A High input as combination as DCAHTRIPSEL
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL = 0;                  // DCAEVT source signal: DCAEVT1 signal
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1;              // DCAEVT force synch: Source is passed through asynchronously

    EPwm2Regs.DCAHTRIPSEL.bit.TRIPINPUT4 = 1; // trip4 as input mux
    EPwm2Regs.DCAHTRIPSEL.bit.TRIPINPUT5 = 1; // trip 5 as input mux



    DELAY_US(1);

    //Reconfigura MUX para periférico ePWM
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;

    EDIS;
}

//S1C,S3C
void InitEPwm6()
{
    //
    // Setup TBCLK
    //

    EPwm6Regs.TBCTL.bit.PRDLD = 0; //Shadow period load
    EPwm6Regs.TBPRD = PERIODO_INICIAL;         // Set timer period
    EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0
    EPwm6Regs.TBCTR = 0x0000;                    // Clear counter

    /*
     * Comportamento em eventos de emulação
     */

    #ifdef POTENCIA
    EPwm6Regs.TBCTL.bit.FREE_SOFT = 0;         //Stop after the next time-base counter increment or decrement
    #else
    EPwm6Regs.TBCTL.bit.FREE_SOFT = 0b10;         //Free run.
    #endif

    //
    // Setup counter mode
    //
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm6Regs.TBCTL.bit.CLKDIV = 0b000;

    //
    // Setup shadowing
    //
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Configura o link do período com o EPwm7
    EPwm6Regs.EPWMXLINK.bit.TBPRDLINK = 0b0110;

    //
    // Set Compare values
    //
    EPwm6Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;    // Set compare A value
    EPwm6Regs.CMPB.bit.CMPB = 0;    // Set Compare B value

    //
    // Set actions
    //
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;     // Set PWMxA on event A, up count
    EPwm6Regs.AQCTLA.bit.CBU = AQ_CLEAR;   // Clear PWMxA on event B, up

    /*
     * Controle de banda morta
     * Registradores: DBRED - Dead-Band Generator Rising Edge Delay
     *                  High Resolution Mirror Register, Página 1587
     *                DBCTL - Dead-Band Generator Control Register, Página 1554
     */
    EPwm6Regs.DBRED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de subida
    EPwm6Regs.DBFED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de descida
    EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; //Active Hi complementary
    EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //enable Dead-band module

    /*
     * Configuração da zona de trip para proteção das saídas
     */
    EALLOW;

    //Gerar trip ao interromper o programa no debugger
    #ifdef POTENCIA
    EPwm6Regs.TZSEL.bit.OSHT6 = 1;             // Enable /TZ6 (Emulation Stop) as a one-shot trip source for this ePWM module
    #endif

    //Trip Zone Control Register, Ações para os eventos A e B
    EPwm6Regs.TZCTL.bit.TZA = 0b10;             //00->alta impedância, 01->força para cima,
    EPwm6Regs.TZCTL.bit.TZB = 0B10;             //10->força para baixo 11-> não faz nada
    //Trip zone enable interrupt register
    EPwm6Regs.TZEINT.bit.OST = 0;               //1 habilita a interrupção do evento, 0 desabilita
    EPwm6Regs.TZFRC.all = 0b100;               // Força evento one-shot via software para inciar com os sinais de comando desabilitados pelo trip

    // Trip para proteção contra picos de correntes
     EPwm6Regs.TZSEL.bit.DCAEVT1 = 1;                // enable DCAEVT as  trip source for pwm
     EPwm6Regs.TZDCSEL.bit.DCAEVT1 = 0b010;         // configure DCAevt1 event:  DCAH High and DCAL don't care

     EPwm6Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0b1111;         // configure Digital compare A High input as combination as DCAHTRIPSEL
     EPwm6Regs.DCACTL.bit.EVT1SRCSEL = 0;                  // DCAEVT source signal: DCAEVT1 signal
     EPwm6Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1;              // DCAEVT force synch: Source is passed through asynchronously

     EPwm6Regs.DCAHTRIPSEL.bit.TRIPINPUT4 = 1; // trip4 as input mux
     EPwm6Regs.DCAHTRIPSEL.bit.TRIPINPUT5 = 1; // trip 5 as input mux




    DELAY_US(1);

    //Reconfigura MUX para periférico ePWM
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;

    EDIS;
}


//S4C,S2C
void InitEPwm9()
{
    //
    // Setup TBCLK
    //
    EPwm9Regs.TBCTL.bit.PRDLD = 0; //Shadow period load
    EPwm9Regs.TBPRD = PERIODO_INICIAL;         // Set timer period
    EPwm9Regs.TBPHS.bit.TBPHS = 0x0000;          // Phase is 0
    EPwm9Regs.TBCTR = 0x0000;                    // Clear counter

    /*
     * Comportamento em eventos de emulação
     */

    #ifdef POTENCIA
    EPwm9Regs.TBCTL.bit.FREE_SOFT = 0;         //Stop after the next time-base counter increment or decrement
    #else
    EPwm9Regs.TBCTL.bit.FREE_SOFT = 0b10;         //Free run.
    #endif

    //
    // Setup counter mode
    //
    EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm9Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm9Regs.TBCTL.bit.CLKDIV = 0b000;

    //
    // Setup shadowing
    //
    EPwm9Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm9Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm9Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm9Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // Configura o link do período com o EPwm2
    EPwm9Regs.EPWMXLINK.bit.TBPRDLINK = 0b0110;


    //
    // Set Compare values
    //
    EPwm9Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;    // Set compare A value
    EPwm9Regs.CMPB.bit.CMPB = 0;    // Set Compare B value

    //
    // Set actions
    //
    EPwm9Regs.AQCTLA.bit.CAU = AQ_SET;     // Set PWMxA on event A, up count
    EPwm9Regs.AQCTLA.bit.CBU = AQ_CLEAR;   // Clear PWMxA on event B, up

    /*
     * Controle de banda morta
     * Registradores: DBRED - Dead-Band Generator Rising Edge Delay
     *                High Resolution Mirror Register, Página 1587
     *                DBCTL - Dead-Band Generator Control Register, Página 1554
     */
    EPwm9Regs.DBRED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de subida
    EPwm9Regs.DBFED.all = tempo_morto_ctr;       // Contador de 14 bits para a banda morta de descida
    EPwm9Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; //Active Hi complementary
    EPwm9Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //enable Dead-band module

    /*
     * Configuração da zona de trip para proteção das saídas
     */
    EALLOW;

    //Gerar trip ao interromper o programa no debugger
    #ifdef POTENCIA
    EPwm9Regs.TZSEL.bit.OSHT6 = 1;             // Enable /TZ6 (Emulation Stop) as a one-shot trip source for this ePWM module
    #endif

    //Trip Zone Control Register, Ações para os eventos A e B
    EPwm9Regs.TZCTL.bit.TZA = 0b10;             //00->alta impedância, 01->força para cima,
    EPwm9Regs.TZCTL.bit.TZB = 0B10;             //10->força para baixo 11-> não faz nada
    //Trip zone enable interrupt register
    EPwm9Regs.TZEINT.bit.OST = 0;               //1 habilita a interrupção do evento, 0 desabilita
    EPwm9Regs.TZFRC.all = 0b100;               // Força evento one-shot via software para inciar com os sinais de comando desabilitados pelo trip


    // Trip para proteção contra picos de correntes
     EPwm9Regs.TZSEL.bit.DCAEVT1 = 1;                // enable DCAEVT as  trip source for pwm
     EPwm9Regs.TZDCSEL.bit.DCAEVT1 = 0b010;         // configure DCAevt1 event:  DCAH High and DCAL don't care

     EPwm9Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0b1111;         // configure Digital compare A High input as combination as DCAHTRIPSEL
     EPwm9Regs.DCACTL.bit.EVT1SRCSEL = 0;                  // DCAEVT source signal: DCAEVT1 signal
     EPwm9Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1;              // DCAEVT force synch: Source is passed through asynchronously

     EPwm9Regs.DCAHTRIPSEL.bit.TRIPINPUT4 = 1; // trip4 as input mux
     EPwm9Regs.DCAHTRIPSEL.bit.TRIPINPUT5 = 1; // trip 5 as input mux



    DELAY_US(1);

    //Reconfigura MUX para periférico ePWM
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;

    EDIS;
}


void InitEPwm11() // Sincroniza o SDFM1
{


    CMPD11 =  1999; // Ajustar

    // Setup TBCLK
    //
    EPwm11Regs.TBPRD = 1999;                     // Set timer period
    EPwm11Regs.TBPHS.bit.TBPHS = 0x0000;         // Phase is 0
    EPwm11Regs.TBCTR = 0x0000;                   // Clear counter

    /*
     * Comportamento em eventos de emulação
     */



    EPwm11Regs.TBCTL.bit.FREE_SOFT = 0;         //Stop after the next time-base counter increment or decrement.

    //
    // Setup counter mode
    //
    EPwm11Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm11Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm11Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm11Regs.TBCTL.bit.CLKDIV = 0b000;


    //
    // Set Compare values
    //
    EPwm11Regs.CMPC = CMPD11;
    EPwm11Regs.CMPD = CMPD11;


}

void InitEPwm12() // Sincroniza o SDFM2 - sincronizar com timer1 - ADC
{

    CMPC12 = 1999; // Ajustar
    CMPD12 =  1999;

    // Setup TBCLK
    //

    EPwm12Regs.TBPRD = 1999 ;                    // Set timer period para Ajustar fctr = 5000 Hz Ajustar ***
    EPwm12Regs.TBPHS.bit.TBPHS = 0x0000;         // Phase is 0
    EPwm12Regs.TBCTR = 0x0000;                   // Clear counter

    /*
     * Comportamento em eventos de emulação
     */


   EPwm12Regs.TBCTL.bit.FREE_SOFT = 0;         //Stop after the next time-base counter increment or decrement


    //
    // Setup counter mode
    //
    EPwm12Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;     // Count up
    EPwm12Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = 0b101;       // Clock ratio to SYSCLKOUT
    EPwm12Regs.TBCTL.bit.CLKDIV = 0b000;


    // Config SOC dos ADs - synch pelo EPWM12
    EPwm12Regs.ETPS.bit.SOCAPRD= 0b01;    // generate SOCA on first event
    EPwm12Regs.ETSEL.bit.SOCAEN = 1;       // enable SOCA
    EPwm12Regs.ETSEL.bit.SOCASEL = 0b010;  //SOCA on PRD event


    //
    // Set Compare values
    //

    EPwm12Regs.CMPC = CMPC12 ;    // Set trigger to SDF2
    EPwm12Regs.CMPD = CMPD12 ;

}

void PWM_Sync()
{

    EPwm7Regs.TBCTR = 0;
    EPwm8Regs.TBCTR = 0;

    EPwm10Regs.TBCTR = 0;
    EPwm2Regs.TBCTR = 0;

    EPwm6Regs.TBCTR = 0;
    EPwm9Regs.TBCTR = 0;

    EPwm11Regs.TBCTR = 0;
    EPwm12Regs.TBCTR = 0;

    //Libera o clock de todos os módulos
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;  // Libera o clock de todos os módulos
    EDIS;

    //Aguarda pelo menos um ciclo completo da portadora do PFC para prosseguir
    EPwm7Regs.ETCLR.bit.INT = 1;
    while (!EPwm7Regs.ETFLG.bit.INT);
    EPwm7Regs.ETCLR.bit.INT = 1;
    while (!EPwm7Regs.ETFLG.bit.INT);
}

//Libera o funcionamento do modulador PWM
void PWM_Run() {
    //Inicializa o período
    EPwm7Regs.TBPRD = PERIODO_INICIAL;

    //Inicializa os valores de comparação
    EPwm7Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;
    EPwm7Regs.CMPB.bit.CMPB = 0;
    EPwm8Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;
    EPwm8Regs.CMPB.bit.CMPB = 0;

    EPwm10Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;
    EPwm10Regs.CMPB.bit.CMPB = 0;
    EPwm2Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;
    EPwm2Regs.CMPB.bit.CMPB = 0;

    EPwm6Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;
    EPwm6Regs.CMPB.bit.CMPB = 0;
    EPwm9Regs.CMPA.bit.CMPA = PERIODO_INICIAL+1;
    EPwm9Regs.CMPB.bit.CMPB = 0;

    EPwm11Regs.CMPC = CMPD11;    // Set trigger to SDF1
    EPwm11Regs.CMPD = CMPD11;
    EPwm12Regs.CMPC = CMPC12;    // Set trigger to SDF2
    EPwm12Regs.CMPD = CMPD12;

    //Aguarda um ciclo completo da portadora para prosseguir
    EPwm7Regs.ETCLR.bit.INT = 1;
    while (!EPwm7Regs.ETFLG.bit.INT);
    EPwm7Regs.ETCLR.bit.INT = 1;
    while (!EPwm7Regs.ETFLG.bit.INT);

    //Limpa os flags do trip-zone do PFC
    #ifdef HABILITAR_COMANDO
    DINT;
    EALLOW;
    EPwm7Regs.TZCLR.all = 0b111;
    EPwm8Regs.TZCLR.all = 0b111;
    EPwm10Regs.TZCLR.all = 0b111;
    EPwm2Regs.TZCLR.all = 0b111;
    EPwm6Regs.TZCLR.all = 0b111;
    EPwm9Regs.TZCLR.all = 0b111;
    EDIS;
    EINT;
    #endif
}

//Força o trip-zone em todos os EPWMs
void ForceTrip() {
    //Força evento One-shot imediatamente nos EPWMs
    DINT;
    EALLOW;
    EPwm7Regs.TZFRC.all = 0b100;
    EPwm8Regs.TZFRC.all = 0b100;
    EPwm10Regs.TZFRC.all = 0b100;
    EPwm2Regs.TZFRC.all = 0b100;
    EPwm6Regs.TZFRC.all = 0b100;
    EPwm9Regs.TZFRC.all = 0b100;
    EDIS;
    EINT;
}


void Interrupt_trip_enable()
{

    EALLOW;

    EPwm7Regs.TZEINT.bit.OST = 1;

    EDIS;
}


void Clr_Flag_DCAEVT()
{

    EALLOW;

    EPwm7Regs.TZCLR.bit.DCAEVT1 = 1;              // limpa flags
    EPwm7Regs.TZOSTCLR.bit.DCAEVT1 = 1;            // limpa flags

    EPwm8Regs.TZCLR.bit.DCAEVT1 = 1;              // limpa flags
    EPwm8Regs.TZOSTCLR.bit.DCAEVT1 = 1;            // limpa flags

    EDIS;
}
