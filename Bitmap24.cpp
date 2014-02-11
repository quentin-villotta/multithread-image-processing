#include "Bitmap24.H"

// Load BMP from file
Bitmap24::Bitmap24(const string& fname)
{
	read(fname);
	height = info_header->bi_height;
	width = info_header->bi_width;
	size = height * width;
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

	file_header = new Bitmap24FileHeader;
	file_header->bf_type = 0x4D42;
	file_header->bf_size = sizeof(Bitmap24FileHeader) + sizeof(Bitmap24InfoHeader) + 3 * size;
	file_header->bf_reserved1 = 0;
	file_header->bf_reserved2 = 0;
	file_header->bf_offbits = sizeof(Bitmap24FileHeader) + sizeof(Bitmap24InfoHeader);

	info_header = new Bitmap24InfoHeader;
	info_header->bi_size = 40;
	info_header->bi_width = width;
	info_header->bi_height = height;
	info_header->bi_planes = 1;
	info_header->bi_bit_count = 24;
	info_header->bi_compression = 0;
	info_header->bi_size_image = 3 * size;
	info_header->bi_x_pels_per_meter = 2835;
	info_header->bi_y_pels_per_meter = 2835;
	info_header->bi_clr_used = 0;
	info_header->bi_clr_important = 0;
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
	cout << "bf_offbits = " << file_header->bf_offbits << "\n";
	cout << "bi_size = " << info_header->bi_size << "\n";
	cout << "bi_width = " << info_header->bi_width << "\n";
	cout << "bi_height = " << info_header->bi_height << "\n";
	cout << "bi_size_image = " << info_header->bi_size_image << "\n";
	cout << "bi_clr_used = " << info_header->bi_clr_used << "\n";
	cout << "bi_clr_important = " << info_header->bi_clr_important << "\n";
}

void Bitmap24::write(const string& fname)
{
	ofstream file(fname, ios::binary);
	file.write((char*)file_header, sizeof(Bitmap24FileHeader));
	file.write((char*)info_header, sizeof(Bitmap24InfoHeader));
	file.write((char*)pixels, 3 * size);
}

void Bitmap24::read(const string& fname)
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
	}
	else {
		cout << "Error while loading " << fname << " file\n";
		exit(EXIT_FAILURE);
	}
}
