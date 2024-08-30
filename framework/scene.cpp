#include "scene.hpp"
#define PI 3.14159265358979323846f

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

std::shared_ptr<Shape> find_shape(Scene const& scene, std::string const& shape_name) {
    // material in material_container finden
    std::shared_ptr<Shape> shape = nullptr;
    // Schleife die über alle Materiale im material_container iteriert
    for (auto const& shape1 : scene.shape_container) {
        // wenn mat mit material_name übereinstimmt, ist das das gesucht material
        if (shape1->get_name() == shape_name) {
            shape = shape1;
            return shape;
        }
    }
}

void make_world_transform(std::shared_ptr<Shape> shape, glm::vec3 const& scale, glm::vec3 const& translate, float const& rot_degree, glm::vec3 const& rotate) {
    float rad = rot_degree * PI / 180.0f;

    /*
    float rad = 45 * PI / 180.0f;


    glm::mat4 translate_m = { 1, 0, 0, 3,
                             0, 1, 0, 0,
                             0, 0, 1, 2,
                             0, 0, 0, 1 };
    glm::mat4 scale_m = { 2, 0, 0, 0,
                         0, 4, 0, 0,
                         0, 0, 2, 0,
                         0, 0, 0, 1 };
    glm::mat4 rotation_m = { 1, 0, 0, 0,
                            0, std::cos(rad), -std::sin(rad), 0,
                            0, std::sin(rad), std::cos(rad), 0,
                            0, 0, 0, 1 };

    glm::mat4 world_transform = translate_m * rotation_m * scale_m;
    glm::mat4 trans_inv = glm::inverse(world_transform);
    */
    glm::mat4 translate_m = { 1, 0, 0, translate.x,
                             0, 1, 0, translate.y,
                             0, 0, 1, translate.z,
                             0, 0, 0, 1 };
    glm::mat4 scale_m = { scale.x, 0, 0, 0,
                         0, scale.y, 0, 0,
                         0, 0, scale.z, 0,
                         0, 0, 0, 1 };
    glm::mat4 rotation_m = { 1, 0, 0, 0,
                            0, std::cos(rad), -std::sin(rad), 0,
                            0, std::sin(rad), std::cos(rad), 0,
                            0, 0, 0, 1 };
    /*
    glm::mat4 minus_translate_m = { 1, 0, 0, -translate.x,
                             0, 1, 0, -translate.y,
                             0, 0, 1, -translate.z,
                             0, 0, 0, 1 };

    glm::mat4 minus_scale_m = { 1/scale.x, 0, 0, 0,
                         0, 1/scale.y, 0, 0,
                         0, 0, 1/scale.z, 0,
                         0, 0, 0, 1 };
    glm::mat4 minus_rotation_m = { 1, 0, 0, 0,
                            0, std::cos(rad), std::sin(rad), 0,
                            0, -std::sin(rad), std::cos(rad), 0,
                            0, 0, 0, 1 };

    //glm::mat4 trans_inv = minus_scale_m * minus_rotation_m * minus_translate_m;
    */

    glm::mat4 world_transform = translate_m * rotation_m * scale_m;
    shape->set_world_transformation(world_transform);
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
            /*
        } else if ("transform" == token) {
            std::string shape_name;
            glm::vec3 scale;
            glm::vec3 translate;
            glm::vec3 rotate;
            float rot_degree;
            line_as_stream >> shape_name;
            line_as_stream >> token;
            if ("scale" == token) {
                line_as_stream >> scale.x;
                line_as_stream >> scale.y;
                line_as_stream >> scale.z;
            }
            if ("translate" == token) {
                line_as_stream >> translate.x;
                line_as_stream >> translate.y;
                line_as_stream >> translate.z;
            }
            if ("rotate" == token) {
                line_as_stream >> rot_degree;
                line_as_stream >> rotate.x;
                line_as_stream >> rotate.y;
                line_as_stream >> rotate.z;
            }*/
        }else if ("transform" == token) {
                std::string shape_name;
                glm::vec3 scale = glm::vec3(1.0f); // Default-Wert (Einheits-Skalierung)
                glm::vec3 translate = glm::vec3(0.0f); // Default-Wert (Keine Translation)
                glm::vec3 rotate = glm::vec3(0.0f); // Default-Wert (Keine Rotation)
                float rot_degree = 0.0f;

                line_as_stream >> shape_name;
                line_as_stream >> token;

                if ("scale" == token) {
                    line_as_stream >> scale.x >> scale.y >> scale.z;
                }
                else if ("translate" == token) {
                    line_as_stream >> translate.x >> translate.y >> translate.z;
                }
                else if ("rotate" == token) {
                    line_as_stream >> rot_degree >> rotate.x >> rotate.y >> rotate.z;
                }


            std::shared_ptr<Shape> shape = find_shape(scene, shape_name);
            if (shape != nullptr) {
                make_world_transform(shape, scale, translate, rot_degree, rotate);
            } else {
                std::cerr << "shape not found: " << shape_name << std::endl;
            }
        }

        else {
            std::cerr << "Unexpected keyword: " << token << std::endl;
        }
    }
    // close our open file at the end
    sdf_file.close();
}