#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#include <stdint.h>

typedef const int16_t TEstado[3];

//Estrutura de dados das tabelas principais
typedef struct {
    uint16_t angfix:12; //Ângulo (duração) de aplicação do vetor, em ponto fixo normalizado em 60 graus
    uint16_t n:4; //Número do vetor no sextante
} TVetorAngulo;

//Permite acessar a palavra completa da estrutura TVetorAngulo
typedef union {
    uint16_t all; //Palavra completa
    TVetorAngulo bit; //Campos definidos em TVetorAngulo
} UVetorAngulo;

typedef struct {
    uint16_t NM;     //Número de linhas com diferentes índices de modulação
    uint16_t M0fix;   //Primeiro índice de modulação
    const UVetorAngulo* Tabela;
} TTabInfo;

typedef struct {
    int16_t Sa0:2;
    int16_t Sb0:2;
    int16_t Sc0:2;

    int16_t Sa1:2;
    int16_t Sb1:2;
    int16_t Sc1:2;
} TEstadosPack;

typedef union {
    uint16_t all;
    int16_t lsb:2;
    TEstadosPack bit;
} UEstadosPack;

typedef struct {
    //Modulação síncrona
    float To;
    unsigned angulos;
    TTabInfo *TabInfo;
    unsigned Nvet;
    int16_t Tm_angfix;

    //Modulação vetorial
    float fo;
    float Tsw;
    uint16_t N_theta;
} TModVars;

#endif /* TYPEDEF_H_ */
