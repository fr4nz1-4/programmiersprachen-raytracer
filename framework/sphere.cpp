#include "sphere.hpp"
#include "tuple"
#include "cmath"

Sphere::Sphere(const std::string &name, const Color &color, glm::vec3 center, float radius) :
        Shape(name, color),
        center_{center},
        radius_{radius} {}

float Sphere::area() const {
    return 4 * M_PI * pow(radius_, 2);
}

float Sphere::volume() const {
    return (4.0f/3.0f) * abs(pow(radius_, 3)) * M_PI;
}