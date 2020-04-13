#include <iostream>
#include "declarations.h"
using namespace std;

void InitialiseDict()	// Dictionary initialisation (initialise root node 0-255)
{
	for (int i = 0; i < 256; i++)
	{
		dictionary[i].suffix = i;  // 根的后缀字符为对应ASCII码
		dictionary[i].parent = -1;  // 前缀字符长度为0，没有前缀
		dictionary[i].firstChild = -1;  // 暂时没有第一个孩子
		dictionary[i].nextSibling = i + 1;	 /* 下一个兄弟根节点下标为下一个ASCII码值 */
	}

	dictionary[255].nextSibling = -1;	// 最后一个根节点没有下一个兄弟
	nextNode = 256;	// 下一个编码索引号为256
}

int InDict(int character, int stringCode)
{
	int sibling;

	if (stringCode == -1)
	{
		/* This character is the start of file, which means its root node must be in the dictionary, so return the ASCII code of character. */
		return character;
	}

	sibling = dictionary[stringCode].firstChild;
	while (sibling != -1)
	{
		if (character == dictionary[sibling].suffix)
		{
			return sibling;
		}
	}

	return -1;
}

void LzwEncoding(FILE* inFilePtr, FILE* outFilePtr)
{
	int character;
	int stringCode;
	int index;

	/* Compute the size of file */
	fseek(inFilePtr, 0, SEEK_END);
	int inFileSize = ftell(inFilePtr);
	fseek(inFilePtr, 0, SEEK_SET);

	/* Initialise the dictionary and prefix */
	InitialiseDict();
	stringCode = -1;

	while ( (character = fgetc(inFilePtr)) != EOF )
	{
		index = InDict(character, stringCode);
	}
}