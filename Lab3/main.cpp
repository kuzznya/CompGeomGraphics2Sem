#include <iostream>
#include "PNMPicture.h"

int main() {
    PNMPicture pic = PNMPicture("cat.pgm");
//    pic.fillWithGradient();
    pic.dither(DitherAlgo::SIERRA, 4);
    pic.write("new.pgm");
}