#include "shape.hpp"
#include "iostream"

Shape::Shape(std::string const& name, Color const& color) :
    name_{name},
    color_{color}
{}

std::ostream& Shape::print(std::ostream &os) const {
    std::string st = "name: " + name_ + " | Farbe: " + std::to_string(color_.r) + ", " + std::to_string(color_.g) + ", " + std::to_string(color_.b);
    return os << st;
}

std::ostream& operator<<(std::ostream& os, Shape const& shape) {
    return shape.print(os);
}