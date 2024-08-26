#include <gtest/gtest.h>
#include "../code/BMPfile.h"

TEST(BMPfileTest, ConstructorStandartdTest){
    BMPfile file(2,2);
    file.saveBmp("Resource/CST.bmp");
    EXPECT_EQ(0,std::system("diff Resource/CST.bmp Resource/test1.bmp"));
    std::system("rm -rf Resource/CST.bmp");
}

TEST(BMPfileTest, ConstructorWithFileTest){
    BMPfile file("Resource/Qr.bmp");
    file.saveBmp("Resource/CWFT.bmp");
    EXPECT_EQ(0,std::system("diff Resource/CWFT.bmp Resource/Qr.bmp"));
    std::system("rm -rf Resource/CWFT.bmp");
}

TEST(BMPfileTest, Setter){
    BMPfile file(2,2);
    file.setPixel(0,0, false);
    file.setPixel(1,1, false);
    file.saveBmp("Resource/setter.bmp");
    EXPECT_EQ(0,std::system("diff Resource/setter.bmp Resource/test2.bmp"));
    std::system("rm -rf Resource/setter.bmp");
}

TEST(BMPfileTest, Getter){
    BMPfile file(2,2);
    EXPECT_EQ(true,file.getPixel(0,0));
    file.setPixel(0,0, false);
    EXPECT_EQ(false,file.getPixel(0,0));
    EXPECT_EQ(true,file.getPixel(0,1));
    file.setPixel(0,1, false);
    EXPECT_EQ(false,file.getPixel(0,1));
    EXPECT_EQ(true,file.getPixel(1,0));
    file.setPixel(1,0, false);
    EXPECT_EQ(false,file.getPixel(1,0));
    EXPECT_EQ(true,file.getPixel(1,1));
    file.setPixel(1,1, false);
    EXPECT_EQ(false,file.getPixel(1,1));
    file.setPixel(1,1,true);
    EXPECT_EQ(true,file.getPixel(1,1));
    file.setPixel(1,0,true);
    EXPECT_EQ(true,file.getPixel(1,0));
    file.setPixel(0,0,true);
    EXPECT_EQ(true,file.getPixel(0,0));
    file.setPixel(0,1,true);
    EXPECT_EQ(true,file.getPixel(0,1));
    EXPECT_EQ(2,file.getHeight());
    EXPECT_EQ(2,file.getWidth());
}

//TEST(BMPfileTest, RulesOfFive){
//
//}