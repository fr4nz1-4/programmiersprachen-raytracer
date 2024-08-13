#include "scene.hpp"

void parse_sdf_file(const std::string& sdf_file_path, Scene& scene) {
    // Datei öffnen
    std::ifstream sdf_file(sdf_file_path);
    // falls nicht geöffnet → fehler
    if (!sdf_file.is_open()) {
        std::cerr << "Could not find or open: " << sdf_file_path << std::endl;
        return;
    }

    // string um datei zeile für zeile zu lesen
    std::string line_buffer;

    // getline reads an input-stream (here: our filestream) line by
    // line (i.e. until a newline character) and stores the read
    // results in a std::string object (our line buffer)
    while (std::getline(sdf_file, line_buffer)) {
        std::istringstream line_as_stream(line_buffer);
        std::string token;
        line_as_stream >> token;

        if ("define" == token) {
            // get the next token, can only be 'material' for now
            line_as_stream >> token;

            if ("material" == token) {
                std::shared_ptr<Material> parsed_material = std::make_shared<Material>();

                // parse the remaining expected parameters one by one and in order
                // into our struct (you need to dereference your pointer, of course)
                line_as_stream >> parsed_material->name_;

                // ambient
                line_as_stream >> parsed_material->ka.r;
                line_as_stream >> parsed_material->ka.g;
                line_as_stream >> parsed_material->ka.b;

                // diffuse
                line_as_stream >> parsed_material->kd.r;
                line_as_stream >> parsed_material->kd.g;
                line_as_stream >> parsed_material->kd.b;

                // specular
                line_as_stream >> parsed_material->ks.r;
                line_as_stream >> parsed_material->ks.g;
                line_as_stream >> parsed_material->ks.b;

                // Spekularreflexionsexponent
                line_as_stream >> parsed_material->m;

                // push material into scene
                scene.material_container.push_back(parsed_material);
            }

            if ("shape" == token) {
                if ("box" == token) {
                    // TODO: hübsch machen für adrian
                    std::string name;
                    Material material;
                    auto material_ptr = std::make_shared<Material>(material);
                    glm::vec3 min;
                    glm::vec3 max;

                    line_as_stream >> name;
                    line_as_stream >> material.name_;
                    line_as_stream >> material.ka.r;
                    line_as_stream >> material.ka.g;
                    line_as_stream >> material.ka.b;
                    line_as_stream >> material.kd.r;
                    line_as_stream >> material.kd.g;
                    line_as_stream >> material.kd.b;
                    line_as_stream >> material.ks.r;
                    line_as_stream >> material.ks.g;
                    line_as_stream >> material.ks.b;
                    line_as_stream >> material.m;
                    line_as_stream >> min.x;
                    line_as_stream >> min.y;
                    line_as_stream >> min.z;
                    line_as_stream >> max.x;
                    line_as_stream >> max.y;
                    line_as_stream >> max.z;

                    std::shared_ptr<Box> parsed_box = std::make_shared<Box>(name, material_ptr, min, max);
                    // push box into scene
                    scene.shape_container.push_back(parsed_box);
                }

                if ("sphere" == token) {
                    // TODO: hübsch machen für adrian
                    std::string name;
                    Material material;
                    auto material_ptr = std::make_shared<Material>(material);
                    glm::vec3 center;
                    float radius;

                    line_as_stream >> name;
                    line_as_stream >> material.name_;
                    line_as_stream >> material.ka.r;
                    line_as_stream >> material.ka.g;
                    line_as_stream >> material.ka.b;
                    line_as_stream >> material.kd.r;
                    line_as_stream >> material.kd.g;
                    line_as_stream >> material.kd.b;
                    line_as_stream >> material.ks.r;
                    line_as_stream >> material.ks.g;
                    line_as_stream >> material.ks.b;
                    line_as_stream >> material.m;
                    line_as_stream >> center.x;
                    line_as_stream >> center.y;
                    line_as_stream >> center.z;
                    line_as_stream >> radius;

                    std::shared_ptr<Sphere> parsed_sphere = std::make_shared<Sphere>(name, material_ptr, center, radius);
                    // push box into scene
                    scene.shape_container.push_back(parsed_sphere);
                }

            } else {
                std::cerr << "Unexpected keyword: " << token << std::endl;
            }
        } else {
            std::cerr << "Unexpected keyword: " << token << std::endl;
        }
    }
    // close our open file at the end
    sdf_file.close();
}