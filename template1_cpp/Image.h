#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include <iostream>

constexpr int WINDOW_WIDTH = 512, WINDOW_HEIGHT = 512;
constexpr int TILE_WIDTH = 32, TILE_HEIGHT = 32;
constexpr int CNT_WIDTH = WINDOW_WIDTH / TILE_WIDTH;
constexpr int CNT_HEIGHT = WINDOW_HEIGHT / TILE_HEIGHT;


struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

constexpr Pixel backgroundColor{ 0, 0, 0, 0 };//here

struct Image
{
    explicit Image(const std::string& a_path);
    Image(int a_width, int a_height, int a_channels);

    int Save(const std::string& a_path);

    int Width()    const { return width; }
    int Height()   const { return height; }
    int Channels() const { return channels; }
    size_t Size()  const { return size; }
    Pixel* Data() { return  data; }
    Pixel GetPixel(int x, int y) {
        try { return data[width * (height - y - 1) + x]; }
        catch (...) { std::cout << "wrong indexes x = " << x << " y = " << y << std::endl; }
    }
    void  PutPixel(int x, int y, const Pixel& pix) {
        try { data[width * y + x] = pix; }
        catch (...) { std::cout << "wrong indexes x = " << x << " y = " << y << std::endl; }
    }

    ~Image();

private:
    int width = -1;
    int height = -1;
    int channels = 3;
    size_t size = 0;
    Pixel* data = nullptr;
    bool self_allocated = false;
};



#endif //MAIN_IMAGE_H