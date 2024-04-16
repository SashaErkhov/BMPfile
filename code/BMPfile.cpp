#include <iostream>
#include <fstream>
#include <stdexcept>
#include "BMPfile.h"

BMPfile::BMPfile(unsigned int width, unsigned int height)
{
	if ( width == 0 )
		throw std::invalid_argument("Bad width");
	if ( height == 0 )
		throw std::invalid_argument("Bad height");

	bmpPtr = nullptr;
	std::fstream hex;
	hex.open("../Resource/template.bmp", std::ios_base::in | std::ios_base::binary);//файл с шаблоном заголовка
	if (!hex.is_open())
		throw  std::runtime_error("Can't load template file");
	
	long long int rowSize = ((width + 31) / 32) * 4;//формула из википедии, правильная
	long long int bodySize = rowSize * height;
	long long int fileSize = bodySize + 62;
	bmpPtr = new unsigned char[fileSize];
	
	for (size_t i = 0; i < HEADER_SIZE; ++i){
		if ( hex.eof() ) throw std::runtime_error("Bad template file");
		hex.get((char&)(bmpPtr[i]));
	}
	hex.close();

	for (size_t i = HEADER_SIZE; i < fileSize; ++i)
		bmpPtr[i] = 0b11111111;//белый

	//записываем параметры

	const size_t FS_OFFSET = 2;
	const size_t WIDTH_OFFSET = 18;
	const size_t HEIGHT_OFFSET = 22;
	const size_t BS_OFFSET = 34;
	
	*(reinterpret_cast<unsigned*>(bmpPtr + FS_OFFSET)) = fileSize;
	m_width = reinterpret_cast<unsigned*>(bmpPtr + WIDTH_OFFSET); 	
	*m_width = width;
	m_height = reinterpret_cast<unsigned*>(bmpPtr + HEIGHT_OFFSET);
 	*m_height = height;
	*(reinterpret_cast<unsigned*>(bmpPtr + BS_OFFSET)) = bodySize;
	
}

BMPfile::BMPfile(const char* namefile)//еще не проверено
{
	std::fstream file;
	file.open(namefile, std::ios_base::in | std::ios_base::binary);
	file.seekg(2, std::ios_base::beg);
	char fileSizeChar[4];
	file.read(fileSizeChar, 4);
	unsigned long long int fileSize = (fileSizeChar[3] << 3 * 8) | (fileSizeChar[2] << 2 * 8) | (fileSizeChar[1] << 8) | (fileSizeChar[0]);
	bmpPtr = new unsigned char[fileSize];
	file.seekg(0, std::ios_base::beg);
	file.read((char*)bmpPtr, fileSize);
    file.close();
}

BMPfile::~BMPfile()
{
	delete[] bmpPtr;
	bmpPtr = nullptr;
}

void BMPfile::saveBmp(const char* nameFile)
{
	if (bmpPtr == nullptr)
	{
		throw "error 4";
	}
	std::fstream file;
	file.open(nameFile, std::ios_base::out | std::ios_base::binary);
	if (!file.is_open())
	{
		//delete[] bmpPtr;
		//bmpPtr = nullptr;
		throw std::invalid_argument("Can't open file to write");
	}
	//вытаскиваем размер всего файла
	unsigned long long int fileSize = (bmpPtr[5] << 3 * 8) | (bmpPtr[4] << 2 * 8) | (bmpPtr[3] << 8) | (bmpPtr[2]);
	file.write((char*)bmpPtr, fileSize);
	file.close();
}

void BMPfile::setPixel(unsigned int row, unsigned int col, bool isWhite) const //проверено, не опасно
{
	if ( row >= *m_height )
		throw std::invalid_argument("Bad row");
	if ( col >= *m_width )
		throw std::invalid_argument("Bad col");

	const size_t BYTE_OFFSET = HEADER_SIZE + (((*m_width + 31) / 32) * 4 * (*m_height - row - 1))+ col/8;
	const size_t BIT_OFFSET = 7 - col % 8;
	if ( !isWhite )//черный
	{
		bmpPtr[BYTE_OFFSET] &= ( ~ (1 << BIT_OFFSET));
	}
	else//белый
	{
		bmpPtr[BYTE_OFFSET] |= (1 << BIT_OFFSET );
	}
}

unsigned int BMPfile::getWidth()const//не готово
{
	return *m_width;
}
unsigned int BMPfile::getHeight()const//не готово
{
	return *m_height;
}
bool BMPfile::getPixel(unsigned int positionM, unsigned int positionN) const//не готово
{
	return 0;
}