#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <iomanip>
#include <cstdlib>

using namespace std;

// See http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm

// struct padding
#pragma pack(push)
#pragma pack(2)

struct Bitmap24FileHeader {
    uint16_t bf_type;
    uint32_t bf_size;
    uint16_t bf_reserved1;
    uint16_t bf_reserved2;
    uint32_t bf_offbits;
};

typedef struct Bitmap24FileHeader Bitmap24FileHeader;

struct Bitmap24InfoHeader {
    uint32_t bi_size;
    uint32_t bi_width;
    uint32_t bi_height;
    uint16_t bi_planes;
    uint16_t bi_bit_count;
    uint32_t bi_compression;
    uint32_t bi_size_image;
    uint32_t bi_x_pels_per_meter;
    uint32_t bi_y_pels_per_meter;
    uint32_t bi_clr_used;
    uint32_t bi_clr_important;
};

typedef struct Bitmap24InfoHeader Bitmap24InfoHeader;

#pragma pack(pop)

class Bitmap24 {

public:
    // Arrays for storing (R,G,B) values
    uint8_t* pixels;

    // Dimensions
    uint32_t height;
    uint32_t width;
    uint32_t size; // size = height * width (total number of pixels)

    // Bitmap headers
    Bitmap24FileHeader* file_header;
    Bitmap24InfoHeader* info_header;

    Bitmap24(const string& fname);
    Bitmap24(const uint32_t _height, const uint32_t _width, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0); 
    ~Bitmap24();

    void print_headers();
    void write(const string& fname);
    void read(const string& fname);
};
