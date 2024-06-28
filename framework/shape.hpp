#ifndef RAYTRACER_SHAPE_HPP
#define RAYTRACER_SHAPE_HPP
#include <tuple>
#include <string>
#include "color.hpp"

class Shape {
public:
    Shape(std::string const& name, Color const& color);
    virtual std::ostream& print(std::ostream& os) const;
    virtual float area() const = 0;
    virtual float volume() const = 0;
    virtual ~Shape();
private:
    std::string name_;
    Color color_;
};

std::ostream& operator<<(std::ostream& os, Shape const& shape);

#endif //RAYTRACER_SHAPE_HPP
