#include "Bitmap24.H"

Bitmap24::Bitmap24(const string& fname)
{
	ifstream file(fname, ios::binary);

	if(file) {
		// Get size (in bytes) of file
		file.seekg(0, ios::end);
		uint64_t length = file.tellg();
		file.seekg(0, ios::beg);
		cout << "lengths = " << length << "\n";

		uint8_t* buffer = new uint8_t[2];
		file.read((char *)buffer, 2);
		uint16_t* bf_type = (uint16_t*) buffer;
		delete[] buffer;
		cout << "bf_type = " << *bf_type << "\n";

		buffer = new uint8_t[4];
		file.read((char *)buffer, 4);
		uint32_t* bf_size = (uint32_t*) buffer;
		delete[] buffer;
		cout << "bf_size = " << *bf_size << "\n";

	}
	else {
		cout << "Error while loading " << fname << " file\n";
	}
}

Bitmap24::Bitmap24(const uint32_t _height, const uint32_t _width)
{
	// height = _height;
	// width = _width;
	// size = height * width;

	// R = new uint8_t[size];
	// G = new uint8_t[size];
	// B = new uint8_t[size];

	// for(unsigned int i = 0; i < size; i++) {
	// 	R[i] = 0;
	// 	G[i] = 0;
	// 	B[i] = 0;
	// }
	;
}

Bitmap24::~Bitmap24()
{
	//delete[] R;
	//delete[] G;
	//delete[] B;
}

void Bitmap24::print_headers()
{
	//cout << "bf_size = " << file_header->bf_size << "\n";
	// cout << "bi_width = " << info_header.bi_width << "\n";
	// cout << "bi_height = " << info_header.bi_height << "\n";
}
