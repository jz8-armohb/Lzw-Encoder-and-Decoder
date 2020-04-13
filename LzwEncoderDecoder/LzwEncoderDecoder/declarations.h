#pragma once
#define DICT_CAPACITY 512   // Capacity of the dictionary

/* Global variables */
struct
{
    int suffix;     //后缀字符
    int parent, firstChild, nextSibling;
} dictionary[DICT_CAPACITY + 1];  //数组下标即为编码
extern int nextNode;    // 下一个编码的索引号


/* Functions */
void LzwEncoding(FILE* inFilePtr, FILE* outFilePtr);