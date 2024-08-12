#include "sphere.hpp"
#include "tuple"
#include "cmath"
#include <glm/gtx/intersect.hpp>

Sphere::Sphere(const std::string &name, const std::shared_ptr<Material> &material, glm::vec3 const& center, float radius) :
    Shape(name, material),
    center_{center},
    radius_{radius} {
//    std::cout<< "Sphere constructor\n";
}

float Sphere::area() const {
    return 4.0f * std::atan(1.0f) * radius_ * radius_;
}

float Sphere::volume() const {
    return (4.0f/3.0f) * fabs(radius_ * radius_ * radius_) * std::atan(1.0f);
}

std::ostream &Sphere::print(std::ostream &os) const {
    return Shape::print(os) << " | Radius: " << radius_ << " | Center: " << center_.x << ", " << center_.y << ", " << center_.z << "\n";
}

HitPoint Sphere::intersect(Ray const& ray) const {
    float distance = 0.0f;
    HitPoint h;
    auto normalized_direction = glm::normalize(ray.direction);

    h.intersection = glm::intersectRaySphere(ray.origin, normalized_direction, center_, radius_ * radius_, distance);
    if(h.intersection) {
        h.distance = distance;
        h.name = name_;
        h.material = material_;
        h.intersection_point = ray.origin + distance * ray.direction;
        h.direction = normalized_direction;
    }
    return h;
}

Sphere::~Sphere() {
//    std::cout<< "Sphere destructor\n";
}
