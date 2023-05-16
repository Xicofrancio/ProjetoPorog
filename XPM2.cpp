#include "XPM2.hpp"
//#include 
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        ifstream input(file);
        if (!input.is_open()){
            return nullptr;
        }

        string line;
        int width, height, nColors;
        char c;

        if (getline(input, line)){
            stringstream ss(line);
            ss >> width >> height >> nColors >> c;
        }

        Image* img = new Image(width, height);

        int y = 0;
        while (getline(input, line) && y < height){
            string pixRow = line.substr(0, width);
            int x = 0;
            for (char pix : pixRow){
                if (x >= width) break;

                // create new object of Color
                Color color;
                color.red() = static_cast<rgb_value>(pix);

                img->at(x, y) = color; // set the color in the object of Image
                x++;
            }
            y++;            
        }
        input.close();

        return img;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        ofstream output(file);
        if (!output.is_open()){
            return;
        }

        int width = image->width();
        int height = image->height();
        int nColors = 1;
        char c = '1';

        output << width << " " << height << " " << nColors << " " << c << endl;

        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                const Color& color = image->at(j, i);
                char pix = static_cast<char>(color.red());
                output << pix;
            }
            output << endl;
        }
        output.close();
    }
}
