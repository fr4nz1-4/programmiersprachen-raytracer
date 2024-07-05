#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP
#include "string"
#include "color.hpp"

struct Material {
    std::string name_ = {"nameless"};
    Color ka = {0, 0, 0};
    Color kd = {0, 0, 0};
    Color ks = {0, 0, 0};
    float m = 0.0f; // Spekularreflexionsexponent m
};

std::ostream& operator<<(std::ostream& os, Material const& material);

#endif //RAYTRACER_MATERIAL_HPP
