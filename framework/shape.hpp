#ifndef RAYTRACER_SHAPE_HPP
#define RAYTRACER_SHAPE_HPP
#include <tuple>
#include <string>
#include "color.hpp"

class Shape {
public:
    Shape(std::string const& name, Color const& color) : name_{name}, color_{color} {}
    virtual float area() const = 0;
    virtual float volume() const = 0;
private:
    std::string name_;
    Color color_;
};

#endif //RAYTRACER_SHAPE_HPP
