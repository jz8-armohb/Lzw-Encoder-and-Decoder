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
		dictionary[i].suffix = i;  // ���ĺ�׺�ַ�Ϊ��ӦASCII��
		dictionary[i].parent = -1;  // ǰ׺�ַ�����Ϊ0��û��ǰ׺
		dictionary[i].firstChild = -1;  // ��ʱû�е�һ������
		dictionary[i].nextSibling = i + 1;	 /* ��һ���ֵܸ��ڵ��±�Ϊ��һ��ASCII��ֵ */
	}

	dictionary[255].nextSibling = -1;	// ���һ�����ڵ�û����һ���ֵ�
	nextNodeIdx = 256;	// The index of next dictionary entry
}

int InDict(int P, int C) {
	if (P == -1) {
		/* In this case, the current character is the start of the file,
		and it's evidently in the dictionary,
		thus return the corresponding ASCII code (let P = this character). */
		return C;
	}

	/* �������ұ���P�ڵ�����к��ӣ���һ�����ӵ������ֵܣ� */
	int sibling = dictionary[P].firstChild;	// ����P�ĵ�һ������Ϊ����
	while (sibling > -1) {	// sibling == -1 indicates the end of sibling traversal
		/* �����ҵ�һ���ֵܵĺ�׺��C���򷵻�PC�ı��루���ֵܵ��±꣩ */
		if (C == dictionary[sibling].suffix) {
			return sibling;
		}
		/* ������ֵܵĺ�׺���Ǹ��ַ�����Ѱ����һ�ֵ� */
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

	/* �����½ڵ���ֵܹ�ϵ */
	if (pFirstChild > -1) {	/* Parent of the new node originally have a child (children) */
		pChild = pFirstChild;	// Start from the first child of P

		/* Look for the youngest child of P (aka the last sibling) */
		while (dictionary[pChild].nextSibling > -1) {
			pChild = dictionary[pChild].nextSibling;
		}

		dictionary[pChild].nextSibling = nextNodeIdx;	// ���½ڵ���Ϊ���һ���ֵܵ���һ���ֵ�
	} else {	/* Parent of the new node originally doesn't have a child */
		dictionary[P].firstChild = nextNodeIdx;	// ���½ڵ�����ΪPC��P�ĵĵ�һ�����ӣ�
	}

	nextNodeIdx++;	// ��һ������������+1
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
//	unsigned long inFileSize = 9;	// �Ժ��Ϊ���ļ��ж�ȡ
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