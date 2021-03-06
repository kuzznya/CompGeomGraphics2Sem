#include "PNMPicture.h"

#include <cmath>

const double orderedMatrix[8][8] = {
        {0.0 / 64.0, 48.0 / 64.0, 12.0 / 64.0, 60.0 / 64.0, 3.0 / 64.0, 51.0 / 64.0, 15.0 / 64.0, 63.0 / 64.0},
        {32.0 / 64.0, 16.0 / 64.0, 44.0 / 64.0, 28.0 / 64.0, 35.0 / 64.0, 19.0 / 64.0, 47.0 / 64.0, 31.0 / 64.0},
        {8.0 / 64.0, 56.0 / 64.0, 4.0 / 64.0, 52.0 / 64.0, 11.0 / 64.0, 59.0 / 64.0, 7.0 / 64.0, 55.0 / 64.0},
        {40.0 / 64.0, 24.0 / 64.0, 36.0 / 64.0, 20.0 / 64.0, 43.0 / 64.0, 27.0 / 64.0, 39.0 / 64.0, 23.0 / 64.0},
        {2.0 / 64.0, 50.0 / 64.0, 14.0 / 64.0, 62.0 / 64.0, 1.0 / 64.0, 49.0 / 64.0, 13.0 / 64.0, 61.0 / 64.0},
        {34.0 / 64.0, 18.0 / 64.0, 46.0 / 64.0, 30.0 / 64.0, 33.0 / 64.0, 17.0 / 64.0, 45.0 / 64.0, 29.0 / 64.0},
        {10.0 / 64.0, 58.0 / 64.0, 6.0 / 64.0, 54.0 / 64.0, 9.0 / 64.0, 57.0 / 64.0, 5.0 / 64.0, 53.0 / 64.0},
        {42.0 / 64.0, 26.0 / 64.0, 38.0 / 64.0, 22.0 / 64.0, 41.0 / 64.0, 25.0 / 64.0, 37.0 / 64.0, 21.0 / 64.0}
};

const double matrixJJN[3][5] = {
        {0, 0, 0, 7.0 / 48.0, 5.0 / 48.0},
        {3.0 / 48.0, 5.0 / 48.0, 7.0 / 48.0, 5.0 / 48.0, 3.0 / 48.0},
        {1.0 / 48.0, 3.0 / 48.0, 5.0 / 48.0, 3.0 / 48.0, 1.0 / 48.0}
};

const double matrixSierra3[3][5] = {
        {0, 0, 0, 5.0 / 32.0, 3.0 / 32.0},
        {2.0 / 32.0, 4.0/ 32.0, 5.0 / 32.0, 4.0 / 32.0, 2.0 / 32.0},
        {0, 2.0 / 32.0, 3.0 / 32.0, 2.0 / 32.0, 0}
};

const int matrixAtkinson[3][5] = {
        {0, 0, 0, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0}
};

const double halftoneMatrix[4][4] = {
        {13.0 / 16.0, 11.0 / 16.0, 4.0 / 16.0, 8.0 / 16.0},
        {6.0 / 16.0, 0, 3.0 / 16.0, 15.0 / 16.0},
        {14.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 7.0 / 16.0},
        {9.0 / 16.0, 5.0 / 16.0, 10.0 / 16.0, 12.0 / 16.0},
};

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
            get(i, j) = correctColor((double) j / width * 255);
        }
    }
}

void PNMPicture::dither(DitherAlgo algo, uchar bitRate) {
    switch (algo) {
        case NONE:
            ditherNone(bitRate);
            break;
        case ORDERED:
            ditherOrdered(bitRate);
            break;
        case RANDOM:
            ditherRandom(bitRate);
            break;
        case FLOYD_STEINBERG:
            ditherFloydSteinberg(bitRate);
            break;
        case JJN:
            ditherJJN(bitRate);
            break;
        case SIERRA:
            ditherSierra(bitRate);
            break;
        case ATKINSON:
            ditherAtkinson(bitRate);
            break;
        case HALFTONE:
            ditherHalftone(bitRate);
            break;
        default:
            throw ExecutionException();
    }
}

void PNMPicture::ditherNone(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double value = undoColorCorrection(get(i, j)) / 255.0;

            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);
            get(i, j) = correctColor(newPaletteColor / maxValue * 255);
        }
    }
}

