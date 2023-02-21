#pragma once

#include "sandpile.h"
#include "bmp.h"

#include <vector>

struct BMP {
    BMP();
    BMP(size_t width, size_t height);

    void Write(const Sandpile* sandpile, const char* path) const;

private:
    int width;
    int height;
};

void CreateSandpileBMP(const Sandpile* sandpile, uint32_t cur_iter);