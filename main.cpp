// Proof of concept for LinearFilter
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <algorithm>

#include "Bitmap24.h"
#include "LinearFilter.h"

using namespace std;

void apply(const Bitmap24& in, Bitmap24& out, const LinearFilter& filter, uint32_t pixel)
{
	int w = filter.width / 2;
	int h = filter.height / 2;

	int i_pixel = pixel / in.width + 1;
	int j_pixel = pixel % in.width + 1;
	double r_sum = 0.0, g_sum = 0.0, b_sum = 0.0;

	// filter.width and filter.height must be odd
	for(int i_neigh = -h; i_neigh <= h; i_neigh++) {
		for(int j_neigh = -w; j_neigh <= w; j_neigh++) {

			int i_neigh_img = i_pixel + i_neigh;
			int j_neigh_img = j_pixel + j_neigh;

			if(i_neigh_img <= 0 || i_neigh_img > in.height || j_neigh_img <= 0 || j_neigh_img > in.width)
				continue;

			int pos_neigh_img = (i_neigh_img - 1) * in.width + j_neigh_img;
			uint8_t r_neigh = in.pixels[3*(pos_neigh_img-1)+2];
			uint8_t g_neigh = in.pixels[3*(pos_neigh_img-1)+1];
			uint8_t b_neigh = in.pixels[3*(pos_neigh_img-1)];

			int pos_neigh_filter = filter.width * (i_neigh + h) + j_neigh + w + 1;

			//printf("r_neigh = %d\n", r_neigh);
			r_sum += filter.weights[pos_neigh_filter-1] * r_neigh;
			g_sum += filter.weights[pos_neigh_filter-1] * g_neigh;
			b_sum += filter.weights[pos_neigh_filter-1] * b_neigh;
		}
	}
	//printf("r_sum = %d\n", (uint8_t) min(max(r_sum, 0.0), 255.0));

	out.pixels[3*(pixel-1)+2] = (uint8_t) min(max(r_sum, 0.0), 255.0); 
	out.pixels[3*(pixel-1)+1] = (uint8_t) min(max(g_sum, 0.0), 255.0); 
	out.pixels[3*(pixel-1)] = (uint8_t) min(max(b_sum, 0.0), 255.0);
}

void apply_range(const Bitmap24& in, Bitmap24& out, const LinearFilter& filter, uint32_t pixel_start, uint32_t pixel_end)
{
	for(uint32_t i = pixel_start; i < pixel_end; i++)
		apply(in, out, filter, i);
}


int main(int argc, char *argv[])
{
	if(argc < 3) {
		cout << "Usage: ./a.out input.bmp output.bmp\n";
		return 1;
	}

	Bitmap24 in(argv[1]);
	Bitmap24 out(in.height, in.width);

	double t[] =
	{
	    -1, -1, -1,
	    -1,  8, -1,
	    -1, -1, -1
	};
	// double t[] =
	// {
	//     0.0, 0.0, 0.0,
	//     0.0, 1.0, 0.0,
	//     0.0, 0.0, 0.0
	// };
	// double t[] =
	// {
	//      1,  1,  1,
	//      1, -7,  1,
	//      1,  1,  1
	// };
	// double t[] =
	// {
	//     1.0/9.0, 0, 0, 0, 0, 0, 0, 0, 0,
	//     0, 1.0/9.0, 0, 0, 0, 0, 0, 0, 0,
	//     0, 0, 1.0/9.0, 0, 0, 0, 0, 0, 0,
	//     0, 0, 0, 1.0/9.0, 0, 0, 0, 0, 0,
	//     0, 0, 0, 0, 1.0/9.0, 0, 0, 0, 0,
	//     0, 0, 0, 0, 0, 1.0/9.0, 0, 0, 0,
	//     0, 0, 0, 0, 0, 0, 1.0/9.0, 0, 0,
	//     0, 0, 0, 0, 0, 0, 0, 1.0/9.0, 0,
	//     0, 0, 0, 0, 0, 0, 0, 0, 1.0/9.0,
	// };
	// double t[] =
	// {
	//      0,  0,  0,  0,  0,
	//      0,  0,  0,  0,  0,
	//     -1, -1,  2,  0,  0,
	//      0,  0,  0,  0,  0,
	//      0,  0,  0,  0,  0,
	// };
	LinearFilter filter(3, 3, t);

	apply_range(in, out, filter, 1, in.size + 1);
	
	out.write(argv[2]);

	return 0;
}
