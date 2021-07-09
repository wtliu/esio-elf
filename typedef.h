#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#include <stdint.h>

typedef const int16_t TEstado[3];

//Estrutura de dados das tabelas principais
typedef struct {
    uint16_t angfix:12; //�ngulo (dura��o) de aplica��o do vetor, em ponto fixo normalizado em 60 graus
    uint16_t n:4; //N�mero do vetor no sextante
} TVetorAngulo;

//Permite acessar a palavra completa da estrutura TVetorAngulo
typedef union {
    uint16_t all; //Palavra completa
    TVetorAngulo bit; //Campos definidos em TVetorAngulo
} UVetorAngulo;

typedef struct {
    uint16_t NM;     //N�mero de linhas com diferentes �ndices de modula��o
    uint16_t M0fix;   //Primeiro �ndice de modula��o
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
    //Modula��o s�ncrona
    float To;
    unsigned angulos;
    TTabInfo *TabInfo;
    unsigned Nvet;
    int16_t Tm_angfix;

    //Modula��o vetorial
    float fo;
    float Tsw;
    uint16_t N_theta;
} TModVars;

#endif /* TYPEDEF_H_ */
