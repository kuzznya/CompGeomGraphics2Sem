#include "PNMPicture.h"

#include <cmath>

PNMPicture::PNMPicture() = default;

PNMPicture::PNMPicture(const std::string& filename) {
    read(filename);
}

PNMPicture::PNMPicture(const std::string& file1Name, const std::string& file2Name, const std::string& file3Name) {
    read(file1Name, file2Name, file3Name);
}

void PNMPicture::read(const std::string& fileName) {
    std::ifstream inputFile(fileName, std::ios::binary);
    if (!inputFile.is_open())
        throw FileIOException();
    read(inputFile);
    inputFile.close();
}

void PNMPicture::read(std::ifstream& inputFile) {
    readP6(inputFile);
}

void PNMPicture::read(const std::string &file1Name,
                      const std::string &file2Name, const std::string &file3Name) {
    std::ifstream inputFile1(file1Name, std::ios::binary);
    if (!inputFile1.is_open())
        throw FileIOException();

    std::ifstream inputFile2(file2Name, std::ios::binary);
    if (!inputFile2.is_open())
        throw FileIOException();

    std::ifstream inputFile3(file3Name, std::ios::binary);
    if (!inputFile3.is_open())
        throw FileIOException();

    read(inputFile1, inputFile2, inputFile3);

    inputFile1.close();
    inputFile2.close();
    inputFile3.close();
}

void PNMPicture::read(std::ifstream &inputFile1,
                      std::ifstream &inputFile2, std::ifstream &inputFile3) {
    readP5(inputFile1, A);
    readP5(inputFile2, B);
    readP5(inputFile3, C);
}

void PNMPicture::write(const std::string& fileName) {
    std::ofstream outputFile(fileName, std::ios::binary);
    if (!outputFile.is_open())
        throw FileIOException();
    write(outputFile);
    outputFile.close();
}

void PNMPicture::write(std::ofstream& outputFile) {
    writeP6(outputFile);
}

void PNMPicture::write(const std::string& file1Name,
                       const std::string& file2Name, const std::string& file3Name) {
    std::ofstream outputFile1(file1Name, std::ios::binary);
    if (!outputFile1.is_open())
        throw FileIOException();

    std::ofstream outputFile2(file2Name, std::ios::binary);
    if (!outputFile2.is_open())
        throw FileIOException();

    std::ofstream outputFile3(file3Name, std::ios::binary);
    if (!outputFile3.is_open())
        throw FileIOException();

    write(outputFile1, outputFile2, outputFile3);

    outputFile1.close();
    outputFile2.close();
    outputFile3.close();
}

void PNMPicture::write(std::ofstream& outputFile1,
                       std::ofstream& outputFile2, std::ofstream& outputFile3) {
    writeP5(outputFile1, A);
    writeP5(outputFile2, B);
    writeP5(outputFile3, C);
}

void PNMPicture::convert(ColorSpace from, ColorSpace to) {
    curColorSpace = from;

    switch (from) {
        case CS_HSL:
            convertFromHSL();
            break;
        case CS_HSV:
            convertFromHSV();
            break;
        case CS_YCbCr_601:
            convertFromYCbCr601();
            break;
        case CS_YCbCr_709:
            convertFromYCbCr709();
            break;
        case CS_YCoCg:
            convertFromYCoCg();
            break;
        case CS_CMY:
            convertFromCMY();
            break;
        case CS_RGB:
            break;
    }

    curColorSpace = ColorSpace::CS_RGB;

    switch (to) {
        case CS_HSL:
            convertToHSL();
            break;
        case CS_HSV:
            convertToHSV();
            break;
        case CS_YCbCr_601:
            convertToYCbCr601();
            break;
        case CS_YCbCr_709:
            convertToYCbCr709();
            break;
        case CS_YCoCg:
            convertToYCoCg();
            break;
        case CS_CMY:
            convertToCMY();
            break;
        case CS_RGB:
            break;
    }

    curColorSpace = to;
}

void PNMPicture::setCurrentColorSpace(ColorSpace cs) {
    curColorSpace = cs;
}

void PNMPicture::printInfo() {
    std::cout << "PNM - Portable Any Map" << std::endl;
    std::cout << "Format: P6 - colored image" << std::endl;
    std::cout << width << "x" << height << std::endl;
    std::cout << colors << " colors" << std::endl;
}

void PNMPicture::readP5(std::ifstream& inputFile, ColorChannelName colorToRead) {
    char P;
    inputFile >> P;
    if (P != 'P')
        throw UnsupportedFormatException();

    short format;
    inputFile >> format;
    if (format != 5)
        throw UnsupportedFormatException();

    inputFile >> width >> height;
    inputFile >> colors;
    if (colors != 255)
        throw UnsupportedFormatException();

    inputFile.get();

    uchar* buffer = new uchar[width * height];
    inputFile.read((char*) buffer, width * height);
    for (int i = 0; i < width * height; i++) {
        switch (colorToRead) {
            case A:
                data[i].A = buffer[i];
                break;
            case B:
                data[i].B = buffer[i];
                break;
            case C:
                data[i].C = buffer[i];
                break;
        }
    }

    delete[] buffer;
}

