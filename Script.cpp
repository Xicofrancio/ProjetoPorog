#include <iostream>
#include <fstream>
#include <algorithm>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert"){
                invert();
                continue;
            }
            if(command == "to_gray_scale"){
                to_gray_scale();
                continue;
            }
            if(command == "replace"){
                replace();
                continue;
            }
            if(command == "fill"){
                fill();
                continue;
            }
            if(command == "h_mirror"){
                h_mirror();
                continue;
            }
            if(command == "v_mirror"){
                v_mirror();
                continue;
            }
            if(command == "add"){
                add();
                continue;
            }
            if(command == "crop"){
                crop();
                continue;
            }
            if(command == "rotate_left"){
                rotate_left();
                continue;
            }
            if(command == "rotate_right"){
                rotate_right();
                continue;
            }
            if(command == "median_filter"){
                median_filter();
                continue;
            }

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert() {
        int w = image->width();
        int h = image->height();

        for(int y = 0; y<h; ++y){
            for(int x = 0; x<w; ++x){
                Color c = image->at(x,y);
                c.red() = 255 - c.red();
                c.green() = 255 - c.green();
                c.blue() = 255 - c.blue();
                image->at(x,y) = c;
            }
        }
        

    }
    void Script::to_gray_scale() {
        int w = image->width();
        int h = image->height();

        for(int y=0; y<h; ++y){
            for(int x=0; x<w; ++x){
                Color c = image->at(x,y);
                int v = (c.blue() + c.green() + c.red()) / 3;
                c.blue() = c.green() = c.red() = v;
                image->at(x,y) = c;
            }
        }
    }
    void Script::replace() {
        int r1,g1,b1,r2,g2,b2;
        input >> r1 >> g1 >> b1>> r2 >> g2 >> b2;
        int w = image->width();
        int h = image->height();

        for(int y=0; y<h; ++y){
            for(int x=0; x<w; ++x){
                Color c = image->at(x,y);
                if((c.red() == r1) && (c.green() == g1) && (c.blue() == b1)){
                    c.red() = r2;
                    c.green() = g2;
                    c.blue() = b2;
                    image->at(x,y) = c;
                }
            }
        }
    }
    void Script::fill() {
        int x,y,w,h,r,g,b;
        input >> x >> y >> w >> h >> r >> g >> b;
        for(int i = y; i < y + h; ++i) {
            for(int j = x; j < x + w; ++j) {
                Color& c = image->at(j, i);
                c.red() = r;
                c.green() = g;
                c.blue() = b;
                image->at(j, i) = c;
            }
        
        }

    }
    void Script::h_mirror() {
        int w = image->width();
        int h = image->height();

        for(int y = 0; y<h; ++y){
            for(int x = 0; x<w/2; ++x){
                Color& c1 = image->at(x,y);
                Color& c2 = image->at(w-1-x,y);
                std::swap(c1, c2);
            }
        }
    }
    void Script::v_mirror() {
        int w = image->width();
        int h = image->height();

        for(int y = 0; y<h/2; ++y){
            for(int x = 0; x<w; ++x){
                Color& c1 = image->at(x,y);
                Color& c2 = image->at(x,h-1-y);
                std::swap(c1, c2);
            }
        }
    }
    void Script::add() {
        
        string filename;
        Color neutral;
        input >> filename >> neutral;
        Image* newImage = loadFromPNG(filename);

        int newW = newImage->width();
        int newH = newImage->height();
        int currW = image->width();
        int currH = image->height();

        int x, y;
        input >> x >> y;

        for (int i = 0; i < newW; i++) {
            for (int j = 0; j < newH; j++) {
                Color& c = newImage->at(i, j);
                if ((c != neutral) && (x + i < currW) && (y + j < currH)) {
                    image->at(x + i, y + j) = c;
                }
            }
        }

        delete newImage;
    }
    void Script::crop() {
        int x,y,w,h;
        input >> x >> y >> w >> h;
        Image* new_image = new Image(w, h);

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                Color& c = image->at(x + j, y + i);
                new_image->at(j, i) = c;
            }
        }

        delete image;
        image = new_image;
    }
    void Script::rotate_left(){
       
        int w = image->width();
        int h = image->height();

        Image* newImage = new Image(h, w, Color(255, 255, 255));

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                Color c = image->at(x, y);
                newImage->at(y, w - x - 1) = c;
        }
        }

        delete image;
        image = newImage;
    }
    void Script::rotate_right() {
        int w = image->width();
        int h = image->height();
        Image *newImage = new Image(h, w, Color(0, 0, 0));
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                newImage->at(h - y - 1, x) = image->at(x, y);
            }   
        }
        delete image;
        image = newImage;
    }
    void Script::median_filter() {
        if (image == nullptr) {
        cout << "Error: no image loaded." << endl;
        return;
    }

    int ws;
    input >> ws;

    if (ws < 3 || ws % 2 == 0) {
        return;
    }

    // Create a copy of the original image.
    Image *copy = new Image(*image);
    int nx_max;
    int nx_min;
    int ny_max;
    int ny_min;

    // Apply the median filter to each pixel.
    for (int y = 0; y < copy->height(); y++) {
        for (int x = 0; x < copy->width(); x++) {
            // Collect pixel values in the window.
            vector<int> r_values;
            vector<int> g_values;
            vector<int> b_values;
            
            nx_max=min(copy->width()-1,x + ws / 2);
            nx_min=max(0,x - ws / 2);
            ny_max=min(copy->height()-1,y + ws / 2);
            ny_min=max(0,y - ws / 2);

            for (int j = ny_min; j <= ny_max; j++) {
                for (int i = nx_min; i <= nx_max; i++) {
                    // Get pixel values within the image boundaries.
                    if (i >= 0 && i < copy->width() && j >= 0 && j < copy->height()) {
                        Color c = image->at(i, j);
                        r_values.push_back(c.red());
                        g_values.push_back(c.green());
                        b_values.push_back(c.blue());
                    }
                }
            }

            // Sort the pixel values and get the median value.
            sort(r_values.begin(), r_values.end());
            sort(g_values.begin(), g_values.end());
            sort(b_values.begin(), b_values.end());
            int median_r;
            int median_g;
            int median_b;
            if(r_values.size()%2==0){
               median_r = (r_values[r_values.size() / 2] + r_values[(r_values.size() / 2)-1])/2; 
            }
            else{
                median_r = r_values[r_values.size() / 2];
            }
            if(g_values.size()%2==0){
                median_g = (g_values[g_values.size() / 2] + g_values[(g_values.size() / 2)-1])/2; 
            }
            else{
                median_g = g_values[g_values.size() / 2];
            }
            if(b_values.size()%2==0){
               median_b = (b_values[b_values.size() / 2] + b_values[(b_values.size() / 2)-1])/2; 
            }
            else{
                median_b = b_values[b_values.size() / 2];
            }  

            // Set the median pixel value to the current pixel.
            Color median_color(median_r, median_g, median_b);
            copy->at(x, y) = median_color;
        }
    }
    image = copy;
    }
}

