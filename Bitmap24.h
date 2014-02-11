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
    //unsigned int max_col_val;

    // Bitmap headers
    Bitmap24FileHeader* file_header;
    Bitmap24InfoHeader* info_header;

    Bitmap24(const string& fname);
    Bitmap24(const uint32_t _height, const uint32_t _width); 
    ~Bitmap24();

    void print_headers();

    // Read the BMP image from fname
    //void read(const std::string &fname);
    // Write the BMP image in fname
    //void write(const std::string &fname);
};

// class ppm {
//     bool flag_alloc;
//     void init();
//     //info about the PPM file (height and width)
//     unsigned int nr_lines;
//     unsigned int nr_columns;

// public:
//     //arrays for storing the R,G,B values
//     unsigned char *r;
//     unsigned char *g;
//     unsigned char *b;
//     //
//     unsigned int height;
//     unsigned int width;
//     unsigned int max_col_val;
//     //total number of elements (pixels)
//     unsigned int size;

//     ppm();
//     //create a PPM object and fill it with data stored in fname 
//     ppm(const std::string &fname);
//     //create an "empty" PPM image with a given width and height;the R,G,B arrays are filled with zeros
//     ppm(const unsigned int _width, const unsigned int _height);
//     //free the memory used by the R,G,B vectors when the object is destroyed
//     ~ppm();
//     //read the PPM image from fname
//     void read(const std::string &fname);
//     //write the PPM image in fname
//     void write(const std::string &fname);
// };