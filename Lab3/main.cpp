#include <iostream>
#include "PNMPicture.h"

int main() {
    PNMPicture pic = PNMPicture("output.pgm");
    pic.fillWithGradient();
    pic.write("new.pgm");
}
