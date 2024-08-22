#include "scene.hpp"

std::shared_ptr<Material> find_material(Scene const& scene, std::string const& material_name) {
    // material in material_container finden
    std::shared_ptr<Material> material = nullptr;
    // Schleife die über alle Materiale im material_container iteriert
    for (auto const& mat : scene.material_container) {
        // wenn mat mit material_name übereinstimmt, ist das das gesucht material
        if (mat->name_ == material_name) {
            material = mat;
            return material;
        }
    }
}

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

            } else if ("shape" == token) {
                std::string shape_type;
                line_as_stream >> shape_type;

                if ("box" == shape_type) {
                    std::string name;
                    std::string material_name;
                    glm::vec3 min;
                    glm::vec3 max;

                    line_as_stream >> name;
                    line_as_stream >> material_name;
                    line_as_stream >> min.x;
                    line_as_stream >> min.y;
                    line_as_stream >> min.z;
                    line_as_stream >> max.x;
                    line_as_stream >> max.y;
                    line_as_stream >> max.z;

                    // material in material_container finden
                    std::shared_ptr<Material> material = find_material(scene, material_name);

                    if (material != nullptr) {
                        auto parsed_box = std::make_shared<Box>(name, material, min, max);
                        scene.shape_container.push_back(parsed_box);
                    } else {
                        std::cerr << "Material not found: " << material_name << std::endl;
                    }
                } else if ("sphere" == shape_type) {
                    std::string name;
                    std::string material_name;
                    glm::vec3 center;
                    float radius;

                    line_as_stream >> name;
                    line_as_stream >> material_name;
                    line_as_stream >> center.x;
                    line_as_stream >> center.y;
                    line_as_stream >> center.z;
                    line_as_stream >> radius;

                    // material in material_container finden
                    std::shared_ptr<Material> material = find_material(scene, material_name);

                    if (material != nullptr) {
                        auto parsed_sphere = std::make_shared<Sphere>(name, material, center, radius);
                        scene.shape_container.push_back(parsed_sphere);
                    } else {
                        std::cerr << "Material not found: " << material_name << std::endl;
                    }
                }
            } else if ("light" == token) {
                std::shared_ptr<Light> parsed_light = std::make_shared<Light>();

                line_as_stream >> parsed_light->name;
                line_as_stream >> parsed_light->position.x;
                line_as_stream >> parsed_light->position.y;
                line_as_stream >> parsed_light->position.z;
                line_as_stream >> parsed_light->intensity.r;
                line_as_stream >> parsed_light->intensity.g;
                line_as_stream >> parsed_light->intensity.b;

                scene.light_container.push_back(parsed_light);
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