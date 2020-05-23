#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef unsigned char uchar;

struct ColorChannel {
    uchar A;
    uchar B;
    uchar C;
};

enum ColorChannelName {
    A, B, C
};

enum ColorSpace {
    CS_RGB,
    CS_HSL,
    CS_HSV,
    CS_YCbCr_601,
    CS_YCbCr_709,
    CS_YCoCg,
    CS_CMY
};

class FileIOException : public std::exception {
public:
    const char* what() const _NOEXCEPT override {
        return "Error while trying to read file or write to file";
    }
};

class UnsupportedFormatException : public std::exception {
public:
    const char* what() const _NOEXCEPT override {
        return "Unsupported format of PNM file";
    }
};

class ExecutionException : public std::exception {
public:
    const char* what() const _NOEXCEPT override {
        return "Failed to execute the command";
    }
};

class PNMPicture {
public:
    PNMPicture();
    explicit PNMPicture(const std::string& fileName);
    explicit PNMPicture(const std::string& file1Name, const std::string& file2Name, const std::string& file3Name);

    void read(const std::string& fileName);
    void read(std::ifstream& inputFile);
    void read(const std::string& file1Name, const std::string& file2Name, const std::string& file3Name);
    void read(std::ifstream& inputFile1, std::ifstream& inputFile2, std::ifstream& inputFile3);

    void write(const std::string& fileName);
    void write(std::ofstream& outputFile);
    void write(const std::string& file1Name, const std::string& file2Name, const std::string& file3Name);
    void write(std::ofstream& outputFile1, std::ofstream& outputFile2, std::ofstream& outputFile3);

    void convert(ColorSpace from, ColorSpace to);

    void setCurrentColorSpace(ColorSpace cs);
    void printInfo();

private:
    int width, height;
    int colors;
    std::vector<ColorChannel> data;
    ColorSpace curColorSpace;

    void readP5(std::ifstream& inputFile, ColorChannelName colorToRead);
    void readP6(std::ifstream& inputFile);

    void writeP5(std::ofstream& outputFile, ColorChannelName colorToWrite);
    void writeP6(std::ofstream& outputFile);

    void convertFromHSL();
    void convertFromHSV();
    void convertFromYCbCr601();
    void convertFromYCbCr709();
    void convertFromYCoCg();
    void convertFromCMY();

    void convertToHSL();
    void convertToHSV();
    void convertToYCbCr601();
    void convertToYCbCr709();
    void convertToYCoCg();
    void convertToCMY();

};

