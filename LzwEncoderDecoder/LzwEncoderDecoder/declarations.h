#pragma once
#include "BitIO.h"
#define DICT_CAPACITY 65535   // Capacity of the dictionary

/* Global variables */
struct {
    int suffix;     //��׺�ַ�
    int parent, firstChild, nextSibling;
} dictionary[DICT_CAPACITY + 1];  //�����±꼴Ϊ����
extern int nextNodeIdx;    // ��һ�������������
extern int decStack[DICT_CAPACITY]; // Stack for decoding a phrase


/* Functions */
void LzwEncoding(FILE* inFilePtr, BITFILE* outBitFilePtr);
//void LzwDecoding(FILE* inFilePtr, FILE* outFilePtr);