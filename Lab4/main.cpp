#include <iostream>
#include "PNMPicture.h"

ColorSpace getColorSpaceFromName(const std::string& colorSpaceName) {
    ColorSpace result;
    if (colorSpaceName == "RGB")
        result = ColorSpace::CS_RGB;
    else if (colorSpaceName == "HSL")
        result = ColorSpace::CS_HSL;
    else if (colorSpaceName == "HSV")
        result = ColorSpace::CS_HSV;
    else if (colorSpaceName == "YCbCr.601")
        result = ColorSpace::CS_YCbCr_601;
    else if (colorSpaceName == "YCbCr.709")
        result = ColorSpace::CS_YCbCr_709;
    else if (colorSpaceName == "YCoCg")
        result = ColorSpace::CS_YCoCg;
    else if (colorSpaceName == "CMY")
        result = ColorSpace::CS_CMY;
    else
        throw std::runtime_error("No color space found with such name");
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 11) {
        std::cerr << "Incorrect count of args" << std::endl;
        return 1;
    }

    std::string inputFile, outputFile, buffer;
    bool inputMultiple = false, outputMultiple = false;
    ColorSpace from = CS_RGB, to = CS_RGB;
    for (int i = 1; i < argc; i++) {
        buffer = argv[i];
        if (buffer == "-f") {
            i++;
            buffer = argv[i];

            try {
                from = getColorSpaceFromName(buffer);
            } catch (std::runtime_error &ex) {
                std::cerr << "Incorrect color space name" << std::endl;
                return 1;
            }
        } else if (buffer == "-t") {
            i++;
            buffer = argv[i];

            try {
                to = getColorSpaceFromName(buffer);
            } catch (std::runtime_error &ex) {
                std::cerr << "Incorrect color space name" << std::endl;
                return 1;
            }
        } else if (buffer == "-i") {
            i++;
            buffer = argv[i];

            if (buffer == "1")
                inputMultiple = false;
            else if (buffer == "3")
                inputMultiple = true;
            else {
                std::cerr << "Incorrect count of input files" << std::endl;
                return 1;
            }
            i++;
            buffer = argv[i];
            inputFile = buffer;
        } else if (buffer == "-o") {
            i++;
            buffer = argv[i];

            if (buffer == "1")
                outputMultiple = false;
            else if (buffer == "3")
                outputMultiple = true;
            else {
                std::cerr << "Incorrect count of input files" << std::endl;
                return 1;
            }
            i++;
            buffer = argv[i];
            outputFile = buffer;
        }
    }

    PNMPicture picture;
    try {
        if (inputMultiple) {
            std::string inputFile1 = inputFile;
            inputFile1.insert(inputFile1.size() - 4, "_1");
            std::string inputFile2 = inputFile;
            inputFile2.insert(inputFile2.size() - 4, "_2");
            std::string inputFile3 = inputFile;
            inputFile3.insert(inputFile1.size() - 4, "_3");
            picture.read(inputFile1, inputFile2, inputFile3);
        } else
            picture.read(inputFile);
    } catch (std::exception& ex) {
        std::cerr << "Error while trying to read picture" << std::endl;
        return 1;
    }

    picture.setCurrentColorSpace(from);

    try {
        picture.convert(from, to);
    } catch (std::exception& ex) {
        std::cerr << "Error while trying to convert" << std::endl;
    }

    try {
        if (outputMultiple) {
            std::string outputFile1 = outputFile;
            outputFile1.insert(outputFile1.size() - 4, "_1");
            std::string outputFile2 = outputFile;
            outputFile2.insert(outputFile2.size() - 4, "_2");
            std::string outputFile3 = outputFile;
            outputFile3.insert(outputFile3.size() - 4, "_3");
            picture.write(outputFile1, outputFile2, outputFile3);
        } else
            picture.write(outputFile);
    } catch (std::exception& ex) {
        std::cerr << "Error while trying to write picture" << std::endl;
        return 1;
    }
}
