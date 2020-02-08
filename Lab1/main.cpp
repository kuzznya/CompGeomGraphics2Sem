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

class PNMFile {
private:
    short format;
    int width, height;
    int colors;
    vector<uchar> dataP5;
    vector<RGB> dataP6;

    void readP5(ifstream& inputFile) {
        dataP6.clear();
        dataP5.resize(width * height);
        for (int i = 0; i < width * height; i++) {
            inputFile.read((char*) &dataP5[i], 1);
        }
    }

    void readP6(ifstream& inputFile) {
        dataP5.clear();
        dataP6.resize(width * height);
        uchar* buffer = new uchar[3];
        for (int i = 0; i < width * height; i++) {
            uchar r, g, b;
            inputFile.read((char*) buffer, 3);
            r = buffer[0];
            g = buffer[1];
            b = buffer[2];
            dataP6[i] = {r, g, b};
        }
        delete[] buffer;
    }

    void writeP5(ofstream& outputFile) {
        for (int i = 0; i < width * height; i++) {
            outputFile << dataP5[i];
        }
    }

    void writeP6(ofstream& outputFile) {
        for (int i = 0; i < width * height; i++) {
            outputFile << dataP6[i].R << dataP6[i].G << dataP6[i].B;
        }
    }

    void inverseP5() {
        for (int i = 0; i < width * height; i++) {
            dataP5[i] = colors - dataP5[i];
        }
    }

    void inverseP6() {
        for (int i = 0; i < width * height; i++) {
            dataP6[i].R = colors - dataP6[i].R;
            dataP6[i].G = colors - dataP6[i].G;
            dataP6[i].B = colors - dataP6[i].B;
        }
    }

    void mirrorHorizontalP5() {
        for (int j = 0; j < width; j++) {
            for (int i = 0; i < height / 2; i++) {
                swap(dataP5[i * width + j], dataP5[(height - 1) * width - i * width + j]);
            }
        }
    }

    void mirrorHorizontalP6() {
        for (int j = 0; j < width; j++) {
            for (int i = 0; i < height / 2; i++) {
                swap(dataP6[i * width + j], dataP6[(height - 1) * width - i * width + j]);
            }
        }
    }

public:

    PNMFile() = default;

    PNMFile(string fileName) {
        ifstream inputFile(fileName);
        read(inputFile);
        inputFile.close();
    }

    void read(string fileName) {
        ifstream inputFile(fileName, ios::binary);
        read(inputFile);
        inputFile.close();
    }

    void read(ifstream& inputFile) {
        char P;
        inputFile >> P;
        inputFile >> format;
        inputFile >> width >> height;
        inputFile >> colors;
        char NLSymbol;
        inputFile.read(&NLSymbol, 1);
        if (format == 5)
            readP5(inputFile);
        else
            readP6(inputFile);
    }

    void write(string fileName) {
        ofstream outputFile(fileName, ios::binary);
        write(outputFile);
        outputFile.close();
    }

    void write(ofstream& outputFile) {
        outputFile << "P" << format << '\n';
        outputFile << width << ' ' << height << '\n';
        outputFile << colors << '\n';
        if (format == 5)
            writeP5(outputFile);
        else
            writeP6(outputFile);
    }

    void execute(Command cmd) {
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
        }
    }

    void inverse() {
        if (this->format == 5)
            inverseP5();
        else
            inverseP6();
    }

    void mirrorHorizontal() {
        if (format == 5)
            mirrorHorizontalP5();
        else
            mirrorHorizontalP6();
    }

    void mirrorVertical() {

    }

    void turn90() {

    }

    void turn270() {

    }

};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Incorrect count of arguments" << endl;
        return -1;
    }

    string inputFileName = argv[1], outputFileName = argv[2];
    Command command = (Command) stoi(argv[3], nullptr);

    PNMFile picture(inputFileName);
    picture.execute(command);

    picture.write(outputFileName);

    return 0;
}
