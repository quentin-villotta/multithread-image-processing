// Proof of concept for LinearFilter
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <algorithm>
#include <vector>
#include <thread>

#include "Bitmap24.h"
#include "LinearFilter.h"

using namespace std;

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
     
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}

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

	out.pixels[3*(pixel-1)+2] = (uint8_t) min(max(filter.factor * r_sum + filter.bias, 0.0), 255.0); 
	out.pixels[3*(pixel-1)+1] = (uint8_t) min(max(filter.factor * g_sum + filter.bias, 0.0), 255.0); 
	out.pixels[3*(pixel-1)] = (uint8_t) min(max(filter.factor * b_sum + filter.bias, 0.0), 255.0);
}

void apply_range(const Bitmap24& in, Bitmap24& out, const LinearFilter& filter, uint32_t pixel_start, uint32_t pixel_end)
{
	for(uint32_t i = pixel_start; i < pixel_end; i++)
		apply(in, out, filter, i);
}


int main(int argc, char *argv[])
{
	if(argc < 3) {
		cout << "Usage: ./a.out input.bmp output.bmp num_thread\n";
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
	LinearFilter filter(3, 3, t, 1.0, 0.0);

	int num_thread = atoi(argv[3]);
	vector<thread> thread_pool;

	int job_size = in.size / num_thread;

	struct timeval start, end;
	gettimeofday(&start , NULL);

	for(int i = 0; i < num_thread - 1; i++) {
		int start = i * job_size + 1;
		thread_pool.push_back(thread(apply_range, ref(in), ref(out), ref(filter), start, start + job_size));
	}

	// use the main thread to do park of the work
	apply_range(in, out, filter, job_size * (num_thread - 1) + 1, in.size);

	// Join
	for(int i = 0; i < num_thread - 1; i++)
		thread_pool[i].join();

	gettimeofday(&end , NULL);

	cout << "Time: " << time_diff(start, end) / 1000.0 << "ms\n";

	// Monothread equivalent
	// apply_range(in, out, filter, 1, in.size + 1);
	
	out.write(argv[2]);

	return 0;
}
