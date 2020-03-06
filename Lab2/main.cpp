#include <iostream>
#include <string>
#include "PNMPicture.h"

int main(int argc, char* argv[]) {

    if (argc < 9 || argc > 10) {
        cerr << "Incorrect count of arguments" << endl;
        return 0;
    }

    string inputFileName = argv[1];
    string outputFileName = argv[2];
    uchar color = stoi(argv[3]);
    float thickness = stof(argv[4]);
    float x0 = stof(argv[5]);
    float y0 = stof(argv[6]);
    float x1 = stof(argv[7]);
    float y1 = stof(argv[8]);
    float gamma = 2.2;
    bool gammaDefined = false;
    if (argc == 10) {
        gamma = stof(argv[9]);
        gammaDefined = true;
    }

    PNMPicture picture;

    try {
        picture.read(inputFileName);
    } catch (FileIOException&) {
        cerr << "Error while trying to read file " << inputFileName << endl;
        return 1;
    } catch (UnsupportedFormatException&) {
        cerr << "Error: unsupported format" << endl;
        return 1;
    }

    try {
        if (gammaDefined)
            picture.drawLine(x0, y0, x1, y1, color, thickness, gamma);
        else
            picture.drawLine(x0, y0, x1, y1, color, thickness);
    } catch (FileIOException&) {
        cerr << "Error while trying to white picture to file " << outputFileName << endl;
        return 1;
    }

    picture.write(outputFileName);

    return 0;
}
