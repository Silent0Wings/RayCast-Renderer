/**
 * @file perlin.h
 * @brief Defines the the procedural perlin texture generator
 */
#ifndef PERLIN_H
#define PERLIN_H

#include "image.h"
#include "ImageRenderer.h"
#include "gmath.h"

using namespace std;

/**
 * @class perlin
 * @brief Represents an perlin image in 2D space.

 */
class perlin
{
public:
    image img;
    vector<vector<vec3>> grid;

    perlin(size_t w, size_t h)
    {

        if (w > 0 && h > 0)
        {
            img = image(500, 500);
        }
        else
        {
            throw std::invalid_argument("perlin::Constructor():Pixel coordinates out of bounds. w: " + std::to_string(w) + " | h: " + std::to_string(h));
        }

        srand(time(0)); // Seed the random number generator
        for (int i = 0; i < (int)w; i++)
        {
            vector<vec3> temp;
            for (int j = 0; j < (int)h; j++)
            {

                int x = (rand() % 101) + 1;  // generate a random number from 1 to 101
                int y = (rand() % 101) + 1;  // generate a random number from 1 to 101
                int x_negative = rand() % 2; // generate a random number from 0 to 1
                int y_negative = rand() % 2; // generate a random number from 0 to 1
                //
                vec3 temp_vec(x * ((x_negative == 1) ? 1 : -1), y * ((y_negative == 1) ? 1 : -1), 0);
                temp_vec = gmath::normalize(temp_vec);
                temp.push_back(temp_vec);
            }
            grid.push_back(temp);
        }
    }

    void buildPerlin()
    {

        for (size_t i = 0; i < img.getheight(); i++)
        {

            for (size_t j = 0; j < img.getwidth(); j++)
            {

                double value = get(i, j);

                color temp(value, value, value);
                img.set(i, j, temp);
            }
        }
    }

    void Save()
    {
        ImageRenderer::renderToFile(img, "perlin.ppm");
    }

    double get(double x, double y)
    {

        // cout <<"x "<<x << " y "<<y<< endl;

        vector<double> dots;

        for (size_t i = 0; i < grid.size(); i++)
        {
            for (size_t j = 0; j < grid.at(i).size(); j++)
            {
                vec3 gcorner = grid[i][j];
                vec3 p = vec3(x, y, 0);
                vec3 corner = vec3(i, j, 0);
                vec3 v = p - corner;
                if (v != vec3::zero())
                    v = gmath::normalize(v);
                double dot = gmath::dot(gcorner, v);
                dots.push_back(dot);

                // interpolation
            }
        }
        double value = (1-x)*dots.at(0)+x*dots.at(1) + (1-y)*dots.at(1)+y*(dots.at(2));
        value*=255;

        double sum = 0;
        for (size_t i = 0; i < dots.size(); i++)
        {
            sum += dots.at(i);
        }
        // sum/=dots.size();
        sum *= -1;
         sum*=255;
        // cout <<sum << " "<< endl;
        return sum;
    }

    // Overload operator<< for prunsigned inting
    friend std::ostream &operator<<(std::ostream &os, const perlin &p)
    {

    

        os << "\n";

       /*
       
       image img = p.img;
       os << "Image(" << img.getwidth() << ", " << img.getheight() << ")\n";
       for (unsigned int i = 0; i < img.getheight(); ++i)
       {
           for (unsigned int j = 0; j < img.getwidth(); ++j)
           {
               os << img.get(i, j) << " | ";
           }
           os << "\n";
       }
       */

        os << "grid(" << p.grid.size() << ", " << p.grid.at(0).size() << ")\n";

        for (unsigned int i = 0; i < p.grid.size(); ++i)
        {
            for (unsigned int j = 0; j < p.grid.at(i).size(); ++j)
            {
                os << p.grid[i][j] << " | ";
            }
            os << "\n";
        }

        return os;
    }
};
#endif // PERLIN_H