#include "box.hpp"
#include <glm/gtx/intersect.hpp>

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

HitPoint Box::intersect(const Ray &ray) const {
    float distance = 0.0f;
    HitPoint hitpoint;
    float intersection_x = 0.0f;
    float intersection_y = 0.0f;
    float intersection_z = 0.0f;
    glm::normalize(ray.direction);

    // test seite min_.x
    distance = (min_.x - ray.origin.x) / ray.direction.x;
    // wenn distance kürzer ist als aktuelle Distanz, aktualisiere
    if (distance < hitpoint.distance) {
        // distance in Strahlgleichung einsetzen um y und z Koordinate zu bekommen
        intersection_y = ray.origin.y + distance * ray.direction.y;
        intersection_z = ray.origin.z + distance * ray.direction.z;

        // wenn y- und z-Koordinaten des Schnittpunkts zwischen ymin und ymax
        // und zmin und zmax liegen --> Schnitt liegt vor --> hitpoint distance aktualisieren
        if ((min_.y <= hitpoint.intersection_point.y <= max_.y) && (min_.z <= hitpoint.intersection_point.z <= max_.z)) {
//            hitpoint = {true, distance, name_, material_, {min_.x, intersection_y, intersection_z}, ray.direction};
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {min_.x, intersection_y, intersection_z};
        }
    }

    // test seite max_.x
    distance = (max_.x - ray.origin.x) / ray.direction.x;
    if (distance < hitpoint.distance) {
        intersection_y = ray.origin.y + distance * ray.direction.y;
        intersection_z = ray.origin.z + distance * ray.direction.z;
        if ((min_.y <= hitpoint.intersection_point.y <= max_.y) && (min_.z <= hitpoint.intersection_point.z <= max_.z)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {max_.x, intersection_y, intersection_z};
        }
    }

    // test seite min_.y
    distance = (min_.y - ray.origin.y) / ray.direction.y;
    if (distance < hitpoint.distance) {
        intersection_x = ray.origin.x + distance * ray.direction.x;
        intersection_z = ray.origin.z + distance * ray.direction.z;
        if ((min_.x <= hitpoint.intersection_point.x <= max_.x) && (min_.z <= hitpoint.intersection_point.z <= max_.z)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {min_.y, intersection_x, intersection_z};
        }
    }

    // test seite max_y
    distance = (max_.y - ray.origin.y) / ray.direction.y;
    if (distance < hitpoint.distance) {
        intersection_x = ray.origin.x + distance * ray.direction.x;
        intersection_z = ray.origin.z + distance * ray.direction.z;
        if ((min_.x <= hitpoint.intersection_point.x <= max_.x) && (min_.z <= hitpoint.intersection_point.z <= max_.z)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {max_.y, intersection_x, intersection_z};
        }
    }

    // test seite min_.z
    distance = (min_.z - ray.origin.z) / ray.direction.z;
    if (distance < hitpoint.distance) {
        intersection_x = ray.origin.x + distance * ray.direction.x;
        intersection_y = ray.origin.y + distance * ray.direction.y;
        if ((min_.x <= hitpoint.intersection_point.x <= max_.x) && (min_.y <= hitpoint.intersection_point.y <= max_.y)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {min_.z, intersection_x, intersection_y};
        }
    }

    // test seite max_.x
    distance = (max_.z - ray.origin.z) / ray.direction.z;
    if (distance < hitpoint.distance) {
        intersection_x = ray.origin.x + distance * ray.direction.x;
        intersection_y = ray.origin.y + distance * ray.direction.y;
        if ((min_.x <= hitpoint.intersection_point.x <= max_.x) && (min_.y <= hitpoint.intersection_point.y <= max_.y)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {max_.z, intersection_x, intersection_y};
        }
    }

    hitpoint.name = name_;
    hitpoint.material = material_;
    hitpoint.material = material_;
    hitpoint.direction = ray.direction;

    return hitpoint;
}