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
    if (thickness <= 0)
        return;

    bool steep = abs(end.y - start.y) > abs(end.x - start.x);

    auto intPart = [](double x) -> int {return (int) x;};
    auto plot = [&](int x, int y, double intensity) -> void {
        if (gamma == 0) {
            if (steep)
                drawPoint(y, x, 1.0 - intensity, color);
            else
                drawPoint(x, y, 1.0 - intensity, color);
        }
        else {
            if (steep)
                drawPoint(y, x, 1.0 - intensity, color, gamma);
            else
                drawPoint(x, y, 1.0 - intensity, color, gamma);
        }
    };
    auto distance = [](Point a, Point b) -> double {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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

    double y = start.y + gradient * (round(start.x) - start.x);

    for(int x = round(start.x); x <= round(end.x); x++) {
        for (int plotY = intPart(y - (thickness - 1) / 2);
             plotY <= intPart(y - (thickness - 1) / 2 + thickness); plotY++)
        {
            plot(x, plotY, min(1.0, (thickness + 1.0) / 2.0 - fabs(y - plotY)));
        }
        y += gradient;
    }

    Point plotStart = {round(start.x), round(start.y)};
    for (int plotX = round(start.x) - thickness / 2; plotX < round(start.x); plotX++) {
        y = start.y + gradient * (plotX - start.x);
        for (int plotY = int(y - (thickness - 1) / 2.0); plotY <= int(y - (thickness - 1) / 2.0 + thickness); plotY++) {
            plot(plotX, plotY, min(1.0, (thickness + 0.5) / 2.0 -
            distance({(float) plotX, (float) plotY}, {plotStart.x, plotStart.y})));
        }
    }

    Point plotEnd = {round(end.x), round(end.y)};
    for (int plotX = round(end.x) + 1; plotX <= round(end.x) + thickness / 2; plotX++) {
        y = start.y + gradient * (plotX - start.x);
        for (int plotY = int(y - (thickness - 1) / 2.0); plotY <= int(y - (thickness - 1) / 2.0 + thickness); plotY++) {
            plot(plotX, plotY, min(1.0, (thickness + 0.5) / 2.0 -
                                       distance({(float) plotX, (float) plotY}, {plotEnd.x, plotEnd.y})));
        }
    }
}

void PNMPicture::drawLine(float x0, float y0, float x1, float y1, uchar color, float thickness, float gamma) {
    drawLine({x0, y0}, {x1, y1}, color, thickness, gamma);
}

void PNMPicture::drawPoint(int x, int y, double transparency, uchar color) {
    transparency = max(min(transparency, 1.0), 0.0);
    if (y < 0 || y >= height || x < 0 || x >= width)
        return;
    double lineColorLinear = color / 255.0;
    double picColorSRGB = data[width * y + x] / 255.0;
    double picColorLinear = picColorSRGB <= 0.04045 ? picColorSRGB / 12.92 : pow((picColorSRGB + 0.055) / 1.055, 2.4);
    double c = (1 - transparency) * lineColorLinear + transparency * picColorLinear;
    double csrgb = c <= 0.0031308 ? 12.92 * c : 1.055 * pow(c, 1 / 2.4) - 0.055;
    data[width * y + x] = 255 * csrgb;
}

void PNMPicture::drawPoint(int x, int y, double transparency, uchar color, float gamma) {
    transparency = max(min(transparency, 1.0), 0.0);
    if (y < 0 || y >= height || x < 0 || x >= width)
        return;
    data[width * y + x] = 255 * pow((transparency * data[width * y + x] + color * (1 - transparency)) / 255.0,
            (1.0 / gamma - 1.0) * (1.0 - transparency) + 1.0);
}
