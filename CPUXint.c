/*
 * CPUXint.c
 *
 *  Created on: 1 de jun de 2021
 *      Author: Esio
 */

#include "F28x_Project.h"
#include "F2837xS_user_device.h"
#include "definicoes.h"             // Definições gerais de operação
#include "global.h"


/* Configura e habilita a interrupção CPU.XINTx
 *
 * Monitora o estado das GPIO selecionadas e em caso de evento aciona uma interrupção
 * GPIOs recebem sinais de proteção do driver
 *
 */

void InitXintCPU(){


    XintRegs.XINT1CR.bit.POLARITY = 0b10; // Interrupt is selected as negative edge triggered - GPIO41 - driver c
    XintRegs.XINT1CR.bit.ENABLE = 1;      // Habilita a interrupção

    XintRegs.XINT2CR.bit.POLARITY = 0b10; // Interrupt is selected as negative edge triggered - GPIO87 - driver b
    XintRegs.XINT2CR.bit.ENABLE = 1;      // Habilita a interrupção

    XintRegs.XINT3CR.bit.POLARITY = 0b10; // Interrupt is selected as negative edge triggered - GPIO78 - driver a
    XintRegs.XINT3CR.bit.ENABLE = 1;      // Habilita a interrupção



}
