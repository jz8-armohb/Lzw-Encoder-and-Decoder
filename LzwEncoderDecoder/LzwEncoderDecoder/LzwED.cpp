#include <iostream>
#include "declarations.h"
#include "BitIO.h"
using namespace std;

/* Global variables */
int nextNodeIdx;
int decStack[DICT_CAPACITY];

/* Macros */
#define Input(f) ((int)BitsInput(f, 16))
#define Output(f, x) BitsOutput(f, (unsigned long)(x), 16)

void InitialiseDict() {	// Dictionary initialisation (initialise root node 0-255)
	for (int i = 0; i < 256; i++) {
		dictionary[i].suffix = i;  // 根的后缀字符为对应ASCII码
		dictionary[i].parent = -1;  // 前缀字符长度为0，没有前缀
		dictionary[i].firstChild = -1;  // 暂时没有第一个孩子
		dictionary[i].nextSibling = i + 1;	 /* 下一个兄弟根节点下标为下一个ASCII码值 */
	}

	dictionary[255].nextSibling = -1;	// 最后一个根节点没有下一个兄弟
	nextNodeIdx = 256;	// The index of next dictionary entry
}

int InDict(int P, int C) {
	if (P == -1) {
		/* In this case, the current character is the start of the file,
		and it's evidently in the dictionary,
		thus return the corresponding ASCII code (let P = this character). */
		return C;
	}

	/* 自左向右遍历P节点的所有孩子（第一个孩子的所有兄弟） */
	int sibling = dictionary[P].firstChild;	// 先以P的第一个孩子为长兄
	while (sibling > -1) {	// sibling == -1 indicates the end of sibling traversal
		/* 若果找到一个兄弟的后缀是C，则返回PC的编码（该兄弟的下标） */
		if (C == dictionary[sibling].suffix) {
			return sibling;
		}
		/* 如果该兄弟的后缀不是该字符，则寻找下一兄弟 */
		sibling = dictionary[sibling].nextSibling;
	}

	/* The suffix of all siblings don't match PC, thus PC isn't in the dictionary */
	return -1;
}

void NewDictEntry(int P, int C) {
	if (P < 0) {
		return;
	}

	dictionary[nextNodeIdx].suffix = C;
	dictionary[nextNodeIdx].parent = P;
	dictionary[nextNodeIdx].nextSibling = -1;	// Indicates that the node is the last sibling
	dictionary[nextNodeIdx].firstChild = -1;	// Temporarily this node doesn't have a child
	int pFirstChild = dictionary[P].firstChild;	// The first child of P
	int pChild;

	/* 设置新节点的兄弟关系 */
	if (pFirstChild > -1) {	/* Parent of the new node originally have a child (children) */
		pChild = pFirstChild;	// Start from the first child of P

		/* Look for the youngest child of P (aka the last sibling) */
		while (dictionary[pChild].nextSibling > -1) {
			pChild = dictionary[pChild].nextSibling;
		}

		dictionary[pChild].nextSibling = nextNodeIdx;	// 把新节点设为最后一个兄弟的下一个兄弟
	} else {	/* Parent of the new node originally doesn't have a child */
		dictionary[P].firstChild = nextNodeIdx;	// 把新节点设置为PC（P的的第一个孩子）
	}

	nextNodeIdx++;	// 下一个词条索引号+1
}

void LzwEncoding(FILE* inFilePtr, BITFILE* outBitFilePtr) {
	int previousStr;	// P
	int currentChar;	// C
	int PC;	// P & C combined as 1 character

	/* Compute the size of file */
	fseek(inFilePtr, 0, SEEK_END);
	int inFileSize = ftell(inFilePtr);
	fseek(inFilePtr, 0, SEEK_SET);
	BitsOutput(outBitFilePtr, inFileSize, 4 * 8);

	/* Initialise the dictionary and P */
	InitialiseDict();
	previousStr = -1;	// Initialise P

	//fprintf(outFilePtr, "LZW-encoded string: ");
	while ( (currentChar = fgetc(inFilePtr)) != EOF ) {
		/* Check if PC is in the dictionary */
		PC = InDict(previousStr, currentChar);
		if (PC >= 0) {	/* PC is in the dictionary */
			previousStr = PC;	// Set P = PC
		} else {	/* PC isn't in the dictionary */
			Output(outBitFilePtr, previousStr);	// Output P
			if (nextNodeIdx < DICT_CAPACITY) {	/* Enough space to add PC into the dictionary */
				NewDictEntry(previousStr, currentChar);
			}
			previousStr = currentChar;	// Set P = C
		}
	}

	Output(outBitFilePtr, previousStr);	// Output the last unencoded character(s)
}

//void LzwDecoding(FILE* inFilePtr, FILE* outFilePtr) {
//	int character;
//	int newCode, lastCode;
//	int phraseLen;
//
//	unsigned long inFileSize = 9;	// 稍后改为从文件中读取
//	if (inFileSize == -1) {
//		inFileSize = 0;
//	}
//
//	/* Initialisation */
//	InitialiseDict();
//	lastCode = -1;	// Initialise pW
//
//	while (inFileSize > 0) {
//
//	}
//}