#include "Bitmap24.H"

// Load BMP from file
Bitmap24::Bitmap24(const string& fname)
{
	ifstream file(fname, ios::binary);

	if(file) {
		// Header buffers
		uint8_t* buf_headers[2] = {NULL, NULL};

		// Get headers
		buf_headers[0] = new uint8_t[sizeof(Bitmap24FileHeader)];
		buf_headers[1] = new uint8_t[sizeof(Bitmap24InfoHeader)];

		file.read((char*)buf_headers[0], sizeof(Bitmap24FileHeader));
		file.read((char*)buf_headers[1], sizeof(Bitmap24InfoHeader));

		file_header = (Bitmap24FileHeader*) buf_headers[0];
		info_header = (Bitmap24InfoHeader*) buf_headers[1];

		//if(file_header->bf_type != 0x4D42) // not a bmp

		// Get pixels
		pixels = new uint8_t[info_header->bi_size_image];
		// (pixels[3*i], pixels[3*i+1], pixels[3*i+2]) = (B,G,R) of i-th pixel

		file.seekg(file_header->bf_offbits);
		file.read((char*)pixels, info_header->bi_size_image);

		height = info_header->bi_height;
		width = info_header->bi_width;
		size = height * width;
	}
	else {
		cout << "Error while loading " << fname << " file\n";
		exit(EXIT_FAILURE);
	}
}

// Create an empty BMP to be filled
Bitmap24::Bitmap24(const uint32_t _height, const uint32_t _width)
{
	height = _height;
	width = _width;
	size = height * width;

	pixels = new uint8_t[3 * size];
	for(uint32_t i = 0; i < 3 * size; i++)
		pixels[i] = 0;

	//TODO : set headers
}

Bitmap24::~Bitmap24()
{
	delete file_header;
	delete info_header;
	delete[] pixels;
}

void Bitmap24::print_headers()
{
	cout << "bf_size = " << file_header->bf_size << "\n";
	cout << "bi_size = " << info_header->bi_size << "\n";
	cout << "bi_width = " << info_header->bi_width << "\n";
	cout << "bi_height = " << info_header->bi_height << "\n";
	cout << "bi_size_image = " << info_header->bi_size_image << "\n";
}
