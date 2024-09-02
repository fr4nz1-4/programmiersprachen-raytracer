#include "sphere.hpp"
#include "tuple"
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>

Sphere::Sphere(const glm::vec3 &center, float radius):
    Shape{"", std::make_shared<Material>(Material{})},
    center_{center},
    radius_{radius} {}

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
        h.normale = glm::normalize(normal(h.intersection_point));
    }
    
    return h;
}
/*
HitPoint Sphere::intersect(Ray const& r_original) const{
    Ray r = r_original;

    float distance = 0.0f;
    std::shared_ptr<Material> material = this->get_Material();

    r.direction = glm::normalize(r.direction);

    bool cut = glm::intersectRaySphere(r.origin, r.direction, center_, radius_ * radius_, distance);

    glm::vec3 normale_zum_kons = glm::normalize(glm::vec3{ r.origin + distance * r.direction });

    HitPoint hitpoint{ cut, distance,Shape::get_name(), material, r.origin + distance * r.direction, r.direction,  normale_zum_kons };

    


    glm::vec3 dis = hitpoint.intersection_point - r_original.origin;
    float dist = glm::distance(hitpoint.intersection_point, r_original.origin);
    HitPoint hp{ hitpoint.intersection, dist, hitpoint.name, hitpoint.material, hitpoint.intersection_point, hitpoint.direction, hitpoint.normale };

    return hp;
}*/

Sphere::~Sphere() {
//    std::cout<< "Sphere destructor\n";
}

glm::vec3 Sphere::get_center() const {
return center_;
}

glm::vec3 Sphere::normal(glm::vec3 const& point) const {
    return glm::normalize(point - center_);
}