//
// Created by sasor on 23.04.2024.
//

#include <fstream>
#include <iostream>

int main()
{
    std::fstream f;
    f.open("C:/Users/sasor/projects/BMPfile/Resource/template.bmp",std::ios_base::in);
    if(!f.is_open()) std::cout<<"File not found"<<std::endl;
    unsigned char buf[62];
    f.read((char*)buf,62);
    for(int i=0;i<62;++i)
    {
        std::cout<<std::hex<<(int)buf[i]<<std::endl;
    }
    f.close();
}