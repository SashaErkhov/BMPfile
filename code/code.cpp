#include <iostream>
#include <fstream>
#include "BMPfile.h"

int main()
{
	try
	{
        BMPfile first(4,4);
        first.setPixel(1,1,false);
        first.setPixel(2,2,false);
        first.saveBmp("first.bmp");
        first.symResize(2,2);
        first.saveBmp("second.bmp");
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

