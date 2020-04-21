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
		dictionary[i].suffix = i;  // The suffix of each node is the corresponding ASCII code
		dictionary[i].parent = -1;  // Temporarily doesn't have a parent node (i.e. prefix)
		dictionary[i].firstChild = -1;  // Temporarily doesn't have any child nodes
		dictionary[i].nextSibling = i + 1;	// The index of the next sibling root node is the next ASCII code
	}

	dictionary[255].nextSibling = -1;	// No next sibling for the last root node
	nextNodeIdx = 256;	// The index of next dictionary entry
}

int InDict(int P, int C) {
	if (P == -1) {
		/* In this case, the current character is the start of the file,
		and it's evidently in the dictionary,
		thus return the corresponding ASCII code (let P = this character). */
		return C;
	}

	/* Traverse all child node(s) of node P from left to right (i.e. all sibling nodes of the first child node) */
	int sibling = dictionary[P].firstChild;	// Start from the first child of P
	while (sibling > -1) {	// sibling == -1 indicates the end of sibling traversal
		/* If a C-suffixed sibling is found, then return the code of PC (i.e. the index of this sibling) */
		if (C == dictionary[sibling].suffix) {
			return sibling;
		}
		/* If the suffixes don't match, then look for the next */
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

	/* Set up the new sibling-relation */
	if (pFirstChild > -1) {	/* Parent of the new node originally have a child node */
		pChild = pFirstChild;	// Start from the first child of P

		/* Look for the youngest child of P (aka the last sibling) */
		while (dictionary[pChild].nextSibling > -1) {
			pChild = dictionary[pChild].nextSibling;
		}

		dictionary[pChild].nextSibling = nextNodeIdx;	// Set the new node as the next sibling of the current last sibling
	} else {	/* Parent of the new node originally doesn't have a child */
		dictionary[P].firstChild = nextNodeIdx;	// Set the new node as PC (i.e. the first child of P)
	}

	nextNodeIdx++;	//Index of the next entry + 1
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

int DecodeString(int start, int code) {
	int count = start;
	while (code >= 0) {
		/* Look for the root node */
		decStack[count] = dictionary[code].suffix;	// Store the original string in inverted order
		code = dictionary[code].parent;	// Set the parent of the current node as the next node
		count++;	// Points to the next node
	}
	return count;	// The distance between the current node and the root
}

void LzwDecoding(BITFILE* inBitFilePtr, FILE* outFilePtr) {
	int character;
	int previousCode;	// pW
	int currentCode;	// cW
	int phraseLen;	// Length of phrase

	unsigned long inFileSize = BitsInput(inBitFilePtr, 4 * 8);
	if (inFileSize == -1) {
		inFileSize = 0;
	}

	/* Initialise dictionary and pW*/
	InitialiseDict();
	previousCode = -1;

	while (inFileSize > 0) {
		currentCode = Input(inBitFilePtr);
		if (currentCode < nextNodeIdx) {	/* cW is in dictionary */
			phraseLen = DecodeString(0, currentCode);	// The length of cW
		} else {	/* When cW ¡Ý next node index, which means cW > current node index, cW isn't in dictionary */
			decStack[0] = character;	// The last character in stack of the last loop, i.e. 1st character of pW
			phraseLen = DecodeString(1, previousCode);	// The length of pW + 1
		}
		character = decStack[phraseLen - 1];	// The last character in the stack, i.e. the 1st character of pW or cW

		while (phraseLen > 0) {
			phraseLen--;
			fputc(decStack[phraseLen], outFilePtr);	// Output the decoded string (in inverted order of decStack)
			inFileSize--;
		}
		if (nextNodeIdx < DICT_CAPACITY) {	/* Add the new phrase into dictionary */
			NewDictEntry(previousCode, character);	// Add "pW + 1st character of cW" or "pW + 1st character of pW" into dictionary
		}
		previousCode = currentCode;	// Set pW = cW
	}
}