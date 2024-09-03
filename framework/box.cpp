#include "box.hpp"
#include <glm/gtx/intersect.hpp>

Box::Box(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 min, glm::vec3 max) :
    Shape(name, material),
    min_{min},
    max_{max} {
//    std::cout<< "Box constructor\n";
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
//    std::cout<< "Box destructor\n";
}

HitPoint Box::intersect(const Ray &ray) const {
    float distance = 0.0f;
    HitPoint hitpoint;
    hitpoint.distance = 1000; // startwert
    float x = 0.0f; // intersection x koordinate
    float y = 0.0f; // intersection y koordinate
    float z = 0.0f; // intersection z koordinate
    glm::vec3 direction = glm::normalize(ray.direction);

    // test seite min_.x
    distance = (min_.x - ray.origin.x) / direction.x;
    // wenn distance kürzer ist als aktuelle Distanz, aktualisiere
    if (distance < hitpoint.distance && distance >= 0.0f) {
        // distance in Strahlgleichung einsetzen um y und z Koordinate zu bekommen
        y = ray.origin.y + distance * direction.y;
        z = ray.origin.z + distance * direction.z;

        // wenn y- und z-Koordinaten des Schnittpunkts zwischen ymin und ymax
        // und zmin und zmax liegen --> Schnitt liegt vor --> hitpoint distance aktualisieren
        if ((min_.y <= y && y <= max_.y) && (min_.z <= z && z <= max_.z)) {
//            hitpoint = {true, distance, name_, material_, {min_.x, y, z}, ray.direction};
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {min_.x, y, z};
        }
    }

    // test seite max_.x
    distance = (max_.x - ray.origin.x) / direction.x;
    if (distance < hitpoint.distance && distance >= 0.0f) {
        y = ray.origin.y + distance * direction.y;
        z = ray.origin.z + distance * direction.z;
        if ((min_.y <= y && y <= max_.y) && (min_.z <= z && z <= max_.z)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {max_.x, y, z};
        }
    }

    // test seite min_.y
    distance = (min_.y - ray.origin.y) / direction.y;
    if (distance < hitpoint.distance && distance >= 0.0f) {
        x = ray.origin.x + distance * direction.x;
        z = ray.origin.z + distance * direction.z;
        if ((min_.x <= x && x <= max_.x) && (min_.z <= z && z <= max_.z)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {x, min_.y, z};
        }
    }

    // test seite max_.y
    distance = (max_.y - ray.origin.y) / direction.y;
    if (distance < hitpoint.distance && distance >= 0.0f) {
        x = ray.origin.x + distance * direction.x;
        z = ray.origin.z + distance * direction.z;
        if ((min_.x <= x && x <= max_.x) && (min_.z <= z && z <= max_.z)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {x, max_.y, z};
        }
    }

    // test seite min_.z
    distance = (min_.z - ray.origin.z) / direction.z;
    if (distance < hitpoint.distance && distance >= 0.0f) {
        x = ray.origin.x + distance * direction.x;
        y = ray.origin.y + distance * direction.y;
        if ((min_.x <= x && x <= max_.x) && (min_.y <= y && y <= max_.y)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {x, y, min_.z};
        }
    }

    // test seite max_.z
    distance = (max_.z - ray.origin.z) / direction.z;
    if (distance < hitpoint.distance && distance >= 0.0f) {
        x = ray.origin.x + distance * direction.x;
        y = ray.origin.y + distance * direction.y;
        if ((min_.x <= x && x <= max_.x) && (min_.y <= y && y <= max_.y)) {
            hitpoint.intersection = true;
            hitpoint.distance = distance;
            hitpoint.intersection_point = {x, y, max_.z};
        }
    }

    hitpoint.name = name_;
    hitpoint.material = material_;
    hitpoint.direction = direction;
    hitpoint.normale = glm::normalize(normal(hitpoint.intersection_point));

//    std::cout<< hitpoint.distance << " | " << hitpoint.intersection_point.x << ", " << hitpoint.intersection_point.y << ", " << hitpoint.intersection_point.z << "\n";

    return hitpoint;
}

glm::vec3 Box::normal(glm::vec3 const& point) const  {
    // Die Box hat 6 Flächen: 2x x=const, 2x y=const, 2x z=const
    //return glm::vec3(1.0f, 0.0f, 0.0f);
    // abstand von intersectionpoint zu Box-Flächen
    float epsilon = 1e-6f * 10.0f; // Kleinster Wert, um numerische Fehler zu vermeiden

    if (std::abs(point.x - min_.x) <= epsilon) {
        // Linke Fläche der Box
        return {-1.0f, 0.0f, 0.0f};
    } else if (std::abs(point.x - max_.x) <= epsilon) {
        // Rechte Fläche der Box
        return {1.0f, 0.0f, 0.0f};
    } else if (std::abs(point.y - min_.y) <= epsilon) {
        // Untere Fläche der Box
        return {0.0f, -1.0f, 0.0f};
    } else if (std::abs(point.y - max_.y) <= epsilon) {
        // Obere Fläche der Box
        return {0.0f, 1.0f, 0.0f};
    } else if (std::abs(point.z - min_.z) <= epsilon) {
        // Vorderfläche der Box
        return {0.0f, 0.0f, -1.0f};
    } else if (std::abs(point.z - max_.z) <= epsilon) {
        // Rückfläche der Box
        return {0.0f, 0.0f, 1.0f};
    } else {
        // Ausnahmefall: Punkt außerhalb der Boxs
        

        return {0.0f, 0.0f, 0.0f};
    }
}
