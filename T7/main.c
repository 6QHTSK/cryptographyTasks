#include <stdio.h>
#include <gmp.h>
#include <time.h>

void expMod(mpz_t exp, const mpz_t m,const mpz_t e, const mpz_t n){
    mpz_t ans;
    mpz_init_set_ui(ans,1);

    unsigned int bitCount = mpz_sizeinbase(e, 2u);

    // Fast Exp
    for(int i = bitCount - 1;i>=0;i--){
        //mpz_mul(ans,ans,ans);
        //mpz_mod(ans,ans,n);
        mpz_powm_ui(ans,ans,2u,n);
        if(mpz_tstbit(e,i)){
            mpz_mul(ans,ans,m);
            mpz_mod(ans,ans,n);
        }
    }
    mpz_set(exp,ans);
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

static mpz_t M,Fac1,Fac2,d1,d2;

void crt_init(const mpz_t p,const mpz_t q){
    mpz_t y;
    mpz_inits(y,M,Fac1,Fac2,NULL);
    mpz_mul(M,p,q);

    invert(q,p,y);
    mpz_mul(Fac1,y,q);

    invert(p,q,y);
    mpz_mul(Fac2,y,p);

    mpz_clear(y);
}

void crt(const mpz_t r1, const mpz_t r2, mpz_t r){
    mpz_t x,ans;
    mpz_inits(x,ans,NULL);

    mpz_mul(ans,r1,Fac1);
    mpz_mod(ans,ans,M);

    mpz_mul(x,r2,Fac2);
    mpz_mod(ans,ans,M);

    mpz_add(ans,ans,x);

    mpz_mod(r,ans,M);
}

void GenerateD(const mpz_t e,const mpz_t p, const mpz_t q, mpz_t d){
    mpz_t p_1,q_1,phiN;
    mpz_inits(p_1,q_1,phiN,d1,d2,NULL);
    mpz_sub_ui(p_1,p,1u);
    mpz_sub_ui(q_1,q,1u);
    mpz_mul(phiN,p_1,q_1);
    invert(e,phiN,d);

    mpz_mod(d1,d,p_1);
    mpz_mod(d2,d,q_1);
}

int main() {
    time_t start = clock();
    //freopen("/programming/decryptionTask/in/T7/10.in","r",stdin);
    //freopen("/programming/decryptionTask/in/T7/10.my.out","w",stdout);
    int N;
    mpz_t p,q,e,d;
    mpz_inits(p,q,e,d,NULL);
    scanf("%d",&N);
    mpz_inp_str(p,stdin,10);
    mpz_inp_str(q,stdin,10);
    mpz_inp_str(e,stdin,10);
    GenerateD(e,p,q,d);

    mpz_t m1,m2,c,ans;
    mpz_inits(m1,m2,c,ans,NULL);
    crt_init(p,q);

    for(int i=0;i<N;i++){
        mpz_inp_str(c,stdin,10);
        expMod(m1,c,d1,p);
        expMod(m2,c,d2,q);
        //mpz_powm(m1,c,d1,p);
        //mpz_powm(m2,c,d2,q);
        crt(m1, m2, ans);
        //expMod(ans,c,d,M);
        mpz_out_str(stdout,10,ans);
        putchar('\n');
    }

    time_t end = clock();
    fprintf(stderr,"%ld ms\n",(end-start)/1000);
    return 0;
}