#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned char uchar;

enum Command {
    inverse = 0,
    mirrorHorizontal = 1,
    mirrorVertical = 2,
    turn90 = 3,
    turn270 = 4
};

struct RGB {
    uchar R;
    uchar G;
    uchar B;
};

class FileIOException : public exception {
public:
    const char* what() const _NOEXCEPT override {
            return "Error while trying to read file or write to file";
    }
};

class UnsupportedFormatException : public exception {
public:
    const char* what() const _NOEXCEPT override {
            return "Unsupported format of PNM file";
    }
};

class ExecutionException : public exception {
public:
    const char* what() const _NOEXCEPT override {
            return "Failed to execute the command";
    }
};

class PNMPicture {
public:
    PNMPicture();
    explicit PNMPicture(string fileName);

    void read(string fileName);
    void read(ifstream& inputFile);

    void write(string fileName);
    void write(ofstream& outputFile);

    void execute(Command cmd);

    void inverse();

    void mirrorVertical();

    void mirrorHorizontal();

    void turn90();

    void turn270();

    void printInfo();

private:
    short format;
    int width, height;
    int colors;
    vector<uchar> dataP5;
    vector<RGB> dataP6;

    void readP5(ifstream& inputFile);
    void readP6(ifstream& inputFile);

    void writeP5(ofstream& outputFile);
    void writeP6(ofstream& outputFile);

    void inverseP5();
    void inverseP6();

    void mirrorVerticalP5();
    void mirrorVerticalP6();

    void mirrorHorizontalP5();
    void mirrorHorizontalP6();

    void turn90P5();
    void turn90P6();

    void turn270P5();
    void turn270P6();

};

