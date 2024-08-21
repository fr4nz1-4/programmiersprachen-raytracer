#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP
#include "glm/vec3.hpp"

struct Camera {
    glm::vec3 origin = {600/2, 600/2, -200.0f};
    glm::vec3 direction = {};
};
#endif //RAYTRACER_CAMERA_HPP
