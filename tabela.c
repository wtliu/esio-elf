#include <stddef.h>

#include "definicoes.h"
#include "global.h"
#include "tabelas.h"

//Tabela de estados comprimida
//[sextante][vetor]
const UEstadosPack TabEstadosPack[6][12] = {
    {0x0,0xF01,0xC05,0xC71,0xF7D,0xD75,0xCC4,0xD34,0xDF7,0x311,0x34D,0x75D},
    {0x0,0xC05,0xCC4,0xD34,0xD75,0xDF7,0xD4,0x1C7,0x5D7,0xF01,0xC71,0xF7D},
    {0x0,0xCC4,0xD4,0x1C7,0xDF7,0x5D7,0x3D0,0x4D3,0x7DF,0xC05,0xD34,0xD75},
    {0x0,0xD4,0x3D0,0x4D3,0x5D7,0x7DF,0x311,0x71C,0x75D,0xCC4,0x1C7,0xDF7},
    {0x0,0x3D0,0x311,0x71C,0x7DF,0x75D,0xF01,0x34D,0xF7D,0xD4,0x4D3,0x5D7},
    {0x0,0x311,0xF01,0x34D,0x75D,0xF7D,0xC05,0xC71,0xD75,0x3D0,0x71C,0x7DF},
};

const int16_t TabSinal[12][3] = {
    {1, -1, -1},
    {1, 1, -1},
    {1, 1, -1},
    {-1, 1, -1},
    {-1, 1, -1},
    {-1, 1, 1},
    {-1, 1, 1},
    {-1, -1, 1},
    {-1, -1, 1},
    {1, -1, 1},
    {1, -1, 1},
    {1, -1, -1},
};

//Tabela de estados expandida na RAM
int16_t TabEstados[6][12][6];

//Lista os estados das chaves S1 e S4 em fun??o da posi??o da chave virtual de 3 estados
//[posicao_3estados][interruptor]
const unsigned TabModulador[3][2] = {
    {0,1},
    {0,0},
    {1,0}
};

const UVetorAngulo TabAngulos6[NM_6][2*6+1] = {
                                               {{0x1117},{0x422D},{0x3137},{0x1071},{0x3051},{0x40DD},{0x34EA},{0x50DD},{0x3051},{0x2071},{0x3137},{0x522D},{0x8B}},
                                               {{0x10FD},{0x4225},{0x310F},{0x105A},{0x306C},{0x40F9},{0x3521},{0x50F9},{0x306C},{0x205A},{0x310F},{0x5225},{0x7E}},

};

const UVetorAngulo TabAngulos7[NM_7][2*7+1] = {
                                               {{0x104F},{0x30FC},{0x40BC},{0x114D},{0x40B1},{0x31C0},{0x110B},{0x32B5},{0x210B},{0x31C0},{0x50B1},{0x214D},{0x50BC},{0x30FC},{0x27}},
                                               {{0x4013},{0x3110},{0x4098},{0x114F},{0x40D5},{0x31BD},{0x1103},{0x32DA},{0x2103},{0x31BD},{0x50D5},{0x214F},{0x5098},{0x3110},{0x9}},
                                               {{0x40BC},{0x30C6},{0x4050},{0x1159},{0x4100},{0x31B8},{0x10FC},{0x3304},{0x20FC},{0x31B8},{0x5100},{0x2159},{0x5050},{0x30C6},{0x5D}},
                                               {{0x41B8},{0x3051},{0x1161},{0x305F},{0x4110},{0x316F},{0x10F8},{0x333E},{0x20F8},{0x316F},{0x5110},{0x305F},{0x2161},{0x3051},{0xDB}},
                                               {{0x41E2},{0x3035},{0x1156},{0x307F},{0x4118},{0x315D},{0x10E8},{0x3355},{0x20E8},{0x315D},{0x5118},{0x307F},{0x2156},{0x3035},{0xF0}},
                                               {{0x4211},{0x3013},{0x114B},{0x30A8},{0x411E},{0x3145},{0x10D8},{0x3372},{0x20D8},{0x3145},{0x511E},{0x30A8},{0x214B},{0x3013},{0x108}},
                                               {{0x421A},{0x113B},{0x403E},{0x30DE},{0x40F9},{0x3108},{0x10CC},{0x33A4},{0x20CC},{0x3108},{0x50F9},{0x30DE},{0x503E},{0x213B},{0x10C}},
                                               {{0x4222},{0x112B},{0x4054},{0x30F7},{0x40F6},{0x30EB},{0x10BB},{0x33C1},{0x20BB},{0x30EB},{0x50F6},{0x30F7},{0x5054},{0x212B},{0x110}},
                                               {{0x4232},{0x111D},{0x4055},{0x30FA},{0x4103},{0x30EF},{0x10A7},{0x33CD},{0x20A7},{0x30EF},{0x5103},{0x30FA},{0x5055},{0x211D},{0x118}},
                                               {{0x4244},{0x110E},{0x4056},{0x3102},{0x4110},{0x30E9},{0x1093},{0x33DE},{0x2093},{0x30E9},{0x5110},{0x3102},{0x5056},{0x210E},{0x121}},
                                               {{0x4215},{0x10FA},{0x40C2},{0x3170},{0x4128},{0x1032},{0x2057},{0x3439},{0x1057},{0x2032},{0x5128},{0x3170},{0x50C2},{0x20FA},{0x10A}},
                                               {{0x4210},{0x10E8},{0x40CA},{0x3151},{0x40E9},{0x3057},{0x1079},{0x3481},{0x2079},{0x3057},{0x50E9},{0x3151},{0x50CA},{0x20E8},{0x107}},
                                               {{0x4216},{0x10D9},{0x40CE},{0x3150},{0x40F9},{0x3058},{0x1065},{0x3497},{0x2065},{0x3058},{0x50F9},{0x3150},{0x50CE},{0x20D9},{0x10A}},
                                               {{0x41E8},{0x10C3},{0x4106},{0x3177},{0x40DD},{0xA014},{0x1046},{0x3530},{0x2046},{0x7014},{0x50DD},{0x3177},{0x5106},{0x20C3},{0xF3}},
                                               {{0x4168},{0x10A2},{0x4165},{0x316F},{0x403B},{0x105A},{0x40B2},{0x3526},{0x50B2},{0x205A},{0x503B},{0x316F},{0x5165},{0x20A2},{0xB3}},
};