void PNMPicture::ditherOrdered(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorSRGB = get(i, j) / 255.0;
            double picColorLinear = undoValueCorrection(picColorSRGB);
            double value = picColorLinear + (orderedMatrix[i % 8][j % 8] - 0.5) / bitRate;

            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);
            get(i, j) = correctColor(newPaletteColor / maxValue * 255);
        }
    }
}

void PNMPicture::ditherRandom(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    srand(time(NULL));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorSRGB = get(i, j) / 255.0;
            double picColorLinear = undoValueCorrection(picColorSRGB);
            double noise =  (double) rand() / RAND_MAX - 0.75;
            double value = picColorLinear + noise / bitRate;

            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);
            get(i, j) = correctColor(newPaletteColor / maxValue * 255);
        }
    }
}

void PNMPicture::ditherFloydSteinberg(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorSRGB = get(i, j) / 255.0;
            double picColorLinear = undoValueCorrection(picColorSRGB);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = get(i, j) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            if (j + 1 < height)
                getError(i, j + 1) += error * 7.0 / 16.0;
            if (i + 1 < height && j + 1 < height)
                getError(i + 1, j + 1) += error * 1.0 / 16.0;
            if (i + 1 < height)
                getError(i + 1, j) += error * 5.0 / 16.0;
            if (i + 1 < height && j - 1 >= 0)
                getError(i + 1, j - 1) += error * 3.0 / 16.0;
        }
    }
}

void PNMPicture::ditherJJN(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorSRGB = get(i, j) / 255.0;
            double picColorLinear = undoValueCorrection(picColorSRGB);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = get(i, j) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        break;
                    if (ie == 0 && je <= 2)
                        break;

                    getError(i + ie, j + (je - 2)) += error * matrixJJN[ie][je];
                }
            }
        }
    }
}

void PNMPicture::ditherSierra(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorSRGB = get(i, j) / 255.0;
            double picColorLinear = undoValueCorrection(picColorSRGB);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = get(i, j) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        break;
                    if (ie == 0 && je <= 2)
                        break;

                    getError(i + ie, j + (je - 2)) += error * matrixSierra3[ie][je];
                }
            }
        }
    }
}

void PNMPicture::ditherAtkinson(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    vector<double> errors(height * width, 0);
    auto getError = [&](int h, int w) -> double& {
        return errors[h * width + w];
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorSRGB = get(i, j) / 255.0;
            double picColorLinear = undoValueCorrection(picColorSRGB);
            double value = picColorLinear + getError(i, j) / 255.0;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            double error = get(i, j) + getError(i, j) - newPaletteColor / (double) maxValue * 255.0;

            get(i, j) = correctColor(newPaletteColor / maxValue * 255);

            for (int ie = 0; ie < 3; ie++) {
                for (int je = 0; je < 5; je++) {
                    if (i + ie >= height || j + (je - 2) >= width || j + (je - 2) < 0)
                        break;
                    if (ie == 0 && je <= 2)
                        break;

                    getError(i + ie, j + (je - 2)) += error * matrixAtkinson[ie][je] / 8.0;
                }
            }
        }
    }
}

void PNMPicture::ditherHalftone(uchar bitRate) {
    uchar maxValue = pow(2, bitRate) - 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double picColorSRGB = get(i, j) / 255.0;
            double picColorLinear = undoValueCorrection(picColorSRGB);
            double value = picColorLinear + (halftoneMatrix[i % 4][j % 4] - 0.75) / bitRate;
            value = min(max(value, 0.0), 1.0);

            double newPaletteColor = round(value * maxValue);

            get(i, j) = round(newPaletteColor / maxValue * 255);
        }
    }
}

uchar& PNMPicture::get(int h, int w) {
    if (h < 0 || w < 0 || h >= height || w >= width)
        throw IndexOutOfBoundsException();
    return data[width * h + w];
}

uchar PNMPicture::undoColorCorrection(uchar color) {
    double colorSRGB = color / 255.0;
    double colorLinear = undoValueCorrection(colorSRGB);
    return colorLinear * 255;
}

double PNMPicture::undoValueCorrection(double k) {
    return k <= 0.04045 ? k / 12.92 : pow((k + 0.055) / 1.055, 2.4);
}

uchar PNMPicture::correctColor(uchar color) {
    double colorLinear = color / 255.0;
    double colorSRGB = correctValue(colorLinear);
    return colorSRGB * 255;
}

double PNMPicture::correctValue(double k) {
    return k <= 0.0031308 ? 12.92 * k : 1.055 * pow(k, 1 / 2.4) - 0.055;
}


