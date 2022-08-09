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
fp = fopen("symmetric_key.txt","r");
int ch;
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
int ech;
fp = fopen("plain.txt","r");
fp2 = fopen("cipher.txt","w");

unsigned long int i=0;
while((ch = fgetc(fp)) !=EOF){
	if(ch+0>=97&&ch+0<=122){
		ech = (ch - 97 + key[i%m]) % 26;
		fputc(alphabet[ech],fp2);
		i++;
	}
	if(ch+0>=65&&ch+0<=90){
		ech = (ch - 65 + key[i%m]) % 26;
		fputc(alphabet[ech],fp2);
		i++;
	}
}

fclose(fp);
fclose(fp2);
/*
fp = fopen("decrypted.txt","w");
fp2 = fopen("cipher.txt","r");
i=0;
while((ch = fgetc(fp2))!=EOF){
	ch = (ch - 97 - key[i%m]) % 26;
	if(ch<0) ch+=26;
	fputc(alphabet[ch],fp);
	i++;
}
fclose(fp);
fclose(fp2);*/

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

mpz_t nA;
mpz_init(nA);
mpz_mul(nA,pA,qA);
//gmp_printf("%Zd\n",nA);

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
printf("\n\n%lu\n\n",b);

fp = fopen("cipher.txt","r");
fp2 = fopen("encrypted.txt","w");

mpz_t sum;

long int j;

mpz_t enc_int;

mpz_t temp1;
mpz_t temp2;

int ch2;
unsigned long int max=b;
long int noc=0;
ch2 = fgetc(fp);
while(ch2!=EOF){
	if((ch2+0>=97&&ch2+0<=122)||(ch2+0>=65&&ch2+0<=90)){
		noc++;
	}
	ch2 = fgetc(fp);
}
printf("\n%ld %c\n",noc,ch2);

fclose(fp);

fp = fopen("cipher.txt","r");
while(noc!=0){
	mpz_init(sum);
	mpz_init(enc_int);
	j=b+1;
	do{
		mpz_init(temp);
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,j-1);
		if((ch=fgetc(fp))==EOF) break;
		if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
			noc--;
			j--;
			if(ch+0>=97&&ch+0<=122) mpz_mul_ui(temp,p,ch-97);
			if(ch+0>=65&&ch+0<=90) mpz_mul_ui(temp,p,ch-65);
			mpz_add(sum,sum,temp);
			cout << j << endl;
		}			
	}
	while(j>0);
	if(ch!=EOF){
		printf("HEY");
		mpz_init(enc_int);
		chinese_remainder(enc_int,sum,dA,nA,pA,qA);

		unsigned long int bs=1;
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_t temp;
		mpz_init(temp);
		while(1){
			if(mpz_cmp(p,enc_int)>=0) break;
			bs++;
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,bs);
		}
		bs--;

		if(bs>max) max=bs;
	}
}
//printf("\n%lu\n",max);
//printf("\n\n%ld\n\n",j);
if(j!=0){
	printf("Hi");
	while(j!=0){
			mpz_mul_ui(temp,p,23);
			mpz_add(sum,sum,temp);
			mpz_init(temp);
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,j-1);
			j--;
		}
	mpz_init(enc_int);
	chinese_remainder(enc_int,sum,dA,nA,pA,qA);
	//gmp_printf("\nenc_int = %Zd",enc_int);

	unsigned long int bs=1;
	mpz_init(p);
	mpz_set_ui(p,26);
	mpz_t temp;
	mpz_init(temp);
	while(1){
		if(mpz_cmp(p,enc_int)>=0) break;
		bs++;
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,bs);
	}
	bs--;

	if(bs>max) max=bs;
}
printf("%lu\n\n",max);
long int arr[max+1];
fclose(fp);
fp = fopen("cipher.txt","r");
noc=0;
while((ch=fgetc(fp))!=EOF){
	if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
		noc++;
	}
}
//printf("\n%ld\n",noc);
fclose(fp);
fp = fopen("cipher.txt","r");
while(noc!=0){
	mpz_init(sum);
	mpz_init(enc_int);
	j=b+1;
	do{
		mpz_init(temp);
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,j-1);
		if((ch=fgetc(fp))==EOF) break;
		if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
			noc--;
			j--;
			if(ch+0>=97&&ch+0<=122) mpz_mul_ui(temp,p,ch-97);
			if(ch+0>=65&&ch+0<=90) mpz_mul_ui(temp,p,ch-65);
			mpz_add(sum,sum,temp);
			cout << j << endl;	
		}		
	}
	while(j>0);
	if(ch!=EOF){
		printf("Helllo");
		mpz_init(enc_int);
		chinese_remainder(enc_int,sum,dA,nA,pA,qA);
		long int k=max+1;
	
		while(k!=0){
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,k-1);
			mpz_init(temp2);
			gmp_printf("\n\nenc = %Zd\n\n",enc_int);
			gmp_printf("\n\n%Zd\n\n",p);
			mpz_fdiv_q(temp2,enc_int,p);
			gmp_printf("\n%Zd\n",temp2);
			arr[k-1] = mpz_get_ui(temp2);
			printf("\n%ld\n",arr[k-1]);
			mpz_fdiv_r(enc_int,enc_int,p);
			k=k-1;
		}
	
		long int l=0;
		while(l!=max+1){ 
			//printf("\n\n%ld\n\n",arr[l]+65);
			fputc(arr[l]+65,fp2);
			//printf("%d",arr[l]+65);
			l=l+1;
		}
		
	}
}

