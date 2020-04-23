#include <iostream>
#include "PNMPicture.h"

int main() {
    PNMPicture pic = PNMPicture("cat.pgm");
//    pic.fillWithGradient();
    pic.dither(DitherAlgo::SIERRA, 1);
    pic.write("new.pgm");
}
