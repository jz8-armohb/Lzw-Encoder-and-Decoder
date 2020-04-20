#include <iostream>
#include "declarations.h"
#include "BitIO.h"
using namespace std;

int main(int argc, char* argv[]) {
	//FILE* oriFilePtr = NULL;
	//FILE* cmprFilePtr = NULL;
	//FILE* decFilePtr = NULL;
	//const char* oriFileName = argv[1];
	//const char* cmprFileName = argv[2];
	//const char* decFileName = argv[3];
	// 
	///* Open the files */
	//if (fopen_s(&oriFilePtr, oriFileName, "rb") == 0) {
	//	cout << "Successfully opened \"" << oriFileName << "\"." << endl;
	//} else {
	//	cout << "Failed to open \"" << oriFileName << "\"." << endl;
	//	exit(-1);
	//}
	//if (fopen_s(&cmprFilePtr, cmprFileName, "wb+") == 0) {
	//	cout << "Successfully opened \"" << cmprFileName << "\"." << endl;
	//} else {
	//	cout << "Failed to open \"" << cmprFileName << "\"." << endl;
	//	exit(-1);
	//}
	//if (fopen_s(&decFilePtr, decFileName, "wb") == 0) {
	//	cout << "Successfully opened \"" << decFileName << "\"." << endl;
	//} else {
	//	cout << "Failed to open \"" << decFileName << "\"." << endl;
	//	exit(-1);
	//}

	//LzwEncoding(oriFilePtr, cmprFilePtr);
	////LzwDecoding(cmprFilePtr, decFilePtr);

	//fclose(oriFilePtr);
	//fclose(cmprFilePtr);
	//fclose(decFilePtr);

	FILE* inFilePtr;
	BITFILE* outBitFilePtr;

	if (argc < 4) {
		fprintf(stdout, "Usage: \n%s <options> <inFile> <outFile>\n", argv[0]);
		fprintf(stdout, "\t<options>: E for LZW encoding or D for LZW decoding.\n");
		fprintf(stdout, "\t<inFile>: name of input file.\n");
		fprintf(stdout, "\t<outFile>: name of output file.\n");
		return -1;
	}

	if ('E' == argv[1][0]) {	/* Do LZW encoding */
		/* Open the files */
		if (fopen_s(&inFilePtr, argv[2], "rb") != 0) {
			cout << "Failed to open \"" << argv[2] << "\"." << endl;
			exit(-1);
		}
		outBitFilePtr = OpenBitFileOutput(argv[3]);

		if ((inFilePtr != NULL) && (outBitFilePtr != NULL)) {
			LzwEncoding(inFilePtr, outBitFilePtr);
			fclose(inFilePtr);
			CloseBitFileOutput(outBitFilePtr);
			fprintf(stdout, "LZW encoding done.\n");
		}
	} 
	//else if ('D' == argv[1][0]) {	// do decoding
	//	outBitFilePtr = OpenBitFileInput(argv[2]);
	//	inFilePtr = fopen(argv[3], "wb");
	//	if (NULL != inFilePtr && NULL != outBitFilePtr) {
	//		lzwDecoding(outBitFilePtr, inFilePtr);
	//		fclose(inFilePtr);
	//		CloseBitFileInput(outBitFilePtr);
	//		fprintf(stdout, "decoding done\n");
	//	}
	//} else {	// otherwise
	//	fprintf(stderr, "not supported operation\n");
	//}
}