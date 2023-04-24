#ifndef __prog_Color_hpp__
#define __prog_Color_hpp__

namespace prog
{
  typedef unsigned char rgb_value;
  class Color
  {
    private:
      // TODO: private fields required for color representation.
      rgb_value r;
      rgb_value g;
      rgb_value b;
    public:
      Color();
      Color(const Color& c);
      Color(rgb_value r, rgb_value g, rgb_value b);
      rgb_value red() const;
      rgb_value& red();
      rgb_value green() const;
      rgb_value& green();
      rgb_value blue() const;
      rgb_value& blue();
      bool operator==(const Color& other) const {
        return r == other.red() && g == other.green() && b == other.blue();
      }
      bool operator!=(const Color& other) const {
        return !(*this == other);
      } 
  };
}
#endif
