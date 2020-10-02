#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <tiffio.h> 
int main(int argc, char** argv)
{
	std::string file_path = argv[1]; 
	std::cout << "Opening file " << file_path << "..." << std::endl;
	std::ifstream file(file_path, std::ifstream::binary);
	if(file)
	{
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);

		uint32_t width;
		uint32_t height;
		uint16_t * buffer = new uint16_t [(length/4) - 2];

		file.read(reinterpret_cast<char*>(&width), sizeof(width));
		file.read(reinterpret_cast<char*>(&height), sizeof(height));

		for(int i = 0; i < length/4; i++)
		{
			float value;
			file.read(reinterpret_cast<char *>(&value), sizeof(value));
			auto f_height = 65535 * value;
			uint16_t height_value = static_cast<uint16_t>(std::round(f_height));
			buffer[i] = height_value;
		}

		TIFF *tif = TIFFOpen("output.tif", "w");

		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width); 
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height); 
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16); 
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1); 
		TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
		TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

		uint16_t * buf = NULL;

		tsize_t linebytes = TIFFScanlineSize(tif);

		buf=(uint16_t*)_TIFFmalloc(linebytes);
		
		for (int i = 0; i < height; i++) {

			std::memcpy(buf, &buffer[i*width], linebytes);
			TIFFWriteScanline(tif, buf, i, 0);
		}

		TIFFClose(tif);
		file.close();
		delete[] buffer;
	}
	return 1;
}
