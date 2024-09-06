#include "composite.hpp"
#define PI 3.14159265358979323846f

Composite::Composite(std::string const& name) :
    Shape{name, nullptr} {};

HitPoint Composite::intersect(Ray const& ray) const {
    glm::vec4 origin{ray.origin, 1.0f };
    glm::vec4 direction{ray.direction, 0.0f };

    origin = Shape::get_world_transformation() * origin;
    direction = Shape::get_world_transformation() * direction;

    Ray r = {glm::vec3{origin.x, origin.y, origin.z}, glm::vec3{direction.x, direction.y, direction.z}};

    HitPoint closest_hit{};

    for (auto s : children_) {
        HitPoint hit = s->intersect(r);
        if (hit.intersection) {
            if (hit.distance < closest_hit.distance) {
                closest_hit = hit;
            }
        }
    }

    closest_hit = transform(Shape::get_world_transformation(), Shape::get_world_transformation_inv(), closest_hit);
    return closest_hit;
}

void Composite::add_shape(std::shared_ptr<Shape> const& shape) {
    children_.push_back(shape);
}

glm::vec3 Composite::normal(glm::vec3 const& point) const{
    glm::vec3 vec{0, 0, 0};
    return vec;
}