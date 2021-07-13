

/**
 * main.c
 */

/*
 * Arquivos inclusos
 */
#include "global.h"


//Definir para usar declarações inline
#define USE_INLINE

#ifdef USE_INLINE
    #define INLINE inline
#else
    #define INLINE
#endif

//Protótipos de funções
INLINE void Inicializacao_Geral();
INLINE void Inicializacao_Controle();
INLINE void Tempo(); //Funçao dos contadores de tempo para temporização e sincronia das rotinas

INLINE void Reinicio(bool inicio); // Funções relacionadas aos estados da máquina de estados
INLINE void Protecao(bool inicio);
INLINE void Inicializacao(bool inicio);
INLINE void Operando(bool inicio);


//Temporização  - funcao Tempo()
unsigned int tempo_partida_OK, tempo_partida_NOK;
unsigned long tempo_reinicio, intervalo_reinicio;
unsigned cont_reinicio;

int main(void)
{

    InitSysCtrl();             // Inicializaçâo do dispositivo - DSP

    Inicializacao_Geral();     // Configurações preliminares dos periféricos
    Inicializacao_Controle();  // Inicialização das variaveis de controle e modulação

    //Define o estado inicial
    estado = _Inicializacao;
    estado_anterior = _Reinicio;
    erro = ERR_Normal;            //Sem erro
    causa_reinicio = RST_Normal; //Sem causa anterior de reinicialização


    // Laço principal
    while(1){

        // Aguarda estouro do timer 0
        while (!CpuTimer0Regs.TCR.bit.TIF) {};
        CpuTimer0Regs.TCR.bit.TIF = 1;  //Limpa o flag do timer

        /*
         * Rotinas síncronas (período = 1ms)
         */
        //Verifica se esta é a primeira iteração dentro do estado atual.
         bool inicio;
         if (estado != estado_anterior)
             inicio = 1;

         else
             inicio = 0;

         estado_anterior = estado;

         //Sinaliza o estado atual através do LED tricolor - Não implementado
         // if (erro == ERR_Normal) Sinalizar(estado);
         // else Sinalizar(erro);

         /*
          * Máquina de estados principal
          */

         //Atualiza os contadores de tempo gerais
           Tempo();

         //Rotinas que trocam o estado antecipadamente

         //Rotinas para tratar os estados individualmente
          switch (estado) {
                case _Reinicio:         Reinicio(inicio);       break;
                case _Inicializacao:    Inicializacao(inicio);  break;
                case _Operando:         Operando(inicio);       break;
                case _Protecao:         Protecao(inicio);       break;
          }

        //asm(" NOP");

    } //while

} //main

//**********************************************************************************************

//Rotina de inicialização geral das variáveis
void Inicializacao_Geral() {
    //Configurações dos pinos de I/O
    Initgpio();

    //Configuração de Xbar

    InitXbar();

    //Configurações de interrupção

    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    //Configurações de ADC
    InitAdcaMultCh(0x1B);   //     Trigger vem EPWM 12 socA
    InitAdcbMultCh(0x1B);   //     Trigger vem EPWM 12 socA


    //Tempo morto
    tempo_morto_ctr = ceil(Tm * f_clk);

    //Configuração dos moduladores PWM

    InitEPwm7();
    InitEPwm8();

    InitEPwm10();
    InitEPwm2();

    InitEPwm6();
    InitEPwm9();

    InitEPwm12(); // sync SD2 with pwm12
    InitEPwm11(); // sync SD1 with pwm11

    //Sincronização dos PWMs
    PWM_Sync();

    //Configuração dos temporizadores da CPU
    ConfigCpuTimer0();
    ConfigCpuTimer1();

    // Configuração do SDFM
    InitSdfm();

    // Configura CPUXint
    InitXintCPU();

}

//Reinicializa as variáveis de controle
void Inicializacao_Controle() {
  //Rampa de partida
     rampa_vf = 0;
     sentido = 0;
     M = 0;
     Mfix = 0;
     fo = fo_mod = 0;
     To = 1/fo;
     G = VS/fo_nom;

  //Ângulo elétrico
    theta = 0;
    thetafix = theta1fix = theta2fix = 0;

  //Modulações
    f_sw = Tsw = Tsw_mod = Tsw2_mod = 0;
    TBPRD = TBPRD2 = 0;

  //Modulação vetorial
    m_albe[1] = m_albe[0] = 0;
    m_max1 = m_min1 = m_sum1 = 0;
    m_max2 = m_min2 = m_sum3 = 0;
    theta_mod1 = theta_mod2 = 0;
    theta_step = 0;
    n_theta = N_theta = N_theta_mod = 0;
    phi_mod = 0;

  //Flags
    equilibrar = false;
    sw_reset = 0;
    sw_protecao = 0;

  //Tempo e ângulo mortos
    Tm_angfix = Tm_angfix_mod = 0;
    ang_min = 0;

  //Depuração
    cont_descarte = 0;

   //
    sdfm_index_SD1 = sdfm_index_SD2 = 0;

}

//Estado em que o sistema fica aguardando um tempo para reiniciar
INLINE void Reinicio(bool inicio) {
    //Inicialização do estado
    if (inicio){
        //Se já estava operando a um longo tempo, considera como primeiro reinicio.
        if(tempo_operando > Tempo_operacao_OK){
            cont_reinicio = 1;
        }
        else{
            //Contabiliza a nova tentativa de reinicio
            cont_reinicio++;
        }

        // zera o contador de tempo em operação
        tempo_operando = 0;

        //Primeira reinicialização recente?
        if(cont_reinicio == 1){
            //Inicializa o contador de tempo para reinício.
            intervalo_reinicio = Tempo_base_reinicio;
        }
        else{
            //Já reiniciou mais que uma vez recentemente. Multiplica o intervalo de tempo.
            intervalo_reinicio *= Multiplicador_tempo_reinicio;
        }
        tempo_reinicio = intervalo_reinicio;
        return;
    }

    //Contabiliza o tempo neste estado.
        if (tempo_reinicio > 0) tempo_reinicio--;

        //Esgotou o tempo ?
        if (tempo_reinicio == 0) {
            //Avança o estado.
            TrocarEstado(_Inicializacao);
        }
}

//Estado em que o sistema fica enquanto os conversores operam normalmente
INLINE void Inicializacao(bool inicio) {
    //Inicializa as variáveis de controle e muda de estado
    Inicializacao_Controle();

    TrocarEstado(_Operando);
}

INLINE void Operando(bool inicio) {
    if (inicio) {

       //Ativa os moduladores PWM
        PWM_Run();
        Interrupt_trip_enable();
        EnableInterrupts(); // Habilitação das interrupções
        }

        //Verifica o flag de reset
    if (sw_reset) {
        //Retira os sinais de comando
        ForceTrip();

        //Reinicia a operação
        TrocarEstado(_Reinicio);
        }

}


//Estado em que o sistema fica após ter atuado alguma proteção
INLINE void Protecao(bool inicio) {
    //Não implementado.

    ForceTrip();    //Retira os sinais de comando
    //ESTOP0;

    if (sw_reset) {
        //Reinicia a operação
       TrocarEstado(_Reinicio);
       }

    asm(" NOP");

}


//Atualiza os contadores de tempo
INLINE void Tempo() {
    //Contador para tempo em segundos
    if (++tempo_segundo == 1000) {
       //Reinicia o contador quando atinge 1000ms
        tempo_segundo = 0;

      //Contabiliza o tempo com a CPU ligada
        tempo_cpu++;

      //Se estiver operando os conversores, contabiliza este tempo.

        if (estado >= _Operando)
            tempo_operando++;
    }
}
