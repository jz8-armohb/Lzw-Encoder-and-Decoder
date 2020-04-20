#include <iostream>
#include "declarations.h"
#include "BitIO.h"
using namespace std;

int main(int argc, char* argv[]) {
	FILE* fp;
	BITFILE* bf;

	if (argc < 4) {
		fprintf(stdout, "Usage: \n%s <options> <inFile> <outFile>\n", argv[0]);
		fprintf(stdout, "\t<options>: E for LZW encoding or D for LZW decoding.\n");
		fprintf(stdout, "\t<inFile>: name of input file.\n");
		fprintf(stdout, "\t<outFile>: name of output file.\n");
		return -1;
	}

	if ('E' == argv[1][0]) {	/* Do LZW encoding */
		/* Open the files */
		if (fopen_s(&fp, argv[2], "rb") != 0) {
			cout << "Failed to open \"" << argv[2] << "\"." << endl;
			exit(-1);
		}
		bf = OpenBitFileOutput(argv[3]);

		if ((fp != NULL) && (bf != NULL)) {
			LzwEncoding(fp, bf);
			//LZWEncode(fp, bf);
			fclose(fp);
			CloseBitFileOutput(bf);
			fprintf(stdout, "LZW encoding done.\n");
		}
	} else if ('D' == argv[1][0]) {	/* Do LZW decoding */
		/* Open the files */
		bf = OpenBitFileInput(argv[2]);
		if (fopen_s(&fp, argv[3], "wb") != 0) {
			cout << "Failed to open \"" << argv[2] << "\"." << endl;
			exit(-1);
		}

		if ((fp != NULL) && (bf != NULL)) {
			LzwDecoding(bf, fp);
			//LZWDecode(bf, fp);
			fclose(fp);
			CloseBitFileInput(bf);
			fprintf(stdout, "LZW decoding done.\n");
		}
	} else {
		fprintf(stderr, "Unsupported operation.\n");
	}
}