const UVetorAngulo TabAngulos8[NM_8][2*8+1] = {
                                               {{0x10AD},{0x407E},{0x1191},{0x405E},{0x3107},{0x11B9},{0x31F9},{0x1046},{0x82},{0x2046},{0x31F9},{0x21B9},{0x3107},{0x505E},{0x2191},{0x507E},{0x56}},
                                               {{0x1228},{0x40D6},{0x308A},{0x11C4},{0x314F},{0x4046},{0x10D5},{0x2092},{0x31A0},{0x1092},{0x20D5},{0x5046},{0x314F},{0x21C4},{0x308A},{0x50D6},{0x113}},
                                               {{0x1001},{0x4077},{0x11B8},{0x4087},{0x30F4},{0x11A6},{0x31EF},{0x10C1},{0x6},{0x20C1},{0x31EF},{0x21A6},{0x30F4},{0x5087},{0x21B8},{0x5077},{0x0}},
                                               {{0x1001},{0x407B},{0x11A6},{0x4097},{0x30EA},{0x119C},{0x31F8},{0x10C2},{0x3017},{0x20C2},{0x31F8},{0x219C},{0x30EA},{0x5097},{0x21A6},{0x507B},{0x0}},
                                               {{0x11B6},{0x40EE},{0x3048},{0x1189},{0x3142},{0x40A0},{0x10B7},{0x20C5},{0x3219},{0x10C5},{0x20B7},{0x50A0},{0x3142},{0x2189},{0x3048},{0x50EE},{0xDA}},
                                               {{0x122E},{0x4110},{0x30A4},{0x118F},{0x313C},{0x409E},{0x1049},{0x20CA},{0x3176},{0x10CA},{0x2049},{0x509E},{0x313C},{0x218F},{0x30A4},{0x5110},{0x116}},
                                               {{0x120E},{0x411B},{0x309F},{0x1183},{0x3139},{0x40A9},{0x104A},{0x20C5},{0x319F},{0x10C5},{0x204A},{0x50A9},{0x3139},{0x2183},{0x309F},{0x511B},{0x106}},
                                               {{0x120B},{0x412A},{0x30A9},{0x117C},{0x3135},{0x40C3},{0x1020},{0x20D6},{0x3185},{0x10D6},{0x2020},{0x50C3},{0x3135},{0x217C},{0x30A9},{0x512A},{0x105}},
                                               {{0x11E3},{0x412E},{0x3092},{0x115B},{0x3129},{0x40CF},{0x103B},{0x20CD},{0x31EB},{0x10CD},{0x203B},{0x50CF},{0x3129},{0x215B},{0x3092},{0x512E},{0xF1}},
                                               {{0x11CE},{0x4138},{0x308C},{0x114A},{0x3125},{0x40E0},{0x1035},{0x20CE},{0x320F},{0x10CE},{0x2035},{0x50E0},{0x3125},{0x214A},{0x308C},{0x5138},{0xE6}},
                                               {{0x1199},{0x410E},{0x30BA},{0x10F6},{0xA04F},{0x4032},{0x31EB},{0x10FA},{0x3226},{0x20FA},{0x31EB},{0x5032},{0x704F},{0x20F6},{0x30BA},{0x510E},{0xCC}},
                                               {{0x1186},{0x4118},{0x30B3},{0x1103},{0xA031},{0x405C},{0x31D0},{0x10F2},{0x3245},{0x20F2},{0x31D0},{0x505C},{0x7031},{0x2103},{0x30B3},{0x5118},{0xC2}},
                                               {{0x1174},{0x4116},{0x30A3},{0x4017},{0x1126},{0x408E},{0x31AE},{0x10E9},{0x3260},{0x20E9},{0x31AE},{0x508E},{0x2126},{0x5017},{0x30A3},{0x5116},{0xB9}},

};

