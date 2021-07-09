/* ###########################################################################
 *
 * Arquivo ADC.c
 *
 * Funções de inicialização, configuração e suporte dos módulos ADC
 *
 * ###########################################################################
 * Versão 0.1a
 * Autor: César Daltoé Berci
 * Setembro de 2016
 * cesar@berci.com.br
 * ###########################################################################
 * Alterado do arquivo:
 * FILE:   F2837xS_Adc_defines.h
 * TITLE:  #defines used in ADC examples
 * $TI Release: F2837xS Support Library v200 $
 * $Release Date: Tue Jun 21 13:52:16 CDT 2016 $
 * $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
 *             http://www.ti.com/ ALL RIGHTS RESERVED $
 * ###########################################################################
 */


/*
 * Arquivos inclusos
 */
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"


/*
 * Inicialização do ADCA
 *
 * Tabela ADC-1-A
 * 00h ADCTRIG0  - Software only
 * 01h ADCTRIG1  - CPU1 Timer 0, TINT0n
 * 02h ADCTRIG2  - CPU1 Timer 1, TINT1n
 * 03h ADCTRIG3  - CPU1 Timer 2, TINT2n
 * 04h ADCTRIG4  - GPIO, ADCEXTSOC
 * 05h ADCTRIG5  - ePWM1, ADCSOCA
 * 06h ADCTRIG6  - ePWM1, ADCSOCB
 * 07h ADCTRIG7  - ePWM2, ADCSOCA
 * 08h ADCTRIG8  - ePWM2, ADCSOCB
 * 09h ADCTRIG9  - ePWM3, ADCSOCA
 * 0Ah ADCTRIG10 - ePWM3, ADCSOCB
 * 0Bh ADCTRIG11 - ePWM4, ADCSOCA
 * 0Ch ADCTRIG12 - ePWM4, ADCSOCB
 * 0Dh ADCTRIG13 - ePWM5, ADCSOCA
 * 0Eh ADCTRIG14 - ePWM5, ADCSOCB
 * 0Fh ADCTRIG15 - ePWM6, ADCSOCA
 * 10h ADCTRIG16 - ePWM6, ADCSOCB
 * 11h ADCTRIG17 - ePWM7, ADCSOCA
 * 12h ADCTRIG18 - ePWM7, ADCSOCB
 * 13h ADCTRIG19 - ePWM8, ADCSOCA
 * 14h ADCTRIG20 - ePWM8, ADCSOCB
 * 15h ADCTRIG21 - ePWM9, ADCSOCA
 * 16h ADCTRIG22 - ePWM9, ADCSOCB
 * 17h ADCTRIG23 - ePWM10, ADCSOCA
 * 18h ADCTRIG24 - ePWM10, ADCSOCB
 * 19h ADCTRIG25 - ePWM11, ADCSOCA
 * 1Ah ADCTRIG26 - ePWM11, ADCSOCB
 * 1Bh ADCTRIG27 - ePWM12, ADCSOCA
 * 1Ch ADCTRIG28 - ePWM12,
 */

/*
 * Funções do arquivo original
 */
