#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

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
        inputFile.read((char*) &dataP5[0], width * height);
    }

    void readP6(ifstream& inputFile) {
        dataP5.clear();
        dataP6.resize(width * height);

        uchar* buffer = new uchar[width * height * 3];
        inputFile.read((char*) buffer, width * height * 3);

        for (int i = 0; i < width * height; i++) {
            dataP6[i].R = buffer[i * 3 + 0];
            dataP6[i].G = buffer[i * 3 + 1];
            dataP6[i].B = buffer[i * 3 + 2];
        }

        delete[] buffer;
    }

    void writeP5(ofstream& outputFile) {
        outputFile.write((char*) &dataP5[0], width * height);
    }

    void writeP6(ofstream& outputFile) {
        uchar* buffer = new uchar[width * height * 3];
        for (int i = 0; i < width * height; i++) {
            buffer[i * 3 + 0] = dataP6[i].R;
            buffer[i * 3 + 1] = dataP6[i].G;
            buffer[i * 3 + 2] = dataP6[i].B;
        }
        outputFile.write((char*) buffer, width * height * 3);
        delete[] buffer;
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

    void mirrorVerticalP5() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width / 2; j++) {
                swap(dataP5[i * width + j], dataP5[i * width + width - j - 1]);
            }
        }
    }

    void mirrorVerticalP6() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width / 2; j++) {
                swap(dataP6[i * width + j], dataP6[i * width + width - j - 1]);
            }
        }
    }

    void turn90P5() {
        vector<uchar> turnedDataP5(width * height);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                turnedDataP5[(height - i - 1) + j * height] = dataP5[i * width + j];
            }
        }

        dataP5.clear();
        dataP5 = turnedDataP5;
    }

    void turn90P6() {
        vector<RGB> turnedDataP6(width * height);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                turnedDataP6[(height - i - 1) + j * height] = dataP6[i * width + j];
            }
        }

        dataP6.clear();
        dataP6 = turnedDataP6;
    }

    void turn270P5() {
        vector<uchar> turnedDataP5(width * height);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                turnedDataP5[height * (width - j - 1) + i] = dataP5[i * width + j];
            }
        }

        dataP5.clear();
        dataP5 = turnedDataP5;
    }

    void turn270P6() {
        vector<RGB> turnedDataP6(width * height);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                turnedDataP6[height * (width - j - 1) + i] = dataP6[i * width + j];
            }
        }

        dataP6.clear();
        dataP6 = turnedDataP6;
    }

public:

    PNMFile() = default;

    PNMFile(string fileName) {
        read(fileName);
    }

    void read(string fileName) {
        ifstream inputFile(fileName, ios::binary);
        if (!inputFile.is_open())
            throw FileIOException();
        read(inputFile);
        inputFile.close();
    }

    void read(ifstream& inputFile) {
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

    void write(string fileName) {
        ofstream outputFile(fileName, ios::binary);
        if (!outputFile.is_open())
            throw FileIOException();
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
            default:
                throw ExecutionException();
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
        if (format == 5)
            mirrorVerticalP5();
        else
            mirrorVerticalP6();
    }

    void turn90() {
        if (format == 5)
            turn90P5();
        else
            turn90P6();
        swap(width, height);
    }

    void turn270() {
        if (format == 5)
            turn270P5();
        else
            turn270P6();
        swap(width, height);
    }

    void printInfo() {
        cout << "PNM - Portable Any Map" << endl;
        cout << "Format: P" << format << " - ";
        if (format == 5)
            cout << "grayscale image" << endl;
        else
            cout << "colored image" << endl;
        cout << width << "x" << height << endl;
        cout << colors << " colors" << endl;
    }

};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Incorrect count of arguments" << endl;
        cout << "Arguments format: CompGeomGraphics <input file name> <output file name> <command>" << endl;
        cout << "Commands:" << endl;
        cout << "\t0 - inverse colors\n\t1 - mirror horizontally\n\t2 - mirror vertically"
                "\n\t3 - turn clockwise\n\t4 - turn counterclockwise" << endl;
        return 0;
    }

    string inputFileName = argv[1], outputFileName = argv[2];
    Command command;
    try {
        command = (Command) stoi(argv[3], nullptr);
        if (command < 0 || command > 4)
            throw invalid_argument("Invalid command");
    } catch (invalid_argument& ex) {
        cout << "Invalid command " << argv[3] << endl;
        return -2;
    }

    PNMFile picture;
    try {
        unsigned startTime = clock();
        picture.read(inputFileName);
        cout << "Picture read in " << clock() - startTime << " clock ticks" << endl;
    } catch (UnsupportedFormatException& ex) {
        cout << "Unsupported PNM file format" << endl;
        return -1;
    } catch (exception& ex) {
        cout << "Error while trying to read file " << inputFileName << endl;
        return -1;
    }
    picture.printInfo();

    cout << "Executing the command..." << endl;
    unsigned startTime = clock();
    try {
        picture.execute(command);
    } catch (ExecutionException& ex) {
        cout << "Failed to execute the command" << endl;
        return -2;
    }
    cout << "Done in " << clock() - startTime << " clock ticks" << endl;

    cout << "Saving changes..." << endl;

    try {
        startTime = clock();
        picture.write(outputFileName);
        cout << "Picture saved in " << clock() - startTime << " clock ticks" << endl;
    } catch (FileIOException& ex) {
        cout << "Error while trying to write data to file " + outputFileName;
        return -1;
    }

    cout << "Picture saved to " << outputFileName << endl;

    return 0;
}
