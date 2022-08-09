#include<iostream>
#include<gmp.h>
#include<time.h>
#include <fstream>
#include<bits/stdc++.h>
using namespace std;

void chinese_remainder(mpz_t x, mpz_t m, mpz_t power, mpz_t n, mpz_t p , mpz_t q){
	
	mpz_t a1;
	mpz_init(a1);
	mpz_t a2;
	mpz_init(a2);

	mpz_t temp;
	mpz_init(temp);

	mpz_init(x);

	mpz_powm(a1,m,power,p);
	mpz_powm(a2,m,power,q);

	mpz_mul(a1,a1,q);
	mpz_invert(temp,q,p);
	mpz_mul(a1,a1,temp);

	mpz_mul(a2,a2,p);
	mpz_invert(temp,p,q);
	mpz_mul(a2,a2,temp);

	mpz_add(a1,a1,a2);

	mpz_mod(x,a1,n);

}
	
int main(){
int m;
char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
FILE *fp,*fp2;	

fp = fopen("input.txt","r");

mpz_t pCA;
mpz_init(pCA);
mpz_t qCA;
mpz_init(qCA);
mpz_t eCA;
mpz_init(eCA);
mpz_t nCA;
mpz_init(nCA);
mpz_t pA;
mpz_init(pA);
mpz_t qA;
mpz_init(qA);
mpz_t dA;
mpz_init(dA);
mpz_t pB;
mpz_init(pB);
mpz_t qB;
mpz_init(qB);
mpz_t dB;
mpz_init(dB);

gmp_fscanf(fp,"%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n%Zd\n",pCA,qCA,eCA,nCA,pA,qA,dA,pB,qB,dB);
//gmp_printf("%Zd\n\n%Zd\n\n%Zd\n\n%Zd\n\n%Zd\n\n%Zd\n\n%Zd\n\n%Zd\n\n%Zd\n\n%Zd\n\n",pCA,qCA,eCA,nCA,pA,qA,dA,pB,qB,dB);

fclose(fp);

fp = fopen("keys.txt","r");

mpz_t enc_eA;
mpz_init(enc_eA);
mpz_t enc_nA;
mpz_init(enc_nA);

gmp_fscanf(fp,"%Zd\n%Zd\n",enc_eA, enc_nA);
//gmp_printf("enc_a = %Zd\nenc_n = %Zd\n",enc_eA,enc_nA);

fclose(fp);

mpz_t eA;
mpz_init(eA);

chinese_remainder(eA,enc_eA,eCA,nCA,pCA,qCA);
//mpz_powm(eB,enc_eB,eCA,nCA);
mpz_t nA;
mpz_init(nA);
//mpz_powm(nB,enc_nB,eCA,nCA);
chinese_remainder(nA,enc_nA,eCA,nCA,pCA,qCA);

//gmp_printf("ea = %Zd\n\nna = %Zd\n\n",eA,nA);


unsigned long int b=1;

mpz_t p;
mpz_init(p);
mpz_set_ui(p,26);
mpz_t temp;
mpz_init(temp);
while(1){
	if(mpz_cmp(p,nA)>=0) break;
	b++;
	mpz_init(p);
	mpz_set_ui(p,26);
	mpz_pow_ui(p,p,b);
}
b--;
//printf("\n\n%lu\n\n",b);

fp = fopen("blocks_cipher.txt","r");
unsigned long int max;
fscanf(fp,"%lu",&max);
fclose(fp);

//printf("\n%lu\n",max);

fp = fopen("encrypted.txt","r");
fp2 = fopen("decrypted_cipher.txt","w");
mpz_t sum;

long int j;

mpz_t enc_int;

int ch;
mpz_t temp1;
mpz_t temp2;

long int noc=0;
while((ch=fgetc(fp))!=EOF){
	if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
		noc++;printf("\n%ld %c\n",noc,ch);
	}
}
fclose(fp);
fp = fopen("encrypted.txt","r");
long int arr[b+1];
while(noc!=0){
	mpz_init(sum);
	mpz_init(enc_int);
	for(j=0;j<max+1;j++)
	{
		mpz_init(temp);
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,j);
		if((ch = fgetc(fp))==EOF) break;

		if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
			noc--;
			printf("\n%ld\n",noc);
			if(ch+0>=97&&ch+0<=122) mpz_mul_ui(temp,p,ch-97);
			if(ch+0>=65&&ch+0<=90) mpz_mul_ui(temp,p,ch-65);
			mpz_add(sum,sum,temp);	
			cout << j << endl;			
		}
	}

	mpz_init(enc_int);
	chinese_remainder(enc_int,sum,eA,nA,pA,qA);

	long int k=b+1;
		while(k!=0){
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,k-1);
			mpz_init(temp2);
			gmp_printf("\n\nenc = %Zd\n\n",enc_int);
			gmp_printf("\n\n%Zd\n\n",p);
			mpz_fdiv_q(temp2,enc_int,p);
			gmp_printf("\n%Zd\n",temp2);
			arr[b+1-k] = mpz_get_ui(temp2);
			printf("\n%ld\n",arr[b+1-k]);
			mpz_fdiv_r(enc_int,enc_int,p);
			k=k-1;
		}
	
	long int l=0;
	while(l!=b+1){ 
		//printf("\n\n%ld\n\n",arr[l]+65);
		fputc(arr[l]+65,fp2);
		//printf("%d",arr[l]+65);
		l=l+1;
	}

}

