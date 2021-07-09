/*************************************************************************
                                INEP-UFSC
__________________________________________________________________________

ARQUIVO: TIMER.c
TÍTULO : Configurações do temporizador de CPU

__________________________________________________________________________

DATA: 29/01/2020
ATUALIZAÇÃO: xx/xx/xxxx
VERSÃO:1.0
__________________________________________________________________________
DESENVOLVIMENTO: ANDRÉ LANGE/ Esio

__________________________________________________________________________
OBSERVAÇÕES:
*************************************/

#include <math.h>
#include "F28x_Project.h"
#include "F2837xS_user_device.h"
#include "definicoes.h"             // Definições gerais de operação
#include "global.h"

//-----------------------------------------------
// Timer0: Base de tempo da rotina principal
//-----------------------------------------------
void ConfigCpuTimer0() {
    CpuTimer0Regs.PRD.all = 200000-1; //Base de tempo para 1kHz - CPU 200 MHz
    CpuTimer0Regs.TPR.all = 0;  //Prescale
    CpuTimer0Regs.TCR.bit.FREE = 0; //Ação em emulação: parado
    CpuTimer0Regs.TCR.bit.SOFT = 0;
    CpuTimer0Regs.TCR.bit.TIF = 1; //Limpa flag de estouro
    CpuTimer0Regs.TCR.bit.TSS = 0; //Habilita o timer
}

//-----------------------------------------------
// Timer1: Base de tempo do controle
//-----------------------------------------------
void ConfigCpuTimer1() {
    CpuTimer1Regs.PRD.all = round(200e6 / f_ctrl); //Base de tempo para 5kHz
    CpuTimer1Regs.TPR.all = 0;  //Prescale
    CpuTimer1Regs.TCR.bit.FREE = 0; //Ação em emulação: parado
    CpuTimer1Regs.TCR.bit.SOFT = 0;
    CpuTimer1Regs.TCR.bit.TIF = 1; //Limpa flag de estouro
    CpuTimer1Regs.TCR.bit.TIE = 1; //Habilita a interrupção
    CpuTimer1Regs.TCR.bit.TSS = 0; //Habilita o timer
}
