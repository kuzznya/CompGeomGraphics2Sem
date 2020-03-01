#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include "PNMPicture.h"

using namespace std;

typedef unsigned char uchar;

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

    PNMPicture picture;
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