const UVetorAngulo TabAngulos9[NM_9][2*9+1] = {
                                               {{0x408B},{0x120F},{0x4021},{0x30B3},{0x11C3},{0x30F6},{0x2065},{0x110F},{0x202E},{0x3101},{0x102E},{0x210F},{0x1065},{0x30F6},{0x21C3},{0x30B3},{0x5021},{0x220F},{0x45}},
                                               {{0x4094},{0x1203},{0x402B},{0x30B6},{0x11BD},{0x3102},{0x2058},{0x1106},{0x2033},{0x3108},{0x1033},{0x2106},{0x1058},{0x3102},{0x21BD},{0x30B6},{0x502B},{0x2203},{0x49}},
                                               {{0x408C},{0x11CF},{0x403A},{0x30A2},{0x11B5},{0x3119},{0x2028},{0x1114},{0x2070},{0x3132},{0x1070},{0x2114},{0x1028},{0x3119},{0x21B5},{0x30A2},{0x503A},{0x21CF},{0x45}},
                                               {{0x40A1},{0x11DF},{0x403F},{0x30B5},{0x11B5},{0x3160},{0x102C},{0x70},{0x1070},{0x3181},{0x2070},{0x70},{0x202C},{0x3160},{0x21B5},{0x30B5},{0x503F},{0x21DF},{0x50}},
                                               {{0x408E},{0x11A3},{0x4052},{0x3099},{0x119F},{0x312D},{0x111A},{0x302A},{0x208E},{0x3122},{0x108E},{0x302A},{0x211A},{0x312D},{0x219F},{0x3099},{0x5052},{0x21A3},{0x46}},
                                               {{0x409E},{0x11A6},{0x405A},{0x30A1},{0x1195},{0x312C},{0x1109},{0x3043},{0x2087},{0x3103},{0x1087},{0x3043},{0x2109},{0x312C},{0x2195},{0x30A1},{0x505A},{0x21A6},{0x4E}},
                                               {{0x40AD},{0x11AB},{0x4064},{0x30AE},{0x1194},{0x3135},{0x10EF},{0x3044},{0x207B},{0x30F3},{0x107B},{0x3044},{0x20EF},{0x3135},{0x2194},{0x30AE},{0x5064},{0x21AB},{0x56}},
                                               {{0x40A6},{0x118F},{0x4070},{0x30A7},{0x118C},{0x3131},{0x402B},{0x10D7},{0x2094},{0x316D},{0x1094},{0x20D7},{0x502B},{0x3131},{0x218C},{0x30A7},{0x5070},{0x218F},{0x52}},
                                               {{0x117A},{0x306D},{0x40A0},{0x1195},{0x4025},{0x3125},{0x115D},{0x3179},{0x206D},{0x33},{0x106D},{0x3179},{0x215D},{0x3125},{0x5025},{0x2195},{0x50A0},{0x306D},{0xBC}},
                                               {{0x115E},{0x3072},{0x40A2},{0x118B},{0x4032},{0x3126},{0x1156},{0x3198},{0x2047},{0x53},{0x1047},{0x3198},{0x2156},{0x3126},{0x5032},{0x218B},{0x50A2},{0x3072},{0xAE}},

};

