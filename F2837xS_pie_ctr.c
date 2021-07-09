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
* Fun��es de inicializa��o, configura��o e controle das interrup��es
* da cpu
*
* Esse arquivo configura as interrup��es que a cpu ir� tratar, por�m
* cada perif�rico precisa ser configurado para gerar a referida
* interrup��o.
**********************************************************************/

void InitPieCtrl(void)
{

    //
    // Disable Interrupts at the CPU level:
    //
    DINT;

    IER = 0x0000;   // Desabilita todas as interrup��es da CPU
    IFR = 0x0000;   // Limpa todos os flags de interrup��o da CPU

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
     * Habilita��o das interrup��es individuais.
     *
     * O grupo vem sempre antes das suas interrup��es individuais
     *
     * Refer�ncias
     *      Registradores PIEIERx, P�ginas 160 � 183
     *
     */


    IER |= M_INT3;       // Habilita o grupo 3 de interrup��es (PIEIER3) - EPWMs INTs
    //IER |= M_INT4;       // Habilita o grupo 3 de interrup��es (PIEIER4) - ECAPs INTs
    IER |= M_INT5;       // Habilita o grupo 5 de interrup��es (PIEIER5) - SDFMs INTs
    IER |= M_INT10;     // Habilita o grupo 10 de interrup��es (PIEIER10) - ADC2
    IER |= M_INT12;     // Habilita o grupo 12 de interrup��es (PIEIER10) - XINT3

    IER |= M_INT1;      // Habilita o grupo 1 de interrup��es (PIEIER1)
    //PieCtrlRegs.PIEIER1.bit.INTx1  = 1; //Habilida a interrup��o ADCA1
    //PieCtrlRegs.PIEIER1.bit.INTx2 = 1; //Habilida a interrup��o ADCB1
    //PieCtrlRegs.PIEIER1.bit.INTx3  = 1; //Habilida a interrup��o ADCC1
    PieCtrlRegs.PIEIER1.bit.INTx4  = 1; //Habilida a interrup��o XINIT1
    PieCtrlRegs.PIEIER1.bit.INTx5  = 1; //Habilida a interrup��o XINIT2
    //PieCtrlRegs.PIEIER1.bit.INTx6  = 1; //Habilida a interrup��o ADCD1
    //PieCtrlRegs.PIEIER1.bit.INTx7  = 1; //Habilida a interrup��o TINT0
    //PieCtrlRegs.PIEIER1.bit.INTx8  = 1; //Habilida a interrup��o WAKE
    //PieCtrlRegs.PIEIER1.bit.INTx13 = 1; //Habilida a interrup��o IPC0
    //PieCtrlRegs.PIEIER1.bit.INTx12 = 1; //Habilida a interrup��o IPC1
    //PieCtrlRegs.PIEIER1.bit.INTx15 = 1; //Habilida a interrup��o IPC2
    //PieCtrlRegs.PIEIER1.bit.INTx16 = 1; //Habilida a interrup��o IPC3

    IER |= M_INT2;       //Habilita o grupo 2 de interrup��es (PIEIER2)
    //PieCtrlRegs.PIEIER2.bit.INTx1  = 1; //Habilida a interrup��o PWM1-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx2  = 1; //Habilida a interrup��o PWM2-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx3  = 1; //Habilida a interrup��o PWM3-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx4  = 1; //Habilida a interrup��o PWM4-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx5  = 1; //Habilida a interrup��o PWM5-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx6  = 1; //Habilida a interrup��o PWM6-TZ
     PieCtrlRegs.PIEIER2.bit.INTx7  = 1; //Habilida a interrup��o PWM7-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx8  = 1; //Habilida a interrup��o PWM8-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx9  = 1; //Habilida a interrup��o PWM9-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx10 = 1; //Habilida a interrup��o PWM10-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx11 = 1; //Habilida a interrup��o PWM11-TZ
    //PieCtrlRegs.PIEIER2.bit.INTx12 = 1; //Habilida a interrup��o PWM12-TZ

    //IER |= M_INT3;       //Habilita o grupo 3 de interrup��es (PIEIER3)
    //PieCtrlRegs.PIEIER3.bit.INTx1  = 1; //Habilida a interrup��o PWM1-INT
    //PieCtrlRegs.PIEIER3.bit.INTx2  = 1; //Habilida a interrup��o PWM2-INT
    //PieCtrlRegs.PIEIER3.bit.INTx3  = 1; //Habilida a interrup��o PWM3-INT
    //PieCtrlRegs.PIEIER3.bit.INTx4  = 1; //Habilida a interrup��o PWM4-INT
    //PieCtrlRegs.PIEIER3.bit.INTx5  = 1; //Habilida a interrup��o PWM5-INT
    //PieCtrlRegs.PIEIER3.bit.INTx6  = 1; //Habilida a interrup��o PWM6-INT
     PieCtrlRegs.PIEIER3.bit.INTx7  = 1; //Habilida a interrup��o PWM7-INT
    //PieCtrlRegs.PIEIER3.bit.INTx8  = 1; //Habilida a interrup��o PWM8-INT
    //PieCtrlRegs.PIEIER3.bit.INTx9  = 1; //Habilida a interrup��o PWM9-INT
    //PieCtrlRegs.PIEIER3.bit.INTx10 = 1; //Habilida a interrup��o PWM10-INT
    //PieCtrlRegs.PIEIER3.bit.INTx11 = 1; //Habilida a interrup��o PWM11-INT
    //PieCtrlRegs.PIEIER3.bit.INTx12 = 1; //Habilida a interrup��o PWM12-INT

    //IER |= M_INT4;       //Habilita o grupo 4 de interrup��es (PIEIER4)
    //PieCtrlRegs.PIEIER4.bit.INTx1  = 1; //Habilida a interrup��o ECAP1
    //PieCtrlRegs.PIEIER4.bit.INTx2  = 1; //Habilida a interrup��o ECAP2
    //PieCtrlRegs.PIEIER4.bit.INTx3  = 1; //Habilida a interrup��o ECAP3
    //PieCtrlRegs.PIEIER4.bit.INTx4  = 1; //Habilida a interrup��o ECAP4
    //PieCtrlRegs.PIEIER4.bit.INTx5  = 1; //Habilida a interrup��o ECAP5
    //PieCtrlRegs.PIEIER4.bit.INTx6  = 1; //Habilida a interrup��o ECAP6

    //IER |= M_INT9;       //Habilita o grupo 9 de interrup��es (PIEIER9)
    //PieCtrlRegs.PIEIER9.bit.INTx1  = 1; //Habilida a interrup��o SCIA_RX
    //PieCtrlRegs.PIEIER9.bit.INTx2  = 1; //Habilida a interrup��o SCIA_TX
    //PieCtrlRegs.PIEIER9.bit.INTx3  = 1; //Habilida a interrup��o SCIB_RX
    //PieCtrlRegs.PIEIER9.bit.INTx4  = 1; //Habilida a interrup��o SCIB_TX
    //PieCtrlRegs.PIEIER9.bit.INTx5  = 1; //Habilida a interrup��o CANA_1
    //PieCtrlRegs.PIEIER9.bit.INTx6  = 1; //Habilida a interrup��o CANA_2
    //PieCtrlRegs.PIEIER9.bit.INTx7  = 1; //Habilida a interrup��o CANB_1
    //PieCtrlRegs.PIEIER9.bit.INTx8  = 1; //Habilida a interrup��o CANB_2

    PieCtrlRegs.PIEIER5.bit.INTx9  = 1; //Habilida a interrup��o SD1-INT
    PieCtrlRegs.PIEIER5.bit.INTx10  = 1; //Habilida a interrup��o SD2-INT
    PieCtrlRegs.PIEIER10.bit.INTx2 = 1; //Habilita a interrup��o ADCA2

    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; //Habilita a interrup��o XINIT3
}

//
// End of file
//














