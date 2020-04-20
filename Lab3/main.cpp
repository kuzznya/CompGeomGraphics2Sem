#include <iostream>
#include "PNMPicture.h"

int main() {
    PNMPicture pic = PNMPicture("output.pgm");
    pic.fillWithGradient();
    pic.dither(DitherAlgo::NONE, 4);
    pic.write("new.pgm");
}