const UVetorAngulo TabAngulos10[NM_10][2*10+1] = {
                                                  {{0x130C},{0x404D},{0x109D},{0x2060},{0x303A},{0x11D4},{0x30A1},{0x2082},{0x112F},{0x207B},{0x30B5},{0x107B},{0x212F},{0x1082},{0x30A1},{0x21D4},{0x303A},{0x1060},{0x209D},{0x504D},{0x185}},
                                                  {{0x1164},{0x400A},{0x304C},{0x124E},{0x309F},{0x202D},{0x118F},{0x2089},{0x30CB},{0x10F8},{0x14},{0x20F8},{0x30CB},{0x1089},{0x218F},{0x102D},{0x309F},{0x224E},{0x304C},{0x500A},{0xB1}},
                                                  {{0x11EE},{0x4019},{0x304D},{0x1229},{0x30AA},{0x2030},{0x1179},{0x2079},{0x30D8},{0x10C6},{0x2B},{0x20C6},{0x30D8},{0x1079},{0x2179},{0x1030},{0x30AA},{0x2229},{0x304D},{0x5019},{0xF6}},
                                                  {{0x11E8},{0x4023},{0x304B},{0x1219},{0x30B4},{0x2023},{0x1176},{0x2078},{0x30DD},{0x10DB},{0x18},{0x20DB},{0x30DD},{0x1078},{0x2176},{0x1023},{0x30B4},{0x2219},{0x304B},{0x5023},{0xF3}},
                                                  {{0x11DA},{0x402D},{0x3049},{0x120A},{0x30BE},{0x2015},{0x1175},{0x2078},{0x30E1},{0x10F6},{0x2},{0x20F6},{0x30E1},{0x1078},{0x2175},{0x1015},{0x30BE},{0x220A},{0x3049},{0x502D},{0xEC}},
                                                  {{0x11D6},{0x4036},{0x3049},{0x11FC},{0x30C8},{0x2009},{0x1171},{0x2077},{0x30E7},{0x10F8},{0x3010},{0x20F8},{0x30E7},{0x1077},{0x2171},{0x1009},{0x30C8},{0x21FC},{0x3049},{0x5036},{0xEA}},
                                                  {{0x11DE},{0x4041},{0x3048},{0x11EC},{0x30D0},{0x116C},{0x3012},{0x2075},{0x30D9},{0x10F2},{0x3025},{0x20F2},{0x30D9},{0x1075},{0x3012},{0x216C},{0x30D0},{0x21EC},{0x3048},{0x5041},{0xEE}},
                                                  {{0x11B7},{0x4051},{0x301D},{0x1167},{0x30A2},{0x119A},{0x30E4},{0x2059},{0x110B},{0x2051},{0x30FF},{0x1051},{0x210B},{0x1059},{0x30E4},{0x219A},{0x30A2},{0x2167},{0x301D},{0x5051},{0xDB}},
                                                  {{0x11DD},{0x405E},{0x3023},{0x114E},{0x309D},{0x1185},{0x30F0},{0x2042},{0x1107},{0x2065},{0x310D},{0x1065},{0x2107},{0x1042},{0x30F0},{0x2185},{0x309D},{0x214E},{0x3023},{0x505E},{0xEE}},

};

const UVetorAngulo TabAngulos11[NM_11][2*11+1] = {
                                                  {{0x144B},{0x3024},{0x2041},{0x118C},{0x2062},{0x3029},{0x10EC},{0x303D},{0x2077},{0x10F5},{0x20A0},{0x305D},{0x10A0},{0x20F5},{0x1077},{0x303D},{0x20EC},{0x3029},{0x1062},{0x218C},{0x1041},{0x3024},{0x225}},
                                                  {{0x145C},{0x3033},{0x2035},{0x1185},{0x2059},{0x3032},{0x10E7},{0x3040},{0x2078},{0x10F2},{0x2098},{0x306A},{0x1098},{0x20F2},{0x1078},{0x3040},{0x20E7},{0x3032},{0x1059},{0x2185},{0x1035},{0x3033},{0x22D}},
                                                  {{0x12DE},{0x302E},{0x2012},{0x11CB},{0x205A},{0x3056},{0x1186},{0x3012},{0x20B8},{0x3060},{0x1121},{0x3018},{0x2121},{0x3060},{0x10B8},{0x3012},{0x2186},{0x3056},{0x105A},{0x21CB},{0x1012},{0x302E},{0x16E}},
                                                  {{0x401A},{0x1233},{0x202F},{0x304C},{0x11C7},{0x3025},{0x2085},{0x3048},{0x1153},{0x3032},{0x20E6},{0x304B},{0x10E6},{0x3032},{0x2153},{0x3048},{0x1085},{0x3025},{0x21C7},{0x304C},{0x102F},{0x2233},{0xC}},
                                                  {{0x1270},{0x3040},{0x11F6},{0x3023},{0x2051},{0x3048},{0x1183},{0x3031},{0x20AB},{0x3051},{0x1110},{0x3035},{0x2110},{0x3051},{0x10AB},{0x3031},{0x2183},{0x3048},{0x1051},{0x3023},{0x21F6},{0x3040},{0x137}},
                                                  {{0x402D},{0x1231},{0x201F},{0x3063},{0x11C3},{0x3029},{0x2076},{0x305D},{0x1145},{0x3030},{0x20D6},{0x3064},{0x10D6},{0x3030},{0x2145},{0x305D},{0x1076},{0x3029},{0x21C3},{0x3063},{0x101F},{0x2231},{0x16}},
                                                  {{0x118B},{0x3053},{0x125D},{0x308F},{0x2041},{0x1176},{0x206C},{0x3088},{0x108C},{0x3047},{0x2068},{0x39},{0x1068},{0x3047},{0x208C},{0x3088},{0x106C},{0x2176},{0x1041},{0x308F},{0x225D},{0x3053},{0xC5}},
                                                  {{0x10DE},{0x3051},{0x1233},{0x3082},{0x200B},{0x114B},{0x2067},{0x3074},{0x111A},{0x3056},{0x20D0},{0x303F},{0x10D0},{0x3056},{0x211A},{0x3074},{0x1067},{0x214B},{0x100B},{0x3082},{0x2233},{0x3051},{0x6E}},

};

