/*
 * CPUXint.c
 *
 *  Created on: 1 de jun de 2021
 *      Author: Esio
 */

#include "F28x_Project.h"
#include "F2837xS_user_device.h"
#include "definicoes.h"             // Defini��es gerais de opera��o
#include "global.h"


/* Configura e habilita a interrup��o CPU.XINTx
 *
 * Monitora o estado das GPIO selecionadas e em caso de evento aciona uma interrup��o
 * GPIOs recebem sinais de prote��o do driver
 *
 */

void InitXintCPU(){


    XintRegs.XINT1CR.bit.POLARITY = 0b10; // Interrupt is selected as negative edge triggered - GPIO41 - driver c
    XintRegs.XINT1CR.bit.ENABLE = 1;      // Habilita a interrup��o

    XintRegs.XINT2CR.bit.POLARITY = 0b10; // Interrupt is selected as negative edge triggered - GPIO87 - driver b
    XintRegs.XINT2CR.bit.ENABLE = 1;      // Habilita a interrup��o

    XintRegs.XINT3CR.bit.POLARITY = 0b10; // Interrupt is selected as negative edge triggered - GPIO78 - driver a
    XintRegs.XINT3CR.bit.ENABLE = 1;      // Habilita a interrup��o



}
