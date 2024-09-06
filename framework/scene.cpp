#include "scene.hpp"
#include <glm/gtx/string_cast.hpp>
#define PI 3.14159265358979323846f

// material in material_container finden
std::shared_ptr<Material> find_material(Scene const& scene, std::string const& material_name) {
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

// shape in shape_container finden
std::shared_ptr<Shape> find_shape(Scene const& scene, std::string const& shape_name) {
    std::shared_ptr<Shape> shape = nullptr;
    // Schleife die über alle Shapes im shape_container iteriert
    for (auto const& shape1 : scene.shape_container) {
        // wenn get_name mit shape_name übereinstimmt, ist es die gesuchte shape
        if (shape1->get_name() == shape_name) {
            shape = shape1;
            return shape;
        }
    }
}

void parse_sdf_file(const std::string& sdf_file_path, Scene& scene) {
    std::vector<std::shared_ptr<Shape>> all_shapes;
    std::vector<std::shared_ptr<Shape>> composite_shapes;

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
                // Reflexionsstärke
                line_as_stream >> parsed_material->reflectivity;

                // push material into scene
                scene.material_container.push_back(parsed_material);

            }
            else if ("shape" == token) {
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
                    }
                    else {
                        std::cerr << "Material not found: " << material_name << std::endl;
                    }
                }
                else if ("sphere" == shape_type) {
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
                    }
                    else {
                        std::cerr << "Material not found: " << material_name << std::endl;
                    }
                }
//                else if (token == "composite") {
//                    std::string name;
//                    line_as_stream >> name;
//
//                    Composite composite{name};
//
//                    std::string composite_shape_name;
//                    std::shared_ptr<Shape> composite_shape;
//
//                    // schleife wird beendet, wenn Ende der sdf-datei erreicht -> um alle shapes hinzuzufügen
////                    while () {
////                        line_as_stream >> composite_shape_name;
////
////                    }
//                }
            }
            else if ("light" == token) {
                std::shared_ptr<Light> parsed_light = std::make_shared<Light>();

                line_as_stream >> parsed_light->name;
                line_as_stream >> parsed_light->position.x;
                line_as_stream >> parsed_light->position.y;
                line_as_stream >> parsed_light->position.z;
                line_as_stream >> parsed_light->color.r;
                line_as_stream >> parsed_light->color.g;
                line_as_stream >> parsed_light->color.b;
                line_as_stream >> parsed_light->brightness;

                scene.light_container.push_back(parsed_light);
            }
            else if ("camera" == token) {
                std::shared_ptr<Camera> parsed_camera = std::make_shared<Camera>();

                line_as_stream >> parsed_camera->name;
                line_as_stream >> parsed_camera->fov;
                line_as_stream >> parsed_camera->origin.x;
                line_as_stream >> parsed_camera->origin.y;
                line_as_stream >> parsed_camera->origin.z;
                line_as_stream >> parsed_camera->direction.x;
                line_as_stream >> parsed_camera->direction.y;
                line_as_stream >> parsed_camera->direction.z;
                line_as_stream >> parsed_camera->up.x;
                line_as_stream >> parsed_camera->up.y;
                line_as_stream >> parsed_camera->up.z;

                scene.camera_container.push_back(parsed_camera);
            }
            else {
                std::cerr << "Unexpected keyword: " << token << std::endl;
            }
        }
        else if ("transform" == token) {
            std::string shape_name;
            glm::vec3 scale = glm::vec3(1.0f); // Default-Wert (Einheits-Skalierung)
            glm::vec3 translate = glm::vec3(0.0f); // Default-Wert (Keine Translation)
            glm::vec3 rotate = glm::vec3(0.0f); // Default-Wert (Keine Rotation)
            float rot_degree = 0.0f;

            line_as_stream >> shape_name;
            
            std::string inner_token;
            while (line_as_stream >> inner_token) {
                if ("rotate" == inner_token) {
                    line_as_stream >> rot_degree >> rotate.x >> rotate.y >> rotate.z;
                }
                else if ("translate" == inner_token) {
                    line_as_stream >> translate.x >> translate.y >> translate.z;
                }
                else if ("scale" == inner_token) {
                    line_as_stream >> scale.x >> scale.y >> scale.z;
                }
            }

            std::shared_ptr<Shape> shape = find_shape(scene, shape_name);
            if (shape != nullptr) {
                shape->make_world_transform(scale, translate, rot_degree, rotate);
            }
            else {
                std::cerr << "shape not found: " << shape_name << std::endl;
            }
        }

        else {
            std::cerr << "Unexpected keyword: " << token << std::endl;
        }
        //make world trnasform {transvecarray[0] .....}
    }

    // close our open file at the end
    sdf_file.close();
}