#include "box.hpp"

Box::Box(std::string const& name, std::shared_ptr<Material> const& color, glm::vec3 min, glm::vec3 max) :
    Shape(name, color),
    min_{min},
    max_{max} {
    std::cout<< "Box constructor\n";
}

float Box::area() const {
    float a = abs(max_.x - min_.x);
    float b = abs(max_.y - min_.y);
    float c = abs(max_.z - min_.z);
    // O = 2 * (a * c + a * b + b * c)
    return 2 * (a * c + a * b + b * c);
}

float Box::volume() const {
    float a = abs(max_.x - min_.x);
    float b = abs(max_.y - min_.y);
    float c = abs(max_.z - min_.z);
    // v = a * b * c
    return a * b * c;
}

std::ostream &Box::print(std::ostream &os) const {
    return Shape::print(os) << " | Minimum-Punkt: " << min_.x << ", " << min_.y << ", " << min_.z << " | Maximum-Punkt: " << max_.x << ", " << max_.y << ", " << max_.z << "\n";
}

Box::~Box() {
    std::cout<< "Box destructor\n";
}