#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
#include <cstdio>

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
    unsigned long n;
    n = DecRead();
    //scanf("%d",&n);
    for (int i = 0; i < n; i++) {
        auto key = new keyStruct();
        auto w = new bitStruct();
        key->key = HexRead<unsigned long long>();
        w->z = HexRead<unsigned short>();
        //scanf("%x%x",&key->key,&w->z);

        // Encrypt
        for (int j = 0; j < 3; j++) {
            XOR(w, key);
            //PI_S(w);
            w->z = pi_s_total[w->z];
            //PI_P(w);
            w->z = pi_p_total[w->z];
        }
        XOR(w, key);
        //PI_S(w);
        w->z = pi_s_total[w->z];
        w->z ^= key->current.key;
        // Encrypt End

        HexPrint(w,' ');

        w->bitmap.bit1 ^= 1u;

        // Decrypt
        w->z ^= key->current.key;
        //PI_S_DECRY(w);
        w->z = pi_de_s_total[w->z];
        XOR_DECRY(w,key);
        for (int j = 2; j >= 0; j--) {
            //PI_P_DECRY(w);
            w->z = pi_p_total[w->z];
            //PI_S_DECRY(w);
            w->z = pi_de_s_total[w->z];
            XOR_DECRY(w, key);
        }
        // Decrypt End
        HexPrint(w,'\n');
        delete key;
        delete w;
    }
    return 0;
}