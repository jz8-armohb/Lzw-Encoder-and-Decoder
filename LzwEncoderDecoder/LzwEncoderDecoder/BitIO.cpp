/* Definitions for bitwise IO */

#include <iostream>
#include <stdlib.h>
#include "BitIO.h"

BITFILE* OpenBitFileInput(char* fileName) {
	//BITFILE* bf = (BITFILE*)malloc(sizeof(BITFILE));
	BITFILE* bf = new BITFILE;
	if (bf == NULL) {
		return NULL;
	}
	if (fileName == NULL) {
		bf->fp = stdin;
	} else {
		fopen_s(&bf->fp, fileName, "rb");
	}
	if (bf->fp == NULL) {
		return NULL;
	}
	bf->mask = 0x80;
	bf->rack = 0;
	return bf;
}

BITFILE* OpenBitFileOutput(char* fileName) {
	//BITFILE* bf = (BITFILE*)malloc(sizeof(BITFILE));
	BITFILE* bf = new BITFILE;
	if (bf == NULL) {
		return NULL;
	}
	if (fileName == NULL) {
		bf->fp = stdout;
	} else {
		fopen_s(&bf->fp, fileName, "wb");
	}
	if (bf->fp == NULL) {
		return NULL;
	}
	bf->mask = 0x80;
	bf->rack = 0;
	return bf;
}

void CloseBitFileInput(BITFILE* bf) {
	fclose(bf->fp);
	//free(bf);
	delete bf;
}

void CloseBitFileOutput(BITFILE* bf) {
	/* Output the remaining bits */
	if (bf->mask != 0x80) {
		fputc(bf->rack, bf->fp);
	}
	fclose(bf->fp);
	//free(bf);
	delete bf;
}

int BitInput(BITFILE* bf) {
	int value;

	if (bf->mask == 0x80) {
		bf->rack = fgetc(bf->fp);
		if (bf->rack == EOF) {
			fprintf(stderr, "Reached the end of file.\n");
			exit(-1);
		}
	}
	value = bf->mask & bf->rack;
	bf->mask >>= 1;
	if (0 == bf->mask) {
		bf->mask = 0x80;
	}
	return((value == 0) ? 0 : 1);
}

unsigned long BitsInput(BITFILE* bf, int count) {
	unsigned long mask;
	unsigned long value;
	mask = 1L << (count - 1);
	value = 0L;
	while (mask != 0) {
		if (BitInput(bf) == 1)
			value |= mask;
		mask >>= 1;
	}
	return value;
}

void BitOutput(BITFILE* bf, int bit) {
	if (bit != 0) {
		bf->rack |= bf->mask;
	}
	bf->mask >>= 1;
	if (bf->mask == 0) {	// 8 bits in rack
		fputc(bf->rack, bf->fp);
		bf->rack = 0;
		bf->mask = 0x80;
	}
}

void BitsOutput(BITFILE* bf, unsigned long code, int count) {
	unsigned long mask;

	mask = 1L << (count - 1);
	while (mask != 0) {
		BitOutput(bf, (int)((code & mask) == 0 ? 0 : 1));
		mask >>= 1;
	}
}