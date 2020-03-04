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

    void drawLine(Point start, Point end, uchar color, float thickness, float gamma = 2.2);
    void drawLine(float x0, float y0, float x1, float y1, uchar brightness, float thickness, float gamma = 2.2);

private:
    short format;
    int width, height;
    int colors;
    vector<uchar> data;

    void drawWuLine(Point start, Point end, uchar color, float gamma = 2.2);
    void drawPoint(int x, int y, double darkness, uchar color, float gamma);
};
