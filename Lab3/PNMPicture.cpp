#include "PNMPicture.h"

#include <cmath>

const double orderedMatrix[8][8] = {
        {0.0 / 64.0, 48.0 / 64.0, 12.0 / 64.0, 60.0 / 64.0, 3.0 / 64.0, 51.0 / 64.0, 15.0 / 64.0, 63.0 / 64.0},
        {32.0 / 64.0, 16.0 / 64.0, 44.0 / 64.0, 28.0 / 64.0, 35.0 / 64.0, 19.0 / 64.0, 47.0 / 64.0, 31.0 / 64.0},
        {8.0 / 64.0, 56.0 / 64.0, 4.0 / 64.0, 52.0 / 64.0, 11.0 / 64.0, 59.0 / 64.0, 7.0 / 64.0, 55.0 / 64.0},
        {40.0 / 64.0, 24.0 / 64.0, 36.0 / 64.0, 20.0 / 64.0, 43.0 / 64.0, 27.0 / 64.0, 39.0 / 64.0, 23.0 / 64.0},
        {2.0 / 64.0, 50.0 / 64.0, 14.0 / 64.0, 62.0 / 64.0, 1.0 / 64.0, 49.0 / 64.0, 13.0 / 64.0, 61.0 / 64.0},
        {34.0 / 64.0, 18.0 / 64.0, 46.0 / 64.0, 30.0 / 64.0, 33.0 / 64.0, 17.0 / 64.0, 45.0 / 64.0, 29.0 / 64.0},
        {10.0 / 64.0, 58.0 / 64.0, 6.0 / 64.0, 54.0 / 64.0, 9.0 / 64.0, 57.0 / 64.0, 5.0 / 64.0, 53.0 / 64.0},
        {42.0 / 64.0, 26.0 / 64.0, 38.0 / 64.0, 22.0 / 64.0, 41.0 / 64.0, 25.0 / 64.0, 37.0 / 64.0, 21.0 / 64.0},
};

PNMPicture::PNMPicture() = default;
PNMPicture::PNMPicture(string filename) {
    read(filename);
}

void PNMPicture::read(string fileName) {
    ifstream inputFile(fileName, ios::binary);
    if (!inputFile.is_open())
        throw FileIOException();
    read(inputFile);
    inputFile.close();
}

void PNMPicture::read(ifstream& inputFile) {
    char P;
    inputFile >> P;
    if (P != 'P')
        throw UnsupportedFormatException();

    inputFile >> format;
    if (format != 5)
        throw UnsupportedFormatException();

    inputFile >> width >> height;
    inputFile >> colors;
    if (colors != 255)
        throw UnsupportedFormatException();

    inputFile.get();

    data.resize(width * height);
    inputFile.read((char*) &data[0], width * height);
    if (inputFile.fail())
        throw FileIOException();
}

void PNMPicture::write(const string& fileName) {
    ofstream outputFile(fileName, ios::binary);
    if (!outputFile.is_open())
        throw FileIOException();
    write(outputFile);
    outputFile.close();
}

void PNMPicture::write(ofstream& outputFile) {
    outputFile << "P" << format << '\n';
    outputFile << width << ' ' << height << '\n';
    outputFile << colors << '\n';
    outputFile.write((char*) &data[0], width * height);
}

void PNMPicture::fillWithGradient() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            get(i, j) = correct((double) j / width * 255);
        }
    }
}

void PNMPicture::dither(DitherAlgo algo, uchar bits) {
    switch (algo) {
        case NONE:
            ditherNone(bits);
            break;
        case ORDERED:
            break;
        case RANDOM:
            break;
        case FLOYD_STEINBERG:
            break;
        case JJN:
            break;
        case SIERRA:
            break;
        case ATKINSON:
            break;
        case HALFTONE:
            break;
        default:
            throw ExecutionException();
    }
}

void PNMPicture::ditherNone(uchar bits) {
    uchar maxValue = pow(2, bits) - 1;

    double value;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            value = undoCorrection(get(i, j)) / 255.0;
            value *= maxValue;
            value = round(value);
            get(i, j) = round(correct(value * 255 / maxValue));
        }
    }
}

void PNMPicture::ditherOrdered(uchar bits) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

        }
    }
}

uchar& PNMPicture::get(int h, int w) {
    if (h < 0 || w < 0 || h >= height || w >= width)
        throw IndexOutOfBoundsException();
    return data[width * h + w];
}

uchar PNMPicture::undoCorrection(uchar color) {
    double colorSRGB = color / 255.0;
    double colorLinear = colorSRGB <= 0.04045 ? colorSRGB / 12.92 : pow((colorSRGB + 0.055) / 1.055, 2.4);
    return colorLinear * 255;
}

uchar PNMPicture::correct(uchar color) {
    double colorLinear = color / 255.0;
    double colorSRGB = colorLinear <= 0.0031308 ? 12.92 * colorLinear : 1.055 * pow(colorLinear, 1 / 2.4) - 0.055;
    return colorSRGB * 255;
}


