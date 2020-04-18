#include <iostream>
#include "declarations.h"
using namespace std;

void InitialiseDict() {	// Dictionary initialisation (initialise root node 0-255)
	for (int i = 0; i < 256; i++) {
		dictionary[i].suffix = i;  // ���ĺ�׺�ַ�Ϊ��ӦASCII��
		dictionary[i].parent = -1;  // ǰ׺�ַ�����Ϊ0��û��ǰ׺
		dictionary[i].firstChild = -1;  // ��ʱû�е�һ������
		dictionary[i].nextSibling = i + 1;	 /* ��һ���ֵܸ��ڵ��±�Ϊ��һ��ASCII��ֵ */
	}

	dictionary[255].nextSibling = -1;	// ���һ�����ڵ�û����һ���ֵ�
	nextNode = 256;	// The index of next dictionary entry
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

void AddIntoDict(int P, int C) {
	if (P < 0) {
		return;
	}

	dictionary[nextNode].suffix = C;
	dictionary[nextNode].parent = P;
	dictionary[nextNode].nextSibling = -1;	// Indicates that the node is the last sibling
	dictionary[nextNode].firstChild = -1;	// Temporarily this node doesn't have a child
	int pFirstChild = dictionary[P].firstChild;	// The first child of P
	int pChild;

	/* �����½ڵ���ֵܹ�ϵ */
	if (pFirstChild > -1) {	/* Parent of the new node originally have a child (children) */
		pChild = pFirstChild;	// Start from the first child of P

		/* Look for the youngest child of P (aka the last sibling) */
		while (dictionary[pChild].nextSibling > -1) {
			pChild = dictionary[pChild].nextSibling;
		}

		dictionary[pChild].nextSibling = nextNode;	// ���½ڵ���Ϊ���һ���ֵܵ���һ���ֵ�
	} else {	/* Parent of the new node originally doesn't have a child */
		dictionary[P].firstChild = nextNode;	// ���½ڵ�����ΪPC��P�ĵĵ�һ�����ӣ�
	}

	nextNode++;	// ��һ������������+1
}

void LzwEncoding(FILE* inFilePtr, FILE* outFilePtr)
{
	int previousStr;	// P
	int currentChar;	// C
	int PC;	// P & C combined as 1 character

	/* Compute the size of file */
	//fseek(inFilePtr, 0, SEEK_END);
	//int inFileSize = ftell(inFilePtr);
	//fseek(inFilePtr, 0, SEEK_SET);

	/* Initialise the dictionary and P */
	InitialiseDict();
	previousStr = -1;	// Initialise P

	while ( (currentChar = fgetc(inFilePtr)) != EOF ) {
		/* Check if PC is in the dictionary */
		PC = InDict(previousStr, currentChar);
		if (PC >= 0) {	/* PC is in the dictionary */
			previousStr = PC;	// Set P = PC
		} else {	/* PC isn't in the dictionary */
			fprintf(outFilePtr, "%d ", previousStr);	// Output P
			if (nextNode < DICT_CAPACITY) {	/* Enough space to add PC into the dictionary */
				AddIntoDict(previousStr, currentChar);
			}
			previousStr = currentChar;	// Set P = C
		}
	}

	fprintf(outFilePtr, "%d\n", previousStr);	// Output the last unencoded character(s)
}