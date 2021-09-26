#pragma warning(disable:4146)
#define _CRT_SECURE_NO_WARNINGS
#include<gmp.h>
#include<stdio.h>

#define MINIUM_GAP (1u << 31u)

static unsigned Uprime[] = {
  2,3,5,7,11 };
static mpz_t prime[5];

void startUp() {
    for (int i = 0; i < 5; i++) {
        mpz_init_set_ui(prime[i], Uprime[i]);
    }
}

void gcd(const mpz_t in_a,const mpz_t in_b, mpz_t gcd) {
    mpz_t a, b, r;
    mpz_init(r);
    // a > b
    if (mpz_cmp(in_a, in_b)) {
        mpz_init_set(a, in_a);
        mpz_init_set(b, in_b);
    }
    else {
        mpz_init_set(b, in_a);
        mpz_init_set(a, in_b);
    }
    mpz_mod(r, a, b); // r0 = a % b
    // ri = ri-2 % ri-1
    while (mpz_cmp_ui(r, 0)) {
        mpz_set(a, b);
        mpz_set(b, r);
        mpz_mod(r, a, b);
    }
    // a // b
    mpz_set(gcd, b);
}

bool Miller_Rabin(const mpz_t n)     //判断素数 
{
    if (mpz_cmp_ui(n,2u) == 0)  return true;   //2是素数 
    if (mpz_cmp_ui(n,2u) < 0 || mpz_divisible_2exp_p(n,1u))  return false;     //如果x是偶数或者是0,1，那它不是素数 
    unsigned s = 0;
    bool flag = true;
    mpz_t d,n_1, b;
    mpz_init(b);
    mpz_init(d);
    mpz_init_set(n_1, n); 
    mpz_sub_ui(n_1, n_1, 1u); // n_1 = d = n - 1
    while (mpz_divisible_2exp_p(n_1, s))//将x分解成(2^s)*t的样子 
        s++;  
    s--;
    mpz_fdiv_q_2exp(d, n_1, s); // n-1 = 2^s * d
    for (const mpz_t& p : prime)      //随便选一个素数进行测试 
    {
        flag = false;
        mpz_powm(b, p, d, n);      //先算出p^d [b = p^d (mod n)]
        if (!mpz_cmp_ui(b, 1u)) {
            flag = true;
            //gmp_printf("%Zd = %Zd ^ %Zd (mod %Zd)\n", b, p, d, n);
            continue; // Case 1: 满足 p^d == 1 (mod n)
        } 
        for (unsigned j = 0; j < s; j++)    //然后进行s次平方 [0~s-1]
        {
            if (mpz_cmp(b, n_1) == 0) { //用二次探测判断 [b != n-1(-1) ]
                flag = true;
                //gmp_printf("%Zd = %Zd ^( 2^%d * %Zd) (mod %Zd)\n", b, p, j, d, n);
                break;  // Case 2: 满足 p^(d*2^r) == -1 (mod n);
            }
            mpz_powm_ui(b, b, 2u, n);   //求b的平方 [b = b^2 (mod n)]
        }
        if (!flag) {
            //gmp_printf("%Zd", p);
            return false;
        }
    }
    return true;   //如果进行多次测试都是对的，那么x就很有可能是素数 
}

void invert(const mpz_t a, const mpz_t r, mpz_t inv) {
    mpz_t u1, u2, u3, v1, v2, v3, q, nv1, nv2, nv3;
    // u1,u2,u3 = 1,0,a
    mpz_init_set_ui(u1, 1u);
    mpz_init_set_ui(u2, 0u);
    mpz_init_set(u3, a);
    // v1,v2,v3 = 0,1,r
    mpz_init_set_ui(v1, 0u);
    mpz_init_set_ui(v2, 1u);
    mpz_init_set(v3, r);
    // other init
    mpz_init(q);
    mpz_init(nv1);
    mpz_init(nv2);
    mpz_init(nv3);
    while (mpz_cmp_ui(v3, 0)) {

        // q = floor(u3/v3)
        mpz_fdiv_q(q, u3, v3);

        mpz_set(nv1, u1);
        mpz_set(nv2, u2);
        mpz_set(nv3, u3);

        mpz_submul(nv1, q, v1); // nv1 = u1 - q*v1
        mpz_submul(nv2, q, v2); // nv1 = u2 - q*v2
        mpz_submul(nv3, q, v3); // nv1 = u3 - q*v3

        mpz_set(u1, v1); // u1 = v1
        mpz_set(u2, v2); // u2 = v2
        mpz_set(u3, v3); // u3 = v3

        mpz_set(v1, nv1); // v1 = nv1
        mpz_set(v2, nv2); // v2 = nv2
        mpz_set(v3, nv3); // v3 = nv3

    }
    mpz_mod(inv, u1, r); // inv = u1 % r
}

int main() {
    startUp();
    //freopen(R"(S:\decryptionTask\in\T5\5.in)", "r", stdin);
    int N;
    mpz_t e, p, q, n, t, p_1, q_1, d , p_1q_1, gcdval;
    mpz_init(e);
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(t);
    mpz_init(p_1);
    mpz_init(q_1);
    mpz_init(d);
    mpz_init(p_1q_1);
    mpz_init(gcdval);
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        mpz_inp_str(e, stdin, 10);
        mpz_inp_str(p, stdin, 10);
        mpz_inp_str(q, stdin, 10);
        // e 不能太小
        if (mpz_cmp_ui(e, 65536) <= 0) {
            printf("ERROR\n");
            continue;
        }
        // p == q
        if (mpz_cmp(p, q) == 0) {
            printf("ERROR\n");
            continue;
        }
        if (Miller_Rabin(p) && Miller_Rabin(q)) {
            mpz_mul(n, p, q);
            mpz_sqrt(n, n); // n = sqrt(pq)

            mpz_sub(t, p, n);
            mpz_abs(t, t);
            // p 太靠近了
            if (mpz_cmp_ui(t, MINIUM_GAP) <= 0) {
                printf("ERROR\n");
                continue;
            }

            mpz_sub(t, q, n);
            mpz_abs(t, t);
            // q 太靠近了
            if (mpz_cmp_ui(t, MINIUM_GAP) <= 0) {
                printf("ERROR\n");
                continue;
            }

            mpz_sub_ui(p_1, p, 1u);
            mpz_sub_ui(q_1, q, 1u);
            mpz_mul(p_1q_1, p_1, q_1);
            gcd(p_1, q_1, gcdval);
            if (mpz_cmp_ui(gcdval, 16) > 0) {
                // 不够平滑
                printf("ERROR\n");
                continue;
            }
            gcd(e, p_1q_1, gcdval);
            if (mpz_cmp_ui(gcdval, 1) != 0) {
                // e和(p-1)(q-1)不互质
                printf("ERROR\n");
                continue;
            }
            invert(e, p_1q_1, d);
            gmp_printf("%Zd\n", d);
        }
        else {
            // p,q 为素数
            printf("ERROR\n");
            continue;
        }
    }
    return 0;
}