const UVetorAngulo TabAngulos12[NM_12][2*12+1] = {
                                                  {{0x17},{0x9023},{0x11E7},{0x205F},{0x1191},{0x2074},{0x3004},{0x1100},{0x3014},{0x2091},{0x1105},{0x20C9},{0x302B},{0x10C9},{0x2105},{0x1091},{0x3014},{0x2100},{0x3004},{0x1074},{0x2191},{0x105F},{0x21E7},{0x6023},{0xB}},
                                                  {{0x1068},{0x2034},{0x11F0},{0x2050},{0x1149},{0x2067},{0x300E},{0x1114},{0x301E},{0x2098},{0x1103},{0x20BA},{0x3034},{0x10BA},{0x2103},{0x1098},{0x301E},{0x2114},{0x300E},{0x1067},{0x2149},{0x1050},{0x21F0},{0x1034},{0x33}},
                                                  {{0x115F},{0x2037},{0x121C},{0x2071},{0x300E},{0x1161},{0x3023},{0x207E},{0x10F1},{0x2090},{0x3036},{0x10C4},{0xF},{0x20C4},{0x3036},{0x1090},{0x20F1},{0x107E},{0x3023},{0x2161},{0x300E},{0x1071},{0x221C},{0x1037},{0xAF}},
                                                  {{0x122B},{0x2034},{0x11E7},{0x205E},{0x3022},{0x1121},{0x3019},{0x2064},{0x10ED},{0x2094},{0x3053},{0x10CF},{0x27},{0x20CF},{0x3053},{0x1094},{0x20ED},{0x1064},{0x3019},{0x2121},{0x3022},{0x105E},{0x21E7},{0x1034},{0x115}},
                                                  {{0x14},{0x1217},{0x3020},{0x2040},{0x1187},{0x2064},{0x3024},{0x10FB},{0x3033},{0x2083},{0x10F6},{0x20A6},{0x3054},{0x10A6},{0x20F6},{0x1083},{0x3033},{0x20FB},{0x3024},{0x1064},{0x2187},{0x1040},{0x3020},{0x2217},{0x9}},
                                                  {{0x142F},{0x2041},{0x301F},{0x118F},{0x3036},{0x2063},{0x110A},{0x206C},{0x303C},{0x10CB},{0x3024},{0x20C2},{0x300C},{0x10C2},{0x3024},{0x20CB},{0x303C},{0x106C},{0x210A},{0x1063},{0x3036},{0x218F},{0x301F},{0x1041},{0x217}},
                                                  {{0x144E},{0x202F},{0x3037},{0x1183},{0x3002},{0x2060},{0x302B},{0x10F0},{0x3043},{0x2075},{0x10EF},{0x209E},{0x3068},{0x109E},{0x20EF},{0x1075},{0x3043},{0x20F0},{0x302B},{0x1060},{0x3002},{0x2183},{0x3037},{0x102F},{0x226}},

};

const UVetorAngulo TabAngulos13[NM_13][2*13+1] = {
                                                  {{0x1234},{0x48},{0x202D},{0x119C},{0x2061},{0x1C},{0x1102},{0xA},{0x207A},{0x1112},{0x20C4},{0x8},{0x10F9},{0x5},{0x20F9},{0x8},{0x10C4},{0x2112},{0x107A},{0xA},{0x2102},{0x1C},{0x1061},{0x219C},{0x102D},{0x48},{0x119}},
                                                  {{0x10EA},{0x1E},{0x2021},{0x11AB},{0x2022},{0x30},{0x2025},{0x112B},{0x2073},{0x111C},{0x20A6},{0x10FE},{0x20D0},{0x4},{0x10D0},{0x20FE},{0x10A6},{0x211C},{0x1073},{0x212B},{0x1025},{0x30},{0x1022},{0x21AB},{0x1021},{0x1E},{0x74}},
                                                  {{0x1216},{0x35},{0x2020},{0x113E},{0x2041},{0x111D},{0x2074},{0x10},{0x111F},{0x300D},{0x20A2},{0x10F2},{0x20BE},{0x300D},{0x10BE},{0x20F2},{0x10A2},{0x300D},{0x211F},{0x10},{0x1074},{0x211D},{0x1041},{0x213E},{0x1020},{0x35},{0x10A}},
                                                  {{0x1203},{0x29},{0x2021},{0x1156},{0x2049},{0x112A},{0x2078},{0x111E},{0x209D},{0x3009},{0x10E0},{0x301A},{0x20B1},{0x14},{0x10B1},{0x301A},{0x20E0},{0x3009},{0x109D},{0x211E},{0x1078},{0x212A},{0x1049},{0x2156},{0x1021},{0x29},{0x101}},
                                                  {{0x11A1},{0x1D},{0x2016},{0x115F},{0x204E},{0x1156},{0x2078},{0x3005},{0x1116},{0x3018},{0x2096},{0x10F4},{0x20B9},{0x3024},{0x10B9},{0x20F4},{0x1096},{0x3018},{0x2116},{0x3005},{0x1078},{0x2156},{0x104E},{0x215F},{0x1016},{0x1D},{0xD0}},
                                                  {{0x4001},{0x1029},{0x2031},{0x11EE},{0x2051},{0x1157},{0x206A},{0x300D},{0x1112},{0x301F},{0x2095},{0x1103},{0x20BC},{0x3033},{0x10BC},{0x2103},{0x1095},{0x301F},{0x2112},{0x300D},{0x106A},{0x2157},{0x1051},{0x21EE},{0x1031},{0x2029},{0x0}},

};

