#ifndef RAYTRACER_HITPOINT_HPP
#define RAYTRACER_HITPOINT_HPP
#include "string"
#include "color.hpp"
#include <glm/vec3.hpp>
#include <limits>
#include "material.hpp"

struct HitPoint {
    bool intersection;
    float distance = 0.0f;
    std::string name;
    std::shared_ptr<Material> material;
    glm::vec3 intersection_point = {0.0f, 0.0f, 0.0f}; // glm::vec3(std::numeric_limits<float>::infinity);
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};
};
#endif //RAYTRACER_HITPOINT_HPP
