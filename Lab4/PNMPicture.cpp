//
// Created by Илья Кузнецов on 01.03.2020.
//

#include "PNMPicture.h"

PNMPicture::PNMPicture() = default;

PNMPicture::PNMPicture(std::string filename) {
    read(filename);
}

void PNMPicture::read(const std::string& fileName) {
    std::ifstream inputFile(fileName, std::ios::binary);
    if (!inputFile.is_open())
        throw FileIOException();
    read(inputFile);
    inputFile.close();
}

void PNMPicture::read(std::ifstream& inputFile) {
    readP6(inputFile);
}

void PNMPicture::read(const std::string &file1Name,
                      const std::string &file2Name, const std::string &file3Name) {
    std::ifstream inputFile1(file1Name, std::ios::binary);
    if (!inputFile1.is_open())
        throw FileIOException();

    std::ifstream inputFile2(file2Name, std::ios::binary);
    if (!inputFile2.is_open())
        throw FileIOException();

    std::ifstream inputFile3(file3Name, std::ios::binary);
    if (!inputFile3.is_open())
        throw FileIOException();

    read(inputFile1, inputFile2, inputFile3);

    inputFile1.close();
    inputFile2.close();
    inputFile3.close();
}

void PNMPicture::read(std::ifstream &inputFile1,
                      std::ifstream &inputFile2, std::ifstream &inputFile3) {
    readP5(inputFile1, R);
    readP5(inputFile2, G);
    readP5(inputFile3, B);
}

void PNMPicture::write(const std::string& fileName) {
    std::ofstream outputFile(fileName, std::ios::binary);
    if (!outputFile.is_open())
        throw FileIOException();
    write(outputFile);
    outputFile.close();
}

void PNMPicture::write(std::ofstream& outputFile) {
    writeP6(outputFile);
}

void PNMPicture::write(const std::string& file1Name,
                       const std::string& file2Name, const std::string& file3Name) {
    std::ofstream outputFile1(file1Name, std::ios::binary);
    if (!outputFile1.is_open())
        throw FileIOException();

    std::ofstream outputFile2(file2Name, std::ios::binary);
    if (!outputFile2.is_open())
        throw FileIOException();

    std::ofstream outputFile3(file3Name, std::ios::binary);
    if (!outputFile3.is_open())
        throw FileIOException();

    write(outputFile1, outputFile2, outputFile3);

    outputFile1.close();
    outputFile2.close();
    outputFile3.close();
}

void PNMPicture::write(std::ofstream& outputFile1,
                       std::ofstream& outputFile2, std::ofstream& outputFile3) {
    writeP5(outputFile1, R);
    writeP5(outputFile1, G);
    writeP5(outputFile1, B);
}

void PNMPicture::convert(ColorSpace from, ColorSpace to) {
    switch (from) {
        case CS_HSL:
            convertFromHSL();
            break;
        case CS_HSV:
            convertFromHSV();
            break;
        case CS_YCbCr_601:
            convertFromYCbCr601();
            break;
        case CS_YCbCr_709:
            convertFromYCbCr709();
            break;
        case CS_YCoCg:
            convertFromYCoCg();
            break;
        case CS_CMY:
            convertFromCMY();
            break;
    }

    switch (to) {
        case CS_HSL:
            convertToHSL();
            break;
        case CS_HSV:
            convertToHSV();
            break;
        case CS_YCbCr_601:
            convertToYCbCr601();
            break;
        case CS_YCbCr_709:
            convertToYCbCr709();
            break;
        case CS_YCoCg:
            convertToYCoCg();
            break;
        case CS_CMY:
            convertToCMY();
            break;
    }
}

void PNMPicture::printInfo() {
    std::cout << "PNM - Portable Any Map" << std::endl;
    std::cout << "Format: P6 - colored image" << std::endl;
    std::cout << width << "x" << height << std::endl;
    std::cout << colors << " colors" << std::endl;
}

void PNMPicture::readP5(std::ifstream& inputFile, colorRGB colorToRead) {
    char P;
    inputFile >> P;
    if (P != 'P')
        throw UnsupportedFormatException();

    short format;
    inputFile >> format;
    if (format != 5)
        throw UnsupportedFormatException();

    inputFile >> width >> height;
    inputFile >> colors;
    if (colors != 255)
        throw UnsupportedFormatException();

    inputFile.get();

    uchar* buffer = new uchar[width * height];
    inputFile.read((char*) buffer, width * height);
    for (int i = 0; i < width * height; i++) {
        switch (colorToRead) {
            case R:
                data[i].R = buffer[i];
                break;
            case G:
                data[i].G = buffer[i];
                break;
            case B:
                data[i].B = buffer[i];
                break;
        }
    }

    delete[] buffer;
}

void PNMPicture::readP6(std::ifstream& inputFile) {
    char P;
    inputFile >> P;
    if (P != 'P')
        throw UnsupportedFormatException();

    short format;
    inputFile >> format;
    if (format != 6)
        throw UnsupportedFormatException();

    inputFile >> width >> height;
    inputFile >> colors;
    if (colors != 255)
        throw UnsupportedFormatException();

    inputFile.get();

    data.resize(width * height);

    uchar* buffer = new uchar[width * height * 3];
    inputFile.read((char*) buffer, width * height * 3);
    if (inputFile.fail())
        throw FileIOException();

    for (int i = 0; i < width * height; i++) {
        data[i].R = buffer[i * 3 + 0];
        data[i].G = buffer[i * 3 + 1];
        data[i].B = buffer[i * 3 + 2];
    }

    delete[] buffer;
}

void PNMPicture::writeP5(std::ofstream& outputFile, colorRGB colorToWrite) {
    outputFile << "P5" << '\n';
    outputFile << width << ' ' << height << '\n';
    outputFile << colors << '\n';
    uchar* buffer = new uchar[width * height];
    for (int i = 0; i < width * height; i++) {
        switch (colorToWrite) {
            case R:
                buffer[i] = data[i].R;
                break;
            case G:
                buffer[i] = data[i].G;
                break;
            case B:
                buffer[i] = data[i].B;
                break;
            default:
                throw ExecutionException();
        }
    }
    outputFile.write((char*) buffer, width * height);
    delete[] buffer;
}

void PNMPicture::writeP6(std::ofstream& outputFile) {
    outputFile << "P6" << '\n';
    outputFile << width << ' ' << height << '\n';
    outputFile << colors << '\n';

    uchar* buffer = new uchar[width * height * 3];
    for (int i = 0; i < width * height; i++) {
        buffer[i * 3 + 0] = data[i].R;
        buffer[i * 3 + 1] = data[i].G;
        buffer[i * 3 + 2] = data[i].B;
    }
    outputFile.write((char*) buffer, width * height * 3);
    delete[] buffer;
}

void PNMPicture::convertFromHSL() {
    //TODO
}

void PNMPicture::convertFromHSV() {
    //TODO
}

void PNMPicture::convertFromYCbCr601() {
    //TODO
}

void PNMPicture::convertFromYCbCr709() {
    //TODO
}

void PNMPicture::convertFromYCoCg() {
    //TODO
}

void PNMPicture::convertFromCMY() {
    //TODO
}


void PNMPicture::convertToHSL() {
    //TODO
}

void PNMPicture::convertToHSV() {
    //TODO
}

void PNMPicture::convertToYCbCr601() {
    //TODO
}

void PNMPicture::convertToYCbCr709() {
    //TODO
}

void PNMPicture::convertToYCoCg() {
    //TODO
}

void PNMPicture::convertToCMY() {
    //TODO
}
