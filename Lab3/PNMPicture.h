#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned char uchar;

class FileIOException : public exception {
public:
    const char* what() const noexcept override {
        return "Error while trying to read file or write to file";
    }
};

class UnsupportedFormatException : public exception {
public:
    const char* what() const noexcept override {
        return "Unsupported format of PNM file";
    }
};

class ExecutionException : public exception {
public:
    const char* what() const noexcept override {
        return "Failed to execute the command";
    }
};

class IndexOutOfBoundsException : public exception {
public:
    const char* what() const noexcept override {
        return "Index is out of bounds";
    }
};

enum DitherAlgo {
    NONE = 0,
    ORDERED = 1,
    RANDOM = 2,
    FLOYD_STEINBERG = 3,
    JJN = 4,
    SIERRA = 5,
    ATKINSON = 6,
    HALFTONE = 7
};

struct Point {
    float x;
    float y;
};

class PNMPicture {
public:
    PNMPicture();
    explicit PNMPicture(string fileName);

    void read(string fileName);
    void read(ifstream& inputFile);

    void write(const string& fileName);
    void write(ofstream& outputFile);

    void fillWithGradient();

    void dither(DitherAlgo algo, uchar bitRate);

private:
    short format;
    int width, height;
    int colors;
    vector<uchar> data;

    uchar& get(int h, int w);

    void ditherNone(uchar bitRate);
    void ditherOrdered(uchar bitRate);
    void ditherRandom(uchar bitRate);
    void ditherFloydSteinberg(uchar bitRate);
    void ditherJJN(uchar bitRate);
    void ditherSierra(uchar bitRate);
    void ditherAtkinson(uchar bitRate);
    void ditherHalftone(uchar bitRate);

    static uchar undoColorCorrection(uchar color);
    static double undoValueCorrection(double k);
    static uchar correctColor(uchar color);
    static double correctValue(double k);

};
