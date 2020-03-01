//
// Created by Илья Кузнецов on 01.03.2020.
//

#include "PNMPicture.h"

PNMPicture::PNMPicture() = default;
PNMPicture::PNMPicture(string filename) {
    read(filename);
}

void PNMPicture::readP5(ifstream& inputFile) {
    dataP6.clear();
    dataP5.resize(width * height);
    inputFile.read((char*) &dataP5[0], width * height);
    if (inputFile.fail())
        throw FileIOException();
}

void PNMPicture::readP6(ifstream& inputFile) {
    dataP5.clear();
    dataP6.resize(width * height);

    uchar* buffer = new uchar[width * height * 3];
    inputFile.read((char*) buffer, width * height * 3);
    if (inputFile.fail())
        throw FileIOException();

    for (int i = 0; i < width * height; i++) {
        dataP6[i].R = buffer[i * 3 + 0];
        dataP6[i].G = buffer[i * 3 + 1];
        dataP6[i].B = buffer[i * 3 + 2];
    }

    delete[] buffer;
}

void PNMPicture::writeP5(ofstream& outputFile) {
    outputFile.write((char*) &dataP5[0], width * height);
}

void PNMPicture::writeP6(ofstream& outputFile) {
    uchar* buffer = new uchar[width * height * 3];
    for (int i = 0; i < width * height; i++) {
        buffer[i * 3 + 0] = dataP6[i].R;
        buffer[i * 3 + 1] = dataP6[i].G;
        buffer[i * 3 + 2] = dataP6[i].B;
    }
    outputFile.write((char*) buffer, width * height * 3);
    delete[] buffer;
}

void PNMPicture::inverseP5() {
    for (int i = 0; i < width * height; i++) {
        dataP5[i] = colors - dataP5[i];
    }
}

void PNMPicture::inverseP6() {
    for (int i = 0; i < width * height; i++) {
        dataP6[i].R = colors - dataP6[i].R;
        dataP6[i].G = colors - dataP6[i].G;
        dataP6[i].B = colors - dataP6[i].B;
    }
}

void PNMPicture::mirrorVerticalP5() {
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height / 2; i++) {
            swap(dataP5[i * width + j], dataP5[(height - 1) * width - i * width + j]);
        }
    }
}

void PNMPicture::mirrorVerticalP6() {
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height / 2; i++) {
            swap(dataP6[i * width + j], dataP6[(height - 1) * width - i * width + j]);
        }
    }
}

void PNMPicture::mirrorHorizontalP5() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            swap(dataP5[i * width + j], dataP5[i * width + width - j - 1]);
        }
    }
}

void PNMPicture::mirrorHorizontalP6() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            swap(dataP6[i * width + j], dataP6[i * width + width - j - 1]);
        }
    }
}

void PNMPicture::turn90P5() {
    vector<uchar> turnedDataP5(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            turnedDataP5[(height - i - 1) + j * height] = dataP5[i * width + j];
        }
    }

    dataP5.clear();
    dataP5 = turnedDataP5;
}

void PNMPicture::turn90P6() {
    vector<RGB> turnedDataP6(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            turnedDataP6[(height - i - 1) + j * height] = dataP6[i * width + j];
        }
    }

    dataP6.clear();
    dataP6 = turnedDataP6;
}

void PNMPicture::turn270P5() {
    vector<uchar> turnedDataP5(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            turnedDataP5[height * (width - j - 1) + i] = dataP5[i * width + j];
        }
    }

    dataP5.clear();
    dataP5 = turnedDataP5;
}

void PNMPicture::turn270P6() {
    vector<RGB> turnedDataP6(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            turnedDataP6[height * (width - j - 1) + i] = dataP6[i * width + j];
        }
    }

    dataP6.clear();
    dataP6 = turnedDataP6;
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
    if (format != 5 && format != 6)
        throw UnsupportedFormatException();

    inputFile >> width >> height;
    inputFile >> colors;
    if (colors != 255)
        throw UnsupportedFormatException();

    inputFile.get();
    if (format == 5)
        readP5(inputFile);
    else
        readP6(inputFile);
}

void PNMPicture::write(string fileName) {
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
    if (format == 5)
        writeP5(outputFile);
    else
        writeP6(outputFile);
}

void PNMPicture::execute(Command cmd) {
    switch (cmd) {
        case Command::inverse:
            inverse();
            break;
        case ::mirrorHorizontal:
            mirrorHorizontal();
            break;
        case ::mirrorVertical:
            mirrorVertical();
            break;
        case ::turn90:
            turn90();
            break;
        case ::turn270:
            turn270();
            break;
        default:
            throw ExecutionException();
    }
}

void PNMPicture::inverse() {
    if (this->format == 5)
        inverseP5();
    else
        inverseP6();
}

void PNMPicture::mirrorVertical() {
    if (format == 5)
        mirrorVerticalP5();
    else
        mirrorVerticalP6();
}

void PNMPicture::mirrorHorizontal() {
    if (format == 5)
        mirrorHorizontalP5();
    else
        mirrorHorizontalP6();
}

void PNMPicture::turn90() {
    if (format == 5)
        turn90P5();
    else
        turn90P6();
    swap(width, height);
}

void PNMPicture::turn270() {
    if (format == 5)
        turn270P5();
    else
        turn270P6();
    swap(width, height);
}

void PNMPicture::printInfo() {
    cout << "PNM - Portable Any Map" << endl;
    cout << "Format: P" << format << " - ";
    if (format == 5)
        cout << "grayscale image" << endl;
    else
        cout << "colored image" << endl;
    cout << width << "x" << height << endl;
    cout << colors << " colors" << endl;
}