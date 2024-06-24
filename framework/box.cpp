#include "box.hpp"

Box::Box(const std::string &name, const Color &color, glm::vec3 min, glm::vec3 max) :
    Shape(name, color),
    min_{min},
    max_{max} {}

//Box::Box(glm::vec3 min, glm::vec3 max) :
//    min_{min},
//    max_{max} {}

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

Box::Box(glm::vec3 min, glm::vec3 max) :
Shape{"grau", {1.0f, 1.0f, 1.0f}},
min_{min},
max_{max} {}
