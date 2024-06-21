#include "sphere.hpp"
#include "cmath"

Sphere::Sphere(glm::vec3 center, float radius) :
    center_{center},
    radius_{radius} {}

unsigned float Sphere::area() const {
    return 4 * M_PI * pow(radius_, 2);
}

float Sphere::volume() const {
    return (4/3) * pow(radius_, 3) * M_PI;
}