/**********************************************************************
* File: PieCtrl.c
* Devices: TMS320F28x7x
* Author: C2000 Technical Training, Texas Instruments
**********************************************************************/

// #include "Lab.h"          // Main include file
#include "F2837xS_device.h"     // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"   // F2837xS Examples Include File

/**********************************************************************
* Function: InitPieCtrl()
*
* Description: Initializes and enables the PIE interrupts on the F28x7x
* Funções de inicialização, configuração e controle das interrupções
* da cpu
*
* Esse arquivo configura as interrupções que a cpu irá tratar, porém
* cada periférico precisa ser configurado para gerar a referida
* interrupção.
**********************************************************************/

void InitPieCtrl(void)
{

    //
    // Disable Interrupts at the CPU level:
    //
    DINT;

    IER = 0x0000;   // Desabilita todas as interrupções da CPU
    IFR = 0x0000;   // Limpa todos os flags de interrupção da CPU

    //
    // Disable the PIE
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;


//--- Disable all PIE interrupts
    PieCtrlRegs.PIEIER1.all =  0x0000;
    PieCtrlRegs.PIEIER2.all =  0x0000;
    PieCtrlRegs.PIEIER3.all =  0x0000;
    PieCtrlRegs.PIEIER4.all =  0x0000;
    PieCtrlRegs.PIEIER5.all =  0x0000;
    PieCtrlRegs.PIEIER6.all =  0x0000;
    PieCtrlRegs.PIEIER7.all =  0x0000;
    PieCtrlRegs.PIEIER8.all =  0x0000;
    PieCtrlRegs.PIEIER9.all =  0x0000;
    PieCtrlRegs.PIEIER10.all = 0x0000;
    PieCtrlRegs.PIEIER11.all = 0x0000;
    PieCtrlRegs.PIEIER12.all = 0x0000;

//--- Clear any potentially pending PIEIFR flags
    PieCtrlRegs.PIEIFR1.all  = 0x0000;
    PieCtrlRegs.PIEIFR2.all  = 0x0000;
    PieCtrlRegs.PIEIFR3.all  = 0x0000;
    PieCtrlRegs.PIEIFR4.all  = 0x0000;
    PieCtrlRegs.PIEIFR5.all  = 0x0000;
    PieCtrlRegs.PIEIFR6.all  = 0x0000;
    PieCtrlRegs.PIEIFR7.all  = 0x0000;
    PieCtrlRegs.PIEIFR8.all  = 0x0000;
    PieCtrlRegs.PIEIFR9.all  = 0x0000;
    PieCtrlRegs.PIEIFR10.all = 0x0000;
    PieCtrlRegs.PIEIFR11.all = 0x0000;
    PieCtrlRegs.PIEIFR12.all = 0x0000;
}

