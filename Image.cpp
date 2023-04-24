#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    this->w = w;
    this->h = h;
    _pixels = new Color*[h];
        for (int i = 0; i < h; ++i)
        {
            _pixels[i] = new Color[w];
            for (int j = 0; j < w; ++j)
            {
                _pixels[i][j] = fill;
            }
        }
    
  }
  Image::~Image()
  {
    for (int i = 0; i < h; ++i)
        {
            delete[] _pixels[i];
        }
        delete[] _pixels;
  }
  int Image::width() const
  {
    return this->w;
  }
  int Image::height() const
  {
    return this->h;
  }


  Color& Image::at(int x, int y)
  {
    return _pixels[y][x];
    
  }

  const Color& Image::at(int x, int y) const
  {
    return _pixels[y][x];
  }
}
  