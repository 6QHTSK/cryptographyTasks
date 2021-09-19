#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>

template<class intType>
inline intType HexRead() {
    int c;
    intType result = 0;
    while((c=getchar()) < 0x30);
    do {
        if (c >= 0x61) {
            result = result * 16 + c - 0x61 + 0x0a;
        } else{
            result = result * 16 + c - 0x30;
        }
    } while((c = getchar()) >= 0x30);
    return result;
}

inline unsigned long DecRead() {
    int c;
    unsigned long result = 0;
    while((c=getchar()) < 0x30);
    do{
        result = result * 10 + c - 0x30;
    }while((c = getchar()) >= 0x30);
    return result;
}

const unsigned char pi_s[0xFFu] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
const unsigned char pi_de_s[0xFFu] = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};
unsigned short pi_s_total[0xFFFFu];
unsigned short pi_de_s_total[0xFFFFu];
unsigned short pi_p_total[0xFFFFu];
const char hexChar[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
typedef union {
    struct {
        unsigned char bit1: 1;
        unsigned char bit5: 1;
        unsigned char bit9: 1;
        unsigned char bit13: 1;
        unsigned char bit2: 1;
        unsigned char bit6: 1;
        unsigned char bit10: 1;
        unsigned char bit14: 1;
        unsigned char bit3: 1;
        unsigned char bit7: 1;
        unsigned char bit11: 1;
        unsigned char bit15: 1;
        unsigned char bit4: 1;
        unsigned char bit8: 1;
        unsigned char bit12: 1;
        unsigned char bit16: 1;
    } pi_p;
    struct {
        unsigned char bit1: 1;
        unsigned char bit2: 1;
        unsigned char bit3: 1;
        unsigned char bit4: 1;
        unsigned char bit5: 1;
        unsigned char bit6: 1;
        unsigned char bit7: 1;
        unsigned char bit8: 1;
        unsigned char bit9: 1;
        unsigned char bit10: 1;
        unsigned char bit11: 1;
        unsigned char bit12: 1;
        unsigned char bit13: 1;
        unsigned char bit14: 1;
        unsigned char bit15: 1;
        unsigned char bit16: 1;
    } bitmap;
    struct {
        unsigned char p1: 4;
        unsigned char p2: 4;
        unsigned char p3: 4;
        unsigned char p4: 4;
    } partMap;
    unsigned short z;
} bitStruct;
typedef union {
    struct {
        unsigned short : 16;
        unsigned short key: 16;
    } current;
    unsigned long long key;
} keyStruct;

inline void HexPrint(bitStruct* num,char append){
    putchar(hexChar[num->partMap.p4]);
    putchar(hexChar[num->partMap.p3]);
    putchar(hexChar[num->partMap.p2]);
    putchar(hexChar[num->partMap.p1]);
    putchar(append);
}

#define PI_S_PART(number) pi_s[number]
#define PI_S_DECRY_PART(number) pi_de_s[number]

inline void XOR(bitStruct *numberBitMap, keyStruct *key) {
    numberBitMap->z ^= key->current.key;
    key->key <<= 4u;
}

inline void PI_S(bitStruct *numberBitMap) {
    numberBitMap->partMap = {
            PI_S_PART(numberBitMap->partMap.p1),
            PI_S_PART(numberBitMap->partMap.p2),
            PI_S_PART(numberBitMap->partMap.p3),
            PI_S_PART(numberBitMap->partMap.p4),
    };
}

inline void PI_P(bitStruct *numberBitMap) {
    numberBitMap->bitmap = {
            numberBitMap->pi_p.bit1,
            numberBitMap->pi_p.bit2,
            numberBitMap->pi_p.bit3,
            numberBitMap->pi_p.bit4,
            numberBitMap->pi_p.bit5,
            numberBitMap->pi_p.bit6,
            numberBitMap->pi_p.bit7,
            numberBitMap->pi_p.bit8,
            numberBitMap->pi_p.bit9,
            numberBitMap->pi_p.bit10,
            numberBitMap->pi_p.bit11,
            numberBitMap->pi_p.bit12,
            numberBitMap->pi_p.bit13,
            numberBitMap->pi_p.bit14,
            numberBitMap->pi_p.bit15,
            numberBitMap->pi_p.bit16,
    };
}

inline void XOR_DECRY(bitStruct *numberBitMap, keyStruct *key){
    key->key >>= 4u;
    numberBitMap->z ^= key->current.key;
}

inline void PI_S_DECRY(bitStruct *numberBitMap){
    numberBitMap->partMap = {
            PI_S_DECRY_PART(numberBitMap->partMap.p1),
            PI_S_DECRY_PART(numberBitMap->partMap.p2),
            PI_S_DECRY_PART(numberBitMap->partMap.p3),
            PI_S_DECRY_PART(numberBitMap->partMap.p4),
    };
}

#define PI_P_DECRY(numberBitMap) PI_P(numberBitMap)

inline void generatePi(){
    for(unsigned short i = 0; ;i++){
        bitStruct t1,t2,t3;
        t1.z = t2.z = t3.z = i;
        PI_S(&t1);
        PI_S_DECRY(&t2);
        PI_P(&t3);
        pi_s_total[i] = t1.z;
        pi_de_s_total[i] = t2.z;
        pi_p_total[i] = t3.z;
        if (i == 0xFFFFu) break;
    }
}

int main() {
    generatePi();
    //unsigned n = DecRead();
    //freopen(R"(S:\decryptionTask\T2\in\1.in)","r",stdin);
    for(int i=0;i<10;i++){
        auto start = clock();
        bitStruct plainText[8000],crypt[8000];
        for(int j=0;j<8000;j++){
            plainText[j].z = HexRead<unsigned short>();
            crypt[j].z = HexRead<unsigned short>();
        }
        // decrypt K52,K54
        unsigned count[16][16] = {0};

        for(int j=0;j<8000;j++){
            for(unsigned char L1=0;L1<16;L1++){
                for(unsigned char L2=0;L2<16;L2++){
                    bitStruct v4,u4,x = plainText[j],y = crypt[j];
                    v4.partMap ={
                            0,
                            (unsigned char)(L1 ^ (y.partMap.p2)),
                            0,
                            (unsigned char)(L2 ^ (y.partMap.p4))
                    };
                    //u4.z = pi_de_s_total[v4.z];
                    PI_S_DECRY(&v4);
                    u4 = v4;
                    if( (x.bitmap.bit5 ^ x.bitmap.bit7 ^ x.bitmap.bit8 ^ u4.bitmap.bit6 ^ u4.bitmap.bit8 ^ u4.bitmap.bit14 ^ u4.bitmap.bit16 & 0x1u) == (unsigned char)0x00u){
                        count[L1][L2]++;
                    }
                }
            }
        }
        auto stop = clock();
        printf("%u ms\n",stop-start);

        auto max = 0;
        unsigned char maxL1,maxL2;
        for(unsigned char L1=0;L1<16;L1++){
            for(unsigned char L2=0;L2<16;L2++){
                int t = abs((int)count[L1][L2]-4000);
                if(t>max){
                    max = t;
                    maxL1 = L1;
                    maxL2 = L2;
                }
            }
        }
        printf("%x %x",maxL1,maxL2);
    }
    return 0;
}
