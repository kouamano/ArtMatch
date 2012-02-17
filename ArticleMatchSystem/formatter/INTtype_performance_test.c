#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef union{
        unsigned int IN;
        char CH[4];
}Fgram;

typedef union{
	unsigned long long int IN;
	char CH[8];
}Egram;

Fgram fgram;
Egram egram;
unsigned int FIN;
unsigned long long int EIN;

int main(){
	int i = 0;
	int j = 0;
	int k = 0;
	char c;
	fgram.CH[0] = 'a';
	fgram.CH[1] = 'b';
	fgram.CH[2] = 'c';
	fgram.CH[3] = 'd';
	egram.CH[0] = 'a';
	egram.CH[1] = 'b';
	egram.CH[2] = 'c';
	egram.CH[3] = 'd';
	egram.CH[4] = 'e';
	egram.CH[5] = 'f';
	egram.CH[6] = 'g';
	egram.CH[7] = 'h';
	FIN = 12;
	EIN = 12;
	c = 'z';
	for(j=0;j<800000000;j++){
		if(FIN == fgram.IN){
			i++;
		}else{
			i--;
		}
	}
	printf("size:%d:\n",sizeof(FIN));
	printf("%d\n",i);
	return(0);
}
