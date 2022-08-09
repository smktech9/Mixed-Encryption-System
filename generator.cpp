#include<iostream>
#include<gmp.h>
#include<time.h>
#include <fstream>
using namespace std;
int i=123213;
int opened=0;
void get_random_number(mpz_t rand_Num ,unsigned long long int bits){

    	unsigned long int seed;
	unsigned long long int base = 2 , exp = 128;
	mpz_t offset;
	mpz_init(offset);
	mpz_ui_pow_ui(offset , base , exp);
	
    	gmp_randstate_t r_state;

    	seed = time(0) + i;
	i=i+12332;

    	gmp_randinit_default (r_state);
    	gmp_randseed_ui(r_state, seed);

    	mpz_init(rand_Num);

       	mpz_urandomb(rand_Num,r_state,bits);
	int res = mpz_cmp(rand_Num,offset);
	if(res<0) mpz_add(rand_Num,rand_Num,offset);
	gmp_randclear(r_state);
	

}

void get_rand_range(mpz_t rand, mpz_t first , mpz_t last){
	
	unsigned long int seed;

	gmp_randstate_t r_state;

    	seed = time(0) + i;
	i=i+12332;

    	gmp_randinit_default (r_state);
    	gmp_randseed_ui(r_state, seed);

    	mpz_init(rand);

	mpz_urandomm(rand, r_state, last);

	if(mpz_cmp(rand,first)<0) mpz_add(rand,rand,first);

}

bool get_miller_prime(mpz_t rand_num){

	mpz_t r,s,rem;
	mpz_init(r);
	mpz_init(s);
	mpz_init(rem);
	mpz_sub_ui(r,rand_num,1);
	do{
		mpz_cdiv_q_ui(r,r,2);
		mpz_add_ui(s,s,1);
		mpz_cdiv_r_ui(rem,r,2);	
	}
	while(mpz_cmp_ui(rem,0)==0);

	mpz_t a,randsub1,y,j;
	mpz_init(j);
	mpz_init(y);
	mpz_init(a);
	mpz_init(randsub1);
	mpz_t offset;
	mpz_init(offset);
	mpz_ui_pow_ui(offset , 2 , 1);
	mpz_sub_ui(randsub1,rand_num,1);
	for(int i=0;i<20;i++){
		get_rand_range(a, offset, randsub1);
		mpz_powm(y,a,r,rand_num);
		if(mpz_cmp_ui(y,1)!=0&&mpz_cmp(y,randsub1)!=0){
			mpz_set_ui(j,1);
			while(mpz_cmp(y,randsub1)!=0&&mpz_cmp(j,s)<0){
				mpz_powm(y,y,offset,rand_num);
				if(mpz_cmp_ui(y,1)==0) return false;
				mpz_add_ui(j,j,1);
			}
			if(mpz_cmp(y,randsub1)!=0) return false;		
		}
	}
	
	return true;
}

bool get_random_prime(mpz_t rand_Prime,unsigned long long int bits=256){

	int arr[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                     31, 37, 41, 43, 47, 53, 59, 61, 67,
                     71, 73, 79, 83, 89, 97, 101, 103,
                     107, 109, 113, 127, 131, 137, 139,
                     149, 151, 157, 163, 167, 173, 179,
                     181, 191, 193, 197, 199, 211, 223,
                     227, 229, 233, 239, 241, 251, 257,
                     263, 269, 271, 277, 281, 283, 293,
                     307, 311, 313, 317, 331, 337, 347, 349};
	
	int size = sizeof(arr)/sizeof(arr[0]);
	
	int i;
	mpz_t rem1,rem2;
	mpz_init(rem1);
	mpz_init(rem2);
	while(1){
		
		get_random_number(rand_Prime,bits);

		mpz_cdiv_r_ui(rem1,rand_Prime,2);

		if(mpz_cmp_ui(rem1,0)==0) continue;

		for(i=0;i<size;i++){
			mpz_cdiv_r_ui(rem2,rand_Prime,arr[i]);
			if(mpz_cmp_ui(rem2,0)==0) break;	
		}
	
		if(i==size) break;
		
	}
	
	return get_miller_prime(rand_Prime);
}

