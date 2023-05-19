#include "XPM2.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

namespace prog
{
    Image *loadFromXPM2(const std::string &file)
    {
        ifstream input(file);
        if (!input.is_open())
        {
            return nullptr;
        }

        string line;
        getline(input, line); // Skip the header line

        // Read image dimensions
        int width, height;
        input >> width >> height;

        // Skip the number of colors and characters per pixel
        getline(input, line); // Read the rest of the line
        getline(input, line); // Skip the colors line

        // Read the colors
        std::vector<Color> colors;
        while (getline(input, line))
        {
            if (line.empty())
                break;

            string hexCode;
            istringstream iss(line);
            iss >> hexCode;

            rgb_value red = stoi(hexCode.substr(1, 2), nullptr, 16);
            rgb_value green = stoi(hexCode.substr(3, 2), nullptr, 16);
            rgb_value blue = stoi(hexCode.substr(5, 2), nullptr, 16);

            colors.emplace_back(red, green, blue);
        }

        Image *img = new Image(width, height, Color());

        for (int i = 0; i < height; ++i)
        {
            getline(input, line);
            for (int j = 0; j < width; ++j)
            {
                char pixelChar = line[j];
                img->at(j, i) = colors[pixelChar - 'a'];
            }
        }

        input.close();
        return img;
    }

    void saveToXPM2(const std::string &file, const Image *image)
    {
        ofstream output(file);
        if (!output.is_open())
        {
            return;
        }

        int nColors = 0;
        map<Color, string> colorMap;

        for (int i = 0; i < image->height(); i++)
        {
            for (int j = 0; i < image->width(); j++)
            {
                const Color &pixColor = image->at(j, i);
                if (colorMap.find(pixColor) == colorMap.end())
                {
                    colorMap[pixColor] = "c" + to_string(nColors);
                    nColors++;
                }
            }
        }

        output << "! XPM2\n";
        output << image->width() << " " << image->height() << " " << nColors << " 1/n";

        for (const auto &c : colorMap)
        {
            const Color &color = c.first;
            const string &code = c.second;
            output << code << " c #" << hex << setfill('0') << setw(2)
                   << static_cast<int>(color.red()) << setw(2)
                   << static_cast<int>(color.green()) << setw(2)
                   << static_cast<int>(color.blue()) << "\n";
        }

        for (int i = 0; i < image->height(); ++i)
        {
            for (int j = 0; j < image->width(); ++j)
            {
                const Color &pixelColor = image->at(j, i);
                const string &code = colorMap[pixelColor];
                output << code.substr(1);
            }
            output << "\n";
        }

        output.close();
    }
}