#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef unsigned char uchar;

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

class IndexOutOfBoundsException : public exception {
public:
    const char* what() const _NOEXCEPT override {
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

    void dither(DitherAlgo algo, uchar bits);

private:
    short format;
    int width, height;
    int colors;
    vector<uchar> data;

    uchar& get(int h, int w);

    void ditherNone(uchar bits);
    void ditherOrdered(uchar bits);
    void ditherRandom(uchar bits);
    void ditherFloydSteinberg(uchar bits);
    void ditherJJN(uchar bits);
    void ditherSierra(uchar bits);
    void ditherAtkinson(uchar bits);
    void ditherHalftone(uchar bits);

    static uchar undoColorCorrection(uchar color);
    static double undoValueCorrection(double k);
    static uchar correctColor(uchar color);
    static double correctValue(double k);

};