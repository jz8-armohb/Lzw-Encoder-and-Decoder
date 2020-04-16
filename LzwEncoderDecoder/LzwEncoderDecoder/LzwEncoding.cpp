#include <iostream>
#include "declarations.h"
using namespace std;

void InitialiseDict()	// Dictionary initialisation (initialise root node 0-255)
{
	for (int i = 0; i < 256; i++)
	{
		dictionary[i].suffix = i;  // ���ĺ�׺�ַ�Ϊ��ӦASCII��
		dictionary[i].parent = -1;  // ǰ׺�ַ�����Ϊ0��û��ǰ׺
		dictionary[i].firstChild = -1;  // ��ʱû�е�һ������
		dictionary[i].nextSibling = i + 1;	 /* ��һ���ֵܸ��ڵ��±�Ϊ��һ��ASCII��ֵ */
	}

	dictionary[255].nextSibling = -1;	// ���һ�����ڵ�û����һ���ֵ�
	nextNode = 256;	// ��һ������������Ϊ256
}

int InDict(int previous, int current)
{
	int sibling;

	if (previous == -1)
	{
		/* This current character is the start of file, 
		which means its root node must be in the dictionary, 
		so return the ASCII code of character. */
		return current;
	}

	sibling = dictionary[previous].firstChild;
	while (sibling != -1)
	{
		if (current == dictionary[sibling].suffix)
		{
			return sibling;
		}
	}

	return -1;
}

void LzwEncoding(FILE* inFilePtr, FILE* outFilePtr)
{
	int currentChar;	// C
	int previousStr;	// P
	int index;

	/* Compute the size of file */
	fseek(inFilePtr, 0, SEEK_END);
	int inFileSize = ftell(inFilePtr);
	fseek(inFilePtr, 0, SEEK_SET);

	/* Initialise the dictionary and prefix */
	InitialiseDict();
	previousStr = -1;	// Initialise the prefix

	while ( (currentChar = fgetc(inFilePtr)) != EOF )
	{
		index = InDict(previousStr, currentChar);
		if (index >= 0)
		{
			/* PC is in the dictionary */
			previousStr = index;	// ��PC��Ӧ�ı�����Ϊǰ׺
		}
		else
		{
			/* PC isn't in the dictionary */
			fprintf(outFilePtr, "%s", previousStr);
			if (nextNode < DICT_CAPACITY)
			{
				/* Enough space to add into the dictionary */

			}
		}
	}
}