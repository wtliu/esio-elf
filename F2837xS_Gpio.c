/* ###########################################################################
 *
 * Arquivo F2837xS_Gpio.c
 *
 * Funções de inicialização, configuração e suporte dos módulos de entrada
 * e saída GPIO
 *
 * As configurações executadas neste documento, sempre que contíverem
 * um número de página para consulta, essa se refere ao guia:
 *          TMS320F2837xS Delfino Microcontrollers
 *          Techinical Reference Manual
 *          SPRUHX5C
 *          Agosto de 2014, Revisão Setembro de 2015
 *
 *
 *
 * ###########################################################################
 * Versão 0.1a
 * Autor: César Daltoé Berci
 * Setembro de 2016
 * cesar@berci.com.br
 * Alterado do arquivo:
 * File: Gpio.c -- Lab File
 * Devices: TMS320F28x7x
 * Author: Technical Training Organization (TTO), Texas Instruments
 * ###########################################################################
 */


#include "F2837xS_device.h" // Main include file


/**********************************************************************
 * Function: InitGpio()
 *
 * Description: Initializes the shared GPIO pins on the F28x7x
**************************************************************/


void Initgpio(void)
{

    asm(" EALLOW");         // Enable EALLOW protected register access

//--- Unlock the GPxLOCK register bits for all ports
    GpioCtrlRegs.GPACR.all   = 0x00000000;
    GpioCtrlRegs.GPBCR.all   = 0x00000000;
    GpioCtrlRegs.GPCCR.all   = 0x00000000;
    GpioCtrlRegs.GPDCR.all   = 0x00000000;
    GpioCtrlRegs.GPECR.all   = 0x00000000;
    GpioCtrlRegs.GPFCR.all   = 0x00000000;

//--- Disable the register locks for all ports
    GpioCtrlRegs.GPALOCK.all = 0x00000000;
    GpioCtrlRegs.GPBLOCK.all = 0x00000000;
    GpioCtrlRegs.GPCLOCK.all = 0x00000000;
    GpioCtrlRegs.GPDLOCK.all = 0x00000000;
    GpioCtrlRegs.GPELOCK.all = 0x00000000;
    GpioCtrlRegs.GPFLOCK.all = 0x00000000;


//--- Group A pins
    GpioCtrlRegs.GPACTRL.all  = 0x00000000;     // QUALPRD = PLLSYSCLK for all group A GPIO
    GpioCtrlRegs.GPAQSEL1.all = 0x00000000;     // Synchronous qualification for all group A GPIO 0-15
    GpioCtrlRegs.GPAQSEL2.all = 0x00000000;     // Synchronous qualification for all group A GPIO 16-31
    GpioCtrlRegs.GPAODR.all   = 0x00000000;     // All outputs normal mode (no open-drain outputs)
    GpioCtrlRegs.GPACSEL1.all = 0x00000000;     // GPIO 0-7   \.
    GpioCtrlRegs.GPACSEL2.all = 0x00000000;     // GPIO 8-15   |. GPIODAT/SET/CLEAR/TOGGLE reg. master:
    GpioCtrlRegs.GPACSEL3.all = 0x00000000;     // GPIO 16-23  |. 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA1
    GpioCtrlRegs.GPACSEL4.all = 0x00000000;     // GPIO 24-31 /.
    GpioCtrlRegs.GPADIR.all   = 0x00000000;     // All GPIO are inputs
    GpioCtrlRegs.GPAPUD.all   = 0x00000000;     // All pullups enabled
    GpioCtrlRegs.GPAINV.all   = 0x00000000;     // No inputs inverted

//SDFM No synch- SD1: D3 e C3
 GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 3;
 GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;

// EPWMs 2,6,7,8,9,10 disable pull up
 GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1; // EPWM 2A 2B
 GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;

 GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1; // EPWM 6A 6B
 GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;

 GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1; // EPWM 7A 7B
 GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;

 GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1; // EPWM 8A 8B
 GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;

 GpioCtrlRegs.GPAPUD.bit.GPIO16 = 1; // EPWM 9A 9B
 GpioCtrlRegs.GPAPUD.bit.GPIO17 = 1;

 GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1; // EPWM 10A 10B
 GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;

//--- Group B pins
      GpioCtrlRegs.GPBCTRL.all  = 0x00000000;       // QUALPRD = PLLSYSCLK for all group B GPIO
      GpioCtrlRegs.GPBQSEL1.all = 0x00000000;       // Synchronous qualification for all group B GPIO 32-47
      GpioCtrlRegs.GPBQSEL2.all = 0x00000000;       // Synchronous qualification for all group B GPIO 48-63
      GpioCtrlRegs.GPBDIR.all   = 0x00000000;       // All group B GPIO are inputs
      GpioCtrlRegs.GPBPUD.all   = 0x00000000;       // All group B pullups enabled
      GpioCtrlRegs.GPBINV.all   = 0x00000000;       // No inputs inverted
      GpioCtrlRegs.GPBODR.all   = 0x00000000;       // All outputs normal mode (no open-drain outputs)
      GpioCtrlRegs.GPBCSEL1.all = 0x00000000;       // GPIO 32-39 \.
      GpioCtrlRegs.GPBCSEL2.all = 0x00000000;       // GPIO 40-47  |. GPIODAT/SET/CLEAR/TOGGLE reg. master:
      GpioCtrlRegs.GPBCSEL3.all = 0x00000000;       // GPIO 48-55  |. 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA1
      GpioCtrlRegs.GPBCSEL4.all = 0x00000000;       // GPIO 56-63 /.

//SDFM No synch- SD2: D2eC2, D3eC3, D4eC4

 GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;
 GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3;

 GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3;
 GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3;

 GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;
 GpioCtrlRegs.GPBQSEL2.bit.GPIO63 = 3;


//--- Group C pins
      GpioCtrlRegs.GPCCTRL.all  = 0x00000000;       // QUALPRD = PLLSYSCLK for all group pinsC GPIO
      GpioCtrlRegs.GPCQSEL1.all = 0x00000000;       // Synchronous qualification for all group C GPIO 64-79
      GpioCtrlRegs.GPCQSEL2.all = 0x00000000;       // Synchronous qualification for all group C GPIO 80-95
      GpioCtrlRegs.GPCDIR.all   = 0x00000000;       // All GPIO are inputs
      GpioCtrlRegs.GPCPUD.all   = 0x00000000;       // All pullups enabled
      GpioCtrlRegs.GPCINV.all   = 0x00000000;       // No inputs inverted
      GpioCtrlRegs.GPCODR.all   = 0x00000000;       // All outputs normal mode (no open-drain outputs)
      GpioCtrlRegs.GPCCSEL1.all = 0x00000000;       // GPIO 64-71 \.
      GpioCtrlRegs.GPCCSEL2.all = 0x00000000;       // GPIO 72-79  |. GPIODAT/SET/CLEAR/TOGGLE reg. master:
      GpioCtrlRegs.GPCCSEL3.all = 0x00000000;       // GPIO 80-87  |. 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA1
      GpioCtrlRegs.GPCCSEL4.all = 0x00000000;       // GPIO 88-95 /.

 //Pinos de habilitaçao drive e cond. - disable pullup
 //GpioCtrlRegs.GPCPUD.GPIO91 = 1;
 //GpioCtrlRegs.GPCPUD.GPIO92 = 1;


//--- Group D pins
      GpioCtrlRegs.GPDCTRL.all  = 0x00000000;       // QUALPRD = PLLSYSCLK for all group D GPIO
      GpioCtrlRegs.GPDQSEL1.all = 0x00000000;       // Synchronous qualification for all group D GPIO 96-111
      GpioCtrlRegs.GPDQSEL2.all = 0x00000000;       // Synchronous qualification for all group D GPIO 112-127
      GpioCtrlRegs.GPDDIR.all   = 0x00000000;       // All GPIO are inputs
      GpioCtrlRegs.GPDPUD.all   = 0x00000000;       // All pullups enabled
      GpioCtrlRegs.GPDINV.all   = 0x00000000;       // No inputs inverted
      GpioCtrlRegs.GPDODR.all   = 0x00000000;       // All outputs normal mode (no open-drain outputs)
      GpioCtrlRegs.GPDCSEL1.all = 0x00000000;       // GPIO 96-103  \.
      GpioCtrlRegs.GPDCSEL2.all = 0x00000000;       // GPIO 104-111  |. GPIODAT/SET/CLEAR/TOGGLE reg. master:
      GpioCtrlRegs.GPDCSEL3.all = 0x00000000;       // GPIO 112-119  |. 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA1
      GpioCtrlRegs.GPDCSEL4.all = 0x00000000;       // GPIO 120-127 /.


 //Pino do Rele GPIO99 - disable pullup
 //GpioCtrlRegs.GPDPUD.GPIO99 = 1;


//--- Group E pins
      GpioCtrlRegs.GPECTRL.all  = 0x00000000;       // QUALPRD = PLLSYSCLK for all group E GPIO
      GpioCtrlRegs.GPEQSEL1.all = 0x00000000;       // Synchronous qualification for all group E GPIO 128-143
      GpioCtrlRegs.GPEQSEL2.all = 0x00000000;       // Synchronous qualification for all group E GPIO 144-159
      GpioCtrlRegs.GPEDIR.all   = 0x00000000;       // All GPIO are inputs
      GpioCtrlRegs.GPEPUD.all   = 0x00000000;       // All pullups enabled
      GpioCtrlRegs.GPEINV.all   = 0x00000000;       // No inputs inverted
      GpioCtrlRegs.GPEODR.all   = 0x00000000;       // All outputs normal mode (no open-drain outputs)
      GpioCtrlRegs.GPECSEL1.all = 0x00000000;       // GPIO 128-135 \.
      GpioCtrlRegs.GPECSEL2.all = 0x00000000;       // GPIO 136-143  |. GPIODAT/SET/CLEAR/TOGGLE reg. master:
      GpioCtrlRegs.GPECSEL3.all = 0x00000000;       // GPIO 144-151  |. 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA1
      GpioCtrlRegs.GPECSEL4.all = 0x00000000;       // GPIO 152-159 /.

//--- Group F pins
      GpioCtrlRegs.GPFCTRL.all  = 0x00000000;       // QUALPRD = PLLSYSCLK for all group F GPIO
      GpioCtrlRegs.GPFQSEL1.all = 0x00000000;       // Synchronous qualification for all group F GPIO 160-168
      GpioCtrlRegs.GPFDIR.all   = 0x00000000;       // All GPIO are inputs
      GpioCtrlRegs.GPFPUD.all   = 0x00000000;       // All pullups enabled
      GpioCtrlRegs.GPFINV.all   = 0x00000000;       // No inputs inverted
      GpioCtrlRegs.GPFODR.all   = 0x00000000;       // All outputs normal mode (no open-drain outputs)
      GpioCtrlRegs.GPFCSEL1.all = 0x00000000;       // GPIO 160-167 \. GPIODAT/SET/CLEAR/TOGGLE reg. master:
      GpioCtrlRegs.GPFCSEL2.all = 0x00000000;       // GPIO 168     /. 0=CPU1, 1=CPU1.CLA1, 2=CPU2, 3=CPU2.CLA1


    /*
     * INICIALIZAÇÃO DOS PINOS DE PWM
     *
     * Inicia configurando como I/O em modo saída escrevendo 0 (não aciona interruptores).
     * Após a configuração dos módulos ePWM os pinos serão reconfigurados para periférico.
     *
     */

   //Escreve 0 nos pinos

   GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; // EPWM 2A 2B
   GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;

   GpioDataRegs.GPACLEAR.bit.GPIO10 = 1; // EPWM 6A 6B
   GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;

   GpioDataRegs.GPACLEAR.bit.GPIO12 = 1; // EPWM 7A 7B
   GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;

   GpioDataRegs.GPACLEAR.bit.GPIO14 = 1; // EPWM 8A 8B
   GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;

   GpioDataRegs.GPACLEAR.bit.GPIO16 = 1; // EPWM 9A 9B
   GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;

   GpioDataRegs.GPACLEAR.bit.GPIO18 = 1; // EPWM 10A 10B
   GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;

   // Configura direção do I/O como saída

   GpioCtrlRegs.GPADIR.bit.GPIO2 = 1; // EPWM 2A 2B
   GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;

   GpioCtrlRegs.GPADIR.bit.GPIO10 = 1; // EPWM 6A 6B
   GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;

   GpioCtrlRegs.GPADIR.bit.GPIO12 = 1; // EPWM 7A 7B
   GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;

   GpioCtrlRegs.GPADIR.bit.GPIO14 = 1; // EPWM 8A 8B
   GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;

   GpioCtrlRegs.GPADIR.bit.GPIO16 = 1; // EPWM 9A 9B
   GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;

   GpioCtrlRegs.GPADIR.bit.GPIO18 = 1; // EPWM 10A 10B
   GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;


   // Inicialização do pino de acionamento do rele

   GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;         // GPIO99 pin is clear - Relay_CPU off
   GpioCtrlRegs.GPDDIR.bit.GPIO99 = 1;           // GPIO99 pin is an output


   // Inicialização dos pinos de habilitação de VCC do Drv. e Cond.

   GpioDataRegs.GPCSET.bit.GPIO91 = 1;         // GPIO91 pin is set to 1 - Enable Cond. 3.3 V
   GpioCtrlRegs.GPCDIR.bit.GPIO91 = 1;         // GPIO91 pin is an output

   GpioDataRegs.GPCSET.bit.GPIO92 = 1;         // GPIO92 pin is set to 1 - Enable Drv. 3.3 V
   GpioCtrlRegs.GPCDIR.bit.GPIO92 = 1;         // GPIO92 pin is an output


   // GPIO4/EPWM3A/CB  -    fica como pino teste por enquanto

   GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;        // GPIO4 pin is clear
   GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;         // GPIO4 pin is an output
   GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;         // disable pullup

   // GPIO85/FLT CB

   GpioCtrlRegs.GPCDIR.bit.GPIO85 = 0;         // GPI85 pin is an input
   GpioCtrlRegs.GPCPUD.bit.GPIO85 = 0;         // enable pullup

   // GPIO41/FLT C

   GpioCtrlRegs.GPBDIR.bit.GPIO41 = 0;         // GPIO41 pin is an input
   GpioCtrlRegs.GPBPUD.bit.GPIO41 = 0;         // enable pullup

   // GPIO87/FLT B

   GpioCtrlRegs.GPCDIR.bit.GPIO87 = 0;         // GPIO87 pin is an input
   GpioCtrlRegs.GPCPUD.bit.GPIO87 = 0;         // enable pullup

   // GPIO78/FLT A

   GpioCtrlRegs.GPCDIR.bit.GPIO78 = 0;         // GPIO78 pin is an input
   GpioCtrlRegs.GPCPUD.bit.GPIO78 = 0;         // enable pullup

//--- Finish up
       asm(" EDIS");        // Disable EALLOW protected register access

} // end Initgpio()