//
// AdcSetMode - Set the resolution and signalmode for a given ADC. This will
//              ensure that the correct trim is loaded.
//
void AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode)
{
    Uint16 adcOffsetTrimOTPIndex; //index into OTP table of ADC offset trims
    Uint16 adcOffsetTrim;         //temporary ADC offset trim

    //
    //re-populate INL trim
    //
    CalAdcINL(adc);

    if(0xFFFF != *((Uint16*)GetAdcOffsetTrimOTP))
    {
        //
        //offset trim function is programmed into OTP, so call it
        //

        //
        //calculate the index into OTP table of offset trims and call
        //function to return the correct offset trim
        //
        adcOffsetTrimOTPIndex = 4*adc + 2*resolution + 1*signalmode;
        adcOffsetTrim = (*GetAdcOffsetTrimOTP)(adcOffsetTrimOTPIndex);
    }
    else
    {
        //
        //offset trim function is not populated, so set offset trim to 0
        //
        adcOffsetTrim = 0;
    }

    //
    //Apply the resolution and signalmode to the specified ADC.
    //Also apply the offset trim and, if needed, linearity trim correction.
    //
    switch(adc)
    {
    case ADC_ADCA:
        AdcaRegs.ADCCTL2.bit.RESOLUTION = resolution;
        AdcaRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
        AdcaRegs.ADCOFFTRIM.all = adcOffsetTrim;
        if(ADC_RESOLUTION_12BIT == resolution)
        {
            //
            //12-bit linearity trim workaround
            //
            AdcaRegs.ADCINLTRIM1 &= 0xFFFF0000;
            AdcaRegs.ADCINLTRIM2 &= 0xFFFF0000;
            AdcaRegs.ADCINLTRIM4 &= 0xFFFF0000;
            AdcaRegs.ADCINLTRIM5 &= 0xFFFF0000;
        }
        break;
    case ADC_ADCB:
        AdcbRegs.ADCCTL2.bit.RESOLUTION = resolution;
        AdcbRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
        AdcbRegs.ADCOFFTRIM.all = adcOffsetTrim;
        if(ADC_RESOLUTION_12BIT == resolution)
        {
            //
            //12-bit linearity trim workaround
            //
            AdcbRegs.ADCINLTRIM1 &= 0xFFFF0000;
            AdcbRegs.ADCINLTRIM2 &= 0xFFFF0000;
            AdcbRegs.ADCINLTRIM4 &= 0xFFFF0000;
            AdcbRegs.ADCINLTRIM5 &= 0xFFFF0000;
        }
        break;
    case ADC_ADCC:
        AdccRegs.ADCCTL2.bit.RESOLUTION = resolution;
        AdccRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
        AdccRegs.ADCOFFTRIM.all = adcOffsetTrim;
        if(ADC_RESOLUTION_12BIT == resolution)
        {
            //
            //12-bit linearity trim workaround
            //
            AdccRegs.ADCINLTRIM1 &= 0xFFFF0000;
            AdccRegs.ADCINLTRIM2 &= 0xFFFF0000;
            AdccRegs.ADCINLTRIM4 &= 0xFFFF0000;
            AdccRegs.ADCINLTRIM5 &= 0xFFFF0000;
        }
        break;
    case ADC_ADCD:
        AdcdRegs.ADCCTL2.bit.RESOLUTION = resolution;
        AdcdRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
        AdcdRegs.ADCOFFTRIM.all = adcOffsetTrim;
        if(ADC_RESOLUTION_12BIT == resolution)
        {
            //
            //12-bit linearity trim workaround
            //
            AdcdRegs.ADCINLTRIM1 &= 0xFFFF0000;
            AdcdRegs.ADCINLTRIM2 &= 0xFFFF0000;
            AdcdRegs.ADCINLTRIM4 &= 0xFFFF0000;
            AdcdRegs.ADCINLTRIM5 &= 0xFFFF0000;
        }
        break;
    }
}

//
// CalAdcINL - Loads INL trim values from OTP into the trim registers of the
//             specified ADC. Use only as part of AdcSetMode function, since
//             linearity trim correction is needed for some modes.
//
void CalAdcINL(Uint16 adc)
{
    switch(adc)
    {
    case ADC_ADCA:
        if(0xFFFF != *((Uint16*)CalAdcaINL))
        {
            //
            //trim function is programmed into OTP, so call it
            //
            (*CalAdcaINL)();
        }
        else
        {
            //
            //do nothing, no INL trim function populated
            //
        }
        break;
    case ADC_ADCB:
        if(0xFFFF != *((Uint16*)CalAdcbINL))
        {
            //
            //trim function is programmed into OTP, so call it
            //
            (*CalAdcbINL)();
        }
        else
        {
            //
            //do nothing, no INL trim function populated
            //
        }
        break;
    case ADC_ADCC:
        if(0xFFFF != *((Uint16*)CalAdccINL))
        {
            //
            //trim function is programmed into OTP, so call it
            //
            (*CalAdccINL)();
        }
        else
        {
            //
            //do nothing, no INL trim function populated
            //
        }
        break;
    case ADC_ADCD:
        if(0xFFFF != *((Uint16*)CalAdcdINL))
        {
            //
            //trim function is programmed into OTP, so call it
            //
            (*CalAdcdINL)();
        }
        else
        {
            //
            //do nothing, no INL trim function populated
            //
        }
        break;
    }
}

