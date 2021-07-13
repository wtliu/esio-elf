/*
 * SFDM.c
 *
 *  Created on: 30 de dez de 2020
 *      Author: Esio Eloi
 */

/*************************************************************************
                                INEP-UFSC
__________________________________________________________________________

ARQUIVO:SFDM.c
TÍTULO :Configurações do Sigma-Delta filter module

__________________________________________________________________________

DATA: 22/12/2021
ATUALIZAÇÃO: xx/xx/xxxx
VERSÃO:1.0
__________________________________________________________________________
DESENVOLVIMENTO: Esio ELoi

__________________________________________________________________________
OBSERVAÇÕES:

In this example, SDFM filter data is read by CPU in SDFM ISR routine. The
//! SDFM configuration is shown below:
//!  -  SDFM used in this example - SDFM1 e SDFM2
//!  -  Input control mode selected - MODE0
//!  -  Comparator settings
//!       - Sinc3 filter selected
//!       - OSR = 32
//!       - HLT = 0x7FFF (Higher threshold setting)
//!       - LLT  = 0x0000(Lower threshold setting)
//!  -  Data filter settings
//!      - All the 4 filter modules enabled
//!      - Sinc3 filter selected
//!      - OSR = 256
//!      - All the 4 filters are synchronized by using MFE
//!       (Master Filter enable bit)
//!      - Filter output represented in 16 bit format
//!      - In order to convert 25 bit Data filter
//!        into 16 bit format user needs to right shift by 9 bits for
//!        Sinc3 filter with OSR = 256
//!  - Interrupt module settings for SDFM filter
//!      - All the 4 higher threshold comparator interrupts disabled
//!      - All the 4 lower threshold comparator interrupts disabled
//!      - All the 4 modulator failure interrupts disabled
//!      - All the 4 filter will generate interrupt when a new filter data
//!        is available.
//!
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 $
// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

1 - GPIOs - input qualification should be set to asynchronous mode, * desativar pull-up internos

Ia: SD2 - C2/D2; Ib: SD2 - C3/D3; VP: SD2 - C4/D4; VN: SD1 - C3/D3 - Configura GPIO.c

**************************************************************************/



#include <stdio.h>
#include "global.h"
#include "F2837xS_sdfm_drivers.h"       // SDFM definitions
#include "F2837xS_struct.h"


/*
 * Definições
 */

//#define MAX_SAMPLES               1024
#define SDFM_PIN_MUX_OPTION1      1
#define SDFM_PIN_MUX_OPTION2      2
#define SDFM_PIN_MUX_OPTION3      3

void done(void)
{
    asm(" ESTOP0");
    for (;;);
}

uint16_t  HLT, LLT;

