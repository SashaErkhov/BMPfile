#include <iostream>
#include <fstream>
#include "BMPfile.h"

int main()
{
	try
	{
        BMPfile first(4,4);
        first.setPixel(0,0,false);
        first.setPixel(0,2,false);
        first.setPixel(1,1,false);
        first.setPixel(1,3,false);
        first.setPixel(2,0,false);
        first.setPixel(2,2,false);
        first.setPixel(3,1,false);
        first.setPixel(3,3,false);
        first.saveBmp("../Resource/Hi.bmp");
        first.resize(4,2);
        first.saveBmp("../Resource/Hi2.bmp");
    }
	catch (const char* error)
	{
		std::cout << error << std::endl;
		system("pause");
		return 1;
	}
    catch (const std::exception& error)
    {
        std::cout << error.what() << std::endl;
        system("pause");
        return 1;
    }
    catch (...)
    {
        std::cout << "Unknown error" << std::endl;
        system("pause");
        return 1;
    }
    //system("pause");
    return 0;
}

