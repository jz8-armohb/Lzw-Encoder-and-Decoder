#pragma once
#include "BitIO.h"
#define DICT_CAPACITY 65535   // Capacity of the dictionary
/* Global variables */
struct {
    int suffix;
    int parent;
    int firstChild;
    int nextSibling;
} dictionary[DICT_CAPACITY + 1];
extern int nextNodeIdx;    // Index of next node (i.e. next dictionary entry)
extern int decStack[DICT_CAPACITY]; // Stack for decoding a phrase

/* Functions */
void LzwEncoding(FILE* inFilePtr, BITFILE* outBitFilePtr);
void LzwDecoding(BITFILE* inBitFilePtr, FILE* outFilePtr);