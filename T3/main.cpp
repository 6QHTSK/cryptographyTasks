#include <cstdio>

template<class intType>
inline intType HexRead() {
    int c;
    intType result = 0;
    while ((c = getchar()) < 0x30);
    do {
        if (c >= 0x61) {
            result = result * 16 + c - 0x61 + 0x0a;
        } else {
            result = result * 16 + c - 0x30;
        }
    } while ((c = getchar()) >= 0x30);
    return result;
}

inline unsigned long DecRead() {
    int c;
    unsigned long result = 0;
    while ((c = getchar()) < 0x30);
    do {
        result = result * 10 + c - 0x30;
    } while ((c = getchar()) >= 0x30);
    return result;
}

const unsigned char pi_s[] = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};
const unsigned char pi_de_s[] = {0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5};
unsigned short Encrypt[0x10000u];
unsigned short Decrypt[0x10000u];
unsigned short pi_s_total[0x10000u];
unsigned short pi_de_s_total[0x10000u];
//unsigned short pi_p_total[0x100u];
const char hexChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
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

inline void HexPrint(unsigned num, char append) {
    putchar(hexChar[num >> 28u & 0xfu]);
    putchar(hexChar[num >> 24u & 0xfu]);
    putchar(hexChar[num >> 20u & 0xfu]);
    putchar(hexChar[num >> 16u & 0xfu]);
    putchar(hexChar[num >> 12u & 0xfu]);
    putchar(hexChar[num >> 8u & 0xfu]);
    putchar(hexChar[num >> 4u & 0xfu]);
    putchar(hexChar[num & 0xfu]);
    putchar(append);
}

#define PI_S_PART(number) pi_s[number]
#define PI_S_DECRY_PART(number) pi_de_s[number]

