#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP
#include "glm/vec3.hpp"

struct Camera {
    std::string name = " ";
    float fov = 45.0f; // "field of view" = sichtfeld
    glm::vec3 origin = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, -1.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};  // wichtig! weil stand jetzt ist y-Achse umgekehrt (-y ist oben)
};
#endif //RAYTRACER_CAMERA_HPP