const UVetorAngulo TabAngulos14[NM_14][2*14+1] = {
                                                  {{0x54},{0x2034},{0x11D7},{0x2026},{0x60},{0x2024},{0x113C},{0x2078},{0x1081},{0x9044},{0x3},{0x20BA},{0x110D},{0x20CA},{0x3A},{0x10CA},{0x210D},{0x10BA},{0x3},{0x6044},{0x2081},{0x1078},{0x213C},{0x1024},{0x60},{0x1026},{0x21D7},{0x1034},{0x29}},
                                                  {{0x4F},{0x1167},{0x47},{0x2032},{0x1144},{0x2053},{0x2E},{0x10F8},{0x1A},{0x2086},{0x1104},{0x2099},{0x43},{0x10A8},{0x3034},{0x20A8},{0x43},{0x1099},{0x2104},{0x1086},{0x1A},{0x20F8},{0x2E},{0x1053},{0x2144},{0x1032},{0x47},{0x2167},{0x27}},
                                                  {{0x30},{0x2042},{0x11E0},{0x2012},{0x4B},{0x203E},{0x115F},{0x2089},{0x104E},{0x902E},{0x1056},{0x20A0},{0x10FC},{0x20CB},{0x24},{0x10CB},{0x20FC},{0x10A0},{0x2056},{0x602E},{0x204E},{0x1089},{0x215F},{0x103E},{0x4B},{0x1012},{0x21E0},{0x1042},{0x17}},
                                                  {{0x42},{0x116D},{0x36},{0x2033},{0x114E},{0x2057},{0x21},{0x110C},{0xF},{0x208B},{0x110A},{0x20A7},{0x27},{0x10BC},{0x3023},{0x20BC},{0x27},{0x10A7},{0x210A},{0x108B},{0xF},{0x210C},{0x21},{0x1057},{0x214E},{0x1033},{0x36},{0x216D},{0x20}},
                                                  {{0x1148},{0x2021},{0x2E},{0x116B},{0x25},{0x2046},{0x1137},{0x2071},{0x13},{0x1111},{0x8},{0x20A3},{0x10F9},{0x20C9},{0xB},{0x10C9},{0x20F9},{0x10A3},{0x8},{0x2111},{0x13},{0x1071},{0x2137},{0x1046},{0x25},{0x216B},{0x2E},{0x1021},{0xA3}},

};

const UVetorAngulo TabAngulos15[NM_15][2*15+1] = {
                                                  {{0x1144},{0x78},{0x10F2},{0x302E},{0x2015},{0x7F},{0x110B},{0x32},{0x2079},{0x2},{0x10F2},{0x5C},{0x207F},{0x10E4},{0x20A1},{0x5E},{0x10A1},{0x20E4},{0x107F},{0x5C},{0x20F2},{0x2},{0x1079},{0x32},{0x210B},{0x7F},{0x1015},{0x302E},{0x20F2},{0x78},{0xA1}},
                                                  {{0x1166},{0x6F},{0x2010},{0x112A},{0x2040},{0x4A},{0x1101},{0x38},{0x206B},{0x10F5},{0x2029},{0x4E},{0x205E},{0x10E3},{0x20AB},{0x4E},{0x10AB},{0x20E3},{0x105E},{0x4E},{0x1029},{0x20F5},{0x106B},{0x38},{0x2101},{0x4A},{0x1040},{0x212A},{0x1010},{0x6F},{0xB2}},
                                                  {{0x1160},{0x6C},{0x1105},{0x301C},{0x202C},{0x5B},{0x111E},{0x34},{0x2077},{0x1100},{0x2022},{0x45},{0x2065},{0x10E1},{0x20AC},{0x43},{0x10AC},{0x20E1},{0x1065},{0x45},{0x1022},{0x2100},{0x1077},{0x34},{0x211E},{0x5B},{0x102C},{0x301C},{0x2105},{0x6C},{0xAF}},
                                                  {{0x1169},{0x66},{0x1103},{0x3025},{0x2024},{0x5D},{0x1121},{0x2C},{0x2078},{0x1103},{0x2021},{0x3D},{0x2069},{0x10E6},{0x20B3},{0x3A},{0x10B3},{0x20E6},{0x1069},{0x3D},{0x1021},{0x2103},{0x1078},{0x2C},{0x2121},{0x5D},{0x1024},{0x3025},{0x2103},{0x66},{0xB4}},
                                                  {{0x1177},{0x58},{0x2012},{0x1138},{0x2043},{0x37},{0x1115},{0x27},{0x2070},{0x1106},{0x202D},{0x32},{0x2065},{0x10EB},{0x20B8},{0x30},{0x10B8},{0x20EB},{0x1065},{0x32},{0x102D},{0x2106},{0x1070},{0x27},{0x2115},{0x37},{0x1043},{0x2138},{0x1012},{0x58},{0xBB}},

};

