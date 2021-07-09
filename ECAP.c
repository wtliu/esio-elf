/*
 * ECap.c
 *
 *  Created on: 30 de dez de 2020
 *      Author: Esio Eloi
 */

/*************************************************************************
                                INEP-UFSC
__________________________________________________________________________

ARQUIVO:ECAP.c
TÍTULO :Configurações do modulo Ecap

__________________________________________________________________________

DATA: 22/12/2021
ATUALIZAÇÃO: xx/xx/xxxx
VERSÃO:1.0
__________________________________________________________________________
DESENVOLVIMENTO: Esio ELoi

***************************************************************************/



#include "global.h"
#include "F2837xS_Cla_typedefs.h"        // CLA type definitions
#include "F2837xS_device.h"              // F2837xD header file peripheral address definitions
#include "F2837xS_Adc_defines.h"         // ADC definitions
#include "F2837xS_defaultisr.h"          // ISR definitions
#include "F2837xS_Pie_defines.h"         // PIE definitions
#include "F2837xS_Examples.h"
#include "F2837xS_struct.h"


extern void Ecap1()
{

//---------------------------------------------------------------------
//--- Configure eCAP1 unit for capture
//---------------------------------------------------------------------
    asm(" EALLOW");                         // Enable EALLOW protected register access
    DevCfgRegs.SOFTPRES3.bit.ECAP1 = 1;     // eCAP1 is reset
    DevCfgRegs.SOFTPRES3.bit.ECAP1 = 0;     // eCAP1 is released from reset
    asm(" EDIS");

   ECap1Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
   ECap1Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
   ECap1Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable capture register loads
   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped/ stop the count

   ECap1Regs.TSCTR = 0;                        // Clear the counter
   ECap1Regs.CTRPHS = 0;                       // Clear the counter phase register


   //
   // Configure peripheral registers
   //
   ECap1Regs.ECCTL2.bit.CAP_APWM = 0;      // Capture mode
   ECap1Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
   ECap1Regs.ECCTL2.bit.STOP_WRAP = 3;     // Stop at 4 events
   ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;      // disable sync in
   ECap1Regs.ECCTL2.bit.SYNCO_SEL = 2;     // disabel  sync out

   ECap1Regs.ECCTL1.bit.CAP1POL = 0;       // Rising edge
   ECap1Regs.ECCTL1.bit.CAP2POL = 1;       // Fallung edge
   ECap1Regs.ECCTL1.bit.CAP3POL = 0;       // Rising edge
   ECap1Regs.ECCTL1.bit.CAP4POL = 1;       // Falling edge
   ECap1Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
   ECap1Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
   ECap1Regs.ECCTL1.bit.CTRRST3 = 1;       // Difference operation
   ECap1Regs.ECCTL1.bit.CTRRST4 = 1;       // Difference operation

   ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

   ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
   ECap1Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
   ECap1Regs.ECEINT.bit.CEVT4 = 1;         // 4 events = __interrupt

}

extern void Ecap2()
{

//---------------------------------------------------------------------
//--- Configure eCAP2 unit for capture
//---------------------------------------------------------------------
    asm(" EALLOW");                         // Enable EALLOW protected register access
    DevCfgRegs.SOFTPRES3.bit.ECAP2 = 1;     // eCAP1 is reset
    DevCfgRegs.SOFTPRES3.bit.ECAP2 = 0;     // eCAP1 is released from reset
    asm(" EDIS");

   ECap2Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
   ECap2Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
   ECap2Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable capture register loads
   ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped/ stop the count

   ECap2Regs.TSCTR = 0;                        // Clear the counter
   ECap2Regs.CTRPHS = 0;                       // Clear the counter phase register


   //
   // Configure peripheral registers
   //
   ECap2Regs.ECCTL2.bit.CAP_APWM = 0;      // Capture mode
   ECap2Regs.ECCTL2.bit.CONT_ONESHT = 1;   // One-shot
   ECap2Regs.ECCTL2.bit.STOP_WRAP = 3;     // Stop at 4 events
   ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;      // disable sync in
   ECap2Regs.ECCTL2.bit.SYNCO_SEL = 2;     // disabel  sync out

   ECap2Regs.ECCTL1.bit.CAP1POL = 0;       // Rising edge
   ECap2Regs.ECCTL1.bit.CAP2POL = 1;       // Fallung edge
   ECap2Regs.ECCTL1.bit.CAP3POL = 0;       // Rising edge
   ECap2Regs.ECCTL1.bit.CAP4POL = 1;       // Falling edge
   ECap2Regs.ECCTL1.bit.CTRRST1 = 1;       // Difference operation
   ECap2Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation
   ECap2Regs.ECCTL1.bit.CTRRST3 = 1;       // Difference operation
   ECap2Regs.ECCTL1.bit.CTRRST4 = 1;       // Difference operation

   ECap2Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units

   ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
   ECap2Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
   ECap2Regs.ECEINT.bit.CEVT4 = 1;         // 4 events = __interrupt

}

// Realizar o re arm para realizar outra captura

