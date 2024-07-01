#include <iostream>
#include <fstream>
#include "BMPfile.h"

int main()
{
	try
	{
		BMPfile first(50, 50);
		first.saveBmp("../Resource/Hi.bmp");
        BMPfile second("../Resource/Hi.bmp");
        second.saveBmp("../Resource/Hi2.bmp");
        first.resize(100,100);
        first.saveBmp("../Resource/Hi3.bmp");
	}
	catch (const char* error)
	{
		std::cout << error << std::endl;
		system("pause");
		return 1;
	}
    return 0;
}

