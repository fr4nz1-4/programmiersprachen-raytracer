#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP
#include "shape.hpp"
#include "string"
#include "tuple"
#include <glm/vec3.hpp>

class Sphere : public Shape {
public:
    Sphere(glm::vec3 center, float radius);
    Sphere(const std::string &name, const Color &color, glm::vec3 center, float radius);
    float area() const override;
    float volume() const override;
private:
    std::string name_;
    Color color_;
    glm::vec3 center_;
    float radius_;
};

#endif //RAYTRACER_SPHERE_HPP
