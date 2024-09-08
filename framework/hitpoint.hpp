#ifndef RAYTRACER_HITPOINT_HPP
#define RAYTRACER_HITPOINT_HPP
#include "string"
#include "color.hpp"
#include <glm/vec3.hpp>
#include <limits>
#include "material.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

struct HitPoint {
    bool intersection = false;
    float distance = 0.0f;
    std::string name = " ";
    std::shared_ptr<Material> material;
    glm::vec3 intersection_point = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};
    glm::vec3 normale = { 0.0f,0.0f,0.0f };
};

HitPoint transform(glm::mat4 const& world_mat, glm::mat4 const& transposed_inverse_world_mat, HitPoint const& hp);
#endif //RAYTRACER_HITPOINT_HPP
