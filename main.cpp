#include <iostream>
#include <cstdint>

#include "Bitmap24.h"

int main(int argc, char *argv[])
{
	if(argc < 3) {
		cout << "Usage: ./a.out input.bmp output.bmp\n";
		return 1;
	}
	//std::cout << "sizeof(Bitmap24FileHeader) = " << sizeof(Bitmap24FileHeader) << " bytes\n";
	//std::cout << "sizeof(Bitmap24InfoHeader) = " << sizeof(Bitmap24InfoHeader) << " bytes\n";

	Bitmap24 in(argv[1]);
	Bitmap24 out(in.height, in.width);

	// RGB to grayscale
	for(unsigned int i = 0; i < in.size; i++) {
		uint8_t r = (uint8_t)in.pixels[3*i+2];
		uint8_t g = (uint8_t)in.pixels[3*i+1];
		uint8_t b = (uint8_t)in.pixels[3*i];

		out.pixels[3*i+2] = (uint8_t)(0.2989 * r);
		out.pixels[3*i+1] = (uint8_t)(0.5870 * g);
		out.pixels[3*i] = (uint8_t)(0.1140 * b);
	}

	out.write(argv[2]);

	return 0;
}