if(j!=0){
	printf("God");
	while(j!=0){
			mpz_mul_ui(temp,p,23);
			mpz_add(sum,sum,temp);
			mpz_init(temp);
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,j-1);
			j--;
		}
	mpz_init(enc_int);
	chinese_remainder(enc_int,sum,dA,nA,pA,qA);
	long int k=max+1;
	
	while(k!=0){
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,k-1);
			mpz_init(temp2);
			gmp_printf("\n\nenc = %Zd\n\n",enc_int);
			gmp_printf("\n\n%Zd\n\n",p);
			mpz_fdiv_q(temp2,enc_int,p);
			gmp_printf("\n%Zd\n",temp2);
			arr[k-1] = mpz_get_ui(temp2);
			printf("\n%ld\n",arr[k-1]);
			mpz_fdiv_r(enc_int,enc_int,p);
			k=k-1;
		}

	long int l=0;
	while(l!=max+1){ 
		//printf("\n\n%ld\n\n",arr[l]+65);
		fputc(arr[l]+65,fp2);
		//printf("%d",arr[l]+65);
		l=l+1;
	}
}
gmp_printf("\n\nsum = %Zd\n\nenc_int = %Zd\n\n",sum,enc_int);
fclose(fp);
fclose(fp2);

fp = fopen("blocks_cipher.txt","w");
fprintf(fp,"%lu\n",max);
fclose(fp);

fp = fopen("symmetric_key.txt","r");
fp2 = fopen("encrypted_key.txt","w");

mpz_init(p);
mpz_set_ui(p,26);
mpz_init(temp);
b=1;
while(1){
	if(mpz_cmp(p,nA)>=0) break;
	b++;
	mpz_init(p);
	mpz_set_ui(p,26);
	mpz_pow_ui(p,p,b);
}
b--;
printf("b is %lu",b);
unsigned long int max2=b;
long int noc2=0;

while(ch2!=EOF){
	if((ch2+0>=97&&ch2+0<=122)||(ch2+0>=65&&ch2+0<=90)){
		noc2++;printf("\n%ld %c\n",noc2,ch2);
	}
	ch2 = fgetc(fp);
}
printf("noc2 is %ld",noc2);
fclose(fp);
fp = fopen("symmetric_key.txt","r");
while(noc2!=0){
	mpz_init(sum);
	mpz_init(enc_int);
	j=b+1;
	do{
		mpz_init(temp);
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,j-1);
		if((ch=fgetc(fp))==EOF) break;
		if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
			noc2--;
			j--;
			if(ch+0>=97&&ch+0<=122) mpz_mul_ui(temp,p,ch-97);
			if(ch+0>=65&&ch+0<=90) mpz_mul_ui(temp,p,ch-65);
			mpz_add(sum,sum,temp);
			cout << j << endl;
		}			
	}
	while(j>0);
	if(ch!=EOF){
		printf("Not here");
		mpz_init(enc_int);
		chinese_remainder(enc_int,sum,dA,nA,pA,qA);

		unsigned long int bs=1;
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_t temp;
		mpz_init(temp);
		while(1){
			if(mpz_cmp(p,enc_int)>=0) break;
			bs++;
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,bs);
		}
		bs--;

		if(bs>max2) max2=bs;
	}
}

