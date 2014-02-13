// Proof of concept for LinearFilter
#include <iostream>
#include <cstdint>

#include "Bitmap24.h"
#include "LinearFilter.h"

int main(int argc, char *argv[])
{
	if(argc < 3) {
		cout << "Usage: ./a.out input.bmp output.bmp\n";
		return 1;
	}

	Bitmap24 in(argv[1]);
	Bitmap24 out(in.height, in.width);

	// const int SIZE_NEIGHBORS = 1;
	// double filter[3 * SIZE_NEIGHBORS] = ;

	double t[] =
	{
	    -1, -1, -1,
	    -1,  8, -1,
	    -1, -1, -1
	};
	LinearFilter filter(3, 3, t);

	// Apply filter
	for(uint32_t pixel = 1; pixel <= in.size; pixel++) {
		
		int i_pixel = pixel / in.width + 1;
		int j_pixel = pixel % in.width;
		double r_sum, g_sum, b_sum = 0.0;

		// filter.width and filter.height must be odd
		int w = filter.width / 2;
		int h = filter.height / 2;
		for(int i_neigh = -h; i_neigh <= h; i_neigh++) {
			for(int j_neigh = -w; j_neigh <= h; j_neigh++) {
				int i_neigh_img = i_pixel - i_neigh * in.width;
				int j_neigh_img = j_pixel - j_neigh * in.height;

				if(i_neigh_img <= 0 || i_neigh_img > in.height || j_neigh_img <= 0 || j_neigh_img > in.width)
					continue;

				int pos_neigh_img = (i_neigh_img - 1) * in.width + j_neigh_img;
				uint8_t r_neigh = in.pixels[3*(pos_neigh_img-1)+2];
				uint8_t g_neigh = in.pixels[3*(pos_neigh_img-1)+1];
				uint8_t b_neigh = in.pixels[3*(pos_neigh_img-1)];

				int pos_neigh_filter = (i_neigh + h) * filter.width + j_neigh + h;
				r_sum += filter.weights[pos_neigh_filter] * r_neigh;
				g_sum += filter.weights[pos_neigh_filter] * g_neigh;
				b_sum += filter.weights[pos_neigh_filter] * b_neigh;
			}
		}

		out.pixels[3*(pixel-1)+2] = (uint8_t) r_sum;
		out.pixels[3*(pixel-1)+1] = (uint8_t) g_sum;
		out.pixels[3*(pixel-1)] = (uint8_t) b_sum;
	}
	
	out.write(argv[2]);

	return 0;
}
