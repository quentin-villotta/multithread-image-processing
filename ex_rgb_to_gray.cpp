#include <iostream>
#include <cstdint>

#include "Bitmap24.h"

int main(int argc, char *argv[])
{
	if(argc < 3) {
		cout << "Usage: ./a.out input.bmp output.bmp\n";
		return 1;
	}

	Bitmap24 in(argv[1]);
	Bitmap24 out(in.height, in.width);

	double filter[] = {0.2989, 0.5870, 0.1140};

	// RGB to grayscale
	for(unsigned int i = 0; i < in.size; i++) {
		uint8_t r = in.pixels[3*i+2];
		uint8_t g = in.pixels[3*i+1];
		uint8_t b = in.pixels[3*i];

		double tmp = filter[0]*r + filter[1]*g + filter[2]*b;

		out.pixels[3*i+2] = (uint8_t)tmp;
		out.pixels[3*i+1] = (uint8_t)tmp;
		out.pixels[3*i] = (uint8_t)tmp;
	}
	
	out.write(argv[2]);

	return 0;
}
