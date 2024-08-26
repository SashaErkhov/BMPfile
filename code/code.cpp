#include <iostream>
#include <fstream>
#include "BMPfile.h"

int main()
{
	try
	{
        std::cout << std::system("fc Hi.bmp test1.bmp")<<std::endl;
        BMPfile first("test1.bmp");
        first.setPixel(0,0,false);
        first.setPixel(1,1,false);
        first.saveBmp("test2.bmp");


//        BMPfile first(4,4);
//        first.setPixel(0,0,false);
//        first.setPixel(0,2,false);
//        first.setPixel(1,1,false);
//        first.setPixel(1,3,false);
//        first.setPixel(2,0,false);
//        first.setPixel(2,2,false);
//        first.setPixel(3,1,false);
//        first.setPixel(3,3,false);
//        first.saveBmp("../Resource/Hi.bmp");
//        first.resize(4,2);
//        first.saveBmp("../Resource/Hi2.bmp");

    }
	catch (const char* error)
	{
		std::cout << error << std::endl;
		return 1;
	}
    catch (const std::exception& error)
    {
        std::cout << error.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "Unknown error" << std::endl;
        return 1;
    }
    return 0;
}

