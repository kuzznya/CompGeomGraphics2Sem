#include "PNMPicture.h"

#include <cmath>

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
            drawPoint(i, j, 0, 255);
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            drawPoint(i, j, (double) j / width, 0);
        }
    }
}

void PNMPicture::dither() {

}

uchar& PNMPicture::get(int h, int w) {
    if (h < 0 || w < 0 || h >= height || w >= width)
        throw IndexOutOfBoundsException();
    return data[width * h + w];
}

void PNMPicture::drawPoint(int h, int w, double transparency, uchar color) {
    transparency = max(min(transparency, 1.0), 0.0);
    if (h < 0 || h >= height || w < 0 || w >= width)
        return;
    double lineColorLinear = color / 255.0;
    double picColorSRGB = get(h, w) / 255.0;
    double picColorLinear = picColorSRGB <= 0.04045 ? picColorSRGB / 12.92 : pow((picColorSRGB + 0.055) / 1.055, 2.4);
    double c = (1 - transparency) * lineColorLinear + transparency * picColorLinear;
    double csrgb = c <= 0.0031308 ? 12.92 * c : 1.055 * pow(c, 1 / 2.4) - 0.055;
    get(h, w) = 255 * csrgb;
}

void PNMPicture::drawPoint(int h, int w, double transparency, uchar color, float gamma) {
    transparency = max(min(transparency, 1.0), 0.0);
    if (h < 0 || h >= height || w < 0 || w >= width)
        return;
    get(h, w) = 255 * pow((transparency * get(h, w) + color * (1 - transparency)) / 255.0,
                                    (1.0 / gamma - 1.0) * (1.0 - transparency) + 1.0);
}
