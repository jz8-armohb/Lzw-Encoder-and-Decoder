#pragma once
#define DICT_CAPACITY 65535   // Capacity of the dictionary

/* Global variables */
struct {
    int suffix;     //��׺�ַ�
    int parent, firstChild, nextSibling;
} dictionary[DICT_CAPACITY + 1];  //�����±꼴Ϊ����
extern int nextNode;    // ��һ�������������


/* Functions */
void LzwEncoding(FILE* inFilePtr, FILE* outFilePtr);