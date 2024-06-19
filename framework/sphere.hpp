#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape {
public:
private:
    glm::vec3 center_;
    glm::vec3 radius_;
};

#endif //RAYTRACER_SPHERE_HPP
