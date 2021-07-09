/*************************************************************************
                                INEP-UFSC
__________________________________________________________________________

ARQUIVO: definicoes.h
TÍTULO : Definições gerais

__________________________________________________________________________

DATA: 26/12/2020
ATUALIZAÇÃO: xx/xx/xxxx
VERSÃO:1.0
__________________________________________________________________________
DESENVOLVIMENTO: ANDRÉ LANGE
__________________________________________________________________________
OBSERVAÇÕES:


**************************************************************************/

/*
 * FLAGS PARA HABILITAR/DESABILITAR PARTES DO CÓDIGO
 * PARA REALIZAÇÃO DE TESTES / DEBUG
 */

// Definir quando for ligar potência. Aciona o trip para o PWM caso o programa seja pausado pelo debugger.

//#define POTENCIA

// Se não definido, os sinais de comando permanecem desabilitados pelo trip-zone.

#define HABILITAR_COMANDO

// Definir para compensar a distorção causada pelo tempo morto

//#define COMPENSAR_TEMPO_MORTO

// Definir para usar estratégia de modulação que equilibra as tensões do barramento CC

//#define EQUILIBRAR_VDC