const UVetorAngulo TabAngulos16[NM_16][2*16+1] = {
                                                  {{0x81},{0x1134},{0x6F},{0x202F},{0x1119},{0x2016},{0x6B},{0x2035},{0x10F5},{0x205B},{0x60},{0x2011},{0x10D2},{0x2091},{0x51},{0x10B0},{0x4},{0x20B0},{0x51},{0x1091},{0x20D2},{0x1011},{0x60},{0x105B},{0x20F5},{0x1035},{0x6B},{0x1016},{0x2119},{0x102F},{0x6F},{0x2134},{0x40}},
                                                  {{0x7B},{0x113A},{0x68},{0x202F},{0x111E},{0x2016},{0x63},{0x2037},{0x10FA},{0x205D},{0x58},{0x2011},{0x10D6},{0x2095},{0x49},{0x10B4},{0x4},{0x20B4},{0x49},{0x1095},{0x20D6},{0x1011},{0x58},{0x105D},{0x20FA},{0x1037},{0x63},{0x1016},{0x211E},{0x102F},{0x68},{0x213A},{0x3D}},
                                                  {{0x6C},{0x1134},{0x62},{0x202D},{0x1129},{0x2021},{0x5F},{0x202C},{0x1102},{0x2072},{0x4C},{0x10D6},{0xE},{0x20A2},{0x27},{0x10BB},{0x24},{0x20BB},{0x27},{0x10A2},{0xE},{0x20D6},{0x4C},{0x1072},{0x2102},{0x102C},{0x5F},{0x1021},{0x2129},{0x102D},{0x62},{0x2134},{0x35}},
                                                  {{0x6D},{0x1147},{0x68},{0x2023},{0x3014},{0x1115},{0x58},{0x204B},{0x1107},{0x206E},{0x4E},{0x10D6},{0x3006},{0x209F},{0x31},{0x10BB},{0x14},{0x20BB},{0x31},{0x109F},{0x3006},{0x20D6},{0x4E},{0x106E},{0x2107},{0x104B},{0x58},{0x2115},{0x3014},{0x1023},{0x68},{0x2147},{0x36}},

};

const UVetorAngulo TabAngulos17[NM_17][2*17+1] = {
                                                  {{0x1104},{0x84},{0x10EA},{0x1E},{0x203A},{0x4D},{0x10EE},{0x6D},{0x2051},{0x10D5},{0x203A},{0x78},{0x2025},{0x10B7},{0x207C},{0x5B},{0x1089},{0x7},{0x2089},{0x5B},{0x107C},{0x20B7},{0x1025},{0x78},{0x103A},{0x20D5},{0x1051},{0x6D},{0x20EE},{0x4D},{0x103A},{0x1E},{0x20EA},{0x84},{0x81}},
                                                  {{0x1109},{0x7E},{0x10EF},{0x1A},{0x203B},{0x4A},{0x10F3},{0x66},{0x2052},{0x10DA},{0x203C},{0x71},{0x2024},{0x10BA},{0x2080},{0x53},{0x108F},{0xB},{0x208F},{0x53},{0x1080},{0x20BA},{0x1024},{0x71},{0x103C},{0x20DA},{0x1052},{0x66},{0x20F3},{0x4A},{0x103B},{0x1A},{0x20EF},{0x7E},{0x84}},
                                                  {{0x111B},{0x7D},{0x10F3},{0xD},{0x203C},{0x4E},{0x10F4},{0x5F},{0x2052},{0x10DC},{0x2048},{0x6A},{0x2016},{0x10B9},{0x208D},{0x3E},{0x1097},{0x23},{0x2097},{0x3E},{0x108D},{0x20B9},{0x1016},{0x6A},{0x1048},{0x20DC},{0x1052},{0x5F},{0x20F4},{0x4E},{0x103C},{0xD},{0x20F3},{0x7D},{0x8D}},
                                                  {{0x111B},{0x73},{0x10F4},{0x1C},{0x203D},{0x38},{0x10FB},{0x5E},{0x204E},{0x10E3},{0x205E},{0x68},{0x10B4},{0x3012},{0x2083},{0x4D},{0x1099},{0xA},{0x2099},{0x4D},{0x1083},{0x3012},{0x20B4},{0x68},{0x105E},{0x20E3},{0x104E},{0x5E},{0x20FB},{0x38},{0x103D},{0x1C},{0x20F4},{0x73},{0x8D}},

};

