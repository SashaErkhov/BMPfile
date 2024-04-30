#include <iostream>
#include <fstream>
#include <stdexcept>
#include "BMPfile.h"

const unsigned char templateBMP[]={
        0x42,
        0x4d,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x3e,
        0x0,
        0x0,
        0x0,
        0x28,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x1,
        0x0,
        0x1,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
        0xff,
        0xff,
        0xff,
        0x0
};

BMPfile::BMPfile(unsigned int width, unsigned int height)
{
	if ( width == 0 )
		throw std::invalid_argument("Bad width");
	if ( height == 0 )
		throw std::invalid_argument("Bad height");

	bmpPtr = nullptr;
	/*std::fstream hex;
	hex.open("../Resource/template.bmp", std::ios_base::in | std::ios_base::binary);//файл с шаблоном заголовка
	if (!hex.is_open())
		throw  std::runtime_error("Can't load template file");
	*/
	long long int rowSize = ((width + 31) / 32) * 4;//формула из википедии, правильная
	long long int bodySize = rowSize * height;
	long long int fileSize = bodySize + 62;
	bmpPtr = new unsigned char[fileSize];
	
	/*for (size_t i = 0; i < HEADER_SIZE; ++i){
		if ( hex.eof() ) throw std::runtime_error("Bad template file");
		hex.get((char&)(bmpPtr[i]));
	}
	hex.close();*/
    for(size_t i=0;i<HEADER_SIZE;++i)
    {
        bmpPtr[i]=templateBMP[i];
    }

	for (size_t i = HEADER_SIZE; i < fileSize; ++i)
		bmpPtr[i] = 0b11111111;//белый

	//записываем параметры
	
	*(reinterpret_cast<unsigned*>(bmpPtr + FS_OFFSET)) = fileSize;
	m_width = reinterpret_cast<unsigned*>(bmpPtr + WIDTH_OFFSET); 	
	*m_width = width;
	m_height = reinterpret_cast<unsigned*>(bmpPtr + HEIGHT_OFFSET);
 	*m_height = height;
	*(reinterpret_cast<unsigned*>(bmpPtr + BS_OFFSET)) = bodySize;
	m_size=fileSize;
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

    m_width = reinterpret_cast<unsigned*>(bmpPtr + WIDTH_OFFSET);
    m_height = reinterpret_cast<unsigned*>(bmpPtr + HEIGHT_OFFSET);
    m_size=fileSize;
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

unsigned int BMPfile::getWidth()const
{
	return *m_width;
}

unsigned int BMPfile::getHeight()const
{
	return *m_height;
}

bool BMPfile::getPixel(unsigned int row, unsigned int col) const//не готово
{
    if ( row >= *m_height )
        throw std::invalid_argument("Bad row");
    if ( col >= *m_width )
        throw std::invalid_argument("Bad col");

    const size_t BYTE_OFFSET = HEADER_SIZE + (((*m_width + 31) / 32) * 4 * (*m_height - row - 1))+ col/8;
    const size_t BIT_OFFSET = 7 - col % 8;
    char byte=bmpPtr[BYTE_OFFSET] & (1 << BIT_OFFSET);
    if((byte>>BIT_OFFSET)==1)return true;
    return false;
}

void BMPfile::resize(unsigned int newWeight, unsigned int newHeight)
{
    BMPfile newBMP(newWeight, newHeight);
    for(size_t i=0;i<*m_height;++i)
    {
        for(size_t j=0;j<*m_width;++j)
        {
            newBMP.setPixel(i,j,getPixel(i,j));
            //newBMP.setPixel(j,i,getPixel(j,i));
        }
    }
    swap(*this,newBMP);
}

void BMPfile::swap(BMPfile& first, BMPfile& second)
{
    std::swap(first.m_height, second.m_height);
    std::swap(first.m_width, second.m_width);
    std::swap(first.m_size, second.m_size);
    std::swap(first.bmpPtr, second.bmpPtr);
}

BMPfile::BMPfile(const BMPfile& other)
{
    m_size=other.m_size;
    bmpPtr = new unsigned char[m_size];
    for(size_t i=0;i<m_size;++i)
    {
        bmpPtr[i]=other.bmpPtr[i];
    }
    m_width = reinterpret_cast<unsigned*>(bmpPtr + WIDTH_OFFSET);
    m_height = reinterpret_cast<unsigned*>(bmpPtr + HEIGHT_OFFSET);
}

BMPfile& BMPfile::operator=(const BMPfile& other)
{
    if(this!= &other)
    {
        BMPfile tmp(other);
        swap(tmp, *this);
    }
    return *this;
}

BMPfile::BMPfile(BMPfile&& other): BMPfile()
{
    swap(other, *this);
}

BMPfile& BMPfile::operator=(BMPfile&& other)
{
    if(this!= &other)
    {
        swap(other, *this);
    }
    return *this;
}