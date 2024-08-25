#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

//#include <glm/glm.hpp>

#include <iostream>
//file stream for opening files
#include <fstream>
#include <string>
// stringstream for getting parts of the read string (tokens) and
// streaming them into our Material Variables
#include <sstream>
#include <vector>
#include "material.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include "camera.hpp"

struct Scene {
    std::vector<std::shared_ptr<Material> > material_container;
    std::vector<std::shared_ptr<Shape> > shape_container;
    std::vector<std::shared_ptr<Light> > light_container;
    std::vector<std::shared_ptr<Camera> > camera_container;
};


void parse_sdf_file(const std::string& sdf_file_path, Scene& scene);

#endif //RAYTRACER_SCENE_HPP
