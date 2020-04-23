#include <iostream>
#include "PNMPicture.h"

int main() {
    PNMPicture pic = PNMPicture("cat.pgm");
//    pic.fillWithGradient();
    pic.dither(DitherAlgo::FLOYD_STEINBERG, 2);
    pic.write("new.pgm");
}