inline void XOR(unsigned short &number, unsigned long long &key) {
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

inline void XOR_DECRY(unsigned short &number, unsigned long long &key) {
    key >>= 4u;
    number ^= (key >> 16) & 0xffffu;
}

inline void PI_S_DECRY(bitStruct *numberBitMap) {
    numberBitMap->partMap = {
            PI_S_DECRY_PART(numberBitMap->partMap.p4),
            PI_S_DECRY_PART(numberBitMap->partMap.p3),
            PI_S_DECRY_PART(numberBitMap->partMap.p2),
            PI_S_DECRY_PART(numberBitMap->partMap.p1),
    };
}

#define PI_P_DECRY(numberBitMap) PI_P(numberBitMap)

inline void generatePi() noexcept {
    for (unsigned short i = 0;; i++) {
        bitStruct t1, t2, t3;
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

inline unsigned short SPNEncrypt(unsigned short w, unsigned long long key) noexcept {
    XOR(w, key);
    w = Encrypt[w];
    XOR(w, key);
    w = Encrypt[w];
    XOR(w, key);
    w = Encrypt[w];
    XOR(w, key);
    w = pi_s_total[w];
    w ^= (key >> 16u) & 0xffff;
    return w;
}

// 此处的Key已经左移了32位了
inline unsigned short SPNDecrypt(unsigned short w, unsigned long long key) noexcept {
    w ^= (key >> 16u) & 0xffff;
    w = pi_de_s_total[w];
    XOR_DECRY(w, key);
    w = Decrypt[w];
    XOR_DECRY(w, key);
    w = Decrypt[w];
    XOR_DECRY(w, key);
    w = Decrypt[w];
    XOR_DECRY(w, key);
    return w;
}

inline void getMax(unsigned map[0x10u][0x10u], unsigned &L1, unsigned &L2) noexcept {
    unsigned max = 0;
    for (int i = 0; i < 0x10; i++) {
        for (int j = 0; j < 0x10; j++) {
            if (map[i][j] > max) {
                max = map[i][j];
                L1 = i;
                L2 = j;
            }
        }
    }
    map[L1][L2] = 0;
}

static unsigned ciphertext[0x10000u];

int main() {
    generatePi();
    //freopen(R"(S:\decryptionTask\in\T3\5.in)", "r", stdin);
    unsigned n = DecRead();
    for (unsigned i = 0; i < n; i++) {
        unsigned K51K53Cnt[0x10u][0x10u] = {0};
        unsigned K52K54Cnt[0x10u][0x10u] = {0};
        for (unsigned int & plainText : ciphertext) {
            plainText = HexRead<unsigned short>();
        }
        auto xp1 = 0x0b00u, xp2 = 0x0020u;
        for (unsigned plainText0 = 0x7013; plainText0 < 0x9013; plainText0++) {
            auto plainText1 = plainText0 ^xp1;
            auto plainText2 = plainText0 ^xp2;
            unsigned cipher0 = ciphertext[plainText0];
            unsigned cipher1 = ciphertext[plainText1];
            unsigned cipher2 = ciphertext[plainText2];
            // Decrypt K52,K54
            if ((cipher0 & 0xf0f0u) == (cipher1 & 0xf0f0u)) {
                auto cipher0_2 = (cipher0 >> 8u) & 0xfu;
                auto cipher0_4 = cipher0 & 0xfu;
                auto cipher1_2 = (cipher1 >> 8u) & 0xfu;
                auto cipher1_4 = cipher1 & 0xfu;
                for (unsigned LK52 = 0; LK52 <= 0xfu; LK52++) {
                    auto u2_xor = PI_S_DECRY_PART(LK52 ^ cipher0_2) ^PI_S_DECRY_PART(LK52 ^ cipher1_2);
                    for (unsigned LK54 = 0; LK54 <= 0xfu; LK54++) {
                        auto u4_xor = PI_S_DECRY_PART(LK54 ^ cipher0_4) ^PI_S_DECRY_PART(LK54 ^ cipher1_4);
                        K52K54Cnt[LK52][LK54] += (unsigned int) (u2_xor == 0x6u && u4_xor == 0x6u);
                    }
                }
            }
            // Decrypt K51,K53
            if ((cipher0 & 0x0f0fu) == (cipher2 & 0x0f0fu)) {
                auto cipher0_1 = (cipher0 >> 12u) & 0xfu;
                auto cipher0_3 = (cipher0 >> 4u) & 0xfu;
                auto cipher2_1 = (cipher2 >> 12u) & 0xfu;
                auto cipher2_3 = (cipher2 >> 4u) & 0xfu;
                for (unsigned LK51 = 0; LK51 <= 0xfu; LK51++) {
                    auto u1_xor = PI_S_DECRY_PART(LK51 ^ cipher0_1) ^ PI_S_DECRY_PART(LK51 ^ cipher2_1);
                    for (unsigned LK53 = 0; LK53 <= 0xfu; LK53++) {
                        auto u3_xor = PI_S_DECRY_PART(LK53 ^ cipher0_3) ^ PI_S_DECRY_PART(LK53 ^ cipher2_3);
                        K51K53Cnt[LK51][LK53] += (unsigned int) ((u1_xor == 0x5u) && (u3_xor == 0x5u));
                    }
                }
            }
        }

        unsigned int K5List[16][5];

        for(auto & j : K5List){
            getMax(K51K53Cnt,j[1],j[3]);
            getMax(K52K54Cnt,j[2],j[4]);
        }

        for (auto & K52K54Situation : K5List) {
            unsigned K52 = K52K54Situation[2], K54 = K52K54Situation[4];
            //getMax(K52K54Cnt, K52, K54);
            for (auto & K51K53Situation : K5List) {
                unsigned K51 = K51K53Situation[1], K53 = K51K53Situation[3];
                //getMax(K51K53Cnt, K51, K53);
                unsigned K5 = K51 << 12u | K52 << 8u | K53 << 4u | K54;
                for (unsigned K1 = 0; K1 < 0x10000u; K1++) {
                    unsigned long long key = K1 << 16u | K5;
                    unsigned plainText = 0;
                    for (plainText = 9024; plainText < 9024 + 24; plainText++) {
                        if (SPNEncrypt(plainText, key) != ciphertext[plainText]) {
                            goto NEXT;
                        }
                    }
                    HexPrint( key, '\n');
                    goto START;
                    NEXT: continue;
                }
            }
        }
        printf("Failed!");
        START:;
    }
    return 0;
}