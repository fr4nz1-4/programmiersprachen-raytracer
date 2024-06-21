#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape {
public:
    Sphere(glm::vec3 center, float radius);
    unsigned float area() const override;
    float volume() const override;
private:
    glm::vec3 center_;
    float radius_;
};

#endif //RAYTRACER_SPHERE_HPP
