#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP
#include "string"
#include "color.hpp"
#include <iostream>

struct Material {
    std::string name_ = " ";
    Color ka = {0, 0, 0};
    Color kd = {0, 0, 0};
    Color ks = {0, 0, 0};
    float m = 0.0f; // Spekularreflexionsexponent m
    float reflectivity = 0.0f; // Reflexionsstärke zwischen 0.0 und 1.0

    friend std::ostream& operator<<(std::ostream& os, Material const& material) {
        return os << "Material name: " << material.name_ << " | Material kd: " << material.kd <<
        " | Material ka: " << material.ka << " | Material ks: " << material.ks <<
        " | Spekularreflexionsexponent: " << material.m << " | Reflexionsstärke: " << material.reflectivity;
    }
};

#endif //RAYTRACER_MATERIAL_HPP
