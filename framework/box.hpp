#ifndef RAYTRACER_BOX_HPP
#define RAYTRACER_BOX_HPP
#include "shape.hpp"
#include <glm/vec3.hpp>

class Box : public Shape {
public:
    Box(glm::vec3 min, glm::vec3 max);
    unsigned float area() const override;
    float volume() const override;
private:
    glm::vec3 min_;
    glm::vec3 max_;
};

#endif //RAYTRACER_BOX_HPP
