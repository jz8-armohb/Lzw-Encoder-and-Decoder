#pragma once
#include "BitIO.h"
#define DICT_CAPACITY 65535   // Capacity of the dictionary

/* Global variables */
struct {
    int suffix;     //后缀字符
    int parent, firstChild, nextSibling;
} dictionary[DICT_CAPACITY + 1];  //数组下标即为编码
extern int nextNodeIdx;    // 下一个编码的索引号
extern int decStack[DICT_CAPACITY]; // Stack for decoding a phrase


/* Functions */
void LzwEncoding(FILE* inFilePtr, BITFILE* outBitFilePtr);
//void LzwDecoding(FILE* inFilePtr, FILE* outFilePtr);