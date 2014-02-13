#include <cstdint>

struct LinearFilter {
	uint32_t height, width, size;
	double* weights;

	LinearFilter(uint32_t _height, uint32_t _width, double _weights[]) 
	{
		height = _height;
		width = _width;
		size = height * width;
		weights = new double[height * width];
		for(uint32_t i = 0; i < size; i++)
			weights[i] = _weights[i];
	}

	~LinearFilter()
	{
		delete[] weights;
	}

};