if(j!=0){
	printf("But here");
	while(j!=0){
			mpz_mul_ui(temp,p,23);
			mpz_add(sum,sum,temp);
			mpz_init(temp);
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,j-1);
			j--;
		}
	mpz_init(enc_int);
	chinese_remainder(enc_int,sum,dA,nA,pA,qA);


	unsigned long int bs=1;
	mpz_init(p);
	mpz_set_ui(p,26);
	mpz_t temp;
	mpz_init(temp);
	while(1){
		if(mpz_cmp(p,enc_int)>=0) break;
		bs++;
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,bs);
	}
	bs--;

	if(bs>max2) max2=bs;
}
printf("\n%lu\n",max2);
fclose(fp);
fp = fopen("symmetric_key.txt","r");
noc2=0;
while((ch=fgetc(fp))!=EOF){ 
	if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
		noc2++;printf("\n%ld %c\n",noc2,ch);
	}
}
fclose(fp);
fp = fopen("symmetric_key.txt","r");

long int arr2[max2+1];

while(noc2!=0){
	mpz_init(sum);
	mpz_init(enc_int);
	j=b+1;
	do{
		mpz_init(temp);
		mpz_init(p);
		mpz_set_ui(p,26);
		mpz_pow_ui(p,p,j-1);
		if((ch=fgetc(fp))==EOF) break;
		if((ch+0>=97&&ch+0<=122)||(ch+0>=65&&ch+0<=90)){
			noc2--;
			j--;
			if(ch+0>=97&&ch+0<=122) mpz_mul_ui(temp,p,ch-97);
			if(ch+0>=65&&ch+0<=90) mpz_mul_ui(temp,p,ch-65);
			mpz_add(sum,sum,temp);
			cout << j << endl;			
		}
	}
	while(j>0);
	if(ch!=EOF){
		printf("Not here");
		mpz_init(enc_int);
		chinese_remainder(enc_int,sum,dA,nA,pA,qA);
	
		long int k=max2+1;
		
		while(k!=0){
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,k-1);
			mpz_init(temp2);
			gmp_printf("\n\nenc = %Zd\n\n",enc_int);
			gmp_printf("\n\n%Zd\n\n",p);
			mpz_fdiv_q(temp2,enc_int,p);
			gmp_printf("\n%Zd\n",temp2);
			arr2[k-1] = mpz_get_ui(temp2);
			printf("\n%ld\n",arr2[k-1]);
			mpz_fdiv_r(enc_int,enc_int,p);
			k=k-1;
		}
		
		long int l=0;
		while(l!=max2+1){ 
			//printf("\n\n%ld\n\n",arr[l]+65);
			fputc(arr2[l]+65,fp2);
			//printf("%d",arr[l]+65);
			l=l+1;
		}
	}

}

if(j!=0){
	printf("But here");
	while(j!=0){
			mpz_mul_ui(temp,p,23);
			mpz_add(sum,sum,temp);
			mpz_init(temp);
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,j-1);
			j--;
		}
	mpz_init(enc_int);
	chinese_remainder(enc_int,sum,dA,nA,pA,qA);
	long int k=max2+1;
	
	while(k!=0){
			mpz_init(p);
			mpz_set_ui(p,26);
			mpz_pow_ui(p,p,k-1);
			mpz_init(temp2);
			gmp_printf("\n\nenc = %Zd\n\n",enc_int);
			gmp_printf("\n\n%Zd\n\n",p);
			mpz_fdiv_q(temp2,enc_int,p);
			gmp_printf("\n%Zd\n",temp2);
			arr2[k-1] = mpz_get_ui(temp2);
			printf("\n%ld\n",arr2[k-1]);
			mpz_fdiv_r(enc_int,enc_int,p);
			k=k-1;
		}
	
	long int l=0;
	while(l!=max2+1){ 
		//printf("\n\n%ld\n\n",arr[l]+65);
		fputc(arr2[l]+65,fp2);
		//printf("%d",arr[l]+65);
		l=l+1;
	}
}

gmp_printf("\n\nFor key : \nsum = %Zd\n\nenc_int = %Zd\n\n",sum,enc_int);
fclose(fp);
fclose(fp2);

fp = fopen("blocks_key.txt","w");
fprintf(fp,"%lu\n",max2);
fclose(fp);

return 0;
}