void get_strong_prime(mpz_t p){
	
	mpz_t s;
	mpz_init(s);
	while(get_random_prime(s)!=true);
	
	mpz_t t;
	mpz_init(t);
	while(get_random_prime(t)!=true);

	mpz_t i;
	mpz_init(i);

	mpz_t first;
	mpz_init(first);
	mpz_set_ui(first,2);
	
	mpz_t last;
	mpz_init(last);
	mpz_set_ui(last,100);

	get_rand_range(i,first,last);

	mpz_t temp;
	mpz_init(temp);

	mpz_t r;
	mpz_init(r);
	mpz_mul_ui(temp,t,2);
	do{
		mpz_mul(r,i,temp);
		mpz_add_ui(r,r,1);
		mpz_add_ui(i,i,1);
	}
	while(get_miller_prime(r)!=true);
	
	mpz_t p0;
	mpz_init(p0);
	
	mpz_t rsub2;
	mpz_init(rsub2);
	mpz_sub_ui(rsub2,r,2);

	mpz_powm(temp,s,rsub2,r);
	mpz_mul_ui(temp,temp,2);
	mpz_mul(temp,temp,s);
	mpz_sub_ui(p0,temp,1);

	mpz_t j;
	mpz_init(j);
	get_rand_range(j,first,last);

	mpz_mul_ui(temp,r,2);
	mpz_mul(temp,temp,s);
	mpz_init(p);
	do{
		mpz_mul(p,j,temp);
		mpz_add(p,p,p0);
		mpz_add_ui(j,j,1);
	}
	while(get_miller_prime(p)!=true);

}

void generate_key(mpz_t d,mpz_t n){
	mpz_t p,q;
	mpz_init(p);
	get_strong_prime(p);
	gmp_printf("p = %Zd\n\n",p);

	mpz_init(q);
	get_strong_prime(q);
	gmp_printf("q = %Zd\n\n",q);

	mpz_t n1;
	mpz_init(n1);
	mpz_mul(n1,p,q);

	mpz_t psub1,qsub1;
	mpz_init(psub1);
	mpz_init(qsub1);
	mpz_sub_ui(psub1,p,1);
	mpz_sub_ui(qsub1,q,1);

	mpz_t phi;
	mpz_init(phi);
	mpz_mul(phi,psub1,qsub1);

	mpz_t e;
	mpz_init(e);

	mpz_t first;
	mpz_init(first);
	mpz_set_ui(first,2);

	mpz_t gcd;
	mpz_init(gcd);

	do{
		get_rand_range(e,first,phi);
		mpz_gcd(gcd,e,phi);
	}
	while(mpz_cmp_ui(gcd,1)!=0);

	mpz_t d1;
	mpz_init(d1);
	mpz_invert(d1,e,phi);
		
	gmp_printf("d = %Zd\n\n",d1);
	gmp_printf("e = %Zd\n\n",e);
	gmp_printf("n = %Zd\n\n",n1);

	mpz_t enc_e;
	mpz_init(enc_e);
	mpz_powm(enc_e,e,d,n);
	
	mpz_t enc_n;
	mpz_init(enc_n);
	mpz_powm(enc_n,n1,d,n);

	FILE* fp;

	if(opened==0){
		fp = fopen("keys.txt","w");
		opened=1;	
	}
	else fp = fopen("keys.txt","a");

	gmp_fprintf(fp, "%Zd\n%Zd\n", enc_e, enc_n);
	
	fclose(fp);

	fp = fopen("input.txt","a");	

	gmp_fprintf(fp, "%Zd\n%Zd\n%Zd\n", p, q, d1);

	fclose(fp);
}

int main(){

//For keys of CA
cout << "Keys for CA : \n\n";
mpz_t p,q;

mpz_init(p);
get_strong_prime(p);
gmp_printf("p = %Zd\n\n",p);


mpz_init(q);
get_strong_prime(q);
gmp_printf("q = %Zd\n\n",q);

mpz_t n;
mpz_init(n);
mpz_mul(n,p,q);

mpz_t psub1,qsub1;
mpz_init(psub1);
mpz_init(qsub1);
mpz_sub_ui(psub1,p,1);
mpz_sub_ui(qsub1,q,1);

mpz_t phi;
mpz_init(phi);
mpz_mul(phi,psub1,qsub1);

mpz_t e;
mpz_init(e);

mpz_t first;
mpz_init(first);
mpz_set_ui(first,2);

mpz_t gcd;
mpz_init(gcd);

do{
	get_rand_range(e,first,phi);
	mpz_gcd(gcd,e,phi);
}
while(mpz_cmp_ui(gcd,1)!=0);

mpz_t d;
mpz_init(d);
mpz_invert(d,e,phi);

FILE* fp;

fp = fopen("input.txt","w");

gmp_fprintf(fp, "%Zd\n%Zd\n%Zd\n%Zd\n", p, q, e, n);

fclose(fp);

gmp_printf("n=%Zd\n\n phi = %Zd\n\n e= %Zd\n\n gcd=%Zd\n\n d=%Zd\n\n",n,phi,e,gcd,d);

//For keys of A
cout << "Keys for A : \n\n";
generate_key(d,n);

//For keys of B
cout << "Keys for B : \n\n";
generate_key(d,n);

return 0;
}
