

/**
 * main.c
 */

/*
 * Arquivos inclusos
 */
#include "global.h"


//Definir para usar declara��es inline
#define USE_INLINE

#ifdef USE_INLINE
    #define INLINE inline
#else
    #define INLINE
#endif

//Prot�tipos de fun��es
INLINE void Inicializacao_Geral();
INLINE void Inicializacao_Controle();
INLINE void Tempo(); //Fun�ao dos contadores de tempo para temporiza��o e sincronia das rotinas

INLINE void Reinicio(bool inicio); // Fun��es relacionadas aos estados da m�quina de estados
INLINE void Protecao(bool inicio);
INLINE void Inicializacao(bool inicio);
INLINE void Operando(bool inicio);


//Temporiza��o  - funcao Tempo()
unsigned int tempo_partida_OK, tempo_partida_NOK;
unsigned long tempo_reinicio, intervalo_reinicio;
unsigned cont_reinicio;

int main(void)
{

    InitSysCtrl();             // Inicializa��o do dispositivo - DSP

    Inicializacao_Geral();     // Configura��es preliminares dos perif�ricos
    Inicializacao_Controle();  // Inicializa��o das variaveis de controle e modula��o

    //Define o estado inicial
    estado = _Inicializacao;
    estado_anterior = _Reinicio;
    erro = ERR_Normal;            //Sem erro
    causa_reinicio = RST_Normal; //Sem causa anterior de reinicializa��o


    // La�o principal
    while(1){

        // Aguarda estouro do timer 0
        while (!CpuTimer0Regs.TCR.bit.TIF) {};
        CpuTimer0Regs.TCR.bit.TIF = 1;  //Limpa o flag do timer

        /*
         * Rotinas s�ncronas (per�odo = 1ms)
         */
        //Verifica se esta � a primeira itera��o dentro do estado atual.
         bool inicio;
         if (estado != estado_anterior)
             inicio = 1;

         else
             inicio = 0;

         estado_anterior = estado;

         //Sinaliza o estado atual atrav�s do LED tricolor - N�o implementado
         // if (erro == ERR_Normal) Sinalizar(estado);
         // else Sinalizar(erro);

         /*
          * M�quina de estados principal
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

//Rotina de inicializa��o geral das vari�veis
void Inicializacao_Geral() {
    //Configura��es dos pinos de I/O
    Initgpio();

    //Configura��o de Xbar

    InitXbar();

    //Configura��es de interrup��o

    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    //Configura��es de ADC
    InitAdcaMultCh(0x1B);   //     Trigger vem EPWM 12 socA
    InitAdcbMultCh(0x1B);   //     Trigger vem EPWM 12 socA


    //Tempo morto
    tempo_morto_ctr = ceil(Tm * f_clk);

    //Configura��o dos moduladores PWM

    InitEPwm7();
    InitEPwm8();

    InitEPwm10();
    InitEPwm2();

    InitEPwm6();
    InitEPwm9();

    InitEPwm12(); // sync SD2 with pwm12
    InitEPwm11(); // sync SD1 with pwm11

    //Sincroniza��o dos PWMs
    PWM_Sync();

    //Configura��o dos temporizadores da CPU
    ConfigCpuTimer0();
    ConfigCpuTimer1();

    // Configura��o do SDFM
    InitSdfm();

    // Configura CPUXint
    InitXintCPU();

}

//Reinicializa as vari�veis de controle
void Inicializacao_Controle() {
  //Rampa de partida
     rampa_vf = 0;
     sentido = 0;
     M = 0;
     Mfix = 0;
     fo = fo_mod = 0;
     To = 1/fo;
     G = VS/fo_nom;

  //�ngulo el�trico
    theta = 0;
    thetafix = theta1fix = theta2fix = 0;

  //Modula��es
    f_sw = Tsw = Tsw_mod = Tsw2_mod = 0;
    TBPRD = TBPRD2 = 0;

  //Modula��o vetorial
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

  //Tempo e �ngulo mortos
    Tm_angfix = Tm_angfix_mod = 0;
    ang_min = 0;

  //Depura��o
    cont_descarte = 0;

   //
    sdfm_index_SD1 = sdfm_index_SD2 = 0;

}

//Estado em que o sistema fica aguardando um tempo para reiniciar
INLINE void Reinicio(bool inicio) {
    //Inicializa��o do estado
    if (inicio){
        //Se j� estava operando a um longo tempo, considera como primeiro reinicio.
        if(tempo_operando > Tempo_operacao_OK){
            cont_reinicio = 1;
        }
        else{
            //Contabiliza a nova tentativa de reinicio
            cont_reinicio++;
        }

        // zera o contador de tempo em opera��o
        tempo_operando = 0;

        //Primeira reinicializa��o recente?
        if(cont_reinicio == 1){
            //Inicializa o contador de tempo para rein�cio.
            intervalo_reinicio = Tempo_base_reinicio;
        }
        else{
            //J� reiniciou mais que uma vez recentemente. Multiplica o intervalo de tempo.
            intervalo_reinicio *= Multiplicador_tempo_reinicio;
        }
        tempo_reinicio = intervalo_reinicio;
        return;
    }

    //Contabiliza o tempo neste estado.
        if (tempo_reinicio > 0) tempo_reinicio--;

        //Esgotou o tempo ?
        if (tempo_reinicio == 0) {
            //Avan�a o estado.
            TrocarEstado(_Inicializacao);
        }
}

//Estado em que o sistema fica enquanto os conversores operam normalmente
INLINE void Inicializacao(bool inicio) {
    //Inicializa as vari�veis de controle e muda de estado
    Inicializacao_Controle();

    TrocarEstado(_Operando);
}

INLINE void Operando(bool inicio) {
    if (inicio) {

       //Ativa os moduladores PWM
        PWM_Run();
        Interrupt_trip_enable();
        EnableInterrupts(); // Habilita��o das interrup��es
        }

        //Verifica o flag de reset
    if (sw_reset) {
        //Retira os sinais de comando
        ForceTrip();

        //Reinicia a opera��o
        TrocarEstado(_Reinicio);
        }

}


//Estado em que o sistema fica ap�s ter atuado alguma prote��o
INLINE void Protecao(bool inicio) {
    //N�o implementado.

    ForceTrip();    //Retira os sinais de comando
    //ESTOP0;

    if (sw_reset) {
        //Reinicia a opera��o
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
