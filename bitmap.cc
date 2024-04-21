#include <fstream>
#include <iostream>
#include "bitmap.h"

using namespace std;

// Save the image data to file
bool bitmap::write() {
    bitmap_file_header file_header;
    bitmap_info_header info_header;

    // Calculate row padding
    int paddingSize = (4 - (width * sizeof(pixel)) % 4) % 4;
    info_header.data_size = (width * sizeof(pixel) + paddingSize) * height;

    // Set file size
    file_header.file_size = sizeof(bitmap_file_header) + sizeof(bitmap_info_header) + info_header.data_size;
    file_header.data_offset = sizeof(bitmap_file_header) + sizeof(bitmap_info_header);

    // Set the image's width and height
    info_header.width = width;
    info_header.height = height;

    // Open the file where we will write the bitmap
	// filename = "/Users/xiupingxue/Downloads/convex_new/output.bmp";
    std::ofstream ofile(filename, std::ios::out | std::ios::binary);

    if (!ofile) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return false;
    }

    // Write the File Header
    ofile.write(reinterpret_cast<char *>(&file_header), sizeof(bitmap_file_header));
    
    // Write the Info Header
    ofile.write(reinterpret_cast<char *>(&info_header), sizeof(bitmap_info_header));
    
    // Write the pixels with padding
    for (int y = 0; y < height; ++y) {
        ofile.write(reinterpret_cast<char*>(&pixels[y * width]), width * sizeof(pixel));
        for (int p = 0; p < paddingSize; ++p) {
            ofile.put(0);
        }
    }

    if (!ofile.good()) {
        std::cerr << "Error occurred at writing time." << std::endl;
        ofile.close();
        return false;
    }
    
    ofile.close();
    std::cout << "Completed" << std::endl;

    return true;
}


// Set the pixel at (x, y)
void bitmap::set_pixel(int x, int y, pixel p) {
	int idx = y * width + x;   // Calculate pixel offset

	pixels[idx] = p;           // The vector index is the pixel offset
}

// Set all the pixels in an entire row
void bitmap::set_row(int row, pixel p) {
	for (int i = 0; i < width; ++i) {
		set_pixel(i, row, p);
	}
}

// Set all the pixels in the image
void bitmap::set_all(pixel p) {
	for (int i = 0; i < height; ++i) {
		set_row(i, p);
	}
}