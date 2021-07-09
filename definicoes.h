/*************************************************************************
                                INEP-UFSC
__________________________________________________________________________

ARQUIVO: definicoes.h
T�TULO : Defini��es gerais

__________________________________________________________________________

DATA: 26/12/2020
ATUALIZA��O: xx/xx/xxxx
VERS�O:1.0
__________________________________________________________________________
DESENVOLVIMENTO: ANDR� LANGE
__________________________________________________________________________
OBSERVA��ES:


**************************************************************************/

/*
 * FLAGS PARA HABILITAR/DESABILITAR PARTES DO C�DIGO
 * PARA REALIZA��O DE TESTES / DEBUG
 */

// Definir quando for ligar pot�ncia. Aciona o trip para o PWM caso o programa seja pausado pelo debugger.

//#define POTENCIA

// Se n�o definido, os sinais de comando permanecem desabilitados pelo trip-zone.

#define HABILITAR_COMANDO

// Definir para compensar a distor��o causada pelo tempo morto

//#define COMPENSAR_TEMPO_MORTO

// Definir para usar estrat�gia de modula��o que equilibra as tens�es do barramento CC

//#define EQUILIBRAR_VDC