// Configurações iniciam aqui:

/*
 * Módulo A:
 */
void InitAdcaMultCh(uint16_t tsel)
{
    EALLOW;
    DevCfgRegs.SOFTPRES13.bit.ADC_A = 1;    // ADC is reset
    DevCfgRegs.SOFTPRES13.bit.ADC_A = 0;    // ADC is released from reset

    //--- Configure the ADC base registers
    AdcaRegs.ADCCTL2.all = 0b0110;      // ADC clock configuration = 4.0
    // bit 15-8      0's:    reserved
    // bit 7         0:      SIGNALMODE, configured by AdcSetMode() below to get calibration correct
    // bit 6         0:      RESOLUTION, configured by AdcSetMode() below to get calibration correct
    // bit 5-4       00:     reserved
    // bit 3-0       0110:   PRESCALE, ADC clock prescaler.  0110=CPUCLK/4

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    // bit 15-14     00:     reserved
    // bit 13        0:      ADCBSY, ADC busy, read-only
    // bit 12        0:      reserved
    // bit 11-8      0's:    ADCBSYCHN, ADC busy channel, read-only
    // bit 7         0:      ADCPWDNZ, ADC power down, 0=powered down, 1=powered up
    // bit 6-3       0000:   reserved
    // bit 2         1:      INTPULSEPOS, INT pulse generation, 0=start of conversion, 1=end of conversion
    // bit 1-0       00:     reserved

    AdcaRegs.ADCBURSTCTL.all = 0x0000;
    // bit 15        0:      BURSTEN, 0=burst mode disabled, 1=burst mode enabled
    // bit 14-12     000:    reserved
    // bit 11-8      0000:   BURSTSIZE, 0=1 SOC converted (don't care)
    // bit 7-6       00:     reserved
    // bit 5-0       000000: BURSTTRIGSEL, 00=software only (don't care)

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //--- Configuração do SOC0 (Start Of Conversion): Canal ADCINA0 - Vo_AB
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = tsel; // Trigger, Tabela ADC-1-A, Página 1251 - CPU1 Timer1
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;      // Convert channel (Para ch=0x0E -> Ch. 14 Pino 23 )
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 19;     // Janela de aquisição: (19+1)=20 ciclos (100 ns para SYSCLK de 200 MHz)
    AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 0;    // No ADC interrupt triggers SOC0 (TRIGSEL field determines trigger)

    //--- Configuração do SOC3 (Start Of Conversion): Canal ADCINA1 - Vo_BC
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = tsel; // Trigger, Tabela ADC-1-A, Página 1251
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 1;      // Convert channel (Para ch=0x0E -> Ch. 14 Pino 23 )
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = 19;     // Janela de aquisição: (19+1)=20 ciclos (100 ns para SYSCLK de 200 MHz)
    AdcaRegs.ADCINTSOCSEL1.bit.SOC3 = 0;    // No ADC interrupt triggers SOC0 (TRIGSEL field determines trigger)

    AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;  // All SOCs handled in round-robin mode

    //--- ADCA1 interrupt configuration
    AdcaRegs.ADCINTSEL1N2.bit.INT2CONT = 1;     // Interrupt pulses regardless of flag state
    AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 0;      // EOC0 triggers the interrupt
    AdcaRegs.ADCINTSEL1N2.bit.INT2E = 1;        // Enable the interrupt in the ADC - ADCA2

    //--- Finish up
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;          // Power up the ADC
    DELAY_US(1000);                             // Wait 1 ms after power-up before using the ADC
    EDIS;
                                     // Disable EALLOW protected register access
} //Fim da inicialização do módulo ADCA