void PNMPicture::readP6(std::ifstream& inputFile) {
    char P;
    inputFile >> P;
    if (P != 'P')
        throw UnsupportedFormatException();

    short format;
    inputFile >> format;
    if (format != 6)
        throw UnsupportedFormatException();

    inputFile >> width >> height;
    inputFile >> colors;
    if (colors != 255)
        throw UnsupportedFormatException();

    inputFile.get();

    data.resize(width * height);

    uchar* buffer = new uchar[width * height * 3];
    inputFile.read((char*) buffer, width * height * 3);
    if (inputFile.fail())
        throw FileIOException();

    for (int i = 0; i < width * height; i++) {
        data[i].A = buffer[i * 3 + 0];
        data[i].B = buffer[i * 3 + 1];
        data[i].C = buffer[i * 3 + 2];
    }

    delete[] buffer;
}

void PNMPicture::writeP5(std::ofstream& outputFile, ColorChannelName colorToWrite) {
    outputFile << "P5" << '\n';
    outputFile << width << ' ' << height << '\n';
    outputFile << colors << '\n';
    uchar* buffer = new uchar[width * height];
    for (int i = 0; i < width * height; i++) {
        switch (colorToWrite) {
            case A:
                buffer[i] = data[i].A;
                break;
            case B:
                buffer[i] = data[i].B;
                break;
            case C:
                buffer[i] = data[i].C;
                break;
            default:
                throw ExecutionException();
        }
    }
    outputFile.write((char*) buffer, width * height);
    delete[] buffer;
}

void PNMPicture::writeP6(std::ofstream& outputFile) {
    outputFile << "P6" << '\n';
    outputFile << width << ' ' << height << '\n';
    outputFile << colors << '\n';

    uchar* buffer = new uchar[width * height * 3];
    for (int i = 0; i < width * height; i++) {
        buffer[i * 3 + 0] = data[i].A;
        buffer[i * 3 + 1] = data[i].B;
        buffer[i * 3 + 2] = data[i].C;
    }
    outputFile.write((char*) buffer, width * height * 3);
    delete[] buffer;
}

void PNMPicture::convertFromHSL() {
    auto f = [](double H, double S, double L, int n) -> double {
        double k = fmod((n + H / 30.0), 12);
        double alpha = S * std::min(L, 1 - L);
        return L - alpha * std::max(-1.0, std::min(std::min(k - 3.0, 9.0 - k), 1.0));
    };

    for (auto& value : data) {
        double H = value.A / 255.0 * 360.0;
        double S = value.B / 255.0;
        double L = value.C / 255.0;

        value.A = f(H, S, L, 0) * 255;
        value.B = f(H, S, L, 8) * 255;
        value.C = f(H, S, L, 4) * 255;
    }
}

void PNMPicture::convertFromHSV() {
    auto f = [](double H, double S, double V, int n) -> double {
        double k = fmod((n + H / 60.0), 6);
        return V - V * S * std::max(0.0, std::min(std::min(k, 4 - k), 1.0));
    };

    for (auto& value : data) {
        double H = value.A / 255.0 * 360.0;
        double S = value.B / 255.0;
        double V = value.C / 255.0;

        value.A = f(H, S, V, 5);
        value.B = f(H, S, V, 3);
        value.C = f(H, S, V, 1);
    }
}

void PNMPicture::convertFromYCbCr601() {
    double Kr = 0.299;
    double Kg = 0.587;
    double Kb = 0.114;

    for (auto& value : data) {
        double Y = value.A / 255.0;
        double Cb = value.B / 255.0 - 0.5;
        double Cr = value.C / 255.0 - 0.5;

        double R = (Y + Cr * (2.0 - 2.0 * Kr));
        double G = (Y - (Kb / Kg) * (2.0 - 2.0 * Kb) * Cb - (Kr / Kg) * (2.0 - 2.0 * Kr) * Cr);
        double B = (Y + (2.0 - 2.0 * Kb) * Cb);

        R = std::min(std::max(R, 0.0), 1.0);
        G = std::min(std::max(G, 0.0), 1.0);
        B = std::min(std::max(B, 0.0), 1.0);

        value.A = R * 255;
        value.B = G * 255;
        value.C = B * 255;
    }
}

void PNMPicture::convertFromYCbCr709() {
    double Kr = 0.0722;
    double Kg = 0.2126;
    double Kb = 0.7152;

    for (auto& value : data) {
        double Y = value.A / 255.0;
        double Cb = value.B / 255.0 - 0.5;
        double Cr = value.C / 255.0 - 0.5;

        double R = (Y + Cr * (2.0 - 2.0 * Kr));
        double G = (Y - (Kb / Kg) * (2.0 - 2.0 * Kb) * Cb - (Kr / Kg) * (2.0 - 2.0 * Kr) * Cr);
        double B = (Y + (2.0 - 2.0 * Kb) * Cb);

        R = std::min(std::max(R, 0.0), 1.0);
        G = std::min(std::max(G, 0.0), 1.0);
        B = std::min(std::max(B, 0.0), 1.0);

        value.A = R * 255;
        value.B = G * 255;
        value.C = B * 255;
    }
}

