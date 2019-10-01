#include <cassert>
#include <cstdlib>
#include <cstring>

#include "pngwrap.hpp"

int main(int argc, char const *argv[])
{
    unsigned char *testimage;
    unsigned int width=256, height=256;
    testimage = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    memset(testimage, 0xFF, width * height * sizeof(unsigned char));
    imwrite("TestImage.png", testimage, width, height);
    width = 0;
    height = 0;
    free(testimage);
    testimage = NULL;
    imread("TestImage.png", &testimage, &width, &height);
    assert(width == 256);
    assert(height == 256);
    for (unsigned int i = 0; i < width * height; i++)
    {
        assert(testimage[i] == 255);
    }
    return 0;
}