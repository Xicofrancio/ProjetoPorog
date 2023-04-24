#include "Color.hpp"

namespace prog {
    Color::Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
    Color::Color(const Color& other) {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        this->r = red;
        this->g = green;
        this->b = blue;
    }
    rgb_value Color::red() const {
        return this->r;
    }
    rgb_value Color::green() const {
        return this->g;
    }
    rgb_value Color::blue() const {
        return this->b;
    }

    // TODO: remove this DUMMY variable once you have appropriate fields for representing colors.
    

    rgb_value& Color::red()  {
        return this->r;
    }
    rgb_value& Color::green()  {
      return this->g;
    }
    rgb_value& Color::blue()  {
      return this->b;
    }

}
