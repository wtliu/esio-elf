/* ###########################################################################
 *
 * Arquivo F2837xS_user_device.h
 *
 *
 * Header com definições de macros e protótipos de funções não disponíveis nos
 * pacotes base do TMS320F2837x
 *
 *
 * ###########################################################################
 * Versão 0.1a
 * Autor: César Daltoé Berci/ Esio
 * Setembro de 2016
 * ###########################################################################
 *
 *
 */


#include "F2837xS_Cla_typedefs.h"   // F2837xS CLA Type definitions
#include "F2837xS_device.h"        // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"     // F2837xS Examples Include File


/*
 * Protótipos de funções
 */
/*extern void can_tx_msg(tCANMsgObject *TXCANMessage,uint16_t mask, uint16_t int_tx, uint16_t len, unsigned char *dados);
extern void can_rx_msg(tCANMsgObject *RXCANMessage, uint16_t msgId, uint16_t mask, uint16_t int_rx, uint16_t len, unsigned char *dados);
extern void can_init(uint32_t ui32Base);
*/

//Configuração GPIO

extern void Initgpio();

//Configurações do PWM - // verificar o CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0 - PWMsync()

extern void InitEPwm2();
extern void InitEPwm3();  //PWM do crowbar - Configurar*** GPIO04 Decidir se usa PWM ou  GPIO
extern void InitEPwm6();
extern void InitEPwm7();
extern void InitEPwm8();
extern void InitEPwm9();
extern void InitEPwm10();
extern void PWM_Sync();
extern void PWM_Run();
extern void ForceTrip();
extern void Interrupt_trip_enable();        // habilita a interrupção do trip - one shot trip

extern void InitEPwm11(); // EPWM11 utilizado para sincronizar SDMF1
extern void InitEPwm12(); // EPWM12 utilizado para sincronizar SDMF2

extern void Clr_Flag_DCAEVT(); // limpa os flags do DCEVT

// Sigma-Delta filter modules - Correntes e tensões
extern void InitSdfm();
extern void Sdfm_run();
extern void done(void);

// Habilita/configura o CPU.XINT - Mmonitora as GPIO
extern void InitXintCPU();

//Xbar - Initializes the Input, Output & ePWM X-Bar on the F28x7x, usado para configurar entrada do Ecap
extern void InitXbar();

// ECAP para temperatura

extern void Ecap1();   // TMP_IA
extern void Ecap2();   // TMP_RA



/*
extern void scia_init(uint16_t baud, uint16_t cpuF);
extern void scib_init(uint16_t baud, uint16_t cpuF);
*/

// ADC
extern void InitAdcaMultCh(uint16_t tsel);
extern void InitAdcbMultCh(uint16_t tsel);

//Timers
extern void ConfigCpuTimer0();
extern void ConfigCpuTimer1();