/*
 * Módulo B:
 */
void InitAdcbMultCh(uint16_t tsel)
{
    EALLOW;

    DevCfgRegs.SOFTPRES13.bit.ADC_B = 1;    // ADC is reset
    DevCfgRegs.SOFTPRES13.bit.ADC_B = 0;    // ADC is released from reset

    //--- Configure the ADC base registers
    AdcbRegs.ADCCTL2.bit.PRESCALE = 0B0110;     // ADC clock configuration = /4.0
    // bit 15-8      0's:    reserved
    // bit 7         0:      SIGNALMODE, configured by AdcSetMode() below to get calibration correct
    // bit 6         0:      RESOLUTION, configured by AdcSetMode() below to get calibration correct
    // bit 5-4       00:     reserved
    // bit 3-0       0110:   PRESCALE, ADC clock prescaler.  0110=CPUCLK/4
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    // bit 15-14     00:     reserved
    // bit 13        0:      ADCBSY, ADC busy, read-only
    // bit 12        0:      reserved
    // bit 11-8      0's:    ADCBSYCHN, ADC busy channel, read-only
    // bit 7         0:      ADCPWDNZ, ADC power down, 0=powered down, 1=powered up
    // bit 6-3       0000:   reserved
    // bit 2         1:      INTPULSEPOS, INT pulse generation, 0=start of conversion, 1=end of conversion
    // bit 1-0       00:     reserved
    AdcbRegs.ADCBURSTCTL.all = 0x0000;
    // bit 15        0:      BURSTEN, 0=burst mode disabled, 1=burst mode enabled
    // bit 14-12     000:    reserved
    // bit 11-8      0000:   BURSTSIZE, 0=1 SOC converted (don't care)
    // bit 7-6       00:     reserved
    // bit 5-0       000000: BURSTTRIGSEL, 00=software only (don't care)
    /*
     * Configuração do módulo ADC
     * Para utilizar vários canais, o comando AdcSetMode pode ser removido
     * pois o módulo precisa ser configurado apenas uma unica vez.
     */

    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //--- Configuração do SOC1 (Start Of Conversion): Canal ADCINB0 - VG_AB
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = tsel; // Trigger, Tabela ADC-1-A, Página 1251
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 0;      // Convert channel (Para ch=0x0E -> Ch. 14 Pino 23 )
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = 19;     // Acquisition window set to (14+1)=15 cycles (100 ns with 200 MHz SYSCLK)
    AdcbRegs.ADCINTSOCSEL1.bit.SOC1 = 0;    // No ADC interrupt triggers SOC1 (TRIGSEL field determines trigger)

    //--- Configuração do SOC4 (Start Of Conversion): Canal ADCINB1 - VG_BC
    AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = tsel; // Trigger, Tabela ADC-1-A, Página 1251
    AdcbRegs.ADCSOC4CTL.bit.CHSEL = 1;      // Convert channel (Para ch=0x0E -> Ch. 14 Pino 23 )
    AdcbRegs.ADCSOC4CTL.bit.ACQPS = 19;     // Janela de aquisição: (19+1)=20 ciclos (100 ns para SYSCLK de 200 MHz)
    AdcbRegs.ADCINTSOCSEL1.bit.SOC4 = 0;    // No ADC interrupt triggers SOC4 (TRIGSEL field determines trigger)

    AdcbRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0;  // All SOCs handled in round-robin mode

   //--- ADCB1 interrupt configuration
    AdcbRegs.ADCINTSEL1N2.bit.INT1CONT = 1;   // Interrupt pulses regardless of flag state
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;      // Enable the interrupt in the ADC
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 1;    // EOC1 triggers the interrupt

    //--- Finish up
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;          // Power up the ADC
    DELAY_US(1000);                             // Wait 1 ms after power-up before using the ADC
    EDIS;
}