void InitSdfm(){

//--- Reset the SDFM.  This is good programming practice.
    asm(" EALLOW");                     // Enable EALLOW protected register access
    DevCfgRegs.SOFTPRES6.bit.SD1 = 1;    // SD1 is reset
    DevCfgRegs.SOFTPRES6.bit.SD1 = 0;    // SD1 is released from reset
    DevCfgRegs.SOFTPRES6.bit.SD2 = 1;    // SD2 is reset
    DevCfgRegs.SOFTPRES6.bit.SD2 = 0;    // SD2 is released from reset
    asm(" EDIS");


 // Reconfigura MUX para periférico SDFM - GPIO como entrada do SDFM

     EALLOW;

       GpioCtrlRegs.GPAGMUX2.bit.GPIO20 = 1; // GPIO 20 e 21: SD1_D3 e SD1_C3
       GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;  // Vn SD1 Filter 3
       GpioCtrlRegs.GPAGMUX2.bit.GPIO21 = 1;
       GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;

       GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 1; // GPIO 58 e 59: SD2_D2 e SD2_C2
       GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 3;  // Ia  - SD2 Filter 2
       GpioCtrlRegs.GPBGMUX2.bit.GPIO59 = 1;
       GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 3;

       GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 1; // GPIO 60 e 61: SD2_D3 e SD2_C3
       GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 3; // Ib SD2 Filter 3
       GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 1;
       GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 3;

       GpioCtrlRegs.GPBGMUX2.bit.GPIO62 = 1; // GPIO 62 e 63: SD2_D4 e SD2_D4
       GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 3; // Vp SD2 Filter 4
       GpioCtrlRegs.GPBGMUX2.bit.GPIO63 = 1;
       GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;

      EDIS;

// Configuração dos registradores do SDFM
// Input Control Module
// Configure Input Control Mode: Modulator Clock rate = Modulator data rate

// Configure SDFM registers - SD1:ch3 e SD2:ch2,3,4

    Sdfm_configureInputCtrl(SDFM1, FILTER3, MODE_0);
    Sdfm_configureInputCtrl(SDFM2, FILTER2, MODE_0);
    Sdfm_configureInputCtrl(SDFM2, FILTER3, MODE_0);
    Sdfm_configureInputCtrl(SDFM2, FILTER4, MODE_0);

// Comparator Module
// Configure Comparator module's comparator filter type and comparator's OSR
// value, higher threshold, lower threshold


   HLT = 0x7FFF;    //Over value threshold settings - default: 32767 - 0x7FFF
   LLT = 0x0000;    //Under value threshold settings

   Sdfm_configureComparator(SDFM1, FILTER3, SINC3, OSR_32, HLT, LLT);   // SDFMx, FILTERx, SINCx, OSR_32, HLT, LLT)
   Sdfm_configureComparator(SDFM2, FILTER2, SINC3, OSR_32, Lim_sup_I, Lim_inf_I);
   Sdfm_configureComparator(SDFM2, FILTER3, SINC3, OSR_32, Lim_sup_I, Lim_inf_I);
   Sdfm_configureComparator(SDFM2, FILTER4, SINC3, OSR_32, HLT, LLT);

//
// Data filter Module
//
//Configure Data filter modules filter type, OSR value and
// enable / disable data filter
    Sdfm_configureData_filter(SDFM1, FILTER3, FILTER_ENABLE, SINC3, OSR_256, DATA_16_BIT, SHIFT_9_BITS);
    Sdfm_configureData_filter(SDFM2, FILTER2, FILTER_ENABLE, SINC3, OSR_256, DATA_16_BIT, SHIFT_9_BITS);
    Sdfm_configureData_filter(SDFM2, FILTER3, FILTER_ENABLE, SINC3, OSR_256, DATA_16_BIT, SHIFT_9_BITS);
    Sdfm_configureData_filter(SDFM2, FILTER4, FILTER_ENABLE, SINC3, OSR_256, DATA_16_BIT, SHIFT_9_BITS);

//
// Enable Master filter bit: Unless this bit is set none of the filter modules
// can be enabled. All the filter modules are synchronized when master filter
// bit is enabled after individual filter modules are enabled.
//

    DELAY_US(1000);

    Sdfm_enableMFE(SDFM1);
    Sdfm_enableMFE(SDFM2);

//
// PWM11.CMPC, PWM11.CMPD, PWM12.CMPC and PWM12.CMPD signals can synchronize
// the filters.
//
   Sdfm_configureExternalreset(SDFM1, FILTER_1_EXT_RESET_DISABLE, FILTER_2_EXT_RESET_DISABLE, FILTER_3_EXT_RESET_ENABLE, FILTER_4_EXT_RESET_DISABLE);
   Sdfm_configureExternalreset(SDFM2, FILTER_1_EXT_RESET_DISABLE, FILTER_2_EXT_RESET_ENABLE, FILTER_3_EXT_RESET_ENABLE, FILTER_4_EXT_RESET_ENABLE);


// Enable interrupts

// Following SDFM interrupts can be enabled / disabled using this function.
//  Enable / disable comparator high threshold
//  Enable / disable comparator low threshold
//  Enable / disable modulator clock failure
//  Enable / disable data filter acknowledge

//Provide sufficient settling time to make sure comparator is getting correct samples

    DELAY_US(1000);
    DELAY_US(1000);

    Sdfm_clearFlagRegister(SDFM1,0xFFFFFFFF);
    Sdfm_clearFlagRegister(SDFM2,0xFFFFFFFF);


    Sdfm_configureInterrupt(SDFM1, FILTER3, IEH_DISABLE, IEL_DISABLE, MFIE_ENABLE, AE_ENABLE);
    Sdfm_configureInterrupt(SDFM2, FILTER2, IEH_ENABLE, IEL_ENABLE, MFIE_ENABLE, AE_ENABLE);
    Sdfm_configureInterrupt(SDFM2, FILTER3, IEH_ENABLE, IEL_ENABLE, MFIE_ENABLE, AE_ENABLE);
    Sdfm_configureInterrupt(SDFM2, FILTER4, IEH_DISABLE, IEL_DISABLE, MFIE_ENABLE, AE_ENABLE);

// Enable master interrupt so that any of the filter interrupts can trigger
// by SDFM interrupt to CPU

//
//Provide sufficient settling time to make sure comparator is getting correct samples

    DELAY_US(1000);

    Clr_Flag_DCAEVT();      // limpa as flags do DCAEVT

    Sdfm_enableMIE(SDFM1);
    Sdfm_enableMIE(SDFM2);

    Sdfm_clearFlagRegister(SDFM1,0xFFFFFFFF);
    Sdfm_clearFlagRegister(SDFM2,0xFFFFFFFF);

} // end InitSDFM()

