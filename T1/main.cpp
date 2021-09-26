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

const unsigned char pi_s[] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
const unsigned char pi_de_s[] = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};
unsigned short Encrypt[0x10000u];
unsigned short Decrypt[0x10000u];
unsigned short pi_s_total[0x10000u];
unsigned short pi_de_s_total[0x10000u];
//unsigned short pi_p_total[0x100u];
const char hexChar[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
typedef union {
    struct {
        unsigned char bit16: 1;
        unsigned char bit12: 1;
        unsigned char bit8: 1;
        unsigned char bit4: 1;
        unsigned char bit15: 1;
        unsigned char bit11: 1;
        unsigned char bit7: 1;
        unsigned char bit3: 1;
        unsigned char bit14: 1;
        unsigned char bit10: 1;
        unsigned char bit6: 1;
        unsigned char bit2: 1;
        unsigned char bit13: 1;
        unsigned char bit9: 1;
        unsigned char bit5: 1;
        unsigned char bit1: 1;
    } pi_p;
    struct {
        unsigned char bit16: 1;
        unsigned char bit15: 1;
        unsigned char bit14: 1;
        unsigned char bit13: 1;
        unsigned char bit12: 1;
        unsigned char bit11: 1;
        unsigned char bit10: 1;
        unsigned char bit9: 1;
        unsigned char bit8: 1;
        unsigned char bit7: 1;
        unsigned char bit6: 1;
        unsigned char bit5: 1;
        unsigned char bit4: 1;
        unsigned char bit3: 1;
        unsigned char bit2: 1;
        unsigned char bit1: 1;
    } bitmap;
    struct {
        unsigned char p4: 4;
        unsigned char p3: 4;
        unsigned char p2: 4;
        unsigned char p1: 4;
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

inline void HexPrint(unsigned short num,char append){
    putchar(hexChar[num >> 12 & 0xf]);
    putchar(hexChar[num >> 8 & 0xf]);
    putchar(hexChar[num >> 4 & 0xf]);
    putchar(hexChar[num & 0xf]);
    putchar(append);
}

#define PI_S_PART(number) pi_s[number]
#define PI_S_DECRY_PART(number) pi_de_s[number]

inline void XOR(unsigned short& number, unsigned long long& key) {
    number ^= (key >> 16u) & 0xffffu;
    key <<= 4u;
}

inline void PI_S(bitStruct *numberBitMap) {
    numberBitMap->partMap = {
            PI_S_PART(numberBitMap->partMap.p4),
            PI_S_PART(numberBitMap->partMap.p3),
            PI_S_PART(numberBitMap->partMap.p2),
            PI_S_PART(numberBitMap->partMap.p1),
    };
}

inline void PI_P(bitStruct *numberBitMap) {
    numberBitMap->bitmap = {
            numberBitMap->pi_p.bit16,
            numberBitMap->pi_p.bit15,
            numberBitMap->pi_p.bit14,
            numberBitMap->pi_p.bit13,
            numberBitMap->pi_p.bit12,
            numberBitMap->pi_p.bit11,
            numberBitMap->pi_p.bit10,
            numberBitMap->pi_p.bit9,
            numberBitMap->pi_p.bit8,
            numberBitMap->pi_p.bit7,
            numberBitMap->pi_p.bit6,
            numberBitMap->pi_p.bit5,
            numberBitMap->pi_p.bit4,
            numberBitMap->pi_p.bit3,
            numberBitMap->pi_p.bit2,
            numberBitMap->pi_p.bit1,
    };
}

inline void XOR_DECRY(unsigned short& number, unsigned long long & key){
    key >>= 4u;
    number ^= (key >> 16) & 0xffffu;
}

inline void PI_S_DECRY(bitStruct *numberBitMap){
    numberBitMap->partMap = {
            PI_S_DECRY_PART(numberBitMap->partMap.p4),
            PI_S_DECRY_PART(numberBitMap->partMap.p3),
            PI_S_DECRY_PART(numberBitMap->partMap.p2),
            PI_S_DECRY_PART(numberBitMap->partMap.p1),
    };
}

#define PI_P_DECRY(numberBitMap) PI_P(numberBitMap)

inline void generatePi(){
    for(unsigned short i = 0; ;i++){
        bitStruct t1,t2,t3;
        t1.z = t2.z = t3.z = i;
        PI_S(&t1);
        pi_s_total[i] = t1.z;
        PI_P(&t1);
        Encrypt[i] = t1.z;
        PI_S_DECRY(&t2);
        pi_de_s_total[i] = t2.z;
        PI_P(&t3);
        PI_S_DECRY(&t3);
        Decrypt[i] = t3.z;

        if (i == 0xFFFFu) break;
    }
}

inline void SPNEncrypt(unsigned short& w, unsigned long long& key){
    XOR(w,key);
    w = Encrypt[w];
    XOR(w,key);
    w = Encrypt[w];
    XOR(w,key);
    w = Encrypt[w];
    XOR(w, key);
    w = pi_s_total[w];
    w ^= (key >> 16) & 0xffff;
}

// 此处的Key已经左移了32位了
inline void SPNDecrypt(unsigned short& w,unsigned long long& key){
    w ^= (key >> 16) & 0xffff;
    w = pi_de_s_total[w];
    XOR_DECRY(w,key);
    w = Decrypt[w];
    XOR_DECRY(w,key);
    w = Decrypt[w];
    XOR_DECRY(w,key);
    w = Decrypt[w];
    XOR_DECRY(w,key);
}

int main() {
    generatePi();
    unsigned long n;
    n = DecRead();
    //scanf("%d",&n);
    for (int i = 0; i < n; i++) {
        //auto w = new bitStruct();
        auto key = HexRead<unsigned long long>();
        auto w = HexRead<unsigned short>();

        // Encrypt
        SPNEncrypt(w,key);
        // Encrypt End

        HexPrint(w,' ');

        w ^= 0x1u;

        // Decrypt
        SPNDecrypt(w,key);
        // Decrypt End
        HexPrint(w,'\n');
    }
    return 0;
}