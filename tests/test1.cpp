#include <gtest/gtest.h>
#include "BMPfile.h"

TEST(BMPfileTest, ConstructorStandartdTest){
    BMPfile file(2,2);
    file.saveBmp("Resource/CST.bmp");
    EXPECT_EQ(0,std::system("fc Resource/CST.bmp Resource/test1.bmp"));
    std::system("rm -rf Resource/CST.bmp");
}