const UVetorAngulo TabAngulos18[NM_18][2*18+1] = {
                                                  {{0x94},{0x10FC},{0x86},{0x2021},{0x10E2},{0x2016},{0x82},{0x201D},{0x10CA},{0x2042},{0x77},{0x2012},{0x10BA},{0x2059},{0x73},{0x2015},{0x109E},{0x207E},{0x73},{0x107E},{0x209E},{0x1015},{0x73},{0x1059},{0x20BA},{0x1012},{0x77},{0x1042},{0x20CA},{0x101D},{0x82},{0x1016},{0x20E2},{0x1021},{0x86},{0x20FC},{0x49}},
                                                  {{0x8E},{0x1101},{0x81},{0x2021},{0x10E7},{0x2019},{0x7D},{0x2019},{0x10CE},{0x204F},{0x76},{0x10A6},{0x3018},{0x2044},{0x7B},{0x201E},{0x10A3},{0x2080},{0x6E},{0x1080},{0x20A3},{0x101E},{0x7B},{0x1044},{0x3018},{0x20A6},{0x76},{0x104F},{0x20CE},{0x1019},{0x7D},{0x1019},{0x20E7},{0x1021},{0x81},{0x2101},{0x46}},
                                                  {{0x88},{0x1107},{0x7A},{0x2023},{0x10ED},{0x2016},{0x75},{0x2020},{0x10D3},{0x2044},{0x6A},{0x2013},{0x10C1},{0x205E},{0x64},{0x2015},{0x10A5},{0x2085},{0x65},{0x1085},{0x20A5},{0x1015},{0x64},{0x105E},{0x20C1},{0x1013},{0x6A},{0x1044},{0x20D3},{0x1020},{0x75},{0x1016},{0x20ED},{0x1023},{0x7A},{0x2107},{0x43}},

};

const UVetorAngulo TabAngulos19[NM_19][2*19+1] = {
                                                  {{0x10F1},{0x9C},{0x200A},{0x10DC},{0x201F},{0x91},{0x2003},{0x10C9},{0x2041},{0x83},{0x10B8},{0x11},{0x205F},{0x6C},{0x10A6},{0x1D},{0x2079},{0x5F},{0x1090},{0x21},{0x2090},{0x5F},{0x1079},{0x1D},{0x20A6},{0x6C},{0x105F},{0x11},{0x20B8},{0x83},{0x1041},{0x20C9},{0x1003},{0x91},{0x101F},{0x20DC},{0x100A},{0x9C},{0x78}},
                                                  {{0x10F5},{0x98},{0x2005},{0x10DD},{0x202A},{0x84},{0x10D6},{0x2A},{0x204A},{0x54},{0x10C1},{0x25},{0x205B},{0x59},{0x10A7},{0x12},{0x2076},{0x65},{0x1092},{0x11},{0x2092},{0x65},{0x1076},{0x12},{0x20A7},{0x59},{0x105B},{0x25},{0x20C1},{0x54},{0x104A},{0x2A},{0x20D6},{0x84},{0x102A},{0x20DD},{0x1005},{0x98},{0x7A}},
                                                  {{0x10FE},{0x90},{0x200B},{0x10EA},{0x201D},{0x84},{0x200D},{0x10DA},{0x2035},{0x7A},{0x2014},{0x10C3},{0x204F},{0x75},{0x2010},{0x10AB},{0x2075},{0x75},{0x108A},{0x3009},{0x208A},{0x75},{0x1075},{0x20AB},{0x1010},{0x75},{0x104F},{0x20C3},{0x1014},{0x7A},{0x1035},{0x20DA},{0x100D},{0x84},{0x101D},{0x20EA},{0x100B},{0x90},{0x7E}},

};

const UVetorAngulo TabAngulos20[NM_20][2*20+1] = {

};

const TTabInfo TabInfo[] = {
    {0, 0, NULL}, //1 ?ngulo
    {0, 0, NULL}, //2
    {0, 0, NULL}, //3
    {0, 0, NULL}, //4
    {0, 0, NULL}, //5
    {NM_6, 114, (UVetorAngulo*)TabAngulos6}, //6
    {NM_7, 100, (UVetorAngulo*)TabAngulos7}, //7
    {NM_8, 89, (UVetorAngulo*)TabAngulos8}, //8
    {NM_9, 80, (UVetorAngulo*)TabAngulos9}, //9
    {NM_10, 73, (UVetorAngulo*)TabAngulos10}, //10
    {NM_11, 67, (UVetorAngulo*)TabAngulos11}, //11
    {NM_12, 62, (UVetorAngulo*)TabAngulos12}, //12
    {NM_13, 57, (UVetorAngulo*)TabAngulos13}, //13
    {NM_14, 53, (UVetorAngulo*)TabAngulos14}, //14
    {NM_15, 50, (UVetorAngulo*)TabAngulos15}, //15
    {NM_16, 47, (UVetorAngulo*)TabAngulos16}, //16
    {NM_17, 44, (UVetorAngulo*)TabAngulos17}, //17
    {NM_18, 42, (UVetorAngulo*)TabAngulos18}, //18
    {NM_19, 40, (UVetorAngulo*)TabAngulos19}, //19
    //{NM_20, 40, (UVetorAngulo*)TabAngulos20}, //20
};

TTabInfo TabInfoRAM[2];
