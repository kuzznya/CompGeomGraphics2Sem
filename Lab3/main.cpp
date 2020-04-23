#include <iostream>
#include "PNMPicture.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Incorrect count of arguments" << endl;
        return 1;
    }

    char* inputFile = argv[1];
    char* outputFile = argv[2];

    bool gradient;
    if (argv[3][0] == '1')
        gradient = true;
    else if (argv[3][0] == '0')
        gradient = false;
    else {
        cerr << "Incorrect type of argument 3: gradient must by 1 (true) or 0 (false)" << endl;
        return 1;
    }

    if (!isdigit(argv[4][0]) || strlen(argv[4]) != 1) {
        cerr << "incorrect type of argument 4: dithering algorithm must be integer from 0 to 7" << endl;
        return 1;
    }
    DitherAlgo algo = (DitherAlgo) (argv[4][0] - 48);
    if (algo > 7) {
        cerr << "incorrect type of argument 4: dithering algorithm must be integer from 0 to 7" << endl;
        return 1;
    }

    uchar bitRate;
    if (!isdigit(argv[5][0]) || strlen(argv[5]) != 1) {
        cerr << "incorrect type of argument 5: dithering algorithm must be integer from 1 to 8" << endl;
        return 1;
    }
    bitRate = argv[5][0] - 48;
    if (bitRate > 8 || bitRate == 0) {
        cerr << "Bit rate must be from 1 to 8";
        return 1;
    }

    PNMPicture picture;
    try {
        picture.read(inputFile);
    } catch (FileIOException& ex) {
        cerr << "Error trying to read file " << inputFile << endl;
        return 1;
    } catch (UnsupportedFormatException& ex) {
        cerr << "Error: unsupported format" << endl;
        return 1;
    }

    if (gradient)
        picture.fillWithGradient();

    try {
        picture.dither(algo, bitRate);
    } catch (ExecutionException& ex) {
        cerr << "Error trying to dither the picture" << endl;
        return 1;
    }

    try {
        picture.write(outputFile);
    } catch (FileIOException& ex) {
        cerr << "Error trying to write data to file " << outputFile << endl;
        return 1;
    }

    return 0;
}
