#include "XPM2.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
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
        getline(input, line); // ignore the header line

        int width, height;
        input >> width >> height;

        getline(input, line);
        getline(input, line);

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
        Color colors[256];
        string colorCodes[256];

        output << "! XPM2\n";
        output << image->width() << " " << image->height() << " " << nColors << " 1\n";

        for (int i = 0; i < image->height(); i++)
        {
            for (int j = 0; j < image->width(); j++)
            {
                const Color &pixColor = image->at(j, i);
                string code;
                bool colorFound = false;

                for (int k = 0; k < nColors; k++)
                {
                    if (pixColor == colors[i])
                    {
                        code = colorCodes[i];
                        colorFound = true;
                        break;
                    }
                }

                // If the color is new, add it to the list
                if (!colorFound)
                {
                    colors[nColors] = pixColor;
                    code = "c" + to_string(nColors);
                    colorCodes[nColors] = code;
                    nColors++;
                }

                output << code.substr(1);
            }
            output << "\n";
        }

        // Write the colors section
        for (int i = 0; i < nColors; ++i)
        {
            const Color &color = colors[i];
            const std::string &code = colorCodes[i];
            output << code << " c #" << std::hex << std::setfill('0') << std::setw(2)
                       << static_cast<int>(color.red()) << std::setw(2)
                       << static_cast<int>(color.green()) << std::setw(2)
                       << static_cast<int>(color.blue()) << "\n";
        }

        output.close();
    }
}