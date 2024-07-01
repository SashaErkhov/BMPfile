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

	bmpPtr_ = nullptr;
    std::uint32_t rowSize = ((width + 31) / 32) * 4;//формула из википедии, правильная
    std::uint32_t bodySize = rowSize * height;
    std::uint32_t fileSize = bodySize + 62;
	bmpPtr_ = new std::uint8_t[fileSize];
    for(size_t i=0;i<HEADER_SIZE_;++i)
    {
        bmpPtr_[i]=templateBMP[i];
    }

	for (auto i = static_cast<std::uint32_t>(HEADER_SIZE_);
            i < fileSize; ++i)
		bmpPtr_[i] = 0b11111111;//белый

	//записываем параметры
	
	*(reinterpret_cast<std::uint32_t*>(bmpPtr_ + FS_OFFSET_)) = fileSize;
	width_ = reinterpret_cast<std::uint32_t*>(bmpPtr_ + WIDTH_OFFSET_);
	*width_ = width;
	height_ = reinterpret_cast<std::uint32_t*>(bmpPtr_ + HEIGHT_OFFSET_);
 	*height_ = height;
	*(reinterpret_cast<std::uint32_t*>(bmpPtr_ + BS_OFFSET_)) = bodySize;
	size_=fileSize;
}

BMPfile::BMPfile(const char* namefile)//еще не проверено
{
	std::fstream file;
	file.open(namefile, std::ios_base::in | std::ios_base::binary);
	file.seekg(2, std::ios_base::beg);
	std::uint8_t fileSizeChar[4];
	file.read((char*)fileSizeChar, 4);
    std::uint32_t fileSize =
            (fileSizeChar[3] << 3 * 8) | (fileSizeChar[2] << 2 * 8) | (fileSizeChar[1] << 8) | (fileSizeChar[0]);
	bmpPtr_ = new std::uint8_t[fileSize];
	file.seekg(0, std::ios_base::beg);
	file.read((char*)bmpPtr_, fileSize);
    file.close();

    width_ = reinterpret_cast<std::uint32_t*>(bmpPtr_ + WIDTH_OFFSET_);
    height_ = reinterpret_cast<std::uint32_t*>(bmpPtr_ + HEIGHT_OFFSET_);
    size_=fileSize;
}

BMPfile::~BMPfile()
{
	delete[] bmpPtr_;
	bmpPtr_ = nullptr;
    height_  = nullptr;
    width_  = nullptr;
}

void BMPfile::saveBmp(const char* nameFile)
{
	if (bmpPtr_ == nullptr)
	{
		throw "error 4";
	}
	std::fstream file;
	file.open(nameFile, std::ios_base::out | std::ios_base::binary);
	if (!file.is_open())
	{
		//delete[] bmpPtr_;
		//bmpPtr_ = nullptr;
		throw std::invalid_argument("Can't open file to write");
	}
	file.write((char*)bmpPtr_, size_);
	file.close();
}

void BMPfile::setPixel(unsigned int row, unsigned int col, bool isWhite) const //проверено, не опасно
{
	if ( row >= *height_ )
		throw std::invalid_argument("Bad row");
	if ( col >= *width_ )
		throw std::invalid_argument("Bad col");

	const size_t BYTE_OFFSET = HEADER_SIZE_ + (((*width_ + 31) / 32) * 4 * (*height_ - row - 1))+ col/8;
	const size_t BIT_OFFSET = 7 - col % 8;
	if ( !isWhite )//черный
	{
		bmpPtr_[BYTE_OFFSET] &= ( ~ (1 << BIT_OFFSET));
	}
	else//белый
	{
		bmpPtr_[BYTE_OFFSET] |= (1 << BIT_OFFSET );
	}
}

bool BMPfile::getPixel(unsigned int row, unsigned int col) const//black - true white - false | не готово
{
    if ( row >= *height_ )
        throw std::invalid_argument("Bad row");
    if ( col >= *width_ )
        throw std::invalid_argument("Bad col");

    const size_t BYTE_OFFSET = HEADER_SIZE_ + (((*width_ + 31) / 32) * 4 * (*height_ - row - 1))+ col/8;
    const size_t BIT_OFFSET = 7 - col % 8;
    char byte=bmpPtr_[BYTE_OFFSET] & (1 << BIT_OFFSET);
    if((byte>>BIT_OFFSET)==1)return true;
    return false;
}

void BMPfile::resize(unsigned int newWeight, unsigned int newHeight)
{
    BMPfile newBMP(newWeight, newHeight);
    for(std::uint32_t i=0;i<*height_;++i)
    {
        for(std::uint32_t j=0;j<*width_;++j)
        {
            newBMP.setPixel(i,j,getPixel(i,j));
            //newBMP.setPixel(j,i,getPixel(j,i));
        }
    }
    swap_(*this,newBMP);
}

void BMPfile::swap_(BMPfile& first, BMPfile& second)
{
    std::swap(first.height_, second.height_);
    std::swap(first.width_, second.width_);
    std::swap(first.size_, second.size_);
    std::swap(first.bmpPtr_, second.bmpPtr_);
}

BMPfile::BMPfile(const BMPfile& other)
{
    size_=other.size_;
    bmpPtr_ = new std::uint8_t[size_];
    for(std::uint32_t i=0;i<size_;++i)
    {
        bmpPtr_[i]=other.bmpPtr_[i];
    }
    width_ = reinterpret_cast<std::uint32_t*>(bmpPtr_ + WIDTH_OFFSET_);
    height_ = reinterpret_cast<std::uint32_t*>(bmpPtr_ + HEIGHT_OFFSET_);
}

BMPfile& BMPfile::operator=(const BMPfile& other)
{
    if(this!= &other)
    {
        BMPfile tmp(other);
        swap_(tmp, *this);
    }
    return *this;
}

BMPfile::BMPfile(BMPfile&& other): BMPfile()
{
    swap_(other, *this);
}

BMPfile& BMPfile::operator=(BMPfile&& other)
{
    if(this!= &other)
    {
        swap_(other, *this);
    }
    return *this;
}