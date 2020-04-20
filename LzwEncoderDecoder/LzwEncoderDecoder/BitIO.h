#pragma once
#include <iostream>

typedef struct {
	FILE* fp;
	unsigned char mask;
	int rack;
}BITFILE;

BITFILE* OpenBitFileInput(char* fileName);
BITFILE* OpenBitFileOutput(char* fileName);
void CloseBitFileInput(BITFILE* bf);
void CloseBitFileOutput(BITFILE* bf);
int BitInput(BITFILE* bf);
unsigned long BitsInput(BITFILE* bf, int count);
void BitOutput(BITFILE* bf, int bit);
void BitsOutput(BITFILE* bf, unsigned long code, int count);