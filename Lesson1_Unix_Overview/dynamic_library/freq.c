#include "freq.h"

bool Check(const char* a, char b, int j) {
	int i;
	for (i = 0; i < j; i++) {
		if (a[i] == b) return true;
	}
	return false;
}

char* Copy(const char* a) {
	int i;
	char* a_copy = (char*) calloc(strlen(a), sizeof(char));
	int j = 1;
	a_copy[0] = a[0];
	for (i = 1; i < strlen(a); i++) {
		if (a[i] == ' ') continue;
		if (Check(a, a[i], 1) == false)
			a_copy[j++] = a[i];
	}
	a_copy[j] = '\0';
	return a_copy;
}

int* Freq(const char* a) {
	int i, j;
	char* copy = Copy(a);
	int* freq = (int*) calloc(strlen(copy), sizeof(int));
	for (i = 0; i < strlen(copy); i++) {
		for (j = 0; j < strlen(a); j++) {
			if (a[j] == copy[i])
				freq[i]++;
		}
	}
	return freq;
}