fclose(fp);
fclose(fp2);
unsigned long int max2;
mpz_init(p);
mpz_set_ui(p,26);
mpz_init(temp);
while(1){
	if(mpz_cmp(p,nA)>=0) break;
	b++;
	mpz_init(p);
	mpz_set_ui(p,26);
	mpz_pow_ui(p,p,b);
}
b--;
fp = fopen("blocks_key.txt","r");
fscanf(fp,"%lu",&max2);
fclose(fp);

fp = fopen("encrypted_key.txt","r");
int long noc2=0;
while((ch=fgetc(fp))!=EOF){
	if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
		noc2++;printf("\n%ld %c\n",noc2,ch);
	}
}
fclose(fp);

fp = fopen("encrypted_key.txt","r");
fp2 = fopen("decrypted_key.txt","w");

long int arr2[b+1];

while(noc2!=0){
	mpz_init(sum);
	mpz_init(enc_int);
	for(j=0;j<max2+1;j++)
	{
		mpz_init(temp);
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,j);
		if((ch = fgetc(fp))==EOF) break;
		if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
			noc2--;
			if(ch+0>=97&&ch+0<=122) mpz_mul_ui(temp,p,ch-97);
			if(ch+0>=65&&ch+0<=90) mpz_mul_ui(temp,p,ch-65);
			mpz_add(sum,sum,temp);	
			cout << j << endl;			
		}
	}

	mpz_init(enc_int);
	chinese_remainder(enc_int,sum,eA,nA,pA,qA);

	long int k=b+1;
		while(k!=0){
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,k-1);
			mpz_init(temp2);
			gmp_printf("\n\nenc = %Zd\n\n",enc_int);
			gmp_printf("\n\n%Zd\n\n",p);
			mpz_fdiv_q(temp2,enc_int,p);
			gmp_printf("\n%Zd\n",temp2);
			arr2[b+1-k] = mpz_get_ui(temp2);
			printf("\n%ld\n",arr2[b+1-k]);
			mpz_fdiv_r(enc_int,enc_int,p);
			k=k-1;
		}

	
	
	long int l=0;
	while(l!=b+1){ 
		//printf("\n\n%ld\n\n",arr[l]+65);
		fputc(arr2[l]+65,fp2);
		//printf("%d",arr[l]+65);
		l=l+1;
	}

}

fclose(fp);
fclose(fp2);
fp = fopen("symmetric_key.txt","r");
vector<int> key;
while((ch = fgetc(fp)) !=EOF){
	if(ch+0>=97&&ch+0<=122){
		key.push_back(ch-97);
	}
	if(ch+0>=65&&ch+0<=90){
		key.push_back(ch-65);
	}
}
m = key.size();
fclose(fp);


fp = fopen("decrypted.txt","w");
fp2 = fopen("cipher.txt","r");
int i=0;
while((ch = fgetc(fp2))!=EOF){
	ch = (ch - 97 - key[i%m]) % 26;
	if(ch<0) ch+=26;
	fputc(alphabet[ch],fp);
	i++;
}
fclose(fp);
fclose(fp2);
return 0;
}

