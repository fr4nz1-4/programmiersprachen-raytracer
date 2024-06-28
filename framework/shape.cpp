#include "shape.hpp"
#include "iostream"

Shape::Shape(std::string const& name, Color const& color) :
    name_{name},
    color_{color} {
    std::cout<< "Shape constructor\n";
}

std::ostream& Shape::print(std::ostream &os) const {
    return os << "name: " + name_ + " | Farbe: " + std::to_string(color_.r) + ", " + std::to_string(color_.g) + ", " + std::to_string(color_.b);
}

Shape::~Shape() {
    std::cout<< "Shape destructor\n";
}

std::ostream& operator<<(std::ostream& os, Shape const& shape) {
    return shape.print(os);
}