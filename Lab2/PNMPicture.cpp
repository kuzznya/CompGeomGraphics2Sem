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

    const bool steep = abs(end.y - start.y) > abs(end.x - start.x);

    if (thickness == 1)
        drawWuLine(start, end, color, gamma);
    else if (thickness < 3) {
        for (float offset = - thickness / 2.0; offset < thickness / 2.0; offset += 0.5)
            drawWuLine({start.x, start.y + offset}, {end.x, end.y + offset}, color, gamma);
    }
    else {
        if (!steep) {
            drawWuLine({start.x, start.y - thickness / 2}, {end.x, end.y - thickness / 2}, color, gamma);
            drawWuLine({start.x, start.y + thickness / 2}, {end.x, end.y + thickness / 2}, color, gamma);
            for (float offset = -thickness / 2.0 + 1; offset <= thickness / 2.0 - 1; offset += 0.5)
                drawBresenhamLine({start.x, start.y + offset}, {end.x, end.y + offset}, color, gamma);
        }
        else {
            drawWuLine({start.x - thickness / 2, start.y}, {end.x - thickness / 2, end.y}, color, gamma);
            drawWuLine({start.x + thickness / 2, start.y}, {end.x + thickness / 2, end.y}, color, gamma);
            for (float offset = -thickness / 2.0 + 1; offset <= thickness / 2.0 - 1; offset += 0.5)
                drawBresenhamLine({start.x + offset, start.y}, {end.x + offset, end.y}, color, gamma);
        }
    }


}

void PNMPicture::drawLine(float x0, float y0, float x1, float y1, uchar brightness, float thickness, float gamma) {
    drawLine({x0, y0}, {x1, y1}, brightness, thickness, gamma);
}

void PNMPicture::drawWuLine(Point start, Point end, uchar color, float gamma) {

    const bool steep = abs(end.y - start.y) > abs(end.x - start.x);

    auto fracPart = [](double x) -> double {return x - floor(x);};
    auto intPart = [](double x) -> int {return (int) x;};
    auto plot = [&](int x, int y, double brightness) -> void {
        if (steep)
            drawPoint(y, x, 1.0 - brightness, color, gamma);
        else
            drawPoint(x, y, 1.0 - brightness, color, gamma);
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

    for(int x = round(start.x); x <= round(end.x); x++){
        plot(x, intPart(y), (1.0 - fracPart(y)));
        plot(x, intPart(y) + 1.0, fracPart(y));
        y += gradient;
    }
}

void PNMPicture::drawBresenhamLine(Point start, Point end, uchar color, float gamma) {

    int dx = (int) abs(end.x - start.x);
    int dy = (int) abs(end.y - start.y);

    int signX = start.x < end.x ? 1 : -1;
    int signY = start.y < end.y ? 1 : -1;

    auto plot = [&](int x, int y) -> void {
        if (dx >= dy)
            drawPoint(x, y, 0, color, gamma);
        else
            drawPoint(y, x, 0, color, gamma);
    };

    auto draw = [plot](const int x1, const int y1,
            const int x2, const int y2,
            const int dx, const int dy,
            const int signX, const int signY) -> void {
        int err = 0;

        int x = x1;
        int y = y1;

        while (true) {
            plot(x, y);
            if (x == x2)
                break;
            x += signX;
            err += 2 * dy;
            if (err > dx) {
                y += signY;
                err -= 2 * dx;
            }
        }
    };

    if (dx >= dy)
        draw(start.x, start.y, end.x, end.y, dx, dy, signX, signY);
    else
        draw(start.y, start.x, end.y, end.x, dy, dx, signY, signX);

}

void PNMPicture::drawPoint(int x, int y, double intensity, uchar color, float gamma) {
    if (intensity < 0 || intensity > 1)
        throw ExecutionException();
    if (y < 0 || y >= height || x < 0 || x >= width)
        return;
    double k = color / (double) data[width * y + x];
     data[width * y + x] *= pow((1 - k) * intensity + k, 1.0 / gamma);
}