void PNMPicture::convertFromYCoCg() {
    for (auto& value : data) {
        double Y = value.A / 255.0;
        double Co = value.B / 255.0 - 0.5;
        double Cg = value.C / 255.0 - 0.5;

        double R = Y + Co - Cg;
        double G = Y + Cg;
        double B = Y - Co - Cg;

        R = std::min(std::max(R, 0.0), 1.0);
        G = std::min(std::max(G, 0.0), 1.0);
        B = std::min(std::max(B, 0.0), 1.0);

        value.A = R * 255;
        value.B = G * 255;
        value.C = B * 255;
    }
}

void PNMPicture::convertFromCMY() {
    for (auto& value : data) {
        double C = value.A / 255.0;
        double M = value.B / 255.0;
        double Y = value.C / 255.0;

        double R = 1 - C;
        double G = 1 - M;
        double B = 1 - Y;

        value.A = R * 255;
        value.B = G * 255;
        value.C = B * 255;
    }
}


void PNMPicture::convertToHSL() {
    for (auto& value : data) {
        double R = value.A / 255.0;
        double G = value.B / 255.0;
        double B = value.C / 255.0;

        double Xmax = std::max(std::max(R, G), B);
        double V = Xmax;
        double Xmin = std::min(std::min(R, G), B);
        double C = Xmax - Xmin;
        double L = (Xmax + Xmin) / 2.0;

        double H = 0;
        if (C == 0)
            H = 0;
        else if (V == R)
            H = 60.0 * (0.0 + (G - B) / C);
        else if (V == G)
            H = 60.0 * (2.0 + (B - R) / C);
        else if (V == B)
            H = 60.0 * (4.0 + (R - G) / C);

        double S;
        if (L < 0.000001 || L > 0.00009)
            S = 0;
        else
            S = (V - L) / std::min(L, 1.0 - L);

        value.A = H * 255;
        value.B = S * 255;
        value.C = L * 255;
    }
}

void PNMPicture::convertToHSV() {
    for (auto& value : data) {
        double R = value.A / 255.0;
        double G = value.B / 255.0;
        double B = value.C / 255.0;

        double Xmax = std::max(std::max(R, G), B);
        double V = Xmax;
        double Xmin = std::min(std::min(R, G), B);
        double C = Xmax - Xmin;
        double L = (Xmax + Xmin) / 2.0;

        double H = 0;
        if (C == 0)
            H = 0;
        else if (V == R)
            H = 60.0 * (0.0 + (G - B) / C);
        else if (V == G)
            H = 60.0 * (2.0 + (B - R) / C);
        else if (V == B)
            H = 60.0 * (4.0 + (R - G) / C);

        double S;
        if (V == 0)
            S = 0;
        else
            S = C / V;

        value.A = H * 255;
        value.B = S * 255;
        value.C = V * 255;
    }
}

void PNMPicture::convertToYCbCr601() {
    double Kr = 0.299;
    double Kg = 0.587;
    double Kb = 0.114;

    for (auto& value : data) {
        double R = value.A / 255.0;
        double G = value.B / 255.0;
        double B = value.C / 255.0;

        double Y = Kr * R + Kg * G + Kb * B;
        double Cb = 0.5 * ((B - Y) / (1.0 - Kb));
        double Cr = 0.5 * ((R - Y) / (1.0 - Kr));

        value.A = Y * 255;
        value.B = (Cb + 0.5) * 255;
        value.C = (Cr + 0.5) * 255;
    }
}

void PNMPicture::convertToYCbCr709() {
    double Kr = 0.0722;
    double Kg = 0.2126;
    double Kb = 0.7152;

    for (auto& value : data) {
        double R = value.A / 255.0;
        double G = value.B / 255.0;
        double B = value.C / 255.0;

        double Y = Kr * R + Kg * G + Kb * B;
        double Cb = 0.5 * ((B - Y) / (1.0 - Kb));
        double Cr = 0.5 * ((R - Y) / (1.0 - Kr));

        value.A = Y * 255;
        value.B = (Cb + 0.5) * 255;
        value.C = (Cr + 0.5) * 255;
    }
}

void PNMPicture::convertToYCoCg() {
    for (auto& value : data) {
        double R = value.A / 255.0;
        double G = value.B / 255.0;
        double B = value.C / 255.0;
        double Y = R / 4 + G / 2 + B / 4;
        double Co = R / 2 - B / 2;
        double Cg = -R / 4 + G / 2 - B / 4;
        value.A = Y * 255.0;
        value.B = (Co + 0.5) * 255;
        value.C = (Cg + 0.5) * 255;
    }
}

void PNMPicture::convertToCMY() {
    for (auto& value : data) {
        double R = value.A / 255.0;
        double G = value.B / 255.0;
        double B = value.C / 255.0;

        double C = 1 - R;
        double M = 1 - G;
        double Y = 1 - B;

        value.A = C * 255;
        value.B = M * 255;
        value.C = Y * 255;
    }
}
