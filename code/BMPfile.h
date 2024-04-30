#pragma once
#ifndef BMP_FILES_PVM
#define BMP_FILES_PVM

#include <iostream>

class BMPfile
{
	unsigned char* bmpPtr;
	unsigned *m_height;
	unsigned *m_width;
	size_t m_size;

	static const size_t HEADER_SIZE = 62;
    static const size_t FS_OFFSET = 2;
    static const size_t WIDTH_OFFSET = 18;
    static const size_t HEIGHT_OFFSET = 22;
    static const size_t BS_OFFSET = 34;
    static void swap(BMPfile& first, BMPfile& second);
public:
    BMPfile(const BMPfile&);
    BMPfile& operator=(const BMPfile&);
    BMPfile(BMPfile&&);
    BMPfile& operator=(BMPfile&&);
	BMPfile(unsigned int weight = 1000, unsigned int height = 1000);
	BMPfile(const char* namefile);
	~BMPfile();
	void saveBmp(const char* nameFile="print.bmp");
	void setPixel(unsigned int positionM, unsigned int positionN, bool size) const;
    void resize(unsigned int newWeight, unsigned int newHeight);
	unsigned int getWidth()const;
	unsigned int getHeight()const;
	bool getPixel(unsigned int, unsigned int) const;
};

#endif