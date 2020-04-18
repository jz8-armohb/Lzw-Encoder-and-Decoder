#include <iostream>
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[]) {
	FILE* oriFilePtr = NULL;
	FILE* cmprFilePtr = NULL;
	FILE* decFilePtr = NULL;
	const char* oriFileName = argv[1];
	const char* cmprFileName = argv[2];
	const char* decFileName = argv[3];
	 
	/* Open the files */
	if (fopen_s(&oriFilePtr, oriFileName, "rb") == 0) {
		cout << "Successfully opened \"" << oriFileName << "\"." << endl;
	} else {
		cout << "Failed to open \"" << oriFileName << "\"." << endl;
		exit(-1);
	}
	if (fopen_s(&cmprFilePtr, cmprFileName, "wb+") == 0) {
		cout << "Successfully opened \"" << cmprFileName << "\"." << endl;
	} else {
		cout << "Failed to open \"" << cmprFileName << "\"." << endl;
		exit(-1);
	}
	if (fopen_s(&decFilePtr, decFileName, "wb") == 0) {
		cout << "Successfully opened \"" << decFileName << "\"." << endl;
	} else {
		cout << "Failed to open \"" << decFileName << "\"." << endl;
		exit(-1);
	}

	LzwEncoding(oriFilePtr, cmprFilePtr);

	fclose(oriFilePtr);
	fclose(cmprFilePtr);
	fclose(decFilePtr);
}