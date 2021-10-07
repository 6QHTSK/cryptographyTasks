#include <stdio.h>
#include <gmp.h>

void expMod(mpz_t exp, const mpz_t m,const mpz_t e, mpz_t n){
    mpz_t currentE,currentM,ans;
    mpz_init_set(currentE,e);
    mpz_init_set(currentM,m);
    mpz_init_set_ui(ans,1);
    // Fast Exp
    while(mpz_cmp_ui(currentE,0u) > 0){
        //gmp_printf("%Zd\n",currentM);
        if(mpz_fdiv_q_ui(currentE,currentE,2u)){
            mpz_mul(ans,ans,currentM);
            mpz_fdiv_r(ans,ans,n);
        }
        mpz_mul(currentM,currentM,currentM);
        mpz_fdiv_r(currentM,currentM,n);
    }
    mpz_set(exp,ans);
}

int main() {
    //freopen("/programming/decryptionTask/in/T6/6.in","r",stdin);
    //freopen("/programming/decryptionTask/in/T6/6.my.out","w",stdout);
    int N;
    scanf("%d",&N);
    mpz_t e,m,p,q,n,exp;
    mpz_inits(e,m,p,q,n,exp,NULL);
    for(int i=0;i<N;i++){
        mpz_inp_str(e,stdin,10u);
        mpz_inp_str(m,stdin,10u);
        mpz_inp_str(p,stdin,10u);
        mpz_inp_str(q,stdin,10u);
        mpz_mul(n,p,q);
        //gmp_printf("%Zd^%Zd(mod %Zd)\n",m,e,n);
        expMod(exp,m,e,n);
        mpz_out_str(stdout,10u,exp);
        putchar('\n');
    }
    //printf("Hello, World!\n");
    return 0;
}
