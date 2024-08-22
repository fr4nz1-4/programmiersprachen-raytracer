#ifndef RAYTRACER_LIGHT_HPP
#define RAYTRACER_LIGHT_HPP
#include "glm/vec3.hpp"
#include "color.hpp"

struct Light {
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    Color intensity = {1.0f, 1.0f, 1.0f};
};
#endif //RAYTRACER_LIGHT_HPP