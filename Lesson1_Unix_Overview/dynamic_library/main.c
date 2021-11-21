#include <stdio.h>
#include "freq.h"

int main()
{
	int i;
	char* a = (char*) calloc(100, sizeof(char));
	printf("Nhap chuoi: ");
	fgets(a, 100, stdin);
	char* copy = Copy(a);
	int* freq = (int*) calloc(strlen(copy), sizeof(int));
        freq = Freq(a);
	printf("Tan suat: \n");
	for(i = 0; i < strlen(copy); i++)
	{
		printf("%c: %d \n", copy[i], freq[i]);
	}
	return 0;
}
