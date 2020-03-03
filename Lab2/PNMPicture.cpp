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

void PNMPicture::drawLine(Point start, Point end, uchar color, float thickness, float gamma) {

    bool steep = abs(end.y - start.y) > abs(end.x - start.x);

    auto fracPart = [](double x) -> double {return x - floor(x);};
    auto intPart = [](double x) -> int {return (int) x;};
    auto plot = [&](int x, int y, double brightness) -> void {
        if (steep)
            drawPoint(y, x, 1 - brightness, color, gamma);
        else
            drawPoint(x, y, 1 - brightness, color, gamma);
    };

    if (steep) {
        swap(start.x, start.y);
        swap(end.x, end.y);
    }
    if (start.x > end.x) {
        swap(start.x, end.x);
        swap(start.y, end.y);
    }

    double dx = end.x - start.x;
    double dy = end.y - start.y;
    double gradient = dy / dx;

    // handle first endpoint
    double xEnd = round(start.x);
    double yEnd = start.y + gradient * (xEnd - start.x);
    double xGap = 1 - fracPart(start.x + 0.5);
    double xPixel1 = xEnd; // will be used in the main loop
    double yPixel1 = intPart(yEnd);

    plot(xPixel1,yPixel1, (1.0 - fracPart(yEnd)) * xGap);
    plot(xPixel1, yPixel1 + 1.0, fracPart(yEnd) * xGap);

    // first y-intersection for the main loop
    double interY = yEnd + gradient;

    // handle second endpoint
    xEnd = round(end.x);
    yEnd = end.y + gradient * (xEnd - end.x);
    xGap = fracPart(end.x + 0.5);
    double xPixel2 = xEnd; // will be used in the main loop
    double yPixel2 = intPart(yEnd);
    plot(xPixel2, yPixel2, (1.0 - fracPart(yEnd)) * xGap);
    plot(xPixel2, yPixel2 + 1.0, fracPart(yEnd) * xGap);

    for(int x = intPart(xPixel1 + 1.0); x <= xPixel2 - 1; x++){
        plot(x, intPart(interY), (1.0 - fracPart(interY)));
        plot(x, intPart(interY) + 1.0, fracPart(interY));
        interY += gradient;
    }

}

void PNMPicture::drawLine(float x0, float y0, float x1, float y1, uchar brightness, float thickness, float gamma) {
    drawLine({x0, y0}, {x1, y1}, brightness, thickness, gamma);
}

void PNMPicture::drawPoint(int x, int y, double brightness, uchar color, float gamma) {
    if (brightness < 0 || brightness > 1)
        throw ExecutionException();
    if (y < 0 || y >= height || x < 0 || x >= width)
        return;
     data[width * y + x] = color * pow(brightness, 1.0 / gamma);
}
