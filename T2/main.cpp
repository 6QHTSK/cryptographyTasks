#include <cstdio>

inline unsigned short HexRead() {
    int c;
    unsigned short result = 0;
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
unsigned short *Encrypt;
unsigned short *Decrypt;
unsigned short *pi_s_total;
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

inline void Hex32Print(unsigned num,char append){
    putchar(hexChar[num >> 28 & 0xf]);
    putchar(hexChar[num >> 24 & 0xf]);
    putchar(hexChar[num >> 20 & 0xf]);
    putchar(hexChar[num >> 16 & 0xf]);
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

inline void PI_S_DECRY(bitStruct *numberBitMap){
    numberBitMap->partMap = {
            PI_S_DECRY_PART(numberBitMap->partMap.p4),
            PI_S_DECRY_PART(numberBitMap->partMap.p3),
            PI_S_DECRY_PART(numberBitMap->partMap.p2),
            PI_S_DECRY_PART(numberBitMap->partMap.p1),
    };
}

inline void generatePi(){
    pi_s_total = new unsigned short[0x10000u];
    Encrypt = new unsigned short[0x10000u];
    Decrypt = new unsigned short[0x10000u];
    for(unsigned short i = 0; ;i++){
        bitStruct t1,t3;
        t1.z = t3.z = i;
        PI_S(&t1);
        pi_s_total[i] = t1.z;
        PI_P(&t1);
        Encrypt[i] = t1.z;
        PI_P(&t3);
        PI_S_DECRY(&t3);
        Decrypt[i] = t3.z;

        if (i == 0xFFFFu) break;
    }
}

inline unsigned short SPNEncrypt(unsigned short w, unsigned long long key){
    XOR(w,key);
    w = Encrypt[w];
    XOR(w,key);
    w = Encrypt[w];
    XOR(w,key);
    w = Encrypt[w];
    XOR(w, key);
    w = pi_s_total[w];
    w ^= (key >> 16) & 0xffff;
    return w;
}

#define abs(a) a = (a) >= 4000? (a) - 4000: 4000 - (a)

int main() {
    generatePi();
    unsigned n = DecRead();
    unsigned short* plainText, *crypt;
    plainText = new unsigned short[8000];
    crypt = new unsigned short[8000];
    for(int i=0;i<n;i++){
        for(int j=0;j<8000;j++){
            plainText[j] = HexRead();
            crypt[j] = HexRead();
        }
        // decrypt K52,K54
        unsigned countK52K54[16][16] = {0};
        for(int j=0;j<8000;j++){
            unsigned short x = plainText[j],y=crypt[j];
            unsigned char part = (x >> 11) ^ (x >> 9) ^ (x >> 8);
            unsigned short yp4 = y & 0xf, yp2 = (y >> 8) & 0xf;
            for(unsigned char L1=0;L1<16;L1++){
                for(unsigned char L2=0;L2<16;L2++){
                    auto u4p4 = PI_S_DECRY_PART(L2 ^ yp4);
                    auto u4p2 = PI_S_DECRY_PART(L1 ^ yp2);
                    countK52K54[L1][L2] += (part ^ (u4p2 >> 2) ^ (u4p2) ^ (u4p4 >> 2) ^ u4p4) & 0x1u;
                }
            }
        }

        for(unsigned L1=0;L1<16;L1++){
            for(unsigned L2=0;L2<16;L2++){
                abs(countK52K54[L1][L2]);
            }
        }

        bool quitFlag = false;
        unsigned short testP2 = plainText[10], testC2 = crypt[10];
        unsigned short testP3 = plainText[100], testC3 = crypt[100];

        for(int K52K54Situation = 0; K52K54Situation < 64; K52K54Situation++){
            unsigned max1 = 50;
            unsigned char K52,K54;
            for(unsigned L1=0;L1<16;L1++){
                for(unsigned L2=0;L2<16;L2++){
                    if(countK52K54[L1][L2]>max1){
                        max1 = countK52K54[L1][L2];
                        K52 = L1;
                        K54 = L2;
                    }
                }
            }

            countK52K54[K52][K54] = 0;
            unsigned countK51K53[16][16] = {0};

            for(int j=0;j<8000;j++){
                unsigned short x = plainText[j],y=crypt[j];
                unsigned char part = (x >> 11) ^ (x >> 10) ^ (x >> 9) ^ (x >> 8);
                unsigned char u4p2 = PI_S_DECRY_PART(K52 ^ (y >> 8) & 0xf);
                unsigned char yp3 = (y >> 4) & 0xf;
                unsigned char yp1 = (y >> 12) & 0xf;
                unsigned char u4p2part = (u4p2 >> 2) ^ u4p2;
                for(unsigned char L1=0;L1<16;L1++){
                    for(unsigned char L2=0;L2<16;L2++){
                        auto u4p3 = PI_S_DECRY_PART(L2 ^ yp3);
                        auto u4p1 = PI_S_DECRY_PART(L1 ^ yp1);
                        countK51K53[L1][L2] += (part ^ (u4p1 >> 2) ^ u4p1 ^ u4p2part ^ (u4p3 >> 2) ^ u4p3) & 0x1u;
                    }
                }
            }

            for(unsigned L1=0;L1<16;L1++){
                for(unsigned L2=0;L2<16;L2++){
                    abs(countK51K53[L1][L2]);
                }
            }

            for(int K51K53Situation = 0; K51K53Situation < 2; K51K53Situation++ ){
                unsigned max2 = 50;
                unsigned char K51,K53;
                for(unsigned L1=0;L1<16;L1++){
                    for(unsigned L2=0;L2<16;L2++){
                        if(countK51K53[L1][L2] > max2){
                            max2 = countK51K53[L1][L2];
                            K51 = L1;
                            K53 = L2;
                        }
                    }
                }
                countK51K53[K51][K53] = 0;
                unsigned short KeyP2 = K51 << 12 | K52 << 8 | K53 << 4 | K54 ;
                for(unsigned KeyP1=0; KeyP1 < 0x10000u; KeyP1++){
                    unsigned long long key = (KeyP1 << 16) | KeyP2;
                    if(SPNEncrypt(testP2,key) == testC2 &&
                       SPNEncrypt(testP3,key) == testC3){
                        Hex32Print((unsigned)key,'\n');
                        quitFlag = true;
                        break;
                    }
                }
                if(quitFlag)
                    break;
            }
            if(quitFlag)
                break;

        }
    }
    delete []pi_s_total;
    delete []Encrypt;
    delete []Decrypt;
    delete []plainText;
    delete []crypt;
    return 0;
}
