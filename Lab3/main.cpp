#include <iostream>
#include "PNMPicture.h"

int main() {
    PNMPicture pic = PNMPicture("LennaGS.pgm");
//    pic.fillWithGradient();
    pic.dither(DitherAlgo::FLOYD_STEINBERG, 4);
    pic.write("new.pgm");
}
