#include <iostream>
#include "declarations.h"
using namespace std;

void InitialiseDict() {	// Dictionary initialisation (initialise root node 0-255)
	for (int i = 0; i < 256; i++) {
		dictionary[i].suffix = i;  // 根的后缀字符为对应ASCII码
		dictionary[i].parent = -1;  // 前缀字符长度为0，没有前缀
		dictionary[i].firstChild = -1;  // 暂时没有第一个孩子
		dictionary[i].nextSibling = i + 1;	 /* 下一个兄弟根节点下标为下一个ASCII码值 */
	}

	dictionary[255].nextSibling = -1;	// 最后一个根节点没有下一个兄弟
	nextNode = 256;	// The index of next dictionary entry
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

	/* 设置新节点的兄弟关系 */
	if (pFirstChild > -1) {	/* Parent of the new node originally have a child (children) */
		pChild = pFirstChild;	// Start from the first child of P

		/* Look for the youngest child of P (aka the last sibling) */
		while (dictionary[pChild].nextSibling > -1) {
			pChild = dictionary[pChild].nextSibling;
		}

		dictionary[pChild].nextSibling = nextNode;	// 把新节点设为最后一个兄弟的下一个兄弟
	} else {	/* Parent of the new node originally doesn't have a child */
		dictionary[P].firstChild = nextNode;	// 把新节点设置为PC（P的的第一个孩子）
	}

	nextNode++;	// 下一个词条索引号+1
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