void EnableInterrupts()
{

//--- Acknowlege all PIE interrupt groups
    PieCtrlRegs.PIEACK.all = 0xFFFF;

//--- Enable the PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;      // Enable the PIE


//---Enable Interrupts at the CPU level

    EINT;

/*
     * Habilitação das interrupções individuais.
     *
     * O grupo vem sempre antes das suas interrupções individuais
     *
     * Referências
     *      Registradores PIEIERx, Páginas 160 à 183
     *
     */


    IER |= M_INT3;       // Habilita o grupo 3 de interrupções (PIEIER3) - EPWMs INTs
    //IER |= M_INT4;       // Habilita o grupo 3 de interrupções (PIEIER4) - ECAPs INTs
    IER |= M_INT5;       // Habilita o grupo 5 de interrupções (PIEIER5) - SDFMs INTs
    IER |= M_INT10;     // Habilita o grupo 10 de interrupções (PIEIER10) - ADC2
    IER |= M_INT12;     // Habilita o grupo 12 de interrupções (PIEIER10) - XINT3

    IER |= M_INT1;      // Habilita o grupo 1 de interrupções (PIEIER1)
    //PieCtrlRegs.PIEIER1.bit.INTx1  = 1; //Habilida a interrupção ADCA1
    //PieCtrlRegs.PIEIER1.bit.INTx2 = 1; //Habilida a interrupção ADCB1
    //PieCtrlRegs.PIEIER1.bit.INTx3  = 1; //Habilida a interrupção ADCC1
    PieCtrlRegs.PIEIER1.bit.INTx4  = 1; //Habilida a interrupção XINIT1
    PieCtrlRegs.PIEIER1.bit.INTx5  = 1; //Habilida a interrupção XINIT2
    //PieCtrlRegs.PIEIER1.bit.INTx6  = 1; //Habilida a interrupção ADCD1
    //PieCtrlRegs.PIEIER1.bit.INTx7  = 1; //Habilida a interrupção TINT0
    //PieCtrlRegs.PIEIER1.bit.INTx8  = 1; //Habilida a interrupção WAKE
    //PieCtrlRegs.PIEIER1.bit.INTx13 = 1; //Habilida a interrupção IPC0
    //PieCtrlRegs.PIEIER1.bit.INTx12 = 1; //Habilida a interrupção IPC1
    //PieCtrlRegs.PIEIER1.bit.INTx15 = 1; //Habilida a interrupção IPC2
    //PieCtrlRegs.PIEIER1.bit.INTx16 = 1; //Habilida a interrupção IPC3

    IER |= M_INT2;       //Habilita o grupo 2 de interrupções (PIEIER2)
    //PieCtrlRegs.PIEIER2.bit.INTx1  = 1; //Habilida a interrupção PWM1-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx2  = 1; //Habilida a interrupção PWM2-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx3  = 1; //Habilida a interrupção PWM3-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx4  = 1; //Habilida a interrupção PWM4-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx5  = 1; //Habilida a interrupção PWM5-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx6  = 1; //Habilida a interrupção PWM6-TZ
     PieCtrlRegs.PIEIER2.bit.INTx7  = 1; //Habilida a interrupção PWM7-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx8  = 1; //Habilida a interrupção PWM8-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx9  = 1; //Habilida a interrupção PWM9-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx10 = 1; //Habilida a interrupção PWM10-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx11 = 1; //Habilida a interrupção PWM11-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx12 = 1; //Habilida a interrupção PWM12-TZ

    //IER |= M_INT3;       //Habilita o grupo 3 de interrupções (PIEIER3)
    //PieCtrlRegs.PIEIER3.bit.INTx1  = 1; //Habilida a interrupção PWM1-INT
    //PieCtrlRegs.PIEIER3.bit.INTx2  = 1; //Habilida a interrupção PWM2-INT
    //PieCtrlRegs.PIEIER3.bit.INTx3  = 1; //Habilida a interrupção PWM3-INT
    //PieCtrlRegs.PIEIER3.bit.INTx4  = 1; //Habilida a interrupção PWM4-INT
    //PieCtrlRegs.PIEIER3.bit.INTx5  = 1; //Habilida a interrupção PWM5-INT
    //PieCtrlRegs.PIEIER3.bit.INTx6  = 1; //Habilida a interrupção PWM6-INT
     PieCtrlRegs.PIEIER3.bit.INTx7  = 1; //Habilida a interrupção PWM7-INT
    //PieCtrlRegs.PIEIER3.bit.INTx8  = 1; //Habilida a interrupção PWM8-INT
    //PieCtrlRegs.PIEIER3.bit.INTx9  = 1; //Habilida a interrupção PWM9-INT
    //PieCtrlRegs.PIEIER3.bit.INTx10 = 1; //Habilida a interrupção PWM10-INT
    //PieCtrlRegs.PIEIER3.bit.INTx11 = 1; //Habilida a interrupção PWM11-INT
    //PieCtrlRegs.PIEIER3.bit.INTx12 = 1; //Habilida a interrupção PWM12-INT

    //IER |= M_INT4;       //Habilita o grupo 4 de interrupções (PIEIER4)
    //PieCtrlRegs.PIEIER4.bit.INTx1  = 1; //Habilida a interrupção ECAP1
    //PieCtrlRegs.PIEIER4.bit.INTx2  = 1; //Habilida a interrupção ECAP2
    //PieCtrlRegs.PIEIER4.bit.INTx3  = 1; //Habilida a interrupção ECAP3
    //PieCtrlRegs.PIEIER4.bit.INTx4  = 1; //Habilida a interrupção ECAP4
    //PieCtrlRegs.PIEIER4.bit.INTx5  = 1; //Habilida a interrupção ECAP5
    //PieCtrlRegs.PIEIER4.bit.INTx6  = 1; //Habilida a interrupção ECAP6

    //IER |= M_INT9;       //Habilita o grupo 9 de interrupções (PIEIER9)
    //PieCtrlRegs.PIEIER9.bit.INTx1  = 1; //Habilida a interrupção SCIA_RX
    //PieCtrlRegs.PIEIER9.bit.INTx2  = 1; //Habilida a interrupção SCIA_TX
    //PieCtrlRegs.PIEIER9.bit.INTx3  = 1; //Habilida a interrupção SCIB_RX
    //PieCtrlRegs.PIEIER9.bit.INTx4  = 1; //Habilida a interrupção SCIB_TX
    //PieCtrlRegs.PIEIER9.bit.INTx5  = 1; //Habilida a interrupção CANA_1
    //PieCtrlRegs.PIEIER9.bit.INTx6  = 1; //Habilida a interrupção CANA_2
    //PieCtrlRegs.PIEIER9.bit.INTx7  = 1; //Habilida a interrupção CANB_1
    //PieCtrlRegs.PIEIER9.bit.INTx8  = 1; //Habilida a interrupção CANB_2

    PieCtrlRegs.PIEIER5.bit.INTx9  = 1; //Habilida a interrupção SD1-INT
    PieCtrlRegs.PIEIER5.bit.INTx10  = 1; //Habilida a interrupção SD2-INT
    PieCtrlRegs.PIEIER10.bit.INTx2 = 1; //Habilita a interrupção ADCA2

    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; //Habilita a interrupção XINIT3
}

//
// End of file
//














